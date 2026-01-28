#include "lcd_init.h"
#include "spi.h"



void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	// zhx
    __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
	#ifndef USE_SPI_HARDWARD
	
	HAL_SPI_DeInit(&hspi1);
	
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	#else
	
	#endif
}


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	

	LCD_CS_Clr();
	#ifndef USE_SPI_HARDWARD
	u8 i;
	
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
	#else
	
	while(SPI_CHECK_FLAG(hspi1.Instance->SR, SPI_FLAG_TXE) == RESET);
	
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
//	HAL_Delay(2);

	#endif
	
  LCD_CS_Set();	
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	#ifndef USE_SPI_HARDWARD	

	LCD_Writ_Bus(dat);

	#else
//	LCD_DC_Set();
	LCD_CS_Clr();
	
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
	
	LCD_CS_Set();	
	#endif
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
#ifndef USE_SPI_HARDWARD	

	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);

#else
	uint8_t data2[2] = {(uint8_t)(dat >>8), (uint8_t)dat};
//	LCD_DC_Set();
	LCD_CS_Clr();
	
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&data2, 2, 1000);
	
	LCD_CS_Set();	
#endif
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//写命令
	LCD_WR_DATA8(dat);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//初始化GPIO
	
	LCD_RES_Clr();//复位
	HAL_Delay(100);  //延时的作用
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//打开背光
	HAL_Delay(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	HAL_Delay(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);			
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);			
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x33); 			

	LCD_WR_REG(0xB7);			
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);			
	LCD_WR_DATA8(0x32); //Vcom=1.35V
					
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			
	LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度
				
	LCD_WR_REG(0xC4);			
	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	LCD_WR_REG(0xC6);			
	LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	LCD_WR_REG(0xD0);			
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1); 

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);
} 

static u32 lcd_BG_Count;
/*
	毫秒中断中调用，控制背光亮度
*/
void lcd_BG_Control(void)
{
	u32 tick;
	
	if(lcd_BG_Count)
	{
		LCD_BLK_Set();//打开背光
		lcd_BG_Count--;
	}
	else
	{	//让背光变暗
		tick = HAL_GetTick();

		if( (tick & 0x00000003) == 0 )
			LCD_BLK_Set();//打开背光
		else
			LCD_BLK_Clr();//关闭背光
	}
}
/*
激活背光计数器，赋值，毫秒计数
*/
void lcd_BG_Active(void)
{
	lcd_BG_Count = 1000 * 60 * 5;
}

#if 1

static void lcd_WR_DATA8(u8 dat)
{
	LCD_CS_Clr();
	
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
	
	LCD_CS_Set();	
}

static void lcd_WR_DATA(u16 dat)
{
	uint8_t data2[2] = {(uint8_t)(dat >>8), (uint8_t)dat};

	LCD_CS_Clr();
	
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&data2, 2, 1000);
	
	LCD_CS_Set();	
}

void lcd_WR_DATA_LEN(u8 *dat, u16 len)
{
	LCD_CS_Clr();
	
	HAL_SPI_Transmit(&hspi1, dat, len, 100 * len);
	
	LCD_CS_Set();	
}

static void lcd_WR_REG(u8 dat)
{
	LCD_DC_Clr();//写命令
	lcd_WR_DATA8(dat);
	LCD_DC_Set();//写数据
}

void lcd_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		lcd_WR_REG(0x2a);//列地址设置
		lcd_WR_DATA(x1);
		lcd_WR_DATA(x2);
		lcd_WR_REG(0x2b);//行地址设置
		lcd_WR_DATA(y1);
		lcd_WR_DATA(y2);
		lcd_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		lcd_WR_REG(0x2a);//列地址设置
		lcd_WR_DATA(x1);
		lcd_WR_DATA(x2);
		lcd_WR_REG(0x2b);//行地址设置
		lcd_WR_DATA(y1);
		lcd_WR_DATA(y2);
		lcd_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		lcd_WR_REG(0x2a);//列地址设置
		lcd_WR_DATA(x1);
		lcd_WR_DATA(x2);
		lcd_WR_REG(0x2b);//行地址设置
		lcd_WR_DATA(y1);
		lcd_WR_DATA(y2);
		lcd_WR_REG(0x2c);//储存器写
	}
	else
	{
		lcd_WR_REG(0x2a);//列地址设置
		lcd_WR_DATA(x1);
		lcd_WR_DATA(x2);
		lcd_WR_REG(0x2b);//行地址设置
		lcd_WR_DATA(y1);
		lcd_WR_DATA(y2);
		lcd_WR_REG(0x2c);//储存器写
	}
}
#endif



