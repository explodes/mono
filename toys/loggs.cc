#include "hoist/init.h"
#include "hoist/logging.h"

int main() {
  Hoist::Init();

  ELOG("error log always"
       << " (ok)");
  WLOG("warning log always"
       << " (ok)");
  ILOG("info log always"
       << " (ok)");
  DLOG("debug log always"
       << " (ok)");

  ELOG_IF(1, "error log sometimes"
                 << " (ok)");
  WLOG_IF(1, "warning log sometimes"
                 << " (ok)");
  ILOG_IF(1, "info log sometimes"
                 << " (ok)");
  DLOG_IF(1, "debug log sometimes"
                 << " (ok)");
}