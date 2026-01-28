/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "rtc.h"
#include "appConfig.h"
#include "win_auth.h"
//#include "App\Gas\gas.h"
#include "SHT31.h"

#include "SEGGER_RTT.h"
//#include "SDdriver.h"
/*********************
 *      DEFINES
 W = 20 X 16 = 320
 H = 10 X 24 = 240

 [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			标题栏 24
 [2][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [3][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [4][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [5][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+     24 X 8
 [6][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [7][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [8][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [9][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [0][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			状态栏 24
 *********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
uint8_t TFtimflag=1;
uint16_t Pape_Flag=0;


static ui_window_t MainWindow =
{
	.pos.x = 0,
	.pos.y = 0,
	.pos.width = 320,
	.pos.height = 240,
};

static u8 WinMainStart = 0;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/*

*/
void MainWin_UpdateTitleBar ( char *title )			//标题栏渲染（左上角）
{
	if ( title )
		ui_show_lable ( 0, 0, 320, 30, " ", efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hleft | eat_vcenter ) );
		ui_show_lable ( 24*4, 0, 320-24*4-10*6, 30, title, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );
}

static u32 UpdateTimeTick;
void MainWin_UpdateTimeBar ( void )							//时间栏渲染（实时时间）
{
	char string[32];
	u32 tick = HAL_GetTick();

	if(Pape_Flag==0)
	{
		if ( tick - UpdateTimeTick > 99 && WinMainStart > 0)
		{
			UpdateTimeTick = tick;
			//16:16
			snprintf ( string, sizeof ( string ), "20%02d-%02d-%02d %02d:%02d", gDateTime.Year, gDateTime.Month, gDateTime.Day, gDateTime.Hours, gDateTime.Minutes );
			ui_show_text_ex ( 320 - 16*5, 0, 16*5, 30, string, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hright | eat_vcenter ) , 0);

			MainWin_UpdateIconBar();
		}
	}
	else
	{
		if ( tick - UpdateTimeTick > 99 && WinMainStart > 0)
	{
		UpdateTimeTick = tick;
		//16:16
		snprintf ( string, sizeof ( string ), "%02d:%02d", gDateTime.Hours, gDateTime.Minutes );
		ui_show_text_ex ( 320 - 10*6, 0, 10*6, 30, string, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hright | eat_vcenter ) , 0);

		MainWin_UpdateIconBar();

	}
	}
	
}

u8 Alarm_Test=1;
u8 Record_Test=1;

void MainWin_UpdateIconBar ( void )							//图标栏渲染（开发者，报警等图标）
{
	char* text;
	u16 fcolor;

	if(g_SelAuth_id==0)
	{
		text = "  ";
	}
	else
	{
		text = "⑶";
	}
	
	//权限
	if(g_SelAuth_id==2)
	{
		ui_show_text_ex ( 0, 0, 24, 30, text, efsize_24, rgb565_icon_auth, rgb565_title_background, ( eat_hcenter | eat_vcenter ) , 0 );
	}
	else
	{
		ui_show_text_ex ( 0, 0, 24, 30, text, efsize_24, rgb565_icon_record, rgb565_title_background, ( eat_hcenter | eat_vcenter ) , 0 );
	}
	AuthCheck();

	if(Alarm_Test)/*GetAlarmEnable()*/
		text = "⑴";
	else
		text = "  ";
	//报警
//	if(gas_GetModuleAlarmState() == evda_Class1)
//		fcolor = rgb565_draw_orange;
//	else if(gas_GetModuleAlarmState() == evda_Class2)
//		fcolor = rgb565_draw_red;		
//	else
		fcolor = rgb565_icon_record;
	ui_show_text_ex ( 24, 0, 24, 30, text, efsize_24, fcolor, rgb565_title_background, ( eat_hcenter | eat_vcenter ) , 0 );
	
	if(Record_Test)/*gSystemData.RecordEnable*/
		text = "⑵";
	else
		text = "  ";
	//数据记录
	ui_show_text_ex ( 24*2, 0, 24, 30, text, efsize_24, rgb565_icon_record, rgb565_title_background, ( eat_hcenter | eat_vcenter ) , 0 );
	if(TFtimflag ==1)
		text = "⑸";
	else
		text = "  ";
	//TF卡
	ui_show_text_ex ( 24*3, 0, 24, 30, text, efsize_24, rgb565_icon_record, rgb565_title_background, ( eat_hcenter | eat_vcenter ) , 0 );
}

