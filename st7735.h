#ifndef __ST7735_H__
#define __ST7735_H__
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "spi.h"

#define CS_PIN	8UL
#define A0_PIN  24UL
#define RST_PIN	25UL

// Screen resolution in normal orientation
#define scr_w         160
#define scr_h         128

// CS pin macros
#define CS_L() gpio_clear_pin(CS_PIN)
#define CS_H() gpio_set_pin(CS_PIN)

// A0 pin macros
#define A0_L() gpio_clear_pin(A0_PIN)
#define A0_H() gpio_set_pin(A0_PIN)

// RESET pin macros
#define RST_L() gpio_clear_pin(RST_PIN)
#define RST_H() gpio_set_pin(RST_PIN)


#define Delay_US(x) usleep(x)
// Colors for spaces between symbols for debug view
//#define V_SEP COLOR565_YELLOW
//#define H_SEP COLOR565_SIENNA
//#define V_SEP COLOR565_WHITE
//#define H_SEP COLOR565_WHITE

typedef enum
{
  scr_normal = 0,
  scr_CW = 1,
  scr_CCW = 2,
  scr_180 = 3
} ScrOrientation_TypeDef;

extern uint16_t scr_width;
extern uint16_t scr_height;

uint16_t RGB565 (uint8_t R, uint8_t G, uint8_t B);
void ST7735_write (uint8_t data);
void ST7735_Init (void);
void ST7735_AddrSet (uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE);
void ST7735_Orientation (uint8_t orientation);
void ST7735_Clear (uint16_t color);
extern void Delay_US (volatile  uint32_t nTime);

#endif
