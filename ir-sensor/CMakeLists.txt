set(target_name ir-sensor)

set(src_files
  "ir-sensor/controller-definitions.hpp"
  "ir-sensor/controller.hpp"
  "ir-sensor/gpio.hpp"
  "ir-sensor/ir-sensor.cpp")

add_executable(${target_name} ${src_files})
target_link_libraries(${target_name}
  compiler_features
  bcm2835)