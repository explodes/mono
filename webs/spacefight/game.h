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
      : clock_(clock),
        last_update_(0),
        started_(false),
        bullet_id_(0),
        player_id_(0),
        explosion_id_(0) {}

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  void start();
  void end();

  void apply(const PlayerInput* const input);
  void getWorld(World* world);

  void update();

 private:
  struct PlayerState {
    PlayerInput input;
    float fire_delay;
    // countdown for how long a player is new
    float new_countdown;
    // countdown until a player respawns
    float dead_countdown;

    bool isNew() { return new_countdown > 0; }
    bool isDead() { return dead_countdown > 0; }
    void update(float dt) {
      new_countdown -= dt;
      dead_countdown -= dt;
    }
  };
  mutable std::shared_timed_mutex mutex_;
  std::shared_ptr<Hoist::Clock> clock_;
  World world_;
  std::unordered_map<std::string, Player*> tokens_;
  std::unordered_map<Player const*, PlayerState> player_states_;
  Hoist::nanos_t last_update_;
  bool started_;
  int64_t bullet_id_;
  int64_t player_id_;
  int64_t explosion_id_;
  std::thread update_thread_;

  // Status
  void logNumPlayers();

  // Input sequence
  void onQuit(const PlayerInput* const input);
  Player* onNewPlayer(const PlayerInput* const input);

  // Update sequence
  float computeTimeDelta();
  void updateBulletCollisions(float dt);
  void updateShips(float dt);
  void updateBullets(float dt);
  void updateExplosions(float dt);
};

}  // namespace spacefight

#endif