#include "net/ezdb/btree.h"

#include "gtest/gtest.h"

namespace ezdb {
namespace {

class BTreeTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  // void TearDown() override {}
};

TEST_F(BTreeTest, SampleTest) {
  EXPECT_EQ(0, 0);
  ASSERT_EQ(0, 0);
}

}  // namespace
}  // namespace ezdb

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}