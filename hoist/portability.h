#ifndef HOIST_PORTABILITY_H
#define HOIST_PORTABILITY_H

#if __clang__ && defined __has_builtin
#define HOIST_CLANG_BUILTIN(x) __has_builtin(x)
#else
#define HOIST_CLANG_BUILTIN(x) 0
#endif

#if !defined __GNUC_MINOR__ || defined __INTEL_COMPILER ||           \
    defined __SUNPRO_C || defined __SUNPRO_CC || defined __llvm__ || \
    defined __clang__
#define HOIST_GCC_VERSION(major, minor) 0
#else
#define HOIST_GCC_VERSION(major, minor) \
  (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
#endif

#endif