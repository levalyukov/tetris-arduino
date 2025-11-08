#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>

#include "config.hpp"
#include "game.hpp"

Config cfg;
Tetris tetris;
Adafruit_TFTLCD* tft = new Adafruit_TFTLCD(cfg.LCD_CS, cfg.LCD_CD, cfg.LCD_WR, cfg.LCD_RD, cfg.LCD_RESET);
unsigned long previousMillis = 0;

void setup(void) {
  tft->reset();
  tft->begin(0x9341);
  tft->setRotation(1);
  tft->fillScreen(0x0000);
  tetris.begin(tft);
};

void loop(void) {
  if (tetris.Game) {
    tetris.control();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL) {      
      previousMillis = currentMillis;
      tetris.update();
    };
  };
};