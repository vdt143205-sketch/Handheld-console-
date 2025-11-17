// ==== THƯ VIỆN ====
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ==== CHÂN KẾT NỐI ====
#define TFT_CS    10
#define TFT_DC    9
#define TFT_RST   12
#define BUZZER    A2

// ==== NÚT BẤM (Đã đổi B và D) ====
#define BTN_A 2   // Lên (Menu/Game)
#define BTN_B 3   // Phải (Game)
#define BTN_C 4   // Xuống (Menu/Game)
#define BTN_D 5   // Trái (Game)
#define BTN_E 6   // Chọn (Menu) / Thoát (Game)
#define BTN_F 7   // Bắn (Game 1) / Boost (Game 2)

// ==== KHỞI TẠO MÀN HÌNH ====
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// ==== BỘ QUẢN LÝ TRẠNG THÁI ====
enum GameState {
  STATE_MENU,
  STATE_LEVEL_SELECT, // <-- THAY ĐỔI: Thêm trạng thái chọn level
  STATE_GAME_SHOOTER,
  STATE_GAME_SNAKE,
  STATE_GAME_PONG,
  STATE_GAME_OVER
};
GameState currentState = STATE_MENU;

// ==== BIẾN NÚT BẤM TOÀN CỤC (TỐI ƯU HÓA) ====
bool btnA, btnB, btnC, btnD, btnE, btnF;

// ==== BIẾN MENU ====
int selectedGame = 0; // Game nào đang được chọn ở Menu (0, 1, 2)
const int numGames = 3; // <-- THAY ĐỔI: Quay lại 3 game
int selectedLevel = 0; // Level nào đang được chọn ở màn hình Level (0, 1, 2, 3)
int currentLevel = 0; // Level Dễ(0), TB(1), Khó(2) SẼ dùng để bắt đầu game

// ==== BIẾN GAME (Giữ nguyên) ====
int playerX, playerY, enemyX, enemyY;
int bulletX, bulletY;
bool bulletActive;
int score;
const int PLAYER_W = 15, PLAYER_H = 15;
const int ENEMY_W = 15, ENEMY_H = 15;
const int GAME_X1 = 0, GAME_Y1 = 20;
const int GAME_W = 240;
const int GAME_H = 220;
const int GRID_SIZE = 10;
const int MAX_W = 24;
const int MAX_H = 24;
int snakeX[100], snakeY[100];
int snakeLength;
int foodX, foodY;
int snakeDir;
int snakeScore;
const int PADDLE_W = 5, PADDLE_H = 40;
int p1Y, p2Y;
int ballX, ballY, ballVX, ballVY;
int scoreP1, scoreP2;
const int SCREEN_W = 240;
const int SCREEN_H = 240;
const int WIN_SCORE = 5;
int finalScore = 0;
String gameName = "";


void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_F, INPUT_PULLUP);

  tft.init(240, 240);

  // <-- THAY ĐỔI: XOAY NGANG NGƯỢC LẠI -->
  tft.setRotation(3);
  // <-- KẾT THÚC THAY ĐỔI -->

  drawMenu();
}

// (Hàm readGlobalButtons giữ nguyên)
void readGlobalButtons() {
  btnA = !digitalRead(BTN_A);
  btnB = !digitalRead(BTN_B);
  btnC = !digitalRead(BTN_C);
  btnD = !digitalRead(BTN_D);
  btnE = !digitalRead(BTN_E);
  btnF = !digitalRead(BTN_F);
}

// (Hàm loop() chính ĐÃ THÊM TRẠNG THÁI MỚI)
void loop() {
  readGlobalButtons();

  // Logic Thoát Game (Giữ nguyên)
  if (currentState == STATE_GAME_SHOOTER || currentState == STATE_GAME_SNAKE || currentState == STATE_GAME_PONG) {
    if (btnE) {
      tone(BUZZER, 800, 150);
      while (!digitalRead(BTN_E)) { }
      currentState = STATE_MENU;
      drawMenu();
      return;
    }
  }

  // Xử lý logic theo từng trạng thái
  switch (currentState) {
    case STATE_MENU: loopMenu(); break;
    case STATE_LEVEL_SELECT: loopLevelSelect(); break; // <-- THAY ĐỔI
    case STATE_GAME_SHOOTER: loopShooter(); break;
    case STATE_GAME_SNAKE: loopSnake(); break;
    case STATE_GAME_PONG: loopPong(); break;
    case STATE_GAME_OVER: loopGameOver(); break;
  }
}


