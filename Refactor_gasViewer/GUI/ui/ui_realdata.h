/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_READDATA_H
#define UI_READDATA_H

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
		ui_lable_t unit;			//数据单位
		ui_lable_t data;			//实时数据

	} ui_realdata_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_realdata_create(ui_widgets_t* parent, ui_realdata_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
