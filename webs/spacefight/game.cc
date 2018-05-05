#include "webs/spacefight/game.h"
#include <unistd.h>
#include <chrono>
#include <mutex>
#include "hoist/logging.h"
#include "hoist/math.h"
#include "webs/spacefight/color.h"
#include "webs/spacefight/debug.h"
#include "webs/spacefight/elements.h"
#include "webs/spacefight/physics.h"

namespace spacefight {

template <typename T>
inline T max(T a, T b) {
  return a > b ? a : b;
}

template <typename T>
inline T min(T a, T b) {
  return a < b ? a : b;
}

float nanosToSeconds(Hoist::nanos_t nanos) {
  return static_cast<float>(nanos) / 1e9;
}

void Game::start() {
  std::unique_lock<std::shared_timed_mutex> lock(mutex_);
  DLOG("Game.start()");
  if (started_) {
    ELOG("already started");
    return;
  }
  started_ = true;
  last_update_ = clock_->nanos();

  update_thread_ = std::thread([this]() {
    DLOG("update loop started");
    while (started_) {
      update();
      std::this_thread::sleep_for(game_update_interval);
    }
    DLOG("update loop ended");
  });
}

void Game::end() {
  std::unique_lock<std::shared_timed_mutex> lock(mutex_);
  DLOG("Game.end()");
  if (!started_) {
    ELOG("not started");
    return;
  }
  started_ = false;
  update_thread_.join();
}

void Game::apply(const std::string& token, const PlayerInput* const input) {
  std::unique_lock<std::shared_timed_mutex> lock(mutex_);
  if (!started_) {
    ELOG("game not started, cannot apply");
    return;
  }

  // if player quit, remove player
  if (input->quit()) {
    DLOG("token " << input->token() << " requesting quit");
    auto search = tokens_.find(input->token());
    if (search != tokens_.end()) {
      Player* player = tokens_[input->token()];
      ILOG("player " << player->username() << " quit.");

      // remove player state
      player_states_.erase(player);

      // remove from tokens
      tokens_.erase(input->token());

      // finally remove from the world
      for (int i = 0; i < world_.players_size(); i++) {
        Player* other = world_.mutable_players(i);
        if (player == other) {
          world_.mutable_players()->DeleteSubrange(i, 1);
          i--;
          break;
        }
      }
    }
    DLOG("There " << (world_.players_size() == 1 ? "is" : "are") << " now "
                  << world_.players_size() << " "
                  << (world_.players_size() == 1 ? "player" : "players")
                  << " playing.");
    return;
  }

  // if token does not exist, create ship
  Player* player(nullptr);
  auto search = tokens_.find(input->token());
  if (search == tokens_.end()) {
    DLOG("new player " << input->username());
    // Create a player.
    player = world_.add_players();
    Ship* ship = player->mutable_ship();
    game::Body* body = ship->mutable_body();
    game::Physics* physics = body->mutable_phys();
    player->set_id(++player_id_);
    player->set_username(input->username());
    player->mutable_color()->set_aarrggbb(
        hsv2int64(hsv{Hoist::RNG::roll() * 360.0, 1, 1}));
    phys::set(physics, 400, 300, -1, 0);
    phys::set(body->mutable_size(), 50, 50);
    phys::set(body->mutable_rotation(), physics->vel());
    tokens_[input->token()] = player;
    DLOG("There " << (world_.players_size() == 1 ? "is" : "are") << " now "
                  << world_.players_size() << " "
                  << (world_.players_size() == 1 ? "player" : "players")
                  << " playing.");
  } else {
    player = search->second;
  }

  // save input for updating
  player_states_[player].input.CopyFrom(*input);
}

void Game::getWorld(World* world) {
  std::shared_lock<std::shared_timed_mutex> lock(mutex_);
  if (!started_) {
    ELOG("game not started, cannot getWorld");
    return;
  }
  world->CopyFrom(world_);
}

void Game::update() {
  std::unique_lock<std::shared_timed_mutex> lock(mutex_);
  if (!started_) {
    ELOG("game not started, cannot update");
    return;
  }

  Hoist::nanos_t now = clock_->nanos();
  float dt = nanosToSeconds(now - last_update_);
  last_update_ = now;

  // create explosions
  for (int bi = 0; bi < world_.bullets_size(); bi++) {
    const Bullet& bullet = world_.bullets(bi);
    for (int pi = 0; pi < world_.players_size(); pi++) {
      const Player& player = world_.players(pi);
      if (bullet.player_id() != player.id() &&
          phys::willCollide(player.ship().body(), bullet.body(), dt)) {
        DLOG("player " << player.username() << " was shot!");
        // TODO(explodes): remove player temporarily
        // TODO(explodes): create explosion
      }
    }
  }
  // TODO(explodes): restore ships
  // update ships
  for (int pi = 0; pi < world_.players_size(); pi++) {
    Player* player = world_.mutable_players(pi);
    Ship* ship = player->mutable_ship();
    game::Body* body = ship->mutable_body();
    game::Physics* physics = body->mutable_phys();
    PlayerState& state = player_states_[player];
    if (state.input.thrust()) {
      game::Vector v;
      phys::set(&v, phys::ship::acc * dt, 0);
      phys::rotate(&v, phys::angle(body->rotation()));
      phys::add(physics->mutable_vel(), &v);
      phys::clampMagnitude(physics->mutable_acc(), phys::ship::max_acc);
      phys::set(body->mutable_rotation(), physics->vel());
    } else {
      phys::reset(physics->mutable_vel());
      phys::reset(physics->mutable_acc());
    }
    if (state.input.rotate_left()) {
      phys::rotate(physics->mutable_vel(), -phys::ship::rotate_speed * dt);
      phys::rotate(body->mutable_rotation(), -phys::ship::rotate_speed * dt);
    } else if (state.input.rotate_right()) {
      phys::rotate(physics->mutable_vel(), phys::ship::rotate_speed * dt);
      phys::rotate(body->mutable_rotation(), phys::ship::rotate_speed * dt);
    }
    phys::update(physics, dt);
    phys::clampMagnitude(physics->mutable_vel(), phys::ship::max_vel);
    // fire bullets
    state.fire_delay -= dt;
    if (state.input.fire() && state.fire_delay <= 0) {
      state.fire_delay = phys::ship::fire_rate;
      Bullet* bullet = world_.add_bullets();
      game::Body* bullet_body = bullet->mutable_body();
      game::Physics* bullet_physics = bullet_body->mutable_phys();
      bullet->set_id(++bullet_id_);
      bullet->set_player_id(player->id());
      bullet->set_lifespan(phys::bullet::lifespan);
      bullet->mutable_color()->CopyFrom(player->color());
      // set centered at player position
      phys::set(bullet_physics->mutable_pos(),
                physics->pos().x() + body->size().x() * 0.5,
                physics->pos().y() + body->size().y() * 0.5);
      // set velocity..
      phys::set(bullet_physics->mutable_vel(), phys::bullet::vel, 0);
      // ..angled to face the same direction as the player
      phys::rotate(bullet_physics->mutable_vel(),
                   phys::angle(body->rotation()));
      // Bullet body
      phys::set(bullet_body->mutable_size(), phys::bullet::size,
                phys::bullet::size);
      phys::set(bullet_body->mutable_rotation(), bullet_physics->vel());
    }
  }
  // move bullets
  for (int i = 0; i < world_.bullets_size(); i++) {
    Bullet* bullet = world_.mutable_bullets(i);
    // remove long-lived bullets
    bullet->set_lifespan(bullet->lifespan() - dt);
    if (bullet->lifespan() <= 0) {
      world_.mutable_bullets()->DeleteSubrange(i, 1);
      i--;
      continue;
    }
    // update bullet
    phys::update(bullet->mutable_body()->mutable_phys(), dt);
  }
  // TODO(explodes): respawn players
}

}  // namespace spacefight