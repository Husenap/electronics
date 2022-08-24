#include <bitset>
#include <chrono>
#include <csignal>
#include <iostream>
#include <optional>
#include <string_view>
#include <thread>
#include <vector>

#include "controller-definitions.hpp"
#include "gpio.hpp"

static constexpr unsigned int SensorPin = RPI_GPIO_P1_12;
static constexpr auto         MaxPulse  = std::chrono::seconds(1);

static constexpr Controller controller{Andersson};

void dump(const std::vector<bool>& bits) {
  std::cout << "[ " << std::hex;
  uint64_t bit   = 0;
  uint32_t bytes = 0;
  while (bit < bits.size()) {
    if (bit > 0 && bit % 32 == 0) {
      std::cout << std::hex << "0x" << bytes << ' ';
      bytes = 0;
    }
    bytes |= bits[bit % 32] << (31 - (bit % 32));
    ++bit;
  }
  std::cout << "0x" << bytes << ' ';

  std::cout << "]" << std::dec;
}
Pulse readPulse() {
  return {measureTime([]() { waitForPin(SensorPin, LOW, MaxPulse); }).count(),
          measureTime([]() { waitForPin(SensorPin, HIGH, MaxPulse); }).count()};
}
std::vector<bool> readSignal() {
  std::vector<bool> bits(controller.bits, false);
  for (auto bit : bits) {
    const auto pulse = readPulse();
    bit              = controller.match(pulse, controller.one);
  }
  dump(bits);
  return bits;
}

int main() {
  std::signal(SIGINT, [](int signum) {
    std::cout << "Shutting down" << std::endl;
    bcm2835_close();
    std::exit(signum);
  });

  if (!bcm2835_init()) {
    std::cerr << "Failed to initialize bcm2835 library!" << std::endl;
    return 1;
  }

  bcm2835_gpio_fsel(SensorPin, BCM2835_GPIO_FSEL_INPT);

  std::cout << "Loaded controller: " << controller.name << std::endl;
  std::vector<bool> bits;
  while (true) {
    const auto pulse = readPulse();

    if (controller.match(pulse, controller.header)) {
      std::cout << "[header]";
      bits = readSignal();
      std::cout << std::endl;
    } else if (controller.match(pulse, controller.repeat)) {
      std::cout << "[repeat]";
      dump(bits);
      std::cout << std::endl;
    }
  }
}