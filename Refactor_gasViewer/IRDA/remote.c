
#include "tim.h"
#include "remote.h"


#define REMOTE_ID			0

#define IRDA_HTIM		TIM2
#define IRDA_TIME		htim2
#define IRDA_CHANNEL	TIM_CHANNEL_4

#define IRDA_PORT		GPIOB
#define IRDA_PIN		GPIO_PIN_11

enum {
	rKey_None   = 0,
	rKey_Power  = 162,		//打开/关闭液晶背光
	rKey_Menu   = 226,		//
	rKey_Test   = 34,		//
	rKey_OK     = 168,		//
	rKey_Cancel = 194,		//
	rKey_Delete = 176,		//
	rKey_Left   = 224,		//
	rKey_Right  = 144,		//
	rKey_Up     = 2,		//
	rKey_Down   = 152,		//
	rKey_0      = 104,
	rKey_1      = 48,
	rKey_2      = 24,
	rKey_3      = 122,
	rKey_4      = 16,
	rKey_5      = 56,
	rKey_6      = 90,
	rKey_7      = 66,
	rKey_8      = 74,
	rKey_9      = 82,
};

//遥控器接收状态
static uint8_t RmtSta = 0;
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留
//[4]:标记上升沿是否已经被捕获
//[3:0]:溢出计时器

static uint16_t Dval;		//下降沿时计数器的值
static uint32_t RmtRec = 0;	//红外接收到的数据
static uint8_t  RmtCnt = 0;	//按键按下的次数

/*
 * *@ author:lanxin
 * *@ brief:tim4定时器溢出中断回调函数
 * *@ note:在 HAL_TIM_IRQHandler(&IRDA_TIME) 中被调用，不用清除标志。
 * *@ param:NONE
 * *@ retval:NONE
 */
//void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef *htim )
void REMOTE_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef *htim )
{
	if ( htim->Instance == IRDA_HTIM )
	{
		if ( RmtSta & 0x80 ) //上次有数据被接收到了
		{
			RmtSta &= ~0x10;						//取消上升沿已经被捕获标记

			if ( ( RmtSta & 0x0F ) == 0x00 )
			{
				RmtSta |= 1 << 6; //标记已经完成一次按键的键值信息采集
			}

			if ( ( RmtSta & 0x0F ) < 14 ) //时间间隔小于14us
			{
				RmtSta++;//计时器计数值加1
			}
			else//超时
			{
				RmtSta &= ~ ( 1 << 7 ); //清空引导标识
				RmtSta &= 0xF0;	//清空计数器
			}
		}
	}
}

/*
 * *@ author:lanxin
 * *@ brief:tim4定时器输入捕获中断回调函数
 * *@ note:在 HAL_TIM_IRQHandler(&IRDA_TIME) 中被调用，不用清除标志。
 * *@ param:NONE
 * *@ retval:NONE
 */
void HAL_TIM_IC_CaptureCallback ( TIM_HandleTypeDef *htim )
{
	if ( htim->Instance == IRDA_HTIM )
	{
		if ( HAL_GPIO_ReadPin ( IRDA_PORT, IRDA_PIN ) ) //上升沿捕获
		{
			TIM_RESET_CAPTUREPOLARITY ( &IRDA_TIME, IRDA_CHANNEL ); //一定要先清除原来的设置！！
			TIM_SET_CAPTUREPOLARITY ( &IRDA_TIME, IRDA_CHANNEL, TIM_ICPOLARITY_FALLING ); //CC2P=1 设置为下降沿捕获
			__HAL_TIM_SET_COUNTER ( &IRDA_TIME, 0 );  	//清空定时器值
			RmtSta |= 0x10;						//标记上升沿已经被捕获
		}
		else //下降沿捕获
		{
			Dval = HAL_TIM_ReadCapturedValue ( &IRDA_TIME, IRDA_CHANNEL ); //读取CCR2也可以清CC2IF标志位
			TIM_RESET_CAPTUREPOLARITY ( &IRDA_TIME, IRDA_CHANNEL );   	//一定要先清除原来的设置！！
			TIM_SET_CAPTUREPOLARITY ( &IRDA_TIME, IRDA_CHANNEL, TIM_ICPOLARITY_RISING ); //配置TIM2通道4上升沿捕获

			if ( RmtSta & 0x10 )							//完成一次高电平捕获
			{
				if ( RmtSta & 0x80 ) //接收到了引导码
				{
					if ( Dval > 300 && Dval < 800 )			//560为标准值,560us
					{
						RmtRec <<= 1;					//左移一位.
						RmtRec |= 0;					//接收到0
					}
					else if ( Dval > 1400 && Dval < 1800 )	//1680为标准值,1680us
					{
						RmtRec <<= 1;					//左移一位.
						RmtRec |= 1;					//接收到1
					}
					else if ( Dval > 2200 && Dval < 2600 )	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt++; 					//按键次数增加1次
						RmtSta &= 0xF0;				//清空计时器
					}
				}
				else if ( Dval > 4200 && Dval < 4700 )		//4500为标准值4.5ms
				{
					RmtSta |= 1 << 7;					//标记成功接收到了引导码
					RmtCnt = 0;						//清除按键次数计数器
				}
			}

			RmtSta &= ~ ( 1 << 4 );
		}
	}
}

/*
 * *@ author:lanxin
 * *@ brief:获取当前的按键接收值
 * *@ note:
 * *@ param:NONE
 * *@ retval:0x00 失败或者未接收
 * *@ retval:others 接收成功
 */
uint8_t Remote_GetResult ( void )
{
	uint8_t sta = 0;
	uint8_t t1, t2;

	if ( RmtSta & ( 1 << 6 ) ) //得到一个按键的所有信息了
	{
		t1 = RmtRec >> 24;			//得到地址码
		t2 = ( RmtRec >> 16 ) & 0xff;	//得到地址反码

		if ( ( t1 == ( uint8_t ) ~t2 ) && t1 == REMOTE_ID ) //检验遥控识别码(ID)及地址
		{
			t1 = RmtRec >> 8;
			t2 = RmtRec;

			if ( t1 == ( uint8_t ) ~t2 )
				sta = t1; //键值正确
		}

		if ( ( sta == 0 ) || ( ( RmtSta & 0x80 ) == 0 ) ) //按键数据错误/遥控已经没有按下了
		{
			RmtSta &= ~ ( 1 << 6 ); //清除接收到有效按键标识
			RmtCnt = 0;		//清除按键次数计数器
		}
	}

	return sta;
}

/*
 * *@ author:lanxin
 * *@ brief:设置接收红外的timer4的中断
 * *@ note:
 * *@ param:NONE
 * *@ retval:
 */
uint8_t Remote_TimerStart ( void )
{
	HAL_TIM_IC_Start_IT ( &IRDA_TIME, IRDA_CHANNEL ); //开始捕获TIM2的通道4
	__HAL_TIM_ENABLE_IT ( &IRDA_TIME, TIM_IT_UPDATE ); //使能timer2更新中断
	return 1;
}

