#ifndef WEBS_RNGAPI_ENDPOINTS_H
#define WEBS_RNGAPI_ENDPOINTS_H

#include <memory>
#include "webs/kv/storage.h"
#include "webs/scaffolding/endpoints.h"

class KvEndpoints final : public Endpoints {
 public:
  KvEndpoints(Storage& storage, Pistache::Address addr, const size_t threads)
      : Endpoints(addr, threads), storage_(storage) {}

 private:
  void setupRoutes(Router& router) override;

  void handleKV_get(const Request& request, ResponseWriter response);
  void handleKV_put(const Request& request, ResponseWriter response);

  Storage& storage_;
};

#endif