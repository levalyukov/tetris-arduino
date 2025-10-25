#include "game.hpp"

Tetris::Figure Tetris::setFigure(const uint8_t id) {
      switch (id) {
        case 1:
          return {
            Vector(GRID_X/2,0), 
            {
              {1,1,1,1},
              {0,0,0,0},
              {0,0,0,0},
              {0,0,0,0} 
            }
          };
        case 2:
          return {
            Vector(GRID_X/2,0), 
            {
              {0,2,0,0},
              {2,2,0,0},
              {0,2,0,0},
              {0,0,0,0} 
            }
          };
        case 3:
          return {
            Vector(GRID_X/2,0), 
            {
              {3,3,0,0},
              {3,3,0,0},
              {0,0,0,0},
              {0,0,0,0} 
            }
          };
        case 4:
          return {
            Vector(GRID_X/2,0),
            {
              {4,4,4,0},
              {0,0,4,0},
              {0,0,0,0},
              {0,0,0,0}
            }
          };
        case 5:
          return {
            Vector(GRID_X/2,0),
            {
              {0,0,5,0},
              {5,5,5,0},
              {0,0,0,0},
              {0,0,0,0}
            }
          };
        case 6:
          return {
            Vector(GRID_X/2,0),
            {
              {0,6,0,0},
              {6,6,0,0},
              {6,0,0,0},
              {0,0,0,0}
            }
          };
        case 7:
          return {
            Vector(GRID_X/2,0),
            {
              {7,0,0,0},
              {7,7,0,0},
              {0,7,0,0},
              {0,0,0,0}
            }
          };
      };
};