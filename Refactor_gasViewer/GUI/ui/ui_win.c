/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
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

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_window_draw(void* parg)
{
	u16 i;
	ui_window_t* win = (ui_window_t*)parg;
	ui_widgets_t* widgets;
	
	ui_DrawBar(win->pos.x, win->pos.y, win->pos.width, win->pos.height, win->color.bcolor);

	if (win->sublist)
	{
		for (i = 0; win->sublist[i] > 0; i++)
		{
			widgets = win->sublist[i];
			if (widgets->event.paint)
				widgets->event.paint(win->sublist[i]);
		}
	}
}

static char title[41];
WM_HWIN ui_window_create(ui_window_t* parent, ui_window_t* self, event_cb cb)
{
	if (self == 0)
		return 0;

	memset(title, 0, sizeof(title));
	self->text.text = title;
	
	self->color.fcolor = rgb565_win_front;
	self->color.bcolor = rgb565_win_background;

	self->event.paint = ui_window_draw;
	self->event.cb = cb;
	self->event.event |= ewm_init;		//初始化事件

	self->prev = (ui_widgets_t*)parent;

	return self;
}

/**********************
  **********************/
