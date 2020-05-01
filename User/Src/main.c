#include "stm32f10x.h"
#include "LCD_Nokia1202.h"
#include "stm32f10x_rtc.h"
#include "main.h"
#include "Time.h"
#include "InitAll.h"

uint8_t _LCD_RAM[LCD_X*LCD_String];
uint16_t delay_count;
uint8_t hour_c				=	12;
uint8_t min_c					= 33;
uint8_t Mode 					= 0;
uint32_t RTC_Counter 	= 0;
RTC_TimeTypeDef	RTC_Time;

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

	if (RTC_Init() == 1) 
	{
		// Если первая инициализация RTC устанавливаем начальное время
		delay_ms(500);
		
		RTC_GetTime(RTC_Counter,&RTC_Time,hour_c*3600+min_c*60);
	}
	
	
while(1)
	{		
		if (Mode == LCD_do)
		{
			RTC_Counter = RTC_GetCounter();
			RTC_GetTime(RTC_Counter,&RTC_Time,hour_c*3600+min_c*60);
			Mode = 1;
		}
		else	if (Mode == ACD_do)
		{
			
			Mode = 2;
		}
		else	if (Mode == DAC_do)
		{
			
			Mode = 3;
		}
		else	if (Mode == BTN_do)
		{
			
			Mode = 0;
		}
	}
}
	
