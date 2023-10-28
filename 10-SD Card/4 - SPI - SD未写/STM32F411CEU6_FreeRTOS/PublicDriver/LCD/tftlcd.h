#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "spi.h"

extern uint16_t	POINT_COLOR;	//默认画笔颜色
extern uint16_t	BACK_COLOR;		//默认背景颜色
extern uint16_t color_buf[];
//LCD的宽和高定义
#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_Width 135
#define LCD_Height 240

#else
#define LCD_Width 240
#define LCD_Height 135
#endif

//画笔颜色
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         		 	0x001F  
#define BRED             	0XF81F
#define GRED 			 				0XFFE0
#define GBLUE			 				0X07FF
#define RED           	 	0xF800
#define MAGENTA       		0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 						0XBC40 //棕色
#define BRRED 			 			0XFC07 //棕红色
#define GRAY  			 			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      		0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 	0X841F //浅绿色
#define LIGHTGRAY        	0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 			0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           	0X2B12 //浅棕蓝色(选择条目的反色)



/*
    	LCD_PWR:	PA3
    	LCD_RST:	PA4
    	LCD_DC:		PA5
    	LCD_CS:		PC4
*/
//#define	LCD_PWR(X) if(X) \
//				HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET); \
//				else \
//				HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET)
//				
//#define	LCD_RST(X) if(X) \
//				HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET); \
//				else \
//				HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)		
//				
//				
//#define	LCD_DC(X) if(X) \
//				HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET); \
//				else \
//				HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET)				
//#define	LCD_CS		PCout(4)

#define	LCD_PWR(n)		(n?HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET))
#define	LCD_RST(n)		(n?HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
#define	LCD_DC(n)			(n?HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_DC_GPIO_Port,LCD_DC_Pin,GPIO_PIN_RESET))


#define LCD_CMD									0
#define LCD_DATA								1

#define SET_LINE_CMD 				0x2A
#define SET_HANG_CMD				0x2B
#define START_TRANSMIT_CMD	0x2C

void LCD_Init(void);																																											//初始化
void LCD_DisplayOn(void);																																									//开显示
void LCD_DisplayOff(void);																																								//关显示
void LCD_Write_HalfWord(const uint16_t da);																																//写半个字节数据到LCD
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);																	//设置数据显示区域
void LCD_Clear(uint16_t color);																																						//清屏
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);				//填充单色
void LCD_Draw_Point(uint16_t x, uint16_t y);																															//画点
void LCD_Draw_ColorPoint(uint16_t x, uint16_t y,uint16_t color);																					//画带颜色点
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);																		//画线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);																//画矩形
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);																								//画圆
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size);																				//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);														//显示一个数字
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);							//显示数字
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,char *p);						//显示字符串
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);						//显示图片
void Display_ALIENTEK_LOGO(uint16_t x,uint16_t y);																												//显示ALIENTEK LOGO
void LCD_Draw_Point1(uint16_t x, uint16_t y,uint8_t t);
void LCD_Fill_COLOR(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color);

#endif


