/*
 * i2cSoft.c
 *
 *  Created on: Apr 11, 2023
 *      Author: DL
 */

#include <i2cSoft.h>

//Pet me-------------------------------------------

void i2cInit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	LL_AHB2_GRP1_EnableClock(i2cSDA_PortClk);
	LL_AHB2_GRP1_EnableClock(i2cSCL_PortClk);

	/*SDA pin*/
	GPIO_InitStruct.Pin = i2cSDA_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(i2cSDA_Port, &GPIO_InitStruct);

	/*SCL pin*/
	GPIO_InitStruct.Pin = i2cSCL_Pin;
	LL_GPIO_Init(i2cSCL_Port, &GPIO_InitStruct);
}

void i2cDeinit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/*SDA pin*/
	GPIO_InitStruct.Pin = i2cSDA_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(i2cSDA_Port, &GPIO_InitStruct);

	/*SCL pin*/
	GPIO_InitStruct.Pin = i2cSCL_Pin;
	LL_GPIO_Init(i2cSCL_Port, &GPIO_InitStruct);
}

void i2cDelay(volatile uint8_t x)
{
	while(x--);
}

void i2cSCL(uint8_t state)
{
	state ? LL_GPIO_SetOutputPin(i2cSCL_Port, i2cSCL_Pin) : LL_GPIO_ResetOutputPin(i2cSCL_Port, i2cSCL_Pin);
	i2cDelay(i2cPinDelay);
}

void i2cSDA(uint8_t state)
{
	state ? LL_GPIO_SetOutputPin(i2cSDA_Port, i2cSDA_Pin) : LL_GPIO_ResetOutputPin(i2cSDA_Port, i2cSDA_Pin);
	i2cDelay(i2cPinDelay);
}

uint8_t i2cReadSDA(void)
{
	return (uint8_t)LL_GPIO_IsInputPinSet(i2cSDA_Port, i2cSDA_Pin);
}

//0 - input
//1 - output
void i2cConfigSDA(uint8_t mode)
{
	LL_GPIO_SetPinMode(i2cSDA_Port, i2cSDA_Pin, mode ? LL_GPIO_MODE_OUTPUT : LL_GPIO_MODE_INPUT);
}

//----------------------------------------------------------------

void i2cStart(void)
{
	i2cConfigSDA(1);

	i2cSCL(1);
	i2cSDA(1);

	i2cSCL(1);
	i2cSDA(0);

	i2cSCL(0);
	i2cSDA(0);
}

void i2cStop(void)
{
	i2cConfigSDA(1);

	i2cSCL(0);
	i2cSDA(0);

	i2cSCL(1);
	i2cSDA(0);

	i2cSCL(1);
	i2cSDA(1);
}

void i2cSendBit(uint8_t bit)
{
	i2cSCL(0);
	i2cSDA( bit );
	i2cSCL(1);
}

uint8_t i2cReceiveBit(void)
{
	uint8_t bit = 0;

	i2cSCL(1);
	bit = i2cReadSDA();
	i2cSCL(0);

	return bit;
}

//----------------------------------------------------------------

void i2cSendByte(uint8_t byte)
{
	for(uint8_t mask = 0x80; mask > 0; mask >>= 1)
	{
		i2cSendBit( byte&mask );
	}
	i2cSCL(0);
}

uint8_t i2cReceiveByte(void)
{
	uint8_t byte = 0;

	for(uint8_t mask = 0x80; mask > 0; mask >>= 1)
	{
		if( i2cReceiveBit() ) { byte |= mask; }
	}

	return byte;
}

//----------------------------------------------------------------

uint32_t i2cSendBytes(uint8_t* bytes, uint32_t numOfBytes)
{
	uint32_t numOfAcks = 0;

	for(uint8_t i = 0; i < numOfBytes; i++)
	{
		i2cConfigSDA(1);
		i2cSendByte( bytes[i] );
		i2cConfigSDA(0);
		if( !i2cReceiveBit() ) numOfAcks++;
	}
	i2cConfigSDA(1);

	return numOfAcks;
}

void i2cReceiveBytes(uint8_t* bytes, uint32_t numOfBytes)
{
	for(uint8_t i = 0; i < numOfBytes; i++)
	{
		i2cConfigSDA(0);
		bytes[i] = i2cReceiveByte();
		i2cConfigSDA(1);
		if(i != numOfBytes - 1) i2cSendBit(0);
		i2cSCL(0);
	}
	i2cSendBit(1);
}

//----------------------------------------------------------------

uint32_t i2cSendData(uint8_t addrSlave, uint8_t addrData, uint8_t* data, uint32_t numOfBytes)
{
	uint32_t numOfAcks = 0;
	uint8_t bytes[2 + numOfBytes];

	bytes[0] = addrSlave << 1;
	bytes[1] = addrData;
	memcpy(bytes + 2, data, numOfBytes);

	i2cStart();
	numOfAcks = i2cSendBytes(bytes, numOfBytes + 2);
	i2cStop();

	return numOfAcks;
}

uint8_t i2cReceiveData(uint8_t addrSlave, int16_t addrData, uint8_t* data, uint32_t numOfBytes)
{
	if( addrData >= 0 )
	{
		i2cStart();
		uint8_t bytes[2] = {addrSlave<<1, (uint8_t)(addrData&0xFF) };
		if( i2cSendBytes(bytes, 2) != 2 )
		{
			i2cStop();
			return 2;
		}
	}

	i2cStart();

	i2cSendByte( (addrSlave<<0x01)|0x01 );

	i2cConfigSDA(0);
	if( i2cReceiveBit() )
	{
		i2cStop();
		return 1;
	}

	i2cReceiveBytes(data, numOfBytes);

	i2cStop();

	return 0;
}
