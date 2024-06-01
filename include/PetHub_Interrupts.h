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

#include "PetHub_Types.h"
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef INTERRUPTS
#define INTERRUPTS


// Prototypes  ----------------------------------------------------------------
void isr();

char timeCount=FALSE;
char chID=FALSE;
long sum=FALSE;
int index=FALSE;
float volts=FALSE;

#endif
