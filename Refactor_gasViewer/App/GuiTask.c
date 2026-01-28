#include "guitask.h"
#include "spi.h"
#include "iwdg.h"
#include "win_startup.h"




//界面任务
/* USER CODE END Header_GUI_Task */
void GUI_Task(void const * argument)
{
  /* USER CODE BEGIN GUI_Task */
	LCD_Init();
	LCD_Fill(0, 0, 320, 115, RED);
	LCD_Fill(0, 115, 320, 120, GREEN);
	LCD_Fill(0, 120, 320, 240, BLUE);
	
	MX_SPI1_Init();			//恢复硬件SPI
	
	lcd_BG_Active();//背光	
	osDelay ( 100 ); //延时的作用
	ui_DrawBar ( 0, 0, LCD_W, LCD_H, GRAY );	//清屏幕
	MainWin_Create(0);
	g_StartupFlag |= ese_GuiTask;
  /* Infinite loop */
  for(;;)
  {
		MX_IWDG_Feed();
		ui_widgets_run_top_win(); //
		MainWin_UpdateTimeBar();
		
    osDelay(50);
  }
  /* USER CODE END GUI_Task */
}
