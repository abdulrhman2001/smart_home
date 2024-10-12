#define F_CPU 8000000UL
#include <util/delay.h>
#include "DIO.h"
#include "LCD.h"
void LCD_Init(void)
{
	#if defined eight_bits_mode
	
	_delay_ms(200);
	DIO_SET_PORT_DIR('A',1);
	DIO_SET_PIN_DIR('B',EN,1);
	DIO_SET_PIN_DIR('B',RS,1);
	LCD_SEND_CMD('A',EIGHT_BITS);
	_delay_ms(1);
	LCD_SEND_CMD('A',CURSOR_ON_DISPLAN_ON);
	_delay_ms(1);
	LCD_SEND_CMD('A',CLR_SCREEN);
	_delay_ms(10);
	LCD_SEND_CMD('A',ENTRY_MODE);
	_delay_ms(1);
	
	#elif defined four_bits_mode
	
	_delay_ms(200);
	DIO_SET_PIN_DIR('A',4,1);
	DIO_SET_PIN_DIR('A',5,1);
	DIO_SET_PIN_DIR('A',6,1);
	DIO_SET_PIN_DIR('A',7,1);
	DIO_SET_PIN_DIR('B',EN,1);
	DIO_SET_PIN_DIR('B',RS,1);
	LCD_SEND_CMD('A',RETURN_HOME);
	_delay_ms(10);
	LCD_SEND_CMD('A',FOUR_BITS); 
	_delay_ms(1);
	LCD_SEND_CMD('A',CURSOR_ON_DISPLAN_ON);
	_delay_ms(1);
	LCD_SEND_CMD('A',CLR_SCREEN);
	_delay_ms(10);
	LCD_SEND_CMD('A',ENTRY_MODE);
	_delay_ms(1);
	
	#endif
}
void ENABLE(unsigned char port,unsigned char pin)
{
	//DIO_SET_PIN_DIR(port,pin,1);
	DIO_WRITE_PIN(port,pin,1);
	_delay_ms(2);
	DIO_WRITE_PIN(port,pin,0);
	_delay_ms(2);
}
void LCD_SEND_char(unsigned char port,unsigned char data)
{
	#if defined eight_bits_mode
	
	//DIO_SET_PORT_DIR(port,1);
	DIO_WRITE_PORT(port,data);
	DIO_WRITE_PIN('B',RS,1);
	ENABLE('B',EN);
	
	#elif defined four_bits_mode
	
	DIO_WRITE_Highnibble(port,data>>4);
	DIO_WRITE_PIN('B',RS,1);
	ENABLE('B',EN);
	DIO_WRITE_Highnibble(port,data);
	ENABLE('B',EN);
	
	#endif
}
void LCD_SEND_CMD(unsigned char port,unsigned char cmd)
{
	#if defined eight_bits_mode
	
	DIO_WRITE_PORT(port,cmd);
	DIO_WRITE_PIN('B',RS,0);
	ENABLE('B',EN);
	_delay_ms(1);
	
	#elif defined four_bits_mode
	
	DIO_WRITE_Highnibble(port,cmd>>4);
	DIO_WRITE_PIN('B',RS,0);
	ENABLE('B',EN);
	DIO_WRITE_Highnibble(port,cmd);
	ENABLE('B',EN);
	_delay_ms(1);
	
	#endif
}
void LCD_CLR_SCREEN(void)
{
	LCD_SEND_CMD('A',0x01);
}
void LCD_Move_Cursor(unsigned char row,unsigned char coloum)
{
	unsigned char data;
	if(row>2||row<1||coloum>16||coloum<1)
	{
		data=0x80;
	}
	else if(row==1)
	{
		data=0x80+coloum-1;
	}
	else if(row==2)
	{
		data=0xc0+coloum-1;
	}
	LCD_SEND_CMD('A',data);
	_delay_ms(1);
}
void LCD_SEND_STRING(unsigned char *ptr)
{
	while((*ptr)!=0)
	{
		LCD_SEND_char('A',*ptr);
		ptr++;
	}
}