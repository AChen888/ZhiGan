/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui_event.h"
#include "ui_win.h"
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
static ui_widgets_t *g_top_widgets;		//当前正在运行的控件
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void ui_widget_update ( ui_widgets_t *hwin )
{
	ui_window_t* win = (ui_window_t*)hwin;
	ui_widgets_t **list = 0;
	ui_widgets_t *widgets = 0;

	list = win->sublist;

	while ( list[0] > 0 )
	{
		widgets = list[0];

		if ( ( widgets[0].event.event & ewm_paint ) && ( widgets[0].event.paint > 0 ) )
		{
			widgets[0].event.paint ( widgets );
			widgets[0].event.event &= ~ewm_paint;
		}

		list++;
	}
}
void ui_widget_go_prev_win ( ui_widgets_t *prev )
{
	if ( prev == 0 )
		return;

	ui_widgets_set_top_win ( prev );
	prev->event.event |= ewm_paint;
}

void ui_widgets_set_event_bit ( u16 codebit )
{
	if ( g_top_widgets )
	{
		g_top_widgets->event.event |= codebit;
	}
}

void ui_widgets_set_top_win ( ui_widgets_t *top )
{
	g_top_widgets = top;
}

ui_widgets_t *ui_widgets_get_top_win ( void )
{
	return g_top_widgets;
}

u32 ui_widgets_run_top_win ( void )
{
	u32 ret = 0;

	if ( g_top_widgets )
	{
		if ( g_top_widgets->event.cb && g_top_widgets->event.event )
		{
			ret = g_top_widgets->event.event;
			g_top_widgets->event.cb ( ( void * ) g_top_widgets );
		}
	}

	return ret;
}

/**********************
  **********************/
