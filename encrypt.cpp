/**
 *  Hashidsxx (c) 2014 Toon Schoenmakers
 *
 *  https://github.com/schoentoon/hashidsxx
 *  hashidsxx may be freely distributed under the MIT license.
 */

#include "hashids.h"

#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    return 1;
  }

  std::string salt(argv[1]);

  hashidsxx::Hashids hash(salt,"abcdefghijklmnopqrstuvwxyz");

  std::cout << hash.encrypt({123456789}) << std::endl;
};