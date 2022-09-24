#pragma once
#include "types.hpp"
#include <cstdint>
#include <chrono>
/**
 * @brief Software driver to control NEMA stepper motor with TB6600 stepper motor driver 
 * 
 */
class StepperDriverTB6600
{
public:
    /**
     * @brief Construct a new Stepper Driver T B 6 6 0 0 object
     * 
     * @param pin_pulse 
     * @param pin_dir 
     * @param pin_ena 
     */
    StepperDriverTB6600(GPIOPin pin_pulse, GPIOPin pin_dir, GPIOPin pin_ena = GPIOPin());
    
    ~StepperDriverTB6600();
    /**
     * @brief Set the target position
     * The run() function will try to move the motor
     * from the current position to the target position set by the most
     * recent call to this function.
     * @param absolute The desired absolute position. Negative is
     * anticlockwise from the 0 position
     */
    void    moveTo(long absolute); 

    /// Set the target position relative to the current position
    /// \param[in] relative The desired position relative to the current position. Negative is
    /// anticlockwise from the current position.

    /**
     * @brief Set the target position relative to the current position
     * 
     * @param relative The desired position relative to the current position. Negative is anticlockwise from the current position.
     */
    void    move(long relative);

    /// Poll the motor and step it if a step is due, implementing
    /// accelerations and decelerations to achive the ratget position. You must call this as
    /// fequently as possible, but at least once per minimum step interval,
    /// preferably in your main loop.
    /// \return true if the motor is at the target position.
    bool run();

    /// Poll the motor and step it if a step is due, implmenting a constant
    /// speed as set by the most recent call to setSpeed().
    /// \return true if the motor was stepped.
    bool runSpeed();

    /// Sets the maximum permitted speed. the run() function will accelerate
    /// up to the speed set by this function.
    /// \param[in] speed The desired maximum speed in steps per second. Must
    /// be > 0. Speeds of more than 1000 steps per second are unreliable. 
    void    setMaxSpeed(float speed);

    /// Sets the acceleration and deceleration parameter.
    /// \param[in] acceleration The desired acceleration in steps per second
    /// per second. Must be > 0.
    void    setAcceleration(float acceleration);

    /// Sets the desired constant speed for use with runSpeed().
    /// \param[in] speed The desired constant speed in steps per
    /// second. Positive is clockwise. Speeds of more than 1000 steps per
    /// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
    /// once per hour, approximately. Speed accuracy depends on the Arduino
    /// crystal. Jitter depends on how frequently you call the runSpeed() function.
    void    setSpeed(float speed);

    /// The most recently set speed
    /// \return the most recent speed in steps per second
    float   speed();

    /// The distance from the current position to the target position.
    /// \return the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    long    distanceToGo();

    /// The most recently set target position.
    /// \return the target position
    /// in steps. Positive is clockwise from the 0 position.
    long    targetPosition();


    /// The currently motor position.
    /// \return the current motor position
    /// in steps. Positive is clockwise from the 0 position.
    long    currentPosition();  

    /// Resets the current position of the motor, so that wherever the mottor
    /// happens to be right now is considered to be the new position. Useful
    /// for setting a zero position on a stepper after an initial hardware
    /// positioning move.
    /// \param[in] position The position in steps of wherever the motor
    /// happens to be right now.
    void    setCurrentPosition(long position);  
    
    /// Moves the motor to the target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    void    runToPosition();

    /// Runs at the currently selected speed until the target position is reached
    /// Does not implement accelerations.
    bool runSpeedToPosition();

    /// Moves the motor to the new target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    /// \param[in] position The new target position.
    void    runToNewPosition(long position);

    /// Disable motor pin outputs by setting them all LOW
    /// Depending on the design of your electronics this may turn off
    /// the power to the motor coils, saving power.
    /// This is useful to support Arduino low power modes: disable the outputs
    /// during sleep and then reenable with enableOutputs() before stepping
    /// again.
    void    disableOutputs();

    /// Enable motor pin outputs by setting the motor pins to OUTPUT
    /// mode. Called automatically by the constructor.
    void    enableOutputs();

protected:

    /// Forces the library to compute a new instantaneous speed and set that as
    /// the current speed. Calls
    /// desiredSpeed(), which can be overridden by subclasses. It is called by
    /// the library:
    /// \li  after each step
    /// \li  after change to maxSpeed through setMaxSpeed()
    /// \li  after change to acceleration through setAcceleration()
    /// \li  after change to target position (relative or absolute) through
    /// move() or moveTo()
    void           computeNewSpeed();

    /// Called to execute a step. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default calls step1(), step2() or step4() depending on the
    /// number of pins defined for the stepper.
    /// \param[in] step The current step phase number (0 to 3)
    virtual void   step(uint8_t step);

    /// Compute and return the desired speed. The default algorithm uses
    /// maxSpeed, acceleration and the current speed to set a new speed to
    /// move the motor from teh current position to the target
    /// position. Subclasses may override this to provide an alternate
    /// algorithm (but do not block). Called by computeNewSpeed whenever a new speed neds to be
    /// computed. 
    virtual float  desiredSpeed();
    
private:
    /// Arduino pin number for the 2 or 4 pins required to interface to the
    /// stepper motor.
    GPIOPin pin_pulse, pin_dir, pin_ena;

    /// The current absolution position in steps.
    long           _currentPos;    // Steps

    /// The target position in steps. The AccelStepper library will move the
    /// motor from teh _currentPos to the _targetPos, taking into account the
    /// max speed, acceleration and deceleration
    long           _targetPos;     // Steps

    /// The current motos speed in steps per second
    /// Positive is clockwise
    float          _speed;         // Steps per second

    /// The maximum permitted speed in steps per second. Must be > 0.
    float          _maxSpeed;

    /// The acceleration to use to accelerate or decelerate the motor in steps
    /// per second per second. Must be > 0
    float          _acceleration;

    /// The current interval between steps in milliseconds.
    unsigned long  _stepInterval;

    /// The last step time in milliseconds
    std::chrono::_V2::system_clock::time_point  _lastStepTime;
};

