#ifndef _W25Q128_H_
#define _W25Q128_H_

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
/********************** 兼容的芯片ID ************************/
#define W25Q80 		0XEF13
#define W25Q16 		0XEF14
#define W25Q32 		0XEF15
#define W25Q64 		0XEF16
#define W25Q128		0XEF17
#define W25Q256 	0XEF18

#define NM25Q80 	0X5213
#define NM25Q16 	0X5214
#define NM25Q32 	0X5215
#define NM25Q64 	0X5216
#define NM25Q128	0X5217
#define NM25Q256 	0X5218

/********************* 定义使用的Flash **********************/
extern uint16_t W25QXX_TYPE;	

/********************* 定义使用的片选 ***********************/
/*关闭片选*/
#define W25QXX_CS_HIGH    HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port,W25Q128_CS_Pin,GPIO_PIN_SET)
/*打开片选*/
#define W25QXX_CS_LOW     HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port,W25Q128_CS_Pin,GPIO_PIN_RESET)


//指令表
#define W25X_WriteEnable				0x06 	/* 写使能，写入数据擦除数据前必须发送这条指令 */
#define W25X_WriteDisable				0x04 
#define W25X_ReadStatusReg1			0x05	/* 读SR1寄存器，通过返回值判断芯片是不是处于空闲状态 */
#define W25X_ReadStatusReg2			0x35 
#define W25X_ReadStatusReg3			0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData						0x03	/* 读数据指令，用于读取Flash的数据 */
#define W25X_FastReadData				0x0B 
#define W25X_FastReadDual				0x3B 
#define W25X_PageProgram				0x02	/* 页写指令，最多写入256个字节数据，多了会复写 */
#define W25X_BlockErase					0xD8 
#define W25X_SectorErase				0x20 	/* 扇区擦除，最小擦除4096个字节（4Kyte）*/
#define W25X_ChipErase					0xC7 
#define W25X_PowerDown					0xB9 
#define W25X_ReleasePowerDown		0xAB 
#define W25X_DeviceID						0xAB 
#define W25X_ManufactDeviceID		0x90	/* 读取制造商ID(手册51页)*/
#define W25X_JedecDeviceID			0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9


uint8_t SPI1_ReadWriteByte(uint8_t TxData);
uint16_t W25QXX_ReadID(void);
uint8_t W25QXX_ReadSR(uint8_t regno);
void W25QXX_Write_SR(uint8_t regno,uint8_t sr);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Wait_Busy(void);
void W25QXX_Erase_Chip(void) ;
void W25QXX_Erase_Sector(uint32_t Dst_Addr);


void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);

void W25QXX_Init(void);
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);


#endif




