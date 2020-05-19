#ifndef __Init
#define __Init

#include "stm32f10x.h"

void InitAll(void);
unsigned char RTC_Init(void);
void Init_ADC(void);
void Init_PWM(void);

#endif
