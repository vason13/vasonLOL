#include <stdint.h>  //include necessary libraries
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"


int main(void)
{
    uint32_t ui32Period; //variable to compute timer delays

    //the line below sets a 40MHz system clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //the two lines below enable the GPIO peripheral and configure the pins connected to the LEDs as outputs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //the two lines below enables the clock to the peripheral
    //and configures Timer0 as a 32-bit timer in periodic mode
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    /*divide SysCtlClockGet() by desired frequency. 2Hz
        Divide that by 4 to get a 25% duty cycle, multiply by 3 for 75%
        */

   //ui32Period = ((SysCtlClockGet() / 2) / 4)*3;
   ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

    IntEnable(INT_TIMER0A);//enables the specific vector associated with Timer0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//enables a specific event within the timer to generate an interrupt
    IntMasterEnable();//master interrupt enable API for all interrupts

    TimerEnable(TIMER0_BASE, TIMER_A);//enable the timer

    while(1) //stay in this loop until the ISR is triggered
    {
    }
}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Read the current state of the GPIO pin and
    // write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        //the line below turns all pins off
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else
    {
        //the line below turns the blue LED on
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}
