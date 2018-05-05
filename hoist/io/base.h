#ifndef HOIST_IO_BASE_H
#define HOIST_IO_BASE_H

#include "hoist/hoist.h"
#include "stddef.h"

class Writer {
 public:
  // write writes from the buffer into this writer
  virtual Hoist::Result<size_t> write(void *buf, size_t len) = 0;
  ~Writer() {}
};

class Reader {
 public:
  // read reads from the buffer into this reader
  virtual Hoist::Result<size_t> read(void *buf, size_t len) = 0;
  ~Reader() {}
};

#endif