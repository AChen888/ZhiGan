/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_MENU_H
#define WIN_MENU_H

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
	
/**********************
 * GLOBAL PROTOTYPES
 **********************/
enum {
 	menu1_Zero_A = 0,
	menu1_Calib_A,
	menu1_Alarm_A,
	menu1_Record_A,
	menu1_System_A,
	menu1_End_A
 };
enum {
 	menu1_Zero_N = 0,
	menu1_Record_N,
	menu1_System_N,
	menu1_End_N
 };
 enum {
 	menu1_Zero = 0,
	menu1_Calib,
	menu1_Alarm,
	menu1_Record,
	menu1_Product,
	menu1_System,
	menu1_End
 };
extern u16 g_Menu1_ItemSelected;
void MainMenu_Create(void* parg);
void MainMenu_Create_A(void* parg);
void MainMenu_Create_N(void* parg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
