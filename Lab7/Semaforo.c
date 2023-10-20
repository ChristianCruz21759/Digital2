#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

int32_t reading1;
int32_t button1State;
int32_t lastButton1State;

int main(void)
{
    SysCtlClockSet(
    SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Configuracion LEDS
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    //Configuracion Input
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    //Configuracion Weak_Pullup
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while (1)
    {

        reading1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);

        if (lastButton1State == GPIO_PIN_4 & 0xFFFF && reading1 == 0)
        {
            button1State = 1;
        }
        else if (lastButton1State == 0 && reading1 == GPIO_PIN_4 & 0xFFFF)
        {
            button1State = 0;
        }

        if (button1State)
        {
            //Verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);
            SysCtlDelay(20000000);
            //Apago verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            SysCtlDelay(7500000);
            //Enciendo verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);
            SysCtlDelay(7500000);
            //Apago verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            SysCtlDelay(7500000);
            //Enciendo verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);
            SysCtlDelay(7500000);
            //Apago verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            SysCtlDelay(7500000);
            //Enciendo verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);
            SysCtlDelay(7500000);
            //Apago verde
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            SysCtlDelay(7500000);
            //Amarillo
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0xFF);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xFF);
            SysCtlDelay(20000000);
            //Rojo
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
            SysCtlDelay(20000000);
        }

        lastButton1State = reading1;
    }
}
