/* Universidad del Valle de Guatemala
 * Archivo:   display_lib.
 * Autor: Christian Cruz
 * Carné: 21759
 * July 18, 2023, 9:21 PM
 */

/*
 * El usuario debera ingresar el numero de display que quiere modificar y 
 * el valor entre 0 y 15 que desee mostrar en este mismo display
 */

#include <xc.h>
#include "display_lib.h"

void display_init(void){
    //Usar PORTC como salida y poner el 0
    TRISC = 0;
    PORTC = 0x77; 
    //Usar PORTD como multiplexador
    TRISD = 0;
    PORTD = 0;

}

void display_change(int display, int value){
    
    PORTD = 0;
    //Encender el pin del PORTD que pertenece al display
    PORTD |= 1 << display - 1;
    
    PORTC = 255;
    //Mostrar el valor decodificado para el display en PORTC
    PORTC = display_decoder(value);
    
}

int display_decoder(int value){
    
    //Decode anodo comun
    //int value_decoded[16] = {0x77, 0x14, 0xB3, 0xB6, 0xD4, 0xE6, 0xE7, 0x34, 
    //0xF7, 0xF6, 0xF5, 0xC7, 0x83, 0x97, 0xE3, 0xE1};
    
    //Display catodo comun
    //Se utiliza un array con todos los valores posibles del PORTC
    int value_decoded[16] = {0x80, 0xE3, 0x44, 0x41, 0x23, 0x11, 
    0x10, 0xC3, 0x00, 0x1, 0x02, 0x30, 0x74, 0x60, 0x14, 0x16};
    //La funcion regresa un elemento del array correspondiente 
    //al numero en el display
    return value_decoded[value];
    
}