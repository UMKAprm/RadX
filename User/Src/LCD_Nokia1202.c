#include "stm32f10x.h"
#include "LCD_Nokia1202.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stdlib.h"
#include "stdio.h"
#include "main.h"

extern uint8_t _LCD_RAM[LCD_X*LCD_String]; // Память нашего LCD

// Отправляем байт данных дисплею
void LCD_SendByte(uint8_t mode, uint8_t c) {
  char i;
	// Опускаем ножку CS для дисплея
	LCD_CS_off;
	LCD_SCLK_off;
  // Формируем первый передаваемый бит - выбор память-команда
	if (mode) 
		{
			LCD_MOSI_on;
		}
	else 
		{
			LCD_MOSI_off;
		};
	// Проталкиваем тактовым импульсом
	LCD_delay(1);
	LCD_SCLK_on;
	LCD_delay(1);
	
	// В цикле передаем остальные биты
	for(i=0; i<8; i++) 
	{
    // Сбрасываем тактовую ножку
		LCD_SCLK_off;
    // Выставляем бит данных
		if (c & 0x80) 
		{
			LCD_MOSI_on;
		}
		else 
		{
			LCD_MOSI_off;
		};
		// Тактом ее, тактом
		LCD_SCLK_on;
		// Сдвигаем данные
    c <<= 1;
		LCD_delay(2);
	}
	LCD_CS_on;
	LCD_delay(1);
}

// Очистка памяти дисплея
void LCD_Clear(void) {
	LCD_SendByte(LCD_C, 0x40);
	LCD_SendByte(LCD_C, 0xb0);
	LCD_SendByte(LCD_C, 0x10);
	LCD_SendByte(LCD_C, 0x00);
	LCD_SendByte(LCD_C, 0xae);
  for (int index = 0; index < 864 ; index++){
    _LCD_RAM[index] = (0x00);
  }
	LCD_SendByte(LCD_C, 0xaf);
}

// Обновляем данные на экране
void LCD_Update(void) {
  for(uint8_t p = 0; p < 9; p++) 
	{
    LCD_SendByte(LCD_C, SetYAddr | p); 
    LCD_SendByte(LCD_C, SetXAddr4);
    LCD_SendByte(LCD_C, SetXAddr3);
    for(uint8_t col=0; col < LCD_X; col++)
		{
			LCD_SendByte(LCD_D, _LCD_RAM[(LCD_X * p) + col]);
    }
  }
}

// Рисование пикселя по координатам и цвету
void LCD_DrawPixel (uint8_t x, uint8_t y, uint8_t color) {
  if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >= LCD_Y)) return;

  if (color) _LCD_RAM[x+ (y/8)*LCD_X] |= 1<<(y%8);
  else       _LCD_RAM[x+ (y/8)*LCD_X] &= ~(1<<(y%8)); 
}

// Рисование линии
void LCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
  int steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) 
  {
	swap(x0, y0);
	swap(x1, y1);
  }
  if (x0 > x1) 
  {
    swap(x0, x1);
    swap(y0, y1);
  }
  int dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);
  int err = dx / 2;
  int ystep;
  if (y0 < y1) 
  {
	ystep = 1;
  }
  else 
  {
	ystep = -1;
  };
  for ( ; x0 <= x1; x0++) 
  {
    if (steep)
	{
		LCD_DrawPixel(y0, x0, color);
	}
    else
	{
		LCD_DrawPixel(x0, y0, color);
	};
		err -= dy;
    if (err < 0)
	{
		y0 += ystep;
		err += dx;
    }
  }
}

// Рисование вертикальной линии
void LCD_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, uint8_t color) {
  LCD_DrawLine(x, y, x, y+h-1, color);
}

// Рисование горизонтальной линии
void LCD_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint8_t color) {
  LCD_DrawLine(x, y, x+w-1, y, color);
}

// Рисование прямоугольника
void LCD_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  LCD_DrawFastHLine(x, y, w, color);
  LCD_DrawFastHLine(x, y+h-1, w, color);
  LCD_DrawFastVLine(x, y, h, color);
  LCD_DrawFastVLine(x+w-1, y, h, color);
}

// Рисование залитый прямоугольник
void LCD_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t i=x; i<x+w; i++) {
    LCD_DrawFastVLine(i, y, h, color);
  }
}

// Заливка экрана
void LCD_FillScreen(uint8_t color) {
  LCD_FillRect(0, 0, LCD_X, LCD_Y, color);
}

