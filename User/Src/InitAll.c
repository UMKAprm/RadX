#include "InitAll.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "system_stm32f10x.h"
#include "LCD_Nokia1202.h"

void InitAll(void)
{		
	//инициализация тайминга
		RTC_Init();	
	
	//инициализация АЦП
		Init_ADC();
	
	//инициализация АЦП
		Init_PWM();
	
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

		// Установить источник тактирования кварц 32,768кГц
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

void Init_ADC(void)
{
	//Порт A настраивать смысла нет, все его ноги по умолчанию входы что нам и нужно
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE); //Включаем тактирование АЦП
}

void Init_PWM(void)
{
	//Включаем Таймер 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//Включем порт А
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		
	GPIO_InitTypeDef PORT;
	// Настроим ноги со светодиодами на выход
  PORT.GPIO_Pin = GPIO_Pin_2;
  //Будем использовать альтернативный режим а не обычный GPIO
  PORT.GPIO_Mode = GPIO_Mode_AF_PP;
  PORT.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &PORT);
  //Разрешаем таймеру использовать ноги PA2 для ШИМа
  TIM2->CCER |= TIM_CCER_CC3E;
	// Задаем инверсный ШИМ.
  TIM2->CCMR2|=(TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);
	//Запускаем таймер!
  TIM2->CR1 |= TIM_CR1_CEN;
	
	TIM2->CCR3=0x7FFF; //скважность 50%
	
}
