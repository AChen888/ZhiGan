/*
 * *@ :file explain:
 * *@ :name -->bsp_fram_driver.c
 * *@ :version -->V1.0.0
 * *@ :point --> 1.写的一个FRAM的驱动程序，引入了中间类型c_memory_data_t 用于保存结构体类型的数据。
 */
/*
 * * change record:-->1
 * * time and author: 21.11.30
 * * what changed:创建初始版本
 * * point:
 */

/* source file.h */
#include "i2c.h"
#include "bsp_fram_driver.h"

/* ADDR */
#define FRAM_DEVICE_ADDR		0xA0//FRAM设备地址
#define FRAM_MAX_ADDR			0x1FFF//FRAM最大地址

#define FRAM_SDA_SCL_PORT   GPIOA
#define FRAM_SDA_PIN        GPIO_PIN_10
#define FRAM_SCL_PIN        GPIO_PIN_9
/* FRAM soft i2c */
//SDA 配置成开漏输出 SCL配置为推挽输出
#define FRAM_WRITE_ENABLE() 	HAL_GPIO_WritePin(FRAM_WP_GPIO_Port,FRAM_WP_Pin,GPIO_PIN_RESET)//取消写保护
#define FRAM_WRITE_DISABLE() 	HAL_GPIO_WritePin(FRAM_WP_GPIO_Port,FRAM_WP_Pin,GPIO_PIN_SET)//开启写保护

#define FRAM_READ_SDA()			HAL_GPIO_ReadPin(FRAM_SDA_SCL_PORT,FRAM_SDA_PIN)

#define FRAM_IIC_SDA_H()		HAL_GPIO_WritePin(FRAM_SDA_SCL_PORT,FRAM_SDA_PIN,GPIO_PIN_SET)
#define FRAM_IIC_SDA_L()		HAL_GPIO_WritePin(FRAM_SDA_SCL_PORT,FRAM_SDA_PIN,GPIO_PIN_RESET)
#define FRAM_IIC_SCL_H()		HAL_GPIO_WritePin(FRAM_SDA_SCL_PORT,FRAM_SCL_PIN,GPIO_PIN_SET)
#define FRAM_IIC_SCL_L()		HAL_GPIO_WritePin(FRAM_SDA_SCL_PORT,FRAM_SCL_PIN,GPIO_PIN_RESET)

/*
GPIO_MODE_INPUT
GPIO_MODE_OUTPUT_PP
*/
void fram_GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t position, uint32_t Mode)
{
	uint32_t temp;
  //GPIO_MODE_INPUT

  temp = GPIOx->MODER;
  temp &= ~(GPIO_MODER_MODER0 << (position * 2u));					//clear
  temp |= ((Mode & GPIO_MODE) << (position * 2u));		//or
  GPIOx->MODER = temp;
}

#define FRAM_IIC_SDA_OUT()		fram_GPIO_SetPinMode(FRAM_SDA_SCL_PORT, 10, GPIO_MODE_OUTPUT_PP);
#define FRAM_IIC_SDA_IN()		fram_GPIO_SetPinMode(FRAM_SDA_SCL_PORT, 10, GPIO_MODE_INPUT);


/* function designed for other to use */
uint8_t fram_init(void);
uint8_t fs_fram_check ( void );
uint8_t fs_fram_clear ( void );
uint8_t fs_fram_write_nbyte ( uint16_t Addr, uint8_t *Data, uint32_t Len );
uint8_t fs_fram_read_nbyte ( uint16_t Addr, uint8_t *r_data, uint32_t Len );
//uint8_t fs_fram_add_memory_block ( c_memory_data_t *memory_data );
//uint8_t fs_fram_write_data ( c_memory_data_t *memory_data );
//uint8_t fs_fram_read_data ( c_memory_data_t *memory_data );
/* function declaration */
static void fs_fram_i2c_delay ( void );
static void fs_fram_i2c_start ( void );
static void fs_fram_i2c_stop ( void );
static void fs_fram_i2c_ack ( void );
static void fs_fram_i2c_nack ( void );
static uint8_t fs_fram_i2c_wait_ack ( void );
static void fs_fram_i2c_write_byte ( uint8_t write_data );
static uint8_t fs_fram_i2c_read_byte ( void );
/* 操作类型，不对外使用 */
typedef enum
{

	WR_FRAM = 0x00,
	RD_FRAM,
} e_fram_wr_t;
/* 错误类型，不对外使用 */
typedef enum
{

	FRAM_ERR  	=   0X00,
	FRAM_OK		=	0X01,
} e_fram_state_t;


