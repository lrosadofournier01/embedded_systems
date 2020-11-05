#define TARGET_IS_TM4C123_RB1
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
//Define value of Pi if it's not defined
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
//Length of the array
#define SERIES_LENGTH 100
//Define the array and initialize the data count for the loop later
float gSeriesData[SERIES_LENGTH];
int dataCount = 0;

int main(void) {
	float fRadians;
	//Enable the FPU and configure the clock
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//Calculate the radians
	fRadians = ((2 * M_PI) / SERIES_LENGTH);
	//Calculate the value for each point until it's done for 100 points
	while(dataCount < SERIES_LENGTH) {
		gSeriesData[dataCount] = sinf(fRadians * dataCount);
		dataCount++;
	}
	while(1) {	}
}
