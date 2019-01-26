#include <assert.h>
#include <unistd.h>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "hoist/init.h"
#include "hoist/logging.h"
#include "hoist/status_macros.h"
#include "hoist/statusor.h"
#include "util/future/function.h"
#include "util/future/queue.h"

namespace {

using ::Hoist::StatusOr;
using std::make_pair;
using std::make_shared;
using std::shared_ptr;
using std::thread;
using std::vector;
using ::util::future::Function;
using ::util::future::Queue;

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
    assert(thread_ != nullptr);
    shared_ptr<Future<T>> fut = make_shared<Future<T>>();
    Job job = make_pair(std::move(func), fut);
    RETURN_IF_ERROR(queue_.Put(job));
    return fut;
  }

  void Loop() {
    assert(thread_ == nullptr);
    running_ = true;
    thread_ = new thread(&Executor::RunEventLoop, this);
  };

  void Join() {
    assert(thread_ != nullptr);
    assert(running_);
    running_ = false;
    queue_.Close();
    thread_->join();
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

}  // namespace

template <typename T>
Function<T> MakeFunc(T t) {
  return [t] {
    sleep(1);
    return t;
  };
}

template <typename T>
void ResolveFuture(int i, shared_ptr<Future<T>> future_ptr) {
  StatusOr<T> result = future_ptr->Get();
  if (!result.ok()) {
    ELOG("ResolveFuture error: " << i << ": " << result.ToString());
  } else {
    T value = result.ValueOrDie();
    ILOG("ResolveFuture success: " << i << ": " << value);
  }
}

int main() {
  Hoist::Init();

  Executor<int> exec;
  exec.Loop();

  vector<thread> resolve_threads;
  for (int i = 0; i < 20; i++) {
    auto enqueue_result = exec.Enqueue(MakeFunc(i * i));
    if (!enqueue_result.ok()) {
      ELOG("Enqueue error i=" << i << ": " << enqueue_result.ToString());
      continue;
    } else {
      ILOG("Enqueue success i=" << i);
    }
    auto future_ptr = enqueue_result.ValueOrDie();
    thread resolve_future_thread(&ResolveFuture<int>, i, future_ptr);
    resolve_threads.push_back(std::move(resolve_future_thread));
  }

  for (auto &t : resolve_threads) {
    t.join();
  }

  exec.Join();
}
