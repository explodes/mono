
#include <assert.h> /* assert */
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
#include <utility>

#include "hoist/logging.h"
#include "hoist/status_macros.h"
#include "hoist/statusor.h"
#include "util/future/function.h"
#include "util/future/queue.h"

namespace {

using ::Hoist::StatusOr;
using std::condition_variable;
using std::deque;
using std::make_pair;
using std::make_shared;
using std::mutex;
using std::shared_lock;
using std::shared_mutex;
using std::shared_ptr;
using std::thread;
using std::unique_lock;
using ::util::future::Function;
using ::util::future::Queue;

template <typename T>
class Future {
 public:
  T Get() {
    unique_lock<mutex> lock(mutex_);
    condition_.wait(lock, [=] { return complete_; });
    return t_;
  }

 private:
  template <typename U>
  friend class Executor;

  mutable mutex mutex_;
  bool complete_;
  T t_;
  condition_variable condition_;

  void Set(T t) {
    assert(!complete_);
    {
      unique_lock<mutex> lock(mutex_);
      if (!complete_) {
        t_ = std::move(t);
        complete_ = true;
      }
    }
    condition_.notify_one();
  }
};

template <typename T>
class Executor {
 public:
  Executor()
      : mutex_(), running_(false), thread_(nullptr), queue_(), condition_() {}

  StatusOr<shared_ptr<Future<T>>> Enqueue(Function<T> &func) {
    unique_lock lock(mutex_);
    assert(thread_ != nullptr);
    auto fut = make_shared<Future<T>>();
    Job job = make_pair(func, fut);
    RETURN_IF_ERROR(queue_.Put(job));
    condition_.notify_one();
    return fut;
  }

  void Loop() {
    unique_lock lock(mutex_);
    assert(thread_ == nullptr);
    running_ = true;
    thread_ = new thread(&Executor::RunEventLoop, this);
  };

  void Join() {
    unique_lock lock(mutex_);
    assert(thread_ != nullptr);
    assert(running_);
    running_ = false;
    queue_.Close();
    condition_.notify_one();
    thread_->join();
  }

  ~Executor() { delete thread_; }

 private:
  typedef std::pair<Function<T>, shared_ptr<Future<T>>> Job;
  mutable mutex mutex_;
  bool running_;
  thread *thread_;
  Queue<Job> queue_;
  condition_variable condition_;

  void RunEventLoop() {
    while (running_) {
      DLOG("Loop start");
      unique_lock lock(mutex_);
      DLOG("Condition start");
      condition_.wait(lock, [=] { return !running_ || !queue_.Empty(); });
      DLOG("Condition end");
      if (running_ && !queue_.Empty()) {
        StatusOr<Job> result = queue_.Get();
        if (!result.ok()) {
          return;
        }
        Job job = std::move(result.ValueOrDie());
        job.second->Set(std::move(job.first()));
      }
      DLOG("Loop end");
    }
  }
};

}  // namespace

Function<int> makeFunc(int i) {
  Function<int> func = [i] {
    // sleep(1);
    return i;
  };
  return func;
}

int main() {
  Executor<int> exec;
  exec.Loop();

  auto func1 = makeFunc(1);
  auto future1 = exec.Enqueue(func1);
  int val1 = future1.ValueOrDie()->Get();
  DLOG("Got=" << val1);

  auto func2 = makeFunc(2);
  auto future2 = exec.Enqueue(func2);
  int val2 = future2.ValueOrDie()->Get();
  DLOG("Got=" << val2);

  auto func3 = makeFunc(3);
  auto future3 = exec.Enqueue(func3);
  int val3 = future3.ValueOrDie()->Get();
  DLOG("Got=" << val3);

  auto func4 = makeFunc(4);
  auto future4 = exec.Enqueue(func4);
  int val4 = future4.ValueOrDie()->Get();
  DLOG("Got=" << val4);

  exec.Join();
}
