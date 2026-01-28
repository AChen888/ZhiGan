/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef UI_KEY_H
#define UI_KEY_H

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
extern void ui_SetInputKey(char key);
extern char ui_GetInputKey(u8 clear);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
