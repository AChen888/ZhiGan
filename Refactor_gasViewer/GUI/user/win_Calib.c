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

static ui_window_t WinCalib =
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
#define CALIB_CLASS_MAX			SENSOR_TARGET_CALI3		//扩展点数量
static u8 CalibSelect;		//校准的扩展点
static const char *CalibClassName[CALIB_CLASS_MAX] = {"1点", "2点", "3点"};
static float fSpanConc[CALIB_CLASS_MAX];
static u32 u32SpanAD[CALIB_CLASS_MAX];

#define CONC_FORMAT		(gCalidata[g_SelectChannel].Format)
#define CONC_UNIT		(char*)(g_ViewData_Unit[ gCalidata[g_SelectChannel].Unit ])
#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[g_SelectChannel].Name ])



static u16 ItemSelected = 0;
#define LINE_TOTAL		5
static u8 ItemList[LINE_TOTAL];
//static u8 ItemModified;
//static u8 CalibPointer;
//static u16 DataFormat;
//static u16 DataUnit;



//创建控件
static void Win_Init ( void )
{
	char string[32];
	CalibTimeout = 0;
//	CalibClass = 0;
//	ItemModified = 0;
	
	WinCalib.sublist = 0;
	WinCalib.tick.step = 0;
	WinCalib.tick.period = 999;
	WinCalib.tick.lasttick = HAL_GetTick();

	ItemSelected = 0;
	CalibSelect = 0;
	memset ( ItemList, 0, sizeof ( ItemList ) );
	ItemList[ItemSelected] = 2;
	
	fSpanConc[0] = gCalidata[g_SelectChannel].fSpanConc1;
	u32SpanAD[0] = gCalidata[g_SelectChannel].u32SpanAD1;
	fSpanConc[1] = gCalidata[g_SelectChannel].fSpanConc2;
	u32SpanAD[1] = gCalidata[g_SelectChannel].u32SpanAD2;
	fSpanConc[2] = gCalidata[g_SelectChannel].fSpanConc3;
	u32SpanAD[2] = gCalidata[g_SelectChannel].u32SpanAD3;
	
//	CalibPointer = SENSOR_TARGET_CALI1 + ( g_SelectChannel * SENSOR_CALIB_TYPE_MAX );

	
	snprintf(string, sizeof(string), "浓度校准-%s", CONC_NAME);
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");

//	DataUnit = gCalidata[g_SelectChannel].Unit % evdu_End;
//	DataFormat = gas_UnitToFormat ( DataUnit );
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
	char string[32];
	Local_BinTimeTypeDef*	time;

	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "目标点",   efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, "目标浓度", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(2), LABLE_W, LINE_H, "目标AD值", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(3), LABLE_W, LINE_H, "实时AD值", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(4), LABLE_W, LINE_H, "设置",    efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(5), LABLE_W, LINE_H, "实时浓度", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));

	ui_show_lable(320 - DATA_W, LINE_Y(1), DATA_W, LINE_H, "ppm", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));
	ui_show_lable(320 - DATA_W, LINE_Y(5), DATA_W, LINE_H, "ppm", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hright | eat_vcenter ));
//	ui_show_lable(160, LINE_Y(4), DATA_W, LINE_H, "确认", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));

	time = (void*)gCalidata[g_SelectChannel].LastConcTime;
	snprintf(string, sizeof(string), "保存 %02d%02d%02d%02d%02d%02d", time->Year, time->Month, time->Day, time->Hours, time->Minutes, time->Seconds);
	ui_show_lable(100, LINE_Y(4), 220, LINE_H, string, efsize_24, rgb565_digital_front, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));		
}

static u16 SelectColor ( u16 index )
{
	u16 bcolor;

	if ( ItemList[index % LINE_TOTAL] == 2 )
	{
		bcolor = rgb565_focused_background;
	}
	else if ( ItemList[index % LINE_TOTAL] == 1 )
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

	bcolor = SelectColor ( line );

	switch ( line )
	{
		case 0://目标点
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, (char*)CalibClassName[CalibSelect], efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		
		case 1://目标浓度
			ui_format_string(string, sizeof ( string ), (u32*)&fSpanConc[CalibSelect], edf_float_23, 0);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		
		case 2://目标AD值
			snprintf(string, sizeof(string), "%d", u32SpanAD[CalibSelect]);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		
		case 3://实时AD值
			snprintf(string, sizeof(string), "%d", g_AdcResult);
			ui_show_text_ex(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ), drawback);
			break;

		case 4://保存，最后修改时间
			//time = (void*)gCalidata[g_SelectChannel].LastConcTime;
			//snprintf(string, sizeof(string), "保存 %02d%02d%02d%02d%02d%02d", time->Year, time->Month, time->Day, time->Hours, time->Minutes, time->Seconds);
			ui_show_lable(100, LINE_Y(line), 60, LINE_H, "保存", efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		
		case 5://实时浓度
			ui_format_string(string, sizeof ( string ), (u32*)&fSensorRTValue[g_SelectChannel], edf_float_23, 0);
			ui_show_text_ex(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, rgb565_grid_line2, ( eat_hleft | eat_vcenter ), drawback);		
			break;
	}
	(void)time;
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
	ShowLine(3, 0);
	ShowLine(5, 0);
}


