/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui.h"
#include "ui_menu.h"
/*********************
*      DEFINES
*********************/
#if 1
//	#define DEBUG_PRINT(...)			log_trace(__VA_ARGS__)
#else
//	#define DEBUG_PRINT(...)
#endif
/**********************
 *      TYPEDEFS
 **********************/
#define LINE_Y(n)		(24 + 32 * n)

#define LINE_H			(31)
#define LABLE_W			(320)
#define DATA_W			(12*8)
#define UNIT_W			(12*5)
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
u8 ui_menu_scroll_up ( ui_menu_t *self )
{
	self->select = self->line + self->page * self->screen;		//计算菜单选择项
	self->list[self->select].flag = 1;		//失去光标

	if ( self->line )
		self->line--;
	else
	{
		self->line = self->screen - 1;		//光标屏幕最下一行

		if ( self->page )		//翻页
			self->page--;
		else
		{
			self->page = ( self->total - 1 ) / self->screen;	//菜单最后一页
			self->line = ( self->total - 1 ) % self->screen;	//菜单最后一行
		}

		self->event.event |= ewm_paint;
	}

	self->select = self->line + self->page * self->screen;		//计算菜单选择项
	self->list[self->select].flag = 2;		//得到光标
//	DEBUG_PRINT ( "id %s, item %d, page %d, pos %d\r\n", self->list[self->select].name, self->select, self->page, self->line );
	return self->select;
}

u8 ui_menu_scroll_down ( ui_menu_t *self )
{
	self->select = self->line + self->page * self->screen;		//计算菜单选择项
	self->list[self->select].flag = 1;		//失去光标
	self->line++;		//向下移动菜单选择项

	if ( self->line >= self->screen )		//屏幕最下一行
	{
		self->line = 0;		//光标归零
		self->page++;		//翻页
		self->event.event |= ewm_paint;
	}

	self->select = self->line + self->page * self->screen;		//计算菜单选择项

	if ( self->select >= self->total )		//菜单最后一行
	{
		self->line = 0;		//光标归零
		self->page = 0;		//翻页
		self->select = 0;	//菜单选项归零
		self->event.event |= ewm_paint;
	}

	self->list[self->select].flag = 2;		//得到光标
//	DEBUG_PRINT ( "id %s, item %d, page %d, pos %d\r\n", self->list[self->select].name, self->select, self->page, self->line );
	return self->select;
}

u8 ui_menu_get_select_id ( ui_menu_t *self )
{
	return self->list[self->select].id;
}

void ui_menu_draw_title ( ui_menu_t *self )
{
	//u8 pos = self->page * self->screen;
	u8 i;

	for ( i = 0; i < self->screen; i++ )
	{
		//ui_show_lable(0, LINE_Y(pos), LABLE_W, LINE_H, str,  efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	}
}


void ui_menu_draw_callback ( void *parg )
{
	u16 i;
	//	ui_menu_t* self = (ui_menu_t*)parg;

	for ( i = 0; i < UI_MULTI_ITEM_MAX; i++ )
	{

	}
}

//事件回调函数
static void ui_menu_callback ( void *parg )
{
	ui_menu_t *self = ( ui_menu_t * ) parg;

	if ( self->event.event & ewm_init )
	{
		self->event.event &= ~ewm_init;
		self->event.event |= ewm_paint;
	}
	else if ( self->event.event & ewm_paint )
	{
		self->event.paint ( self );
		self->event.event &= ~ewm_paint;
	}
	else if ( self->event.event & ewm_update )
	{
		self->event.event &= ~ewm_update;
	}
	else if ( self->event.event & ewm_time )
	{
		//win->event.event &= ~ewm_time;		//周期运行
	}
	else if ( self->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_resc:
			case ewk_esc:
				ui_widget_go_prev_win ( self->prev );
				break;

			case ewk_rdown:
			case ewk_down:

				self->event.event |= ewm_paint;
				break;

			case ewk_rok:
			case ewk_ok:

				self->event.event |= ewm_paint;
				break;
		}

		self->event.event &= ~ewm_key;
	}
	else if ( self->event.event & ewm_delete )
	{
		self->event.event &= ~ewm_delete;
	}
}

WM_HWIN ui_menu_create ( ui_widgets_t *parent, ui_menu_t *self )
{
	u16 i, r, c;
	u16 rows;

	if ( self == 0 )
		return 0;

	self->prev = parent;
	self->event.paint = ui_menu_draw_callback;
	self->event.cb = ui_menu_callback;
	self->event.event |= ewm_init;		//初始化事件

	if ( self->total )
	{
		rows = self->pos.height / UI_MULTI_ITEM_HEIGHT;

		//		columns = self->pos.width / UI_MULTI_ITEM_WIDTH;
		for ( i = 0, r = 0, c = 0; i < UI_MULTI_ITEM_MAX; i++ )
		{


			r++;

			if ( r >= rows )
			{
				r = 0;
				c++;
			}
		}
	}

	ui_widgets_set_top_win ( ( ui_widgets_t * ) self );		//很重要，一定要有这一句，第一个运行的窗口

	return self;
}

/**********************
  **********************/
