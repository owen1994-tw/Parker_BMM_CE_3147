/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: lubricate
 * File: lubricate.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program lubricate --- oil lubrication control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "edge.h"               /*  edge function  */
#include "keyap2100.h"
//#include "keyap880.h"

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT lubr_init(void)
{
	TimeStep  = 0;
   LubricateStep  = 0;
   LMold.Lubricate.Step = 0;
   RMold.Lubricate.Step = 0;
	Count = 0;
}  /* end of _INIT */
/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				   ****
**************************************************************************************/
void _CYCLIC lubr_cyclic(void)
{
	/* -------------- Machine running time -------------- */
	Count++;
	if(Count >=300000)Count=0;
	if(0 == Count%3000)
	{
		gMachineWorkTime++;
	}
   
   /* -------------- Machine yield -------------- */
   if(gMachinePara.DieheadNum < 1)gMachinePara.DieheadNum = 1;
   gMachineInfo.Yield = gMachineInfo.CycleTimes * gMachinePara.DieheadNum;
	
	if(1 == gMachineInfo.Auto)
	{
		if(1 == LMold.Option.Enable && 1 == RMold.Option.Enable)
		{
			if(LMold.TimeDis.CycleAlarmTime != 0 && RMold.TimeDis.CycleAlarmTime != 0)
			{
				gMachineInfo.Yield_H = 3600.0 * 100 * 4/(LMold.TimeDis.CycleAlarmTime + RMold.TimeDis.CycleAlarmTime) * gMachinePara.DieheadNum;
			}
			else
			{
				gMachineInfo.Yield_H = 0;  
			}
		}
		else if(1 == LMold.Option.Enable && 0 == RMold.Option.Enable)
		{
			if(LMold.TimeDis.CycleAlarmTime != 0)
			{
				gMachineInfo.Yield_H = 3600.0 * 100/LMold.TimeDis.CycleAlarmTime * gMachinePara.DieheadNum;
			}
			else
			{
				gMachineInfo.Yield_H = 0;  
			}	
		}
		else if(0 == LMold.Option.Enable && 1 == RMold.Option.Enable)
		{
			if(RMold.TimeDis.CycleAlarmTime != 0)
			{
				gMachineInfo.Yield_H = 3600.0 * 100 /RMold.TimeDis.CycleAlarmTime * gMachinePara.DieheadNum;
			}
			else
			{
				gMachineInfo.Yield_H = 0;  
			}	
		}
	}

	
   if((gMachinePara.StopYield > 0) && (1 == gMachinePara.bReachYieldStop))
   {
      if((gMachineInfo.Yield >= gMachinePara.StopYield) && (1 == gMachineInfo.Auto) /*&& (0==gMachineInfo.CycleTimeCounReach)*/)
      {
      //   gPROCESS_info.semi_auto = 1;
			gAlarm.CycleTimeCounReach =1;
			gMachineInfo.CycleTimeCounReach = 1;
			gPanelObj.RKeyMatric[S$KEYMATRIX_AUTO_STOP] = 1;
			gDisableAll_HMI = 1;
			//			gMachineIn.AutoStop =1;   //ipis0131
      }
   }
//	if (0==gAlarm.CycleTimeCounReach)
//	{
//		CycleTimeReachFlag =0;
//	}
	
//	if((gMachinePara.StopYield > 0) && (0 == gMachinePara.bReachYieldStop) &&(0==gAlarm.CycleTimeCounReach)) //ipis0129
//	{		
//		if((gMachineInfo.Yield >= gMachinePara.StopYield) && (1 == gMachineInfo.Auto) &&( gCycleTimesOld==gCycleTimes ))
//		{
//
//			CycleTimeReachFlag =1;
//						
//		}	
//	}
//	else
//	{
//		CycleTimeReachFlag =0;
//	}
//	
//	if (EDGEPOS(CycleTimeReachFlag,PCycleTimeReachFlag))
//	{	
//		gAlarm.CycleTimeCounReach =1;
//	}

		
	if (1==bCycleTimeReset)
	{
		gCycleTimes=0;
		gCycleTimesL =0;
		gCycleTimesR =0;
	}
	gMachineInfo.CycleTimes  = gCycleTimes;
	gMachineInfo.CycleTimesL = gCycleTimesL;
	gMachineInfo.CycleTimesR = gCycleTimesR;
	
	if (1 == bBatchDoneCountReset)
	{
		bBatchDoneCountReset = 0;
		gBatchInfo.Batch = 0;
		gBatchInfo.BatchDoneCount = 0;
	}
	
   /* -------------- Default lubrication time -------------- */
	if(gMachineFix.Lubrication.LubrPumpWorkTime == 0)gMachineFix.Lubrication.LubrPumpWorkTime = 5;
	if(gMachineFix.Lubrication.LubrPumpStopTime == 0)gMachineFix.Lubrication.LubrPumpStopTime = 6000;
   if(gMachineFix.Lubrication.SetCycleTimes == 0   )gMachineFix.Lubrication.SetCycleTimes = 2000;
   
   /* ---------------- Start lubrication ---------------- */
	if(1 == EDGEPOS(gMachineOut.OilPump,OilPump))
	{
      if(1 == gUserFix_new.bTimeLubricate)
      {
         if(gMachineFix.Lubrication.LubrPumpWorkTime != 0 && gMachineFix.Lubrication.LubrPumpStopTime != 0)
         {
            TimeStep = 100;  /* first stop time then work time */
         }
         else
         {
            gAlarm.LubrTimeZero = 1;
         }
      }
	}
		
	if(0 == gMachineOut.OilPump)
	{
		TimeStep = 0;
	}

	if(1 == gMachineIn.LubriFilter || 1 == gMachineIn.LubriOverload)
	{
		TimeStep = 0;
      LubricateStep = 0;
	}

   if(1 == gUserFix_new.bTimeLubricate)     /*  时间控制  */
   {
      
      /* ---------- step machine control ---------- */
      switch(TimeStep)
      {
         case 0:
            gMacTimer.LubrWork.IN = 0;
            gMacTimer.LubrStop.IN = 0;
			LubrPumpWorkDis = 0;
			LubrPumpStopDis = 0;
				
            gMachineOut.Lubricate = gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION];
            break;
         
         case 100:
            gMacTimer.LubrWork.IN = 0;
            gMacTimer.LubrStop.IN = 0;
            TimeStep = 200;
            break;
         
         case 200:
            if(gMachineFix.Lubrication.LubrPumpStopTime != 0)
            {
               gMacTimer.LubrStop.IN = 1;
               gMacTimer.LubrStop.PT = gMachineFix.Lubrication.LubrPumpStopTime * 100 ;
               TimeStep = 300;
            }
            else
            {
               TimeStep = 400;
            }
            break;
         
         case 300:
            gMachineOut.Lubricate = 0;
            LubrPumpStopDis = gMacTimer.LubrStop.ET/100;
   
            if(1 == gMacTimer.LubrStop.Q)
            {
               gMachineOut.Lubricate = 0;
               gMacTimer.LubrStop.IN = 0;
               TimeStep = 400;
            }
            break;
         
         case 400:
            if(gMachineFix.Lubrication.LubrPumpWorkTime != 0)
            {
               gMacTimer.LubrWork.IN = 1;
               gMacTimer.LubrWork.PT = gMachineFix.Lubrication.LubrPumpWorkTime * 100 ;
               TimeStep = 500;
            }
            else
            {
               TimeStep = 600;
            }
            break;
         
         case 500:
            gMachineOut.Lubricate = 1;
            LubrPumpWorkDis = gMacTimer.LubrWork.ET/100;
   
            if(1 == gMacTimer.LubrWork.Q)
            {
               gMachineOut.Lubricate = 0;
               gMacTimer.LubrWork.IN = 0;
               TimeStep = 600;
            }
            break;
         
         case 600:
            TimeStep = 100;
            break;
      }/* switch(Step) */
   }/* if(1 == gUserFix_new.bTimeLubricate) */
   else
   {
      TimeStep = 0;
   }
   
   /* Lubrication auto start*/
   if((0 ==(gMachineInfo.CycleTimes % gMachineFix.Lubrication.SetCycleTimes))&& (gMachineInfo.CycleTimes != 0))
   {
      if(gMachineInfo.CycleTimes != OldCycleTimes)
      {
         if(0 == gUserFix_new.bTimeLubricate)LubricateStep = 100;
      }
   }
 	
   OldCycleTimes = gMachineInfo.CycleTimes;
   
   if(0 == gUserFix_new.bTimeLubricate)     /*  次数控制  */
   {
      /* step machine control */
      switch(LubricateStep)
      {
         case 0:
            gMacTimer.LubrWork.IN = 0;
            gMachineOut.Lubricate = gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION];
            LubrPumpWorkDis = 0;
            break;
         
         case 100:
            gMacTimer.LubrWork.IN = 1;
            gMacTimer.LubrWork.PT = gMachineFix.Lubrication.LubrPumpWorkTime * 100;
            LubricateStep  = 200;	
            break;
   			
         case 200:
            gMachineOut.Lubricate = 1;
            LubrPumpWorkDis = gMacTimer.LubrWork.ET/100;
            if(1 == gMacTimer.LubrWork.Q)
            {
               gMachineOut.Lubricate = 0;
               LubrPumpWorkDis  = 0;
               gMacTimer.LubrWork.IN = 0;
               LubricateStep  = 300;
            }
            break;
   			
         case 300:
            gMachineOut.Lubricate = 0;
            LubricateStep = 0;
            break;
      }/* switch(LubricateStep) */
   }/* if(0 == gUserFix_new.bTimeLubricate) */
   else 
   {
      LubricateStep = 0;  
   }
   
   /*  left mold manual button start  */
   if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION])
   {
		if((0 == RightOption) && (1 == LeftOption))
		{
			if(0 == LMold.Lubricate.Step)LMold.Lubricate.Step = 100;
		}
   }

   /* left mold auto start*/
   if((0 ==(gMachineInfo.CycleTimesL % gMachineFix.Lubrication.SetCycleTimes))&& (gMachineInfo.CycleTimesL != 0))
   {
      if(gMachineInfo.CycleTimesL != OldCycleTimesL)
      {
         if(1 == LMold.Option.Enable)LMold.Lubricate.Step = 100;
      }
   }
 	
   OldCycleTimesL = gMachineInfo.CycleTimesL;
   
   /* step machine control */
   switch(LMold.Lubricate.Step)
   {
      case 0:
         LMold.Lubricate.Timer.IN = 0;
         gMachineOut.LMold.Lubricate = 0;
         LubrPumpWorkDisL = 0;
         break;
      
      case 100:
         LMold.Lubricate.Timer.IN = 1;
         LMold.Lubricate.Timer.PT = gMachineFix.Lubrication.LubrPumpWorkTime * 100;
         LMold.Lubricate.Step  = 200;	
         break;
			
      case 200:
         gMachineOut.LMold.Lubricate = 1;
         LubrPumpWorkDisL = LMold.Lubricate.Timer.ET/100;
         if(1 == LMold.Lubricate.Timer.Q)
         {
            LubrPumpWorkDisL  = 0;
            LMold.Lubricate.Timer.IN = 0;
            LMold.Lubricate.Step  = 300;
         }
         break;
			
      case 300:
         gMachineOut.LMold.Lubricate = 0;
         if(0 == gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION])
         {
            gMachineOut.LMold.Lubricate = 0;
            LMold.Lubricate.Step = 0;
         }
         break;
   }/* switch(LMold.Lubricate.Step) */
   
   /*  Right mold manual button start  */
   if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION])
   {
		if((1 == RightOption) && (0 == LeftOption))
		{
			if(0 == RMold.Lubricate.Step)RMold.Lubricate.Step = 100;
		}
   }
 	
   /* right mold */
   if((0 ==(gMachineInfo.CycleTimesR % gMachineFix.Lubrication.SetCycleTimes))&& (gMachineInfo.CycleTimesR != 0))
   {
      if(gMachineInfo.CycleTimesR != OldCycleTimesR)
      {
         if(1 == RMold.Option.Enable)RMold.Lubricate.Step = 100;
      }
   }
 	
   OldCycleTimesR = gMachineInfo.CycleTimesR;
   
   /* step machine control */
   switch(RMold.Lubricate.Step)
   {
      case 0:
         RMold.Lubricate.Timer.IN = 0;
         gMachineOut.RMold.Lubricate = 0;
         LubrPumpWorkDisR = 0;
         break;
      
      case 100:
         RMold.Lubricate.Timer.IN = 1;
         RMold.Lubricate.Timer.PT = gMachineFix.Lubrication.LubrPumpWorkTime * 100;
         RMold.Lubricate.Step  = 200;	
         break;
			
      case 200:
         gMachineOut.RMold.Lubricate = 1;
         LubrPumpWorkDisR = RMold.Lubricate.Timer.ET/100;
         if(1 == RMold.Lubricate.Timer.Q)
         {
            LubrPumpWorkDisR  = 0;
            RMold.Lubricate.Timer.IN = 0;
            RMold.Lubricate.Step  = 300;
         }
         break;
			
      case 300:
         gMachineOut.RMold.Lubricate = 1;
         if(0 == gPanelObj.KeyMatric[S$KEYMATRIX_LUBRICATION])
         {
            gMachineOut.RMold.Lubricate = 0;
            RMold.Lubricate.Step = 0;
         }
         break;
   }/* switch(RMold.Lubricate.Step) */
   
}  /* end of _CYCLIC */


