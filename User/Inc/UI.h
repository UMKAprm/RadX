#ifndef UI_h
#define UI_h

#include "Time.h"
#include "LCD_Nokia1202.h"
#include "stdio.h"
#include "Time.h"

void printTime(RTC_TimeTypeDef *rtc_time);
void printBatteryLevel(void);
void printGraph(uint16_t *mass);

#endif
