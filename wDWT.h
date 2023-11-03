/*
 * wDWT.h
 *
 *  Created on: 11 янв. 2023 г.
 *      Author: DL
 */

#ifndef WDWT_H_
#define WDWT_H_

#include "main.h"

#include "stdint.h"

typedef enum
{
	dwt_us,
	dwt_ms,
	dwt_s
}dwtUnits_t;

typedef struct
{
	volatile uint32_t* Counter;

	uint32_t (*ToTicks)(uint32_t value, dwtUnits_t units);
	void (*Delay) (uint32_t tics);
	void (*WaitValue) (uint32_t value);
	void (*UpdateUnits) (void);
}wDWT_t;

extern wDWT_t wDWT;

void initDWT(void);

#endif /* WDWT_H_ */
