/**
 * @file
 *
 */

#include "ui\ui_config.h"


#ifndef UI_EVENT_PROCESS_H
#define UI_EVENT_PROCESS_H

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
extern  void ui_widget_update ( ui_widgets_t *hwin );
extern void ui_widget_go_prev_win ( ui_widgets_t *prev );
extern void ui_widgets_set_event_bit ( u16 codebit );
extern void ui_widgets_set_top_win ( ui_widgets_t *top );
extern ui_widgets_t *ui_widgets_get_top_win ( void );
extern u32 ui_widgets_run_top_win ( void );
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
