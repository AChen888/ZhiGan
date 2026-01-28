/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_product.h"
/*********************
 *      DEFINES
 *********************/
// 菜单布局常量：2行3列（6个图标）

#define AUTH0_MENU_ROWS   	1    // 总行数
#define AUTH0_MENU_COLS   	3    // 总列数
#define AUTH1_MENU_ROWS   	2    // 总行数
#define AUTH1_MENU_COLS    	3    // 总列数
#define AUTH2_MENU_ROWS    	2    // 总行数
#define AUTH2_MENU_COLS    	3    // 总列数
#define AUTH0_ICON_COUNT 		3
#define AUTH1_ICON_COUNT 		5
#define AUTH2_ICON_COUNT 		6
/**********************
*      TYPEDEFS
**********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static ui_window_t MainMenu =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

static ui_window_t MainMenu_N =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 -30 * 2,
};

static ui_window_t MainMenu_A =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};
/* 100 X 96*/

static ui_icon_t Icon1_N =
{
	.pos.x = 10 + 100 * 0,
	.pos.y = 30 + 43,
	.name.text.text = "零点校准", 
	.icon.text.text = "⑴",
};

static ui_icon_t Icon4_N =
{
	.pos.x = 10 + 100 * 1,
	.pos.y = 30 + 43,
	.name.text.text = "数据记录",
	.icon.text.text = "⑷",
};

static ui_icon_t Icon6_N =
{
	.pos.x = 10 + 100 * 2,
	.pos.y = 30 + 43,
	.name.text.text = "系统设置",
	.icon.text.text = "⑹",
};

static ui_icon_t Icon1_A =
{
	.pos.x = 10 + 100 * 0,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "零点校准",
	.icon.text.text = "⑴",
};
static ui_icon_t Icon2_A =
{
	.pos.x = 10 + 100 * 1,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "浓度校准",
	.icon.text.text = "⑵",
};
static ui_icon_t Icon3_A =
{
	.pos.x = 10 + 100 * 2,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "报警设置",
	.icon.text.text = "⑶",
};
static ui_icon_t Icon4_A =
{
	.pos.x = 10 + 100 * 0,
	.pos.y = 30 + 86 * 1,
	.name.text.text = "数据记录",
	.icon.text.text = "⑷",
};

static ui_icon_t Icon6_A =
{
	.pos.x = 10 + 100 * 1,
	.pos.y = 30 + 86 * 1,
	.name.text.text = "系统设置",
	.icon.text.text = "⑹",
};

static ui_icon_t Icon1 =
{
	.pos.x = 10 + 100 * 0,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "零点校准",
	.icon.text.text = "⑴",
};
static ui_icon_t Icon2 =
{
	.pos.x = 10 + 100 * 1,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "浓度校准",
	.icon.text.text = "⑵",
};
static ui_icon_t Icon3 =
{
	.pos.x = 10 + 100 * 2,
	.pos.y = 30 + 86 * 0,
	.name.text.text = "报警设置",
	.icon.text.text = "⑶",
};

static ui_icon_t Icon4 =
{
	.pos.x = 10 + 100 * 0,
	.pos.y = 30 + 86 * 1,
	.name.text.text = "数据记录",
	.icon.text.text = "⑷",
};

static ui_icon_t Icon5 =
{
	.pos.x = 10 + 100 * 1,
	.pos.y = 30 + 86 * 1,
	.name.text.text = "生产设置",
	.icon.text.text = "⑸",
};
static ui_icon_t Icon6 =
{
	.pos.x = 10 + 100 * 2,
	.pos.y = 30 + 86 * 1,
	.name.text.text = "系统设置",
	.icon.text.text = "⑹",
};

// 根据当前选中索引，计算对应的行和列
static void get_row_col(int index, int *row, int *col) {
    *row = index / AUTH2_MENU_COLS;  // 行 = 索引 ÷ 列数（整数除法）
    *col = index % AUTH2_MENU_COLS;  // 列 = 索引 % 列数（取余）
}

// 根据行列，计算对应的选中索引
static int get_index(int row, int col) {
    return row * AUTH2_MENU_COLS + col;  // 索引 = 行×列数 + 列
}


const ui_widgets_t *MainMenuWidgetsList[] =
{
	( ui_widgets_t * ) &Icon1,
	( ui_widgets_t * ) &Icon2,
	( ui_widgets_t * ) &Icon3,
	( ui_widgets_t * ) &Icon4,
	( ui_widgets_t * ) &Icon5,
	( ui_widgets_t * ) &Icon6,
	0					//结束
};

const ui_widgets_t *MainMenuWidgetsList_N[] =
{
	( ui_widgets_t * ) &Icon1_N,
	( ui_widgets_t * ) &Icon4_N,
	( ui_widgets_t * ) &Icon6_N,
	0					//结束
};

