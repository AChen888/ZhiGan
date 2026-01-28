/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "App\appConfig.h"
/*********************
 *      DEFINES
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

static ui_window_t WinAlarm =
{
	.pos.x = 0,
	.pos.y = 24,
	.pos.width = 320,
	.pos.height = 240 - 24 * 2,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static u16 ItemSelected = 0;
#define LINE_TOTAL		6
static u8 ItemList[LINE_TOTAL];

static const char* OnOff[2] = {"关闭", "开启"};

//static float fData;
static u16 DataFormat;
static u16 DataUnit;

#define CONC_FORMAT		(gCalidata[g_SelectChannel].Format)
#define CONC_UNIT		(char*)(g_ViewData_Unit[ gCalidata[g_SelectChannel].Unit ])
#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[g_SelectChannel].Name ])

/*
typedef struct {
    float HighLimit;//高报警限值
    float LowLimit;//低报警限值
    uint16_t ActDelay;	//报警动作延时，防止频繁启动
    uint16_t HoldTime;//报警持续时间
    uint8_t Enable;//是否使能报警
    //    uint8_t already_save;//是否已经保存
}c_AlarmParam_t;

*/

static c_AlarmParam_t Alarm;

//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinAlarm.sublist = 0;
	WinAlarm.tick.step = 0;
	WinAlarm.tick.period = 999;
	WinAlarm.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "报警设置-%s", CONC_NAME);
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");


	//gAlarmParam
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;
	
	Alarm = gAlarmParam[g_SelectChannel];
	Alarm.Class1Limit = gas_PPMConverTo (gAlarmParam[g_SelectChannel].Class1Limit, gCalidata[g_SelectChannel].Unit);
	Alarm.Class2Limit = gas_PPMConverTo (gAlarmParam[g_SelectChannel].Class2Limit, gCalidata[g_SelectChannel].Unit);
	DataUnit = gCalidata[g_SelectChannel].Unit % evdu_End;
	DataFormat = gCalidata[g_SelectChannel].Format;
}

#define LINE1_Y			(24 + 32 * 0)
#define LINE2_Y			(24 + 32 * 1)
#define LINE3_Y			(24 + 32 * 2)
#define LINE4_Y			(24 + 32 * 3)
#define LINE5_Y			(24 + 32 * 4)
#define LINE6_Y			(24 + 32 * 5)

#define LINE_H			(31)
#define LABLE_W			(320)
#define DATA_W			(12*8)
#define UNIT_W			(12*5)

#define LINE_Y(n)		(24 + 32 * n)
#define DATA_X			(160)
#define UNIT_X			(320 - UNIT_W)




static void Win_Paint ( void )
{
	char string[32];
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable(0, LINE1_Y, LABLE_W, LINE_H, "开启报警", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE2_Y, LABLE_W, LINE_H, "一级报警", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE3_Y, LABLE_W, LINE_H, "二级报警", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE4_Y, LABLE_W, LINE_H, "动作延时", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE5_Y, LABLE_W, LINE_H, "持续时间", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE6_Y, LABLE_W, LINE_H, "设置",    efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));

	
	snprintf(string, sizeof(string), "%s", g_ViewData_Unit[DataUnit]);
	ui_show_lable(320 - DATA_W, LINE2_Y, DATA_W, LINE_H, string, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hright | eat_vcenter ));
	
	snprintf(string, sizeof(string), "%s", g_ViewData_Unit[DataUnit]);
	ui_show_lable(320 - DATA_W, LINE3_Y, DATA_W, LINE_H, string, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));

	ui_show_lable(320 - UNIT_W, LINE4_Y, UNIT_W, LINE_H, "s", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hright | eat_vcenter ));
	ui_show_lable(320 - UNIT_W, LINE5_Y, UNIT_W, LINE_H, "s", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));	
}

static u16 SelectColor(u16 index)
{
	u16 bcolor;
	
	if(ItemList[index % LINE_TOTAL] == 2)
	{	
		bcolor = rgb565_focused_background;
	}
	else if(ItemList[index % LINE_TOTAL] == 1)
	{
		ItemList[index % LINE_TOTAL] = 0;
	}
	
	if(ItemList[index % LINE_TOTAL] != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line2;
		else
			bcolor = rgb565_grid_line1;
	}
	
	return bcolor;
}

static void ShowLine1(void)
{
	u16 bcolor;
	
	bcolor = SelectColor(0);
	ui_show_lable(160, LINE1_Y, DATA_W, LINE_H, (char*)OnOff[Alarm.Enable % 2], efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));		
}

