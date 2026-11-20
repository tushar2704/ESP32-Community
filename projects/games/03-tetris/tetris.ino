/**
 * ESP32-Community: 60 FPS Retro OLED Tetris
 * 
 * Hardware:
 *   - ESP32 DevKit (or S3 / C3)
 *   - 0.96" SSD1306 I2C OLED (128x64)
 *   - 4x Tactile Pushbuttons (Internal Pull-Up)
 *   - 1x Passive Piezo Buzzer
 *
 * Pin Connections:
 *   - OLED SDA -> GPIO 21
 *   - OLED SCL -> GPIO 22
 *   - Button Left   -> GPIO 18 (GND on press)
 *   - Button Right  -> GPIO 19 (GND on press)
 *   - Button Rotate -> GPIO 23 (GND on press)
 *   - Button Drop   -> GPIO 5  (GND on press)
 *   - Piezo Buzzer  -> GPIO 25
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

#define PIN_BTN_LEFT   18
#define PIN_BTN_RIGHT  19
#define PIN_BTN_ROTATE 23
#define PIN_BTN_DROP   5
#define PIN_BUZZER     25

#define GRID_WIDTH  10
#define GRID_HEIGHT 20
#define BLOCK_SIZE  3
#define BOARD_X     16
#define BOARD_Y     2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Preferences prefs;

// Board representation (0 = empty, 1 = occupied)
uint8_t board[GRID_HEIGHT][GRID_WIDTH] = {0};

// 7 Standard Tetromino shapes (4x4 grids)
const uint16_t SHAPES[7][4] = {
  // I
  { 0x0F00, 0x2222, 0x00F0, 0x4444 },
  // J
  { 0x8E00, 0x6440, 0x0E20, 0x44C0 },
  // L
  { 0x2E00, 0x4460, 0x0E80, 0xC440 },
  // O
  { 0x6600, 0x6600, 0x6600, 0x6600 },
  // S
  { 0x6C00, 0x4620, 0x06C0, 0x8C40 },
  // T
  { 0x4E00, 0x4640, 0x0E40, 0x4C40 },
  // Z
  { 0xC600, 0x2640, 0x0C60, 0x4C80 }
};

int curPiece = 0;
int curRotation = 0;
int curX = 3;
int curY = 0;
uint32_t score = 0;
uint32_t highScore = 0;
uint16_t linesCleared = 0;
uint32_t dropInterval = 600;
uint32_t lastDrop = 0;
bool gameOver = false;

// Audio tone helper
void playTone(int freq, int durationMs) {
  #if defined(ESP32)
  tone(PIN_BUZZER, freq, durationMs);
  #endif
}

bool checkCollision(int piece, int rot, int px, int py) {
  uint16_t shape = SHAPES[piece][rot];
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (shape & (0x8000 >> (row * 4 + col))) {
        int x = px + col;
        int y = py + row;
        if (x < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT) {
          return true;
        }
        if (y >= 0 && board[y][x]) {
          return true;
        }
      }
    }
  }
  return false;
}

void lockPiece() {
  uint16_t shape = SHAPES[curPiece][curRotation];
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (shape & (0x8000 >> (row * 4 + col))) {
        int x = curX + col;
        int y = curY + row;
        if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH) {
          board[y][x] = 1;
        }
      }
    }
  }

  // Check for line clears
  int cleared = 0;
  for (int row = GRID_HEIGHT - 1; row >= 0; row--) {
    bool full = true;
    for (int col = 0; col < GRID_WIDTH; col++) {
      if (!board[row][col]) {
        full = false;
        break;
      }
    }
    if (full) {
      cleared++;
      for (int r = row; r > 0; r--) {
        for (int c = 0; c < GRID_WIDTH; c++) {
          board[r][c] = board[r - 1][c];
        }
      }
      for (int c = 0; c < GRID_WIDTH; c++) board[0][c] = 0;
      row++; // Check same row index again after shift
    }
  }

  if (cleared > 0) {
    linesCleared += cleared;
    uint32_t pts = (cleared == 1) ? 100 : (cleared == 2) ? 300 : (cleared == 3) ? 500 : 800;
    score += pts;
    if (score > highScore) {
      highScore = score;
      prefs.putUInt("high", highScore);
    }
    playTone(1046, 120); // High C sound
    if (dropInterval > 150) dropInterval -= (cleared * 15);
  } else {
    playTone(330, 40);
  }

  // Spawn new piece
  curPiece = random(0, 7);
  curRotation = 0;
  curX = 3;
  curY = 0;

  if (checkCollision(curPiece, curRotation, curX, curY)) {
    gameOver = true;
    playTone(150, 400);
  }
}

void resetGame() {
  memset(board, 0, sizeof(board));
  score = 0;
  linesCleared = 0;
  dropInterval = 600;
  gameOver = false;
  curPiece = random(0, 7);
  curRotation = 0;
  curX = 3;
  curY = 0;
}

void drawGame() {
  display.clearDisplay();

  // Draw board border
  int bx = BOARD_X - 2;
  int by = BOARD_Y - 1;
  int bw = GRID_WIDTH * BLOCK_SIZE + 3;
  int bh = GRID_HEIGHT * BLOCK_SIZE + 2;
  display.drawRect(bx, by, bw, bh, SSD1306_WHITE);

  // Draw placed blocks
  for (int r = 0; r < GRID_HEIGHT; r++) {
    for (int c = 0; c < GRID_WIDTH; c++) {
      if (board[r][c]) {
        display.fillRect(BOARD_X + c * BLOCK_SIZE, BOARD_Y + r * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);
      }
    }
  }

  // Draw active piece
  if (!gameOver) {
    uint16_t shape = SHAPES[curPiece][curRotation];
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        if (shape & (0x8000 >> (row * 4 + col))) {
          int px = BOARD_X + (curX + col) * BLOCK_SIZE;
          int py = BOARD_Y + (curY + row) * BLOCK_SIZE;
          if (py >= BOARD_Y) {
            display.fillRect(px, py, BLOCK_SIZE - 1, BLOCK_SIZE - 1, SSD1306_WHITE);
          }
        }
      }
    }
  }

  // Draw stats sidebar
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(60, 4);
  display.print("TETRIS");

  display.setCursor(60, 18);
  display.printf("SCR:%u", score);

  display.setCursor(60, 30);
  display.printf("HI :%u", highScore);

  display.setCursor(60, 42);
  display.printf("LIN:%u", linesCleared);

  if (gameOver) {
    display.fillRect(10, 20, 108, 24, SSD1306_BLACK);
    display.drawRect(10, 20, 108, 24, SSD1306_WHITE);
    display.setCursor(24, 24);
    display.print("GAME OVER!");
    display.setCursor(14, 34);
    display.print("Press Any Button");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0) + esp_random());

  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_ROTATE, INPUT_PULLUP);
  pinMode(PIN_BTN_DROP, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize NVS Preferences
  prefs.begin("tetris", false);
  highScore = prefs.getUInt("high", 0);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("[Tetris] SSD1306 allocation failed.");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println("ESP32 TETRIS");
  display.setCursor(15, 36);
  display.println("Ready to Play!");
  display.display();
  delay(1200);

  resetGame();
}

void loop() {
  // Input handling
  bool btnLeft   = (digitalRead(PIN_BTN_LEFT) == LOW);
  bool btnRight  = (digitalRead(PIN_BTN_RIGHT) == LOW);
  bool btnRotate = (digitalRead(PIN_BTN_ROTATE) == LOW);
  bool btnDrop   = (digitalRead(PIN_BTN_DROP) == LOW);

  if (gameOver) {
    if (btnLeft || btnRight || btnRotate || btnDrop) {
      resetGame();
      delay(300);
    }
    drawGame();
    delay(50);
    return;
  }

  if (btnLeft && !checkCollision(curPiece, curRotation, curX - 1, curY)) {
    curX--;
    playTone(523, 20);
    delay(120);
  }
  if (btnRight && !checkCollision(curPiece, curRotation, curX + 1, curY)) {
    curX++;
    playTone(523, 20);
    delay(120);
  }
  if (btnRotate) {
    int nextRot = (curRotation + 1) % 4;
    if (!checkCollision(curPiece, nextRot, curX, curY)) {
      curRotation = nextRot;
      playTone(659, 25);
    }
    delay(150);
  }
  if (btnDrop) {
    while (!checkCollision(curPiece, curRotation, curX, curY + 1)) {
      curY++;
    }
    lockPiece();
    delay(150);
  }

  // Periodic gravity drop
  if (millis() - lastDrop >= dropInterval) {
    lastDrop = millis();
    if (!checkCollision(curPiece, curRotation, curX, curY + 1)) {
      curY++;
    } else {
      lockPiece();
    }
  }

  drawGame();
  delay(16); // Target ~60 FPS
}