const ui_widgets_t *MainMenuWidgetsList_A[] =
{
	( ui_widgets_t * ) &Icon1_A,
	( ui_widgets_t * ) &Icon2_A,
	( ui_widgets_t * ) &Icon3_A,
	( ui_widgets_t * ) &Icon4_A,
	( ui_widgets_t * ) &Icon6_A,
	0					//结束
};

u16 g_Menu1_ItemSelected = 0;
ui_icon_t *IconList_N[] =
{
	( ui_icon_t * ) &Icon1_N,
	( ui_icon_t * ) &Icon4_N,
	( ui_icon_t * ) &Icon6_N,
};

ui_icon_t *IconList_A[] =
{
	( ui_icon_t * ) &Icon1_A,
	( ui_icon_t * ) &Icon2_A,
	( ui_icon_t * ) &Icon3_A,
	( ui_icon_t * ) &Icon4_A,
	( ui_icon_t * ) &Icon6_A,
};

ui_icon_t *IconList[] =
{
	( ui_icon_t * ) &Icon1,
	( ui_icon_t * ) &Icon2,
	( ui_icon_t * ) &Icon3,
	( ui_icon_t * ) &Icon4,
	( ui_icon_t * ) &Icon5,
	( ui_icon_t * ) &Icon6
};

//创建控件
static void MainMenu_InitWidgetsList ( void )
{
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon1 );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon2 );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon3 );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon4 );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon5 );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu, &Icon6 );
	g_Menu1_ItemSelected = 0;
	IconList[g_Menu1_ItemSelected]->selected = 1;
}

//创建控件
static void MainMenu_InitWidgetsList_N ( void )
{
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_N, &Icon1_N );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_N, &Icon4_N );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_N, &Icon6_N );
	g_Menu1_ItemSelected = 0;
	IconList_N[g_Menu1_ItemSelected]->selected = 1;
}

//创建控件
static void MainMenu_InitWidgetsList_A ( void )
{
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_A, &Icon1_A );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_A, &Icon2_A );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_A, &Icon3_A );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_A, &Icon4_A );
	ui_icon_create ( ( ui_widgets_t * ) &MainMenu_A, &Icon6_A );
	g_Menu1_ItemSelected = 0;
	IconList_A[g_Menu1_ItemSelected]->selected = 1;
}

//事件回调函数
static void MainMenu_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		MainMenu_InitWidgetsList();
		MainMenu.sublist = ( ui_widgets_t ** ) &MainMenuWidgetsList[0];

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		
		MainWin_UpdateTitleBar("系统菜单");
		MainWin_UpdateStatusBar("返回", "下移", "确认");
	}
	else if ( win->event.event & ewm_update )
	{
		ui_widget_update((ui_widgets_t*)win);

		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_rok:
			case ewk_ok:
				switch(g_Menu1_ItemSelected)
				{
					case menu1_Zero:
//						WinZero_Create(win);	//返回时跳过这个窗口
						break;
					case menu1_Calib:
//						WinCalib_Create(win);
						break;
					case menu1_Alarm:
//						WinAlarm_Create(win);
						break;
					case menu1_Record:
//						WinRecord_Create(win);
						break;
					case menu1_Product:
						WinProduct_Create(win);
						break;
					case menu1_System:
						if(ZhuYe_Flag==0)
							WinSystemSet_Create_Yes_NoZhu(win);
						else
							WinSystemSet_Create(win);
					break;
				default:
					break;
				}
				break;
			case ewk_rup:
				IconList[g_Menu1_ItemSelected]->selected = 0;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_up, col_up;
				get_row_col(g_Menu1_ItemSelected, &row_up, &col_up);
				row_up--;
				if (row_up <0) 
				{
					row_up = AUTH2_MENU_ROWS - 1;
				}
				g_Menu1_ItemSelected = get_index(row_up, col_up);

				IconList[g_Menu1_ItemSelected]->selected = 1;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
			
			case ewk_left:
				IconList[g_Menu1_ItemSelected]->selected = 0;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_left, col_left;
				get_row_col(g_Menu1_ItemSelected, &row_left, &col_left);
				if (col_left > 0) 
				{
					// 场景1：非第一列 → 仅列减1
					col_left--;
				} 
				else 
				{
					// 场景2：第一列 → 按行区分跳转目标
					if (row_left == 0) {
							// 第一行第一列 → 跳最后一行最后一列（第二行第三列）
							row_left = AUTH2_MENU_ROWS - 1;  // 行=1（最后一行）
							col_left = AUTH2_MENU_COLS - 1;  // 列=2（最后一列）
					} else {
							// 第二行第一列 → 跳第一行最后一列（第一行第三列）
							row_left = 0;              // 行=0（第一行）
							col_left = AUTH2_MENU_COLS - 1;  // 列=2（最后一列）
					}
				}
				g_Menu1_ItemSelected = get_index(row_left, col_left);
				
				IconList[g_Menu1_ItemSelected]->selected = 1;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_down:
			case ewk_right:
				IconList[g_Menu1_ItemSelected]->selected = 0;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_right, col_right;
				get_row_col(g_Menu1_ItemSelected, &row_right, &col_right);
				
				if (col_right < AUTH2_MENU_COLS - 1) 
				{
					// 场景1：非最后一列 → 仅列加1
					col_right++;
				} 
				else 
				{
					// 场景2：最后一列 → 按行区分跳转目标（与向左键镜像）
					if (row_right == 0) {
							// 第一行最后一列 → 跳第二行第一列（第二行第一列）
							row_right = AUTH2_MENU_ROWS - 1;  // 行=1（最后一行）
							col_right = 0;              // 列=0（第一列）
					} else {
							// 第二行最后一列 → 跳第一行第一列（第一行第一列）
							row_right = 0;              // 行=0（第一行）
							col_right = 0;              // 列=0（第一列）
					}
				}
				
				// 更新选中索引和高亮状态
				g_Menu1_ItemSelected = get_index(row_right, col_right);
				
				IconList[g_Menu1_ItemSelected]->selected = 1;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_rdown:
				IconList[g_Menu1_ItemSelected]->selected = 0;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_down, col_down;
				get_row_col(g_Menu1_ItemSelected, &row_down, &col_down);
				row_down++;
				if (row_down >= AUTH2_MENU_ROWS) 
				{
					row_down = 0;
				}
				g_Menu1_ItemSelected = get_index(row_down, col_down);

				IconList[g_Menu1_ItemSelected]->selected = 1;
				IconList[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
			case ewk_resc:
			case ewk_esc:
				Pape_Flag &= ~(1 << 0);
				ui_widget_go_prev_win ( win->prev );
				break;
			case ewk_C:
				break;
		}
		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}

