#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "hoist/hoist.h"
#include "hoist/sync/waitgroup.h"

static std::mutex outputMutex;

class Action final {
 public:
  Action(WaitGroup &wg) : wg_(wg), id_(++next_id_) {}

  void act() {
    wait();
    write();
    wg_.done();
  }

 private:
  static int next_id_;
  WaitGroup &wg_;
  const int id_;

  void wait() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }

  void write() {
    std::lock_guard<std::mutex> lock{outputMutex};
    std::cout << "Hello from Action thread " << id_ << std::endl;
  }
};

int Action::next_id_ = 0;

void joinAll(std::vector<std::thread> &threads) {
  for (auto &t : threads) {
    t.detach();
  }
}

int main() {
  static const int NUM_THREADS = 100;

  Hoist::Init();

#ifdef DEBUG
  Hoist::waitForInput("waiting for input...");
#endif

  WaitGroup wg;
  wg.add(NUM_THREADS);

  std::vector<std::thread> threads;
  threads.reserve(NUM_THREADS);

  std::vector<Action> actions;
  actions.reserve(NUM_THREADS);

  for (int i = 0; i < NUM_THREADS; i++) {
    Action action(wg);
    actions.push_back(std::move(action));
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    std::thread thread(&Action::act, &actions[i]);
    threads.push_back(std::move(thread));
  }

  std::cout << "Waiting..." << std::endl;
  wg.wait();
  std::cout << "Joining..." << std::endl;
  joinAll(threads);
  std::cout << "Done." << std::endl;

  return 0;
}