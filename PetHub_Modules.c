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

// Includes ==================================================================
#include "PetHub_Modules.h"

// Functions  =================================================================

/*>>> setFrequency: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Set the oscillator frequency to 4 Mhz and wait until oscillator
			frequency becomes stable
Input: 		None
Returns:	None
 ============================================================================*/
void setFrequency(void)
{
	OSCCON = OSCCONSETTING;
	while(!(OSCCONbits.HFIOFS));	
} // eo setFrequency::

/*>>> setPort: ============================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Set PORTA, PORTB and PORTC
Input: 		None
Returns:	None
 ============================================================================*/
void setPort(void)
{
	// Configure PORTA pins for ADC and PBs
	ANSELA=0x00;
	LATA=0x00;
	TRISA=0xFF;

	// Configure PORTC pins for USART and LEDS
	ANSELC=0x00;
	LATC=0x00;
	TRISC=0xF0;

	// Configure PORTB pins for EXHAUST LEDS
	ANSELB=0x00;
	LATB=0x00;
	TRISB=0xF0;

} // eo setPort::

/*>>> setSerialPort: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Configure USART, serial port on, Tx and Rx enabled, Async Mode, 
			8Bit mode, 9600 Baud rate
Returns:	None
 ============================================================================*/
void setSerialPort(void)
{
	// BRG16:0
	BAUDCON1=BAUDCON1SETTING;
	// TXEN:1,BRGH:1,SYNC:0
	TXSTA1=TXSTA1SETTING;
	// SPEN:1,CREN:1
	RCSTA1=RCSTA1SETTING;
	// SPBRGH1:SPBRG1=25
	SPBRG1=SPBRG1SETTING;
	SPBRGH1=SPBRGH1SETTING;
} // eo setSerialPort::

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

/*>>> setTimer: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Reset the timer0, set the timer0 to on, select internal clock 
			source and set pre scaler to 2.
Input: 		preset, preset value of timer0 to set desired rollover time. 
Returns:	None
 ============================================================================*/
void setTimer(int preset)
{
	resetTimer(preset);
	T0CON=T0CONSETTING;
} // eo setTimer::

/*>>> configureADC: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Configure the ADC for 12 TAD, Fosc/8, right justified, standard 
			voltage references and ADC module ON
Input: 		None
Returns:	None
 ============================================================================*/
void configureADC(void)
{
	ADCON0=ADCON0SETTING;
	ADCON1=ADCON1SETTING;
	ADCON2=ADCON2SETTING;
} // eo configureADC::

/*>>> initSensorCh: ==================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Intialize global sensor values
Input: 		sen, pointer to sensor data structure
Returns:	None
 ==============initSensorCh==============================================================*/
void initSensorCh(sensorCh_t *sen)
{
	int index=FALSE;
	for(index=FALSE;index<SAMPSIZE;index++)
	{
		sen->samples[index]=FALSE;
	}
	sen->avg=FALSE;
	sen->insert=FALSE;
	sen->avgRdy=FALSE;

}// eo initSensorCh::

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
	setFrequency();
	setPort();
	setTimer(HUNDREADMSEC);
	setSerialPort();
	configureADC();
	for(index=0;index<SENCOUNT;index++)
	{
		initSensorCh(&sensors[index]);
		sensors[index].lLimit=initLL[index];
		sensors[index].hLimit=initHL[index];
	}

	initStepper(&vent);
	initPbs(&pbs);
	initStatus(&currentStatus);
	
	
} // eo systemInitialization::


/*>>> startADCConversion: ===========================================================
Author:		Edwin Poulose
Date:		14/05/2024
Modified:	None
Desc:		Select an ADC channel to be sampled, start the sampling process 
			wait untill it finished and return the result 
Input: 		channelID, channel number of ADC channel to be sampled	
Returns: 	ADRES, ADC result value.	
 ============================================================================*/
