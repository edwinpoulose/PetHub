/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: This File holds all the sub module prototypes.
=============================================================================*/

#include "PetHub_Types.h"
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MODULES
#define MODULES
// Extern Global Varibles======================================================

extern sensorCh_t sensors[SENCOUNT];
extern pbs_t pbs;
extern stepper_t vent;
extern char patternArray[4];
extern sensor_t maxLimits[SENCOUNT];
extern sensor_t minLimits[SENCOUNT];
extern sensor_t initHL[SENCOUNT];
extern sensor_t initLL[SENCOUNT];
extern system_t currentStatus;
extern system_t newStatus;
extern shedules_t newShedule;
extern shedules_t currentShedule;
extern time_t systemTime;
extern char displayFlag;
extern char dispenseCheckFlag;

// Functions Prototypes=================================================================


void setFrequency(void);
void setPort(void);
void setSerialPort(void);
void resetTimer(int preset);
void setTimer(int preset);
void configureADC(void);
void initSensorCh(sensorCh_t *sen);
void systemInitialization(void);
int startADCConversion(char channelID);
void displayData();
void initPbs(pbs_t *button);
void initStepper(stepper_t *motor);
void initStatus(system_t *status);
void initShedule(shedules_t *shedule);
void changeMode();
void overrideSel();
void inc();
void dec();
void initTime(time_t *time);
void transmitToESP(char control);
void dispenseFood();
#endif