/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: hmiset
 * File: hmiset.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program hmiset --- HMI setting parameter control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>
/*********************************************************************************
** 							    defines                     			                 **
*********************************************************************************/
#define S$STEP_SETUP 		   10
#define S$STEP_IDLE 			   20
#define S$STEP_SETCONTRAST 	30
#define S$STEP_SETBRIGHTNESS 	40

#define S$STEP_INIT 			    0
#define S$STEP_GET_TIME 	  100
#define S$STEP_SET_TIME 	  200
/********************************************************************************
****	        		I N I T U P  O F   T H E   T A S K 		 		              ****
********************************************************************************/
_INIT void hmiset_Init(void)
{
	hmiStep  = S$STEP_SETUP;
   hmiTimeStep = S$STEP_INIT;
    
   strcpy(gPanelObj.VisualName, "visu15");
}
/*********************************************************************************
***    C Y C L I C   P A R T   O F   T H E   T A S K  			              ****
*********************************************************************************/
_CYCLIC void hmiset_Cyclic(void)
{
	
	BackLightTime = hmiBackLightTime_Min *60;
	
    STRING NumName[5];
    
	/*  contrast and brigfhtness set in hmi */
	switch (hmiStep)
	{
		case S$STEP_SETUP:
			gPanelObj.vcHandle = VA_Setup(1, gPanelObj.VisualName);

			if (gPanelObj.vcHandle != 0)
			{
				if (!VA_Saccess(1, gPanelObj.vcHandle))
				{
					Contrast = VA_GetContrast(1,gPanelObj.vcHandle);
					contrast_old = Contrast;

					VA_GetBrightness(1,gPanelObj.vcHandle,(UDINT)&Brightness);
					brightness_old = Brightness;

					VA_Srelease(1, gPanelObj.vcHandle);
				}
				hmiStep = S$STEP_IDLE;
			}
		break;

		case S$STEP_IDLE:		/* wait for changes in contrast/brightness setting */
			/* Limit Values */
			if (Contrast   > 100)Contrast   = 100;
			if (Brightness > 100)Brightness = 100;

			if (Contrast != contrast_old)
			{
				contrast_old = Contrast ;
				hmiStep = S$STEP_SETCONTRAST;
			}
			else if (Brightness != brightness_old)
			{
				brightness_old = Brightness ;
				hmiStep = S$STEP_SETBRIGHTNESS;
			}
		break;

		case S$STEP_SETCONTRAST:	/* set new contrast value */
			if (!VA_Saccess(1, gPanelObj.vcHandle))
			{
				VA_SetContrast(1, gPanelObj.vcHandle, Contrast);
				VA_Srelease(1, gPanelObj.vcHandle);
			}
			hmiStep = S$STEP_IDLE;
		break;

		case S$STEP_SETBRIGHTNESS:	/* set new brightness value */
			if (!VA_Saccess(1, gPanelObj.vcHandle))
			{
				VA_SetBrightness(1, gPanelObj.vcHandle, Brightness);
				VA_Srelease(1, gPanelObj.vcHandle);
			}
			hmiStep = S$STEP_IDLE;
		break;
	}/* switch (hmiStep) */
    
    /* -------------------------- system time set in HMI -------------------------- */
    switch (hmiTimeStep)
    {
        case S$STEP_INIT:				   /*  Init start   */
            RTC_gettime(&GetRTCTime);
            SetRTCTime = GetRTCTime;
			
            /*  定时停机  */
            if(!OldRTCTime.year)    	/* 第一次下载  */
            {
               OldRTCTime = GetRTCTime;
					gMachineWorkDays = 0;
               gMachineStopDays = 0;
            }
            else
            {
            	if(GetRTCTime.year >= OldRTCTime.year)     /*  battery keep */
               {
						gMachineWorkDays = gMachineWorkDays + ((GetRTCTime.year - OldRTCTime.year)* 12 + GetRTCTime.month - OldRTCTime.month) * 30
						                      + GetRTCTime.day - OldRTCTime.day;
                  gMachineStopDays = gMachineStopDays + ((GetRTCTime.year - OldRTCTime.year)* 12 + GetRTCTime.month - OldRTCTime.month) * 30
                        + GetRTCTime.day - OldRTCTime.day;
               }
               OldRTCTime = GetRTCTime;
            }/* if(!OldRTCTime.year) */

            hmiTimeStep = S$STEP_GET_TIME;
            break;
        case S$STEP_GET_TIME :
            if((SetRTCTime.hour   != GetRTCTime.hour)     ||
               (SetRTCTime.minute != GetRTCTime.minute)   ||
               (SetRTCTime.second != GetRTCTime.second)   ||
               (SetRTCTime.month  != GetRTCTime.month)    ||
               (SetRTCTime.day    != GetRTCTime.day )     ||
               (SetRTCTime.year   != GetRTCTime.year))
            {
                hmiTimeStep = S$STEP_SET_TIME ;		
            }
            else
            {
                RTC_gettime(&GetRTCTime);
                SetRTCTime = GetRTCTime;

                if(GetRTCTime.day != GetRTCTime.day )
                {
						 gMachineWorkDays++;
                   gMachineStopDays++;
                }

                OldRTCTime = GetRTCTime;

                strcpy(gPanelObj.Password4,"");
                strcpy(CurrentTime,"");
                strcpy(Time,"");
                itoa((long)(GetRTCTime.year ),(unsigned long)NumName);
                strcat(Time,NumName);

                strcat(gPanelObj.Password4,NumName);

                strcat(Time,"-");
                if(GetRTCTime.month < 10)
                {
                    strcat(Time,"0");
                }
                itoa((long)(GetRTCTime.month ),(unsigned long)NumName);
                strcat(Time,NumName);
                strcat(Time,"-");

                if(GetRTCTime.day < 10)
                {
                    strcat(Time,"0");
                }
                itoa((long)(GetRTCTime.day ),(unsigned long)NumName);
                strcat(Time,NumName);

                strcat(CurrentTime,Time);

                strcat(CurrentTime," ");

                if(GetRTCTime.hour < 10)
                {
                    strcat(CurrentTime,"0");
                }
                itoa((long)(GetRTCTime.hour ),(unsigned long)NumName);
                strcat(CurrentTime,NumName);

                strcat(CurrentTime,":");
                if(GetRTCTime.minute < 10)
                {
                    strcat(CurrentTime,"0");
                }
                itoa((long)(GetRTCTime.minute ),(unsigned long)NumName);
                strcat(CurrentTime,NumName);

                strcpy(gPanelObj.CurrentTime,CurrentTime);

                strcat(gPanelObj.CurrentTime,":");
                if(GetRTCTime.second < 10)
                {
                    strcat(gPanelObj.CurrentTime,"0");
                }
                itoa((long)(GetRTCTime.second ),(unsigned long)NumName);
                strcat(gPanelObj.CurrentTime,NumName);

                strcpy(gPanelObj.PasswordHigh,"2016");

                itoa((long)(GetRTCTime.day%10),(unsigned long)NumName);
                strcat(gPanelObj.PasswordHigh,NumName);

                itoa((long)(GetRTCTime.hour%10),(unsigned long)NumName);
                strcat(gPanelObj.PasswordHigh,NumName);

                itoa((long)(GetRTCTime.minute/10),(unsigned long)NumName);
                strcat(gPanelObj.PasswordHigh,NumName);

                itoa((long)(GetRTCTime.hour%10),(unsigned long)NumName);
                strcat(gPanelObj.Password4,NumName);

                itoa((long)(GetRTCTime.minute%10),(unsigned long)NumName);
                strcat(gPanelObj.Password4,NumName);
            }
            break;
        case S$STEP_SET_TIME :
                
            RTC_settime(&SetRTCTime); 
			
            if((SetRTCTime.day   != OldRTCTime.day  ) ||
               (SetRTCTime.month != OldRTCTime.month) ||
               (SetRTCTime.year  != OldRTCTime.year ))
            {
                gMachineStopDays++;
            }	
            OldRTCTime = SetRTCTime;
            GetRTCTime = SetRTCTime;
			
            hmiTimeStep = S$STEP_INIT;
            break;
    }
	
	
	
	
}/* end of cycle */
/* ----------------------------------------------------------------------------- */


