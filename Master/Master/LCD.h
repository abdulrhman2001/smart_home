#ifndef LCD_H_
#define LCD_H_
#include "Config.h"
#define CLR_SCREEN 0x01
#define CURSOR_ON_DISPLAN_ON 0x0e
#define RETURN_HOME 0x02
#define ENTRY_MODE 0x06
#if defined four_bits_mode
#define EN 0
#define RS 1
#define RW 2
#define FOUR_BITS 0x28
#elif defined eight_bits_mode
#define EN 0
#define RS 1
#define RW 2
#define EIGHT_BITS 0x38
#endif
void LCD_Init(void);
void ENABLE(unsigned char port,unsigned char pin);
void LCD_SEND_char(unsigned char port,unsigned char data);
void LCD_SEND_CMD(unsigned char port,unsigned char cmd);
void LCD_CLR_SCREEN(void);
void LCD_Move_Cursor(unsigned char row,unsigned char coloum);
void LCD_SEND_STRING(unsigned char *ptr);
#endif