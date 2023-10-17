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
	
	
	
 
FATFS
	1. 官网地址 http://elm-chan.org/fsw//ff/00index_e.html 
	2. 下载地址 http://elm-chan.org/fsw/ff/archives.html
	2. CubeMX是使用的R0.12 ,移植使用的是R0.14a版本
	
	ffconf.h  	FATFS配置文件
	ff.h		FATFS和应用模块公用的包含文件
	ff.c		FATFS的源码文件
	diskio.h	FATFS和应diskio公用的包含文件
	diskio.c	底层接口文件
	ffunicode.c 字体代码转换表
	ffsystem.c	与OS相关的示例代码
	
移植过程
	一般移植 5 个函数，在 diskio.c 里面。
	disk_initialize 函数	初始化磁盘驱动器
	disk_status函数	返回当前磁盘驱动器的状态
	disk_read 	从磁盘驱动器上读取扇区
	disk_write	向磁盘写入一个或多个扇区
	disk_ioctl 控制设备指定特性和除了读/写外的杂项功能 
	 
	为了OS做准备修改了ff.c里面的一些代码
	 
实际测得 在16Mhz可以正常读写 24Mhz只能读写入有问题

不支持exFat,不支持长文件名

	
	