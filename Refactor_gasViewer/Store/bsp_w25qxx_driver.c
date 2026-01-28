/*
* *@ :file explain:
* *@ :name -->bsp_w25qxx_driver.c
* *@ :version -->v1.0.0
* *@ :point -->关于W25Q64的驱动
*/
/*
 * * change record:-->1
 * * time and author:
 * * what changed:创建初始版本
 * * point:1.
 */

/* source file.h */
#include "spi.h"
#include "stdint.h"

#include "bsp_w25qxx_driver.h"
#include "iwdg.h"

#if 1/* W25Q64 Message */

#endif

//#define W25QXX_TEST 		16//读写测试，默认应不开启

#if 1/* 写保护和片选引脚 */
	//CS 和 WP 配置为推挽输出
	#define W25Q_WP_PORT    GPIOC
	#define W25Q_WP_PIN     GPIO_PIN_6

	#define W25Q_CS_PORT    GPIOA
	#define W25Q_CS_PIN     GPIO_PIN_0

	#define W25Q_WP_EN()  HAL_GPIO_WritePin(W25Q_WP_PORT,W25Q_WP_PIN,GPIO_PIN_RESET)//使能写保护
	#define W25Q_WP_UN()  HAL_GPIO_WritePin(W25Q_WP_PORT,W25Q_WP_PIN,GPIO_PIN_SET)//取消写保护

	#define W25Q_CS_EN()  HAL_GPIO_WritePin(W25Q_CS_PORT,W25Q_CS_PIN,GPIO_PIN_RESET)//选中W25QXX
	#define W25Q_CS_UN()  HAL_GPIO_WritePin(W25Q_CS_PORT,W25Q_CS_PIN,GPIO_PIN_SET)//不选中W25QXX

	#define W25Q64_SPI hspi2//W25QXX硬件SPI标号

#endif
#if 1/* 芯片ID */

//	#define W25Q80 	0XEF13
//	#define W25Q16 	0XEF14
//	#define W25Q32 	0XEF15
	#define W25Q64 	0XEF16
//	#define W25Q128	0XEF17
//	#define W25Q256 0XEF18

//	#define NM25Q80 	0X5213
//	#define NM25Q16 	0X5214
//	#define NM25Q32 	0X5215
//	#define NM25Q64 	0X5216
//	#define NM25Q128	0X5217
//	#define NM25Q256 	0X5218

#endif

#if 1/* W25QXX系列芯片的指令 */

	#define W25X_WriteEnable		0x06
	#define W25X_WriteDisable		0x04
	#define W25X_ReadStatusReg1		0x05
	#define W25X_ReadStatusReg2		0x35
	#define W25X_ReadStatusReg3		0x15
	#define W25X_WriteStatusReg1    0x01
	#define W25X_WriteStatusReg2    0x31
	#define W25X_WriteStatusReg3    0x11
	#define W25X_ReadData			0x03
	#define W25X_FastReadData		0x0B
	#define W25X_FastReadDual		0x3B
	#define W25X_PageProgram		0x02
	#define W25X_BlockErase			0xD8
	#define W25X_SectorErase		0x20
	#define W25X_ChipErase			0xC7
	#define W25X_PowerDown			0xB9
	#define W25X_ReleasePowerDown	0xAB
	#define W25X_DeviceID			0xAB
	#define W25X_ManufactDeviceID	0x90
	#define W25X_JedecDeviceID		0x9F
	#define W25X_Enable4ByteAddr    0xB7
	#define W25X_Exit4ByteAddr      0xE9

#endif
/* variable from other file */

/* function from other file */
extern uint8_t tfflag2;
/* function designed for other to use */
uint8_t w25qxx_init ( void );
void w25qxx_erase_sector ( uint32_t sector_num );
void w25qxx_erase_chip ( void );
void w25qxx_write_Nbyte ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite );
void w25qxx_read_Nbyte ( uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead );
/* function declaration */
static uint8_t w25qxx_spi_read_write_byte ( uint8_t data_send );
//static void w25qxx_delay_us(uint16_t us);
static void w25qxx_hard_init ( void );
static void w25qxx_wait_busy ( void );
//static void w25qxx_wake_up(void);
//static void w25qxx_power_down(void);
static uint16_t w25qxx_read_ID ( void );
//static void w25qxx_disable_write(void);
static void w25qxx_enable_write ( void );
//static void w25qxx_write_register(uint8_t regno,uint8_t sr);
static uint8_t w25qxx_read_register ( uint8_t regno );
static void w25qxx_write_Nbyte_without_check ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite );
static void w25qxx_write_in_page ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite );
#ifdef W25QXX_TEST
	static uint8_t w25qxx_write_read_test ( void );
