#include "db/bolt/node.h"

#include "gtest/gtest.h"

namespace bolt {
namespace {

class Test : public ::testing::Test {
 protected:
  void SetUp() override {}

  // void TearDown() override {}
};

TEST_F(Test, Sample) { EXPECT_EQ(0, 0); }

}  // namespace
}  // namespace bolt

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}