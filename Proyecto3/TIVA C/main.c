/*
 * Universidad del Valle de Guatemala
 * Proyecto 3 Digital 2
 * Christian Cruz 21759
 * Christian Campos 21760
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

#define XTAL 40000000
void InitUART();                    //Funcion para inicializar el UART1

int32_t reading1;                   //Variables para debouncers
int32_t reading2;
int32_t reading3;
int32_t reading4;
int32_t button1State;
int32_t button2State;
int32_t button3State;
int32_t button4State;
int32_t lastButton1State;
int32_t lastButton2State;
int32_t lastButton3State;
int32_t lastButton4State;
int8_t parkingState = 0;            //Variable que almacena informacion de los parqueos
int32_t data;                       //Variable dummy para leer datos el UART

extern void UART1IntHandler(void);  //Funcion externa para interrupcion del UART1

int main(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //Enable GPIOs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //Configuracion LEDS

    //  GPIOS LEDS VERDES - Salidas
    //  1 - PA_3
    //  2 - PB_7
    //  3 - PD_6
    //  4 - PE_2

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,
    GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,
    GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,
    GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,
    GPIO_PIN_2);

    //  GPIOS LEDS ROJOS - Salidas
    //  1 - PA_4
    //  2 - PE_0
    //  3 - PC_7
    //  4 - PE_1

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,
    GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,
    GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,
    GPIO_PIN_0 | GPIO_PIN_1);

    //  GPIOS LDRs - Entradas con weak pull ups
    //  1 - PA_2
    //  2 - PB_6
    //  3 - PC_6
    //  4 - PE_3


    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,
    GPIO_PIN_2);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,
    GPIO_PIN_6);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,
    GPIO_PIN_6);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
    GPIO_PIN_3);

    //Configuracion Weak_Pullup
    GPIOPadConfigSet(GPIO_PORTA_BASE,
    GPIO_PIN_2,
                     GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE,
    GPIO_PIN_6,
                     GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTC_BASE,
    GPIO_PIN_6,
                     GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE,
    GPIO_PIN_3,
                     GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);

    InitUART();                 //Iniciamos el UART1
    IntEnable(INT_UART1);       //Activamos interrupcion del UART1

    // Se habilitan las interrupciones Globales
    IntMasterEnable();

    while (1)
    {

        reading1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
        reading2 = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_6);
        reading3 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6);
        reading4 = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);


        //Debounce Boton 1
        if (lastButton1State == GPIO_PIN_2 & 0xFFFF && reading1 == 0)
        {
            button1State = 1;
        }
        else if (lastButton1State == 0 && reading1 == GPIO_PIN_2 & 0xFFFF)
        {
            button1State = 0;
        }
        //Debounce Boton 2
        if (lastButton2State == GPIO_PIN_6 & 0xFFFF && reading2 == 0)
        {
            button2State = 1;
        }
        else if (lastButton2State == 0 && reading2 == GPIO_PIN_6 & 0xFFFF)
        {
            button2State = 0;
        }
        //Debounce Boton 3
        if (lastButton3State == GPIO_PIN_6 & 0xFFFF && reading3 == 0)
        {
            button3State = 1;
        }
        else if (lastButton3State == 0 && reading3 == GPIO_PIN_6 & 0xFFFF)
        {
            button3State = 0;
        }
        //Debounce Boton 4
        if (lastButton4State == GPIO_PIN_3 & 0xFFFF && reading4 == 0)
        {
            button4State = 1;
        }
        else if (lastButton4State == 0 && reading4 == GPIO_PIN_3 & 0xFFFF)
        {
            button4State = 0;
        }


        //Condicional Parqueo 1
        if (button1State)
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);        // Espacio 1 Libre se enciende la luz verde
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
            parkingState |= 0b00000001;                             //Se hace un set al bit 0 de la variable del parqueo
        }
        else
        {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0xFF);
            parkingState &= 0b11111110;                             //Se hace un reset al bit 0 de la variable del parqueo
        }
        //Condicional Parqueo 2
        if (button2State)
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0xFF);        // Espacio 2 Libre
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
            parkingState |= 0b00000010;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xFF);
            parkingState &= 0b011111101;
        }
        //Condicional Parqueo 3
        if (button3State)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0xFF);        // Espacio 3 Libre
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
            parkingState |= 0b00000100;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x00);
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0xFF);
            parkingState &= 0b011111011;
        }
        //Condicional Parqueo 4
        if (button4State)
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0xFF);        // Espacio 4 Libre
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
            parkingState |= 0b00001000;
        }
        else
        {
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
            parkingState &= 0b011110111;
        }

        lastButton1State = reading1;
        lastButton2State = reading2;
        lastButton3State = reading3;
        lastButton4State = reading4;

    }

}


void UART1IntHandler(void)              //Rutina de interrupcion del UART1
{
    data = UARTCharGetNonBlocking(UART1_BASE);              //Leemos el valor dummy del ESP32 y lo guardamos en una variable dummy
    UARTCharPutNonBlocking(UART1_BASE, parkingState);       //Escribimos el valor actual de los parqueos al puerto UART1

}

void InitUART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); //Habilitar módulo UART1
    GPIOPinConfigure(GPIO_PB0_U1RX); //GPIO_PB0 como RX
    GPIOPinConfigure(GPIO_PB1_U1TX); //GPIO_PB1 como TX
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); //Habilitar los pines para el UART
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 38400, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE)); //UART1 con 38400 de baudrate, de 8 bits, de un dato y sin paridad
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT); //Habilitar interrupción RX y TX
    UARTEnable(UART1_BASE); //Iniciar UART1
}
