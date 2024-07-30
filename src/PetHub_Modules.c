/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: This program contains all the submodules used by the main function
=============================================================================*/

// Includes ==================================================================
#include "PetHub_Modules.h"

// Functions  =================================================================


/*>>> resetTimer: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Reset the timer 0 interrupt and set the preset value for desired 
			rollover time.
Input: 		preset, preset value of timer0 to set desired rollover time.
Returns:	None
 ============================================================================*/
void resetTimer(int preset)
{
	TIMERFLAG=FALSE;
	TMR0H=preset>>BYTESIZE;
	TMR0L=preset;
} // eo resetTimer::


/*>>> systemInitialization: ==================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Set the clock frequency and initializes USART, PORT and ADC 
			modules and sensor arrays.
Input: 		None
Returns:	None
 ============================================================================*/
void systemInitialization(void)
{
	int index=0;
	
	// set Frequency
	OSCCON = OSCCONSETTING;
	while(!(OSCCONbits.HFIOFS));
	
	//set Port
	
	// Configure PORTA pins for ADC and PBs
	ANSELA=0x00;
	LATA=0x00;
	TRISA=0xFF;

	// Configure PORTC(7,6) pins for USART 
	ANSELC=0x00;
	LATC=0x00;
	TRISC=0xC0;

	// Configure PORTB(5-2) pins for STEPPER CONTROL
	ANSELB=0x00;
	LATB=0x00;
	TRISB=0xC3;

	// Cooling FAN
	ANSELDbits.ANSD2 = 0;
    COOLINGFAN = 0; // Set TRIG pin low
    TRISDbits.TRISD2 = 0; // Set TRIG pin as output
		
		

	// set Timer
	resetTimer(ONESEC);
	T0CON=T0CONSETTING;
	
	
	//set SerialPort
	
	// BRG16:0
	BAUDCON1=BAUDCON1SETTING;
	// TXEN:1,BRGH:1,SYNC:0
	TXSTA1=TXSTA1SETTING;
	// SPEN:1,CREN:1
	RCSTA1=RCSTA1SETTING;
	// SPBRGH1:SPBRG1=25
	SPBRG1=SPBRG1SETTING;
	SPBRGH1=SPBRGH1SETTING;
	
	
	//configureInterrupt
	//TMR0
	INTCON2bits.TMR0IP=FALSE;
	TIMERFLAG=FALSE;
	INTCONbits.TMR0IE=TRUE;
	
	// USART1 RECEIVER
	IPR1bits.RC1IP=FALSE;
	RC1FLAG=FALSE;
	PIE1bits.RC1IE=FALSE;
	
	// INTERRUPT PRIORITY LEVEL
	RCONbits.IPEN=FALSE;
	
	// GLOBAL INTERRUPT ENABLE
	INTCON |=INTGON;
	

	initStepper(&vent);
	initPbs(&pbs);
	initStatus(&newStatus);
	initStatus(&currentStatus);
	initShedule(&newShedule);
	initShedule(&currentShedule);
	initTime(&systemTime);
	initTime(&newSystemTime);
	SPIInit(); 
	initSensor();

} // eo systemInitialization::


/*>>> displayData: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Display the sensor readings
Input: 		None
Returns:	None	
 ============================================================================*/
void displayData()
{
	int i;
	printf("\033[2J\033[HPetHub Status\n\r");
	printf("\n\rCurrent Time: %02i:%02i:%02i",systemTime.hour,systemTime.min,systemTime.second);

	printf("\n\r\n\rMode:");

		switch(newStatus.mode)
		{
			case 1:
				printf("Running");
				break;
			case 2:
				printf("Schedule");
				break;
			case 3:
				printf("Portion");
				break;
			case 4:
				printf("Hour");
				break;
			case 5:
				printf("Minute");
				break;
			case 6:
				printf("Teperature");
				break;
			default:
				break;
		}
	printf("\n\rShedule: %3i\n\r",newStatus.shedule);
	for (i=0;i<newStatus.shedule;i++)
	{
		if(newShedule.sheduleSelect && newShedule.sheduleIndex==i)
		{
			printf("Setting ");
		}
		printf("Shedule_%i: %3i\n\r",i+1,newShedule.shedules[i]);
	}
	printf("Portion: %3i",newStatus.portion);
	printf("\n\rHour: %3i",systemTime.hour);
	printf("\n\rMin: %3i",systemTime.min);
	printf("\n\rTemp: %3i",newStatus.temp);
} // eo displayData::

/*>>> initPbs: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize pushbutton data stucture
Input: 		button, pointer to button data structure
Returns:	None	
 ============================================================================*/
void initPbs(pbs_t *button)
{
	button->pbState=FALSE;
	button->pbLastState=PBMASK;
} // eo initPbs::

/*>>> initStepper: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize stepper motor data stucture
Input: 		motor, pointer to stepper motor data structure
Returns:	None	
 ============================================================================*/
