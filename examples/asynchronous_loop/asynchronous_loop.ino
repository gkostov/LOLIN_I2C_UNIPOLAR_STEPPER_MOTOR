/*
 *	Run the unipolar stepper motor using a separate task so to not block the main loop().
 */

#include <Wire.h>
#include <LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR.h>

LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR motor; //I2C address 0x30

void stepLoopImpl(std::function<bool()> cb, std::function<void()> done) {
	// on the ESP32 there is `xTaskCreate()` to get you a new task on the scheduler
	
	// keep the callbacks in a static tuple for the task to use
	static std::tuple<std::function<bool()>, std::function<void()>> taskArgs = std::make_tuple(cb, done);
	xTaskCreate(
		[](void *arg){
			std::function<bool()> cb = std::get<0>(taskArgs);
			std::function<void()> done = std::get<1>(taskArgs);
			
			while(cb()) ;
			done();
			
			vTaskDelete(NULL);
		},
		"stepLoopImpl",
		5000,
		NULL,
		1,
		NULL
	);
}

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

	// create the looper task
	motor.stepLoopFn = stepLoopImpl;
}

void loop()
{
	if(millis() > 2000 && !motor.isLooping){
		if(motor.position == 0){
			Serial.println("Turning the motor so it goes to its 40th step position");
			motor.setPosition(40);
		}else if(motor.position == 40){
			Serial.println("Turning the motor 20 steps further ahead");
			motor.step(20);
		}else if(motor.position == 60){
			Serial.println("Turning the motor back to its 20th step position");
			motor.setPosition(20);
		}else if(motor.position == 20){
			Serial.println("Turning the motor back to its -20 position");
			motor.setPosition(-20);
		}else if(motor.position == -20)
			Serial.println("Done");
	}else{
		Serial.println("Current position: " + String(motor.position));
		delay(20);
	}
}
