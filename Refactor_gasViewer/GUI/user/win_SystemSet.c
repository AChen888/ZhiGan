/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "usart.h"
#include "win_information.h"
#include "win_usart.h"
#include "win_life.h"


static u16 ItemSelected = 0;
static u16 Address;

#define LINE_TOTAL		5
#define LINE_TOTAL_YES_NOZHU		4
#define LINE_TOTAL_NO		3
#define LINE_Y(n)		(30 + 30 * n)
#define LINE_H			(30)
#define LABLE_W			(320)
#define DATA_W			(160)

static ui_window_t WinSystemSet =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};
static ui_window_t WinSystemSet_Yes_NoZhu =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};
static ui_window_t WinSystemSet_No =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

static t_MenuItem MenuItemList[LINE_TOTAL] =
{
	[0].name = "设备地址",
	[0].text = 0,
	[0].state = 0,

	[1].name = "串口设置",
	[1].text = "---->",
	[1].state = 0,
	
	[2].name = "电流输出",
	[2].text = "---->",
	[2].state = 0,
	
	[3].name = "注液设置",
	[3].text = "---->",
	[3].state = 0,
	
	[4].name = "设备信息",
	[4].text = "---->",
	[4].state = 0,

};

static t_MenuItem MenuItemList_Yes_NoZhu[LINE_TOTAL] =
{
	[0].name = "设备地址",
	[0].text = 0,
	[0].state = 0,

	[1].name = "串口设置",
	[1].text = "---->",
	[1].state = 0,
	
	[2].name = "电流输出",
	[2].text = "---->",
	[2].state = 0,
	
	[3].name = "设备信息",
	[3].text = "---->",
	[3].state = 0,

};

static t_MenuItem MenuItemList_No[LINE_TOTAL] =
{
	[0].name = "设备地址",
	[0].text = 0,
	[0].state = 0,

	[1].name = "串口设置",
	[1].text = "---->",
	[1].state = 0,
	
	[2].name = "设备信息",
	[2].text = "---->",
	[2].state = 0,

};

//创建控件
static void Win_Init ( void )
{
	u16 i;
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
	
	
	ItemSelected = 0;
	for(i = 0; i < LINE_TOTAL; i++)
	{
		MenuItemList[i].state = 0;
	}
	MenuItemList[ItemSelected].state = 2;
	Address = gSystemData.u8MoudleID;
}

//创建控件
static void Win_Init_Yes_NoZhu ( void )
{
	u16 i;
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
	
	
	ItemSelected = 0;
	for(i = 0; i < LINE_TOTAL_YES_NOZHU; i++)
	{
		MenuItemList_Yes_NoZhu[i].state = 0;
	}
	MenuItemList_Yes_NoZhu[ItemSelected].state = 2;
	Address = gSystemData.u8MoudleID;
}

//创建控件
static void Win_Init_No ( void )
{
	u16 i;
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
	
	
	ItemSelected = 0;
	for(i = 0; i < LINE_TOTAL_NO; i++)
	{
		MenuItemList_No[i].state = 0;
	}
	MenuItemList_No[ItemSelected].state = 2;
	Address = gSystemData.u8MoudleID;
}

static u16 SelectColor(u16 index)
{
	u16 bcolor;
	
	if(MenuItemList[index % LINE_TOTAL].state == 2)
	{	
		bcolor = rgb565_focused_background;
	}
	else if(MenuItemList[index % LINE_TOTAL].state == 1)
	{
		MenuItemList[index % LINE_TOTAL].state = 0;
	}
	
	if(MenuItemList[index % LINE_TOTAL].state != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line2;
		else
			bcolor = rgb565_grid_line1;
	}
	
	return bcolor;
}

static u16 SelectColor_Yes_NoZhu(u16 index)
{
	u16 bcolor;
	
	if(MenuItemList_Yes_NoZhu[index % LINE_TOTAL_YES_NOZHU].state == 2)
	{	
		bcolor = rgb565_focused_background;
	}
	else if(MenuItemList_Yes_NoZhu[index % LINE_TOTAL_YES_NOZHU].state == 1)
	{
		MenuItemList_Yes_NoZhu[index % LINE_TOTAL_YES_NOZHU].state = 0;
	}
	
	if(MenuItemList_Yes_NoZhu[index % LINE_TOTAL_YES_NOZHU].state != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line2;
		else
			bcolor = rgb565_grid_line1;
	}
	
	return bcolor;
}

