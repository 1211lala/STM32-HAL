/*
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-09-26 09:51:15
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-26 11:13:22
 * @FilePath: \RGB\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */



/**
    16bit RGB656 R4 R3 R2 R1 R0          G5 G4 G3 G2 G1 G0       B4 B3 B2 B1 B0
    24ibt RGB888 R4 R3 R2 R1 R0 0  0  0  G5 G4 G3 G2 G1 G0 0  0  B4 B3 B2 B1 B0 0  0  0
    24ibt RGB888 R4 R3 R2 R1 R0 R2 R1 R0 G5 G4 G3 G2 G1 G0 G1 G0 B4 B3 B2 B1 B0 B2 B1 B0

    网站 http://tools.jb51.net/static/colorpicker/
*/


#include "stdio.h"

#define uint8_t unsigned char
#define uint16_t unsigned short int
#define uint32_t unsigned int


uint16_t rgb888_to_rgb565(uint32_t rgb888)
{
    uint16_t rgb565;
    uint8_t r = rgb888 >> 16;
    uint8_t g = rgb888 >> 8;
    uint8_t b = rgb888 >> 0;

    rgb565 = rgb565 | (r >> 3) << 11;
    rgb565 = rgb565 | ((g >> 2) << 5);
    rgb565 = rgb565 | (b >> 3);

    printf("rgb888转换前的三原色 %3d,%3d,%3d \n", r ,g , b);
    printf("rgb888转换后的三原色 %3d,%3d,%3d \n", (r>>3)<<3, (g>>2)<<2, (b>>3)<<3);
    return rgb565;
}

uint32_t rgb565_to_rgb888(uint16_t rgb565)
{
    uint32_t rgb888;
    uint8_t r = (rgb565 >> 11) << 3;
    uint8_t g = (rgb565 >> 5) << 2;
    uint8_t b = (rgb565 >> 0) << 3;

    printf("%d %d %d \n", r, g, b);

    rgb888 = rgb888 | r << 16;
    rgb888 = rgb888 | r << 8;
    rgb888 = rgb888 | r << 0;

    return rgb888;
}

int main(void)
{
    printf("0x%x", rgb888_to_rgb565(0xEE1289));
    return 0;
}