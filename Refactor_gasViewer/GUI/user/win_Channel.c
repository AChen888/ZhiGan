/**
 * @file
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ui\ui.h"
#include "App\appConfig.h"
/*********************
 *      DEFINES
 *********************/
#if 0
#define DEBUG_PRINT(...)			log_trace(__VA_ARGS__)
#else
//#define DEBUG_PRINT(...)
#endif
/**********************
*      TYPEDEFS
**********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static ui_window_t WinChannel =
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

static u16 UnitSelected;
static u16 NameSelected;
static u16 ModbusSelected;
static u16 CalibPoint;
static float Ratio1;
static float Ratio2;
static float Ratio3;

#define SCREEN_LINE_MAX		6
static u8 ScreenLine = 0;
static u8 ScreenPage = 0;

#define LINE_Y(n)		(24 + 32 * n)

#define LINE_H			(31)
#define LABLE_W			(320)
#define DATA_W			(160)

enum MENU_ITEM_ID {
	mii_name = 0,
	mii_unit,
	mii_calib_total,
	mii_ratio_a,
	mii_ratio_b,
	mii_ratio_c,
	mii_save,
	mii_clear,
};

enum MENU_ITEM_TYPE {
	mit_string = 0,
	mit_data,
	mit_menu,
	mit_operate,
};

static u16 ItemSelected = 0;
#define MENU_ITEM_TOTAL		7

static t_CustomMenuItem MenuItem_List[MENU_ITEM_TOTAL + 1] =
{
	[0].name = "名称",
	[0].type = mit_string,
	[0].id = mii_name,

	[1].name = "单位",
	[1].type = mit_string,
	[1].id = mii_unit,
	
	[2].name = "校准点数",
	[2].type = mit_data,
	[2].format = edf_uint_1,
	[2].id = mii_calib_total,
	
	[3].name = "系数a",
	[3].type = mit_data,
	[3].format = edf_float_23,
	[3].id = mii_ratio_a,
	
	[4].name = "系数b(ppm)",
	[4].type = mit_data,
	[4].format = edf_sfloat_23,
	[4].id = mii_ratio_b,

	[5].name = "系数c(ppm)",
	[5].type = mit_data,
	[5].format = edf_sfloat_23,
	[5].id = mii_ratio_c,

	[6].name = "设置",
	[6].type = mit_operate,
	[6].id = mii_save,

	[7].name = "",
	[7].type = mit_operate,
	[7].id = mii_clear,
};

//创建控件
static void Win_Init ( void )
{
	char string[32];
	u8 i;
	
	WinChannel.sublist = 0;
	WinChannel.tick.step = 0;
	WinChannel.tick.period = 999;
	WinChannel.tick.lasttick = HAL_GetTick();

	snprintf(string, sizeof(string), "通道-%d 设置", g_SelectChannel + 1);
	MainWin_UpdateTitleBar(string);
	MainWin_UpdateStatusBar("返回", "->", "修改");

	NameSelected = gCalidata[g_SelectChannel].Name;
	UnitSelected = gCalidata[g_SelectChannel].Unit;
	CalibPoint = gSystemData.CaliPoints;
	Ratio1          = gCalidata[g_SelectChannel].Ratio1;
	Ratio2          = gCalidata[g_SelectChannel].Ratio2;
	Ratio3          = gCalidata[g_SelectChannel].Ratio3;
	
	ItemSelected = 0;
	for(i = 0; i < MENU_ITEM_TOTAL; i++)
		MenuItem_List[i].flag = 0;
	MenuItem_List[ItemSelected].flag = 2;

	ScreenPage = 0;
	ScreenLine = 0;
}

static void Show_Title(u8 pos, u8 id)
{
	char* str = "---";
	u8 index = (ScreenPage*SCREEN_LINE_MAX+pos) % MENU_ITEM_TOTAL;
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行

	switch (id)
		{
		case mii_calib_total:
		case mii_ratio_a:
		case mii_ratio_b:
		case mii_ratio_c:
			if(AuthCheck())	
				str = MenuItem_List[index].name;
			goto show_title;
			//break;
			
		case mii_name:
		case mii_unit:
		case mii_save:
			str = MenuItem_List[index].name;
			goto show_title;
			//break;
		
		case mii_clear:
			str = 0;
			goto show_title;
			//break;
		}
		return;
		
show_title:
	ui_show_lable(0, LINE_Y(pos), LABLE_W, LINE_H, str,  efsize_24, rgb565_lable_text, rgb565_grid_line1, ( eat_hleft | eat_vcenter ));
}

static u16 SelectColor(u16 pos)
{
	u16 bcolor;
	u8 index = (ScreenPage*SCREEN_LINE_MAX+pos) % MENU_ITEM_TOTAL;
	
	if(MenuItem_List[index].flag == 2)		//被选中
	{
		bcolor = rgb565_focused_background;		//选中的颜色
	}
	else if(MenuItem_List[index].flag == 1)	//取消选中
	{
		MenuItem_List[index].flag = 0;
	}
	
	if(MenuItem_List[index].flag != 2)	
	{
		bcolor = rgb565_grid_line1;				//未选中的颜色
	}
	
	return bcolor;
}

static void Show_Saved(u8 pos, u8 ok)
{
	u16 bcolor;
	char* text = "保存";

	if(ok)
		text = "成功";
	
	bcolor = SelectColor(pos);
	ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, text, efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));
}

static void Show_Data(u8 pos, u8 id)
{
	char* str = "---";
	u16 bcolor = SelectColor(pos);
	char string[32];
	//u8 index = (ScreenPage*SCREEN_LINE_MAX+pos) % MENU_ITEM_TOTAL;
	if(AuthCheck())	
		str = string;
	// 240 - 24 -24 = 192 = 32 * 6 , 最多显示6行
	switch (id)
		{
		case mii_name:
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, (char*)g_ViewData_Name[NameSelected % evdn_End], efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case mii_unit:
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, (char*)g_ViewData_Unit[UnitSelected % evdu_End], efsize_24, rgb565_lable_text, bcolor, ( eat_hleft | eat_vcenter ));		
			break;
		case mii_calib_total:
			ui_format_string(string, sizeof ( string ), (u32*)&CalibPoint, edf_uint_1, 0);
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, str, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case mii_ratio_a:
			if(AuthCheck())	
				str = string;
			ui_format_string(string, sizeof ( string ), (u32*)&Ratio1, edf_float_23, 0);
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, str, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;	
		case mii_ratio_b:
			if(AuthCheck())	
				str = string;
			ui_format_string(string, sizeof ( string ), (u32*)&Ratio2, edf_sfloat_23, 0);
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, str, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case mii_ratio_c:
			
			ui_format_string(string, sizeof ( string ), (u32*)&Ratio3, edf_sfloat_23, 0);
			ui_show_lable(160, LINE_Y(pos), DATA_W, LINE_H, str, efsize_24, rgb565_digital_front, bcolor, ( eat_hleft | eat_vcenter ));
			break;
		case mii_save:
			Show_Saved(pos, 0);
			break;
		}
}

static void Win_Paint ( void )
{
	if(ScreenPage == 0)
	{
		Show_Title(0, mii_name);
		Show_Title(1, mii_unit);
		Show_Title(2, mii_calib_total);
		Show_Title(3, mii_ratio_a);
		Show_Title(4, mii_ratio_b);
		Show_Title(5, mii_ratio_c);
	}
	else
	{
		Show_Title(0, mii_save);
		Show_Title(1, mii_clear);
		Show_Title(2, mii_clear);
		Show_Title(3, mii_clear);
		Show_Title(4, mii_clear);
		Show_Title(5, mii_clear);
	}
}

static void Win_Update ( void )
{
	if(ScreenPage == 0)
	{
		Show_Data(0, mii_name);
		Show_Data(1, mii_unit);
		Show_Data(2, mii_calib_total);
		Show_Data(3, mii_ratio_a);
		Show_Data(4, mii_ratio_b);
		Show_Data(5, mii_ratio_c);
	}
	else
	{
		Show_Saved(0, 0);
	}
}

static void Win_Timer ( void )
{
	
}

//事件回调函数
static void Win_Callback ( void *parg )
{
	ui_window_t *win = ( ui_window_t * ) parg;

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
		Win_Update();
	}
	else if ( win->event.event & ewm_key )
	{
			switch ( ui_GetInputKey ( 1 ) )
			{
				case ewk_rok:
				case ewk_ok:						//
//					DEBUG_PRINT("id %s, item %d, page %d, pos %d\r\n", MenuItem_List[ItemSelected].name, ItemSelected, ScreenPage, ScreenLine);
						if(AuthCheck() == 0)
						{
							ui_show_lable(160, LINE_Y(ScreenLine), DATA_W, LINE_H, "没有权限", efsize_24, rgb565_draw_yellow, rgb565_win_background, ( eat_hleft | eat_vcenter ));
							osDelay(1000);
							break;
						}
						switch(MenuItem_List[ItemSelected].id)
						{
							case mii_name:		//名称
								NameSelected++;
								if(NameSelected>=evdn_End)
								{
									NameSelected = 54;
									ModbusSelected = catl_name_NMP;
								}
								else
								{
									NameSelected = 71;
									ModbusSelected=	catl_name_DEC;
								}
								break;
							case mii_unit:		//单位
								UnitSelected++;
								if(UnitSelected >= evdu_End)
									UnitSelected = 2;
								break;

							case mii_calib_total:
								//g_Digital.udata = CalibPoint;
								//ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_uint_1, 160, LINE_Y(ScreenLine), DATA_W, LINE_H, efsize_24);
								CalibPoint++;
								if(CalibPoint > 3)
									CalibPoint = 1;
								break;
								
							case mii_ratio_a:
								g_Digital.fdata = Ratio1;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_float_23, 160, LINE_Y(ScreenLine), DATA_W, LINE_H, efsize_24);
								break;
								
							case mii_ratio_b:
								g_Digital.fdata = Ratio2;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_sfloat_23, 160, LINE_Y(ScreenLine), DATA_W, LINE_H, efsize_24);
								break;

							case mii_ratio_c:
								g_Digital.fdata = Ratio3;
								ui_edit_create_ex((ui_widgets_t*)win, &g_EditData, g_Digital, edf_sfloat_23, 160, LINE_Y(ScreenLine), DATA_W, LINE_H, efsize_24);
								break;
								
							case mii_save:		//保存
								gCalidata[g_SelectChannel].Name = NameSelected;
								gCalidata[g_SelectChannel].CATL_Modbus_Name = ModbusSelected;
								gCalidata[g_SelectChannel].Unit = UnitSelected;
								gSystemData.CaliPoints = CalibPoint;
								gCalidata[g_SelectChannel].Ratio1 = Ratio1;
								gCalidata[g_SelectChannel].Ratio2 = Ratio2;
								gCalidata[g_SelectChannel].Ratio3 = Ratio3;
								switch (gCalidata[g_SelectChannel].Unit)
									{
									case evdu_PPM:
										gCalidata[g_SelectChannel].Format = edf_float_23;
										break;
									case evdu_ppb:
										gCalidata[g_SelectChannel].Format = edf_float_50;
										break;
									case evdu_mg_m3:
										gCalidata[g_SelectChannel].Format = edf_float_32;
										break;
									}
								
								g_SaveParamFlag |= SAVE_CALIDATA_BIT;
											if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
								g_SaveParamFlag |= SAVE_SYSTDATA_BIT;
				if ( g_SaveParamFlag )		//保存参数到fram
		{
			System_SaveCommonData ( g_SaveParamFlag );
			g_SaveParamFlag = 0;
		}
								Show_Saved(0, 1);
								osDelay(1000);
								break;
						}
					break;

				case ewk_up:
				case ewk_rup:
				case ewk_left:
					ItemSelected = ScreenLine+ScreenPage*SCREEN_LINE_MAX;		//计算菜单选择项
					MenuItem_List[ItemSelected].flag = 1;		//失去光标
					if(ScreenLine)
						ScreenLine--;
					else
					{
						ScreenLine = SCREEN_LINE_MAX - 1;		//光标屏幕最下一行
						if(ScreenPage)		//翻页
							ScreenPage--;
						else
						{
							ScreenPage = (MENU_ITEM_TOTAL-1) / SCREEN_LINE_MAX;	//菜单最后一页
							ScreenLine = (MENU_ITEM_TOTAL-1) % SCREEN_LINE_MAX;	//菜单最后一行
						}
						win->event.event |= ewm_paint;
					}
					ItemSelected = ScreenLine+ScreenPage*SCREEN_LINE_MAX;		//计算菜单选择项
					MenuItem_List[ItemSelected].flag = 2;		//得到光标
//					DEBUG_PRINT("id %s, item %d, page %d, pos %d\r\n", MenuItem_List[ItemSelected].name, ItemSelected, ScreenPage, ScreenLine);
					break;

				case ewk_right:
				case ewk_rdown:
				case ewk_down:						//
					ItemSelected = ScreenLine+ScreenPage*SCREEN_LINE_MAX;		//计算菜单选择项
					MenuItem_List[ItemSelected].flag = 1;		//失去光标
					ScreenLine++;		//向下移动菜单选择项
					if(ScreenLine >= SCREEN_LINE_MAX)		//屏幕最下一行
					{
						ScreenLine = 0;		//光标归零
						ScreenPage++;		//翻页
						win->event.event |= ewm_paint;
					}
					ItemSelected = ScreenLine+ScreenPage*SCREEN_LINE_MAX;		//计算菜单选择项
					if(ItemSelected >= MENU_ITEM_TOTAL)		//菜单最后一行
					{
						ScreenLine = 0;		//光标归零
						ScreenPage = 0;		//翻页
						ItemSelected = 0;	//菜单选项归零
						win->event.event |= ewm_paint;
					}
					MenuItem_List[ItemSelected].flag = 2;		//得到光标
//					DEBUG_PRINT("id %s, item %d, page %d, pos %d\r\n", MenuItem_List[ItemSelected].name, ItemSelected, ScreenPage, ScreenLine);
					break;

				case ewk_home:
				case ewk_resc:
				case ewk_esc:
						ui_widget_go_prev_win ( win->prev );		//校准中不能推出界面
					break;

			}
			
			Win_Update();

		win->event.event &= ~ewm_key;
	}
	else if ( win->event.event & ewm_time )
	{
		Win_Timer();
		//win->event.event &= ~ewm_time;		//周期运行
	}
	else if ( win->event.event & ewm_update )
	{
		//数据被编辑
		//g_EditData.format;
		if(g_Digital.udata != g_EditData.udata)
		{
			g_Digital.udata = g_EditData.udata;
			switch(MenuItem_List[ItemSelected].id)
			{
				case mii_calib_total:		//
					//CalibPoint = g_Digital.udata;
					break;
				case mii_ratio_a:		//
					Ratio1 = g_Digital.fdata;
					break;
				case mii_ratio_b:		//
					Ratio2 = g_Digital.fdata;
					break;
				case mii_ratio_c:		//
					Ratio3 = g_Digital.fdata;
					break;
			}
		}
		
		Win_Update();
		MainWin_UpdateStatusBar("返回", "->", "修改");
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
void WinChannel_Create ( void *parg )
{
	( void ) parg;

	ui_window_create ( ( ui_window_t * ) parg, &WinChannel, Win_Callback );
	ui_widgets_set_top_win ( ( ui_widgets_t * ) &WinChannel );		//很重要，一定要有这一句，第一个运行的窗口
}
/**********************
  **********************/
