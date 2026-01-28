/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_lable.h"
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
static void ui_lable_draw(void* parg)
{
	ui_lable_t* win = (ui_lable_t*)parg;
	
	//ui_DrawBar(win->pos.x, win->pos.y, win->pos.width, win->pos.height, win->text.color.bcolor);			//background
	ui_show_text(&win->pos, &win->text, 1);																	//text
}

WM_HWIN ui_lable_create(ui_widgets_t*parent, ui_lable_t *self)
{
//	ui_window_t* p = (ui_window_t *)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->event.paint = ui_lable_draw;

	return self;
}

WM_HWIN ui_lable_create_ex(ui_widgets_t*parent, ui_lable_t *self, u16 x, u16 y, char *text, u16 cheight, u16 fc, u16 bc, u16 align)
{
//	ui_window_t* p = (ui_window_t *)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->event.paint = ui_lable_draw;
	self->text.align = (eat_hleft | eat_vcenter);
	self->text.color.bcolor = rgb565_lable_background;
	self->text.color.fcolor = rgb565_lable_text;
	self->pos.x = x,
	self->pos.y = y,
	self->pos.width = ui_GetDrawWidth(text, cheight);
	self->pos.height = cheight,
	self->text.height = cheight,
	self->text.align = align,
	self->text.text = text;

	return self;
}
/**********************
  **********************/
