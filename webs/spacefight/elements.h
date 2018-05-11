#ifndef WEBS_SPACEFIGHT_SHIP_H
#define WEBS_SPACEFIGHT_SHIP_H

#include "webs/spacefight/maths.h"

namespace spacefight {

namespace settings {
static constexpr std::chrono::milliseconds world_update_interval(26);
static constexpr std::chrono::milliseconds game_update_interval(26);
}  // namespace settings

namespace world {
static constexpr int width = 800;
static constexpr int height = 600;
}  // namespace world

namespace ships {
static constexpr float size = 50.0f;
static constexpr float thrust = 5000.0f;
static constexpr float max_vel = 50.0f;
static constexpr float max_acc = 10.0f;
static constexpr float rotate_speed = degToRad(180);
static constexpr float fire_rate = 0.66f;
static constexpr float new_invincibility_time = 3.5f;
static constexpr float respawn_time = 4.0f;
}  // namespace ships

namespace bullets {
static constexpr float size = 10.0f;
static constexpr float vel = 320.0f;
static constexpr float lifespan = 2;
}  // namespace bullets

namespace explosions {
static constexpr float size = 50.0f;
static constexpr float lifespan = 4;
}  // namespace explosions

}  // namespace spacefight

#endif