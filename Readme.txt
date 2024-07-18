Mode	|Increment|Decrement|Manual
--------------------------------------------------
Run 	|-		  |-		|Active
Schedule|+1		  |-1		|Inactive

Shedules[MAXSHEDULES]
Portion	|+1		  |-1		|Inactive
Time(h)	|+1	to 24 |-1		|Inactive
Time(m)	|+1	to 60 |-1		|Inactive
Temp	|+1		  |-1		|Inactive

Pinout

RA0 AN0 TEMP
RA1 AN1 AIR

pbs
RA4 MODE
RA5 INCPRESS
RA6 DECPRESS
RA7 MANUAL

RB0 ECHO INT0
RB1 ECHO INT1

Stepper Motor
RB2
RB3
RB4
RB5

RC3 SPI CLK
RC5 SPI SDO
RC6	TX 25
RC7 RX 26

RD0 TRIG1
RD1 TRIG2

RD4 RESET
RD5 DATA
RD6 CMD

