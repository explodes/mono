#include "input.h"
#include <iostream>
#include <limits>

namespace Hoist {

void waitForInput(const std::string &message) { prompt<std::string>(message); }

}  // namespace Hoist