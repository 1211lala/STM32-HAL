#ifndef _WK2124_H_
#define _WK2124_H_

#include "main.h"


/*External clock=11.0592Mhz*/
#define 	WK2XXX_FREQ_CLK     (11059200)
/*TX FIFO Trigger level*/
#define 	WK2XXX_TX_FIFO_ TRI    (0X10)
/*RX FIFO Trigger level*/
#define 	WK2XXX_RX_FIFO_ TRI    (0X40)


/* WK2xxx全局六位寄存器地址定义 */
#define 	WK2XXX_GENA_REG     (0X00)		/* 全局控制寄存器 */
#define 	WK2XXX_GRST_REG     (0X01)		/* 全局子串口复位寄存器 */
#define		WK2XXX_GMUT_REG     (0X02)		/* 没找到啥定义 */
#define 	WK2XXX_GIER_REG     (0X10)		/* 全局中断寄存器 */
#define 	WK2XXX_GIFR_REG     (0X11)		/* 全局中断标志寄存器 */
	
#define 	WK2XXX_GPDIR_REG    (0X21)		/* only on wk2168/wk2212*/
#define 	WK2XXX_GPDAT_REG    (0X31)		/* only on wk2168/wk2212*/


/*WK2xxx子串口四位寄存器地址定义*/
#define 	WK2XXX_SPAGE_REG    (0X03)		/* 子串口页控制寄存器 */
/*PAGE0*/
#define 	WK2XXX_SCR_REG      (0X04)		/* 子串口控制寄存器 */
#define 	WK2XXX_LCR_REG      (0X05)  	/* 子串口配置寄存器 */
#define 	WK2XXX_FCR_REG      (0X06)  	/* 子串口 FIFO 控制寄存器 */
#define 	WK2XXX_SIER_REG     (0X07)  	/* 子串口中断使能寄存器 */
#define 	WK2XXX_SIFR_REG     (0X08)		/* 子串口中断标志寄存器 */
#define 	WK2XXX_TFCNT_REG    (0X09)  	/* 子串口发送 FIFO 计数寄存器 */
#define 	WK2XXX_RFCNT_REG    (0X0A)  	/* 子串口接收 FIFO 计数寄存器 */
#define 	WK2XXX_FSR_REG      (0X0B)		/* 子串口 FIFO 状态寄存器 */
#define 	WK2XXX_LSR_REG      (0X0C)		/* 子串口接收状态寄存器 */
#define 	WK2XXX_FDAT_REG     (0X0D)		/* 子串口 FIFO 数据寄存器 */

#define 	WK2XXX_FWCR_REG     (0X0E)		/* WK2124无定义 */
#define 	WK2XXX_RS485_REG    (0X0F)		/* WK2124无定义 */
/*PAGE1*/
#define 	WK2XXX_BAUD1_REG    (0X04)		/* 子串口波特率配置寄存器高字节 */
#define 	WK2XXX_BAUD0_REG    (0X05)		/* 子串口波特率配置寄存器低字节 */
#define 	WK2XXX_PRES_REG     (0X06)		/* 子串口波特率配置寄存器小数部分 */
#define 	WK2XXX_RFTL_REG     (0X07)  	/* 子串口接收 FIFO 中断触发点配置寄存器 */
#define 	WK2XXX_TFTL_REG     (0X08)  	/* 子串口发送 FIFO 中断触发点配置寄存器 */

#define 	WK2XXX_FWTH_REG     (0X09)		/* WK2124无定义 */
#define 	WK2XXX_FWTL_REG     (0X0A)		/* WK2124无定义 */
#define 	WK2XXX_XON1_REG     (0X0B)  	/* WK2124无定义 */
#define 	WK2XXX_XOFF1_REG    (0X0C)		/* WK2124无定义 */
#define 	WK2XXX_SADR_REG     (0X0D)		/* WK2124无定义 */
#define 	WK2XXX_SAEN_REG     (0X0E)		/* WK2124无定义 */
#define 	WK2XXX_RTSDLY_REG   (0X0F)		/* WK2124无定义 */


