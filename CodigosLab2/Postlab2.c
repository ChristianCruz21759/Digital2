/* 
 * Archivo:     Postlab2.c
 * Autor:       Christian Cruz 
 * Carné:       21759
 *
 * July 24, 2023, 3:21 PM
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
#include "lcd_8bits_lib.h"
#include "adc_config_lib.h"
#include "map_lib.h"
#include "eusart_lib.h"

// ** PROTOTIPOS DE FUNCIONES *********************************************** //

void setup(void);

// ** VARIABLES GLOBALES **************************************************** //
uint8_t *ptr_potValue;
char *ptr_vol_int, *ptr_vol_dec1, *ptr_vol_dec2;
uint8_t counter = 157, menu_flag = 0;

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
// ** VECTOR DE INTERRUPCIÓN ************************************************ //

void __interrupt() isr(void) {
    
    if(PIR1bits.RCIF){
        
        if(RCREG == 43){        //Sumar al contador
            counter++;
        }
        if(RCREG == 45){        //Restar al contador0
            counter--;
        }
        if(RCREG == 49){        //Leer el potenciometro
            menu_flag = 1;
        }
        

        
    }
    if(ADIF){
        uint8_t potValue = adc_read();   //Leer el valor del ADC
        ptr_potValue = &potValue;
        ADIF = 0;           //Limpieza Bandera ADc
    }
}
    
// ** LOOP PRINCIPAL ******************************************************** //

void main(void) {
    setup();
    while (1) {
        __delay_ms(10);
        int vol = map(*ptr_potValue, 0, 255, 0, 500);       //Separar valor en
        char vol_int = vol/100 + 48;                        //centenas
        char vol_dec1 = (vol % 100)/10 + 48;                //decenas
        char vol_dec2 = vol % 10 + 48;                      //unidades
        
        ptr_vol_int = &vol_int;
        ptr_vol_dec1 = &vol_dec1;
        ptr_vol_dec2 = &vol_dec2;
        
        PORTB = vol_int;
        
        Lcd_8bits_Clear();                          //Limpiar pantalla
        Lcd_8bits_Set_Cursor(1, 2);                 //Mover cursor
        Lcd_8bits_Write_String("Pot1:   CPU:");     //Escribir string
        Lcd_8bits_Set_Cursor(2, 2);
        Lcd_8bits_Write_Char(vol_int);              //Escribir char
        Lcd_8bits_Write_Char(46);
        Lcd_8bits_Write_Char(vol_dec1);
        Lcd_8bits_Write_Char(vol_dec2);
        Lcd_8bits_Set_Cursor(2, 10);
        Lcd_8bits_Write_Char(counter/100 + 48);
        Lcd_8bits_Write_Char(counter%100/10 + 48);
        Lcd_8bits_Write_Char(counter%10 + 48);
        
        if(menu_flag){                              //Escribir menu en EUSART
            EUSART_write_char(13);
            EUSART_write_string("Valor del potenciometro: \n");
            EUSART_write_char(*ptr_vol_int);
            EUSART_write_char(46);
            EUSART_write_char(*ptr_vol_dec1);
            EUSART_write_char(*ptr_vol_dec2);
            EUSART_write_char(86);
            menu_flag = 0;
        }
        ADCON0bits.GO = 1;
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

    TRISC = 0b10000000;
    PORTC = 0;

    TRISD = 0;
    PORTD = 0;

    TRISE = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 111;
    OSCCONbits.SCS = 1;
    

    Lcd_8bits_Init();       //Inicializar la LCD en modo de 8 bits
    adc_init(0);            //Inicializar ADC
        
    EUSART_RX_config(9600); //Configuracion EUSART
    EUSART_TX_config(9600);
    
    EUSART_write_string("Valor del potenciometro: ");
        
}

// ** FUNCIONES SECUNDARIAS ************************************************* //


