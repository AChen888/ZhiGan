/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "appconfig.h"
#include "ui_config.h"

T_MODBUS_ENTRY g_ModbusEntry;
u8 Usart_Chage_Flag;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
//	if(Usart_Chage_Flag==1)
//	{
//		switch(gCalidata[g_SelectChannel].Baud)
//		{
//			case 0:
//				huart1.Init.BaudRate = 2400;
//			break;
//			case 1:
//				huart1.Init.BaudRate = 4800;
//			break;
//			case 2:
//				huart1.Init.BaudRate = 9600;
//			break;
//			case 3:
//				huart1.Init.BaudRate = 14400;
//			break;
//			case 4:
//				huart1.Init.BaudRate = 19200;
//			break;
//		}
//		Usart_Chage_Flag=0;
//	}
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void UART1_Init_IT(void)
{
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); // 启用接收非空中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); // 启用空闲中断
    MODBUS_DIR_RX(); // 初始化为接收模式
	
		// 清空接收数组和标志
    memset(g_ModbusEntry.RxBuf, 0, RX_BUF_SIZE);
    g_ModbusEntry.RxCount = 0;
    g_ModbusEntry.RxStatus = 0;
}

void Send_Modbus(uint8_t *pData, uint16_t Size)
{
	MODBUS_DIR_TX();
	HAL_UART_Transmit(&huart1, pData, Size, 1000);
	MODBUS_DIR_RX();
}

void hal_uart1_deinit(void)
{
	HAL_UART_Abort(&huart1);
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_Delay (20);
	
	HAL_UART_DeInit(&huart1);
	HAL_Delay (20);

	MX_USART1_UART_Init();
	HAL_Delay (20);
}


void UART1_Interrupt_Check(void)
{
  SEGGER_RTT_printf(0,"===== USART1 中断状态检测 =====\r\n");
  
  // 1. 串口外设是否使能（CR1_UE位，Bit13）
  uint32_t cr1 = USART1->CR1;
  SEGGER_RTT_printf(0,"USART1_CR1 = 0x%08X\r\n", cr1);
  SEGGER_RTT_printf(0,"  - 串口外设使能(UE)：%s\r\n", (cr1 & USART_CR1_UE) ? "? 1" : "? 0");
  
  // 2. IDLE中断使能（CR1_IDLEIE位，Bit4）
  SEGGER_RTT_printf(0,"  - IDLE中断使能(IDLEIE)：%s\r\n", (cr1 & USART_CR1_IDLEIE) ? "? 1" : "? 0");
  
  // 3. RXNE中断使能（CR1_RXNEIE位，Bit5，DMA接收时可关闭，但IDLE中断需开启）
  SEGGER_RTT_printf(0,"  - RXNE中断使能(RXNEIE)：%s\r\n", (cr1 & USART_CR1_RXNEIE) ? "? 1" : "? 0");
  
  // 4. NVIC全局中断使能（USART1_IRQn=37 → NVIC_ISER1[5]）
  uint32_t iser1 = NVIC->ISER[1];
  SEGGER_RTT_printf(0,"NVIC_ISER1 = 0x%08X\r\n", iser1);
  SEGGER_RTT_printf(0,"  - USART1全局中断使能：%s\r\n", (iser1 & (1<<5)) ? "? 1" : "? 0");
  
  // 5. 中断优先级（Preemption优先级需低于FreeRTOS内核优先级）
  uint32_t ipr9 = NVIC->IP[USART1_IRQn]; // USART1_IRQn=37 → IP[37/4]=IP[9]，偏移37%4=1 → 8~15位
  uint8_t pree_prio = (ipr9 >> 4) & 0x0F;
  SEGGER_RTT_printf(0,"  - USART1抢占优先级：111%d111（建议<5）\r\n", pree_prio);
  
  // 6. DMA接收状态（IDLE中断依赖DMA）
  SEGGER_RTT_printf(0,"  - DMA接收是否启动：%s\r\n", 
         (HAL_UART_GetState(&huart1) & HAL_UART_STATE_BUSY_RX) ? "? 1" : "? 0");
				 
	uint32_t dma_clk = RCC->AHBENR & RCC_AHBENR_DMA1EN;
	SEGGER_RTT_printf(0,"DMA1时钟使能：%s\r\n", dma_clk ? "? 1" : "? 0");
}
/* USER CODE END 1 */
