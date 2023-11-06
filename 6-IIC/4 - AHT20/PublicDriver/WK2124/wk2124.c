#include "./WK2124/wk2124.h"

/*
1、WK2124 作为 SPI 从设备，支持 SPI 模式 0,即在主机端需要设置 CPOL=0(SPI 时钟极性选择位），CPHA=0( SPI 时钟相位选择位)。
2、片选信号在空闲时应该保持为高电平，拉低片选信号作为一次读写操作的开始，拉高作为一次操作的结束。
*/

uint8_t SPI2_CS_Ctrl(uint8_t cs)
{
	if (cs == 0)
	{
		HAL_GPIO_WritePin(WK_CS_GPIO_Port, WK_CS_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(WK_CS_GPIO_Port, WK_CS_Pin, GPIO_PIN_SET);
	}
	return 0;
}

HAL_StatusTypeDef SPI2_TransmitReceive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	HAL_StatusTypeDef ret;
	SPI2_CS_Ctrl(0);
	ret = HAL_SPI_TransmitReceive(&hspi2, pTxData, pRxData, Size, 10);
	SPI2_CS_Ctrl(1);
	return ret;
}

/**@Description WK2124初始化电平
 */
void Wk2xxxRstInit(void)
{
	HAL_GPIO_WritePin(WK_RST_GPIO_Port, WK_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(WK_RST_GPIO_Port, WK_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

/**@Description 向全局寄存器中写数据
 *@param greg:全局寄存器地址
 *@param dat:向全局寄存器中写的值
 *@retval 无
 */
void WkWriteGlobalRegister(uint8_t greg, uint8_t dat)
{
	uint8_t tdat[2], rdat[2], cmd;
	cmd = 0 | greg; /* 写寄存器时控制字节cmd的bit7 bit6 设置为 00 21页*/
	tdat[0] = cmd;	/* 地址 */
	tdat[1] = dat;	/* 数据 */
	SPI2_TransmitReceive(tdat, rdat, 2);
}

/**@Description 从全局寄存器中读数据
 *@param  greg:全局寄存器地址
 *@retval :读取到的值
 */
uint8_t WkReadGlobalRegister(uint8_t greg)
{
	uint8_t tdat[2], rdat[2], cmd;
	cmd = 0x40 | greg; /* 读寄存器时控制字节cmd的bit7 bit6 设置为 01 21页*/
	tdat[0] = cmd;
	SPI2_TransmitReceive(tdat, rdat, 2);
	return rdat[1];
}

/**@Description 写子串口寄存器数据
	* @param port:子串口号
	* @param sreg:子串口的4位地址
	* @param dat:向子串口地址写入的值
	* @retval
	C1 C2 == 0 0 串口一
	C1 C2 == 0 1 串口二
	C1 C2 == 1 0 串口三
	C1 C2 == 1 1 串口四
	*写子串口寄存器格式 == 0 0 C1 C2 + 四位子串口地址
*/
void WkWriteSlaveRegister(uint8_t port, uint8_t sreg, uint8_t dat)
{
	uint8_t tdat[2], rdat[2], cmd;
	cmd = 0x0 | ((port - 1) << 4) | sreg;
	tdat[0] = cmd;
	tdat[1] = dat;
	SPI2_TransmitReceive(tdat, rdat, 2);
}

/**@Description 读子串口寄存器数据
	*@param  sreg:子串口号
	*@param  sreg:子串口的4位地址
	*@retval :Read 从子串口地址读出的值

	C1 C2 == 0 0 串口一
	C1 C2 == 0 1 串口二
	C1 C2 == 1 0 串口三
	C1 C2 == 1 1 串口四
	*读子串口寄存器格式 == 0 1 C1 C2 +四位子串口地址
*/
uint8_t WkReadSlaveRegister(uint8_t port, uint8_t sreg)
{
	uint8_t tdat[2], rdat[2], cmd;
	cmd = 0x40 | ((port - 1) << 4) | sreg;
	tdat[0] = cmd;
	SPI2_TransmitReceive(tdat, rdat, 2);
	return rdat[1];
}

/**@Description 向子串口FIFO中写数据
* @param port:串口号
* @param dat:发送的数据
* @param len:发送数据的长度
* @retval
	C1 C2 == 0 0 串口一
	C1 C2 == 0 1 串口二
	C1 C2 == 1 0 串口三
	C1 C2 == 1 1 串口四
	*写子串口FIFO格式 == 1 0 C1 C2 + x x x x
*/
void WkWriteSlaveFifo(uint8_t port, uint8_t *dat, uint16_t len)
{
	uint8_t tdat[257], rdat[257], cmd;
	int i;
	cmd = 0x80 | ((port - 1) << 4);
	tdat[0] = cmd;
	for (i = 0; i < len; i++)
	{
		tdat[1 + i] = *(dat + i);
	}
	SPI2_TransmitReceive(tdat, rdat, len + 1);
}

/**@Description 从子串口FIFO中读数据
	* @param port:串口号
	* @param rec:接收的长度
	* @param len:接收数据的长度
	* @retval
	C1 C2 == 0 0 串口一
	C1 C2 == 0 1 串口二
	C1 C2 == 1 0 串口三
	C1 C2 == 1 1 串口四
	*读子串口FIFO格式 == 1 1 C1 C2 + x x x x
*/
void WkReadSlaveFifo(uint8_t port, uint8_t *rec, int len)
{
	uint8_t tdat[257], rdat[257], cmd;
	int i;
	cmd = 0xc0 | ((port - 1) << 4);
	tdat[0] = cmd;
	SPI2_TransmitReceive(tdat, rdat, len + 1);
	for (i = 0; i < len; i++)
	{
		*(rec + i) = rdat[1 + i];
	}
}

/**
 *@Description 初始化子串口
 */
void WkUartInit(uint8_t port)
{
	uint8_t gena, grst, gier, sier, scr;
	/* 读取全局寄存器中的值 */
	gena = WkReadGlobalRegister(WK2XXX_GENA_REG);
	/* 使能子串口时钟 */
	gena = gena | (1 << (port - 1));
	WkWriteGlobalRegister(WK2XXX_GENA_REG, gena);
	/* 读取全局子串口复位寄存器的值 */
	grst = WkReadGlobalRegister(WK2XXX_GRST_REG);
	/* 软件复位子串口 */
	grst = grst | (1 << (port - 1));
	WkWriteGlobalRegister(WK2XXX_GRST_REG, grst);
	/*使能串口总中断 */
	gier = WkReadGlobalRegister(WK2XXX_GIER_REG);
	gier = gier | (1 << (port - 1));
	WkWriteGlobalRegister(WK2XXX_GIER_REG, gier);
	// 使能子串口接收触点中断和超时中断
	sier = WkReadSlaveRegister(port, WK2XXX_SIER_REG);
	sier |= SIER_RFTRIG_IEN_BIT | SIER_RXOUT_IEN_BIT;
	WkWriteSlaveRegister(port, WK2XXX_SIER_REG, sier);
	// 初始化FIFO和设置固定中断触点
	WkWriteSlaveRegister(port, WK2XXX_FCR_REG, 0XFF);
	// 设置任意中断触点，如果下面的设置有效，
	// 那么上面FCR寄存器中断的固定中断触点将失效
	WkWriteSlaveRegister(port, WK2XXX_SPAGE_REG, 1);   // 切换到page1
	WkWriteSlaveRegister(port, WK2XXX_RFTL_REG, 0X40); // 设置接收触点为64个字节
	WkWriteSlaveRegister(port, WK2XXX_TFTL_REG, 0X10); // 设置发送触点为16个字节
	WkWriteSlaveRegister(port, WK2XXX_SPAGE_REG, 0);   // 切换到page0
	// 使能子串口的发送和接收使能
	scr = WkReadSlaveRegister(port, WK2XXX_SCR_REG);
	scr |= SCR_TXEN_BIT | SCR_RXEN_BIT;
	WkWriteSlaveRegister(port, WK2XXX_SCR_REG, scr);
}

/**
 *@Description 复位子串口
 */
void WkUartDeInit(uint8_t port)
{
	uint8_t gena, grst, gier;
	// 关闭子串口总时钟
	gena = WkReadGlobalRegister(WK2XXX_GENA_REG);
	gena = gena & (~(1 << (port - 1)));
	WkWriteGlobalRegister(WK2XXX_GENA_REG, gena);
	// 使能子串口总中断
	gier = WkReadGlobalRegister(WK2XXX_GIER_REG);
	gier = gier & (~(1 << (port - 1)));
	WkWriteGlobalRegister(WK2XXX_GIER_REG, gier);
	// 软件复位子串口
	grst = WkReadGlobalRegister(WK2XXX_GRST_REG);
	grst = grst | (1 << (port - 1));
	WkWriteGlobalRegister(WK2XXX_GRST_REG, grst);
}

/****@Description 设置子串口波特率函数、此函数中波特率的匹配值是根据16Mhz下的外部晶振计算的
 * @param port:子串口号
 * baud:波特率大小.波特率表示方式，
 * @retval
 */
uint8_t WkUartSetBaud(uint8_t port, uint32_t baudrate)
{
	uint32_t temp, freq;
	uint8_t scr;
	uint8_t baud1, baud0, pres;
	//	freq=11059200;/*芯片外部时钟频率*/
	freq = 16000000; /*芯片外部时钟频率*/
	if (freq >= (baudrate * 16))
	{
		temp = (freq) / (baudrate * 16);
		temp = temp - 1;
		baud1 = (uint8_t)((temp >> 8) & 0xff);
		baud0 = (uint8_t)(temp & 0xff);

		/* 写入小数部分 不是很理解为什么这样写 直接得出小数扩大一百倍带入不就好了嘛 */
		temp = (((freq % (baudrate * 16)) * 100) / (baudrate));
		pres = (temp + 100 / 2) / 100;
		/* 写入小数部分 不是很理解为什么这样写 直接得出小数扩大一百倍带入不就好了嘛 */

		printf("Wk_SetBaud---freq:%d,baudrate:%d\n", freq, baudrate);
		printf("Wk_SetBaud---baud1:%x,baud0:%x,pres:%x\n", baud1, baud0, pres);
		// 关掉子串口收发使能
		scr = WkReadSlaveRegister(port, WK2XXX_SCR_REG);
		WkWriteSlaveRegister(port, WK2XXX_SCR_REG, 0);
		// 设置波特率相关寄存器
		WkWriteSlaveRegister(port, WK2XXX_SPAGE_REG, 1); // 切换到page1
		WkWriteSlaveRegister(port, WK2XXX_BAUD1_REG, baud1);
		WkWriteSlaveRegister(port, WK2XXX_BAUD0_REG, baud0);
		WkWriteSlaveRegister(port, WK2XXX_PRES_REG, pres);
		WkWriteSlaveRegister(port, WK2XXX_SPAGE_REG, 0); // 切换到page0
		// 使能子串口收发使能
		WkWriteSlaveRegister(port, WK2XXX_SCR_REG, scr);
		return 0;
	}
	else
	{
		printf("Wk_SetBaud error!!!\n");
		return 1;
	}
}

/**************************设置数据格式*******************************************************/
// 函数功能：设置子串口通信数据格式。设置停止位和校验方式
// 参数：
/**********************************************************************************************/
// stopformat:支持一位停止位和两位停止位，参数如下：
// WK2XXX_UART_STOPBITS_1 ,WK2XXX_UART_STOPBITS_2
/**********************************************************************************************/
// paritytype:校验方式：支持多种校验方式：NONE(无校验),ODD,EVEN,MARK,SPACE，具体函数参数如下：
// WK2XXX_UART_PARITY_NONE,  无校验
// WK2XXX_UART_PARITY_ODD,   奇校验
// WK2XXX_UART_PARITY_EVEN,  偶校验
// WK2XXX_UART_PARITY_MARK ，MARK校验,1校验
// WK2XXX_UART_PARITY_SPACE，SPACE校验，0校验
/***********************************************************************************************/
unsigned char WkUartFormatSet(uint8_t port, uint8_t stopformat, uint8_t paritytype)
{
	unsigned char scr = 0, sier = 0, lcr = 0;

	/* 在设置前失能子串口收发 */
	scr = WkReadSlaveRegister(port, WK2XXX_SCR_REG);
	WkWriteSlaveRegister(port, WK2XXX_SCR_REG, 0);
	/* 在设置前失能子串口中断收发 */
	sier = WkReadSlaveRegister(port, WK2XXX_SIER_REG);
	WkWriteSlaveRegister(port, WK2XXX_SIER_REG, 0);

	if (stopformat == WK2XXX_UART_STOPBITS_1)
	{
		lcr = lcr | 0;
	}
	else if (stopformat == WK2XXX_UART_STOPBITS_2)
	{
		lcr = lcr | LCR_STPL_BIT;
	}
	else
	{
		return 1;
	}
	if (paritytype == WK2XXX_UART_PARITY_NONE)
	{
		lcr = lcr | 0;
	}
	else if (paritytype == WK2XXX_UART_PARITY_ODD)
	{
		lcr = lcr | (LCR_PAM0_BIT | LCR_PAEN_BIT);
	}
	else if (paritytype == WK2XXX_UART_PARITY_EVEN)
	{
		lcr = lcr | (LCR_PAM1_BIT | LCR_PAEN_BIT);
	}
	else if (paritytype == WK2XXX_UART_PARITY_MARK)
	{
		lcr = lcr | (LCR_PAM0_BIT | LCR_PAM1_BIT | LCR_PAEN_BIT);
	}
	else if (paritytype == WK2XXX_UART_PARITY_SPACE)
	{
		lcr = lcr | LCR_PAEN_BIT;
	}
	else
	{
		return 1;
	}
	WkWriteSlaveRegister(port, WK2XXX_LCR_REG, lcr);
	/* 使能子串口收发和中断 */
	WkWriteSlaveRegister(port, WK2XXX_SCR_REG, scr);
	WkWriteSlaveRegister(port, WK2XXX_SIER_REG, sier);

	printf("Wk_FormatSet--LCR:%x,SCR:%x,SIER:%x\n", lcr, scr, sier);
	return 0;
}

/* @Description:获取子串口发送FIFO剩余空间长度
 * @param port:串口号
 * @retval:发送FIFO剩余空间长度
 */
int WkUartTxLen(uint8_t port)
{
	uint8_t fsr, tfcnt;
	int len = 0;
	/* 得到子串口FIFO状态寄存器的值 */
	fsr = WkReadSlaveRegister(port, WK2XXX_FSR_REG);
	/* 得到子串口发送 FIFO 中的数据个数 */
	tfcnt = WkReadSlaveRegister(port, WK2XXX_TFCNT_REG);
	if (fsr & FSR_TFULL_BIT) /* 子串口发送 FIFO 满标志如果置一了说明满了 */
	{
		len = 0;
	}
	else
	{
		len = 256 - tfcnt;
	}
	return len;
}

/* @Description:通过子串口发送固定长度数据
 * @param port:串口号
 * @param len:单次发送长度不超过256
 */
int WkUartTxChars(uint8_t port, int len, uint8_t *sendbuf)
{
#if 1
	WkWriteSlaveFifo(port, sendbuf, len); // 通过fifo方式发送数据
	return 0;
#else
	int num = len;
	for (num = 0; num < len; num++)
	{
		/* 向数据寄存器写数据 */
		WkWriteSlaveRegister(port, WK2XXX_FDAT_REG, *(sendbuf + num));
	}
	return 0;
#endif
}

/* @Description:读取子串口fifo中的数据
 * @param port:端口号
 * @param recbuf:接收到的数据
 * @retval 接收数据的长度
 */
int WkUartRxChars(uint8_t port, uint8_t *recbuf)
{
	uint8_t fsr = 0, rfcnt = 0, rfcnt2 = 0, sifr = 0;
	int len = 0;
	sifr = WkReadSlaveRegister(port, WK2XXX_SIFR_REG);
	/*  接收中断                         接收超时中断    */
	if ((sifr & SIFR_RFTRIG_INT_BIT) || (sifr & SIFR_RXOVT_INT_BIT)) // 有接收中断和接收超时中断
	{
		fsr = WkReadSlaveRegister(port, WK2XXX_FSR_REG);
		if (fsr & FSR_RDAT_BIT) /* 如果接收非空 */
		{
			rfcnt = WkReadSlaveRegister(port, WK2XXX_RFCNT_REG);
			if (rfcnt == 0)
			{
				rfcnt = WkReadSlaveRegister(port, WK2XXX_RFCNT_REG);
			}
			rfcnt2 = WkReadSlaveRegister(port, WK2XXX_RFCNT_REG);
			if (rfcnt2 == 0)
			{
				rfcnt2 = WkReadSlaveRegister(port, WK2XXX_RFCNT_REG);
			}
			/*判断fifo中数据个数*/
			rfcnt = (rfcnt2 >= rfcnt) ? rfcnt : rfcnt2;
			len = (rfcnt == 0) ? 256 : rfcnt;
#if 1
			WkReadSlaveFifo(port, recbuf, len);
#else
			for (n = 0; n < len; n++)
				*(recbuf + n) = WkReadSlaveRegister(port, WK2XXX_FDAT_REG);
#endif
			return len;
		}
		else
		{
			return len;
		}
	}
	return len;
}




/************************************************************************************************/
// 函数功能:数据处理。本函数主要实现把接收的数据，然后再发送出来。
/**************************uint8_t WK2xxxPortProcess(void)********************************************/
uint8_t WkUartProcess(void)
{
	int rxlen = 0;
	uint8_t gifr, rxbuf[260];
	gifr = WkReadGlobalRegister(WK2XXX_GIFR_REG);
	do
	{
		// 判断子串口1是否有中断
		if (gifr & GIFR_UT1INT_BIT)
		{									 /*数据处理*/
											 /*数据接收*/
			rxlen = WkUartRxChars(1, rxbuf); // 一次接收的数据不会超过256Byte
			/*数据发送*/
			// 把接收的数据发送出去
			if (rxlen > 0)
			{
				WkUartTxChars(1, rxlen, rxbuf);
			}
		}

		// 判断子串口2是否有中断
		if (gifr & GIFR_UT2INT_BIT)
		{
			/*数据接收*/
			rxlen = WkUartRxChars(2, rxbuf); // 一次接收的数据不会超过256Byte
			/*数据发送*/
			// 把接收的数据发送出去
			if (rxlen > 0)
			{
				WkUartTxChars(2, rxlen, rxbuf);
			}
		}
		// 判断子串口3是否有中断
		if (gifr & GIFR_UT3INT_BIT)
		{
			/*数据接收*/
			rxlen = WkUartRxChars(3, rxbuf); // 一次接收的数据不会超过256Byte
			/*数据发送*/
			// 把接收的数据发送出去
			if (rxlen > 0)
			{
				WkUartTxChars(3, rxlen, rxbuf);
			}
		}
		// 判断子串口4是否有中断
		if (gifr & GIFR_UT4INT_BIT)
		{
			/*数据接收*/
			rxlen = WkUartRxChars(4, rxbuf); // 一次接收的数据不会超过256Byte
			/*数据发送*/
			// 把接收的数据发送出去
			if (rxlen > 0)
			{
				WkUartTxChars(4, rxlen, rxbuf);
			}
		}

		gifr = WkReadGlobalRegister(WK2XXX_GIFR_REG);
	} while (gifr & 0x0f);

	return gifr;
}



