/*
 * wUSART.h
 *
 *  Created on: Jan 15, 2023
 *      Author: DL
 */

#ifndef WUSART_H_
#define WUSART_H_

#include "main.h"

#include "stm32l1xx_ll_usart.h"
#include "stdint.h"

#define useUSART1	0
#define useUSART2	0
#define useUSART3	0
#define useUSART4	0
#define useUSART5	0

typedef struct
{
	volatile uint8_t *txBuffer;
	volatile uint8_t *rxBuffer;
	volatile uint16_t rxCounter;
	volatile int8_t ReceiveFlag;

	void (*SendByte) (uint8_t byte);
	void (*SendChar) (char ch);
	void (*SendTxBuffer) (uint8_t sendFull);

	void (*SendBytes) (uint8_t *bytes, uint16_t numOfBytes);
	void (*SendString) (char *str);

	void (*ClearTx)(void);
	void (*ClearRx)(void);
} wUSART_t;

#if useUSART1
	#define usart1TxBufferSize	128
	#define usart1RxBufferSize	128

	void handlerReceiveUSART1(void);

	extern volatile wUSART_t wUSART1;
#endif

#if useUSART2
	#define usart2TxBufferSize	128
	#define usart2RxBufferSize	128

	void handlerReceiveUSART2(void);

	extern volatile wUSART_t wUSART2;
#endif

#if useUSART3
	#define usart3TxBufferSize	128
	#define usart3RxBufferSize	128

	void handlerReceiveUSART3(void);

	extern volatile wUSART_t wUSART3;
#endif

#if useUSART4
	#define usart4TxBufferSize	128
	#define usart4RxBufferSize	128

	void handlerReceiveUSART4(void);

	extern volatile wUSART_t wUSART4;
#endif

#if useUSART5
	#define usart5TxBufferSize	128
	#define usart5RxBufferSize	128

	void handlerReceiveUSART5(void);

	extern volatile wUSART_t wUSART5;
#endif

#endif /* WUSART_H_ */
