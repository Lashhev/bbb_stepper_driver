#pragma once
#include "bbb_stepper/stepper_driver.hpp"
#include <chrono>
namespace beaglebone {
namespace motor_driver {

class StepperDriverTB6600 : public StepperDriver {
private:
  /* data */
public:
  StepperDriverTB6600(
      beaglebone::gpio::GPIOPin dir_pin, beaglebone::gpio::GPIOPin pulse_pin,
      beaglebone::gpio::GPIOPin enable_pin = beaglebone::gpio::GPIOPin());
  ~StepperDriverTB6600();
  void setSpeed(float speed) override;

protected:
  /// Disable motor pin outputs by setting them all LOW
  /// Depending on the design of your electronics this may turn off
  /// the power to the motor coils, saving power.
  /// This is useful to support Arduino low power modes: disable the outputs
  /// during sleep and then reenable with enableOutputs() before stepping
  /// again.
  void disableOutputs() override;

  /// Enable motor pin outputs by setting the motor pins to OUTPUT
  /// mode. Called automatically by the constructor.
  void enableOutputs() override;
  void step(uint8_t step) override;
  // motor pin numbers:
  beaglebone::gpio::GPIOPin motor_pins[3];
};
} // namespace motor_driver
} // namespace beaglebone
