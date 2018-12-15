#ifndef DB_BOLT_BUCKET_H
#define DB_BOLT_BUCKET_H

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "db/bolt/bytes.h"
#include "db/bolt/page.h"
#include "db/bolt/status.h"
#include "db/bolt/statusor.h"

namespace bolt {

// kDefaultFillPercent is the percentage that split pages are filled.
// This value can be changed with Bucket::SetFillPercent.
const float kDefaultFillPercent{0.5};

const int kMaxKeySize{32768};
const int kMaxValueSize{(1u << 31u) - 2u};

const float kMinFillPercent{0.1};
const float kMaxFillPercent{1.0};

const uint32_t kBucketLeafFlag = 0x01;

using ::std::function;
using ::std::optional;
using ::std::shared_ptr;
using ::std::string;
using ::std::unique_ptr;
using ::std::unordered_map;
using ::std::vector;

class Cursor;
class Bucket;

class Writer {
  // TODO(explodes): Replace stub with something idiomatic.
};

template <typename T>
class Chan {
  // TODO(explodes): Replace stub with something idiomatic.
};

class DbMeta {};

// DB represents a collection of buckets persisted to a file on disk.
// All data access is performed through transactions which can be obtained
// through the DB. All the functions on DB will return a ErrDatabaseNotOpen if
// accessed before Open() is called.
class DB {};

// TxStats represents statistics about the actions performed by the transaction.
struct TxStats {
 public:
  // Page statistics.
  int page_count;  // number of page allocations
  int page_alloc;  // total bytes allocated

  // Cursor statistics.
  int cursor_count;  // number of cursors created

  // Node statistics
  int node_count;  // number of node allocations
  int node_deref;  // number of node dereferences

  // Rebalance statistics.
  int rebalance;            // number of node rebalances
  uint64_t rebalance_time;  // total time spent rebalancing

  // Split/Spill statistics.
  int split;            // number of nodes split
  int spill;            // number of nodes spilled
  uint64_t spill_time;  // total time spent spilling

  // Write statistics.
  int write;            // number of writes performed
  uint64_t write_time;  // total time spent writing to disk

  void Add(const TxStats& other) {
    page_count += other.page_count;
    page_alloc += other.page_alloc;
    cursor_count += other.cursor_count;
    node_count += other.node_count;
    node_deref += other.node_deref;
    rebalance += other.rebalance;
    rebalance_time += other.rebalance_time;
    split += other.split;
    spill += other.spill;
    spill_time += other.spill_time;
    write += other.write;
    write_time += other.write_time;
  }

  TxStats Sub(const TxStats& other) {
    TxStats diff;
    diff.page_count = page_count - other.page_count;
    diff.page_alloc = page_alloc - other.page_alloc;
    diff.cursor_count = cursor_count - other.cursor_count;
    diff.node_count = node_count - other.node_count;
    diff.node_deref = node_deref - other.node_deref;
    diff.rebalance = rebalance - other.rebalance;
    diff.rebalance_time = rebalance_time - other.rebalance_time;
    diff.split = split - other.split;
    diff.spill = spill - other.spill;
    diff.spill_time = spill_time - other.spill_time;
    diff.write = write - other.write;
    diff.write_time = write_time - other.write_time;
    return diff;
  }
};

struct PageInfo {
 public:
  int id;
  string type;
  int count;
  int overflow_count;
};

// Tx represents a read-only or read/write transaction on the database.
// Read-only transactions can be used for retrieving values for keys and
// creating cursors. Read/write transactions can create and remove buckets and
// create and remove keys.
//
// IMPORTANT: You must commit or rollback transactions when you are done with
// them. Pages can not be reclaimed by the writer until no more transactions
// are using them. A long running read transaction can cause the database to
// quickly grow.
class Tx {
 public:
  // Init initializes the transaction.
  void Init(DB* db);

  // ID returns the transaction id.
  int ID() const;

  // GetDB returns a reference to the database that created the transaction.
  DB* GetDB() const { return db_; }

