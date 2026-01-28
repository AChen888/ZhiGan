/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_LIFE_H
#define WIN_LIFE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
extern uint8_t Life_DataModified;
extern uint16_t Use_Life;
/**********************
 *      TYPEDEFS
 **********************/
	
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern void WinLife_Create(void* parg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
