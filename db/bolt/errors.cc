#include "db/bolt/errors.h"

namespace bolt {

const Status kErrTxNotWritable =
    Status(error::FAILED_PRECONDITION, "Database is in read-only mode.");

const Status kErrTxClosed =
    Status(error::FAILED_PRECONDITION, "Transaction is closed.");

const Status kErrBucketNameRequired =
    Status(error::INVALID_ARGUMENT, "Bucket name is required.");

const Status kErrBucketExists =
    Status(error::ALREADY_EXISTS, "Bucket already exists.");

const Status kErrIncompatibleValue =
    Status(error::FAILED_PRECONDITION, "Incompatible value.");

const Status kErrBucketNotFound = Status(error::NOT_FOUND, "Bucket not found.");

const Status kErrKeyRequired =
    Status(error::INVALID_ARGUMENT, "Key is required.");

const Status kErrKeyTooLarge =
    Status(error::INVALID_ARGUMENT, "Key too large.");

const Status kErrValueTooLarge =
    Status(error::INVALID_ARGUMENT, "Value too large.");

}  // namespace bolt