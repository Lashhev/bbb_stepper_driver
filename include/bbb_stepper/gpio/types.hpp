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

enum GPIO_VALUE { GPIO_VALUE_LOW = 0, GPIO_VALUE_HIGH };
enum GPIO_DIR { GPIO_DIR_IN = 0, GPIO_DIR_OUT = 1 };
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
  char port;
  char pin;
};
} // namespace gpio
} // namespace beaglebone