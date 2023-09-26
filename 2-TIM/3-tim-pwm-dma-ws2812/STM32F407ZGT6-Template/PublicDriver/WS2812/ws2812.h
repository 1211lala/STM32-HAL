#ifndef __WS2812_H__
#define __WS2812_H__

#include "main.h"
#include "tim.h"


#define 	WS2812_NUM           8                  
#define 	CODE_1               60       	/* 实际对应高电平时间为672ns */         
#define 	CODE_0               28         /* 实际对应高电平时间为312ns*/   
#define 	WS2812_TIM           &htim5            
#define   WS2812_CHANNEL     	 TIM_CHANNEL_2


void WS2812_DMA_Send_buf(void);
void WS2812_Set_RGB_Color(uint16_t ID,uint32_t Color);
void WS2812_Set_R_G_B_Color(uint16_t ID, uint8_t r, uint8_t g, uint8_t b);
void WS2812_Reset(void);
void WS2812_Refresh(void);


//void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);  /*颜色算法 RGB-> hsv*/
//void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);  /*颜色算法 hsv-> RGB*/
//RGB_Color_TypeDef WS2812_SetBrightness(float num, RGB_Color_TypeDef rgb);

#endif
