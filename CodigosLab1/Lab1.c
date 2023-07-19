/*
 * File:   Lab1.c
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

#include <xc.h>
#include <stdio.h>
#include "ioc_lib.h"
#include "adc_config_lib.h"

void setup(void);
int counter;

#define _XTAL_FREQ 4000000

void __interrupt () isr (void){

    if (INTCONbits.RBIF) {

        // Verificar si el pushbutton de suma fue presionado
        if (!PORTBbits.RB1) {
            counter++;
        }

        // Verificar si el pushbutton de resta fue presionado
        if (!PORTBbits.RB0) {
            counter--;
        }

        INTCONbits.RBIF = 0;    // Limpiar la bandera de interrupción del puerto B
    }

    if(ADIF){
        //if(adc_get_channel == 6){
          //  PORTA = adc_read();
            //adc_change_channel(7);
        //}
        //else if(adc_get_channel == 7){
          //  PORTA = adc_read();
            //adc_change_channel(6);
        //}
        __delay_ms(10);
        PORTA = adc_read();
        ADIF = 0;
    }

}

void main(void) {
    setup();
    while(1){
        ADCON0bits.GO = 1;
        PORTD = counter;
    }
    return;
}

void setup(void){
    
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
    
    ioc_init(0);
    ioc_init(1);
    
    adc_init(7);
    
}