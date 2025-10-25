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
uint8_t display[GRID_X][GRID_Y];
Tetris::Figure figure;
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
  // over();
};

void game(void) {
  tft.fillScreen(0x0000);
  figure = tetris.setFigure(3);

  drawGrid();
  // drawBoard();
  initDisplayGrid();
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

void drawBoard(void) {
  String title = "Scores:";
  String scores = String(tetris.scores);

  tft.setCursor(12*SCALE, GRID_MARGINS.y*SCALE);
  tft.setTextColor(0xC618);
  tft.setTextSize(2);
  tft.println(title);

  tft.setCursor(12*SCALE, GRID_MARGINS.y*SCALE+25);
  tft.setTextColor(0xC618);
  tft.setTextSize(2);
  tft.println(scores);

  tft.drawFastVLine(11*SCALE, 0, tft.height(), 0xFFFF);
};

void addFigure(void) {
  clearFigure();
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) display[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
      };
    };
  };
};

void clearFigure(void) {
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) display[figure.position.x+x][figure.position.y+y] = 0;
      };
    };
  };
};

uint8_t getFigureHeight(void) {
  uint8_t heigth = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[y][x] != 0) {
        heigth++; break;
      };
    };
  }; return heigth;
};

uint8_t getFigureWidth(void) {
  uint8_t width = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[x][y] != 0) {
        width++; break;
      };
    };
  }; return width;
};

bool checkCollision(void) {
  for (size_t x = 0; x < getFigureWidth(); x++) {
    for (size_t y = 0; y < getFigureHeight(); y++) {
      if (figure.shape[x][y] != 0) {
        int positionX = figure.position.x+x;
        int positionY = figure.position.y+y;
        if (positionX < 0 || positionX >= GRID_X || 
            positionY < 0 || positionY >= GRID_Y ||
            display[positionX][positionY] != 0
        ) return true;
      }
    };
  }; return false;
};

void movementFigure(void) {  
  clearFigure();
  figure.position.y++;
  if (checkCollision()) {
    figure.position.y--;
    addFigure();
    for (size_t x = 0; x < getFigureWidth(); x++) {
      for (size_t y = 0; y < getFigureHeight(); y++) {
        if (figure.shape[x][y] != 0) {
          display[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
        };
      };
    }; figure = tetris.setFigure(random(1,8));
  }; addFigure();
};

void initDisplayGrid(void) {
  for (size_t x = 0; x < GRID_X; x++) {
    for (size_t y = 0; y < GRID_Y; y++) {
      display[x][y] = 0;
    };
  };
};

void drawDisplay(void) {
  for (size_t x = 0; x < GRID_X; x++) {
    for (size_t y = 0; y < GRID_Y; y++) { 
      if (display[x][y] == 1) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0x07FF);
      else if (display[x][y] == 2) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0xFD20);
      else if (display[x][y] == 3) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0xFFE0);
      else if (display[x][y] == 4) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0xF800);
      else if (display[x][y] == 5) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0xF81C);
      else if (display[x][y] == 6) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0x07ED);
      else if (display[x][y] == 7) tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0xF7D7);
      else tft.fillRect(GRID_MARGINS.x*SCALE+x*SCALE+1, GRID_MARGINS.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, 0x0000);
    };
  };
};

void update(void) {
  movementFigure();
  drawDisplay();
};

bool canMoveHorizontal(int move) {
  if (figure.position.x+move < 0) return false;
  if (figure.position.x+getFigureWidth()+move > GRID_X) return false;
  for (size_t x = 0; x < getFigureWidth(); x++) {
    for (size_t y = 0; y < getFigureHeight(); y++) {
      if (figure.shape[x][y] != 0) {
        int positionX = figure.position.x+move+x;
        int positionY = figure.position.y+y;
        if (display[positionX][positionY] != 0) return false;
      };
    };
  }; return true;
};

void moveHorizontal(int move) {
  if (canMoveHorizontal(move)) {
    clearFigure();
    figure.position.x+=move;
  };
};

void control(void) {
  if (Serial.available() > 0) {
    consoleCommand = Serial.read();
    switch (consoleCommand) {
      case 'q': // left
        moveHorizontal(-1);
        break;

      case 'e': // right
        moveHorizontal(1);
        break;

      default: break;
    }; Serial.read();
  };
};

void loop(void) {
  if (tetris.Game) {
    control();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL) {      
      previousMillis = currentMillis;
      update();
    };
  } else over();
};

//

void over(void) {
  tft.fillScreen(0x0000);
  String title = "-- Game Over --";
  String scores = "Scores: " + String(tetris.scores);
  String restart = "Press Restart";

  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor((tft.width()-(title.length()*CHAR_WIDTH*2))/2, 25);
  tft.println(title);

  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor((tft.width()-(scores.length()*CHAR_WIDTH*2))/2, 75);
  tft.println(scores);

  tft.setTextColor(0xC618);
  tft.setTextSize(1);
  tft.setCursor((tft.width()-(restart.length()*CHAR_WIDTH))/2, tft.height()-50);
  tft.println(restart);
};