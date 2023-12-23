#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim1;
DMA_HandleTypeDef tim1_dma;

TIM_OC_InitTypeDef tim1_chx;

void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	/************************************************设置时基参数*******************************************/
	mytim1.Instance = TIM1;
	mytim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	mytim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim1.Init.Period = period - 1;
	mytim1.Init.Prescaler = psc - 1;
	/* 重复计数不知道干啥用 */
	mytim1.Init.RepetitionCounter = 0;
	/*设置时基参数 相当于 HAL_TIM_Base_Init() */
	HAL_TIM_PWM_Init(&mytim1);
	/* 清除定时器的中断标志  TIM_Base_SetConfig()函数会设置触发一次更新中断 */
	__HAL_TIM_CLEAR_FLAG(&mytim1, TIM_FLAG_UPDATE);
	/* 开启定时器的更新中断 */
	__HAL_TIM_ENABLE_IT(&mytim1, TIM_FLAG_UPDATE);

	/************************************************设置PWM参数并设置通道*******************************************/
	tim1_chx.OCFastMode = TIM_OCFAST_DISABLE;
	tim1_chx.OCMode = TIM_OCMODE_PWM1; /* 设置PWM模式 */
	/*
		只考虑向上计数
		PWMMODE1	在CNT > CCR1时为无效电平，反之为有效电平
		PWMMODE2	在CNT > CCR1时为有效电平，反之为无效电平
	*/
	tim1_chx.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim1_chx.Pulse = pulse; /* 设置比较值 */
//	tim1_chx.OCIdleState =   	/* 高级定时器特有 配置空闲状态 */
//	tim1_chx.OCNIdleState =		/* 高级定时器特有 配置互补通道空闲状态 */
//	tim1_chx.OCNPolarity =		/* 高级定时器特有 配置互补通道空闲极性 */
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_1);
	/* CH1开启PWM输出 */
	HAL_TIM_PWM_Start(&mytim1, TIM_CHANNEL_1);

	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_2);
	/* CH2开启PWM输出 中断模式 */
	HAL_TIM_PWM_Start_IT(&mytim1, TIM_CHANNEL_2);

	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_3);
		
	/************************************************设置DMA并LINK*******************************************/
	tim1_dma.Instance = DMA1_Channel6;												/* 使用固定的DMA通道 */
	tim1_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;				 		/* DMA方向 */
	tim1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	/* DMA传输内存数据大小 */
	tim1_dma.Init.MemInc = DMA_MINC_ENABLE;						 		/* DMA内存地址自加 */
	tim1_dma.Init.Mode = DMA_NORMAL;							 				/* DMA单次模式 */
	tim1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* DMA传输寄存器数据大小 */
	tim1_dma.Init.PeriphInc = DMA_PINC_DISABLE;					 	/* DMA寄存器地址不自加 */
	tim1_dma.Init.Priority = DMA_PRIORITY_LOW;					 	/* DMA优先级低 */
	HAL_DMA_Init(&tim1_dma);									 						/* 初始化DMA参数 */

	__HAL_LINKDMA(&mytim1, hdma[TIM_DMA_ID_CC3], tim1_dma); /* 将 DMA1_Channel2 与 CCR1 寄存器联系起来 */
}



void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim == &mytim1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; /* TIM1 CH1 PWM PA8  */
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9; /* TIM1 CH2 PWM PA9  */
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 3, 0); /* 设置通道中断优先级 对应通道二 中断模式 */
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

		GPIO_InitStruct.Pin = GPIO_PIN_10; /* TIM1 CH3 PWM PA10 */
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 3, 0); /* 设置DMA优先级 对应通道三 DMA模式 */
		HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

		HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0); /* 设置定时器更新中断优先级 */
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
}
