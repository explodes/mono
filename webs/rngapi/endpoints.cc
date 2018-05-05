#include "endpoints.h"
#include <sstream>
#include <unordered_map>
#include "hoist/hoist.h"
#include "nlohmann/json.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

void RngEndpoints::setupRoutes(Router& router) {
  Pistache::Rest::Routes::Get(
      router, "/",
      Pistache::Rest::Routes::bind(&RngEndpoints::handleIndex, this));
  Pistache::Rest::Routes::Get(
      router, "/json",
      Pistache::Rest::Routes::bind(&RngEndpoints::handleJSON, this));
  Pistache::Rest::Routes::Get(
      router, "/algo",
      Pistache::Rest::Routes::bind(&RngEndpoints::handleAlgo, this));
  Pistache::Rest::Routes::Get(
      router, "/kv/:key",
      Pistache::Rest::Routes::bind(&RngEndpoints::handleKV, this));
  Pistache::Rest::Routes::Put(
      router, "/kv/:key/:value",
      Pistache::Rest::Routes::bind(&RngEndpoints::handleKV, this));
}

void RngEndpoints::handleIndex(const Request& request,
                               ResponseWriter response) {
  std::ostringstream strs;
  strs << Hoist::RNG::rand<double>(0.0, 1.0);
  const std::string str = strs.str();

  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Text, Plain));
  response.send(Pistache::Http::Code::Ok, str);
}

void RngEndpoints::handleJSON(const Request& request, ResponseWriter response) {
  auto json = nlohmann::json::object();
  json["number"] = Hoist::RNG::rand<double>(0.0, 1.0);

  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Application, Json));
  const std::string jsonStr = json.dump();
  response.send(Pistache::Http::Code::Ok, jsonStr);
}

void RngEndpoints::handleAlgo(const Request& request, ResponseWriter response) {
  std::unordered_map<std::string, std::string> m;

  m.insert({"alex", "trebek"});
  m.insert({"shaun", "connery"});
  m.insert({"pink", "floyd"});
  m.insert({"taco", "tuesday"});

  auto json = nlohmann::json::object();
  for (const auto& v : m) {
    json[v.first] = v.second;
  }

  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Application, Json));
  const std::string jsonStr = json.dump();
  response.send(Pistache::Http::Code::Ok, jsonStr);
}

void RngEndpoints::handleKV(const Request& request, ResponseWriter response) {
  response.headers().add<Pistache::Http::Header::ContentType>(
      MIME(Text, Plain));

  auto key = request.param(":key").as<std::string>();

  if (request.hasParam(":value")) {
    auto value = request.param(":value").as<std::string>();
    Hoist::Status status = storage_.put(key, value);
    if (!status.ok()) {
      response.send(Pistache::Http::Code::Internal_Server_Error);
      return;
    } else {
      response.send(Pistache::Http::Code::Ok);
      return;
    }
  }

  Hoist::Result<std::string> fetch = storage_.get(key);
  if (!fetch.ok()) {
    response.send(Pistache::Http::Code::Internal_Server_Error);
    return;
  } else {
    response.send(Pistache::Http::Code::Ok, fetch.value());
    return;
  }
}
