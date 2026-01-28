/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_icon.h"
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


void ui_icon_draw(void* parg)
{
	ui_icon_t* self = (ui_icon_t*)parg;
	
	//ui_DrawBar(self->pos.x, self->pos.y, self->pos.width, self->pos.height, self->color.fcolor);
	if (self->selected)
	{
		self->name.text.color.fcolor = rgb565_lable_text;
		self->name.text.color.bcolor = rgb565_focused_background;
	}
	else
	{
		self->name.text.color.fcolor = rgb565_lable_text;
		self->name.text.color.bcolor = rgb565_lable_background;
	}

	if(self->name.text.text)
		self->name.event.paint(&(self->name));
	if (self->icon.text.text)
		self->icon.event.paint(&(self->icon));
}

WM_HWIN ui_icon_create(ui_widgets_t* parent, ui_icon_t* self)
{
	if (self == 0)
		return 0;
	
	
	
	self->pos.width = 100;
	self->pos.height = 96;
//	self->color.fcolor = rgb565_win_front;
//	self->color.bcolor = rgb565_win_background;
	self->event.paint = ui_icon_draw;
	self->selected = 0;
	//图标在上，64X64
	self->icon.pos.x = self->pos.x;
	self->icon.pos.y = self->pos.y+4;
	self->icon.pos.width = self->pos.width;
	self->icon.pos.height = 60;
	self->icon.text.height = efsize_48;
	self->icon.text.align = (eat_hcenter | eat_vcenter);
	self->icon.text.color.bcolor = rgb565_icon_background;
	self->icon.text.color.fcolor = rgb565_icon_front;
	//名称在下
	self->name.pos.x = self->pos.x;
	self->name.pos.y = self->pos.y + 60;
	self->name.pos.width = self->pos.width;
	self->name.pos.height = 32;
	self->name.text.height = efsize_24;
	self->name.text.align = (eat_hcenter | eat_vcenter);
	self->name.text.color.bcolor = rgb565_lable_background;
	self->name.text.color.fcolor = rgb565_lable_text;

	ui_lable_create((ui_widgets_t*)self, &(self->name));
	ui_lable_create((ui_widgets_t*)self, &(self->icon));

	return self;
}



/**********************
  **********************/
