/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	PetHub.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: A program that updates the system status through pushbuttons
				 dispense food as per shedule, sample sensor data and send 
				 critical informations to ESP module.
=============================================================================*/


/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
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
	char buffer[25];
	int index=0;
	systemInitialization();

	oledInit(); // Initialize the OLED display
	displayClear(0);  


    sprintf(buffer, "Welcome");
	oledPrintString(0,0,buffer);
	while(TRUE)
	{
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
					dispenseFood();
					break;
				default:
					break;
			}
			Delay10KTCYx(2);//10ms
		}

		// if any status has changed 
		if(currentStatus.statusChange)
		{
			// send new status to ESP
			transmitToESP(1);

			currentStatus.statusChange=FALSE;
		}	
		if(displayFlag)
		{
			displayData();
			displayFlag=FALSE;
		}	
		// dispence food at shedules
		if(dispenseCheckFlag)
		{
			//check if its scheduled to dispence at this hour
			for(index=0;index<currentStatus.shedule;index++)
			{
				if(currentShedule.shedules[index]==systemTime.hour)
				{
					dispenseFood();
				}
			}
			dispenseCheckFlag=FALSE;
		}
		

	}// eo while(TRUE)
} // eo main::
