/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		20/07/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: A program that handles timer0 interrupts
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
Date:		20/07/2024
Modified:	None
Desc:		isr to handle timer0, timer2, INT0, INT1 and RC1 interrupts
Input: 		None
Returns:	None
 ----------------------------------------------------------------------------*/
#pragma interrupt isr
void isr()
{
	if(TIMERFLAG)
	{
		resetTimer(ONESEC);
		secondFlag=TRUE;
		systemTime.second++;
		if (MINUTE<=systemTime.second)
		{
			minuteFlag=TRUE;
			systemTime.second=0;
			systemTime.min++;
			if (HOUR<=systemTime.min)
			{
				hourFlag=TRUE;
				systemTime.min=0;
				systemTime.hour++;
				// check to dispense food
				dispenseCheckFlag=TRUE;
				if (DAY<=systemTime.hour)
				{
					systemTime.hour=0;
				}
			}
		}
		// if motor is in wait mode
		if(motorStatus==1)
		{
			portionTimer++;
		}

	}
	if(RC1FLAG)
	{
		// For future updates if serial reception is used
	}
	if (PIR1bits.TMR2IF)
	{
		PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
		overflowCount++;     // Increment overflow count
	}

    if (INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0; // Clear INT0 interrupt flag

        if (edge == 0)
        {
            // Rising edge detected, start timer
            overflowCount = 0;     // Reset overflow count
            TMR2 = 0;              // Clear Timer2 register
            T2CONbits.TMR2ON = 1;  // Start Timer2
            INTCON2bits.INTEDG0 = 0; // Switch to falling edge detection
            edge = 1;
        }
        else
        {
            // Falling edge detected, stop timer
            stopTime = ((unsigned long)overflowCount * 256) + (unsigned long)TMR2;
            T2CONbits.TMR2ON = 0;  // Stop Timer2
			distanceRdy=1;
        }
    }
   if (INTCON3bits.INT1IF)
   {
       INTCON3bits.INT1IF = 0; // Clear INT1 interrupt flag

       if (edge == 0)
       {
           // Rising edge detected, start timer
           overflowCount = 0;     // Reset overflow count
           TMR2 = 0;              // Clear Timer2 register
           T2CONbits.TMR2ON = 1;  // Start Timer2
           INTCON2bits.INTEDG1 = 0; // Switch to falling edge detection
           edge = 1;
       }
       else
       {
           // Falling edge detected, stop timer
           stopTime = ((unsigned long)overflowCount * 256) + (unsigned long)TMR2;
           T2CONbits.TMR2ON = 0;  // Stop Timer2
           distanceRdy = 1;
       }
   }
	INTCON |=INTGON;
} //eo isr
