#include "game.hpp"

void Tetris::begin(Adafruit_TFTLCD* _tft) {
  tft = _tft;
  clearGrid();
  Game = true;
  spawnFigure();
  drawGrid();
  update();
};

void Tetris::stop(void) {
  clearGrid();
  Game = false;
};

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

uint8_t Tetris::getFigureHeight(void) {
  uint8_t heigth = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[y][x] != 0) {
        heigth++; break;
      };
    };
  }; return heigth;
};

uint8_t Tetris::getFigureWidth(void) {
  uint8_t width = 0;
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.shape[x][y] != 0) {
        width++; break;
      };
    };
  }; return width;
};

bool Tetris::canMoveHorizontal(int8_t move) {
  if (figure.position.x+move < 0) return false;
  if (move > 0 && figure.position.x+getFigureWidth()+move > GRID_X) return false;
  for (size_t x = 0; x < getFigureWidth(); x++) {
    for (size_t y = 0; y < getFigureHeight(); y++) {
      int8_t positionX = figure.position.x+move;
      int8_t positionY = figure.position.y+y;
      if (move > 0) positionX = figure.position.x+getFigureWidth();
      if (GRID[positionX][positionY] != 0) return false;
    };
  }; return true;
};

void Tetris::cleanLines(void) {
  for (size_t y = GRID_Y-1; y > 0; y--) {
    bool lineFull = true; 
    for (size_t x = 0; x < GRID_X; x++) {
      if (GRID[x][y] == 0) {
        lineFull = false;
        break;
      };
    };

    if (lineFull) {
      scores+=10;
      for (size_t yy = y; yy > 0; yy--) {
        for (size_t x = 0; x < GRID_X; x++) {
          GRID[x][yy] = GRID[x][yy-1];
        };
      };

      for (size_t x = 0; x < GRID_X; x++) {
        GRID[x][y] = 0;
      }; y++;
    };
  };
};

void Tetris::drawDisplay(void) {
  if (Game) {
    for (size_t x = 0; x < GRID_X; x++) {
      for (size_t y = 0; y < GRID_Y; y++) { 
        uint16_t color = 0x0000;

        switch (GRID[x][y]) {
          case 1: color = 0x07FF; break;
          case 2: color = 0xFD20; break;
          case 3: color = 0xFFE0; break;
          case 4: color = 0xF800; break;
          case 5: color = 0xF81C; break;
          case 6: color = 0x07ED; break;
          case 7: color = 0xF7D7; break;
        };
        
        tft->fillRect(margins.x*SCALE+x*SCALE+1, margins.y*SCALE+y*SCALE+1, SCALE-1, SCALE-1, color);
      };
    };
  };
};

void Tetris::drawGrid(void) {
  for (size_t x = margins.x*SCALE; x <= GRID_X*SCALE+margins.x*SCALE; x++) {
    for (size_t y = margins.y*SCALE; y <= GRID_Y*SCALE+margins.y*SCALE; y++) {
      if (x % SCALE == 0) tft->drawPixel(x,y,0xC618);
      else if (y % SCALE == 0) tft->drawPixel(x,y,0xC618);
    };
  };
};

void Tetris::clearFigure(void) {
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) GRID[figure.position.x+x][figure.position.y+y] = 0;
      };
    };
  };
};

void Tetris::addFigure(void) {
  clearFigure();
  for (size_t x = 0; x < FIGURE_SIZE; x++) {
    for (size_t y = 0; y < FIGURE_SIZE; y++) {
      if (figure.position.x + x >= 0 && figure.position.x < GRID_X && figure.position.y + y >= 0 && figure.position.y < GRID_Y) {
        if (figure.shape[x][y] != 0) GRID[figure.position.x+x][figure.position.y+y] = figure.shape[x][y];
      };
    };
  };
};

bool Tetris::checkCollision(void) {
  for (size_t x = 0; x < figureWidth; x++) {
    for (size_t y = 0; y < figureHeight; y++) {
      if (figure.shape[x][y] != 0) {
        int8_t positionX = figure.position.x+x;
        int8_t positionY = figure.position.y+y;
        if (positionX < 0 || positionX >= GRID_X || 
            positionY < 0 || positionY >= GRID_Y ||
            GRID[positionX][positionY] != 0
        ) return true;
      };
    };
  }; return false;
};

void Tetris::spawnFigure(void) {
  figure = setFigure(random(1,8));
  figureWidth = getFigureWidth();
  figureHeight = getFigureHeight();
  figure.position.x = GRID_X/2+figureWidth/2;

  if (checkCollision()) {
    stop();
    over();
  };
};

void Tetris::movementFigure(void) {  
  clearFigure();
  figure.position.y++;
  if (checkCollision()) {
    figure.position.y--;
    addFigure();
    cleanLines();
    spawnFigure();
    scores+=5;
    if (!Game) return;
  }; addFigure();
};

void Tetris::moveHorizontal(int8_t move) {
  if (canMoveHorizontal(move)) {
    clearFigure();
    figure.position.x+=move;
  };
};

void Tetris::update(void) {
  movementFigure();
  drawDisplay();
};

void Tetris::control(void) {
  if (Serial.available() > 0) {
    consoleCommand = Serial.read();
    switch (consoleCommand) {
      case 'q': moveHorizontal(-1); break;
      case 'e': moveHorizontal(1); break;
      default: break;
    }; Serial.read();
  };
};

void Tetris::over() {
  tft->fillScreen(0x0000);

  char title[] = "Game Over";
  char scores[16] = "Scores: ";
  char restart[] = "Press to Restart ->";
  char num_buff[8];

  sprintf(num_buff, "%i", scores);
  strcat(scores, num_buff);

  tft->setTextSize(3);
  tft->setTextColor(0xFFFF);
  tft->setCursor((tft->width()-(strlen(title)*CHAR_WIDTH*3))/2,50);
  tft->println(title);

  tft->setTextSize(2);
  tft->setTextColor(0xFFFF);
  tft->setCursor((tft->width()-(strlen(scores)*CHAR_WIDTH*2))/2,100);
  tft->println(scores);

  tft->setTextSize(2);
  tft->setTextColor(0xC618);
  tft->setCursor((tft->width()-(strlen(restart)*CHAR_WIDTH*2))/2,tft->height()-50);
  tft->println(restart);
};