void initStepper(stepper_t *motor)
{
	motor->currentPattern=TRUE;
	motor->patCounter=FALSE;
	motor->currentPosition=FALSE;
	motor->setPosition=FALSE;
	motor->isMoving=FALSE;
} // eo initStepper::


/*>>> initStatus: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize status data strucure
Input: 		status, pointer to status data structure
Returns:	None	
 ============================================================================*/
void initStatus(system_t *status)
{
	status->mode=TRUE;
	status->shedule=2;
	status->portion=3;
	status->temp=25;
	status->statusChange=FALSE;

} // eo initStatus::

/*>>> initTime: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize time data stucture
Input: 		time, pointer to time data structure
Returns:	None	
 ============================================================================*/
void initTime(time_t *time)
{
	time->hour=FALSE;
	time->min=FALSE;
	time->second=FALSE;

} // eo initStepper::

/*>>> initShedule: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize shedule data stucture
Input: 		motor, pointer to shedule data structure
Returns:	None	
 ============================================================================*/
void initShedule(shedules_t *shedule)
{
	int i;
	shedule->sheduleIndex=-1;
	shedule->sheduleSelect=FALSE;
	for(i=0;i<MAXSHEDULES;i++)
	{
		shedule->shedules[i]=i+1;
	}


} // eo initShedule::

/*>>> transmitToESP: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Transmit data to ESP module over USART
Input: 		control, control variabl that specify kind of message is transmitted.
Returns:	None	
 ============================================================================*/
void transmitToESP(char control,int value)
{
	int i;
	if(control==1)// current settings
	{
		printf("$1,%02i,%02i#\r\n",currentStatus.portion,currentStatus.temp);
		printf("$2");
		for (i=0;i<newStatus.shedule;i++)
		{
			printf(",%02i",currentShedule.shedules[i]);
		}
		printf("#\r\n");
	}

	if(control==2)// Temp
	{
		printf("$3,01,%02i#\r\n",value);
	}
	if(control==3)// odour
	{
		printf("$3,02,%04i#\r\n",value);
	}
	if(control==4)// water
	{
		printf("$3,03,%02i#\r\n",value);
	}
	if(control==5)// food
	{
		printf("$3,04,%02i#\r\n",value);
	}
} // eo initShedule::

/*>>> dispenseFood: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Dispense food according to set portion size
Input: 		None
Returns:	None	
 ============================================================================*/
void dispenseFood()
{
	switch(motorStatus)
	{
		case 0:// motor forward
			// Open food dispencer door
			if(rotationCounter<512)
			{
				vent.patCounter++;
				if (vent.patCounter >= 4)
				{
					vent.patCounter=0;
				}
				VENTPORT =  (patternArray[vent.patCounter]<<2);
				Delay1KTCYx(3);
				rotationCounter++;
			}
			else
			{
				motorStatus=1;// motor waits
				rotationCounter=0;
				portionTimer=0;
				// start timer
			}
			break;
		case 1:
			//wait based on portion size
			if(portionTimer>currentStatus.portion+1)
			{
				motorStatus=2;// motor backward
			}
			else
			{
				// Do nothing
			}
			break;
		case 2:
			// close food dispencer door
			if(rotationCounter<512)
			{
				vent.patCounter--;
				if (vent.patCounter < 0)
				{
					vent.patCounter=3;
				}
				VENTPORT =  (patternArray[vent.patCounter]<<2);
				Delay1KTCYx(3);
				rotationCounter++;
			}
			else
			{
				motorStatus=0;// motor forward
				rotationCounter=0;
				if(manualOverRideFlag)
				{	
					// Dispencing sequence was initiated by ManualOverRide
					// Disable it
					manualOverRideFlag=FALSE;
				}
				else
				{	
					// Dispensing sequence was initiated by scheduling
					// Once dispencing completed move to next shedule
					currentShedule.sheduleIndex++;
					if(currentShedule.sheduleIndex>=currentStatus.shedule)
					{
						currentShedule.sheduleIndex=0;
						dispenseCheckFlag=FALSE;
					}					
				}

			}
			break;
		default:
			break;
	}



} // eo dispenseFood::


/*>>> changeMode: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Change current mode
Input: 		button, pointer to button data structure
Returns:	None	
 ============================================================================*/
