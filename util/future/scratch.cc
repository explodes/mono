#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "hoist/init.h"
#include "hoist/logging.h"
#include "hoist/statusor.h"
#include "util/future/future.h"

using ::Hoist::StatusOr;
using ::std::shared_ptr;
using ::std::thread;
using ::std::vector;
using ::util::future::Executor;
using ::util::future::Function;
using ::util::future::Future;

static constexpr int NUM_THREADS = 30;
static constexpr int JOB_MILLIS = 100;

template <typename T>
Function<T> MakeFunc(T t) {
  return [t] {
    std::this_thread::sleep_for(std::chrono::milliseconds(JOB_MILLIS));
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
  for (int i = 0; i < NUM_THREADS; i++) {
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
