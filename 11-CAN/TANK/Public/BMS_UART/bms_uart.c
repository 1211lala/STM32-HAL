#include "./BMS_UART/bms_uart.h"

/*
	7F 10 02 06 12 57
*/

uint8_t battery_ark_buf[6] = {0x7f, 0x10, 0x02, 0x06, 0x12, 0x57};

void UART4_Send_Msgs(uint8_t* buf, uint8_t len)
{
	HAL_GPIO_WritePin(UART4_485_EN_GPIO_Port, UART4_485_EN_Pin, GPIO_PIN_SET);		/* 使能发送 */
	HAL_UART_Transmit(&huart4, buf, len, 0xff);
	HAL_GPIO_WritePin(UART4_485_EN_GPIO_Port, UART4_485_EN_Pin, GPIO_PIN_RESET);	/* 使能接收 */
}


void UART5_Send_Msgs(uint8_t* buf, uint8_t len)
{
	HAL_GPIO_WritePin(UART5_485_EN_GPIO_Port, UART5_485_EN_Pin, GPIO_PIN_SET);		/* 使能发送 */
	HAL_UART_Transmit(&huart5, buf, len, 0xff);
	HAL_GPIO_WritePin(UART5_485_EN_GPIO_Port, UART5_485_EN_Pin, GPIO_PIN_RESET);		/* 使能发送 */
}


void UART4_Process_Funtion(uint16_t size)
{
	uart4_485_buf[size] = '\0';
	
	if(uart4_485_buf[0] == 0x7f && uart4_485_buf[1] == 0x10 && uart4_485_buf[2] == 0x02)
	{
			uint8_t real_h = uart4_485_buf[size - 5];
			uint8_t real_l = uart4_485_buf[size - 6];
			battery_big = real_h << 8 | real_l;
			
			if(battery_big > 660)
			{
				batter_big_flag = 0;
			}else
			{
				batter_big_flag = 1;
			}
	}
}

void UART5_Process_Funtion(uint16_t size)
{
	uart5_485_buf[size] = '\0';
		if(uart5_485_buf[0] == 0x7f && uart5_485_buf[1] == 0x10 && uart5_485_buf[2] == 0x02)
		{
			uint8_t real_h = uart5_485_buf[size - 5];
			uint8_t real_l = uart5_485_buf[size - 6];
			battery_small = real_h << 8 | real_l;
			
			if(battery_small > 300)
			{
				batter_small_flag = 0;
			}else
			{
				batter_small_flag = 1;
			}
		}
}


void bms_power_led(uint8_t bms_big, uint8_t bms_small)
{
	if(bms_big == 1)	/* 大电池闪红灯 */
	{
		HAL_GPIO_TogglePin(POWER_R_GPIO_Port, POWER_R_Pin);
	}else	/* 关闭红灯 */
	{
		HAL_GPIO_WritePin(POWER_R_GPIO_Port, POWER_R_Pin, GPIO_PIN_RESET);
	}
	
	if(bms_small == 1)	/* 大电池闪蓝灯 */
	{
		HAL_GPIO_TogglePin(POWER_B_GPIO_Port, POWER_B_Pin);
	}else/* 关闭蓝灯 */
	{
		HAL_GPIO_WritePin(POWER_B_GPIO_Port, POWER_B_Pin, GPIO_PIN_RESET);
	}
	
	if(bms_big == 0 && bms_small == 0)	/* 都正常常亮绿灯 */
	{
		HAL_GPIO_WritePin(POWER_G_GPIO_Port, POWER_G_Pin, GPIO_PIN_SET);
	}else
	{
		HAL_GPIO_WritePin(POWER_G_GPIO_Port, POWER_G_Pin, GPIO_PIN_RESET);
	}
}


void powerled_test(void)
{
	HAL_GPIO_WritePin(POWER_R_GPIO_Port, POWER_R_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER_G_GPIO_Port, POWER_G_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POWER_B_GPIO_Port, POWER_B_Pin, GPIO_PIN_RESET);
	
	HAL_Delay(2000);
	HAL_GPIO_WritePin(POWER_R_GPIO_Port, POWER_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POWER_G_GPIO_Port, POWER_G_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(POWER_B_GPIO_Port, POWER_B_Pin, GPIO_PIN_RESET);
	
	HAL_Delay(2000);
	
	HAL_GPIO_WritePin(POWER_R_GPIO_Port, POWER_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POWER_G_GPIO_Port, POWER_G_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(POWER_B_GPIO_Port, POWER_B_Pin, GPIO_PIN_SET);
	
	HAL_Delay(2000);
}


void light_test(void)
{
	HAL_GPIO_WritePin(SPOTLIGHT1_GPIO_Port, SPOTLIGHT1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPOTLIGHT2_GPIO_Port, SPOTLIGHT2_Pin, GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(SPOTLIGHT1_GPIO_Port, SPOTLIGHT1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPOTLIGHT2_GPIO_Port, SPOTLIGHT2_Pin, GPIO_PIN_RESET);
	HAL_Delay(2000);
	
}

