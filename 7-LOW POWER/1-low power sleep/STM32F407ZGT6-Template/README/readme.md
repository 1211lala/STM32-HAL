<!--
 * @Author: liuao 2494210546@qq.com
 * @Date: 2023-09-28 09:05:26
 * @LastEditors: liuao 2494210546@qq.com
 * @LastEditTime: 2023-09-28 10:38:39
 * @FilePath: \undefinedc:\Users\liuao\Desktop\E_Board-C8T6-IIC\README\readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->

### 1.2v调压器 

1. 运行模式: 程序正常工作

2. 停止模式: 1.2v的所有时钟被关闭，保留内部的SRAM 寄存器的内容，外设停止工作

3. 待机模式：调压器掉电


### 低功耗模式

1. sleep模式: **内核时钟停止** **1.2v调压器正常工作** **外设正常运行** **引脚保持与运行时相同**CPU不在执行新的代码。可以通过 WFI或WFE 进入睡眠模式

2. stop模式: 1.2V的所有时钟全部停止，外设停止工作，内部调压器可以处于运行和低功耗模式 内部SRAM和寄存器的内容被保留。HSI 和HSE外部时钟源接口关闭。通过EXTI中断或EXTI事件唤醒，CPU从停止处执行代码

3. standby模式: 调压器停止， 1.2v阈断电。只能通过 WKUP、RTC闹钟事件、RTC唤醒事件、RTC入侵事件、NRST复位唤醒。CUP将从头执行代码相当于重启


### 进入睡眠模式

1.  void HAL_PWR_EnableSleepOnExit(void) 将  SLEEPONEXIT设为 1    
    void HAL_PWR_DisableSleepOnExit(void) 将  SLEEPONEXIT设为 0     

    在系统控制寄存器**SCR**的**SLEEPONEXIT**位为**0**时执行WFI或WFE指令时会立即进入sleep模式       
    在系统控制寄存器**SCR**的**SLEEPONEXIT**位为**1**时会退出优先级最低的那个中断再sleep模式       

2.  void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);    
    功能: 进入睡眠模式    
    参数: Regulator --> Regulator state as no effect in SLEEP mode(对于F1和F4没有使用到这个参数)     
    参数: SLEEPEntry --> 选择进入低功耗的方式(PWR_SLEEPENTRY_WFI/PWR_SLEEPENTRY_WFE)        

```C
    void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
    {
        UNUSED(Regulator);

        assert_param(IS_PWR_SLEEP_ENTRY(SLEEPEntry));

        /* SCB_SCR_SLEEPDEEP_Msk对应进入stop模式 */
        /* 清楚SCR位的SCB_SCR_SLEEPDEEP_Msk位清零,不进入深度睡眠模式即stop模式 */
        CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

        /* 选择怎么进入睡眠模式 -------------------------------------------------*/
        if(SLEEPEntry == PWR_SLEEPENTRY_WFI)
        {
            /* PWR_SLEEPENTRY_WFI方式 */
            __WFI();
        }
        else
        {
            /* PWR_SLEEPENTRY_WFE方式 */
            __SEV();
            __WFE();
            __WFE();
        }
    }
```

### 退出睡眠模式

1. **PWR_SLEEPENTRY_WFI** 方式进入睡眠模式的可以使用任何的外设中断唤醒MCU,唤醒后先执行外设中断的函数,在执行WFI指令后的内容    
    1. 因为systick定时器在初始化时设定了1ms中断一次所以会不断的唤醒睡眠模式可以使用   
        __weak void HAL_SuspendTick(void)   
        __weak void HAL_ResumeTick(void)   
    挂起和恢复systick定时器

2. **PWR_SLEEPENTRY_WFE** 方式进入睡眠模式在有事件发生时立刻退出睡眠模式,并执行WFE指令后的内容 