/**
 *  Hashidsxx (c) 2014 Toon Schoenmakers
 *
 *  https://github.com/schoentoon/hashidsxx
 *  hashidsxx may be freely distributed under the MIT license.
 */

#include <getopt.h>

#include "hashids.h"

#include <iostream>

static const struct option g_LongOpts[] = {
  { "help",       no_argument,       0, 'h' },
  { "salt",       required_argument, 0, 's' },
  { "alphabet",   required_argument, 0, 'a' },
  { "input",      required_argument, 0, 'i' },
  { 0, 0, 0, 0 }
};

static int usage(const char *prog) {
  std::cerr << "USAGE: " << prog << " [options]" << std::endl
            << "-h, --help      Show this help message" << std::endl
            << "-s, --salt      The salt to use for the encryption" << std::endl
            << "-a, --alphabet  A custom alphabet to use" << std::endl
            << "-i, --input     Input string to decrypt" << std::endl;
  return 1;
};

int main(int argc, char **argv) {
  std::string salt;
  std::string alphabet(DEFAULT_ALPHABET);

  std::string input;

  int arg, optindex;
  while ((arg = getopt_long(argc, argv, "hs:a:i:", g_LongOpts, &optindex)) != -1) {
    switch (arg) {
    case 'h':
      return usage(argv[0]);
    case 's':
      salt = std::string(optarg);
      break;
    case 'a':
      alphabet = std::string(optarg);
      break;
    case 'i':
      input = std::string(optarg);
      break;
    }
  }

  try {
    hashidsxx::Hashids hash(salt, 0, alphabet);

    for (uint32_t number : hash.decrypt(input)) std::cout << number << std::endl;
  } catch (const std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  };

  return 0;
  //for (uint32_t number : output) std::cout << number << std::endl;
};