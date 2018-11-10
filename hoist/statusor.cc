#include "hoist/statusor.h"

#include <iostream>

namespace Hoist {
namespace internal {

void StatusOrHelper::Crash(const Status& status) {
  ::std::cerr << "Attempting to fetch value instead of handling error "
              << status.ToString();
}

}  // namespace internal
}  // namespace Hoist