#include "util/future/executor.h"

#include <memory>
#include <thread>
#include <vector>
#include "gtest/gtest.h"
#include "util/future/function.h"
#include "util/future/future.h"
#include "util/future/queue.h"

namespace util {
namespace future {
namespace {

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::thread;
using std::unique_ptr;
using std::vector;

template <typename T>
class DelayedExecutor final : public Executor<T> {
 public:
  unique_ptr<Future<T>> Enqueue(const Function<T>& f) {
    shared_ptr<Queue<T>> queue = make_shared<Queue<T>>();

    auto functor = [&] {
      DLOG("calling functor");
      T value = f();
      DLOG("push");
      queue->push(std::move(value));
    };

    shared_ptr<thread> exec = make_shared<thread>(functor);
    threads_.push_back(exec);

    return make_unique<DeferredFuture<T>>(queue);
  }

  ~DelayedExecutor() {
    for (const auto& thread : threads_) {
      thread->join();
    }
  }

 private:
  vector<shared_ptr<thread>> threads_;
};

class FutureTest : public ::testing::Test {
 protected:
  void SetUp() override {
    immediate_executor_ = new ImmediateExecutor<int>();
    delayed_executor_ = new DelayedExecutor<int>();
  }

  void TearDown() override {
    delete immediate_executor_;
    delete delayed_executor_;
  }

  Executor<int>* immediate_executor_;
  Executor<int>* delayed_executor_;
};

TEST_F(FutureTest, EnqueueImmediate) {
  Function<int> func = [] { return 100; };
  auto future = immediate_executor_->Enqueue(func);
  int result = future->Get();

  EXPECT_EQ(result, 100);
}

TEST_F(FutureTest, EnqueueDelayed) {
  DLOG("EnqueueDelayed");

  Function<int> func = [] {
    DLOG("Inner func");
    return 100;
  };
  DLOG("Enqueue");
  auto future = delayed_executor_->Enqueue(func);
  DLOG("Get");
  int result = future->Get();

  EXPECT_EQ(result, 100);
}

}  // namespace
}  // namespace future
}  // namespace util

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}