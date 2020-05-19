#include "UI.h"

void printTime(RTC_TimeTypeDef *rtc_time) {
	char buffer[80] = {'\0'};
	
	sprintf(buffer,"%02d:%02d:%02d", rtc_time->RTC_Hours, rtc_time->RTC_Minutes, rtc_time->RTC_Seconds);
	LCD_print(36,61,1,buffer);
		
	LCD_Update();
}

void printBatteryLevel(void)
{
	char buffer[6];
	sprintf(buffer,"%d", ADC1->JDR1);
	LCD_print(0,0,1,buffer);
	LCD_FillRect(86,52,8,14,1);
	LCD_FillRect(86,52,8,1+((ADC1->JDR1)-1900)/32,0); 					//выводим первое измерение батареи
}

void printGraph(uint16_t *mass)
{
	uint8_t i;
	uint16_t max = mass[0];
	
	//поск максемального значения в массиве
	for(i=1;i<97;i++)
	{
		if (mass[i] >  max)
		{
		max = mass[i];
		}
	}
	
	//нормируем элементы массива под размер экрана
	max /= 30;
	for(i=0;i<96;i++)
	{
		mass[i] /= max; 
	}
	
	//вывод на экран
	for (i=0;i<96;i++)
	{
		LCD_DrawLine(i,46,i,46-mass[i],1);
	}
}
