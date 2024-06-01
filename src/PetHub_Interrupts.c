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


#include "PetHub_Interrupts.h"
#include "PetHub_Modules.h"


// Program from memory address 0x0008
#pragma code interruptvector=0x0008
void interruptvector(void)
{
	_asm
		GOTO isr
	_endasm
}
#pragma code



/*>>> isr: -------------------------------------------------------------------
Author:		Edwin Poulose
Date:		14/11/2023
Modified:	None
Desc:		isr to handle timer0 overflow and data received in RCREG1
Input: 		None
Returns:	None
 ----------------------------------------------------------------------------*/
#pragma interrupt isr
void isr()
{
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
	}
	if(RC1FLAG)
	{
		
	}
	INTCON |=INTGON;
} //eo isr
