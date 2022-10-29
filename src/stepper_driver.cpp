#include "bbb_stepper/stepper_driver.hpp"
#include <chrono>
#include <cmath>
#include <unistd.h>

#define IS_INIT()                                                              \
  if (!(is_init)) {                                                            \
    printf("[ERROR] Stepper Driver is not initialized yet. Call "              \
           "StepperDriver::init() before\n");                                  \
    return false;                                                              \
  }
using namespace beaglebone::gpio;
using namespace beaglebone::motor_driver;

void StepperDriver::moveTo(long absolute) {
  _targetPos = absolute;
  computeNewSpeed();
  // compute new n?
}

void StepperDriver::move(long relative) { moveTo(_currentPos + relative); }

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
bool StepperDriver::runSpeed() {
  auto time = std::chrono::system_clock::now();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(time -
                                                            _lastStepTime)
          .count() > _stepInterval) {
    if (_speed > 0) {
      // Clockwise
      _currentPos += 1;
    } else if (_speed < 0) {
      // Anticlockwise
      _currentPos -= 1;
    }
    // printf("step\n");
    step(abs(_currentPos % 4));

    _lastStepTime = time;
    return true;
  } else
    return false;
}

long StepperDriver::distanceToGo() { return _targetPos - _currentPos; }

long StepperDriver::targetPosition() { return _targetPos; }

long StepperDriver::currentPosition() { return _currentPos; }

// Useful during initialisations or after initial positioning
// Sets speed to 0
void StepperDriver::setCurrentPosition(long position) {
  _currentPos = position;
}

void StepperDriver::computeNewSpeed() { setSpeed(desiredSpeed()); }

float StepperDriver::desiredSpeed() {
  long distanceTo = distanceToGo();

  // Max possible speed that can still decelerate in the available distance
  float requiredSpeed;
  if (distanceTo == 0)
    return 0.0;            // Were there
  else if (distanceTo > 0) // Clockwise
    requiredSpeed = sqrt(2.0 * distanceTo * _acceleration);
  else // Anticlockwise
    requiredSpeed = -sqrt(2.0 * -distanceTo * _acceleration);

  if (requiredSpeed > _speed) {
    // Need to accelerate in clockwise direction
    if (_speed == 0)
      requiredSpeed = sqrt(2.0 * _acceleration);
    else
      requiredSpeed = _speed + abs(_acceleration / _speed);
    if (requiredSpeed > _maxSpeed)
      requiredSpeed = _maxSpeed;
  } else if (requiredSpeed < _speed) {
    // Need to accelerate in anticlockwise direction
    if (_speed == 0)
      requiredSpeed = -sqrt(2.0 * _acceleration);
    else
      requiredSpeed = _speed - abs(_acceleration / _speed);
    if (requiredSpeed < -_maxSpeed)
      requiredSpeed = -_maxSpeed;
  }
  //  Serial.println(requiredSpeed);
  return requiredSpeed;
}
// Run the motor to implement speed and acceleration in order to proceed to the
// target position You must call this at least once per step, preferably in your
// main loop If the motor is in the desired position, the cost is very small
// returns true if the motor is still running to the target position.
bool StepperDriver::run() {
  IS_INIT()
  if (_targetPos == _currentPos)
    return false;

  if (runSpeed())
    computeNewSpeed();
  return true;
}

StepperDriver::StepperDriver()
    : is_init(false), _currentPos(0), _targetPos(0), _speed(0), _maxSpeed(1.0),
      _acceleration(1.0), _stepInterval(0) {}

bool StepperDriver::init() {
  context = GPIOManager::getInstance();
  if (!context) {
    printf("Failed to create iolib context\n");
    return false;
  }
  enableOutputs();
  is_init = true;
  return true;
}

void StepperDriver::setMaxSpeed(float speed) {
  _maxSpeed = speed;
  computeNewSpeed();
}

void StepperDriver::setAcceleration(float acceleration) {
  _acceleration = acceleration;
  computeNewSpeed();
}

float StepperDriver::speed() { return _speed; }
// Blocks until the target position is reached and stopped
bool StepperDriver::runToPosition() {
  IS_INIT()
  while (run())
    ;
  return true;
}

bool StepperDriver::runSpeedToPosition() {
  return _targetPos != _currentPos ? StepperDriver::runSpeed() : false;
}

// Blocks until the new target position is reached
bool StepperDriver::runToNewPosition(long position) {
  IS_INIT()
  moveTo(position);
  runToPosition();
  return true;
}

StepperDriver::~StepperDriver() {}