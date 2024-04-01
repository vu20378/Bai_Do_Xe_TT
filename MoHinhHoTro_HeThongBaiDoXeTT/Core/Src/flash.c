#include "flash.h"
#include "stm32f1xx.h"

void flash_erase(uint32_t add)
	{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInitStruct;
	
	EraseInitStruct.Banks = 1; //tap hop cac page la bank - trong stm32f103c8 nay chi co 1 bank(128page hoac 64 page)
	EraseInitStruct.NbPages = 1; //xoa 119 page tu page 5 toi page 124
	EraseInitStruct.PageAddress = add; //dia chi muon xoa
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES; //kieu xoa
	/*FLASH_TYPEERASE_PAGES     0x00U xoa page
	FLASH_TYPEERASE_MASSERASE 0x02U xoa ca flash*/
	
	uint32_t pageser;
	HAL_FLASHEx_Erase(&EraseInitStruct, &pageser);
	HAL_FLASH_Lock();
}
	
void flash_write_Rfid(uint32_t add, RFID *data) {
    HAL_FLASH_Unlock();
    uint16_t *pt = (uint16_t*)data;
    uint16_t halfwords = sizeof(RFID) / 2;
    for (uint16_t i = 0; i < halfwords; i++) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, add + 2 * i, *pt);
			//co 3 loai ghi: ghi 2 byte, ghi 4 byte, ghi 8 bye
        pt++;
    }
    HAL_FLASH_Lock();
}
	
void flash_read_array(uint32_t add, uint8_t *arr){
		uint16_t * pt = (uint16_t*)arr;
		int i = 0;
		while(1){
			*pt = *(__IO uint16_t *)(add + 2*i); //doc theo 2byte hoac 4byte
			if(pt[i] == 0xFF) break;
			pt++;
			i++;
		}
}

void flash_read_Rfid(uint32_t add, RFID *data){
		flash_read_array(add, (uint8_t*)data);
	}


