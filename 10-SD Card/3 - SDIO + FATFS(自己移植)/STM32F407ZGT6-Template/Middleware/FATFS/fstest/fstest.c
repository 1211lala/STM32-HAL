#include "fstest.h"


/**
 * @brief       查看绝对路径下的文件和文件加不支持深度查看,不支持中文路径,输出结果串口查看
 * @param       path : 路径
 * @retval      执行结果(参见FATFS, FRESULT的定义)
 */
FRESULT fs_scan_dir(uint8_t* path)
{
	fs_status = f_opendir(FSdir, (const TCHAR*)path);
	if(fs_status == FR_OK)	/* 目录打开成功 */
	{
		while(1)
		{
			fs_status = f_readdir(FSdir, FSinfo);
			if((fs_status != FR_OK) || (FSinfo->fname[0] == 0))break;
//			if(FSinfo->fname[0] == '.') continue;
			printf("%s/",path);
			printf("%s\r\n", FSinfo->fname);
		}
		f_closedir(FSdir);
		return FR_OK;
	}else
	{
		printf("%s open fail! \r\n", path);
		return FR_DISK_ERR;
	}
}

/**
 * @brief       得到磁盘的大小和剩余大小,串口打印数据 但是实际测试32G的容量有问题
 * @param       path : 路径
 * @retval      执行结果(参见FATFS, FRESULT的定义)
 */
FRESULT fs_get_free(uint8_t* path)
{
	uint32_t fre_clust = 0;
	fs_status = f_getfree((const TCHAR*)path, (DWORD*)&fre_clust, &SDFATFS);
	if(fs_status == FR_OK)
	{
		uint32_t total = (SDFATFS->n_fatent - 2) * SDFATFS->csize;	/* 得到总扇区数 */
		uint32_t free  = fre_clust * SDFATFS->csize;          			/* 得到空闲扇区数 */
		#if FF_MAX_SS!=512
        tot_sect *= fs1->ssize / 512;
        fre_sect *= fs1->ssize / 512;
		#endif
		
		if (total < 20480)   /* 总容量小于20M */
		{
			printf("\r\n磁盘总容量:%d KB\r\n""可用空间:%d KB\r\n", total >> 1, free >> 1);
		}else
    {
			printf("\r\n磁盘总容量:%d MB\r\n""可用空间:%d MB\r\n",total >> 11, free >> 11);
		}
		return FR_OK;
	}else
	{
		printf("%s get free error.\r\n", path);
	}
	return FR_DISK_ERR;
}


/**
 * @brief       判断文件或者目录是否存在,串口打印数据
 * @param       path : 路径
 * @param       fsinfo : 带入结果
 * @retval      执行结果(参见FATFS, FRESULT的定义)
 */
FRESULT fs_judge_exist(uint8_t* path, FILINFO* fsinfo)
{
	fs_status = f_stat((const TCHAR*)path, fsinfo);
	if(fs_status == FR_OK)
	{
		#if 0
		printf("fsinfo->fsize %d \r\n",fsinfo->fsize);			/* 文件大小 */
		printf("fsinfo->fdate %d \r\n",fsinfo->fdate);			/* 修改日期 */
		printf("fsinfo->fname %s \r\n",fsinfo->fname);			/* 文件名称 */
		printf("fsinfo->ftime %d \r\n",fsinfo->ftime);			/* 修改时间 */
		printf("fsinfo->fattrib %d \r\n",fsinfo->fattrib);	/* 文件属性 */
		printf("%s exist!\r\n",path);
		#endif
		return FR_OK;
	}else
	{
		#if 0 
		printf("%s no exist!\r\n",path);
		#endif
	}
	return FR_DISK_ERR;
}



/**
 * @brief       创造一个新文件
 * @param       fsfile : 文件对象
 * @param       path : 路径
 * @retval      FR_OK 创造成功   FR_EXIST 文件已经创造 其他看(参见FATFS, FRESULT的定义)
 */
FRESULT fs_create_file(FIL* fsfile, uint8_t* path)
{
	return f_open(fsfile, (const TCHAR*)path, FA_CREATE_NEW|FA_READ|FA_WRITE);
}

/**
 * @brief       写入数据
 * @param       fsfile : 文件对象
 * @param       buf : 数据数组
 * @param       len : 数组长度
 * @param       write_len 实际写入长度
 * @param       lseek : 0/从头写入  1/从尾写入
 * @retval      FR_OK 写入成功  其他看(参见FATFS, FRESULT的定义)
 */
FRESULT fs_write(FIL* fsfile, uint8_t* buf, uint32_t len, uint32_t* write_len,uint8_t lseek)
{
	uint32_t size;
	if(lseek == 1)
	{
		size = f_size(fsfile);
		f_lseek(fsfile, size);
	}
	return f_write(fsfile ,buf, len, write_len);
}



uint8_t dir_path[]= "0:/STM32";
uint8_t file_path[]= "0:/STM32/file.txt";

void sd_test(void)
{
	static uint32_t da;
	da += 1;
	if(fs_judge_exist(dir_path, FSinfo) != FR_OK)
	{
		if( f_mkdir((const TCHAR*)dir_path) == FR_OK)
		{
			printf("%s create ok\r\n", dir_path);
		}
		else
		{
			printf("%s create fail\r\n", dir_path);
		}
	}
	
	fs_status = f_opendir(FSdir, (const TCHAR*)dir_path);
	if(fs_status ==FR_OK)
	{
		printf("%s open ok\r\n", dir_path);
		fs_status = fs_create_file(FSFile, file_path);
		
		f_write(FSFile, "12345\r\n56789", strlen("12345\r\n56789"), &fs_w_count);
		
		f_close(FSFile);
		f_closedir(FSdir);
		
	}else
	{
		printf("%s open fail\r\n", dir_path);
	}

}


void flash_test(void)
{
	printf("*********************************FLASH********************************\r\n");
	f_mount(FLASHFATFS, "1:", 0);
	//创造文件用
	fs_status = f_open(FSFile, "1:flash.txt", FA_CREATE_NEW);
	if( fs_status == FR_OK )
	{
		printf("1:flash.txt creat ok.  \r\n");
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt creat fail.  error code %d\r\n", fs_status);
	}
	fs_status = f_open(FSFile, "1:flash.txt", FA_WRITE);
	if( fs_status == FR_OK )
	{
		sprintf((char*)fs_buf, "1234567890");
		fsize = f_size(FSFile);
		f_lseek(FSFile, fsize);
		fs_status = f_write(FSFile, fs_buf, strlen((char*)fs_buf), &fs_w_count);
		if(fs_status == FR_OK)
		{
			printf("写入大小 %d\r\n", strlen((char*)fs_buf));
			printf("写入内容 %s\r\n", fs_buf);
		}
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt write fail.  error code %d\r\n", fs_status);
	}


	fs_status = f_open(FSFile, "1:flash.txt", FA_READ);
	if( fs_status == FR_OK )
	{
		fsize = f_size(FSFile);
		/* 数据读取 */
		fs_status = f_read(FSFile, fs_buf, fsize, &fs_r_count);
		if(fs_status == FR_OK)
		{
			fs_buf[fs_r_count] = '\0';
			printf("文件大小 %d\r\n", fsize);
			printf("读出大小 %d\r\n", fs_r_count);
			printf("%s\r\n\r\n", fs_buf);
		}
		f_close(FSFile);
	}else
	{
		printf("1:flash.txt read fail.  error code %d\r\n", fs_status);
	}
}

