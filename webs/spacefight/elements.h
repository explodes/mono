#ifndef WEBS_SPACEFIGHT_SHIP_H
#define WEBS_SPACEFIGHT_SHIP_H

#include "webs/spacefight/maths.h"

namespace spacefight {

static constexpr std::chrono::milliseconds world_update_interval(13);
static constexpr std::chrono::milliseconds game_update_interval(13);

namespace phys {
namespace ship {

static const float acc = 5000.0f;
static const float max_vel = 50.0f;
static const float max_acc = 10.0f;
static const float rotate_speed = degToRad(180);
static const float fire_rate = 0.66f;

}  // namespace ship

namespace bullet {

static const float size = 10.0f;
static const float vel = 320.0f;
static const float lifespan = 2;

}  // namespace bullet

}  // namespace phys
}  // namespace spacefight

#endif