#endif
/* variable declaration */
static uint8_t w25qxx_wr_fifo[4096]   __attribute__ ( ( at ( 0x10000000 + 4096 ) ) );				//写入数据时，一个sector的数据缓冲区
/*
 * *@ author:lanxin
 * *@ brief:检测当前的FLASH是否正常
 * *@ note:
 * *@ param:NONE
 * *@ retval:0 失败 1 成功
 */
uint8_t norflash_SelfCheck = 0;
uint8_t w25qxx_init ( void )
{
	uint8_t temp;
	uint16_t flash_type = W25Q64;
	w25qxx_hard_init();//硬件初始化
	W25Q_WP_UN();//取消写保护
	flash_type = w25qxx_read_ID();	      //读取FLASH ID.

	if ( flash_type == W25Q64 )             //SPI FLASH为W25Q256
	{
		temp = w25qxx_read_register ( 3 );  //读取状态寄存器3，判断地址模式

		if ( ( temp & 0X01 ) == 0 )			 //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25Q_CS_EN();
			w25qxx_spi_read_write_byte ( W25X_Enable4ByteAddr ); //发送进入4字节地址模式指令
			W25Q_CS_UN();
		}

#ifdef W25QXX_TEST

		/* w25qxx读写测试 */
		if ( w25qxx_write_read_test() != 1 )
		{
			return 0;
		}

#endif

		norflash_SelfCheck = 1;

		return 1;
	}
	else
	{
		norflash_SelfCheck = 0;

		return 0;
	}
}
#ifdef USE_W25QXX_EX
/*
 * *@ author:lanxin
 * *@ brief:w25qxx us级延时
 * *@ note:当前使粗略的延时，但是已经传入延时的时间长度，可自行修改
 * *@ param:us 延时的时间长度
 * *@ retval:NONE
 */
static void w25qxx_delay_us ( uint16_t us )
{
	us = ( uint16_t ) us;
	/* 根据实际情况设置delay_cnt的值，或者使用比较精确的延时代替以下代码 */
	uint16_t delay_cnt = 4000;

	while ( delay_cnt > 0 )
	{
		delay_cnt--;
	}
}
#endif
/*
 * *@ author:lanxin
 * *@ brief:w25qxx的硬件外设初始化
 * *@ note:当前只是提供了一个接口，并未实际使用
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_hard_init ( void )
{
	/* add your hard init code */
	/* 当前的硬件初始化由CubeMX完成 */
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 读取寄存器的值
 * *@ note:一共三个寄存器
 * *状态寄存器1：
 * *BIT7  6   5   4   3   2   1   0
 * *SPR   RV  TB BP2 BP1 BP0 WEL BUSY
 * *SPR:默认0,状态寄存器保护位,配合WP使用
 * *TB,BP2,BP1,BP0:FLASH区域写保护设置
 * *WEL:写使能锁定
 * *BUSY:忙标记位(1,忙;0,空闲)
 * *默认:0x00
 * *状态寄存器2：
 * *BIT7  6   5   4   3   2   1   0
 * *SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
 * *状态寄存器3：
 * *BIT7      6    5    4   3   2   1   0
 * *HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
 * *@ param:regno 寄存器标号
 * *@ retval:状态寄存器值
 */
static uint8_t w25qxx_read_register ( uint8_t regno )
{
	uint8_t byte = 0, command = 0;

	switch ( regno )
	{
		case 1:
		{
			command = W25X_ReadStatusReg1;  //读状态寄存器1指令
			break;
		}

		case 2:
		{
			command = W25X_ReadStatusReg2;  //读状态寄存器2指令
			break;
		}

		case 3:
		{
			command = W25X_ReadStatusReg3;  //读状态寄存器3指令
			break;
		}

		default:
		{
			command = W25X_ReadStatusReg1;
			break;
		}
	}

	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( command ); //发送读取状态寄存器命令
	byte = w25qxx_spi_read_write_byte ( 0Xff );
	W25Q_CS_UN();
	return byte;
}
#ifdef USE_W25QXX_EX
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 向寄存器写入值
 * *@ note:
 * *@ param:regno 寄存器标号
 * *@ param:sr 具体的值
 * *@ retval:NONE
 */
