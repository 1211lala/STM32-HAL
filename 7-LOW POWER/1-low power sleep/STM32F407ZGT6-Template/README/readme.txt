创建时间2023.9.26  工程内容:使用SSD1306驱动128*32的oled屏幕(软件IIC) 	
参考连接 https://www.bilibili.com/video/BV19u4y197df/?spm_id_from=333.1007.top_right_bar_window_history.content.click







修改工程 --  取消了使用 use Micro LIB 库 为了解决程序运行不了的问题在private.c中添加以下代码

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
	
	
参考连接 https://blog.csdn.net/qq_36098477/article/details/11766 