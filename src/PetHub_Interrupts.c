/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
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
		resetTimer(ONESEC);
			systemTime.second++;
			if (MINUTE<=systemTime.second)
			{
				systemTime.second=0;
				systemTime.min++;
				if (HOUR<=systemTime.min)
				{
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
		displayFlag=TRUE;
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
            INTCON2bits.INTEDG0 = 1; // Switch back to rising edge detection
            edge = 0;
			discalc=1;
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
           INTCON2bits.INTEDG1 = 1; // Switch back to rising edge detection
           edge = 0;
           discalc = 1;
       }
   }
	INTCON |=INTGON;
} //eo isr
