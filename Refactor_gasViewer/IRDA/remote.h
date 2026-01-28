#ifndef __REMOTE_H
#define __REMOTE_H 

#include "main.h"

uint8_t Remote_GetResult(void);
uint8_t Remote_TimerStart(void);
void REMOTE_TIM_PeriodElapsedCallback( TIM_HandleTypeDef *htim );

#endif