void changeMode()
{
	/*
	 if in schedule mode,when change mode pressed, it runs through each shedule
	 setting incrementing schedule index until it reaches max shedule limit 
	 previously set.
	 */
	if(newStatus.mode==2)
	{
		newShedule.sheduleIndex++;
		if(newShedule.sheduleIndex<newStatus.shedule)
		{
			newShedule.sheduleSelect=TRUE;
			displayShedule();
		}
		else
		{
			newShedule.sheduleSelect=FALSE;
			newStatus.mode++;
			displayMode();
		}
	}
	else
	{
		if(newStatus.mode==TRUE)
		{
		    // on first entry to menu, clear all lines
			lineClear(3);
			lineClear(4);
			lineClear(5);	
		}
		newStatus.mode++;
		if(newStatus.mode==4)
		{
			// time changing
			newSystemTime.hour=systemTime.hour;
		}
		else if(newStatus.mode==5)
		{
			// time changing
			newSystemTime.min=systemTime.min;
		}	
		else if(newStatus.mode>TOTALMODES)
		{
			newStatus.mode=TRUE;
		}
		if(newStatus.mode==TRUE)
		{
			currentStatus.mode=newStatus.mode;
			currentStatus.shedule=newStatus.shedule;
			currentStatus.portion=newStatus.portion;
			currentStatus.temp=newStatus.temp;
			currentStatus.statusChange=TRUE;
			for (newShedule.sheduleIndex=0;newShedule.sheduleIndex<newStatus.shedule
							;newShedule.sheduleIndex++)
				{
					currentShedule.shedules[newShedule.sheduleIndex]=
								newShedule.shedules[newShedule.sheduleIndex];
				}
			newShedule.sheduleIndex=-1;
			systemTime.hour=newSystemTime.hour;
			systemTime.min=newSystemTime.min;
			displayMenu();
		}
		else
		{
			displayMode();
		}
	}

} // eo changeMode::


/*>>> inc: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Increment data based on mode selected
Input: 		None
Returns:	None	
 ============================================================================*/
void inc()
{

	if(newShedule.sheduleSelect)
	{
		newShedule.shedules[newShedule.sheduleIndex]++;
		if(newShedule.shedules[newShedule.sheduleIndex]>23)
		{
			newShedule.shedules[newShedule.sheduleIndex]=0;

		}
    	displayTime12Hr(12,4,newShedule.shedules[newShedule.sheduleIndex]);


	}
	else
	{
		switch(newStatus.mode)
		{
			case 2:
				newStatus.shedule++;
				if(newStatus.shedule>MAXSHEDULES)
				{
					newStatus.shedule=1;
				}
				sprintf(buffer, "%2i",newStatus.shedule);
				oledPrintString(12,4,buffer);
				break;

			case 3:
				newStatus.portion++;
				if(newStatus.portion>5)
				{
					newStatus.portion=1;
				}
				sprintf(buffer, "%2i",newStatus.portion);
				oledPrintString(12,4,buffer);
				break;

			case 4:
				newSystemTime.hour++;
				if(newSystemTime.hour>23)
				{
					newSystemTime.hour=0;
				}
				displayTime12Hr(12,4,newSystemTime.hour);
				break;
			case 5:
				newSystemTime.min++;
				if(newSystemTime.min>59)
				{
					newSystemTime.min=0;
				}
				sprintf(buffer, "%2i",newSystemTime.min);
            	oledPrintString(12,4,buffer);
				break;

			case 6:
				newStatus.temp++;
				if(newStatus.temp>30)
				{
					newStatus.temp=18;
				}
				sprintf(buffer, "%2i",newStatus.temp);
            	oledPrintString(12,4,buffer);
				break;
			default:
				break;
		}
	}

} // eo inc::

/*>>> dec: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Decrement data based on mode selected
Input: 		None
Returns:	None	
 ============================================================================*/
void dec()
{
	if(newShedule.sheduleSelect)
	{
		newShedule.shedules[newShedule.sheduleIndex]--;
		if(newShedule.shedules[newShedule.sheduleIndex]<0)
		{
			newShedule.shedules[newShedule.sheduleIndex]=23;
		}
    	displayTime12Hr(12,4,newShedule.shedules[newShedule.sheduleIndex]);
	}
	else
	{
		switch(newStatus.mode)
		{
			case 2:
				newStatus.shedule--;
				if(newStatus.shedule<1)
				{
					newStatus.shedule=MAXSHEDULES;
				}
				sprintf(buffer, "%2i",newStatus.shedule);
				oledPrintString(12,4,buffer);
				break;

			case 3:
				newStatus.portion--;
				if(newStatus.portion<1)
				{
					newStatus.portion=5;
				}
				sprintf(buffer, "%2i",newStatus.portion);
				oledPrintString(12,4,buffer);
				break;

			case 4:
				newSystemTime.hour--;
				if(newSystemTime.hour<0)
				{
					newSystemTime.hour=23;
				}
				displayTime12Hr(12,4,newSystemTime.hour);
				break;
			case 5:
				newSystemTime.min--;
				if(newSystemTime.min<0)
				{
					newSystemTime.min=59;
				}
				sprintf(buffer, "%2i",newSystemTime.min);
				oledPrintString(12,4,buffer);
				break;

			case 6:
				newStatus.temp--;
				if(newStatus.temp<18)
				{
					newStatus.temp=30;
				}
				sprintf(buffer, "%2i",newStatus.temp);
            	oledPrintString(12,4,buffer);
				break;
			default:
				break;
		}
	}
} // eo dec::
