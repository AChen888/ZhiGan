/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui_button.h"
#include "ui.h"
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
void ui_button_draw(void* parg)
{
	ui_button_t* win = (ui_button_t*)parg;
	ui_DrawBar(win->pos.x, win->pos.y, win->pos.width - 2, win->pos.height - 2, win->text.color.fcolor);
	ui_DrawBar(win->pos.x + 2, win->pos.y + win->pos.height - 2, win->pos.width - 2, 2, win->text.color.bcolor);	//bottom line
	ui_DrawBar(win->pos.x + win->pos.width - 2, win->pos.y + 2, 2, win->pos.height - 2, win->text.color.bcolor);	//right line

	ui_show_text(&win->pos, &win->text, 1);
}

WM_HWIN ui_button_create(ui_widgets_t* parent, ui_button_t* self)
{
	//ui_window_t* p = (ui_window_t*)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->color.fcolor = rgb565_button_background;			//background
	self->color.bcolor = rgb565_button_edge;				//button shadow
	self->text.align = (eat_hcenter | eat_vcenter);
	self->text.color.fcolor = rgb565_lable_text;
	self->text.color.bcolor = rgb565_button_background;
	self->selected = 0;

	self->event.paint = ui_button_draw;

	return self;
}
