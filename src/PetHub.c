/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: A program that samples multiple sensors at regular intervals.
				 Average of these samples are printed to screen.
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




/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	int index=0;
	systemInitialization();
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
		}
		if(pbs.pbState==NOPRESS)
		{
			pbs.pbLastState=NOPRESS;
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
