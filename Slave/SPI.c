#include "DIO.h"
#include "STD_MACROS.h"
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
void SPI_vMasterInit(void)
{
	DIO_SET_PIN_DIR('B',4,1);//set SS,MOSI and SCK as output
	DIO_SET_PIN_DIR('B',5,1);
	DIO_SET_PIN_DIR('B',7,1);
	
	SET_BIT(SPCR,MSTR);//Enable Master mode
	
	SET_BIT(SPCR,SPR0);//Set clock fosc/16
	
	SET_BIT(SPCR,SPE);//Enable SPI
	
	DIO_WRITE_PIN('B',4,1);//make ss=1
}
void SPI_vSlaveInit(void)
{
	SET_BIT(SPCR,SPE);//Enable SPI
	
	DIO_SET_PIN_DIR('B',6,1);//Set MOSI as output
}
unsigned char SPI_MasterTransmitchar(unsigned char data)
{
	DIO_WRITE_PIN('B',4,0);//Clear SS to select specific SLAVE
	
	SPDR=data;
	while(READ_BIT(SPSR,SPIF)==0);
	return SPDR;
}
unsigned char SPI_SlaveReceivechar(unsigned char data)
{
	SPDR=data;
	while(READ_BIT(SPSR,SPIF)==0);
	return SPDR;
}
void SPI_MasterTransmitString(unsigned char* ptr)
{
	while((*ptr)!=0)
	{
		SPI_MasterTransmitchar(*ptr);
		_delay_ms(300);
		ptr++;
	}
}