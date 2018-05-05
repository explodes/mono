#ifndef WEBS_SPACEFIGHT_COLOR_H
#define WEBS_SPACEFIGHT_COLOR_H

#include <cmath>
#include <cstdint>

namespace spacefight {

inline int64_t argb2int64(int a, int r, int g, int b) {
  return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) |
         ((b & 0xff) << 0);
}

typedef struct {
  double r;  // a fraction between 0 and 1
  double g;  // a fraction between 0 and 1
  double b;  // a fraction between 0 and 1
} rgb;

typedef struct {
  double h;  // angle in degrees
  double s;  // a fraction between 0 and 1
  double v;  // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(rgb in) {
  hsv out;
  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min < in.b ? min : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max > in.b ? max : in.b;

  out.v = max;  // v
  delta = max - min;
  if (delta < 0.00001) {
    out.s = 0;
    out.h = 0;  // undefined, maybe nan?
    return out;
  }
  if (max > 0.0) {  // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);  // s
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, h is undefined
    out.s = 0.0;
    out.h = NAN;  // its now undefined
    return out;
  }
  if (in.r >= max)                  // > is bogus, just keeps compilor happy
    out.h = (in.g - in.b) / delta;  // between yellow & magenta
  else if (in.g >= max)
    out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
  else
    out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

  out.h *= 60.0;  // degrees

  if (out.h < 0.0) out.h += 360.0;

  return out;
}

rgb hsv2rgb(hsv in) {
  double hh, p, q, t, ff;
  long i;
  rgb out;

  if (in.s <= 0.0) {  // < is bogus, just shuts up warnings
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
    case 0:
      out.r = in.v;
      out.g = t;
      out.b = p;
      break;
    case 1:
      out.r = q;
      out.g = in.v;
      out.b = p;
      break;
    case 2:
      out.r = p;
      out.g = in.v;
      out.b = t;
      break;

    case 3:
      out.r = p;
      out.g = q;
      out.b = in.v;
      break;
    case 4:
      out.r = t;
      out.g = p;
      out.b = in.v;
      break;
    case 5:
    default:
      out.r = in.v;
      out.g = p;
      out.b = q;
      break;
  }
  return out;
}

inline int64_t rgb2int64(rgb in) {
  return argb2int64(255, static_cast<int>(255 * in.r),
                    static_cast<int>(255 * in.g), static_cast<int>(255 * in.b));
}

inline int64_t hsv2int64(hsv in) { return rgb2int64(hsv2rgb(in)); }

}  // namespace spacefight

#endif
