#include "./UART3/uart3.h"


/* 询问传感器ID */
void ASK_K5S_ID(void)
{
	uint8_t ask_id_buf[8] ={0xff, 0x03, 0x20, 0x11, 0x00, 0x01, 0xca, 0x11};
	HAL_UART_Transmit(&huart3, ask_id_buf, sizeof ask_id_buf, HAL_MAX_DELAY);
}
/* 询问传感器浓度/数据 */
void ASK_K5S_DATA(void)
{
	uint8_t ask_data_buf[8] ={0xff,0x03 ,0x60, 0x01, 0x00, 0x01, 0xDE, 0x14};
	HAL_UART_Transmit(&huart3, ask_data_buf, sizeof ask_data_buf, HAL_MAX_DELAY);
}
/* 询问传感器数据小数点 */
void ASK_K5S_POINT(void)
{
	uint8_t ask_point_buf[8] ={0xff, 0x03, 0x20, 0x31, 0x00, 0x01, 0xcb, 0xdb};
	HAL_UART_Transmit(&huart3, ask_point_buf, sizeof ask_point_buf, HAL_MAX_DELAY);
}



uint8_t hw_port5_start = 0;

void MY_UART3_Idle_Callback(uint8_t size)
{
	uart3_rec_buf[size] = '\0';
	

		
		
	if(hw_port5_start ==0 && uart3_rec_buf[0] == 0xff && uart3_rec_buf[1] == 0x03 && uart3_rec_buf[2] == 0x02)
	{
		uint16_t ID = ((uint16_t)uart3_rec_buf[3] << 8)|uart3_rec_buf[4];
		hw_port5_get_id = ID;
		
		#if 0
		printf("hw_port5_start = 0  ");
		for(uint8_t i=0; i<size; i++)
		{
			printf("%02x ",uart3_rec_buf[i]);
		}
		printf("\r\n");
		#endif
		ASK_K5S_DATA();
		hw_port5_start = 1;
	}
	else if(hw_port5_start ==1 && uart3_rec_buf[0] == 0xff && uart3_rec_buf[1] == 0x03 && uart3_rec_buf[2] == 0x02)
	{
		uint16_t data = ((uint16_t)uart3_rec_buf[3] << 8)|uart3_rec_buf[4];
		hw_port5_get_data = data;
		
		#if 0
		printf("hw_port5_start = 1  ");
		for(uint8_t i=0; i<size; i++)
		{
			printf("%02x ",uart3_rec_buf[i]);
		}
		printf("\r\n");
		#endif
		
		ASK_K5S_POINT();
		hw_port5_start = 2;
	}	
	else if(hw_port5_start ==2 && uart3_rec_buf[0] == 0xff && uart3_rec_buf[1] == 0x03 && uart3_rec_buf[2] == 0x02)
	{
		
		#if 1
		printf("hw_port5_start = 2  ");
		for(uint8_t i=0; i<size; i++)
		{
			printf("%02x ",uart3_rec_buf[i]);
		}
		printf("\r\n\r\n");
		#endif
		
		uint16_t point = ((uint16_t)uart3_rec_buf[3] << 8)|uart3_rec_buf[4];
		if(point == 0)
		{
			hw_port5_get_data = hw_port5_get_data *10;
		}
		else if(point == 1)
		{
			/* 默认发送的数据扩大十倍 data/10 *10 = data 不变*/
		}
		else if(point > 1)
		{
			hw_port5_get_data = (float)hw_port5_get_data/((point-1)*10.0);
		}
		hw_port5_start = 0;
	}	
}








