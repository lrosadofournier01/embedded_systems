/*******************************************************************************
* File Name: main.c
*
* Version 1.0
*
* Description:
*  This project demonstrates the operation of the Heart Rate Profile
*  in Server (Peripheral) role.
*
* Note:
*
* Hardware Dependency:
*  CY8CKIT-042 BLE
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "hrss.h"

CYBLE_API_RESULT_T apiResult;


/*******************************************************************************
* Function Name: AppCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive events from the BLE Component.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
*******************************************************************************/
void AppCallBack(uint32 event, void* eventParam)
{
#ifdef DEBUG_OUT    
    DebugOut(event, eventParam);
#endif

    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            heartRateSimulation = DISABLED;
            /* Put the device into discoverable mode so that remote can search it. */
            StartAdvertisement();
            break;
            
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            Advertising_LED_Write(LED_OFF);
            break;

        default:
            break;
    }
}

int main()
{
    CYBLE_STACK_LIB_VERSION_T stackVersion;

    CyGlobalIntEnable;
    
    UART_DEB_Start();               /* Start communication component */
    printf("BLE Heart Rate Sensor Example Project \r\n");
    
    Disconnect_LED_Write(LED_OFF);
    Advertising_LED_Write(LED_OFF);

    /* Start CYBLE component and register generic event handler */
    apiResult = CyBle_Start(AppCallBack);
    if(apiResult != CYBLE_ERROR_OK)
    {
        printf("CyBle_Start API Error: %x \r\n", apiResult);
    }
    
    apiResult = CyBle_GetStackLibraryVersion(&stackVersion);
    if(apiResult != CYBLE_ERROR_OK)
    {
        printf("CyBle_GetStackLibraryVersion API Error: 0x%x \r\n", apiResult);
    }
    else
    {
        printf("Stack Version: %d.%d.%d.%d \r\n", stackVersion.majorVersion, 
            stackVersion.minorVersion, stackVersion.patch, stackVersion.buildNumber);
    }
    
    /* Services initialization */
    HrsInit();
    CapSense_Start();
    
    //Initialize baselines
    CapSense_InitializeAllBaselines();
    
    /***************************************************************************
    * Main polling loop
    ***************************************************************************/
    while(1)
    {        
        /***********************************************************************
        * Wait for connection established with Central device
        ***********************************************************************/
        if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
        {
            //Update baselines
            CapSense_UpdateEnabledBaselines();
            //Scan enabled sensors
            CapSense_ScanEnabledWidgets();
            //Wait for scanning to finish
            while(CapSense_IsBusy());
            
            /*******************************************************************
            *  if touched, simulate heart rate and measure a battery level 
            *  and send results to the Client
            *******************************************************************/        
            if(CapSense_CheckIsWidgetActive(CapSense_BUTTON0__BTN))
            {
                if(heartRateSimulation == ENABLED)
                {
                    SimulateHeartRate();
                }
            }
        }
        
        /*******************************************************************
        *  Process all pending BLE events in the stack
        *******************************************************************/
        CyBle_ProcessEvents();
    }
}


/* [] END OF FILE */
