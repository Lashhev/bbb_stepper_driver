#pragma once
#include "bbb_stepper/gpio/types.hpp"
#include <memory>
namespace beaglebone {
namespace gpio {

class GPIOManager {
protected:
  GPIOManager(/* args */);

public:
  GPIOManager(GPIOManager const &) = delete;
  void operator=(GPIOManager const &) = delete;

  static GPIOManager *getInstance();
  bool setPinDirection(GPIOPin pin, GPIO_DIR dir);
  void digitalWrite(GPIOPin pin, GPIO_VALUE value);
  GPIO_VALUE digitalRead(GPIOPin pin);
  ~GPIOManager();
};
} // namespace gpio
} // namespace beaglebone