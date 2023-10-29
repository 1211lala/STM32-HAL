#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim1;


void MY_TIM1_Base_Tim_Config(uint16_t psc, uint16_t period)
{
	mytim1.Instance = TIM1;
	mytim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim1.Init.Period = period-1;
	mytim1.Init.Prescaler = psc-1;
	/* 重复计数不知道干啥用 */
	mytim1.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&mytim1);
	
	/* 清除定时器的中断标志 */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	/* 以中断方式开启定时器 */
	HAL_TIM_Base_Start_IT(&mytim1);
}




TIM_OC_InitTypeDef tim1_ch1;
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	MY_TIM1_Base_Tim_Config(psc, period);
	HAL_TIM_PWM_Init(&mytim1);
	/* 清除定时器的中断标志  TIM_Base_SetConfig()函数会设置触发一次更新中断 */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	
	tim1_ch1.OCFastMode = TIM_OCFAST_DISABLE;			
	tim1_ch1.OCMode = TIM_OCMODE_PWM1;  					/* 设置PWM模式 */
	/* 
		只考虑向上计数
		PWMMODE1	在CNT > CCR1时为无效电平，反之为有效电平
		PWMMODE2	在CNT > CCR1时为有效电平，反之为无效电平
	*/
	tim1_ch1.OCPolarity =	TIM_OCPOLARITY_HIGH;
	tim1_ch1.Pulse = pulse;							/* 设置比较值 */
//	tim1_ch1.OCIdleState =   	/* 高级定时器特有 配置空闲状态 */
//	tim1_ch1.OCNIdleState =		/* 高级定时器特有 配置互补通道空闲状态 */								
//	tim1_ch1.OCNPolarity =		/* 高级定时器特有 配置互补通道空闲极性 */
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_ch1, TIM_CHANNEL_1);
	
	/* 开启PWM输出 */
	HAL_TIM_PWM_Start(&mytim1, TIM_CHANNEL_1);

	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 更新中断*/
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	
	if(htim == &mytim2)	/* TIM2 外部输入口 */
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 PWM */
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}






