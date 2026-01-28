#ifndef __BSP_W25QXX_DRIVER_H
#define __BSP_W25QXX_DRIVER_H 

#include "main.h"

/*


*/

extern uint8_t norflash_SelfCheck;

extern uint8_t w25qxx_init ( void );
extern void w25qxx_erase_sector(uint32_t sector_num);	/* 扇区4K个字节 */
extern void w25qxx_erase_chip(void); 
extern void w25qxx_write_Nbyte(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite); 
extern void w25qxx_read_Nbyte(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
extern uint8_t w25qxx_CheckBufferBlank ( uint32_t addr, uint16_t len );
extern void w25qxx_WriteBuffer ( uint32_t addr, uint8_t *pBuffer, uint16_t len );

/*

W25Q64是华邦公司推出的大容量SPI FLASH产品，其容量为64Mb。

W25Q64将8M字节的容量分为128个块，每个块大小为64K字节，每个块又分为16个扇区，每个扇区4K个字节。

容量：128 * 16 * 4 * 1024 = 8388608 = 0x800000


存储区分配，区间按照4K取整

采集数据记录区：0x310000
IAP数据区：   0x20000
*/

#define NORFLASH_SECTOR_SIZE					(0x1000)

#define IAP_SECTION_SIZE								0x20000u
#define RECORD_HEAD_SECTION_SIZE				0x1000u
#define RECORD_DATA_SECTION_SIZE				0x30F000u


#define NORFLASH_START_ADDR						0u

#define NORFLASH_IAP_ADDR						(NORFLASH_START_ADDR + 0x8000u)

#define NORFLASH_RECORD_HEAD_ADDR				(NORFLASH_IAP_ADDR + IAP_SECTION_SIZE)
#define NORFLASH_RECORD_DATA_ADDR				(NORFLASH_RECORD_HEAD_ADDR + RECORD_HEAD_SECTION_SIZE)

#define NORFLASH_END_ADDR						(NORFLASH_RECORD_DATA_ADDR + RECORD_DATA_SECTION_SIZE + 1)

#if (NORFLASH_END_ADDR > 0x800000)
	#pragma message("norflash is oversize")
#endif

#endif















