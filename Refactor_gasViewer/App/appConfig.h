#ifndef __APPCONFIG_H_
#define __APPCONFIG_H_

#include "stm32f3xx_hal.h"
#include "main.h"

/********生产信息*********/
#define SYS_DEVICE_TYPE				"GTQ-ZG-001"
#define SYS_PROGRAM_VERSION  	"V1.3.0"

/********通道数量*********/
#define MODULE_CHANNEL_MAX 	1
#define USE_CHANNEL_MAX			1

/******电流输出量程*******/
#define CURRENT_DAC_4MA					(4095.0f * 4 / 33)
#define CURRENT_DAC_20MA				(4095.0f * 20 / 33)

/*****铁电读写标志位******/
#define SAVE_BOOTDATA_BIT					(0x0001 << 0)
#define SAVE_SYSTDATA_BIT					(0x0001 << 1)
#define SAVE_CALIDATA_BIT					(0x0001 << 2)
#define SAVE_FACTORY_CALIDATA_BIT	(0x0001 << 3)
#define SAVE_SENSITIVITY_BIT			(0x0001 << 4)
#define SAVE_BIN_RECEIVE_BIT			(0x0001 << 5)
#define SAVE_COMPENSATION_BIT			(0x0001 << 6)
#define SAVE_ALARM_BIT						(0x0001 << 7)

/****铁电数据保存地址****/
//铁电的地址是“字”为单位的
#define  FRAM_ADDR_BOOT										(0x00)
#define  FRAM_ADDR_SYS										(0x100)
#define  FRAM_ADDR_CALIDATA								(FRAM_ADDR_SYS 							+ 16 +  sizeof(gSystemData))        			//系统数据//提前空余出来位置的做法，提供了很大的容错和可修改余地
#define  FRAM_ADDR_FACTORY_CALIDATA				(FRAM_ADDR_CALIDATA 				+ 16 +  sizeof(gCalidata))								//
#define  FRAM_ADDR_SENSITIVITY						(FRAM_ADDR_FACTORY_CALIDATA + 16 +  sizeof(gFactoryCalidata))
#define  FRAM_ADDR_ALARM									(FRAM_ADDR_SENSITIVITY 			+ 16 +  3 * sizeof(fSensorSensitivity1))
#define  FRAM_ADDR_BIN_RECEIVE						(FRAM_ADDR_ALARM 						+ 16 +  sizeof(gAlarmParam))

#define  FRAM_ADDR_END										(FRAM_ADDR_TIME 		+ 16 +  0)

//测试数据
#define  FRAM_ADDR_TIME										(0x900)			

//BOOT铁电中的数据
typedef struct 
{
	uint8_t		u8Chk;					//是否初始化过，校验和
	uint8_t 	u8NeedIap; 				//是否需要IAP
	uint8_t 	u8IapStatus;			//IAP的反馈
	uint8_t 	u8Crc;					//CRC
	uint32_t 	u32BinLen; 				//Bin的长度
}BOOT_IAP_typedef; 


