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

volatile uint32_t ui32TempAvg;
volatile uint32_t ui32TempValueC;
volatile uint32_t ui32TempValueF;

volatile  int i; //for loop variable


void UARTIntHandler(void)
{
    uint8_t arr[10];
    uint32_t ui32ADC0Value[4];
    uint32_t ui32Status;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    char newchar; //variable to hold character typed by the user
    UARTIntClear(UART0_BASE, ui32Status);

    while(UARTCharsAvail(UART0_BASE)) //while there are characters in the terminal
    {
        newchar = UARTCharGet(UART0_BASE); //set newchar equal to the character typed by the user
        UARTCharPut(UART0_BASE, newchar); //echo the character typed by the user
        if(newchar == 'B')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); //turn Blue LED on

        if(newchar == 'b')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00); //turn Blue LED off

        if(newchar == 'R')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02); //turn red LED on
        if(newchar == 'r')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //turn red LED off

        if(newchar == 'G')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08); //turn Green LED on
        if(newchar == 'g')
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); //turn Green LED off

        if(newchar == 'T')
        {
            ROM_ADCIntClear(ADC0_BASE, 1);
            ROM_ADCProcessorTrigger(ADC0_BASE, 1);

            while(!ROM_ADCIntStatus(ADC0_BASE, 1, false))
            {
            }

            ROM_ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
            ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
            ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
            ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
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

    }
}

int main(void) {

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

    IntMasterEnable();
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);




    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 't');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'T');
    UARTCharPut(UART0_BASE, 'h');
    UARTCharPut(UART0_BASE, 'e');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'c');
    UARTCharPut(UART0_BASE, 'm');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, ' ');

    //The section below initializes the ADC for temperature reading
    //------------------------------------------------------
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ROM_ADCHardwareOversampleConfigure(ADC0_BASE, 32);
    ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    ROM_ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ROM_ADCSequenceEnable(ADC0_BASE, 1);
    //-------------------------------------------------------

    while (1)
    {

    }

}
