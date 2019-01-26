#include "hoist/init.h"

#include "hoist/logging.h"

namespace Hoist {

void Init() {
  // reserved for future use.
  InitLogging();
  DLOG("Hoist initialized.");
}

}  // namespace Hoist