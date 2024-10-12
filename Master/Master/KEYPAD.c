#include "DIO.h"
#include "KEYPAD.h"
void KEYPAD_Init(void)
{
	DIO_SET_PIN_DIR('D',0,1);//rows
	DIO_SET_PIN_DIR('D',1,1);
	DIO_SET_PIN_DIR('D',2,1);
	DIO_SET_PIN_DIR('D',3,1);
	
	DIO_SET_PIN_DIR('D',4,0);//coloums
	DIO_SET_PIN_DIR('D',5,0);
	DIO_SET_PIN_DIR('D',6,0);
	DIO_SET_PIN_DIR('D',7,0);
	
	DIO_ENABLE_PULL_UP('D',4,1);//set pull-up resistances 
	DIO_ENABLE_PULL_UP('D',5,1);
	DIO_ENABLE_PULL_UP('D',6,1);
	DIO_ENABLE_PULL_UP('D',7,1);
}
unsigned char KEYPAD_CHECK_PRESS(void)
{
	unsigned char arr[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'A','0','=','+'}};
	unsigned char coloum,row,x;
	unsigned char return_val=0xFF;
	for(row=0;row<=3;row++)
	{
		DIO_WRITE_PIN('D',0,1);
		DIO_WRITE_PIN('D',1,1);
		DIO_WRITE_PIN('D',2,1);
		DIO_WRITE_PIN('D',3,1);
		DIO_WRITE_PIN('D',row,0);
		for(coloum=0;coloum<=3;coloum++)
		{
			x=DIO_u8_READ_PIN('D',coloum+4);
			if(x==0)
			{
				return_val=arr[row][coloum];
				break;
			}
		}
		if(x==0)
		{
			break;
		}
	}
	return return_val;
}