#include "./IIC1/iic1.h"

/**
 * @brief       控制I2C速度的延时
 * @param       无
 * @retval      无
 */
static void iic1_delay(void)
{
    my_delay_us(2);
}

/**
 * @brief       IIC接口初始化
 * @param       无
 * @retval      无
 */
void iic1_init(void)
{
    iic1_start();  /* 停止总线上所有设备 */
}

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void iic1_start(void)
{
    IIC1_SDA(1);
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SDA(0);      /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    iic1_delay();
    IIC1_SCL(0);      /* 钳住I2C总线，准备发送或接收数据 */
    iic1_delay();
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void iic1_stop(void)
{
    IIC1_SDA(0);      /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SDA(1);      /* 发送I2C总线结束信号 */
    iic1_delay();
}

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
uint8_t iic1_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;
    
    IIC1_SDA(1);      /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    iic1_delay();
    IIC1_SCL(1);      /* SCL=1, 此时从机可以返回ACK */
    iic1_delay();

    while (IIC1_READ_SDA()) /* 等待应答 */
    {
        waittime++;

        if (waittime > 250)
        {
            iic1_start();
            rack = 1;
            break;
        }

        iic1_delay();
    }

    IIC1_SCL(0);      /* SCL=0, 结束ACK检查 */
    iic1_delay();
    return rack;
}


/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void iic1_ack(void)
{
    IIC1_SDA(0);  /* SCL 0 -> 1  时SDA = 0,表示应答 */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SCL(0);
    iic1_delay(); 
    IIC1_SDA(1);  /* 主机释放SDA线 */
    iic1_delay(); 
}

/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void iic1_nack(void)
{
    IIC1_SDA(1);  /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    iic1_delay();
    IIC1_SCL(1);
    iic1_delay();
    IIC1_SCL(0);
    iic1_delay();
}

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void iic1_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC1_SDA((data & 0x80) >> 7); /* 高位先发送 */
        iic1_delay();
        IIC1_SCL(1);
        iic1_delay();
        IIC1_SCL(0);
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }

    IIC1_SDA(1);      /* 发送完成, 主机释放SDA线 */
}

/**
 * @brief       IIC发送一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t iic1_read_byte(unsigned char ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {
        receive <<= 1;          /* 高位先输出,所以先收到的数据位要左移 */
        IIC1_SCL(1);
        iic1_delay();

        if (IIC1_READ_SDA())
        {
            receive++;
        }
        
        IIC1_SCL(0);
        iic1_delay();

    }

    if (!ack)
    {
        iic1_nack();  /* 发送nACK */
    }
    else
    {
        iic1_ack();   /* 发送ACK */
    }

    return receive;
}







