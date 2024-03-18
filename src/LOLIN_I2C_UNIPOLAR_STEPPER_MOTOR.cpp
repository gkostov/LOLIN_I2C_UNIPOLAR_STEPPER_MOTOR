/*
 * LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR.cpp
 *
 *  Created on: 1 Mar 2024
 *      Author: gkostov
 */

#include "LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR.h"

#define MOTOR_PIN_MIN 1
#define MOTOR_PIN_MAX 4
#define MOTOR_PIN_MID MOTOR_PIN_MAX / 2

LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR(unsigned char address)
: LOLIN_I2C_MOTOR(address) {
	motorPin = MOTOR_PIN_MIN;
}

void LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::doStep() {
		// send the pulse to the hardware
		changeStatus(
			motorPin > MOTOR_PIN_MID ? MOTOR_CH_B : MOTOR_CH_A,
			motorPin & 1 ? MOTOR_STATUS_CW : MOTOR_STATUS_CCW
		);
		delay(pulseLength);
		changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_STOP);
}

bool LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::stepIfNeeded() {
	if(targetPosition != position){
		// move the pointers to the next motor position
		if(targetPosition > position){
			++position;
			if(motorPin < MOTOR_PIN_MAX)
				++motorPin;
			else
				motorPin = MOTOR_PIN_MIN;
		}else{
			--position;
			if(motorPin > MOTOR_PIN_MIN)
				--motorPin;
			else
				motorPin = MOTOR_PIN_MAX;
		}
			
		doStep();
		return true;
	}else
		return false;
}

void LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::doLoop() {
	if(!isLooping){
		isLooping = true;
		if(stepLoopFn)
			// do it so that a non-blocking asynchronous implementation can be used
			stepLoopFn(
				[this]() {
					return stepIfNeeded();
				},
				[this]() {
					isLooping = false;
					// if by a chance there has been another position change call in between
					// the last stepIfNeeded() invocation and running this callback
					if(targetPosition != position)
						doLoop();	// start the loop again
				}
			);
		else{
			// do it the simple blocking way
			while(stepIfNeeded()) yield();
			isLooping = false;
		}
	}
}

void LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::step(int16_t steps) {
	targetPosition += steps;
	if(motorSteps){
		// if the motor has a range then keep the position within that range
		if(targetPosition > motorSteps)
			targetPosition = motorSteps;
		else if(targetPosition < -motorSteps)
			targetPosition = -motorSteps;
	}
	
	doLoop();
}

void LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR::setPosition(int16_t newPosition) {
	if(motorSteps){
		// if the motor has a range then keep the position within that range
		if(newPosition > motorSteps)
			newPosition = motorSteps;
		else if(newPosition < -motorSteps)
			newPosition = -motorSteps;
		targetPosition = newPosition;
	}

	doLoop();
}

