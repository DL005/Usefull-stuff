/*
 * wDWT.c
 *
 *  Created on: 11 янв. 2023 г.
 *      Author: DL
 */


#include "wDWT.h"

uint32_t dwtToTicks(uint32_t value, dwtUnits_t units);
void dwtDelay(uint32_t tics);
void dwtWaitValue (uint32_t value);
void dwtUpdateUnits(void);

wDWT_t wDWT =
{
	&DWT->CYCCNT,

	dwtToTicks,
	dwtDelay,
	dwtWaitValue,
	dwtUpdateUnits
};

uint32_t dwtTicksIn[] = { 16000000 / 1000000, 16000000 / 1000, 16000000 };

void initDWT(void)
{
	SystemCoreClockUpdate();
	dwtUpdateUnits();

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t dwtToTicks(uint32_t value, dwtUnits_t units)
{
	switch( units )
	{
		case dwt_us: 	return dwtTicksIn[dwt_us] 	* value;
		case dwt_ms: 	return dwtTicksIn[dwt_ms] 	* value;
		case dwt_s:		return dwtTicksIn[dwt_s] 	* value;
		
		default: return 0;
	}
}

void dwtDelay(uint32_t ticks)
{
	DWT->CYCCNT = 0U;
	while(DWT->CYCCNT < ticks);
}

void dwtWaitValue (uint32_t value)
{
	while(DWT->CYCCNT < value);
}

void dwtUpdateUnits(void)
{
	dwtTicksIn[dwt_us] = SystemCoreClock/1000000U;
	dwtTicksIn[dwt_ms] = SystemCoreClock/1000U;
	dwtTicksIn[dwt_s] = SystemCoreClock;
}
