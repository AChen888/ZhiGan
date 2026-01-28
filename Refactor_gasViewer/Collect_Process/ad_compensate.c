/**********************************************************************************************************
*	                             Embedded Framework base on STM32F40X series
*                                          (c) Copyright 2018   
*                       All rights reserved.  Protected by international copyright laws.
*
*    File    : Data_Compensate.c
*    Module  : Data
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2024-01-13     SNT           Creat
**********************************************************************************************************/

#include "ad_compensate.h"
#include "ad_config.h"


/***********************************温度补偿函数*******************************************/
float Data_Compensate_Tem(uint8_t channel,float value)
{
	float tmp_value = 0.0;
	tmp_value = value * Channel_Config[channel].Channel_TemComA + Channel_Config[channel].Channel_TemComB;
	return tmp_value;
}

/***********************************湿度补偿函数*******************************************/
float Data_Compensate_Hum(uint8_t channel,float value)
{
	float tmp_value = 0.0;
	tmp_value = value * Channel_Config[channel].Channel_HumComA + Channel_Config[channel].Channel_HumComB;
	return tmp_value;
}

/**********************************补偿函数**************************************/
void Ad_Compensate(void)
{
	float tmp_value = 0.0;								//临时存放数据计算值
	
	for (uint8_t loop_channel_i = 0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)				//遍历全部通道
	{
		tmp_value = 0.0;
		if(gas_Data_Flag.gas_Data_Flag_Conversion & (0x01 << loop_channel_i))
		{
			//进行温度补偿
			tmp_value = Data_Compensate_Tem(loop_channel_i,gas_Channel[loop_channel_i].gas_Channel_Value_Process_Con);
			//进行湿度补偿
			tmp_value = Data_Compensate_Hum(loop_channel_i,tmp_value);
			//经过补偿的计算结果赋值给对应位置
			gas_Channel[loop_channel_i].gas_Channel_Value_Process_Com = tmp_value;
			//更改补偿位标志位置为一
			gas_Data_Flag.gas_Data_Flag_Compensate |= (0x01<<loop_channel_i);
			//更改换算位标志位置位零
			gas_Data_Flag.gas_Data_Flag_Conversion &= ~(0x01<<loop_channel_i);
		}
	}
}
