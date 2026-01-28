/**
 * @file 
 *
 */

#include "ui\ui_config.h"
#include "ui\ui_win.h"
#include "ui\ui_lable.h"
#include "ui\ui_digital.h"
#include "ui\ui_button.h"
#include "ui\ui_draw.h"
#include "ui\ui_event.h"

#include "ui\ui_checkbox.h"
#include "ui\ui_realdata.h"
#include "ui\ui_multi.h"
#include "ui\ui_list.h"
#include "ui\ui_icon.h"
#include "ui\ui_key.h"
#include "ui\ui_confirm.h"
#include "ui\ui_edit.h"


#include "user\win_startup.h"
#include "user\win_main.h"
#include "user\win_menu.h"
#include "user\win_SelectChannel.h"
#include "user\win_Zero.h"
#include "user\win_Calib.h"
#include "user\win_Channel.h"
#include "user\win_Alarm.h"
#include "user\win_Record.h"
#include "user\win_DateTime.h"
#include "user\win_SystemSet.h"
#include "user\win_Store.h"
#include "user\win_Current.h"
#include "user\win_Auth.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "appConfig.h"

#ifndef WIN_UI_H
#define WIN_UI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES ≤‚ ‘
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
extern union_Data g_Digital;
extern ui_edit_t g_EditData;
extern uint16_t Pape_Flag;

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
