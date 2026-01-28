/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_confirm.h"
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


void ui_confirm_draw(void* parg)
{
	ui_confirm_t* self = (ui_confirm_t*)parg;
	
	ui_DrawBar(self->pos.x, self->pos.y, self->pos.width, self->pos.height, self->color.bcolor);
	ui_DrawString(self->pos.x, self->pos.y, self->text.text, self->text.color.fcolor, self->text.color.bcolor, self->text.height);
	if (self->selected)
	{
		self->ok.text.color.bcolor = rgb565_lable_background;
		self->cancel.text.color.bcolor = rgb565_digital_front;
	}
	else
	{
		self->ok.text.color.bcolor = rgb565_digital_front;
		self->cancel.text.color.bcolor = rgb565_lable_background;
	}

	self->ok.event.paint(&self->ok);
	self->cancel.event.paint(&self->cancel);
}

static void ui_confirm_callback(void* parg)
{
	ui_confirm_t* self = (ui_confirm_t*)parg;

	if (self->event.event & ewm_init)
	{
		self->event.event &= ~ewm_init;
		self->event.event |= ewm_paint;
	}
	else if (self->event.event & ewm_paint)
	{
		self->event.paint(self);
		self->event.event &= ~ewm_paint;
	}
	else if (self->event.event & ewm_update)
	{
		self->event.event &= ~ewm_update;
	}
	else if (self->event.event & ewm_key)
	{
		switch (ui_GetInputKey(1))
		{
		case ewk_rok:
				case ewk_ok:
			if(self->selected == 0)
			{
				self->result = ecr_OK;
			}
			else
			{
				self->result = ecr_Cancel;
			}
			self->prev->event.event |= ewm_update;
			//break;
		case ewk_esc:
			if (self->prev)
			{
				ui_widgets_set_top_win(self->prev);
				self->prev->event.event |= ewm_paint;
			}
			break;
			
		case ewk_up:
		case ewk_down:
			if(self->selected)
				self->selected = 0;
			else
				self->selected = 1;
			
			self->event.event |= ewm_paint;
			break;
		}
		self->event.event &= ~ewm_key;
	}
	else if (self->event.event & ewm_delete)
	{
		self->event.event &= ~ewm_delete;
	}
	else if (self->event.event & ewm_time)
	{
		//win->event.event &= ~ewm_time;		//周期运行
	}

}


WM_HWIN ui_confirm_create(ui_widgets_t* parent, ui_confirm_t* self, char* msg)
{
	if (self == 0)
		return 0;

	self->pos.x = (320 - 240) / 2;
	self->pos.y = (240 - 120) / 2;
	self->pos.width = 240;
	self->pos.height = 120;
	self->color.fcolor = rgb565_lable_text;
	self->color.bcolor = rgb565_button_edge;
	self->text.color.fcolor = rgb565_lable_text;
	self->text.color.bcolor = rgb565_button_edge;
	self->text.height = efsize_24;
	self->text.align = (eat_hleft | eat_vcenter);
	if(msg == 0)
		self->text.text = "请确认修改";
	else
		self->text.text = msg;
	self->event.paint = ui_confirm_draw;
	self->event.cb = ui_confirm_callback;
	self->prev = parent;
	self->selected = 0;
	self->result = ecr_None;
	//
	self->ok.pos.x = self->pos.x;
	self->ok.pos.y = self->pos.y + 24 + 16;
	self->ok.pos.width = self->pos.width / 2;
	self->ok.pos.height = 48;
	self->ok.text.height = efsize_24;
	self->ok.text.align = (eat_hcenter | eat_vcenter);
	self->ok.text.color.bcolor = rgb565_lable_background;
	self->ok.text.color.fcolor = rgb565_lable_text;
	self->ok.text.text = "确认";
	//
	self->cancel.pos.x = self->pos.x + 120;
	self->cancel.pos.y = self->pos.y + 24 + 16;
	self->cancel.pos.width = self->pos.width / 2;
	self->cancel.pos.height = 48;
	self->cancel.text.height = efsize_24;
	self->cancel.text.align = (eat_hcenter | eat_vcenter);
	self->cancel.text.color.bcolor = rgb565_lable_background;
	self->cancel.text.color.fcolor = rgb565_lable_text;
	self->cancel.text.text = "取消";

	ui_lable_create((ui_widgets_t*)self, &(self->ok));
	ui_lable_create((ui_widgets_t*)self, &(self->cancel));

	return self;
}

/**********************
  **********************/