static u16 SelectColor_No(u16 index)
{
	u16 bcolor;
	
	if(MenuItemList_No[index % LINE_TOTAL_NO].state == 2)
	{	
		bcolor = rgb565_focused_background;
	}
	else if(MenuItemList_No[index % LINE_TOTAL_NO].state == 1)
	{
		MenuItemList_No[index % LINE_TOTAL_NO].state = 0;
	}
	
	if(MenuItemList_No[index % LINE_TOTAL_NO].state != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line2;
		else
			bcolor = rgb565_grid_line1;
	}
	
	return bcolor;
}

static void Win_Paint ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable( 0, LINE_Y(0), DATA_W, LINE_H, MenuItemList[0].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(1), DATA_W, LINE_H, MenuItemList[1].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(2), DATA_W, LINE_H, MenuItemList[2].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(3), DATA_W, LINE_H, MenuItemList[3].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(4), DATA_W, LINE_H, MenuItemList[4].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
}

static void Win_Paint_Yes_NoZhu ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable( 0, LINE_Y(0), DATA_W, LINE_H, MenuItemList_Yes_NoZhu[0].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(1), DATA_W, LINE_H, MenuItemList_Yes_NoZhu[1].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(2), DATA_W, LINE_H, MenuItemList_Yes_NoZhu[2].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(3), DATA_W, LINE_H, MenuItemList_Yes_NoZhu[3].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
}

static void Win_Paint_No ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable( 0, LINE_Y(0), DATA_W, LINE_H, MenuItemList_No[0].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(1), DATA_W, LINE_H, MenuItemList_No[1].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(2), DATA_W, LINE_H, MenuItemList_No[2].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	
	MainWin_UpdateTitleBar("系统设置");
	MainWin_UpdateStatusBar("返回", "下移", "确认");
}

static void Win_ModifyPaint ( void )
{
	u16 bcolor;
	u16 i;
	char string[16];
	
	for(i = 0; i < LINE_TOTAL; i++)
	{
		bcolor = SelectColor(i);
		if(i == 0)
		{
			snprintf(string, sizeof(string), "*%03d", Address);
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, string, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
		else
		{
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, MenuItemList[i].text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
	}
}

static void Win_ModifyPaint_Yes_NoZhu ( void )
{
	u16 bcolor;
	u16 i;
	char string[16];
	
	for(i = 0; i < LINE_TOTAL_YES_NOZHU; i++)
	{
		bcolor = SelectColor_Yes_NoZhu(i);
		if(i == 0)
		{
			snprintf(string, sizeof(string), "*%03d", Address);
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, string, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
		else
		{
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, MenuItemList_Yes_NoZhu[i].text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
	}
}

static void Win_ModifyPaint_No ( void )
{
	u16 bcolor;
	u16 i;
	char string[16];
	
	for(i = 0; i < LINE_TOTAL_NO; i++)
	{
		bcolor = SelectColor_No(i);
		if(i == 0)
		{
			snprintf(string, sizeof(string), "*%03d", Address);
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, string, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
		else
		{
			ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, MenuItemList_No[i].text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
		}
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
							case 0:		//设备地址设置
								g_Digital.udata = Address;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, 160, LINE_Y(0), DATA_W, LINE_H, efsize_24);
								break;
							case 1:		//串口设置
									WinUsart_Create(win);
								break;
							case 2:		//电流输出设置
									WinCurrent_Create(win);
								break;
							case 3:		//注液设置
								if(AuthCheck())
								{
								}
								break;
							case 4:		//系统信息设置
								WinInformation_Create(win);
								break;
						}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					MenuItemList[ItemSelected].state = 1;
					if(ItemSelected)
						ItemSelected--;
					else
						ItemSelected = LINE_TOTAL - 1;
					MenuItemList[ItemSelected].state = 2;

					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:						//
					MenuItemList[ItemSelected].state = 1;
					ItemSelected++;
					if(ItemSelected >= LINE_TOTAL)
						ItemSelected = 0;
					MenuItemList[ItemSelected].state = 2;

					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					ui_widget_go_prev_win ( win->prev );		//
					break;
				case ewk_C:
				break;
			}
			
			Win_ModifyPaint();

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
				case 1:
					break;
				case 0:		//
					Address = g_Digital.udata;
					if(Address < 1 || Address > 247)
						Address = 1;
					gSystemData.u8MoudleID = Address;
					g_ModbusEntry.Addr = Address;
					g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
					if ( g_SaveParamFlag )		//保存参数到fram
					{
						System_SaveCommonData ( g_SaveParamFlag );
						g_SaveParamFlag = 0;
					}
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

//事件回调函数
static void Win_Callback_Yes_NoZhu ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		Win_Init_Yes_NoZhu();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		Win_Paint_Yes_NoZhu();
		Win_ModifyPaint_Yes_NoZhu();
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//
						switch(ItemSelected)
						{
							case 0:		//设备地址设置
								g_Digital.udata = Address;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, 160, LINE_Y(0), DATA_W, LINE_H, efsize_24);
								break;
							case 1:		//串口设置
									WinUsart_Create(win);
								break;
							case 2:		//电流输出设置
									WinCurrent_Create(win);
								break;
							case 3:		//系统信息设置
								WinInformation_Create(win);
								break;
						}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					MenuItemList_Yes_NoZhu[ItemSelected].state = 1;
					if(ItemSelected)
						ItemSelected--;
					else
						ItemSelected = LINE_TOTAL_YES_NOZHU - 1;
					MenuItemList_Yes_NoZhu[ItemSelected].state = 2;
					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:						//
					MenuItemList_Yes_NoZhu[ItemSelected].state = 1;
					ItemSelected++;
					if(ItemSelected >= LINE_TOTAL_YES_NOZHU)
						ItemSelected = 0;
					MenuItemList_Yes_NoZhu[ItemSelected].state = 2;
					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					ui_widget_go_prev_win ( win->prev );		//
					break;
				case ewk_C:
				break;
			}
			
			Win_ModifyPaint_Yes_NoZhu();

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
				case 0:		//
					Address = g_Digital.udata;
					if(Address < 1 || Address > 247)
						Address = 1;
					gSystemData.u8MoudleID = Address;
					g_ModbusEntry.Addr = Address;
					g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
					if ( g_SaveParamFlag )		//保存参数到fram
					{
						System_SaveCommonData ( g_SaveParamFlag );
						g_SaveParamFlag = 0;
					}
					break;
			}
		}
		MainWin_UpdateStatusBar("返回", "下移", "确认");
		Win_ModifyPaint_Yes_NoZhu();
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}

