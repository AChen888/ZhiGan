#include "collecttask.h"
#include "ad_conversion.h"
#include "ad_amend.h"
#include "ad_compensate.h"
#include "ad_smooth.h"
#include "appconfig.h"
#include "adc.h"
#include "dac.h"
#include "sht31.h"
#include "i2c.h"
#include "rtc.h"
#include "win_current.h"

/********内部温度采集*********/
uint16_t adc_TempCount;
uint16_t adc_TempValue;
uint16_t gas_TempValue;

uint8_t sht31_state = 0;
uint8_t i2c_error = 0;
uint8_t step = 0;
uint8_t pre_error = 0;

uint16_t Gas_CalcTemp(uint16_t ad)
{
	int v25=143;
	int v;
	int t;

	v = ad * 33000 / 4096;		//放大了10000倍
	t = (v25*100 - v) * 100 / 43 + 2500;
	
	return t;		//放大了100倍
}

void temp_Internal(void)								//通过计数器来控制采样周期   奇数采样，偶数读取
{
	adc_TempCount++;
	if(adc_TempCount & 0x0001)
	{
		HAL_ADC_Start(&hadc1);
	}
	else
	{
		adc_TempValue = (uint16_t)HAL_ADC_GetValue (&hadc1);
		gas_TempValue = Gas_CalcTemp(adc_TempValue);
		if(Sensor_SHT31.humidity < 0)
		{
			Sensor_SHT31.temper = gas_TempValue / 100.0f;
		}
	}
}

static void SHT31 ( void )
{
	step++;
	if ( step < 4)								//采集SHT31传感器的温湿度数据
	{
		if ( sht31_state == 0 )
		{
			if ( SHT31_Init())
			{	
				sht31_state = 1;
			}
			else
				i2c_error++;
		}
		else
		{
			if ( SHT31_Measure() == 0 )
				sht31_state = 0;
			else
				step = 0;
		}
	}
	else if( step < 5)
	{
		if ( i2c_error > 0 )				//清除i2c，重新初始化i2c
		{
			SHT31_ResetBus();
			osDelay ( 10 );
			HAL_I2C_DeInit ( &hi2c1 );
			osDelay ( 10 );
			SHT31_ClearBus();
			MX_I2C1_Init();
			osDelay ( 10 );
			Sensor_SHT31.humidity = -1;
		}
	}
	else													//使用内部温度兜底，超过50次，重新使用外部SHT31
	{
		temp_Internal();
		if(step > 50)
		{
			step = 0;
			i2c_error = 0;
		}
	}
}


// -------------------------- FreeRTOS任务入口 --------------------------
void COLLECT_Task(void const *argument) 
{
	/*********sht31初始化************/
	uint32_t 	sht31_tick=0;
	uint8_t 	sht31_refresh=0;
	
	/*********ad7171初始化***********/
	uint32_t 				last_ad7171_collect_tick = 0; 											// 记录上一次采集的系统时间（FreeRTOS 滴答数）
	const uint32_t 	ad7171_collect_interval = pdMS_TO_TICKS(8); 	// 采集间隔：30ms（转成滴答数）
	uint32_t 				ad7171_timeout_count = 0;           // 超时计数器
	
	/*********dac测试****************/
	uint32_t 				last_dac_tick = 0; 											// 记录上一次dac输出系统时间（FreeRTOS 滴答数）
	const uint32_t 	dac_interval = pdMS_TO_TICKS(5000); 	// 采集间隔：5000ms（转成滴答数）
	uint32_t 				dac_count = 0;           // dac计数器
	
  BSP_AD7171_Init();																					//AD7171总初始化
  last_ad7171_collect_tick = xTaskGetTickCount();							// 初始化上一次采集时间为当前时间
  //任务主循环（阻塞等待中断）
  while (1)
	{
		/*********ad7171采集任务***********/
		
		GPIO_PinState current_dout_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
		
    if (xSemaphoreTake(ad7171_data_sem, pdMS_TO_TICKS(200)) == pdPASS)												// 等待信号量（数据就绪后中断释放信号量）
		{
			ad7171_timeout_count = 0;  // 重置超时计数
			
      if ((xTaskGetTickCount() - last_ad7171_collect_tick) >= ad7171_collect_interval)	// 关键：判断是否达到 12ms 采集间隔
			{
				AD7171_CollectHandle();

//				AD_to_Concentration();			//数据换算
//				Ad_Compensate();						//数据补偿
//				Ad_Amend();									//数据修正

				last_ad7171_collect_tick = xTaskGetTickCount();																	// 更新上一次采集时间
			}
		}
		else
		{
			// 超时处理
			ad7171_timeout_count++;
			if (ad7171_timeout_count >= 5)
			{
				ad7171_timeout_count = 0;
					
					// 重新初始化AD7171和中断
				for (int i = 0; i < 10; i++)
				{
					uint32_t value = BSP_AD7171_GetResult();
				}
			}
		}
		/*********sht31采集任务***********/
		sht31_tick = HAL_GetTick() % 1000;
		if(sht31_tick < 200)
		{
			sht31_refresh = 0;
		}
		else
		{
			if(sht31_refresh == 0)
			{
				sht31_refresh = 1;
				LocalTime_Update();		//读取实时钟
				SHT31();				//读取温湿度

				MX56XX_Read_Prom_Data();
				MS56XX_GetTemperature();
				MS56XX_GetPressure();
			}
		}
		
		if(Current_Open==0)
		{
			if ((xTaskGetTickCount() - last_dac_tick) >= dac_interval)	// 关键：判断是否达到 5000ms 采集间隔
			{
				dac_OutTest((dac_count%2));
				dac_count++;
				last_dac_tick = xTaskGetTickCount();
			}
		}
		osDelay(2);
	}
}
