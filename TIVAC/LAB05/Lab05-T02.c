#include <stdint.h> //include all necessary libraries
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

#include "inc/tm4c123gh6pm.h" //added libraries for task 2
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#ifdef DEBUG
void__error__(char *pcFilename, uint32_t ui32Line)
{

}
#endif

uint8_t ui8PinData;
    volatile uint32_t ui32TempAvg;
     volatile uint32_t ui32TempValueC;
     volatile uint32_t ui32TempValueF;
int main(void)
{
    uint32_t ui32Period;
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCHardwareOversampleConfigure(ADC0_BASE, 32); //Hardware averaging to 32

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2); //set pins as outputs

    //For every ADC0_BASE , 1 change the 1 to a 2 to switch the ADC Sequencer to SS2
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE,2,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); //enable peripheral timer1
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); //configure timer1 to periodic mode

    ui32Period = SysCtlClockGet()/2;
       TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);

       IntEnable(INT_TIMER1A);//enables the specific vector associated with Timer0A
       TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);//enables a specific event within the timer to generate an interrupt
       IntMasterEnable();//master interrupt enable API for all interrupts

       TimerEnable(TIMER1_BASE, TIMER_A);//enable the timer


    while(1)
    {


    }

}


void Timer1IntHandler(void)
{
    uint32_t ui32ADC0Value[4];

    // Clear the timer interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Read the current state of the GPIO pin and
    // write back the opposite state
    ADCIntClear(ADC0_BASE, 2); //clear the ADC interrupt status flag
                ADCProcessorTrigger(ADC0_BASE, 2);

                while(!ADCIntStatus(ADC0_BASE, 2, false))
                {
                }

                ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);
                ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
                ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
                ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
                if(ui32TempValueF > 72) //if temperature is over this number then the LED at PF2 will turn on, if not it will stay off
                {
                    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0x0E); //turn LED on at PF2

                }
                else
                    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0x00); //turn LED off at PF2
                TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/2); //reset timer1

}
