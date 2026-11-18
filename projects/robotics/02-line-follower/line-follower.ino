/*
 * ESP32-Community | Robotics Project 02: High-Speed PID Line Follower Robot
 * 
 * Implements a 5-channel infrared sensor array (TCRT5000) line follower
 * using a Proportional-Integral-Derivative (PID) feedback loop for
 * ultra-smooth curve navigation and drift correction.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>

// 5-Channel Infrared Sensor Array Pins (Digital or Analog)
// Sensor logic: 1 = Black Line, 0 = White Floor (or vice-versa depending on jumper)
const int SENSOR_PIN_1 = 34; // Far Left
const int SENSOR_PIN_2 = 35; // Left
const int SENSOR_PIN_3 = 32; // Center
const int SENSOR_PIN_4 = 33; // Right
const int SENSOR_PIN_5 = 25; // Far Right

// Motor Driver Pins (L298N / TB6612FNG)
const int ENA_PIN = 14;   // Speed Enable Left (PWM)
const int IN1_PIN = 27;   // Forward Left
const int IN2_PIN = 26;   // Reverse Left

const int ENB_PIN = 12;   // Speed Enable Right (PWM)
const int IN3_PIN = 13;   // Forward Right
const int IN4_PIN = 15;   // Reverse Right

// PWM Settings
const int PWM_FREQ = 2000;
const int PWM_RES = 8;
const int PWM_CH_A = 0;
const int PWM_CH_B = 1;

// Base driving speeds
const int BASE_SPEED = 180;
const int MAX_SPEED = 240;

// PID Tuning Constants
float Kp = 25.0;  // Proportional gain
float Ki = 0.0;   // Integral gain
float Kd = 15.0;  // Derivative gain

// PID Internal Variables
float lastError = 0;
float integral = 0;

void setMotors(int leftSpeed, int rightSpeed) {
  // Constrain speeds
  leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

  // Left Motor Direction
  if (leftSpeed >= 0) {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
  } else {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    leftSpeed = -leftSpeed;
  }

  // Right Motor Direction
  if (rightSpeed >= 0) {
    digitalWrite(IN3_PIN, HIGH);
    digitalWrite(IN4_PIN, LOW);
  } else {
    digitalWrite(IN3_PIN, LOW);
    digitalWrite(IN4_PIN, HIGH);
    rightSpeed = -rightSpeed;
  }

  // Set PWM Duty
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcWrite(ENA_PIN, leftSpeed);
  ledcWrite(ENB_PIN, rightSpeed);
#else
  ledcWrite(PWM_CH_A, leftSpeed);
  ledcWrite(PWM_CH_B, rightSpeed);
#endif
}

// Calculate position error from sensor array (-2.0 to +2.0)
float readSensorsError() {
  int s1 = digitalRead(SENSOR_PIN_1);
  int s2 = digitalRead(SENSOR_PIN_2);
  int s3 = digitalRead(SENSOR_PIN_3);
  int s4 = digitalRead(SENSOR_PIN_4);
  int s5 = digitalRead(SENSOR_PIN_5);

  // Black line on white surface detection (active HIGH)
  // Weights: Far Left: -2, Left: -1, Center: 0, Right: +1, Far Right: +2
  int sum = s1 + s2 + s3 + s4 + s5;
  if (sum == 0) {
    // Line lost! Maintain last known direction
    return (lastError > 0) ? 2.5 : -2.5;
  }

  float weightedSum = (s1 * -2.0) + (s2 * -1.0) + (s3 * 0.0) + (s4 * 1.0) + (s5 * 2.0);
  return weightedSum / sum;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure Sensors
  pinMode(SENSOR_PIN_1, INPUT);
  pinMode(SENSOR_PIN_2, INPUT);
  pinMode(SENSOR_PIN_3, INPUT);
  pinMode(SENSOR_PIN_4, INPUT);
  pinMode(SENSOR_PIN_5, INPUT);

  // Configure Motors
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcAttach(ENA_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(ENB_PIN, PWM_FREQ, PWM_RES);
#else
  ledcSetup(PWM_CH_A, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA_PIN, PWM_CH_A);
  ledcSetup(PWM_CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENB_PIN, PWM_CH_B);
#endif

  setMotors(0, 0);

  Serial.println("==================================================");
  Serial.println("  ESP32 Community: 5-Channel PID Line Follower    ");
  Serial.println("==================================================");
  Serial.println("Place robot on track. Starting in 2 seconds...");
  delay(2000);
}

void loop() {
  float error = readSensorsError();

  // PID calculations
  float P = error;
  integral += error;
  integral = constrain(integral, -50.0, 50.0); // Anti-windup
  float I = integral;
  float D = error - lastError;
  lastError = error;

  float correction = (Kp * P) + (Ki * I) + (Kd * D);

  // Calculate differential motor speeds
  int leftMotorSpeed = BASE_SPEED + (int)correction;
  int rightMotorSpeed = BASE_SPEED - (int)correction;

  setMotors(leftMotorSpeed, rightMotorSpeed);

  // Telemetry log every 200ms
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 200) {
    lastLog = millis();
    Serial.printf("[PID] Err: %+.2f | Corr: %+.1f | L_Speed: %3d | R_Speed: %3d\n",
                  error, correction, leftMotorSpeed, rightMotorSpeed);
  }

  delay(5); // Loop runs at ~200Hz
}
