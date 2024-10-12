#include "STD_MACROS.h"
#include <avr/io.h>
#include <avr/interrupt.h>
void TIMER0_CTC_MODE_Init(void)
{
	SET_BIT(TCCR0,WGM01);//Select CTC mode
	
	SET_BIT(TCCR0,CS02);//Select clock/1024
	SET_BIT(TCCR0,CS00);
	
	OCR0=78;//Select tick time=10ms
	
	sei();//Enable interrupt
	SET_BIT(TIMSK,OCIE0);
}
void TIMER0_STOP(void)
{
	CLR_BIT(TCCR0,CS02);
	CLR_BIT(TCCR0,CS00);
}
void TIMER0_WAVE_nonPWM(void)
{
	SET_BIT(DDRB,3);//Set OC0 as output
	
	SET_BIT(TCCR0,WGM01);//Select CTC mode
	
	OCR0=64;//Select tick time=10ms
	
	SET_BIT(TCCR0,CS02);//Select clock/1024
	SET_BIT(TCCR0,CS00);
	
	SET_BIT(TCCR0,COM00);//Toggle OC0 on compare match
}
void TIMER0_WAVE_fastPWM(void)
{
	SET_BIT(DDRB,3);//set OC0 as output pin
	
	SET_BIT(TCCR0,WGM00);//Select Fast PWM mode
	SET_BIT(TCCR0,WGM01);
	
	OCR0=64;//load value in ORC0=64 (duty cycle=25%)
	
	SET_BIT(TCCR0,CS00);//Select timer clock/1024
	//SET_BIT(TCCR0,CS01);
	SET_BIT(TCCR0,CS02);
	
	CLR_BIT(TCCR0,COM00);//Set OC0 on compare match and clear OC0 at Bottom
	SET_BIT(TCCR0,COM01);
}
void TIMER0_WAVE_phasecorrectPWM(void)
{
	SET_BIT(DDRB,3);//set OC0 as output pin
	
	SET_BIT(TCCR0,WGM00);//Select Fast PWM mode
	
	OCR0=64;//load value in ORC0=64 (duty cycle=25%)
	
	SET_BIT(TCCR0,CS00);//Select timer clock/1024
	SET_BIT(TCCR0,CS02);
	
	CLR_BIT(TCCR0,COM00);//Set OC0 on compare match and clear OC0 at Bottom
	SET_BIT(TCCR0,COM01);
}
void TIMER2_CTC_init_interrupt(void)
{
	SET_BIT(TCCR2,WGM21);//Select CTC mode
	
	SET_BIT(TCCR2,CS22);//Select clock/1024
	SET_BIT(TCCR2,CS20);
	SET_BIT(TCCR2,CS21);
	OCR2=78;//Select tick time=10ms
	
	sei();//Enable interrupt
	SET_BIT(TIMSK,OCIE2);
}
void TIMER1_wave_fastPWM(double x)
{
	SET_BIT(DDRD,5);
	SET_BIT(TCCR1A,WGM11);//Select Fast PWM mode 14 
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,WGM13);
	
	ICR1=19999;
	OCR1A=x*1000;
	
	SET_BIT(TCCR1B,CS10);//No prescaler
	
	SET_BIT(TCCR1A,COM1A1);//Clear OCR1 on compare match , set in Bottom (non-inverting mode) 
}
void TIMER1_wave_fastPWMservo(double angle)
{
	SET_BIT(DDRD,5);
	SET_BIT(TCCR1A,WGM11);//Select Fast PWM mode 14
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,WGM13);
	
	ICR1=19999;
	double x;
	if(angle>=0&&angle<=90)
	{
		x=1.5+(angle/180.0);
	}
	else
	{
		x=1.5-(-angle/180.0);
	}
	OCR1A=x*1000;
	SET_BIT(TCCR1B,CS10);//No prescaler
	
	SET_BIT(TCCR1A,COM1A1);//Clear OCR1 on compare match , set in Bottom (non-inverting mode)
}