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

  /* 1. Open-loop response.
   *    Uncomment the code below to test your motors and encoders. */
  while (true) {
    motorOn(LMOTOR, 255);  // Change the speeds and direction to test
    motorOn(RMOTOR, 255);
    Serial.print(enc_l.read());
    Serial.print(" ");
    Serial.println(enc_r.read());
  }


  /* 2. Closed-loop step response
   *    Uncomment the code below to test your PID controller step response. */
  // long step_start_time;
  // while (true) {

  //   // Run step response for 5 seconds
  //   resetPID();
  //   step_start_time = millis();
  //   while ((millis() - step_start_time) < 5000) {
  //     updatePID(1500, 1500, true);
  //   }

  //   // Stop motors
  //   updatePID(0, 0, false);
  //   motorHalt(LMOTOR);
  //   motorHalt(RMOTOR);

  //   delay(1000);  // Wait 1 second
  // }
}
