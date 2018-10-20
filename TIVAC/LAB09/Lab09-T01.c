#define PART_TM4C123GH6PM //define the chipset
#define TARGET_IS_TM4C123_RB1 //define the target board

#include <stdint.h> //include necessary libraries
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "inc/tm4c123gh6pm.h"

#ifndef M_PI
#define M_PI                    3.14159265358979323846 //define float variable for Pi
#endif

#define SERIES_LENGTH 100 //this is the depth of our data buffer

float gSeriesData[SERIES_LENGTH]; //declares an array of floats

int32_t i32DataCount = 0; //counter for computation loop

int main(void)
{
    float fRadians; //variable to calculate sine

    ROM_FPULazyStackingEnable(); //turn on lazy stacking
    ROM_FPUEnable(); //turn the FPU on

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //set the system clock for 50MHz

    fRadians = ((2 * M_PI)/SERIES_LENGTH); //variable to hold the value 2pi/depth of data buffer
                             
    while(i32DataCount < SERIES_LENGTH) //calculates the sine wave for each of the 1000 values
    {
        //The line below calculates each sine value for each t value 
        gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount); 

        i32DataCount++; //increment counter
    }

    while(1) //endless loop
    {
    }
}