static void fram_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	HAL_I2C_DeInit(&hi2c2);

	/*Configure GPIO pin Output Level */
  	HAL_GPIO_WritePin(FRAM_SDA_SCL_PORT, FRAM_SDA_PIN|FRAM_SCL_PIN, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = FRAM_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(FRAM_SDA_SCL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FRAM_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(FRAM_SDA_SCL_PORT, &GPIO_InitStruct);
}

/* variable declaration */
/* FRAM保存的每个数据的起始地址和当前已添加的数据个数 */
//static uint16_t fram_data_addr[FRAM_MAX_BLOCK_NUM];
//static uint16_t fram_data_num = 0x00;
/*
 * *@ author:lanxin
 * *@ brief:FRAM的初始化
 * *@ note:
 * *@ param:NONE
 * *@ retval:0 失败 1 成功
 */
uint8_t fram_init(void)
{
	fram_gpio_init();
	FRAM_WRITE_ENABLE();
	
    if(fs_fram_check())
    {
        return 1;
    }
    return 0;
} 
/*
 * *@ author:lanxin
 * *@ brief:用于模拟i2c的延时
 * *@ note:这是个空延时，时间并不准确
 * *@ param:NONE
 * *@ retval:NONE
 */
static void fs_fram_i2c_delay ( void )
{
	for ( int i = 0; i < 400; i++ )//for ( int i = 0; i < 4000; i++ )
	{
		;
	}
}
/*
 * *@ author:lanxin
 * *@ brief:产生起始信号
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void fs_fram_i2c_start ( void )
{
	FRAM_IIC_SDA_OUT();
	FRAM_IIC_SCL_H();
	FRAM_IIC_SDA_H();
	fs_fram_i2c_delay();
	FRAM_IIC_SDA_L();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_L();
}
/*
 * *@ author:lanxin
 * *@ brief:产生停止信号
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void fs_fram_i2c_stop ( void )
{
	FRAM_IIC_SDA_OUT();
	FRAM_IIC_SCL_L();
	FRAM_IIC_SDA_L();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_H();
	fs_fram_i2c_delay();
	FRAM_IIC_SDA_H();
	fs_fram_i2c_delay();
}
/*
 * *@ author:lanxin
 * *@ brief:产生应答信号
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void fs_fram_i2c_ack ( void )
{
	FRAM_IIC_SCL_L();
	FRAM_IIC_SDA_OUT();
	FRAM_IIC_SDA_L();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_H();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_L();
}
/*
 * *@ author:lanxin
 * *@ brief:产生非应答信号
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void fs_fram_i2c_nack ( void )
{
	FRAM_IIC_SCL_L();
	FRAM_IIC_SDA_OUT();
	FRAM_IIC_SDA_H();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_H();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_L();
}
/*
 * *@ author:lanxin
 * *@ brief:等待应答信号
 * *@ note:
 * *@ param:NONE
 * *@ retval:1 有应答 0无应答
 */
static uint8_t fs_fram_i2c_wait_ack ( void )
{
	uint8_t uTime = 0;

	FRAM_IIC_SDA_H();
	FRAM_IIC_SDA_IN();
	fs_fram_i2c_delay();
	FRAM_IIC_SCL_H();
	fs_fram_i2c_delay();

	while ( FRAM_READ_SDA() )
	{
		uTime++;

		if ( uTime > 50 )
		{
			fs_fram_i2c_stop();
			return 1;
		}
	}

	FRAM_IIC_SCL_L();
	return 0;
}
/*
 * *@ author:lanxin
 * *@ brief:写一字节数据到铁电存储器
 * *@ note:
 * *@ param:write_data 写入的数据
 * *@ retval:NONE
 */
static void fs_fram_i2c_write_byte ( uint8_t write_data )
{
	uint32_t t, i;

	FRAM_IIC_SDA_OUT();
	FRAM_IIC_SCL_L();

	for ( t = 0; t < 8; t++ )
	{
		if ( write_data & ( 0x80 >> t ) )
		{
			FRAM_IIC_SDA_H();
		}
		else
		{
			FRAM_IIC_SDA_L();
		}

		FRAM_IIC_SCL_H();
		fs_fram_i2c_delay();
		FRAM_IIC_SCL_L();

		for ( i = 0; i < 10; i++ );
	}
}
/*
 * *@ author:lanxin
 * *@ brief:从铁电存储器读一字节数据
 * *@ note:
 * *@ param:write_data 写入的数据
 * *@ retval:读取的数据 uint8_t 类型
 */
static uint8_t fs_fram_i2c_read_byte ( void )
{
	uint8_t  rByte = 0;
	uint32_t i;

	FRAM_IIC_SDA_IN();
	for ( i = 0; i < 8; i++ )
	{
		FRAM_IIC_SCL_L();
		fs_fram_i2c_delay();
		FRAM_IIC_SCL_H();
		fs_fram_i2c_delay();
		rByte <<= 1;

		if ( FRAM_READ_SDA() )
		{
			rByte++;
		}
	}

	return rByte;
}
/*
 * *@ author:lanxin
 * *@ brief:写数据到铁电存储器
 * *@ note:
 * *@ param:Addr:写入的起始地址
 * *@ param:w_data写入的数据指针 uint8_t类型
 * *@ param:Len：写入的数据长度
 * *@ retval:result != FRAM_OK fail
 */
uint8_t fs_fram_write_nbyte ( uint16_t Addr, uint8_t *w_data, uint32_t Len )
{
	if ( ( Addr + Len ) > FRAM_MAX_ADDR )
	{
		return FRAM_ERR;
	}

	fs_fram_i2c_start();
	fs_fram_i2c_write_byte ( FRAM_DEVICE_ADDR | WR_FRAM );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) ( Addr >> 8 ) );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) Addr );
	fs_fram_i2c_wait_ack();

	while ( Len-- )
	{
		fs_fram_i2c_write_byte ( *w_data++ );
		fs_fram_i2c_wait_ack();
	}

	fs_fram_i2c_stop();

	return FRAM_OK;
}
/*
 * *@ author:lanxin
 * *@ brief:从铁电存储器读取数据
 * *@ note:
 * *@ param:Addr:读取的起始地址
 * *@ param:Data：r_data存放读取的数据指针 uint8_t类型
 * *@ param:Len：读取的数据长度
 * *@ retval:result != FRAM_OK fail
 */
