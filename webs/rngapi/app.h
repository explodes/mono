#ifndef WEBS_RNGAPI_APP_H
#define WEBS_RNGAPI_APP_H

#include <vector>
#include "endpoints.h"
#include "pistache/endpoint.h"
#include "storage.h"
#include "webs/scaffolding/component.h"

class App final : public Component {
 public:
  explicit App(std::string dbPath, Pistache::Address addr,
               const size_t threads) {
    storage_ = new Storage(std::move(dbPath));
    endpoints_ = new RngEndpoints(*storage_, addr, threads);

    std::vector<Component *> v;
    v.reserve(2);
    v.push_back(storage_);
    v.push_back(endpoints_);
    components_ = CompositeComponent(v);
  }

  void init() override;
  void start() override;
  void shutdown() override;

  ~App() {
    delete endpoints_;
    delete storage_;
  }

 private:
  Storage *storage_;
  RngEndpoints *endpoints_;
  CompositeComponent components_;
};

#endif