#include "hoist/logging.h"

#include <map>
#include <thread>

static std::size_t next_thread_index = 0;
static std::map<std::thread::id, std::size_t> thread_ids;

size_t FriendlyThreadId() {
  const std::thread::id id = std::this_thread::get_id();
  if (thread_ids.find(id) == thread_ids.end()) {
    thread_ids[id] = next_thread_index++;
  }
  return thread_ids[id];
}

char* LogTimeNow(char* buffer) {
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

void InitLogging() {
  // Capture the main thread as Thread ID 0.
  FriendlyThreadId();
  DLOG("Logging initialized.");
}