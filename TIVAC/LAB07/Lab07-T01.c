#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "driverlib/rom.h"
#include "inc/tm4c123gh6pm.h" //added libraries for task 1
#include "driverlib/timer.h"
#include "driverlib/debug.h"

volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;
volatile uint32_t tempF;

volatile  int i; //for loop variable



int main(void) {

    uint32_t ui32Period;
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2| GPIO_PIN_1| GPIO_PIN_3);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTCharPut(UART0_BASE, 'R');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'a');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, 'i');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 'g');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'T');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'm');
    UARTCharPut(UART0_BASE, 'p');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, '\n'); //print new line
    UARTCharPut(UART0_BASE, '\r'); //return on new line


    //The section below initializes the ADC for temperature reading
    //------------------------------------------------------
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 32);
    ROM_ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE,2,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ROM_ADCSequenceEnable(ADC0_BASE, 2);
    //-------------------------------------------------------
    //-------------------------------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); //enable peripheral timer1
       TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); //configure timer1 to periodic mode

       ui32Period = SysCtlClockGet()/2;
          TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);

          IntEnable(INT_TIMER1A);//enables the specific vector associated with Timer0A
          TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);//enables a specific event within the timer to generate an interrupt
          IntMasterEnable();//master interrupt enable API for all interrupts

          TimerEnable(TIMER1_BASE, TIMER_A);//enable the timer
    //-------------------------------------------------------

    while (1)
    {

    }

}

void Timer1IntHandler(void)
{
    uint32_t ui32ADC0Value[4];
    uint8_t arr[10];

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
                    tempF = ui32TempValueF;
                    int sizeOfInteger = 5;
                               i = 0;
                               while(ui32TempValueF != 0) //this loop converts an integer to an array of characters
                               {
                                   arr[i++] = (ui32TempValueF%10)+ '0';
                                   ui32TempValueF /=10;
                               }

                               for( i = 0; i<sizeOfInteger; i++) //this loop prints the temperature character by character
                               {
                                   UARTCharPut(UART0_BASE, arr[i]);
                               }
                               UARTCharPut(UART0_BASE, 'F'); //print F for fahrenheit
                               UARTCharPut(UART0_BASE, '\n'); //print new line
                               UARTCharPut(UART0_BASE, '\r'); //return on new line
}

