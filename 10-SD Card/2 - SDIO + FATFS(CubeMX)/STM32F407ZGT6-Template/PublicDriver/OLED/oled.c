#include "./OLED/oled.h"
#include "./OLED/oled_font.h"


uint8_t OLED_RAM[lie][hang/8];

/**
 * 函数功能: OLED发送一个字节
 * 输入参数: dat	发送的数据
 * 输入参数: mode:数据/命令标志 OLED_CMD 表示命令 OLED_DATA 表示数据;
 * 说    明:	实际是发送三个字节 分别位  器件地址  命令字符  数据
 */
void OLED_Write_Byte(uint8_t dat, uint8_t mode)
{
	/* 下面两个发送函数都可以 HAL_I2C_Mem_Write()会更快一点 */
	uint8_t buf[2];
	buf[0] = mode;
	buf[1] = dat;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ARDESS, buf, 2, 0xff);
	
	
//  HAL_I2C_Mem_Write(&hi2c1, OLED_ARDESS, mode, I2C_MEMADD_SIZE_8BIT, &dat,  1, 0xff);
	
	
}


/**
 * 函数功能: OLED初始化
 * 说    明:	
 */
void OLED_Init(void)
{

	HAL_Delay(100);					 /* OLED上电比MCU慢 */
	
	#if OLED_096 == 1
	OLED_Write_Byte(0xAE,OLED_CMD);   //display off
	OLED_Write_Byte(0x20,OLED_CMD);	//Set Memory Addressing Mode	
	OLED_Write_Byte(0x10,OLED_CMD);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_Write_Byte(0xb0,OLED_CMD);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_Write_Byte(0xc8,OLED_CMD);	//Set COM Output Scan Direction
	OLED_Write_Byte(0x00,OLED_CMD);//---set low column address
	OLED_Write_Byte(0x10,OLED_CMD);//---set high column address
	OLED_Write_Byte(0x40,OLED_CMD);//--set start line address
	OLED_Write_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_Write_Byte(0x7f,OLED_CMD);
	OLED_Write_Byte(0xa1,OLED_CMD);//--set segment re-map 0 to 127
	OLED_Write_Byte(0xa6,OLED_CMD);//--set normal display
	OLED_Write_Byte(0xa8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_Write_Byte(0x3F,OLED_CMD);//
	OLED_Write_Byte(0xa4,OLED_CMD);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_Write_Byte(0xd3,OLED_CMD);//-set display offset
	OLED_Write_Byte(0x00,OLED_CMD);//-not offset
	OLED_Write_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Byte(0xf0,OLED_CMD);//--set divide ratio
	OLED_Write_Byte(0xd9,OLED_CMD);//--set pre-charge period
	OLED_Write_Byte(0x22,OLED_CMD); //
	OLED_Write_Byte(0xda,OLED_CMD);//--set com pins hardware configuration
	OLED_Write_Byte(0x12,OLED_CMD);
	OLED_Write_Byte(0xdb,OLED_CMD);//--set vcomh
	OLED_Write_Byte(0x20,OLED_CMD);//0x20,0.77xVcc
	OLED_Write_Byte(0x8d,OLED_CMD);//--set DC-DC enable
	OLED_Write_Byte(0x14,OLED_CMD);//
	OLED_Clear(0);
	OLED_Refresh();
	OLED_Write_Byte(0xaf,OLED_CMD);//--turn on oled panel 
	#endif
	
	#if OLED_091
	OLED_Write_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_Write_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_Write_Byte(0x00, OLED_CMD); /*set display start line*/
	OLED_Write_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_Write_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_Write_Byte(0xff, OLED_CMD); /*128*/
	OLED_Write_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_Write_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_Write_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_Write_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
	OLED_Write_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_Write_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_Write_Byte(0x80, OLED_CMD);
	OLED_Write_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_Write_Byte(0x1f, OLED_CMD);
	OLED_Write_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_Write_Byte(0x00, OLED_CMD);
	OLED_Write_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_Write_Byte(0x40, OLED_CMD);
	OLED_Write_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
	OLED_Write_Byte(0x14, OLED_CMD);
	OLED_Clear(0);
	OLED_Write_Byte(0xAF, OLED_CMD); /*display ON*/
	#endif
}

/**
 * 函数功能: 开启OLED显示
 * 说    明: 无
 */
void OLED_DisPlay_On(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* 电荷泵使能 */
	OLED_Write_Byte(0x14, OLED_CMD); /* 开启电荷泵 */
	OLED_Write_Byte(0xAF, OLED_CMD); /* 点亮屏幕 */
}

/**
 * 函数功能: 关闭OLED显示
 * 说    明: 无
 */
void OLED_DisPlay_Off(void)
{
	OLED_Write_Byte(0x8D, OLED_CMD); /* 电荷泵使能 */
	OLED_Write_Byte(0x10, OLED_CMD); /* 关闭电荷泵 */
	OLED_Write_Byte(0xAE, OLED_CMD); /* 关闭屏幕 */
}

/**
 * 函数功能: OLED刷屏实际写数据函数
 * 输入参数: 无
 * 输入参数: 无
 * 说    明:	无
 */
void OLED_Refresh(void)
{
	uint8_t i;
	for (i = 0; i < hang/8; i++)
	{
		OLED_Write_Byte(0xb0 + i, OLED_CMD); 	/* 设置行起始地址 */
		OLED_Write_Byte(0x00, OLED_CMD);	 		/* 设置低列起始地址 只取0x00的低四位 */
		OLED_Write_Byte(0x10, OLED_CMD);			/* 设置高列起始地址 只取0x10的低四位*/
																					/* 合计 0x00 所以起始列地址 0x00 */
		
		
		/* 下面两个发送函数都可以 HAL_I2C_Mem_Write()会更快一点 */
		uint8_t buf[1+128];
		buf[0] = OLED_DATA;
		for (uint8_t n=1; n < 128+1 ;n++)
		{
			buf[n] = OLED_RAM[n-1][i];
		}
		HAL_I2C_Master_Transmit(&hi2c1, OLED_ARDESS, buf, 129, 0xff);
		
			
//		uint8_t buf[128];
//		for (uint8_t n = 0; n < 128;n++)
//		{
//			buf[n] = OLED_RAM[n][i];
//		}
//		HAL_I2C_Mem_Write(&hi2c1, OLED_ARDESS, OLED_DATA, I2C_MEMADD_SIZE_8BIT, buf,  128, 0xff);
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
	uint8_t i, j;

	if (mode)
	{
		dat = 0xff;
	}
	else
	{
		dat = 0x00;
	}
	for (i = 0; i < hang/8; i++)
	{
		for (j = 0; j < 128; j++)
		{
			OLED_RAM[j][i] = dat;
		}
	}
//	OLED_Refresh(); /* 更新显示 */
}

/**
 * 函数功能: 画点函数
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: t 填充/清空 1:填充 0:清空
 * 说    明:	无
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
	uint8_t i, j;
	i = y / 8; /* 算出数据在第几行 */
	j = y % 8; /* 算出数据在第几行的具体哪一位*/
	if (t)
	{
		OLED_RAM[x][i] |= 1 << j;
	}
	else
	{
		OLED_RAM[x][i] &= ~(1 << j);
	}
}

/**
 * 函数功能: 显示一个英文字符
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: chr 需要显示的字符
 * 输入参数: char_size 显示字符的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
/*
1. ----->		2. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
3. ----->		4. ------->
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0
*/
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t char_size, uint8_t mode)
{
	uint8_t ram_size, size1, chr1, array, x0, y0;
	x0 = x, y0 = y;

	ram_size = char_size / 8;
	if (char_size % 8)
	{
		ram_size += 1;
	}
	ram_size = ram_size * (char_size / 2); /* 这个/2就是得出这个字体的宽度是多少 如12*6 32*16, 如果要改成长和宽一样的字体就要改成 /1 同理推出其他的字体 */

	if(char_size == 8) ram_size = 6;	/* 为了兼容 8*6 的字体 */

	chr1 = chr - ' '; /* 计算偏移后的值 */

	for (uint8_t i = 0; i < ram_size; i++)
	{
		switch (char_size)
		{
			case(8) : array = ascll_0806[chr1][i];break;
			case(16): array = ascll_1608[chr1][i]; break;
			case(24): array = ascll_2412[chr1][i];break;
			case(32):break;
		}

		for (uint8_t j = 0; j < 8; j++) /* 写入一个字节的数据*/
		{
			if (array & 0x01) /* 按位读取字节数据*/
			{
				OLED_DrawPoint(x, y, mode);
			}
			else
			{
				OLED_DrawPoint(x, y, !mode);
			}
			array = array >> 1; /* 移位操作 */
			y += 1;				/* 显示的y轴位置加一 */
		}

		x += 1;										 /* x轴向右加一 */
		if ((size1 != 8) && ((x - x0) == size1 / 2)) /* 如果这一行数据显示完了,开始写入下一行的数据 */
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0; /* y的值加8*/
	}
}


