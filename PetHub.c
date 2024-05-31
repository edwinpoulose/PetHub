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
#include "PetHub_Modules.h"

/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char timeCount=FALSE;
	char chID=FALSE;
	long sum=FALSE;
	int index=FALSE;
	float volts=FALSE;
	systemInitialization();
	while(TRUE)
	{
		pbs.pbState=PBPORT&PBMASK;
		if(pbs.pbState!=pbs.pbLastState && pbs.pbState!=NOPRESS)
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
					overrideSel();
					break;
				default:
					break;
			}
		}
		else if(pbs.pbState==NOPRESS)
		{
			pbs.pbLastState=NOPRESS;
		}
				
		// When timer overflows reset timer, increment counter
		if(TIMERFLAG)
		{
			resetTimer(HUNDREADMSEC);
			timeCount++;
			// when it reaches one second, sample the data.
			if(timeCount>=ONESEC)
			{
				timeCount=FALSE;
				for(chID=FALSE;chID<SENCOUNT;chID++)
				{
					sensors[chID].samples[sensors[chID].insert]=startADCConversion(chID);
					sensors[chID].insert++;
					// reset insert point when it reaches sample size
					if(sensors[chID].insert>=SAMPSIZE)
					{
						sensors[chID].insert=FALSE;
						sensors[chID].avgRdy=TRUE;
					}
					// calculate average when average ready flag sets
					if(sensors[chID].avgRdy)
					{
						volts=FALSE;
						for(sum=FALSE,index=FALSE;index<SAMPSIZE;index++)
						{
							sum+=sensors[chID].samples[index];
						}
						sensors[chID].avg=sum/SAMPSIZE;
						volts=sensors[chID].avg*ADCRES;
						switch(chID)
						{
							case TEMP:
								volts-=TEMPB;
								sensors[chID].avg=volts/TEMPM;
								break;
							case HUMID:
								sensors[chID].avg=volts/HUMIDM;
								break;
							case CO2:
								sensors[chID].avg=volts/CO2M;
								break;
							default:
								break;
						}
					}//eo if(sensors[chID].avgRdy)
				}// eo for(chID=FALSE;chID<SENCOUNT;chID++)
				// Call display function to display sensor data
				displayData();
			}// eo if(timeCount>=ONESEC)
			else
			{
				/* Do nothing*/
			}
		}// eo if(TIMERFLAG)
	}// eo while(TRUE)
} // eo main::
