#include "InitAll.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_conf.h"
#include "system_stm32f10x.h"
#include "LCD_Nokia1202.h"

void InitAll(void)
{		
	//инициализация тайминга
		RTC_Init();	
	
	//enable PC
		GPIO_InitTypeDef GPIO_InitStructure;

   	// Enable PORTC Periph clock  
  	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
   	// Configure PC.8 & 9 as Push Pull output at max 10Mhz
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init( GPIOC, &GPIO_InitStructure);
		
	//инициализация дисплея
		LCD_Init();
}

unsigned char RTC_Init(void)
{
// Включить тактирование модулей управления питанием и управлением резервной областью
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	// Разрешить доступ к области резервных данных
	PWR_BackupAccessCmd(ENABLE);
	// Если RTC выключен - инициализировать
	if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
	{
		// Сброс данных в резервной области
		RCC_BackupResetCmd(ENABLE);
		RCC_BackupResetCmd(DISABLE);

		// Установить источник тактирования кварц 32768
		RCC_LSEConfig(RCC_LSE_ON);
		while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		RTC_SetPrescaler(0x7FFF); // Устанавливаем делитель, чтобы часы считали секунды

		// Включаем RTC
		RCC_RTCCLKCmd(ENABLE);

		// Ждем синхронизацию
		RTC_WaitForSynchro();

		return 1;
	}
	return 0;
}

