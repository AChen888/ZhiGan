/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define MODBUS_DIR_TX()   HAL_GPIO_WritePin(CTRL485_GPIO_Port,CTRL485_Pin, GPIO_PIN_SET)
#define MODBUS_DIR_RX()   HAL_GPIO_WritePin(CTRL485_GPIO_Port,CTRL485_Pin, GPIO_PIN_RESET)

#define RX_BUF_SIZE		256
#define TX_BUF_SIZE		256

typedef struct
{
	u8 RxBuf[RX_BUF_SIZE];
	u16 RxCount;
	u8 RxStatus;
	u8 RxNewFlag;

	u8 TxBuf[TX_BUF_SIZE];
	u16 TxCount;
	u16 TxTotal;
	u8 RspCode;

	u8 Addr;
	u8 Func;
	u32 Baudrate;
	u16 TimeOut35;
	u16 TimeCount;

	u32 RxTimeStamp;
	u32 ReInitTimeStamp;
	u32 TxTimeStamp;
}T_MODBUS_ENTRY;


extern T_MODBUS_ENTRY g_ModbusEntry;
extern u8 Usart_Chage_Flag;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void hal_uart1_deinit(void);
void Send_Modbus(uint8_t *pData, uint16_t Size);
void Rev_Modbus(void);
void UART1_Interrupt_Check(void);
void UART1_Init_IT(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

