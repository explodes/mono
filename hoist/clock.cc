#include "clock.h"
#include <time.h>

namespace Hoist {

nanos_t SystemClock::nanos() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  nanos_t nanos = static_cast<nanos_t>((ts.tv_sec * 1000000000) + ts.tv_nsec);
  return nanos;
}

}  // namespace Hoist