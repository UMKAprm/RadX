#ifndef MAIN_H
#define MAIN_H

#define LCD_do 		0
#define ACD_do 		1
#define GRAPH_do 	2
#define BTN_do 		3

#include "stm32f10x.h"

void LED_Blink(void);
void delay_ms(uint16_t delay_temp);

#endif
