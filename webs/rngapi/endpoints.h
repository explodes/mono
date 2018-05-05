#ifndef WEBS_RNGAPI_ENDPOINTS_H
#define WEBS_RNGAPI_ENDPOINTS_H

#include <memory>
#include "hoist/hoist.h"
#include "storage.h"
#include "webs/scaffolding/endpoints.h"

class RngEndpoints final : public Endpoints {
 public:
  RngEndpoints(Storage& storage, Pistache::Address addr, const size_t threads)
      : Endpoints(addr, threads), storage_(storage) {}

 private:
  void setupRoutes(Router& router) override;

  void handleIndex(const Request& request, ResponseWriter response);
  void handleJSON(const Request& request, ResponseWriter response);
  void handleAlgo(const Request& request, ResponseWriter response);
  void handleKV(const Request& request, ResponseWriter response);

  Storage& storage_;
};

#endif