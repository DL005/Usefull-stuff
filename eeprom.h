/*
 * eeprom.h
 *
 *  Created on: 27 янв. 2023 г.
 *      Author: DL
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "stm32l4xx_hal_flash.h"

#define eepromStartAddress 	0x08100000
#define eepromStopAddress 	0x081FFFFF

#define eepromStuffAddress	eepromStartAddress
#define eepromSystemAdress	0x081FF000

#define eepromTriesUnlock	5
#define eepromTriesErase	5
#define eepromTriesWrite	5
#define eepromTriesLock		5

typedef enum
{
	eepromError_Ok,
	eepromError_DataOutOfRange,
	eepromError_Unlock,
	eepromError_Erase,
	eepromError_Write,
	eepromError_Lock,
}eepromError_t;

typedef struct
{
	float UsefulData[32];
	char VeryImportantString[128];
}eeStuff_t

typedef struct
{
	uint32_t SysFreq;
	uint32_t ExtBaudrate;
	
	uint64_t Flags;
}eeSystem_t;

typedef struct
{
	volatile eeStuff_t* Stuff;
	volatile eeSystem_t* System;
}eeData_t;

typedef struct
{
	uint32_t StartAddress;
	uint32_t StopAddress;
	uint32_t Size;

	eeData_t Data;

	eepromError_t (*Write) (void* ptrFrom, void* ptrTo, uint32_t bytes );
	eepromError_t (*WriteString) (char* str, void* ptrTo);
}eeprom_t;

extern volatile eeprom_t eeprom;

#endif /* EEPROM_H_ */
