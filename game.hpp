#pragma once

#include <Arduino.h>
#include <stdint.h> // for uint8_t, uint16_t and etc...
#include <stddef.h> // for size_t

#define SCALE 16
#define GRID_Y 10
#define GRID_X 7
#define FIGURE_SIZE 4
#define INTERVAL 1000

class Tetris {
  public:
    Tetris() {};
    ~Tetris() = default;

    struct Vector {
      uint16_t x; uint8_t y;
      Vector() = default;
      Vector(uint16_t x_val, uint8_t y_val) : x(x_val), y(y_val) {};
    };

    struct Figure {
      Vector position;
      int shape[4][4];
    };

    enum PAGES {Menu, Game, Over};
    uint8_t GRID[GRID_X][GRID_Y];
    uint8_t page = PAGES::Game;
    uint16_t scores = 0;


    inline void begin(void) {
      initGrid();
    };

    inline Figure setFigure(const uint8_t id) {
      switch (id) {
        case 1:
          return {
            Vector(0,0), 
            {
              {1,1,1,1},
              {0,0,0,0},
              {0,0,0,0},
              {0,0,0,0} 
            }
          };
        case 2:
          return {
            Vector(0,0), 
            {
              {0,2,0,0},
              {2,2,0,0},
              {0,2,0,0},
              {0,0,0,0} 
            }
          };
        case 3:
          return {
            Vector(3,0), 
            {
              {3,3,0,0},
              {3,3,0,0},
              {0,0,0,0},
              {0,0,0,0} 
            }
          };
      };
    };

  private:
    inline void initGrid(void) {
      for (uint8_t x = 0; x < GRID_X; x++) {
        for (uint8_t y = 0; y < GRID_Y; y++) GRID[x][y] = 0;
      };
    };
};