/* GENA 寄存器的位配置 */
#define 	GENA_UT4EN_BIT		  (1 << 3)		/* 子串口4时钟使能位 1:使能 0:未使能 */
#define 	GENA_UT3EN_BIT		  (1 << 2)		/* 子串口3时钟使能位 1:使能 0:未使能 */
#define 	GENA_UT2EN_BIT		  (1 << 1)		/* 子串口2时钟使能位 1:使能 0:未使能 */
#define 	GENA_UT1EN_BIT	      (1 << 0)		/* 子串口1时钟使能位 1:使能 0:未使能 */
/* GRST 寄存器的位配置 */
#define 	GRST_UT4SLEEP_BIT	  (1 << 7)		/* 子串口 4 休眠状态位 */
#define 	GRST_UT3SLEEP_BIT	  (1 << 6)		/* 子串口 3 休眠状态位 */
#define 	GRST_UT2SLEEP_BIT	  (1 << 5)		/* 子串口 2 休眠状态位 */
#define 	GRST_UT1SLEEP_BIT	  (1 << 4)		/* 子串口 1 休眠状态位 */
#define 	GRST_UT4RST_BIT	      (1 << 3)		/* 子串口 4 软复位控制位 */
#define 	GRST_UT3RST_BIT	      (1 << 2)		/* 子串口 3 软复位控制位 */
#define 	GRST_UT2RST_BIT	      (1 << 1)		/* 子串口 2 软复位控制位 */	
#define 	GRST_UT1RST_BIT	      (1 << 0)		/* 子串口 1 软复位控制位 */
/* GIER 寄存器的位配置 */
#define 	GIER_UT4IE_BIT	      (1 << 3)		/* 子串口 4 中断使能控制位 */
#define 	GIER_UT3IE_BIT	      (1 << 2)		/* 子串口 3 中断使能控制位 */
#define 	GIER_UT2IE_BIT	      (1 << 1)		/* 子串口 2 中断使能控制位 */	
#define 	GIER_UT1IE_BIT	      (1 << 0)		/* 子串口 1 中断使能控制位 */
/* GIFR 寄存器的位配置 */
#define 	GIFR_UT4INT_BIT	      (1 << 3)		/* 子串口 4 中断标志位 */
#define 	GIFR_UT3INT_BIT	      (1 << 2)		/* 子串口 3 中断标志位 */
#define 	GIFR_UT2INT_BIT	      (1 << 1)		/* 子串口 2 中断标志位 */
#define 	GIFR_UT1INT_BIT	      (1 << 0)		/* 子串口 1 中断标志位 */
/* GPDIR register bits */
/* GPDAT register bits */


