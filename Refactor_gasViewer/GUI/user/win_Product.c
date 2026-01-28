/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "win_product.h"
#include "win_life.h"

static u16 ItemSelected = 0;

#define LINE_TOTAL		4
#define LINE_Y(n)		(30 + 30 * n)
#define LINE_H			(30)
#define LABLE_W			(320)
#define DATA_W			(160)

static ui_window_t WinProduct =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

static t_MenuItem MenuItemList[LINE_TOTAL] =
{
	[0].name = "气体类型设置",
	[0].text = "---->",
	[0].state = 0,

	[1].name = "浓度参数设置",
	[1].text = "---->",
	[1].state = 0,
	
	[2].name = "寿命运维设置",
	[2].text = "---->",
	[2].state = 0,
	
	[3].name = "设备功能测试",
	[3].text = "---->",
	[3].state = 0,
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

static void Win_Paint ( void )
{
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable( 0, LINE_Y(0), DATA_W, LINE_H, MenuItemList[0].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(1), DATA_W, LINE_H, MenuItemList[1].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(2), DATA_W, LINE_H, MenuItemList[2].name, efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable( 0, LINE_Y(3), DATA_W, LINE_H, MenuItemList[3].name, efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	
	MainWin_UpdateTitleBar("生产设置");
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
		ui_show_lable(160, LINE_Y(i), LABLE_W, LINE_H, MenuItemList[i].text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
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
							case 0:		//气体类型设置
								
								break;
							case 1:		//浓度参数设置
		
								break;
							case 2:		//寿命运维设置
									WinLife_Create(win);
								break;
							case 3:		//设备功能设置
									
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
					ui_widget_go_prev_win ( win->prev );
					break;
				case ewk_C:
				break;
			}
			
			Win_ModifyPaint();

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
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
void WinProduct_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinProduct, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinProduct );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