static void Win_Timer ( void )
{
	//	char string[16];

	WinCalib.tick.newtick = HAL_GetTick();

	if ( WinCalib.tick.newtick - WinCalib.tick.lasttick > WinCalib.tick.period )
	{
		WinCalib.tick.lasttick = WinCalib.tick.newtick;

		Win_UpdatePaint();
	}
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
		if ( CalibTimeout == 0 )
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
							case 0:		//目标点
								CalibSelect++;
								if(CalibSelect >= gSystemData.CaliPoints)
									CalibSelect = 0;
								break;
								
							case 1:		//目标浓度
								g_Digital.fdata = fSpanConc[CalibSelect];
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_float_23, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							
							case 2:		//目标AD值
								g_Digital.udata = u32SpanAD[CalibSelect];
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_5, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;

							case 3:		//实时AD值
								if(u32RT_ADFIFO[4]>0)
								{
								u32SpanAD[CalibSelect] =(u32RT_ADFIFO[0]+u32RT_ADFIFO[1]+u32RT_ADFIFO[2]+u32RT_ADFIFO[3]+u32RT_ADFIFO[4])/5;
								}
								else
								{
								u32SpanAD[CalibSelect] = g_AdcResult;
								}
								ui_show_text_ex(160, LINE_Y(ItemSelected), DATA_W, LINE_H, "实时AD值", efsize_24, rgb565_digital_front, rgb565_focused_background, ( eat_hleft | eat_vcenter ), 0);
								osDelay(1000);
								Win_ModifyPaint();
								break;
								
							case 4:		//保存
//								sensor_set_cali_data_conc(CalibPointer, fSpanConc[CalibSelect]);						
//								sensor_set_cali_data_ad(CalibPointer, u32SpanAD[CalibSelect]);	
								
								//设置目标点浓度
								if(CalibSelect == 0)
								{
									gCalidata[g_SelectChannel].fSpanConc1 = fSpanConc[0];
									gCalidata[g_SelectChannel].u32SpanAD1 = u32SpanAD[0];
									//log_trace("set gCalidata[%d].fSpanConc1 = %8.3f\r\n", g_SelectChannel, gCalidata[g_SelectChannel].fSpanConc1);
									//log_trace("set gCalidata[%d].u32SpanAD1 = %u\r\n", g_SelectChannel, gCalidata[g_SelectChannel].u32SpanAD1);
								}
								else if(CalibSelect == 1)
								{
									gCalidata[g_SelectChannel].fSpanConc2 = fSpanConc[1];
									gCalidata[g_SelectChannel].u32SpanAD2 = u32SpanAD[1];
									//log_trace("set gCalidata[%d].fSpanConc2 = %8.3f\r\n", g_SelectChannel, gCalidata[g_SelectChannel].fSpanConc2);
									//log_trace("set gCalidata[%d].u32SpanAD2 = %u\r\n", g_SelectChannel, gCalidata[g_SelectChannel].u32SpanAD2);
								}
								else if(CalibSelect == 2)
								{
									gCalidata[g_SelectChannel].fSpanConc3 = fSpanConc[2];
									gCalidata[g_SelectChannel].u32SpanAD3 = u32SpanAD[2];
									//log_trace("set gCalidata[%d].fSpanConc3 = %8.3f\r\n", g_SelectChannel, gCalidata[g_SelectChannel].fSpanConc3);
									//log_trace("set gCalidata[%d].u32SpanAD3 = %u\r\n", g_SelectChannel, gCalidata[g_SelectChannel].u32SpanAD3);
								}
								//计算灵敏度
								SetSensorSpanRatio ( SENSOR_CHANNEL1 );		
								
								Local_BinTimeTypeDef* time = (void*)gCalidata[g_SelectChannel].LastConcTime;
								*time = gDateTime;
								g_SaveParamFlag |= SAVE_CALIDATA_BIT | SAVE_SENSITIVITY_BIT;
										if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
								ui_show_lable(160, LINE_Y(4), DATA_W, LINE_H, "成功", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
								osDelay(1000);
								win->event.event |= ewm_paint;
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
		if ( g_Digital.udata != g_EditData.udata )
		{
			g_Digital.udata = g_EditData.udata;
			switch(ItemSelected)
			{
				case 1:				//目标浓度
					fSpanConc[CalibSelect] = g_Digital.fdata;
					break;
				case 2:				//目标AD值
					u32SpanAD[CalibSelect] = g_Digital.udata;
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
}


/*
* 创建第一个窗口
*/
void WinCalib_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinCalib, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinCalib );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
