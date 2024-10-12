#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "DIO.h"
#include "SPI.h"
#include "TIMER.h"
#include "LED.h"
#include "KEYPAD.h"
#include "LCD.h"
#include "EEPROM.h"
#include "STD_MACROS.h"
/*STATUS LOCATION*/
#define EEPROM_ADMIN_STATUS 0x00
#define EEPROM_GUEST_STATUS 0x01
/*ADMIN PASS LOCATIONS*/
#define EEPROM_LOCATION1 0x100
#define EEPROM_LOCATION2 0x101
#define EEPROM_LOCATION3 0x102
#define EEPROM_LOCATION4 0x103
/*GUEST PASS LOCATIONS*/
#define EEPROM_LOCATION5 0x104
#define EEPROM_LOCATION6 0x105
#define EEPROM_LOCATION7 0x106
#define EEPROM_LOCATION8 0x107
/*BLOCK LOCATION*/
#define EEPROM_BLOCK 0x108
unsigned char admin_pass[4]={0xff,0xff,0xff,0xff};
unsigned char guest_pass[4]={0xff,0xff,0xff,0xff};
unsigned char admin_login=0xff;
unsigned char guest_login=0xff;
unsigned char check_mode=0xff;
unsigned char tries;
unsigned char i=0;//pass counter
unsigned char room_check=0xff;
unsigned char status_check=0xff;
unsigned char option_check=0xff;
unsigned char air_cond=0xff;
#define MAXTRIES 3
volatile unsigned short counter_timer0=0;
unsigned char flag=1;
unsigned char temp1=0xff;
unsigned char temp2=0xff;
unsigned char temp=0xff;
int main(void)
{
	LCD_Init();
	KEYPAD_Init();
	SPI_vMasterInit();
	LED_vInit('A',1);
	LED_vInit('A',2);
	LED_vInit('A',3);
	tries=MAXTRIES;
	/*Welcome message*/
	LCD_SEND_STRING("Welcome To:Smart");
	LCD_Move_Cursor(2,1);
	LCD_SEND_STRING("Home System!!");
	_delay_ms(1000);
	LCD_CLR_SCREEN();
    while(1)
    { 
		 /*Set admin Pass*/
         if((EEPROM_READ(EEPROM_ADMIN_STATUS)==0xFF)||(EEPROM_READ(EEPROM_GUEST_STATUS)==0xFF))
		 {
			 LCD_SEND_STRING("Login For");
			 LCD_Move_Cursor(2,1);
			 LCD_SEND_STRING("First Time!!");
			 _delay_ms(1000);
			 LCD_CLR_SCREEN();
			 LCD_SEND_STRING("Set Admin");
			 LCD_Move_Cursor(2,1);
			 LCD_SEND_STRING("Pass:");
			 LCD_Move_Cursor(2,6);
			 for(i=0;i<=3;i++)
			 {
				 do
				 {
					 admin_login=KEYPAD_CHECK_PRESS();
				 } while (admin_login==0xFF); 
				 EEPROM_WRITE(EEPROM_LOCATION1+i,admin_login);
				 LCD_SEND_char('A',admin_login);
				 _delay_ms(200);
				 LCD_Move_Cursor(2,6+i);
				 LCD_SEND_char('A','*');
				 admin_login=0xFF;
			 }//set admin pass
			 EEPROM_WRITE(EEPROM_ADMIN_STATUS,0);
			 _delay_ms(500);
			 LCD_CLR_SCREEN();
			 /*Set Guest Pass*/
			 LCD_SEND_STRING("Set Guest");
			 LCD_Move_Cursor(2,1);
			 LCD_SEND_STRING("Pass:");
			 LCD_Move_Cursor(2,6);
			 for(i=0;i<=3;i++)
			 {
				 do
				 {
					 admin_login=KEYPAD_CHECK_PRESS();
				 } while (admin_login==0xFF);
				 EEPROM_WRITE(EEPROM_LOCATION5+i,admin_login);
				 LCD_SEND_char('A',admin_login);
				 _delay_ms(200);
				 LCD_Move_Cursor(2,6+i);
				 LCD_SEND_char('A','*');
				 admin_login=0xFF;
			 }//set guest pass
			 EEPROM_WRITE(EEPROM_GUEST_STATUS,0);
			 _delay_ms(500);
			 LCD_CLR_SCREEN();
		 }//end of if condition to set admin pass
		 /*******************************************************************Check pass**************************************************************/
	if((EEPROM_READ(EEPROM_ADMIN_STATUS)==0x00)&&(EEPROM_READ(EEPROM_GUEST_STATUS)==0x00)&&(flag==1))
	{
		LCD_SEND_STRING("Admin Mode:0");
		LCD_Move_Cursor(2,1);
		LCD_SEND_STRING("Guest Mode:1");
		do 
		{
			check_mode=KEYPAD_CHECK_PRESS();
			_delay_ms(100);
		} while (check_mode==0xff);
		SPI_MasterTransmitchar(check_mode);
		if(check_mode=='0')
		{
			while(tries>0)
			{
				W:
						LCD_CLR_SCREEN();
						LCD_SEND_STRING("Check Admin");
						LCD_Move_Cursor(2,1);
						LCD_SEND_STRING("Password:");
						LCD_Move_Cursor(2,10);
						for(i=0;i<=3;i++)
						{
							do
							{
								admin_pass[i]=KEYPAD_CHECK_PRESS();
								_delay_ms(100);
							} while (admin_pass[i]==0xff);
							LCD_SEND_char('A',admin_pass[i]);
							LCD_Move_Cursor(2,10+i);
							_delay_ms(150);
							LCD_SEND_char('A','*');
							_delay_ms(100);
						}
						if(EEPROM_READ(EEPROM_LOCATION1)==admin_pass[0]&&EEPROM_READ(EEPROM_LOCATION2)==admin_pass[1]&&EEPROM_READ(EEPROM_LOCATION3)==admin_pass[2]&&EEPROM_READ(EEPROM_LOCATION4)==admin_pass[3])
						{
							flag=0;
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("Right Password!");
							TIMER0_CTC_MODE_Init();
							LED_ON('A',1);
							_delay_ms(1000);
							B:
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("1-ROOM1 2-ROOM2");
							LCD_Move_Cursor(2,1);
							LCD_SEND_STRING("3-ROOM3 4-MORE");
							room_check=0xff;
							do
							{
								room_check=KEYPAD_CHECK_PRESS();
							} while (room_check==0xff);
							switch(room_check)
							{
								case '1':
								SPI_MasterTransmitchar(1);
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("ROOM1 S:");
								while(option_check!='0'||flag==0)
								{
									status_check=SPI_MasterTransmitchar(0xff);
									if(status_check==1)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(status_check==0)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(100);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto B;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto B;
									}
								}
								break;
								case '2':
								SPI_MasterTransmitchar(2);
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("ROOM2 S:");
								while(option_check!='0'||flag==0)
								{
									status_check=SPI_MasterTransmitchar(0xff);
									if(status_check==1)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(status_check==0)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(100);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto B;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto B;
									}
								}
								break;
								case '3':
								SPI_MasterTransmitchar(3);
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("ROOM3 S:");
								while(option_check!='0'||flag==0)
								{
									status_check=SPI_MasterTransmitchar(0xff);
									if(status_check==1)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(status_check==0)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(100);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto B;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto B;
									}
								}
								break;
							case '4':
							SPI_MasterTransmitchar(4);
							LCD_CLR_SCREEN();
							C:
							LCD_SEND_STRING("1-ROOM4  2-TV");
							LCD_Move_Cursor(2,1);
							LCD_SEND_STRING("3-Air Cond 0-RET");
							option_check=0xff;
							do 
							{
								option_check=KEYPAD_CHECK_PRESS();
							} while (option_check==0xff);
							switch(option_check)
							{
								case '1':
								SPI_MasterTransmitchar(1);
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("ROOM4 S:");
								while(option_check!=0||flag==0)
								{
									status_check=SPI_MasterTransmitchar(0xff);
									if(status_check==1)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
									}
									else if(status_check==0)
									{
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
									}
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									V:
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(100);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("ON ");
										goto V;
									}
									 else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,9);
										LCD_SEND_STRING("OFF");
										goto V;
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto C;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto C;
									}
								}
								break;
								case '2':
								SPI_MasterTransmitchar(2);
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("TV  S:");
								while(option_check!=0||flag==0)
								{
									status_check=SPI_MasterTransmitchar(0xff);
									if(status_check==1)
									{
										LCD_Move_Cursor(1,7);
										LCD_SEND_STRING("ON ");
									}
									else if(status_check==0)
									{
										LCD_Move_Cursor(1,7);
										LCD_SEND_STRING("OFF");
									}
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									P:
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(100);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										LCD_Move_Cursor(1,7);
										LCD_SEND_STRING("ON ");
										goto P;
									}
									else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,7);
										LCD_SEND_STRING("OFF");
										goto P;
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto C;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto C;
									}
								}
								break;
								case '3':
								SPI_MasterTransmitchar(3);
								LCD_CLR_SCREEN();
								U:
								air_cond=0xff;
								option_check=0xff;
								LCD_SEND_STRING("1-Set Temp");
								LCD_Move_Cursor(2,1);
								LCD_SEND_STRING("2-Control  0-RET");
								do 
								{
									air_cond=KEYPAD_CHECK_PRESS();
								} while (air_cond==0xff);
								_delay_ms(200);
								switch(air_cond)
								{
									case '1':
									SPI_MasterTransmitchar(1);
									LCD_CLR_SCREEN();
									LCD_SEND_STRING("Temp:--");
									LCD_SEND_char('A',0xDF);
									LCD_SEND_char('A','C');
									do 
									{
										temp1=KEYPAD_CHECK_PRESS();
									} while (temp1==0xff);
									_delay_ms(200);
									LCD_Move_Cursor(1,6);
									LCD_SEND_char('A',temp1);
									do
									{
										temp2=KEYPAD_CHECK_PRESS();
									} while (temp2==0xff);
									_delay_ms(200);
									LCD_SEND_char('A',temp2);
									temp=((temp1-48)*10)+(temp2-48);
									SPI_MasterTransmitchar(temp);
									_delay_ms(500);
									LCD_CLR_SCREEN();
									goto U;
									break;
									case '2':
									flag=0;
									SPI_SlaveReceivechar(2);
									LCD_CLR_SCREEN();
									K:
									LCD_SEND_STRING("Air Cond S:");
									LCD_Move_Cursor(2,1);
									LCD_SEND_STRING("1-ON 2-OFF 0-RET");
									option_check=0xff;
									do
									{
										option_check=KEYPAD_CHECK_PRESS();
									} while (option_check==0xff);
									_delay_ms(200);
									if(option_check=='1')
									{
										SPI_MasterTransmitchar(1);
										SPI_MasterTransmitchar(2);
										LCD_Move_Cursor(1,12);
										LCD_SEND_STRING("ON ");
										LCD_CLR_SCREEN();
										_delay_ms(500);
										goto K;
									}
									else if(option_check=='2')
									{
										SPI_MasterTransmitchar(0);
										SPI_MasterTransmitchar(0);
										LCD_Move_Cursor(1,12);
										LCD_SEND_STRING("OFF");
										LCD_CLR_SCREEN();
										_delay_ms(500);
										goto K;
									}
									else if(option_check=='0')
									{
										SPI_MasterTransmitchar(3);
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										goto U;
									}
									else
									{
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto B;
									}
									break;
									case '0':
									break;
								}
								break;
								case '0':
								LCD_CLR_SCREEN();
								SPI_MasterTransmitchar(0);
								goto B;
								break;
								default:
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("Wrong Option");
								_delay_ms(1000);
								goto A;
								break;
							}
						} 							
							/*I will continue this code later*/
						}//for right password
						else
						{
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("Wrong Password!!!");
							_delay_ms(500);
							if(tries>0)
							{
								tries--;
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("Tries Left:");
								LCD_Move_Cursor(1,12);
								LCD_SEND_char('A',tries+48);
								_delay_ms(1000);
							}
							if(tries<=0)
							{
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("YOU ARE BLOCKED");
								EEPROM_WRITE(EEPROM_BLOCK,0x01);
								_delay_ms(1500); 
								if(EEPROM_READ(EEPROM_BLOCK)==0x01)
								{
									LED_ON('A',3);
								}
								LCD_CLR_SCREEN();
								unsigned char block_time=10;
								LCD_SEND_STRING("Wait    Sec");
								while(block_time>0)
								{
									LCD_Move_Cursor(1,6);
									LCD_SEND_char('A',(block_time/10)+48);
									LCD_SEND_char('A',(block_time%10)+48);
									_delay_ms(1000);
									block_time--;
								}
								EEPROM_WRITE(EEPROM_BLOCK,0xFF);
								if(EEPROM_READ(EEPROM_BLOCK)==0xFF)
								{
									LED_OFF('A',3);
								}
								LCD_CLR_SCREEN();
							}
						}
					}//end of if(check_mode==Admin)
				}//end of if(check pass)
				else if(check_mode=='1')
                {
                    tries=MAXTRIES;
                    if(tries>0)
			{
						LCD_CLR_SCREEN();
						LCD_SEND_STRING("Check Guest");
						LCD_Move_Cursor(2,1);
						LCD_SEND_STRING("Password:");
						LCD_Move_Cursor(2,10);
						for(i=0;i<=3;i++)
						{
							do
							{
								guest_pass[i]=KEYPAD_CHECK_PRESS();
								_delay_ms(100);
							} while (guest_pass[i]==0xff);
							LCD_SEND_char('A',guest_pass[i]);
							LCD_Move_Cursor(2,10+i);
							_delay_ms(150);
							LCD_SEND_char('A','*');
							_delay_ms(100);
						}
						if(EEPROM_READ(EEPROM_LOCATION5)==guest_pass[0]&&EEPROM_READ(EEPROM_LOCATION6)==guest_pass[1]&&EEPROM_READ(EEPROM_LOCATION7)==guest_pass[2]&&EEPROM_READ(EEPROM_LOCATION8)==guest_pass[3])
						{
							flag=0;
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("Right Password!");
		                	TIMER0_CTC_MODE_Init();	
							LED_ON('A',2);						
							_delay_ms(1000);
							A:
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("1-ROOM1 2-ROOM2");
							LCD_Move_Cursor(2,1);
							LCD_SEND_STRING("3-ROOM3 4-ROOM4");
							room_check=0xff;
								do 
								{
									room_check=KEYPAD_CHECK_PRESS();
								} while (room_check==0xff);
								switch(room_check)
								{
									case '1':
									SPI_MasterTransmitchar(1);
									LCD_CLR_SCREEN();
									LCD_SEND_STRING("ROOM1 S:");
									while(option_check!='0'||flag==0)
									{
										status_check=SPI_MasterTransmitchar(0xff);
										if(status_check==1)
										{
											LCD_Move_Cursor(1,9);
											LCD_SEND_STRING("ON ");
										}
										else if(status_check==0)
										{
											LCD_Move_Cursor(1,9);
											LCD_SEND_STRING("OFF");
										}
										LCD_Move_Cursor(2,1);
										LCD_SEND_STRING("1-ON 2-OFF 0-RET");
										do
										{
											option_check=KEYPAD_CHECK_PRESS();
										} while (option_check==0xff);
										_delay_ms(100);
										if(option_check=='1')
										{
											SPI_MasterTransmitchar(1);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
										}
										else if(option_check=='2')
										{
											SPI_MasterTransmitchar(0);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
										}
										else if(option_check=='0')
										{
											SPI_MasterTransmitchar(3);
											LCD_CLR_SCREEN();
										    goto A;
										}
										else
										{
											LCD_CLR_SCREEN();
											LCD_SEND_STRING("Wrong Option");
											_delay_ms(1000);
											goto A;
										}
									}
									break;
									case '2':
										SPI_MasterTransmitchar(2);
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("ROOM2 S:");
										while(option_check!=0||flag==0)
										{
											status_check=SPI_MasterTransmitchar(0xff);
											if(status_check==1)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
											}
											else if(status_check==0)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
											}
											LCD_Move_Cursor(2,1);
											LCD_SEND_STRING("1-ON 2-OFF 0-RET");
											do
											{
												option_check=KEYPAD_CHECK_PRESS();
											} while (option_check==0xff);
											_delay_ms(100);
											if(option_check=='1')
											{
												SPI_MasterTransmitchar(1);
													LCD_Move_Cursor(1,9);
													LCD_SEND_STRING("ON ");
											}
											else if(option_check=='2')
											{
												SPI_MasterTransmitchar(0);
													LCD_Move_Cursor(1,9);
													LCD_SEND_STRING("OFF");
											}
											else if(option_check=='0')
											{
												SPI_MasterTransmitchar(3);
												LCD_CLR_SCREEN();
												goto A;
											}
											else
											{
												LCD_CLR_SCREEN();
												LCD_SEND_STRING("Wrong Option");
												_delay_ms(1000);
												goto A;
											}
										}
										break;
										case '3':
										SPI_MasterTransmitchar(3);
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("ROOM3 S:");
										while(option_check!=0||flag==0)
										{
											status_check=SPI_MasterTransmitchar(0xff);
											if(status_check==1)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
											}
											else if(status_check==0)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
											}
											LCD_Move_Cursor(2,1);
											LCD_SEND_STRING("1-ON 2-OFF 0-RET");
											do
											{
												option_check=KEYPAD_CHECK_PRESS();
											} while (option_check==0xff);
											_delay_ms(100);
											if(option_check=='1')
											{
												SPI_MasterTransmitchar(1);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
											}
											else if(option_check=='2')
											{
												SPI_MasterTransmitchar(0);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
											}
											else if(option_check=='0')
											{
												SPI_MasterTransmitchar(3);
												LCD_CLR_SCREEN();
												goto A;
											}
											else
											{
												LCD_CLR_SCREEN();
												LCD_SEND_STRING("Wrong Option");
												_delay_ms(1000);
												goto A;
											}
										}
										break;
										case '4':
										SPI_MasterTransmitchar(4);
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("ROOM4 S:");
										while(option_check!=0||flag==0)
										{
											status_check=SPI_MasterTransmitchar(0xff);
											if(status_check==1)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
											}
											else if(status_check==0)
											{
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
											}
											LCD_Move_Cursor(2,1);
											LCD_SEND_STRING("1-ON 2-OFF 0-RET");
											do
											{
												option_check=KEYPAD_CHECK_PRESS();
											} while (option_check==0xff);
											_delay_ms(100);
											if(option_check=='1')
											{
												SPI_MasterTransmitchar(1);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("ON ");
											}
											else if(option_check=='2')
											{
												SPI_MasterTransmitchar(0);
												LCD_Move_Cursor(1,9);
												LCD_SEND_STRING("OFF");
											}
											else if(option_check=='0')
											{
												SPI_MasterTransmitchar(3);
												LCD_CLR_SCREEN();
												goto A;
											}
											else 
											{
												LCD_CLR_SCREEN();
												LCD_SEND_STRING("Wrong Option");
												_delay_ms(1000);
												goto A;
											}
										}
										break;
										default:
										LCD_CLR_SCREEN();
										LCD_SEND_STRING("Wrong Option");
										_delay_ms(1000);
										goto A;
								}
							/*I will continue this code later*/
						}//for right password
						else
						{
							LCD_CLR_SCREEN();
							LCD_SEND_STRING("Wrong Password!!!");
							_delay_ms(500);
							if(tries>0)
							{
								tries--;
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("Tries Left:");
								LCD_Move_Cursor(1,12);
								LCD_SEND_char('A',tries+48);
								_delay_ms(1000);
							}
							if(tries<=0)
							{
								LCD_CLR_SCREEN();
								LCD_SEND_STRING("YOU ARE BLOCKED");
								EEPROM_WRITE(EEPROM_BLOCK,0x01);
								_delay_ms(1500);
								if(EEPROM_READ(EEPROM_BLOCK)==0x01)
								{
									LED_ON('A',3);
								}
								LCD_CLR_SCREEN();
								unsigned char block_time=10;
								LCD_SEND_STRING("Wait    Sec");
								while(block_time>0)
								{
									LCD_Move_Cursor(1,6);
									LCD_SEND_char('A',(block_time/10)+48);
									LCD_SEND_char('A',(block_time%10)+48);
									_delay_ms(1000);
									block_time--;
								}
								EEPROM_WRITE(EEPROM_BLOCK,0xFF);
								if(EEPROM_READ(EEPROM_BLOCK)==0xFF)
								{
									LED_OFF('A',3);
								}
								LCD_CLR_SCREEN();
							}
						}
					}//end of if(check_mode==Admin)
                }
			}
    }//End of While(1) 
}//End of main()
ISR(TIMER0_COMP_vect)
{
	counter_timer0++;
	if(counter_timer0==20000)
	{
		LCD_CLR_SCREEN();
		LCD_SEND_STRING("Session Time Out");
		LED_OFF('A',1);
		LED_OFF('A',2);
		_delay_ms(1000);
		counter_timer0=0;
		admin_login=0xff;
		guest_login=0xff;
		check_mode=0xff;
		tries;
		i=0;//pass counter
		room_check=0xff;
		status_check=0xff;
		option_check=0xff;
		TIMER0_STOP();
		flag=1;
		main();
	}
}