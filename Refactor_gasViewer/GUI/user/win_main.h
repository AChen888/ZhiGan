/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_MAIN_H
#define WIN_MAIN_H

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
	extern void MainWin_Create(void* parg);
	extern void MainWin_UpdateTitleBar(char* title);
	extern void MainWin_UpdateTimeBar ( void );
	extern void MainWin_UpdateIconBar ( void );
	extern void MainWin_UpdateStatusBar(char* up, char* down, char* ok);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
