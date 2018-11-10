#include "webs/scaffolding/endpoints.h"

#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

void Endpoints::init() {
  auto opts = Pistache::Http::Endpoint::options().threads(threads_).flags(
      Pistache::Tcp::Options::ReuseAddr);
  httpEndpoint_->init(opts);
  setupRoutes(router_);
}

void Endpoints::start() {
  httpEndpoint_->setHandler(router_.handler());
  httpEndpoint_->serve();
}

void Endpoints::shutdown() { httpEndpoint_->shutdown(); }