/********UI格式******/
 typedef enum {
	edf_int_5 = 0,
	edf_uint_1,
	edf_uint_2,
	edf_uint_3,
	edf_uint_4,
	edf_uint_5,
	edf_uint_9,
	edf_ulong_10,
	edf_float_14,
	edf_float_23,
	edf_float_32,
	edf_float_41,
	edf_float_50,

	edf_sint_5,
	edf_sfloat_14,
	edf_sfloat_23,
	edf_sfloat_32,
	edf_sfloat_41,
	edf_sfloat_50,

	edf_End,

} e_ViewData_Format;
/*********波特率选择*************/
typedef enum {
	evdb_2400,
	evdb_4800,
	evdb_9600,
	evdb_14400,
	evdb_19200,
	evdb_End
} e_ViewData_Baud;
/********气体因子选择***********/
typedef enum {
	evdn_O2,
	evdn_EX,
	evdn_H2S,
	evdn_CO,
	evdn_NH3,
	evdn_O3,
	evdn_CO2,
	evdn_VOC,
	evdn_SO2,
	evdn_NO2,
	evdn_H2,
	evdn_H2CO,
	evdn_PH3,
	evdn_C12,
	evdn_C24O,
	evdn_NO,
	evdn_HC1,
	evdn_HCN,
	evdn_CLO2,
	evdn_CH4,
	evdn_HF,
	evdn_C2H6O,
	evdn_C2H2,
	evdn_CH4O,
	evdn_C3H8,
	evdn_C4H10,
	evdn_N2H4,
	evdn_N2O4,
	evdn_C8H10,
	evdn_C6H6,
	evdn_CH3CHO,
	evdn_CH3Br,
	evdn_C7H8,
	evdn_C6H7N,
	evdn_C6H5F,
	evdn_CS2,
	evdn_CHC13,
	evdn_NOX,
	evdn_CF4,
	evdn_Ar,
	evdn_H2O2,
	evdn_N2,
	evdn_CC14,
	evdn_C3H3N,
	evdn_C2H4,
	evdn_CH3C1,
	evdn_C2H6CO,
	evdn_C6H14,
	evdn_C8H11N,
	evdn_C3H5FO2,
	evdn_POC13,
	evdn_C2SO4,
  evdn_HNO,			
	evdn_DMC,
	evdn_NMP,
	evdn_SO3,
	evdn_CH4S,
  evdn_C3H9N,
	evdn_C2H4C12,
	evdn_Br2,
	evdn_CH3C11,
  evdn_SOx,
	evdn_CH4S1,
	evdn_AsH3,
	evdn_TDI,
  evdn_C5H12,
	evdn_CH2C12,
	evdn_C4H8,
  evdn_N2O,
	evdn_F2,
	evdn_TVOC,
	evdn_DEC,
	evdn_C3H9A1,
	evdn_BC13,
	evdn_C5H6,
	evdn_C2H2C12,
	evdn_C2C14,
	evdn_C2H3C1,
	evdn_C2HC13,
	evdn_NC13,
	evdn_C3H5C1O,
	evdn_CPTES,
	evdn_SiHC13,
	evdn_C4H10Q2,
	evdn_TMP,
	evdn_DO,
	evdn_Petrol,
	evdn_PM10,
	evdn_End
} e_ViewData_Name;		//协议2的气体因子编号

typedef enum {
	catl_name_O2 = 0,
	catl_name_DEC,	
	catl_name_C2H5OH ,
	catl_name_CH4,
	catl_name_NMP,
	catl_name_H2,
	catl_name_H2S,
	catl_name_DMF,
	catl_name_CH4O,
	catl_name_NH3,
	catl_name_HCN,
	catl_name_C3H6O3,
	catl_name_C2H2,
	catl_name_C4H10,
	catl_name_HF,
	catl_name_COCL2,
	catl_name_CH2O,
	catl_name_VOC,
	catl_name_C6H5OH,
	catl_name_SiH4,
	catl_name_EX,
	catl_name_F2,
	catl_name_SO2,
	catl_name_HCL,
	catl_name_NO2,
	catl_name_NOX,
	catl_name_End,
} e_CATL_Modbus_Name;		//协议1的气体因子编号
 
/*********测试开关*********/
typedef enum {
	evdb_open,
	evdb_close,
	evdo_End
} e_ViewData_Open;

/*********报警*********/
typedef struct {
    float Class2Limit;//高报警限值
    float Class1Limit;//低报警限值
    uint16_t ActDelay;	//报警动作延时，防止频繁启动
    uint16_t HoldTime;//报警持续时间
    uint8_t Enable;//是否使能报警
}c_AlarmParam_t;

/*******任务状态******/
typedef enum
{
	ese_HardwareInit   = (0x00000001u << 0),
	ese_SysDataInit    = (0x00000001u << 1),
	ese_CommTask       = (0x00000001u << 2),
	ese_GasTask        = (0x00000001u << 3),
	ese_GuiTask        = (0x00000001u << 4),
	ese_KeyTask        = (0x00000001u << 5),
	ese_StartTask      = (0x00000001u << 6),
	ese_GasCalc        = (0x00000001u << 7),
	ese_SelfCheck      = (0x00000001u << 8),
} e_StartupEvent;

