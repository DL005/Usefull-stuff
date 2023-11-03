/*
 * eeprom.c
 *
 *  Created on: 27 янв. 2023 г.
 *      Author: DL
 */


#include "eeprom.h"

#define eepromPageSizeInBytes	4*1024

#define eepromGetPageAddr(addr)	( ((uint32_t)(addr)>>12)<<12 )
#define eepromGetPage(addr)	((eepromGetPageAddr(addr) - eepromStartAddress)>>12)

eepromError_t eeWrite(void* ptrFrom, void* ptrTo, uint32_t bytes );
eepromError_t eeWriteString(char* str, void* ptrTo);

volatile eeprom_t eeprom =
{
	eepromStartAddress,						//	StartAddress
	eepromStopAddress,						//	StopAddress
	eepromStopAddress - eepromStartAddress,	//	Size

	//	Data
	{
			(volatile eeStuff_t* ) eepromStuffAddress,
			(volatile eeSystem_t*) eepromSystemAdress,
	},

	eeWrite, 		//	Write
	eeWriteString,	//	WriteString
};

void configEEPROM(void)
{

}

eepromError_t eeWrite(void* ptrFrom, void* ptrTo, uint32_t bytes )
{
	if( (uint32_t)ptrTo < eepromStartAddress || ( (uint32_t)ptrTo + bytes ) > eepromStopAddress ) return eepromError_DataOutOfRange;

	uint8_t pageBackup[eepromPageSizeInBytes] = {0};
	uint64_t* pageBackupDWordPtr = (uint64_t*)pageBackup;

	uint16_t pageStart 	= eepromGetPage(ptrTo);
	uint16_t pageStop 	= eepromGetPage(ptrTo + bytes);

	uint32_t restBytesStart = (uint32_t)ptrTo - eepromGetPageAddr(ptrTo);
	uint32_t restBytesEnd = eepromPageSizeInBytes - ( ((uint32_t)ptrTo + bytes) - eepromGetPageAddr(ptrTo+bytes) );

	uint32_t PAGEError = 0;
	FLASH_EraseInitTypeDef EraseInitStruct;

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = FLASH_BANK_2;
	EraseInitStruct.NbPages = (uint32_t) 0x1;

	for( uint16_t try = 0; try <= eepromTriesUnlock && HAL_FLASH_Unlock(); try++ )
	{
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
		HAL_FLASH_Lock();
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
		if( try == eepromTriesUnlock )
		{
			HAL_FLASH_Lock();
			return eepromError_Unlock;
		}
	}

	for( uint16_t page = pageStart; page <= pageStop; page++ )
	{
		memcpy(pageBackup, (void*)((page << 12) + eepromStartAddress), eepromPageSizeInBytes);

		if( page == pageStart )
		{
			memcpy( pageBackup + restBytesStart, ptrFrom, pageStart == pageStop ? bytes : eepromPageSizeInBytes - restBytesStart );
		}
		else if( page == pageStop )
		{
			memcpy( pageBackup, ptrFrom + (eepromGetPageAddr(ptrTo + bytes) - (uint32_t)ptrTo), eepromPageSizeInBytes - restBytesEnd );
		}
		else
		{
			memcpy( pageBackup, ptrFrom + (page - pageStart)*eepromPageSizeInBytes - restBytesStart, eepromPageSizeInBytes );
		}

		EraseInitStruct.Page = (uint32_t) page;

		for( uint16_t try = 0; try <= eepromTriesErase && HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError); try++ )
		{
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
			if( try == eepromTriesErase ) { return eepromError_Erase; }
		}

		for( uint16_t currentDWord = 0; currentDWord < (eepromPageSizeInBytes >> 3); currentDWord++)
		{
			for( uint16_t try = 0; try <= eepromTriesWrite && HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)( eepromStartAddress + (page<<12) + (currentDWord<<3) ), pageBackupDWordPtr[currentDWord]); try++ )
			{
				__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
				if( try == eepromTriesWrite )
				{
					__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
					HAL_FLASH_Lock();
					return eepromError_Write;
				}
			}
		}
	}

	for( uint16_t try = 0; try <= eepromTriesLock && HAL_FLASH_Lock(); try++ )
	{
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
		if( try == eepromTriesLock ) { return eepromError_Lock; }
	}

	return eepromError_Ok;
}

eepromError_t eeWriteString(char* str, void* ptrTo)
{
	return eeWrite((void*)str, ptrTo, strlen(str));
}
