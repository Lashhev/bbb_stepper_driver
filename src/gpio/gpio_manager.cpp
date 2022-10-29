#include "bbb_stepper/gpio/gpio_manager.hpp"
#include "gpio/BBBiolib.h"
using namespace beaglebone::gpio;
GPIOManager::GPIOManager(/* args */) {
  auto status = iolib_init();
  if (status != 0) {
    printf("[ERROR] Failed to initialize iolib. Maybe iolib is already "
           "initialized or your hardware "
           "doesn't support it\n");
  }
}
GPIOManager *GPIOManager::getInstance() {

  static std::shared_ptr<GPIOManager> ptr{new GPIOManager()};
  return ptr.get();
}
bool GPIOManager::setPinDirection(GPIOPin pin, GPIO_DIR dir) {
  if (iolib_setdir(pin.port, pin.pin, char(dir)) == 0)
    return true;
  else
    return false;
}
void GPIOManager::digitalWrite(GPIOPin pin, GPIO_VALUE value) {
  value == GPIO_VALUE_LOW ? pin_low(pin.port, pin.pin)
                          : pin_high(pin.port, pin.pin);
}
GPIO_VALUE GPIOManager::digitalRead(GPIOPin pin) {
  if (is_low(pin.port, pin.pin))
    return GPIO_VALUE_LOW;
  else
    return GPIO_VALUE_HIGH;
}
GPIOManager::~GPIOManager() { iolib_free(); }