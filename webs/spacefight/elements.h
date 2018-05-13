#ifndef WEBS_SPACEFIGHT_SHIP_H
#define WEBS_SPACEFIGHT_SHIP_H

#include "webs/spacefight/maths.h"

namespace spacefight {

namespace settings {
static constexpr std::chrono::milliseconds world_update_interval(26);
static constexpr std::chrono::milliseconds game_update_interval(26);
}  // namespace settings

namespace world {
static constexpr float spawn_radius = 2500;
}  // namespace world

namespace ships {
static constexpr float size = 50.0f;
static constexpr float thrust = 5000.0f;
static constexpr float max_vel = 250.0f;
static constexpr float max_acc = 100.0f;
static constexpr float rotate_speed = degToRad(180);
static constexpr float fire_rate = 0.56f;
static constexpr float new_invincibility_time = 3.5f;
static constexpr float respawn_time = 4.0f;
}  // namespace ships

namespace bullets {
static constexpr float size = 7.0f;
static constexpr float vel = 400.0f;
static constexpr float lifespan = 3;
}  // namespace bullets

namespace explosions {
static constexpr float size = 50.0f;
static constexpr float lifespan = 4;
}  // namespace explosions

}  // namespace spacefight

#endif