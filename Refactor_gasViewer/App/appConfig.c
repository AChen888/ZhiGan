#include "appConfig.h"
#include "stdio.h"
#include "bsp_fram_driver.h"
#include "win_auth.h"
#include "rtc.h"


#define SYS_DEBUG_VERSION    "." __DATE__  __TIME__
#ifdef SYS_DEBUG_VERSION
const char* Date = __DATE__;
const char* Time = __TIME__;
#define MONTH (Date[2]=='n'?1 \
	:Date[2]=='b'?2 \
	:Date[2]=='r'?(__DATE__[0]=='M'?3:4) \
	:Date[2]=='y'?5 \
	:Date[2]=='n'?6 \
	:Date[2]=='l'?7 \
	:Date[2]=='g'?8 \
	:Date[2]=='p'?9 \
	:Date[2]=='t'?10 \
	:Date[2]=='v'?11:12)
#define DAY ((Date[4]==' '?0:Date[4]-'0')*10 \
	+(Date[5]-'0'))

static char dateStamp[15];
#endif

uint8_t ZhuYe_Flag=0;				//是否为注液机设备
//uint8_t Save_Flag=0;				//监测设置页面保存标注位
uint8_t Win_Save_Flag;			//界面数据保存标志位			(1<<0):寿命运维数据保存	(1<<1):电流输出数据保存	(1<<2):串口设置数据保存	
u32 g_StartupFlag;					//任务状态标志位
uint16_t g_SaveParamFlag;		//保存标志位
c_AlarmParam_t gAlarmParam[MODULE_CHANNEL_MAX];
SYSTEM_DATA_typedef gSystemData;
CALIDATA_typedef gCalidata[MODULE_CHANNEL_MAX];
CALIDATA_typedef gFactoryCalidata[MODULE_CHANNEL_MAX];
BOOT_IAP_typedef gBootIapData;



/********显示信息********/
const char* ViewData_Baud[evdb_End] = {
	"*2400",
	"*4800",
	"*9600",
	"*14400",
	"*19600"
};

const char* Channel_Name_EN[evdn_End] = {
	"O2",			"EX",			"H2S",			"CO",			"NH3",			"O3",			"CO2",			"VOC",			"SO2",			"NO2",
	"H2",			"H2CO",		"PH3",			"C12",		"C24O",			"NO",			"HC1",			"HCN",			"CLO2",			"CH4",
	"HF",			"C2H6O",	"C2H2",			"CH4O",		"C3H8",			"C4H10",	"N2H4",			"N2O4",			"C8H10",		"C6H6",
	"CH3CHO",	"CH3Br",	"C7H8",			"C6H7N",	"C6H5F",		"CS2",		"CHC13",		"NOX",			"CF4",			"Ar",
	"H2O2",		"N2",			"CC14",			"C3H3N",	"C2H4",			"CH3C1",	"C2H6CO",		"C6H14",		"C8H11N",		"C3H5FO2",
	"POC13",	"C2SO4",	"HNO3",			"DMC",		"NMP",			"SO3",		"CH4S",			"C3H9N",		"C2H4C12",	"Br2",
	"CH3C1",	"SOx",		"CH4S",			"AsH3",		"TDI",			"C5H12",	"CH2C12",		"C4H8",			"N2O",			"F2",
	"TVOC",		"DEC",		"C3H9A1",		"BC13",		"C5H6",			"C2H2C12","C2C14",		"C2H3C1",		"C2HC13",		"NC13",
	"C3H5C1O","CPTES",	"SiHC13",		"C4H10Q2","TMP",			"DO",			"Petrol",		"PM10"
};

const char* ViewData_Open[evdo_End] = {
	"*开启状态",
	"*关闭状态"
};

/********生产信息********/
//获得生产类型号
char* GetDeviceType ( void )
{
	return (char*)SYS_DEVICE_TYPE;
}
//获得程序版本号
char* GetSoftwareVersion ( void )
{
	return (char*)SYS_PROGRAM_VERSION;
}

/********检测重启标志************/
void CheckResetSource(void)
{
    SEGGER_RTT_printf(0, "Reset Source: ");
    
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        SEGGER_RTT_printf(0, "POR (Power-On Reset)\r\n");
        // 可能是静电导致的电源瞬断
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        SEGGER_RTT_printf(0, "PIN (External Reset)\r\n");
        // 可能是静电干扰NRST引脚
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        SEGGER_RTT_printf(0, "SFT (Software Reset)\r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        SEGGER_RTT_printf(0, "IWDG (Independent Watchdog)\r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
        SEGGER_RTT_printf(0, "WWDG (Window Watchdog)\r\n");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST)) {
        SEGGER_RTT_printf(0, "LPWR (Low Power Reset)\r\n");
    }
    
    // 清除所有复位标志
    __HAL_RCC_CLEAR_RESET_FLAGS();
}

