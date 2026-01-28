/**
 * @file 
 *
 */



#ifndef WIN_AUTH_H
#define WIN_AUTH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui_config.h"
/*********************
 *      DEFINES
 *********************/
extern u32 g_SelAuth;
extern u8 g_SelAuth_id;
/**********************
 *      TYPEDEFS
 **********************/
	
/**********************
 * GLOBAL PROTOTYPES
 **********************/
void SelAuth_Create(void* prev, void* next);
u32 AuthCheck(void);
u32 AuthExpand(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
