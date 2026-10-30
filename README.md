# TME 441 Milestone 1: Differential Drive

Arduino code for testing a two-motor differential-drive robot with wheel encoders.
The project includes basic motor control and a PID speed controller.

## Project files

- `tme441-milestone-1.ino` — Arduino sketch and selectable motor-control tests.
- `motor_control.h` — motor pin assignments, `Motor` class, and PID implementation.

## Requirements

- Arduino IDE or another Arduino-compatible build/upload environment.
- The [Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html).
- A compatible Arduino board, motor driver, motors, and encoders.

Install the Encoder library through the Arduino IDE Library Manager, select the target board and port, then build and upload the sketch.
Note that this sketch defines its own `main()` instead of the usual Arduino `setup()` and `loop()` functions.

## Instructions

For safety, raise the drive wheels off the ground before powering the robot.
Switch off your battery before uploading new code.
Switch off your battery _and_ disconnect your Arduino from your computer before making any changes to hardware or wiring.

### Test motor open-loop response

Connect your L293D motor driver to your Arduino using the pins specified below:

| Signal | L293D pin | Arduino pin |
| --- | --- | --- |
| Left motor speed | 1,2EN | 5 |
| Left motor direction A | 1A | 6 |
| Left motor direction B | 2A | 7 |
| Right motor speed | 3,4EN | 8 |
| Right motor direction A | 4A | 9 |
| Right motor direction B | 3A | 10 |

Connect your L203D motor driver to your motor terminals:

| Motor pin | L293D pin |
| --- | --- |
| Left motor (+) | 1Y |
| Left motor (-) | 2Y |
| Right motor (+) | 3Y |
| Right motor (-) | 4Y |

Connect the remaining motor wires to enable the encoders:

#### Left motor: 
| Motor pin | Arduino pin |
| --------- | ----------- |
| Encoder ground | Ground (breadboard) |
| Encoder V_CC | +5V (breadboard) |
| Encoder signal A | 18 |
| Encoder signal B | 19 |

#### Right motor:

| Motor pin | Arduino pin |
| --------- | ----------- |
| Encoder ground | Ground (breadboard) |
| Encoder V_CC | +5V (breadboard) |
| Encoder signal A | 20 |
| Encoder signal B | 21 |

Test your open-loop response to make sure the motors turn the correct direction (forwards with a positive duty cycle, backwards with a negative duty cycle). Check that both encoder readings count the correct direction. Test several possible combinations, e.g. both motors on or off, one motor at a time, forwards and backwards. Change the line of code `testOpenLoop(255, 255);` to change the duty cycle and direction for each motor.

### Test closed-loop PID control

The controller is implemented in `motor_control.h`:

- Each motor is represented by a `Motor` object with its encoder. Encoder
  pins are supplied to the `Motor` constructor. The object provides `init()`,
  `on(speed)`, `halt()`, and `readEncoder()` methods.
- `K_P`, `K_I`, and `K_D` are the proportional, integral, and derivative gains.
- `PID_INT` sets the update interval in milliseconds.
- `PID_REP_INT` sets the reporting interval in milliseconds.
- `updatePID(speed_l, speed_r, report)` applies the requested left and right
  speeds in encoder ticks per second when called repeatedly.
- `resetPID()` clears the controller's stored state before a new test.

Comment out `testOpenLoop()` and uncomment `testClosedLoop()` to enable the PID controller.
The closed-loop test function will repeatedly command a constant speed for a specified duration (in milliseconds), and then turn off for 1 second.
Open the Arduino Serial Plotter to graph the motor speed and duty cycle.
The gains are all set to zero by default. Increase `K_P` and `K_I` slowly and one at a time to see their effect on the motor behavior.

After tuning the gains, the motor speed should quickly rise to the desired speed and stay roughly constant for the duration of the step.
Test the robustness of your controller by adding and removing a small amount of friction on the wheel to see if the controller compensates appropriately.
