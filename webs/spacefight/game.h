#ifndef WEBS_SPACEFIGHT_GAME_H
#define WEBS_SPACEFIGHT_GAME_H

#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include "hoist/clock.h"
#include "proto/spacefight/spacefight.pb.h"

namespace spacefight {

class Game final {
 public:
  Game() : Game(std::make_shared<Hoist::SystemClock>()) {}
  explicit Game(std::shared_ptr<Hoist::Clock> clock)
      : started_(false),
        bullet_id_(0),
        player_id_(0),
        last_update_(0),
        clock_(clock) {}

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  void start();
  void end();

  void apply(const std::string& token, const PlayerInput* const input);
  void getWorld(World* world);

  void update();

 private:
  struct PlayerState {
    PlayerInput input;
    float fire_delay;
  };
  mutable std::shared_timed_mutex mutex_;
  std::shared_ptr<Hoist::Clock> clock_;
  World world_;
  std::unordered_map<std::string, Player*> tokens_;
  std::unordered_map<Player*, PlayerState> player_states_;
  Hoist::nanos_t last_update_;
  bool started_;
  int64_t bullet_id_;
  int64_t player_id_;
  std::thread update_thread_;
};

}  // namespace spacefight

#endif