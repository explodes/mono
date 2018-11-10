#ifndef WEBS_SCAFFOLDING_ENDPOINTS_H
#define WEBS_SCAFFOLDING_ENDPOINTS_H

#include <memory>
#include "pistache/endpoint.h"
#include "pistache/router.h"
#include "webs/scaffolding/component.h"

class Endpoints : public Component {
 public:
  explicit Endpoints(const int port)
      : Endpoints(
            Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(port))) {}
  explicit Endpoints(Pistache::Address addr)
      : Endpoints(addr,
                  static_cast<std::size_t>(Pistache::hardware_concurrency())) {}
  Endpoints(Pistache::Address addr, const size_t threads)
      : threads_(threads),
        httpEndpoint_(std::make_shared<Pistache::Http::Endpoint>(addr)) {}

  Endpoints(const Endpoints&) = delete;
  Endpoints& operator=(const Endpoints&) = delete;

  void init() override;
  void start() override;
  void shutdown() override;

 protected:
  typedef Pistache::Rest::Request Request;
  typedef Pistache::Http::ResponseWriter ResponseWriter;
  typedef Pistache::Rest::Router Router;

  virtual void setupRoutes(Router& router) = 0;

 private:
  size_t threads_;
  std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint_;
  Router router_;
};

#endif