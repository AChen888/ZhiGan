/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_Calib.h"
#include "App\appConfig.h"
#include "App\Gas\gas.h"
#include "rtc.h"
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

static ui_window_t WinZero =
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
static u16 CalibTimeout;

//static u16 CalibClass;
#define CALIB_CLASS_MAX			1
//static const char* CalibClassName[CALIB_CLASS_MAX] = {"零点"};//, "二级"};
static float fSpanConc;
static u32 adResult;

#define CONC_FORMAT		(gCalidata[g_SelectChannel].Format)
#define CONC_UNIT		(char*)(g_ViewData_Unit[ gCalidata[g_SelectChannel].Unit ])
#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[g_SelectChannel].Name ])



static u16 ItemSelected = 0;
#define LINE_TOTAL		4
static u8 ItemList[LINE_TOTAL];
//static u8 ItemModified;
//static u8 CalibPointer;
//static u8 CalibStart;
//static u16 DataFormat;
//static u16 DataUnit;



//创建控件
static void Win_Init ( void )
{
	char string[32];
	CalibTimeout = 0;
//	CalibClass = 0;
//	ItemModified = 0;
	
	WinZero.sublist = 0;
	WinZero.tick.step = 0;
	WinZero.tick.period = 999;
	WinZero.tick.lasttick = HAL_GetTick();

	
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;
	fSpanConc = gCalidata[g_SelectChannel].fZeroConc;
	adResult = gCalidata[g_SelectChannel].u32ZeroAD;

//	CalibPointer = SENSOR_ZERO_CALI + (g_SelectChannel * SENSOR_CALIB_TYPE_MAX);
//	CalibStart = 0;
	
	snprintf(string, sizeof(string), "零点校准-%s", CONC_NAME);
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");

//	DataUnit = gCalidata[g_SelectChannel].Unit % evdu_End;
//	DataFormat = gas_UnitToFormat(DataUnit);
}

#define LINE_Y(n)		(24 + 32 * n)

#define LINE_H			(31)
#define LABLE_W			(320)
#define DATA_W			(12*8)
#define UNIT_W			(12*5)

#define LINE_Y(n)		(24 + 32 * n)
#define DATA_X			(160)
#define UNIT_X			(320 - UNIT_W)


