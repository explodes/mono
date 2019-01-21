#ifndef UTIL_FUTURE_FUNCTION_H
#define UTIL_FUTURE_FUNCTION_H

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

namespace util {
namespace future {

using std::condition_variable;
using std::deque;
using std::mutex;

template <typename T>
using Function = ::std::function<T()>;

template <typename T>
class Queue {
 public:
  void push(T const& value) {
    {
      std::unique_lock<std::mutex> lock(this->d_mutex);
      d_queue.push_front(value);
    }
    this->d_condition.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=] { return !this->d_queue.empty(); });
    T rc(std::move(this->d_queue.back()));
    this->d_queue.pop_back();
    return rc;
  }

 private:
  std::mutex d_mutex;
  std::condition_variable d_condition;
  std::deque<T> d_queue;
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUNCTION_H