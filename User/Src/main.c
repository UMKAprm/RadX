#include "stm32f10x.h"
#include "LCD_Nokia1202.h"
#include "stm32f10x_rtc.h"
#include "main.h"
#include "Time.h"
#include "ADC.h"
#include "InitAll.h"
#include "UI.h"

uint8_t _LCD_RAM[LCD_X*LCD_String];
uint16_t delay_count;

//время по умолчанию
uint8_t hour_c				=	12;
uint8_t min_c					= 33;
RTC_TimeTypeDef	RTC_Time;	//структура для часов

//режим работы
uint8_t Mode 					= 0;
uint32_t RTC_Counter 	= 0;
uint8_t count_i				=	0;
uint16_t graph_mass[96] ={1,2,3,4,5,6,7,8,9,10,11,12,13,35,36,37,38,39,40,100,0,};


void SysTick_Handler(void)
{
	if (delay_count > 0)
	{
		delay_count--;
	}
}

void delay_ms(uint16_t delay_temp)
{
	delay_count = delay_temp;
	while(delay_count){}
}

void LED_Blink(void)
{
		GPIO_SetBits( GPIOC, GPIO_Pin_8);
		GPIO_ResetBits( GPIOC, GPIO_Pin_9);
		delay_ms(1);
    GPIO_ResetBits( GPIOC, GPIO_Pin_8);
		GPIO_SetBits( GPIOC, GPIO_Pin_9);
}

int main(void)
{	
//инициализация	
	InitAll();
	FirstBoot();
	get_ADC();

	if (RTC_Init() == 1) 
	{
		// Если первая инициализация RTC устанавливаем начальное время
		delay_ms(500);
		
		printBatteryLevel();
		
		RTC_GetTime(RTC_Counter,&RTC_Time,hour_c*3600+min_c*60);	//начинаем считать время
		printTime(&RTC_Time);																			//выводим время
	}
	
	
while(1)
	{		
		if (Mode == LCD_do)
		{
			RTC_Counter = RTC_GetCounter();
			RTC_GetTime(RTC_Counter,&RTC_Time,hour_c*3600+min_c*60);
			printTime(&RTC_Time);
			
			
			Mode = 1;
		}
		else	if (Mode == ACD_do)
		{
			if (count_i == 255)
			{
				printBatteryLevel();
				count_i = 0;
			}
			count_i++;
			Mode = 2;
		}
		else	if (Mode == GRAPH_do)
		{
			printGraph(&graph_mass[0]);
			Mode = 3;
		}
		else	if (Mode == BTN_do)
		{
			
			Mode = 0;
		}
		
		
		LCD_Update();
	}
}
	
