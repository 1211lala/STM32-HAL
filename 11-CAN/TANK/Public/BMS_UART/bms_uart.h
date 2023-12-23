#ifndef _BMS_UART_H_
#define _BMS_UART_H_


#include "main.h"


extern uint8_t battery_ark_buf[];

void UART4_Send_Msgs(uint8_t* buf, uint8_t len);
void UART5_Send_Msgs(uint8_t* buf, uint8_t len);

void UART4_Process_Funtion(uint16_t size);
void UART5_Process_Funtion(uint16_t size);

void bms_power_led(uint8_t bms_big, uint8_t bms_small);

void powerled_test(void);
void light_test(void);
#endif


