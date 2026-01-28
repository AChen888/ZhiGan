/**
 * @file 
 *
 */
#ifdef __WINDOWS__
#include "lcd_init.h"
#include "lcd.h"
#else
#include "lcd.h"
#include "ui\gui.h"
#endif

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "cmsis_os.h"
/*********************
 *      DEFINES
 *********************/
#if defined ( __CC_ARM   )
    #pragma anon_unions
#endif
/**********************
 *      TYPEDEFS
 **********************/

 
 enum {
		esize_none = 0,

		efsize_16 = 16,
	  efsize_22 = 22,
		efsize_24 = 24,
		efsize_32 = 32,
		efsize_48 = 48,
		efsize_64 = 64,

	} ;//e_key_event;			//
	
	enum {
		ewk_none = 0,
		

		ewk_esc,
		ewk_down,
		ewk_ok,		
		ewk_up,
		
		//组合键或数字键（红外遥控器）
		ewk_resc,
		ewk_rdown,
		ewk_rup,
		ewk_rok,		
		ewk_left,
		ewk_right,
		ewk_home,
		ewk_test,
		ewk_mute,			//POWER
		ewk_C,				//C， Auth
		ewk_func0, 
		ewk_func1,
		ewk_func2,
		ewk_func3,
		ewk_func4,
		ewk_func5,
		ewk_func6,
		ewk_func7,
		ewk_func8,
		ewk_func9,

		ewk_end
	} ;//e_key_event;			//

