#include <avr/io.h>
#include "STD_MACROS.h"
void EEPROM_WRITE(unsigned short address,unsigned char data)
{
	EEAR=address;
	//*((volatile unsigned short*)(0x3E))=address;
	/*EEARL=(char)(address);
	EEARH=(char)(address>>8);*/
	EEDR=data;
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);
	while((READ_BIT(EECR,EEWE))==1);
}
unsigned char EEPROM_READ(unsigned short address)
{
	EEAR=address;
	SET_BIT(EECR,EERE);
	return EEDR;
}