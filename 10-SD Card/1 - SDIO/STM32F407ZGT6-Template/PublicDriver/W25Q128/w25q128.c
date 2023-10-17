/*****************************************************************

W25Q128的SPI模式为模式0或模式3。这里采用模式0
数据格式为8位，第一个bit为MSB

一个W25Q128有256的块
一个块有16个扇区 	(扇区 == 最小的的擦除单位)
一个扇区有16个页 	(页 == 最小的的写入单位)
一个页有256个字节 

地址从 0x000000 - 0xffffff

读状态寄存器SR1 
	拉低CS --> 发送0x05指令 --> 接收SR1的值 --> 拉高片选
	
页写时序
	拉低CS --> 发送0x02指令 --> 发送24位地址 --> 发送n个数据(n的范围为1-256) --> 拉高片选 
	
扇区擦除指令
	拉低CS --> 发送0x20指令 --> 发送24未地址 --> 拉高片选


在参考手册13页
状态寄存器SR1的值 bit0(BUSY) 0：空闲状态	1：忙碌状态
状态寄存器SR1的值 bit1(WEL) 0：写禁止	1：写使能

D0 --- MISO
D1 --- MOSI


注意 在向W25Q128写入大量数据(0.5kb - nkb之间要考虑有没有可能栈溢出)
stm32CubeMX默认的栈大小为 512b
写数据时因为定义了一个4k的全局数组，所以不用考虑
在读数据时要特别注意，比如要读一个1k字节的数据，就要把栈大小设置成2K左右(我是这样设置的，可以正常使用，小了就不能正常读数据，同时程序也卡死)


STM32的SPI1的速度手册上规定最快18Mhz,实测二分屏36Mhz也可以，但是稳定还是18Mhz
*****************************************************************/
#include "./W25Q128/w25q128.h"


uint16_t W25QXX_TYPE = W25Q256;
/**
 * @brief SPI1读写一个字节
 * @param TxData 要写入的字节
 * @return 返回收到的数据
 */
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
  uint8_t Rxdata;
	
	HAL_SPI_TransmitReceive(&hspi1,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    	
}

/*读取芯片ID
	0XEF13,表示芯片型号为W25Q80  
	0XEF14,表示芯片型号为W25Q16    
	0XEF15,表示芯片型号为W25Q32  
	0XEF16,表示芯片型号为W25Q64 
	0XEF17,表示芯片型号为W25Q128 	  
	0XEF18,表示芯片型号为W25Q256 
*/
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25QXX_CS_LOW;			/* 片选拉低 */	    
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS_HIGH;			/* 片选拉高 */	  
	#if 1
	printf("W25Q128 ID = 0X%X\r\n",Temp);
	#endif
	return Temp;
}   


/**读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
	状态寄存器1：
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:默认0,状态寄存器保护位,配合WP使用
	TB,BP2,BP1,BP0:FLASH区域写保护设置
	WEL:写使能锁定
	BUSY:忙标记位(1,忙;0,空闲)
	默认:0x00
	状态寄存器2：
	BIT7  6   5   4   3   2   1   0
	SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
	状态寄存器3：
	BIT7      6    5    4   3   2   1   0
	HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
	regno:状态寄存器号，范:1~3
	返回值:状态寄存器值
*/
uint8_t W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
	switch(regno)
	{
			case 1:
					command=W25X_ReadStatusReg1;    /* 读状态寄存器1指令 */
					break;
			case 2:
					command=W25X_ReadStatusReg2;    /* 读状态寄存器2指令 */
					break;
			case 3:
					command=W25X_ReadStatusReg3;    /* 读状态寄存器3指令 */
					break;
			default:
					command=W25X_ReadStatusReg1;    
					break;
	}    
	W25QXX_CS_LOW;                          /* 使能器件 */    
	SPI1_ReadWriteByte(command);            /* 发送读取状态寄存器命令 */    
	byte=SPI1_ReadWriteByte(0Xff);          /* 读取一个字节 */  
	W25QXX_CS_HIGH;                         /* 取消片选 */     
	return byte;   
} 


/**
 * @brief 写W25QXX状态寄存器
 * @param regno 要写入的状态寄存器
 * @param sr    要写入的数据
 * @return 无
 */
void W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
	uint8_t command=0;
	switch(regno)
	{
			case 1:
					command=W25X_WriteStatusReg1;    /* 写状态寄存器1指令 */
					break;
			case 2:
					command=W25X_WriteStatusReg2;    /* 写状态寄存器2指令 */
					break;
			case 3:
					command=W25X_WriteStatusReg3;    /* 写状态寄存器3指令 */
					break;
			default:
					command=W25X_WriteStatusReg1;    
					break;
	}   
	W25QXX_CS_LOW;                          /* 使能器件 */     
	SPI1_ReadWriteByte(command);            /* 发送读取状态寄存器命令 */    
	SPI1_ReadWriteByte(sr);                 /* 读取一个字节 */    
	W25QXX_CS_HIGH;                         /* 取消片选 */  	      
}   


