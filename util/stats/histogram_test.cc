#include "util/stats/histogram.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace util {
namespace stats {
namespace {

TEST(HistogramTest, Basic) {
  Histogram<int> h;
  h.Put(42);

  vector<Bucket<int>> graph;
  h.MakeHistogram(graph);

  EXPECT_EQ(graph.size(), 1);
  EXPECT_EQ(graph[0].min(), 42);
  EXPECT_EQ(graph[0].max(), 42);
  EXPECT_EQ(graph[0].count(), 1);
}

TEST(HistogramTest, LimitedHistogram) {
  Histogram<int> h(50);
  for (int i = 0; i < 100; ++i) {
    h.Put(i);
  }

  vector<Bucket<int>> expected;
  expected.push_back(Bucket<int>(50, 66, 17));
  expected.push_back(Bucket<int>(66, 82, 16));
  expected.push_back(Bucket<int>(82, 98, 16));
  expected.push_back(Bucket<int>(98, 114, 1));

  vector<Bucket<int>> graph;
  h.MakeHistogram(graph);

  EXPECT_THAT(graph, ::testing::ContainerEq(expected));
}

TEST(HistogramTest, FullHistogram) {
  Histogram<int> h;
  for (int i = 0; i < 100; ++i) {
    h.Put(i);
  }

  vector<Bucket<int>> expected;
  expected.push_back(Bucket<int>(0, 33, 34));
  expected.push_back(Bucket<int>(33, 66, 33));
  expected.push_back(Bucket<int>(66, 99, 33));

  vector<Bucket<int>> graph;
  h.MakeHistogram(graph);

  EXPECT_THAT(graph, ::testing::ContainerEq(expected));
}

}  // namespace
}  // namespace stats
}  // namespace util

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}