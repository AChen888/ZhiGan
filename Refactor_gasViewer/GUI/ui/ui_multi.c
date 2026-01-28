/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_multi.h"
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
u16 ui_multi_get_state(ui_multi_t* self, u16 index)
{
	if(self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;

		if (self->checkbox[index].flag == ecf_selected)
			return 1;
	}

	return 0;
}

u16 ui_multi_set_state(ui_multi_t* self, u16 index, u16 state)
{
	if (self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;

		if(state)
			self->checkbox[index].flag = ecf_selected;
		else
			self->checkbox[index].flag = ecf_none;

		return self->checkbox[index].flag;
	}
	return 0;
}

u16 ui_multi_reverse_state(ui_multi_t* self, u16 index, u16 state)
{
	if (self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;

		if (state != ecf_selected)
			self->checkbox[index].flag = ecf_selected;
		else
			self->checkbox[index].flag = ecf_none;

		return self->checkbox[index].flag;
	}
	return 0;
}

u16 ui_multi_get_radio(ui_multi_t* self)
{
	u16 i;

	if (self->mode == emf_single)
	{
		for (i = 0; i < self->total; i++)
		{
			if (self->checkbox[i].flag == ecf_checked)
				return i;
		}
	}

	return 0;
}

u16 ui_multi_set_radio(ui_multi_t* self, u16 index)
{
	u16 i;

	if (self->mode == emf_single)
	{
		if (index >= self->total)
			index = 0;

		for (i = 0; i < UI_MULTI_ITEM_MAX; i++)
			self->checkbox[i].flag = ecf_unchecked;
		self->checkbox[index].flag = ecf_checked;
	}

	return 0;
}

void ui_multi_draw_callback(void* parg)
{
	u16 i;
	ui_multi_t* self = (ui_multi_t*)parg;
	
	for (i = 0; i < UI_MULTI_ITEM_MAX; i++)
	{
		if (self->focused == i)
			self->checkbox[i].text.color.bcolor = rgb565_draw_orange;
		else
			self->checkbox[i].text.color.bcolor = self->text.color.bcolor;
		self->checkbox[i].event.paint(&(self->checkbox[i]));
	}
}

//事件回调函数
static void ui_multi_callback(void* parg)
{
	ui_multi_t* self = (ui_multi_t*)parg;

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
	else if (self->event.event & ewm_time)
	{
		//win->event.event &= ~ewm_time;		//周期运行
	}
	else if (self->event.event & ewm_key)
	{
		switch (ui_GetInputKey(1))
		{
		case ewk_resc:
				case ewk_esc:
			ui_widget_go_prev_win(self->prev);
			break;
		case ewk_rdown:
				case ewk_down:
			self->focused++;
			if (self->focused >= self->total)
				self->focused = 0;
			self->event.event |= ewm_paint;
			break;
		case ewk_rok:
				case ewk_ok:
			if (self->mode == emf_single)
				ui_multi_set_radio(self, self->focused);
			else
				ui_multi_reverse_state(self, self->focused, self->checkbox[self->focused].flag);
			self->event.event |= ewm_paint;
			break;
		}
		self->event.event &= ~ewm_key;
	}
	else if (self->event.event & ewm_delete)
	{
		self->event.event &= ~ewm_delete;
	}
}

WM_HWIN ui_multi_create(ui_widgets_t* parent, ui_multi_t* self)
{
	u16 i, r, c;
	u16 rows;
	if (self == 0)
		return 0;

	self->prev = parent;
	self->event.paint = ui_multi_draw_callback;
	self->event.cb = ui_multi_callback;
	self->event.event |= ewm_init;		//初始化事件
	self->focused = 0;
	if (self->total)
	{
		rows = self->pos.height / UI_MULTI_ITEM_HEIGHT;
//		columns = self->pos.width / UI_MULTI_ITEM_WIDTH;
		for (i = 0, r = 0, c = 0; i < UI_MULTI_ITEM_MAX; i++)
		{
			self->checkbox[i].pos.x = self->pos.x + c * UI_MULTI_ITEM_WIDTH;
			self->checkbox[i].pos.y = self->pos.y + r * UI_MULTI_ITEM_HEIGHT;
			self->checkbox[i].pos.width = UI_MULTI_ITEM_WIDTH;
			self->checkbox[i].pos.height = UI_MULTI_ITEM_HEIGHT;
			self->checkbox[i].text.height = UI_MULTI_CHAR_HEIGHT;
			self->checkbox[i].text.align = (eat_hleft | eat_vcenter);
			self->checkbox[i].text.color.bcolor = self->text.color.bcolor;
			self->checkbox[i].text.color.fcolor = self->text.color.fcolor;
			if (self->mode == emf_single)
				self->checkbox[i].flag = ecf_unchecked;
			else
				self->checkbox[i].flag = ecf_none;

			ui_checkbox_create((ui_widgets_t*)self, &(self->checkbox[i]));
			
			if(i >= self->total)
				self->checkbox[i].text.text = 0;

			r++;
			if (r >= rows)
			{
				r = 0;
				c++;
			}
		}
	}

	ui_widgets_set_top_win((ui_widgets_t*)self);		//很重要，一定要有这一句，第一个运行的窗口

	return self;
}

/**********************
  **********************/