// (Hàm Căn Giữa giữ nguyên)
void drawCenteredText(String text, int y, int size) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(size); 
  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = (tft.width() - w) / 2;
  tft.setCursor(x, y);
  tft.print(text);
}


// =======================================================
// ===== PHẦN 1: MENU CHÍNH (ĐÃ SỬA LẠI) ================
// =======================================================
void drawMenu() {
  tft.fillScreen(ST77XX_BLACK);
  
  // Tiêu đề
  tft.setTextSize(2); 
  tft.setTextColor(ST77XX_WHITE);
  drawCenteredText("MAY CHOI GAME", 30, 2); 
  
  tft.drawFastHLine(40, 55, 160, ST77XX_YELLOW);
  
  // Menu items (Quay lại 3 game)
  tft.setTextSize(2); 
  int menuX = 40;
  int menuY = 90;
  int lineHeight = 30; 

  if (selectedGame == 0) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY);
  tft.print(selectedGame == 0 ? "> Game 1: Shooter" : "  Game 1: Shooter");

  if (selectedGame == 1) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY + lineHeight);
  tft.print(selectedGame == 1 ? "> Game 2: Snake" : "  Game 2: Snake");

  if (selectedGame == 2) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY + 2 * lineHeight);
  tft.print(selectedGame == 2 ? "> Game 3: Pong" : "  Game 3: Pong");

  // Hướng dẫn (Đã bỏ chữ "Doi Level")
  tft.setTextSize(1); 
  tft.setTextColor(ST77XX_CYAN);
  drawCenteredText("A/C: Chon, E: Bat dau", 200, 1); 
  drawCenteredText("Trong game: Nhan E de thoat", 215, 1);
}

// (Hàm loopMenu đã SỬA LẠI)
void loopMenu() {
  static unsigned long lastButtonPress = 0;
  const long menuDebounceTime = 200;
  if (millis() - lastButtonPress < menuDebounceTime) {
    return;
  }
  if (btnA) { // Lên
    lastButtonPress = millis();
    selectedGame--;
    if (selectedGame < 0) selectedGame = numGames - 1;
    drawMenu();
  }
  else if (btnC) { // Xuống
    lastButtonPress = millis();
    selectedGame++;
    if (selectedGame >= numGames) selectedGame = 0;
    drawMenu();
  }
  else if (btnE) { // Chọn
    lastButtonPress = millis();
    tone(BUZZER, 1200, 100);
    while (!digitalRead(BTN_E)) { delay(10); }

    // <-- THAY ĐỔI: Chuyển sang màn hình CHỌN LEVEL -->
    // (Biến selectedGame (0, 1, 2) đã được lưu)
    selectedLevel = 0; // Reset con trỏ level về "Dễ"
    currentState = STATE_LEVEL_SELECT;
    drawLevelSelect(); // Vẽ màn hình mới
    return;
    // <-- KẾT THÚC THAY ĐỔI -->
  }
}

// =======================================================
// ===== PHẦN MỚI: MÀN HÌNH CHỌN LEVEL ===================
// =======================================================

// Hàm MỚI: Vẽ màn hình chọn level
void drawLevelSelect() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  drawCenteredText("CHON CAP DO", 30, 2);

  // Hiển thị tên Game đang chọn
  String gameName;
  if (selectedGame == 0) gameName = "Game: Shooter";
  else if (selectedGame == 1) gameName = "Game: Snake";
  else if (selectedGame == 2) gameName = "Game: Pong";
  tft.setTextColor(ST77XX_CYAN);
  drawCenteredText(gameName, 60, 2);

  int menuX = 60;
  int menuY = 110;
  int lineHeight = 30;

  // Option 0: Dễ
  if (selectedLevel == 0) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY);
  tft.print(selectedLevel == 0 ? "> 1. De" : "  1. De");
  
  // Option 1: Trung Bình
  if (selectedLevel == 1) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY + lineHeight);
  tft.print(selectedLevel == 1 ? "> 2. Trung Binh" : "  2. Trung Binh");

  // Option 2: Khó
  if (selectedLevel == 2) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY + 2 * lineHeight);
  tft.print(selectedLevel == 2 ? "> 3. Kho" : "  3. Kho");

  // Option 3: Quay Lại
  if (selectedLevel == 3) tft.setTextColor(ST77XX_YELLOW);
  else tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(menuX, menuY + 3 * lineHeight);
  tft.print(selectedLevel == 3 ? "> 4. Quay Lai Menu" : "  4. Quay Lai Menu");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_CYAN);
  drawCenteredText("A/C: Chon, E: Bat dau", 215, 1);
}

