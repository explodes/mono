#ifndef NET_EZDB_PAGE_H
#define NET_EZDB_PAGE_H

#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <utility>
#include <vector>
#include "hoist/status.h"
#include "hoist/statusor.h"

namespace ezdb {

using ::Hoist::Status;
using ::Hoist::StatusOr;
using ::std::map;
using ::std::shared_mutex;
using ::std::vector;

// Page is a thread-safe wrapper around a binary key-store.
// Page is a mapping between a Page::Key and Page::Data.
class Page {
 public:
  typedef vector<char> Key;
  typedef vector<char> Data;

  static const Key kNoKey;

  explicit Page() : mutex_(), data_map_() {}

  const StatusOr<Data> Get(const Key& key) const;
  Status Set(Key key, Data data);

  int NumEntries() const;
  const Key FirstKey() const;
  const Key LastKey() const;

 private:
  mutable shared_mutex mutex_;
  map<Key, Data> data_map_;
};

}  // namespace ezdb

#endif