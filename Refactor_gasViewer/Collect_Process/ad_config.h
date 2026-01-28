#ifndef __AD_CONFIG_H_
#define __AD_CONFIG_H_

#include "stm32f3xx_hal.h"
#include "main.h"
#include "ad7171.h"

#define 	GROUP_MAX 									60
#define 	CHANNEL_MAX									2
#define   ALARM_GRADE_MAX   					3									//最大报警通道等级

/**********************************结构体*************************************/

/**********************通道配置参数**************************/
typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

}SYSTEM_TIME_TYPE;             //时间结构体

typedef struct
{
    /************基础配置**************/
    char                Channel_Name[8];                                                //物质名称，用以对用户展示
    uint16_t        		Channel_Flag;                                                       //当前通道状态和标志
																							/*0：预留
																									1：是否为主测量因子；
																									2：是否上传；
																									3：是否为特殊计算因子，如OU；
																									4：是否通过AD7171模拟量采集
																									5：是否默认进行零点校准
																									6：是否可以被修改单位
	
	*/
																				
    uint8_t         		Channel_Accuracy;                                               //显示精度用以记录数据精度
    char                Channel_UpCode[8];                                          	//上传代码
    float               Channel_UpLim;                                                  //显示上限
    float               Channel_LowLim;                                                 //显示下限
    float               Channel_MolecularWeight;                                			//相对分子质量
		uint8_t							Channel_Screen_Count;																				//屏幕相对位置
		uint8_t							Channel_Calibration_Count;																		//校准页面相对位置
		uint16_t						Channel_Lifetime;																					//传感器寿命
		uint8_t							Channel_Lifetime_Con;																			//传感器寿命系数
		uint8_t							Channel_Unit;																							//通道单位
		float								Channel_LEL;																							//爆炸下限值
    /************滤波配置**************/
    uint8_t         		Channel_FilterMode;                                         //滤波方式
    uint8_t         		Channel_FilterLength;                                       //滤波长度
    uint8_t         		Channel_JumpSlope;                          //跳出斜率
    uint8_t         		Channel_JumpLength;                         //跳出队列长度
    uint8_t         		Channel_Outliert;                               //异常值变化率
    /************校准配置**************/
    //uint16_t        		Channel_ZeroAD;                                 //零点AD值
    uint16_t        		Channel_RangeAD[4];                         //量程点AD值,共三组,第一组作为零点AD值
    float               Channel_RangeCon[4];  	                      //量程点浓度值，共三组，第一组作为零点浓度值
		uint8_t							Channel_Calibration_Tem;										//最后一次校准的温度
		uint8_t							Channel_Calibration_Hum;										//最后一次校准的湿度
    SYSTEM_TIME_TYPE 		Channel_Calibration_ZeroTime;								//最后一次零点成功校准时间
		SYSTEM_TIME_TYPE		Channel_Calibration_RangeTime;							//最后一次量程成功校准时间
		uint16_t						Channel_Calibration_Stabilize;							//校准稳定阈值
		uint16_t						Channel_Calibration_SetupTime;							//校准准备时间
		

    /************交叉运算配置**************/
    uint8_t         		Channel_Crossover[4];                       //交叉运算通道编号，共四组
    uint8_t         		Channel_CrossoverMode[4];               		//交叉运算方法
																																		/*0：测量值不参与运算，即仅依靠Y=AX+B+C
																																				1：浓度值相加再加上参数；
																																				2：浓度值减去交叉运算通道浓度值加上参数；
																																				3：浓度值加浓度值乘以参数；
																																				4：不大于运算通道浓度值乘以参数；
																																				5：不小于运算通道浓度值乘以参数；*/
    float               Channel_CrossoverParameter[4];  						//交叉运算参数,共四组
    /************换算补偿配置**************/
    float               Channel_OUA;                                        //OU计算系数A
    float               Channel_OUB;                                        //OU计算系数B
    float               Channel_OUC;                                        //OU计算系数C
    uint8_t         		Channel_OUD;                                        //OU计算系数D
    float               Channel_TemComA;                                //温度补偿系数A
    float               Channel_TemComB;                                //温度补偿系数B
    float               Channel_HumComA;                                //湿度补偿系数A
    float               Channel_HumComB;                                //湿度补偿系数B
    float               Channel_AmendParameterA;                //修正系数A
    float               Channel_AmendParameterB;                //修正系数B
    uint8_t         		Channel_AmendParameterC;                //修正系数C,随机波动百分比0~1%FS波动
    /************报警配置**************/
    float               Channel_AlarmValue[ALARM_GRADE_MAX];                  //报警值
    uint8_t         		Channel_AlarmTime[ALARM_GRADE_MAX];                       //报警时间
    //uint8_t         		Channel_AlarmMode[ALARM_GRADE_MAX];                       //报警动作
																																		/*0位：报警灯；
																																				1位：串口输出；
																																				2位：湿节点；
																																				3位：报警开关：0：不启用，1：启用   */
    /************预留**************/
    uint16_t        		Channel_Reserved;                               //预留


} CHANNEL_CONFIG;