// Нарисовать букву
void LCD_DrawChar(uint8_t x, uint8_t y, uint8_t color, unsigned char c) {
  if((x >= LCD_X) ||(y >= LCD_Y) || ((x + 4) < 0) || ((y + 7) < 0)) return;
  if(c<128)            c = c-32;
  if(c>=144 && c<=175) c = c-48;
  if(c>=128 && c<=143) c = c+16;
  if(c>=176 && c<=191) c = c-48;
  if(c>191)  return;
  for (uint8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) {line = 0x00;}
    else {line = font[(c*5)+i];
    for (uint8_t j = 0; j<8; j++)
			{
				if (line & 0x01) {LCD_DrawPixel(x+i, y+j, color);}
				else {LCD_DrawPixel(x+i, y+j, !color);};
				line >>= 1;
			}
		}
  }
}

// Вывод строки
void LCD_print(uint8_t x, uint8_t y, uint8_t color, char *str) {
  unsigned char type = *str;
  if(type>=128) x=x-3;
  while(*str){ 
    LCD_DrawChar(x, y, color, *str++);
    unsigned char type = *str;
    if (type>=128) {x=x+3;}
    else {x=x+6;};
  }
}

// Вывод числовых значений
void LCD_write(uint8_t x, uint8_t y, uint8_t color, float num){
char c[10];
// sprintf(c, "text %f\n", num);
sprintf(c, "%5.2f", num);
LCD_print(x, y, color, c);
}

// Вывод картинки
void LCD_DrawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, uint8_t color) {
  for (int16_t j=0; j<h; j++) {
    for (int16_t i=0; i<w; i++ ) {
      if (bitmap[i + (j/8)*w] & 1<<(j%8)) { LCD_DrawPixel(x+i, y+j, color); }
    }
  }
}


// Инициализируем дисплей
void LCD_Init(void) {
		GPIO_InitTypeDef LCD_GPIO_InitStructure;
  	RCC_APB2PeriphClockCmd(LCD_RCC, ENABLE);

  	LCD_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12 |
																			GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	LCD_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	LCD_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init( LCD_port, &LCD_GPIO_InitStructure);
	
// Инициализация дисплея
	LCD_delay(10);
	LCD_CS_off;
	LCD_RST_off;
	LCD_delay(100);
	LCD_RST_on;
	LCD_delay(1);

	LCD_SendByte(LCD_C, 0xE2);  // Сброс чипа
  LCD_delay(5);
  // Устанавливаем энергию заряда сегмента
	LCD_SendByte(LCD_C, 0x3D);  // Умножитель энергии заряда 
  LCD_SendByte(LCD_C, 0x02); 	// Не понятное значение умножителя
  // Команда и следом данные по контрастности
	LCD_SendByte(LCD_C, 0xE1);  // Additional VOP for contrast increase
 	LCD_SendByte(LCD_C, 0x90);  // from -127 to +127
  // Устанавливаем режим работы Normal
  LCD_SendByte(LCD_C, 0xA4);  // Power saver off
	
	LCD_SendByte(LCD_C, 0x2F);  // Booster ON Voltage regulator ON Voltage follover ON
	LCD_SendByte(LCD_C, 0xA0);  // Segment driver direction select: Normal
  LCD_SendByte(LCD_C, 0xAF);  // Включение дисплея
	LCD_delay(10);
  // Очищаем, обновляем
  LCD_Clear();
  LCD_Update();
}


//	Задержка для дисплея. Можно подставить местную функцию задержки
void LCD_delay(uint16_t p) 
{
	delay_ms(p/1000);
	//p=p*5;
	//while(p-->0){};
}

void LightEnable(void)
{
	
}

void FirstBoot(void)
{
	//первый запуск дисплея	
		LCD_Clear();
	//надпись мкР/ч
		LCD_print(79,0,1,"мкР");
		LCD_DrawLine(79,8,96,8,1);
		LCD_print(79,9,1," ч ");
	
	//батарейка
		LCD_DrawRect(84,48,12,20,1);
		LCD_FillRect(86,50,8,16,1);
		LCD_FillRect(87,46,6,2,1);
		
	//график
		LCD_DrawLine(0,46,82,46,1);
		
	//накопленная доза
		LCD_print(0,48,1,"Доза:");
		LCD_print(40,48,1,"ххх");
		LCD_DrawLine(36,56,60,56,1);
		LCD_print(66,48,1,"мкР");
}
