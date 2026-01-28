#ifndef __AD_CONVERSION_H_
#define __AD_CONVERSION_H_

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"

float Concentration_UnitConversion(uint8_t channel,float value);
void AD_to_Concentration(void);

#endif
