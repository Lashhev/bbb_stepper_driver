#include "bbb_stepper/stepper_driver_uln2003.hpp"
#include <cmath>
#include <unistd.h>

using namespace beaglebone::gpio;
using namespace beaglebone::motor_driver;

void StepperDriverULN2003::setSpeed(float speed) {
  _speed = speed;
  _stepInterval = fabs(1000.0 / speed);
}

void StepperDriverULN2003::setOutputPins(uint8_t mask) {
  for (int i = 0; i < 4; i++)
    context->digitalWrite(motor_pins[i], (mask & (1 << i)) ? (GPIO_VALUE_HIGH)
                                                           : (GPIO_VALUE_LOW));
}
StepperDriverULN2003::StepperDriverULN2003(GPIOPin motor_pin_1,
                                           GPIOPin motor_pin_2,
                                           GPIOPin motor_pin_3,
                                           GPIOPin motor_pin_4)
    : StepperDriver() {
  this->motor_pins[0] = motor_pin_1;
  this->motor_pins[1] = motor_pin_2;
  this->motor_pins[2] = motor_pin_3;
  this->motor_pins[3] = motor_pin_4;
}

void StepperDriverULN2003::disableOutputs() {
  // setup the pins on the microcontroller:
  for (size_t i = 0; i < 4; i++)
    context->digitalWrite(motor_pins[i], GPIO_VALUE_LOW);
}

void StepperDriverULN2003::enableOutputs() {
  // setup the pins on the microcontroller:
  for (size_t i = 0; i < 4; i++)
    context->setPinDirection(motor_pins[i], GPIO_DIR_OUT);
}

void StepperDriverULN2003::step(uint8_t step) {
  switch (step) {
  case 0: // 1010
    context->digitalWrite(motor_pins[0], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[1], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[2], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[3], GPIO_VALUE_LOW);
    break;

  case 1: // 0110
    context->digitalWrite(motor_pins[0], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[1], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[2], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[3], GPIO_VALUE_LOW);
    break;

  case 2: // 0101
    context->digitalWrite(motor_pins[0], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[1], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[2], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[3], GPIO_VALUE_HIGH);
    break;

  case 3: // 1001
    context->digitalWrite(motor_pins[0], GPIO_VALUE_HIGH);
    context->digitalWrite(motor_pins[1], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[2], GPIO_VALUE_LOW);
    context->digitalWrite(motor_pins[3], GPIO_VALUE_HIGH);
    break;
  }
  // usleep(10);
}