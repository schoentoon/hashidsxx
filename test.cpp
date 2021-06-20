#include <gtest/gtest.h>

#include "hashids.h"

TEST(Encode, Saltless) {
  hashidsxx::Hashids hash;
  ASSERT_EQ(hash.encode({123}), "Mj3");
}

TEST(Decode, Saltless) {
  hashidsxx::Hashids hash;
  std::vector<uint64_t> output = hash.decode("Mj3");
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0], 123);
}

TEST(Encode, Salt) {
  hashidsxx::Hashids hash("salt");
  ASSERT_EQ(hash.encode({123}), "0lb");
}

TEST(Decode, Salt) {
  hashidsxx::Hashids hash("salt");
  std::vector<uint64_t> output = hash.decode("0lb");
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0], 123);
}

TEST(Encode, SaltMinLength) {
  hashidsxx::Hashids hash("salt", 15);
  std::string output = hash.encode(1,2,3,4);
  ASSERT_GE(output.size(), 15);
  ASSERT_EQ(output, "7P231QiYuXU53Y9");
}

TEST(Decode, SaltMinLength) {
  hashidsxx::Hashids hash("salt", 15);
  std::vector<uint64_t> output = hash.decode("7P231QiYuXU53Y9");
  ASSERT_EQ(output.size(), 4);
  ASSERT_EQ(output[0], 1);
  ASSERT_EQ(output[1], 2);
  ASSERT_EQ(output[2], 3);
  ASSERT_EQ(output[3], 4);

  // we should be able to decode shorter hashes with the same salt regardless of the length..
  output = hash.decode("0lb");
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0], 123);
}

TEST(Encode, SaltMinLengthAlphabet) {
  const std::string alphabet("abcdefghijklmnopqrstuvwxyz");
  hashidsxx::Hashids hash("salt", 16, alphabet);
  std::string output = hash.encode({123456789});
  ASSERT_GE(output.size(), 16);
  ASSERT_EQ(output, "oavlpogkzrxrkpxd");
  // we should NOT find any characters other than our alphabet in the output
  ASSERT_EQ(output.find_first_not_of(alphabet), std::string::npos);
}

TEST(Decode, SaltMinLengthAlphabet) {
  const std::string alphabet("abcdefghijklmnopqrstuvwxyz");
  hashidsxx::Hashids hash("salt", 16, alphabet);
  std::vector<uint64_t> output = hash.decode("oavlpogkzrxrkpxd");
  ASSERT_EQ(output.size(), 1);
  ASSERT_EQ(output[0], 123456789);
}

TEST(Encode, Hex) {
  hashidsxx::Hashids hash("this is my salt");
  std::string output = hash.encodeHex("abc");
  ASSERT_EQ(output, "5RzW");
  output = hash.encodeHex("f000000000000000f");
  ASSERT_EQ(output, "8OlB1X6RjYuzPWW");
}

TEST(Decode, Hex) {
  hashidsxx::Hashids hash("this is my salt");
  std::string output = hash.decodeHex("5RzW");
  ASSERT_EQ(output, "abc");
  output = hash.decodeHex("8OlB1X6RjYuzPWW");
  ASSERT_EQ(output, "f000000000000000f");
}

TEST(EncodeDecode, IntLoopUnroll) {
  hashidsxx::Hashids hash("this is my salt");
  for (uint64_t i = 0; i < 1000000; i += 5) {
    ASSERT_EQ(hash.decode(hash.encode(i + 0))[0], i + 0);
    ASSERT_EQ(hash.decode(hash.encode(i + 1))[0], i + 1);
    ASSERT_EQ(hash.decode(hash.encode(i + 2))[0], i + 2);
    ASSERT_EQ(hash.decode(hash.encode(i + 3))[0], i + 3);
    ASSERT_EQ(hash.decode(hash.encode(i + 4))[0], i + 4);
  }
}

TEST(EncodeDecode, HexLoopUnroll) {
  hashidsxx::Hashids hash("this is my salt");
  for (uint64_t i = 0; i < 1000000; i += 5) {
    {
      std::stringstream ss; ss << std::hex << i + 0;
      EXPECT_TRUE(hash.decodeHex(hash.encodeHex(ss.str())) == ss.str());
    }
    {
      std::stringstream ss; ss << std::hex << i + 1;
      EXPECT_TRUE(hash.decodeHex(hash.encodeHex(ss.str())) == ss.str());
    }
    {
      std::stringstream ss; ss << std::hex << i + 2;
      EXPECT_TRUE(hash.decodeHex(hash.encodeHex(ss.str())) == ss.str());
    }
    {
      std::stringstream ss; ss << std::hex << i + 3;
      EXPECT_TRUE(hash.decodeHex(hash.encodeHex(ss.str())) == ss.str());
    }
    {
      std::stringstream ss; ss << std::hex << i + 4;
      EXPECT_TRUE(hash.decodeHex(hash.encodeHex(ss.str())) == ss.str());
    }
  }
}

// check hex invalid

int main(int argc, char** argv) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ::testing::FLAGS_gtest_shuffle = true;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
