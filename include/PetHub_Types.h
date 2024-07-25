/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: This File defines all the types.
=============================================================================*/

#ifndef TYPES
#define TYPES
#include "PetHub_Defines.h"


// Global Variables  ==========================================================
typedef int sensor_t;

typedef struct
{
	sensor_t samples[SAMPSIZE];
	sensor_t avg;
	sensor_t lLimit;
	sensor_t hLimit;
	char  	 insert;
	char 	 avgRdy;
} sensorCh_t;

typedef struct
{
	char pbState;
	char pbLastState;
}pbs_t;

typedef struct
{
	char currentPattern;
	char patCounter;
	char currentPosition;
	char setPosition;
	char isMoving;
}stepper_t;

/*Structure to save current status of system*/
typedef struct
{
	char mode;
	char shedule;
	char portion;
	char temp;
	char statusChange;
}system_t;


/*Structure to save current status of system*/
typedef struct
{
	char hour;
	char min;
	char second;
}time_t;

typedef struct
{
	char shedules[MAXSHEDULES];
	char sheduleSelect;
	char sheduleIndex;

}shedules_t;




#endif