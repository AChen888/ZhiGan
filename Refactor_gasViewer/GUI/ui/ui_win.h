/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_WINDOW_H
#define UI_WINDOW_H

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
		ui_widgets_text text;		//
		ui_widgets_event event;		//事件
		ui_widgets_color color;

		ui_widgets_t **sublist;			//sub widgets list
		ui_widgets_t* topwin;			//parent window

		ui_widgets_t* prev;						//上一个窗�?
		ui_widgets_t* next;						//下一个窗�?
		
		ui_widgets_tick tick;
	} ui_window_t;						//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern WM_HWIN ui_window_create(ui_window_t* parent, ui_window_t* self, event_cb cb);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
