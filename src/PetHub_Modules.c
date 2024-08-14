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
	
	// set Port configurations
	// sensor and display configurations are 
	// performed in indivitual initialization functions
	
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
    COOLINGFAN = 0; 
    TRISDbits.TRISD2 = 0; 
		
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
	
	// USART1 RECEIVER : Disabled
	IPR1bits.RC1IP=FALSE;
	RC1FLAG=FALSE;
	PIE1bits.RC1IE=FALSE;
	
	// INTERRUPT PRIORITY LEVEL 0
	RCONbits.IPEN=FALSE;
	
	// GLOBAL INTERRUPT ENABLE
	INTCON |=INTGON;
	
	// initialising all system settings to initial status
	initStepper(&vent);
	initPbs(&pbs);
	initStatus(&newStatus);
	initStatus(&currentStatus);
	initSchedule(&newSchedule);
	initSchedule(&currentSchedule);
	initTime(&systemTime);
	initTime(&newSystemTime);

	// SPI initialization for OLED
	SPIInit();

	// All sensor module initialization 
	initSensor();

} // eo systemInitialization::


/*>>> displayData: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		This is a test function used to display information on terminal
			have no significance in production setup
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
	printf("\n\rSchedule: %3i\n\r",newStatus.schedule);
	for (i=0;i<newStatus.schedule;i++)
	{
		if(newSchedule.scheduleSelect && newSchedule.scheduleIndex==i)
		{
			printf("Setting ");
		}
		printf("Schedule_%i: %3i\n\r",i+1,newSchedule.schedules[i]);
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

/*>>> initStepper: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Write data to EEPROM
Input: 		address, address to write to
			data, data to write
Returns:	None	
 ============================================================================*/
void eepromWrite(unsigned char address, unsigned char data) 
{
    while (EECON1bits.RD || EECON1bits.WR); // Wait for previous read/write to complete
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0; // Access data memory
    EECON1bits.CFGS = 0;  // Access EEPROM
    EECON1bits.WREN = 1;  // Enable writes
    INTCONbits.GIE = 0;   // Disable interrupts
    EECON2 = 0x55;        // Required sequence
    EECON2 = 0xAA;        // Required sequence
    EECON1bits.WR = 1;    // Start write
	while (EECON1bits.WR);// Wait for write to complete
    INTCONbits.GIE = 1;   // Enable interrupts
    EECON1bits.WREN = 0;  // Disable writes
}

/*>>> initStepper: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Read data FROM EEPROM
Input: 		address, address to read from
Returns:	data, data from address	
 ============================================================================*/
int eepromRead(unsigned char address) 
{
    while (EECON1bits.RD || EECON1bits.WR); // Wait for previous read/write to complete
    EEADR = address;
    EECON1bits.EEPGD = 0; // Access data memory
    EECON1bits.CFGS = 0;  // Access EEPROM
    EECON1bits.RD = 1;    // Start read
    while (EECON1bits.RD || EECON1bits.WR); // Wait for previous read/write to complete
	return EEDATA;
}

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
	status->schedule=2;
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

/*>>> initSchedule: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Initialize schedule data stucture
Input: 		motor, pointer to schedule data structure
Returns:	None	
 ============================================================================*/
void initSchedule(schedules_t *schedule)
{
	int i;
	schedule->scheduleIndex=-1;
	schedule->scheduleSelect=FALSE;
	for(i=0;i<MAXSHEDULES;i++)
	{
		schedule->schedules[i]=i+1;
	}


} // eo initSchedule::

/*>>> transmitToESP: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Transmit data to ESP module over UART
Input: 		control, control variable what specify kind of message is transmitted.
Returns:	None	
 ============================================================================*/
