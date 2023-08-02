/*
 * File:   adc_config_lib.c
 * Author: Christian Cruz 21759
 *
 * Created on July 18, 2023, 4:07 PM
 */

#include <xc.h>
#include "adc_config_lib.h"

#define _XTAL_FREQ 4000000

/*
 * EL usuario deber ingresar el canal entre 0 y 13 que quiere iniciar o cambiar 
 */

void adc_init(int channel){
    
    //Configuracion ADC
    PIR1bits.ADIF = 0;            //Bandera ADC
    PIE1bits.ADIE = 1;            //INT ADC
    ADCON0bits.ADCS = 0b01;       //Conversion de reloj
    
    adc_change_channel(channel);
            
    ADCON1bits.ADFM = 0;          //Justificado a la izquierda
    
    ADCON1bits.VCFG0 = 0;         //Referencias
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADON = 1;          //GO
    
    INTCONbits.GIE = 1;         //INT globales
    INTCONbits.PEIE= 1;          //INT perifericas
}

int adc_read(){
    //Regresa el valor del adc como int
    return ADRESH;
}

void adc_change_channel(int channel){
    
    __delay_ms(20);
    //Configuracion TRIS para cada AN
    switch (channel){
        
        case 0:     //AN0
            TRISA |= 1<<0;
            ANSEL |= 1<<0;
            break;
        case 1:     //AN 
            TRISA |= 1<<1;
            ANSEL |= 1<<1;
            break;
        case 2:     //AN
            TRISA |= 1<<2;
            ANSEL |= 1<<2;
            break;
        case 3:     //AN
            TRISA |= 1<<3;
            ANSEL |= 1<<3;
            break;   
        case 4: 
            TRISA |= 1<<5;
            ANSEL |= 1<<5;
            break;
        case 5: 
            TRISE |= 1<<0;
            ANSEL |= 1<<6;
            break;
        case 6: 
            TRISE |= 1<<1;
            ANSEL |= 1<<7;
            break;
        case 7: 
            TRISE |= 1<<2;
            ANSEL |= 1<<8;
            break;
        case 8: 
            TRISB |= 1<<2;
            ANSELH |= 1<<0;
            break;
        case 9: 
            TRISB |= 1<<3;
            ANSELH |= 1<<1;
            break;
        case 10: 
            TRISB |= 1<<1;
            ANSELH |= 1<<2;
            break;
        case 11: 
            TRISB |= 1<<4;
            ANSELH |= 1<<3;
            break;
        case 12: 
            TRISB |= 1<<0;
            ANSELH |= 1<<4;
            break;  
        case 13: 
            TRISB |= 1<<5;
            ANSELH |= 1<<5;
            break; 
    }
    
    ADCON0bits.CHS = channel;      //Seleccion Canal AN0
}

int adc_get_channel(){
    //Obtener el numero de AN
    return ADCON0bits.CHS;
}
