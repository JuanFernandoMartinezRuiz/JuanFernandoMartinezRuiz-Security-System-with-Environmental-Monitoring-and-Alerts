#include <xc.h>
#include "LCD.h"
#include "CONFIG.h"

void LCD_Init(void)
{
    LCD_Port = 0;         
    __delay_ms(15);       
    LCD_Command(0x02);   
    LCD_Command(0x28);    
    LCD_Command(0x01);    
    LCD_Command(0x0c);    
    LCD_Command(0x06);    
}

void LCD_Command(unsigned char cmd )
{
	ldata = (ldata & 0x0f) |(0xF0 & cmd); 
	RS = 0;  
	EN = 1;  
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (cmd<<4)); 
	EN = 1;
	NOP();
	EN = 0;
	__delay_ms(3);
}

void LCD_Char(unsigned char dat)
{
	ldata = (ldata & 0x0f) | (0xF0 & dat);  
	RS = 1;  
	EN = 1;  
	NOP();
	EN = 0;
	__delay_ms(1);
    ldata = (unsigned char)((ldata & 0x0f) | (dat<<4));  
	EN = 1;  
	NOP();
	EN = 0;
	__delay_ms(3);
}

void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f);  
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f);  
        LCD_Command(location);    
    }  
    LCD_String(msg);
}

void LCD_Clear(void)
{
   	LCD_Command(0x01);
    __delay_ms(3);
}

