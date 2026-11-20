/**
 * ESP32-Community: Autonomous Obstacle Avoidance Rover
 * 
 * Hardware:
 *   - ESP32 DevKit (or S3 / C3)
 *   - HC-SR04 Ultrasonic Distance Sensor
 *   - SG90 Micro Servo Motor (Sensor Pan Mount)
 *   - L298N / L9110S Dual DC Motor H-Bridge Driver
 *   - 2WD / 4WD Smart Robot Chassis with TT Gearmotors
 *   - 2x 18650 Li-ion Batteries (7.4V) + LM2596 5V Buck Regulator
 *
 * Pin Connections:
 *   HC-SR04 Ultrasonic:
 *     - TRIG -> GPIO 5
 *     - ECHO -> GPIO 18 (via 1k/2k resistor voltage divider to 3.3V)
 *     - VCC  -> 5V
 *     - GND  -> GND
 *
 *   SG90 Servo:
 *     - Signal (PWM) -> GPIO 19
 *     - VCC          -> 5V
 *     - GND          -> GND
 *
 *   Motor Driver (L298N):
 *     - Left Motor Forward (IN1)   -> GPIO 26
 *     - Left Motor Reverse (IN2)   -> GPIO 27
 *     - Right Motor Forward (IN3)  -> GPIO 32
 *     - Right Motor Reverse (IN4)  -> GPIO 33
 */

#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_TRIG        5
#define PIN_ECHO        18
#define PIN_SERVO       19

#define PIN_MOTOR_L_FWD 26
#define PIN_MOTOR_L_REV 27
#define PIN_MOTOR_R_FWD 32
#define PIN_MOTOR_R_REV 33

#define DISTANCE_THRESHOLD_CM 25
#define SERVO_CENTER          90
#define SERVO_LEFT            150
#define SERVO_RIGHT           30

Servo scanServo;

// Measure distance in centimeters
int readDistanceCM() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH, 30000); // 30ms timeout (~5 meters max)
  if (duration == 0) return 999; // No echo received
  return (int)(duration * 0.0343 / 2.0);
}

// Motor Control Primitives
void moveForward() {
  digitalWrite(PIN_MOTOR_L_FWD, HIGH);
  digitalWrite(PIN_MOTOR_L_REV, LOW);
  digitalWrite(PIN_MOTOR_R_FWD, HIGH);
  digitalWrite(PIN_MOTOR_R_REV, LOW);
}

void moveBackward() {
  digitalWrite(PIN_MOTOR_L_FWD, LOW);
  digitalWrite(PIN_MOTOR_L_REV, HIGH);
  digitalWrite(PIN_MOTOR_R_FWD, LOW);
  digitalWrite(PIN_MOTOR_R_REV, HIGH);
}

void turnLeft() {
  digitalWrite(PIN_MOTOR_L_FWD, LOW);
  digitalWrite(PIN_MOTOR_L_REV, HIGH);
  digitalWrite(PIN_MOTOR_R_FWD, HIGH);
  digitalWrite(PIN_MOTOR_R_REV, LOW);
}

void turnRight() {
  digitalWrite(PIN_MOTOR_L_FWD, HIGH);
  digitalWrite(PIN_MOTOR_L_REV, LOW);
  digitalWrite(PIN_MOTOR_R_FWD, LOW);
  digitalWrite(PIN_MOTOR_R_REV, HIGH);
}

void stopMotors() {
  digitalWrite(PIN_MOTOR_L_FWD, LOW);
  digitalWrite(PIN_MOTOR_L_REV, LOW);
  digitalWrite(PIN_MOTOR_R_FWD, LOW);
  digitalWrite(PIN_MOTOR_R_REV, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- [ESP32-Community] Autonomous Obstacle Avoidance Rover ---");

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  pinMode(PIN_MOTOR_L_FWD, OUTPUT);
  pinMode(PIN_MOTOR_L_REV, OUTPUT);
  pinMode(PIN_MOTOR_R_FWD, OUTPUT);
  pinMode(PIN_MOTOR_R_REV, OUTPUT);
  stopMotors();

  // Attach servo to GPIO 19
  ESP32PWM::allocateTimer(0);
  scanServo.setPeriodHertz(50);
  scanServo.attach(PIN_SERVO, 500, 2400);
  scanServo.write(SERVO_CENTER);
  delay(500);

  Serial.println("[Rover] System initialized. Entering autonomous navigation loop...");
}

void loop() {
  scanServo.write(SERVO_CENTER);
  delay(50);

  int forwardDistance = readDistanceCM();
  Serial.printf("[Sensors] Forward Distance: %d cm\n", forwardDistance);

  if (forwardDistance > DISTANCE_THRESHOLD_CM) {
    moveForward();
    delay(60);
  } else {
    // Obstacle detected ahead!
    Serial.println("[Obstacle] Too close! Stopping and evaluating alternatives...");
    stopMotors();
    delay(200);

    // Back up slightly
    moveBackward();
    delay(250);
    stopMotors();
    delay(150);

    // Look Left
    scanServo.write(SERVO_LEFT);
    delay(350);
    int leftDistance = readDistanceCM();
    Serial.printf("[Scan] Left Distance: %d cm\n", leftDistance);

    // Look Right
    scanServo.write(SERVO_RIGHT);
    delay(400);
    int rightDistance = readDistanceCM();
    Serial.printf("[Scan] Right Distance: %d cm\n", rightDistance);

    // Return servo to center
    scanServo.write(SERVO_CENTER);
    delay(200);

    // Decision Logic
    if (leftDistance > rightDistance && leftDistance > DISTANCE_THRESHOLD_CM) {
      Serial.println("[Decision] Turning Left >>>");
      turnLeft();
      delay(400);
    } else if (rightDistance > DISTANCE_THRESHOLD_CM) {
      Serial.println("[Decision] Turning Right >>>");
      turnRight();
      delay(400);
    } else {
      Serial.println("[Decision] Dead end! 180° Spin Around >>>");
      turnRight();
      delay(750);
    }

    stopMotors();
    delay(100);
  }
}
