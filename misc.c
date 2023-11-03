/*
 * misc.c
 *
 *  Created on: 14 мар. 2023 г.
 *      Author: DL
 */

#include "misc.h"

uint8_t miscIntLen(int64_t x)
{
	if(x < 0) x *= -1;

    if (x >= 1000000000000000000) 	return 19;
    if (x >= 100000000000000000)  	return 18;
    if (x >= 10000000000000000)   	return 17;
    if (x >= 1000000000000000)    	return 16;
    if (x >= 100000000000000)     	return 15;
    if (x >= 10000000000000)      	return 14;
    if (x >= 1000000000000)       	return 13;
    if (x >= 100000000000)        	return 12;
    if (x >= 10000000000)         	return 11;
    if (x >= 1000000000) 			return 10;
    if (x >= 100000000)  			return 9;
    if (x >= 10000000)   			return 8;
    if (x >= 1000000)    			return 7;
    if (x >= 100000)     			return 6;
    if (x >= 10000)      			return 5;
    if (x >= 1000)       			return 4;
    if (x >= 100)        			return 3;
    if (x >= 10)         			return 2;
    return 1;
}
