/*
 * Universidad del Valle de Guatemala
 * Archivo:   Lab2.c
 * Autor: Christian Cruz
 * Carné: 21759
 * July 23, 2023, 8:42 AM
 */

// ** PALABRA DE CONFIGURACION ********************************************** //

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

// ** LIBRERIAS

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "lcd_8bits_lib.h"
#include "adc_config_lib.h"
#include "map_lib.h"

// ** VARIABLES ************************************************************* //
uint8_t *ptr_potValue;
// ** PROTOTIPOS DE FUNCION ************************************************* //

void setup(void);

// ** VECTOR DE INTERRUPCION ************************************************ //

void __interrupt() isr(void) {
    
    if(ADIF){
        uint8_t potValue = adc_read();   //Leer el valor del ADC
        ptr_potValue = &potValue;
        ADIF = 0;           //Limpieza Bandera ADc
    }
    
}

// ** FUNCION PRINCIPAL ***************************************************** //

void main(void) {
    setup();
    while (1) {
        __delay_ms(10);
        int vol = map(*ptr_potValue, 0, 255, 0, 500);
        char vol_int = vol/100 + 48;
        char vol_dec1 = (vol % 100)/10 + 48;
        char vol_dec2 = vol % 10 + 48;
        
        PORTB = vol_int;
        
        Lcd_8bits_Clear();
        Lcd_8bits_Set_Cursor(1, 1);
        Lcd_8bits_Write_String(" Pot1:");
        Lcd_8bits_Set_Cursor(2, 2);
        Lcd_8bits_Write_Char(vol_int);
        Lcd_8bits_Write_Char(46);
        Lcd_8bits_Write_Char(vol_dec1);
        Lcd_8bits_Write_Char(vol_dec2);
        
        ADCON0bits.GO = 1;
    }
    return;
}

// ** SETUP ***************************************************************** //

void setup(void) {
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    Lcd_8bits_Init();
    adc_init(0);
    

    
    return;
}

// ** FUNCIONES SECUNDARIAS ************************************************* //


