#ifndef NET_SPACEFIGHT_MATHS_H
#define NET_SPACEFIGHT_MATHS_H

#define M_PI 3.14159265358979323846

// convert degrees to radians
constexpr float degToRad(float deg) { return deg * M_PI / 180.0f; }

// convert radians to degress
constexpr float radToDeg(float rad) { return rad * 180.0f / M_PI; }

#endif