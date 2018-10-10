#include <stdint.h> //include all necessary libraries
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

#ifdef DEBUG
void__error__(char *pcFilename, uint32_t ui32Line)
{

}
#endif

uint8_t ui8PinData;
int main(void)
{
    uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2); //set pins as outputs

    //For every ADC0_BASE , 1 change the 1 to a 2 to switch the ADC Sequencer to SS2
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE,2,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);

    while(1)
    {
        ADCIntClear(ADC0_BASE, 2); //clear the ADC interrupt status flag
              ADCProcessorTrigger(ADC0_BASE, 2);

              while(!ADCIntStatus(ADC0_BASE, 2, false))
              {
              }

              ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);
              ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
              ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
              ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
              if(ui32TempValueF > 67) //if temperature is over this number then the LED at PF2 will turn on, if not it will stay off
                                      //I lowered the 72 to 67 because I was having trouble getting the temp to reach 72 degrees due to my environment being very cold
              {
                  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0x0E); //turn LED on at PF2

              }
              else
                  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0x00); //turn LED off at PF2

    }

}
