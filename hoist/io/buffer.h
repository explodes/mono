#ifndef HOIST_IO_BUFFER_H
#define HOIST_IO_BUFFER_H

#include <string>
#include "base.h"
#include "stdint.h"

class Buffer : public Writer {
 public:
  Buffer() : buf_() {}
  explicit Buffer(const size_t capacity) : buf_() { buf_.reserve(capacity); }

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  Hoist::Result<size_t> write(void* buf, size_t len) override;

  // reset clears this buffer
  void reset();

  // string creates a string based on the contents of this buffer
  std::string string() const;

 private:
  std::string buf_;
};

#endif