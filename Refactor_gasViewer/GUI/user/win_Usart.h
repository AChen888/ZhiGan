/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_USART_H
#define WIN_USART_H

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
extern int SysBaud; 
 
extern void WinUsart_Create(void* parg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