//事件回调函数
static void Win_Callback_No ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		Win_Init_No();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		Win_Paint_No();
		Win_ModifyPaint_No();
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//
						switch(ItemSelected)
						{
							case 0:		//设备地址设置
								g_Digital.udata = Address;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_3, 160, LINE_Y(0), DATA_W, LINE_H, efsize_24);
								break;
							case 1:		//串口设置
								WinUsart_Create(win);
								break;
							case 2:		//系统信息设置
								WinInformation_Create(win);
								break;
						}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					MenuItemList_No[ItemSelected].state = 1;
					if(ItemSelected)
						ItemSelected--;
					else
						ItemSelected = LINE_TOTAL_NO - 1;
					MenuItemList_No[ItemSelected].state = 2;

					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:						//
					MenuItemList_No[ItemSelected].state = 1;
					ItemSelected++;
					if(ItemSelected >= LINE_TOTAL_NO)
						ItemSelected = 0;
					MenuItemList_No[ItemSelected].state = 2;

					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					ui_widget_go_prev_win ( win->prev );		//
					break;
				case ewk_C:
				break;
			}
			
			Win_ModifyPaint_No();

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
				case 0:		//
					Address = g_Digital.udata;
					if(Address < 1 || Address > 247)
						Address = 1;
					gSystemData.u8MoudleID = Address;
					g_ModbusEntry.Addr = Address;
					g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
					if ( g_SaveParamFlag )		//保存参数到fram
					{
						System_SaveCommonData ( g_SaveParamFlag );
						g_SaveParamFlag = 0;
					}
					break;
			}
		}
		MainWin_UpdateStatusBar("返回", "下移", "确认");
		Win_ModifyPaint_No();
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
void WinSystemSet_Create ( void *parg )												//有权限是注液机
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinSystemSet, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSystemSet );		//很重要，一定要有这一句，第一个运行的窗口
}

void WinSystemSet_Create_No ( void *parg )					//无权限系统菜单
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinSystemSet_No, Win_Callback_No );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSystemSet_No );		//很重要，一定要有这一句，第一个运行的窗口
}

void WinSystemSet_Create_Yes_NoZhu ( void *parg )							//有权限但不是注液机
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinSystemSet_Yes_NoZhu, Win_Callback_Yes_NoZhu );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSystemSet_Yes_NoZhu );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
