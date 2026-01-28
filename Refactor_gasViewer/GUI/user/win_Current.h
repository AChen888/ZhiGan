/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_CURRENT_H
#define WIN_CURRENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
extern u8 out20mA;
extern u8 out4mA;
extern u8 Current_Open;
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
	
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern void WinCurrent_Create(void* parg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
