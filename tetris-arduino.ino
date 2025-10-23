#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>
#include <stdlib.h>
#include <string.h>

#include "config.hpp"
#include "game.hpp"

Config cfg;
Tetris tetris;
Adafruit_TFTLCD tft(cfg.LCD_CS, cfg.LCD_CD, cfg.LCD_WR, cfg.LCD_RD, cfg.LCD_RESET);

#define CHAR_WIDTH 6
const Tetris::Vector GRID_MARGINS(2,2);
uint8_t display[GRID_X][GRID_Y];
Tetris::Figure figure = {
  Tetris::Vector(0,0), 
  {
    {1,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
  }
};

void setup(void) {
  // Test only
  Serial.begin(9600);
  // ---------

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(0x0000);

  tetris.begin();
  game();
};

void game(void) {
  drawGrid();
  drawBoard();
  initDisplayGrid();
  addFigure(); // for test
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

  for (size_t y = 0; y < tft.height(); y++) {
    tft.drawPixel(11*SCALE, y, 0xFFFF);
  };
};

void initDisplayGrid(void) {
  for (size_t x = 0; x < GRID_X; x++) {
    for (size_t y = 0; y < GRID_Y; y++) {
      display[x][y] = tetris.GRID[x][y];
    };
  };
};

void addFigure(void) {
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) {
          display[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
        };
      };
    };
  };
};

void clearFigure(void) {
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X-1 && figure.position.y + y >= 0 && figure.position.y < GRID_Y-1) {
        if (figure.shape[x][y] != 0) {
          display[figure.position.x+x][figure.position.y+y] = 0;
        };
      };
    };
  };
};

void movementFigure(void) {
  if (figure.position.y+1 == GRID_Y-1) return;

  clearFigure();
  figure.position.y++;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X-1 && figure.position.y + y >= 0 && figure.position.y < GRID_Y-1) {
        if (figure.shape[x][y] != 0) {
          display[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
        };
      };
    };
  };
};

void drawDisplay(void) {
  for (size_t x = 0; x < GRID_X; x++) {
    for (size_t y = 0; y < GRID_Y; y++) { 
      if (display[x][y] == 1) {
        tft.fillRect(
          GRID_MARGINS.x*SCALE+x*SCALE+1,
          GRID_MARGINS.y*SCALE+y*SCALE+1,
          SCALE-2, SCALE-1, 0xF800
        );
      } else {
        tft.fillRect(
          GRID_MARGINS.x*SCALE+x*SCALE+1,
          GRID_MARGINS.y*SCALE+y*SCALE+1,
          SCALE-2, SCALE-1, 0x0000
        );
      };
    };
  };
};

void update(void) {
  movementFigure();
  drawDisplay();
};

void loop(void) {
  update();
  delay(1000);
};