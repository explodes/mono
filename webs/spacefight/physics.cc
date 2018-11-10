#include "webs/spacefight/physics.h"

#include <cmath>

namespace spacefight {
namespace phys {

float angle(const game::Vector& v) { return atan2(v.y(), v.x()); }

float magnitude(const game::Vector* const v) { return hypot(v->x(), v->y()); }

void reset(game::Vector* v) { set(v, 0, 0); }

void set(game::Vector* v, const float x, const float y) {
  v->set_x(x);
  v->set_y(y);
}

void set(game::Vector* v, const game::Vector& u) {
  v->set_x(u.x());
  v->set_y(u.y());
}

void add(game::Vector* v, const game::Vector* const add) {
  v->set_x(v->x() + add->x());
  v->set_y(v->y() + add->y());
}

void add(game::Vector* v, const float x, const float y) {
  v->set_x(v->x() + x);
  v->set_y(v->y() + y);
}

void scale(game::Vector* v, const float p) {
  v->set_x(v->x() * p);
  v->set_y(v->y() * p);
}

void rotate(game::Vector* v, const float radians) {
  float s = sin(radians);
  float c = cos(radians);
  float x = v->x();
  float y = v->y();
  v->set_x(x * c - y * s);
  v->set_y(x * s + y * c);
}

void clampMagnitude(game::Vector* v, const float max) {
  float length = magnitude(v);
  if (length == 0) {
    return;
  } else if (length > max) {
    scale(v, max / length);
  }
}

// Physics functions

void reset(game::Physics* p) { set(p, 0, 0, 0, 0, 0, 0); }

void set(game::Physics* p, const float x, const float y) {
  set(p, x, y, 0, 0, 0, 0);
}
// set multiple properties of a physics
void set(game::Physics* p, const float x, const float y, const float dx,
         const float dy) {
  set(p, x, y, dx, dy, 0, 0);
}
// set multiple properties of a physics
void set(game::Physics* p, const float x, const float y, const float dx,
         const float dy, const float ddx, const float ddy) {
  set(p->mutable_pos(), x, y);
  set(p->mutable_vel(), dx, dy);
  set(p->mutable_acc(), ddx, ddy);
}

void update(game::Physics* p, const float dt) {
  add(p->mutable_pos(), p->vel().x() * dt, p->vel().y() * dt);
  add(p->mutable_vel(), p->acc().x() * dt, p->acc().y() * dt);
}

// Body functions

void update(game::Body* b, const float dt) { update(b->mutable_phys(), dt); }

bool willCollide(const game::Body& a, const game::Body& b, const float dt) {
  // future a-rectangle
  float ax1 = a.phys().pos().x() + a.phys().vel().x() * dt;
  float ay1 = a.phys().pos().y() + a.phys().vel().y() * dt;
  float ax2 = ax1 + a.size().x();
  float ay2 = ay1 + a.size().y();

  // future b-rectangle
  float bx1 = b.phys().pos().x() + b.phys().vel().x() * dt;
  float by1 = b.phys().pos().y() + b.phys().vel().y() * dt;
  float bx2 = bx1 + b.size().x();
  float by2 = by1 + b.size().y();

  // collision detection
  if (ax1 > bx2 || bx1 > ax2) {
    return false;
  }
  if (ay1 > by2 || by1 > ay2) {
    return false;
  }
  return true;
}

bool doesCollide(const game::Body& a, const game::Body& b) {
  float ax1 = a.phys().pos().x() + a.phys().vel().x();
  float ay1 = a.phys().pos().y() + a.phys().vel().y();
  float ax2 = ax1 + a.size().x();
  float ay2 = ay1 + a.size().y();

  float bx1 = b.phys().pos().x() + b.phys().vel().x();
  float by1 = b.phys().pos().y() + b.phys().vel().y();
  float bx2 = bx1 + b.size().x();
  float by2 = by1 + b.size().y();

  // collision detection
  if (ax1 > bx2 || bx1 > ax2) {
    return false;
  }
  if (ay1 > by2 || by1 > ay2) {
    return false;
  }
  return true;
}

}  // namespace phys
}  // namespace spacefight