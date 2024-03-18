/*
 *	Simple unipolar stepper motor test.
 */

#include <Wire.h>
#include <LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR.h>

LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR motor; //I2C address 0x30

void setup()
{
	Serial.begin(115200);
	Serial.println("Motor Shield Testing...");

	while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) // wait for motor shield to be ready.
	{
		Serial.print(".");
		motor.getInfo();
	}
	
	// if this motor can only turn 300 steps in one direction (like a valve motor for example) then better set this value here
	// so the library will take care not to try to drive the motor past its limits.
	motor.motorSteps = 300;
	// if the motor needs a 50 ms pulse to make a single step (this should be taken from the motor's specification)
	motor.pulseLength = 50;
	// not sure what the default values for frequency and duty in the shield are so it's better to set them to some safe values
	motor.changeFreq(MOTOR_CH_BOTH, 1);
	motor.changeDuty(MOTOR_CH_BOTH, 100);
	// make sure it's stopped
	motor.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_STOP);
}

void loop()
{
	// assuming the motor is in its 0 position
	Serial.println("Turning the motor so it goes to its 40th step position");
	motor.setPosition(40);
	// 40 steps by at least 50 ms each means the above command will block the execution for at least two seconds
	Serial.println("Current position: " + String(motor.position));

	Serial.println("Turning the motor 20 steps further ahead");
	motor.step(20);
	// one second later the motor is at the 60th step of its range
	Serial.println("Current position: " + String(motor.position));

	Serial.println("Turning the motor back to its 20th step position");
	motor.setPosition(20);
	// two seconds later the motor is at the 20th step of its range
	Serial.println("Current position: " + String(motor.position));

	Serial.println("Turning the motor back to its 0 position");
	motor.setPosition(0);
	Serial.println("Current position: " + String(motor.position));

	Serial.println("Done");
	
	delay(3000);
}