#define COLOR_RED(C)		((C >> 3) << 11)
#define COLOR_GREEN(C)		((C >> 2) << 5)
#define COLOR_BLUE(C)		((C >> 3) << 0)
#define COLOR_RGB565(R,G,B)	(COLOR_RED(R) | COLOR_GREEN(G) | COLOR_BLUE(B))
	//RGB565
	enum {
		rgb565_WHITE = 0xFFFF,
		rgb565_BLACK = 0x0000,
		rgb565_BLUE = 0x001F,
		rgb565_BRED = 0xF81F,
		rgb565_GRED = 0xFFE0,
		rgb565_GBLUE = 0x07FF,
		rgb565_RED = 0xF800,
		rgb565_MAGENTA = 0xF81F,
		rgb565_GREEN = 0x07E0,
		rgb565_CYAN = 0x7FFF,
		rgb565_YELLOW = 0xFFE0,
		rgb565_BROWN = 0xBC40, //棕色
		rgb565_BRRED = 0xFC07, //棕红色
		rgb565_GRAY = 0x8430, //灰色
		rgb565_DARKBLUE = 0x01CF,	//深蓝色
		rgb565_LIGHTBLUE = 0x7D7C,	//浅蓝色  
		rgb565_GRAYBLUE = 0x5458,//灰蓝色
		rgb565_LIGHTGREEN = 0x841F,//浅绿色
		rgb565_LGRAY = 0xC618, //浅灰色(PANNEL),窗体背景色
		rgb565_LGRAYBLUE = 0xA651,//浅灰蓝色(中间层颜色)
		rgb565_LBBLUE = 0x2B12, //浅棕蓝色(选择条目的反色)

		rgb565_draw_darkblue = COLOR_RGB565(8, 16, 48),
		rgb565_draw_dark = COLOR_RGB565(0x28, 0x28, 0x28),
		rgb565_draw_gray = COLOR_RGB565(0x47, 0x47, 0x47),
		rgb565_draw_yellow = COLOR_RGB565(0xef, 0xf2, 0x84),
		rgb565_draw_cyan = COLOR_RGB565(0x0d, 0xc6, 0xdb),
		rgb565_draw_blue = COLOR_RGB565(0x14, 0x84, 0xf0),
		rgb565_draw_red = COLOR_RGB565(0xf9, 0x50, 0x50),
		rgb565_draw_green = COLOR_RGB565(0x08, 0xc8, 0xab),
		rgb565_draw_orange = COLOR_RGB565(0xf8, 0x8c, 0x6b),
		rgb565_draw_white = COLOR_RGB565(0xd9, 0xd9, 0xd9),

		rgb565_logo_front = rgb565_BLUE,
		rgb565_win_background = rgb565_draw_dark,
		rgb565_win_front = rgb565_draw_gray,
		rgb565_title_background = rgb565_DARKBLUE,
		rgb565_title_front = rgb565_draw_white,
		rgb565_icon_background = rgb565_win_background,
		rgb565_icon_front = COLOR_RGB565(180, 180, 130),
		rgb565_text_background = rgb565_win_background,
		rgb565_text_front = rgb565_draw_white,
		rgb565_button_edge = rgb565_LIGHTBLUE,
		rgb565_button_background = rgb565_draw_darkblue,
		rgb565_edit_background = rgb565_draw_gray,
		rgb565_edit_front = COLOR_RGB565(240, 240, 240),
		rgb565_digital_front = COLOR_RGB565(240, 240, 240),
		rgb565_lable_background = rgb565_win_background,
		rgb565_lable_text = rgb565_draw_white,
		rgb565_focused_background = COLOR_RGB565(50, 100, 70),
		rgb565_focused_front = COLOR_RGB565(102, 205, 170),
		rgb565_grid_line1 = COLOR_RGB565(0, 32, 48),
		rgb565_grid_line2 = COLOR_RGB565(0, 24, 48),

		rgb565_icon_auth = rgb565_draw_red,
		rgb565_icon_alarm = COLOR_RGB565(222, 184, 135),
		rgb565_icon_record = COLOR_RGB565(143, 184, 143),
	} ;//e_rgb565_color;

	enum {
		ewm_none = 0,
		ewm_init = (0x0001 << 0),
		ewm_delete = (0x0001 << 1),
		ewm_key = (0x0001 << 2),
		ewm_time = (0x0001 << 3),
		
		ewm_timeOS = (0x0001 << 0),
		
		ewm_paint = (0x0001 << 4),
		ewm_update = (0x0001 << 5),
	} ;//e_widgets_event;			//控件消息

	enum {
		ewt_none = 0,
		ewt_win,
		ewt_text,
		ewt_lable,
		ewt_digital,
		ewt_button,
		ewt_checkbox,
		ewt_edit,

		ewt_multi,
		ewt_realdata,
		ewt_icon,
		ewt_confirm,
		ewt_password,
	} ;//e_widgets_type;			//控件类型

	typedef union {
			u32 udata;
			int idata;
			float fdata;
		}union_Data;

	typedef struct	{
			char* name;
			char* data;
			u8 id;
			u8 type;
			u8 format;
			u8 flag;
		} t_CustomMenuItem;

	typedef struct  {
		char* name;
		char* text;
		u8 state;
	} t_MenuItem;

	enum {
		//水平
		eat_hleft     = (0x0001 << 0),
		eat_hright   = (0x0001 << 1),
		eat_hcenter = (0x0001 << 2),
		//垂直
		eat_vtop      = (0x0001 << 3),
		eat_vbottom    = (0x0001 << 4),
		eat_vcenter  = (0x0001 << 5),
	} ;//e_alignment_type;		//对齐方式

	//控件位置
	typedef struct {
		u16 x;						//左上角坐标
		u16 y;						//左上角坐标
		u16 width;				//宽
		u16 height;				//高
	} ui_widgets_pos;
	//控件颜色
	typedef struct {
		u16 fcolor;						//前景色
		u16 bcolor;						//背景色
	} ui_widgets_color;
	//控件文本
	typedef struct {
		ui_widgets_color color;				//颜色
		u16 height;							//字符高度（点阵大小）
		u16 align;							//水平对齐
		char* text;							//显示文本
	} ui_widgets_text;

	typedef struct {
		u16 step;
		u16 period;
		u32 lasttick;
		u32 newtick;
	} ui_widgets_tick;

	typedef void*		WM_HWIN;		//窗口句柄

	typedef void (*event_cb)(void* pwin);				//事件回调函数
	typedef WM_HWIN (*func_cb)(void* pwin, void *parg);	//动作回调函数

	//控件事件
	typedef struct {
		u32 event;						//事件
		event_cb cb;					//事件回调
		event_cb paint;					//事件回调
	} ui_widgets_event;
	//通用控件
	typedef struct
	{
		ui_widgets_pos pos;			//位置信息
		ui_widgets_text text;		//显示文本
		ui_widgets_event event;		//事件
	} ui_widgets_t;					//通用控件

/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
	extern u16 g_SelectChannel;

	extern u16 ui_format_string(char *string, u16 size, u32* data, u16 format, char *print);
 	extern char ui_int2char(u8 i);
	extern void* ui_malloc(void** mem, u16 size);
	extern void* ui_free(void** mem);
	
	extern ui_widgets_pos ui_show_text(ui_widgets_pos* pos, ui_widgets_text  * text, u8 drawback);
	extern void ui_show_lable(u16 x, u16 y, u16 w, u16 h, char *string, u16 cheight, u16 fc, u16 bc, u16 align);
	extern void ui_show_text_ex(u16 x, u16 y, u16 w, u16 h, char *string, u16 cheight, u16 fc, u16 bc, u16 align, u8 drawback);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
