#ifndef __FLASH_H
#define __FLASH_H

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#define ADDRESS_DATA_STORAGE 0x0800FC00

#pragma pack(1)
typedef struct{
		uint8_t UID[4];
		uint8_t checkMaster; 
		uint8_t checkInOut;
	} RFID;
#pragma pack()

void flash_erase(uint32_t add);
	
void flash_write_Rfid(uint32_t add, RFID *data);
	
void flash_read_array(uint32_t add, uint8_t *arr);

void flash_read_Rfid(uint32_t add, RFID *data);

#endif