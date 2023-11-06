#ifndef _OTHER_H_
#define _OTHER_H_

#include "main.h"

void my_delay_us(uint16_t nus);
uint8_t test_crc_sum(uint8_t *buffer,uint8_t size);
uint8_t get_crc_sum(uint8_t *buffer,uint8_t size);
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf);
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf);
unsigned short CRC16_MODBUS(unsigned char *data, unsigned int datalen);
uint32_t my_strlen(uint8_t* buf);

void rank(uint16_t *array, uint16_t len);
	


#endif