uint8_t fs_fram_read_nbyte ( uint16_t Addr, uint8_t *r_data, uint32_t Len )
{
	if ( ( Addr + Len ) > FRAM_MAX_ADDR )
	{
		return FRAM_ERR;
	}

	fs_fram_i2c_start();
	fs_fram_i2c_write_byte ( FRAM_DEVICE_ADDR | WR_FRAM );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) ( Addr >> 8 ) );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) Addr );
	fs_fram_i2c_wait_ack();

	fs_fram_i2c_start();
	fs_fram_i2c_write_byte ( FRAM_DEVICE_ADDR | RD_FRAM );
	fs_fram_i2c_wait_ack();

	while ( Len-- )
	{
		*r_data++ = fs_fram_i2c_read_byte();

		if ( Len == 0 )
		{
			fs_fram_i2c_nack();
		}
		else
		{
			fs_fram_i2c_ack();
		}
	}

	fs_fram_i2c_stop();

	return FRAM_OK;
}
/*
 * *@ author:lanxin
 * *@ brief:将数据写入铁电存储器并校验
 * *@ note:
 * *@ param:NONE
 * *@ retval:result != FRAM_OK fail
 */
 uint8_t fram_SelfCheck = 0;
uint8_t fs_fram_check ( void )
{
	uint16_t i;
	uint8_t data_write[8] = "zhx & ty"; //fsy
	char data_read[8] = {0};
	
	fs_fram_read_nbyte ( FRAM_MAX_ADDR - sizeof(data_read), (uint8_t *)data_read, sizeof(data_read) );
	
	for(i = 0; i < sizeof(data_read); i++)
	{
		if ( data_read[i] != data_write[i] )
		{
			goto write_sign_to_fram;
		}
	}

	fram_SelfCheck = 1;
	
	return FRAM_OK;
	
write_sign_to_fram:	
	fs_fram_write_nbyte ( FRAM_MAX_ADDR - sizeof(data_read), (uint8_t *)data_write, sizeof(data_read) );
	fs_fram_read_nbyte ( FRAM_MAX_ADDR - sizeof(data_read), (uint8_t *)data_read, sizeof(data_read) );

	fram_SelfCheck = 0;

	return FRAM_ERR;
}
/*
 * *@ author:lanxin
 * *@ brief:FRAM全部清零
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
uint8_t fs_fram_clear ( void )
{
	uint16_t len;

	len = 0x1FFF;

	fs_fram_i2c_start();
	fs_fram_i2c_write_byte ( FRAM_DEVICE_ADDR | WR_FRAM );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) ( 0x00 >> 8 ) );
	fs_fram_i2c_wait_ack();
	fs_fram_i2c_write_byte ( ( uint8_t ) 0x00 );
	fs_fram_i2c_wait_ack();

	while ( len-- )
	{
		fs_fram_i2c_write_byte ( 0x00 );
		fs_fram_i2c_wait_ack();
	}

	fs_fram_i2c_stop();

	return 1;
}
///*
// * *@ author:lanxin
// * *@ brief:在FRAM中添加一个存储块
// * *@ note:
// * *@ param:memory_data要添加的句柄
// * *@ retval:0 失败 1成功
// */
//uint8_t fs_fram_add_memory_block ( c_memory_data_t *memory_data )
//{
//	if ( memory_data != 0x00 )
//	{
//		if ( memory_data->data != 0x00 )
//		{
//			uint16_t len = memory_data->data_len;

