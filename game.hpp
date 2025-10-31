#pragma once

#include <Arduino.h>
#include <stdint.h> // for uint8_t, uint16_t and etc...
#include <stddef.h> // for size_t

#define SCALE 20
#define GRID_Y 12 //16
#define GRID_X 8 // 10
#define FIGURE_SIZE 4
#define INTERVAL 250

class Tetris {
  public:
    Tetris() {};
    ~Tetris() = default;

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

    inline void begin(void) {
      clearGrid();
      Game = true;
    };

    inline void stop(void) {
      clearGrid();
      Game = false;
    };

    Figure setFigure(const uint8_t id);
    uint8_t getFigureHeight(Figure figure);
    uint8_t getFigureWidth(Figure figure);
  
  private:
    inline void clearGrid(void) {
      for (uint8_t x = 0; x < GRID_X; x++) {
        for (uint8_t y = 0; y < GRID_Y; y++) GRID[x][y] = 0;
      };
    };
};