static void w25qxx_write_register ( uint8_t regno, uint8_t sr )
{
	uint8_t command = 0;

	switch ( regno )
	{
		case 1:
		{
			command = W25X_WriteStatusReg1;  //写状态寄存器1指令
			break;
		}

		case 2:
		{
			command = W25X_WriteStatusReg2;  //写状态寄存器2指令
			break;
		}

		case 3:
		{
			command = W25X_WriteStatusReg3;  //写状态寄存器3指令
			break;
		}

		default:
		{
			command = W25X_WriteStatusReg1;
			break;
		}
	}

	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( command );  //发送写取状态寄存器命令
	w25qxx_spi_read_write_byte ( sr );       //写入一个字节
	W25Q_CS_UN();
}
#endif
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 使能写
 * *@ note:将WEL置1
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_enable_write ( void )
{
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_WriteEnable ); //发送写使能
	W25Q_CS_UN();
}
#ifdef USE_W25QXX_EX
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 禁止写
 * *@ note:将WEL清零
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_disable_write ( void )
{
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_WriteDisable ); //发送写禁止指令
	W25Q_CS_UN();
}
#endif
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 读取芯片ID
 * *@ note:芯片ID
 * *0XEF13,表示芯片型号为W25Q80
 * *0XEF14,表示芯片型号为W25Q16
 * *0XEF15,表示芯片型号为W25Q32
 * *0XEF16,表示芯片型号为W25Q64
 * *0XEF17,表示芯片型号为W25Q128
 * *0XEF18,表示芯片型号为W25Q256
 * *@ param:NONE
 * *@ retval:芯片ID
 */
static uint16_t w25qxx_read_ID ( void )
{
	uint16_t Temp = 0;
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( 0x90 ); //发送读取ID命令
	w25qxx_spi_read_write_byte ( 0x00 );
	w25qxx_spi_read_write_byte ( 0x00 );
	w25qxx_spi_read_write_byte ( 0x00 );
	Temp |= w25qxx_spi_read_write_byte ( 0xFF ) << 8;
	Temp |= w25qxx_spi_read_write_byte ( 0xFF );
	W25Q_CS_UN();
	return Temp;
}
uint8_t w25qxx_CheckBufferBlank ( uint32_t addr, uint16_t len )
{
	uint8_t *BYTE_BUF;
	uint16_t i;

	if(len > sizeof(w25qxx_wr_fifo))
		len = sizeof(w25qxx_wr_fifo);

	BYTE_BUF = w25qxx_wr_fifo;

	w25qxx_read_Nbyte ( BYTE_BUF, addr, len ); //读出整个扇区的内容

	for ( i = 0; i < len; i++ ) //校验数据
	{
		if ( BYTE_BUF[i] != 0xFF )
		{
			return 0;//需要擦除
		}
		MX_IWDG_Feed();
	}

	return 1;
}

void w25qxx_WriteBuffer ( uint32_t addr, uint8_t *pBuffer, uint16_t len )
{
	w25qxx_write_Nbyte_without_check ( pBuffer, addr, len ); //写已经擦除了的,直接写入扇区.
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 在指定地址开始读取指定长度的数据
 * *@ note:
 * *@ param:pBuffer:数据存储区
 * *@ param:ReadAddr:开始读取的地址(24bit)
 * *@ param:NumByteToRead:要读取的字节数(最大65535)
 * *@ retval:NONE
 */
void w25qxx_read_Nbyte ( uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead )
{
	uint16_t i;
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_ReadData );   //发送读取命令

	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( ReadAddr ) >> 16 ) ); //发送24bit地址
	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( ReadAddr ) >> 8 ) );
	w25qxx_spi_read_write_byte ( ( uint8_t ) ReadAddr );

	for ( i = 0; i < NumByteToRead; i++ )
	{
		pBuffer[i] = w25qxx_spi_read_write_byte ( 0XFF ); //循环读数
		MX_IWDG_Feed();
	}

	W25Q_CS_UN();
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 在一页(0~65535)内写入少于256个字节的数据
 * *@ note:必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
 * *@ param:pBuffer:数据存储区
 * *@ param:WriteAddr:开始写入的地址(24bit)
 * *@ param:NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
 * *@ retval:NONE
 */
