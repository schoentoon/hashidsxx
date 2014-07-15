/**
 *  Hashidsxx (c) 2014 Toon Schoenmakers
 *
 *  https://github.com/schoentoon/hashidsxx
 *  hashidsxx may be freely distributed under the MIT license.
 */

#include "hashids.h"

#include <algorithm>

#define RATIO_SEPARATORS 3.5
#define RATIO_GUARDS 12

#include <iostream>

namespace hashidsxx {

const static std::string separators("cfhistuCFHISTU");

Hashids::Hashids(const std::string &salt, unsigned int min_length, const std::string alphabet)
: _salt(salt)
, _alphabet(alphabet)
, _min_length(min_length)
{
  std::for_each(separators.begin(), separators.end(), [this](char c) {
    if (_alphabet.find(c) != std::string::npos) _separators.push_back(c);
  });
  _alphabet.erase(std::remove_if(_alphabet.begin(), _alphabet.end(), [this](char c) { return _separators.find(c) != std::string::npos; }), _alphabet.end());
  if (_alphabet.size() + _separators.size() < 16) throw std::runtime_error("Alphabet must contain at least 16 unique characters");

  _separators = _reorder(_separators, _salt);

  int min_separators = (int) std::ceil((float) _alphabet.length() / RATIO_SEPARATORS);

  if (_separators.empty() || _separators.length() < min_separators) {
    if (min_separators == 1) min_separators = 2;
    if (min_separators > _separators.length()) {
      int split_at = min_separators - _separators.length();
      _separators.append(_alphabet.substr(0, split_at));
      _alphabet = _alphabet.substr(split_at);
    };
  };

  _alphabet = _reorder(_alphabet, _salt);
  int num_guards = (int) std::ceil((float) _alphabet.length() / RATIO_GUARDS);

  if (_alphabet.length() < 3) {
    _guards = _separators.substr(0, num_guards);
    _separators = _separators.substr(num_guards);
  } else {
    _guards = _alphabet.substr(0, num_guards);
    _alphabet = _alphabet.substr(num_guards);
  };
}

std::string& Hashids::_reorder(std::string &input, const std::string &salt) const
{
  if (salt.empty()) return input;

  int i = input.length() - 1;
  int index = 0;
  int integer_sum = 0;

  while (i > 0) {
    index %= salt.length();
    int integer = salt[index];
    integer_sum += integer;
    unsigned int j = (integer + index + integer_sum) % i;

    char tmp = input[j];
    std::string trailer((j + 1 < input.length()) ? input.substr(j+1) : "");
    input = input.substr(0, j) + input[i] + trailer;
    input = input.substr(0, i) + tmp + input.substr(i+1);

    --i;
    ++index;
  };

  return input;
}

std::string Hashids::_reorder_norewrite(const std::string &input, const std::string &salt) const
{
  std::string output(input);
  return _reorder(output, salt);
}

std::string Hashids::_hash(uint32_t number, const std::string &alphabet) const
{
  std::string output;
  while (true) {
    output.insert(output.begin(), alphabet[number % alphabet.size()]);
    number /= alphabet.size();
    if (number == 0) return output;
  };
}

uint32_t Hashids::_unhash(const std::string& input, const std::string &alphabet) const
{
  uint32_t output = 0;
  for (int i = 0; i < input.size(); ++i) {
    char c = input[i];
    std::string::size_type pos = alphabet.find(c);
    output += pos * std::pow(alphabet.size(), input.size() - i - 1);
  };

  return output;
}

void Hashids::_ensure_length(std::string &output, std::string &alphabet, int values_hash) const
{
  int guard_index = (values_hash + output[0]) % _guards.size();
  output.insert(output.begin(), _guards[guard_index]);

  if (output.size() < _min_length) {
    guard_index = (values_hash + output[2]) % _guards.size();
    output.push_back(_guards[guard_index]);
  };

  int split_at = alphabet.size() / 2;
  while (output.size() < _min_length) {
    alphabet = _reorder_norewrite(alphabet, alphabet);

    output = alphabet.substr(split_at) + output + alphabet.substr(0, split_at);

    int excess = output.size() - _min_length;
    if (excess > 0) {
      int from_index = excess / 2;
      output = output.substr(from_index, _min_length);
    };
  };
}

std::vector<std::string> Hashids::_split(const std::string &input, const std::string &splitters) const
{
  std::vector<std::string> parts;
  std::string tmp;

  for (char c : input) {
    if (splitters.find(c) != std::string::npos) {
      parts.push_back(tmp);
      tmp.clear();
    } else tmp.push_back(c);
  };
  if (!tmp.empty()) parts.push_back(tmp);

  return parts;
}

std::string Hashids::encrypt(const std::vector<uint32_t>& input) const
{
  // Encrypting nothing makes no sense
  if (input.empty()) return "";

  // Make a copy of our alphabet so we can reorder it on the fly etc
  std::string alphabet(_alphabet);

  int values_hash = 0;
  for (int i = 0; i < input.size(); ++i) values_hash += (input[i] % (i + 100));

  char encoded = _alphabet[values_hash % _alphabet.size()];
  char lottery = encoded;

  std::string output;
  output.push_back(encoded);

  for (int i = 0; i < input.size(); ++i) {
    uint32_t number = input[i];

    std::string alphabet_salt;
    alphabet_salt.push_back(lottery);
    alphabet_salt.append(_salt).append(alphabet);

    alphabet = _reorder(alphabet, alphabet_salt);

    std::string last = _hash(number, alphabet);
    output.append(last);

    number %= last[0] + i;
    output.push_back(_separators[number % _separators.size()]);
  };

  output.pop_back();

  if (output.size() < _min_length) _ensure_length(output, alphabet, values_hash);

  return output;
}

std::vector<uint32_t> Hashids::decrypt(const std::string& input) const
{
  std::vector<uint32_t> output;

  std::vector<std::string> parts = _split(input, _guards);

  std::string hashid = parts[0];
  if (parts.size() >= 2) hashid = parts[1];

  if (hashid.empty()) return output;

  char lottery = hashid[0];
  std::string alphabet(_alphabet);

  hashid.erase(hashid.begin());

  std::vector<std::string> hash_parts = _split(hashid, _separators);
  for (const std::string &part : hash_parts) {
    std::string alphabet_salt = (lottery + _salt + alphabet);
    alphabet_salt = alphabet_salt.substr(0, alphabet.size());

    alphabet = _reorder(alphabet, alphabet_salt);

    output.push_back(_unhash(part, alphabet));
  };

  return output;
}

};