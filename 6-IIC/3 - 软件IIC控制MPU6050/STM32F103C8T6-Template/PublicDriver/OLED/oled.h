/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-09-28 09:05:26
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-28 15:47:15
 * @FilePath: \MDK-ARMc:\Users\liuao\Desktop\E_Board-C8T6-IIC\PublicDriver\OLED\oled.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __OLED_H
#define __OLED_H


#include "main.h"


#define OLED_091	0				/* 对应OLED使用那个尺寸的屏幕，不同的屏幕初始化程序好像不一样，我的屏幕是这样 */
#if OLED_091 
#else
	#define	OLED_096  1
#endif

#define lie  128
#if OLED_091
	#define hang 	32
#else 
	#define hang 	64
#endif

// 0111 1000   0011 1100
// 1111 0000
#define OLED_ARDESS 0x78	/* SSD1306的地址 */

#define OLED_CMD 0x00  		/* 命令 */
#define OLED_DATA 0x40 		/* 数据 */


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


void OLED_Draw_Black(uint8_t x, uint8_t y, uint8_t size);
#endif
