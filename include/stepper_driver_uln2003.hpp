#pragma once
#include "types.hpp"
#include <chrono>

class StepperDriverULN2003
{
  public:
    // constructors:
    StepperDriverULN2003(int number_of_steps, GPIOPin motor_pin_1, GPIOPin motor_pin_2,
                                 GPIOPin motor_pin_3, GPIOPin motor_pin_4);

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    int version(void);

  private:
    void stepMotor(int this_step, int &steps_left);

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in us, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int step_number;          // which step the motor is on

    // motor pin numbers:
    GPIOPin motor_pin_1;
    GPIOPin motor_pin_2;
    GPIOPin motor_pin_3;
    GPIOPin motor_pin_4;

    std::chrono::_V2::system_clock::time_point last_step_time; // timestamp in us of when the last step was taken
};
