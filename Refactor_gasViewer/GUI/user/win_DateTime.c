/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "appConfig.h"
#include "rtc.h"
#include "ui_edit.h"
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

static ui_window_t WinDateTime =
{
	.pos.x = 0,
	.pos.y = 24,
	.pos.width = 320,
	.pos.height = 240 - 24 * 2,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static u16 ItemSelected = 0;
#define LINE_TOTAL		2
static u8 ItemList[LINE_TOTAL];

static u8 Year;
static u8 Month;
static u8 Day;
static u8 Hour;
static u8 Minute;
static u32 Time_10;

// 日期编辑字段索引：0=年，1=月，2=日，3=时，4=分
static u8 DateEditIndex = 0;
// 编辑模式标志
static u8 DateEditing = 0;

//创建控件
static void Win_Init ( void )
{
	MainWin_UpdateTitleBar("设置日期时间");
	MainWin_UpdateStatusBar("返回", "->", "修改");
	
	ItemSelected = 0;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;

	Year = gDateTime.Year;
	if(Year > 99)
		Year = 0;
	
	Month = gDateTime.Month;
	if(Month < 1 || Month > 12)
		Month = 1;
	
	Day = gDateTime.Day;
	if(Day < 1 || Day > 31)
		Day = 1;
	
	Hour = gDateTime.Hours;
	if(Hour > 23)
		Hour = 0;
	
	Minute = gDateTime.Minutes;
	if(Minute > 59 || Minute < 1)
		Minute = 1;
	// 重置编辑状态
  DateEditIndex = 0;
	  DateEditing = 0;
}

#define LINE1_Y			(24 + 96 * 0)
#define LINE2_Y			(24 + 96 * 1)
#define LINE1_X			(52 + 48 * 0)
#define LINE2_X			(52 + 48 * 1)
#define LINE3_X			(52 + 48 * 2)
#define LINE4_X			(52 + 48 * 3)
#define LINE5_X			(52 + 48 * 4)


#define LINE_H			(95)
#define LABLE_W			(320)
#define DATA_W			(320)


static void Win_Paint ( void )
{
	ui_show_lable(0, LINE1_Y, LABLE_W, LINE_H, "年-月-日-时-分", efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hcenter | eat_vcenter ));
}

static u16 SelectColor(u16 index)
{
	u16 bcolor;
	
	if(ItemList[index % LINE_TOTAL] == 2)
	{	
		bcolor = rgb565_focused_background;
	}
	else if(ItemList[index % LINE_TOTAL] == 1)
	{
		ItemList[index % LINE_TOTAL] = 0;
	}
	
	if(ItemList[index % LINE_TOTAL] != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line1;
		else
			bcolor = rgb565_grid_line2;
	}
	
	return bcolor;
}

static void ShowLine1(void)
{
	char string[32];
	u16 bcolor;
	
	bcolor = SelectColor(0);
	snprintf(string, sizeof(string), "20%02d年%02d月%02d日%02d时%02d分", Year,Month,Day,Hour,Minute);
	ui_show_lable(0, LINE2_Y, DATA_W, LINE_H, string, efsize_24, rgb565_lable_text, bcolor, ( eat_hcenter | eat_vcenter ));		
}

// 解析10位数字（YYMMDDHHMM）到年月日时分
static void parse_10digit_time(u32 input)
{
	// 拆分逻辑：从左到右每2位拆分
	Year    = (input / 100000000) % 100;    // 前2位：年份（YY）
	Month   = (input / 1000000) % 100;      // 第3-4位：月份（MM）
	Day     = (input / 10000) % 100;        // 第5-6位：日期（DD）
	Hour    = (input / 100) % 100;          // 第7-8位：小时（HH）
	Minute  = input % 100;                  // 第9-10位：分钟（MM）
	
	// 边界校验
	Year    = (Year > 99) ? 99 : Year;
	Month   = (Month < 1 || Month > 12) ? 1 : Month;
	if(Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12)
	{
		if(Day > 31)
			Day = 31;
	}
	else if(Month == 4 || Month == 6 || Month == 9 || Month == 11)
	{
		if(Day > 30)
			Day = 30;
	}
	else if(Month == 2 && Year % 4 ==0)
	{
		if(Day > 29)
			Day = 29;
	}				
	else if(Month == 2 && Year % 4 !=0)
	{
		if(Day > 28)
			Day = 28;
	}						
	Hour    = (Hour > 23) ? 23 : Hour;
	Minute  = (Minute > 59) ? 59 : Minute;
}

