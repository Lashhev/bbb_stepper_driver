#include "bbb_stepper/gpio/gpio_manager.hpp"
#include "gpio/BBBiolib.h"

#ifndef __arm__
#define __important_massage__                                                  \
  "!!![stepper_driver_lib] The library is ONLY applicable for a BeagleBone BLACK controller \
 and thus should be build on arm system. In the case the library will be build in DEBUG mode without any hardware support\n"
#endif // __arm__

using namespace beaglebone::gpio;
GPIOManager::GPIOManager(/* args */) {
#ifdef __arm__
  auto status = iolib_init();
  if (status != 0) {
    throw GPIOInitException();
  }
#endif // __arm__
}
GPIOManager *GPIOManager::getInstance() {
#ifndef __arm__
  printf(__important_massage__);
#endif // __arm__
  static std::shared_ptr<GPIOManager> ptr{new GPIOManager()};
  return ptr.get();
}
bool GPIOManager::setPinDirection(GPIOPin pin, GPIO_DIR dir) {
#ifdef __arm__
  if (iolib_setdir(pin.port, pin.pin, char(dir)) == 0)
    return true;
  else
    return false;
#else
  return true;
#endif // __arm__
}
void GPIOManager::digitalWrite(GPIOPin pin, GPIO_VALUE value) {
#ifdef __arm__
  value == GPIO_VALUE_LOW ? pin_low(pin.port, pin.pin)
                          : pin_high(pin.port, pin.pin);
#endif // __arm__
}
GPIO_VALUE GPIOManager::digitalRead(GPIOPin pin) {
#ifdef __arm__
  if (is_low(pin.port, pin.pin))
    return GPIO_VALUE_LOW;
  else
    return GPIO_VALUE_HIGH;
#else
  return GPIO_VALUE_LOW;
#endif // __arm__
}
GPIOManager::~GPIOManager() {
#ifdef __arm__
  auto status = iolib_free();
  if (status != 0)
    throw GPIOFreeException();
#endif // __arm__
}