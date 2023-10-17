创建时间2023.9.22 

修改时间 -- 2023.9.25 取消了使用 use Micro LIB 库 为了解决程序运行不了的问题在private.c中添加以下代码

	/* 告知连接器不从C库链接使用半主机的函数 */
	#pragma import(__use_no_semihosting)
	 
	/* 定义 _sys_exit() 以避免使用半主机模式 */
	void _sys_exit(int x)
	{
		x = x;
	}
	 
	/* 标准库需要的支持类型 */
	struct __FILE
	{
		int handle;
	};
	 
	FILE __stdout;
	
	
参考连接 https://blog.csdn.net/qq_36098477/article/details/117666808  



/********************************************************************************************************************************************************************/


CubeMX生成SD的bug
	在选用了四线SDIO接口时MX生成的代码的参数是 SDIO_BUS_WIDE_4B
	但是在初始化时不能一开始就用 4线模式，应该手动改成 SDIO_BUS_WIDE_1B
	在调用 HAL_SD_Init() 函数后， 再调用 if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)设置为4线模式
	不然会进入系统错误函数去
	
	SDIO_TRANSFER_CLK_DIV HAL库帮我们定义的一个设置SDIO的宏 最大 48/0+2 =24Mhz 
	且在初始化时钟频率不能高于400K，SDIO_INIT_CLK_DIV 但是这个时HAL库自己做的不用我们考虑 

	
	
CubeMX生成的FATFS  
	在 sd_diskio.c中定义了接口函数
	static DSTATUS SD_CheckStatus(BYTE lun);
	DSTATUS SD_initialize(BYTE lun);
	DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count);
	DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);
	
	
	
	
	