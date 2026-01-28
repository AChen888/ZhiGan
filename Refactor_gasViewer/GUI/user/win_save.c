/**
 * @file	串口设置页面
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "win_information.h"
#include "ui_config.h"
#include "win_save.h"
#include "win_main.h"
#include "win_life.h"
#include "win_current.h"
#include "win_usart.h"


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

static ui_window_t WinSave =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static u16 ItemSelected = 0;
#define LINE_TOTAL		2
static u8 ItemList[LINE_TOTAL];

//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinSave.sublist = 0;
	WinSave.tick.step = 0;
	WinSave.tick.period = 999;
	WinSave.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "保存设置");
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "下移", "修改");
	
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;
}


#define LINE_H			(90)
#define LABLE_W			(320)
#define DATA_W			(12*8)
#define UNIT_W			(12*5)

#define LINE_Y(n)		(30 + 90 * n)
#define DATA_X			(160)
#define UNIT_X			(320 - UNIT_W)

static void Win_Paint ( void )
{
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "保存数据",	efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hcenter | eat_vcenter ));
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, "取消保存",	efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hcenter | eat_vcenter ));
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

static void ShowSaved(u8 ok)
{
	u16 bcolor;
	char* text = "保存数据";

	if(ok)
		text = "保存成功";
	
	bcolor = SelectColor(0);
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, text, efsize_24, rgb565_lable_text, bcolor, ( eat_hcenter | eat_vcenter ));
}

static void ShowSaved1(u8 ok)
{
	u16 bcolor;
	char* text = "取消保存";

	if(ok)
		text = "取消成功";
	
	bcolor = SelectColor(1);
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, text, efsize_24, rgb565_lable_text, bcolor, ( eat_hcenter | eat_vcenter ));
}

static void Win_ModifyPaint ( void )
{
	ShowSaved(0);
	ShowSaved1(0);
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
				case ewk_ok:
					switch(ItemSelected)
					{
						case 0:
							if(Win_Save_Flag & (1<<0))
							{
								Win_Save_Flag &= ~(1<<0);
								gSystemData.Life = Use_Life;
								g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
								if ( g_SaveParamFlag )		//保存参数到fram
								{
									System_SaveCommonData ( g_SaveParamFlag );
									g_SaveParamFlag = 0;
								}
							}
							else if(Win_Save_Flag & (1<<1))
							{
								Win_Save_Flag &= ~(1<<1);
								gCalidata[g_SelectChannel].Out_4mA = out4mA;
								gCalidata[g_SelectChannel].Out_20mA = out20mA;
								gCalidata[g_SelectChannel].Current_Open = Current_Open;
								g_SaveParamFlag |= SAVE_CALIDATA_BIT;
								if ( g_SaveParamFlag )		//保存参数到fram
								{
									System_SaveCommonData ( g_SaveParamFlag );
									g_SaveParamFlag = 0;
								}
							}
							else if(Win_Save_Flag & (1<<2))
							{
								Win_Save_Flag &= ~(1<<2);
								gCalidata[g_SelectChannel].Baud = SysBaud;
//								Usart_Chage_Flag=1;
								g_SaveParamFlag |= SAVE_CALIDATA_BIT;
								if ( g_SaveParamFlag )		//保存参数到fram
								{
									System_SaveCommonData ( g_SaveParamFlag );
									g_SaveParamFlag = 0;
								}
								
								g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
								if ( g_SaveParamFlag )		//保存参数到fram
								{
									System_SaveCommonData ( g_SaveParamFlag );
									g_SaveParamFlag = 0;
								}
							}
							ShowSaved(1);
							osDelay(1000);
							ui_widget_go_prev_win ( win->prev );
							break;
						case 1:
							if(Win_Save_Flag & (1<<0))
							{
								Win_Save_Flag &= ~(1<<0);
							}
							else if(Win_Save_Flag & (1<<1))
							{
								Win_Save_Flag &= ~(1<<1);
							}
							else if(Win_Save_Flag & (1<<2))
							{
								Win_Save_Flag &= ~(1<<2);
							}
							ShowSaved1(1);
							osDelay(1000);
							ui_widget_go_prev_win ( win->prev );
							break;
					}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					ItemList[ItemSelected] = 1;
					if(ItemSelected>0)
					{
						ItemSelected--;
					}
					else
					{
						ItemSelected = LINE_TOTAL - 1;
					}
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
					break;
			}
			Win_ModifyPaint();
		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		MainWin_UpdateStatusBar("返回", "下移", "确认");
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
void WinSave_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinSave, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSave );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
