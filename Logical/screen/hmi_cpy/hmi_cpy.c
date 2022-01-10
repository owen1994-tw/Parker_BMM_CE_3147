/********************************************************************
 * COPYRIGHT -- B&R Automation China 
 ********************************************************************
 * Program: hmi_cpy
 * File: hmi_cpy.c
 * Author: lijianjun
 * Created: May 27, 2015
 ********************************************************************
 * Implementation of program hmi_cpy
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "pic_id.h"

void _INIT hmi_cpyINIT(void)
{
	/* TODO: Add code here */
   StepCopyScreen = 0;
   
   /* ----- get task cycle time ----- */
   RTInfo_Task.enable = 1;
   RTInfo(&RTInfo_Task);

   tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
}

void _CYCLIC hmi_cpyCYCLIC(void)
{
  
   /* there is insert USB disk */
   if(1 == gRecipeOperate.usbDiskOk)
   {
      if(0 == StepCopyScreen)
      {
         if(1 == gScreenShot.CmdScreenShotAll)
         {
            gScreenShot.CmdScreenShotAll = 0;
            StepCopyScreen = 100;
         }
      }
      else
      {
         gScreenShot.CmdScreenShotAll = 0; 
      }
   }
   else
   {
      if(1 == gScreenShot.CmdScreenShotAll)
      {
         gImply.NoUSB = 1; 
         gScreenShot.CmdScreenShotAll = 0;
      }
      StepCopyScreen = 0;
      idxPicture = 0;
   }
   
   /* ------ step machine control ------ */
   switch(StepCopyScreen)
   {
      case 0:
         gScreenShot.ScreenShotRun = 0;
         tTimeOut = 0.0;
         break;
      
      /* change to first picture  */
      case 100:
         gScreenShot.ScreenShotRun = 1;
         idxPicture = PIC_OVERVIEW;      /* first picture --- PIC_OVERVIEW   */
         tTimeOut = 0.0;
         if(gPanelObj.GetPicture != PIC_OVERVIEW)
         {   
            gPanelObj.SetPicture = PIC_OVERVIEW;
         }
         StepCopyScreen = 200;
         break;
      
      /*  change picture success or false  */
      case 200:   
         tTimeOut = tTimeOut + tTask;
         if(idxPicture == gPanelObj.GetPicture)
         {
            StepCopyScreen = 300; 
         }
         
         if(tTimeOut > 3.0)      /* change timeout reach */
         {
            tTimeOut = 0.0;
            StepCopyScreen = 600; 
         }
         break;
      
      /* change picture ok then send screen shot command  */
      case 300:
         tTimeOut = 0.0;
         gScreenShot.CmdScreenShot = 1;
         StepCopyScreen = 400;
         break;
      
      case 400:      
         tTimeOut = tTimeOut + tTask;
         
         if(tTimeOut > 0.5)
         {
            if(0 == gScreenShot.CmdScreenShot)
            {
               if(1 == gScreenShot.Status)
               {
                  tTimeOut = 0.0;
                  StepCopyScreen = 600;
               }
               else if(3 == gScreenShot.Status)
               {
                  tTimeOut = 0.0;
                  StepCopyScreen = 500;
               }
            }
         }
      
         if(tTimeOut > 20.0)      /* screen shot timeout reach */
         {
            tTimeOut = 0.0;
            gImply.ScreenShotFalse = 1;  /* screen shot error */
            StepCopyScreen = 3000; 
         }
         break;
      
      /* screen shot flase then send command again  */
      case 500:
         if(1 == gScreenShot.Status)
         {
            tTimeOut = 0.0;
            StepCopyScreen = 300;
         }
         break;
      
      /* screen shot ok then change to next picture */
      case 600:
         tTimeOut = 0.0;
       // if(idxPicture < PIC_GAP_CALIBRATE_HELP + 1)
		if(idxPicture < PIC_FAST_SETTING + 1)  //ipis1227
		
         {
            idxPicture = idxPicture + 1;
            gPanelObj.SetPicture = idxPicture;
            StepCopyScreen = 200;
         }
         else
         {
            gPanelObj.SetPicture = 1;
            StepCopyScreen = 3000;
         }
         break;
            
      case 3000:
         gScreenShot.ScreenShotRun = 0;
         StepCopyScreen = 0;
         break;
   }/* switch(StepCopyScreen) */
   
}

void _EXIT hmi_cpyEXIT(void)
{
	/* TODO: Add code here */
}
