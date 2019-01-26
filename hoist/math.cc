#include "hoist/math.h"

#include <random>

namespace Hoist {
namespace RNG {

double roll() {
  static std::random_device rd{};
  static std::mt19937 mersenne(rd());
  static std::uniform_real_distribution<> distribution(0, 1);

  return distribution(mersenne);
}

}  // namespace RNG
}  // namespace Hoist