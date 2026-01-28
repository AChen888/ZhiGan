/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "win_Calib.h"
#include "appConfig.h"
//#include "App\Gas\gas.h"
#include "rtc.h"
//#include "gasSample.h"
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

static ui_window_t WinInformation =
{
	.pos.x = 0,
	.pos.y = 30,
	.pos.width = 320,
	.pos.height = 240 - 30 * 2,
};

/**********************
 *      MACROS
 **********************/
#define LINE_TOTAL		5
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static u16 CalibTimeout;
static u32 Gname;

static u16 ItemSelected = 0;
static u8 ItemList[LINE_TOTAL];
static u8 Year;
static u8 Month;
static u8 Day;
static u8 Hour;
static u8 Minute;

// 日期编辑字段索引：0=年，1=月，2=日，3=时，4=分
static u8 DateEditIndex = 0;
// 编辑模式标志
static u8 DateEditing = 0;

u8 Gas_SNname(u32 SN)
{
	u8 i;
	u8 j;
	u8 z;
	u32 Name_SN;
	Name_SN = (gSystemData.name_sn%100000000)%10000000;
	i=SN/100000000%10;
	j=SN/10000000%10;
	z=i*10+j;
	switch(z)              //考虑输错的情况
	{
		    case 1:
					sprintf(gSystemData.name, "A%d", Name_SN);
			break;
				case 2:
					sprintf(gSystemData.name, "B%d", Name_SN);
			break;
				case 3:
					sprintf(gSystemData.name, "C%d", Name_SN);
			break;
				case 4:
					sprintf(gSystemData.name, "D%d", Name_SN);
			break;
				case 5:
					sprintf(gSystemData.name, "E%d", Name_SN);
			break;
				case 6:
					sprintf(gSystemData.name, "F%d", Name_SN);
			break;
				case 7:
					sprintf(gSystemData.name, "G%d", Name_SN);
			break;
				case 8:
					sprintf(gSystemData.name, "H%d", Name_SN);
			break;
				case 9:
					sprintf(gSystemData.name, "I%d", Name_SN);
			break;
				case 10:
					sprintf(gSystemData.name, "J%d", Name_SN);
			break;
				case 11:
					sprintf(gSystemData.name, "K%d", Name_SN);
			break;
				case 12:
					sprintf(gSystemData.name, "L%d", Name_SN);
			break;
				case 13:
					sprintf(gSystemData.name, "M%d", Name_SN);
			break;
				case 14:
					sprintf(gSystemData.name, "N%d", Name_SN);
			break;
				case 15:
					sprintf(gSystemData.name, "O%d", Name_SN);
			break;
				case 16:
					sprintf(gSystemData.name, "P%d", Name_SN);
			break;
				case 17:
					sprintf(gSystemData.name, "Q%d", Name_SN);
			break;
				case 18:
					sprintf(gSystemData.name, "R%d", Name_SN);
			break;
				case 19:
					sprintf(gSystemData.name, "S%d", Name_SN);
			break;
				case 20:
					sprintf(gSystemData.name, "T%d", Name_SN);
			break;
				case 21:
					sprintf(gSystemData.name, "U%d", Name_SN);
			break;
				case 22:
					sprintf(gSystemData.name, "V%d", Name_SN);
			break;
				case 23:
					sprintf(gSystemData.name, "W%d", Name_SN);
			break;
				case 24:
					sprintf(gSystemData.name, "X%d", Name_SN);
			break;
				case 25:
					sprintf(gSystemData.name, "Y%d", Name_SN);
			break;
				case 26:
					sprintf(gSystemData.name, "Z%d", Name_SN);
			break;
				default:
			break;
			}

}


//创建控件
static void Win_Init ( void )
{
	char string[32];
	CalibTimeout = 0;
	
	WinInformation.sublist = 0;
	WinInformation.tick.step = 0;
	WinInformation.tick.period = 999;
	WinInformation.tick.lasttick = HAL_GetTick();
	
	ItemSelected = 2;
	memset(ItemList, 0, sizeof(ItemList));
	ItemList[ItemSelected] = 2;
	Gname = gSystemData.name_sn;
	
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
	
	snprintf(string, sizeof(string), "设备信息");
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "下移", "修改");
}

#define LINE_Y(n)		(30 + 30 * n)

#define LINE_H			(30)
#define LABLE_W			(80)
#define DATA_W			(320)
#define UNIT_W			(12*5)

#define DATA_X			(160)
#define UNIT_X			(320 - UNIT_W)

#define LINE1_X			(58 + 48 * 0)
#define LINE2_X			(58 + 48 * 1)
#define LINE3_X			(58 + 48 * 2)
#define LINE4_X			(58 + 48 * 3)
#define LINE5_X			(58 + 48 * 4)


