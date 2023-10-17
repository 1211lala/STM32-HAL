#include "./SDIO_CARD/sdio_card.h"


/**
 * @brief       获取卡信息函数
 * @param       cardinfo:SD卡信息句柄
 * @retval      返回值:读取卡信息状态值
 */
uint8_t get_sd_card_info(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
    
    sta = HAL_SD_GetCardInfo(&hsd, cardinfo);
    
    return sta;
}

/**
 * @brief 通过串口打印sd卡信息
 * 
 */
void show_sdcard_info(void)
{
    HAL_SD_CardCIDTypeDef   sd_card_cid;            /* CID结构体 */
    HAL_SD_CardInfoTypeDef  g_sd_card_info_handle;  /* SD卡信息结构体 */

    HAL_SD_GetCardCID(&hsd, &sd_card_cid); 
    get_sd_card_info(&g_sd_card_info_handle);           

    switch (g_sd_card_info_handle.CardType)
    {
    case CARD_SDSC:
    {
        if (g_sd_card_info_handle.CardVersion == CARD_V1_X)
        {
            printf("Card Type:SDSC V1\r\n");
        }
        else if (g_sd_card_info_handle.CardVersion == CARD_V2_X)
        {
            printf("Card Type:SDSC V2\r\n");
        }
    }
    break;

    case CARD_SDHC_SDXC:
        printf("Card Type:SDHC\r\n");
        break;
    default: break;
    }

    printf("制造商ID:%d\r\n", sd_card_cid.ManufacturerID);                         /*制造商ID */
    printf("卡相对地址:%d\r\n", g_sd_card_info_handle.RelCardAdd);                 /*卡相对地址 */
    printf("逻辑块数量:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));   /*显示逻辑块数量 */
    printf("逻辑块大小:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));  /*显示逻辑块大小 */
    printf("SD卡容量:%d MB\r\n", (uint32_t)SD_TOTAL_SIZE_MB(&hsd));                /*显示容量 */
    printf("块大小:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);                  /*显示块大小 */
}



/**
 * @brief       判断SD卡是否可以传输(读写)数据
 * @param       无
 * @retval      返回值:SD_TRANSFER_OK      传输完成，可以继续下一次传输
                       SD_TRANSFER_BUSY SD 卡正忙，不可以进行下一次传输
 */
uint8_t get_sd_card_state(void)
{
    return ((HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
 * @brief       读SD卡(fatfs/usb调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区地址
 * @param       cnt   : 扇区个数
 * @retval      0, 正常;  其他, 错误代码(详见SD_Error定义);
 */
uint8_t sd_read_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t sta = HAL_OK;
    uint32_t timeout = SD_TIMEOUT_;
    long long lsector = saddr;
    __disable_irq();                                                           /* 关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!) */
    sta = HAL_SD_ReadBlocks(&hsd, (uint8_t *)pbuf, lsector, cnt, SD_TIMEOUT_); /* 多个sector的读操作 */

    /* 等待SD卡读完 */
    while (get_sd_card_state() != SD_TRANSFER_OK)
    {
        if (timeout-- == 0)
        {
            sta = SD_TRANSFER_BUSY;
        }
    }
    __enable_irq(); /* 开启总中断 */
    
    return sta;
}

/**
 * @brief       写SD卡(fatfs/usb调用)
 * @param       pbuf  : 数据缓存区
 * @param       saddr : 扇区地址
 * @param       cnt   : 扇区个数
 * @retval      0, 正常;  其他, 错误代码(详见SD_Error定义);
 */
uint8_t sd_write_disk(uint8_t *pbuf, uint32_t saddr, uint32_t cnt)
{
    uint8_t sta = HAL_OK;
    uint32_t timeout = SD_TIMEOUT_;
    long long lsector = saddr;
    __disable_irq();                                                            /* 关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!) */
    sta = HAL_SD_WriteBlocks(&hsd, (uint8_t *)pbuf, lsector, cnt, SD_TIMEOUT_); /* 多个sector的写操作 */

    /* 等待SD卡写完 */
    while (get_sd_card_state() != SD_TRANSFER_OK)
    {
        if (timeout-- == 0)
        {
            sta = SD_TRANSFER_BUSY;
        }
    }
    __enable_irq();     /* 开启总中断 */
    
    return sta;
}
