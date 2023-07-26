/* Universidad del Valle de Guatemala
 * Archivo:   lcd_lib.
 * Autor: Christian Cruz
 * Carné: 21759
 * July 22, 2023, 9:10 PM
 */

#include <xc.h>
#include <stdio.h>
#include "lcd_lib.h"

//LCD Functions Developed by electroSome

void Lcd_Port(char a){

    D4 = (a & 1)? 1 : 0;
    
    D5 = (a & 2)? 1 : 0;
    
    D6 = (a & 4)? 1 : 0;
    
    D7 = (a & 8)? 1 : 0;

}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
    EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char row, char column)
{
	char temp,z,y;
	if(row == 1)
	{
	  temp = 0x80 + column - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(row == 2)
	{
		temp = 0xC0 + column - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);    //Inicio de modo de funcionamiento en 4 bits
  
  Lcd_Cmd(0x02);    //Modo de 4 bits, 2 lineas, 5x8 pix
  Lcd_Cmd(0x08);
  
  Lcd_Cmd(0x00);    //Activar el visualizador, encendido, cursor desactivado
  Lcd_Cmd(0x0C);    //parpadeo de cursor apagado
  
  Lcd_Cmd(0x00);    //Modo de entrada, incremento por 1
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);      //Desplazar el visualizador a la derecha
	Lcd_Cmd(0x0C);      
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);      //Desplazar el visualizador a la izquierda
	Lcd_Cmd(0x08);
}

