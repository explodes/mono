#include "endpoints.h"
#include <sstream>
#include <unordered_map>
#include "hoist/hoist.h"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

void KvEndpoints::setupRoutes(Router& router) {
  Pistache::Rest::Routes::Get(
      router, "/kv/:key",
      Pistache::Rest::Routes::bind(&KvEndpoints::handleKV_get, this));
  Pistache::Rest::Routes::Put(
      router, "/kv/:key/:value",
      Pistache::Rest::Routes::bind(&KvEndpoints::handleKV_put, this));
}

void KvEndpoints::handleKV_get(const Request& request,
                               ResponseWriter response) {
  std::string key = request.param(":key").as<std::string>();

  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Text, Plain));

  auto fetch = storage_.get(key);
  DLOG("get " << key << " " << (fetch.ok() ? "ok" : "not ok"));
  if (fetch.ok()) {
    response.send(Pistache::Http::Code::Ok, fetch.value());
  } else {
    response.send(Pistache::Http::Code::Internal_Server_Error);
  }
}

void KvEndpoints::handleKV_put(const Request& request,
                               ResponseWriter response) {
  std::string key = request.param(":key").as<std::string>();
  std::string value = request.param(":value").as<std::string>();

  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Text, Plain));

  auto status = storage_.put(key, value);
  DLOG("set " << key << "=" << value << (status.ok() ? " ok" : " not ok"));
  if (status.ok()) {
    response.send(Pistache::Http::Code::Created);
  } else {
    response.send(Pistache::Http::Code::Internal_Server_Error);
  }
}
