#ifndef _UART3_H_
#define _UART3_H_


#include "main.h"
#include "usart.h"


extern uint8_t hw_port5_start;		/* hw_port5启动标志位 */


void ASK_K5S_ID(void);
void ASK_K5S_DATA(void);
void ASK_K5S_POINT(void);


void MY_UART3_Idle_Callback(uint8_t size);

#endif

