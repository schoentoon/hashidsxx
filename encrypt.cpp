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
  { "min",        required_argument, 0, 'm' },
  { "input",      required_argument, 0, 'i' },
  { 0, 0, 0, 0 }
};

static int usage(const char *prog) {
  std::cerr << "USAGE: " << prog << " [options]" << std::endl
            << "-h, --help      Show this help message" << std::endl
            << "-s, --salt      The salt to use for the encryption" << std::endl
            << "-a, --alphabet  A custom alphabet to use" << std::endl
            << "-m, --min       The minimum amount of characters to use" << std::endl
            << "-i, --input     Input numbers, repeat this for multiple input numbers" << std::endl;
  return 1;
};

int main(int argc, char **argv) {
  std::string salt;
  std::string alphabet(DEFAULT_ALPHABET);
  unsigned int min_length = 0;

  std::vector<uint32_t> input;

  int arg, optindex;
  while ((arg = getopt_long(argc, argv, "hs:a:m:i:", g_LongOpts, &optindex)) != -1) {
    switch (arg) {
    case 'h':
      return usage(argv[0]);
    case 's':
      salt = std::string(optarg);
      break;
    case 'a':
      alphabet = std::string(optarg);
      break;
    case 'm':
      min_length = std::stoi(std::string(optarg));
      break;
    case 'i':
      input.push_back(std::stoi(std::string(optarg)));
      break;
    }
  }

  try {
    hashidsxx::Hashids hash(salt, min_length, alphabet);

    std::cout << hash.encode(input) << std::endl;
  } catch (const std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  };

  return 0;
};