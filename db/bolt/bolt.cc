#include "db/bolt/bolt.h"

#include "db/bolt/constants.h"
#include "db/bolt/errors.h"

namespace bolt {

using ::std::make_unique;

template <typename K, typename V>
inline void FreeMap(unordered_map<K, V>& m) {
  for (auto it = m.begin(); it != m.end(); it++) {
    delete it->second;
  }
  m.clear();
}

unique_ptr<Cursor> Bucket::CreateCursor() {
  tx_->IncrementCursorCount();
  return make_unique<Cursor>(this);
}

Bucket* Bucket::NestedBucket(bytes name) {
  // TODO(explodes): impl
  auto search = buckets_.find(name);
  if (search != buckets_.end()) {
    return search->second;
  }

  // Move cursor to key.
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kv = cursor->SeekHelper(name);
  if (!BytesEqual(name, kv.key) || !kv.IsLeaf()) {
    return nullptr;
  }

  // Otherwise create a Bucket and cache it.
  // TODO(explodes): Is this a correct transfer of ownership?
  unique_ptr<Bucket> child_ptr = OpenBucket(kv.value);
  Bucket* child = child_ptr.release();
  if (child != nullptr) {
    buckets_[name] = child;
  }
  return child;
}

unique_ptr<Bucket> Bucket::OpenBucket(bytes value) {
  unique_ptr<Bucket> child = make_unique<Bucket>(tx_);

  // If unaligned load/stores are broken on this arch and value is
  // unaligned simply clone to an aligned byte array.
  bytes& aligned_value = value;
  bool unaligned = kBrokenUnaligned &&
                   (reinterpret_cast<std::uintptr_t>(&value[0]) & 3) != 0;
  if (unaligned) {
    bytes value_copy;
    value_copy = value;
    aligned_value = value_copy;
  }

  // If this is a writable transaction then we need to copy the bucket entry.
  // Read-only transactions can point directly at the mmap entry.
  BucketMeta* bucket_meta_ptr = &child->bucket_meta_;
  if (!unaligned && tx_->IsWriteable()) {
    *bucket_meta_ptr = *reinterpret_cast<BucketMeta*>(&aligned_value[0]);
  } else {
    bucket_meta_ptr = reinterpret_cast<BucketMeta*>(&aligned_value[0]);
  }

  // Save a reference to the inline page if the bucket is inline.
  if (child->bucket_meta_.root_ == 0) {
    child->page_ = reinterpret_cast<Page*>(&aligned_value[sizeof(BucketMeta)]);
  }

  return child;
}

StatusOr<Bucket*> Bucket::CreateBucket(bytes key) {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  } else if (key.empty()) {
    return kErrBucketNameRequired;
  }

  // Move cursor to correct position.
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kvf = cursor->SeekHelper(key);

  // Return an error if there is an existing key.
  if (BytesEqual(key, kvf.key)) {
    if (kvf.IsLeaf()) {
      return kErrBucketExists;
    }
    return kErrIncompatibleValue;
  }

  // Create empty, inline bucket.
  Bucket bucket = Bucket();
  bytes value = bucket.Write();

  // Insert into node.
  bytes value_copy;  // TODO(explodes): Possibly not required.
  value_copy = value;
  cursor->CurrentNode()->Put(key, key, value, 0, kBucketLeafFlag);

  // Since subbuckets are not allowed on inline buckets, we need to
  // dereference the inline page, if it exists. This will cause the bucket
  // to be treated as a regular, non-inline bucket for the rest of the tx.
  page_ = nullptr;

  return NestedBucket(key);
}

StatusOr<Bucket*> Bucket::CreateBucketIfNotExists(bytes key) {
  StatusOr<Bucket*> child = CreateBucket(key);
  if (child.status() == kErrBucketExists) {
    return NestedBucket(key);
  }
  return child;
}

Status Bucket::DeleteBucket(bytes key) {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  }

  // Move cursor to correct position.
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kvf = cursor->SeekHelper(key);

  // Return an error if bucket doesn't exist or is not a bucket.
  if (!BytesEqual(key, kvf.key)) {
    return kErrBucketNotFound;
  } else if (!kvf.IsLeaf()) {
    return kErrIncompatibleValue;
  }

  // Recursively delete all child buckets.
  StatusOr<Bucket*> child_status = NestedBucket(key);
  if (!child_status.ok()) {
    return child_status.status();
  }
  Bucket* child = child_status.ValueOrDie();
  function<Status(const bytes& key, bytes* value)> lambda =
      [&child](const bytes& key, bytes* value) {
        if (value != nullptr) {
          return Status::OK;
        }
        return child->DeleteBucket(key);
      };
  Status recursion_status = child->ForEach(lambda);
  if (!recursion_status.ok()) {
    return recursion_status;
  }

  // Remove cached copy.
  delete buckets_[key];

  // Release all bucket pages to freelist.
  FreeMap(child->nodes_);
  delete root_node_;
  root_node_ = nullptr;
  child->Free();

  // Delete the node if we have a matching key.
  cursor->CurrentNode()->Del(key);

  return Status::OK;
}

