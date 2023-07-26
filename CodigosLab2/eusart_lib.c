/* 
 * File:   eusart_lib.c
 * Author: Christian Cruz 21759
 *
 * Created on July 18, 2023, 4:06 PM
 */

#define FREQ 8000000

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "eusart_lib.h"
#include <string.h>

void EUSART_Init(uint16_t baudrate){

    uint8_t baudL, baudH;
    uint16_t temp;
    
    temp = (FREQ/baudrate)/4 - 1;   //Calcular valor para el baud rate
    
    baudL = temp & 0x00FF;
    baudH = temp >> 8;
           
    //Configuracion EUSART
    TXSTAbits.SYNC = 0;             //Modo asincrono
    TXSTAbits.BRGH = 1;             //High speed baud rate

    BAUDCTLbits.BRG16 = 1;          //16-bit Baud Rate
    SPBRG = baudL;                 
    SPBRGH = baudH;
    
    RCSTAbits.SPEN = 1;             //Serial port enable
    RCSTAbits.RX9 = 0;              //8 bits de datos
    
    //Interrupciones EUSART
    PIR1bits.RCIF = 0;            //Bandera RX
    PIE1bits.RCIE = 1;            //INT EUSART RC
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}

void EUSART_RX_config (uint16_t baudrate){
        EUSART_Init(baudrate);
        RCSTAbits.CREN = 1;             //Habilitar para recibir datos
        
}
void EUSART_TX_config (uint16_t baudrate){
    EUSART_Init(baudrate);
    TXSTAbits.TXEN = 1;             //Habilitar para enviar datos    
}

void EUSART_write_char (char c){
    __delay_ms(15);
    TXREG = c;                      //Escribir un caracter
}
char EUSART_read_char (){
    return RCREG;                   //Regresar valor leido
}

void EUSART_write_string(char c[]){
    
    int j = strlen(c);              
    
    for(int i = 0; i <= j; ++i){
        TXREG = c[i];
        __delay_ms(15);
    }
    
}
