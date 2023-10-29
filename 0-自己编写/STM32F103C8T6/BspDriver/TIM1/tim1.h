#ifndef _TIM1_H_
#define _TIM1_H_

#include "main.h"

extern TIM_HandleTypeDef mytim1;
extern TIM_OC_InitTypeDef tim1_ch1;
void MY_TIM1_Base_Tim_Config(uint16_t psc, uint16_t period);
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse);

#endif

