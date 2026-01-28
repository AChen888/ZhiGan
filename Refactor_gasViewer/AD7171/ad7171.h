#ifndef __AD7171_H_
#define __AD7171_H_

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"
#include "cmsis_os.h"
#include "ad_config.h"

#define QUEUE_DISCARD			5
#define QUEUE_TOTAL				15

// -------------------------- 全局变量声明（供中断/任务使用） --------------------------
extern	SemaphoreHandle_t 		ad7171_data_sem; 							//中断-任务同步信号量
extern	uint8_t 							ADChannel;

// -------------------------- 函数声明（分模块封装） --------------------------
void BSP_AD7171_Init(void);                  							// 初始化（GPIO+中断+信号量+复位）
void BSP_AD7171_Init2(void);
uint32_t AD7171_ReadRawData(void);       							// 子模块：读取24位原始数据
void ad7171_task(void const *argument);  							// FreeRTOS任务入口
uint32_t BSP_AD7171_GetResult(void);
uint32_t BSP_AD7171_GetResult2(void);
void AD7171_CollectHandle(void);
void ReInitAD7171Interrupt(void);

#endif
