#ifndef UTIL_FUTURE_FUNCTION_H
#define UTIL_FUTURE_FUNCTION_H

#include <condition_variable>
#include <deque>
#include <mutex>

namespace util {
namespace future {

using std::condition_variable;
using std::deque;
using std::mutex;
using std::unique_lock;

template <typename T>
class Queue {
 public:
  void push(T const& value) {
    {
      unique_lock<mutex> lock(mutex_);
      queue_.push_front(value);
    }
    condition_.notify_one();
  }

  T pop() {
    unique_lock<mutex> lock(mutex_);
    condition_.wait(lock, [=] { return !queue_.empty(); });
    T rc(std::move(queue_.back()));
    queue_.pop_back();
    return rc;
  }

 private:
  mutable mutex mutex_;
  condition_variable condition_;
  deque<T> queue_;
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUNCTION_H