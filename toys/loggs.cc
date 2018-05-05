#include "hoist/hoist.h"

int main() {
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