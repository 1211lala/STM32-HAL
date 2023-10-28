#ifndef _KEY_H
#define _KEY_H

#include "main.h"



#define UNDO_STATUS 	0		/* 按下时的电平 */
#define DOWN_STATUS 1		/* 松开时的电平 */

enum key_status
{
	key_null = 0,
	s1_down,
	s2_down,
	s3_down,
	s4_down
};
uint8_t Get_key_with_down(uint8_t waittime);
uint8_t Get_key_with_undo(uint8_t waittime);
#endif

