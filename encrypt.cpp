#include "hashids.h"

#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    return 1;
  }

  std::string salt(argv[1]);

  hashidsxx::Hashids hash(salt, 16);

  std::cout << hash.encrypt({1337}) << std::endl;
};