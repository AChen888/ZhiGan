/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "appconfig.h"
#include "rtc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId guiTaskHandle;
uint32_t guitaskBuffer[ 512 ];
osStaticThreadDef_t guitaskControlBlock;
osThreadId keyTaskHandle;
uint32_t keyTaskBuffer[ 256 ];
osStaticThreadDef_t keyTaskControlBlock;
osThreadId collectTaskHandle;
uint32_t gasTaskBuffer[ 512 ];
osStaticThreadDef_t gasTaskControlBlock;
osThreadId controlTaskHandle;
uint32_t ControlTaskBuffer[ 256 ];
osStaticThreadDef_t ControlTaskControlBlock;
osThreadId commTaskHandle;
uint32_t commTaskBuffer[ 1024 ];
osStaticThreadDef_t commTaskControlBlock;
osThreadId resetTaskHandle;
uint32_t resetTaskBuffer[ 128 ];
osStaticThreadDef_t resetTaskControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void GUI_Task(void const * argument);
void KEY_Task(void const * argument);
void COLLECT_Task(void const * argument);
void CONTROL_StartTask(void const * argument);
void COMM_Task(void const * argument);
void Reset_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of guiTask */
  osThreadStaticDef(guiTask, GUI_Task, osPriorityHigh, 0, 512, guitaskBuffer, &guitaskControlBlock);
  guiTaskHandle = osThreadCreate(osThread(guiTask), NULL);

  /* definition and creation of keyTask */
  osThreadStaticDef(keyTask, KEY_Task, osPriorityRealtime, 0, 256, keyTaskBuffer, &keyTaskControlBlock);
  keyTaskHandle = osThreadCreate(osThread(keyTask), NULL);

  /* definition and creation of collectTask */
  osThreadStaticDef(collectTask, COLLECT_Task, osPriorityNormal, 0, 512, gasTaskBuffer, &gasTaskControlBlock);
  collectTaskHandle = osThreadCreate(osThread(collectTask), NULL);

  /* definition and creation of controlTask */
  osThreadStaticDef(controlTask, CONTROL_StartTask, osPriorityNormal, 0, 256, ControlTaskBuffer, &ControlTaskControlBlock);
  controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);

  /* definition and creation of commTask */
  osThreadStaticDef(commTask, COMM_Task, osPriorityRealtime, 0, 1024, commTaskBuffer, &commTaskControlBlock);
  commTaskHandle = osThreadCreate(osThread(commTask), NULL);

  /* definition and creation of resetTask */
  osThreadStaticDef(resetTask, Reset_Task, osPriorityLow, 0, 128, resetTaskBuffer, &resetTaskControlBlock);
  resetTaskHandle = osThreadCreate(osThread(resetTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_GUI_Task */
/**
* @brief Function implementing the guiTask thread.
* @param argument: Not used
* @retval None
*/
//界面任务
/* USER CODE END Header_GUI_Task */
__weak void GUI_Task(void const * argument)
{
  /* USER CODE BEGIN GUI_Task */
  /* Infinite loop */
  for(;;)
  {
		
    osDelay(1);
  }
  /* USER CODE END GUI_Task */
}

/* USER CODE BEGIN Header_KEY_Task */
/**
* @brief Function implementing the keyTask thread.
* @param argument: Not used
* @retval None
*/
//按键任务
/* USER CODE END Header_KEY_Task */
__weak void KEY_Task(void const * argument)
{
  /* USER CODE BEGIN KEY_Task */
  /* Infinite loop */
  for(;;)
  {
		
    osDelay(1);
  }
  /* USER CODE END KEY_Task */
}

/* USER CODE BEGIN Header_COLLECT_Task */
/**
* @brief Function implementing the collectTask thread.
* @param argument: Not used
* @retval None
*/
//采集任务（气体，温湿度，气压）
/* USER CODE END Header_COLLECT_Task */
__weak void COLLECT_Task(void const * argument)
{
  /* USER CODE BEGIN COLLECT_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END COLLECT_Task */
}

/* USER CODE BEGIN Header_CONTROL_StartTask */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
//控制任务（继电器）
/* USER CODE END Header_CONTROL_StartTask */
__weak void CONTROL_StartTask(void const * argument)
{
  /* USER CODE BEGIN CONTROL_StartTask */
  /* Infinite loop */
  for(;;)
  {
		LED_TOGGLE;
    osDelay(1000);
  }
  /* USER CODE END CONTROL_StartTask */
}

/* USER CODE BEGIN Header_COMM_Task */
/**
* @brief Function implementing the commTask thread.
* @param argument: Not used
* @retval None
*/
//通信任务
/* USER CODE END Header_COMM_Task */
__weak void COMM_Task(void const * argument)
{
  /* USER CODE BEGIN COMM_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END COMM_Task */
}

/* USER CODE BEGIN Header_Reset_Task */
/**
* @brief Function implementing the resetTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Reset_Task */
void Reset_Task(void const * argument)
{
  /* USER CODE BEGIN Reset_Task */
  /* Infinite loop */
  for(;;)
  {
		LocalTime_Update();		//读取实时钟
		if(gDateTime.Hours==1 && gDateTime.Minutes==11 && gDateTime.Seconds==11)
		{
			gSystemData.Life+=1;
			gSystemData.resetflag = 1;
			g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
			if ( g_SaveParamFlag )		//保存参数到fram
			{
				System_SaveCommonData ( g_SaveParamFlag );
				g_SaveParamFlag = 0;
			}
			HAL_NVIC_SystemReset();
		}
//		u8 i=RestoreTimeFromFRAM();
//		SEGGER_RTT_printf(0,"chenggong=%d\r\n",i);
    osDelay(1000);
  }
  /* USER CODE END Reset_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

