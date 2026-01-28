/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_MENU_H
#define UI_MENU_H

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

		ui_widgets_t* prev;						//上一个窗体
		char* title;
		char* status;
/*
	W = 20 X 16 = 320
	H = 10 X 24 = 240
   
    [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]			标题栏 24
    [2][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][1]			+
    [3][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][2]			|
    [4][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][3]			|
    [5][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][4]			+     24 X 8 = 32 X 6
    [6][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][5]			+
    [7][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][6]			|
    [8][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][7]			|
    [9][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][8]			+
    [0][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][9]			状态栏 24
*/
		u8 total;			//菜单项总数
		u8 select;			//选中的菜单项
		u8 screen;			//屏幕可以显示的菜单项行数，一般为6
		u8 line;			//屏幕的第几行 0~5
		u8 page;			//菜单项分成几页
		t_CustomMenuItem * list;		//菜单项列表

	} ui_menu_t;				//窗体
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern u16 ui_menu_get_state(ui_menu_t* self, u16 index);
	extern u16 ui_menu_set_state(ui_menu_t* self, u16 index, u16 state);
	extern u16 ui_menu_reverse_state(ui_menu_t* self, u16 index, u16 state);
	extern u16 ui_menu_get_radio(ui_menu_t* self);
	extern u16 ui_menu_set_radio(ui_menu_t* self, u16 index);
	//自定义控件，用来显示特殊图像
	extern WM_HWIN ui_menu_create(ui_widgets_t* parent, ui_menu_t* self);
	
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
