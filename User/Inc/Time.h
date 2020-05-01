#ifndef __Time
#define __Time

#include "stm32f10x.h"

typedef struct
{
	uint8_t RTC_Hours;
	uint8_t RTC_Minutes;
	uint8_t RTC_Seconds;
} RTC_TimeTypeDef;

void RTC_GetTime(uint32_t RTC_Counter, RTC_TimeTypeDef* RTC_TimeStruct, uint16_t corr);

#endif