optional<bytes> Bucket::Get(bytes key) {
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kvf = cursor->SeekHelper(key);

  if (!kvf.IsLeaf()) {
    return {};
  }

  if (!BytesEqual(key, kvf.key)) {
    return {};
  }

  return {kvf.value};
}

Status Bucket::Put(bytes key, bytes value) {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  } else if (key.empty()) {
    return kErrKeyRequired;
  } else if (key.size() > kMaxKeySize) {
    return kErrKeyTooLarge;
  } else if (key.size() > kMaxValueSize) {
    return kErrValueTooLarge;
  }

  // Move cursor to correct position.
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kvf = cursor->SeekHelper(key);

  // Return an error if there is an existing key with a bucket value.
  if (BytesEqual(key, kvf.key) && kvf.IsLeaf()) {
    return kErrIncompatibleValue;
  }

  // Insert into node.
  bytes value_copy;  // TODO(explodes): Possibly not required.
  value_copy = value;
  cursor->CurrentNode()->Put(key, key, value, 0, 0);

  return Status::OK;
}

Status Bucket::Delete(bytes key) {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  }

  // Move cursor to correct position.
  unique_ptr<Cursor> cursor = CreateCursor();
  KeyValueFlags kvf = cursor->SeekHelper(key);

  // Return an error if there is already existing bucket value.
  if (kvf.IsLeaf()) {
    return kErrIncompatibleValue;
  }

  cursor->CurrentNode()->Del(key);

  return Status::OK;
}

Status Bucket::SetSequence(uint64_t sequence) {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  }

  // Materialize the root node if it hasn't been already so that the
  // bucket will be saved during commit.
  if (root_node_ == nullptr) {
    GetNode(bucket_meta_.root_, nullptr);
  }

  // Set and return the sequence.
  bucket_meta_.sequence_ = sequence;
  return Status::OK;
}

StatusOr<uint64_t> Bucket::NextSequence() {
  if (tx_->GetDB() == nullptr) {
    return kErrTxClosed;
  } else if (!tx_->IsWriteable()) {
    return kErrTxNotWritable;
  }

  // Materialize the root node if it hasn't been already so that the
  // bucket will be saved during commit.
  if (root_node_ == nullptr) {
    GetNode(bucket_meta_.root_, nullptr);
  }

  // Increment and return the sequence.
  return ++bucket_meta_.sequence_;
}

Status Bucket::ForEach(
    function<Status(const bytes& key, bytes* value)> const& fn) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "ForEach is not implemented.");
}

BucketStats Bucket::Stats() {
  // TODO(explodes): impl
  return BucketStats();
}

void Bucket::ForEachPage(function<void(Page*, int)> fn) {
  // TODO(explodes): impl
}

void Bucket::ForEachPageNode(function<void(Page*, Node*, int)> fn) {
  // TODO(explodes): impl
}

void Bucket::ForEachPageNodeHelper(pgid id, int depth,
                                   function<void(Page*, Node*, int)> fn) {
  // TODO(explodes): impl
}

Status Bucket::Spill() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Spill is not implemented.");
}

bool Bucket::Inlineable() {
  // TODO(explodes): impl
  return false;
}

int Bucket::MaxInlineBucketSize() {
  // TODO(explodes): impl
  return 0;
}

bytes Bucket::Write() {
  // TODO(explodes): impl
  return bytes();
}

void Bucket::Rebalance() {
  // TODO(explodes): impl
}

Node* GetNode(pgid id, Node* parent) {
  // TODO(explodes): impl
  return nullptr;
}

void Bucket::Free() {
  // TODO(explodes): impl
}

void Bucket::Dereference() {
  // TODO(explodes): impl
}

Bucket::PageNode Bucket::GetPageNode(pgid id) {
  // TODO(explodes): impl
  return PageNode();
}

KeyValue Cursor::First() {
  // TODO(explodes): impl
  return KeyValue();
}

KeyValue Cursor::Last() {
  // TODO(explodes): impl
  return KeyValue();
}

KeyValue Cursor::Next() {
  // TODO(explodes): impl
  return KeyValue();
}

KeyValue Cursor::Prev() {
  // TODO(explodes): impl
  return KeyValue();
}

KeyValue Cursor::Seek(bytes seek) {
  // TODO(explodes): impl
  return KeyValue();
}

Status Cursor::Delete() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Delete is not implemented.");
}

KeyValueFlags Cursor::SeekHelper(bytes seek) {
  // TODO(explodes): impl
  return KeyValueFlags();
}

void Cursor::FirstHelper() {
  // TODO(explodes): impl
}

void Cursor::LastHelper() {
  // TODO(explodes): impl
}

optional<KeyValueFlags> Cursor::NextHelper() {
  // TODO(explodes): impl
  return std::nullopt;
}