/*
计算时间差，使用系统的HAL_GetTick()取得时间戳，毫秒级
*/
u32 gas_CheckTimeDiff(uint32_t lasttick)
{
	uint32_t newtick;
	uint32_t diff;

	newtick = HAL_GetTick();				//取得新的时间戳

	if(newtick >= lasttick)
		diff = newtick - lasttick;
	else
		diff = 0xffffffff - newtick + lasttick;

	return diff;
}

u8 GetAlarmEnable(void)
{
	uint16_t i;
	u8 count = 0;

	for(i = 0; i < MODULE_CHANNEL_MAX; i++)
	{
		if(gAlarmParam[i].Enable)
			count++;
	}
	return count;
}

uint8_t sys_SumCheckCode(uint8_t *buf, uint16_t len)
{
	uint16_t i;
	uint8_t chk;

	for(i = 0, chk = 0; i < len; i++)
	{
		chk += buf[i];
	}

	return ~chk;
}

void System_ReadCommonData(uint16_t u16Flag)
{
	if(u16Flag & SAVE_BOOTDATA_BIT)
	{
		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_BOOT,(uint8_t *)(&gBootIapData),sizeof(gBootIapData));//铁电读
	}
	if(u16Flag & SAVE_SYSTDATA_BIT)
	{
		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_SYS,(uint8_t *)(&gSystemData),sizeof(gSystemData)); 	//铁电读
	}
	if(u16Flag & SAVE_CALIDATA_BIT)
	{
		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_CALIDATA,(uint8_t *)(&gCalidata),sizeof(gCalidata)); 	//铁电读
	}
	if(u16Flag & SAVE_FACTORY_CALIDATA_BIT)
	{
		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_FACTORY_CALIDATA,(uint8_t *)(&gFactoryCalidata),sizeof(gFactoryCalidata)); 	//铁电读
	}
//	if(u16Flag & SAVE_SENSITIVITY_BIT)
//	{
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 0 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity1),sizeof(fSensorSensitivity1)); 	//铁电读
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 1 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity2),sizeof(fSensorSensitivity1));
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 2 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity3),sizeof(fSensorSensitivity1));
//	}
//	if(u16Flag & SAVE_ALARM_BIT)
//	{
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_ALARM,(uint8_t *)(&gAlarmParam),sizeof(gAlarmParam)); 	//报警参数
//	}
//	if(u16Flag & SAVE_BIN_RECEIVE_BIT)
//	{
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_BIN_RECEIVE,(uint8_t *)(&gBinData),sizeof(gBinData)); 	//铁电读
//	}
//	if(u16Flag & SAVE_COMPENSATION_BIT)
//	{
//		fs_fram_read_nbyte((uint16_t)FRAM_ADDR_COMPENSATION,(uint8_t *)(&gCompensationData),sizeof(gCompensationData)); 	//铁电读
//	}
}


void System_SaveCommonData(uint16_t u16Flag)
{
	uint8_t chk[3];
	
	if(u16Flag & SAVE_BOOTDATA_BIT)
	{
		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_BOOT,(uint8_t *)(&gBootIapData),sizeof(gBootIapData)); 		//铁电写
	}
	if(u16Flag & SAVE_SYSTDATA_BIT)
	{
		gSystemData.chk = 0;
		gSystemData.chk = sys_SumCheckCode((uint8_t*)&gSystemData, sizeof(gSystemData));
		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_SYS,(uint8_t *)(&gSystemData),sizeof(gSystemData)); 	//铁电写
	}
	if(u16Flag & SAVE_CALIDATA_BIT)
	{
		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_CALIDATA,(uint8_t *)(&gCalidata),sizeof(gCalidata)); 	//铁电写
		
		chk[0] = sys_SumCheckCode((uint8_t*)&gCalidata, sizeof(gCalidata));
		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_CALIDATA + sizeof(gCalidata), chk, 1);
		
//		MODBUS_LoadParamToHoldingReg(0, gAlarmParam[0].Class1Limit, gAlarmParam[0].Class2Limit);
//		MODBUS_LoadSysToHoldingReg(0, 0);
	}
	if(u16Flag & SAVE_FACTORY_CALIDATA_BIT)
	{
		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_FACTORY_CALIDATA,(uint8_t *)(&gFactoryCalidata),sizeof(gFactoryCalidata)); 	//铁电写
	}
