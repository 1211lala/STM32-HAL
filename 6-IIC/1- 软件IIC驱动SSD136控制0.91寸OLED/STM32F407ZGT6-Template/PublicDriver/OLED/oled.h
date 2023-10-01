#ifndef __OLED_H_
#define __OLED_H_


#include "main.h"
#include ".\OLED\iic.h"

#define OLED_ARDESS		0x78

#define OLED_CMD  0x00	/* ÃüÁî */
#define OLED_DATA 0x40	/* Êý¾Ý */



void OLED_Write_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(uint8_t mode);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Init(void);
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode);
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str, uint8_t size, uint8_t mode);

#endif
