/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011EPLab2.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: This program contains all the submodules related to display
=============================================================================*/

// Includes ==================================================================
#include "PetHub_Modules.h"

// Functions  =================================================================


void displayTemp()
{
    int result=calculateTemperature();
    sprintf(buffer, "%02i",result);
    transmitToESP(2,result);
    oledPrintString(15,1,buffer);
    oledPrintSpecialChar(17,1,0);
    oledPrintChar(18,1,'C');
}

void displayTime()
{

    if (systemTime.hour == 0)
    {
        sprintf(buffer,"12:%02iAM",systemTime.min);
    }
    else if (systemTime.hour == 12)
    {
        sprintf(buffer,"12:%02iPM",systemTime.min);
    }
    else if (systemTime.hour < 12)
    {
        sprintf(buffer,"%02i:%02iAM",systemTime.hour,systemTime.min);
    }
    else
    {
        sprintf(buffer,"%02i:%02iPM",systemTime.hour-12,systemTime.min);
    }

    oledPrintString(2, 1, buffer);
}
void displayTime12Hr(unsigned char x, unsigned char y, char hour)
{
    if (hour == 0)
    {
        sprintf(buffer,"12AM");
    }
    else if (hour == 12)
    {
        sprintf(buffer,"12PM");
    }
    else if (hour < 12)
    {
        sprintf(buffer,"%2iAM", hour);
    }
    else
    {
        sprintf(buffer,"%2iPM",(hour - 12));
    }

    oledPrintString(x, y, buffer);
}


void displayMenu()
{
    displayTime();
	sprintf(buffer, "Shedules:%2i",currentStatus.shedule);
	oledPrintString(4,3,buffer);

	sprintf(buffer, "Portions:%2i",currentStatus.portion);
	oledPrintString(4,4,buffer);  
}

displayShedule()
{
    sprintf(buffer, "Shedule #%2i    ",newShedule.sheduleIndex+1);
    oledPrintString(4,3,buffer);

    sprintf(buffer, "Set to :    ");
    oledPrintString(4,4,buffer);
    displayTime12Hr(12,4,newShedule.shedules[newShedule.sheduleIndex]);
}

displayMode()
{
    sprintf(buffer, "%s     ",modeSelect[newStatus.mode-1]);
    oledPrintString(4,3,buffer);
    sprintf(buffer, "Set to :     ");
    oledPrintString(4,4,buffer);
    switch(newStatus.mode)
    {
        case 2:
            sprintf(buffer, "%2i",newStatus.shedule);
            oledPrintString(12,4,buffer);
            break;

        case 3:
            sprintf(buffer, "%2i",newStatus.portion);
            oledPrintString(12,4,buffer);
            break;

        case 4:
            displayTime12Hr(12,4,newSystemTime.hour);
            break;
        case 5:
            sprintf(buffer, "%2i",systemTime.min);
            oledPrintString(12,4,buffer);
            break;

        case 6:
            sprintf(buffer, "%2i",newStatus.temp);
            oledPrintString(12,4,buffer);
            break;
        default:
            break;
    }
}
displaylevel()
{
    int result,level;
    switch (triggerFlag)
    {
    case 0:
        startTrigger(1);
        triggerFlag++;
        break;
    case 1:

        result=checkDistance();
        if (result<10)
        {
            level=4;
        }
        else if (result<15)
        {
            level=3;
        }
        else if (result<20)
        {
            level=2;
        }
        else if (result<25)
        {
            level=1;
        }
        else
        {
            level=0;
        }
        triggerFlag++;
        drawProgressBar(6,20,(char)level);
        transmitToESP(5,level);
        break;	
    case 2:
        startTrigger(2);
        triggerFlag++;
        break;
    case 3:
        result=checkDistance();
        if (result<10)
        {
            level=4;
        }
        else if (result<15)
        {
            level=3;
        }
        else if (result<20)
        {
            level=2;
        }
        else if (result<25)
        {
            level=1;
        }
        else
        {
            level=0;
        }
        drawProgressBar(6,68,(char)level);
        triggerFlag=0;
        transmitToESP(4,level);
        break;						
    default:
        break;
    }
}

