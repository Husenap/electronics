#pragma once

#include <cstdint>
#include <string_view>

struct Controller {
  struct Pulse {
    int64_t pulse, space;
  };
  // name of the controller
  std::string_view name;
  // number of bits in the scancodes
  uint16_t bits = 32;
  // relative error tolerance 30 = 30%
  uint8_t eps = 30;
  // absolute error tolerance in microseconds
  int64_t aeps = 100;

  // header  - the initial pulse and space
  Pulse header;
  // one - the pulse and space representing a one
  Pulse one;
  // zero - the pulse and space representing a zero
  Pulse zero;

  // repeat - the pulse and space for repeating a signal
  Pulse repeat;

  // gap - a long pulse after
  int64_t gap;

  bool isWithinRange(int64_t input, int64_t target) const {
    const int64_t error = std::max(target * eps / 100, aeps);
    return input >= target - error && input <= target + error;
  }
  bool match(Pulse input, Pulse target) const {
    return isWithinRange(input.pulse, target.pulse) && isWithinRange(input.space, target.space);
  }
};
