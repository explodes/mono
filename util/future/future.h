#ifndef UTIL_FUTURE_FUTURE_H
#define UTIL_FUTURE_FUTURE_H

#include <functional>
#include <memory>
#include <thread>

#include "hoist/status_macros.h"
#include "hoist/statusor.h"
#include "util/future/queue.h"

namespace util {
namespace future {

namespace error = ::Hoist::error;

using ::Hoist::Status;
using ::Hoist::StatusOr;
using std::make_pair;
using std::make_shared;
using std::shared_ptr;
using std::thread;

template <typename T>
using Function = ::std::function<T()>;

template <typename T>
class Future {
 public:
  Future() : queue_() {}

  StatusOr<T> Get() {
    StatusOr<T> val = std::move(queue_.Get());
    queue_.Close();
    return val;
  }

 private:
  template <typename U>
  friend class Executor;

  Queue<T> queue_;

  void Set(T &&t) { queue_.Put(std::move(t)); }
};

template <typename T>
class Executor {
 public:
  Executor() : running_(false), thread_(nullptr), queue_() {}

  StatusOr<shared_ptr<Future<T>>> Enqueue(Function<T> &&func) {
    if (thread_ == nullptr) {
      return Status(error::Code::FAILED_PRECONDITION,
                    "Executor must be started before Enqueing.");
    }
    shared_ptr<Future<T>> future_ptr = make_shared<Future<T>>();
    Job job = make_pair(std::move(func), future_ptr);
    RETURN_IF_ERROR(queue_.Put(job));
    return future_ptr;
  }

  Status Loop() {
    if (thread_ != nullptr) {
      return Status(error::Code::FAILED_PRECONDITION,
                    "Executor must not already be looping.");
    }
    running_ = true;
    thread_ = new thread(&Executor::RunEventLoop, this);
    return Status::OK;
  };

  Status Join() {
    if (!running_ || thread_ != nullptr) {
      return Status(error::Code::FAILED_PRECONDITION,
                    "Executor must not already be looping.");
    }
    running_ = false;
    queue_.Close();
    thread_->join();
    return Status::OK;
  }

  ~Executor() { delete thread_; }

 private:
  typedef std::pair<Function<T>, shared_ptr<Future<T>>> Job;
  bool running_;
  thread *thread_;
  Queue<Job> queue_;

  void RunEventLoop() {
    while (running_) {
      StatusOr<Job> result = queue_.Get();
      if (!running_ || !result.ok()) {
        break;
      }
      Job job = std::move(result.ValueOrDie());
      job.second->Set(std::move(job.first()));
    }
  }
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUTURE_H