#ifndef _SHT31_H
#define _SHT31_H

#include "stm32f3xx_hal.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MS5607_I2C_ADDR               0xEC

typedef struct
{
	float temper;
	float humidity;
	float temper1;
	float humidity1;
} t_SHT31_STRUCT;

typedef struct
{
	float Temperature;
	
	float Pressure;
	float Pressure1;
	float Pressure2;
} t_ms5607_STRUCT;


extern t_SHT31_STRUCT Sensor_SHT31;

extern char SHT31_Measure ( void );
extern char SHT31_Init ( void );

extern char SHT31_ClearBus ( void );
extern char SHT31_ResetBus(void);

extern t_ms5607_STRUCT MS5607;

extern char MS5607BA_Reset ( void );
extern void MS5607_Send_CMD ( u8 command );

extern void MS5607_ADC_Answer(u8 *Data);
extern void MS5607BA_readPROM(void);
extern u32 MS5607BA_Conversion_GetPressure();
extern u32  MS5607BA_Conversion_Temp();
extern char MS5607_GetTemperature();
extern char MS5607_GetPressure();
extern char MS5607_Init ( unsigned char Addr );
extern uint8_t MS56XX_ReadWriteByte(uint8_t TxData);
extern uint8_t MS56XX_Read_Buf(uint8_t reg ,uint8_t *pbuf,uint8_t len);
extern uint8_t MS56XX_Write_Buf(u8 reg, u8 *pBuf, u8 len);
extern uint32_t MS56XX_Do_Conversion_Pres(void);
extern uint32_t MS56XX_Do_Conversion_Temp(void);
extern void MX56XX_Read_Prom_Data(void);
extern char MS56XX_GetTemperature (void);
extern char MS56XX_GetPressure (void);
extern char MS5607_ClearBus ( void );
extern char IIC_Start ( void );
extern char IIC_Stop ( void );	


void IIC_MS5607_WriteCommand( unsigned char Addr,unsigned char Cmd );
char MS5607_Reset(uint8_t device_id );
void IIC_MS5607_Read(unsigned char Addr,uint8_t *pData,unsigned char Num);
char readCoefficients(uint8_t device_id);
char MS5607_Pressure_Proc(uint8_t device_id );
char ReadMs5607(void);




#ifdef __cplusplus
}
#endif


#endif

