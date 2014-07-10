#include "hashids.h"

namespace hashidsxx {

Hashids::Hashids(const std::string &salt, const std::string alphabet, int min_length)
: _salt(salt)
, _alphabet(alphabet)
, _min_length(min_length)
{
}

std::string Hashids::encrypt(const std::vector<uint32_t>& input) const
{
  // Encrypting nothing makes no sense
  if (input.empty()) return "";

  return ""; // @todo Implement
}

std::vector<uint32_t> Hashids::decrypt(const std::string& input) const
{
  std::vector<uint32_t> output;
  return output;
}

};