/*******系统数据******/
typedef struct 
{
	uint8_t 	chk;								//校验码
	uint8_t		status;
	uint8_t 	u8MoudleID; 
	uint8_t 	u8MoudleCondition; 	//工况：0 / 标况：1

	uint8_t 	RecordEnable;				//开启存储
	uint8_t		RecordType;					//0定时触发，1报警触发
	uint16_t 	RecordPeriod;				//存储周期 0-1分钟，1-5分钟，2-10分钟
	uint8_t 	preHotTime;

	uint8_t 	CaliPoints;					//多点校准
	
	uint8_t 	u8Bak[62];
	char 			name[24];				
	u32 			name_sn;						//按键输入SN
	uint8_t 	resetflag;					//复位标志位
	uint8_t		modbus_flag;				//协议标志位
	u16 			Life;      					//运行天数
	
}SYSTEM_DATA_typedef; 

typedef struct
{
	uint8_t			Current_Open;			//电流输出测试开关标志位
	u16 				Baud;							//串口波特率
	float				Range;						//量程

	uint8_t			Name;							//协议2气体名称
	uint8_t			CATL_Modbus_Name;	//协议1气体名称

	uint8_t			Unit;							//测量单位
	uint8_t			Format;						//数据格式
	uint8_t			Enable;						//启用通道

	uint32_t    u32SpanAD2;   		//目标点AD值
	float    		fSpanConc2;      	//目标点浓度值
	
	uint8_t			Out_4mA;					//特定DAC值对应输出电流大小
	uint8_t			Out_20mA;					//特定DAC值对应输出电流大小

	//以下共  字节	
	uint8_t			Bak[24];

	uint8_t 		LastZeroTime[6];		//最后修改时间
	uint8_t 		LastConcTime[6];		//最后修改时间
	uint8_t 		LastOutTime[6];			//最后修改时间
	uint8_t 		LastRangeTime[6];		//最后修改时间
	uint8_t 		LastChannelTime[6];		//最后修改时间
}CALIDATA_typedef;

typedef struct 
{
	uint8_t 	u8Saved;				//铁电已存储标志
	uint16_t	u16BinSize;				//Bin的大小（2的倍数）
	uint16_t	u16PacketsSum;			//总包数
	uint32_t	u32BinCrc;				//Bin CRC
	uint32_t	u32Process_FlashAddr;	//当前FLASH地址
	uint16_t	u16Process_BinSize;		//要写入的大小
	uint16_t	u16Process_PacketNum;	//当前包号
	uint8_t		u8Process_Busy;			//用来表示正在忙什么
	uint8_t		u8Process_Err;			//用来表示当前的状态
	
}BIN_RECEIVE_typedef; 


/**********外部声明***********/
extern uint8_t ZhuYe_Flag;
//extern uint8_t Save_Flag;
extern uint8_t Win_Save_Flag;
extern BOOT_IAP_typedef gBootIapData;	//BOOT 铁电中的数据
extern c_AlarmParam_t gAlarmParam[MODULE_CHANNEL_MAX];
extern u32 g_StartupFlag;
extern uint16_t g_SaveParamFlag;
extern SYSTEM_DATA_typedef gSystemData;
extern CALIDATA_typedef gCalidata[MODULE_CHANNEL_MAX];
extern BIN_RECEIVE_typedef gBinData;
extern CALIDATA_typedef gFactoryCalidata[MODULE_CHANNEL_MAX];
extern const char* ViewData_Baud[evdb_End];
extern const char* Channel_Name_EN[evdn_End];
extern const char* ViewData_Open[evdo_End];



/**********函数声明***********/
char* GetDeviceType ( void );
char* GetSoftwareVersion ( void );
void System_ReadCommonData(uint16_t u16Flag);
void System_SaveCommonData(uint16_t u16Flag);
void System_CommonData_Init(void);
void CheckResetSource(void);
void lcd_BG_Control(void);


u32 gas_CheckTimeDiff(uint32_t lasttick);
u8 GetAlarmEnable(void);

#endif
