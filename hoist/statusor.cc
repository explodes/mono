#include "hoist/statusor.h"

#include <stdlib.h>
#include <iostream>

namespace Hoist {
namespace internal {

void StatusOrHelper::Crash(const Status& status) {
  ::std::cerr << "Status not OK: " << status.ToString();
  exit(EXIT_FAILURE);
}

}  // namespace internal
}  // namespace Hoist