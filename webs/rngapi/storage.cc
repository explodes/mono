#include "webs/rngapi/storage.h"

#ifdef DEBUG
#include <iostream>
#endif

void Storage::init() {
  db_ << "CREATE TABLE IF NOT EXISTS kvstore ("
         "    key text unique not null,"
         "    value text not null"
         ");";
}

void Storage::start() {}

void Storage::shutdown() {}

Hoist::StatusOr<std::string> Storage::get(const std::string &key) {
  try {
    std::string value;
    db_ << "SELECT value FROM kvstore WHERE key = ?;" << key >> value;
    return value;
  } catch (const std::exception &e) {
#ifdef DEBUG
    std::cerr << "error: " << e.what() << std::endl;
#endif
    return Hoist::Status(Hoist::error::NOT_FOUND, "key not found");
  }
}

Hoist::Status Storage::put(const std::string &key, const std::string &value) {
  try {
    if (get(key).ok()) {
      db_ << "UPDATE kvstore SET value = ? WHERE key = ?;" << value << key;
    } else {
      db_ << "INSERT INTO kvstore (key, value) VALUES (?, ?);" << key << value;
    }
    return Hoist::Status::OK;
  } catch (const std::exception &e) {
#ifdef DEBUG
    std::cerr << "error: " << e.what() << std::endl;
#endif
    return Hoist::Status(Hoist::error::INTERNAL, "failed to put value");
  }
}