#ifndef __MPU6050_IIC_H_
#define __MPU6050_IIC_H_


#include "main.h"
#include "gpio.h"
/******************************************************************************************/
/* CT_IIC 引脚 定义 */

#define MPU_IIC_SCL_GPIO_PORT            GPIOA
#define MPU_IIC_SCL_GPIO_PIN             GPIO_PIN_6
#define MPU_IIC_SCL_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define MPU_IIC_SDA_GPIO_PORT            GPIOA
#define MPU_IIC_SDA_GPIO_PIN             GPIO_PIN_7
#define MPU_IIC_SDA_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/******************************************************************************************/

/* IO操作 */
#define MPU_IIC_SCL(x)     do{ x ? \
                              HAL_GPIO_WritePin(MPU_IIC_SCL_GPIO_PORT, MPU_IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(MPU_IIC_SCL_GPIO_PORT, MPU_IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */

#define MPU_IIC_SDA(x)     do{ x ? \
                              HAL_GPIO_WritePin(MPU_IIC_SDA_GPIO_PORT, MPU_IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(MPU_IIC_SDA_GPIO_PORT, MPU_IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */

#define MPU_READ_SDA       HAL_GPIO_ReadPin(MPU_IIC_SDA_GPIO_PORT, MPU_IIC_SDA_GPIO_PIN) /* 读取SDA */


/* IIC所有操作函数 */
void mpu_iic_init(void);             /* 初始化IIC的IO口 */
void mpu_iic_stop(void);             /* 发送IIC停止信号 */
void mpu_iic_start(void);            /* 发送IIC开始信号 */

void mpu_iic_ack(void);              /* IIC发送ACK信号 */
void mpu_iic_nack(void);             /* IIC不发送ACK信号 */
uint8_t mpu_iic_wait_ack(void);      /* IIC等待ACK信号 */

void mpu_iic_send_byte(uint8_t txd);         /* IIC发送一个字节 */
uint8_t mpu_iic_read_byte(unsigned char ack);/* IIC读取一个字节 */

#endif
