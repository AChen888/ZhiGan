/**
 * @file 
 *
 */

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

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
	typedef struct
	{
		ui_widgets_pos pos;			//位置信息
		ui_widgets_text text;		//显示文本
		ui_widgets_event event;		//事件回调
		
		ui_widgets_color color;
		u16 selected;				//光标选中

	} ui_button_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern WM_HWIN ui_button_create(ui_widgets_t* parent, ui_button_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
