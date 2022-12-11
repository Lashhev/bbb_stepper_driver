#pragma once
#include "bbb_stepper/stepper_driver.hpp"
#include <chrono>
namespace beaglebone {
namespace motor_driver {

/**
 * @brief A Software stepper Driver to control hardware stepper driver TB6600
 *
 */
class StepperDriverTB6600 : public StepperDriver {
private:
  // motor pin numbers:
  beaglebone::gpio::GPIOPin motor_pins[3];
  bool m_inverse_dir; // reverse direction
public:
  /**
   * @brief Constructor
   *
   * @param dir_pin direction pin
   * @param pulse_pin pulse pin
   * @param enable_pin enable pin
   * @param inverse_dir reverse direction
   */
  StepperDriverTB6600(
      beaglebone::gpio::GPIOPin dir_pin, beaglebone::gpio::GPIOPin pulse_pin,
      beaglebone::gpio::GPIOPin enable_pin = beaglebone::gpio::GPIOPin(),
      bool inverse_dir = false);
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
};
} // namespace motor_driver
} // namespace beaglebone