static void Win_ModifyPaint ( void )
{
	ShowLine1();
}

// 保存当前编辑字段的值
static void SaveCurrentField(void) 
{
	Local_BinTimeTypeDef dt;
  switch (DateEditIndex) 
	{
		case 0:
			Year = g_Digital.udata;
			break;
		case 1:
			Month = g_Digital.udata;
			if(Month > 12)
				Month = 12;
			break;
		case 2:		//
			Day = g_Digital.udata;
			if(Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12)
			{
				if(Day > 31)
					Day = 31;
			}
			else if(Month == 4 || Month == 6 || Month == 9 || Month == 11)
			{
				if(Day > 30)
					Day = 30;
			}
			else if(Month == 2 && Year % 4 ==0)
			{
				if(Day > 29)
					Day = 29;
			}				
			else if(Month == 2 && Year % 4 !=0)
			{
				if(Day > 28)
					Day = 28;
			}						
			break;
		case 3:
			Hour = g_Digital.udata;
			if(Hour > 23)
				Hour = 23;
			break;
		case 4:
			Minute = g_Digital.udata;
			if(Minute > 59)
				Minute = 59;
			break;
  }
	if(DateEditIndex==5)
	{
		dt.Year = Year;
		dt.Month = Month;
		dt.Day = Day;
		dt.Hours = Hour;
		dt.Minutes = Minute;
		dt.Seconds = 30;

		LocalTime_SetRTC(&dt);
	}
}

// 创建当前字段的编辑框
static void CreateFieldEdit(ui_window_t *win) 
{
	switch (DateEditIndex)
	{
		case 0:
			g_Digital.udata = Year;
			ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE1_X, LINE2_Y, 24, LINE_H, efsize_24);
		break;
		case 1:
			g_Digital.udata = Month;
			ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE2_X, LINE2_Y, 24, LINE_H, efsize_24);
		break;
		case 2:
			g_Digital.udata = Day;
			ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE3_X, LINE2_Y, 24, LINE_H, efsize_24);
		break;
		case 3:
			g_Digital.udata = Hour;
			ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE4_X, LINE2_Y, 24, LINE_H, efsize_24);
		break;
		case 4:
			g_Digital.udata = Minute;
			ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE5_X, LINE2_Y, 24, LINE_H, efsize_24);
		break;
	}
}


//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;
	Local_BinTimeTypeDef dt;

	if ( win->event.event & ewm_init )
	{
		Win_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
	}
	else if ( win->event.event & ewm_paint )
	{
		win->event.paint ( win );
		win->event.event &= ~ewm_paint;
		Win_Paint();
		Win_ModifyPaint();
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//二级
						switch(ItemSelected)
						{
							case 0:		//年
							  if (!DateEditing) 
								{
									// 开始编辑，从年开始
									DateEditing = 1;
									DateEditIndex = 0;
								}
								CreateFieldEdit(win);
								break;
						}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:
					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					if (DateEditing) 
					{
						// 在编辑模式下按返回，保存当前字段并退出
//							SaveCurrentField();
							DateEditing = 0;
							DateEditIndex = 0;
					} 
					else
					{
						ui_widget_go_prev_win(win->prev); // 返回上一窗口
					}
				break;
			}
		Win_ModifyPaint();
		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		g_Digital.udata = g_EditData.udata;
		SaveCurrentField();
		// 移动到下一字段
		DateEditIndex++;
		if (DateEditIndex < 5) 
		{
			// 继续编辑下一字段
			CreateFieldEdit(win);
		} 
		else 
		{
			// 所有字段编辑完成，退出编辑模式
			DateEditing = 0;
			DateEditIndex = 0;
		}
		MainWin_UpdateStatusBar("返回", "->", "修改");
		Win_ModifyPaint();
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
}


/*
* 创建第一个窗口
*/
void WinDateTime_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinDateTime, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinDateTime );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
