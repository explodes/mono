#ifndef HOIST_ERRORS_H
#define HOIST_ERRORS_H

#include <string>

namespace Hoist {

enum class StatusCode {
  Unknown,
  OK,
  Invalid,
  UnknownError,
};

class Status final {
 public:
  Status(StatusCode code = Hoist::StatusCode::Unknown) : code_(code) {}

  Hoist::StatusCode code() const { return code_; }
  bool ok() const { return code_ == Hoist::StatusCode::OK; }

 private:
  Hoist::StatusCode code_;
};

// Result is the result and status of an action
template <class T>
class Result final {
 public:
  Result(const StatusCode code = StatusCode::Unknown) : status_(code) {}
  Result(const Status s) : status_(s) {}
  Result(T val) : status_(StatusCode::OK), val_(val) {}

  const Hoist::Status status() const { return status_; }
  bool ok() const { return status_.ok(); }

  T& value() { return val_; }
  const T& value() const { return val_; }

 private:
  Hoist::Status status_;
  T val_;
};

}  // namespace Hoist
#endif