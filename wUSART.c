/*
 * wUSART.c
 *
 *  Created on: Jan 15, 2023
 *      Author: DL
 */
#include "wUSART.h"
#include "string.h"

static inline uint16_t usartStrLength(uint8_t* str, uint16_t strMaxSize)
{
	for(uint16_t i = 0; i < strMaxSize; i++)
	{
		if(str[i] == '\0')	return i;
	}

	return strMaxSize;
}

static inline void usartSendBytes(USART_TypeDef * usartBase, uint8_t *bytes, uint16_t numOfBytes)
{
	while(numOfBytes--)
	{
		while (!LL_USART_IsActiveFlag_TXE(usartBase));
		LL_USART_TransmitData8(usartBase, *bytes);

		bytes++;
	}
}
static inline void usartSendString(USART_TypeDef * usartBase, char *str)
{
	while(*str)
	{
		while (!LL_USART_IsActiveFlag_TXE(usartBase));
		LL_USART_TransmitData8(usartBase, (uint8_t)*str);

		str++;
	}
}
static inline void usartReceiveHandler(USART_TypeDef * usartBase, uint8_t* txBuffer, uint8_t* rxBuffer, volatile int8_t* flag, volatile uint16_t* counter)
{
	if(LL_USART_IsActiveFlag_RXNE(usartBase))
	{
		LL_USART_ClearFlag_RXNE(usartBase);

		rxBuffer[*counter] = LL_USART_ReceiveData8(usartBase);

		if(rxBuffer[*counter] == '\n')
		{
			*flag = 1;
			*counter = 0;
		}
		else
		{
			*flag = 0;
			(*counter)++;
		}
	}
}

#if useUSART1
	static uint8_t usart1TxBuffer[usart1TxBufferSize] = {0};
	static uint8_t usart1RxBuffer[usart1RxBufferSize] = {0};

	void usart1SendByte (uint8_t byte) 	{ usartSendBytes(USART1, &byte, 1); }
	void usart1SendChar (char ch) 		{ usartSendBytes(USART1, (uint8_t*)&ch, 1); }
	void usart1SendTxBuffer (uint8_t sendFull) 		{ usartSendBytes(USART1, usart1TxBuffer, sendFull ? usart1TxBufferSize : usartStrLength(usart1TxBuffer, usart1TxBufferSize)); }

	void usart1SendBytes (uint8_t *bytes, uint16_t numOfBytes) 	{ usartSendBytes(USART1, bytes, numOfBytes); }
	void usart1SendString (char *str) 							{ usartSendString(USART1, str); }

	void usart1ClearTxBuffer(void) { memset(usart1TxBuffer, 0, usart1TxBufferSize); }
	void usart1ClearRxBuffer(void); //{ memset(usart1RxBuffer, 0, usart1RxBufferSize); }

	volatile wUSART_t wUSART1 =
	{
		usart1TxBuffer,
		usart1RxBuffer,
		0,
		0,

		usart1SendByte,
		usart1SendChar,
		usart1SendTxBuffer,

		usart1SendBytes,
		usart1SendString,

		usart1ClearTxBuffer,
		usart1ClearRxBuffer,
	};

	void usart1ClearRxBuffer(void) { memset(usart1RxBuffer, 0, usart1RxBufferSize); wUSART1.ReceiveFlag = 0; }
	void handlerReceiveUSART1(void) { usartReceiveHandler(USART1, usart1TxBuffer, usart1RxBuffer, &wUSART1.ReceiveFlag, &wUSART1.rxCounter); }
#endif

#if useUSART2
	static uint8_t usart2TxBuffer[usart2TxBufferSize] = {0};
	static uint8_t usart2RxBuffer[usart2RxBufferSize] = {0};

	void usart2SendByte (uint8_t byte) 	{ usartSendBytes(USART2, &byte, 1); }
	void usart2SendChar (char ch) 		{ usartSendBytes(USART2, (uint8_t*)&ch, 1); }
	void usart2SendTxBuffer (uint8_t sendFull) 		{ usartSendBytes(USART2, usart2TxBuffer, sendFull ? usart2TxBufferSize : usartStrLength(usart2TxBuffer, usart2TxBufferSize)); }

	void usart2SendBytes (uint8_t *bytes, uint16_t numOfBytes) 	{ usartSendBytes(USART2, bytes, numOfBytes); }
	void usart2SendString (char *str) 							{ usartSendString(USART2, str); }

	void usart2ClearTxBuffer(void) { memset(usart2TxBuffer, 0, usart2TxBufferSize); }
	void usart2ClearRxBuffer(void); // { memset(usart2RxBuffer, 0, usart2RxBufferSize); }

	volatile wUSART_t wUSART2 =
	{
		usart2TxBuffer,
		usart2RxBuffer,
		0,
		0,

		usart2SendByte,
		usart2SendChar,
		usart2SendTxBuffer,

		usart2SendBytes,
		usart2SendString,

		usart2ClearTxBuffer,
		usart2ClearRxBuffer,
	};

	void usart2ClearRxBuffer(void) { memset(usart2RxBuffer, 0, usart2RxBufferSize); wUSART2.ReceiveFlag = 0;}
	void handlerReceiveUSART2(void) { usartReceiveHandler(USART2, usart2TxBuffer, usart2RxBuffer, &wUSART2.ReceiveFlag, &wUSART2.rxCounter); }
#endif