//	if(u16Flag & SAVE_SENSITIVITY_BIT)
//	{
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 0 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity1),sizeof(fSensorSensitivity1)); 	//铁电读
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 1 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity2),sizeof(fSensorSensitivity1));
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 2 * sizeof(fSensorSensitivity1),(uint8_t *)(&fSensorSensitivity3),sizeof(fSensorSensitivity1));
//		
//		chk[0] = sys_SumCheckCode((uint8_t*)&fSensorSensitivity1, sizeof(fSensorSensitivity1));
//		chk[1] = sys_SumCheckCode((uint8_t*)&fSensorSensitivity2, sizeof(fSensorSensitivity1));
//		chk[2] = sys_SumCheckCode((uint8_t*)&fSensorSensitivity3, sizeof(fSensorSensitivity1));
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 3 * sizeof(fSensorSensitivity1), chk, 3);
//	}
//	if(u16Flag & SAVE_ALARM_BIT)
//	{
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_ALARM,(uint8_t *)(&gAlarmParam),sizeof(gAlarmParam)); 	//报警参数
//		
//		chk[0] = sys_SumCheckCode((uint8_t*)&gAlarmParam, sizeof(gAlarmParam));
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_ALARM + sizeof(gAlarmParam), chk, 1);
//	}
//	if(u16Flag & SAVE_BIN_RECEIVE_BIT)
//	{
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_BIN_RECEIVE,(uint8_t *)(&gBinData),sizeof(gBinData)); 	//铁电写
//	}
//	if(u16Flag & SAVE_COMPENSATION_BIT)
//	{
//		fs_fram_write_nbyte((uint16_t)FRAM_ADDR_COMPENSATION,(uint8_t *)(&gCompensationData),sizeof(gCompensationData)); 	//铁电写
//	}
//	osDelay(1);
}