  // Size returns current database size in bytes as seen by this transaction.
  int64_t Size() const;

  // GetCursor creates a cursor associated with the root bucket.
  // All items in the cursor will return a nil value because all root bucket
  // keys point to buckets. The cursor is only valid as long as the transaction
  // is open. Do not use a cursor after the transaction is closed.
  unique_ptr<Cursor> GetCursor();

  // GetStats retrieves a copy of the current transaction statistics.
  TxStats GetStats() const { return stats_; }

  // GetBucket retrieves a bucket by name.
  // Returns nil if the bucket does not exist.
  // The bucket instance is only valid for the lifetime of the transaction.
  Bucket* GetBucket(bytes name);

  // CreateBucket creates a new bucket.
  // Returns an error if the bucket already exists, if the bucket name is blank,
  // or if the bucket name is too long. The bucket instance is only valid for
  // the lifetime of the transaction.
  StatusOr<Bucket> CreateBucket(bytes name);

  // CreateBucketIfNotExists creates a new bucket if it doesn't already exist.
  // Returns an error if the bucket name is blank, or if the bucket name is too
  // long. The bucket instance is only valid for the lifetime of the
  // transaction.
  StatusOr<Bucket> CreateBucketIfNotExists(bytes name);

  // DeleteBucket deletes a bucket.
  // Returns an error if the bucket cannot be found or if the key represents a
  // non-bucket value.
  Status DeleteBucket(bytes name);

  // ForEach executes a function for each bucket in the root.
  // If the provided function returns an error then the iteration is stopped and
  // the error is returned to the caller.
  Status ForEach(function<Status(bytes, Bucket*)> fn);

  // OnCommit adds a handler function to be executed after the transaction
  // successfully commits.
  void OnCommit(shared_ptr<function<void()>> fn) {
    commit_handlers_.push_back(fn);
  }

  // Commit writes all changes to disk and updates the meta page.
  // Returns an error if a disk write error occurs, or if Commit is
  // called on a read-only transaction.
  Status Commit();

  // Rollback closes the transaction and ignores all previous updates. Read-only
  // transactions must be rolled back and not committed.
  Status Rollback();

  // WriteTo writes the entire database to a writer.
  // If err == nil then exactly tx.Size() bytes will be written into the writer.
  StatusOr<int64_t> WriteTo(Writer& writer);

  // CopyFile copies the entire database to file at the given path.
  // A reader transaction is maintained during the copy so it is safe to
  // continue using the database while a copy is in progress.
  Status CopyFile(string path, int mode);

  // Check performs several consistency checks on the database for this
  // transaction. An error is returned if any inconsistency is found.
  //
  // It can be safely run concurrently on a writable transaction. However, this
  // incurs a high cost for large databases and databases with a lot of
  // subbuckets because of caching. This overhead can be removed if running on a
  // read-only transaction, however, it is not safe to execute other writer
  // transactions at the same time.
  Chan<Status> Check();

  // GetPageInfo returns page information for a given page number.
  // This is only safe for concurrent use when used by a writable transaction.
  StatusOr<PageInfo> GetPageInfo(int id);

  bool IsWriteable() const { return writable_; }

  // WriteFlag specifies the flag for write-related methods like WriteTo().
  // Tx opens the database file with the specified flag to copy the data.
  //
  // By default, the flag is unset, which works well for mostly in-memory
  // workloads. For databases that are much larger than available RAM,
  // set the flag to syscall.O_DIRECT to avoid trashing the page cache.
  void SetWriteFlag(int write_flag) { write_flag_ = write_flag; }

  void IncrementCursorCount() { stats_.cursor_count++; }

 private:
  friend class Bucket;

  bool writable_;
  bool managed_;
  DB* db_;
  DbMeta* meta_;
  Bucket* root_;
  unordered_map<pgid, Page*> pages_;
  TxStats stats_;
  vector<shared_ptr<function<void()>>> commit_handlers_;
  int write_flag_;

  void RollbackHelper();

  void CloseHelper();