static void Win_Paint ( void )
{
	//char string[32];
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "零点浓度", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, "零点AD值", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(2), LABLE_W, LINE_H, "实时AD值", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(3), LABLE_W, LINE_H, "设置",    efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(4), LABLE_W, LINE_H, "实时浓度", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));

	

	ui_show_lable(320 - DATA_W, LINE_Y(0), DATA_W, LINE_H, "ppm", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));
	ui_show_lable(320 - DATA_W, LINE_Y(4), DATA_W, LINE_H, "ppm", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));
	ui_show_lable(160, LINE_Y(3), DATA_W, LINE_H, "确认", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
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
		bcolor = rgb565_grid_line1;
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

static void ShowLine(u16 line, u8 drawback)
{
	char string[32];
	Local_BinTimeTypeDef*	time;
	u16 bcolor;

	bcolor = SelectColor(line);	
	
	switch(line)
	{
		case 0://零点浓度
			ui_format_string(string, sizeof ( string ), (u32*)&fSpanConc, edf_float_23, 0);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case 1://零点AD值
			snprintf(string, sizeof(string), "%d", adResult);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 2://实时AD值
			snprintf(string, sizeof(string), "%d", g_AdcResult);
			ui_show_text_ex(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ), drawback);
			break;
		case 3://保存
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, "确认", efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 4://实时浓度
			ui_format_string(string, sizeof ( string ), (u32*)&fSensorRTValue[g_SelectChannel], edf_float_23, 0);
			ui_show_text_ex(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, rgb565_grid_line2, ( eat_hleft | eat_vcenter ), drawback);		
			break;
		case 5:	//最后修改时间
			time = (void*)gCalidata[g_SelectChannel].LastZeroTime;
			snprintf(string, sizeof(string), "%02d-%02d-%02d %02d:%02d:%02d", time->Year, time->Month, time->Day, time->Hours, time->Minutes, time->Seconds);
			ui_show_lable(0, LINE_Y(5), LABLE_W, LINE_H, string, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
			break;
	}
	
}


static void Win_ModifyPaint ( void )
{
	ShowLine(0, 1);
	ShowLine(1, 1);
	ShowLine(2, 1);
	ShowLine(3, 1);
	ShowLine(4, 1);
	ShowLine(5, 1);
}

static void Win_UpdatePaint ( void )
{
	ShowLine(2, 0);
	ShowLine(4, 0);
}


static void Win_Timer ( void )
{
	//	char string[16];

	WinZero.tick.newtick = HAL_GetTick();

	if ( WinZero.tick.newtick - WinZero.tick.lasttick > WinZero.tick.period )
	{
		WinZero.tick.lasttick = WinZero.tick.newtick;

		Win_UpdatePaint();
	}
}

//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;
	float temp;

	if ( win->event.event & ewm_init )
	{
		Win_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
		win->event.event |= ewm_time;
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
		if(CalibTimeout == 0)
		{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//
						if(AuthCheck() == 0)
						{
							ui_show_lable(160, LINE_Y(ItemSelected), DATA_W, LINE_H, "没有权限", efsize_24, rgb565_draw_yellow, rgb565_win_background, ( eat_hleft | eat_vcenter ));
							osDelay(1000);
							break;
						}
						switch(ItemSelected)
						{
							case 0:		//目标浓度
								g_Digital.fdata = fSpanConc;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, CONC_FORMAT, 160, LINE_Y(0), DATA_W, LINE_H, efsize_24);
								break;
							
							case 1:		//目标AD值
								g_Digital.udata = adResult;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_5, 160, LINE_Y(1), DATA_W, LINE_H, efsize_24);
								break;

							case 2:		//实时AD值
								if(u32RT_ADFIFO[4]>0)
								{
								adResult =(u32RT_ADFIFO[0]+u32RT_ADFIFO[1]+u32RT_ADFIFO[2]+u32RT_ADFIFO[3]+u32RT_ADFIFO[4])/5;
								}
								else
								{
								adResult = g_AdcResult;
								}
								ui_show_text_ex(160, LINE_Y(1), DATA_W, LINE_H, "实时AD值", efsize_24, rgb565_digital_front, rgb565_focused_background, ( eat_hleft | eat_vcenter ), 0);
								osDelay(1000);
								Win_ModifyPaint();
								break;

							case 3:		//保存
								gCalidata[SENSOR_CHANNEL1].fZeroConc = fSpanConc;						//设置目标点浓度
								gCalidata[SENSOR_CHANNEL1].u32ZeroAD =   adResult;	
//								log_trace("set gCalidata[%d].fZeroConc = %8.3f\r\n", SENSOR_CHANNEL1, gCalidata[SENSOR_CHANNEL1].fZeroConc);
//								log_trace("set gCalidata[%d].u32ZeroAD = %u\r\n", SENSOR_CHANNEL1, gCalidata[SENSOR_CHANNEL1].u32ZeroAD);
								SetSensorSpanRatio ( SENSOR_CHANNEL1 );		//计算灵敏度 
								Local_BinTimeTypeDef* time = (void*)gCalidata[SENSOR_CHANNEL1].LastZeroTime;
								*time = gDateTime;
								g_SaveParamFlag |= SAVE_CALIDATA_BIT | SAVE_SENSITIVITY_BIT;
		if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}							
								ui_show_lable(160, LINE_Y(3), DATA_W, LINE_H, "成功", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
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

					break;

				case ewk_right:
			case ewk_rdown:
			case ewk_down:						//
					ItemList[ItemSelected] = 1;
					ItemSelected++;
					if(ItemSelected >= LINE_TOTAL)
						ItemSelected = 0;
					ItemList[ItemSelected] = 2;

					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					ui_widget_go_prev_win ( win->prev );		//校准中不能推出界面
					break;

			}
			
			Win_ModifyPaint();
		}

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;
			switch(ItemSelected)
			{
				case 0:				//目标浓度
					fSpanConc = g_Digital.fdata;
					break;
				case 1:				//目标AD值
					adResult = g_Digital.udata;
					break;
			}
		}
		Win_ModifyPaint();
		MainWin_UpdateStatusBar("返回", "->", "修改");
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_time )
	{
		Win_Timer();
		//win->event.event &= ~ewm_time;		//周期运行
	}

	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
	(void)temp;
}


/*
* 创建第一个窗口
*/
void WinZero_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinZero, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinZero );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
