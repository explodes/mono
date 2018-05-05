#ifndef WEBS_RNGAPI_DB_H
#define WEBS_RNGAPI_DB_H

#include <memory>
#include "hoist/hoist.h"
#include "sqlite_modern_cpp.h"
#include "webs/scaffolding/component.h"

class Storage final : public Component {
 public:
  Storage(std::string dbPath) : db_(sqlite::database(std::move(dbPath))) {}

  Storage(const Storage &) = delete;
  Storage &operator=(const Storage &) = delete;

  void init() override;
  void start() override;
  void shutdown() override;

  Hoist::Result<std::string> get(const std::string &key);
  Hoist::Status put(const std::string &key, const std::string &value);

 private:
  sqlite::database db_;
};

#endif