#include "main.h"
#include "i2c.h"
#include "SHT31.h"
#include "tim.h"
#include "i2c.h"
#include "main.h"
#include "string.h"
#include <stdio.h>
#include "alarm.h"

#define U8HU8L_U16(a, b)	(((uint16_t)(a) << 8) + (b))
#define U16H_U8(c)			((uint8_t)((c) >> 8))
#define U16L_U8(c)			((uint8_t)((c)))

#define DBG_PRINTF(...)


/*
Datasheet SHT3x-DIS
Humidity and Temperature Sensor
? Fully calibrated, linearized, and temperature compensated digital output
? Wide supply voltage range, from 2.4 V to 5.5 V
? I2C Interface with communication speeds up to 1 MHz and two user selectable addresses
? Typical accuracy of ? 1.5 %RH and ? 0.1 °C for SHT35
? Very fast start-up and measurement time
? Tiny 8-Pin DFN package

Pin Name Comments
	1 SDA Serial data; input / output
	2 ADDR Address pin; input; connect to either logic high or low, do not leave floating
	3 ALERT Indicates alarm condition; output; must be left floating if unused
	4 SCL Serial clock; input / output
	5 VDD Supply voltage; input
	6 nRESET 	Reset pin active low; input; if not used it is recommended to be left floating; can
		be connected to VDD with a series resistor of R ≥2 kΩ
	7 R No electrical function; to be connected to VSS
	8 VSS Ground

SHT3x-DIS I2C Address in Hex.representation Condition
I2C address A 0x44 (default) ADDR (pin 2) connected to logic low
I2C address B 0x45 ADDR (pin 2) connected to logic high

*/
#define I2C_SCL_Pin GPIO_PIN_8
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_9
#define I2C_SDA_GPIO_Port GPIOB

//#define SHT31_ADDR 						0x8C		// left shift 1 bit
#define SHT31_ADDR 						0x88		// left shift 1 bit

//#define MS5607_I2C_ADDR               0xEC
#define MS5607_ADDR_READ 						0xED	


#define SHT31_CMD_SOFT_RESET			0x30A2		//软件复位
#define SHT31_CMD_READ_STATUS			0xF32D		//状态查询
#define SHT31_CMD_READ_VALUE			0x2C06		//数值查询
#define SHT31_CMD_READ_SINGLE			0x2400		//单点查询

#define MS5607BA_D1_OSR_4096      0X48
#define MS5607BA_RESET            0x1E
#define MS5607BA_D2_OSR_4096      0X58
#define PROM_ADS0                  0xA0      //prom首地址
#define PROM_ADS1                  0xA2
#define PROM_ADS2                  0xA4
#define PROM_ADS3                  0xA6
#define PROM_ADS4                  0xA8
#define PROM_ADS5                  0xAA
#define PROM_ADS6                  0xAC
#define PROM_ADS7                  0xAE

#define MS5607BA_ADC               0x00      

t_SHT31_STRUCT Sensor_SHT31;
t_ms5607_STRUCT MS5607;

uint16_t		Cal_C[7]; //用于存放PROM中的6组数据
uint32_t		D1_Pres, D2_Temp; // 存放数字压力和温度
float			Press; //温度补偿大气压
float			dT, Temperature, T2; //实际和参考温度之间的差异,实际温度,中间值
 
double OFF, SENS; //实际温度抵消,实际温度灵敏度
float Aux, OFF2, SENS2; //温度校验值

uint32_t _D1;
uint32_t _D2;
uint8_t IICSHT31;
uint8_t IICMS5607;
uint8_t MS5607f;



/*
Name CRC-8
Width 8 bit
*/
uint8_t SHT31_Crc8(uint8_t *data, uint16_t count)
{
    uint8_t crc = 0xFF;						//Initialization 0xFF
    uint8_t i;
    uint8_t crc_bit;
 
    /* calculates 8-Bit checksum with given polynomial */
    for (i = 0; i < count; ++i)
    {
        crc ^= (data[i]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;	//Polynomial 0x31 (x8 + x5 + x4 + 1)
            else
                crc = (crc << 1);
        }
    }

	//Final XOR 0x00
    return crc;
}

