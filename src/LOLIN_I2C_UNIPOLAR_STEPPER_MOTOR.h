/*
 * LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR.h
 *
 *  Created on: 1 Mar 2024
 *      Author: gkostov
 */

#ifndef LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR_H_
#define LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR_H_

#include <LOLIN_I2C_MOTOR.h>

class LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR: public LOLIN_I2C_MOTOR{
  public:
		LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR(unsigned char address = DEFAULT_I2C_MOTOR_ADDRESS);
		/*
		 * A loop to repeatedly call the provided callback until it returns false.
		 * The callback adjusts the motor one step in the direction of the target position
		 * until the position is reached.
		 */
		void (*stepLoopFn)(std::function<bool()> cb, std::function<void()> done) = NULL;
		/*
		 * Change the desired position of the motor with the provided number of steps
		 *
		 * @param steps A positive or a negative number
		 */
		void step(int16_t steps);
		/*
		 * Change the desired position of the motor
		 *
		 * @param newPosition A positive or a negative number
		 */
		void setPosition(int16_t newPosition);
		/*
		 * The length of a pulse (in ms) to drive the motor with.
		 */
		uint32_t pulseLength = 100;
		/*
		 * The total number of steps that the motor can travel.
		 * If the motor does not have such a limit (it can move indefinitely) the value can be left as 0.
		 */
		uint16_t motorSteps = 0;
		/*
		 * Gives the current position of the motor.
		 * This value will keep changing as the motor moves to reach the target position.
		 */
		int16_t position = 0;
		/*
		 * If the step loop is currently executing
		 */
		bool isLooping = false;

  private:
		/*
		 * The target position to be reached.
		 */
		int16_t targetPosition = 0;
		/*
		 * Indicates the last activated motor wire pin (can have a value between 1 and 4)
		 */
		uint8_t motorPin;
		/*
		 * A flag to know when the asynchronous `doStep(cb)` is currently executing
		 */
		bool isStepping = false;
		/*
		 * Start the step loop if that's not already started.
		 * If `stepLoopFn` is assigned then that will be called to execute the loop.
		 * Otherwise a simple loop
		 * is started (which calls `yield()` between callback invocations). This means that it is going to block
		 * the current task until the motor reaches the target position. If this is not desirable then you need
		 * to provide a better implementation as `stepLoopFn`.
		 */
		void doLoop();
		/*
		 * Move the motor's position one step further. It will activate the next wire pin for the duration of `pulseLength` milliseconds.
		 */
		void doStep();
		/*
		 * Check if the current `position` value matches `targetPosition` and if that is not the case then it will
		 * call `moveStepPointers()` and `doStep()` to move the motor one step closer to where it has to be.
		 *
		 * @return Returns `true` if `doMove()` has been called or false if `position` currently matches `targetPosition`
		 */
		bool stepIfNeeded();
};

#endif /* LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR_H_ */
