#include "./USART1/usart1.h"

UART_HandleTypeDef myuart1;

void my_uasrt1_uart_init(uint32_t baud)
{
	
	myuart1.Instance = USART1;
	myuart1.Init.BaudRate = baud;
	myuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	myuart1.Init.Mode = UART_MODE_TX_RX;
	/* 16倍过采样数据 */
	myuart1.Init.OverSampling = UART_OVERSAMPLING_16;
	myuart1.Init.Parity = UART_PARITY_NONE;
	myuart1.Init.StopBits = UART_STOPBITS_1;
	myuart1.Init.WordLength = UART_WORDLENGTH_8B;
	
	HAL_UART_Init(&myuart1);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart == &myuart1)
	{
		/* 时钟 */
		__HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
		
		/*
			RX -- PA10
			TX -- PA9
		*/
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		/* 设置NVIC */
		HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