static void ShowLine2(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(1);
	ui_format_string(string, sizeof ( string ), (u32*)&Alarm.Class1Limit, DataFormat, 0);
	ui_show_lable(160, LINE2_Y, DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
}

static void ShowLine3(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(2);
	ui_format_string(string, sizeof ( string ), (u32*)&Alarm.Class2Limit, DataFormat, 0);
	ui_show_lable(160, LINE3_Y, DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
}

static void ShowLine4(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(3);
	snprintf(string, sizeof(string), "%d", Alarm.ActDelay);
	ui_show_lable(160, LINE4_Y, DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
}

static void ShowLine5(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(4);
	snprintf(string, sizeof(string), "%d", Alarm.HoldTime);
	ui_show_lable(160, LINE5_Y, DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
}

static void ShowSaved(u8 ok)
{
	u16 bcolor;
	char* text = "保存";

	if(ok)
		text = "成功";
	
	bcolor = SelectColor(5);
	ui_show_lable(160, LINE6_Y, DATA_W, LINE_H, text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
}


static void Win_ModifyPaint ( void )
{
	ShowLine1();
	ShowLine2();
	ShowLine3();
	ShowLine4();
	ShowLine5();
	ShowSaved(0);
}


static void Win_Timer ( void )
{
	
}

//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		Win_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		Win_Paint();
		Win_ModifyPaint();
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//二级
						switch(ItemSelected)
						{
							case 0:		//开启报警
								if(Alarm.Enable)
									Alarm.Enable = 0;
								else
									Alarm.Enable = 1;
								break;
							case 1:		//一级报警
								g_Digital.fdata = Alarm.Class1Limit;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, DataFormat, 160, LINE2_Y, DATA_W, LINE_H, efsize_24);
								break;
							case 2:		//二级报警
								g_Digital.fdata = Alarm.Class2Limit;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, DataFormat, 160, LINE3_Y, DATA_W, LINE_H, efsize_24);
								break;
							case 3: 	//动作延时
								g_Digital.udata = Alarm.ActDelay;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE4_Y, DATA_W, LINE_H, efsize_24);
								break;
							case 4: 	//持续时间
								g_Digital.udata = Alarm.HoldTime;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, 160, LINE5_Y, DATA_W, LINE_H, efsize_24);
								break;
							case 5:		//保存
								gAlarmParam[g_SelectChannel] = Alarm;
								gAlarmParam[g_SelectChannel].Class1Limit = gas_ConverToPPM (Alarm.Class1Limit, gCalidata[g_SelectChannel].Unit);
								gAlarmParam[g_SelectChannel].Class2Limit = gas_ConverToPPM (Alarm.Class2Limit, gCalidata[g_SelectChannel].Unit);
								g_SaveParamFlag |= SAVE_ALARM_BIT;
									if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
								ShowSaved(1);
								osDelay(1000);
								break;
						}
					break;

					case ewk_up:
					case ewk_rup:
					case ewk_left:
					ItemList[ItemSelected] = 1;
					if(ItemSelected)
						ItemSelected--;
					else
						ItemSelected = LINE_TOTAL - 1;
					ItemList[ItemSelected] = 2;
//					Win_ModifyPaint();
					break;

			case ewk_right:
			case ewk_rdown:
			case ewk_down:						//
					ItemList[ItemSelected] = 1;
					ItemSelected++;
					if(ItemSelected >= LINE_TOTAL)
						ItemSelected = 0;
					ItemList[ItemSelected] = 2;
//					Win_ModifyPaint();
					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
						ui_widget_go_prev_win ( win->prev );		//校准中不能推出界面
					break;

			}
			
			Win_ModifyPaint();

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_time )
	{
		Win_Timer();
		//win->event.event &= ~ewm_time;		//周期运行
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		//g_EditData.format;
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;
			switch(ItemSelected)
			{
				case 0:		//开启报警
					break;
				case 1:		//一级报警
					Alarm.Class1Limit = g_Digital.fdata;
					break;
				case 2:		//二级报警
					Alarm.Class2Limit = g_Digital.fdata;
					break;
				case 3: 	//动作延时
					Alarm.ActDelay = g_Digital.udata;
					break;
				case 4: 	//持续时间
					Alarm.HoldTime = g_Digital.udata;
					break;
			}
		}
		Win_ModifyPaint();
		MainWin_UpdateStatusBar("返回", "->", "修改");
		//win->event.event |= ewm_paint;
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}


/*
* 创建第一个窗口
*/
void WinAlarm_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinAlarm, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinAlarm );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
