#include "text.h"

/* 显示的是GB2312 高位的范围是 0xa1 - 0xf7  低位的范围为 0xa1 - 0xfe */
/* OLED的取模方式为 阴码 列行式 */




/**
 * 函数功能: 显示一个汉字
 * 输入参数: x 显示的x轴
 * 输入参数: y 显示的y轴
 * 输入参数: size 字体大小 必须是8的整数倍
 * 输入参数: buff 字体数组
 */
void FS_Show_Picture_Chinese(uint8_t x, uint8_t y, uint8_t size, uint8_t* buff, uint8_t mode)
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
				OLED_DrawPoint(x, y, mode);
			}else
			{
				OLED_DrawPoint(x, y, !mode);
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
 * 函数功能: 显示汉字 数字显示还没兼容
 * 输入参数: x 显示的x轴
 * 输入参数: y 显示的y轴
 * 输入参数: size 字体大小 必须是8的整数倍
 * 输入参数: buff 字体数组
 */
uint8_t FS_Show_Text(uint8_t x, uint8_t y, uint8_t size, uint8_t* gbk, uint8_t mode)
{
	
	switch(size)	
	{
		case(16):	fs_status =  f_open(FSFile, "H_16.HZK", FA_READ); break;
		case(24):	fs_status =  f_open(FSFile, "H_24.HZK", FA_READ); break;
		case(32):	fs_status =  f_open(FSFile, "H_32.HZK", FA_READ); break;
	}
	if( fs_status == FR_OK )
	{
		for(uint16_t i=0; i<strlen((char*)gbk); )
		{
			if(gbk[i] > 0x80)		/* 判断是否为GB2312 */
			{
				uint32_t addr;
				switch(size)			/* 读取字体大小 并计算一个字体大小 */
				{
					case(16): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 32;	break;
					case(24): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 72;	break;
					case(32): addr = ((gbk[i] - 0xa1) * 94 + gbk[i+1] - 0xa1) * 128;	break;
				}
				f_lseek(FSFile, addr);		/* 设置字体数据偏移 */
				 
				if(f_read(FSFile, fs_buf, size*size/8,  &fs_r_count)  == FR_OK )
				{
					FS_Show_Picture_Chinese(x, y, size, fs_buf, mode);	/* 实际写入数据 */
				}
				x += size;	/* 设置每个字之间的空隙 */
				i += 2;			/* GB2312加两个字节 */
			}else	/* 判断是否为ASCLL */
			{
				OLED_Show_Char(x, y, gbk[i], size, mode);	/* 实际写入数据 */
				x += size/2;	/* 设置每个ASCLL之间的空隙 */
				i += 1;		/* ASCLL加一个字节 */
			}
		}
		f_close(FSFile);
	}else
	{
		return 0;
	}
	return 1;
}

/**
 * 函数功能: 显示单色图片
 * 输入参数: x 显示的x轴
 * 输入参数: y 显示的y轴
 * 输入参数: size 字体大小 必须是8的整数倍
 * 输入参数: buff 字体数组
 */
uint8_t FS_Show_Picture(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end,uint8_t * path, uint32_t size, uint8_t mode)
{
	fs_status = f_open(FSFile, (const TCHAR*)path, FA_READ);
	if(fs_status == FR_OK)
	{
		if(f_read(FSFile, fs_buf, size,  &fs_r_count)  == FR_OK )
		{
			OLED_ShowPicture(x_start, y_start, x_end, y_end, fs_buf, mode);
			OLED_Refresh();
		}
		f_close(FSFile);
	}else
	{
		return 0;
	}
	return 1;
}

