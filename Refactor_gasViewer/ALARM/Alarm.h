/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    XXX.h
  * @brief   This file contains 
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALARM_H__
#define __ALARM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
//±¨¾¯Æ÷
#define ALARM_ON()  	HAL_GPIO_WritePin(ALARM_CTRL_GPIO_Port,ALARM_CTRL_Pin,GPIO_PIN_SET)
#define ALARM_OFF() 	HAL_GPIO_WritePin(ALARM_CTRL_GPIO_Port,ALARM_CTRL_Pin,GPIO_PIN_RESET)
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /*  */

