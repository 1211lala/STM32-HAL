#ifndef __OLED_H
#define __OLED_H


#include "main.h"

#define lie  128
#define hang 	64


#define OLED_CMD 0x00  		/* 命令 */
#define OLED_DATA 0x40 		/* 数据 */
														/* DC引脚 */
#define OLED_MODE_CMD()		HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_MODE_DATA()		HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)


void OLED_Write_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(uint8_t mode);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Init(void);
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode);
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str, uint8_t size, uint8_t mode);
void OLED_Draw_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t bmp_x,uint8_t bmp_y,uint8_t BMP[],uint8_t mode);
#endif
