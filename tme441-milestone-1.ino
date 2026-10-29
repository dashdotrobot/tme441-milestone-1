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
  testOpenLoop();

  // 2. Closed-loop step response
  //testClosedLoop(5000);
}

// Test functions for open-loop and closed-loop motor control
void testOpenLoop() {
  while (true) {
    motorOn(LMOTOR, 255);  // Change the speeds and direction to test
    motorOn(RMOTOR, 255);
    Serial.print(enc_l.read());
    Serial.print(" ");
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