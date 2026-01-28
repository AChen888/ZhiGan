/**
 * @file 
 *
 */

#include "ui\ui_config.h"


#ifndef UI_DRAW_H
#define UI_DRAW_H

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
extern  u16 ui_GetDrawWidth(char* str, u16 sizey);
extern	u8 ui_DrawBar(u16 x, u16 y, u16 w, u16 h, u16 fc);
extern	u8 ui_DrawChar(u16 x, u16 y, u8 ch, u16 fc, u16 bc, u8 sizey);
extern	u8 ui_DrawChinese(u16 x, u16 y, char* ch, u16 fc, u16 bc, u8 sizey);
extern	u8 ui_DrawString(u16 x, u16 y, char* str, u16 fc, u16 bc, u8 sizey);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