static void w25qxx_write_in_page ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite )
{
	uint16_t i;
	w25qxx_enable_write();                  //SET WEL
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_PageProgram ); //发送写页命令
	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( WriteAddr ) >> 16 ) ); //发送24bit地址
	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( WriteAddr ) >> 8 ) );
	w25qxx_spi_read_write_byte ( ( uint8_t ) WriteAddr );

	for ( i = 0; i < NumByteToWrite; i++ )
	{
		w25qxx_spi_read_write_byte ( pBuffer[i] ); //循环写数
	}

	W25Q_CS_UN();
	w25qxx_wait_busy();					   //等待写入结束
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 无检验写
 * *@ note:必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
 * *@ note:具有自动换页功能
 * *@ param:pBuffer:数据存储区
 * *@ param:WriteAddr:开始写入的地址(24bit)
 * *@ param:NumByteToWrite:要写入的字节数(最大65535)
 * *@ retval:NONE
 */
static void w25qxx_write_Nbyte_without_check ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite )
{
uint16_t pageremain;
pageremain = 256 - WriteAddr % 256; //单页剩余的字节数

if ( NumByteToWrite <= pageremain )
{
	pageremain = NumByteToWrite; //不大于256个字节
	MX_IWDG_Feed();
}

while ( 1 )
{
	w25qxx_write_in_page ( pBuffer, WriteAddr, pageremain );

	if ( NumByteToWrite == pageremain )
	{
		break;//写入结束了
	}
	else //NumByteToWrite>pageremain
	{
		pBuffer += pageremain;
		WriteAddr += pageremain;

		NumByteToWrite -= pageremain;			 //减去已经写入了的字节数

		if ( NumByteToWrite > 256 )
		{
			pageremain = 256; //一次可以写入256个字节
		}
		else
		{
			pageremain = NumByteToWrite; 	 //不够256个字节了
		}
		MX_IWDG_Feed();
	}
};
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx 在指定地址开始写入指定长度的数据
 * *@ note:该函数带擦除操作!但不会覆盖其他未涉及的数据。
 * *@ param:pBuffer:数据存储区
 * *@ param:WriteAddr:开始写入的地址(24bit)
 * *@ param:NumByteToWrite:要写入的字节数(最大65535)
 * *@ retval:NONE
 */
void w25qxx_write_Nbyte ( uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite )
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *W25QXX_BUF;
	W25QXX_BUF = w25qxx_wr_fifo;
	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小

	if ( NumByteToWrite <= secremain )
	{
		secremain = NumByteToWrite; //不大于4096个字节
	}

	while ( 1 )
	{
		w25qxx_read_Nbyte ( W25QXX_BUF, secpos * 4096, 4096 ); //读出整个扇区的内容

		for ( i = 0; i < secremain; i++ ) //校验数据
		{
			if ( W25QXX_BUF[secoff + i] != 0XFF )
			{
				break;//需要擦除
			}
			MX_IWDG_Feed();
		}

		if ( i < secremain ) //需要擦除
		{
			w25qxx_erase_sector ( secpos ); //擦除这个扇区

			for ( i = 0; i < secremain; i++ )
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
				MX_IWDG_Feed();
			}

			w25qxx_write_Nbyte_without_check ( W25QXX_BUF, secpos * 4096, 4096 ); //写入整个扇区
		}
		else
		{
			w25qxx_write_Nbyte_without_check ( pBuffer, WriteAddr, secremain ); //写已经擦除了的,直接写入扇区剩余区间.
		}

		if ( NumByteToWrite == secremain )
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff = 0; //偏移位置为0

			pBuffer += secremain; //指针偏移
			WriteAddr += secremain; //写地址偏移
			NumByteToWrite -= secremain;				//字节数递减

			if ( NumByteToWrite > 4096 )
			{
				secremain = 4096;	//下一个扇区还是写不完
			}
			else
			{
				secremain = NumByteToWrite;			//下一个扇区可以写完了
			}
		}
	};
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx擦除整个芯片
 * *@ note:时间超长,不建议使用
 * *@ param:NONE
 * *@ retval:NONE
 */