int startADCConversion(char channelID)
{
	// Set the channel
	ADCON0bits.CHS=channelID;
	// Start the sampling
	ADCON0bits.GO=TRUE;
	// Wait till sampling is done
	while(ADCON0bits.GO);
	// Return the result
	return ADRES;

} // eo startADCConversion::

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
	printf("\033[2J\033[HPetHub Status\n\r");

	printf("\n\rMode: %3i",currentStatus.mode);
	printf("\n\rShedule: %3i",currentStatus.shedule);
	printf("\n\rPortion: %3i",currentStatus.portion);
	printf("\n\rHour: %3i",currentStatus.hour);
	printf("\n\rMin: %3i",currentStatus.min);
	printf("\n\rTemp: %3i",currentStatus.temp);
} // eo displayData::

/*>>> initPbs: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
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
Desc:		
Input: 		motor, pointer to stepper motor data structure
Returns:	None	
 ============================================================================*/
void initStepper(stepper_t *motor)
{
	motor->currentPattern=TRUE;
	motor->patternCounter=FALSE;
	motor->currentPosition=FALSE;
	motor->setPosition=FALSE;
	motor->isMoving=FALSE;
} // eo initStepper::


/*>>> initStepper: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
Input: 		motor, pointer to stepper motor data structure
Returns:	None	
 ============================================================================*/
void initStatus(system_t *status)
{
	status->mode=TRUE;
	status->shedule=2;
	status->portion=3;
	status->hour=FALSE;
	status->min=FALSE;
	status->temp=20;

} // eo initStepper::

/*>>> changeMode: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
Input: 		button, pointer to button data structure
Returns:	None	
 ============================================================================*/
void changeMode()
{
	currentStatus.mode++;
	if(currentStatus.mode>TOTALMODES)
	{
		currentStatus.mode=TRUE;
	}
} // eo changeMode::

/*>>> overrideSel: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
Input: 		button, pointer to button data structure
Returns:	None	
 ============================================================================*/
void overrideSel()
{


} // eo overrideSel::

/*>>> inc: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
Input: 		channel, current channel selection
			mode, upper or lower limit mode
Returns:	None	
 ============================================================================*/
void inc()
{
	switch(currentStatus.mode)
	{
		case 2:
			currentStatus.shedule++;
			if(currentStatus.shedule>5)
			{
				currentStatus.shedule=1;
			}
			break;

		case 3:
			currentStatus.portion++;
			if(currentStatus.portion>5)
			{
				currentStatus.portion=1;
			}
			break;

		case 4:
			currentStatus.hour++;
			if(currentStatus.hour>23)
			{
				currentStatus.hour=0;
			}
			break;
		case 5:
			currentStatus.min++;
			if(currentStatus.min>59)
			{
				currentStatus.min=0;
			}
			break;

		case 6:
			currentStatus.temp++;
			if(currentStatus.temp>25)
			{
				currentStatus.temp=18;
			}
			break;
		default:
			break;
	}
} // eo inc::

/*>>> decLimit: ===========================================================
Author:		Edwin Poulose
Date:		27/05/2024
Modified:	None
Desc:		
Input: 		channel, current channel selection
			mode, upper or lower limit mode
Returns:	None	
 ============================================================================*/
void dec()
{
	switch(currentStatus.mode)
	{
		case 2:
			currentStatus.shedule--;
			if(currentStatus.shedule<1)
			{
				currentStatus.shedule=5;
			}
			break;

		case 3:
			currentStatus.portion--;
			if(currentStatus.portion<1)
			{
				currentStatus.portion=5;
			}
			break;

		case 4:
			currentStatus.hour--;
			if(currentStatus.hour<0)
			{
				currentStatus.hour=23;
			}
			break;
		case 5:
			currentStatus.min--;
			if(currentStatus.min<0)
			{
				currentStatus.min=59;
			}
			break;

		case 6:
			currentStatus.temp--;
			if(currentStatus.temp<18)
			{
				currentStatus.temp=25;
			}
			break;
		default:
			break;
	}
} // eo dec::
