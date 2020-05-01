#include "stm32f10x.h"
#include "Time.h"
#include "main.h"

/*
* Как конвертировать дату можно прочитать здесь:
* https://ru.m.wikipedia.org/wiki/%D0%AE%D0%BB%D0%B8%D0%B0%D0%BD%D1%81%D0%BA%D0%B0%D1%8F_%D0%B4%D0%B0%D1%82%D0%B0
*/


RTC_TimeTypeDef	RTC_DataTime;
unsigned long time;
unsigned long t1;
int hour	= 0;
int min 	= 0;
int sec 	= 0;

void RTC_GetTime(uint32_t RTC_Counter, RTC_TimeTypeDef* RTC_TimeStruct, uint16_t corr) 
{
	time = RTC_Counter+corr;
	t1 = time/60;
	sec = time - t1*60;

	time = t1;
	t1 = time/60;
	min = time - t1*60;

	time = t1;
	t1 = time/24;
	hour = time - t1*24;

	RTC_TimeStruct->RTC_Hours = hour;
	RTC_TimeStruct->RTC_Minutes = min;
	RTC_TimeStruct->RTC_Seconds = sec;
}
