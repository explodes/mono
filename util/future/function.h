#ifndef UTIL_FUTURE_FUNCTION_H
#define UTIL_FUTURE_FUNCTION_H

#include <functional>

namespace util {
namespace future {

template <typename T>
using Function = ::std::function<T()>;

}  // namespace future
}  // namespace util

#endif  // UTIL_FUTURE_FUNCTION_H