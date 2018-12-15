#ifndef DB_BOLT_CONSTANTS_H
#define DB_BOLT_CONSTANTS_H

#include <cstdint>
#include "db/bolt/arch.h"

namespace bolt {

#ifndef BOLT_ARCH
#define BOLT_ARCH BOLT_ARCH_386
#endif  // BOLT_ARCH

// Define the architecture-dependent constants used for bolt.
//
// kMaxMapSize: Represents the largest mmap size supported by Bolt.
// kMaxAllocSize: The size used when creating array pointers.
// kBrokenUnaligned: Are unaligned load/stores broken on this arch?

#if BOLT_ARCH == BOLT_ARCH_386
const int64_t kMaxMapSize{0x7FFFFFFF};  // 2GB
const int64_t kMaxAllocSize{0xFFFFFFF};
const bool kBrokenUnaligned{false};

#elif BOLT_ARCH == BOLT_ARCH_AMD64
const int64_t kMaxMapSize{0xFFFFFFFFFFFF};  // 256 TB
const int64_t kMaxAllocSize{0x7FFFFFFF};
const bool kBrokenUnaligned{false};
#endif

}  // namespace bolt

#endif  // DB_BOLT_CONSTANTS_H