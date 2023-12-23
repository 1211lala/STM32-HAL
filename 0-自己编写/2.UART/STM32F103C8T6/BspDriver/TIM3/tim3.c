
#include "./TIM3/tim3.h"

TIM_HandleTypeDef mytim3;
TIM_IC_InitTypeDef tim3_chx;

/* 输入捕获模式 */
void MY_TIM3_Chx_IC_Config(uint16_t psc, uint16_t period)
{
	/************************************************设置时基参数*******************************************/
	mytim3.Instance = TIM3;
	mytim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim3.Init.Period = period - 1;
	mytim3.Init.Prescaler = psc - 1;
	mytim3.Init.RepetitionCounter = 0;
	mytim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&mytim3);
	__HAL_TIM_CLEAR_FLAG(&mytim3, TIM_FLAG_UPDATE);
	/* 开启TIM3的更新中断 */
	__HAL_TIM_ENABLE_IT(&mytim3, TIM_FLAG_UPDATE);

	/************************************************捕获通道参数*******************************************/
	tim3_chx.ICFilter = 0x04;
	tim3_chx.ICPolarity = TIM_ICPOLARITY_RISING;
	tim3_chx.ICPrescaler = TIM_ICPSC_DIV1;
	tim3_chx.ICSelection = TIM_ICSELECTION_DIRECTTI;
	HAL_TIM_IC_ConfigChannel(&mytim3, &tim3_chx, TIM_CHANNEL_1);
	/* 开启TIM3IC捕获 */
	HAL_TIM_IC_Start(&mytim3, TIM_CHANNEL_1);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim == &mytim3)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM3_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Mode = GPIO_MODE_INPUT; /* TIM3 CH1 IC PA6  */
		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0); 
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
}

