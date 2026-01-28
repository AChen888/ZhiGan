#include "ad7171.h"
#include "string.h"
#include "ad_config.h"

// -------------------------- 引脚操作宏 --------------------------
#define BSP_ADC_AD7171_SCLK_PORT GPIOC
#define BSP_ADC_AD7171_DATA_PORT GPIOC
#define BSP_ADC_AD7171_PDRST_PORT GPIOA
#define AD_SELECT_PORT GPIOA

#define BSP_ADC_AD7171_PDRST_PIN GPIO_PIN_15
#define BSP_ADC_AD7171_SCLK_PIN GPIO_PIN_10
#define BSP_ADC_AD7171_DATA_PIN GPIO_PIN_11

#define AD_SELECT_PORT_PIN GPIO_PIN_2

#define BSP_ADC_AD7171_DISABLE() HAL_GPIO_WritePin(BSP_ADC_AD7171_PDRST_PORT ,BSP_ADC_AD7171_PDRST_PIN ,GPIO_PIN_RESET)
#define BSP_ADC_AD7171_ENABLE() HAL_GPIO_WritePin(BSP_ADC_AD7171_PDRST_PORT ,BSP_ADC_AD7171_PDRST_PIN ,GPIO_PIN_SET)

#define BSP_ADC_AD7171_RDY()	(HAL_GPIO_ReadPin(BSP_ADC_AD7171_DATA_PORT, BSP_ADC_AD7171_DATA_PIN) == GPIO_PIN_RESET) 	   /*低电平表示就绪*/

#define AD7171_SCLK_L() HAL_GPIO_WritePin(BSP_ADC_AD7171_SCLK_PORT ,BSP_ADC_AD7171_SCLK_PIN ,GPIO_PIN_RESET)
#define AD7171_SCLK_H() HAL_GPIO_WritePin(BSP_ADC_AD7171_SCLK_PORT ,BSP_ADC_AD7171_SCLK_PIN ,GPIO_PIN_SET)

#define AD_SELECT_L() HAL_GPIO_WritePin(AD_SELECT_PORT ,AD_SELECT_PORT_PIN ,GPIO_PIN_RESET)
#define AD_SELECT_H() HAL_GPIO_WritePin(AD_SELECT_PORT ,AD_SELECT_PORT_PIN ,GPIO_PIN_SET)

#define AD7171_DOUT_READ() HAL_GPIO_ReadPin (BSP_ADC_AD7171_DATA_PORT ,BSP_ADC_AD7171_DATA_PIN)

// -------------------------- 全局变量 --------------------------
static	uint8_t			static_tail_poninter;
static	float   		static_sum_AD;
static  uint16_t   	static_max_AD;
static  uint16_t   	static_min_AD;

static	uint8_t			static_tail_poninter2;
static	float   		static_sum_AD2;
static  uint16_t   	static_max_AD2;
static  uint16_t   	static_min_AD2;

uint8_t ADChannel = 0;
uint8_t AD_Collect_count;								//AD 采集计数，达到100，重新初始化AD
uint8_t AD_Channel_Flag=0;


typedef enum 
{
	AD7171_OK = 0, 
	AD7171_ERROR
} AD7171_Status_Type;

typedef struct
{
	u32 tick;
	u32 sum;
	u16 result;
	u16 count;
} t_Adc_Data;


// 信号量句柄
SemaphoreHandle_t ad7171_data_sem = NULL;
AD7171_Status_Type AD7171_Check(AD7171_Status_Type Status);
static t_Adc_Data adcDataCollect;
static t_Adc_Data adcDataCollect2;

/**
 * @brief  微秒延时（适配AD7171时序）
 * @param  us：延时微秒数
 */
static void AD7171_DelayUs(uint32_t us) {
   // 空循环基准值：72MHz 主频下，1us 约需 72 个 CPU 周期
    const u32 cycle_per_us = 72;
    u32 count = us * cycle_per_us;
    
    // 空函数核心：无意义的循环消耗时间
    while(count--)
    {
        __NOP();  // 空操作指令（避免编译器优化掉空循环）
    }
}

//选择通道
void SwitchADChannel(uint8_t _ucChannel)
{
	switch(_ucChannel)
	{
		case 0:
			AD_SELECT_L();
			break;
		case 1:
			AD_SELECT_H();
			break;
	}
}

