#include <assert.h>
#include <unistd.h>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
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
using std::condition_variable;
using std::condition_variable_any;
using std::deque;
using std::make_pair;
using std::make_shared;
using std::mutex;
using std::scoped_lock;
using std::shared_lock;
using std::shared_mutex;
using std::shared_ptr;
using std::thread;
using std::unique_lock;
using std::vector;
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
      scoped_lock<mutex> lock(mutex_);
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

  StatusOr<shared_ptr<Future<T>>> Enqueue(Function<T> &&func) {
    scoped_lock lock(mutex_);
    DLOG("Enqueue");
    assert(thread_ != nullptr);
    auto fut = make_shared<Future<T>>();
    Job job = make_pair(std::move(func), fut);
    RETURN_IF_ERROR(queue_.Put(job));
    condition_.notify_one();
    return std::move(fut);
  }

  void Loop() {
    scoped_lock lock(mutex_);
    DLOG("Loop");
    assert(thread_ == nullptr);
    running_ = true;
    thread_ = new thread(&Executor::RunEventLoop, this);
  };

  void Join() {
    thread *t(nullptr);
    {
      scoped_lock lock(mutex_);
      DLOG("Join");
      assert(thread_ != nullptr);
      assert(running_);
      running_ = false;
      queue_.Close();
      condition_.notify_one();
      t = thread_;
    }
    t->join();
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
    DLOG("RunEventLoop");
    while (running_) {
      DLOG("Loop start");
      unique_lock lock(mutex_);
      DLOG("Condition start");
      condition_.wait(lock, [=] { return !running_ || !queue_.Empty(); });
      DLOG("Condition end");
      if (running_ && !queue_.Empty()) {
        DLOG("Get start");
        StatusOr<Job> result = queue_.Get();
        DLOG("Get end");
        if (!result.ok()) {
          DLOG("Queue not ok: " << result.ToString());
          return;
        }
        Job job = std::move(result.ValueOrDie());
        DLOG("Job start");
        job.second->Set(std::move(job.first()));
        DLOG("Job end");
      }
      DLOG("Loop end");
    }
  }
};

}  // namespace

Function<int> makeFunc(int i) {
  return [i] {
    ILOG("func i=" << i);
    sleep(1);
    return i;
  };
}

int main() {
  Hoist::Init();

  Executor<int> exec;
  exec.Loop();

  // vector<thread> threads;
  for (int i = 0; i < 20; i++) {
    auto future = exec.Enqueue(makeFunc(i * i));
    auto resolve_future = [](int j, StatusOr<shared_ptr<Future<int>>> &future) {
      DLOG("Thread start: " << j);
      int val = future.ValueOrDie()->Get();
      ILOG("Got i=" << j << " val=" << val);
      DLOG("Thread end: " << j);
    };
    thread resolve_future_thread(resolve_future, i, std::ref(future));
    resolve_future_thread.detach();
    // threads.push_back(std::move(resolve_future_thread));
  }

  exec.Join();
}
