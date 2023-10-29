#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"


#define key_s1_Pin GPIO_PIN_6
#define key_s2_Pin GPIO_PIN_7
#define key_s3_Pin GPIO_PIN_8
#define key_s4_Pin GPIO_PIN_9

#define key_s1_GPIO_Port GPIOB
#define key_s2_GPIO_Port GPIOB
#define key_s3_GPIO_Port GPIOB
#define key_s4_GPIO_Port GPIOB


#define UNDO_STATUS 	0		/* 按下时的电平 */
#define DOWN_STATUS 1			/* 松开时的电平 */

enum key_status
{
	key_null = 0,
	s1_down,
	s2_down,
	s3_down,
	s4_down
};

void MY_Key_Config(void);
uint8_t Get_key_with_undo(uint8_t waittime);
#endif


