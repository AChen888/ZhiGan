/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_test_adc.h"
/*********************
 *      DEFINES
 W = 20 X 16 = 320
 H = 10 X 24 = 240

 [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			标题栏 24
 [2][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [3][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [4][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [5][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+     24 X 8
 [6][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [7][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [8][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			|
 [9][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			+
 [0][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			状态栏 24
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
static ui_window_t TestDacWindow =
{
	.pos.x = 0,
	.pos.y = 0,
	.pos.width = 320,
	.pos.height = 240,
};

ui_widgets_t *TestAdcWidgetsList[1] = {0};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/*

*/

//创建控件
static void WinTestAdc_Init ( void )
{
	TestDacWindow.tick.step = 0;
	TestDacWindow.tick.period = 1000;
	TestDacWindow.tick.lasttick = HAL_GetTick();
	TestDacWindow.event.event |= ewm_time;
}

static void WinTestAdc_Deinit ( void )
{
}

static void WinTestAdc_Paint ( void )
{
	
}

static void WinTestAdc_Update ( void )
{
	
}

static void WinTestAdc_TimerCallback ( void )
{
	//	char string[16];

	TestDacWindow.tick.newtick = HAL_GetTick();

	if ( TestDacWindow.tick.newtick - TestDacWindow.tick.lasttick > TestDacWindow.tick.period )
	{
		TestDacWindow.tick.lasttick = TestDacWindow.tick.newtick;

		switch ( TestDacWindow.tick.step )		//
		{
			case 0:				//
				break;

			case 1:				//
				break;

			case 2:				//
			default:
				break;
		}

		TestDacWindow.tick.step++;

		WinTestAdc_Update();
	}
}


//事件回调函数
static void WinTestAdc_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		WinTestAdc_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;

		WinTestAdc_Paint();
	}
	else if ( win->event.event & ewm_update )
	{
		ui_widget_update ( ( ui_widgets_t * ) win );
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_rok:
				case ewk_ok:
				WinTestAdc_Deinit();

				MainMenu_Create ( win );
				break;
		}

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_time )
	{
		WinTestAdc_TimerCallback();
		//win->event.event &= ~ewm_time;		//周期运行
	}
}

/*
* 创建第一个窗口
*/
void WinTestAdc_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &TestDacWindow, WinTestAdc_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &TestDacWindow );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