//			/* 地址不超，数量不超 */
//			if ( len + fram_data_addr[fram_data_num] < FRAM_MAX_ADDR && fram_data_num + 1 < FRAM_MAX_BLOCK_NUM )
//			{
//				memory_data->block_num = fram_data_num;

//				fram_data_addr[fram_data_num + 1] = fram_data_addr[fram_data_num] + len;
//				fram_data_num++;
//				return 1;
//			}
//		}
//	}

//	return 0;
//}
///*
// * *@ author:lanxin
// * *@ brief:读取FRAM中的数据块
// * *@ note:
// * *@ param:memory_data要读取的句柄
// * *@ retval:0 失败 1成功
// */
//uint8_t fs_fram_read_data ( c_memory_data_t *memory_data )
//{
//	if ( memory_data != 0x00 )
//	{
//		if ( memory_data->data != 0 && memory_data->block_num < FRAM_MAX_BLOCK_NUM )
//		{
//			fs_fram_read_nbyte ( fram_data_addr[memory_data->block_num], memory_data->data, memory_data->data_len );
//			return 1;
//		}
//	}

//	return 0;
//}
///*
// * *@ author:lanxin
// * *@ brief:向FRAM中写入一个数据块
// * *@ note:
// * *@ param:memory_data要写入的句柄
// * *@ retval:0 失败 1成功
// */
//uint8_t fs_fram_write_data ( c_memory_data_t *memory_data )
//{
//	if ( memory_data != 0x00 )
//	{
//		if ( memory_data->data != 0 && memory_data->block_num < FRAM_MAX_BLOCK_NUM )
//		{
//			fs_fram_write_nbyte ( fram_data_addr[memory_data->block_num], memory_data->data, memory_data->data_len );
//			return 1;
//		}
//	}

//	return 0;
//}



typedef struct class_memory_data
{

	uint8_t *data;//数据
	uint16_t data_len;//数据长度
	uint16_t block_num;//存储块编号，初始化之后分配的一个编号
} c_memory_data_t;

