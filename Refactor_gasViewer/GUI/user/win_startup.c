/**
 * @file
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include "ui\ui.h"
#include "tim.h"

//#include "Store\bsp_w25qxx_driver.h"
//#include "Store\bsp_fram_driver.h"
//#include "AD7171\bsp_adc.h"
//#include "App\Gas\gas.h"
//#include "Store\SDdriver.h"

  /*********************
   *      DEFINES
   *********************/

   /**********************
	*      TYPEDEFS
	**********************/
/*
	W = 20 X 16 = 320
	H = 10 X 24 = 240
   
    [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			标题栏 24
    [2][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][1]			+
    [3][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][2]			|
    [4][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][3]			|
    [5][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][4]			+     24 X 8
    [6][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][5]			+
    [7][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][6]			|
    [8][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][7]			|
    [9][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][8]			+
    [0][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][9]			状态栏 24
*/
#define LINE_1_X (16 * 0)
#define LINE_1_Y (24 * 2)
#define LINE_1_H (24 * 3)

#define LINE_2_X (16 * 0)
#define LINE_2_Y (24 * 5)
#define LINE_2_H (24 * 2)

#define LINE_3_X (16 * 10)
#define LINE_3_Y (24 * 5)
#define LINE_3_H (24 * 2)

#define LINE_X (100)
#define LINE_Y(n) (24 + 24 * n)
#define LINE_H (24)

	/**********************
	 *  STATIC PROTOTYPES
	 **********************/

	 /**********************
	  *  STATIC VARIABLES
	  **********************/
static ui_window_t StartupWindow =
{
		.pos.x = 0,
		.pos.y = 0,
		.pos.width = 320,
		.pos.height = 240,
};

ui_widgets_t* StartupWidgetsList[6] = {0};

//static ui_edit_t LocalEdit;
static u16 StartTimeout;
static u16 StepTime;
static u32 LastTick, NewTick;
/**********************
 *      MACROS
 **********************/

 /**********************
  *   GLOBAL FUNCTIONS
  **********************/
//void StartupSelfCheck(void)
//{
//	char* string;
//	char text[32];

//	#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[g_SelectChannel].Name ])

////	snprintf(text, sizeof(text), "Type: %s", GetDeviceType()); 
////	ui_show_text_ex(LINE_X, LINE_Y(0), 220, LINE_H, text, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, ( eat_hleft | eat_vcenter ), 0);
//	if(gSystemData.resetflag==0)
//	{
//	snprintf(text, sizeof(text), "Ver: %s", GetSoftwareVersion());
//	ui_show_text_ex(LINE_X, LINE_Y(0), 220, LINE_H, text, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, ( eat_hleft | eat_vcenter ), 0);

//	ui_show_lable(LINE_X, LINE_Y(1), 220, LINE_H, "turn on alarm led", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	alarm_SelfCheck(0, 1);		//打开报警灯
//	osDelay(500);
//	
//	if(1)
//		string = "norflash ok!";
//	else
//		string = "norflash error!";
//	ui_show_lable(LINE_X, LINE_Y(2), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	osDelay(500);
//	
//	if(fram_SelfCheck)
//		string = "fram ok!";
//	else
//		string = "fram error!";
//	ui_show_lable(LINE_X, LINE_Y(3), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	osDelay(500);

//	alarm_SelfCheck(0, 0);		//关闭报警灯
//	ui_show_lable(LINE_X, LINE_Y(4), 220, LINE_H, "turn off alarm led", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	osDelay(500);

//	if(adc_SelfCheck)
//		string = "adc ok!";
//	else
//		string = "adc error!";
//	ui_show_lable(LINE_X, LINE_Y(5), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	
//		if(SD_Initialize()==0)
//		string = "TFcard ok!";
//	else
//		string = "TFcard error!";
//	ui_show_lable(LINE_X, LINE_Y(5), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	osDelay(3000);
//}
//	else{
//			snprintf(text, sizeof(text), "Ver: %s", GetSoftwareVersion());
//	ui_show_text_ex(LINE_X, LINE_Y(0), 220, LINE_H, text, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, ( eat_hleft | eat_vcenter ), 0);

//	ui_show_lable(LINE_X, LINE_Y(1), 220, LINE_H, "turn on alarm led", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
////	alarm_SelfCheck(0, 1);		//打开报警灯
////	osDelay(500);
//	
//	if(1)
//		string = "norflash ok!";
//	else
//		string = "norflash error!";
//	ui_show_lable(LINE_X, LINE_Y(2), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
////	osDelay(500);
//	
//	if(fram_SelfCheck)
//		string = "fram ok!";
//	else
//		string = "fram error!";
//	ui_show_lable(LINE_X, LINE_Y(3), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
////	osDelay(500);

