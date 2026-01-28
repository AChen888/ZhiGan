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

static ui_window_t WinStore =
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
#define LINE_TOTAL		4
static u8 ItemList[LINE_TOTAL];

static const char* OnOff[2] = {"关闭", "开启"};
static const char* StoreType[2] = {"定时记录", "报警记录"};

static uint8_t RecordEnable;	//开启存储
static uint8_t RecordType;		//0定时触发，1报警触发
static uint32_t RecordPeriod;	//存储周期 0-1分钟，1-5分钟，2-10分钟



//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinStore.sublist = 0;
	WinStore.tick.step = 0;
	WinStore.tick.period = 999;
	WinStore.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "存储设置");
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");


	//gAlarmParam
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;

	RecordEnable = (gSystemData.RecordEnable == 0) ? 0:1;
	RecordType     = 0;//(gSystemData.RecordType == 0) ? 0:1;
	RecordPeriod = 60;//gSystemData.RecordPeriod;
}

#define LINE1_Y			(24 + 32 * 0)
#define LINE2_Y			(24 + 32 * 1)
#define LINE3_Y			(24 + 32 * 2)
#define LINE4_Y			(24 + 32 * 3)
#define LINE5_Y			(24 + 32 * 4)
#define LINE6_Y			(24 + 32 * 5)
#define LINE_Y(n)		(24 + 32 * n)

#define LINE_H			(31)
#define LABLE_W			(320)
#define DATA_W			(160 * 2 / 3)
#define UNIT_W			(160 * 1 / 3)



static void Win_Paint ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable(0, LINE1_Y, LABLE_W, LINE_H, "开启存储", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE2_Y, LABLE_W, LINE_H, "存储方式", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE3_Y, LABLE_W, LINE_H, "存储周期", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE4_Y, LABLE_W, LINE_H, "设置",    efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));

	ui_show_lable(320 - UNIT_W, LINE3_Y, UNIT_W, LINE_H, "s", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hright | eat_vcenter ));
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

static void ShowLine(u16 line)
{
	char string[32];
	u16 bcolor;

	bcolor = SelectColor(line);	
	
	switch(line)
	{
		case 0:
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, (char*)OnOff[RecordEnable % 2], efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case 1:
			
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, (char*)StoreType[RecordType % 2], efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 2:
			ui_format_string(string, sizeof ( string ), (u32*)&RecordPeriod, edf_uint_3, 0);
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case 3:
			ui_show_lable(160, LINE_Y(line), DATA_W, LINE_H, "保存", efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
	}
}

static void Win_ModifyPaint ( void )
{
	u16 i;
	
	for(i = 0; i < LINE_TOTAL; i++)
		ShowLine(i);
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
							case 0:		//开启
								if(RecordEnable)
									RecordEnable = 0;
								else
									RecordEnable = 1;
								break;
							case 1:		//方式
//								if(RecordType)
									RecordType = 0;
//								else
//									RecordType = 1;
								break;
							case 2:		//周期
//								g_Digital.udata = RecordPeriod;
//								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, 160, LINE3_Y, DATA_W, LINE_H, efsize_24);
								break;
							case 3:		//保存
								gSystemData.RecordEnable = (RecordEnable == 0) ? 0:1;
								gSystemData.RecordType     = (RecordType == 0) ? 0:1;
								gSystemData.RecordPeriod = RecordPeriod;
								g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
									if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
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
				case 2:		//周期
					RecordPeriod = g_Digital.udata;
					break;
			}
		}
		
		Win_ModifyPaint();
		MainWin_UpdateStatusBar("返回", "->", "修改");
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
void WinStore_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinStore, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinStore );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
