#PROJECT CODE By Engineer Denis Njunwa

#ifndef F_CPU
# define F_CPU 16000000UL
#endif
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define LCD_data PORTD
#define ctrl PORTB
#define en PB2
#define rw PB1
#define rs PB0

void LCD_cmd(unsigned char cmd), void init_LCD (void), void LCD_write(unsigned char data);
void LCD_write_string();
unsigned int press;
int main (void)
{
  DDRD = 0XFF, DDRB = 0XFF, DDRC = 0b11111110;
  init_LCD();
  LCD_write_string("smart Dehydrator");
  LCD_cmd(0x80 + 0x40 + 8);
  LCD_write_string("DIT");
  LCD_cmd(0xc0), _delay_ms(10);
  LCD_cmd(0x01);
  while (1)
  {
    ADMUX |= (1 << REFS0) | (0 << REFS1);
    ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    int16_t COUNTA = 0;
    char SHOWA [3];
    ADCSRA |= (1 << ADSC); //starting the ADC conversion
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
    COUNTA = ADC / 2;
    LCD_write_string ("Temp(C)=");
    LCD_cmd(0x80 + 0x40 + 8);
    itoa(COUNTA, SHOWA, 10);
    LCD_write_string(SHOWA);
   _delay_ms(100), LCD_cmd(0x01);
    if (COUNTA >= 66) {
      PORTB = 0b11000111;
      //Stops Fan Rotating and heater
      PORTC = 0b00000000; //00000000
 LCD_write_string ("Over heat");
      LCD_cmd(0x80 + 0x40 + 8);
      LCD_write_string(SHOWA);
      _delay_ms(300);
      LCD_cmd(0x01);
      continue;
      //break;
    }
    else if (COUNTA <= 65) {
      PORTB = 0b11000111;;
      //Rotates Fan in Clockwise
      PORTC = 0b01101000; //00000010
      _delay_ms(300);
      LCD_write_string ("Temp(C)=");
      LCD_cmd(0x80 + 0x40 + 8);
      LCD_write_string(SHOWA);
      _delay_ms(500);
      LCD_cmd(0x01);
      continue;
    } }
  return 0;
}
void init_LCD(void)
{
  LCD_cmd(0x38);
  _delay_ms(1);
  LCD_cmd(0x38);
  _delay_ms(1);
  LCD_cmd(0x38);
  _delay_ms(1);
  LCD_cmd(0x38);
  _delay_ms(1);
  LCD_cmd(0x38);
  _delay_ms(1);
  LCD_cmd(0x06);
  _delay_ms(1);
  LCD_cmd(0x0c);
  _delay_ms(1);
  LCD_cmd(0x01);
  _delay_ms(1);
  LCD_cmd(0x02);
  _delay_ms(1);
  return ;
}
void LCD_cmd(unsigned char cmd)
{
  PORTD = cmd;
  ctrl = (0 << rs) | (0 << rw) | (1 << en);
  _delay_ms(1);
  ctrl = (0 << rs) | (0 << rw) | (0 << en);
  _delay_ms(50);
  return;
}
void LCD_write(unsigned char data)
{
  PORTD = data;
  ctrl = (1 << rs) | (0 << rw) | (1 << en);
  _delay_ms(1);
  ctrl = (1 << rs) | (0 << rw) | (0 << en);
  _delay_ms(50);
  return;
}
void LCD_write_string(unsigned char *str)
{
  int i = 0;
  while (str[i] != '\0')
  {
    LCD_write(str[i]);
    i++;
  }
  return;
}
