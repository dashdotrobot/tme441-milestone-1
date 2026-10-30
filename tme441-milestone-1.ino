/* TME 441 Fall 2026
*  Milestone 1: Differential drive
*
*  Follow a pre-programmed trajectory using PID motor control.
*
*  @authors YourNamesHere
*  @team YourTeamNameHere
*/
#include <Encoder.h>
#include "motor_control.h"


int main() {
  init();
  Serial.begin(9600);

  initMotors();  // Configure motor pins for output

  // 1. Open-loop response
  testOpenLoop(255, 255);

  // 2. Closed-loop step response
  //testClosedLoop(5000);
}

/**
 * Test open-loop motor control.
 *
 * @param l_motor_speed Speed for the left motor (-255 to +255)
 * @param r_motor_speed Speed for the right motor (-255 to +255)
 */
void testOpenLoop(int l_motor_speed, int r_motor_speed) {
  while (true) {
    motorOn(LMOTOR, l_motor_speed);
    motorOn(RMOTOR, r_motor_speed);
    Serial.print("L: ");
    Serial.print(enc_l.read());
    Serial.print(", R: ");
    Serial.println(enc_r.read());
  }
}

/**
 * Test closed-loop behavior with repeated step responses.
 *
 * @param desired_speed Target speed for both motors in ticks/sec.
 * @param step_duration Duration of each step in milliseconds.
 */
void testClosedLoop(int desired_speed, int step_duration) {
  long step_start_time;
  long step_end_time;

  while (true) {

    // Command constant speed for specified duration
    resetPID();
    step_start_time = millis();
    while ((millis() - step_start_time) < step_duration) {
      updatePID(desired_speed, desired_speed, true);
    }

    // Stop motors for 1 second
    step_end_time = millis();
    while ((millis() - step_end_time) < 1000) {
      updatePID(0, 0, true);
    }
  }
}