/*********************************************************************************************************
*										   AD7171_Check
*
* @Description : SPI2 init function,AD7171 init function ????
* @Arguments   : none
* @Returns	   : none
*********************************************************************************************************/
uint8_t adc_SelfCheck = 0;
static AD7171_Status_Type AD7171_Check(AD7171_Status_Type Status)
{
	AD7171_Status_Type tStatus = Status;

	if (0x20 == (tStatus & 0x20)) //ERR
	{
		tStatus 			= AD7171_ERROR;
	}

	if (0x05 != (tStatus & 0x05)) //PAT_ERR
	{
		tStatus 			= AD7171_ERROR;
	}

	if (0x08 != (tStatus & 0x08)) //ID_ERR
	{
		tStatus 			= AD7171_ERROR;
	}
	else if (0x0D == (tStatus & 0x0D))
	{
		tStatus 			= AD7171_OK;
		adc_SelfCheck = 1;
	}

	return tStatus;
}

// -------------------------- 读取24位原始数据 --------------------------
static void AD7171_ReadWord(uint32_t * rData)
{

	uint8_t 		i, j, ReceivedBit = 0x00;
	uint32_t		TempInput = 0x000000;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 8; j++)
		{
			TempInput <<= 1;
			AD7171_SCLK_L();
			AD7171_DelayUs(2);
			AD7171_SCLK_H();
			AD7171_DelayUs(2);
			ReceivedBit = AD7171_DOUT_READ();

			if (0x1 == ReceivedBit)
			{
				TempInput |= 1;
			}
		}
	}

	*rData = TempInput;
	AD7171_SCLK_H();
}

static void AD7171_ReadWord2(uint32_t * rData)
{

	uint8_t 		i, j, ReceivedBit = 0x00;
	uint32_t		TempInput = 0x000000;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 8; j++)
		{
			TempInput <<= 1;
			AD7171_SCLK_L();
			AD7171_DelayUs(2);
			AD7171_SCLK_H();
			AD7171_DelayUs(2);
			ReceivedBit 		= AD7171_DOUT_READ();

			if (0x1 == ReceivedBit)
			{
				TempInput			|= 1;
			}
		}
	}

	*rData				= TempInput;
	AD7171_SCLK_H();
}

// -------------------------- 初始化 --------------------------
void BSP_AD7171_Init(void)
{
	BSP_ADC_AD7171_DISABLE();						/*DISABLE */
	HAL_Delay(100);
	BSP_ADC_AD7171_ENABLE();						/*ENABLE  */
	HAL_Delay(100);
}

/**
 * @brief  重新初始化AD7171外部中断
 * @note   在异常重启后调用此函数恢复中断功能
 */
void ReInitAD7171Interrupt(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 1. 禁用中断
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    
    // 2. 清除可能存在的中断标志
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
    
    // 3. 反初始化GPIO
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);
    
    // 4. 重新配置GPIO为外部中断模式
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // 5. 清除中断标志（再次确保清除）
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
    
    // 6. 重新配置NVIC
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/*********************************************************************************************************
*										   获得数据
*
* @Description : SPI2 init function,AD7171 init function ????
* @Arguments   : none
* @Returns	   : none
*********************************************************************************************************/
uint32_t BSP_AD7171_GetResult(void)
{
	uint32_t		AD_Value = 0;
	uint32_t		AD_Status = 0x00;
	uint32_t		AD_ValidAD = 0;
	uint8_t			loop_num = 0;

	AD7171_SCLK_H();
	
	//判断电位是否为低，低表示准备就绪,增加循环跳出条件，防止出现死循环
	while (!BSP_ADC_AD7171_RDY())
	{
		loop_num++;
		if (loop_num > 200)
		{
			return AD_ValidAD;
		}
	}

	AD7171_ReadWord(&AD_Value);

	AD_Status			= 0x000000FF & AD_Value;		//8位状态

	AD_Value >>= 8;		//16位数据

	if (AD7171_Check((AD7171_Status_Type) AD_Status) == AD7171_OK)
	{
		AD_ValidAD = AD_Value;
	}
	return AD_ValidAD;
}

uint32_t BSP_AD7171_GetResult2(void)
{
	uint32_t		_u32reValue = 0;
	uint32_t		_u32Status = 0x00;
	uint32_t		_u32ValidAD = 0;

	AD7171_SCLK_H();

	while (!BSP_ADC_AD7171_RDY())
		;

	AD7171_ReadWord2(&_u32reValue);

	_u32Status			= 0x000000FF & _u32reValue;		//8位状态

	_u32reValue >>= 8;		//16位数据

	if (AD7171_Check((AD7171_Status_Type) _u32Status) == AD7171_OK)
	{
		_u32ValidAD 		= _u32reValue;
	}

	return _u32ValidAD;
}



