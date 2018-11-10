#include "webs/kv/storage.h"

#include <tuple>

#ifdef DEBUG
#include <iostream>
#endif

void Storage::init() {
  db_ << "CREATE TABLE IF NOT EXISTS kvstore ("
         "    key text unique not null,"
         "    value text not null,"
         "    expiry integer not null"
         ");";
}

void Storage::start() {}

void Storage::shutdown() {}

bool isExpirationValid(const int expiry) {
  // TODO(explodes): expiration time
  return expiry == 0;
}

Hoist::StatusOr<std::string> Storage::get(const std::string &key) {
  try {
    std::string value;
    int expiry;
    db_ << "SELECT value, expiry FROM kvstore WHERE key = ?;" << key >>
        std::tie(value, expiry);
    if (isExpirationValid(expiry)) {
      return value;
    } else {
      return Hoist::Status(Hoist::error::NOT_FOUND, "key not found");
    }
  } catch (const std::exception &e) {
#ifdef DEBUG
    std::cerr << "error: " << e.what() << std::endl;
#endif
    return Hoist::Status(Hoist::error::INTERNAL, "could not get key");
  }
}

Hoist::Status Storage::put(const std::string &key, const std::string &value) {
  try {
    if (get(key).ok()) {
      db_ << "UPDATE kvstore SET value = ? WHERE key = ?;" << value << key;
    } else {
      db_ << "INSERT INTO kvstore (key, value, expiry) VALUES (?, ?, 0);" << key
          << value;
    }
    return Hoist::Status::OK;
  } catch (const std::exception &e) {
#ifdef DEBUG
    std::cerr << "error: " << e.what() << std::endl;
#endif
    return Hoist::Status(Hoist::error::INTERNAL, "could not set key");
  }
}