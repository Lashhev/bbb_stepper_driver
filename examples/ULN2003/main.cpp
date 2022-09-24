#include "gpio/BBBiolib.h"
#include "stepper_driver_uln2003.hpp"
#include <unistd.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <chrono>

#define IN1 8, 8
#define IN2 8, 10
#define IN3 8, 12
#define IN4 8, 14

using namespace boost::program_options;

// Number of steps per internal motor revolution 
const float DEFAULT_SPEED = 100; 

//  Amount of Gear Reduction
const float DEFAULT_STEPS = 900;

GPIOPin convert2GPIOPin(const std::string &pin_string)
{
  int port, pin;
  sscanf(pin_string.c_str(), "P%d.%d", &port, &pin);
  GPIOPin p = GPIOPin(port, pin);
  return p;
}

class StepperDriverArgs
{
private:
  std::string in1, in2, in3, in4;
  int speed, steps;
  variables_map vm;
public:
  StepperDriverArgs(/* args */);
  bool process(int argc, char *argv[]);
  void get(GPIOPin& in1, GPIOPin& in2, GPIOPin& in3, GPIOPin& in4, int &speed, int &steps);
  ~StepperDriverArgs();
};

StepperDriverArgs::StepperDriverArgs(/* args */)
{
}

bool StepperDriverArgs::process(int argc, char *argv[])
{
  options_description desc("ULN2003 Stepper driver options");
  desc.add_options()
    ("help,h", "Show help")
    ("IN1", value<std::string>(&in1), "First pin of ULN2003 Stepper driver. Example 'P8.8'")
    ("IN2", value<std::string>(&in2), "Second pin of ULN2003 Stepper driver. Example 'P8.10'")
    ("IN3", value<std::string>(&in3), "Third pin of ULN2003 Stepper driver. Example 'P8.12'")
    ("IN4", value<std::string>(&in4), "Third pin of ULN2003 Stepper driver. Example 'P8.14'")
    ("speed", value<int>(&speed), "Motor speed.")
    ("steps", value<int>(&steps), "Motor steps.");
  
  parsed_options parsed = command_line_parser(argc, argv).options(desc).run();
  store(parsed, vm);
  notify(vm);
  if(vm.count("help"))
  {
    std::cout << desc;
    return false;
  }
  return true;
}

void StepperDriverArgs::get(GPIOPin& in1, GPIOPin& in2, GPIOPin& in3, GPIOPin& in4, int &speed, int &steps)
{
  if(vm.count("IN1"))
    in1 = convert2GPIOPin(this->in1);
  else
  {
    in1 = GPIOPin(IN1);
  }
  if(vm.count("IN2"))
    in2 = convert2GPIOPin(this->in2);
  else
  {
    in2 = GPIOPin(IN2);
  }
  if(vm.count("IN3"))
    in3 = convert2GPIOPin(this->in3);
  else
  {
    in3 = GPIOPin(IN3);
  }
  if(vm.count("IN4"))
    in4 = convert2GPIOPin(this->in4);
  else
  {
    in4 = GPIOPin(IN4);
  }

  if(vm.count("speed"))
    speed = this->speed;
  else
  {
    speed = DEFAULT_SPEED;
  }
  if(vm.count("steps"))
    steps = this->steps;
  else
  {
    steps = DEFAULT_STEPS;
  }
  
}

StepperDriverArgs::~StepperDriverArgs()
{
}


int main(int argc, char *argv[])
{
  // Define Variables
  GPIOPin in1, in2, in3, in4;
  int speed, steps;
  StepperDriverArgs sda;
  if(!sda.process(argc, argv))
    return EXIT_SUCCESS;
  sda.get(in1, in2, in3, in4, speed, steps);
  // Number of Steps Required
  int StepsRequired;
  iolib_init();
	auto stepper = StepperDriverULN2003(32, in1, in2, in3, in4);
   // Rotate CW 1/2 turn slowly
  printf("Rotate CW 1/2 turn slowly\n");
  stepper.setSpeed(speed);   
  stepper.step(steps);
  sleep(2);
  iolib_free();





  // TIME_IT(iolib_init(), "iolib_init")
  // TIME_IT(iolib_setdir(8,11, BBBIO_DIR_OUT), "iolib_setdir")
	// TIME_IT(iolib_setdir(8,12, BBBIO_DIR_OUT), "iolib_setdir")
  // TIME_IT(pin_low(8,11), "pin low")
  // TIME_IT(pin_high(8,12), "pin high")
  // TIME_IT(iolib_free(), "iolib_free")

	return 0;
}
