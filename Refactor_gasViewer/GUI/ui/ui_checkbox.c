/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui_checkbox.h"
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
void ui_checkbox_draw(void* parg)
{
	u16 dx = 0, dy = 0, len;
	char* fch = 0;

	ui_checkbox_t* win = (ui_checkbox_t*)parg;
	ui_DrawBar(win->pos.x, win->pos.y, win->pos.width, win->pos.height, win->text.color.bcolor);

	switch (win->flag)
	{
	case ecf_selected:
		fch = "¡Ì";
		break;
	case ecf_unchecked:
		fch = "¡ğ";
		break;
	case ecf_checked:
		fch = "¡ñ";
		break;
	case ecf_none:
	default:
		fch = "  ";
		break;
	}

	len = ui_GetDrawWidth(win->text.text, win->text.height) + win->text.height;

	if (win->text.align & eat_hleft)
		dx = win->pos.x;
	else if (win->text.align & eat_hright)
		dx = win->pos.x + (win->pos.width - len);
	else if (win->text.align & eat_hcenter)
		dx = win->pos.x + (win->pos.width - len) / 2;

	if (win->text.align & eat_vtop)
		dy = win->pos.y;
	else if (win->text.align & eat_vbottom)
		dy = win->pos.y + (win->pos.height - win->text.height);
	else if (win->text.align & eat_vcenter)
		dy = win->pos.y + (win->pos.height - win->text.height) / 2;

	//·ûºÅ
	ui_DrawString(dx, dy, fch, win->text.color.fcolor, win->text.color.bcolor, (u8)win->text.height);
	//ÎÄ±¾
	ui_DrawString(dx + win->text.height, dy, win->text.text, win->text.color.fcolor, win->text.color.bcolor, (u8)win->text.height);
}

WM_HWIN ui_checkbox_create(ui_widgets_t* parent, ui_checkbox_t* self)
{
	//ui_window_t* p = (ui_window_t*)parent;
	if (parent == 0 || self == 0)
		return 0;

	self->event.paint = ui_checkbox_draw;

	return self;
}
