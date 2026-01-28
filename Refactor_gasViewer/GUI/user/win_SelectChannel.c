/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_menu.h"
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

static ui_window_t WinSelChannel =
{
	.pos.x = 0,
	.pos.y = 24,
	.pos.width = 320,
	.pos.height = 240 - 24 * 2,
};

 /*
 typedef enum {
	evdn_NO2 = 0,
	evdn_O3,
	evdn_SO2,
	evdn_CO,
	evdn_TVOC,
	evdn_End
} e_ViewData_Name;
*/


u16 g_SelectChannel = 0;
static u8 ItemList[USE_CHANNEL_MAX];

static const char* ChannelTitle[] = 
{
	"通道-1",
	"通道-2",
	"通道-3",
	"通道-4",
	"通道-5",
	"通道-6",
};
/**********************
 *      MACROS
 **********************/
#define LINE_Y(n)			(24 + 32 * n)

#define LINE_H			(31)
#define LABLE_W			(320 / 2 - 16)
#define DATA_W			LABLE_W

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
 static void Win_GotoNextWin(ui_window_t* win, u8 selected)
{
	//ui_widgets_t* next;
	if(win == 0)
		return;
	
	switch(selected)
	{
	case menu1_Zero:
		WinZero_Create(win);	//返回时跳过这个窗口
		//next = ui_widgets_get_top_win();
		//AuthCheckPower(win, next);
		break;
	case menu1_Calib:
		WinCalib_Create(win);
		//next = ui_widgets_get_top_win();
		//AuthCheckPower(win, next);
		break;
	case menu1_Alarm:
		WinAlarm_Create(win);
		break;
	case menu1_Channel:
		WinChannel_Create(win);
		//next = ui_widgets_get_top_win();
		//AuthCheckPower(win, next);
		break;
	}
 }

//创建控件
static void Win_Init ( void )
{
	WinSelChannel.sublist = 0;
	g_SelectChannel = 0;

	if(USE_CHANNEL_MAX == 1)
	{
		g_SelectChannel = 0;
		Win_GotoNextWin((ui_window_t*)WinSelChannel.prev, g_Menu1_ItemSelected);
	}
	
	MainWin_UpdateTitleBar("选择通道");
	MainWin_UpdateStatusBar("返回", "->", "确认");
}

static void Win_Paint ( void )
{
	char string[32];
	u16 i;
	for(i = 0; i < USE_CHANNEL_MAX; i++)
	{
		ui_show_lable(16, LINE_Y(i), LABLE_W, LINE_H, (char*)ChannelTitle[i], efsize_24, rgb565_lable_text, rgb565_lable_background, ( eat_hcenter | eat_vcenter ));

		snprintf(string, sizeof(string), "%s,%s", g_ViewData_Name[ gCalidata[i].Name % evdn_End ], g_ViewData_Unit[ gCalidata[i].Unit % evdu_End ]);
		ui_show_lable(160, LINE_Y(i), DATA_W, LINE_H, string, efsize_24, rgb565_lable_text, rgb565_lable_background, ( eat_hcenter | eat_vcenter ));
		ItemList[i] = 0;
	}
	ItemList[g_SelectChannel] = 2;
}

static void Win_UpdatePaint ( void )
{
	u16 i;
	for(i = 0; i < USE_CHANNEL_MAX; i++)
	{
		if(ItemList[i] == 1)		//去掉焦点
		{
			ui_show_lable(16, LINE_Y(i), LABLE_W, LINE_H, (char*)ChannelTitle[i], efsize_24, rgb565_lable_text, rgb565_lable_background, ( eat_hcenter | eat_vcenter ));
			ItemList[i] = 0;
		}
		else if(ItemList[i] == 2)		//进入焦点
		{
			ui_show_lable(16, LINE_Y(i), LABLE_W, LINE_H, (char*)ChannelTitle[i], efsize_24, rgb565_lable_text, rgb565_focused_background, ( eat_hcenter | eat_vcenter ));
		}
	}
}


//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		Win_Init();  //可以删除

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		Win_Paint();
		Win_UpdatePaint(); //？？？
	}
	else if ( win->event.event & ewm_update )
	{
		//ui_widget_update((ui_widgets_t*)win);
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_rok:
			case ewk_ok:
				Win_GotoNextWin(win, g_Menu1_ItemSelected);
				break;

			case ewk_up:   //？？
				case ewk_rup:
				case ewk_left:
				ItemList[g_SelectChannel] = 1;
				if(g_SelectChannel)
					g_SelectChannel--;
				else
					g_SelectChannel = USE_CHANNEL_MAX - 1;
				ItemList[g_SelectChannel] = 2;
				Win_UpdatePaint();
				break;

			case ewk_right:   //？？？
			case ewk_rdown:
			case ewk_down:
				ItemList[g_SelectChannel] = 1;
				g_SelectChannel++;
				if(g_SelectChannel >= USE_CHANNEL_MAX)
					g_SelectChannel = 0;
				ItemList[g_SelectChannel] = 2;
				Win_UpdatePaint();
				break;

			case ewk_home:
				case ewk_resc:
				case ewk_esc:  //返回
				ui_widget_go_prev_win ( win->prev );
				break;

		}

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_time )
	{
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
void SelChannel_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinSelChannel, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinSelChannel );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
