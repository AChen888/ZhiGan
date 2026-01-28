/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
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
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f3xx_hal.h"
#include "stdio.h"
/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
typedef struct
{
  uint8_t Month;    /*!< Specifies the RTC Day Month (in BCD format).
                         This parameter can be a value of @ref RTC_Month_Date_Definitions */

  uint8_t Day;     /*!< Specifies the RTC Day.
                         This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

  uint8_t Year;     /*!< Specifies the RTC Day Year.
                         This parameter must be a number between Min_Data = 0 and Max_Data = 99 */
						 	
  uint8_t Hours;            /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */

  uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

  uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
} Local_BinTimeTypeDef;


typedef struct {
    uint8_t Month;    				/*!< Specifies the RTC Day Month (in BCD format).
																		This parameter can be a value of @ref RTC_Month_Date_Definitions */

		uint8_t Day;     					/*!< Specifies the RTC Day.
																		This parameter must be a number between Min_Data = 1 and Max_Data = 31 */

		uint8_t Year;     				/*!< Specifies the RTC Day Year.
																		This parameter must be a number between Min_Data = 0 and Max_Data = 99 */
								
		uint8_t Hours;            /*!< Specifies the RTC Time Hour.
																	 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
																	 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected */

		uint8_t Minutes;          /*!< Specifies the RTC Time Minutes.
																	 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */

		uint8_t Seconds;          /*!< Specifies the RTC Time Seconds.
																		This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
} Time_Save_t;

extern Local_BinTimeTypeDef gDateTime;


extern void LocalTime_Update(void);
extern void LocalTime_SetRTC(Local_BinTimeTypeDef* datetime);
void Main_LocalTime_Update(void);
uint8_t SaveTimeToFRAM(void);
uint8_t RestoreTimeFromFRAM(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