void transmitToESP(char control,int value)
{
	int i;
	if(control==1)// current settings
	{
		printf("$1,%02i,%02i#\r\n",currentStatus.portion,currentStatus.temp);
		printf("$2");
		for (i=0;i<newStatus.schedule;i++)
		{
			printf(",%02i",currentSchedule.schedules[i]);
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
} // eo initSchedule::

/*>>> dispenseFood: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		Dispense food according to set portion size
			The motor status variable motorStatus has 3 states
Input: 		None
Returns:	None	
 ============================================================================*/
void dispenseFood()
{
	switch(motorStatus)
	{
		case 0:// motor forward
			// Open food dispenser door
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
				// after motor reaches 90 degrees, motor waits 
				motorStatus=1;// motor waits
				rotationCounter=0;
				portionTimer=0;
				// portionTimer is incremented in timer0 at every second
				// meaning it waits until it reached the specified time
			}
			break;
		case 1:
			//wait based on portion size
			if(portionTimer>currentStatus.portion+1)
			{
				motorStatus=2;// set to backward when specified time is reached
			}
			else
			{
				// Do nothing
			}
			break;
		case 2:
			// close food dispenser door
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
				motorStatus=0;// motor set to forward
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
					// Once dispencing completed move to next schedule
					// and repeat the process
					currentSchedule.scheduleIndex++;
					if(currentSchedule.scheduleIndex>=currentStatus.schedule)
					{
						currentSchedule.scheduleIndex=0;
						dispenseCheckFlag=FALSE;
						eepromWrite(DISADR, dispenseCheckFlag);
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
	 if in schedule mode,when change mode pressed, it runs through each schedule
	 setting incrementing schedule index until it reaches max schedule limit 
	 previously set.
	 */
	if(newStatus.mode==2)
	{
		newSchedule.scheduleIndex++;
		if(newSchedule.scheduleIndex<newStatus.schedule)
		{
			newSchedule.scheduleSelect=TRUE;
			displaySchedule();
		}
		else
		{
			newSchedule.scheduleSelect=FALSE;
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
		/*
			if in any other mode it cycles through the modes
		*/
		newStatus.mode++;
		// in time changing mode copy the system time to update
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
		// once cycled through all the modes, save the new settings
		if(newStatus.mode==TRUE)
		{
			currentStatus.mode=newStatus.mode;
			currentStatus.schedule=newStatus.schedule;
			currentStatus.portion=newStatus.portion;
			currentStatus.temp=newStatus.temp;
			// save the new settings to EEPROM
			eepromWrite(SHEDULESADR, newStatus.schedule);
			eepromWrite(PORTIONADR, newStatus.portion);
			eepromWrite(TEMPADR, newStatus.temp);
			eepromWrite(HOURADR, newSystemTime.hour);
			eepromWrite(MINADR, newSystemTime.min);

			// to transmt to esp
			currentStatus.statusChange=TRUE;

			for (newSchedule.scheduleIndex=0;newSchedule.scheduleIndex<newStatus.schedule
							;newSchedule.scheduleIndex++)
			{
				currentSchedule.schedules[newSchedule.scheduleIndex]=
							newSchedule.schedules[newSchedule.scheduleIndex];
				eepromWrite(SHEDULESTARTADR+newSchedule.scheduleIndex,
							newSchedule.schedules[newSchedule.scheduleIndex]);
			}
			newSchedule.scheduleIndex=-1;
			systemTime.hour=newSystemTime.hour;
			systemTime.min=newSystemTime.min;

			// A settings saved message can be displayed in future here

			// Display the menu
			// show number of schedules and portions
			displayMenu();
		}
		else
		{	
			// display the current mode to OLED
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
	// if in schedule select change the current selected schedule
	if(newSchedule.scheduleSelect)
	{
		newSchedule.schedules[newSchedule.scheduleIndex]++;
		if(newSchedule.schedules[newSchedule.scheduleIndex]>23)
		{
			newSchedule.schedules[newSchedule.scheduleIndex]=0;

		}
		// display change in OLED
    	displayTime12Hr(12,4,newSchedule.schedules[newSchedule.scheduleIndex]);


	}
	else
	{
		// if not in schedule select change the current mode value
		switch(newStatus.mode)
		{
			case 2: // schedule size
				newStatus.schedule++;
				if(newStatus.schedule>MAXSHEDULES)
				{
					newStatus.schedule=1;
				}
				sprintf(buffer, "%2i",newStatus.schedule);
				oledPrintString(12,4,buffer);
				break;

			case 3: //portion size
				newStatus.portion++;
				if(newStatus.portion>5)
				{
					newStatus.portion=1;
				}
				sprintf(buffer, "%2i",newStatus.portion);
				oledPrintString(12,4,buffer);
				break;

			case 4: // Time hour
				newSystemTime.hour++;
				if(newSystemTime.hour>23)
				{
					newSystemTime.hour=0;
				}
				displayTime12Hr(12,4,newSystemTime.hour);
				break;
			case 5: // Time minute
				newSystemTime.min++;
				if(newSystemTime.min>59)
				{
					newSystemTime.min=0;
				}
				sprintf(buffer, "%2i",newSystemTime.min);
            	oledPrintString(12,4,buffer);
				break;

			case 6: // temp setting
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
	if(newSchedule.scheduleSelect)
	{
		// if in schedule select change the current selected schedule
		newSchedule.schedules[newSchedule.scheduleIndex]--;
		if(newSchedule.schedules[newSchedule.scheduleIndex]<0)
		{
			newSchedule.schedules[newSchedule.scheduleIndex]=23;
		}
    	displayTime12Hr(12,4,newSchedule.schedules[newSchedule.scheduleIndex]);
	}
	else
	{
		// if not in schedule select change the current mode value
		switch(newStatus.mode)
		{
			case 2:
				newStatus.schedule--;
				if(newStatus.schedule<1)
				{
					newStatus.schedule=MAXSHEDULES;
				}
				sprintf(buffer, "%2i",newStatus.schedule);
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


