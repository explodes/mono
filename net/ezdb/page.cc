#include "net/ezdb/page.h"

#include "hoist/likely.h"

namespace ezdb {

namespace error = ::Hoist::error;

using ::std::shared_lock;
using ::std::unique_lock;

const Page::Key Page::kNoKey = Page::Key();

const StatusOr<Page::Data> kKeyNotFound =
    Status(error::NOT_FOUND, "key not found");

const StatusOr<Page::Data> Page::Get(const Key& key) const {
  shared_lock<shared_mutex> lock(mutex_);

  const auto search = data_map_.find(key);
  if (search != data_map_.end()) {
    return search->second;
  }

  return kKeyNotFound;
}

Status Page::Set(Key key, Data data) {
  unique_lock<shared_mutex> lock(mutex_);

  data_map_[key] = data;

  return Status::OK;
}

int Page::NumEntries() const {
  shared_lock<shared_mutex> lock(mutex_);

  return data_map_.size();
}

const Page::Key Page::FirstKey() const {
  shared_lock<shared_mutex> lock(mutex_);

  // If there is no data, we have no key.
  // Return no key.
  if (UNLIKELY(data_map_.size() == 0)) {
    return kNoKey;
  }

  return data_map_.begin()->first;
}

const Page::Key Page::LastKey() const {
  shared_lock<shared_mutex> lock(mutex_);

  // If there is no data, we have no key.
  // Return no key.
  if (UNLIKELY(data_map_.size() == 0)) {
    return kNoKey;
  }

  return data_map_.rbegin()->first;
}

}  // namespace ezdb