#ifndef HOIST_LIKELY_H
#define HOIST_LIKELY_H

#include "hoist/portability.h"

#if HOIST_GCC_VERSION(3, 1) || HOIST_CLANG_BUILTIN(__builtin_expect)
#define LIKELY(condition) __builtin_expect(static_cast<bool>(condition), 1)
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)
#else
#define LIKELY(condition) (condition)
#define UNLIKELY(condition) (condition)
#endif

#endif