typedef struct
{
	float 								Channel_data_24[24];														//存储通道的最近小时均值
	uint32_t							Channel_data_24_LastTime;												//相对00年1月1日0时0分的相对小时值
	
	float									Channel_data_30[30];														//存储通道的最近日均值
	uint32_t							Channel_data_30_LastTime;												//相对00年1月1日0时0分的相对小时值
}DATA_SAVE_HOMEPAGE;

typedef struct
{
		uint32_t					gas_Data_Flag_Setting;															//数据配置标志
    uint32_t        	gas_Data_Flag_Collect;                          		//数据采集完成标志
		uint32_t					gas_Data_Flag_Smooth;																//数据滤波完成标志
		uint32_t					gas_Data_Flag_Conversion;														//数据转换完成标志
		uint32_t					gas_Data_Flag_Compensate;														//数据补偿完成标志
		uint32_t					gas_Data_Flag_Amend;																//数据修正完成标志
    uint32_t        	gas_Data_Flag_Dispose;                          		//数据处理完成标志
    uint32_t        	gas_Data_Falg_Export;                               //数据输出完成标志
} G_DATA_FLAG;

/************************通道过程数据**************************/
typedef struct
{
		uint16_t 					gas_Channel_AD_Real;																		//当前数据实时AD值
		float							gas_Channel_Value_Real;																	//当前检测浓度值（单位换算后，最终显示记录）
		uint16_t					gas_Channel_AD_Previously;															//前一时刻记录数据有效AD值
		float							gas_Channel_Value_Previously;														//前一时刻记录数据有效浓度值
		uint16_t					gas_Channel_AD_Process;																	//当前数据计算用AD值，过程量
		float 						gas_Channel_Value_Process_Con;													//存放当前计算节拍下数据换算后的浓度值
		float							gas_Channel_Value_Process_Com;													//存放当前计算节拍下数据补偿后的浓度值
		float							gas_Channel_Value_Process_Amend;												//存放当前计算节拍下数据修正后的浓度值
}G_CHANNEL_TYPE;

typedef struct
{
	float 				Curve_Group_Value[GROUP_MAX];										//曲线队列
	uint8_t				Curve_Group_Tail;																//曲线队列队尾
	uint16_t			Curve_Group_Timer;															//数据更新计时器
	float					Curve_Group_Max;																//曲线队列最大值
	float					Curve_Group_Min;																//曲线队列最小值
	float					Curve_Rate;																			//曲线显示时的放大倍率
	uint8_t				Curve_Rate_Grade;																//曲线放大标志等级
}G_CURVE_GROUP;

typedef struct
{
	uint8_t				Com_R_buf[512];
	uint8_t				Com_T_buf[512];
	uint16_t			Com_R_num;
	uint8_t				Com_R_TimeOut;
	uint8_t				Com_R_Sem;
}G_COM_SYSTEM_DATA;




/**********************************变量声明*************************************/

extern 	CHANNEL_CONFIG 			Channel_Config[CHANNEL_MAX];                             	//采集通道变量声明
extern	DATA_SAVE_HOMEPAGE	Data_Save_HomePage[CHANNEL_MAX];													//通道的历史均值数据
extern 	G_DATA_FLAG 				gas_Data_Flag;                                               //数据状态标志
//extern 	unsigned char 			Channel_real_AD;                                         	//模拟通道数量
extern 	G_CHANNEL_TYPE			gas_Channel[CHANNEL_MAX];																		//全局通道的数据
//extern		G_COM_SYSTEM_DATA		g_Com_System_Data[COM_CHANNEL_MAX];												//串口通讯数据全局变量
//extern		G_ENV_THP						g_Env_THP[2];																							//气室和环境温湿压
extern	G_CURVE_GROUP				g_Curve_Group[CHANNEL_MAX];																//数据曲线队列
//extern		G_PM								g_Pm;																											//颗粒物测量值
//extern		DATA_STORAGE_CONFIG	Data_storage_SD_config;																		//SD 卡数据存储配置信息
extern	uint8_t							g_AD_Collect_Timer;																				//AD重启计数时间
extern 	uint32_t						g_System_Channel_Valid;												//全局变量，系统模拟量通道配置标志

#endif
