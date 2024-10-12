#include "STD_MACROS.h"
#include <avr/io.h>
void ADC_Init(void)
{
	SET_BIT(ADMUX,REFS0); //Config VREF
	SET_BIT(ADMUX,REFS1);
	SET_BIT(ADCSRA,ADEN);//enaple ADC
	
	SET_BIT(ADCSRA,ADPS2);//adjust ADC clock /64
	SET_BIT(ADCSRA,ADPS1);
	CLR_BIT(ADCSRA,ADPS0);
}
unsigned short ADC_READ(void)
{
	unsigned short read_val;
	SET_BIT(ADCSRA,ADSC);
	while((READ_BIT(ADCSRA,ADSC))==1);
	read_val=ADCL;
	read_val|=(ADCH<<8);
	return read_val;
}