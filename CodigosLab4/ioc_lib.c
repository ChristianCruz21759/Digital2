/*
 * File:   ioc_lib.c
 * Author: Christian Cruz 21759
 *
 * Created on July 18, 2023, 3:55 PM
 */

#include <xc.h>
#include "ioc_lib.h"

void ioc_init(char pin){
    
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    OPTION_REGbits.nRBPU = 0; //Activar pullups gloables   
    
    TRISB |= 1 << pin; //Activar el pin como entrada
    WPUB |= 1 << pin;   //Activar pullup del pin
    IOCB |= 1 << pin;   //Activar la interrupcion del pin
    
    INTCONbits.GIE = 1;         //INT globales
    INTCONbits.PEIE= 1;          //INT perifericas
    
}