/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_ICON_H
#define UI_ICON_H

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
		ui_widgets_color color;

		ui_lable_t name;			//数据名称
		ui_lable_t icon;			//图标
		u16 selected;				//光标选中

	} ui_icon_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_icon_create(ui_widgets_t* parent, ui_icon_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