// Hàm MỚI: Logic màn hình chọn level
void loopLevelSelect() {
  static unsigned long lastButtonPress = 0;
  const long menuDebounceTime = 200;
  if (millis() - lastButtonPress < menuDebounceTime) {
    return;
  }

  if (btnA) { // Lên
    lastButtonPress = millis();
    selectedLevel--;
    if (selectedLevel < 0) selectedLevel = 3; // 3 là "Quay Lại"
    drawLevelSelect();
  }
  else if (btnC) { // Xuống
    lastButtonPress = millis();
    selectedLevel++;
    if (selectedLevel > 3) selectedLevel = 0;
    drawLevelSelect();
  }
  else if (btnE) { // Chọn
    lastButtonPress = millis();
    tone(BUZZER, 1200, 100);
    while (!digitalRead(BTN_E)) { delay(10); }

    if (selectedLevel == 3) { // 3 là "Quay Lại"
      currentState = STATE_MENU;
      drawMenu();
      return;
    }

    // Nếu không, hãy đặt cấp độ và bắt đầu game
    currentLevel = selectedLevel; // Đặt cấp độ toàn cục

    // Bắt đầu game đã chọn (selectedGame)
    if (selectedGame == 0) {
        currentState = STATE_GAME_SHOOTER;
        setupShooter();
    } else if (selectedGame == 1) {
        currentState = STATE_GAME_SNAKE;
        setupSnake();
    } else if (selectedGame == 2) {
        currentState = STATE_GAME_PONG;
        setupPong();
    }
    return;
  }
}


// =======================================================
// ===== PHẦN 2: GAME 1 - SHOOTER (Giữ nguyên) =========
// =======================================================
// (Toàn bộ logic game giữ nguyên, nó sẽ tự động dùng
// biến 'currentLevel' đã được đặt ở màn hình Level Select)

void drawPlayer(int x, int y, uint16_t color) {
  tft.fillRect(x, y, PLAYER_W, PLAYER_H, color);
}
void drawEnemy(int x, int y, uint16_t color) {
  tft.fillRect(x, y, ENEMY_W, ENEMY_H, color);
}
void setupShooter() {
  tft.fillScreen(ST77XX_BLACK);
  playerX = 30; playerY = 120 - (PLAYER_H/2);
  enemyX = 200; enemyY = 120 - (ENEMY_H/2);
  bulletX = -10; bulletY = 0;
  bulletActive = false;
  score = 0;
  tft.drawRect(GAME_X1, GAME_Y1, GAME_W, GAME_H, ST77XX_WHITE); 
  tft.setTextSize(1); 
  tft.setTextColor(ST77XX_YELLOW);
  int16_t x1, y1;
  uint16_t w, h;
  String title = "SHOOTER";
  tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor(SCREEN_W - w - 5, 5); 
  tft.print(title);
  drawScoreShooter(); 
}

