#include <csignal>
#include <iostream>
#include "hoist/init.h"
#include "hoist/logging.h"
#include "pistache/endpoint.h"
#include "webs/kv/app.h"

int main() {
  Hoist::Init();

#if DEBUG
  Hoist::waitForInput("Press enter to start.");
#endif

  static const int port = 9080;

  const std::size_t threads =
      static_cast<std::size_t>(Pistache::hardware_concurrency());
  Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(port));

  ILOG("Starting on " << addr.host() << ':' << addr.port() << " with "
                      << threads << " threads.");

  App app(":memory:", addr, threads);
  executeComponent(app);
}