void System_CommonData_Init(void)
{	
	uint8_t i = 0;
	uint8_t k = 0;
	uint8_t chk;
//	fs_fram_clear();

	System_ReadCommonData(SAVE_BOOTDATA_BIT);
	System_ReadCommonData(SAVE_SYSTDATA_BIT);  //系统参数“通讯地址，存储使能，跨点”
	System_ReadCommonData(SAVE_CALIDATA_BIT);  //浓度计算参数“气体名称，单位，零点”
//	System_ReadCommonData(SAVE_FACTORY_CALIDATA_BIT);
//	System_ReadCommonData(SAVE_SENSITIVITY_BIT);
//	System_ReadCommonData(SAVE_ALARM_BIT);  //报警参数“报警使能，报警周期”
//	System_ReadCommonData(SAVE_BIN_RECEIVE_BIT);
//	System_ReadCommonData(SAVE_COMPENSATION_BIT);//补偿系数

	chk = gSystemData.chk;
	gSystemData.chk = 0;
	if(chk != sys_SumCheckCode((uint8_t*)&gSystemData, sizeof(gSystemData))) //if(gSystemData.status != 0x01)
	{
		gSystemData.status = 0x01;
		gSystemData.u8MoudleID 			= 0x01;		//默认设备地址为1
		gSystemData.u8MoudleCondition 	= 0;		//默认设备为工况

		gSystemData.RecordEnable = 1;
		gSystemData.RecordType = 0;
		gSystemData.RecordPeriod = 60;

		gSystemData.CaliPoints = 1;
		gSystemData.resetflag = 0;
		
		gSystemData.modbus_flag = 1;
		gSystemData.Life=0;

		System_SaveCommonData(SAVE_SYSTDATA_BIT);
	}
	
	fs_fram_read_nbyte((uint16_t)FRAM_ADDR_CALIDATA + sizeof(gCalidata), &chk, 1);
	if(chk != sys_SumCheckCode((uint8_t*)&gCalidata, sizeof(gCalidata)))
	{
		for(i=0; i<USE_CHANNEL_MAX; i++)
		{
//			gCalidata[i].Name = evdn_NMP;
//			gCalidata[i].CATL_Modbus_Name = catl_name_NMP;
//			gCalidata[i].Unit = evdu_PPM;
			gCalidata[i].Format = edf_float_23;
			gCalidata[i].Enable = 0;
//			gCalidata[i].Ratio1 = 1;
//			gCalidata[i].Ratio2 = 0;
//			gCalidata[i].Ratio3 = 0;
//			gCalidata[i].Range = SENSOR_CONC_MAX;
			gCalidata[i].Out_4mA = (4.0f);
			gCalidata[i].Out_20mA = (20.0f);
			gCalidata[g_SelectChannel].Current_Open = evdb_close;

//			gCalidata[i].u32ZeroAD = SENSOR_AD_MIN;
//			gCalidata[i].fZeroConc = SENSOR_CONC_MIN;
//			gCalidata[i].u32SpanAD1 = SENSOR_AD_MAX;
//			gCalidata[i].fSpanConc1 = 390;
//			gCalidata[i].u32SpanAD2 = SENSOR_AD_MAX;
//			gCalidata[i].fSpanConc2 = 200;
//			gCalidata[i].u32SpanAD3 = SENSOR_AD_MAX;
//			gCalidata[i].fSpanConc3 = 100;
			
			gCalidata[i].Baud = evdb_9600;
			
			gFactoryCalidata[i] = gCalidata[i];
		}
		System_SaveCommonData(SAVE_CALIDATA_BIT);
		System_SaveCommonData(SAVE_FACTORY_CALIDATA_BIT);
	}
//	else
//	{
//		for(i=0; i<USE_CHANNEL_MAX; i++)
//		{
//			if(gCalidata[i].Unit == evdu_PPM)
//			{
//				gCalidata[i].Format = edf_float_23;
//			}
//			else if(gCalidata[i].Unit == evdu_ppb)
//			{
//				gCalidata[i].Format = edf_float_50;
//			}
//			else
//			{
//				gCalidata[i].Format = edf_float_32;
//			}
//		}
//	}
	
//	fs_fram_read_nbyte((uint16_t)FRAM_ADDR_SENSITIVITY + 3 * sizeof(fSensorSensitivity1), &chk, 1);
//	if(chk != sys_SumCheckCode((uint8_t*)&fSensorSensitivity1, sizeof(fSensorSensitivity1)))
//	{
//		for(i=0; i<USE_CHANNEL_MAX; i++)
//		{
//			fSensorSensitivity1[i] =  (gCalidata[i].u32SpanAD1 - gCalidata[i].u32ZeroAD) /
//                                 (gCalidata[i].fSpanConc1 - gCalidata[i].fZeroConc);
//		}
//		System_SaveCommonData(SAVE_SENSITIVITY_BIT);
//	}
	
//	fs_fram_read_nbyte((uint16_t)FRAM_ADDR_ALARM + sizeof(gAlarmParam), &chk, 1);
//	if(chk != sys_SumCheckCode((uint8_t*)&gAlarmParam, sizeof(gAlarmParam)))
//	{
//		for(i=0; i<USE_CHANNEL_MAX; i++)
//		{
//		//LX
//			//设置报警的相关参数
//			gAlarmParam[i].Enable=1;
//			gAlarmParam[i].Class2Limit=20.0;
//			gAlarmParam[i].Class1Limit=5.0;
//			gAlarmParam[i].ActDelay=15;
//			gAlarmParam[i].HoldTime=600;
//		}
//		System_SaveCommonData(SAVE_ALARM_BIT);
//	}

//	if(gBinData.u8Saved != 0x01)
//	{
//		gBinData.u8Saved 				= 0x01;
//		gBinData.u16BinSize 			= 0;
//		gBinData.u16PacketsSum 			= 0;
//		gBinData.u32BinCrc 				= 0;
//		gBinData.u32Process_FlashAddr 	= 0;
//		gBinData.u16Process_BinSize 	= 0;
//		gBinData.u16Process_PacketNum 	= 0;
//		gBinData.u8Process_Busy			= 0;
//		gBinData.u8Process_Err			= 0;
//		System_SaveCommonData(SAVE_BIN_RECEIVE_BIT);
//	}
	
//	if(gCompensationData.u8Saved != 0x01)
//	{
//		gCompensationData.u8Saved = 0x01;
//		//每个气体本身的补偿系数为1，其余为0
//		for(i=0; i<USE_CHANNEL_MAX; i++)
//		{
//			for(k=0; k<CP_SUM; k++)
//			{
//				gCompensationData.fValue[i][k] = 0;
//			}
//			gCompensationData.fValue[i][i] = 1;			
//		}
//		System_SaveCommonData(SAVE_COMPENSATION_BIT);
//	}
	//检查异常参数
	if(gSystemData.u8MoudleID > 247 || gSystemData.u8MoudleID == 0)
		gSystemData.u8MoudleID = 1;
	if(gSystemData.RecordPeriod < 30 || gSystemData.RecordPeriod > 999)
		gSystemData.RecordPeriod = 30;
	if(gSystemData.CaliPoints == 0 || gSystemData.CaliPoints > 3)
		gSystemData.CaliPoints = 2;
	
//	CheckSensorSpanRatio(SENSOR_CHANNEL1);

	g_SaveParamFlag = 0;
	g_SelAuth = 0;

//	MODBUS_LoadParamToHoldingReg(1, gAlarmParam[0].Class1Limit, gAlarmParam[0].Class2Limit);
//	MODBUS_LoadSysToHoldingReg(0, 0);
}

