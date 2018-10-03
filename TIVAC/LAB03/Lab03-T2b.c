#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"



uint8_t ui8PinData; //this is responsible for the color of the LED
					 //2 = red
					//4 = Blue
					//8 = green


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	//---------------------Task 2b---------------------------------
    //25ns(x) = 425ms
    //x = 17 x 10^6
    
    while(1)
    {
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667); //17000000 / 3 = 5.6mil
    ui8PinData=2; //blinks red

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=8; //blinks green

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=4;//blinks blue

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=10;//blinks RG

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=6;//blinks RB

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=12;//blinks GB

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=14;//blinks white/RGB

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=2; //blinks red

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    SysCtlDelay(5666667);//17000000 / 3 = 5.6mil
    ui8PinData=8; //blinks green
    }
}