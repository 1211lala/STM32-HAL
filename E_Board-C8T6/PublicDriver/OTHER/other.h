#ifndef _OTHER_H_
#define _OTHER_H_

#include "main.h"

uint8_t test_crc_sum(uint8_t *buffer,uint8_t size);
uint8_t get_crc_sum(uint8_t *buffer,uint8_t size);

void my_delay_us(uint16_t nus);
#endif
