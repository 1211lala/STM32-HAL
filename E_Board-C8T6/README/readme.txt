#include "./UART1_232/uart1_232.h"


#define UART1_ASK_CMD 0X55					/* 查询命令 */
#define UART1_DATA_CMD 0XAA 				/* 数据命令 */
#define UART1_FRAME_HANDER 0XAAFF		/* 头帧*/
#define PRINTF_UART1  1


uint8_t uart1_send_ask_buf[20];		/* ROS询问帧返回的数据 */
uint8_t uart1_send_data_buf[40];	/* ROS数据帧返回的数据 */


uint16_t Sensor1_data  = 1111;		 /* 假设定义的传感器数据 */	
uint16_t Sensor2_data  = 2222; 
uint16_t Sensor3_data  = 3333; 
uint16_t Sensor4_data  = 4444; 
uint16_t Sensor5_data  = 5555; 
uint16_t Sensor6_data  = 6666; 
uint16_t Sensor7_data  = 1111;		 /* 假设定义的传感器数据 */	
uint16_t Sensor8_data  = 2222; 
uint16_t Sensor9_data  = 3333; 
uint16_t Sensor10_data = 4444; 
uint16_t Sensor11_data = 5555; 
uint16_t Sensor12_data = 6666; 
uint16_t Sensor13_data = 1111;		 /* 假设定义的传感器数据 */	
uint16_t Sensor14_data = 2222; 
uint16_t Sensor15_data = 3333; 
uint16_t Sensor16_data = 4444; 
uint16_t Sensor17_data = 5555; 
uint16_t Sensor18_data = 6666; 
uint16_t Sensor19_data = 6666; 
uint16_t Sensor20_data = 6666; 


uint16_t read_sensor(uint8_t code)
{
	uint16_t sensor_data = 0;
	switch(code)	/* 轮询读取命令字的数据 */
	{
		case(0x01): sensor_data = Sensor1_data; break;
		case(0x02): sensor_data = Sensor2_data; break;
		case(0x03): sensor_data = Sensor3_data; break;
		case(0x04): sensor_data = Sensor4_data; break;
		case(0x05): sensor_data = Sensor5_data; break;
		case(0x06): sensor_data = Sensor6_data; break;
		case(0x07): sensor_data = Sensor7_data; break;
		case(0x08): sensor_data = Sensor8_data; break;
		case(0x09): sensor_data = Sensor9_data; break;
		case(0x0A): sensor_data = Sensor10_data; break;
		case(0x0B): sensor_data = Sensor11_data; break;
		case(0x0C): sensor_data = Sensor12_data; break;
		case(0x0D): sensor_data = Sensor13_data; break;
		case(0x0E): sensor_data = Sensor14_data; break;
		case(0x0F): sensor_data = Sensor15_data; break;
		case(0x10): sensor_data = Sensor16_data; break;
		case(0x11): sensor_data = Sensor17_data; break;
		case(0x12): sensor_data = Sensor18_data; break;
		case(0x13): sensor_data = Sensor19_data; break;
		case(0x14): sensor_data = Sensor20_data; break;
		default:break;
	}
}

void MY_UART1_Idle_Callback(uint8_t size)
{
	uart1_rec_buf[size] = '\0';
	
	#if PRINTF_UART1
		printf("\r\n接收到%d个数据\r\n", size);
		for(int i=0; i<size; i++)
		{
			printf("rec[%d]=0x%x  ", i, uart1_rec_buf[i]);
		}
	#endif
	
	uint16_t rec_header = (uart1_rec_buf[0] << 8) | uart1_rec_buf[1];
	if(rec_header == UART1_FRAME_HANDER)					/* 判断帧头是否对应 */
	{
		
		#if PRINTF_UART1
			printf("\r\n帧头接收成功\r\n");
		#endif
		
		if(uart1_rec_buf[2] == UART1_ASK_CMD)				/* 判断ROS发送来的是否为询问传感器在线命令 */
		{
			#if PRINTF_UART1
				printf("询问命令接收成功\r\n");
			#endif
			if(uart1_rec_buf[3] == 0x00)							/* 如果是询问命令第3位为0x00 */
			{
				uint8_t rec_sum = test_crc_sum(uart1_rec_buf, size);	/* 这个函数的size不能用strlen()获得，数组中可能会有0x00的数据会影响strlen()*/
				if(rec_sum == uart1_rec_buf[4])					/* 判断crc_sum */
				{
					printf("在线传感器编号发送成功\r\n");
					/*发送在线传感器编号函数*/
				}
			}
		}
		
		
		else if(uart1_rec_buf[2] == UART1_DATA_CMD)	/* 判断ROS发送来的是否为获取传感器数据命令 */
		{
			#if PRINTF_UART1
				printf("数据命令接收成功\r\n");
			#endif
			uint8_t rec_code_num = uart1_rec_buf[3];	/* 得到ROS需要传感器的的数量 最多六个*/
			uint8_t count =3;													/* 传感器数据在数组中的起始位,应该是4的在下面的for循环中加一了*/
			uart1_send_data_buf[0] = 0xaa;						/* 定义发送帧头 */
			uart1_send_data_buf[1] = 0xff;
			uart1_send_data_buf[2] = 0xaa;						/* 定义数据命令字 */
			uart1_send_data_buf[3] = rec_code_num*2;	/* 定义发送数据长度 */
			
			for(int i=4; i<4+rec_code_num; i++)
			{
				uint16_t read;
				read = read_sensor(uart1_rec_buf[i]);
				count += 1;
				uart1_send_data_buf[count] = read >> 8;
				count += 1;
				uart1_send_data_buf[count] = read;
			}
			uart1_send_data_buf[count+1] = get_crc_sum(uart1_send_data_buf, count+2);
			printf("%s",uart1_send_data_buf);

			
			
			
			#if PRINTF_UART1
				uint8_t rec_code_buf[6];									/* 保存传来的传感器标号 */				
				for(int i=0; i<rec_code_num; i++)			
				{
					rec_code_buf[i] = uart1_rec_buf[3+1+i];	/* 保存数据同时保存顺序 */
				}
				printf("接收到%d个命令字\r\n", rec_code_num);
				for(int i=0; i<rec_code_num; i++)
				{
					printf("rec[%d]=0x%x  ", i, rec_code_buf[i]);
				}
			#endif
		}
	}
}













