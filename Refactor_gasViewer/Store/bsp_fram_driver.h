#ifndef __BSP_FRAM_DRIVER_H
#define __BSP_FRAM_DRIVER_H 

#include "main.h"

#define FRAM_MAX_SIZE			0x2000

extern uint8_t fram_SelfCheck;

uint8_t fram_init(void);
uint8_t fs_fram_check ( void );
uint8_t fs_fram_clear ( void );
uint8_t fs_fram_write_nbyte ( uint16_t Addr, uint8_t *Data, uint32_t Len );
uint8_t fs_fram_read_nbyte ( uint16_t Addr, uint8_t *r_data, uint32_t Len );

#endif















