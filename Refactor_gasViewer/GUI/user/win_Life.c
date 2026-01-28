/**
 * @file	串口设置页面
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "usart.h"
#include "win_information.h"
#include "ui_config.h"
#include "win_save.h"


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

static ui_window_t WinLife =
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
#define LINE_TOTAL		1
static u8 ItemList[LINE_TOTAL];
uint16_t Use_Life;

//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinLife.sublist = 0;
	WinLife.tick.step = 0;
	WinLife.tick.period = 999;
	WinLife.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "寿命运维");
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "下移", "修改");
	
	Use_Life  = gSystemData.Life;
	
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;

}


#define LINE_H			(30)
#define LABLE_W			(320)
#define DATA_W			(12*8)
#define UNIT_W			(12*5)

#define LINE_Y(n)		(30 + 30 * n)
#define DATA_X			(160)
#define UNIT_X			(320 - UNIT_W)

static void Win_Paint ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "使用天数",	efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(UNIT_X, LINE_Y(0), UNIT_W, LINE_H, "天",	efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
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
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(0);
	snprintf(string, sizeof(string), "%03d", Use_Life);
	ui_show_lable(DATA_X, LINE_Y(0), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
}

static void Win_ModifyPaint ( void )
{
	ShowLine1();

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
							g_Digital.udata = Use_Life;
							ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, DATA_X, LINE_Y(0), DATA_W, LINE_H, efsize_24);
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
					if(Win_Save_Flag)
					{
						WinSave_Create(win);
					}
					else
					{
						ui_widget_go_prev_win ( win->prev );		//校准中不能推出界面
					}
					break;

			}
			
			Win_ModifyPaint();

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;
			switch(ItemSelected)
			{
				case 0:		//
					Use_Life = g_Digital.udata;
					Win_Save_Flag	|= (1<<0);
					break;
			}
		}
		MainWin_UpdateStatusBar("返回", "下移", "确认");
		Win_ModifyPaint();
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
void WinLife_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinLife, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinLife );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
