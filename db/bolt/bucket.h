#ifndef DB_BOLT_BUCKET_H
#define DB_BOLT_BUCKET_H

#include <string>
#include <unordered_map>
#include "db/bolt/internal/page.h"
#include "db/bolt/tx.h"

namespace bolt {

using ::std::string;
using ::std::unordered_map;

// kDefaultFillPercent is the percentage that split pages are filled.
// This value can be changed by setting Bucket.FillPercent.
const float kDefaultFillPercent{0.5};

namespace internal {

const int kMaxKeySize{32768};
const int kMaxValueSize{(1u << 31u) - 2u};

const float kMinFillPercent{0.1};
const float kMaxFillPercent{1.0};

// TODO(explodes): Remove this? It is represented as an embedded pointer in
// bolt::Bucket.
//
// Bucket represents the on-file representation of a bucket. This
// is stored as the "value" of a bucket key. If the bucket is small enough, then
// its root page can be stored inline in the "value", after the bucket header.
// In the case of inline buckets, the "root" will be 0.
class Bucket {
 private:
  // page id of the bucket's root-level page
  pgid root_;
  // monotonically incrementing, used by NextSequence()
  uint64_t sequence_;
};

}  // namespace internal

// Bucket represents a collection of key/value pairs inside the database.
class Bucket {
 public:
  friend class bucket;

 private:
  internal::Bucket* bucket_;
  // the associated transaction
  Tx* tx_;
  // subbucket cache
  unordered_map<string, Bucket*> buckets_;
  // inline page reference
  internal::Page* page_;
};

namespace internal {
const int kBucketHeaderSize = sizeof(Bucket);
}  // namespace internal

}  // namespace bolt

#endif  // DB_BOLT_BUCKET_H