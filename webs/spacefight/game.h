#ifndef WEBS_SPACEFIGHT_GAME_H
#define WEBS_SPACEFIGHT_GAME_H

#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include "hoist/clock.h"
#include "proto/spacefight/spacefight.pb.h"

namespace spacefight {

#define WRITE_LOCKED
#define READ_LOCKED

class Game final {
 public:
  Game() : Game(std::make_shared<Hoist::SystemClock>()) {}
  Game(std::shared_ptr<Hoist::Clock> clock, const int numBots = 15)
      : clock_(clock),
        last_update_(0),
        started_(false),
        bullet_id_(0),
        player_id_(0),
        explosion_id_(0) {
    for (int i = 0; i < numBots; i++) {
      std::string name("gunther");
      name.append(std::to_string(i));
      std::string token("token");
      token.append(std::to_string(i));
      PlayerInput* leaked_input = new PlayerInput();
      leaked_input->set_username(name);
      leaked_input->set_token(token);
      createNewAI(leaked_input);
    }
  }

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  WRITE_LOCKED void start();
  WRITE_LOCKED void end();

  WRITE_LOCKED int64_t createNewPlayer(const PlayerInput* const input);

  WRITE_LOCKED void apply(const PlayerInput* const input);
  READ_LOCKED void getWorld(World* world);

  WRITE_LOCKED void update();

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
  struct BotState {
    float time;
  };
  typedef std::unique_lock<std::shared_timed_mutex> WriteLock;
  typedef std::shared_lock<std::shared_timed_mutex> ReadLock;
  mutable std::shared_timed_mutex mutex_;
  std::shared_ptr<Hoist::Clock> clock_;
  World world_;
  std::unordered_map<std::string, Player*> tokens_;
  std::unordered_map<Player const*, PlayerState> player_states_;
  std::unordered_map<Player const*, BotState> bot_states_;
  std::vector<Player*> bots_;
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

  void createNewAI(const PlayerInput* const input);
  Player* createNewPlayerUnlocked(const PlayerInput* const input);
  void applyUnlocked(const PlayerInput* const input);

  // Update sequence
  float computeTimeDelta();
  void updateBulletCollisions(float dt);
  void updateShips(float dt);
  void updateBullets(float dt);
  void updateExplosions(float dt);

  // AI
  void updateAI(float dt);
};

}  // namespace spacefight

#endif