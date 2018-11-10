#ifndef WEBS_KV_STORAGE_H
#define WEBS_KV_STORAGE_H

#include <memory>
#include "hoist/status.h"
#include "hoist/statusor.h"
#include "sqlite_modern_cpp.h"
#include "webs/scaffolding/component.h"

class Storage final : public Component {
 public:
  explicit Storage(std::string dbPath)
      : db_(sqlite::database(std::move(dbPath))) {}

  Storage(const Storage &) = delete;
  Storage &operator=(const Storage &) = delete;

  void init() override;
  void start() override;
  void shutdown() override;

  Hoist::StatusOr<std::string> get(const std::string &key);
  Hoist::Status put(const std::string &key, const std::string &value);

 private:
  sqlite::database db_;
};

#endif