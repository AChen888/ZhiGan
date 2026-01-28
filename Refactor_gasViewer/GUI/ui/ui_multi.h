/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_MULTI_H
#define UI_MULTI_H

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
		emf_single = 0,
		emf_multi,
	} ;//e_multi_flag;

	typedef struct
	{
		ui_widgets_pos pos;
		ui_widgets_text text;		//空闲
		ui_widgets_event event;		//事件
		
#define UI_MULTI_ITEM_MAX	16
#define UI_MULTI_ITEM_HEIGHT	24
#define UI_MULTI_ITEM_WIDTH		160
#define UI_MULTI_CHAR_HEIGHT	16

		ui_checkbox_t checkbox[UI_MULTI_ITEM_MAX];			//数据名称
		u16 total;
		u16 focused;
		u16 mode;
		ui_widgets_t* prev;						//上一个窗体

	} ui_multi_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern u16 ui_multi_get_state(ui_multi_t* self, u16 index);
	extern u16 ui_multi_set_state(ui_multi_t* self, u16 index, u16 state);
	extern u16 ui_multi_reverse_state(ui_multi_t* self, u16 index, u16 state);
	extern u16 ui_multi_get_radio(ui_multi_t* self);
	extern u16 ui_multi_set_radio(ui_multi_t* self, u16 index);
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_multi_create(ui_widgets_t* parent, ui_multi_t* self);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
