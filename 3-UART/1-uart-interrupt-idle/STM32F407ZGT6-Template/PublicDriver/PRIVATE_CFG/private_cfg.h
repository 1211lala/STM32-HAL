#ifndef _sys_h_
#define _sys_h_


#include "main.h"
#include "usart.h"
#include "rng.h"
int fputc(int ch, FILE *f);


typedef enum 
{
	key_null = 0,
	key0_down,
	key_up_down,
}Key_Status;

Key_Status Get_key_with_down(uint8_t waittime);
Key_Status Get_key_with_undo(uint8_t waittime);


void MY_Delay_Init(uint16_t freq);
void MY_Delay_Us(uint32_t nus);

uint32_t RNG_GetRandomNumber(uint32_t min,uint32_t max);

#endif
