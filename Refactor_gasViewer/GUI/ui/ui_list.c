/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_list.h"
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
u16 ui_list_get_state(ui_list_t* self, u16 index)
{
	if(self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;

	}

	return 0;
}

u16 ui_list_set_state(ui_list_t* self, u16 index, u16 state)
{
	if (self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;


	}
	return 0;
}

u16 ui_list_reverse_state(ui_list_t* self, u16 index, u16 state)
{
	if (self->mode == emf_multi)
	{
		if (index >= self->total)
			index = 0;

	}
	return 0;
}

u16 ui_list_get_radio(ui_list_t* self)
{
	u16 i;

	if (self->mode == emf_single)
	{
		for (i = 0; i < self->total; i++)
		{
		}
	}

	return 0;
}

u16 ui_list_set_radio(ui_list_t* self, u16 index)
{
//	u16 i;

	if (self->mode == emf_single)
	{
		if (index >= self->total)
			index = 0;


	}

	return 0;
}

void ui_list_draw_callback(void* parg)
{
	u16 i;
//	ui_list_t* self = (ui_list_t*)parg;
	
	for (i = 0; i < UI_MULTI_ITEM_MAX; i++)
	{

	}
}

//事件回调函数
static void ui_list_callback(void* parg)
{
	ui_list_t* self = (ui_list_t*)parg;

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

WM_HWIN ui_list_create(ui_widgets_t* parent, ui_list_t* self)
{
	u16 i, r, c;
	u16 rows;
	if (self == 0)
		return 0;

	self->prev = parent;
	self->event.paint = ui_list_draw_callback;
	self->event.cb = ui_list_callback;
	self->event.event |= ewm_init;		//初始化事件
	self->focused = 0;
	if (self->total)
	{
		rows = self->pos.height / UI_MULTI_ITEM_HEIGHT;
//		columns = self->pos.width / UI_MULTI_ITEM_WIDTH;
		for (i = 0, r = 0, c = 0; i < UI_MULTI_ITEM_MAX; i++)
		{


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
