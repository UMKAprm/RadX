#include "UI.h"

void printTime(RTC_TimeTypeDef *rtc_time) {
	char buffer[80] = {'\0'};
	
	sprintf(buffer,"%02d:%02d:%02d", rtc_time->RTC_Hours, rtc_time->RTC_Minutes, rtc_time->RTC_Seconds);
	LCD_print(36,61,1,buffer);
		
	LCD_Update();
}
