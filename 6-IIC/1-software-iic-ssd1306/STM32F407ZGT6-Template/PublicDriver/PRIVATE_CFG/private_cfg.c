#include  ".\PRIVATE_CFG\private_cfg.h"



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



/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用前勾选MDK的 use Mrico lib
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}





/**
  * 函数功能: 按下获取键值
  * 输入参数: 消抖时间
  * 返 回 值: 按下的键值
  * 说    明: 无死等
  */
Key_Status Get_key_with_down(uint8_t waittime)
{
	/* 保存uwtick的值 */
	static uint32_t tick;
	/* 按键标志位 */
	static uint8_t flag = 0;
	
	uint8_t key_0_status,key_up_status;
	
	key_0_status = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
	key_up_status = HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
	
	/* 如果有案件按下 进入 if 保持当前 uwtick的值 */
	if(flag == 0 && (key_0_status != 0 || key_up_status != 0 ))
	{
		flag = 1;
		tick = uwTick;
	}
	/* 上一步有按键按下后 等待 100ms 如果还是满足if里面的案件检测, 说明确实有按键按下返回标号 */
	if(flag == 1 &&  (uwTick - tick ) > waittime)
	{
		if(flag == 1 && key_0_status == 0 && key_up_status == 1 )
		{
			flag = 2;
			return key_up_down;
		}
		else if(flag == 1 && key_0_status == 1 && key_up_status == 0 )
		{
			flag = 2;
			return key0_down;
		}
	}
	/* 在没有按键按下时令flag =0 等待按键按下 */
	if(key_0_status == 0 && key_up_status == 0)
	{
		flag = 0;
	}
	return key_null;
}





/**
  * 函数功能: 松开获取键值
  * 输入参数: 消抖时间
  * 返 回 值: 按下的键值
  * 说    明: 无死等
  */
Key_Status Get_key_with_undo(uint8_t waittime)
{
	/* 保存uwtick的值 */
	static uint32_t tick;
	/* 按键标志位 */
	static uint8_t flag = 0;
	/* 保存那个按键按下 */
	static uint8_t down = 0;
	
	uint8_t key_0_status,key_up_status;
	
	key_0_status = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
	key_up_status = HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
	
	/* 如果有案件按下 进入 if 保持当前 uwtick的值 */
	if(flag == 0 && (key_0_status != 0 || key_up_status != 0 ))
	{
		flag = 1;
		tick = uwTick;
	}
	/* 上一步有按键按下后 等待 100ms 如果还是满足if里面的案件检测, 说明确实有按键按下返回标号 */
	if(flag == 1 &&  (uwTick - tick ) > waittime)
	{
		if(flag == 1 && key_0_status == 0 && key_up_status == 1 )
		{
			flag = 2;
			down = key_up_down;
		}
		else if(flag == 1 && key_0_status == 1 && key_up_status == 0 )
		{
			flag = 2;
			down =  key0_down;
		}
	}
	
	if(flag == 2 && key_0_status == 0 && key_up_status == 0)
	{
		flag = 0;
		return down;
		
	}
	/* 在没有按键按下时令flag =0 等待按键按下 */
	if(key_0_status == 0 && key_up_status == 0)
	{
		flag = 0;
	}
	return key_null;
}




/**
	* 函数功能: 获得一个随机数
  * 输入参数: min 获得值的最大值 max 获得值的最小值
  * 返 回 值: 返回得到的数值
  * 说    明: 无
  */
uint32_t RNG_GetRandomNumber(uint32_t min,uint32_t max)
{
	return HAL_RNG_GetRandomNumber(&hrng)%(max - min + 1) + min;;        
}


/**
	* 函数功能: 最高延时10000us 因为周期就给了10000个
  * 输入参数: nus  延时的us数 
  * 返 回 值: 无
	* 说    明: 比实际大概多了1.2倍
  */
void my_delay_us(uint16_t nus)
{
	__HAL_TIM_SetCounter(&htim6,0);
	
	__HAL_TIM_ENABLE(&htim6);

	while(__HAL_TIM_GetCounter(&htim6)<nus);
	
	__HAL_TIM_DISABLE(&htim6);
}