////	alarm_SelfCheck(0, 0);		//关闭报警灯
//	ui_show_lable(LINE_X, LINE_Y(4), 220, LINE_H, "turn off alarm led", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
////	osDelay(500);

//	if(adc_SelfCheck)
//		string = "adc ok!";
//	else
//		string = "adc error!";
//	ui_show_lable(LINE_X, LINE_Y(5), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//	
//		if(SD_Initialize()==0)
//		string = "TFcard ok!";
//	else
//		string = "TFcard error!";
//	ui_show_lable(LINE_X, LINE_Y(5), 220, LINE_H, string, efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
//		
//	
//	}
//	g_StartupFlag |= ese_SelfCheck;
//}

  //创建控件
static void Startup_InitWidgetsList(void)
{
	StepTime = 0;
	StartTimeout = 1;
	LastTick = HAL_GetTick();
}


static void Startup_TimerCallback(void)
{
	char string[16];
	
	NewTick = HAL_GetTick();
	if(NewTick - LastTick > 999)
	{		
		LastTick = NewTick;
		if(StartTimeout)
		{
			StartTimeout--;

			switch (StepTime)		//倒计时过程中
				{
				case 0:				//开机显示logo
					ui_show_lable(LINE_2_X, LINE_2_Y, 320, LINE_2_H, "智感技术(天津)有限公司", efsize_24, rgb565_lable_text, StartupWindow.color.bcolor, (eat_hcenter | eat_vcenter));			//20230302
					break;

				case 1:				//开机自检
					ui_show_lable(LINE_2_X, LINE_2_Y, 100, LINE_2_H, "开机自检", efsize_24, rgb565_lable_text, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));
				
					//清除右半屏
					ui_show_lable(LINE_X, LINE_Y(0), 220, LINE_H*8, " ", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));

//					StartupSelfCheck();

					//清除右半屏
					ui_show_lable(LINE_X, LINE_Y(0), 220, LINE_H*8, " ", efsize_24, rgb565_text_front, StartupWindow.color.bcolor, (eat_hleft | eat_vcenter));

					break;

				case 2:				//开机预热
				default:
					if(gSystemData.resetflag==0)
					{
					ui_show_lable(LINE_2_X, LINE_2_Y, 100, LINE_2_H, "开机预热", efsize_24, rgb565_lable_text, StartupWindow.color.bcolor, (eat_hcenter | eat_vcenter));
					snprintf(string, sizeof(string), "%d", StartTimeout);
					ui_show_lable(LINE_3_X, LINE_3_Y, 220, LINE_3_H, string, efsize_64, rgb565_digital_front, StartupWindow.color.bcolor, (eat_hcenter | eat_vcenter));
					}
					break;
				}
		}
		
		if(StartTimeout == 0)
		{
			switch (StepTime)		//结束
				{
				case 0:				//开机显示logo
					StartTimeout = 1;	//自检时间
					break;

				case 1:				//开机自检
					if(gSystemData.resetflag==0)
					{
					StartTimeout = 11;//预热时间
					}
					else
					{
						StartTimeout = 1;
					}
					break;

				case 2:				//开机预热
				default:
				 gSystemData.resetflag=0;
	       g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
									if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
					
					MainWin_Create(0);		//下一个窗口

				StepTime = 0;
					break;
				}

			StepTime++;
		}
	}
}


//事件回调函数
static void Startup_Callback(void* parg)
{
	ui_window_t* win = (ui_window_t*)parg;

	if (win->event.event & ewm_init)
	{
		Startup_InitWidgetsList();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if (win->event.event & ewm_paint)
	{
		win->event.paint(win);
		win->event.event &= ~ewm_paint;
	}
	else if (win->event.event & ewm_update)
	{
		ui_widget_update((ui_widgets_t*)win);
		win->event.event &= ~ewm_update;
	}
	else if (win->event.event & ewm_key)
	{
		win->event.event &= ~ewm_key;
	}
	else if (win->event.event & ewm_time)
	{
		Startup_TimerCallback();
	}
	else if (win->event.event & ewm_delete)
	{
		win->event.event &= ~ewm_delete;
	}
}

/*
* 创建第一个窗口
*/
void Startup_Create(void* parg)
{
	(void)parg;

	ui_window_create((ui_window_t*)parg, &StartupWindow, Startup_Callback);
	StartupWindow.event.event |= ewm_time;						//打开定时器
	ui_widgets_set_top_win((ui_widgets_t*)&StartupWindow);		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
