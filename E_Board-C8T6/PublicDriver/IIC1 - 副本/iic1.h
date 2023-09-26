#ifndef _IIC_H_
#define _IIC_H_

#include "main.h"


#define IIC1_SCL_Pin		I2C1_SCL_Pin
#define IIC1_SCL_Port 	I2C1_SCL_GPIO_Port	

#define IIC1_SDA_Pin		I2C1_SDA_Pin
#define IIC1_SDA_Port 	I2C1_SCL_GPIO_Port



#define IIC1_SCL(x)     do{ x ? \
                              HAL_GPIO_WritePin(IIC1_SCL_Port, IIC1_SCL_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC1_SCL_Port, IIC1_SCL_Pin, GPIO_PIN_RESET); \
                          }while(0)    

#define IIC1_SDA(x)     do{ x ? \
                              HAL_GPIO_WritePin(IIC1_SDA_Port, IIC1_SDA_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC1_SDA_Port, IIC1_SDA_Pin, GPIO_PIN_RESET); \
                          }while(0)      

#define IIC1_READ_SDA()     HAL_GPIO_ReadPin(I2C1_SDA_GPIO_Port, I2C1_SDA_Pin) 


/* IIC所有操作函数 */
void iic1_init(void);             /* 初始化IIC的IO口 */
void iic1_stop(void);             /* 发送IIC停止信号 */
void iic1_start(void);            /* 发送IIC开始信号 */

void iic1_ack(void);              /* IIC发送ACK信号 */
void iic1_nack(void);             /* IIC不发送ACK信号 */
uint8_t iic1_wait_ack(void);      /* IIC等待ACK信号 */

void iic1_send_byte(uint8_t txd);         /* IIC发送一个字节 */
uint8_t iic1_read_byte(unsigned char ack);/* IIC读取一个字节 */

#endif
