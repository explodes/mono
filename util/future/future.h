#ifndef UTIL_FUTURE_FUTURE_H
#define UTIL_FUTURE_FUTURE_H

#include <functional>
#include <mutex>

#include "hoist/status_macros.h"
#include "hoist/statusor.h"
#include "util/future/queue.h"

namespace util {
namespace future {

namespace error = ::Hoist::error;

using ::Hoist::Status;
using ::Hoist::StatusOr;
using std::mutex;
using std::scoped_lock;

template <typename T>
class Future {
 public:
  Future()
      : mutex_(),
        set_mutex_(),
        complete_(false),
        set_(false),
        queue_(),
        value_(Status(error::Code::UNAVAILABLE, "Value not yet acquired.")) {}

  StatusOr<T> Get() {
    {
      scoped_lock lock(mutex_);
      if (!complete_) {
        value_ = std::move(queue_.Get());
        queue_.Close();
        complete_ = true;
      }
    }
    return value_;
  }

  Status Set(T &&t) {
    {
      scoped_lock lock(set_mutex_);
      if (set_) {
        return Status(error::Code::FAILED_PRECONDITION,
                      "Future value is already set.");
      }
      set_ = true;
    }
    queue_.Put(std::move(t));
    return Status::OK;
  }

 private:
  mutable mutex mutex_;
  mutable mutex set_mutex_;
  bool complete_;
  bool set_;
  Queue<T> queue_;
  StatusOr<T> value_;
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUTURE_H