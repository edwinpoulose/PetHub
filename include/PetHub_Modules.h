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

// Define TRIG pin
#define TRIG1 LATDbits.LATD0


// Define TRIG pin
#define TRIG2 LATDbits.LATD1



// Extern Global Varibles======================================================

extern pbs_t pbs;
extern stepper_t vent;
extern char patternArray[4];
extern system_t currentStatus;
extern system_t newStatus;
extern schedules_t newSchedule;
extern schedules_t currentSchedule;
extern time_t systemTime;
extern time_t newSystemTime;
extern char secondFlag;
extern char minuteFlag;
extern char hourFlag;
extern char triggerFlag;
extern char dispenseCheckFlag;
extern char buffer[25];
extern char modeSelect[6][15];
extern unsigned int overflowCount;
extern unsigned long stopTime;
extern unsigned char edge;
extern unsigned char distanceRdy;
extern unsigned int overflowCount;
extern int rotationCounter;
extern int motorStatus;
extern char portionTimer;
extern char manualOverRideFlag;
// Functions Prototypes=================================================================


void setFrequency(void);
void setPort(void);
void setSerialPort(void);
void resetTimer(int preset);
void setTimer(int preset);
void configureADC(void);
void systemInitialization(void);
int  startADCConversion(char channelID);
void displayData();
void initPbs(pbs_t *button);
void initStepper(stepper_t *motor);
void initStatus(system_t *status);
void initSchedule(schedules_t *schedule);
void changeMode();
void overrideSel();
void inc();
void dec();
void initTime(time_t *time);
void transmitToESP(char control,int value);
void dispenseFood();
void initSensor();
void startTrigger(int sensor);
int checkDistance();
int calculateTemperature();
int calculateAirQuality();


#endif
