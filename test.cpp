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

int main(int argc, char** argv) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ::testing::FLAGS_gtest_shuffle = true;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