void loopShooter() {
  static unsigned long lastFrame = 0;
  if (millis() - lastFrame < 33) return;
  lastFrame = millis();

  if (btnD && playerX > GAME_X1 + 2) playerX -= 3;
  if (btnB && playerX < GAME_X1 + GAME_W - PLAYER_W - 2) playerX += 3;
  if (btnA && playerY > GAME_Y1 + 2) playerY -= 3;
  if (btnC && playerY < GAME_Y1 + GAME_H - PLAYER_H - 2) playerY += 3;
  if (btnF && !bulletActive) {
    bulletActive = true;
    bulletX = playerX + PLAYER_W; 
    bulletY = playerY + (PLAYER_H / 2); 
    tone(BUZZER, 1000, 80);
  }
  if (bulletActive) {
    bulletX += 6;
    if (bulletX > GAME_X1 + GAME_W) bulletActive = false;
  }
  
  static unsigned long lastMove = 0;
  
  // Logic Tự động tăng cấp (Giữ nguyên)
  int gameplayLevel = currentLevel + (score / 2);
  if (gameplayLevel > 2) gameplayLevel = 2; 

  unsigned long enemySpeed = 350; 
  if (gameplayLevel == 0) enemySpeed = 500; 
  if (gameplayLevel == 2) enemySpeed = 250; 

  if (millis() - lastMove > enemySpeed) { 
    lastMove = millis();
    enemyY += random(-15, 16);
    if (enemyY < GAME_Y1) enemyY = GAME_Y1;
    if (enemyY > GAME_Y1 + GAME_H - ENEMY_H) enemyY = GAME_Y1 + GAME_H - ENEMY_H;
  }

  if (bulletActive &&
      bulletX < (enemyX + ENEMY_W) &&
      (bulletX + 5) > enemyX && 
      bulletY < (enemyY + ENEMY_H) &&
      (bulletY + 2) > enemyY) 
  {
    bulletActive = false;
    tone(BUZZER, 1500, 120);
    score++;
    drawScoreShooter();
  }
  if (playerX < (enemyX + ENEMY_W) &&
      (playerX + PLAYER_W) > enemyX &&
      playerY < (enemyY + ENEMY_H) &&
      (playerY + PLAYER_H) > enemyY)
  {
    tone(BUZZER, 300, 500);
    setupGameOver(score, "SHOOTER");
    return;
  }
  
  drawGameFrameShooter();
}


void drawGameFrameShooter() {
  static int lastPlayerX = 0, lastPlayerY = 0;
  static int lastEnemyX = 0, lastEnemyY = 0;
  static int lastBulletX = 0, lastBulletY = 0;
  static bool lastBulletState = false;
  drawPlayer(lastPlayerX, lastPlayerY, ST77XX_BLACK);
  drawEnemy(lastEnemyX, lastEnemyY, ST77XX_BLACK);
  if (lastBulletState) {
    tft.fillRect(lastBulletX, lastBulletY, 5, 2, ST77XX_BLACK);
  }
  drawPlayer(playerX, playerY, ST77XX_CYAN);
  drawEnemy(enemyX, enemyY, ST77XX_RED);
  if (bulletActive) {
    tft.fillRect(bulletX, bulletY, 5, 2, ST77XX_YELLOW);
    lastBulletX = bulletX;
    lastBulletY = bulletY;
  }
  lastPlayerX = playerX;
  lastPlayerY = playerY;
  lastEnemyX = enemyX;
  lastEnemyY = enemyY;
  lastBulletState = bulletActive;
}
void drawScoreShooter() {
  tft.fillRect(0, 0, 90, 18, ST77XX_BLACK); 
  tft.setTextSize(1); 
  tft.setCursor(5, 5); 
  tft.setTextColor(ST77XX_GREEN);
  tft.print("Score:");
  tft.print(score);
}


// =======================================================
// ===== PHẦN 3: GAME 2 - SNAKE (Giữ nguyên) ============
// =======================================================
void newFood() {
  foodX = random(1, MAX_W - 2);
  foodY = random(1, MAX_H - 2);
  for (int i = 0; i < snakeLength; i++) {
    if (snakeX[i] == foodX && snakeY[i] == foodY) {
      newFood();
      break;
    }
  }
}
void drawScoreSnake() {
  tft.fillRect(2, 2, 80, 10, ST77XX_BLACK); 
  tft.setTextSize(1); 
  tft.setCursor(5, 3); 
  tft.setTextColor(ST77XX_GREEN);
  tft.print("Score: ");
  tft.print(snakeScore);
}

void setupSnake() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(0, 0, SCREEN_W, SCREEN_H, ST77XX_WHITE);
  snakeLength = 5;
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = (MAX_W / 2) - i;
    snakeY[i] = MAX_H / 2;
  }
  snakeDir = 1;
  snakeScore = 0;
  newFood();
  drawScoreSnake();
}
void drawSnake() {
  tft.fillRect(foodX * GRID_SIZE, foodY * GRID_SIZE, GRID_SIZE, GRID_SIZE, ST77XX_RED);
  for (int i = 0; i < snakeLength; i++) {
    if (i == 0) {
      tft.fillRect(snakeX[i] * GRID_SIZE, snakeY[i] * GRID_SIZE, GRID_SIZE, GRID_SIZE, ST77XX_GREEN);
    } else {
      tft.fillRect(snakeX[i] * GRID_SIZE, snakeY[i] * GRID_SIZE, GRID_SIZE, GRID_SIZE, ST77XX_CYAN);
    }
  }
}

