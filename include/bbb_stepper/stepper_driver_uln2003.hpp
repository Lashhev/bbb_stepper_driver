#pragma once
#include "bbb_stepper/stepper_driver.hpp"
#include <chrono>
namespace beaglebone {
namespace motor_driver {

class StepperDriverULN2003 : public StepperDriver {
public:
  // constructors:
  StepperDriverULN2003(beaglebone::gpio::GPIOPin motor_pin_1,
                       beaglebone::gpio::GPIOPin motor_pin_2,
                       beaglebone::gpio::GPIOPin motor_pin_3,
                       beaglebone::gpio::GPIOPin motor_pin_4);
  ~StepperDriverULN2003() { disableOutputs(); }
  void setSpeed(float speed) override;

protected:
  // You might want to override this to implement eg serial output
  // bit 0 of the mask corresponds to _pin[0]
  // bit 1 of the mask corresponds to _pin[1]
  // ....
  void setOutputPins(uint8_t mask);
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
  beaglebone::gpio::GPIOPin motor_pins[4];
};
} // namespace motor_driver
} // namespace beaglebone
