#ifndef HOIST_MACROS_H
#define HOIST_MACROS_H

#include <errno.h>
#include <string.h>

#define ERRNO_AS_STATUS() Hoist::Status(Hoist::error::UNKNOWN, strerror(errno));

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;

#endif  // HOIST_MACROS_H