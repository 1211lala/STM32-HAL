/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "./OTHER/other.h"
#include "./UART1_232/uart1_232.h"
#include "./UART3/uart3.h"
#include "./WK2124/wk2124.h"
#include "./IIC1/iic1.h"
#include "./AHT20/aht20.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

extern uint8_t uart1_rec_buf[];			/* 串口一的接收缓存 */
extern uint8_t uart3_rec_buf[];			/* 串口三的接收缓存 */

extern uint8_t hw_port1_get_id;			/* 保存接口一得到的ID值 */
extern uint16_t hw_port1_get_data;	/* 保存接口一得到的数据值 */

extern uint8_t hw_port2_get_id;
extern uint16_t hw_port2_get_data;

extern uint8_t hw_port3_get_id;
extern uint16_t hw_port3_get_data;

extern uint8_t hw_port4_get_id;
extern uint16_t hw_port4_get_data;

extern uint8_t hw_port5_get_id;
extern uint16_t hw_port5_get_data;


extern uint16_t adc_dma_buf[];		/* 保存ADC的值 */


//extern uint32_t hum_temp   
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define LIT_Pin GPIO_PIN_4
#define LIT_GPIO_Port GPIOA
#define FLM_Pin GPIO_PIN_5
#define FLM_GPIO_Port GPIOA
#define WK_RST_Pin GPIO_PIN_0
#define WK_RST_GPIO_Port GPIOB
#define WK_CS_Pin GPIO_PIN_12
#define WK_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
