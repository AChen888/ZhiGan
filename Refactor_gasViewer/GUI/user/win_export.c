/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "App\appConfig.h"
#include "rtc.h"
#include "App\Gas\gas.h"
//#include "Store\bsp_w25qxx_driver.h"
#include "win_export.h"
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

static ui_window_t WinExport =
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
static u8 MenuSelected = 0;		// 0：时间选择，1：记录显示

static Local_BinTimeTypeDef TimeSelected;
static u16 ItemSelected = 0;
static u16 ItemTotal = 7;
#define LINE_TOTAL		ItemTotal
static u8 ItemList[8];
static u8 ChannelSelected;
static u16 DataFormat;
//static u16 DataUnit;

#define CONC_FORMAT		(gCalidata[g_SelectChannel].Format)
#define CONC_UNIT		(char*)(g_ViewData_Unit[ gCalidata[g_SelectChannel].Unit ])
#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[g_SelectChannel].Name ])

//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinExport.sublist = 0;
	WinExport.tick.step = 0;
	WinExport.tick.period = 999;
	WinExport.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "打印数据-%s", CONC_NAME);
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");

	MenuSelected = 0;		//时间输入
	TimeSelected = gDateTime;			//当前时间
	ChannelSelected = 0;			//通道

	//DataUnit = gCalidata[g_SelectChannel].Unit % evdu_End;
	DataFormat = gCalidata[g_SelectChannel].Format;

	ItemTotal = 8;
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;
}

#define LINE_H			(24)
#define LINE_Y(n)		(24 + LINE_H * n)
#define LABLE_W			(320)
#define DATA_W			(160)

