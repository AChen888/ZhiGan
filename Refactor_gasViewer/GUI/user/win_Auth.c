/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_menu.h"
#include "appConfig.h"
#include "win_auth.h"
//#include "gas\gasRecord.h"
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

static ui_window_t WinSelAuth =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

u32 g_SelAuth = 0;
u8 g_SelAuth_id = 0;

#define ADMIN_PASSWORD			1900
#define GOD_PASSWORD				1122

#define LINE_TOTAL		4
static u16 PassWord = 0;
/**********************
 *      MACROS
 **********************/
#define LINE_Y(n)		(30 + 30 * 2)
#define LINE_H			(30*2)
#define LABLE_W			(160)
#define DATA_W			(32*4)

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
//创建控件
static void Win_Init ( void )
{
	WinSelAuth.sublist = 0; 
	PassWord = 1111;
	
	MainWin_UpdateTitleBar("PASSWORD");
	MainWin_UpdateStatusBar("+", "下移", "确认");
}

static void Win_Paint ( void )
{
	char string[32];

	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "password", efsize_32, rgb565_lable_text, rgb565_lable_background, ( eat_hleft | eat_vcenter ));
	snprintf(string, sizeof(string), "%04d", PassWord);
	ui_show_lable(160, LINE_Y(0), DATA_W, LINE_H, string, efsize_64, rgb565_lable_text, rgb565_lable_background, ( eat_hleft | eat_vcenter ));
}

static void Win_UpdatePaint ( void )
{
	char string[16] = {0};

	snprintf(string, sizeof(string), "%04d", PassWord);
	ui_show_lable(160, LINE_Y(0), DATA_W, LINE_H, string, efsize_64, rgb565_lable_text, rgb565_lable_background, ( eat_hleft | eat_vcenter ));
}

static void Win_CheckPassword ( void )
{
	if(PassWord == ADMIN_PASSWORD)
	{
		ui_show_lable(160, LINE_Y(0), DATA_W, LINE_H, "ADMIN", efsize_24, rgb565_icon_record, rgb565_draw_gray, ( eat_hcenter | eat_vcenter ));
		g_SelAuth = HAL_GetTick();
		g_SelAuth_id=1;
		osDelay(500);
		Pape_Flag &= ~(1 << 0);
		ui_widget_go_prev_win ( WinSelAuth.next );
	}
	else if(PassWord == GOD_PASSWORD)
	{
		ui_show_lable(160, LINE_Y(0), DATA_W, LINE_H, "GOD", efsize_24, rgb565_draw_red, rgb565_draw_gray, ( eat_hcenter | eat_vcenter ));  
		g_SelAuth = HAL_GetTick();
		g_SelAuth_id=2;
		osDelay(500);
		Pape_Flag &= ~(1 << 0);
		ui_widget_go_prev_win ( WinSelAuth.next );
	}
	else
	{
		ui_show_lable(160, LINE_Y(0), DATA_W, LINE_H, "ERROR", efsize_24, rgb565_lable_text, rgb565_draw_gray, ( eat_hcenter | eat_vcenter ));
		g_SelAuth = 0;
		g_SelAuth_id=0;
		osDelay(1000);
		Pape_Flag &= ~(1 << 0);
		ui_widget_go_prev_win ( WinSelAuth.prev );
	}
}
/*
操作权限

0：普通用户
1：管理员
2: 上帝
*/
u32 AuthCheck(void)
{
	u32 diff = 0;

	if(g_SelAuth)
	{
		diff = gas_CheckTimeDiff(g_SelAuth);
		if(diff < (10 * 60 * 1000))					//十分钟内有效
		{
			return 1;
		}
		g_SelAuth = 0;
		g_SelAuth_id = 0;
	}

	return 0;
}

/*
延长管理员权限
*/
u32 AuthExpand(void)
{
	u32 diff = 0;

	if(g_SelAuth)
	{
		diff = gas_CheckTimeDiff(g_SelAuth);
		if(diff < (5 * 60 * 1000))					//少于五分钟
		{
			g_SelAuth += 5 * 60 * 1000;
		}
	}

	return 0;
}


//事件回调函数
static void Win_Callback ( void *parg )
{
	u16 key;
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
		Win_UpdatePaint();

		g_Digital.udata = PassWord;
		ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_4, 160, LINE_Y(0), DATA_W, LINE_H, efsize_64);
	}
	
	else if ( win->event.event & ewm_key )
	{
		key = ui_GetInputKey ( 1 );
		switch ( key )
		{
			case ewk_esc:
			case ewk_resc:
				PassWord = 1111;
				Win_CheckPassword();
				break;

			case ewk_ok:
			case ewk_rok:
				
				break;
		}
		Win_UpdatePaint(); 

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		//g_EditData.format;
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;

			PassWord = g_Digital.udata;
		}
		Win_UpdatePaint();
		MainWin_UpdateStatusBar("返回", "下移", "确认");
		Win_CheckPassword();
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
void SelAuth_Create ( void* prev , void* next)
{
	Pape_Flag|=(1<<0);
	ui_window_create ( ( ui_window_t * ) prev, &WinSelAuth, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSelAuth );		//很重要，一定要有这一句，第一个运行的窗口
	WinSelAuth.next = next;
}
/**********************
  **********************/
