#pragma once

#include <string>
#include <vector>

#define DEFAULT_ALPHABET "xcS4F6h89aUbidefI7jkyunopqrsgCYE5GHTKLMtARXz"

namespace hashidsxx {

class Hashids {
private:
  /**
   *  The salt that will be used to generate the hashes
   */
  const std::string _salt;

  /**
   *  The alphabet that will be used to generate the hashes
   */
  std::string _alphabet;

  /**
   *  Minimum length of the hash, 0 means that there's not minimum
   */
  const unsigned int _min_length;

  std::string _separators;
  std::string _guards;

public:
  Hashids(const std::string &salt = "", const std::string alphabet = DEFAULT_ALPHABET, unsigned int min_length = -1);

  std::string encrypt(const std::vector<uint32_t> &input) const;

  std::vector<uint32_t> decrypt(const std::string &input) const;

private:
  std::string& _reorder(std::string &input, const std::string &salt) const;
};

};