#pragma once

extern "C" {
#include <bcm2835/bcm2835.h>
}

template <typename _Rep, typename _Period>
void waitForPin(uint8_t pin, uint8_t state, const std::chrono::duration<_Rep, _Period>& timeout) {
  const auto t0 = std::chrono::high_resolution_clock::now();
  while (bcm2835_gpio_lev(pin) == state &&
         std::chrono::high_resolution_clock::now() - t0 < timeout) {
    std::this_thread::sleep_for(std::chrono::microseconds(5));
  }
}

template <typename Op>
auto measureTime(Op operation) {
  const auto t0 = std::chrono::high_resolution_clock::now();
  operation();
  return std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::high_resolution_clock::now() - t0);
}