void MainWin_UpdateStatusBar ( char *up, char *down, char *ok )						//底部栏渲染（返回，确定等显示）
{
	char* str;
	char* str1 = "    ";
	char buf[16] = {0};
	
	ui_DrawBar ( 0, 240 - 30, 320, 30, rgb565_title_background );

	if(up == 0)
		str = str1;
	else
		str = up;
	snprintf(buf, sizeof(buf), "[%s]", str);
	ui_show_lable ( 40 + 90 * 0, 240 - 30, 80, 30, buf, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );

	if(down == 0)
		str = str1;
	else
		str = down;
	snprintf(buf, sizeof(buf), "[%s]", str);
	ui_show_lable ( 40 + 90 * 1, 240 - 30, 80, 30, buf, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );

	if(ok == 0)
		str = str1;
	else
		str = ok;
	snprintf(buf, sizeof(buf), "[%s]", str);
	ui_show_lable ( 40 + 90 * 2, 240 - 30, 80, 30, buf, efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );
}

#define _UNIT_W		(12 * 5)
#define _NAME_W		_UNIT_W

static void MainWin_UpdateViewData ( u16 x, u16 y, u16 w, u16 h, char *name, char *unit, char *data, u8 drawback )			
{
	u16 fsize = 24 * 6;
	// 计算可容纳的字体长度（6=单个字符的像素宽度系数）
	u16 flen = (w - _UNIT_W - 4) / 6;
	u16 fcolor;
	// 若区域足够大（flen>32 且 高度>64）→ 用64号大字体（醒目显示数值）
	if(flen > 32 && h > 64)
		fsize = efsize_64;
	else
		fsize = efsize_24;

	//报警
//	if(gas_GetModuleAlarmState() == evda_Class1)
//		fcolor = rgb565_draw_orange;
//	else if(gas_GetModuleAlarmState() == evda_Class2)
//		fcolor = rgb565_draw_red;
//	else
		fcolor = rgb565_digital_front;
		
	if ( data )
		ui_show_text_ex ( x  + 2, y + 2, w - _UNIT_W - 4, h - 4, data,   fsize,
		                fcolor, rgb565_button_background, ( eat_hleft | eat_vcenter ), drawback );

	if ( unit )
	{
		ui_show_lable ( x + w - 64+2, y + 2,    _NAME_W, (h - 4) / 2, name,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
		ui_show_lable ( x + w - 64+2, y + h / 2, _UNIT_W, (h - 4) / 2, unit,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
	}
	else
	{
		ui_show_lable ( x + w - 64+2, y + 2,    _NAME_W, h - 4, name,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
	}
	
	ui_DrawBar(x, y, w, 2, rgb565_button_background + 0xe7);
	ui_DrawBar(x, y + h -2, w, 2, rgb565_button_background + 0xe7);
	ui_DrawBar(x, y, 2, h, rgb565_button_background + 0xe7);
	ui_DrawBar(x + w - 2, y, 2, h, rgb565_button_background + 0xe7);
}

static void MainWin_UpdateViewData_Master (char *name, char *unit, char *data)			
{
	char buf[16] = {0};//名称数组
	u16 fcolor;
	snprintf(buf, sizeof(buf), "监测因子:%s", name);
	//报警
//	if(gas_GetModuleAlarmState() == evda_Class1)
//		fcolor = rgb565_draw_orange;
//	else if(gas_GetModuleAlarmState() == evda_Class2)
//		fcolor = rgb565_draw_red;
//	else
	fcolor = rgb565_digital_front;
	if ( data )
		
		ui_show_text_ex ( 0, 60, 240, 150, data,   efsize_64,
										fcolor, rgb565_button_background, ( eat_hcenter | eat_vcenter ), 0 );

	if ( unit )
	{
		ui_show_lable ( 0, 30, 320, 30, buf,   efsize_24,
										rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
		ui_show_lable ( 240, 60, 80, 150, unit,   efsize_24,
										rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
	}
}



static void MainWin_UpdateViewData_LeftName(u16 x, u16 y, u16 w, u16 h, char *name, char *unit, char *data)
{
    u16 fsize = 24 * 6;
	// 计算可容纳的字体长度（6=单个字符的像素宽度系数）
	u16 flen = (w - _UNIT_W - 4) / 6;
	u16 fcolor;
	// 若区域足够大（flen>32 且 高度>64）→ 用64号大字体（醒目显示数值）
	if(flen > 32 && h > 64)
		fsize = efsize_64;
	else
		fsize = efsize_24;

	
	//报警
//	if(gas_GetModuleAlarmState() == evda_Class1)
//		fcolor = rgb565_draw_orange;
//	else if(gas_GetModuleAlarmState() == evda_Class2)
//		fcolor = rgb565_draw_red;
//	else
		fcolor = rgb565_digital_front;
		
	if ( data )
		ui_show_text_ex ( x + _UNIT_W + 2, y + 2, w - _UNIT_W - 4, h - 4, data,   fsize,
		                fcolor, rgb565_button_background, ( eat_hleft | eat_vcenter ), 0 );

	if ( unit )
	{
		ui_show_lable ( x + 2, y + 2,    _NAME_W, (h - 4) / 2, name,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
		ui_show_lable ( x + 2, y + h / 2, _UNIT_W, (h - 4) / 2, unit,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
	}
	else
	{
		ui_show_lable ( x + 2, y + 2,    _NAME_W, h - 4, name,   efsize_24,
		                rgb565_lable_text, rgb565_button_background, ( eat_hleft | eat_vcenter ) );
	}
	ui_DrawBar(x, y, w, 2, rgb565_button_background + 0xe7);
	ui_DrawBar(x, y + h -2, w, 2, rgb565_button_background + 0xe7);
	ui_DrawBar(x, y, 2, h, rgb565_button_background + 0xe7);
	ui_DrawBar(x + w - 2, y, 2, h, rgb565_button_background + 0xe7);
}


//测试界面
static void ViewData_1(char *name, char *unit, char *data1,char *data2, u8 drawback)
{
	if(WinMainStart == 1){
		MainWin_UpdateViewData_Master (name, unit, data1);
	}
	else if(WinMainStart==2)
	{
		MainWin_UpdateViewData ( 0, 30 * 1, 320, 30*3, name, unit, data1, drawback);
	  MainWin_UpdateViewData ( 0, 30 * 4, 320, 30*1, "AD", 0, data2, drawback );
	}
	else
	{
		MainWin_UpdateViewData ( 0, 30 * 1, 320, 30*3, name, unit, data1, drawback);
	}
}

static void ViewData_3(u8 drawback)
{
	char string[16];
	
	if(WinMainStart == 2)
	{
		snprintf ( string, sizeof ( string ), "%.2f", Sensor_SHT31.temper );
		MainWin_UpdateViewData ( 0, 30 * 5, 160, 30 * 1, "T", 0, string, drawback );				//湿度
		snprintf ( string, sizeof ( string ), "%.2f", Sensor_SHT31.humidity );
		MainWin_UpdateViewData ( 0, 30 * 6, 160, 30 * 1, "H", 0, string, drawback );			//温度
	}
	else if(WinMainStart==3)																																//测试用
	{
		snprintf ( string, sizeof ( string ), "11111" );
		MainWin_UpdateViewData_LeftName ( 0, 30 * 4, 160, 30 * 1, "零AD", 0, string);				//
		snprintf ( string, sizeof ( string ), "22222");
		MainWin_UpdateViewData_LeftName ( 0, 30 * 5, 160, 30 * 1, "目浓", 0, string);			//
		snprintf ( string, sizeof ( string ), "33333" );
		MainWin_UpdateViewData_LeftName ( 0, 30 * 6, 160, 30 * 1, "目AD", 0, string);			//
	}
}

static void ViewData_4(u8 drawback)
{
	char string[16];
	if(WinMainStart == 2){
		snprintf ( string, sizeof ( string ), "%d", gas_Channel[1].gas_Channel_AD_Real );
		MainWin_UpdateViewData ( 160, 30 * 5, 160, 30 * 1, "AD2", 0, string, drawback );			//AD
		snprintf ( string, sizeof ( string ), "%.2f", MS5607.Pressure );
		MainWin_UpdateViewData ( 160, 30 * 6, 160, 30 * 1, "P", 0, string, drawback );	
	}
	else if(WinMainStart==3)
	{
		snprintf(string, sizeof(string), "%s", GetSoftwareVersion());
		MainWin_UpdateViewData_LeftName ( 160, 30 * 4, 160, 30 * 1, "Ver", 0, string);				//
		snprintf ( string, sizeof ( string ), "%03d",gSystemData.u8MoudleID);
		MainWin_UpdateViewData_LeftName ( 160, 30 * 5, 160, 30 * 1, "ID ", 0, string);			//
		snprintf(string, sizeof(string), "%s", gSystemData.name);
		MainWin_UpdateViewData_LeftName ( 160, 30 * 6, 160, 30 * 1, "SN ", 0, string);			//
	}
}

//创建控件
static void MainWin_Init ( void )
{
	MainWindow.tick.step = 0;
	MainWindow.tick.period = 1000;
	MainWindow.tick.lasttick = HAL_GetTick();

	WinMainStart = 1;
	UpdateTimeTick = 0;
}

static void MainWin_Paint ( void )
{
	ui_DrawBar(0, 30, 320, 30*6, MainWindow.color.bcolor);
	
	MainWin_UpdateTitleBar ( "" );
	MainWin_UpdateTimeBar();
	
	if(WinMainStart==1)
	{
		MainWin_UpdateStatusBar ( 0, 0, "菜单" );
	}
	else if(WinMainStart==2)
	{
		ui_show_lable ( 0, 240 - 30, 320, 30, "按下POWER按键设备重启", efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );
	}
	else
	{
		ui_show_lable ( 0, 240 - 30, 320, 30, "按下TEST按键返回主界面", efsize_24, rgb565_lable_text, rgb565_title_background, ( eat_hcenter | eat_vcenter ) );
	}
//	ViewData_1 ( (char*)g_ViewData_Name[  gCalidata[0].Name ], 
//		(char*)g_ViewData_Unit[  gCalidata[0].Unit ], "0.0","0", 1 );		//TVOC
	ViewData_1("H2","%LEL", "0.0","0", 1 );

	ViewData_3 (1);			//DA

	ViewData_4 (1);			//AD
}


static void MainWin_Update ( void )
{
	char string[16];
	char string2[16];
	
//	ui_format_string(string, sizeof ( string ), (u32*)&fSensorUnitValue[0], gas_UnitToFormat(gCalidata[0].Unit), 0);

	snprintf ( string, sizeof ( string ), "%d", gas_Channel[0].gas_Channel_AD_Real );//测试用
	snprintf ( string2, sizeof ( string2 ), "%d", gas_Channel[0].gas_Channel_AD_Real );

	ViewData_1 ( 0, 0, string,string2, 0 );		//TVOC
	ViewData_3 (0);				//addr
	if(WinMainStart!=3)
	{
		ViewData_4 (0);			//AD
	}
}


static void MainWin_TimerCallback ( void )
{
	MainWindow.tick.newtick = HAL_GetTick();

	if ( MainWindow.tick.newtick - MainWindow.tick.lasttick > MainWindow.tick.period )
	{
		MainWindow.tick.lasttick = MainWindow.tick.newtick;
		MainWin_Update();
	}
}


//事件回调函数
static void MainWin_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		MainWin_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;

		MainWin_Paint();
	}
	else if ( win->event.event & ewm_update )
	{
		ui_widget_update ( ( ui_widgets_t * ) win );
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_mute:
				if(AuthCheck()&&WinMainStart != 1)
				{
					HAL_NVIC_SystemReset();						//重启
				}
				break;
			case ewk_home:	
			case ewk_rok:
			case ewk_ok:
				if(g_SelAuth_id==0)
				{
					MainMenu_Create_N(win);
				}
				else if(g_SelAuth_id==1)
				{
					MainMenu_Create_A(win);
				}
				else
				{
					MainMenu_Create ( win );
				}
				break;

			case ewk_test:
				if(AuthCheck())
				{
					if(WinMainStart != 2)
					{
						WinMainStart = 2;
					}
					else
						WinMainStart = 1;
					MainWin_Paint();
				}
				else
				{
					if(WinMainStart != 3)
					{
						WinMainStart = 3;
					}
					else
						WinMainStart = 1;
					MainWin_Paint();
				}
				break;

			case ewk_C:
				SelAuth_Create(win, win);
				break;
		}

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_time )
	{
		MainWin_TimerCallback();
	}

	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}

/*
* 创建第一个窗口
*/
void MainWin_Create ( void *parg )
{
	( void ) parg;
	Pape_Flag=0;
	ui_window_create ( ( ui_window_t * ) parg, &MainWindow, MainWin_Callback );
	MainWindow.event.event |= ewm_time;
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &MainWindow );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
