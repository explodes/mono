#include "util/stats/counter.h"

#include "gtest/gtest.h"

namespace util {
namespace stats {
namespace {

TEST(CounterTest, Counter) {
  Counter c;

  c.Put();
  ASSERT_EQ(1, c.Count());

  c.Put();
  ASSERT_EQ(2, c.Count());

  c.Put(10);
  ASSERT_EQ(12, c.Count());

  c.Put(15);
  ASSERT_EQ(27, c.Count());
}

}  // namespace
}  // namespace stats
}  // namespace util

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}