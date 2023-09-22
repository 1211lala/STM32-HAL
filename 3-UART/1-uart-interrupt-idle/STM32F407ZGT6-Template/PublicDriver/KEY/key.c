#include "key.h"


uint8_t ScanKey(void)
{
    static uint8_t flag=0;
    uint8_t waittime=30;
    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_SET && flag==0)
    {
        HAL_Delay(waittime);
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_SET && flag==0)
        {
            flag=1;
            return key_up;
        }
    }
    else if(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_SET && flag==0)
    {
        HAL_Delay(waittime);
        if(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_SET && flag==0)
        {
            flag=1;
            return key0;
        }
    }

    if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)==GPIO_PIN_RESET && HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_RESET)
    {
        flag=0;
    }
    return key_none;
}
