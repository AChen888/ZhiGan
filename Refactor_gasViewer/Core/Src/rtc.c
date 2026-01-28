/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "appConfig.h"
#include "bsp_fram_driver.h"

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */
	RTC_DateTypeDef DateToUpdate = {0};
	__HAL_RCC_PWR_CLK_ENABLE();		  //开启电源时钟
  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
	if ( HAL_RTCEx_BKUPRead ( &hrtc, RTC_BKP_DR0 ) != 0x1234 )
	{
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x01;
  sDate.Year = 0x00;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
		HAL_RTCEx_BKUPWrite ( &hrtc, RTC_BKP_DR0, 0x1234 );
	}
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*
设置系统时间：HAL_RTC_SetTime();
读取系统时间: HAL_RTC_GetTime();
设置系统日期: HAL_RTC_SetDate();
读取系统日期: HAL_RTC_GetDate();

*/

Local_BinTimeTypeDef gDateTime;

void LocalTime_Update(void)
{
	RTC_TimeTypeDef lTime;
	RTC_DateTypeDef lDate;

 	HAL_RTC_GetTime(&hrtc, &lTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &lDate, RTC_FORMAT_BIN);
	
	gDateTime.Year    = lDate.Year;
	gDateTime.Month   = lDate.Month;
	gDateTime.Day     = lDate.Date;
	gDateTime.Hours   = lTime.Hours;
	gDateTime.Minutes = lTime.Minutes;
	gDateTime.Seconds = lTime.Seconds;
}

void Main_LocalTime_Update(void)
{
	RTC_TimeTypeDef lTime;
	RTC_DateTypeDef lDate;

 	HAL_RTC_GetTime(&hrtc, &lTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &lDate, RTC_FORMAT_BIN);
	
	if(lDate.Year==0)
	{
		RestoreTimeFromFRAM();
	}
	else 
	{
		gDateTime.Year    = lDate.Year;
		gDateTime.Month   = lDate.Month;
		gDateTime.Day     = lDate.Date;
		gDateTime.Hours   = lTime.Hours;
		gDateTime.Minutes = lTime.Minutes;
		gDateTime.Seconds = lTime.Seconds;
	}
}

void LocalTime_SetRTC(Local_BinTimeTypeDef* datetime)
{
	RTC_TimeTypeDef lTime;
	RTC_DateTypeDef lDate;

 	HAL_RTC_GetTime(&hrtc, &lTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &lDate, RTC_FORMAT_BIN);
	
	lDate.Year    = datetime->Year;
	lDate.Month   = datetime->Month;
	lDate.Date    = datetime->Day;
	lDate.WeekDay = RTC_WEEKDAY_MONDAY;
	HAL_RTC_SetDate(&hrtc, &lDate, RTC_FORMAT_BIN);

	lTime.Hours   = datetime->Hours;
	lTime.Minutes = datetime->Minutes;
	lTime.Seconds = datetime->Seconds;
	lTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	lTime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &lTime, RTC_FORMAT_BIN);
}



/**
 * @brief 将当前RTC时间保存到铁电
 * @return 0:成功 其他:失败
 */
uint8_t SaveTimeToFRAM(void)
{
    uint8_t *p;
    uint16_t i;
    uint32_t sum = 0;
    
    // 获取当前RTC时间
		RTC_TimeTypeDef lTime;
		RTC_DateTypeDef lDate;
		Local_BinTimeTypeDef dt;

		HAL_RTC_GetTime(&hrtc, &lTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &lDate, RTC_FORMAT_BIN);
	
		dt.Hours = lTime.Hours;
		dt.Minutes = lTime.Minutes;
		dt.Seconds = lTime.Seconds;
		dt.Year = lDate.Year;
		dt.Month = lDate.Month;
		dt.Day = lDate.Date;
    
	
		SEGGER_RTT_printf(0,"write:nian=%d,yue=%d,ri=%d,shi=%d,fen=%d,miao=%d\r\n",dt.Year,dt.Month,dt.Day,dt.Hours,dt.Minutes,dt.Seconds);
    // 写入铁电
    return fs_fram_write_nbyte((uint16_t)FRAM_ADDR_TIME,(uint8_t *)(&dt),sizeof(dt)); 		//铁电写
		
}

/**
 * @brief 从铁电恢复时间到RTC
 * @return 0:成功 其他:失败
 */
uint8_t RestoreTimeFromFRAM(void)
{
		Local_BinTimeTypeDef dt;
    
    // 从铁电读取
    if(fs_fram_read_nbyte((uint16_t)FRAM_ADDR_TIME,(uint8_t *)(&dt),sizeof(dt)) == 0)										//铁电读
        return 0;  // 读取失败
		SEGGER_RTT_printf(0,"read:nian=%d,yue=%d,ri=%d,shi=%d,fen=%d,miao=%d\r\n",dt.Year,dt.Month,dt.Day,dt.Hours,dt.Minutes,dt.Seconds);
    // 设置RTC时间
    LocalTime_SetRTC(&dt);
    
    return 1;  // 成功
}


/* USER CODE END 1 */
