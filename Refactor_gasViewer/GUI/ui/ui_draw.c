/**
 * @file
 *
 */

 /*********************
  *      INCLUDES
  *********************/
//#include "lcd.h"
#include "ui_draw.h"
#include "ui_font.h"
#include "lcd.h"
#include "lcd_init.h"
#include "ui\gui.h"
  /*********************
   *      DEFINES
   *********************/
extern void lcd_WR_DATA_LEN(u8 *dat, u16 len);
extern void lcd_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
   /**********************
	*      TYPEDEFS
	**********************/

	/**********************
	 *  STATIC PROTOTYPES
	 **********************/
	 /**********************
	  *  STATIC VARIABLES
	  **********************/
	  // one point is u16 (u8 * 2)
	  // 320 X 240 = 76800
	  // 48 X 48 = 2304
	  // 32 X 32 = 1024
	  // 24 X 24 = 576
	  // 16 X 16 = 256
	  // 320 / 2 = 32 X 5 = 160
	  // 240

#define UI_DRAW_BUFFER_MAX			(0x1000 / 2)

// CCM 0x10000000, 0x2000
static u16 draw_buffer[UI_DRAW_BUFFER_MAX]   __attribute__ ((at(0x10000000)));	  
static u16 draw_count;

/*
* 复位缓冲区
*/
void ui_ResetData ( void )
{
	draw_count = 0;
}

/*
* 把填充好的点画到液晶上面
*/
void ui_SendData ( void )
{
	if ( draw_count > 0 )
		lcd_WR_DATA_LEN ( ( u8 * ) draw_buffer, draw_count * 2 );

	draw_count = 0;
}
/*
* 填充点
*/
void ui_AddData ( u16 color )
{
	u16 temp;

	if ( draw_count >= UI_DRAW_BUFFER_MAX )
	{
		ui_SendData();
	}

	if ( draw_count < UI_DRAW_BUFFER_MAX )
	{
		temp = ( color << 8 );
		temp += ( color >> 8 );
		draw_buffer[draw_count++] = temp;
	}
}
/**********************
 *      MACROS
 **********************/

 /**********************
  *   GLOBAL FUNCTIONS
  **********************/
#if 1

/*
    get string display width
*/
u16 ui_GetDrawWidth(char* str, u16 sizey)
{
	u16 sizex = sizey / 2;
	u16 i, len = 0;

	if (str == 0 || sizey == 0)
		return 0;

	for (i = 0; str[i] != 0; i++)
	{
		if (str[0] & 0x80 && str[1] & 0x80)		//汉字
		{
			i++;
			len += sizey;
		}
		else
		{
			len += sizex;
		}
	}
	return len;
}
/******************************************************************************
	  函数说明：查找字符点阵
	  入口数据：
				u8* hz,      要显示的字符
				u16 sizey 字符尺寸
	  返回值：  字符点阵缓冲区首地址

******************************************************************************/
static u8* ui_FindFontBuffer(u8* ch, u16 sizey)
{
	u8* buf = 0;
	GUI_FONT_PROP* fontitem = 0;
	struct t_ascii_lib* lib = 0;
	u8 hz[2];
	u32 libsize;
	u16 itemsize;
	u16 i, code;
	u16 total = 0;	//统计数目

	if (ch == 0 || sizey == 0)
		return 0;

	hz[0] = ch[0];
	hz[1] = ch[1];
	if (hz[0] & 0x80 && hz[1] & 0x80)		//汉字
	{
		if (sizey == efsize_24)
		{
			fontitem = (GUI_FONT_PROP*)GUI_FontHZ24x24.Prop;
		}
		else if (sizey == efsize_48)
		{
			fontitem = (GUI_FONT_PROP*)GUI_FontHZ48x48.Prop;
		}
		else if (sizey == efsize_64)
		{
			fontitem = (GUI_FONT_PROP*)GUI_FontHZ64x64.Prop;
		}
	/*	else if (sizey == efsize_32)
		{
			fontitem = (GUI_FONT_PROP*)GUI_FontHZ32x32.Prop;
		}
		else if (sizey == efsize_16)
		{
			fontitem = (GUI_FONT_PROP*)GUI_FontHZ16x16.Prop;
		}*/
emwin_font:

		if (fontitem > 0)
		{
			for (i = 0; fontitem > 0; i++)
			{
				if (fontitem->First < fontitem->Last)
				{
					for (code = fontitem->First; code <= fontitem->Last; code++)
					{
						if (((u8)(code >> 8) == hz[0]) && ((u8)(code & 0xFF) == hz[1]))
						{
							buf = (u8*)fontitem->paCharInfo[code - fontitem->First].pData;
							break;
						}
					}
				}
				else
				{
					if (((u8)(fontitem->First >> 8) == hz[0]) && ((u8)(fontitem->First & 0xFF) == hz[1]))
					{
						buf = (u8*)fontitem->paCharInfo->pData;
						break;
					}
				}
				fontitem = (GUI_FONT_PROP*)fontitem->pNext;
			}
		}
		else
		{
			buf = (u8*)&ascii_24X12_lib;
		}
	}
	else
	{
		if (sizey == efsize_24)
		{
			lib = (struct t_ascii_lib*)&ascii_24X12_lib;
			libsize = lib->libsize;
			itemsize = 48;
		}
		else if (sizey == efsize_32)
		{
			lib = (struct t_ascii_lib*)&ascii_32X16_lib;
			libsize = lib->libsize;
			itemsize = 64;
		}
		else if (sizey == efsize_64)
		{
			hz[0] = 0;
			hz[1] = ch[0];
			fontitem = (GUI_FONT_PROP*)GUI_FontC64x64.Prop;
			goto emwin_font;
		}
/*		else if (sizey == efsize_16)
		{
			lib = (struct t_ascii_lib*)&ascii_16X08_lib;
			libsize = lib->libsize;
			itemsize = 16;
		}*/
//raw_font:
		if (lib > 0)
		{
			total = libsize / itemsize;
			i = ch[0] - ' ';
			if (i < total)
			{
				buf = &lib->pdata[i * itemsize];
			}
		}
		else
		{
			buf = (u8*)&ascii_24X12_lib;
		}
	}

	return buf;
}


