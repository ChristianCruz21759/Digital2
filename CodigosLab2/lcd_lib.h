/* Universidad del Valle de Guatemala
 * Archivo:   lcd_lib.h
 * Autor: Christian Cruz
 * Carné: 21759
 * July 22, 2023, 9:43 PM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#define RS RE1
#define EN RE0
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#ifndef LCD_LIB_H
#define	LCD_LIB_H

void Lcd_Port(char a);

void Lcd_Cmd(char a);

void Lcd_Clear();

void Lcd_Set_Cursor(char row, char column);

void Lcd_Init();

void Lcd_Write_Char(char a);

void Lcd_Write_String(char *a);

void Lcd_Shift_Right();

void Lcd_Shift_Left();

#endif	/* LCD_LIB_H */

