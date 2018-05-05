#include "waitgroup.h"

void WaitGroup::add(const int amount) {
  std::lock_guard<std::mutex> lock{mtx_};
  count_ += amount;
  if (count_ < 0) {
    throw std::runtime_error("WaitGroup count has gone negative");
  }
  if (count_ == 0) {
    cond_.notify_all();
  }
}

void WaitGroup::done() { add(-1); }

void WaitGroup::wait() {
  std::unique_lock<std::mutex> lock{mtx_};
  cond_.wait(lock, [this]() { return count_ == 0; });
}