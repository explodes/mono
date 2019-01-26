#include "util/future/queue.h"

#include <thread>

#include "gtest/gtest.h"
#include "hoist/status_macros.h"

namespace util {
namespace future {
namespace {

namespace error = ::Hoist::error;

class QueueTest : public ::testing::Test {
 protected:
  QueueTest() : queue_() {}

  Queue<int> queue_;
};

TEST_F(QueueTest, Put) { ASSERT_OK(queue_.Put(100)); }

TEST_F(QueueTest, PutGet) {
  ASSERT_OK(queue_.Put(100));

  StatusOr<int> result = queue_.Get();
  ASSERT_OK(result.status());
  EXPECT_EQ(result.ValueOrDie(), 100);
}

TEST_F(QueueTest, PutPutGet) {
  ASSERT_OK(queue_.Put(1));
  ASSERT_OK(queue_.Put(2));

  StatusOr<int> result1 = queue_.Get();
  ASSERT_OK(result1.status());
  EXPECT_EQ(result1.ValueOrDie(), 1);

  StatusOr<int> result2 = queue_.Get();
  ASSERT_OK(result2.status());
  EXPECT_EQ(result2.ValueOrDie(), 2);
}

TEST_F(QueueTest, ClosePut) {
  ASSERT_OK(queue_.Close());

  StatusOr<int> result = queue_.Put(1);
  EXPECT_CODE(result.status(), error::Code::UNAVAILABLE);
}

TEST_F(QueueTest, CloseGet) {
  ASSERT_OK(queue_.Close());

  StatusOr<int> result = queue_.Get();
  EXPECT_CODE(result.status(), error::Code::CANCELLED);
}

TEST_F(QueueTest, CloseClose) {
  ASSERT_OK(queue_.Close());

  Status status = queue_.Close();
  EXPECT_CODE(status, error::Code::UNAVAILABLE);
}

TEST_F(QueueTest, AsyncPutGet) {
  auto put = [](Queue<int>& q) { q.Put(100); };
  std::thread t(put, std::ref(queue_));

  StatusOr<int> result = queue_.Get();

  EXPECT_OK(result.status());
  EXPECT_EQ(result.ValueOrDie(), 100);

  t.join();
}

TEST_F(QueueTest, AsyncGetPut) {
  StatusOr<int> result = Status(error::Code::UNKNOWN, "not executed");

  auto get = [&result](Queue<int>& q) { result = q.Get(); };
  std::thread t(get, std::ref(queue_));

  ASSERT_OK(queue_.Put(100));

  t.join();

  EXPECT_OK(result.status());
  EXPECT_EQ(result.ValueOrDie(), 100);
}

}  // namespace
}  // namespace future
}  // namespace util

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}