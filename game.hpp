#pragma once

#include <Arduino.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <stdint.h> // for uint8_t, uint16_t and etc...
#include <stddef.h> // for size_t

#define CHAR_WIDTH 6
#define SCALE 20
#define GRID_Y 12 //16
#define GRID_X 8 // 10
#define FIGURE_SIZE 4
#define INTERVAL 250

class Tetris {
  public:
    Tetris() {};
    ~Tetris() {};

    struct Vector {
      int8_t x, y;
      Vector() = default;
      Vector(int8_t x_val, int8_t y_val) : x(x_val), y(y_val) {};
    };

    struct Figure {
      Vector position;
      int shape[4][4];
    };

    bool Game = false;
    uint8_t GRID[GRID_X][GRID_Y];
    uint16_t scores = 0;
    Figure figure;
    uint8_t figureWidth = 0;
    uint8_t figureHeight = 0;
    char consoleCommand;
    Adafruit_TFTLCD* tft = nullptr;
    const Vector margins = {0,0};

    void begin(Adafruit_TFTLCD* tft);
    void stop(void);

    void control(void);
    void update(void);
  
  private:
    inline void clearGrid(void) {
      for (uint8_t x = 0; x < GRID_X; x++) {
        for (uint8_t y = 0; y < GRID_Y; y++) GRID[x][y] = 0;
      };
    };

    Figure setFigure(const uint8_t id);
    uint8_t getFigureHeight(void);
    uint8_t getFigureWidth(void);

    void drawGrid(void);
    void drawDisplay(void);

    void spawnFigure(void);
    void cleanLines(void);
    bool canMoveHorizontal(int8_t move);
    void movementFigure(void);
    void clearFigure(void);
    void addFigure(void);
    bool checkCollision(void);
    void moveHorizontal(int8_t move);

    void over(void);
};