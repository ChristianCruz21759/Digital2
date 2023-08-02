/* 
 * Archivo:     mainMaster.c
 * Autor:       Christian Cruz 
 * Carn�:       21759
 *
 * July 30, 2023, 7:31 PM
 */

// ** PALABRA DE CONFIGURACI�N ********************************************** //

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
#include "SPI.h"
#include "lcd_8bits_lib.h"
#include "map_lib.h"

// ** PROTOTIPOS DE FUNCIONES *********************************************** //

void setup(void);

// ** VARIABLES GLOBALES **************************************************** //

uint8_t valPot1, valPot2, valCount;

// ** VECTOR DE INTERRUPCI�N ************************************************ //

void __interrupt() isr(void) {

}

// ** LOOP PRINCIPAL ******************************************************** //

void main(void) {
    setup();
    while (1) {
        
        PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);             //Pedir valor pot1
       valPot1 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
        PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1);             //Pedir valor counter
       valCount = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
        PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);             //Pedir valor pot2
       valPot2 = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       Lcd_8bits_Set_Cursor(2, 1);
       Lcd_8bits_Write_Char((valPot1/100 + 48));
       Lcd_8bits_Write_Char((valPot1 % 100)/10 + 48);
       Lcd_8bits_Write_Char(valPot1 % 10 + 48);
       
       Lcd_8bits_Set_Cursor(2, 7);
       Lcd_8bits_Write_Char((valPot2/100 + 48));
       Lcd_8bits_Write_Char((valPot2 % 100)/10 + 48);
       Lcd_8bits_Write_Char(valPot2 % 10 + 48);
       
       Lcd_8bits_Set_Cursor(2, 13);
       Lcd_8bits_Write_Char(valCount/100 + 48);
       Lcd_8bits_Write_Char(valCount%100/10 + 48);
       Lcd_8bits_Write_Char(valCount%10 + 48);
       
    }
    return;
}

// ** SETUP ***************************************************************** //

void setup(void) {

    //CONFIGURACI�N DE PUERTOS

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
    
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    OSCCONbits.IRCF = 111;
    OSCCONbits.SCS = 1;
    
    Lcd_8bits_Init();
    
    Lcd_8bits_Clear();
    Lcd_8bits_Set_Cursor(1, 1);
    Lcd_8bits_Write_String("Pot1: Pot2: CPU:");

}

// ** FUNCIONES SECUNDARIAS ************************************************* //
