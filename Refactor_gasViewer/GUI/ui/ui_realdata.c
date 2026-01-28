/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_realdata.h"
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


void ui_realdata_draw(void* parg)
{
	ui_realdata_t* win = (ui_realdata_t*)parg;
	
	ui_DrawBar(win->pos.x, win->pos.y, win->pos.width, win->pos.height, win->color.fcolor);
	ui_DrawBar(win->pos.x+8, win->pos.y+8, win->pos.width-16, win->pos.height-16, win->color.bcolor);

	if(win->name.text.text)
		win->name.event.paint(&(win->name));
	if (win->unit.text.text)
		win->unit.event.paint(&(win->unit));
	if (win->data.text.text)
		win->data.event.paint(&(win->data));
}

WM_HWIN ui_realdata_create(ui_widgets_t* parent, ui_realdata_t* self)
{
	if (self == 0)
		return 0;

	self->color.fcolor = rgb565_win_front;
	self->color.bcolor = rgb565_win_background;

	self->event.paint = ui_realdata_draw;

	self->name.pos.x = self->pos.x+8;
	self->name.pos.y = self->pos.y + 8;
	self->name.pos.width = 8 * 5;
	self->name.pos.height = 16;
	self->name.text.height = 16;
	self->name.text.align = (eat_hleft | eat_vcenter);
	self->name.text.color.bcolor = rgb565_lable_background;
	self->name.text.color.fcolor = rgb565_lable_text;

	self->unit.pos.x = self->pos.x+8;
	self->unit.pos.y = self->pos.y + 24 * 3;
	self->unit.pos.width = 8 * 5;
	self->unit.pos.height = 16;
	self->unit.text.height = 16;
	self->unit.text.align = (eat_hleft | eat_vcenter);
	self->unit.text.color.bcolor = rgb565_lable_background;
	self->unit.text.color.fcolor = rgb565_lable_text;

	self->data.pos.x = self->pos.x + 8*6;
	self->data.pos.y = self->pos.y + 24;
	self->data.pos.width = 160 - 8 * 7;
	self->data.pos.height = 48;
	self->data.text.height = 32;
	self->data.text.align = (eat_hleft | eat_vcenter);
	self->data.text.color.bcolor = rgb565_lable_background;
	self->data.text.color.fcolor = rgb565_digital_front;

	ui_lable_create((ui_widgets_t*)self, &(self->name));
	ui_lable_create((ui_widgets_t*)self, &(self->unit));
	ui_lable_create((ui_widgets_t*)self, &(self->data));

	return self;
}

/**********************
  **********************/