void Cursor::Search(bytes key, pgid id) {
  // TODO(explodes): impl
}

void Cursor::SearchNode(bytes key, Node* node) {
  // TODO(explodes): impl
}

void Cursor::SearchPage(bytes key, Page* page) {
  // TODO(explodes): impl
}

void Cursor::NSearch(bytes key) {
  // TODO(explodes): impl
}

KeyValueFlags Cursor::CurrentKeyValue() {
  // TODO(explodes): impl
  return KeyValueFlags();
}

Node* Cursor::CurrentNode() {
  // TODO(explodes): impl
  return nullptr;
}

void Tx::Init(DB* db) {
  // TODO(explodes): impl
}

int Tx::ID() const {
  // TODO(explodes): impl
  return 0;
}

int64_t Tx::Size() const {
  // TODO(explodes): impl
  return 0;
}

unique_ptr<Cursor> Tx::GetCursor() {
  // TODO(explodes): impl
  return nullptr;
}

Bucket* Tx::GetBucket(bytes name) {
  // TODO(explodes): impl
  return nullptr;
}

StatusOr<Bucket> Tx::CreateBucket(bytes name) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "CreateBucket is not implemented.");
}

StatusOr<Bucket> Tx::CreateBucketIfNotExists(bytes name) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED,
                "CreateBucketIfNotExists is not implemented.");
}

Status Tx::DeleteBucket(bytes name) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "DeleteBucket is not implemented.");
}

Status Tx::ForEach(function<Status(bytes, Bucket*)> fn) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "ForEach is not implemented.");
}

Status Tx::Commit() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Commit is not implemented.");
}

Status Tx::Rollback() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Rollback is not implemented.");
}

void Tx::RollbackHelper() {
  // TODO(explodes): impl
}

void Tx::CloseHelper() {
  // TODO(explodes): impl
}

Status Tx::CopyFile(string path, int mode) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "CopyFile is not implemented.");
}

Chan<Status> Tx::Check() {
  // TODO(explodes): impl
  return Chan<Status>();
}

void Tx::CheckHelper(Chan<Status>& ch) {
  // TODO(explodes): impl
}

void Tx::CheckBucket(Bucket* bucket, unordered_map<pgid, Page*>& reachable,
                     unordered_map<pgid, bool>& freed, Chan<Status>& ch) {
  // TODO(explodes): impl
}

StatusOr<Page*> Tx::Allocate(int count) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Allocate is not implemented.");
}

Status Tx::Write() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Write is not implemented.");
}

Status Tx::WriteMeta() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "WriteMeta is not implemented.");
}

Page* Tx::GetPageHelper(pgid id) {
  // TODO(explodes): impl
  return nullptr;
}

void Tx::ForEachPage(pgid id, int depth, function<void(Page*, int)> fn) {
  // TODO(explodes): impl
}

StatusOr<PageInfo> Tx::GetPageInfo(int id) {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "GetPageInfo is not implemented.");
}

Node* Node::Root() const {
  // TODO(explodes): impl
  return nullptr;
}

int Node::MinKeys() {
  // TODO(explodes): impl
  return 0;
}

int Node::Size() const {
  // TODO(explodes): impl
  return 0;
}

bool Node::SizeLessThan(int v) const {
  // TODO(explodes): impl
  return false;
}

int Node::PageElementSize() const {
  // TODO(explodes): impl
  return 0;
}

Node* Node::ChildAt(int index) const {
  // TODO(explodes): impl
  return nullptr;
}

int Node::ChildIndex(Node* child) const {
  // TODO(explodes): impl
  return 0;
}

Node* Node::NextSibling() const {
  // TODO(explodes): impl
  return nullptr;
}

Node* Node::PrevSibling() const {
  // TODO(explodes): impl
  return nullptr;
}

void Node::Put(bytes old_key, bytes new_key, bytes value, pgid id,
               uint32_t flags) {
  // TODO(explodes): impl
}

void Node::Del(bytes key) {
  // TODO(explodes): impl
}

void Node::Read(Page* page) {
  // TODO(explodes): impl
}

void Node::Write(Page* page) {
  // TODO(explodes): impl
}

vector<Node*> Node::Split(int page_size) {
  // TODO(explodes): impl
  vector<Node*> v;
  return v;
}

std::pair<Node*, Node*> Node::SplitTwo(int page_size) {
  // TODO(explodes): impl
  return {nullptr, nullptr};
}

Node::SplitIndexType Node::SplitIndex(int threshold) {
  // TODO(explodes): impl
  return Node::SplitIndexType();
}

Status Node::Spill() {
  // TODO(explodes): impl
  return Status(error::UNIMPLEMENTED, "Spill is not implemented.");
}

void Node::Rebalance() {
  // TODO(explodes): impl
}

void Node::RemoveChild(Node* target) {
  // TODO(explodes): impl
}

void Node::Dereference() {
  // TODO(explodes): impl
}

void Node::Free() {
  // TODO(explodes): impl
}

}  // namespace bolt