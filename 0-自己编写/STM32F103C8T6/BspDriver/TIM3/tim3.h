#ifndef _TIM3_H_
#define _TIM3_H_

#include "main.h"

extern TIM_HandleTypeDef mytim3;
extern TIM_IC_InitTypeDef tim3_chx;

void MY_TIM3_Chx_IC_Config(uint16_t psc, uint16_t period);
#endif

