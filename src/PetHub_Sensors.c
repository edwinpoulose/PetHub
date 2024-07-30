// Libraries ------------------------------------------------------------------
#include "PetHub_Modules.h"


void initSensor()
{
    // Configure ADC
    ADCON0=ADCON0SETTING;
	ADCON1=ADCON1SETTING;
	ADCON2=ADCON2SETTING;
    // Initialize Timer2
    T2CON = 0x00;            // Clear Timer2 configuration
    T2CONbits.T2CKPS = 0b10; // Prescaler is 16
    T2CONbits.T2OUTPS = 0;   // Postscaler is 1:1
    PR2 = 255;               // Load the period register with maximum value
    TMR2 = 0;                // Clear Timer2 register
    PIR1bits.TMR2IF = 0;     // Clear Timer2 interrupt flag
    PIE1bits.TMR2IE = 1;     // Enable Timer2 interrupt
    T2CONbits.TMR2ON = 0;    // Turn off Timer2

    // Initialize Trigger pin
    ANSELDbits.ANSD0 = 0;
    TRIG1 = 0; // Set TRIG pin low
    TRISDbits.TRISD0 = 0; // Set TRIG pin as output

    ANSELDbits.ANSD1 = 0;
    TRIG2 = 0; // Set TRIG pin low
    TRISDbits.TRISD1 = 0; // Set TRIG pin as output

    // Configure INT0 pin as input
    TRISBbits.TRISB0 = 1;
    INTCON2bits.INTEDG0 = 1; // Interrupt on rising edge

    // Enable INT0 interrupt
    INTCONbits.INT0IF = 0;  // Clear INT0 interrupt flag
    INTCONbits.INT0IE = 1;  // Enable INT0 interrupt

    // Configure INT1 pin as input
    TRISBbits.TRISB1 = 1;
    INTCON2bits.INTEDG1 = 1; // Interrupt on rising edge

    // Enable INT1 interrupt
    INTCON3bits.INT1IF = 0;  // Clear INT1 interrupt flag
    INTCON3bits.INT1IE = 1;  // Enable INT1 interrupt
}

void startTrigger(int sensor)
{
	// edge detection, edge 
    edge=0;
	distanceRdy=0;

    if(sensor==1)
    {
		// it is neccessary to switch back to detecting rising edge 
		// in case trigger is fired before prevous edge is detected
		INTCON2bits.INTEDG0 = 1;
        // Trigger the ultrasonic food sensor
        TRIG1 = 1;
        Delay10TCYx(1); // 10 us
        TRIG1 = 0;
    }
    if(sensor==2)
    {
        INTCON2bits.INTEDG1 = 1; // Switch back to rising edge detection
        // Trigger the ultrasonic water sensor
        TRIG2 = 1;
        Delay10TCYx(1); // 10 us
        TRIG2 = 0;
    }

}



int checkDistance()
{
    int distance = 0;

    // Check if edge detection is complete
    if (distanceRdy)
    {
        // Timer tick is (1 / (Fosc / 4)) * Prescaler
        // For 4 MHz internal oscillator, tick is (1 / (4 MHz / 4)) * 16 = 16 us
        // Speed of sound is 343 m/s = 0.0343 cm/us
        // Distance = (totalTicks * tick time in us) * 0.0343 / 2 (since it's round trip)
        distance = (stopTime * 16.0) * 0.0343 / 2.0;
        return distance;
    }
    else
    {
        // Edge detection not complete
        return -1;
    }
}

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

int calculateTemperature()
{
    float temp;
    int tempi;
    temp = startADCConversion(TEMP) * 5000.0 / 1024.0; // Convert to mV
    tempi = temp / 12.0;  // 10mV equals 1 degree, 12 used to account for errors
    return tempi;

} 


int calculateAirQuality()
{
    int air;
    air = startADCConversion(AIR) * 5000.0 / 1024.0; // Convert to mV
    return air;

} 
