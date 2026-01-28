/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_DIGITAL_H
#define UI_DIGITAL_H

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

		union {
			u32 udata;
			int idata;
			float fdata;
		};

		u16 format;
		char* print;
		char string[16];

	} ui_digital_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern WM_HWIN ui_digital_create(ui_widgets_t* parent, ui_digital_t* lable);
extern WM_HWIN ui_digital_create_ex(ui_widgets_t*parent, ui_digital_t *self, u16 x, u16 y, char *text, u16 cheight, u16 fc, u16 bc, u16 align);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