static void MainMenu_Callback_N ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		MainMenu_InitWidgetsList_N();
		MainMenu_N.sublist = ( ui_widgets_t ** ) &MainMenuWidgetsList_N[0];

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		
		MainWin_UpdateTitleBar("系统菜单");
		MainWin_UpdateStatusBar("返回", "下移", "确认");
	}
	else if ( win->event.event & ewm_update )
	{
		ui_widget_update((ui_widgets_t*)win);

		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_rok:
			case ewk_ok:
				switch(g_Menu1_ItemSelected)
				{
					case menu1_Zero_N:
//						WinZero_Create(win);	//返回时跳过这个窗口
						break;
					case menu1_Record_N:
//						WinRecord_Create(win);
						break;
					case menu1_System_N:
						WinSystemSet_Create_No(win);
						break;
					default:
						break;
				}
				break;
			case ewk_rup:
				// 取消当前选中状态
				IconList_N[g_Menu1_ItemSelected]->selected = 0;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				if ( g_Menu1_ItemSelected == 0 )
				{
					g_Menu1_ItemSelected = AUTH0_ICON_COUNT - 1;
				}
				else if(g_Menu1_ItemSelected)
				{
					g_Menu1_ItemSelected--;
				}
				
				// 设置新选中状态
				IconList_N[g_Menu1_ItemSelected]->selected = 1;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
			break;
			
			case ewk_left:
				IconList_N[g_Menu1_ItemSelected]->selected = 0;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				if ( g_Menu1_ItemSelected == 0 )
				{
					g_Menu1_ItemSelected = AUTH0_ICON_COUNT - 1;
				}
				else if(g_Menu1_ItemSelected)
				{
					g_Menu1_ItemSelected--;
				}
				// 设置新选中状态
				IconList_N[g_Menu1_ItemSelected]->selected = 1;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_down:
			case ewk_right:
				IconList_N[g_Menu1_ItemSelected]->selected = 0;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				g_Menu1_ItemSelected++;
				if ( g_Menu1_ItemSelected == 3 )
				{
					g_Menu1_ItemSelected = 0;
				}
				// 设置新选中状态
				IconList_N[g_Menu1_ItemSelected]->selected = 1;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_rdown:
				IconList_N[g_Menu1_ItemSelected]->selected = 0;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				g_Menu1_ItemSelected++;
				if ( g_Menu1_ItemSelected == 3 )
				{
					g_Menu1_ItemSelected = 0;
				}

				// 设置新选中状态
				IconList_N[g_Menu1_ItemSelected]->selected = 1;
				IconList_N[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
			case ewk_resc:
			case ewk_esc:
				Pape_Flag &= ~(1 << 0);
				ui_widget_go_prev_win ( win->prev );
				break;
			case ewk_C:
//			SelAuth_Create(win, win);
				break;
		}
		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}

