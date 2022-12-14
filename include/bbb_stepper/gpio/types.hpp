#pragma once
#include <chrono>
#include <iostream>
#define TIME_IT(process)                                                       \
  do {                                                                         \
    auto begin = std::chrono::system_clock::now();                             \
    process;                                                                   \
    auto end = std::chrono::system_clock::now();                               \
    printf("%s perf: %d [mks]\n", #process,                                    \
           std::chrono::duration_cast<std::chrono::microseconds>(end - begin)  \
               .count());                                                      \
  } while (0);

namespace beaglebone {
namespace gpio {
/**
 * @brief Possible GPIO pin values
 *
 */
enum GPIO_VALUE { GPIO_VALUE_LOW = 0, GPIO_VALUE_HIGH };
/**
 * @brief Possible GPIO pin directions
 *
 */
enum GPIO_DIR { GPIO_DIR_IN = 0, GPIO_DIR_OUT = 1 };
/**
 * @brief BeagleBone GPIO pin object
 *
 */
struct GPIOPin {
  GPIOPin() : port(0), pin(0) {}
  GPIOPin(char port, char pin) {
    this->port = port;
    this->pin = pin;
  }
  bool operator==(const GPIOPin &gpio) const {
    return ((GPIOPin::pin == gpio.pin) && (GPIOPin::port == gpio.port)) ? true
                                                                        : false;
  }
  bool operator!=(const GPIOPin &gpio) const {
    return ((GPIOPin::pin == gpio.pin) && (GPIOPin::port == gpio.port)) ? false
                                                                        : true;
  }
  /**
   * @brief Convert GPIOPin object to string
   *
   * @return std::string
   */
  std::string to_string() {
    return std::string("P") + std::to_string(port) + std::string(".") +
           std::to_string(pin);
  }
  char port; // GPIO Pins Shield number(P8 or P9)
  char pin;  // GPIO Pin number of corresponding Shield
};
} // namespace gpio
} // namespace beaglebone