  void CheckHelper(Chan<Status>& ch);
  void CheckBucket(Bucket* bucket, unordered_map<pgid, Page*>& reachable,
                   unordered_map<pgid, bool>& freed, Chan<Status>& ch);

  // Allocate returns a contiguous block of memory starting at a given page.
  StatusOr<Page*> Allocate(int count);

  // Write writes any dirty pages to disk.
  Status Write();

  // WriteMeta writes the meta to the disk.
  Status WriteMeta();

  // GetPageHelper returns a reference to the page with a given id.
  // If page has been written to then a temporary buffered page is returned.
  Page* GetPageHelper(pgid id);

  // forEachPage iterates over every page within a given page and executes a
  // function.
  void ForEachPage(pgid id, int depth, function<void(Page*, int)> fn);
};

// BucketMeta represents the on-file representation of a bucket. This
// is stored as the "value" of a bucket key. If the bucket is small enough, then
// its root page can be stored inline in the "value", after the bucket header.
// In the case of inline buckets, the "root" will be 0.
class BucketMeta {
 private:
  friend class Bucket;

  // page id of the bucket's root-level page
  pgid root_;
  // monotonically incrementing, used by NextSequence()
  uint64_t sequence_;
};

// BucketStats records statistics about resources used by a bucket.
struct BucketStats {
 public:
  // Page count statistics.
  int branch_page_n;      // number of logical branch pages
  int branch_overflow_n;  // number of physical branch overflow pages
  int leaf_page_n;        // number of logical leaf pages
  int leaf_overflow_n;    // number of physical leaf overflow pages

  // Tree statistics.
  int key_n;  // number of keys/value pairs
  int depth;  // number of levels in B+tree

  // Page size utilization.
  int branch_alloc;   // bytes allocated for physical branch pages
  int branch_in_use;  // bytes actually used for branch data
  int leaf_alloc;     // bytes allocated for physical leaf pages
  int leaf_in_use;    // bytes actually used for leaf data

  // Bucket statistics
  int bucket_n;         // total number of buckets including the top bucket
  int inline_bucket_n;  // total number on inlined buckets
  // bytes used for inlined buckets (also accounted for in LeafInuse)
  int inline_bucket_in_use;

  void Add(const BucketStats& other) {
    branch_page_n += other.branch_page_n;
    branch_overflow_n += other.branch_overflow_n;
    leaf_page_n += other.leaf_page_n;
    leaf_overflow_n += other.leaf_overflow_n;

    key_n += other.key_n;
    if (depth < other.depth) {
      depth = other.depth;
    }

    branch_alloc += other.branch_alloc;
    branch_in_use += other.branch_in_use;
    leaf_alloc += other.leaf_alloc;
    leaf_in_use += other.leaf_in_use;

    bucket_n += other.bucket_n;
    inline_bucket_n += other.inline_bucket_n;
    inline_bucket_in_use += other.inline_bucket_in_use;
  }
};

// inode represents an internal node inside of a node.
// It can be used to point to elements in a page or point
// to an element which hasn't been added to a page yet.
struct inode {
 public:
  uint32_t flags;
  pgid page_id;
  bytes key;
  bytes value;
};

// Node represents an in-memory, deserialized page.
class Node {
 public:
  Node(bool is_leaf) : is_leaf_(is_leaf) {}

 private:
  friend class Bucket;

  Bucket* bucket_;
  bool is_leaf_;
  bool unbalanced_;
  bool spilled_;
  bytes key_;
  pgid pgid_;
  Node* parent;
  vector<Node*> children_;
  vector<inode> inodes_;

  // Root returns the top-level node this node is attached to.
  Node* Root() const;

  // MinKeys returns the minimum number of inodes this node should have.
  int MinKeys();

  // Size returns the size of the node after serialization.
  int Size() const;

  // SizeLessThan returns true if the node is less than a given size.
  // This is an optimization to avoid calculating a large node when we only need
  // to know if it fits inside a certain page size.
  bool SizeLessThan(int v) const;