/**
 * @brief W25QXX写使能	将WEL置位 
 */
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS_LOW;                           
  SPI1_ReadWriteByte(W25X_WriteEnable);     
	W25QXX_CS_HIGH;                           	      
} 

/**
 * @brief W25QXX写禁止	将WEL清零 
 */
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS_LOW;                          
  SPI1_ReadWriteByte(W25X_WriteDisable);  
	W25QXX_CS_HIGH;                          	      
}

/**
 * @brief 进入掉电模式
 */
void W25QXX_PowerDown(void)   
{ 
	W25QXX_CS_LOW;                           /* 使能器件 */   
	SPI1_ReadWriteByte(W25X_PowerDown);      /* 发送掉电命令 */  
	W25QXX_CS_HIGH;                          /* 取消片选 */     	      
	HAL_Delay(1);                     			 /* 等待TPD Delay_us(&htim6, 3)*/
} 

/**
 * @brief 退出掉电模式
 */
void W25QXX_WAKEUP(void)   
{  
	W25QXX_CS_LOW;                                 /* 使能器件 */   
	SPI1_ReadWriteByte(W25X_ReleasePowerDown);     /* 发送退出掉电命令 */  
	W25QXX_CS_HIGH;                                /* 取消片选 */   	      
	HAL_Delay(1);                     			       /* 等待TRES1 Delay_us(&htim6, 3)*/                    
}    
/**
 * @brief 等待空闲
 */
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   //等待BUSY位清空
}  

/**
 * @brief 擦除整个芯片	等待时间超长 
 */
void W25QXX_Erase_Chip(void)   
{                                   
  W25QXX_Write_Enable();                 /* 首先发送写使能 */
  W25QXX_Wait_Busy();   								 /* 等待空闲 */
	W25QXX_CS_LOW;                         /* 使能器件 */   
	SPI1_ReadWriteByte(W25X_ChipErase);    /* 发送片擦除命令 */  
	W25QXX_CS_HIGH;                        /* 取消片选 */     	      
	W25QXX_Wait_Busy();   				   			 /* 等待芯片擦除结束 */
} 


/**
 * @brief 擦除一个扇区最少时间:150ms
 * @param Dst_Addr 扇区地址 根据实际容量设置
 * @return 无
 */
void W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	Dst_Addr*=4096;
	W25QXX_Write_Enable();                  /* 首先发送写使能 */
	W25QXX_Wait_Busy();   									/* 等待空闲 */
	W25QXX_CS_LOW;                          /* 使能器件 */   
	SPI1_ReadWriteByte(W25X_SectorErase);   /* 发送扇区擦除命令 */  
	if(W25QXX_TYPE==W25Q256)                /* 如果是W25Q256的话地址为4字节的，要发送最高8位 */
	{
			SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>24)); /* 发送32bit地址 */   
	}
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  /* 发送24bit地址 */   
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8));   
	SPI1_ReadWriteByte((uint8_t)Dst_Addr);  
	W25QXX_CS_HIGH;                            	/* 取消片选 */     	      
	W25QXX_Wait_Busy();   				       				/* 等待擦除完成 */
}  



/**
 * @brief 设定是4字节地址还是3字节
 * @note  SP1的NSS是软件控制,在gpio.c中编写
 */
void W25QXX_Init(void)
{ 
	uint8_t temp;
	W25QXX_CS_HIGH;			                	/* SPI FLASH不选中 */
	W25QXX_TYPE=W25QXX_ReadID();	        /* 读取FLASHID */
	if(W25QXX_TYPE==W25Q256)              /* SPI FLASH为W25Q128 */
	{
		temp=W25QXX_ReadSR(3);              /* 读取状态寄存器3，判断地址模式 */
		if((temp&0X01)==0)			        		/* 如果不是4字节地址模式,则进入4字节地址模式 */
		{
			W25QXX_CS_LOW; 			        
			SPI1_ReadWriteByte(W25X_Enable4ByteAddr);	/*发送进入4字节地址模式指令 */  
			W25QXX_CS_HIGH;       		     
		}
	}
}  


/**
 * @brief 读取SPI FLASH 数据
 * @param pBuffer 数据存储区
 * @param ReadAddr 开始读取的地址(24bit)
 * @param NumByteToRead:要读取的字节数(最大65535)
 * @return 无
 */
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	W25QXX_CS_LOW;                          	  
	SPI1_ReadWriteByte(W25X_ReadData);      
	if(W25QXX_TYPE==W25Q256)                
	{
		SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>24));    
	}
	SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));       
	SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
	SPI1_ReadWriteByte((uint8_t)ReadAddr);  
	
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=SPI1_ReadWriteByte(0XFF);    /* 循环读数 */ 
	}
	W25QXX_CS_HIGH;  				    	      
}  