static void MainMenu_Callback_A ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

	if ( win->event.event & ewm_init )
	{
		MainMenu_InitWidgetsList_A();
		MainMenu_A.sublist = ( ui_widgets_t ** ) &MainMenuWidgetsList_A[0];

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		
		MainWin_UpdateTitleBar("系统菜单");
		MainWin_UpdateStatusBar("返回", "下移", "确认");
	}
	else if ( win->event.event & ewm_update )
	{
		ui_widget_update((ui_widgets_t*)win);

		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_key )
	{
		switch ( ui_GetInputKey ( 1 ) )
		{
			case ewk_rok:
			case ewk_ok:
				switch(g_Menu1_ItemSelected)
				{
					case menu1_Zero_A:
//						WinZero_Create(win);	//返回时跳过这个窗口
						break;
					case menu1_Calib_A:
//						WinCalib_Create(win);
						break;
					case menu1_Alarm_A:
//						WinAlarm_Create(win);
						break;
					case menu1_Record_A:
//						WinRecord_Create(win);
						break;
				case menu1_System_A:
					if(ZhuYe_Flag==0)
						WinSystemSet_Create_Yes_NoZhu(win);
					else
						WinSystemSet_Create(win);
					break;
				default:
					break;
				}
				break;
			case ewk_rup:
				// 取消当前选中状态
				IconList_A[g_Menu1_ItemSelected]->selected = 0;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_up, col_up;
				get_row_col(g_Menu1_ItemSelected, &row_up, &col_up);
				row_up--;
				if(col_up < 2)
				{
					if (row_up <0) 
					{
						row_up = AUTH1_MENU_ROWS - 1;
						g_Menu1_ItemSelected = get_index(row_up, col_up);
					}
					else
					{
						g_Menu1_ItemSelected = get_index(row_up, col_up);
					}
				}
			
				// 设置新选中状态
				IconList_A[g_Menu1_ItemSelected]->selected = 1;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
			
			case ewk_left:
				IconList_A[g_Menu1_ItemSelected]->selected = 0;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;

				if ( g_Menu1_ItemSelected == 0 )
				{
					g_Menu1_ItemSelected = AUTH1_ICON_COUNT - 1;
				}
				else if(g_Menu1_ItemSelected)
				{
					g_Menu1_ItemSelected--;
				}

				// 设置新选中状态
				IconList_A[g_Menu1_ItemSelected]->selected = 1;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_down:
			case ewk_right:
				IconList_A[g_Menu1_ItemSelected]->selected = 0;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				g_Menu1_ItemSelected++;
				if ( g_Menu1_ItemSelected == 5 )
				{
					g_Menu1_ItemSelected = 0;
				}
				// 设置新选中状态
				IconList_A[g_Menu1_ItemSelected]->selected = 1;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
				
			case ewk_rdown:
				IconList_A[g_Menu1_ItemSelected]->selected = 0;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				
				int row_down, col_down;
				get_row_col(g_Menu1_ItemSelected, &row_down, &col_down);
				row_down++;
				if(col_down <2)
				{
					if (row_down >= AUTH1_MENU_ROWS) 
					{
						row_down = 0;
					}
					g_Menu1_ItemSelected = get_index(row_down, col_down);
				}
				// 设置新选中状态
				IconList_A[g_Menu1_ItemSelected]->selected = 1;
				IconList_A[g_Menu1_ItemSelected]->event.event |= ewm_paint;
				win->event.event |= ewm_update;
				break;
			case ewk_resc:
			case ewk_esc:
				Pape_Flag &= ~(1 << 0);
				ui_widget_go_prev_win ( win->prev );
				break;
			case ewk_C:
//			SelAuth_Create(win, win);
				break;

		}
		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}

/*
* 创建第一个窗口
*/
void MainMenu_Create ( void *parg )
{
	( void ) parg;
	Pape_Flag|=(1<<0);
	ui_window_create ( ( ui_window_t * ) parg, &MainMenu, MainMenu_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &MainMenu );		//很重要，一定要有这一句，第一个运行的窗口
}

void MainMenu_Create_N ( void *parg )
{
	( void ) parg;
	Pape_Flag|=(1<<0);
	ui_window_create ( ( ui_window_t * ) parg, &MainMenu_N, MainMenu_Callback_N );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &MainMenu_N );		//很重要，一定要有这一句，第一个运行的窗口
}

void MainMenu_Create_A ( void *parg )
{
	( void ) parg;
	Pape_Flag|=(1<<0);
	ui_window_create ( ( ui_window_t * ) parg, &MainMenu_A, MainMenu_Callback_A );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &MainMenu_A );		//很重要，一定要有这一句，第一个运行的窗口
}



/**********************
  **********************/
