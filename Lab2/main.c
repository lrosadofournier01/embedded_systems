#include <stdint.h>
#include <stdbool.h>
#include "utils/ustdlib.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/hibernate.h"
#include "driverlib/gpio.h"
int main(void)
{
  //Set the clock frequency
  SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
  //Enables GPIO F
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  //Configure pin 3 as the output
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
  //Write a high to the pin to turn on the LED
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08);
  //Enable the hibernation module
  SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
  //Set the clock to the hibernation module
  HibernateEnableExpClk(SysCtlClockGet());
  //Enable GPIO pin state suring hibernation
  HibernateGPIORetentionEnable();
  //4 second delay
  SysCtlDelay(64000000);
  //Configure the RTC wake up parameters
  HibernateRTCSet(0);
  //Turn on the RTC
  HibernateRTCEnable();
  //Set the wakeup time to 5 seconds
  HibernateRTCMatchSet(0,5);
  //Add the wakeup pin
  HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);
  //Write low to the pin to turn off the LED
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0x00);
  //Enable hibernation module 
  HibernateRequest();
  while(1)
  {
  }
}
