#include "stepper_driver_tb6600.hpp"
#include "gpio/BBBiolib.h"
#include <chrono>
#include <cmath>
#include <unistd.h>

void StepperDriverTB6600::moveTo(long absolute)
{
    _targetPos = absolute;
    computeNewSpeed();
}

void StepperDriverTB6600::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Implements steps according to the current speed
// You must call this at least once per step
// returns true if a step occurred
bool StepperDriverTB6600::runSpeed()
{
    auto time = std::chrono::system_clock::now();
  
    if (std::chrono::duration_cast<std::chrono::milliseconds>(time - _lastStepTime).count() > _stepInterval)
    {
	if (_speed > 0)
	{
	    // Clockwise
	    _currentPos += 1;
	}
	else if (_speed < 0)
	{
	    // Anticlockwise  
	    _currentPos -= 1;
	}
	step(_currentPos & 0x3); // Bottom 2 bits (same as mod 4, but works with + and - numbers) 

	_lastStepTime = time;
	return true;
    }
    else
	return false;
}

long StepperDriverTB6600::distanceToGo()
{
    return _targetPos - _currentPos;
}

long StepperDriverTB6600::targetPosition()
{
    return _targetPos;
}

long StepperDriverTB6600::currentPosition()
{
    return _currentPos;
}

// Useful during initialisations or after initial positioning
void StepperDriverTB6600::setCurrentPosition(long position)
{
    _currentPos = position;
}

void StepperDriverTB6600::computeNewSpeed()
{
    setSpeed(desiredSpeed());
}

// Work out and return a new speed.
// Subclasses can override if they want
// Implement acceleration, deceleration and max speed
// Negative speed is anticlockwise
// This is called:
//  after each step
//  after user changes:
//   maxSpeed
//   acceleration
//   target position (relative or absolute)
float StepperDriverTB6600::desiredSpeed()
{
    long distanceTo = distanceToGo();

    // Max possible speed that can still decelerate in the available distance
    float requiredSpeed;
    if (distanceTo == 0)
	return 0.0; // Were there
    else if (distanceTo > 0) // Clockwise
	requiredSpeed = sqrt(2.0 * distanceTo * _acceleration);
    else  // Anticlockwise
	requiredSpeed = -sqrt(2.0 * -distanceTo * _acceleration);

    if (requiredSpeed > _speed)
    {
	// Need to accelerate in clockwise direction
	if (_speed == 0)
	    requiredSpeed = sqrt(2.0 * _acceleration);
	else
	    requiredSpeed = _speed + abs(_acceleration / _speed);
	if (requiredSpeed > _maxSpeed)
	    requiredSpeed = _maxSpeed;
    }
    else if (requiredSpeed < _speed)
    {
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

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if we are still running to position
bool StepperDriverTB6600::run()
{
    if (_targetPos == _currentPos)
	return false;
    
    if (runSpeed())
	computeNewSpeed();
    return true;
}

StepperDriverTB6600::StepperDriverTB6600(GPIOPin pin_pulse, GPIOPin pin_dir, GPIOPin pin_ena)
{
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _acceleration = 1.0;
    _stepInterval = 0;
    // _lastStepTime = 0;
    this->pin_pulse = pin_pulse;
    this->pin_dir = pin_dir;
    this->pin_ena = pin_ena;
    enableOutputs();
}

void StepperDriverTB6600::setMaxSpeed(float speed)
{
    _maxSpeed = speed;
    computeNewSpeed();
}

void StepperDriverTB6600::setAcceleration(float acceleration)
{
    _acceleration = acceleration;
    computeNewSpeed();
}

void StepperDriverTB6600::setSpeed(float speed)
{
    _speed = speed;
    if(pin_ena != GPIOPin())
    {
        if(_speed != 0)
            if (!is_high(pin_ena.port, pin_ena.pin))
                pin_high(pin_ena.port, pin_ena.pin);  
        else
            if (!is_low(pin_ena.port, pin_ena.pin))
                pin_low(pin_ena.port, pin_ena.pin);
    }
    _stepInterval = abs(1000.0 / _speed);
}

float StepperDriverTB6600::speed()
{
    return _speed;
}
// 1 pin step function (ie for stepper drivers)
// This is passed the current step number (0 to 3)
// Subclasses can override
void StepperDriverTB6600::step(uint8_t step)
{
    if(_speed > 0)
      pin_high(pin_dir.port, pin_dir.pin);
    else
    {
      pin_low(pin_dir.port, pin_dir.pin);
    }
    // Caution 200ns setup time 
    pin_high(pin_pulse.port, pin_pulse.pin);
    // Caution, min Step pulse width for 3967 is 1microsec
    // Delay 1microsec
    usleep(1);
    pin_low(pin_pulse.port, pin_pulse.pin);
}
// Prevents power consumption on the outputs
void    StepperDriverTB6600::disableOutputs()
{   
  if(pin_ena != GPIOPin())
    pin_low(pin_ena.port, pin_ena.pin);
  pin_low(pin_pulse.port, pin_pulse.pin);
  pin_low(pin_dir.port, pin_dir.pin);
}

void    StepperDriverTB6600::enableOutputs()
{
    
    iolib_setdir(this->pin_dir.port, this->pin_dir.pin, BBBIO_DIR_OUT);
        iolib_setdir(this->pin_pulse.port, this->pin_pulse.pin, BBBIO_DIR_OUT);
    if(pin_ena != GPIOPin())
        iolib_setdir(this->pin_ena.port, this->pin_ena.pin, BBBIO_DIR_OUT);
}

// Blocks until the target position is reached
void StepperDriverTB6600::runToPosition()
{
    while (run())
	;
}

bool StepperDriverTB6600::runSpeedToPosition()
{
    return _targetPos!=_currentPos ? StepperDriverTB6600::runSpeed() : false;
}

// Blocks until the new target position is reached
void StepperDriverTB6600::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

StepperDriverTB6600::~StepperDriverTB6600()
{
    disableOutputs();
}


