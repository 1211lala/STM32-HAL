#ifndef _TEXT_H_
#define _TEXT_H_

#include "main.h"


uint8_t FS_Show_Text(uint8_t x, uint8_t y, uint8_t size, uint8_t* gbk, uint8_t mode);
uint8_t FS_Show_Picture(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end,uint8_t * path, uint32_t size, uint8_t mode);

#endif

