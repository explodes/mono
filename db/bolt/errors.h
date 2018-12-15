#ifndef DB_BOLT_ERRORS_H
#define DB_BOLT_ERRORS_H

#include "db/bolt/status.h"

namespace bolt {

// kErrTxNotWritable is returned when performing a write operation on a
// read-only transaction.
static const Status kErrTxNotWritable;

// kErrTxClosed is returned when committing or rolling back a transaction
// that has already been committed or rolled back.
static const Status kErrTxClosed;

// kErrBucketNameRequired is returned when creating a bucket with a blank name.
static const Status kErrBucketNameRequired;

// kErrBucketExists is returned when creating a bucket that already exists.
static const Status kErrBucketExists;

// kErrIncompatibleValue is returned when trying create or delete a bucket
// on an existing non-bucket key or when trying to create or delete a
// non-bucket key on an existing bucket key.
static const Status kErrIncompatibleValue;

// kErrBucketNotFound is returned when trying to access a bucket that has
// not been created yet.
static const Status kErrBucketNotFound;

// kErrKeyRequired is returned when inserting a zero-length key.
static const Status kErrKeyRequired;

// kErrKeyTooLarge is returned when inserting a key that is larger than kMaxKeySize.
static const Status kErrKeyTooLarge;

// kErrValueTooLarge is returned when inserting a value that is larger than kMaxValueSize.
static const Status kErrValueTooLarge;

}  // namespace bolt

#endif  // DB_BOLT_ERRORS_H