/*
* 填充缓冲区中的一个点
*/
static void ui_FillPixel(u8 pixel, u16 fc, u16 bc)
{
	if (pixel)
		ui_AddData(fc);//画一个点
	else
		ui_AddData(bc);
}


/*
* 向缓冲区填充8个点的数据
*/
static void ui_Fill_Byte(u8 B, u8 b, u16 fc, u16 bc)
{
	u8 i;

	if (b > 8)
		b = 8;

	for (i = 0; i < b; i++)
	{
		ui_FillPixel(B & (0x80 >> i), fc, bc);//画一个点
	}
}

static void ui_Fill_ReverseByte(u8 B, u8 b, u16 fc, u16 bc)
{
	u8 i;

	if (b > 8)
		b = 8;

	for (i = 0; i < b; i++)
	{
		ui_FillPixel(B & (0x01 << i), fc, bc);//画一个点
	}
}

/*
* 填充一个实心矩形
* fc 填充颜色
*/
u8 ui_DrawBar(u16 x, u16 y, u16 w, u16 h, u16 fc)
{
	u32 i, total = w * h;

	if (total == 0 || total > (LCD_W * LCD_H))
		return 0;

	lcd_Address_Set ( x, y, x + w - 1, y + h - 1 );
	ui_ResetData();
	
	for (i = 0; i < total; i++)
	{
		ui_AddData(fc);
	}
	
	ui_SendData();

	return 1;
}
/*
* 填充一个字符
* ch字符
* fc 填充颜色
* bc 背景色
* sizey 字符高度，宽度减半
*/
u8 ui_DrawChar(u16 x, u16 y, u8 ch, u16 fc, u16 bc, u8 sizey)
{
	u8* buf;
	u16 i, j, k;
	u8 w;
	u8 sizex = (sizey / 2) / 8;		//横向宽度
	if ((sizey / 2) % 8 > 0)
		sizex++;
	//sizey;						//纵向宽度

	buf = ui_FindFontBuffer(&ch, sizey);
	if (buf > 0)
	{
		lcd_Address_Set ( x, y, x + sizey/2 - 1, y + sizey - 1 );
		ui_ResetData();
		k = 0;
		for (i = 0; i < sizey; i++)
		{
			w = sizey / 2;
			for (j = 0; j < sizex; j++)
			{
				if (w > 8)
					ui_Fill_ReverseByte(buf[k++], 8, fc, bc);
				else
					ui_Fill_ReverseByte(buf[k++], w, fc, bc);
				w -= 8;
			}
		}
		ui_SendData();
		return 1;
	}
	return 0;
}

