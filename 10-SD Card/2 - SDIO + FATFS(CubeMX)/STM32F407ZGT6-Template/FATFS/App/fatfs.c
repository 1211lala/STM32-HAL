/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"
#include "./OLED/oled.h"


uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
	FRESULT fre;
	fre = f_mount(&SDFatFS, SDPath, 1);
	if(fre == FR_OK)
	{
		printf("fatfs mount ok\r\n");
		show_sdcard_info();
	}
	else
	{
		printf("fatfs error code %d\r\n", fre);
	}
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
void OLED_Show_Chinese(uint8_t x, uint8_t y, uint8_t* buff)
{

		uint8_t x0, y0;
		x0 = x, y0 = y;
		for (uint8_t i = 0; i < 32; i++) 
		{
			for (uint8_t j = 0; j < 8; j++) /* 写入一个字节的数据*/
			{
				if (buff[i] & 0x01) /* 按位读取字节数据*/
				{
					OLED_DrawPoint(x, y, 1);
				}else
				{
					OLED_DrawPoint(x, y, 0);
				}
				buff[i] = buff[i] >> 1; 
				y += 1;				
			}
			
			x += 1;										 /* x轴向右加一 */
			if (x - x0 == 16) /* 如果这一行数据显示完了,开始写入下一行的数据 */
			{
				x = x0;
				y0 = y0 + 8;
			}
			y = y0;
		}
}


uint8_t buff[32];
void Font_read_text(uint8_t x, uint8_t y, uint8_t* gbk)
{
	uint32_t addr = 0;
	printf("%s high %X 	low %X\r\n", gbk, gbk[0], gbk[1]);
	addr = ((gbk[0] - 0xa1) * 94 + gbk[1] - 0xa1) * 32;
	
	if(f_open(&SDFile, "H_16.HZK", FA_READ) == FR_OK )
	{
		printf("H_16.HZK open ok\r\n");
		f_lseek(&SDFile, addr);
	
		
		uint32_t br = 0;
		if(f_read(&SDFile, buff, 32,  &br)  == FR_OK )
		{
			printf("H_16.HZK read ok\r\n");
			
			for(uint8_t i=0; i<32; i++)
			{
				printf("0x%02x ", buff[i]);
			}
			
			OLED_Show_Chinese(x, y, buff);
			OLED_Refresh();
			f_close(&SDFile);
		}else
		{
			printf("H_16.HZK read fail\r\n");
		}
		
	}else
	{
		printf("H_16.HZK open fail\r\n");
	}
	
}



/* USER CODE END Application */