char SHT31_Init ( void )
{
	uint16_t error = 0;
	uint8_t temp[2];
	
	(void)error;
	if ( !HAL_I2C_IsDeviceReady ( &hi2c1, SHT31_ADDR, 1, 100 ) )
	{
		DBG_PRINTF ( "SHT31 is ok.\n\r" );
	}
	else
	{
		DBG_PRINTF ( "SHT31 is error.\n\r" );
		return 0;
	}

	temp[0] = U16H_U8(SHT31_CMD_SOFT_RESET);
	temp[1] = U16L_U8(SHT31_CMD_SOFT_RESET);
	if ( HAL_I2C_Master_Transmit ( &hi2c1, SHT31_ADDR, temp, 2, 100 ))
		return 0;
	
	return 1;

}

char SHT31_Measure ( void )
{
	uint8_t temp[6];
	uint16_t result[2];
	float t;
	
	temp[0] = U16H_U8(SHT31_CMD_READ_SINGLE);
	temp[1] = U16L_U8(SHT31_CMD_READ_SINGLE);
	

	if ( HAL_I2C_Master_Transmit ( &hi2c1, SHT31_ADDR, temp, 2, 100 ))
		goto measure_error;
	HAL_Delay ( 20 );

	if ( HAL_I2C_Master_Receive ( &hi2c1, SHT31_ADDR, temp, 6, 100 ))
		goto measure_error;

	if(SHT31_Crc8(&temp[0], 2) == temp[2])
		result[0] = U8HU8L_U16(temp[0], temp[1]) ;
	else
		goto measure_error;

	if(SHT31_Crc8(&temp[3], 2) == temp[5])
		result[1] = U8HU8L_U16(temp[3], temp[4]) ;
	else
		goto measure_error;
	
		if(result[0]!=0)
		{
			if(IICSHT31>10)
			{
			IICSHT31=0;
			}
	    t = result[0];
	    t = ( t / 65535 ) * 175 - 45;
 
		  Sensor_SHT31.temper1 = t;//KalmanFilter(&kalman_T, t);
			Sensor_SHT31.temper = t;//KalmanFilter(&kalman_T, t);
	}
	else
	{
		Sensor_SHT31.temper = Sensor_SHT31.temper1;//KalmanFilter(&kalman_T, t);
	}
	if(result[1]!=0)
	{
	t = result[1];
	t = ( t / 65535 ) * 100;

	Sensor_SHT31.humidity1 = t;
	Sensor_SHT31.humidity = t;
	}
	else if(result[1]<1)
	{
		Sensor_SHT31.humidity = Sensor_SHT31.humidity1;
	}
//	DBG_PRINTF ( "SHT31: T = %.02f, H = %.02f\n\r" , Sensor_SHT31.temper, Sensor_SHT31.humidity);
	
	return 1;

measure_error:
	IICSHT31++;
	if(IICSHT31<=30)
	{
	Sensor_SHT31.temper = Sensor_SHT31.temper1;
	Sensor_SHT31.humidity = Sensor_SHT31.humidity1;
	}
	else 
	{
	Sensor_SHT31.temper = 0.000;
	Sensor_SHT31.humidity = 0.000;
	}
	return 0;
	
}


/*
SET_BIT(I2Cx->CR1, I2C_CR1_SWRST);
CLEAR_BIT(I2Cx->CR1, I2C_CR1_SWRST);

*/

char SHT31_ResetBus(void)
{
	SET_BIT(hi2c1.Instance->CR1, I2C_CR1_SWRST);
	HAL_Delay(100);
	CLEAR_BIT(hi2c1.Instance->CR1, I2C_CR1_SWRST);
	return 0;
}

char SHT31_ClearBus ( void )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint16_t i;
		
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(I2C_SCL_GPIO_Port, &GPIO_InitStruct);
	//read
	for(i = 0; i < 9; i++)
	{
		HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
		HAL_Delay(5);
		HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);
		HAL_Delay(5);
	}
	//stop
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);

	HAL_Delay(50);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_DeInit(I2C_SCL_GPIO_Port, I2C_SCL_Pin|I2C_SDA_Pin);
	HAL_Delay(50);
	return 0;
}

char IIC_Start ( void )
{
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	//stop
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);

	return 0;
}

char IIC_Stop ( void )
{
	//read
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	//stop
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	
	return 0;
}

char MS5607_Reset(uint8_t device_id )
{
	if(MS5607_Init(device_id))
	{	return 1;}
	else{return 0;}
}

