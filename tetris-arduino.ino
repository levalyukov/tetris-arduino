#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>

#include "config.hpp"
#include "game.hpp"

Config cfg;
Tetris tetris;
Adafruit_TFTLCD tft(cfg.LCD_CS, cfg.LCD_CD, cfg.LCD_WR, cfg.LCD_RD, cfg.LCD_RESET);

#define CHAR_WIDTH 6
const Tetris::Vector GRID_MARGINS(0,0);
Tetris::Figure figure;
uint8_t figureWidth = 0;
uint8_t figureHeight = 0;
char consoleCommand;
unsigned long previousMillis = 0;

void setup(void) {
  Serial.begin(9600);

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(0x0000);

  tetris.begin();
  game();
};

void game(void) {
  spawnFigure();
  drawGrid();
  update();
};

void drawGrid(void) {
  for (size_t x = GRID_MARGINS.x*SCALE; x <= GRID_X*SCALE+GRID_MARGINS.x*SCALE; x++) {
    for (size_t y = GRID_MARGINS.y*SCALE; y <= GRID_Y*SCALE+GRID_MARGINS.y*SCALE; y++) {
      if (x % SCALE == 0) tft.drawPixel(x,y,0xC618);
      else if (y % SCALE == 0) tft.drawPixel(x,y,0xC618);
    };
  };
};

void addFigure(void) {
  clearFigure();
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) tetris.GRID[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
      };
    };
  };
};

void clearFigure(void) {
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) tetris.GRID[figure.position.x+x][figure.position.y+y] = 0;
      };
    };
  };
};

bool checkCollision(void) {
  for (size_t x = 0; x < figureWidth; x++) {
    for (size_t y = 0; y < figureHeight; y++) {
      if (figure.shape[x][y] != 0) {
        int8_t positionX = figure.position.x+x;
        int8_t positionY = figure.position.y+y;
        if (positionX < 0 || positionX >= GRID_X || 
            positionY < 0 || positionY >= GRID_Y ||
            tetris.GRID[positionX][positionY] != 0
        ) return true;
      };
    };
  }; return false;
};

void spawnFigure(void) {
  figure = tetris.setFigure(random(1,8));
  figureWidth = tetris.getFigureWidth(figure);
  figureHeight = tetris.getFigureHeight(figure);
  figure.position.x = GRID_X/2+figureWidth/2;

  if (checkCollision()) {
    tetris.stop();
    over();
  };
};

void movementFigure(void) {  
  clearFigure();
  figure.position.y++;
  if (checkCollision()) {
    figure.position.y--;
    addFigure();
    spawnFigure();
    tetris.scores+=5;
    if (!tetris.Game) return;
  }; addFigure();
};

void drawDisplay(void) {
  if (tetris.Game) {
    for (size_t x = 0; x < GRID_X; x++) {
      for (size_t y = 0; y < GRID_Y; y++) { 
        uint16_t color = 0x0000;

        switch (tetris.GRID[x][y]) {
          case 1: color = 0x07FF; break;
          case 2: color = 0xFD20; break;
          case 3: color = 0xFFE0; break;
          case 4: color = 0xF800; break;
          case 5: color = 0xF81C; break;
          case 6: color = 0x07ED; break;
          case 7: color = 0xF7D7; break;
        };

        tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, color);
      };
    };
  };
};

bool canMoveHorizontal(int8_t move) {
  if (figure.position.x+move < 0) return false;
  if (move > 0 && figure.position.x+figureWidth+move > GRID_X) return false;
  for (size_t x = 0; x < figureWidth; x++) {
    for (size_t y = 0; y < figureHeight; y++) {
      int8_t positionX = figure.position.x+move;
      int8_t positionY = figure.position.y+y;
      if (move > 0) positionX = figure.position.x+figureWidth;
      if (tetris.GRID[positionX][positionY] != 0) return false;
    };
  }; return true;
};

void moveHorizontal(int8_t move) {
  if (canMoveHorizontal(move)) {
    clearFigure();
    figure.position.x+=move;
  };
};

void control(void) {
  if (Serial.available() > 0) {
    consoleCommand = Serial.read();
    switch (consoleCommand) {
      case 'q': moveHorizontal(-1); break;
      case 'e': moveHorizontal(1); break;
      default: break;
    }; Serial.read();
  };
};

void update(void) {
  movementFigure();
  drawDisplay();
};

void loop(void) {
  if (tetris.Game) {
    control();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL) {      
      previousMillis = currentMillis;
      update();
    };
  };
};

void over(void) {
  tft.fillScreen(0x0000);

  char title[] = "Game Over";
  char scores[16] = "Scores: ";
  char restart[] = "Press to Restart ->";
  char num_buff[8];

  sprintf(num_buff, "%i", tetris.scores);
  strcat(scores, num_buff);

  tft.setTextSize(3);
  tft.setTextColor(0xFFFF);
  tft.setCursor((tft.width()-(strlen(title)*CHAR_WIDTH*3))/2,50);
  tft.println(title);

  tft.setTextSize(2);
  tft.setTextColor(0xFFFF);
  tft.setCursor((tft.width()-(strlen(scores)*CHAR_WIDTH*2))/2,100);
  tft.println(scores);

  tft.setTextSize(2);
  tft.setTextColor(0xC618);
  tft.setCursor((tft.width()-(strlen(restart)*CHAR_WIDTH*2))/2,tft.height()-50);
  tft.println(restart);
};