/**
 * 函数功能: 显示英文字符串
 * 输入参数: x 坐标的x轴 范围:0-127
 * 输入参数: y 坐标的y轴 范围:0-31
 * 输入参数: str 需要显示的字符串
 * 输入参数: char_size 显示字符串的大小
 * 输入参数: mode 正常显示/反色显示 1 正常显示 0 反色显示
 * 说    明: 兼容 8*6的字体
 */
void OLED_Show_String(uint8_t x, uint8_t y, uint8_t *str, uint8_t str_size, uint8_t mode)
{
	while ((*str >= ' ') && (*str <= '~'))
	{
		OLED_Show_Char(x, y, *str, str_size, mode);
		if(str_size == 8)x += 6;
		else x += str_size / 2;
		str++;
	}
}

/**
 * 函数功能: 画直线寒素
 * 输入参数: x0 坐标的x轴 范围:0-127
 * 输入参数: y0 坐标的y轴 范围:0-31
 * 输入参数: x1 坐标的x轴 范围:0-127
 * 输入参数: y1 坐标的y轴 范围:0-31
 * 说    明: 无
 */
void OLED_Draw_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t dm = 0;
	uint8_t lx = abs(x1 - x0);
	uint8_t ly = abs(y1 - y0);
	float x = 0;;
	float y = 0;
	
	if(lx >= ly){dm = lx;}
	else dm = ly;
	
	float dx = (float)(x1 - x0) / dm;
	float dy = (float)(y1 - y0) / dm;
	
	x = (double)x0 + 0.5;
	y = (double)y0 + 0.5;
	
	for(uint8_t i=0; i<dm; i++)
	{
		OLED_DrawPoint(x, y, 1);
		x += dx;
		y += dy;
	}
}

/**
* 函数功能: 显示图片
 * 输入参数: x 图片的x轴起点
 * 输入参数: y 图片的y轴起点
 * 输入参数: bmp_x 图片的x长度
 * 输入参数: bmp_y 图片的y长度
 * 说    明: 无
 */
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t bmp_x,uint8_t bmp_y,uint8_t BMP[],uint8_t mode)
{
	uint16_t j=0;
	
	uint8_t x0=x,y0=y;
	
	bmp_y=bmp_y/8+((bmp_y%8)?1:0);
	
	for(uint8_t n=0;n<bmp_y;n++)
	{
		 for(uint8_t i=0;i<bmp_x;i++)
		 {
				uint8_t temp=BMP[j];
				j++;
				for(uint8_t m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==bmp_x)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}

	



