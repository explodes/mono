#ifndef UTIL_STAT_STATS_H
#define UTIL_STAT_STATS_H

#include <stdint.h>
#include <vector>

namespace util {
namespace stats {

using ::std::vector;

template <typename T>
class Histogram {
 public:
  Histogram() : values_(), max_values_(DEFAULT_MAX_VALUES) {}

  Histogram(const Histogram&) = delete;
  Histogram& operator=(Histogram const&) = delete;

  void put(T value);
  vector<T> histogram();

 private:
  static constexpr DEFAULT_MAX_VALUES = 256;

  vector<T> values_;
  uint64_t max_values_;
};

template <typename T>
class Counter {
 public:
  Counter() : count_(0) {}

  Counter(const Counter&) = delete;
  Counter& operator=(Counter const&) = delete;

  void put();
  uint64_t count() const;

 private:
  uint64_t count_;
};

}  // namespace stats
}  // namespace util

#endif  // UTIL_STAT_STATS_H