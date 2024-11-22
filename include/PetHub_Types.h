/*=============================================================================
    File Name:  PetHub_Types.h  
    Author:     Edwin Poulose
    Date:       28/05/2024
    Modified:   None
    � Fanshawe College, 2024

    Description: This File defines all the types.
=============================================================================*/

#ifndef TYPES
#define TYPES
#include "PetHub_Defines.h"


// Global Variables  ==========================================================
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
    unsigned char mode;
    unsigned char schedule;
    unsigned char portion;
    unsigned char temp;
    unsigned char statusChange;
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
    char schedules[MAXSHEDULES];
    char scheduleSelect;
    char scheduleIndex;

}schedules_t;




#endif