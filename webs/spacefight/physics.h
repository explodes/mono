#ifndef WEBS_SPACEFIGHT_PHYSICS_H
#define WEBS_SPACEFIGHT_PHYSICS_H

#include "proto/spacefight/spacefight.pb.h"

namespace spacefight {
namespace phys {

// Vector functions

// get the angle of a vector
float angle(const game::Vector& v);

// get the magnitude of a vector
float magnitude(const game::Vector* const v);

// set all properties of a vector to zero
void reset(game::Vector* v);

// set x/y properties of a vector
void set(game::Vector* v, const float x, const float y);

// set x/y properties of a vector to the x/y properties of another vector
void set(game::Vector* v, const game::Vector& u);

// add another vector to a vector
void add(game::Vector* v, const game::Vector* const add);

// add x/y magnitude to a vector
void add(game::Vector* v, const float x, const float y);

// scale the size of a vector by a percentage
void scale(game::Vector* v, const float p);

// rotate a vector
void rotate(game::Vector* v, const float radians);

// clamps the magnitude of a vector, preserving the original angle
void clampMagnitude(game::Vector* v, const float max);

// Physics functions

// set all properties of a physics to zero
void reset(game::Physics* p);
// set multiple properties of a physics
void set(game::Physics* p, const float x, const float y);
// set multiple properties of a physics
void set(game::Physics* p, const float x, const float y, const float dx,
         const float dy);
// set multiple properties of a physics
void set(game::Physics* p, const float x, const float y, const float dx,
         const float dy, const float ddx, const float ddy);
// apply velocity and acceleration for a given time interval
void update(game::Physics* p, const float dt);

// Body functions

void update(game::Body* b, const float dt);
bool willCollide(const game::Body& a, const game::Body& b, const float dt);
bool doesCollide(const game::Body& a, const game::Body& b);

}  // namespace phys
}  // namespace spacefight

#endif