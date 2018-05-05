#ifndef HOIST_SYNC_WAITGROUP_H
#define HOIST_SYNC_WAITGROUP_H

#include <condition_variable>
#include <mutex>

class WaitGroup final {
 public:
  WaitGroup() : count_(0) {}

  WaitGroup(const WaitGroup&) = delete;
  WaitGroup& operator=(const WaitGroup&) = delete;

  void add(const int amount);
  void done();
  void wait();

 private:
  std::mutex mtx_;
  std::condition_variable cond_;
  int count_;
};

#endif