/* 
 * Archivo:     mainSlave1.c
 * Autor:       Christian Cruz 
 * Carné:       21759
 *
 * July 30, 2023, 7:32 PM
 */

// ** PALABRA DE CONFIGURACIÓN ********************************************** //

// PIC16F887 Configuration Bit Settings

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

// ** LIBRERIAS ************************************************************* //

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "adc_config_lib.h"
#include "SPI.h"

// ** PROTOTIPOS DE FUNCIONES *********************************************** //
void setup(void);
// ** VARIABLES GLOBALES **************************************************** //

uint8_t valPot2;

// ** VECTOR DE INTERRUPCIÓN ************************************************ //

void __interrupt() isr(void) {

    if(SSPIF == 1){
        PORTD = spiRead();
        spiWrite(valPot2);
        
        SSPIF = 0;
    }
    
        //Interrupcion ADC
    if(ADIF){
        valPot2 = adc_read();   //Leer el valor del ADC
        ADIF = 0;           //Limpieza Bandera ADc
    }
   
    
}

// ** LOOP PRINCIPAL ******************************************************** //

void main(void) {
    setup();
    while (1) {

    }
    return;
}

// ** SETUP ***************************************************************** //

void setup(void) {

    //CONFIGURACIÓN DE PUERTOS

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
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //Configuracion inicial adc
    adc_init(0);

}

// ** FUNCIONES SECUNDARIAS ************************************************* //
