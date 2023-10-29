#include "./TIM2/tim2.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim2;

TIM_ClockConfigTypeDef tim2_clock;

/* 设置定时器的时钟来源为外部信号 */
void MY_TIM2_CountMode_Config(uint16_t psc, uint16_t period)
{
	mytim2.Instance = TIM2;
	mytim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim2.Init.Period = period-1;
	mytim2.Init.Prescaler = psc-1;
	/* 重复计数不知道干啥用 */
	mytim2.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&mytim2);
	/* 清除定时器的中断标志  TIM_Base_SetConfig()函数会设置触发一次更新中断 */
	__HAL_TIM_CLEAR_FLAG(&mytim2,TIM_FLAG_UPDATE);
	
	
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
	tim2_clock.ClockSource = TIM_TS_TI1FP1;		/* 时钟来源为通道一的边缘检测模式 */
	HAL_TIM_ConfigClockSource(&mytim2, &tim2_clock);
	
	
	/* 开启TRIG触发中断 */
	__HAL_TIM_ENABLE_IT(&mytim2, TIM_IT_TRIGGER);
	/* 开启定时器 并 开启更新中断 */
	HAL_TIM_Base_Start_IT(&mytim2);
}