void loopSnake() {
  static unsigned long lastMove = 0;

  // Logic Tự động tăng cấp (Giữ nguyên)
  int gameplayLevel = currentLevel + (snakeScore / 2);
  if (gameplayLevel > 2) gameplayLevel = 2; 

  int gameSpeed = 200; 
  if (gameplayLevel == 0) gameSpeed = 300; 
  if (gameplayLevel == 2) gameSpeed = 120; 

  if (btnF) { 
    gameSpeed = 75;
  }

  if (millis() - lastMove < gameSpeed) return; 
  lastMove = millis();

  if (btnA && snakeDir != 2) snakeDir = 0;
  else if (btnB && snakeDir != 3) snakeDir = 1;
  else if (btnC && snakeDir != 0) snakeDir = 2;
  else if (btnD && snakeDir != 1) snakeDir = 3;

  tft.fillRect(snakeX[snakeLength - 1] * GRID_SIZE, snakeY[snakeLength - 1] * GRID_SIZE, GRID_SIZE, GRID_SIZE, ST77XX_BLACK);
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  if (snakeDir == 0) snakeY[0]--;
  if (snakeDir == 1) snakeX[0]++;
  if (snakeDir == 2) snakeY[0]++;
  if (snakeDir == 3) snakeX[0]--;
  bool gameOver = false;
  if (snakeX[0] < 0 || snakeX[0] >= MAX_W || snakeY[0] < 0 || snakeY[0] >= MAX_H) {
    gameOver = true;
  }
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameOver = true;
      break;
    }
  }
  if (gameOver) {
    tone(BUZZER, 300, 500);
    setupGameOver(snakeScore, "SNAKE");
    return;
  }
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    snakeScore++;
    tone(BUZZER, 1500, 100);
    newFood();
    drawScoreSnake();
  }
  drawSnake();
}


