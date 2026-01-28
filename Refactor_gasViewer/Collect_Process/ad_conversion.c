#include "ad_conversion.h"
#include "string.h"
#include "ad_config.h"
#include <math.h>
#include "stdlib.h"


/***********************************数据单位转换*************************************/
//根据通道单位，进行数值转换
float Concentration_UnitConversion(uint8_t channel,float value)
{
	float tmp_value = 0.0;
	switch (Channel_Config[channel].Channel_Unit)
	{
		//无单位，此处不进行单位切换操作，传进来多少，传出去多少
		case 0:	
			tmp_value = value;
			return tmp_value;
		//PPM单位，此次默认采集单位为ppm，传进来多少，传出去多少
		case 1:
			tmp_value = value;
			return tmp_value;
		//PPB单位，此处默认采集单位为ppm，单位换算时在原有基础上，乘以1000
		case 2:
			tmp_value = value * 1000.0f;
			return tmp_value;
		//毫克每立，此处默认采集单位为PPM，单位换算在原有基础上，乘以分子量，除以22.4
		case 3:
			tmp_value = value * (float)Channel_Config[channel].Channel_MolecularWeight/22.4f;
			return tmp_value;			
		//毫克每立，此处默认采集单位为PPM，单位换算在原有基础上，乘以分子量，除以22.4，再乘以1000
		case 4:
			tmp_value = (value * (float)Channel_Config[channel].Channel_MolecularWeight/22.4f) * 1000.0f;
			return tmp_value;	
		//%LEL,此处默认采集单位为%LEL，不进行单位转换
		case 5:
			tmp_value = value;
			return tmp_value;
		//%,此处默认采集单位为%，不进行单位转换
		case 6:
			tmp_value = value;
			return tmp_value;
		default:
			tmp_value = value;
			return tmp_value;
	}
}

/***********************************由AD值转换成浓度值********************************/
void AD_to_Concentration(void)
{
	float tmp_value = 0.0;	//数据临时变量
	for (uint8_t loop_channel_i=0;loop_channel_i < CHANNEL_MAX;loop_channel_i++)		//循环遍历全部通道
	{
		tmp_value = 0.0;		//初始化临时变量，置零
		//判断数据是否来自于通用计算，判断AD值是否有效，是否完成采集，是否在有效区间内，不在区间内，则保持前一刻值不动
		if ((gas_Data_Flag.gas_Data_Flag_Collect & (0x01 << loop_channel_i))\
				&&(gas_Channel[loop_channel_i].gas_Channel_AD_Process > 0)\
				&&(gas_Channel[loop_channel_i].gas_Channel_AD_Process <= 32768))
		{
			for (uint8_t loop_rang_j = 0;loop_rang_j < 4;loop_rang_j++)					//循环各个量程标定段
			{
					//如果标定段有意义，即后一量程点AD值大于前一量程点，则依次选择有效校准段进行校准
				if ((gas_Channel[loop_channel_i].gas_Channel_AD_Process > Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j])\
						&&(gas_Channel[loop_channel_i].gas_Channel_AD_Process < Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j + 1])\
						&&(Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j] < Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j + 1]))
				{
					//计算通道对应的浓度值：（实AD-量0AD）/（量1AD-量0AD）*（量1浓-量0浓）
					 tmp_value= \
					(float)(gas_Channel[loop_channel_i].gas_Channel_AD_Process - Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j]) \
					/ (float)(Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j + 1] - Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j])\
					* (float)(Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j + 1] - Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j])\
					+ (float)Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j];
					//进行单位换算
					gas_Channel[loop_channel_i].gas_Channel_Value_Process_Con = Concentration_UnitConversion(loop_channel_i,tmp_value);
					gas_Data_Flag.gas_Data_Flag_Conversion |= (0x01<<loop_channel_i);//转换标志位置一，标识该通道已经完成转换
//								gas_Data_Flag.gas_Data_Flag_Smooth &= ~(0x01<<loop_channel_i);//将对应的采集通道标志位置零，避免重复读到相同采集位
					break;//当计算完成浓度值后，进入下一通道进行后续计算
					
				}
				//如果校准下一校准段无意义，则执行当前校准段数据进行越界校准,利用前一个校准区间的数据进行校准
				else if ((gas_Channel[loop_channel_i].gas_Channel_AD_Process > Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j])\
								&&(Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j] > Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j + 1]))
				{
					 tmp_value= \
					(float)(gas_Channel[loop_channel_i].gas_Channel_AD_Process - Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j]) \
					/ (float)(Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j + 1] - Channel_Config[loop_channel_i].Channel_RangeAD[loop_rang_j])\
					* (float)(Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j + 1] - Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j])\
					+ (float)Channel_Config[loop_channel_i].Channel_RangeCon[loop_rang_j];
					//进行单位换算
					gas_Channel[loop_channel_i].gas_Channel_Value_Process_Con = Concentration_UnitConversion(loop_channel_i,tmp_value);
					gas_Data_Flag.gas_Data_Flag_Conversion |= (0x01<<loop_channel_i);//转换标志位置一，标识该通道已经完成转换
//								gas_Data_Flag.gas_Data_Flag_Smooth &= ~(0x01<<loop_channel_i);//将对应的采集通道标志位置零，避免重复读到相同采集位
					break;//当计算完成浓度值后，进入下一通道进行后续计算
					
				}
				//如果AD值小于零点标定值，则浓度测量值赋值为0
				else if(gas_Channel[loop_channel_i].gas_Channel_AD_Process < Channel_Config[loop_channel_i].Channel_RangeAD[0])
				{
					gas_Channel[loop_channel_i].gas_Channel_Value_Process_Con = 0;
					gas_Data_Flag.gas_Data_Flag_Conversion |= (0x01<<loop_channel_i);//转换标志位置一，标识该通道已经完成转换
//								gas_Data_Flag.gas_Data_Flag_Smooth &= ~(0x01<<loop_channel_i);//将对应的采集通道标志位置零，避免重复读到相同采集位
					break;//当计算完成浓度值后，进入下一通道进行后续计算
				}
			}
		}
	}
}
