#include "gpio/BBBiolib.h"
#include "stepper_driver_tb6600.hpp"
#include <unistd.h>
#include <boost/program_options.hpp>
#include <iostream>

#define ENA 0, 0
#define DIR 8, 10
#define PUL 8, 12

using namespace boost::program_options;

// Maximum speed  
const int MAX_SPEED = 1000; 

//  Acceleration
const int ACCEL = 500;

//  Target position
const int TARGET_POS = 4000;

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
  std::string ena_pin, dir_pin, pul_pin;
  int max_speed, accel, position;
  variables_map vm;
public:
  StepperDriverArgs(/* args */);
  bool process(int argc, char *argv[]);
  void get(GPIOPin& ena_pin, GPIOPin& dir_pin, GPIOPin& pul_pin, int &max_speed, int &accel, int& position);
  ~StepperDriverArgs();
};

StepperDriverArgs::StepperDriverArgs(/* args */)
{
}

bool StepperDriverArgs::process(int argc, char *argv[])
{
  options_description desc("TB6600 Stepper driver options");
  desc.add_options()
    ("help,h", "Show help")
    ("ena_pin", value<std::string>(&ena_pin), "Enable pin of TB6600 Stepper driver. Example 'P8.8'")
    ("dir_pin", value<std::string>(&dir_pin), "Direction pin of TB6600 Stepper driver. Example 'P8.10'")
    ("pul_pin", value<std::string>(&pul_pin), "Pulse pin of TB6600 Stepper driver. Example 'P8.12'")
    ("max_speed", value<int>(&max_speed), "Maximum Stepper motor speed.")
    ("accel", value<int>(&accel), "Stepper motor acceleration.")
    ("target_pos", value<int>(&position), "Stepper motor target position.");
  
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

void StepperDriverArgs::get(GPIOPin& ena_pin, GPIOPin& dir_pin, GPIOPin& pul_pin, int &max_speed, int& accel, int& position)
{
  if(vm.count("ena_pin"))
    ena_pin = convert2GPIOPin(this->ena_pin);
  else
  {
    ena_pin = GPIOPin(ENA);
  }
  if(vm.count("dir_pin"))
    dir_pin = convert2GPIOPin(this->dir_pin);
  else
  {
    dir_pin = GPIOPin(DIR);
  }
  if(vm.count("pul_pin"))
    pul_pin = convert2GPIOPin(this->pul_pin);
  else
  {
    pul_pin = GPIOPin(PUL);
  }

  if(vm.count("max_speed"))
    max_speed = this->max_speed;
  else
  {
    max_speed = MAX_SPEED;
  }
  if(vm.count("accel"))
    accel = this->accel;
  else
  {
    accel = ACCEL;
  }

  if(vm.count("position"))
    position = this->position;
  else
  {
    position = TARGET_POS;
  }
  
}

StepperDriverArgs::~StepperDriverArgs()
{
}


int main(int argc, char *argv[])
{
  iolib_init();
  // Define Variables
  GPIOPin ena_pin, dir_pin, pul_pin;
  int max_speed, accel, position;
  StepperDriverArgs sda;
  if(!sda.process(argc, argv))
    return EXIT_FAILURE;
  sda.get(ena_pin, dir_pin, pul_pin, max_speed, accel, position);
	auto stepper = StepperDriverTB6600(pul_pin, dir_pin, ena_pin);
	// Slow - 4-step CW sequence to observe lights on driver board
  printf("Slow \n");
  stepper.setMaxSpeed(max_speed);    
  stepper.setAcceleration(accel);
  stepper.moveTo(position);
  stepper.runToPosition();
  sleep(1);
  stepper.moveTo(0);
  stepper.runToPosition();
  iolib_free();
	return EXIT_SUCCESS;
}