/* SPAGE register */
#define 	WK2XXX_SPAGE0	      (0x00)		/*PAGE 子串口页控制位(子串口寄存器分布在 PAGE0 和PAGE1 上，不同页之间的切换，通过该寄存器来控制)*/
#define 	WK2XXX_SPAGE1         (0x01)
/* SCR register bits */
#define 	SCR_SLEEPEN_BIT	      (1 << 2)		/* 子串口休眠使能位 */
#define 	SCR_TXEN_BIT          (1 << 1)		/* 子串口发送使能位 */
#define 	SCR_RXEN_BIT          (1 << 0)		/* 子串口接收使能位 */
/* LCR register bits */
#define 	LCR_BREAK_BIT	      (1 << 5)		/* 子串口 Line-Break 输出控制位 */
#define 	LCR_IREN_BIT          (1 << 4)		/* 子串口红外使能位 */
#define 	LCR_PAEN_BIT          (1 << 3)		/* 子串口校验使能位  0：无校验位（8 位数据） 1：有校验位（9 位数据） */
#define 	LCR_PAM1_BIT          (1 << 2)		/* 子串口校验模式选择位  00：0 校验 ；01：奇校验 ；10：偶校验 ；11：1校验*/
#define 	LCR_PAM0_BIT          (1 << 1)		/* 子串口校验模式选择位 */
#define 	LCR_STPL_BIT          (1 << 0)		/* 子串口停止位长度控制位 0:1bit 1:2bits*/
/* FCR register bits */
#define 	FCR_TFTRIG1_BIT	      (1 << 7)		/* 子串口发送 FIFO 触点设置位*/
#define 	FCR_TFTRIG0_BIT       (1 << 6)		/* 子串口发送 FIFO 触点设置位*/
#define 	FCR_RFTRIG1_BIT	      (1 << 5)		/* 子串口接收 FIFO 触点设置位 */
#define 	FCR_RFTRIG0_BIT       (1 << 4)		/* 子串口接收 FIFO 触点设置位 */
#define 	FCR_TFEN_BIT          (1 << 3)		/* 子串口发送 FIFO 使能位 */
#define 	FCR_RFEN_BIT          (1 << 2)		/* 子串口接收 FIFO 使能位 */
#define 	FCR_TFRST_BIT         (1 << 1)		/* 子串口发送 FIFO 复位位 */
#define 	FCR_RFRST_BIT         (1 << 0)		/* 子串口接收 FIFO 复位位 */
/* SIER register bits */
#define 	SIER_FERR_IEN_BIT     (1 << 7)		/* 接收 FIFO 数据错误中断使能位 */
#define 	SIER_CTS_IEN_BIT      (1 << 6)		/* */
#define 	SIER_RTS_IEN_BIT      (1 << 5)		/* */
#define 	SIER_XOFF_IEN_BIT     (1 << 4)		/* */
#define 	SIER_TFEMPTY_IEN_BIT  (1 << 3)		/* 发送 FIFO 空中断使能位 */
#define 	SIER_TFTRIG_IEN_BIT   (1 << 2)		/* 发送 FIFO 触点中断使能位 */	
#define 	SIER_RXOUT_IEN_BIT    (1 << 1)		/* 接收 FIFO 超时中断使能位 */
#define 	SIER_RFTRIG_IEN_BIT   (1 << 0)		/* 接收 FIFO 触点中断使能位 */
/* SIFR register bits */
#define 	SIFR_FERR_INT_BIT     (1 << 7)		/* 接收 FIFO 数据错误中断标志位 */
#define 	SIFR_CTS_INT_BIT      (1 << 6)
#define 	SIFR_RTS_INT_BIT      (1 << 5)
#define 	SIFR_XOFF_INT_BIT     (1 << 4)
#define 	SIFR_TFEMPTY_INT_BIT  (1 << 3)		/* 发送 FIFO 空中断标志位 */
#define 	SIFR_TFTRIG_INT_BIT   (1 << 2)		/* 发送 FIFO 触点中断标志位 */
#define 	SIFR_RXOVT_INT_BIT    (1 << 1)		/* 接收 FIFO 超时中断标志位 */
#define 	SIFR_RFTRIG_INT_BIT   (1 << 0)		/* 接收 FIFO 触点中断标志位 */