  // pageElementSize returns the size of each page element based on the type of
  // node.
  int PageElementSize() const;

  // ChildAt returns the child node at a given index.
  Node* ChildAt(int index) const;

  // ChildIndex returns the index of a given child node.
  int ChildIndex(Node* child) const;

  // NumChildren returns the number of children.
  int NumChildren() const { return inodes_.size(); }

  // NextSibling returns the next node with the same parent.
  Node* NextSibling() const;

  // PrevSibling returns the previous node with the same parent.
  Node* PrevSibling() const;

  // Put inserts a key/value.
  void Put(bytes old_key, bytes new_key, bytes value, pgid id, uint32_t flags);

  // Del removes a key from the node.
  void Del(bytes key);

  // Read initializes the node from a page.
  void Read(Page* page);

  // Write writes the items onto one or more pages.
  void Write(Page* page);

  // Split breaks up a node into multiple smaller nodes, if appropriate.
  // This should only be called from the spill() function.
  vector<Node*> Split(int page_size);

  // SplitTwo breaks up a node into two smaller nodes, if appropriate.
  // This should only be called from the split() function.
  std::pair<Node*, Node*> SplitTwo(int page_size);

  struct SplitIndexType {
    int index;
    int size;
  };
  // SplitIndex finds the position where a page will fill a given threshold.
  // It returns the index as well as the size of the first page.
  // This is only be called from split().
  // TODO(explodes): inline?
  SplitIndexType SplitIndex(int threshold);

  // Spill writes the nodes to dirty pages and splits nodes as it goes.
  // Returns an error if dirty pages cannot be allocated.
  Status Spill();

  // Rebalance attempts to combine the node with sibling nodes if the node fill
  // size is below a threshold or if there are not enough keys.
  void Rebalance();

  // Removes a node from the list of in-memory children.
  // This does not affect the inodes.
  void RemoveChild(Node* target);

  // Dereference causes the node to copy all its inode key/value references to
  // heap memory. This is required when the mmap is reallocated so inodes are
  // not pointing to stale data.
  void Dereference();

  // Free adds the node's underlying page to the freelist.
  void Free();
};

// ElemRef represents a reference to an element on a given page/node.
class ElemRef {
 public:
  ElemRef(Page* page, Node* node, int index)
      : page_(page), node_(node), index_(index) {}

  // IsLeaf returns whether the ref is pointing at a leaf page/node.
  bool IsLeaf() const {
    // TODO(explodes): impl
    return false;
  }

  // Count returns the number of inodes or page elements.
  int Count() const {
    // TODO(explodes): impl
    return 0;
  }

 private:
  Page* page_;
  Node* node_;
  int index_;
};

struct KeyValue {
  bytes key;
  bytes value;
};

struct KeyValueFlags {
  bytes key;
  bytes value;
  uint32_t flags;

  inline bool IsLeaf() const {
    return (flags & kBucketLeafFlag) == kBucketLeafFlag;
  }
};

// Cursor represents an iterator that can traverse over all key/value pairs in a
// bucket in sorted order. Cursors see nested buckets with value == nil. Cursors
// can be obtained from a transaction and are valid as long as the transaction
// is open.
//
// Keys and values returned from the cursor are only valid for the life of the
// transaction.
//
// Changing data while traversing with a cursor may cause it to be invalidated
// and return unexpected keys and/or values. You must reposition your cursor
// after mutating data.
class Cursor {
 public:
  Cursor(Bucket* bucket) : bucket_(bucket), stack_() {}

  // GetBucket returns the bucket that this cursor was created from.
  Bucket* GetBucket() const { return bucket_; }

  // First moves the cursor to the first item in the bucket and returns its key
  // and value.
  // If the bucket is empty then a nil key and value are returned.
  // The returned key and value are only valid for the life of the transaction.
  KeyValue First();

  // Last moves the cursor to the last item in the bucket and returns its key
  // and value.
  // If the bucket is empty then a nil key and value are returned.
  // The returned key and value are only valid for the life of the transaction.
  KeyValue Last();

