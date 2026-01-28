/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_CONFIRM_H
#define UI_CONFIRM_H

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
 	ecr_None = 0,
	ecr_OK,
	ecr_Cancel
 };
	
	typedef struct
	{
		ui_widgets_pos pos;
		ui_widgets_text text;		//空闲
		ui_widgets_event event;		//事件
		ui_widgets_color color;

		ui_lable_t ok;				//确认
		ui_lable_t cancel;			//取消
		u16 selected;				//光标选中
		u16 result;					//结果
		ui_widgets_t* prev;						//上一个窗�?

	} ui_confirm_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_confirm_create(ui_widgets_t* parent, ui_confirm_t* self, char* msg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
