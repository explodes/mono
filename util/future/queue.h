#ifndef UTIL_FUTURE_QUEUE_H
#define UTIL_FUTURE_QUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>

#include "hoist/logging.h"
#include "hoist/status.h"
#include "hoist/statusor.h"

namespace util {
namespace future {

namespace error = ::Hoist::error;

using ::Hoist::Status;
using ::Hoist::StatusOr;
using std::condition_variable;
using std::deque;
using std::mutex;
using std::scoped_lock;
using std::unique_lock;

template <typename T>
class Queue {
 public:
  Queue() : mutex_(), available_(true), queue_(), condition_() {}

  Status Put(const T& t) {
    {
      scoped_lock lock(mutex_);
      if (!available_) {
        return Status(error::Code::UNAVAILABLE, "Put on closed Queue.");
      }
      queue_.push_front(t);
    }
    condition_.notify_one();
    return Status::OK;
  }

  Status Put(T&& t) {
    {
      scoped_lock lock(mutex_);
      if (!available_) {
        return Status(error::Code::UNAVAILABLE, "Put on closed Queue.");
      }
      queue_.push_front(t);
    }
    condition_.notify_one();
    return Status::OK;
  }

  StatusOr<T> Get() {
    unique_lock lock(mutex_);
    condition_.wait(lock, [this] { return !available_ || !queue_.empty(); });
    if (!available_) {
      return Status(error::Code::CANCELLED, "Get on closed Queue.");
    }
    T rc(std::move(queue_.back()));
    queue_.pop_back();
    return rc;
  }

  Status Close() {
    {
      scoped_lock lock(mutex_);
      if (!available_) {
        return Status(error::Code::UNAVAILABLE, "Close on closed Queue.");
      }
      available_ = false;
    }
    condition_.notify_one();
    return Status::OK;
  }

  bool Empty() {
    scoped_lock lock(mutex_);
    return queue_.empty();
  }

 private:
  mutable mutex mutex_;
  bool available_;
  deque<T> queue_;
  condition_variable condition_;
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_QUEUE_H