  // Next moves the cursor to the next item in the bucket and returns its key
  // and value. If the cursor is at the end of the bucket then a nil key and
  // value are returned. The returned key and value are only valid for the life
  // of the transaction.
  KeyValue Next();

  // Prev moves the cursor to the previous item in the bucket and returns its
  // key and value. If the cursor is at the beginning of the bucket then a nil
  // key and value are returned. The returned key and value are only valid for
  // the life of the transaction.
  KeyValue Prev();

  // Seek moves the cursor to a given key and returns it.
  // If the key does not exist then the next key is used. If no keys
  // follow, a nil key is returned.
  // The returned key and value are only valid for the life of the transaction.
  KeyValue Seek(bytes seek);

  // Delete removes the current key/value under the cursor from the bucket.
  // Delete fails if current key/value is a bucket or if the transaction is not
  // writable.
  Status Delete();

 private:
  friend class Bucket;

  Bucket* bucket_;
  vector<ElemRef> stack_;

  // SeekHelper moves the cursor to a given key and returns it.
  // If the key does not exist then the next key is used.
  KeyValueFlags SeekHelper(bytes seek);

  // FirstHelper moves the cursor to the first leaf element under the last page
  // in the stack.
  void FirstHelper();

  // LastHelper moves the cursor to the last leaf element under the last page in
  // the stack.
  void LastHelper();

  // NextHelper moves to the next leaf element and returns the key and value.
  // If the cursor is at the last leaf element then it stays there and returns
  // nil.
  optional<KeyValueFlags> NextHelper();

  // Search recursively performs a binary search against a given page/node until
  // it finds a given key.
  void Search(bytes key, pgid id);
  void SearchNode(bytes key, Node* node);
  void SearchPage(bytes key, Page* page);

  // NSearch searches the leaf node on the top of the stack for a key.
  void NSearch(bytes key);

  // CurrentKeyValue returns the key and value of the current leaf element.
  KeyValueFlags CurrentKeyValue();

  // CurrentNode returns the node that the cursor is currently positioned on.
  Node* CurrentNode();
};

// Bucket represents a collection of key/value pairs inside the database.
class Bucket {
 public:
  // Creates a zero-value, unusable bucket.
  Bucket() {}

  Bucket(Tx* tx)
      : bucket_meta_(),
        tx_(tx),
        buckets_(),
        page_(nullptr),
        root_node_(nullptr),
        nodes_(),
        fill_percent_(kDefaultFillPercent) {}

  // Sets the threshold for filling nodes when they split. By default,
  // the bucket will fill to 50% but it can be useful to increase this
  // amount if you know that your write workloads are mostly append-only.
  //
  // This is non-persisted across transactions so it must be set in every Tx.
  void SetFillPercent(double fill_percent) { fill_percent_ = fill_percent; }

  // Tx returns the tx of the bucket.
  Tx* GetTx() { return tx_; }

  // Root returns the root of the bucket.
  pgid GetRoot() const { return bucket_meta_.root_; }

  // IsWriteable returns whether the bucket is writable.
  bool IsWriteable() const { return tx_->IsWriteable(); }

  // Cursor creates a cursor associated with the bucket.
  // The cursor is only valid as long as the transaction is open.
  // Do not use a cursor after the transaction is closed.
  unique_ptr<Cursor> CreateCursor();

  // NestedBucket retrieves a nested bucket by name.
  // Returns nil if the bucket does not exist.
  // The bucket instance is only valid for the lifetime of the transaction.
  Bucket* NestedBucket(bytes name);

  // CreateBucket creates a new bucket at the given key and returns the new
  // bucket. Returns an error if the key already exists, if the bucket name is
  // blank, or if the bucket name is too long. The bucket instance is only valid
  // for the lifetime of the transaction.
  StatusOr<Bucket*> CreateBucket(bytes key);

