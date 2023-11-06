#include "./AHT20/aht20.h"


/**
 * @brief   写入IIC写地址加一个字节数据
 * @param		data : 写入的数据
 * @retval  无
 */
void AHT20_Write_Byte(uint8_t data)
{
	iic1_start();
	iic1_send_byte(W_ADDRESS);
	iic1_wait_ack();
	iic1_send_byte(data);
	iic1_wait_ack();
	iic1_stop();
}

/**
 * @brief   写入IIC写地址+指令+两个数据
 * @param		cmd  : 写入的指令
 * @param		data : 写入的数据
 * @param		data : 数据长度
 * @retval  无
 */
void AHT20_Write_Bytes(uint8_t cmd, uint8_t * data, uint16_t len)
{
	iic1_start();
	iic1_send_byte(W_ADDRESS);
	iic1_wait_ack();
	iic1_send_byte(cmd);
	iic1_wait_ack();
	for(uint16_t i=0; i<len; i++)
	{
		iic1_send_byte(*(data+i));
		iic1_wait_ack();
	}
	iic1_stop();
}

/**
 * @brief   获得AHT20的状态字节
 * @retval  状态字节
 */
uint8_t AHT20_Get_Status_Byte(void)
{
	iic1_start();
	iic1_send_byte(0x71);
	iic1_wait_ack();
	uint8_t rec = iic1_read_byte(0);
	iic1_stop();	
	return rec;
}

/**
 * @brief   获得AHT20的状态字节的bit3位
 * @retval  1:bit3 = 1可以读取 / 0：bit3 = 0 不可以读取需发送初始化命令
 */
uint8_t AHT20_Get_Cal_Enable(void)
{
	uint8_t status = AHT20_Get_Status_Byte();
	if((status & 0x08) == 0x08)			/* 官方的例程是与0x68相与，我是只判断 bit3 Cal_Enable位 */
	{
		return 1;	/* 可以读取 */
	}
	return 0;		/* 不可以读取 */
}


/**
 * @brief   对AHT20进行初始化，
 * @retval  1:bit3 = 1可以读取 / 0：bit3 = 0 不可以读取需发送初始化命令
 * @note  因为AHT20的上电比MCU慢要在此函数前延时50ms左右
 * @note  注意此函数没有对IIC硬件接口初始化，须在使用这个函数前使用iic1_init()函数
 */
uint8_t AHT20_Init(void)
{
	uint8_t status = AHT20_Get_Cal_Enable();
	if(status){return 1;}
	else
	{
		uint8_t data[2] = {0x08, 0x00};
		AHT20_Write_Bytes(INIT_CMD, data, 2);
		HAL_Delay(10);
		status = AHT20_Get_Cal_Enable();
		if(status){return 1;}
	}
	return 0;
}

/**
 * @brief 触发AHT20测量函数 
 * @note  函数触发测量后要等待75ms，才能读取AHT20的值
 */
void AHT20_Start_Measure(void)
{
	uint8_t data[2] = {0x33, 0x00};
	AHT20_Write_Bytes(START_CMD, data, 2);
}

/**
 * @brief   读取ATH20的温湿度原始数据
 * @param		buf  : 保存数据的地址
 * @retval  无
 */
uint8_t AHT20_Read_Hex_Hum_Temp(uint32_t* buf)
{
	uint32_t hum_temp = 0;
	
	uint8_t rec = AHT20_Get_Status_Byte();
	
	while(rec & 0x80 )	/* 等待转换完成, 如果等待100ms还没数据就退出函数 */
	{
		
	}
	
	iic1_start();
	iic1_send_byte(0x71);
	iic1_wait_ack();
	
	uint8_t byte_1th = iic1_read_byte(1);	/* 状态字节不需要计算 */
	uint8_t byte_2th = iic1_read_byte(1);	/* 湿度数据 */
	uint8_t byte_3th = iic1_read_byte(1);	/* 湿度数据 */
	uint8_t byte_4th = iic1_read_byte(1);	/* 半温半湿*/
	uint8_t byte_5th = iic1_read_byte(1);	/* 温度数据 */
	uint8_t byte_6th = iic1_read_byte(0);	/* 温度数据 */
	
	hum_temp = (hum_temp|byte_2th)<<8;
	hum_temp = (hum_temp|byte_3th)<<8;
	hum_temp = (hum_temp|byte_4th);
	hum_temp =hum_temp >>4;
	buf[0] = hum_temp;   		//湿度
	hum_temp = 0;
	hum_temp = (hum_temp|byte_4th)<<8;
	hum_temp = (hum_temp|byte_5th)<<8;
	hum_temp = (hum_temp|byte_6th);
	hum_temp = hum_temp&0xfffff;
	buf[1] =hum_temp; 			//温度
	
	return 1;
}



/**
 * @brief   读取ATH20的Float数据 
 * @param		buf :保存数据的地址 buf[0] = 湿度  buf[1] = 温度
 * @retval  无
 */
uint8_t AHT20_Read_Float_Hum_Temp(float *buf)
{
	uint32_t hex_data[2];
	
	
	AHT20_Init();
	AHT20_Start_Measure();
	
	HAL_Delay(75);
	uint8_t status = AHT20_R ead_Hex_Hum_Temp(hex_data);

	
	buf[0] = (hex_data[0]*100*10/1024/1024);  
	buf[1] = (hex_data[1]*200*10/1024/1024-500);
	
	return 1;
}

