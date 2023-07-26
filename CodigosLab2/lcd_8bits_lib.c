/* Universidad del Valle de Guatemala
 * Archivo:   lcd_8bits_lib.c
 * Autor: Christian Cruz
 * Carné: 21759
 * July 23, 2023, 8:44 AM
 */

#include <xc.h>
#include "lcd_8bits_lib.h"

void Lcd_8bits_Port(char a){
    
    PORTD = a;
}

void Lcd_8bits_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_8bits_Port(a);
	EN  = 1;             // => E = 1
    __delay_ms(4);
    EN  = 0;             // => E = 0
}

void Lcd_8bits_Clear()
{
	Lcd_8bits_Cmd(0x01);
}

void Lcd_8bits_Set_Cursor(char row, char column)
{
	char temp;                      //Mover cursor a un espacio de memoria
	if(row == 1)
	{
	  temp = 0x80 + column - 1;
		Lcd_8bits_Cmd(temp);

	}
	else if(row == 2)
	{
		temp = 0xC0 + column - 1;
		Lcd_8bits_Cmd(temp);
	}
}

void Lcd_8bits_Init()
{
    
    Lcd_8bits_Port(0x00);
     __delay_ms(20);
    Lcd_8bits_Cmd(0x38);
      __delay_ms(5);
    Lcd_8bits_Cmd(0x38);
    __delay_ms(11);
    Lcd_8bits_Cmd(0x38);    //Modo de 8 bits 
    /////////////////////////////////////////////////////
    
    
    Lcd_8bits_Cmd(0x0C);  //Visualizador apagado
    __delay_ms(5);
    Lcd_8bits_Cmd(0x01);  //Visualizador borrado
    Lcd_8bits_Cmd(0x06);  //Modo de entrada derecha con desplazamiento activado
  
}

void Lcd_8bits_Write_Char(char a)
{

   RS = 1;              // => RS = 1
   Lcd_8bits_Port(a);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   
}

void Lcd_8bits_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_8bits_Write_Char(a[i]);
}

void Lcd_8bits_Shift_Right()
{
	Lcd_8bits_Cmd(0x1C);      //Desplazar el visualizador a la derecha     
}

void Lcd_8bits_Shift_Left()
{
	Lcd_8bits_Cmd(0x18);      //Desplazar el visualizador a la izquierda
}

