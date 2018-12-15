#include "db/bolt/statusor.h"

#include <stdlib.h>
#include <iostream>

namespace bolt {
namespace internal {

void StatusOrHelper::Crash(const Status& status) {
  ::std::cerr << "Status not OK: " << status.ToString();
  exit(EXIT_FAILURE);
}

}  // namespace internal
}  // namespace bolt