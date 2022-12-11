#include "bbb_stepper/stepper_driver_tb6600.hpp"
#include <chrono>
#include <cmath>
#include <thread>
#include <unistd.h>
using namespace beaglebone::gpio;
using namespace beaglebone::motor_driver;
using namespace std::chrono_literals;

void StepperDriverTB6600::setSpeed(float speed) {
  if (_speed != speed) {
    _speed = speed;
    auto pin_dir = motor_pins[0];
    auto pin_ena = motor_pins[2];

    if (pin_ena != GPIOPin()) {
      if (_speed != 0) {
        if (context->digitalRead(pin_ena) == GPIO_VALUE_HIGH)
          context->digitalWrite(pin_ena, GPIO_VALUE_LOW);
      } else {
        if (context->digitalRead(pin_ena) == GPIO_VALUE_LOW)
          context->digitalWrite(pin_ena, GPIO_VALUE_HIGH);
      }
    }
    if (_speed > 0 && !m_inverse_dir) {
      if (context->digitalRead(pin_dir) != GPIO_VALUE_LOW)
        context->digitalWrite(pin_dir, GPIO_VALUE_HIGH);
    } else {
      if (context->digitalRead(pin_dir) != GPIO_VALUE_HIGH)
        context->digitalWrite(pin_dir, GPIO_VALUE_LOW);
    }
    _stepInterval = fabs(1000.0 / speed);
  }
}

StepperDriverTB6600::StepperDriverTB6600(GPIOPin dir_pin, GPIOPin pulse_pin,
                                         GPIOPin enable_pin, bool inverse_dir)
    : StepperDriver(), m_inverse_dir(inverse_dir) {
  this->motor_pins[0] = dir_pin;
  this->motor_pins[1] = pulse_pin;
  this->motor_pins[2] = enable_pin;
}

void StepperDriverTB6600::disableOutputs() {
  // setup the pins on the microcontroller:
  for (size_t i = 0; i < 2; i++)
    context->digitalWrite(motor_pins[i], GPIO_VALUE_LOW);
  auto pin_ena = motor_pins[2];
  if (pin_ena != GPIOPin())
    context->digitalWrite(pin_ena, GPIO_VALUE_LOW);
}

void StepperDriverTB6600::enableOutputs() {
  // setup the pins on the microcontroller:
  for (size_t i = 0; i < 2; i++)
    context->setPinDirection(motor_pins[i], GPIO_DIR_OUT);
  auto pin_ena = motor_pins[2];
  if (pin_ena != GPIOPin())
    context->setPinDirection(pin_ena, GPIO_DIR_OUT);
}

void StepperDriverTB6600::step(uint8_t step) {

  auto pin_pulse = motor_pins[1];
  // Caution 200ns setup time
  context->digitalWrite(pin_pulse, GPIO_VALUE_HIGH);
  // Caution, min Step pulse width for 3967 is 1microsec
  // Delay 1microsec
  std::this_thread::sleep_for(1us);
  context->digitalWrite(pin_pulse, GPIO_VALUE_LOW);
}
StepperDriverTB6600::~StepperDriverTB6600() { disableOutputs(); }
