#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

int main(void)
{
  /*Configures the device clock. Decides which input crystal frequency, oscillator to be used, PLL use
  and system clock divider.*/
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
  //Enable a peripheral (port F in this case)
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  //Port F GPIO Pins 1, 2, 3 are configured to be used as outputs (these are the LEDs)
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3);

  //While loop to turn on LED, wait, turn off LED, wait
  while(1){
    /*Write a value to a specific pin. 0x02 is red (pin1), 0x04 is blue (pin2), and 0x08 is green (pin3)
    Using pipes, can specify multiple LEDs to turn on
    GPIOPinWrite(base address [f in this case], bit packed rep of pin [again the LED pins], value to write to pins)
    Seems like order of the pins and values doesn't matter*/
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08);

    /*Code for the exercises are commented below:
    Exercise: turn on 2 LEDs (green and red)
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08|0x02);
    Exercise: turn on all LEDs
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08|0x04|0x02);*/

    //Specifies an amount of time to delay (500ms in this case)
    SysCtlDelay(20000000);
    //Write an off value to the pin. 0x00 is off
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
    //Specifies an amount of time to delay
    SysCtlDelay(20000000);
  }
}
