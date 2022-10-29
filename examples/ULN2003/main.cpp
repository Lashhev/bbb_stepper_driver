#include "bbb_stepper/stepper_driver_uln2003.hpp"
#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <unistd.h>

#define IN1 8, 8
#define IN2 8, 10
#define IN3 8, 12
#define IN4 8, 14
using namespace beaglebone::gpio;
using namespace beaglebone::motor_driver;
// Maximum speed
const int MAX_SPEED = 1000;

//  Acceleration
const int ACCEL = 500;

//  Target position
const int TARGET_POS = 4000;
using namespace boost::program_options;

// Number of steps per internal motor revolution
const float DEFAULT_SPEED = 100;

//  Amount of Gear Reduction
const float DEFAULT_STEPS = 900;

GPIOPin convert2GPIOPin(const std::string &pin_string) {
  int port, pin;
  sscanf(pin_string.c_str(), "P%d.%d", &port, &pin);
  GPIOPin p = GPIOPin(port, pin);
  return p;
}

class StepperDriverArgs {
private:
  std::string in1, in2, in3, in4;
  int max_speed, accel, position, speed;
  variables_map vm;

public:
  StepperDriverArgs(/* args */);
  bool process(int argc, char *argv[]);
  void get(GPIOPin &in1, GPIOPin &in2, GPIOPin &in3, GPIOPin &in4, int &speed,
           int &max_speed, int &accel, int &position);
  ~StepperDriverArgs();
};

StepperDriverArgs::StepperDriverArgs(/* args */) {}

bool StepperDriverArgs::process(int argc, char *argv[]) {
  options_description desc("ULN2003 Stepper driver options");
  desc.add_options()("help,h", "Show help")(
      "IN1", value<std::string>(&in1),
      "First pin of ULN2003 Stepper driver. Example 'P8.8'")(
      "IN2", value<std::string>(&in2),
      "Second pin of ULN2003 Stepper driver. Example 'P8.10'")(
      "IN3", value<std::string>(&in3),
      "Third pin of ULN2003 Stepper driver. Example 'P8.12'")(
      "IN4", value<std::string>(&in4),
      "Third pin of ULN2003 Stepper driver. Example 'P8.14'")(
      "speed", value<int>(&speed), "Stepper motor speed.")(
      "max_speed", value<int>(&max_speed), "Max stepper motor speed.")(
      "accel", value<int>(&accel), "Stepper motor acceleration.")(
      "target_pos", value<int>(&position), "Stepper motor target position.");

  parsed_options parsed = command_line_parser(argc, argv).options(desc).run();
  store(parsed, vm);
  notify(vm);
  if (vm.count("help")) {
    std::cout << desc;
    return false;
  }
  return true;
}

void StepperDriverArgs::get(GPIOPin &in1, GPIOPin &in2, GPIOPin &in3,
                            GPIOPin &in4, int &speed, int &max_speed,
                            int &accel, int &position) {
  if (vm.count("IN1"))
    in1 = convert2GPIOPin(this->in1);
  else {
    in1 = GPIOPin(IN1);
  }
  if (vm.count("IN2"))
    in2 = convert2GPIOPin(this->in2);
  else {
    in2 = GPIOPin(IN2);
  }
  if (vm.count("IN3"))
    in3 = convert2GPIOPin(this->in3);
  else {
    in3 = GPIOPin(IN3);
  }
  if (vm.count("IN4"))
    in4 = convert2GPIOPin(this->in4);
  else {
    in4 = GPIOPin(IN4);
  }

  if (vm.count("speed"))
    speed = this->speed;
  else {
    speed = DEFAULT_SPEED;
  }
  if (vm.count("max_speed"))
    max_speed = this->max_speed;
  else {
    max_speed = MAX_SPEED;
  }
  if (vm.count("accel"))
    accel = this->accel;
  else {
    accel = ACCEL;
  }

  if (vm.count("target_pos"))
    position = this->position;
  else {
    position = TARGET_POS;
  }
}

StepperDriverArgs::~StepperDriverArgs() {}

int main(int argc, char *argv[]) {
  // Define Variables
  GPIOPin in1, in2, in3, in4;
  int speed, max_speed, accel, position;
  StepperDriverArgs sda;
  if (!sda.process(argc, argv))
    return EXIT_SUCCESS;
  sda.get(in1, in2, in3, in4, speed, max_speed, accel, position);
  auto stepper = StepperDriverULN2003(in1, in2, in3, in4);
  stepper.init();
  stepper.setMaxSpeed(max_speed);
  stepper.setAcceleration(accel);
  printf("max_speed: %d\naccel: %d\nposition: %d\n", max_speed, accel,
         position);
  stepper.runToNewPosition(position);
  sleep(1);
  stepper.runToNewPosition(0);

  return 0;
}
