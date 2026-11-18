/*
 * ESP32-Community | Games Project 02: Retro Snake with NVS High-Score
 * 
 * Classic Nokia 3310-style Snake game on an I2C SSD1306 OLED (128x64 display).
 * Features persistent high-score saving to ESP32 Non-Volatile Storage (NVS/Preferences),
 * intuitive 2-button rotational steering (Left / Right), and sound effects.
 * 
 * Part of ESP32-Community Hub: https://github.com/tushar2704/ESP32-Community
 * Video Tutorials & Community: @IamNotIntoTech | @Makerrdesigns
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Preferences prefs;

// Pushbutton Inputs (Internal pull-ups enabled)
const int BTN_LEFT = 18;   // Turn Left
const int BTN_RIGHT = 19;  // Turn Right
const int BUZZER_PIN = 23;

// Snake Grid Settings
const int BLOCK_SIZE = 4;
const int GRID_W = SCREEN_WIDTH / BLOCK_SIZE;   // 32 columns
const int GRID_H = SCREEN_HEIGHT / BLOCK_SIZE;  // 16 rows

// Direction enumeration: 0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT
enum Direction { DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3 };
Direction currentDir = DIR_RIGHT;

struct Point {
  int8_t x;
  int8_t y;
};

const int MAX_SNAKE_LENGTH = 120;
Point snake[MAX_SNAKE_LENGTH];
int snakeLength = 4;
Point food;

int currentScore = 0;
int highScore = 0;
bool isGameOver = false;

void beep(int freq, int durationMs) {
  tone(BUZZER_PIN, freq, durationMs);
}

void spawnFood() {
  bool onSnake;
  do {
    onSnake = false;
    food.x = random(1, GRID_W - 1);
    food.y = random(2, GRID_H - 1); // Row 0-1 reserved for score header
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        onSnake = true;
        break;
      }
    }
  } while (onSnake);
}

void resetGame() {
  snakeLength = 4;
  currentDir = DIR_RIGHT;
  currentScore = 0;
  isGameOver = false;

  int startX = GRID_W / 4;
  int startY = GRID_H / 2;
  for (int i = 0; i < snakeLength; i++) {
    snake[i].x = startX - i;
    snake[i].y = startY;
  }
  spawnFood();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Load High Score from ESP32 NVS flash storage
  prefs.begin("snake-game", false);
  highScore = prefs.getInt("high_score", 0);

  // Initialize I2C OLED (SDA=21, SCL=22)
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("[ERROR] SSD1306 allocation failed! Check I2C wiring.");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(25, 20);
  display.println("ESP32 RETRO SNAKE");
  display.setCursor(20, 36);
  display.printf("High Score: %d", highScore);
  display.display();
  delay(1500);

  resetGame();
}

void loop() {
  if (isGameOver) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(35, 15);
    display.println("GAME OVER!");
    display.setCursor(25, 30);
    display.printf("Score: %d  High: %d", currentScore, highScore);
    display.setCursor(15, 48);
    display.println("Press Button to Play");
    display.display();

    if (digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
      delay(300); // debounce
      resetGame();
    }
    delay(50);
    return;
  }

  // Handle Rotational Steering Inputs
  static bool lastBtnLeft = HIGH;
  static bool lastBtnRight = HIGH;

  bool btnLeft = digitalRead(BTN_LEFT);
  bool btnRight = digitalRead(BTN_RIGHT);

  if (btnLeft == LOW && lastBtnLeft == HIGH) {
    // Turn Left (Counter-Clockwise: UP -> LEFT -> DOWN -> RIGHT)
    currentDir = (Direction)((currentDir + 3) % 4);
    beep(600, 15);
  }
  if (btnRight == LOW && lastBtnRight == HIGH) {
    // Turn Right (Clockwise: UP -> RIGHT -> DOWN -> LEFT)
    currentDir = (Direction)((currentDir + 1) % 4);
    beep(600, 15);
  }
  lastBtnLeft = btnLeft;
  lastBtnRight = btnRight;

  // Move Snake Body
  Point nextHead = snake[0];
  switch (currentDir) {
    case DIR_UP:    nextHead.y--; break;
    case DIR_RIGHT: nextHead.x++; break;
    case DIR_DOWN:  nextHead.y++; break;
    case DIR_LEFT:  nextHead.x--; break;
  }

  // Wall Collision Detection (Boundaries)
  if (nextHead.x < 0 || nextHead.x >= GRID_W || nextHead.y < 2 || nextHead.y >= GRID_H) {
    isGameOver = true;
    beep(200, 300);
  }

  // Self Collision Detection
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i].x == nextHead.x && snake[i].y == nextHead.y) {
      isGameOver = true;
      beep(200, 300);
      break;
    }
  }

  if (isGameOver) {
    // Save new High Score to NVS if beaten
    if (currentScore > highScore) {
      highScore = currentScore;
      prefs.putInt("high_score", highScore);
      Serial.printf("[NVS] New High Score Saved: %d\n", highScore);
    }
    return;
  }

  // Check if eating food
  bool ateFood = (nextHead.x == food.x && nextHead.y == food.y);

  // Shift body segments
  int tailIndex = ateFood ? snakeLength : snakeLength - 1;
  for (int i = tailIndex; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0] = nextHead;

  if (ateFood) {
    if (snakeLength < MAX_SNAKE_LENGTH - 1) {
      snakeLength++;
    }
    currentScore += 10;
    beep(1200, 40);
    spawnFood();
  }

  // Render Frame
  display.clearDisplay();

  // Top Status Bar (Score & High Score)
  display.setTextSize(1);
  display.setCursor(2, 0);
  display.printf("Score:%d", currentScore);
  display.setCursor(75, 0);
  display.printf("Hi:%d", highScore);
  display.drawFastHLine(0, 8, SCREEN_WIDTH, SSD1306_WHITE);

  // Draw Food
  display.fillRect(food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);

  // Draw Snake
  for (int i = 0; i < snakeLength; i++) {
    if (i == 0) {
      // Snake Head solid
      display.fillRect(snake[i].x * BLOCK_SIZE, snake[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
    } else {
      // Snake Body rounded
      display.drawRect(snake[i].x * BLOCK_SIZE, snake[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SSD1306_WHITE);
    }
  }

  display.display();

  // Game speed accelerates slightly as score grows
  int frameDelay = max(40, 110 - (currentScore / 5));
  delay(frameDelay);
}