static void Win_Paint ( void )
{
	

	
	if(MenuSelected == 0)
	{
		// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
		ui_show_lable(16, LINE_Y(0), LABLE_W, LINE_H, "始" , efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
		ui_show_lable(16, LINE_Y(1), LABLE_W, LINE_H, "年"   , efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
		ui_show_lable(16, LINE_Y(2), LABLE_W, LINE_H, "月"   , efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
		ui_show_lable(16, LINE_Y(3), LABLE_W, LINE_H, "日"   , efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
		ui_show_lable(16, LINE_Y(4), LABLE_W, LINE_H, "时"   , efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
		ui_show_lable(16, LINE_Y(5), LABLE_W, LINE_H, "分"   , efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
//		if(AuthCheck()){
//		//ui_show_lable(16, LINE_Y(6), LABLE_W, LINE_H, "区间" , efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
//			ui_show_lable(160, LINE_Y(6), DATA_W, LINE_H, "区间", efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
//		}
		ui_show_lable(16, LINE_Y(6), LABLE_W, LINE_H, " " , efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
//		ShowLine2(0, 1);
//		ShowLine2(1, 1);
//		ShowLine2(2, 1);
//		ShowLine2(3, 1);
//		ShowLine2(4, 1);
//		ShowLine2(5, 1);
//		ShowLine2(6, 1);
	}
	
	else
	{
	}
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


static void ShowLine0(u16 line, u8 drawback)
{
	char string[32];
	//Local_BinTimeTypeDef*	time;
	u16 bcolor;

	bcolor = SelectColor(line);	
	
	switch(line)
	{
		case 0://
			snprintf(string, sizeof(string), "%d", ChannelSelected + 1);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 1://
			snprintf(string, sizeof(string), "%02d", TimeSelected.Year);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 2://
			snprintf(string, sizeof(string), "%02d", TimeSelected.Month);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 3://
			snprintf(string, sizeof(string), "%02d", TimeSelected.Day);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 4://
			snprintf(string, sizeof(string), "%02d", TimeSelected.Hours);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 5://
			snprintf(string, sizeof(string), "%02d", TimeSelected.Minutes);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 6:	//
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, "读取", efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
	}
	
}

static void Win_ModifyPaint ( void )
{
//	u32 index;

	if(MenuSelected == 0)
	{
		ShowLine0(0, 1);
		ShowLine0(1, 1);
		ShowLine0(2, 1);
		ShowLine0(3, 1);
		ShowLine0(4, 1);
		ShowLine0(5, 1);
		ShowLine0(6, 1);
		if(AuthCheck())
		ShowLine0(7, 1);
	}
	
}
	
//	else if(MenuSelected == 1)
//	{
//		Local_BinTimeTypeDef	time = TimeSelected;
//		//gas_SubMinutes(&time, 8);
//	//	index = Compute_CurrentMinutes(&time);		//计算分钟时间戳
//		index = Compute_CurrentMinutes(&time);
//		ShowLine1(0, index++);
//		ShowLine1(1, index++);
//		ShowLine1(2, index++);
//		ShowLine1(3, index++);
//		ShowLine1(4, index++);
//		ShowLine1(5, index++);
//		ShowLine1(6, index++);
//		ShowLine1(7, index++);
//		
//		分割-------------------------------------------------------------------------------------------------------------------------
//		gas_SubSecond(&time, 8);
//		
//		index = Compute_CurrentSecond(&time);

//		ShowLine1Onesecond(0, index++);
//		ShowLine1Onesecond(1, index++);
//		ShowLine1Onesecond(2, index++);
//		ShowLine1Onesecond(3, index++);
//		ShowLine1Onesecond(4, index++);
//		ShowLine1Onesecond(5, index++);
//		ShowLine1Onesecond(6, index++);
//		ShowLine1Onesecond(7, index++);
//      //printRecoed();
//		分割-------------------------------------------------------------------------------------------------------------------------
//	}
//	else{
//		
//		Win_Paint1();
//		
//		Win_Paint2();
//		
//		ShowLine2(0, 1);
//		ShowLine2(1, 1);
//		ShowLine2(2, 1);
//		ShowLine2(3, 1);
//		ShowLine2(4, 1);
//		ShowLine2(5, 1);
//		ShowLine2(6, 1);
//	
//	}
//}

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
		if(gSystemData.RecordEnable)		{
		Win_Paint();
		Win_ModifyPaint();
			}
		else {
			ui_show_lable((320- 240)/2, LINE_Y(2), 240, LINE_H*2, "没有记录", efsize_24, rgb565_focused_front, rgb565_focused_background, ( eat_hcenter | eat_vcenter ));
			osDelay(2000);
			ui_widget_go_prev_win ( win->prev );		//
			}
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//
				if(MenuSelected == 0) {
						switch(ItemSelected)	{
							case 1:		//年
								g_Digital.udata = TimeSelected.Year;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							case 2:		//月
								g_Digital.udata = TimeSelected.Month;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							case 3:		//日
								g_Digital.udata = TimeSelected.Day;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							case 4: 	//时
								g_Digital.udata = TimeSelected.Hours;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							case 5: 	//分
								g_Digital.udata = TimeSelected.Minutes;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, 160, LINE_Y(ItemSelected), DATA_W, LINE_H, efsize_24);
								break;
							case 6:		//保存

									if(TimeSelected.Month > 12 || TimeSelected.Month == 0)
										TimeSelected.Month = 12;
									
									if(TimeSelected.Day > 31 || TimeSelected.Day == 0)
										TimeSelected.Day = 31;
									
									if(TimeSelected.Hours > 23)
										TimeSelected.Hours = 23;
									
									if(TimeSelected.Minutes > 59)
										TimeSelected.Minutes = 59;

									MenuSelected = 1;	//记录列表
									ItemTotal = 8;
									ItemSelected = ItemTotal - 1;
								break;
							
							case 7: 	//区间
								if(AuthCheck()){
                   MenuSelected = 2;	//选取区间
								}
								break;
						
									
						}
					}
				else{
					
					}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					if(MenuSelected == 0) {
					ItemList[ItemSelected] = 1;
					if(ItemSelected)
						ItemSelected--;
					else
						ItemSelected = LINE_TOTAL - 1;
					ItemList[ItemSelected] = 2;
						}
					else {
//						gas_SubMinutes(&TimeSelected, 8);
						gas_SubSecond(&TimeSelected, 8);
						}

					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:						//
				if(MenuSelected == 0) {
						ItemList[ItemSelected] = 1;
						ItemSelected++;
						if(ItemSelected >= LINE_TOTAL)
							ItemSelected = 0;
						ItemList[ItemSelected] = 2;
					}
				else{
//					gas_AddMinutes(&TimeSelected, 8);
					gas_AddSecond(&TimeSelected, 8);
					}

					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					if(MenuSelected == 0) {
						ui_widget_go_prev_win ( win->prev );		//
					}
					else{
						MenuSelected = 0;
						ItemTotal = 7;
						ItemSelected = ItemTotal - 1;
						win->event.event |= ewm_paint;
					}
					break;

			}
			
			Win_ModifyPaint();

		win->event.event &= ~ewm_key;
	}
//	else if ( win->event.event & ewm_time )
//	{
//		Win_Timer();
//		//win->event.event &= ~ewm_time;		//周期运行
//	}
	
		else if ( win->event.event & ewm_timeOS )
	{
		Win_Timer();
		//win->event.event &= ~ewm_time;		//周期运行
	}
	
	
	else if ( win->event.event & ewm_update )
	{
	if(MenuSelected == 0) {
		//数据被编辑
		//g_EditData.format;
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;
			switch(ItemSelected)
			{
				case 1:		//
					TimeSelected.Year = g_Digital.udata;
					break;
				case 2:		//
					TimeSelected.Month = g_Digital.udata;
					if(TimeSelected.Month > 12 || TimeSelected.Month == 0)
						TimeSelected.Month = 12;
					break;
				case 3:		//
					TimeSelected.Day = g_Digital.udata;
					if(TimeSelected.Day > 31 || TimeSelected.Day == 0)
						TimeSelected.Day = 31;
					break;
				case 4:		//
					TimeSelected.Hours = g_Digital.udata;
					if(TimeSelected.Hours > 23)
						TimeSelected.Hours = 23;
					break;
				case 5:		//
					TimeSelected.Minutes = g_Digital.udata;
					if(TimeSelected.Minutes > 59)
						TimeSelected.Minutes = 59;
					break;
			}
		}
		
		win->event.event |= ewm_paint;
		}
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}



//void printRecoedSection(void)				//20230301
//{

//	
//	char string[64];
//	char datastr[32];					
//	//s_GasDataRecord record;
//	
////	MenuSelected = 0;		//时间输入
////	TimeSelected = gDateTime;			//当前时间
////	ChannelSelected = 0;			//通道

//	//DataUnit = gCalidata[g_SelectChannel].Unit % evdu_End;
//	//DataFormat = gCalidata[g_SelectChannel].Format;

//	
//	Local_BinTimeTypeDef ntime;
//	
//	RTC_TimeTypeDef GetTime;
//	RTC_DateTypeDef GetDate;

// 	HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &GetDate, RTC_FORMAT_BIN);
//	
//	ntime.Year    = GetDate.Year;
//	ntime.Month   = GetDate.Month;
//	ntime.Day    = GetDate.Date;
//	ntime.Hours   = GetTime.Hours;
//	ntime.Minutes = GetTime.Minutes;
//	ntime.Seconds = GetTime.Seconds;
//	
//			
//	//s_GasDataRecord record;
//	
//	recorddataonesecond oneseconddata;
//	
//	float value;
//	//u16 bcolor;
//	char* alarm;
//	u32 index;
//	u32 index1;
//	u32 index2;
//	u32 nindex;
//	
//	index1 = Compute_CurrentSecond(&ntime);
//	index2 = (Compute_CurrentSecond(&ntime)-86400);
//	
//		DataFormat = gCalidata[0].Format;
//		SEGGER_RTT_printf ( 0, "\r\n");
//		SEGGER_RTT_printf ( 0, "Pring record start!!\r\n");
//		for(index=index2;index<index1;index++)
//			{
//				
//				
//		//		(index % GAS_RECORD_TOTAL)<GAS_RECORD_TOTAL
//				
//			nindex = (index % GAS_RECORD_TOTAL);				//使用分钟数计算记录序号

//			if(gas_ReadRecordEx(nindex, oneseconddata.Data.buffer))	
//			{
//					bf_TimeStamp2LocalTime(oneseconddata.Record.MinuteStamp, &ntime);

//					switch(oneseconddata.Record.Alarm[ChannelSelected])
//					{
//							case evda_Class1:
//							alarm = "一级";                                     
//							break;
//							case evda_Class2:
//							alarm = "二级";
//							break;
//							default:
//							alarm = "OK";
//							break;
//					}
//					value = gas_PPMConverTo (oneseconddata.Record.Concentration[0], gCalidata[0].Unit);
//					ui_format_string(datastr, sizeof ( datastr ), (u32*)&value, DataFormat, 0);
//					snprintf(string, sizeof(string), "%02d%02d%02d %02d:%02d:%02d %s %s %.2f %.2f %u", ntime.Year, ntime.Month, ntime.Day, ntime.Hours, ntime.Minutes, ntime.Seconds,
//												datastr, alarm, (float)oneseconddata.Record.T/100, (float)oneseconddata.Record.H/100, oneseconddata.Record.AD0);
//		
//					SEGGER_RTT_printf ( 0, "%s\r\n",string);
//					
////			if(index == Compute_CurrentSecond(&ntime))
////			{break;
////			}
//				}
//			
//				
//			
//			}
//			SEGGER_RTT_printf ( 0, "Pring record end!!\r\n");

//}


void WinExport_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinExport, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinExport );		//很重要，一定要有这一句，第一个运行的窗口
}

/**********************
  **********************/