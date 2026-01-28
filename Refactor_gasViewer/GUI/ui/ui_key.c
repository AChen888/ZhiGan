/**
 * @file 
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui_event.h"
#include "ui_key.h"
#include "appConfig.h"
#include "win_auth.h"
#include "lcd_init.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static char inputKey;
void ui_SetInputKey(char key)
{
    ui_widgets_t* win = (ui_widgets_t*)ui_widgets_get_top_win();
    if (win > 0 && key > 0)
    {
        inputKey = key;
        win->event.event |= ewm_key;
		
		AuthExpand();						//延长权限使用时间
		lcd_BG_Active();					//延长LCD背光
    }
}

char ui_GetInputKey(u8 clear)
{
    char key = inputKey;

    if (clear)
    {
        inputKey = 0;
    }

    return key;
}
/**********************
  **********************/
