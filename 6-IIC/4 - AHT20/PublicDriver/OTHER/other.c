/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-09-25 13:39:35
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-25 18:06:03
 * @FilePath: \MDK-ARMc:\Users\liuao\Desktop\E_Board\E_Board-C8T6\PublicDriver\OTHER\other.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "./OTHER/other.h"



#pragma import(__use_no_semihosting)
 
void _sys_exit(int x)
{
    x = x;
}

struct __FILE
{
    int handle;
};
FILE __stdout;



/*us延时*/
void my_delay_us(uint16_t nus)
{
	__HAL_TIM_SetCounter(&htim2, 0);
	__HAL_TIM_ENABLE(&htim2);
	while(__HAL_TIM_GetCounter(&htim2) < nus);
	__HAL_TIM_DISABLE(&htim2);
}


/* 测试发来的数据sum */
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


/* 计算发出的数据的sum */
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




/****************************Info**********************************************
 * Name:    InvertUint8
 * Note: 	把字节颠倒过来，如0x12变成0x48
            0x12: 0001 0010
            0x48: 0100 1000
 *****************************************************************************/
void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf)
{
    int i;
    unsigned char tmp[4] = {0};

    for (i = 0; i < 8; i++)
    {
        if (srcBuf[0] & (1 << i))
            tmp[0] |= 1 << (7 - i);
    }
    dBuf[0] = tmp[0];
}
void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf)
{
    int i;
    unsigned short tmp[4] = {0};

    for (i = 0; i < 16; i++)
    {
        if (srcBuf[0] & (1 << i))
            tmp[0] |= 1 << (15 - i);
    }
    dBuf[0] = tmp[0];
}
unsigned short CRC16_MODBUS(unsigned char *data, unsigned int datalen)
{
    unsigned short wCRCin = 0xFFFF;     /* CRC初始值 */
    unsigned short wCPoly = 0x8005;     /* 多项式值 */
    unsigned char wChar = 0;            

    while (datalen--)
    {
        wChar = *(data++);
        InvertUint8(&wChar, &wChar);    /* 字节颠倒/数据反转 */
        wCRCin ^= (wChar << 8);         /* 数据的第一个字节异或上CRC初始值的高八位*/
        for (int i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)        /* 如果最高位为1，则左移一位再异或上多项式值 */
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else                        /* 如果最高位为0，则左移一位 */
                wCRCin = wCRCin << 1;
        }
    }
    InvertUint16(&wCRCin, &wCRCin);     /* 字节颠倒/数据反转 */
    return (wCRCin);
}




void rank(uint16_t *array, uint16_t len)
{
    /* 冒泡排序 */
    for (int i = 1; i < len; i++) /* 总轮数=元素个数-1 */
    {
        for (int j = 0; j < len - i; j++) /* j代表每轮排序次数，次数=个数-轮数-1，但j初值为0 */
        {
            if (array[j] > array[j + 1]) /* 如果前一项比后一项大，则两项的值互换 */
            {
                int temp; /* temp为数值交换时使用的临时变量 */
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}