#if useUSART3
	static uint8_t usart3TxBuffer[usart3TxBufferSize] = {0};
	static uint8_t usart3RxBuffer[usart3RxBufferSize] = {0};

	void usart3SendByte (uint8_t byte) 	{ usartSendBytes(USART3, &byte, 1); }
	void usart3SendChar (char ch) 		{ usartSendBytes(USART3, (uint8_t*)&ch, 1); }
	void usart3SendTxBuffer (uint8_t sendFull) 		{ usartSendBytes(USART3, usart3TxBuffer, sendFull ? usart3TxBufferSize : usartStrLength(usart3TxBuffer, usart3TxBufferSize)); }

	void usart3SendBytes (uint8_t *bytes, uint16_t numOfBytes) 	{ usartSendBytes(USART3, bytes, numOfBytes); }
	void usart3SendString (char *str) 							{ usartSendString(USART3, str); }

	void usart3ClearTxBuffer(void) { memset(usart3TxBuffer, 0, usart3TxBufferSize); }
	void usart3ClearRxBuffer(void); // { memset(usart3RxBuffer, 0, usart3RxBufferSize); }

	volatile wUSART_t wUSART3 =
	{
		usart3TxBuffer,
		usart3RxBuffer,
		0,
		0,

		usart3SendByte,
		usart3SendChar,
		usart3SendTxBuffer,

		usart3SendBytes,
		usart3SendString,

		usart3ClearTxBuffer,
		usart3ClearRxBuffer,
	};

	void usart3ClearRxBuffer(void) { memset(usart3RxBuffer, 0, usart3RxBufferSize); wUSART3.ReceiveFlag = 0;}
	void handlerReceiveUSART3(void) { usartReceiveHandler(USART3, usart3TxBuffer, usart3RxBuffer, &wUSART3.ReceiveFlag, &wUSART3.rxCounter); }
#endif

#if useUSART4
	static uint8_t usart4TxBuffer[usart4TxBufferSize] = {0};
	static uint8_t usart4RxBuffer[usart4RxBufferSize] = {0};

	void usart4SendByte (uint8_t byte) 	{ usartSendBytes(USART4, &byte, 1); }
	void usart4SendChar (char ch) 		{ usartSendBytes(USART4, (uint8_t*)&ch, 1); }
	void usart4SendTxBuffer (uint8_t sendFull) 		{ usartSendBytes(USART4, usart4TxBuffer, sendFull ? usart4TxBufferSize : usartStrLength(usart4TxBuffer, usart4TxBufferSize)); }

	void usart4SendBytes (uint8_t *bytes, uint16_t numOfBytes) 	{ usartSendBytes(USART4, bytes, numOfBytes); }
	void usart4SendString (char *str) 							{ usartSendString(USART4, str); }

	void usart4ClearTxBuffer(void) { memset(usart4TxBuffer, 0, usart4TxBufferSize); }
	void usart4ClearRxBuffer(void); // { memset(usart4RxBuffer, 0, usart4RxBufferSize); }

	volatile wUSART_t wUSART4 =
	{
		usart4TxBuffer,
		usart4RxBuffer,
		0,
		0,

		usart4SendByte,
		usart4SendChar,
		usart4SendTxBuffer,

		usart4SendBytes,
		usart4SendString,

		usart4ClearTxBuffer,
		usart4ClearRxBuffer,
	};

	void usart4ClearRxBuffer(void) { memset(usart4RxBuffer, 0, usart4RxBufferSize); wUSART4.ReceiveFlag = 0;}
	void handlerReceiveUSART4(void) { usartReceiveHandler(USART4, usart4TxBuffer, usart4RxBuffer, &wUSART4.ReceiveFlag, &wUSART4.rxCounter); }
#endif

#if useUSART5
	static uint8_t usart5TxBuffer[usart5TxBufferSize] = {0};
	static uint8_t usart5RxBuffer[usart5RxBufferSize] = {0};

	void usart5SendByte (uint8_t byte) 	{ usartSendBytes(USART5, &byte, 1); }
	void usart5SendChar (char ch) 		{ usartSendBytes(USART5, (uint8_t*)&ch, 1); }
	void usart5SendTxBuffer (uint8_t sendFull) 		{ usartSendBytes(USART5, usart5TxBuffer, sendFull ? usart5TxBufferSize : usartStrLength(usart5TxBuffer, usart5TxBufferSize)); }

	void usart5SendBytes (uint8_t *bytes, uint16_t numOfBytes) 	{ usartSendBytes(USART5, bytes, numOfBytes); }
	void usart5SendString (char *str) 							{ usartSendString(USART5, str); }

	void usart5ClearTxBuffer(void) { memset(usart5TxBuffer, 0, usart5TxBufferSize); }
	void usart5ClearRxBuffer(void); // { memset(usart5RxBuffer, 0, usart5RxBufferSize); }

	volatile wUSART_t wUSART5 =
	{
		usart5TxBuffer,
		usart5RxBuffer,
		0,
		0,

		usart5SendByte,
		usart5SendChar,
		usart5SendTxBuffer,

		usart5SendBytes,
		usart5SendString,

		usart5ClearTxBuffer,
		usart5ClearRxBuffer,
	};

	void usart5ClearRxBuffer(void) { memset(usart5RxBuffer, 0, usart5RxBufferSize); wUSART5.ReceiveFlag = 0;}
	void handlerReceiveUSART5(void) { usartReceiveHandler(USART5, usart5TxBuffer, usart5RxBuffer, &wUSART5.ReceiveFlag, &wUSART5.rxCounter); }
#endif
