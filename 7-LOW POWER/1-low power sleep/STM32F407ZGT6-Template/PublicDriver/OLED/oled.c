#include ".\OLED\oled.h"

uint8_t OLED_RAM[128][4];	


/**
  * 函数功能: OLED发送一个字节
  * 输入参数: dat	发送的数据
  * 输入参数: mode:数据/命令标志 OLED_CMD 表示命令 OLED_DATA 表示数据;
  * 说    明:	实际是发送三个字节 分别位  器件地址  命令字符  数据
  */
void OLED_Write_Byte(uint8_t dat,uint8_t mode)
{
	iic_start();
	iic_send_byte(OLED_ARDESS);	/* 写地址 */
	iic_wait_ack();
	iic_send_byte(mode);				/* 写命令字符 */
	iic_wait_ack();
	iic_send_byte(dat);					/* 写数据*/
	iic_wait_ack();
	iic_stop();
}

/**
  * 函数功能: OLED初始化
  * 说    明:	没有包含IIC引脚的初始化所以要放在IIC初始化之后
  */
void OLED_Init(void)
{
	
	HAL_Delay(100);		/* OLED上电比MCU慢 */
	OLED_Write_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_Write_Byte(0x00,OLED_CMD); /*set lower column address*/ 
	OLED_Write_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_Write_Byte(0x00,OLED_CMD); /*set display start line*/ 
	OLED_Write_Byte(0xB0,OLED_CMD); /*set page address*/ 
	OLED_Write_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_Write_Byte(0xff,OLED_CMD); /*128*/ 
	OLED_Write_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_Write_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	OLED_Write_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_Write_Byte(0x1F,OLED_CMD); /*duty = 1/32*/ 
	OLED_Write_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	OLED_Write_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_Write_Byte(0x00,OLED_CMD); 
	OLED_Write_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_Write_Byte(0x80,OLED_CMD); 
	OLED_Write_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_Write_Byte(0x1f,OLED_CMD); 
	OLED_Write_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_Write_Byte(0x00,OLED_CMD); 
	OLED_Write_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
	OLED_Write_Byte(0x40,OLED_CMD); 
	OLED_Write_Byte(0x8D,OLED_CMD); /*set charge pump enable*/ 
	OLED_Write_Byte(0x14,OLED_CMD);
	OLED_Clear(0);
	OLED_Write_Byte(0xAF,OLED_CMD); /*display ON*/ 
}

/**
  * 函数功能: 开启OLED显示 
  * 说    明: 无
  */
void OLED_DisPlay_On(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);	/* 电荷泵使能 */
	OLED_Write_Byte(0x14,OLED_CMD);	/* 开启电荷泵 */
	OLED_Write_Byte(0xAF,OLED_CMD);	/* 点亮屏幕 */
}

/**
  * 函数功能: 关闭OLED显示 
  * 说    明: 无
  */
void OLED_DisPlay_Off(void)
{
	OLED_Write_Byte(0x8D,OLED_CMD);/* 电荷泵使能 */
	OLED_Write_Byte(0x10,OLED_CMD);/* 关闭电荷泵 */
	OLED_Write_Byte(0xAE,OLED_CMD);/* 关闭屏幕 */
}

/**
  * 函数功能: OLED刷屏实际写数据函数
  * 输入参数: 无
  * 输入参数: 无
  * 说    明:	无
  */
void OLED_Refresh(void)
{
	uint8_t i,n;
	for(i=0;i<4;i++)
	{
		OLED_Write_Byte(0xb0+i,OLED_CMD); /* 设置行起始地址 */
		OLED_Write_Byte(0x00,OLED_CMD);   /* 设置低列起始地址 只取0x00的低四位 */
		OLED_Write_Byte(0x10,OLED_CMD);   /* 设置高列起始地址 只取0x10的低四位*/	
		iic_start();											/* 合计 0x00 所以启示列地址 0x00 */
		iic_send_byte(0x78);
		iic_wait_ack();
		iic_send_byte(0x40);
		iic_wait_ack();
		for(n=0;n<128;n++)
		{
			iic_send_byte(OLED_RAM[n][i]);
			iic_wait_ack();
		}
		iic_stop();
  }
}


/**
  * 函数功能: OLED屏幕清屏函数
	* 输入参数: mode 全屏点亮刷屏/全屏熄灭刷屏  1:亮  0:灭
  * 说    明:	无
  */
void OLED_Clear(uint8_t mode)
{
	uint8_t dat;
	uint8_t i,n;
	
	if(mode){dat = 0xff;}
	else{dat = 0x00;}
	for(i=0;i<4;i++)
	{
	   for(n=0;n<128;n++)
			{
				OLED_RAM[n][i]=dat;
			}
  }
	OLED_Refresh(); /* 更新显示 */
}

/**
  * 函数功能: 画点函数
	* 输入参数: x 坐标的x轴 范围:0-127
	* 输入参数: y 坐标的y轴 范围:0-31
	* 输入参数: t 填充/清空 1:填充 0:清空	
  * 说    明:	无
  */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_RAM[x][i]|=n;}
	else
	{
		OLED_RAM[x][i]=~OLED_RAM[x][i];
		OLED_RAM[x][i]|=n;
		OLED_RAM[x][i]=~OLED_RAM[x][i];
	}
}

