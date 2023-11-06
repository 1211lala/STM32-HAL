#ifndef _TIM1_H_
#define _TIM1_H_

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_OC_InitTypeDef sConfigOC;
extern DMA_HandleTypeDef hdma_tim1_ch3;
extern uint16_t hdma_tim1_ch3_ch3_buf[];

void MY_TIM1_Base_Tim_Config(uint16_t psc, uint16_t period);
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse);

#endif
