# Arduino library for the LOLIN I2C MOTOR Shield allowing it to drive a 5-wire unipolar stepper motor
The Lolin (wemos) AT8870 Motor Shield is an I2C dual motor driver based on the AT8870 chip. The <https://github.com/wemos/LOLIN_I2C_MOTOR_Library> library
allows you to drive two bipolar motors but it won't easily let you drive a 5-wire unipolar stepper motor.

The library implemented here provides a class `LOLIN_I2C_UNIPOLAR_STEPPER_MOTOR` which extends `LOLIN_I2C_MOTOR` so you still have access to all base methods
like `changeAddress` or `getInfo` to setup and query the board. In addition there are also `void step(int16_t steps)` and `void setPosition(int16_t newPosition)`
to move the stepper back and forth, with the library taking care to activate the pins in the correct sequence so the motor moves as expected.

## Examples

A simple example can be found in [simple.ino](./examples/simple/simple.ino). The library will block the execution of the current task while it drives the motor.

If you don't like the idea of blocking your `loop()` for this long as the motor turns to reach its target position (it could take over 15 seconds
for the motor to travel 300 steps from one side of its range to the other, like when having to close a completely open valve) then it takes to
create a new task which the library will use for driving the motor. This will leave your `loop()` free to do the usual sensor checking, etc. An example for this
can be found in [asynchronous_loop.ino](./examples/asynchronous_loop/asynchronous_loop.ino). This, of course, means that you have an environment that can run
multiple tasks concurrently (like ESP32, or SAM(D) Arduino's).

A good reason to use a separate task would be the need to drive the motor in a time-sensitive manner. That is, if you don't want sensor readings in your main code
to cause the motor to receive a pulse that's longer than the defined value, or have the motor to unexpectedly wait between steps, then you can create a task with
a higher priority. Or if you need to achieve the opposite effect (to not block the execution of a time-critical sensor reading) then the task can be run with a lower priority.


## Installation
Download the repository as .zip and include it as a new library into the IDE selecting the menu:

```
 Sketch -> include Library -> Add .Zip library
```

## License

This library is licensed under `MIT license`.