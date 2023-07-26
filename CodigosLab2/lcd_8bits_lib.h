/* Universidad del Valle de Guatemala
 * Archivo:   lcd_8bits_lib.h
 * Autor: Christian Cruz
 * Carné: 21759
 * July 23, 2023, 8:43 AM
 */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#define RS RE1
#define EN RE0

#ifndef LCD_8BITS_LIB_H
#define	LCD_8BITS_LIB_H

void Lcd_8bits_Port(char a);

void Lcd_8bits_Cmd(char a);

void Lcd_8bits_Clear();

void Lcd_8bits_Set_Cursor(char row, char column);

void Lcd_8bits_Init();

void Lcd_8bits_Write_Char(char a);

void Lcd_8bits_Write_String(char *a);

void Lcd_8bits_Shift_Right();

void Lcd_8bits_Shift_Left();

#endif	/* LCD_8BITS_LIB_H */

