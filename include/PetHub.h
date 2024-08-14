/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: This File holds all the global and status variables.
=============================================================================*/


// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include "PetHub_Types.h"


#ifndef PETHUB
#define PETHUB
pbs_t pbs;
stepper_t vent;
system_t currentStatus;
system_t newStatus;
schedules_t newSchedule;
schedules_t currentSchedule;
time_t systemTime;
time_t newSystemTime;

// Prototypes  ----------------------------------------------------------------
void isr();
rom char patternArray[4]={0x01,0x02,0x04,0x08};
char secondFlag=FALSE;
char minuteFlag=FALSE;
char hourFlag=FALSE;
volatile char triggerFlag=FALSE;
char dispenseCheckFlag=TRUE;
char manualOverRideFlag=FALSE;
char buffer[25];
rom char modeSelect[6][15]={"SET","Schedules","Portion","Time (Hour)","Time (Min)","Temperature"};
unsigned long stopTime = 0;
unsigned char edge = 0;
unsigned char distanceRdy=0;
unsigned int overflowCount = 0;
int rotationCounter=0;
int motorStatus=0;
char portionTimer=0;

#endif