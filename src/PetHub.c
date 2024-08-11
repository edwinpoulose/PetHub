/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	PetHub.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: A program that updates the system status through pushbuttons
				 dispense food as per shedule, sample sensor data, display 
				 information and send critical informations to ESP module.
=============================================================================*/


/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF //DEBUG
//#pragma config PWRTEN	= ON  //PROGRAM
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR



// Libraries ==================================================================

#include "PetHub.h"
#include <delays.h>



/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	int result=0;
	systemInitialization();
	currentShedule.sheduleIndex=0;
	oledInit(); // Initialize the OLED display
	oleddisplayOff();
	oledPrintLogo();// Display Logo
	oleddisplayOn();
	Delay10KTCYx(500);

	if(eepromRead(RESETADR))// if its the first time use initialized data
	{
   		eepromWrite(RESETADR, 0x00);
		eepromWrite(SHEDULESADR, newStatus.shedule);
		eepromWrite(PORTIONADR, newStatus.portion);
		eepromWrite(TEMPADR, newStatus.temp);
		eepromWrite(HOURADR, newSystemTime.hour);
		eepromWrite(MINADR, newSystemTime.min);
		eepromWrite(SECADR, systemTime.second);
		for (currentShedule.sheduleIndex=0;currentShedule.sheduleIndex<MAXSHEDULES
						;currentShedule.sheduleIndex++)
		{
			eepromWrite(SHEDULESTARTADR+currentShedule.sheduleIndex,
				currentShedule.shedules[currentShedule.sheduleIndex]);
		}
	}
	else
	{
		//retrieve saved data

		newStatus.shedule=currentStatus.shedule=eepromRead(SHEDULESADR);
		newStatus.portion=currentStatus.portion=eepromRead(PORTIONADR);
		newStatus.temp=currentStatus.temp=eepromRead(TEMPADR);
		newSystemTime.hour=systemTime.hour=eepromRead(HOURADR);
		newSystemTime.min=systemTime.min=eepromRead(MINADR);
		for (currentShedule.sheduleIndex=0;currentShedule.sheduleIndex<MAXSHEDULES
						;currentShedule.sheduleIndex++)
		{
			currentShedule.shedules[currentShedule.sheduleIndex]=
					newShedule.shedules[currentShedule.sheduleIndex]=
						eepromRead(SHEDULESTARTADR+currentShedule.sheduleIndex);
		}
		currentShedule.sheduleIndex=0;

	}
	oleddisplayOff();

	// Print basic data on display
	displayClear(0);  
    sprintf(buffer, "PetHub");
	oledPrintString(7,0,buffer);
    displayTime();
    displayTemp();
	displayMenu();
	oledPrintSpecialChar(2,6,1);
	drawProgressBar(6,20,0);
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
		// pull the sensor data
		// save time on eeprom
		if(secondFlag)
		{
			eepromWrite(SECADR, systemTime.second);
			secondFlag=FALSE;
			displayTime();
			if( dispenseCheckFlag==0 && manualOverRideFlag==0 )
			{
				// sensor checks disabled when motor is running(food dispensing)
				// data transmission over uart takes significant time, it interrupts motor operation
				displayTemp();
				displaylevel();
				result=calculateAirQuality();
				transmitToESP(3,result);
				//sprintf(buffer, "AirQuality =%4i",(int)result);
				//oledPrintString(2,7,buffer);
			}
		}// eo if(secondFlag)
		// on every minute 
		// save time on eeprom	
		if(minuteFlag)
		{
			minuteFlag=FALSE;
			eepromWrite(MINADR, systemTime.min);
		}
		// on every hour 
		// save time on eeprom	
		if(hourFlag)
		{
			hourFlag=FALSE;
			eepromWrite(MINADR, systemTime.min);
		}
		// dispence food at shedules or manualoveride pressed
		if(dispenseCheckFlag || manualOverRideFlag == TRUE)
		{
			// check if this hour is in shedule
			// currentShedule.sheduleIndex is incremented at every iteration of while(TRUE) infinite loop
			// which loop through all set shedules
			if(currentShedule.shedules[currentShedule.sheduleIndex]==systemTime.hour || manualOverRideFlag == TRUE)
			{
				dispenseFood();
			}
			else
			{
				currentShedule.sheduleIndex++;
				if(currentShedule.sheduleIndex>=currentStatus.shedule)
				{
					// No matches found in the current shedules against this hour
					// index reset to zero, dispence disabled
					currentShedule.sheduleIndex=0;

					dispenseCheckFlag=FALSE;
				}
			}
		}// eo if(dispenseCheckFlag || manualOverRideFlag == TRUE)
	}// eo while(TRUE)
} // eo main::
