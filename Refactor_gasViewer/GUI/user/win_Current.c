/**
 * @file	电流输出页面
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "win_save.h"
#include "win_current.h"

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

static ui_window_t WinCurrent =
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
u16 g_SelectChannel = 0;


static u16 ItemSelected = 0;
#define LINE_TOTAL		2
static u8 ItemList[LINE_TOTAL];

#define CONC_FORMAT		(gCalidata[0].Format)
#define CONC_UNIT		(char*)(g_ViewData_Unit[ gCalidata[0].Unit ])
#define CONC_NAME		(char*)(g_ViewData_Name[   gCalidata[0].Name ])

u8 out20mA;
u8 out4mA;
u8 Current_Open;

//创建控件
static void Win_Init ( void )
{
	char string[32];
	
	WinCurrent.sublist = 0;
	WinCurrent.tick.step = 0;
	WinCurrent.tick.period = 999;
	WinCurrent.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "电流输出");
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "下移", "修改");

	out4mA   = gCalidata[g_SelectChannel].Out_4mA;
	out20mA = gCalidata[g_SelectChannel].Out_20mA;
	Current_Open = gCalidata[g_SelectChannel].Current_Open;
	
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
	char string[32];
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, 			"4mA	校准",	efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, 			"20mA校准",		efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));

	ui_show_lable(UNIT_X, LINE_Y(0), UNIT_W, LINE_H, "mA",			efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(UNIT_X, LINE_Y(1), UNIT_W, LINE_H, "mA",			efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
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

static void ShowLine2(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(1);
//	ui_format_string(string, sizeof ( string ), (u32*)&out4mA, edf_uint_2, 0);
	snprintf(string, sizeof(string), "*%d", out4mA);
	ui_show_lable(DATA_X, LINE_Y(0), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
}


static void ShowLine3(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(2);
//	ui_format_string(string, sizeof ( string ), (u32*)&out20mA, edf_uint_2, 0);
	snprintf(string, sizeof(string), "*%d", out20mA);
	ui_show_lable(DATA_X, LINE_Y(1), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
}


static void Win_ModifyPaint ( void )
{
	ShowLine2();
	ShowLine3();
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
				case ewk_ok:						//
						if(AuthCheck() == 0)
						{
							if(ItemSelected)
							{
								ui_show_lable(160, LINE_Y(ItemSelected), DATA_W, LINE_H, "没有权限", efsize_24, rgb565_draw_yellow, rgb565_win_background, ( eat_hleft | eat_vcenter ));
								osDelay(1000);
							}
							break;
						}
						switch(ItemSelected)
						{
							case 1:
								g_Digital.udata = out4mA;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, DATA_X, LINE_Y(0), DATA_W, LINE_H, efsize_24);
								break;
							case 2:
								g_Digital.udata = out20mA;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, DATA_X, LINE_Y(1), DATA_W, LINE_H, efsize_24);
								break;
						}
					break;
						
				case ewk_up:
				case ewk_rup:
				case ewk_left:
					ItemList[ItemSelected] = 1;
					if(ItemSelected>1)
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
						ItemSelected = 1;
					ItemList[ItemSelected] = 2;
					break;
					
				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					if (Win_Save_Flag & (1<<1)) 
					{
						WinSave_Create(win);
					}
					else
					{
						ui_widget_go_prev_win(win->prev); // 返回上一窗口
					}
					break;
			}
			
			Win_ModifyPaint();

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		g_Digital.udata = g_EditData.udata;
		switch(ItemSelected)
		{
			case 1:		//
				out4mA = g_Digital.udata;
				Win_Save_Flag |= (1<<1);
				break;
			case 2:		//
				out20mA = g_Digital.udata;
				Win_Save_Flag |= (1<<1);
				break;
		}
	
		Win_ModifyPaint();
		MainWin_UpdateStatusBar("返回", "下移", "修改");
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
void WinCurrent_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinCurrent, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinCurrent );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
