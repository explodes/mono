#include "gtest/gtest.h"

TEST(HelloTest, GetGreet) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }
TEST(HelloTest, GetGreet1) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }
TEST(HelloTest, GetGreet2) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }
TEST(HelloTest, GetGreet3) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }
TEST(HelloTest, GetGreet4) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }
TEST(HelloTest, GetGreet5) { EXPECT_EQ("Hello Bazel", "Hello Bazel"); }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}