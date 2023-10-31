#ifndef _TIM2_H_
#define _TIM2_H_

#include "main.h"

extern TIM_HandleTypeDef mytim2;
extern TIM_ClockConfigTypeDef tim2_source;

void MY_TIM2_CountMode_Config(uint16_t psc, uint16_t period);

#endif
