/**
 * @file 
 *
 */

#ifndef UI_CHECKBOX_H
#define UI_CHECKBOX_H

#include "ui\ui_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
	enum {
		ecf_none = 0,
		ecf_unchecked,
		ecf_checked,
		ecf_selected
	} ;//e_checkbox_flag;

	typedef struct
	{
		ui_widgets_pos pos;			//位置信息
		ui_widgets_text text;		//显示文本
		ui_widgets_event event;		//事件回调
		u16 flag;					//显示符号
	} ui_checkbox_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern WM_HWIN ui_checkbox_create(ui_widgets_t* parent, ui_checkbox_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