char MS5607_Init ( unsigned char Addr )
{
	uint16_t error = 0;
	uint8_t temp[1];
	uint8_t _cmd=0x1e;

	(void)error;
	if ( !HAL_I2C_IsDeviceReady ( &hi2c1, Addr, 1, 100 ) )
	{
	//	DBG_PRINTF ( "MS5607 is ok.\n\r" );
//		SEGGER_RTT_printf ( 0, "MS5607 is ok.\n\r");
	}
	else
	{
//		DBG_PRINTF ( "MS5607 is error.\n\r" );
//		SEGGER_RTT_printf ( 0, "MS5607 is error.\n\r");
		return 0;
	}

	temp[0] = U16H_U8(MS5607BA_RESET);
	if ( HAL_I2C_Master_Transmit ( &hi2c1, Addr, temp, 1, 100 ))
		return 0;
	
	HAL_Delay ( 50 );

	//DBG_PRINTF ( "MS5607 RESET is ok.\n\r" );
//	SEGGER_RTT_printf ( 0, "MS5607 RESET is ok.\n\r");
	
	return 1;

}
void MX56XX_Read_Prom_Data(void)
{

	uint8_t _cmd;
	uint8_t _ctr;
	uint8_t  _buff[2];	
	

	_cmd = PROM_ADS0;   //prom首地址

	for(_ctr = 0;_ctr<8;_ctr++)  
	{
//		MS5607_SPI_CS(0);	
		IIC_Start();
		HAL_I2C_Master_Transmit(&hi2c1,MS5607_I2C_ADDR,&_cmd,1, 100);
		HAL_I2C_Master_Receive(&hi2c1,MS5607_ADDR_READ,_buff,2, 100);

		Cal_C[_ctr]=(_buff[0]<<8)+_buff[1];		
		_cmd += 0x02;
		IIC_Stop();
		}
		
	}


uint8_t  _presbuf[3];
uint32_t MS56XX_Do_Conversion_Pres(void)  //数据读取
{
	uint8_t  _buf1;
	IIC_Start();
	_buf1 = MS5607BA_D1_OSR_4096;
	HAL_I2C_Master_Transmit(&hi2c1,MS5607_I2C_ADDR,&_buf1,1, 100);
	IIC_Stop();
		
	if(MS56XX_Read_Buf(0x00,_presbuf,3))
	{
	_D1=(_presbuf[0]<<16)+ (_presbuf[1]<<8) +(_presbuf[2]);//读取adc值

	return _D1;
	}
	else
	{
		return 0;
	}
}
uint8_t  _tempbuf[3];
uint32_t MS56XX_Do_Conversion_Temp(void)  //数据读取
{
	uint8_t  _buf2;
	IIC_Start();
	_buf2 = MS5607BA_D2_OSR_4096;
	HAL_I2C_Master_Transmit(&hi2c1,MS5607_I2C_ADDR,&_buf2,1, 100);
	
  IIC_Stop();	
	if(MS56XX_Read_Buf(0x00,_tempbuf,3))
	{
	_D2=(_tempbuf[0]<<16)+ (_tempbuf[1]<<8) +(_tempbuf[2]);//读取adc值	
	return _D2;
	}
	else
	{
		return 0;
	}
}

char MS56XX_GetTemperature (void)     //计算温度
{
	D2_Temp = MS56XX_Do_Conversion_Temp();      //循环读取 D1 D2

	if(D2_Temp == 0)
		return 0;

	if(D2_Temp > (  ( uint32_t ) Cal_C[5] << 8 ))
		dT	= D2_Temp - (  ( uint32_t ) Cal_C[5]  << 8 );  //公式 dT = D2 - TREF = D2 - C5 * 2^8
	else
	{
		dT	= (  ( uint32_t ) Cal_C[5]  << 8 ) - D2_Temp;   	
		dT *= -1;
	}
	Temperature	= 2000 + ( dT * Cal_C[6] ) / (0x00000001 << 23); //算出温度值的100倍，2001表示20.01°  公式TEMP =20°C + dT * TEMPSENS =2000 + dT * C6 / 2^23

	if(Temperature<-4000) Temperature=-4000;
	if(Temperature>8500) Temperature=8500;
	return 1;
}

