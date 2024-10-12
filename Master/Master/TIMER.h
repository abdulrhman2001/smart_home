#ifndef TIMER_H_
#define TIMER_H_
void TIMER0_CTC_MODE_Init(void);
void TIMER0_STOP(void);
void TIMER0_WAVE_nonPWM(void);
void TIMER0_WAVE_fastPWM(void);
void TIMER0_WAVE_phasecorrectPWM(void);
void TIMER2_CTC_init_interrupt(void);
void TIMER1_wave_fastPWM(double x);
void TIMER1_wave_fastPWMservo(double angle);
#endif