#include "net/ezdb/key.h"

#include "gtest/gtest.h"

namespace ezdb {
namespace {

class KeyTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  // void TearDown() override {}
};

TEST_F(KeyTest, SampleTest) {
  EXPECT_EQ(0, 0);
  ASSERT_EQ(0, 0);
}

TEST_F(KeyTest, CharVectorKeyTest) {
  vector<char> v{'1', '2', '3'};
  Key<vector<char>> key = Key<vector<char>>(v);

  Key<vector<char>> binary_key = key.ToBinaryKey();

  ASSERT_EQ(key, binary_key);
}

}  // namespace
}  // namespace ezdb

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}