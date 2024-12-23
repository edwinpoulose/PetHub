/*=============================================================================
    File Name:  PetHub_Display.c  
    Author:     Edwin Poulose
    Date:       28/05/2024
    Modified:   None
    � Fanshawe College, 2024

    Description: This program contains all the sub modules related to
                 displaying data in OLED
=============================================================================*/

// Includes ==================================================================
#include "PetHub_Modules.h"

// Functions  =================================================================

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display Temperature data in OLED
Input:      None
Returns:    None    
 ============================================================================*/
void displayTemp()
{
    int result=calculateTemperature();
    if(result>currentStatus.temp)
    {
        COOLINGFAN=TRUE;
    }
    else
    {
         COOLINGFAN=FALSE;     
    }
    sprintf(buffer, "%02i",result);
    transmitToESP(2,result);
    oledPrintString(15,1,buffer);
    oledPrintSpecialChar(17,1,0);
    oledPrintChar(18,1,'C');
}

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display Sytem Time data in OLED
Input:      None
Returns:    None    
 ============================================================================*/
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

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Convert hour data to 12 hr format and display
Input:      None
Returns:    None    
 ============================================================================*/
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

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display main menu
Input:      None
Returns:    None    
 ============================================================================*/
void displayMenu()
{
    displayTime();
    sprintf(buffer, "Schedules:%2i",currentStatus.schedule);
    oledPrintString(4,3,buffer);

    sprintf(buffer, "Portions :%2i",currentStatus.portion);
    oledPrintString(4,4,buffer);  
}

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display schedule select settings
Input:      None
Returns:    None    
 ============================================================================*/
void displaySchedule()
{
    sprintf(buffer, "Schedule #%2i  ",newSchedule.scheduleIndex+1);
    oledPrintString(4,3,buffer);

    sprintf(buffer, "Set to :   ");
    oledPrintString(4,4,buffer);
    displayTime12Hr(12,4,newSchedule.schedules[newSchedule.scheduleIndex]);
}

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display individual mode setting
Input:      None
Returns:    None    
 ============================================================================*/
void displayMode()
{
    sprintf(buffer, "%s  ",modeSelect[newStatus.mode-1]);
    oledPrintString(4,3,buffer);
    sprintf(buffer, "Set to :    ");
    oledPrintString(4,4,buffer);
    switch(newStatus.mode)
    {
        case 2:
            sprintf(buffer, "%2i",newStatus.schedule);
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

/*>>> dec: ===========================================================
Author:     Edwin Poulose
Date:       27/05/2024
Modified:   None
Desc:       Display water and food level in OLED
Input:      None
Returns:    None    
 ============================================================================*/
void displaylevel()
{
    unsigned long result=0;
    char level=0;
    switch (triggerFlag)// switched to next state in every second.
    {
        case 0:
            // send trigger pulse to food Ultrasonic
            startTrigger(1);
            triggerFlag++;
            break;
        case 1:
            // check distance to food level
            result=checkDistance();
            if (result<0)
            {   
                level=4;
                break;
            }
            else if (result<3)
            {
                level=4;
            }
            else if (result<6)
            {
                level=3;
            }
            else if (result<9)
            {
                level=2;
            }
            else if (result<13)
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
            // send trigger pulse to water Ultrasonic
            startTrigger(2);
            triggerFlag++;
            break;
        case 3:
            // check distance to water level
            result=checkDistance();
            if (result<0)
            {
                level=4;
                break;
            }
            else if (result<3)
            {
                level=4;
            }
            else if (result<6)
            {
                level=3;
            }
            else if (result<9)
            {
                level=2;
            }
            else if (result<13)
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

