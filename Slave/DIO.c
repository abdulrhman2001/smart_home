#include "STD_MACROS.h"
#include <avr/io.h>
void DIO_SET_PIN_DIR(unsigned char port,unsigned char pin,unsigned char dir)
{
	switch(port)
	{
		case 'A':
		case 'a':
			if(dir==1)
			{
				SET_BIT(DDRA,pin);
			}
			else
			{
				CLR_BIT(DDRA,pin);
			}
			break;
		case 'B':
		case 'b':
			if(dir==1)
			{
				SET_BIT(DDRB,pin);
			}
			else
			{
				CLR_BIT(DDRB,pin);
			}
			break;
		case 'C':
		case 'c':
			if(dir==1)
			{
				SET_BIT(DDRC,pin);
			}
			else
			{
				CLR_BIT(DDRC,pin);
			}
			break;
		case 'D':
		case 'd':
			if(dir==1)
			{
				SET_BIT(DDRD,pin);
			}
			else
			{
				CLR_BIT(DDRD,pin);
			}
			break;
		default:
		break;	
	}
}
void DIO_WRITE_PIN(unsigned char port,unsigned char pin,unsigned char val)
{
	switch(port)
	{
		case 'A':
		case 'a':
		if(val==1)
		{
			SET_BIT(PORTA,pin);
		}
		else
		{
			CLR_BIT(PORTA,pin);
		}
		break;
		case 'B':
		case 'b':
		if(val==1)
		{
			SET_BIT(PORTB,pin);
		}
		else
		{
			CLR_BIT(PORTB,pin);
		}
		break;
		case 'C':
		case 'c':
		if(val==1)
		{
			SET_BIT(PORTC,pin);
		}
		else
		{
			CLR_BIT(PORTC,pin);
		}
		break;
		case 'D':
		case 'd':
		if(val==1)
		{
			SET_BIT(PORTD,pin);
		}
		else
		{
			CLR_BIT(PORTD,pin);
		}
		break;
	}
}
void DIO_TOGGLE_PIN(unsigned char port,unsigned char pin)
{
	switch(port)
	{
		case 'A':
		case 'a':
		TOG_BIT(PORTA,pin);
		break;
		case 'B':
		case 'b':
		TOG_BIT(PORTB,pin);
		break;
		case 'C':
		case 'c':
		TOG_BIT(PORTC,pin);
		break;
		case 'D':
		case 'd':
		TOG_BIT(PORTD,pin);
		break;
	}
}
unsigned char DIO_u8_READ_PIN(unsigned char port,unsigned char pin)
{
	unsigned char val;
	switch(port)
	{
		case 'A':
		case 'a':
		val=READ_BIT(PINA,pin);
		break;
		case 'B':
		case 'b':
		val=READ_BIT(PINB,pin);
		break;
		case 'C':
		case 'c':
		val=READ_BIT(PINC,pin);
		break;
		case 'D':
		case 'd':
		val=READ_BIT(PIND,pin);
		break;
	}
	return val;	
}
void DIO_SET_PORT_DIR(unsigned char port,unsigned char dir)
{
	switch(port)
	{
		case 'A':
		case 'a':
		DDRA=dir;
		break;
		case 'B':
		case 'b':
		DDRB=dir;
		break;
		case 'C':
		case 'c':
		DDRC=dir;
		break;
		case 'D':
		case 'd':
		DDRD=dir;
		break;
	}
}
void DIO_WRITE_PORT(unsigned char port,unsigned char val)
{
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA=val;
		break;
		case 'B':
		case 'b':
		PORTB=val;
		break;
		case 'C':
		case 'c':
		PORTC=val;
		break;
		case 'D':
		case 'd':
		PORTD=val;
		break;
	}
}
unsigned char DIO_u8_READ_PORT(unsigned char port)
{
	unsigned char val;
	switch(port)
	{
		case 'A':
		case 'a':
		val=PINA;
		break;
		case 'B':
		case 'b':
		val=PINB;
		break;
		case 'C':
		case 'c':
		val=PINC;
		break;
		case 'D':
		case 'd':
		val=PIND;
		break;
	}
	return val;
}
void DIO_TOGGLE_PORT (unsigned char port)
{
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA=~PORTA;
		break;
		case 'B':
		case 'b':
		PORTB=~PORTB;
		break;
		case 'C':
		case 'c':
		PORTC=~PORTC;
		break;
		case 'D':
		case 'd':
		PORTD=~PORTD;
		break;
	}
}
void DIO_ENABLE_PULL_UP(unsigned char port,unsigned char pin,unsigned char enable)
{
	switch(port)
	{
		case 'A':
		case 'a':
		if(enable==1)
		{
			SET_BIT(PORTA,pin);
		}
		else
		{
			CLR_BIT(PORTA,pin);
		}
		break;
		case 'B':
		case 'b':
		if(enable==1)
		{
			SET_BIT(PORTB,pin);
		}
		else
		{
			CLR_BIT(PORTB,pin);
		}
		break;
		case 'C':
		case 'c':
		if(enable==1)
		{
			SET_BIT(PORTC,pin);
		}
		else
		{
			CLR_BIT(PORTC,pin);
		}
		break;
		case 'D':
		case 'd':
		if(enable==1)
		{
			SET_BIT(PORTD,pin);
		}
		else
		{
			CLR_BIT(PORTD,pin);
		}
		break;
	}
}
void DIO_WRITE_Lownibble(unsigned char port,unsigned char val)
{
	DIO_WRITE_PIN(port,0,READ_BIT(val,0));
	DIO_WRITE_PIN(port,1,READ_BIT(val,1));
	DIO_WRITE_PIN(port,2,READ_BIT(val,2));
	DIO_WRITE_PIN(port,3,READ_BIT(val,3));
	/*val&=0x0F;
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA&=0xF0;
		PORTA|=val;
		break;
		case 'B':
		case 'b':
		PORTB&=0xF0;
		PORTB|=val;
		break;
		case 'C':
		case 'c':
		PORTC&=0xF0;
		PORTC|=val;
		break;
		case 'D':
		case 'd':
		PORTD&=0xF0;
		PORTD|=val;
		break;
	}*/
}
void DIO_WRITE_Highnibble(unsigned char port,unsigned char val)
{
	DIO_WRITE_PIN(port,4,READ_BIT(val,0));
	DIO_WRITE_PIN(port,5,READ_BIT(val,1));
	DIO_WRITE_PIN(port,6,READ_BIT(val,2));
	DIO_WRITE_PIN(port,7,READ_BIT(val,3));
	/*val<<=4;
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA&=0x0F;
		PORTA|=val;
		break;
		case 'B':
		case 'b':
		PORTB&=0x0F;
		PORTB|=val;
		break;
		case 'C':
		case 'c':
		PORTC&=0x0F;
		PORTC|=val;
		break;
		case 'D':
		case 'd':
		PORTD&=0x0F;
		PORTD|=val;
		break;
	}*/
}