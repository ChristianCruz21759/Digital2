/* 
 * Archivo:     mainMaster.c
 * Autor:       Christian Cruz 
 * Carné:       21759
 *
 * August 4, 2023, 7:05 PM
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
#include <stdbool.h>
#include "I2C.h"
#include "lcd_8bits_lib.h"
#include "ioc_lib.h"

// ** PROTOTIPOS DE FUNCIONES *********************************************** //

void setup(void);
uint8_t RTC_read(uint8_t temp);
void RTC_write(uint8_t temp, uint8_t value, uint8_t nibble);
void rtc_lcd(uint8_t a, uint8_t max);

// ** VARIABLES GLOBALES **************************************************** //

#define _XTAL_FREQ 8000000
uint8_t valPot, type = 0;
uint8_t valPot_last = 0, sec_last = 1;
bool change_flag1 = 0, change_flag2 = 0, mode = 1, change_lcd = 0, debounce = 0;
uint8_t rtc_counter[] = {10,56,30,12,12,2};     //Iniciar valores a cargar

#define sec 0x00
#define min 0x01
#define hour 0x02
#define day 0x03
#define date 0x04
#define month 0x05
#define year 0x06

#define add RB4
#define sub RB1
#define change_value RB0
#define change_mode RB3

// ** VECTOR DE INTERRUPCIÓN ************************************************ //

void __interrupt () isr (void){
    
    if (INTCONbits.RBIF) {

        if(mode == 1){
                
            
        if (!change_value) {        //Cambiar valor a modificar
            debounce = 1;
        }
            
            
        // Verificar si el pushbutton de suma fue presionado
        if (!add) {
            rtc_counter[type]++;
            change_lcd = 1;
        }

        // Verificar si el pushbutton de resta fue presionado
        if (!sub) {
            rtc_counter[type]--;
            change_lcd = 1;
        }
        

        }
        
        if (!change_mode) {         //Cambia entre modo lectua y escritura
            mode = !mode;
            
            if(mode == 0){          //modo escritura
                
                RTC_write(hour, rtc_counter[0], 0x30);
                RTC_write(min, rtc_counter[1], 0x70);
                RTC_write(sec, rtc_counter[2], 0x70);
                RTC_write(date, rtc_counter[3], 0x30);
                RTC_write(month, rtc_counter[4], 0x10);
                RTC_write(year, rtc_counter[5], 0xF0);

            }
            
        }

        INTCONbits.RBIF = 0;    // Limpiar la bandera de interrupción del puerto B
    }
    
    
}
// ** LOOP PRINCIPAL ******************************************************** //

void main(void) {
    setup();
    
    while (1) {
        
        if(debounce & change_value){        //debouncer
            type++;
            if(type >= 6){                  //cambia el valor a modificar
                type = 0;                   //rst
                }
            change_lcd = 1;
            debounce = 0;
        }
        
        RA1 = (mode)?1:0;               //led para indicar modo
        
        __delay_ms(20);

        //PIC SLAVE 1
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        valPot = I2C_Master_Read(0);        //lectura del potenciometro
        I2C_Master_Stop();

        if(valPot != valPot_last){          //verifica si el valor de pot cambia
            change_flag1 = 1;
        }
        valPot_last = valPot;
        
        if(change_flag1){                   //lcd pot
            Lcd_8bits_Set_Cursor(2, 1);
            Lcd_8bits_Write_Char(valPot/100+48);
            Lcd_8bits_Write_Char(valPot%100/10+48);
            Lcd_8bits_Write_Char(valPot%10+48);
            change_flag1 = 0;
        }
        
        if(mode == 0){                      //modo de lectura
        
        if(RTC_read(sec) != sec_last){
            change_flag2 = 1;
        }
        sec_last = RTC_read(sec);
        
        if(change_flag2){                   //lcd lectura
            Lcd_8bits_Set_Cursor(1, 7);
            Lcd_8bits_Write_Char(((RTC_read(hour) & 0x30)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(hour) & 0x0F)+48);
            Lcd_8bits_Set_Cursor(1, 10);
            Lcd_8bits_Write_Char(((RTC_read(min) & 0x70)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(min) & 0x0F)+48);
            Lcd_8bits_Set_Cursor(1, 13);
            Lcd_8bits_Write_Char(((RTC_read(sec) & 0x70)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(sec) & 0x0F)+48);

            Lcd_8bits_Set_Cursor(2, 6);
            Lcd_8bits_Write_Char(((RTC_read(date) & 0x30)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(date) & 0x0F)+48);
            Lcd_8bits_Set_Cursor(2, 9);
            Lcd_8bits_Write_Char(((RTC_read(month) & 0x10)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(month) & 0x0F)+48);
            Lcd_8bits_Set_Cursor(2, 14);
            Lcd_8bits_Write_Char(((RTC_read(year) & 0xF0)>>4)+48);
            Lcd_8bits_Write_Char((RTC_read(year) & 0x0F)+48);
            change_flag2 = 0;
        }
        
        }
        
        if(mode == 1 & change_lcd == 1){    //modo modificar
            
            switch(type){
                
                case 0:
                    Lcd_8bits_Set_Cursor(1, 7);
                    rtc_lcd(0, 25);
                    break;
                case 1:
                    Lcd_8bits_Set_Cursor(1, 10);
                    rtc_lcd(1, 60);
                    break;
                case 2:
                    Lcd_8bits_Set_Cursor(1, 13);
                    rtc_lcd(2, 60);
                    break;
                case 3:
                    Lcd_8bits_Set_Cursor(2, 6);
                    rtc_lcd(3, 32);
                    break;
                case 4:
                    Lcd_8bits_Set_Cursor(2, 9);
                    rtc_lcd(4, 13);
                    break;
                case 5:
                    Lcd_8bits_Set_Cursor(2, 14);
                    rtc_lcd(5, 100);
                    break;
            }
            change_lcd = 0;
        }

        
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
    
    I2C_Master_Init(400000);        // Inicializar Comuncación I2C
    
	I2C_Master_Start();                            // Start I2C communication

	I2C_Master_Write(0xD0);        // Connect to DS1307 by sending its ID on I2c Bus
	I2C_Master_Write(0x07);// Select the Ds1307 ControlRegister to configure Ds1307

	I2C_Master_Write(0x00);                        // Write 0x00 to Control register to disable SQW-Out

	I2C_Master_Stop();                             // Stop I2C communication after initializing DS1307
    
    Lcd_8bits_Init();

    Lcd_8bits_Clear();          //lcd inicial
    Lcd_8bits_Set_Cursor(1, 1);
    Lcd_8bits_Write_String("S1:   00:00:00");
    Lcd_8bits_Set_Cursor(2, 6);
    Lcd_8bits_Write_String("00/00/2000"); 
    
    Lcd_8bits_Set_Cursor(1, 7);
    rtc_lcd(0, 25);
    
    Lcd_8bits_Set_Cursor(1, 10);
    rtc_lcd(1, 60);

    Lcd_8bits_Set_Cursor(1, 13);
    rtc_lcd(2, 60);

    Lcd_8bits_Set_Cursor(2, 6);
    rtc_lcd(3, 32);

    Lcd_8bits_Set_Cursor(2, 9);
    rtc_lcd(4, 13);

    Lcd_8bits_Set_Cursor(2, 14);
    rtc_lcd(5, 100);
    
    
    
    
    ioc_init(0);    //Cambiar modo escritura a lectura
    ioc_init(1);    //Cambiar valor
    ioc_init(3);    //Sumar 
    ioc_init(4);    //Restar
    
    
}

// ** FUNCIONES SECUNDARIAS ************************************************* //

uint8_t RTC_read(uint8_t temp){     //funcion que lee un valor del rtc
    
    I2C_Master_Start();         
    I2C_Master_Write(0xD0);
    I2C_Master_Write(temp);
    I2C_Master_Stop();

    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    uint8_t value = I2C_Master_Read(0);
    I2C_Master_Stop();
         
    return value;   
}

//funcion para modificar un valor de la rtc
void RTC_write(uint8_t temp, uint8_t value, uint8_t nibble){

    uint8_t load = (((value/10)<<4) & nibble) | ((value%10) & 0x0F) ;
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(temp);
    I2C_Master_Write(load);
    I2C_Master_Stop();
         
    return;   
}

//funcion para mostrar el valor a precargar en la rtc en la lcd
void rtc_lcd(uint8_t a, uint8_t max){       
    
    __delay_ms(20);
    

    if(rtc_counter[a] >= max){
        rtc_counter[a] = 0;
    }
    Lcd_8bits_Write_Char((rtc_counter[a]/10)+48);
    Lcd_8bits_Write_Char((rtc_counter[a]%10)+48);
    
}
