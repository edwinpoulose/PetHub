/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
    File Name:  PetHub.c  
    Author:     Edwin Poulose
    Date:       09/08/2024
    Modified:   None
    � Fanshawe College, 2024

    Description: A program that updates the system status through push-buttons
                 dispense food as per schedule, sample sensor data, display 
                 information and send critical information to ESP module.
=============================================================================*/


/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC     = INTIO67
#pragma config PLLCFG   = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRTEN   = OFF 
#pragma config BOREN    = ON
#pragma config BORV     = 285 
#pragma config WDTEN    = OFF
#pragma config PBADEN   = OFF
#pragma config LVP      = OFF
#pragma config MCLRE    = EXTMCLR



// Libraries ==================================================================

#include "PetHub.h"
#include <delays.h>



/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
    int result=0;
    // initialize all system settings
    systemInitialization();
    currentSchedule.scheduleIndex=0;
    oledInit(); // Initialize the OLED display
    oleddisplayOff();
    oledPrintLogo();// Display Logo
    oleddisplayOn();
    Delay10KTCYx(500);

    if(eepromRead(RESETADR))// if its the first time use initialized data
    {
        eepromWrite(RESETADR, 0x00);
        eepromWrite(SHEDULESADR, newStatus.schedule);
        eepromWrite(PORTIONADR, newStatus.portion);
        eepromWrite(TEMPADR, newStatus.temp);
        eepromWrite(HOURADR, newSystemTime.hour);
        eepromWrite(MINADR, newSystemTime.min);
        eepromWrite(SECADR, systemTime.second);
        eepromWrite(DISADR, dispenseCheckFlag);
        for (currentSchedule.scheduleIndex=0;currentSchedule.scheduleIndex<MAXSHEDULES
                        ;currentSchedule.scheduleIndex++)
        {
            eepromWrite(SHEDULESTARTADR+currentSchedule.scheduleIndex,
                currentSchedule.schedules[currentSchedule.scheduleIndex]);
        }
    }
    else
    {
        //retrieve saved data
        
        dispenseCheckFlag=eepromRead(DISADR);
        newStatus.schedule=currentStatus.schedule=eepromRead(SHEDULESADR);
        newStatus.portion=currentStatus.portion=eepromRead(PORTIONADR);
        newStatus.temp=currentStatus.temp=eepromRead(TEMPADR);
        newSystemTime.hour=systemTime.hour=eepromRead(HOURADR);
        newSystemTime.min=systemTime.min=eepromRead(MINADR);
        for (currentSchedule.scheduleIndex=0;currentSchedule.scheduleIndex<MAXSHEDULES
                        ;currentSchedule.scheduleIndex++)
        {
            currentSchedule.schedules[currentSchedule.scheduleIndex]=
                    newSchedule.schedules[currentSchedule.scheduleIndex]=
                        eepromRead(SHEDULESTARTADR+currentSchedule.scheduleIndex);
        }
        currentSchedule.scheduleIndex=0;

    }
    oleddisplayOff();

    // Print basic data on display
    displayClear(0);  
    sprintf(buffer, "PetHub");
    oledPrintString(7,0,buffer);
    displayTime();
    displayTemp();
    displayMenu();
    // display food symbol and level 0
    oledPrintSpecialChar(2,6,1);
    drawProgressBar(6,20,0);
    // display water symbol and level 0
    oledPrintSpecialChar(10,6,2);
    drawProgressBar(6,68,0);
    oleddisplayOn();

    // infinite loop
    while(TRUE)
    {   
        //check for push button press
        pbs.pbState=PBPORT&PBMASK;
        if(pbs.pbState!=pbs.pbLastState)
        {
            pbs.pbLastState=pbs.pbState;
            switch(pbs.pbState)
            {
                case MODEPRESS:
                    changeMode();
                    break;
                case INCPRESS:
                    inc();
                    break;
                case DECPRESS:
                    dec();
                    break;
                case MANUAL:
                    manualOverRideFlag=TRUE;
                    break;
                default:
                    break;
            }
            Delay10KTCYx(2);//10ms
        }// eo if(pbs.pbState!=pbs.pbLastState)

        // if any status has changed 
        if(currentStatus.statusChange)
        {
            // send new status to ESP
            transmitToESP(1,0);

            currentStatus.statusChange=FALSE;
        }   
        // on every second 
        // pull ,process and send sensor data to esp
        // save time on EEPROM
        if(secondFlag)
        {
            eepromWrite(SECADR, systemTime.second);
            secondFlag=FALSE;
            displayTime();
            if( dispenseCheckFlag==0 && manualOverRideFlag==0 )
            {
                // sensor checks disabled when motor is running(food dispensing)
                // data transmission over UART takes significant time, it interrupts stepper motor operation
                displayTemp();
                displaylevel();
                // air quality is not displayed, but transferred to ESP
                result=calculateAirQuality();
                transmitToESP(3,result);
            }
        }// eo if(secondFlag)
        // on every minute 
        // save time on EEPROM  
        if(minuteFlag)
        {
            minuteFlag=FALSE;
            eepromWrite(MINADR, systemTime.min);
        }
        // on every hour 
        // save time on EEPROM  
        if(hourFlag)
        {
            hourFlag=FALSE;
            eepromWrite(HOURADR, systemTime.hour);
            eepromWrite(DISADR, dispenseCheckFlag);
        }
        // dispense food at schedules or manual override pressed
        if(dispenseCheckFlag == TRUE || manualOverRideFlag == TRUE)
        {
            // check if this hour is in schedule
            // currentSchedule.scheduleIndex is incremented at every iteration of while(TRUE) infinite loop
            // which loop through all set schedules
            if(currentSchedule.schedules[currentSchedule.scheduleIndex]==systemTime.hour || manualOverRideFlag == TRUE)
            {
                dispenseFood();
            }
            else
            {
                currentSchedule.scheduleIndex++;
                if(currentSchedule.scheduleIndex>=currentStatus.schedule)
                {
                    // No matches found in the schedules against this hour
                    // index reset to zero, dispense disabled
                    currentSchedule.scheduleIndex=0;
                    dispenseCheckFlag=FALSE;
                    eepromWrite(DISADR, dispenseCheckFlag);
                }
            }
        }// eo if(dispenseCheckFlag || manualOverRideFlag == TRUE)
    }// eo while(TRUE)
} // eo main::
