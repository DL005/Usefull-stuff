/*
 * i2cSoft.h
 *
 *  Created on: Apr 11, 2023
 *      Author: DL
 */

#ifndef I2CSOFT_H_
#define I2CSOFT_H_

#include "main.h"
#include "string.h"

#define i2cSCL_Port		GPIOC
#define i2cSCL_PortClk	LL_AHB2_GRP1_PERIPH_GPIOC
#define i2cSCL_Pin		LL_GPIO_PIN_9

#define i2cSDA_Port		GPIOC
#define i2cSDA_PortClk	LL_AHB2_GRP1_PERIPH_GPIOC
#define i2cSDA_Pin		LL_GPIO_PIN_8

#define i2cPinDelay	1

void i2cInit(void);
void i2cDeinit(void);

uint32_t i2cSendData(uint8_t addrSlave, uint8_t addrData, uint8_t* data, uint32_t numOfBytes);
uint8_t i2cReceiveData(uint8_t addrSlave, int16_t addrData, uint8_t* data, uint32_t numOfBytes);

#endif /* I2CSOFT_H_ */
