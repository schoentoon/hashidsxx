#pragma once

#include <string>
#include <vector>

#define DEFAULT_ALPHABET "xcS4F6h89aUbidefI7jkyunopqrsgCYE5GHTKLMtARXz"

namespace hashidsxx {

class Hashids {
private:
  const std::string _salt;
  const std::string _alphabet;

  /**
   *  Minimum length of the hash, -1 means that there's not minimum
   */
  const int _min_length = -1;
public:
  Hashids(const std::string &salt, const std::string alphabet = DEFAULT_ALPHABET, int min_length = -1);

  std::string encrypt(const std::vector<uint32_t> &input) const;

  std::vector<uint32_t> decrypt(const std::string &input) const;
};

};