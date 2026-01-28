/**
 * @file 
 *
 */

#include "ui\ui_config.h"

#ifndef WIN_CALIB_H
#define WIN_CALIB_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
#define CALIB_CONCENTRATION_START(class, P_ARG)
#define CALIB_CONCENTRATION_RUNNING(class, P_ARG)
#define CALIB_CONCENTRATION_STOP(class, P_ARG)


/**********************
 *      TYPEDEFS
 **********************/
	
/**********************
 * GLOBAL PROTOTYPES
 **********************/
	extern void WinCalib_Create(void* parg);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
