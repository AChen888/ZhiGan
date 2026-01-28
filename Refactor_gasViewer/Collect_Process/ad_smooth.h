#ifndef __AD_SMOOTH_H_
#define __AD_SMOOTH_H_

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"

#define 		AD_MAX  32768
//************************************滤波******************************************

#define FILTER_NULL									0							//无滤波
#define FILTER_AVERAGE							1							//均值滤波
#define FILTER_GAUSSIAN							2							//高斯滤波
#define FILTER_EXPONENTIAL					3							//指数滤波

/*------------变量声明---------*/

static uint16_t AD_Queue[CHANNEL_MAX][60] = {0};                                           	//AD 差值队列
static uint8_t	AD_Tail[CHANNEL_MAX] ={0};																									//AD
/*-----------------------------*//*-----------函数声明----------*/


void Data_Smooth(void);	
void Data_Smooth_Init(void);

#endif
