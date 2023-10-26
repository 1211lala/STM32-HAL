#include "text.h"



/**
 * 函数功能: 显示一个数据
 * 输入参数: x 显示的x轴
 * 输入参数: y 显示的y轴
 * 输入参数: size 字体大小 必须是8的整数倍
 * 输入参数: buff 字体数组
 */
void OLED_Show_Chinese(uint8_t x, uint8_t y, uint8_t size, uint8_t* buff)
{
	uint8_t x0, y0;
	x0 = x;
	y0 = y;
	for (uint8_t i = 0; i < size*size/8; i++)	/* 写入数组数据 */
	{
		for (uint8_t j = 0; j < 8; j++) 
		{
			if (buff[i] & 0x01) /* 按位读取字节数据 */
			{
				OLED_DrawPoint(x, y, 1);
			}else
			{
				OLED_DrawPoint(x, y, 0);
			}
			buff[i] = buff[i] >> 1; 
			y += 1;				
		}
		x += 1;								/* x轴向右加一 */
		if (x - x0 == size) 	/* 如果这一行数据显示完了,开始写入下一行的数据 */
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}


/**
 * 函数功能: 显示一个数据
 * 输入参数: x 显示的x轴
 * 输入参数: y 显示的y轴
 * 输入参数: size 字体大小 必须是8的整数倍
 * 输入参数: buff 字体数组
 */
void OLED_Show_Text(uint8_t x, uint8_t y, uint8_t size, uint8_t* gbk)
{
	
	switch(size)	
	{
		case(16):	fs_status =  f_open(FSFile, "H_16.HZK", FA_READ); break;
		case(24):	fs_status =  f_open(FSFile, "H_24.HZK", FA_READ); break;
	}
	if( fs_status == FR_OK )
	{
		for(uint16_t i=0; i<strlen((char*)gbk); i += 2)
		{
			uint32_t addr;
			switch(size)
			{
				case(16): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 32;break;
				case(24): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 72;break;
			}
			f_lseek(FSFile, addr);
			
			
			if(f_read(FSFile, fs_buf, size*size/8,  &fs_r_count)  == FR_OK )
			{
				printf("fs_buf %s\r\n",fs_buf);
				OLED_Show_Chinese(x, y, size, fs_buf);
			}else
			{
				printf("H_16.HZK read fail\r\n");
			}
			x += size+1;
		}
		f_close(FSFile);
	}else
	{
		printf("H_16.HZK open fail\r\n");
	}
}