static void Win_Paint ( void )
{
	//char string[32];
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	ui_show_lable(0, LINE_Y(0), LABLE_W, LINE_H, "Type:",efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(1), LABLE_W, LINE_H, "Ver:", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(2), LABLE_W, LINE_H, "SN:",  efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
	ui_show_lable(0, LINE_Y(3), DATA_W, LINE_H, "时间:", efsize_24, rgb565_lable_text, rgb565_grid_line2, ( eat_hleft | eat_vcenter ));
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
		bcolor = rgb565_grid_line1;
	}
	if(ItemList[index % LINE_TOTAL] != 2)
	{
		if(index & 0x001)
			bcolor = rgb565_grid_line2;
		else
			bcolor = rgb565_grid_line1;
	}
	return bcolor;
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
	if(DateEditing)
	{
		switch (DateEditIndex)
		{
			case 0:
				g_Digital.udata = Year;
				ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE1_X, LINE_Y(4), 24, LINE_H, efsize_24);
			break;
			case 1:
				g_Digital.udata = Month;
				ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE2_X, LINE_Y(4), 24, LINE_H, efsize_24);
			break;
			case 2:
				g_Digital.udata = Day;
				ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE3_X, LINE_Y(4), 24, LINE_H, efsize_24);
			break;
			case 3:
				g_Digital.udata = Hour;
				ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE4_X, LINE_Y(4), 24, LINE_H, efsize_24);
			break;
			case 4:
				g_Digital.udata = Minute;
				ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_2, LINE5_X, LINE_Y(4), 24, LINE_H, efsize_24);
			break;
		}
	}
}



static void ShowLine(u16 line, u8 drawback)
{
	char string[32];
	Local_BinTimeTypeDef*	time;
	u16 bcolor;
	u16 life;
	life= 480-gSystemData.Life;
	if(gSystemData.Life>480)
		life=0;
	bcolor = SelectColor(line);	
	
	switch(line)
	{
		case 0:
	   	snprintf(string, sizeof(string), "%s", GetDeviceType());
			ui_show_lable(80, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case 1:
			snprintf(string, sizeof(string), "%s", GetSoftwareVersion());
			ui_show_lable(80, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 2:
			snprintf(string, sizeof(string), "*%s", gSystemData.name);
			ui_show_lable(80, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case 4:
			snprintf(string, sizeof(string), "*20%02d年%02d月%02d日%02d时%02d分", Year,Month,Day,Hour,Minute);
			ui_show_lable(0, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hcenter | eat_vcenter ));
			break;
		case 5:	
			snprintf(string, sizeof(string), "剩余寿命 %d 天",  life);
			ui_show_lable(0, LINE_Y(line), DATA_W, LINE_H, string, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
	}
}

static void Win_ModifyPaint ( void )
{
	ShowLine(0, 1);
	ShowLine(1, 1);
	ShowLine(2, 1);
	ShowLine(3, 1);
	ShowLine(4, 1);
	ShowLine(5, 1);
}

//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;
	float temp;

	if ( win->event.event & ewm_init )
	{
		Win_Init();

		win->event.event &= ~ewm_init;
		win->event.event |= ewm_paint;
		win->event.event |= ewm_time;
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
		if(CalibTimeout == 0)
		{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:
						if(AuthCheck() == 0)
						{
							if(ItemSelected==2)
							{
								ui_show_lable(80, LINE_Y(ItemSelected), DATA_W, LINE_H, "没有权限", efsize_24, rgb565_draw_yellow, rgb565_win_background, ( eat_hleft | eat_vcenter ));
								osDelay(1000);
							}
							else if(ItemSelected==4)
							{
								ui_show_lable(0, LINE_Y(ItemSelected), DATA_W, LINE_H, "没有权限", efsize_24, rgb565_draw_yellow, rgb565_win_background, ( eat_hcenter | eat_vcenter ));
								osDelay(1000);
							}
							break;
						}
						switch(ItemSelected)
						{
							case 2:
								g_Digital.udata = gSystemData.name_sn;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_9, 80, LINE_Y(2), DATA_W, LINE_H, efsize_24);
								break;
							case 4:		//年
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
					ItemList[ItemSelected] = 1;
					if(ItemSelected==2)
					{
						ItemSelected=4;
					}
					else if(ItemSelected==4)
					{					
						ItemSelected = 2;
					}
					ItemList[ItemSelected] = 2;
					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:
					ItemList[ItemSelected] = 1;
					if(ItemSelected ==4)
						ItemSelected = 2;
					else if(ItemSelected==2)
					{
						ItemSelected=4;
					}
					ItemList[ItemSelected] = 2;
					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
					ui_widget_go_prev_win(win->prev); // 返回上一窗口
					break;
			}
			
			Win_ModifyPaint();
		}

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		switch(ItemSelected)
		{
			case 2:				//输入的SN码
				if(g_Digital.udata != g_EditData.udata)
				{
					g_Digital.udata = g_EditData.udata;	
					gSystemData.name_sn = g_Digital.udata;
					Gas_SNname(gSystemData.name_sn);
					g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
					if ( g_SaveParamFlag )		//保存参数到fram
					{
						System_SaveCommonData ( g_SaveParamFlag );
						g_SaveParamFlag = 0;
					}
				}
			break;
			case 4:
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
					SaveCurrentField();
					// 所有字段编辑完成，退出编辑模式
					DateEditing = 0;
					DateEditIndex = 0;
				}
			break;
		}
		Win_ModifyPaint();
		MainWin_UpdateStatusBar("返回", "下移", "修改");
		win->event.event &= ~ewm_update;
	}
	else if ( win->event.event & ewm_time )
	{
	}

	else if ( win->event.event & ewm_delete )
	{
		win->event.event &= ~ewm_delete;
	}
	(void)temp;
}


/*
* 创建第一个窗口
*/
void WinInformation_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinInformation, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinInformation );		//很重要，一定要有这一句，第一个运行的窗口
}