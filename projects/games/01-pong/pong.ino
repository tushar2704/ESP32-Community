/*
 * ESP32-Community | Games Project 01: Retro Pong for SSD1306 OLED
 * 
 * Classic 2D arcade Pong game running on an I2C SSD1306 OLED (128x64 display).
 * Features smooth 60 FPS physics, an adaptive AI opponent paddle,
 * dual pushbutton controls with internal pull-ups, and audio effects.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Controller Inputs (Internal pull-ups enabled)
const int BTN_UP = 18;
const int BTN_DOWN = 19;
const int BUZZER_PIN = 23;

// Game Dimensions
const int PADDLE_WIDTH = 3;
const int PADDLE_HEIGHT = 16;
const int BALL_SIZE = 3;

// Player Paddle (Left side)
int playerY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int playerScore = 0;

// AI Paddle (Right side)
int aiY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int aiScore = 0;
const int AI_SPEED = 2;

// Ball state
float ballX = SCREEN_WIDTH / 2;
float ballY = SCREEN_HEIGHT / 2;
float ballSpeedX = 2.0;
float ballSpeedY = 1.5;

void beep(int freq, int durationMs) {
  tone(BUZZER_PIN, freq, durationMs);
}

void resetBall(bool toPlayer) {
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballSpeedX = toPlayer ? -2.0 : 2.0;
  ballSpeedY = (random(0, 2) == 0 ? 1.0 : -1.0) * (random(10, 25) / 10.0);
  beep(400, 150);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize I2C for SSD1306 (SDA = GPIO 21, SCL = GPIO 22 on classic ESP32)
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("[ERROR] SSD1306 allocation failed! Check I2C wiring (SDA=21, SCL=22).");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 20);
  display.println("ESP32 RETRO PONG");
  display.setCursor(15, 36);
  display.println("@IamNotIntoTech");
  display.display();
  delay(1500);

  resetBall(true);
}

void loop() {
  // Read Player Input (Active LOW)
  if (digitalRead(BTN_UP) == LOW) {
    playerY -= 3;
  }
  if (digitalRead(BTN_DOWN) == LOW) {
    playerY += 3;
  }
  playerY = constrain(playerY, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  // AI Paddle Logic (Follows ball Y with slight delay)
  int aiCenter = aiY + (PADDLE_HEIGHT / 2);
  if (aiCenter < ballY - 2) {
    aiY += AI_SPEED;
  } else if (aiCenter > ballY + 2) {
    aiY -= AI_SPEED;
  }
  aiY = constrain(aiY, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

  // Update Ball Position
  ballX += ballSpeedX;
  ballY += ballSpeedY;

  // Ceiling and Floor Bounce
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
    ballSpeedY = -ballSpeedY;
    beep(800, 20);
  }

  // Player Paddle Collision (Left: x = 2 to 2 + PADDLE_WIDTH)
  if (ballX <= 2 + PADDLE_WIDTH && ballX >= 2) {
    if (ballY + BALL_SIZE >= playerY && ballY <= playerY + PADDLE_HEIGHT) {
      ballSpeedX = -ballSpeedX * 1.05; // Slightly increase speed on volley
      ballSpeedX = constrain(ballSpeedX, -4.5, 4.5);
      // Angle reflection based on where ball hits paddle
      float hitOffset = (ballY + (BALL_SIZE / 2)) - (playerY + (PADDLE_HEIGHT / 2));
      ballSpeedY = hitOffset * 0.25;
      beep(1200, 30);
    }
  }

  // AI Paddle Collision (Right: x = SCREEN_WIDTH - 2 - PADDLE_WIDTH)
  if (ballX + BALL_SIZE >= SCREEN_WIDTH - 2 - PADDLE_WIDTH && ballX + BALL_SIZE <= SCREEN_WIDTH - 2) {
    if (ballY + BALL_SIZE >= aiY && ballY <= aiY + PADDLE_HEIGHT) {
      ballSpeedX = -ballSpeedX * 1.05;
      ballSpeedX = constrain(ballSpeedX, -4.5, 4.5);
      float hitOffset = (ballY + (BALL_SIZE / 2)) - (aiY + (PADDLE_HEIGHT / 2));
      ballSpeedY = hitOffset * 0.25;
      beep(1200, 30);
    }
  }

  // Score Tracking: Player Missed
  if (ballX < 0) {
    aiScore++;
    resetBall(true);
  }
  // Score Tracking: AI Missed
  if (ballX > SCREEN_WIDTH) {
    playerScore++;
    resetBall(false);
  }

  // Render Frame
  display.clearDisplay();

  // Draw Center Net
  for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
    display.drawFastVLine(SCREEN_WIDTH / 2, y, 2, SSD1306_WHITE);
  }

  // Draw Scores
  display.setTextSize(1);
  display.setCursor(SCREEN_WIDTH / 2 - 20, 4);
  display.print(playerScore);
  display.setCursor(SCREEN_WIDTH / 2 + 14, 4);
  display.print(aiScore);

  // Draw Paddles
  display.fillRect(2, playerY, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  display.fillRect(SCREEN_WIDTH - 2 - PADDLE_WIDTH, aiY, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);

  // Draw Ball
  display.fillRect((int)ballX, (int)ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);

  display.display();
  delay(16); // ~60 FPS
}
