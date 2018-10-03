#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


uint8_t ui8PinData = 2; //this is for task 1
//uint8_t ui8PinData; //this is for Task 2a and 2b

int main(void)
{
       //The line below sets a 40 MHz clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //set pins as outputs

    while(1)
     {
         //Pin1 = red
         //Pin2 = blue
         //Pin3 = green
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
         SysCtlDelay(5666667); //delay of 0.425s
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
         SysCtlDelay(5666667); //delay of 0.425s
         if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
     }

    /*

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

    */

    //---------------------Task 2b---------------------------------
    //25ns(x) = 425ms
    //x = 17 x 10^6
    /*
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
*/

}
