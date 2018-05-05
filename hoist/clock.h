#ifndef HOIST_CLOCK_H
#define HOIST_CLOCK_H

namespace Hoist {

typedef long long nanos_t;

class Clock {
 public:
  virtual nanos_t nanos() = 0;
  virtual ~Clock(){};
};

class SystemClock final : public Clock {
 public:
  nanos_t nanos() override;
};

}  // namespace Hoist
#endif