// =======================================================
// ===== PHẦN 4: GAME 3 - PONG (Giữ nguyên) =============
// =======================================================
void drawScorePong() {
  tft.setTextSize(2); 
  tft.fillRect(80, 5, 40, 20, ST77XX_BLACK);
  tft.fillRect(140, 5, 40, 20, ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(90, 10); 
  tft.print(scoreP1);
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(150, 10); 
  tft.print(scoreP2);
}

// (resetBall giữ nguyên)
void resetBall(int level) {
  ballX = SCREEN_W / 2;
  ballY = SCREEN_H / 2;

  int ballSpeed = 2; 
  if (level == 2) ballSpeed = 3; 

  ballVX = (random(0, 2) == 0) ? ballSpeed : -ballSpeed; 
  ballVY = (random(0, 2) == 0) ? ballSpeed : -ballSpeed; 
  
  tone(BUZZER, 900, 100);
}

// (setupPong giữ nguyên)
void setupPong() {
  tft.fillScreen(ST77XX_BLACK);
  for (int i = 0; i < SCREEN_H; i += 10) {
    tft.drawFastVLine(SCREEN_W / 2, i, 5, ST77XX_WHITE);
  }
  scoreP1 = 0;
  scoreP2 = 0;
  p1Y = 100;
  p2Y = 1G00;
  drawScorePong();
  resetBall(currentLevel);
}

// (drawFramePong giữ nguyên)
void drawFramePong() {
  static int lastP1Y = 0, lastP2Y = 0, lastBallX = 0, lastBallY = 0;
  tft.fillRect(0, lastP1Y, PADDLE_W, PADDLE_H, ST77XX_BLACK);
  tft.fillRect(SCREEN_W - PADDLE_W, lastP2Y, PADDLE_W, PADDLE_H, ST77XX_BLACK);
  tft.fillRect(lastBallX - 2, lastBallY - 2, 5, 5, ST77XX_BLACK);
  tft.fillRect(0, p1Y, PADDLE_W, PADDLE_H, ST77XX_GREEN);
  tft.fillRect(SCREEN_W - PADDLE_W, p2Y, PADDLE_W, PADDLE_H, ST77XX_RED);
  tft.fillRect(ballX - 2, ballY - 2, 5, 5, ST77XX_YELLOW);
  lastP1Y = p1Y;
  lastP2Y = p2Y;
  lastBallX = ballX;
  lastBallY = ballY;
}

// (loopPong giữ nguyên)
void loopPong() {
  static unsigned long lastFrame = 0;
  if (millis() - lastFrame < 25) return;
  lastFrame = millis();

  if (btnA && p1Y > 0) p1Y -= 4;
  if (btnC && p1Y < (SCREEN_H - PADDLE_H)) p1Y += 4;

  // Logic Tự động tăng cấp (Giữ nguyên)
  int gameplayLevel = currentLevel + (scoreP1 / 2); 
  if (gameplayLevel > 2) gameplayLevel = 2; 

  int aiSpeed = 3; 
  if (gameplayLevel == 0) aiSpeed = 2; 
  if (gameplayLevel == 2) aiSpeed = 4; 

  if (p2Y + (PADDLE_H / 2) < ballY) p2Y += aiSpeed; 
  else if (p2Y + (PADDLE_H / 2) > ballY) p2Y -= aiSpeed; 
  if (p2Y < 0) p2Y = 0;
  if (p2Y > (SCREEN_H - PADDLE_H)) p2Y = (SCREEN_H - PADDLE_H);
  
  ballX += ballVX;
  ballY += ballVY;
  if (ballY <= 0 || ballY >= (SCREEN_H - 1)) {
    ballVY = -ballVY;
    tone(BUZZER, 1500, 50);
  }
  if (ballX <= PADDLE_W && ballY > p1Y && ballY < (p1Y + PADDLE_H)) {
    ballVX = -ballVX;
    ballX = PADDLE_W + 1;
    tone(BUZZER, 1200, 80);
  }
  if (ballX >= (SCREEN_W - PADDLE_W) && ballY > p2Y && ballY < (p2Y + PADDLE_H)) {
    ballVX = -ballVX;
    ballX = (SCREEN_W - PADDLE_W) - 1;
    tone(BUZZER, 1200, 80);
  }
  
  if (ballX < 0) { 
    scoreP2++;
    if (scoreP2 >= WIN_SCORE) {
      tone(BUZZER, 300, 500);
      setupGameOver(scoreP2, "PONG (AI WINS)");
      return;
    }
    drawScorePong();
    resetBall(gameplayLevel);
  } else if (ballX > SCREEN_W) { 
    scoreP1++;
    if (scoreP1 >= WIN_SCORE) {
      tone(BUZZER, 2000, 500);
      setupGameOver(scoreP1, "PONG (YOU WIN!)");
      return;
    }
    drawScorePong();
    int newGameplayLevel = currentLevel + (scoreP1 / 2);
    if (newGameplayLevel > 2) newGameplayLevel = 2;
    resetBall(newGameplayLevel); 
  }
  drawFramePong();
}


// =======================================================
// ===== PHẦN 5: MÀN HÌNH GAME OVER (Giữ nguyên) =========
// =======================================================
void setupGameOver(int fScore, String gName) {
  currentState = STATE_GAME_OVER;
  finalScore = fScore;
  gameName = gName;
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(3); 
  tft.setTextColor(ST77XX_RED);
  drawCenteredText("GAME OVER", 40, 3); 
  tft.setTextSize(2); 
  tft.setTextColor(ST77XX_WHITE);
  drawCenteredText(gameName, 90, 2); 
  String scoreText = "Final Score: " + String(finalScore);
  drawCenteredText(scoreText, 115, 2); 
  tft.setTextColor(ST77XX_YELLOW);
  drawCenteredText("Hoi non", 160, 2); 
  tft.setTextColor(ST77XX_CYAN);
  drawCenteredText("Press (E) to Menu", 200, 2); 
}

void loopGameOver() {
  if (btnE) {
    tone(BUZZER, 1200, 100);
    while (!digitalRead(BTN_E)) {
      delay(10);
    }
    currentState = STATE_MENU;
    drawMenu();
    return;
  }
}