#include "./TIM2/tim2.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim2;

TIM_ClockConfigTypeDef tim2_clock;

/* 设置定时器2的时钟来源为外部信号 */
void MY_TIM2_CountMode_Config(uint16_t psc, uint16_t period)
{
	mytim2.Instance = TIM2;
	mytim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim2.Init.Period = period - 1;
	mytim2.Init.Prescaler = psc - 1;
	/* 重复计数不知道干啥用 */
	mytim2.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&mytim2);
	/* 清除定时器的中断标志  TIM_Base_SetConfig()函数会设置触发一次更新中断 */
	__HAL_TIM_CLEAR_FLAG(&mytim2, TIM_FLAG_UPDATE);

	/*
		选择不同的时钟源时，有些参数可以不用设置 具体参考 中文参考手册 200页
		psc决定输入分频 -- 注意
	*/
	/* 设置滤波参数 对ETR, 通道1，2有效 在中文参考手册242页的 IC1F 位*/
	tim2_clock.ClockFilter = 0x03;
	/* 设置输入信号极性 对ED双边沿无效 */
	tim2_clock.ClockPolarity = TIM_CLOCKPOLARITY_RISING;
	/* 设置输入分频 只对ETR通道有用 */
	tim2_clock.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
	/* 设置信号来源 */
	tim2_clock.ClockSource = TIM_TS_TI1FP1; /* 时钟来源为通道一滤波模式 */
	HAL_TIM_ConfigClockSource(&mytim2, &tim2_clock);

	/* 开启TRIG触发中断 */
	__HAL_TIM_ENABLE_IT(&mytim2, TIM_IT_TRIGGER);
	/* 开启定时器 并 开启更新中断 */
	HAL_TIM_Base_Start_IT(&mytim2);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim == &mytim1) /* TIM1 更新中断*/
	{
		__HAL_RCC_TIM1_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}

	if (htim == &mytim2) /* TIM2 CH1 PA0 外部输入口 */
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}