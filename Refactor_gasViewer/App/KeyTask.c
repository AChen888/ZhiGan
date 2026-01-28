#include "guitask.h"
#include "remote.h"
#include "appconfig.h"
#include "ui_config.h"
#include "ui_key.h"
#include "gpio.h"

struct t_KEY_SAMPLE {
	u8 key;
	u8 count;
	u32 tick;
	u32 timeout;
};

struct t_KEY_SAMPLE hardKey;
struct t_KEY_SAMPLE remoteKey;

static void key_HardKeyConvertToUiKey(u8 key)
{
	u8 res = ewk_none;

	if( (key & HARD_KEY_DOWN_BIT) && (key & HARD_KEY_UP_BIT) )
		res = ewk_C;
	else if( (key & HARD_KEY_DOWN_BIT) && (key & HARD_KEY_OK_BIT) )
		res = ewk_test;
	else if( (key & HARD_KEY_DOWN_BIT) && (key & HARD_KEY_UP_BIT)  && (key & HARD_KEY_OK_BIT) )
		res = ewk_mute;
	else if(key & HARD_KEY_DOWN_BIT)
		res = ewk_down;
	else if(key & HARD_KEY_UP_BIT)
		res = ewk_esc;
	else if(key & HARD_KEY_OK_BIT)
		res = ewk_ok;

	if(res)
		ui_SetInputKey(res);
}

static u8 key_RemoteKeyConvertToUiKey(u8 key)
{
	char* str;
	uint8_t res = ewk_none;
	
	switch ( key )
	{
		case 176: str = "C";
			res = ewk_C;		break;

		case 226: str = "MENU";
			res = ewk_home;		break;

		case  34: str = "TEST"; 
			res = ewk_test;		break;

		case 194:str = "BACK";
			res = ewk_resc;		break;

		case 168: str = "PLAY"; 
			res = ewk_rok;		break;

		case 144: str = "RIGHT"; 
			res = ewk_right;		break;

		case 224: str = "LEFT"; 
			res = ewk_left;		break;

		case   2: str = "UP"; 
			res = ewk_rup;		break;

		case 152: str = "DOWN";
			res = ewk_rdown;		break;

		case 162: str = "POWER"; 
			res = ewk_mute;	break;

		case 104: str = "0"; res = ewk_func0; break;

		case  48: str = "1"; res = ewk_func1; break;

		case  24: str = "2"; res = ewk_func2; break;

		case 122: str = "3"; res = ewk_func3; break;

		case  16: str = "4"; res = ewk_func4; break;

		case  56: str = "5"; res = ewk_func5; break;

		case  90: str = "6"; res = ewk_func6; break;

		case  66: str = "7"; res = ewk_func7; break;

		case  74: str = "8"; res = ewk_func8; break;

		case  82: str = "9"; res = ewk_func9; break;
	}
	
	(void)str;

	if(res)
		ui_SetInputKey(res);
	
	return res;
}

static void keyProcess(void)
{
	u32 tick;
	u8 hkey, rkey;

	tick = HAL_GetTick();
	//机械按键
	hkey = hard_GetKeyState();
	if(hkey > 0 && hkey == hardKey.key)
	{
		hardKey.count++;
		if(hardKey.count > 9)		//按键按下去100毫秒
		{
			hardKey.count = 0;
			if( (tick - hardKey.tick) > 500 )
			{
				hardKey.tick = tick;
				key_HardKeyConvertToUiKey(hardKey.key);
			}
		}
	}
	else
	{
		hardKey.key = hkey;
		hardKey.count = 0;
	}
	
	//红外遥控按键
	rkey = Remote_GetResult();
	if(rkey > 0)
	{
		
		if((tick - remoteKey.tick) > 500 )//红外连续发送多次
		{
			remoteKey.tick = tick;
			remoteKey.key = rkey;
			rkey = key_RemoteKeyConvertToUiKey(remoteKey.key);
		}
		
//		switch(rkey)
//		{
//			case ewk_mute:
//				gas_Mute(); 				//消音，直接做在这里
//				break;
//		}
	}
}

//按键任务
/* USER CODE END Header_KEY_Task */
void KEY_Task(void const * argument)
{
  Remote_TimerStart();
	g_StartupFlag |= ese_KeyTask;
  for(;;)
  {
		keyProcess();
    osDelay(10);
  }
}