void w25qxx_erase_chip ( void )
{
	w25qxx_enable_write();                  //SET WEL
	w25qxx_wait_busy();
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_ChipErase ); //发送片擦除命令
	W25Q_CS_UN();
	w25qxx_wait_busy();   				   //等待芯片擦除结束
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx擦除一个扇区
 * *@ note:擦除一个扇区的最少时间:150ms
 * *@ param:sector_num:扇区编号
 * *@ retval:NONE
 */
void w25qxx_erase_sector ( uint32_t sector_num )
{
	sector_num *= 4096;
	uint32_t Dst_Addr = sector_num;
	w25qxx_enable_write();                  //SET WEL
	w25qxx_wait_busy();
	W25Q_CS_EN();
	w25qxx_spi_read_write_byte ( W25X_SectorErase ); //发送扇区擦除指令

	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( Dst_Addr ) >> 16 ) ); //发送24bit地址
	w25qxx_spi_read_write_byte ( ( uint8_t ) ( ( Dst_Addr ) >> 8 ) );
	w25qxx_spi_read_write_byte ( ( uint8_t ) Dst_Addr );
	W25Q_CS_UN();
	w25qxx_wait_busy();   				    //等待擦除完成
}
/*
 * *@ author:lanxin
 * *@ brief:等待w25qxx空闲
 * *@ note:是否添加最大等待时间待定
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_wait_busy ( void )
{
	uint32_t try_cnt = 131068; //根据实际情况设置

	while ( ( ( w25qxx_read_register ( 1 ) & 0x01 ) == 0x01 ) && ( try_cnt > 0 ) /*&& tfflag2!=0*/ ) // 等待BUSY位清空
	{
		try_cnt--;
		MX_IWDG_Feed();
		/* add your delay,but it is not necessary */
	}
}
#ifdef USE_W25QXX_EX
/*
 * *@ author:lanxin
 * *@ brief:使w25qxx进入掉电模式
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_power_down ( void )
{
	W25Q_CS_EN();                            //使能器件
	w25qxx_spi_read_write_byte ( W25X_PowerDown ); //发送掉电命令
	W25Q_CS_UN();                            //取消片选
	w25qxx_delay_us ( 3 ); //等待3us生效
}
/*
 * *@ author:lanxin
 * *@ brief:唤醒w25qxx
 * *@ note:
 * *@ param:NONE
 * *@ retval:NONE
 */
static void w25qxx_wake_up ( void )
{
	W25Q_CS_EN();                                 //使能器件
	w25qxx_spi_read_write_byte ( W25X_ReleasePowerDown ); //  send W25X_PowerDown command 0xAB
	W25Q_CS_UN();                                //取消片选
	w25qxx_delay_us ( 3 ); //等待3us生效
}
#endif
/*
 * *@ author:lanxin
 * *@ brief:硬件SPI读写一个字节
 * *@ note:全双工工作
 * *@ param:data_send 发送的一个字节
 * *@ retval:data_rcv 接收到的一个字节
 */
static uint8_t w25qxx_spi_read_write_byte ( uint8_t data_send )
{
	uint8_t data_rcv;
	HAL_SPI_TransmitReceive ( &W25Q64_SPI, &data_send, &data_rcv, 1, 1000 );
	return data_rcv;
}
/*
 * *@ author:lanxin
 * *@ brief:w25qxx读写测试
 * *@ note:由W25QXX_TEST 控制是否使用，最后会擦掉写入的数据
 * *@ param:NONE
 * *@ retval:0 失败 1 成功
 */
#ifdef W25QXX_TEST
static uint8_t w25qxx_write_read_test ( void )
{
	static uint8_t test_data_write[W25QXX_TEST];
	static uint8_t test_data_read[W25QXX_TEST];

	for ( int i = 0; i < 256; i++ )
	{
		test_data_write[i] = 7;
	}

	w25qxx_write_Nbyte ( test_data_write, 0x00, W25QXX_TEST );
	w25qxx_read_Nbyte ( test_data_read, 0x00, W25QXX_TEST );

	for ( int j = 0; j < W25QXX_TEST; j++ )
	{
		if ( test_data_read[j] != test_data_write[j] )
		{
			return 0;
		}
	}

	/* 擦除写入的数据 */
	w25qxx_erase_sector ( 0 );

	return 1;
}
#endif
