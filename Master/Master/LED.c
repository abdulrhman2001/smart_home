#include "DIO.h"
void LED_vInit(unsigned char port,unsigned char pin)
{
	DIO_SET_PIN_DIR(port,pin,1);
}
void LED_ON(unsigned char port,unsigned char pin)
{
	DIO_WRITE_PIN(port,pin,1);
}
void LED_OFF(unsigned char port,unsigned char pin)
{
	DIO_WRITE_PIN(port,pin,0);
}
void LED_TOGGLE(unsigned char port,unsigned char pin)
{
	DIO_TOGGLE_PIN(port,pin);
}
unsigned char LED_READ_STATUS(unsigned char port,unsigned char pin)
{
	return DIO_u8_READ_PIN(port,pin);
}