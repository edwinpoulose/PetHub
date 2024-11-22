/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
    File Name:  PetHub_Defines.c  
    Author:     Edwin Poulose
    Date:       28/05/2024
    Modified:   None
    � Fanshawe College, 2024

    Description: This File holds all the defines
=============================================================================*/

#ifndef DEFINES
#define DEFINES
// Constants  =================================================================
#define TRUE    1   
#define FALSE   0

#define SPBRG1SETTING     25
#define SPBRGH1SETTING    0
#define OSCCONSETTING     0x52
#define BYTESIZE          0x08
#define TXSTA1SETTING     0x26
#define RCSTA1SETTING     0x90
#define BAUDCON1SETTING   0x40
#define ADCON0SETTING     0x01
#define ADCON1SETTING     0x00
#define ADCON2SETTING     0xA9
#define TIMERFLAG         INTCONbits.TMR0IF
#define BYTESIZE          0x08
#define T0CONSETTING      0x93
#define ONESEC            0x0BDC
#define ADCRES            5.0/1024
#define TEMP              0
#define HUMID             1
#define CO2               2
#define TEMPB             0.5
#define TEMPM             0.010
#define HUMIDM            0.050
#define CO2M              0.00034782
#define DEGREE            248
#define PBMASK            0xF0
#define STEPPERMASK       0xC3
#define VENTPORT          PORTB
#define PBPORT            PORTA
#define MODEPRESS         0xE0
#define INCPRESS          0xD0
#define DECPRESS          0xB0
#define MANUAL            0x70
#define NOPRESS           0xF0
#define RC1FLAG           PIR1bits.RC1IF
#define INTGON            0xC0
#define MINUTE            60
#define HOUR              60
#define DAY               24

#define TOTALMODES        6
#define MAXSHEDULES       10

#define TEMP 0
#define AIR  1
// FOOD US TRIG pin
#define TRIG1 LATDbits.LATD0

// WATER US TRIG pin
#define TRIG2 LATDbits.LATD1

#define COOLINGFAN LATDbits.LATD2

// EEPROM ADDRESSES
#define RESETADR 0x00
#define SHEDULESADR 0x01
#define PORTIONADR  0x02
#define TEMPADR 0x03
#define HOURADR 0x04
#define MINADR 0x05
#define SECADR 0x06
#define DISADR 0x07
#define SHEDULESTARTADR 0x10

#endif
