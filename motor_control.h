/* Motor Control Library
*  Utilities for configuring and controlling DC motors.
*
*  @author Matthew Ford
*  @version 2026
*/

#include <Encoder.h>

// Define motor control pins
#define RMOTOR_SPD 8
#define RMOTOR_DIR_A 9
#define RMOTOR_DIR_B 10

#define LMOTOR_SPD 5
#define LMOTOR_DIR_A 6
#define LMOTOR_DIR_B 7

// Define PID options
#define PID_INT 20       // PID update interval [ms]
#define PID_REP_INT 250  // PID report interval [ms]

/* Global variables for PID motor control. You will need to tune these
*  parameters for your own robot. */
const float K_P = 0.0;    // Proportional gain
const float K_I = 0.000;  // Integral gain
const float K_D = 0.0;    // Derivative gain (damping)

long last_enc_l = 0;  // Last encoder position [steps]
long last_enc_r = 0;
long last_pid_time = 0;  // Time of last control loop update [microsecs]
long last_pid_rep_time = 0;
long err_int_l = 0;  // Error integrals
long err_int_r = 0;
long last_speed_l = 0;  // Previous measured speed [steps/sec]
long last_speed_r = 0;

class Motor {
 public:
  Motor(int direction_pin_a, int direction_pin_b, int speed_pin,
        int encoder_pin_a, int encoder_pin_b)
      : direction_pin_a(direction_pin_a),
        direction_pin_b(direction_pin_b),
        speed_pin(speed_pin),
        encoder(encoder_pin_a, encoder_pin_b) {}

  void init() {
    pinMode(direction_pin_a, OUTPUT);
    pinMode(direction_pin_b, OUTPUT);
    pinMode(speed_pin, OUTPUT);
  }

  long readEncoder() {
    return encoder.read();
  }

  /* Turn motor on at desired speed from -255 to 255.
   * Negative speed runs the motor backwards.
   */
  void on(int speed) {
    digitalWrite(direction_pin_a, (speed > 0));
    digitalWrite(direction_pin_b, !(speed > 0));
    analogWrite(speed_pin, abs(speed));
  }

  // Stop motor quickly using electronic braking.
  void halt() {
    digitalWrite(direction_pin_a, LOW);
    digitalWrite(direction_pin_b, LOW);
    digitalWrite(speed_pin, HIGH);
  }

 private:
  int direction_pin_a;
  int direction_pin_b;
  int speed_pin;
  Encoder encoder;
};

Motor motor_l(LMOTOR_DIR_A, LMOTOR_DIR_B, LMOTOR_SPD, 18, 19);
Motor motor_r(RMOTOR_DIR_A, RMOTOR_DIR_B, RMOTOR_SPD, 20, 21);

/* Update motor voltages using feedback control
*
*  This function should be called repeatedly in a fast loop to update
*  the control signals. The PID update interval can be adjusted by
*  changing the constant PID_INT.
*
*  @param speed_l Desired speed of left motor [ticks/second].
*  @param speed_r ...              right motor.
*  @param report Print data to Serial Monitor periodically.
*/
void updatePID(long speed_l, long speed_r, bool report) {

  long curr_time = millis();  // Get current clock time
  long dt = curr_time - last_pid_time;

  if (dt >= PID_INT) {
    // Calculate motor speeds
    long curr_enc_l = motor_l.readEncoder();
    long curr_enc_r = motor_r.readEncoder();

    long speed_meas_l = (1000 * (curr_enc_l - last_enc_l)) / dt;
    long speed_meas_r = (1000 * (curr_enc_r - last_enc_r)) / dt;

    // Calculate error signals
    long err_l = speed_l - speed_meas_l;
    long err_r = speed_r - speed_meas_r;

    // Update error integrals, with anti-windup correction.
    err_int_l = err_int_l + err_l * dt;
    err_int_r = err_int_r + err_r * dt;

    // Calculate control signals
    int u_P_l = K_P * err_l;
    int u_P_r = K_P * err_r;
    int u_I_l = K_I * err_int_l;
    int u_I_r = K_I * err_int_r;
    int u_D_l = K_D * ((speed_meas_l - last_speed_l) / dt);
    int u_D_r = K_D * ((speed_meas_r - last_speed_r) / dt);

    int u_l = constrain(int(u_P_l + u_I_l + u_D_l), -255, 255);
    int u_r = constrain(int(u_P_r + u_I_r + u_D_r), -255, 255);

    // Update "memory" variables
    last_enc_l = curr_enc_l;
    last_enc_r = curr_enc_r;
    last_pid_time = curr_time;
    last_speed_l = speed_meas_l;
    last_speed_r = speed_meas_r;

    // Set motor speeds
    motor_l.on(u_l);
    motor_r.on(u_r);

    // Report values to Serial Monitor
    if (report && (curr_time - last_pid_rep_time) > PID_REP_INT) {
      Serial.print("time:");
      Serial.print(curr_time);
      Serial.print(", v_meas_l:");
      Serial.print(speed_meas_l);
      Serial.print(", v_meas_r:");
      Serial.print(speed_meas_r);
      Serial.print(", voltage_l:");
      Serial.print(u_l);
      Serial.print(", voltage_r:");
      Serial.println(u_r);

      last_pid_rep_time = curr_time;
    }
  }
}

// Reset PID "memory" terms
void resetPID() {
  last_speed_l = 0;
  last_speed_r = 0;
  err_int_l = 0;
  err_int_r = 0;
  last_enc_l = motor_l.readEncoder();
  last_enc_r = motor_r.readEncoder();
  last_pid_time = millis();
}