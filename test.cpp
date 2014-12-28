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
  std::string output = hash.encode({1,2,3,4});
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

int main(int argc, char** argv) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ::testing::FLAGS_gtest_shuffle = true;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