/* FSR register bits */
#define 	FSR_RFOE_BIT          (1 << 7)		/* 子串口接收 FIFO 中数据溢出出错标志位 */
#define 	FSR_RFBI_BIT          (1 << 6)		/* 子串口接收 FIFO 中数据有 Line-Break 错误 */
#define 	FSR_RFFE_BIT          (1 << 5)		/* 子串口接收 FIFO 中数据帧错误标志位 */
#define 	FSR_RFPE_BIT          (1 << 4)		/* 子串口接收 FIFO 中数据校验错误标志位 */
#define 	FSR_RDAT_BIT          (1 << 3)		/* 子串口接收 FIFO 空标志位 */
#define 	FSR_TDAT_BIT          (1 << 2)		/* 子串口发送 FIFO 空标志位 */
#define 	FSR_TFULL_BIT         (1 << 1)		/* 子串口发送 FIFO 满标志位 */
#define 	FSR_TBUSY_BIT         (1 << 0)		/* 子串口发送 TX 忙标志位 */
/* LSR register bits */
#define 	LSR_OE_BIT            (1 << 3)		/* 子串口接收 FIFO 中当前被读取的字节溢出出错标志位 */
#define 	LSR_BI_BIT            (1 << 2)		/* 子串口接收 FIFO 中当前被读取字节 Line-Break 错误标志位 */
#define 	LSR_FE_BIT            (1 << 1)		/* 子串口接收 FIFO 中当前被读取字节帧错误标志位 */
#define 	LSR_PE_BIT            (1 << 0)		/* 子串口接收 FIFO 中当前被读取字节校验错误标志位 */
/* FWCR register bits */
#define 	FWCR_X2RF_BIT         (1 << 7)
#define 	FWCR_FWM2_BIT         (1 << 6)
#define 	FWCR_FWM1_BIT         (1 << 5)
#define 	FWCR_FWM0_BIT         (1 << 4)
#define 	FWCR_FWTRIG1_BIT      (1 << 3)
#define 	FWCR_FWTRIG0_BIT      (1 << 2)
#define 	FWCR_RTS_BIT          (1 << 1)
#define 	FWCR_CTS_BIT          (1 << 0)
/* RS485 register bits */
#define 	RS485_RS485_BIT       (1 << 6)
#define 	RS485_ATADD_BIT       (1 << 5)
#define 	RS485_DATEN_BIT       (1 << 4)
#define 	RS485_RTSEN_BIT       (1 << 1)
#define 	RS485_RTSINV_BIT      (1 << 0)



#define WK2XXX_UART_STOPBITS_1                     0x00
#define WK2XXX_UART_STOPBITS_2                     ((uint8_t)LCR_STPL_BIT)


/**UART_Parity : UART 极性选择
*/
#define WK2XXX_UART_PARITY_NONE                    0x00																										/* 无校验(NONE) */
#define WK2XXX_UART_PARITY_SPACE                   ((uint8_t)( LCR_PAEN_BIT)) 								/* 0校验(SPACE) */
#define WK2XXX_UART_PARITY_EVEN                    ((uint8_t)(LCR_PAM1_BIT | LCR_PAEN_BIT))					/* 偶校验(EVEN) */
#define WK2XXX_UART_PARITY_ODD                     ((uint8_t)(LCR_PAM0_BIT | LCR_PAEN_BIT))					/* 奇校验(ODD) */ 
#define WK2XXX_UART_PARITY_MARK                    ((uint8_t)(LCR_PAM0_BIT | LCR_PAM1_BIT|LCR_PAEN_BIT)) 	/* 1校验(MARK) */







void Wk2xxxRstInit(void);
void WkWriteGlobalRegister(uint8_t greg,uint8_t dat);
uint8_t WkReadGlobalRegister(uint8_t greg);
void WkWriteSlaveRegister(uint8_t port,uint8_t sreg,uint8_t dat);
uint8_t WkReadSlaveRegister(uint8_t port,uint8_t sreg);
void WkWriteSlaveFifo(uint8_t port,uint8_t *dat,uint16_t len);
void WkReadSlaveFifo(uint8_t port,uint8_t *rec,int len);
void WkUartInit(uint8_t port);
void WkUartDeInit(uint8_t port);
uint8_t WkUartSetBaud(uint8_t port,uint32_t baudrate);
unsigned char WkUartFormatSet(uint8_t port,uint8_t stopformat,uint8_t paritytype);


int WkUartTxChars(uint8_t port,int len,uint8_t *sendbuf);
int WkUartTxLen(uint8_t port);
int WkUartRxChars(uint8_t port,uint8_t *recbuf);
void WkUartRS485(uint8_t port);
void WkUartRTSCTS(uint8_t port);
uint8_t WkUartProcess(void);



#endif
