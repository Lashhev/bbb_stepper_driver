#pragma once
#include "bbb_stepper/gpio/types.hpp"
#include <memory>
namespace beaglebone {
namespace gpio {
/**
 * @brief A C++ Wrapper of a BeagleBone GPIO control library.
 * The class provides functionality to control BeagleBone GPIO
 */
class GPIOManager {
protected:
  GPIOManager(/* args */);

public:
  GPIOManager(GPIOManager const &) = delete;
  void operator=(GPIOManager const &) = delete;
  /**
   * @brief Get the GPIOManager pointer
   *
   * @return GPIOManager*
   */
  static GPIOManager *getInstance();
  /**
   * @brief Set the GPIO pin direction
   *
   * @param pin GPIO pin
   * @param dir pin direction (Possible values: GPIO_DIR_IN = 0, GPIO_DIR_OUT =
   * 1)
   * @return true if success
   */
  bool setPinDirection(GPIOPin pin, GPIO_DIR dir);
  /**
   * @brief Set digital value of a GPIO pin
   *
   * @param pin GPIO pin
   * @param value pin value (Possible values: GPIO_VALUE_LOW = 0,
   * GPIO_VALUE_HIGH)
   */
  void digitalWrite(GPIOPin pin, GPIO_VALUE value);
  /**
   * @brief Read GPIO pin value
   *
   * @param pin GPIO pin
   * @return (Possible values: GPIO_VALUE_LOW = 0,
   * GPIO_VALUE_HIGH)
   */
  GPIO_VALUE digitalRead(GPIOPin pin);
  ~GPIOManager();
};
} // namespace gpio
} // namespace beaglebone