#pragma once

#include <stdint.h>
#include <stddef.h>

class Config {
  public:
    const uint8_t LCD_CS    =   A3;
    const uint8_t LCD_CD    =   A2;
    const uint8_t LCD_WR    =   A1;
    const uint8_t LCD_RD    =   A0;
    const uint8_t LCD_RESET =   A4;
};