/**
 * @brief 页写 - 在指定地址开始写入最大256字节的数据
 * @param pBuffer 数据存储区
 * @param ReadAddr 开始写入的地址(24bit)
 * @param NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	
 * @return 无
 */
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25QXX_Write_Enable();                  /* 首先发送写使能 */
	W25QXX_CS_LOW;                            
	SPI1_ReadWriteByte(W25X_PageProgram);   /* 发送写页命令 */   
	if(W25QXX_TYPE==W25Q256)                /* 如果是W25Q256的话地址为4字节的，要发送最高8位 */
	{
			SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>24)); 
	}
	SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); 
	SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
	SPI1_ReadWriteByte((uint8_t)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
	{
		SPI1_ReadWriteByte(pBuffer[i]);/* 循环写数 */ 
	}
	W25QXX_CS_HIGH;                  /* 取消片选 */
	W25QXX_Wait_Busy();					     /* 等待写入结束 */
} 

/**
 * @brief 页写 - 无检验写SPI FLASH,具有自动换页功能,在指定地址开始写入指定长度的数据,但是要确保地址不越界!
 * @param pBuffer 数据存储区
 * @param ReadAddr 开始写入的地址(24bit)
 * @param NumByteToWrite 要写入的字节数(最大65535)!	
 * @return 无
 */
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; 	/* 得到地址所在页的剩余的字节数 */		 	    
	if(NumByteToWrite<=pageremain)	/* 如果本页能写完 */
	{
		pageremain=NumByteToWrite;	
	}
	while(1)
	{	 
		/*
		(1): 可以在本页写完，就直接写完退出		
		(2): 本页写不完
					1.先把本页的数据写完 W25QXX_Write_Page(pBuffer,WriteAddr,pageremain)
					2.写入数据数组要偏移已经写的数据个数 pBuffer+=pageremain;
					3.写入地址要增加已经写入数据的个数 WriteAddr+=pageremain;	
					4.需写入数据个数要减去已经写入数据个数 NumByteToWrite-=pageremain;同时判断是否小于256，如果小于256一页就可以写下了，满足(1)条件
					5.再次执行1.循环写入
		*/
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);	
		if(NumByteToWrite==pageremain)	/* 本页可以写完 */
		{
			break;
		}
	 	else /* 本页剩余地址写不完 */
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;		/* 减去已经写入了的字节数 */
			if(NumByteToWrite>256)
			{
				pageremain=256; 						/* 一次可以写入256个字节 */
			}
			else 
			{
				pageremain=NumByteToWrite; 	/* 不够256个字节了 */
			}
		}
	};   
}

/**
 * @note 	该函数带擦除操作!	
 * @brief 写SPI FLASH，在指定地址开始写入指定长度的数据
 * @param pBuffer 数据存储区
 * @param WriteAddr 开始写入的地址(24bit)
 * @param NumByteToWrite 要写入的字节数(最大65535)!	
 * @return 无
 */


//uint8_t W25QXX_BUFFER[4096];	
uint8_t* W25QXX_BUFFER = NULL;

void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	W25QXX_BUFFER = mymalloc(SRAMEX, 4096);
	
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;	/* 扇区地址 */ 
	secoff=WriteAddr%4096;	/* 在扇区内的偏移 */
	secremain=4096-secoff;	/* 扇区剩余空间大小 */   
 	//printf("ad:%X,nb:%d\r\n",WriteAddr,NumByteToWrite);/* 测试用 */
 	if(NumByteToWrite<=secremain)	/*扇区内的剩余地址够用 */
	{
		secremain=NumByteToWrite;
	}
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);/* 读出整个扇区的内容 */
		for(i=0;i<secremain;i++)/* 校验数据 */
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;/* 需要擦除这个扇区 */  	  
		}
		if(i<secremain)/* 检测到扇区内有已经被写过的地址 */
		{
			W25QXX_Erase_Sector(secpos);/* 擦除这个扇区 */
			for(i=0;i<secremain;i++)	  /* 复制 */
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);/* 写入整个扇区  */ 

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);/* 写已经擦除了的,直接写入扇区剩余区间 */
		if(NumByteToWrite==secremain)break;	/* 写入结束了 */
		else	/* 写入未结束 */
		{
			secpos++;	/* 扇区地址增1 */
			secoff=0;	/* 偏移位置为0 */ 	 

			pBuffer+=secremain;  	/* 指针偏移 */
			WriteAddr+=secremain;	/* 写地址偏移 */	   
			NumByteToWrite-=secremain;							/* 字节数递减 */
			if(NumByteToWrite>4096)secremain=4096;	/* 下一个扇区还是写不完 */
			else secremain=NumByteToWrite;					/* 下一个扇区可以写完了 */
		}	 
	};	 
}








