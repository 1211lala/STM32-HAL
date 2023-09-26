/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-09-25 13:39:35
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-25 18:06:03
 * @FilePath: \MDK-ARMc:\Users\liuao\Desktop\E_Board\E_Board-C8T6\PublicDriver\OTHER\other.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "./OTHER/other.h"


/* 测试发来的数据crc */
uint8_t test_crc_sum(uint8_t *buffer,uint8_t size)
{
    uint8_t cr = 0;
    /* 这里累加的数据抛弃了前两个字节和最后一个字节 */
    for (uint8_t i = 2; i < size - 1; i++)
    {
        // cr += *(buffer++);
        cr += buffer[i];
    }
    return cr;
}


/* 计算发出的数据的crc */
uint8_t get_crc_sum(uint8_t *buffer,uint8_t size)
{
    uint8_t cr = 0;
    /* 这里累加的数据抛弃了前两个字节 */
    for (uint8_t i = 2; i < size; i++)
    {
        // cr += *(buffer++);
        cr += buffer[i];
    }
    return cr;
}



/*us延时*/
void my_delay_us(uint16_t nus)
{
	__HAL_TIM_SetCounter(&htim2, 0);
	__HAL_TIM_ENABLE(&htim2);
	while(__HAL_TIM_GetCounter(&htim2) < nus);
	__HAL_TIM_DISABLE(&htim2);
}














