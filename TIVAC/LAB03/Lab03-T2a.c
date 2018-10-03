/*
 * Lab03-T2a.c
 *
 *  Created on: Oct 2, 2018
 *      Author: vtuas
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"



uint8_t ui8PinData; //this is responsible for the color of the LED

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //--------------------------Task 2a----------------------------------------------
    //25ns(x) = 425ms
    //x = 17 x 10^6

     //2 = red
     //4 = Blue
     //8 = green

    while(1)
    {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667); //blinks Blue
    ui8PinData=4;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667); //blinks Green
    ui8PinData=8;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);  //blinks Red
    ui8PinData=2;
    }
}
