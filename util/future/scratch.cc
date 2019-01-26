#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "hoist/init.h"
#include "hoist/logging.h"
#include "hoist/statusor.h"
#include "util/future/executor.h"
#include "util/future/future.h"

using ::Hoist::StatusOr;
using ::std::shared_ptr;
using ::std::thread;
using ::std::vector;
using ::util::future::Executor;
using ::util::future::Function;
using ::util::future::Future;

static constexpr int NUM_THREADS = 30;
static constexpr int JOB_MILLIS = 50;
static constexpr int NUM_FUTURE_GETS = 2;

template <typename T>
Function<T> MakeFunc(T t) {
  return [t] {
    std::this_thread::sleep_for(std::chrono::milliseconds(JOB_MILLIS));
    return t;
  };
}

template <typename T>
void ResolveFuture(int i, shared_ptr<Future<T>> future_ptr) {
  for (int j = 0; j < NUM_FUTURE_GETS; j++) {
    StatusOr<T> result = future_ptr->Get();
    if (!result.ok()) {
      ELOG("ResolveFuture error " << (j + 1) << "/" << NUM_FUTURE_GETS << ": "
                                  << i << ": " << result.ToString());
    } else {
      T value = result.ValueOrDie();
      ILOG("ResolveFuture success " << (j + 1) << "/" << NUM_FUTURE_GETS << ": "
                                    << i << ": " << value);
    }
  }
}

int main() {
  Hoist::Init();

  Executor<int> executor;
  executor.Run();

  vector<thread> resolve_threads;
  for (int i = 0; i < NUM_THREADS; i++) {
    auto enqueue_result = executor.Enqueue(MakeFunc(i * i));
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

  for (auto &resolve_thread : resolve_threads) {
    resolve_thread.join();
  }

  executor.Join();
}
