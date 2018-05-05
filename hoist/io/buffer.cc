#include "buffer.h"
#include <sstream>

Hoist::Result<size_t> Buffer::write(void* buf, size_t len) {
  buf_.append(static_cast<char*>(buf), len);
  return Hoist::Status(Hoist::StatusCode::OK);
}

void Buffer::reset() { buf_.clear(); }

std::string Buffer::string() const {
  std::stringstream ss;

  for (char c : buf_) {
    ss << c;
  }

  return ss.str();
}