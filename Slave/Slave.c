#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "ADC.h"
#include "DIO.h"
#include "SPI.h"
#include "TIMER.h"
#include "LED.h"
#include "EEPROM.h"
#include "STD_MACROS.h"
unsigned char room_check=0xff;
unsigned char status_check_r1=0xff; 
unsigned char status_check_r2=0xff; 
unsigned char status_check_r3=0xff; 
unsigned char status_check_r4=0xff; 
unsigned char status_check_r5=0xff; 
unsigned char option_check=0xff;
unsigned char check_mode=0xff;
unsigned char led_option=0xff;
unsigned char air_cond=0xff;
unsigned char temp=0xff;
unsigned char adc_temp=0xff;
unsigned char air_on=0xff;
volatile unsigned short counter_timer0=0;
volatile unsigned short counter_timer2=0;
int main(void)
{
	LED_vInit('A',1);
	LED_vInit('A',2);
	LED_vInit('A',3);
	LED_vInit('A',4);
	LED_vInit('A',5);
	LED_vInit('A',6);
	SPI_vSlaveInit();
	ADC_Init();
	TIMER2_CTC_init_interrupt();
	check_mode=SPI_SlaveReceivechar(0x30);
	A:
	room_check=SPI_SlaveReceivechar(0x00);
    while(1)
    {
		switch(check_mode)
		{
			case '0':
			TIMER0_CTC_MODE_Init();
			status_check_r1=DIO_u8_READ_PIN('A',1);
			status_check_r2=DIO_u8_READ_PIN('A',2);
			status_check_r3=DIO_u8_READ_PIN('A',3);
			status_check_r4=DIO_u8_READ_PIN('A',4);
			switch(room_check)
			{
				case 1:
				status_check_r1=DIO_u8_READ_PIN('A',1);
				SPI_SlaveReceivechar(status_check_r1);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',1);
				}
				if(option_check==0)
				{
					LED_OFF('A',1);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 2:
				status_check_r1=DIO_u8_READ_PIN('A',2);
				SPI_SlaveReceivechar(status_check_r2);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',2);
				}
				if(option_check==0)
				{
					LED_OFF('A',2);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 3:
				status_check_r1=DIO_u8_READ_PIN('A',3);
				SPI_SlaveReceivechar(status_check_r3);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',3);
				}
				if(option_check==0)
				{
					LED_OFF('A',3);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 4:
				L:
				option_check=SPI_SlaveReceivechar(0x00);
				switch(option_check)
				{
					case 1:
					status_check_r4=DIO_u8_READ_PIN('A',4);
					SPI_SlaveReceivechar(status_check_r4);
					D:
					led_option=SPI_SlaveReceivechar(0x30);
					if(led_option==1)
					{
						LED_ON('A',4);
						goto D;
					}
					if(led_option==0)
					{
						LED_OFF('A',4);
						goto D;
					}
					if(led_option==3)
					{
						goto L;
					}
					break;
					case 2:
					status_check_r5=DIO_u8_READ_PIN('A',5);
					SPI_SlaveReceivechar(status_check_r5);
					P:
					led_option=SPI_SlaveReceivechar(0x30);
					if(led_option==1)
					{
						LED_ON('A',5);
						goto P;
					}
					if(led_option==0)
					{
						LED_OFF('A',5);
						goto P;
					}
					if(led_option==3)
					{
						goto L;
					}
					break;
					case 3:
					U:
					air_cond=SPI_SlaveReceivechar(0x50);
					switch(air_cond)
					{
						case 1:
						temp=SPI_SlaveReceivechar(0x60);
						EEPROM_WRITE(5,temp);
						goto U;
						break;
						case 2:
						air_cond=SPI_SlaveReceivechar(0x60);
						air_on=SPI_SlaveReceivechar(0x30);
						goto U;
						break;
						case 3:
						air_on=SPI_SlaveReceivechar(0x35);
						goto U;
						break;
						case 0:
						goto U;
						break;
					}
				}
			}
			break;
			/*guest mode*/
			case '1':
			TIMER0_CTC_MODE_Init();
			_delay_ms(100);
			status_check_r1=DIO_u8_READ_PIN('A',1);
			status_check_r2=DIO_u8_READ_PIN('A',2);
			status_check_r3=DIO_u8_READ_PIN('A',3);
			status_check_r4=DIO_u8_READ_PIN('A',4);
			switch(room_check)
			{
				case 1:
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',1);
				}
				if(option_check==0)
				{
					LED_OFF('A',1);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 2:
				status_check_r1=DIO_u8_READ_PIN('A',2);
				SPI_SlaveReceivechar(status_check_r2);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',2);
				}
				if(option_check==0)
				{
					LED_OFF('A',2);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 3:
				status_check_r1=DIO_u8_READ_PIN('A',3);
				SPI_SlaveReceivechar(status_check_r3);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',3);
				}
				if(option_check==0)
				{
					LED_OFF('A',3);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
				case 4:
				status_check_r1=DIO_u8_READ_PIN('A',4);
				SPI_SlaveReceivechar(status_check_r4);
				option_check=SPI_SlaveReceivechar(0x30);
				if(option_check==1)
				{
					LED_ON('A',4);
				}
				if(option_check==0)
				{
					LED_OFF('A',4);
				}
				if(option_check==3)
				{
					goto A;
				}
				break;
			}
		}
    }
}
ISR(TIMER0_COMP_vect)
{
	counter_timer0++;
	if(counter_timer0==20000)
	{
		counter_timer0=0;
		_delay_ms(1000);
		room_check=0xff;
		status_check_r1=0xff;
		status_check_r2=0xff;
		status_check_r3=0xff;
		status_check_r4=0xff;
		status_check_r5=0xff;
		option_check=0xff;
		check_mode=0xff;
		led_option=0xff;
		TIMER0_STOP();
		main();
	}
}
ISR(TIMER2_COMP_vect)
{
	counter_timer2++;
	if(counter_timer2==10)
	{
		counter_timer2=0;
		adc_temp=((ADC_READ())*0.25);
		EEPROM_WRITE(7,adc_temp);
		if((adc_temp>=temp))
		{
			if(air_on==2)
			{
				LED_ON('A',6);
			}
			else if(air_on!=2)
			{
				LED_OFF('A',6);
			}
		}
		else
		{
			LED_OFF('A',6);
		}
	}
}