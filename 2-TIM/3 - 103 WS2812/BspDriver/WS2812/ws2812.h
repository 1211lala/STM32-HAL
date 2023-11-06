#ifndef __WS2812_H__
#define __WS2812_H__

#include "main.h"



#define 		WS_OFF					0x000000
#define     WS_WHITE        0xFFFFFF
#define     WS_RED          0XFF0000
#define     WS_RGREEN       0X00FF00
#define     WS_BLUE         0X0000FF

#define     WS_ORANGE       0XF28500     /* ³ÈÉ« */

#define     WS_IvyGreen     0X36BF36    /* ³£´ºÌÙÂÌ */
#define     ForestGreen     0X228B22    /* É­ÁÖÂÌ */
#define     LimeGreen       0X32CD32    /* ÄûÃÊÂÌ */
#define     DarkGreen       0X006400    /* °µÂÌ */
#define     Lime            0X00FF00    /* ÏÊÂÌÉ« */
#define     Malachite       0X22C32E    /* ¿×È¸Ê¯ÂÌ */


#define     SalviaBlue      0X4D80E6    /* Î²²ÝÀ¶ */
#define     DarkPowder      0X003399    /* °µÓ¤¶ù·ÛÀ¶ */
#define     Sapphire        0X082567    /* À¶±¦Ê¯É« */
#define     International   0X002FA7    /* ¹ú¼ÊÆæÁ¬À¶ */
#define     Ceruleanblue    0X2A52BE    /* ÎµÀ¶ */
#define     RoyalBlue       0X4169E1    /* Æ·À¶ */
#define     DarkMineralBlue 0X24367D    /* °µ¿óÀ¶ */
#define     Ultramarine     0X0033FF    /* ¼«Å¨º£À¶ */
#define     LapisLazuli     0X0D33FF    /* ÌìÇàÊ¯À¶ */



#define     Purple              0X800080    /* ×ÏÉ« */
#define     DarkMagenta         0X8B008B    /* °µÑóºì */
#define     Magenta             0XFF00FF    /* Ñóºì */
#define     Fuchsia             0XF400A1    /* Æ·ºì */
#define     Orchid              0XDA70D6    /* À¼×Ï */
#define     PearlPink           0XFFB3E6    /* Ç³ÕäÖéºì */
#define     OldRose             0XB85798    /* ³ÂÃµºì */
#define     RosePink            0XFF66CC    /* Ç³Ãµ¹åºì */
#define     MediumVioletRed     0XC71585    /* ÖÐÇà×Ïºì */
#define     MagentaRose         0XFF0DA6    /* ÑóÃµ¹åºì */
#define     Rose                0XFF007F    /* Ãµ¹åºì */
#define     Ruby                0XCC0080    /* ºì±¦Ê¯É« */
#define     Camellia            0XE63995    /* É½²èºì */
#define     DeepPink            0XFF1493    /* Éî·Ûºì */
#define     Flamingo            0XE68AB8    /* »ðº×ºì */
#define     CoralPink           0XFF80BF    /* Ç³Éºº÷ºì */


#define 	WS2812_NUM           30

#define 	CODE_1               49       	       
#define 	CODE_0               23 


#define 	WS2812_TIM           &htim1            
#define   WS2812_CHANNEL     	 TIM_CHANNEL_3



void WS2812_DMA_Send_buf(void);
void WS2812_Set_RGB_Color(uint16_t ID,uint32_t Color);
void WS2812_Set_R_G_B_Color(uint16_t ID, uint8_t r, uint8_t g, uint8_t b);
void WS2812_Reset(void);
void WS2812_Refresh(void);
void WS2812_Clear(uint32_t color);
void WS2812_Random_Display(uint16_t time);
void WS2812_Random_Point_Flow(uint16_t time);
void WS2812_Random_Line_Flow(uint16_t time);


uint16_t rgb888_to_rgb565(uint32_t rgb888);
uint32_t rgb565_to_rgb888(uint16_t rgb565);

//void rgb2hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);  /*ÑÕÉ«Ëã·¨ RGB-> hsv*/
//void hsv2rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);  /*ÑÕÉ«Ëã·¨ hsv-> RGB*/
//RGB_Color_TypeDef WS2812_SetBrightness(float num, RGB_Color_TypeDef rgb);

#endif
