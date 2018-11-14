#ifndef HOIST_LOGGING_H
#define HOIST_LOGGING_H

#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <cmath>
#include <mutex>

static constexpr char ERROR_TAG[] = " ERROR ";
static constexpr char WARN_TAG[] = " WARN  ";
static constexpr char INFO_TAG[] = " INFO  ";
static constexpr char DEBUG_TAG[] = " DEBUG ";

static std::mutex log_mutex;

#define NO_LOG 0
#define ERROR_LEVEL 1
#define WARN_LEVEL 2
#define INFO_LEVEL 3
#define DEBUG_LEVEL 4

#ifndef LOG_LEVEL
#define LOG_LEVEL WARN_LEVEL
#endif

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#define LOG_FUNC(tag, x)                                            \
  do {                                                              \
    char buffer[24];                                                \
    std::unique_lock<std::mutex> lock(log_mutex);                   \
    std::cerr << timenow(buffer) << (tag) << " " << __FILE__ << ":" \
              << __LINE__ << "\t" << x << std::endl;                \
  } while (false)

#if LOG_LEVEL >= ERROR_LEVEL
#define ELOG(x) LOG_FUNC(ERROR_TAG, x)
#define ELOG_IF(condition, x) \
  if (condition) ELOG(x)
#else
#define ELOG(x)
#define ELOG_IF(condition, x)
#endif

#if LOG_LEVEL >= WARN_LEVEL
#define WLOG(x) LOG_FUNC(WARN_TAG, x)
#define WLOG_IF(condition, x) \
  if (condition) WLOG(x)
#else
#define WLOG(x)
#define WLOG_IF(condition, x)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define ILOG(x) LOG_FUNC(INFO_TAG, x)
#define ILOG_IF(condition, x) \
  if (condition) ILOG(x)
#else
#define ILOG(x)
#define ILOG_IF(condition, x)
#endif

#if LOG_LEVEL >= DEBUG_LEVEL
#define DLOG(x) LOG_FUNC(DEBUG_TAG, x)
#define DLOG_IF(condition, x) \
  if (condition) DLOG(x)
#define PRINT(x) DLOG((#x) << " " << (x))
#else
#define DLOG(x)
#define DLOG_IF(condition, x)
#define PRINT(x)
#endif

#include <iostream>

static inline char* timenow(char* buffer) {
  int millisec;
  struct tm* tm_info;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  millisec = lrint(tv.tv_usec / 1000.0);  // Round to nearest millisec
  if (millisec >= 1000) {  // Allow for rounding up to nearest second
    millisec -= 1000;
    tv.tv_sec++;
  }

  tm_info = localtime(&tv.tv_sec);

  // 0         1         2
  // 012345678901234567890123
  // 2018-05-04 18:32:49.777x // x = \0
  strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", tm_info);
  snprintf(&buffer[19], 5, ".%03d", millisec);

  return buffer;
}

#endif