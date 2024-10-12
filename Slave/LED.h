#ifndef LED_H_
#define LED_H_
void LED_vInit(unsigned char port,unsigned char pin);
void LED_ON(unsigned char port,unsigned char pin);
void LED_OFF(unsigned char port,unsigned char pin);
void LED_TOGGLE(unsigned char port,unsigned char pin);
unsigned char LED_READ_STATUS(unsigned char port,unsigned char pin);
#endif 