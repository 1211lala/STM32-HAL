#include "./KEY/key.h"


void MY_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = key_s1_Pin|key_s2_Pin|key_s3_Pin|key_s4_Pin;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}




/**
 * 函数功能: 松开获取键值
 * 输入参数: 消抖时间
 * 返 回 值: 按下的键值
 * 说    明: 无死等
 */
uint8_t Get_key_with_undo(uint8_t waittime)
{
	/* 保存uwtick的值 */
	static uint32_t tick;
	/* 按键标志位 */
	static uint8_t flag = 0;
	/* 保存那个按键按下 */
	static uint8_t down = 0;
	uint8_t key_s1_status,key_s2_status,key_s3_status,key_s4_status;
	
	key_s1_status = HAL_GPIO_ReadPin(key_s1_GPIO_Port, key_s1_Pin);
	key_s2_status = HAL_GPIO_ReadPin(key_s2_GPIO_Port, key_s2_Pin);
	key_s3_status = HAL_GPIO_ReadPin(key_s3_GPIO_Port, key_s3_Pin);
	key_s4_status = HAL_GPIO_ReadPin(key_s4_GPIO_Port, key_s4_Pin);
	/* 如果有案件按下 进入 if 保持当前 uwtick的值 */
	if(flag == 0 && (key_s1_status == DOWN_STATUS || key_s2_status == DOWN_STATUS || key_s3_status == DOWN_STATUS || key_s4_status == DOWN_STATUS))
	{
		flag = 1;
		tick = uwTick;
	}
	/* 上一步有按键按下后 等待 waittime时间 如果还是满足if里面的案件检测, 说明确实有按键按下返回标号 */
	if(flag == 1 &&  (uwTick - tick ) > waittime)
	{
		if(flag == 1 && key_s1_status == DOWN_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s1_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == DOWN_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s2_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == DOWN_STATUS && key_s4_status == UNDO_STATUS)
		{
			flag = 2;
			down = s3_down;
		}
		else if(flag == 1 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == DOWN_STATUS)
		{
			flag = 2;
			down = s4_down;
		}
	}
	/* 在没有案件按下时令flag =0 等待按键按下 */
	if(flag == 2 && key_s1_status == UNDO_STATUS && key_s2_status == UNDO_STATUS && key_s3_status == UNDO_STATUS && key_s4_status == UNDO_STATUS)
	{
		flag = 0;
		return down;
	}
	return key_null;
}