  // CreateBucketIfNotExists creates a new bucket if it doesn't already exist
  // and returns a reference to it. Returns an error if the bucket name is
  // blank, or if the bucket name is too long. The bucket instance is only valid
  // for the lifetime of the transaction.
  StatusOr<Bucket*> CreateBucketIfNotExists(bytes key);

  // DeleteBucket deletes a bucket at the given key.
  // Returns an error if the bucket does not exists, or if the key represents a
  // non-bucket value.
  Status DeleteBucket(bytes key);

  // Get retrieves the value for a key in the bucket.
  // Returns a nil value if the key does not exist or if the key is a nested
  // bucket. The returned value is only valid for the life of the transaction.
  optional<bytes> Get(bytes key);

  // Put sets the value for a key in the bucket.
  // If the key exist then its previous value will be overwritten.
  // Supplied value must remain valid for the life of the transaction.
  // Returns an error if the bucket was created from a read-only transaction, if
  // the key is blank, if the key is too large, or if the value is too large.
  Status Put(bytes key, bytes value);

  // Delete removes a key from the bucket.
  // If the key does not exist then nothing is done and a nil error is returned.
  // Returns an error if the bucket was created from a read-only transaction.
  Status Delete(bytes key);

  // Sequence returns the current integer for the bucket without incrementing
  // it.
  uint64_t Sequence() const { return bucket_meta_.sequence_; }

  // SetSequence updates the sequence number for the bucket.
  Status SetSequence(uint64_t sequence);

  // NextSequence returns an autoincrementing integer for the bucket.
  StatusOr<uint64_t> NextSequence();

  // ForEach executes a function for each key/value pair in a bucket.
  // If the provided function returns an error then the iteration is stopped and
  // the error is returned to the caller. The provided function must not modify
  // the bucket; this will result in undefined behavior.
  Status ForEach(function<Status(const bytes& key, bytes* value)> const& fn);

  // Stats returns stats on a bucket.
  BucketStats Stats();

 private:
  BucketMeta bucket_meta_;
  // the associated transaction
  Tx* tx_;
  // subbucket cache
  unordered_map<bytes, Bucket*> buckets_;
  // inline page reference
  Page* page_;
  // materialized node for the root page.
  Node* root_node_;
  // node cache
  unordered_map<pgid, Node*> nodes_;
  // See Bucket::SetFillPercent.
  double fill_percent_;

  // Helper method that re-interprets a sub-bucket value
  // from a parent into a Bucket
  unique_ptr<Bucket> OpenBucket(bytes value);

  // TODO(explodes): INLINE
  // ForEachPage iterates over every page in a bucket, including inline pages.
  void ForEachPage(function<void(Page*, int)> fn);

  // TODO(explodes): INLINE
  // ForEachPageNode iterates over every page (or node) in a bucket.
  // This also includes inline pages.
  void ForEachPageNode(function<void(Page*, Node*, int)> fn);

  // TODO(explodes): INLINE
  void ForEachPageNodeHelper(pgid id, int depth,
                             function<void(Page*, Node*, int)> fn);

  // Spill writes all the nodes for this bucket to dirty pages.
  Status Spill();

  // Inlineable returns true if a bucket is small enough to be written inline
  // and if it contains no subbuckets. Otherwise returns false.
  bool Inlineable();

  // Returns the maximum total size of a bucket to make it a candidate for
  // inlining.
  int MaxInlineBucketSize();

  // Write allocates and writes a bucket to a byte slice.
  bytes Write();

  // Rebalance attempts to balance all nodes.
  void Rebalance();

  // GetNode gets or creates a node from a page and associates it with a given
  // parent.
  Node* GetNode(pgid id, Node* parent);

  // Free recursively frees all pages in the bucket.
  void Free();

  // Dereference removes all references to the old mmap.
  void Dereference();

  struct PageNode {
    Page* page;
    Node* node;
  };
  // GetPageNode returns the in-memory node, if it exists.
  // Otherwise returns the underlying page.
  PageNode GetPageNode(pgid id);
};

}  // namespace bolt

#endif  // DB_BOLT_BUCKET_H