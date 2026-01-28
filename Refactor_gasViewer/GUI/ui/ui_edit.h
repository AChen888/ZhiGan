/**
 * @file 
 *
 */

#include "ui\ui_config.h"



#ifndef UI_EDIT_H
#define UI_EDIT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
enum {
	eem_move = 0,
	eem_edit,
	eem_result
} ;//e_edit_mode;
/**********************
 *      TYPEDEFS
 **********************/

	typedef struct
	{
		ui_widgets_pos pos;			//位置信息
		ui_widgets_text text;		//显示文本
		ui_widgets_event event;		//事件回调

/*		union {
			ui_widgets_t* widgets;
			ui_digital_t* digital;
			ui_lable_t* lable;
		};
	*/	
		union {
			u32 udata;
			int idata;
			float fdata;
		};

		ui_widgets_tick tick;

		u16 format;								//数据格式
		u16 focused;							//光标在第几个字符�?
		u16 length;								//字符串长�?
		u16  mode;								//编辑模式
		u16 twinkle;								//??

		char string[32];
		ui_confirm_t confirm;
		ui_widgets_t* prev;						//上一个窗�?
	} ui_edit_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern WM_HWIN ui_edit_create(ui_widgets_t* parent, ui_edit_t* self, union_Data data, u16 format, char* title);
	extern WM_HWIN ui_edit_create_ex(ui_widgets_t*parent, ui_edit_t* self, union_Data data, u16 format, u16 x, u16 y, u16 w, u16 h, u16 fsize);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
