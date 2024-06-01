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


// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include "PetHub_Types.h"
#include "PetHub_Modules.h"


#ifndef PETHUB
#define PETHUB
sensorCh_t sensors[SENCOUNT];
pbs_t pbs;
stepper_t vent;
system_t currentStatus;
system_t newStatus;
shedules_t newShedule;
shedules_t currentShedule;

// Prototypes  ----------------------------------------------------------------
void isr();
char patternArray[4]={0x01,0x02,0x04,0x08};
sensor_t maxLimits[SENCOUNT]={450,100,6000};
sensor_t minLimits[SENCOUNT]={-50,0,1190};


sensor_t initHL[SENCOUNT]={35,60,1200};
sensor_t initLL[SENCOUNT]={15,40,800};



#endif