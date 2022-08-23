#include <bitset>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string_view>
#include <thread>

#include "controller-definitions.hpp"
#include "gpio.hpp"

static constexpr unsigned int SensorPin = RPI_GPIO_P1_12;
static constexpr auto         MaxPulse  = std::chrono::seconds(1);

void signalHandler(int signum) {
  std::cout << "Shutting down" << std::endl;

  bcm2835_close();

  exit(signum);
}

Controller::Pulse readPulse() {
  return {measureTime([]() { waitForPin(SensorPin, LOW, MaxPulse); }).count(),
          measureTime([]() { waitForPin(SensorPin, HIGH, MaxPulse); }).count()};
}

int main() {
  std::signal(SIGINT, signalHandler);

  if (!bcm2835_init()) {
    std::cerr << "Failed to initialize bcm2835 library!" << std::endl;
    return 1;
  }

  bcm2835_gpio_fsel(SensorPin, BCM2835_GPIO_FSEL_INPT);

  constexpr Controller controller{Andersson};
  std::cout << "Loaded controller: " << controller.name << '\n';
  while (true) {
    const auto pulse = readPulse();
    if (controller.match(pulse, controller.header)) {
      std::cout << "Recieved: header" << '\n';
    } else if (controller.match(pulse, controller.repeat)) {
      std::cout << "Recieved: repeat" << '\n';
    }
    // std::cout << pulseTime.count() << ' ' << spaceTime.count() << '\n';
  }
}
