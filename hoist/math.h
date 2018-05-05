#ifndef HOIST_MATH_H
#define HOIST_MATH_H

namespace Hoist {
namespace RNG {

// roll will generate a random value in the range [0, 1)
double roll();

// rand will generate a random value in the range [from, toExclusive)
template <typename T>
const T rand(const T& from, const T& toExclusive) {
  double result = from + (toExclusive - from) * roll();
  return static_cast<T>(result);
}

// to will generate a random value in the range [0, toExclusive)
template <typename T>
const T rand(const T& toExclusive) {
  return rand(0, toExclusive);
}

}  // namespace RNG
}  // namespace Hoist

#endif