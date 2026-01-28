/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_digital.h"
#include "stdio.h"
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

static void ui_digital_data(ui_digital_t* self)
{
	char* p = self->print;
	switch (self->format)
	{
	case edf_int_5:
		if (p == 0)
			p = "%5d";
		snprintf(self->string, sizeof(self->string), p, self->idata);
		break;
	case edf_uint_1:
		if (p == 0)
			p = "%1u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_uint_2:
		if (p == 0)
			p = "%2u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_uint_3:
		if (p == 0)
			p = "%3u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_uint_4:
		if (p == 0)
			p = "%4u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_uint_5:
		if (p == 0)
			p = "%5u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_uint_9:
		if (p == 0)
			p = "%9u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_ulong_10:
		if (p == 0)
			p = "%10u";
		snprintf(self->string, sizeof(self->string), p, self->udata);
		break;
	case edf_float_14:
		if (p == 0)
			p = "%6.4f";
		snprintf(self->string, sizeof(self->string), p, self->fdata);
		break;
	case edf_float_23:
		if (p == 0)
			p = "%6.3f";
		snprintf(self->string, sizeof(self->string), p, self->fdata);
		break;
	case edf_float_32:
		if (p == 0)
			p = "%6.2f";
		snprintf(self->string, sizeof(self->string), p, self->fdata);
		break;
	case edf_float_41:
		if (p == 0)
			p = "%6.1";
		snprintf(self->string, sizeof(self->string), p, self->fdata);
		break;
		
	case edf_float_50:		
	default:
		if (p == 0)
			p = "%5.0";
		snprintf(self->string, sizeof(self->string), p, self->fdata);
		break;
	}
}

static void ui_digital_draw(void* parg)
{
	ui_digital_t* self = (ui_digital_t*)parg;
	self->text.text = self->string;
	ui_digital_data(self);

	ui_show_text(&self->pos, &self->text, 1);
}

WM_HWIN ui_digital_create(ui_widgets_t*parent, ui_digital_t*self)
{
//	ui_window_t* p = (ui_window_t *)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->string[0] = 0;
	self->event.paint = ui_digital_draw;

	return self;
}

WM_HWIN ui_digital_create_ex(ui_widgets_t*parent, ui_digital_t *self, u16 x, u16 y, char *text, u16 cheight, u16 fc, u16 bc, u16 align)
{
//	ui_window_t* p = (ui_window_t *)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->string[0] = 0;
	self->event.paint = ui_digital_draw;
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
	self->format = 	edf_int_5;
	self->print = 0;
	self->udata = 0;

	return self;
}

/**********************
  **********************/
