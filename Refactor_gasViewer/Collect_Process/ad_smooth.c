#include "ad_smooth.h"
#include "string.h"
#include "ad_conversion.h"
#include "ad7171.h"
#include "stdio.h"
#include <math.h>
#include "ad_config.h"

/**********************************************************************************************************
*                                          AD_Calculate_Queue
*
* @Description : 均值计算函数
* @Arguments   :
* @Returns     :
**********************************************************************************************************/
uint16_t AD_Calculate_Queue(uint8_t channel_number)
{
	float tmp_coef_queue = 0.0;									//临时存放队列中元素的系数
	uint8_t	loop_queue_i = 0;
	float sum_queue = 0.0;											//队列之和
	
	for (loop_queue_i = 0 ; loop_queue_i < AD_Tail[channel_number] ; loop_queue_i++)
	{
		switch (Channel_Config[channel_number].Channel_FilterMode)			//判断滤波方式，计算滤波元素系数
		{
			//均值滤波
			case FILTER_AVERAGE:
				tmp_coef_queue = (float)(1.0/AD_Tail[channel_number]);
				break;
			//高斯滤波
			case FILTER_GAUSSIAN:
				break;
			//指数滤波
			case FILTER_EXPONENTIAL:
				break;
			//其他默认使用均值滤波
			default:
				tmp_coef_queue = (float)(1.0/AD_Tail[channel_number]);
		}
		sum_queue = sum_queue + AD_Queue[channel_number][loop_queue_i] * tmp_coef_queue;
	}
	//当计算出现异常错误值时，使用前一次正确的AD值代替计算结果
	if ((sum_queue < 0 )||(sum_queue) > 32768)
	{
		sum_queue = gas_Channel[channel_number].gas_Channel_AD_Previously;
		return gas_Channel[channel_number].gas_Channel_AD_Previously;
	}
	//若无异常，则返回队列乘以系数后的和
	else
	{
		gas_Channel[channel_number].gas_Channel_AD_Previously  = (uint16_t)sum_queue;
		return (uint16_t)sum_queue;
	}
}


/**********************************************************************************************************
*                                          AD_Judge_Queue
*
* @Description : 数据跳出判断函数
* @Arguments   :AD_Queue[CHANNEL_MAX][60]  数据队列
								AD_Tail[CHANNEL_MAX]  队尾标志，同样标志队列中保存的数据个数
								g_Channel[channel_number].g_Channel_AD_Real	实时AD值
* @Returns     :	   
**********************************************************************************************************/
uint8_t AD_Judge_Queue(uint8_t channel_number)
{
	uint8_t loop_queue_i = 0;
	float 	sum_queue = 0.0;
	uint8_t	tmp_taillength = 0;
	//如果AD_Queue队列中的数据为1个，则返回0，即显示实时值
	if (AD_Tail[channel_number] == 1)
	{
		return 0;
	}
	//如果AD_Queue队列中的数据大于1，则对数据中队尾的数据进行比较，比较数量为队列长度与跳出长度少者
	else
	{
		if (AD_Tail[channel_number] < Channel_Config[channel_number].Channel_JumpLength)
		{
			tmp_taillength = AD_Tail[channel_number];
		}
		else
		{
			tmp_taillength = Channel_Config[channel_number].Channel_JumpLength;
		}
		//评价标准为AD值变化差值之和占满量程百分比，从AD_Queue队尾开始向前循环，依次相减，并计算相减只差的绝对值之和的均值
		for (loop_queue_i = AD_Tail[channel_number];loop_queue_i > (AD_Tail[channel_number] - tmp_taillength + 1);loop_queue_i--)
		{
			sum_queue = sum_queue + fabs(((float)AD_Queue[channel_number][loop_queue_i-1] \
														- (float)AD_Queue[channel_number][loop_queue_i - 2]));
		}
		//如果数据变化差值均值小于满AD值与跳出斜率百分比的乘积，说明数据稳定，则显示滤波队列处理后的值，返回1；
		//斜率数据范围为0.1~20.0%，记录程序的数据需要乘以0.1才表示百分比。
		if ((float)(sum_queue / (tmp_taillength - 1)) < ( (float)AD_MAX  / 1000.0f * Channel_Config[channel_number].Channel_JumpSlope))
		{
			return 1;
		}//否则显示使用实时AD进行后续计算
		else
		{
			AD_Tail[channel_number] = 0;
			return 0;
		}
	}
}

