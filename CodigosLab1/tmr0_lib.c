/* Universidad del Valle de Guatemala
 * Archivo:   tmr0_lib.
 * Autor: Christian Cruz
 * Carné: 21759
 * July 18, 2023, 9:20 PM
 */

/*
 * El usuario debe ingresar el valor del prescaler calculado y el valor 
 * inicial de TMR0 para el tiempo que desee
 */

#include <xc.h>
#include "tmr0_lib.h"

int initial_value;

void tmr0_init(int prescaler, int preload){
    
    INTCONbits.T0IE = 1;        //Activar interrupcion tmr0
    INTCONbits.T0IF = 1;        //Bandera tmr0
    
    OPTION_REGbits.T0CS = 0;                //Configuracion TMR0
    OPTION_REGbits.PSA = 0;
    
    //Configuracion prescaler
    switch(prescaler){
        case(1):
            OPTION_REGbits.PSA = 1;
            OPTION_REGbits.PS = 0b000;
            break;
        case(2):
            OPTION_REGbits.PS = 0b000;
            break;
        case(4):
            OPTION_REGbits.PS = 0b001;
            break;
        case(8):
            OPTION_REGbits.PS = 0b010;
            break;
        case(16):
            OPTION_REGbits.PS = 0b011;
            break;
        case(32):
            OPTION_REGbits.PS = 0b100;
            break;
        case(64):
            OPTION_REGbits.PS = 0b101;
            break;
        case(128):
            OPTION_REGbits.PS = 0b110;
            break;
        case(256):
            OPTION_REGbits.PS = 0b111;
            break;
    }
    
    //valor para reiniciar el tmr0
    initial_value = preload;
    //realizar el reset de tmr0
    tmr0_reset();
    
    INTCONbits.GIE = 1;         //INT globales
}

void tmr0_reset(void){
    //valor initial de tmr0 para cade reset
    TMR0 = initial_value;
    //reset bandera
    T0IF = 0;
    
}