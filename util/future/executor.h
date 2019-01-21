#ifndef UTIL_FUTURE_EXECUTOR_H
#define UTIL_FUTURE_EXECUTOR_H

#include <functional>
#include <memory>
#include "util/future/function.h"
#include "util/future/future.h"

namespace util {
namespace future {

using ::std::make_unique;
using ::std::unique_ptr;

template <typename T>
class Executor {
 public:
  virtual unique_ptr<Future<T>> Enqueue(const Function<T>& f) = 0;
  virtual ~Executor() {}
};

template <typename T>
class ImmediateExecutor final : public Executor<T> {
 public:
  unique_ptr<Future<T>> Enqueue(const Function<T>& f) {
    T value = f();
    return make_unique<ImmediateFuture<T>>(value);
  }
};

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_EXECUTOR_H