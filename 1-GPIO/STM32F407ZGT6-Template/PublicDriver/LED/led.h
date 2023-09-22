#ifndef _led_h_
#define _led_h_


#include "main.h"

#ifdef LED_R_Pin
		#define LED_R_ON()   HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET)
		#define LED_R_OFF()  HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET)
#endif

#ifdef LED_R_Pin
		#define LED_R_Toggle()   HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin)
#endif

#ifdef LED_G_Pin
		#define LED_G_ON()   HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_RESET)
		#define LED_G_OFF()  HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin,GPIO_PIN_SET)
#endif

#ifdef LED_G_Pin
		#define LED_G_Toggle()   HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin)
#endif


#endif
