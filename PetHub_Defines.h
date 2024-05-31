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

#ifndef DEFINES
#define DEFINES
// Constants  =================================================================
#define TRUE	1	
#define FALSE	0

#define SPBRG1SETTING 	  25
#define SPBRGH1SETTING    0
#define OSCCONSETTING 	  0x52
#define BYTESIZE    	  0x08
#define TXSTA1SETTING     0x26
#define RCSTA1SETTING     0x90
#define BAUDCON1SETTING   0x40
#define	ADCON0SETTING 	  0x01
#define	ADCON1SETTING 	  0x00
#define	ADCON2SETTING 	  0xA9
#define TIMERFLAG	  	  INTCONbits.TMR0IF
#define BYTESIZE    	  0x08
#define T0CONSETTING      0x90
#define HUNDREADMSEC	  0x3CB0
#define ONESEC			  5
#define SAMPSIZE		  10
#define SENCOUNT		  3
#define ADCRES 			  5.0/1024
#define TEMP			  0
#define HUMID			  1
#define CO2				  2
#define TEMPB			  0.5
#define TEMPM			  0.010
#define HUMIDM			  0.050
#define CO2M 			  0.00034782
#define DEGREE			  248
#define PBMASK			  0xF0
#define STEPPERMASK		  0x0F
#define FANLED			  LATCbits.LATC3
#define HEATERLED		  LATCbits.LATC2
#define COOLERLED		  LATCbits.LATC1
#define LIGHTINGLED		  LATCbits.LATC0
#define PBPORT			  PORTA
#define MODEPRESS		  0xE0
#define INCPRESS		  0xD0
#define DECPRESS		  0xB0
#define MANUAL			  0x70
#define NOPRESS			  0xF0

#define TOTALMODES		  6
#define MAXSHEDULES		  10

#endif