/**********************************************************************************************************
*                                          AD_Input_Queue
*
* @Description : 数据滤波入队函数
* @Arguments   :AD_Queue[CHANNEL_MAX][60]  数据队列
								AD_Tail[CHANNEL_MAX]  队尾标志，同样标志队列中保存的数据个数
								g_Channel[channel_number].g_Channel_AD_Real	实时AD值
* @Returns     :	   
**********************************************************************************************************/
void AD_Input_Queue(uint8_t channel_number)
{
	uint8_t loop_queue_i = 0;
	//如果队列长度小于通道配置滤波长度最大值，则顺序存入数据，并将队尾标志位自增
	if (AD_Tail[channel_number] < Channel_Config[channel_number].Channel_FilterLength)
	{
		AD_Queue[channel_number][AD_Tail[channel_number]] = gas_Channel[channel_number].gas_Channel_AD_Real;
		AD_Tail[channel_number] = AD_Tail[channel_number] + 1;
	}
	//如果队列已满，即队尾标志指向数据队尾，则将队列中的数据顺次前移并将实时AD值存入队尾
	else
	{

		for (loop_queue_i = 0; loop_queue_i < AD_Tail[channel_number] - 1; loop_queue_i++)
		{
			AD_Queue[channel_number][loop_queue_i] = AD_Queue[channel_number][loop_queue_i + 1];
		}
		
		AD_Queue[channel_number][AD_Tail[channel_number] - 1] = gas_Channel[channel_number].gas_Channel_AD_Real;
		
	}
	//如果队尾值大于等于队列长度，则将其指向队尾
	if (AD_Tail[channel_number] > Channel_Config[channel_number].Channel_FilterLength)
	{
		AD_Tail[channel_number] = Channel_Config[channel_number].Channel_FilterLength;
	}
}

/**********************************************************************************************************
*                                          Data_Smooth
*
* @Description : 数据滤波流程函数
* @Arguments   :
* @Returns     :
**********************************************************************************************************/
void Data_Smooth(void)
{
	uint8_t	loop_channel_i = 0;											//通道循环变量
	uint8_t	judge = 0;															//跳出判断标志
	for (loop_channel_i = 0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)					//遍历全部通道，根据标志位找到需要滤波的通道
	{
		if(Channel_Config[loop_channel_i].Channel_Flag & (0x01<<4))										//比较数据配置参数的Channel_Flag的第4位是否为1
		{
			g_System_Channel_Valid |= (0x01<<loop_channel_i);														//将通道可用标志对应位置1
		}
	}
	//遍历所有通道，判断当前通道是否需要进行滤波
	for (loop_channel_i = 0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)
	{
		//判断当前通道是否有效且数据完成采集更新
		if((g_System_Channel_Valid & (0x01<<loop_channel_i))&&(gas_Data_Flag.gas_Data_Flag_Collect & (0x01<<loop_channel_i)))
		{
			AD_Input_Queue(loop_channel_i);																							//数据入队
			if (Channel_Config[loop_channel_i].Channel_JumpSlope != 0)									//如果跳出斜率位0，则不进行跳出判断
			{
				judge = AD_Judge_Queue(loop_channel_i);																		//判断是否需要进行数据跳出
			}
			if (judge == 1)																															//返回值为1，说明需要进行均值计算
			{
				gas_Channel[loop_channel_i].gas_Channel_AD_Process = AD_Calculate_Queue(loop_channel_i);	
			}
			else																																				//返回值为0或其他，说明计算用值为实时值
			{
				gas_Channel[loop_channel_i].gas_Channel_AD_Process = gas_Channel[loop_channel_i].gas_Channel_AD_Real;
			}
		}
		gas_Data_Flag.gas_Data_Flag_Collect &= ~(0x01 << loop_channel_i);									//标志位置零，标志采集元素已经被使用，避免下次使用前一次的旧值
		gas_Data_Flag.gas_Data_Flag_Smooth |= (0x01 << loop_channel_i);										//标志位置一，标志该位已经完成滤波，可以被后续操作使用
	}
}

/**********************************************************************************************************
*                                          data_smooth
*
* @Description : 滤波参数初始化
* @Arguments   :
* @Returns     :
**********************************************************************************************************/
void Data_Smooth_Init(void)
{
	uint8_t loop_channel_i = 0;
	for (loop_channel_i = 0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)					//遍历全部通道，根据标志位找到需要滤波的通道
	{
		if(Channel_Config[loop_channel_i].Channel_Flag & (0x01<<4))										//比较数据配置参数的Channel_Flag的第4位是否为1
		{
			g_System_Channel_Valid |= (0x01<<loop_channel_i);														//将通道可用标志对应位置1
		}
	}
}



