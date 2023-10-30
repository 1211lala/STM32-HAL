#include "./TIM1/tim1.h"
#include "usbd_cdc_if.h"

TIM_HandleTypeDef mytim1;
DMA_HandleTypeDef tim1_dma;
uint16_t tim1_dma_ch2_buf[5] = {1000, 2000, 3000, 4000, 5000} ;

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




TIM_OC_InitTypeDef tim1_chx;
void MY_TIM1_Chx_Pwm_Config(uint16_t psc, uint16_t period, uint16_t pulse)
{
	MY_TIM1_Base_Tim_Config(psc, period);
	HAL_TIM_PWM_Init(&mytim1);
	/* 清除定时器的中断标志  TIM_Base_SetConfig()函数会设置触发一次更新中断 */
	__HAL_TIM_CLEAR_FLAG(&mytim1,TIM_FLAG_UPDATE);
	
	tim1_chx.OCFastMode = TIM_OCFAST_DISABLE;			
	tim1_chx.OCMode = TIM_OCMODE_PWM1;  					/* 设置PWM模式 */
	/* 
		只考虑向上计数
		PWMMODE1	在CNT > CCR1时为无效电平，反之为有效电平
		PWMMODE2	在CNT > CCR1时为有效电平，反之为无效电平
	*/
	tim1_chx.OCPolarity =	TIM_OCPOLARITY_HIGH;
	tim1_chx.Pulse = pulse;							/* 设置比较值 */
//	tim1_chx.OCIdleState =   	/* 高级定时器特有 配置空闲状态 */
//	tim1_chx.OCNIdleState =		/* 高级定时器特有 配置互补通道空闲状态 */								
//	tim1_chx.OCNPolarity =		/* 高级定时器特有 配置互补通道空闲极性 */
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_1);
	/* 开启PWM输出 中断模式 */
	HAL_TIM_PWM_Start_IT(&mytim1, TIM_CHANNEL_1);
	
	
	HAL_TIM_PWM_ConfigChannel(&mytim1, &tim1_chx, TIM_CHANNEL_2);
	tim1_dma.Instance = DMA1_Channel2;													/* 使用固定的DMA通道 */
	tim1_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;							/* DMA方向 */
	tim1_dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;		/* DMA传输内存数据大小 */
	tim1_dma.Init.MemInc = DMA_MINC_ENABLE;											/* DMA内存地址自加 */
	tim1_dma.Init.Mode = DMA_NORMAL;														/* DMA单次模式 */
	tim1_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;/* DMA传输寄存器数据大小 */
	tim1_dma.Init.PeriphInc = DMA_PINC_DISABLE;									/* DMA寄存器地址不自加 */
	tim1_dma.Init.Priority = DMA_PRIORITY_LOW;									/* DMA优先级低 */
	HAL_DMA_Init(&tim1_dma);																		/* 初始化DMA参数 */
	
	__HAL_LINKDMA(&mytim1, hdma[TIM_DMA_ID_CC2], tim1_dma);			/* 将 DMA1_Channel2 与 CCR2 寄存器联系起来 */
	
	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	/* TIM1 更新中断*/
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	}
	
	if(htim == &mytim2)	/* TIM2 CH1 PA0 外部输入口 */
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &mytim1)	
	{
		__HAL_RCC_TIM1_CLK_ENABLE();		/* TIM1 CH1 PWM PA8  */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DMA1_CLK_ENABLE();			/* TIM1 CH1 PWM PA9  */
		
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};	
		
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		HAL_NVIC_SetPriority(TIM1_CC_IRQn,3,0);
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
		
																			
		
		
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn,3,0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
	}
}






