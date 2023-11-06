#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch3;

TIM_OC_InitTypeDef sConfigOC;

/*
	CH1 PA8
	CH2 PA9
	CH2 PA10
	CH2 PA11
*/
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	/************************************************设置时基参数*******************************************/
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 90-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

	/************************************************设置PWM参数并设置通道*******************************************/
//	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
//  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
//	sConfigOC.OCIdleState =   	TIM_OCIDLESTATE_RESET;/* 高级定时器特有 配置空闲状态 */
//	sConfigOC.OCNIdleState =		TIM_OCNIDLESTATE_RESET;/* 高级定时器特有 配置互补通道空闲状态 */
//	sConfigOC.OCNPolarity =		TIM_OCNPOLARITY_HIGH;/* 高级定时器特有 配置互补通道空闲极性 */
//	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
//	/* CH1开启PWM输出 */
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

//	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
//	/* CH2开启PWM输出 中断模式 */
//	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);

	
		

	
	/* CH3开启PWM输出 DMA模式 */
//	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t *)WS2812_Buf, 5);
}



void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim == &htim1)
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

		/************************************************设置DMA并LINK*******************************************/

		hdma_tim1_ch3.Instance = DMA1_Channel6;												/* 使用固定的DMA通道 */
		hdma_tim1_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;				 	/* DMA方向 */
		hdma_tim1_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;		/* DMA传输内存数据大小 */
		hdma_tim1_ch3.Init.MemInc = DMA_MINC_ENABLE;						 			/* DMA内存地址自加 */
		hdma_tim1_ch3.Init.Mode = DMA_NORMAL;							 						/* DMA单次模式 */
		hdma_tim1_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* DMA传输寄存器数据大小 */
		hdma_tim1_ch3.Init.PeriphInc = DMA_PINC_DISABLE;					 		/* DMA寄存器地址不自加 */
		hdma_tim1_ch3.Init.Priority = DMA_PRIORITY_LOW;					 			/* DMA优先级低 */
		HAL_DMA_Init(&hdma_tim1_ch3);									 								/* 初始化DMA参数 */
		
		

		__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC3], hdma_tim1_ch3); /* 将 DMA1_Channel2 与 CCR3 寄存器联系起来 */
	
	
		GPIO_InitStruct.Pin = GPIO_PIN_10; /* TIM1 CH3 PWM PA10 */
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 3, 0); /* 设置DMA优先级 对应通道三 DMA模式 */
		HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

		HAL_NVIC_SetPriority(TIM1_UP_IRQn, 3, 0); /* 设置定时器更新中断优先级 */
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
}
