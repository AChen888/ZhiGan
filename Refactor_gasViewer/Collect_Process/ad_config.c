#include "ad_config.h"
#include "string.h"

//全局配置变量声明
CHANNEL_CONFIG 					Channel_Config[CHANNEL_MAX]; 
G_DATA_FLAG 						gas_Data_Flag;
G_CHANNEL_TYPE					gas_Channel[CHANNEL_MAX];
//SYSTEM_MALFUNCTION			System_Malfunction;
//G_SYSTEM_STATE_TYPE     g_System_State;
uint32_t								g_System_Channel_Valid;
//SYSTEM_INIT_KEY     		gSysInitKey;
//SYSTEM_CONFIG						System_config;
//SYSTEM_PRODUCTIONINFO		System_ProductiongInfo;
//SYSTEM_COM_CONFIG				System_Com_config[SYSTEM_COM_MAXNUMBER];
//SYSTEM_DURATION					System_Duration;
//G_COM_SYSTEM_DATA				g_Com_System_Data[COM_CHANNEL_MAX];	
//G_PM										g_Pm;
//SYSTEM_TIME_TYPE				g_DateTime;	
//DATA_SAVE_HOMEPAGE			Data_Save_HomePage[CHANNEL_MAX];
//NETWORK_CONFIG					g_System_Network[NETWORK_CONNECT_MAX];
//GPS_CONFIG							g_GPS_Config;
//DATA_STORAGE_CONFIG			Data_storage_SD_config;

uint16_t								g_System_Operation_Flag;
uint8_t									g_System_Screen_Curve;
uint16_t 								g_System_Screen_Flag;
uint8_t 								g_System_Screen_Page;
uint8_t									g_System_Screen_Calibration_Page;
uint8_t									g_System_Screen_Calibration_Range_Change;
uint8_t									g_Alarm_Time_Count[ALARM_GRADE_MAX][CHANNEL_MAX];
uint8_t									g_Alarm_Mode_Tmp;

uint16_t								g_System_Calibration_Stabilize;
//uint16_t								g_System_Calibration_Queue[CHANNEL_MAX][CAILIBRITION_QUEUE_MAX];
uint8_t									g_System_Calibration_QueueTail[CHANNEL_MAX];
uint16_t								g_System_Calibration_Sellect;
uint16_t								g_System_Calibration_RunTime;
uint16_t								g_System_Calibration_Flag;
uint8_t									g_System_Advanced_Channel;
uint8_t									g_System_Advanced_CrossChoose;
uint16_t								g_Communication_Flag;

uint8_t									g_AD_Collect_Timer;



