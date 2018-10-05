#include <stdint.h>  //include necessary libraries
#include <stdbool.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "driverlib/systick.h"
#include "inc/hw_pwm.h"
#include "driverlib/udma.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

int main(void)
{
	uint32_t ui32Period; //variable to compute timer delays
	
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY; //this unlocks SW2 so we can use it
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0; //this is the pin for the button

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

   ui32Period = ((SysCtlClockGet() / 2) / 4)*3;
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
	uint32_t currentState = 0;
	uint32_t status = 0;
	
	status = GPIOIntStatus(BUttonBase,true);
	GPIOIntClear(ButtonBase,status);
	if(status & ButtonInt == ButtonInt)
	{
		TIMER0_CTL_R = 0;	//turn the timer off
		TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); 	//clear the interrupt caused by timer
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3, 0); //turn off all LEDS
	}
	

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

void timer1A_delaySec(int ttime)
{
	int i;
	SYSCTL_RCGCTIMER_R |= 2; 	//enable clock to timer block 1
	TIMER1_CTL_R = 0;			//disable timer before initialization
	TIMER1_CFG_R = 0x04			//16-bit option
	TIMER1_TAMR_R = 0x02;		//periodic mode and down counter
	TIMER1_TAILR_R = 64000 -1;	//TImerA interval load value reg
	TIMER1_TAPR_R = 250 -1;		//TimerA prescaler 16MHz/250 = 64000Hz
	TIMER1_ICR_R = 0x1;			//clear the TimerA timeout flag
	TIMER1_CTL_R |= 0X01;		//enable Timer A after initialization
	for(int i = 0;, i<ttime;i++)
	{
		while((TIMER1_RIS_R & 0x1) == 0)
			;					//wait for TimerA timeout flag
		TIMER1_ICR_R = 0x1;		//clear the TimerA timeout flag
	}
	
	

}