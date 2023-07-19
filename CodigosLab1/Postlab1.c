/*
 * File:   Postlab1.c
 * Author: Christian Cruz
 *
 * Created on July 18, 2023, 3:49 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//Librerias
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "ioc_lib.h"
#include "adc_config_lib.h"
#include "tmr0_lib.h"
#include "display_lib.h"

//Prototipos y variables globales
void setup(void);
uint8_t counter = 0, display = 1, activate_display = 0;
uint8_t value1, value2, adc = 0;

#define _XTAL_FREQ 4000000

//Vector de Interrupccion
void __interrupt () isr (void){
    
    //Interrupcion timer0
    if (T0IF){
        activate_display = 1;   //Bandera para activar los display
        tmr0_reset();           //reset timer0
    }
    
    //Interrupcion ADC
    if(ADIF){
        adc = adc_read();   //Leer el valor del ADC
        ADIF = 0;           //Limpieza Bandera ADc
    }
    
    //Interrupcion botones
    if (INTCONbits.RBIF) {

        // Verificar si el pushbutton de suma fue presionado
        if (!PORTBbits.RB0) {
            counter++;
        }

        // Verificar si el pushbutton de resta fue presionado
        if (!PORTBbits.RB1) {
            counter--;
        }

        INTCONbits.RBIF = 0;    // Limpiar la bandera de interrupción del puerto B
    }



}

//Loop Principal
void main(void) {
    setup();
    while(1){
        
        //Verificar si el valor del adc es mayor a la referencia del contador
        if (adc >= counter){
            RB2 = 1;
        }else{
            RB2 = 0;
        }
        PORTA = counter;        //Puerto A igual a contador
        value1 = adc & 0x0F;    //4 bits menos significativos del adc
        value2 = adc >> 4;      //4 bits mas significativos del adc
        
        if(activate_display){   //Espacio para alternar displays
        switch(display){
            case 1:
                display_change(1, value1);  //Encender display 2 valor1
                display = 2;                //Cambiar a display 1
                break;
            case 2:
                display_change(2, value2);
                display = 1;
                break;
        }
            activate_display = 0;           //Apagar bandera
        }
        

        
        ADCON0bits.GO = 1;
    }
    return;
}

//Setup
void setup(void){
    
    //Configuracion de puertos
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    PORTA = 0;

    TRISB = 0;
    PORTB = 0;
    
    TRISC = 0;
    PORTC = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    //Iniciar los botones 0 y 1
    ioc_init(0);
    ioc_init(1);
    
    //Configuracion inicial adc
    adc_init(6);
    
    //Configuracion tmr0
    tmr0_init(2, 0);
    
    //Configuracion display
    display_init();
    
    
    
}