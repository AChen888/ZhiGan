/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_LIST_H
#define UI_LIST_H

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
		ui_widgets_pos pos;
		ui_widgets_text text;		//空闲
		ui_widgets_event event;		//事件

		u16 total;
		u16 focused;
		u16 mode;
		ui_widgets_t* prev;						//上一个窗体

	} ui_list_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern u16 ui_list_get_state(ui_list_t* self, u16 index);
	extern u16 ui_list_set_state(ui_list_t* self, u16 index, u16 state);
	extern u16 ui_list_reverse_state(ui_list_t* self, u16 index, u16 state);
	extern u16 ui_list_get_radio(ui_list_t* self);
	extern u16 ui_list_set_radio(ui_list_t* self, u16 index);
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_list_create(ui_widgets_t* parent, ui_list_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