/*
* 填充一个字符，使用和汉字一样的字模
* ch字符
* fc 填充颜色
* bc 背景色
* sizey 字符高度，宽度减半


特殊字体，例如 efsize_64 = 32*48，需要上下各空余1/8的空间
*/
u8 ui_DrawChar2(u16 x, u16 y, u8 ch, u16 fc, u16 bc, u8 sizey)
{
	u8* buf;
	u16 i, j, k;
	u8 w;
	u8 sizex = (sizey / 2) / 8;		//横向宽度
	u8 inter = sizey / 8;
	if ((sizey / 2) % 8 > 0)
		sizex++;
	//sizey;						//纵向宽度

	buf = ui_FindFontBuffer(&ch, sizey);
	if (buf > 0)
	{
		lcd_Address_Set ( x, y, x + sizey/2 - 1, y + sizey - 1 );
		ui_ResetData();
		
		
		
		for (i = 0; i < inter; i++)		//8*4
		{
			w = sizey / 2;
			for (j = 0; j < sizex; j++)
			{
				if (w > 8)
					ui_Fill_Byte(0, 8, fc, bc);
				else
					ui_Fill_Byte(0, w, fc, bc);
				w -= 8;
			}
		}
		
		k = 0;
		for (i = inter; i < sizey - inter; i++)//48*4
		{
			w = sizey / 2;
			for (j = 0; j < sizex; j++)
			{
				if (w > 8)
					ui_Fill_Byte(buf[k++], 8, fc, bc);
				else
					ui_Fill_Byte(buf[k++], w, fc, bc);
				w -= 8;
			}
		}
		
		for (i = 0; i < inter; i++)		//8*4
		{
			w = sizey / 2;
			for (j = 0; j < sizex; j++)
			{
				if (w > 8)
					ui_Fill_Byte(0, 8, fc, bc);
				else
					ui_Fill_Byte(0, w, fc, bc);
				w -= 8;
			}
		}
		ui_SendData();
		return 1;
	}
	return 0;
}


/*
* 填充一个汉字
* *ch汉字
* fc 填充颜色
* bc 背景色
* sizey 汉字高度，宽度相同
*/
u8 ui_DrawChinese(u16 x, u16 y, char* ch, u16 fc, u16 bc, u8 sizey)
{
	u8* buf;
	u16 i, j, k;
	//u16 x0 = x;
	u8 sizex = sizey / 8;		//横向宽度
	//sizey;						//纵向宽度

	if (ch == 0 || sizey == 0)
		return 0;

	buf = ui_FindFontBuffer((u8*)ch, sizey);

	if (buf > 0)
	{
		lcd_Address_Set ( x, y, x + sizey - 1, y + sizey - 1 );
		ui_ResetData();
		k = 0;
		for (i = 0; i < sizey; i++)
		{
			for (j = 0; j < sizex; j++)
			{
				ui_Fill_Byte(buf[k++], 8, fc, bc);
			}
		}
		ui_SendData();
		return 1;
	}
	return 0;
}

/*
* 填充一个汉字
* *ch汉字
* fc 填充颜色
* bc 背景色
* sizey 汉字高度，宽度相同
*/
u8 ui_DrawString(u16 x, u16 y, char* str, u16 fc, u16 bc, u8 sizey)
{
	u16 i = 0;
	u8* ch;

	if (str == 0 || sizey == 0)
		return 0;

	for (i = 0; str[i] != 0; i++)
	{
		ch = (u8*)&str[i];
		if (ch[0] & 0x80 && ch[1] & 0x80)		//汉字
		{
			if (ui_DrawChinese(x, y, (char*)ch, fc, bc, sizey) > 0)
			{
				x += sizey;
				i++;
			}
		}
		else if(sizey == efsize_64)
		{
			if (ui_DrawChar2(x, y, ch[0], fc, bc, sizey) > 0)
			{
				x += sizey / 2;
			}
		}
		else
		{
			if (ui_DrawChar(x, y, ch[0], fc, bc, sizey) > 0)
			{
				x += sizey / 2;
			}
		}
	}

	return (u8)i;
}

void DebugShowString(u16 line, char* string)
{
	if(line > (240 / 16))
		line = 0;
	ui_DrawString(0, line * 16, string, WHITE, BLUE, 16);
}
#endif


/**********************
  **********************/
