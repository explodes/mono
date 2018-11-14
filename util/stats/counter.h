#ifndef UTIL_STATS_COUNTER_H
#define UTIL_STATS_COUNTER_H

#include <stdint.h>
#include <mutex>

namespace util {
namespace stats {

using std::lock_guard;
using std::mutex;

class Counter {
 public:
  Counter() : count_(0) {}

  Counter(const Counter&) = delete;
  Counter& operator=(Counter const&) = delete;

  void Put(uint64_t count) {
    lock_guard lock(mutex_);
    count_ += count;
  }

  void Put() { Put(1); }

  uint64_t Count() const { return count_; }

 private:
  mutex mutex_;
  uint64_t count_;
};

}  // namespace stats
}  // namespace util

#endif  // UTIL_STATS_COUNTER_H