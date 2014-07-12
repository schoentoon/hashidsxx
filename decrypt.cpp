#include "hashids.h"

#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    return 1;
  }

  std::string salt(argv[1]);

  hashidsxx::Hashids hash(salt, 16);

  std::vector<uint32_t> output = hash.decrypt("ZQ2byW3j1OmYLJMD");
  for (uint32_t number : output) std::cout << number << std::endl;
};