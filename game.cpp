#include "game.hpp"

Tetris::Figure Tetris::setFigure(const uint8_t id) {
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
        Vector(0,0), 
        {
          {3,3,0,0},
          {3,3,0,0},
          {0,0,0,0},
          {0,0,0,0} 
        }
      };
    case 4:
      return {
        Vector(0,0),
        {
          {4,4,4,0},
          {0,0,4,0},
          {0,0,0,0},
          {0,0,0,0}
        }
      };
    case 5:
      return {
        Vector(0,0),
        {
          {0,0,5,0},
          {5,5,5,0},
          {0,0,0,0},
          {0,0,0,0}
        }
      };
    case 6:
      return {
        Vector(0,0),
        {
          {0,6,0,0},
          {6,6,0,0},
          {6,0,0,0},
          {0,0,0,0}
        }
      };
    case 7:
      return {
        Vector(0,0),
        {
          {7,0,0,0},
          {7,7,0,0},
          {0,7,0,0},
          {0,0,0,0}
        }
      };
  };
};

uint8_t Tetris::getFigureHeight(const Figure figure) {
  uint8_t heigth = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[y][x] != 0) {
        heigth++; break;
      };
    };
  }; return heigth;
};

uint8_t Tetris::getFigureWidth(const Figure figure) {
  uint8_t width = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[x][y] != 0) {
        width++; break;
      };
    };
  }; return width;
};