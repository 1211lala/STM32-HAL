/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"		/* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

#include "main.h"
#include "sdio.h"
#include "./SDIO_CARD/sdio_card.h"
/* Definitions of physical drive number for each drive */

#define DEV_SD 0 /* 定义设备 如果增加设备 同时也要修改 #define FF_VOLUMES	  */
#define DEV_FLASH 1

/**
 * 对于25Q128 FLASH芯片, 我们规定前 12M 给FATFS使用, 12M以后
 * 紧跟字库, 3个字库 + UNIGBK.BIN, 总大小3.09M, 共占用15.09M
 * 15.09M以后的存储空间大家可以随便使用.
 */
#define SPI_FLASH_SECTOR_SIZE 512
#define SPI_FLASH_SECTOR_COUNT 12 * 1024 * 2 /* 25Q128, 前12M字节给FATFS占用 */
#define SPI_FLASH_BLOCK_SIZE 8				 /* 每个BLOCK有8个扇区 */
#define SPI_FLASH_FATFS_BASE 0				 /* FATFS 在外部FLASH的起始地址 从0开始 */
/*-----------------------------------------------------------------------*/
/* Get Drive Status获取SD卡状态                                          */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv) /* Physical drive nmuber to identify the drive */
{
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv) /* Physical drive nmuber to identify the drive */
{
	if (pdrv == DEV_SD)
	{
		MX_SDIO_SD_Init();
	}

	if (pdrv == DEV_FLASH)
	{
		W25QXX_Init();
	}
	return RES_OK; /* 默认成功 */
}

/*-----------------------------------------------------------------------*/
//	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
//	BYTE *buff,		/* Data buffer to store read data */
//	LBA_t sector,	/* Start sector in LBA */
//	UINT count		/* Number of sectors to read */*/
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
	uint8_t status;
	if (pdrv == DEV_SD)
	{
		status = sd_read_disk(buff, sector, count);
		if (status)
			return RES_ERROR;
		else
			return RES_OK;
	}
	else if (pdrv == DEV_FLASH)
	{
		for (; count > 0; count--)
		{
			W25QXX_Read(buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
			sector++;
			buff += SPI_FLASH_SECTOR_SIZE;
		}
		return RES_OK;
	}

	return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
// Write Sector(s)
//	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
//	const BYTE *buff,	/* Data to be written */
//	LBA_t sector,		/* Start sector in LBA */
//	UINT count			/* Number of sectors to write */*/
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
	int result;
	if (pdrv == DEV_SD)
	{
		result = sd_write_disk((uint8_t *)buff, sector, count);
		if (result == 0)
		{
			return RES_OK;
		}
	}

	else if (pdrv == DEV_FLASH)
	{
		for (; count > 0; count--)
		{
			W25QXX_Write((uint8_t *)buff, SPI_FLASH_FATFS_BASE + sector * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_SECTOR_SIZE);
			sector++;
			buff += SPI_FLASH_SECTOR_SIZE;
		}
		return RES_OK;
	}

	return RES_ERROR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
//	BYTE pdrv,		/* Physical drive nmuber (0..) */
//	BYTE cmd,		/* Control code */
//	void *buff		/* Buffer to send/receive control data */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
	DRESULT res;
	if (pdrv == DEV_SD)
	{
		switch (cmd)
		{
		case CTRL_SYNC:
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE:
			*(DWORD *)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE:
			*(WORD *)buff = g_sd_card_info_handle.LogBlockSize;
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT:
			*(DWORD *)buff = g_sd_card_info_handle.LogBlockNbr;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
		}
	}

	else if (pdrv == DEV_FLASH)
	{
		switch (cmd)
		{
		case CTRL_SYNC:
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE:
			*(WORD *)buff = SPI_FLASH_SECTOR_SIZE;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE:
			*(WORD *)buff = SPI_FLASH_BLOCK_SIZE;
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT:
			*(DWORD *)buff = SPI_FLASH_SECTOR_COUNT;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
		}
	}
	else
	{
		res = RES_ERROR;
	}
	return res;
}
