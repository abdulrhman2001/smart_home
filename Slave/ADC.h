#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
void ADC_Init(void);
unsigned short ADC_READ(void);
#endif