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
