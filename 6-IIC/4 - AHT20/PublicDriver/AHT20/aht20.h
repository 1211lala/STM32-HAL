#ifndef __AHT20_H
#define __AHT20_H


#include "main.h"
#include "gpio.h"


#define	W_ADDRESS	0x70
#define	R_ADDRESS	0x71



#define INIT_CMD		0xBE
#define START_CMD		0xAC
#define RESET_CMD		0xBA
#define STATUS_CMD	0x71


void AHT20_Write_Byte(uint8_t data);
void AHT20_Write_Bytes(uint8_t cmd, uint8_t * data, uint16_t len);
uint8_t AHT20_Get_Status_Byte(void);
uint8_t AHT20_Get_Cal_Enable(void);
uint8_t AHT20_Init(void);
void AHT20_Start_Measure(void);
uint8_t AHT20_Read_Hex_Hum_Temp(uint32_t* buf);
uint8_t AHT20_Read_Float_Hum_Temp(float *buf);

#endif

