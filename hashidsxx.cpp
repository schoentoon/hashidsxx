/**
 *  Hashidsxx (c) 2014 Toon Schoenmakers
 *
 *  https://github.com/schoentoon/hashidsxx
 *  hashidsxx may be freely distributed under the MIT license.
 */

#include <getopt.h>
#include <string.h>

#include "hashids.h"

#include <iostream>
#include <list>

static const struct option g_LongOpts[] = {
  { "help", no_argument, 0, 'h' },
  { "salt", required_argument, 0, 's' },
  { "alphabet", required_argument, 0, 'a' },
  { "min", required_argument, 0, 'm' },
  { "input", required_argument, 0, 'i' },
  { 0, 0, 0, 0 }
};

static int usage(const char *prog) {
  std::cerr << "USAGE: [encode|decode]" << prog << " [options]" << std::endl
            << "-h, --help      Show this help message" << std::endl
            << "-s, --salt      The salt to use for the encryption" << std::endl
            << "-a, --alphabet  A custom alphabet to use" << std::endl
            << "-m, --min       The minimum amount of characters to use, only "
               "useful when encoding" << std::endl
            << "-i, --input     Input numbers, in case of encode repeat this "
               "for multiple input numbers" << std::endl;
  return 1;
};

int main(int argc, char **argv) {
  if (argc == 1)
    return usage(argv[0]);

  std::string salt;
  std::string alphabet(DEFAULT_ALPHABET);
  unsigned int min_length = 0;

  bool encode = (strcmp(argv[1], "encode") == 0);
  if (encode == false && strcmp(argv[1], "decode") != 0)
    return usage(argv[0]); // we must make sure we were at least decode or
                           // encode you know

  std::list<uint32_t> encode_input;
  std::string decode_input;

  int arg, optindex;
  while ((arg = getopt_long(argc, argv, "hs:a:m:i:", g_LongOpts, &optindex)) !=
         -1) {
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
      if (encode)
        encode_input.push_back(std::stoi(std::string(optarg)));
      else
        decode_input = std::string(optarg);
      break;
    }
  }

  try {
    hashidsxx::Hashids hash(salt, min_length, alphabet);

    if (encode)
      std::cout << hash.encode(encode_input.begin(), encode_input.end())
                << std::endl;
    else {
      for (uint32_t number : hash.decode(decode_input))
        std::cout << number << std::endl;
    }
  }
  catch (const std::runtime_error &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  };

  return 0;
};
