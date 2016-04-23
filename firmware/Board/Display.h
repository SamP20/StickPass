#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <stdint.h>

#define DISPLAY_CONTRAST_CMD 0x81
#define DISPLAY_ALLONRESUME_CMD 0xA4
#define DISPLAY_ALLON_CMD 0xA5
#define DISPLAY_NORMAL_CMD 0xA6
#define DISPLAY_INVERT_CMD 0xA7
#define DISPLAY_OFF_CMD 0xAE
#define DISPLAY_ON_CMD 0xAF
#define DISPLAY_MUXRATIO_CMD 0xA8
#define DISPLAY_OFFSET_CMD 0xD3
#define DISPLAY_CLKDIV_CMD 0xD5
#define DISPLAY_PRECHG_CMD 0xD9
#define DISPLAY_VCOMDESELECT_CMD 0xDB
#define DISPLAY_CHARGEPUMP_CMD 0x8D
#define DISPLAY_SEGREMAP_CMD 0xA0
#define DISPLAY_COMSCANINC_CMD 0xC0
#define DISPLAY_COMSCANDEC_CMD 0xC8
#define DISPLAY_COMCFG_CMD 0xDA

#define DISPLAY_COLS 64
#define DISPLAY_PAGES 6


void Display_Init(void);
void Display_Command(uint8_t data);
void Display_Data(uint8_t data);
void Display_PageAddr(uint8_t addr);
void Display_ColAddr(uint8_t addr);
void Display_Update(void);
void Display_Clear(void);
void Display_SetCursor(uint8_t row, uint8_t col);
void Display_Write(const char *str);
void Display_Write_P(const char *str);
void Display_WritePos(uint8_t row, uint8_t col, const char *str);
void Display_WritePos_P(uint8_t row, uint8_t col, const char *str);


#endif
