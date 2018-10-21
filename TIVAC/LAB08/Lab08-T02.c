#define PART_TM4C123GH6PM


#include <stdbool.h>    //include necessary libraries
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include "driverlib/adc.h"
#include "driverlib/systick.h"
#include "inc/tm4c123gh6pm.h"
#include "utils/Nokia5110.h"
#include "utils/Nokia5110.c"
#include <stdio.h>

int main(void)
{
    char tempArray[8];
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysTick_Init();
    startSSI0();
    initialize_screen(BACKLIGHT_ON,SSI0);
    int i;
    int max=11,current_pos=0;
    set_buttons_up_down();
    unsigned char menu_elements[12][25];
    menu_elements[0][0]='1';
    menu_elements[0][1]=0x00;
    menu_elements[1][0]='2';
    menu_elements[1][1]=0x00;
    menu_elements[2][0]='3';
    menu_elements[2][1]=0x00;
    menu_elements[3][0]='4';
    menu_elements[3][1]=0x00;
    menu_elements[4][0]='5';
    menu_elements[4][1]=0x00;
    menu_elements[5][0]='6';
    menu_elements[5][1]=0x00;
    menu_elements[6][0]='7';
    menu_elements[6][1]=0x00;
    menu_elements[7][0]='8';
    menu_elements[7][1]=0x00;
    menu_elements[8][0]='9';
    menu_elements[8][1]=0x00;
    menu_elements[9][0]='1';
    menu_elements[9][1]='0';
    menu_elements[9][2]=0x00;
    menu_elements[10][0]='1';
    menu_elements[10][1]='1';
    menu_elements[10][2]=0x00;
    menu_elements[11][0]='1';
    menu_elements[11][1]='2';
    menu_elements[11][2]=0x00;
    set_menu(menu_elements);
    clear_screen(SSI0);
    screen_write("CPE\n403!",ALIGN_CENTRE_CENTRE,SSI0);
    SysTick_Wait50ms(100);

    uint32_t ui32ADC0Value[4];      //variable for temperature calculation
        volatile uint32_t ui32TempAvg;  //variable for temperature calculation
        volatile uint32_t ui32TempValueC;   //variable for temperature calculation
        volatile uint32_t ui32TempValueF;   //variable for temperature calculation

        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
        ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_TS);
        ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_TS);
        ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_TS);
        ADCSequenceStepConfigure(ADC0_BASE,2,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
         ADCSequenceEnable(ADC0_BASE, 2);

    while(1)
    {
        clear_screen(SSI0);

        ADCIntClear(ADC0_BASE, 2);
                ADCProcessorTrigger(ADC0_BASE, 2);
                while(!ADCIntStatus(ADC0_BASE, 2, false))
                {

                }
                ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value);
                  ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
                  ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
                  ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

            sprintf(tempArray, "Temperature: %3d F, %3d C", ui32TempValueF, ui32TempValueC); 
			screen_write(tempArray, ALIGHT_CENTRE_CENTRE, SSI0);//print Fahrenheit temperature on LCD screen


      /*  char data=GPIO_PORTB_DATA_R&0x03;
        if((data==0x01) && current_pos<max)
        {
            current_pos++;
        }
        else
        {
            if((data==0x02) && current_pos>0)
            {
                current_pos--;
            }
        }
        show_menu(current_pos,SSI0);
        SysTick_Wait50ms(20);
        */
    }
    return 0;
}


// The delay parameter is in units of the 16 MHz core clock.
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}


void SysTick_Wait50ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 50ms
  }
}


void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