// -------------------------- AD值处理 --------------------------
void AD7171_CollectHandle()
{
	uint16_t   	temp_AD_valid = 0;															//临时存放AD值变量
	uint16_t   	temp_AD_valid2 = 0;
	
	/*-------------------------------------------*/
	HAL_NVIC_DisableIRQ( EXTI15_10_IRQn );			//修改中状态，将中断EXTI3_IRQn状态更改成不可使用状态，避免通讯中触发
	
	if ( AD_Channel_Flag==0 )
	{
		temp_AD_valid = BSP_AD7171_GetResult();		
		if(temp_AD_valid > 0x8000)							//把32768~65535的数据转换成0~32767
		{
			temp_AD_valid = temp_AD_valid - 0x8000;
		} 
		else
		{
			temp_AD_valid = 0;
		}
		//-----------------------------------------------------------
		if(temp_AD_valid>0)
		{
			static_tail_poninter++;												//记录点自增
			
			if (static_tail_poninter == QUEUE_DISCARD)								//初始化AD值最大和最小值
			{
				static_max_AD = temp_AD_valid;
				static_min_AD = temp_AD_valid;
			}
			if(static_tail_poninter >= QUEUE_DISCARD)									//抛弃前几个被前一个通道影响的数据
			{
				static_sum_AD = static_sum_AD + temp_AD_valid;							//对AD值进行求和
				if (temp_AD_valid > static_max_AD)												//记录最大值
				{
					static_max_AD = temp_AD_valid;
				}
				if (temp_AD_valid < static_min_AD)												//记录最小值
				{
					static_min_AD = temp_AD_valid;
				}
			}
			if (static_tail_poninter >= QUEUE_TOTAL)									//当记录数据个数满足需求时，计算AD值均值
			{
				adcDataCollect.result = (uint16_t)((static_sum_AD - static_max_AD - static_min_AD)/(QUEUE_TOTAL - QUEUE_DISCARD - 2 + 1));
				//当前通道使用数据清零
				AD_Channel_Flag=1;
				SwitchADChannel(AD_Channel_Flag);
				static_sum_AD = 0;
				static_max_AD = 0;
				static_min_AD = 0;
				static_tail_poninter = 0;
			}
		}
	}
	else if( AD_Channel_Flag==1 )
	{
		temp_AD_valid2 = BSP_AD7171_GetResult2();
		
		if(temp_AD_valid2 > 0x8000)						//把32768~65535的数据转换成0~32767
		{
			temp_AD_valid2 = temp_AD_valid2 - 0x8000;
		} 
		else
		{
			temp_AD_valid2 = 0;
		}
		//-----------------------------------------------------------
		if(temp_AD_valid2>0)
		{
			static_tail_poninter2++;												//记录点自增
			
			if (static_tail_poninter2 == QUEUE_DISCARD)								//初始化AD值最大和最小值
			{
				static_max_AD2 = temp_AD_valid2;
				static_min_AD2 = temp_AD_valid2;
			}
			if(static_tail_poninter2 >= QUEUE_DISCARD)									//抛弃前几个被前一个通道影响的数据
			{
				static_sum_AD2 = static_sum_AD2 + temp_AD_valid2;							//对AD值进行求和
				if (temp_AD_valid2 > static_max_AD2)												//记录最大值
				{
					static_max_AD2 = temp_AD_valid2;
				}
				if (temp_AD_valid2 < static_min_AD2)												//记录最小值
				{
					static_min_AD2 = temp_AD_valid2;
				}
			}
			if (static_tail_poninter2 >= QUEUE_TOTAL)									//当记录数据个数满足需求时，计算AD值均值
			{
				adcDataCollect2.result = (uint16_t)((static_sum_AD2 - static_max_AD2 - static_min_AD2)/(QUEUE_TOTAL - QUEUE_DISCARD - 2 + 1));
				//当前通道使用数据清零
				AD_Channel_Flag = 2;
				static_sum_AD2 = 0;
				static_max_AD2 = 0;
				static_min_AD2 = 0;
				static_tail_poninter2 = 0;
			}
		}
	}
	if ( AD_Channel_Flag == 2 )
	{		
		AD_Collect_count++;
		if (AD_Collect_count >= 100)
		{
			AD_Collect_count = 0;
			BSP_ADC_AD7171_DISABLE();						/*DISABLE */
			AD7171_DelayUs(100);
			BSP_ADC_AD7171_ENABLE();						/*ENABLE  */
		}
		AD_Channel_Flag=0;
		SwitchADChannel(AD_Channel_Flag);
		gas_Channel[0].gas_Channel_AD_Real = adcDataCollect.result;
		gas_Channel[1].gas_Channel_AD_Real = adcDataCollect2.result;
		gas_Data_Flag.gas_Data_Flag_Collect |= (0x01 << ADChannel);
	}

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);															//读取数据完成后，打开中断使能
}
