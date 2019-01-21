#ifndef UTIL_FUTURE_FUTURE_H
#define UTIL_FUTURE_FUTURE_H

#include <functional>
#include <mutex>
#include "hoist/logging.h"
#include "util/future/queue.h"

namespace util {
namespace future {

using ::std::mutex;
using ::std::shared_ptr;
using ::std::unique_lock;

template <typename T>
class Future {
 public:
  virtual T Get() = 0;
};

template <typename T>
class ImmediateFuture final : public Future<T> {
 public:
  ImmediateFuture(T t) : t_(std::move(t)) {}
  T Get() override { return t_; }

 private:
  T t_;
};

template <typename T>
class DeferredFuture final : public Future<T> {
 public:
  DeferredFuture() { DLOG("DeferredFuture::DeferredFuture()"); }
  DeferredFuture(shared_ptr<Queue<T>> queue) : queue_(queue) {}
  T Get() override {
    DLOG("DeferredFuture::Get()");
    if (done_) {
      DLOG("Future already done");
      return t_;
    }
    {
      unique_lock<mutex> lock(mutex_);
      if (!done_) {
        T t(std::move(queue_->pop()));
        done_ = true;
        t_ = t;
      }
    }
    return t_;
  }

 private:
  mutable mutex mutex_;
  shared_ptr<Queue<T>> queue_;
  bool done_ = false;
  T t_;
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUTURE_H