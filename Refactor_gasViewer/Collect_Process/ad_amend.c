#include "ad_Amend.h"
#include "ad7171.h"
#include "ad_config.h"
#include <stdlib.h>


/********************************************交叉干扰计算函数********************************************/
float CrossOver_Calculate(uint8_t channel,float value)
{
	uint8_t mode = 0;
	float   tmp_value = 0.0;
	float   return_value = 0.0;
	for (uint8_t loop_num = 0;loop_num < 5;loop_num++)
	{
			tmp_value = 0.0;
			mode = Channel_Config[channel].Channel_CrossoverMode[loop_num];
			switch (mode)
			{
				//不执行任何操作
				case 0:
					tmp_value = 0.0;
					break;
				//当前通道值加上被选通道测量值，再加上参数系数值
				case 1:
					tmp_value = gas_Channel[Channel_Config[channel].Channel_Crossover[loop_num]].gas_Channel_Value_Previously \
										+ Channel_Config[channel].Channel_CrossoverParameter[loop_num];
					break;
				//当前通道值减去被选通道测量值，再加上参数系数值
				case 2:
					tmp_value = (-1) * gas_Channel[Channel_Config[channel].Channel_Crossover[loop_num]].gas_Channel_Value_Previously \
										+ Channel_Config[channel].Channel_CrossoverParameter[loop_num];
					break;
				//当前浓度值加上一定比例系数的交叉通道浓度值
				case 3:
					tmp_value =  gas_Channel[Channel_Config[channel].Channel_Crossover[loop_num]].gas_Channel_Value_Previously \
										* Channel_Config[channel].Channel_CrossoverParameter[loop_num];
					break;
				//不大于交叉运算通道测量值,判断：如果k倍交叉通道大于本通道测量值，则不进行处理，
				//否则，补偿系数等于负（本通道值减去-k倍的交叉通道测量值）
				case 4:
					if(gas_Channel[Channel_Config[channel].Channel_Crossover[loop_num]].gas_Channel_Value_Previously \
						* Channel_Config[channel].Channel_CrossoverParameter[loop_num] >= value)
					{
						tmp_value = 0.0;
					}
					else
					{
						tmp_value = (-1.0) * value + gas_Channel[Channel_Config[channel].Channel_Crossover[loop_num]].gas_Channel_Value_Previously;
					}
					break;
				//不小于交叉运算通道测量值，判断：如果k倍交叉通道小于本通道值，则不予处理
				//否则，补偿系数等于负（）
				case 5:
					break;
				default:
					break;
			}
			
			return_value = return_value + tmp_value;
	}
return value + return_value;
	
}

/********************************************交叉干扰计算函数********************************************/
float Amend_Calculate(uint8_t channel,float value)
{
	float tmp_rand = 0.0;
	float tmp_value = 0.0;
	
	//Y=AX+B
	tmp_value = value * Channel_Config[channel].Channel_AmendParameterA\
							+ Channel_Config[channel].Channel_AmendParameterB;

	//(量程上限-量程下限)*（0~x）‰，取值为0.00-0.10
	
		tmp_rand = (Channel_Config[channel].Channel_UpLim - Channel_Config[channel].Channel_LowLim) \
							*(rand()%(Channel_Config[channel].Channel_AmendParameterC * 10)) * 0.00001f;

	

	
	return tmp_value + tmp_rand;
}


/*******************************************数据修正主函数**************************************************/
void Ad_Amend(void)
{
	float tmp_value = 0.0;								//临时存放数据计算值
	
	for (uint8_t loop_channel_i = 0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)				//遍历全部通道
	{
		if(gas_Data_Flag.gas_Data_Flag_Compensate & (0x01 << loop_channel_i))
		{
			//进行交叉干扰影响计算
			tmp_value = CrossOver_Calculate(loop_channel_i,gas_Channel[loop_channel_i].gas_Channel_Value_Process_Com);
			//进行系数和随机值计算
			tmp_value = Amend_Calculate(loop_channel_i,tmp_value);
			
			gas_Channel[loop_channel_i].gas_Channel_Value_Process_Amend = tmp_value;
			//更改补偿位标志位置为一
			gas_Data_Flag.gas_Data_Flag_Amend |= (0x01<<loop_channel_i);
			//更改换算位标志位置位零
			gas_Data_Flag.gas_Data_Flag_Compensate &= ~(0x01<<loop_channel_i);
		}
	}
	
}

