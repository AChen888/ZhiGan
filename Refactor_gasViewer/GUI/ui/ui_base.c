/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui_config.h"
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
union_Data g_Digital;
ui_edit_t g_EditData;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
 
static union {
			u32 udata;
			int idata;
			float fdata;
		} unionData;

u16 ui_format_string(char *string, u16 size, u32* data, u16 format, char *print) //不能输入16进制数
{
	char* p = print;
	unionData.udata = *data;
	u16 len = 0;
	switch (format)
	{
	case edf_int_5:
		if (p == 0)
			p = "%5d";
		len = snprintf(string, size, p, unionData.idata);
		break;
	case edf_uint_1:
		if (p == 0)
			p = "%1u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_uint_2:
		if (p == 0)
			p = "%2u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_uint_3:
		if (p == 0)
			p = "%3u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_uint_4:
		if (p == 0)
			p = "%4u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_uint_5:
		if (p == 0)
			p = "%5u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_uint_9:
		if (p == 0)
			p = "%9u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_ulong_10:
		if (p == 0)
			p = "%10u";
		len = snprintf(string, size, p, unionData.udata);
		break;
	case edf_sfloat_14:
		if (p == 0)
		{
			if(unionData.fdata >= 0)
				p = "+%06.4f";
			else
				p = "%06.4f";
		}
		snprintf(string, size, p, unionData.fdata);
		break;
	case edf_sfloat_23:
		if (p == 0)
		{
			if(unionData.fdata >= 0)
				p = "+%06.3f";
			else
				p = "%07.3f";
		}
		snprintf(string, size, p, unionData.fdata);
		break;
	case edf_sfloat_32:
		if (p == 0)
		{
			if(unionData.fdata >= 0)
				p = "%+06.2f";
			else
				p = "%07.2f";
		}
		snprintf(string, size, p, unionData.fdata);
		break;
	case edf_sfloat_41:

		if (p == 0)
		{
			if(unionData.fdata >= 0)
				p = "+%06.1f";
			else
				p = "%07.1f";
		}
		snprintf(string, size, p, unionData.fdata);
		break;
	case edf_sfloat_50:
		if (p == 0)
		{
			if(unionData.fdata >= 0)
				p = "+%05.0f";
			else
				p = "%06.0f";
		}
		snprintf(string, size, p, unionData.fdata);
		break;
		
	case edf_float_14:
		if (p == 0)
			p = "%6.4f";
		len = snprintf(string, size, p, unionData.fdata);
		break;
	case edf_float_23:
		if (p == 0)
			p = "%6.3f";
		len = snprintf(string, size, p, unionData.fdata);
		break;
	case edf_float_32:
		if (p == 0)
			p = "%6.2f";
		len = snprintf(string, size, p, unionData.fdata);
		break;
	case edf_float_41:
		if (p == 0)
			p = "%6.1f";
		len = snprintf(string, size, p, unionData.fdata);
		break;
		
	case edf_float_50:	
	default:
		if (p == 0)
			p = "%5.0f";
		len = snprintf(string, size, p, unionData.fdata);
		break;
	}

	return len;
}


char ui_int2char(u8 i)
{
	const char ch[] = "01234567890ABCDEF"; //多写了0

	return ch[i & 0x0f];
}

void* ui_malloc(void** mem, u16 size)
{
	u16 len = size / 4;
	if(size & 0x03)
		len++;
	len *= 4;
	
	if(*mem)
		free(*mem);
	
	*mem = (u32*)malloc(len);
	if(*mem != 0)
//		SEGGER_RTT_printf(0, "malloc %d failed!\n", len);
		memset(*mem, 0, len);
	
	return *mem;
}

void* ui_free(void** mem)
{
	if(*mem)
		free(*mem);
	*mem = 0;
	
	return *mem;
}


/*
    display string
*/
ui_widgets_pos ui_show_text(ui_widgets_pos* pos, ui_widgets_text  * text, u8 drawback)
{
	u16 dx = 0, dy = 0, len;
	ui_widgets_pos newpos = *pos;

	if(drawback)
		ui_DrawBar(pos->x,pos->y,pos->width,pos->height, text->color.bcolor);
	
	len = ui_GetDrawWidth(text->text, text->height);
	
	if (text->align & eat_hleft)
		dx =pos->x;
	else if (text->align & eat_hright)
		dx =pos->x + (pos->width - len);
	else if (text->align & eat_hcenter)
		dx =pos->x + (pos->width - len) / 2;

	if (text->align & eat_vtop)
		dy =pos->y;
	else if (text->align & eat_vbottom)
		dy =pos->y + (pos->height - text->height);
	else if (text->align & eat_vcenter)
		dy =pos->y + (pos->height - text->height) / 2;

	newpos.x = dx;
	newpos.y = dy;

	ui_DrawString(dx, dy, text->text, text->color.fcolor, text->color.bcolor, text->height);
	
	if(drawback == 0)
	{
		if(text->align & eat_hleft)
		{
			ui_DrawBar(pos->x + len,pos->y,pos->width - len,pos->height, text->color.bcolor);
		}
		else if(text->align & eat_hcenter)
		{
			ui_DrawBar(pos->x, pos->y, (pos->width - len)/2, pos->height, text->color.bcolor);
			ui_DrawBar(pos->x + ((pos->width+len)/2), pos->y, (pos->width - len)/2, pos->height, text->color.bcolor);
			ui_DrawBar(pos->x, pos->y, pos->width, (pos->height - text->height)/2, text->color.bcolor);
			ui_DrawBar(pos->x, pos->y+(pos->height+text->height)/2, pos->width, (pos->height - text->height)/2, text->color.bcolor);
		}
		
	}
	return newpos;
}

void ui_show_text_ex(u16 x, u16 y, u16 w, u16 h, char *string, u16 cheight, u16 fc, u16 bc, u16 align, u8 drawback)
{
	ui_widgets_pos pos =
	{
		.x = x,
		.y = y,
		.width = w,
		.height = h,
	};
	
	ui_widgets_text text =
	{
		.height = cheight,
		.align = align,
		.color.bcolor = bc,
		.color.fcolor = fc,
		.text = string,
	};
	
	if(cheight == 0)
		return;
	if(string == 0)
		return;
	if(w == 0)
		pos.width = ui_GetDrawWidth(text.text, text.height);
	if(h == 0)
		pos.height = cheight;
	if(align == 0)
		text.align = eat_hleft | eat_vcenter;
	
	ui_show_text(&pos, &text, drawback);
}

void ui_show_lable(u16 x, u16 y, u16 w, u16 h, char *string, u16 cheight, u16 fc, u16 bc, u16 align)
{
	ui_show_text_ex(x, y, w, h, string, cheight, fc, bc, align, 1);
}
/**********************
  **********************/
