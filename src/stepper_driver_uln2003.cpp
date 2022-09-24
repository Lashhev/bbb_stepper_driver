#include "stepper_driver_uln2003.hpp"
#include "gpio/BBBiolib.h"
#include <chrono>
#include <mutex>
std::once_flag once_flag_;

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
StepperDriverULN2003::StepperDriverULN2003(int number_of_steps, GPIOPin motor_pin_1, GPIOPin motor_pin_2,
                                      GPIOPin motor_pin_3, GPIOPin motor_pin_4)
{
    this->step_number = 0;    // which step the motor is on
    this->direction = 0;      // motor direction
    // this->last_step_time = 0; // timestamp in us of the last step taken
    this->number_of_steps = number_of_steps; // total number of steps for this motor

    // Arduino pins for the motor control connection:
    this->motor_pin_1 = motor_pin_1;
    this->motor_pin_2 = motor_pin_2;
    this->motor_pin_3 = motor_pin_3;
    this->motor_pin_4 = motor_pin_4;

    // setup the pins on the microcontroller:
    iolib_setdir(this->motor_pin_1.port, this->motor_pin_1.pin, BBBIO_DIR_OUT);
    iolib_setdir(this->motor_pin_2.port, this->motor_pin_2.pin, BBBIO_DIR_OUT);
    iolib_setdir(this->motor_pin_3.port, this->motor_pin_3.pin, BBBIO_DIR_OUT);
    iolib_setdir(this->motor_pin_4.port, this->motor_pin_4.pin, BBBIO_DIR_OUT);
}

/*
 * Sets the speed in revs per minute
 */
void StepperDriverULN2003::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
  printf("current delay: %lu\n", this->step_delay);
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void StepperDriverULN2003::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
      // step the motor to step number 0, 1, ..., {3 or 10}
      // TIME_IT(
        stepMotor(this->step_number % 4, steps_left);
        // )
  }
}

/*
 * Moves the motor forward or backwards.
 */
void StepperDriverULN2003::stepMotor(int thisStep, int &steps_left)
{
  auto now = std::chrono::system_clock::now();
  // move only if the appropriate delay has passed:
  if (std::chrono::duration_cast<std::chrono::microseconds>(now-this->last_step_time).count() >= this->step_delay)
  {
    // printf("%lu, %lu\n", std::chrono::duration_cast<std::chrono::microseconds>(now-this->last_step_time).count(), this->step_delay);
    // get the timeStamp of when you stepped:
    this->last_step_time = now;
    // increment or decrement the step number,
    // depending on direction:
    if (this->direction == 1)
    {
      this->step_number++;
      if (this->step_number == this->number_of_steps) {
        this->step_number = 0;
      }
    }
    else
    {
      if (this->step_number == 0) {
        this->step_number = this->number_of_steps;
      }
      this->step_number--;
    }
    // decrement the steps left:
    steps_left--;
  }
  switch (thisStep) {
    case 0:  // 1010
      pin_high(motor_pin_1.port, motor_pin_1.pin);
      pin_low(motor_pin_2.port, motor_pin_2.pin);
      pin_high(motor_pin_3.port, motor_pin_3.pin);
      pin_low(motor_pin_4.port, motor_pin_4.pin);
    break;
    case 1:  // 0110
      pin_low(motor_pin_1.port, motor_pin_1.pin);
      pin_high(motor_pin_2.port, motor_pin_2.pin);
      pin_high(motor_pin_3.port, motor_pin_3.pin);
      pin_low(motor_pin_4.port, motor_pin_4.pin);
    break;
    case 2:  //0101
      pin_low(motor_pin_1.port, motor_pin_1.pin);
      pin_high(motor_pin_2.port, motor_pin_2.pin);
      pin_low(motor_pin_3.port, motor_pin_3.pin);
      pin_high(motor_pin_4.port, motor_pin_4.pin);
    break;
    case 3:  //1001
      pin_high(motor_pin_1.port, motor_pin_1.pin);
      pin_low(motor_pin_2.port, motor_pin_2.pin);
      pin_low(motor_pin_3.port, motor_pin_3.pin);
      pin_high(motor_pin_4.port, motor_pin_4.pin);
    break;
  }
}

/*
  version() returns the version of the library:
*/
int StepperDriverULN2003::version(void)
{
  return 5;
}
