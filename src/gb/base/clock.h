// Copyright (c) 2020 John Pursey
//
// Use of this source code is governed by an MIT-style License that can be found
// in the LICENSE file or at https://opensource.org/licenses/MIT.

#ifndef GB_BASE_CLOCK_H_
#define GB_BASE_CLOCK_H_

#include "absl/time/clock.h"

namespace gb {

class Clock {
 public:
  Clock(const Clock&) = delete;
  Clock& operator=(const Clock&) = delete;
  virtual ~Clock() = default;

  // Returns the current time.
  virtual absl::Time Now() = 0;

  // Sleeps the current thread for the specified duration.
  virtual void SleepFor(absl::Duration duration) = 0;

 protected:
  Clock() = default;
};

class RealtimeClock : public Clock {
 public:
  RealtimeClock() = default;
  ~RealtimeClock() override = default;

  static Clock* GetClock() {
    static RealtimeClock clock;
    return &clock;
  }

  absl::Time Now() override { return absl::Now(); }
  void SleepFor(absl::Duration duration) override { absl::SleepFor(duration); }
};

}  // namespace gb

#endif  // GB_BASE_CLOCK_H_