char MS56XX_GetPressure (void)  //计算温度补偿压力
{
	D1_Pres = MS56XX_Do_Conversion_Pres();   //循环读取 D1 D2

	if(D1_Pres == 0)
		return 0;
	
	OFF	=  ( ( int64_t )Cal_C[2] * (0x00000001 << 17) ) + ( ( int64_t ) Cal_C[4] * dT ) / 64.0;    //公式 OFF = OFFT1 + TCO *dT = C2 *2^17 +(C4 *dT )/ 2^6
	SENS 		= ( ( int64_t ) Cal_C[1] * (0x00000001 << 16) ) + ( ( int64_t ) Cal_C[3] * dT ) / 128.0; //公式SENS = SENST1 + TCS* dT= C1 * 2^16 + (C3 * dT )/ 2^7

	//温度补偿部分    逻辑看芯片手册   
//	Temperature = Sensor_SHT31.temper*100;
	
	if ( Temperature < 2000 ) // second order temperature compensation when under 20 degrees C
	{
		T2 = ( dT * dT ) / (( uint64_t )0x0000000001 << 31);
		Aux			= ( Temperature - 2000 ) * ( Temperature - 2000 );
		OFF2 		= 61 *  Aux / 16;
		SENS2		= 2 * Aux ;
		if ( Temperature < -1500 )
		{
			Aux			= ( Temperature + 1500 ) * ( Temperature + 1500 );
			OFF2 		= OFF2 + 15 * Aux ;
			SENS2		= SENS2 + 8 * Aux ;
		}
	}
	else		//(Temperature > 2000)
	{
		T2 = 0;
		OFF2 		= 0;
		SENS2		= 0;
	}

	Temperature = Temperature - T2;
	OFF			= OFF - OFF2;
	SENS 		= SENS - SENS2;
	
	
	Press = ( D1_Pres * SENS / (0x00000001 << 21) - OFF ) / (0x00000001 << 15); //公式 P = D1 * SENS - OFF = (D1 * SENS / 2^21 - OFF) / 2^15

	MS5607.Temperature=(float)Temperature/100;  		//单位  ℃  
	MS5607.Pressure=(float)Press/1000;           //单位  Kbar 
	if(MS5607f==1)
	{
		if(MS5607.Pressure-MS5607.Pressure1>2 || MS5607.Pressure1-MS5607.Pressure>2)
	{
		MS5607.Pressure = MS5607.Pressure1;
		IICMS5607++;
		return 0;
	}
	else
	{
		IICMS5607=0;
	}
}
	if(MS5607.Pressure>80 && MS5607.Pressure<120)
	{
		MS5607.Pressure1 = MS5607.Pressure;
		MS5607f=1;
		
	}

	if(MS5607.Temperature<-40) MS5607.Temperature=-40;
	if(MS5607.Temperature>85)  MS5607.Temperature=85;
	if(IICMS5607>15)
	{		
		MS5607.Pressure=0.000;
		return 0;
	}
	return 1; 
}

// 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t MS56XX_ReadWriteByte(uint8_t TxData)
{
  uint8_t Rxdata;
	HAL_I2C_Master_Transmit(&hi2c1,MS5607_I2C_ADDR,&TxData,1, 100);
	
  HAL_I2C_Master_Receive(&hi2c1,MS5607_ADDR_READ,&Rxdata,1, 100);   
	
  return Rxdata;                              //返回收到的数据               
}


uint8_t MS56XX_Read_Buf(uint8_t reg ,uint8_t *pbuf,uint8_t len)//参数说明  发送寄存器位置   读取指定长度的数据 放在pbuf里
{
  u8 _ctr;
	uint8_t Rxdata[3];

	IIC_Start();
	HAL_I2C_Master_Transmit(&hi2c1,MS5607_I2C_ADDR,&reg,1, 100);
	HAL_I2C_Master_Receive(&hi2c1,MS5607_ADDR_READ,Rxdata,3, 100);

	for(_ctr=0;_ctr<len;_ctr++) 
	{
    pbuf[_ctr] = Rxdata[_ctr]; 
	}

	IIC_Stop();
	return 1;//返回收到的数据    
	
}



uint8_t MS56XX_Write_Buf(u8 reg, u8 *pBuf, u8 len)//参数说明  发送寄存器位置   写指定长度的数据 
{
		u8 status,_ctr;
		status = MS56XX_ReadWriteByte(reg);   //发送寄存器值(位置),并读取状态值
	
		for(_ctr=0; _ctr<len; _ctr++) 
		{
			MS56XX_ReadWriteByte(*pBuf++);    //写入数据
		}
		return status;                      //返回读到的状态值
}

	




