/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: extruder
 * File: extruder.c
 * Author: lij
 * Created: January 05, 2015
 ********************************************************************
 * Implementation of program extruder
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
** 					               include user files								 **
**************************************************************************************/
#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
** 					 	                 define								                   **
**************************************************************************************/
#define     S$EXTRUDER_WAIT		           	 0
#define     S$EXTRUDER_CONTACTOR_ON		   	10
#define     S$EXTRUDER_POWER_ON_DELAY			20
#define     S$EXTRUDER_POWER_ON		        	30
#define     S$EXTRUDER_READY		       		100
#define     S$EXTRUDER_BASIC_ZONE		  		110
#define     S$EXTRUDER_PRODUCTION_ZONE	   	130
#define     S$EXTRUDER_DEC_ZONE		       	150
#define     S$EXTRUDER_POWER_OFF_DECEL			200
#define     S$EXTRUDER_POWER_OFF_PROTECT		210
#define     S$EXTRUDER_POWER_OFF_DELAY			220
/**************************************************************************************
                               Function Declarations
**************************************************************************************/
void SpeedUpDw(short ins_des,short command,speedAdjust_typ * pSpeedUpDwData);

void ExtruderCtrl(Extruder_Ctrl_typ * pExtruder,Extruder_Para_typ * pExtrPara,speedAdjust_typ * pSpeedUpDwData, Lubricate_Check_typ * pLubricCheck);

/**************************************************************************************
     	        	       I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT extruderINIT(void)
{
	/* TODO: Add code here */
    RTInfo_Task.enable = 1;
    RTInfo(&RTInfo_Task);

    tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
    
    MotorSpeedA.minVal  	  = 0;
    MotorSpeedA.frequence	  = 30;
    MotorSpeedA.waitTimeSlow = 200;
    MotorSpeedA.waitTimeFast = 500;
    MotorSpeedA.unitsSlow    = 1;
    MotorSpeedA.unitsFast	= 5;
    MotorSpeedA.unitsVFast	= 20;

    MotorSpeedA.maxVal = 100;
    MotorSpeedA.minVal = 0;

    MotorSpeedB.minVal  	 = 0;
    MotorSpeedB.frequence	 = 30;
    MotorSpeedB.waitTimeSlow = 200;
    MotorSpeedB.waitTimeFast = 500;
    MotorSpeedB.unitsSlow    = 1;
    MotorSpeedB.unitsFast	 = 5;
    MotorSpeedB.unitsVFast	 = 20;

    MotorSpeedB.maxVal = 100;
    MotorSpeedB.minVal = 0;

    MotorSpeedVisu.frequence	= 30;
    MotorSpeedVisu.waitTimeSlow = 200;
    MotorSpeedVisu.waitTimeFast = 500;
    MotorSpeedVisu.unitsSlow    = 1;
    MotorSpeedVisu.unitsFast	= 5;
    MotorSpeedVisu.unitsVFast	= 20;

    MotorSpeedVisu.maxVal = 100;
    MotorSpeedVisu.minVal = 0;
    
    cntExtruderStopDelay = 0;

    gExtruderA.Step = S$EXTRUDER_WAIT;
    gExtruderB.Step = S$EXTRUDER_WAIT;
    gExtruderVisu.Step = S$EXTRUDER_WAIT;
    
    gExtruderA.tTask  = tTask;
    gExtruderB.tTask  = tTask;
    gExtruderVisu.tTask = tTask; 
	
	bHideCurrencyA = 0;
	bHideMeltTempA = 1;
	bHidePressureA = 1;
	
	bHideCurrencyB = 0;
	bHideMeltTempB = 1;
	bHidePressureB = 1;
	
	bHideCurrencyV = 1;
	bHideMeltTempV = 1;
	bHidePressureV = 1;
	
}
/**************************************************************************************
        				   C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _CYCLIC extruderCYCLIC(void)
{     
	if(0 == gMachineFix.ExtruderA.RPM10v )gMachineFix.ExtruderA.RPM10v = 1500;
	if(0 == gMachineFix.ExtruderB.RPM10v)gMachineFix.ExtruderB.RPM10v  = 1500;
	if(0 == gMachineFix.ExtruderVisu.RPM10v )gMachineFix.ExtruderVisu.RPM10v = 1500;
   
	if(0 == gMachineFix.ExtruderA.Currency10V )gMachineFix.ExtruderA.Currency10V = 50;
	if(0 == gMachineFix.ExtruderB.Currency10V)gMachineFix.ExtruderB.Currency10V= 50;
	if(0 == gMachineFix.ExtruderVisu.Currency10V )gMachineFix.ExtruderVisu.Currency10V = 50;
   
	/*  -------------------- Extruder motor speed and current -------------------------- */
	if(gMachineOut.ExtrA_On)
	{
		gExtruderA.MotorRPM = gExtruderA.aiActRPM/32767.0  * gMachineFix.ExtruderA.RPM10v;
		gExtruderA.Current  = gExtruderA.aiCurrent/32767.0 * gMachineFix.ExtruderA.Currency10V;
		gExtruderA.ActRPM   = gExtruderA.ActSpeed/100.0  * gMachineFix.ExtruderA.RPM10v;
	}
	else
	{
		gExtruderA.MotorRPM = 0.0;
		gExtruderA.Current  = 0.0;
		gExtruderA.ActRPM   = 0.0;
	}
	   
	if(gMachineOut.ExtrB_On)
	{
		gExtruderB.MotorRPM = gExtruderB.aiActRPM/32767.0  * gMachineFix.ExtruderB.RPM10v;
		gExtruderB.Current  = gExtruderB.aiCurrent/32767.0 * gMachineFix.ExtruderB.Currency10V;
	}
	else
	{
		gExtruderB.MotorRPM = 0.0;
		gExtruderB.Current  = 0.0;
	}
	   	
	if(gMachineOut.ExtrVisu_On)
	{
		gExtruderVisu.MotorRPM = gExtruderVisu.aiActRPM/32767.0  * gMachineFix.ExtruderVisu.RPM10v;
		gExtruderVisu.Current  = gExtruderVisu.aiCurrent/32767.0 * gMachineFix.ExtruderVisu.Currency10V;
	}
	else
	{
		gExtruderVisu.MotorRPM = 0.0;
		gExtruderVisu.Current  = 0.0;
	}
   
	/* ------------------ Extruder zero and max pressure ------------------ */
	if((1 == btnZeroExtrA) && (0 == btnZeroExtrA_POS))
	{
		gMachineFix.ExtruderA.MeltP0V = gExtruderA.aiMeltPressure/3276.7;
		gRecipeOperate.OnceSave = 1;
	}
	
	if(1 == btnMaxExtrA)
	{
		gMachineOut.MeltPrShunt = 1;
		tMaxExtrA = tMaxExtrA + tTask;
	}
	else
	{
		tMaxExtrA = 0.0;
	}
	
	if(tMaxExtrA > 3.0 - tTask  && tMaxExtrA < 3.0 + tTask  )
	{
		gMachineFix.ExtruderA.MeltP10V = gExtruderA.aiMeltPressure/3276.7;
		gRecipeOperate.OnceSave = 1;
	}
	
	if((1 == btnZeroExtrB) && (0 == btnZeroExtrB_POS))
	{
		gMachineFix.ExtruderB.MeltP0V = gHardWare.AI4632_3[1]/3276.7;		/* no this input  */
		gRecipeOperate.OnceSave = 1;
	}
	
	if(1 == btnMaxExtrB)
	{
		gMachineOut.MeltPrShunt = 1;
		tMaxExtrB = tMaxExtrB + tTask;
	}
	else
	{
		tMaxExtrB = 0.0;
	}
	
	if(tMaxExtrB > 3.0 - tTask  && tMaxExtrB < 3.0 + tTask )
	{
		gMachineFix.ExtruderB.MeltP10V = gHardWare.AI4632_3[1]/3276.7;
		gRecipeOperate.OnceSave = 1;
	}
	
	if(0 == btnMaxExtrA && 0 == btnMaxExtrB)
	{
		gMachineOut.MeltPrShunt = 0;	
	}
	/*  -------------------- Melt pressure alarm  -------------------------- */
	if(gMachineFix.ExtruderA.WarningMeltP < 50)gMachineFix.ExtruderA.WarningMeltP = 50.0;
	if(gMachineFix.ExtruderA.AlarmMeltP < 100)gMachineFix.ExtruderA.AlarmMeltP = 100.0;
   
	if(gExtruderA.aiMeltPressure != 32767)
	{
		if(gExtruderA.MeltPressure > gMachineFix.ExtruderA.AlarmMeltP)
		{
			if(0 == btnMaxExtrA)
			{
				gAlarm.ExtrAMeltP_Alarm = 1;
				gAlarm.ExtrAMeltP_Warn  = 0;
			}
		}
		else if(gExtruderA.MeltPressure > gMachineFix.ExtruderA.WarningMeltP)
		{
			gAlarm.ExtrAMeltP_Alarm = 0;
			gAlarm.ExtrAMeltP_Warn  = 1;
		}
		else
		{
			gAlarm.ExtrAMeltP_Alarm = 0;
			gAlarm.ExtrAMeltP_Warn  = 0;
		}
	}
	else
	{
		gAlarm.ExtrAMeltP_Alarm = 0;
		gAlarm.ExtrAMeltP_Warn  = 0;
	}
	
	/* --------------------  current alarm and warning -------------------- */
	if(gMachineFix.ExtruderA.WarningCurrent < 2)gMachineFix.ExtruderA.WarningCurrent = 2;
	if(gMachineFix.ExtruderA.AlarmCurrent   < 5)gMachineFix.ExtruderA.AlarmCurrent = 5;

	if(gExtruderA.aiCurrent != 32767)
	{
		if(gExtruderA.Current > gMachineFix.ExtruderA.AlarmCurrent)
		{
			tAlarmCurentA = tAlarmCurentA+tTask;
			if(tAlarmCurentA > 10000)tAlarmCurentA= 1000;
			tWarnCurentA  = 0.0;
		}
		else if(gExtruderA.Current > gMachineFix.ExtruderA.WarningCurrent)
		{
			tAlarmCurentA = 0.0;
			tWarnCurentA = tWarnCurentA+tTask;
			if(tWarnCurentA > 10000)tWarnCurentA = 1000;
		}
		else
		{
			tWarnCurentA = 0.0;
			tAlarmCurentA = 0.0;
		}
		
		if(tAlarmCurentA > 5.0)
		{
			gAlarm.ExtrACurrentAlarm = 1;	
		}
		else
		{
			gAlarm.ExtrACurrentAlarm = 0;	
		}
		
		if(tWarnCurentA > 5.0)
		{
			gAlarm.ExtrACurrentWarn = 1;	
		}
		else
		{
			gAlarm.ExtrACurrentWarn = 0;
		}
	}
	else
	{
		tWarnCurentA = 0.0;
		tAlarmCurentA = 0.0;

		gAlarm.ExtrACurrentWarn = 0;
		gAlarm.ExtrACurrentAlarm = 0;
	}
	
	if(gMachineFix.ExtruderB.WarningCurrent < 2)gMachineFix.ExtruderB.WarningCurrent = 2;
	if(gMachineFix.ExtruderB.AlarmCurrent   < 5)gMachineFix.ExtruderB.AlarmCurrent = 5;

	if(gExtruderB.aiCurrent != 32767)
	{
		if(gExtruderB.Current > gMachineFix.ExtruderB.AlarmCurrent)
		{
			tAlarmCurentB = tAlarmCurentB+tTask;
			if(tAlarmCurentB > 10000)tAlarmCurentB= 1000;
			tWarnCurentB  = 0.0;
		}
		else if(gExtruderB.Current > gMachineFix.ExtruderB.WarningCurrent)
		{
			tAlarmCurentB = 0.0;
			tWarnCurentB = tWarnCurentB+tTask;
			if(tWarnCurentB > 10000)tWarnCurentB = 1000;
		}
		else
		{
			tWarnCurentB = 0.0;
			tAlarmCurentB = 0.0;
		}
		
		if(tAlarmCurentB > 5.0)
		{
			gAlarm.ExtrBCurrentAlarm = 1;	
		}
		else
		{
			gAlarm.ExtrBCurrentAlarm = 0;	
		}
		
		if(tWarnCurentB > 5.0)
		{
			gAlarm.ExtrBCurrentWarn = 1;	
		}
		else
		{
			gAlarm.ExtrBCurrentWarn = 0;
		}
	}
	else
	{
		tWarnCurentB = 0.0;
		tAlarmCurentB = 0.0;

		gAlarm.ExtrBCurrentWarn = 0;
		gAlarm.ExtrBCurrentAlarm = 0;
	}
	
	/* -------------------- Hopper cooling  -------------------------- */
	bHideHopperCooling = !gMachineFix.Option.bHopperCooling;
	gMachineFix.Option.bHopperCooling =0; //ipis0319
	if(1 == gMachineFix.Option.bHopperCooling)
	{
		if(!gMachinePara.HopperCoolStart)gMachinePara.HopperCoolStart = 65;
		if(!gMachinePara.HopperCoolStop )gMachinePara.HopperCoolStop = 55;
	
		if(gMachineInfo.TempHopper > gMachinePara.HopperCoolStart)
		{
			gMachineOut.HopperCoolingA = 1;
		}
	
		if(gMachineInfo.TempHopper < gMachinePara.HopperCoolStop)
		{
			gMachineOut.HopperCoolingA = 0;
		}
	}
	else
	{
		gMachineOut.HopperCoolingA = 0;
	}
	/* -------------------- Extruder motor cooling  -------------------------- */
	bHideExtrMotorCooling = !gMachineFix.Option.bExtruderMotorCooling;
	gMachineFix.Option.bExtruderMotorCooling=0; //ipis0319
	if(!gMachinePara.ExtruderA.TempCoolStart)gMachinePara.ExtruderA.TempCoolStart = 75;
	if(!gMachinePara.ExtruderA.TempCoolStop)gMachinePara.ExtruderA.TempCoolStop = 65;
	if(!gMachinePara.ExtrLiftLower.warningMotorTemp)gMachinePara.ExtrLiftLower.warningMotorTemp= 90;
	if(!gMachinePara.ExtrLiftLower.AlarmMotorTemp)gMachinePara.ExtrLiftLower.AlarmMotorTemp = 100;
	
	if(1 == gMachineFix.Option.bExtruderMotorCooling)
	{
		if(gMachineInfo.TempExtrCoolWater > gMachinePara.ExtruderA.TempCoolStart)
		{
			gMachineOut.ExtrA_CoolWaterOn = 1;
		}
	
		if(gMachineInfo.TempExtrCoolWater < gMachinePara.ExtruderA.TempCoolStop)
		{
			gMachineOut.ExtrA_CoolWaterOn = 0;
		}
	
		if(gMachineInfo.TempExtrCoolWater > gMachinePara.ExtrLiftLower.warningMotorTemp)
		{
			tAlarmExtrTempA = tAlarmExtrTempA + tTask;	
		
			if(tAlarmExtrTempA > 10000.0)tAlarmExtrTempA = 100.0;
		}
		else
		{
			tAlarmExtrTempA = 0.0;
		}
	
		if(tAlarmExtrTempA > 10.0)
		{
			gAlarm.A_ExtrMotorTempAlarm = 1;
		}

		if(gMachineInfo.TempExtrCoolWater > gMachinePara.ExtrLiftLower.AlarmMotorTemp)
		{
			tStopExtrTempA = tStopExtrTempA + tTask;
		
			if(tStopExtrTempA > 10000.0)tStopExtrTempA = 100.0;
		}
		else
		{
			tStopExtrTempA = 0.0;
		}
	
		if(tStopExtrTempA > 10.0)
		{
			gAlarm.A_ExtrMotorTempStop = 1;	
		}
   
		if(0 == gMachineIn.ExtrA_ColiTempNormal)
		{
			tColiTempNormalA = tColiTempNormalA + tTask;
		
			if(tColiTempNormalA > 10000.0)tColiTempNormalA = 100.0;
		}
		else
		{
			tColiTempNormalA = 0.0;	
		}
	
		if(tColiTempNormalA > 10.0)
		{
			gAlarm.A_ColiTempNormal = 1;
		}
	}
	else
	{
		gAlarm.A_ExtrMotorTempStop = 0;
		gAlarm.A_ColiTempNormal = 0;
		gMachineOut.ExtrA_CoolWaterOn = 0;
	}
	
   /* ----------------------- extruder key ------------------------- */
   if(1 == gMachineIn.MainControl && gPanelObj.GetPicture != PIC_INIT && gPanelObj.GetPicture != PIC_SCREEN_SAVE)
   {
     gExtruderA.bKeyOnOff       = gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_A];
     gExtruderB.bKeyOnOff    = gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_B];
	  gMachineIn.ExtrA_HopperKey = gPanelObj.KeyMatric[S$KEYMATRIX_HOPPER_A];
   }/*if(1 == gMachineIn.MainControl ..... */
	
   /* ----- A extruder increase or decrease speed ----- */
   if(16384 == gExtruderA.FoucsBasic || 16384 == gExtruderA.FocusProduct || PIC_SPEED != gPanelObj.GetPicture)
   {
      gExtruderA.bKeyInc = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_UP];
      gExtruderA.bKeyDec = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_DN];
   }
   else
   {
      gExtruderA.bKeyInc = 0;
      gExtruderA.bKeyDec = 0;
   }
   gExtruderA.tProtect = gMachineFix.ExtruderA.tProtect;
   gExtruderA.tOnDelay = gMachineFix.ExtruderA.tOnDelay;
   gExtruderA.tOffDelay = gMachineFix.ExtruderA.tOffDelay;
	
	LubricCheckA.diLubricCheck = gMachineIn.ExtrA_LubrNormal;
	LubricCheckA.tSetCheckTime = gMachineFix.ExtruderA.tLubricateCheck;
		
   ExtruderCtrl(&gExtruderA, &gMachinePara.ExtruderA, &MotorSpeedA, &LubricCheckA);
   gMachineOut.ExtrA_On = gExtruderA.doOn;
	
	gAlarm.A_ExtrLubricCheck = LubricCheckA.bLubricError;
//	gAlarm.A_ExtruderMotorOverload = gMachineIn.ExtrA_MotorErr;
	
	
	/* extruder A on and feedback  */
	gMachineIn.ExtrA_OnOff = gMachineOut.ExtrA_On;
	if(gMachineOut.ExtrA_On != gMachineIn.ExtrA_OnOff)
	{
		tExtrA_OnNormal = tExtrA_OnNormal + tTask;
		if(tExtrA_OnNormal >= 600.0)tExtrA_OnNormal = 60.0;
	}
	else
	{
		tExtrA_OnNormal = 0.0;	
	}
	
	if(tExtrA_OnNormal >= 30.0)
	{
		gExtruderA.OnFeedbackErr = 1;
		gAlarm.ExtrA_NotOn = 1;
	}
	else
	{
		gExtruderA.OnFeedbackErr = 0;	
	}
	
	/*  extruder A hopper  */
	if(1 == gMachineIn.ExtrA_HopperKey && 0 == ExtrA_HopperKeyOld)
	{
		if((1 == gMachineIn.ExtrA_OnOff) && (gExtruderA.Out > 0.0))
		{
			gMachineOut.HopperGateOpenA = !gMachineOut.HopperGateOpenA;
		}
		else
		{
			if(0 == gMachineIn.ExtrA_OnOff)gImply.ExtruderA_NotOn = 1;
			if(0.0 == gExtruderA.Out)gImply.ExtruderA_NotOut = 1;
		}
	}
	ExtrA_HopperKeyOld = gMachineIn.ExtrA_HopperKey;
	
	if((0 == gMachineIn.ExtrA_OnOff) || (0.0 == gExtruderA.Out))
	{
		if(1 == gMachineOut.HopperGateOpenA)
		{
			if(0 == gMachineIn.ExtrA_OnOff)gImply.ExtruderA_NotOn = 1;
			if(0.0 == gExtruderA.Out)gImply.ExtruderA_NotOut = 1;
			gMachineOut.HopperGateOpenA = 0;
		}
	}
	
   /* ----- B extruder increase or decrease speed ----- */
   if(16384 == gExtruderB.FoucsBasic || 16384 == gExtruderB.FocusProduct)
   {
      gExtruderB.bKeyInc = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_UP];
      gExtruderB.bKeyDec = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_DN];
   }
   else
   {
      gExtruderB.bKeyInc = 0;
      gExtruderB.bKeyDec = 0;
   }   
   gExtruderB.tProtect = gMachineFix.ExtruderA.tProtect;
   gExtruderB.tOnDelay = gMachineFix.ExtruderA.tOnDelay;
   gExtruderB.tOffDelay = gMachineFix.ExtruderA.tOffDelay;
	
	LubricCheckB.diLubricCheck = gMachineIn.ExtrB_LubrNormal;
	LubricCheckB.tSetCheckTime = gMachineFix.ExtruderA.tLubricateCheck;
	
   ExtruderCtrl(&gExtruderB, &gMachinePara.ExtruderB, &MotorSpeedB, &LubricCheckB);
   gMachineOut.ExtrB_On = gExtruderB.doOn;
	
	gAlarm.B_ExtrLubricCheck = LubricCheckB.bLubricError;
//	gAlarm.B_ExtruderMotorOverload = gMachineIn.ExtrB_MotorErr;
	
	/* extruder B on and feedback  */
	gMachineIn.ExtrB_OnOff = gMachineOut.ExtrB_On;
	if(gMachineOut.ExtrB_On != gMachineIn.ExtrB_OnOff)
	{
		tExtrB_OnNormal = tExtrA_OnNormal + tTask;
		if(tExtrB_OnNormal >= 600.0)tExtrB_OnNormal = 60.0;
	}
	else
	{
		tExtrB_OnNormal = 0.0;	
	}
	
	if(tExtrB_OnNormal >= 30.0)
	{
		gExtruderB.OnFeedbackErr = 1;
		gAlarm.ExtrB_NotOn = 1;
	}
	else
	{
		gExtruderB.OnFeedbackErr = 0;
	}
	
	if(1 == gMachineIn.ExtrB_HopperKey && 0 == ExtrB_HopperKeyOld)
	{
		if((1 == gMachineIn.ExtrB_OnOff) && (gExtruderB.Out > 0.0))
		{
			gMachineOut.HopperGateOpenB = !gMachineOut.HopperGateOpenB;
		}
		else
		{
			if(0 == gMachineIn.ExtrB_OnOff)gImply.ExtruderB_NotOn = 1;
			if(0.0 == gExtruderB.Out)gImply.ExtruderB_NotOut = 1;
		}
	}
	ExtrB_HopperKeyOld = gMachineIn.ExtrB_HopperKey;
	
	if((0 == gMachineIn.ExtrB_OnOff) || (0.0 == gExtruderB.Out))
	{
		if(1 == gMachineOut.HopperGateOpenB)
		{
			if(0 == gMachineIn.ExtrB_OnOff)gImply.ExtruderB_NotOn = 1;
			if(0.0 == gExtruderB.Out)gImply.ExtruderB_NotOut = 1;
			gMachineOut.HopperGateOpenB = 0;
		}
	}
	
   /* ----- Visual extruder increase or decrease speed ----- */
   if(16384 == gExtruderVisu.FoucsBasic || 16384 == gExtruderVisu.FocusProduct)
   {
      gExtruderVisu.bKeyInc = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_UP];
      gExtruderVisu.bKeyDec = gPanelObj.KeyMatric[S$KEYMATRIX_SPEED_DN];
   }
   else
   {
      gExtruderVisu.bKeyInc = 0;
      gExtruderVisu.bKeyDec = 0;
   }   
   gExtruderVisu.tProtect = gMachineFix.ExtruderA.tProtect;
   gExtruderVisu.tOnDelay = gMachineFix.ExtruderA.tOnDelay;
   gExtruderVisu.tOffDelay = gMachineFix.ExtruderA.tOffDelay;
	
	LubricCheckVisu.diLubricCheck = 1;
	LubricCheckVisu.tSetCheckTime = gMachineFix.ExtruderA.tLubricateCheck;
	
   ExtruderCtrl(&gExtruderVisu, &gMachinePara.ExtruderVisu, &MotorSpeedVisu, &LubricCheckVisu);
   gMachineOut.ExtrVisu_On = gExtruderVisu.doOn;
	
	gAlarm.VisuExtrMotorOverload = gMachineIn.ExtrV_MotorErr;
	gAlarm.VisuExtrCoolOverload  = gMachineIn.ExtrV_CoolMotorErr;
	
	/* extruder A on and feedback  */
	if(gMachineOut.ExtrVisu_On != gMachineIn.ExtrV_OnOff)
	{
		tExtrV_OnNormal = tExtrV_OnNormal + tTask;
		if(tExtrV_OnNormal >= 600.0)tExtrV_OnNormal = 60.0;
	}
	else
	{
		tExtrV_OnNormal = 0.0;	
	}
	
	if(tExtrV_OnNormal >= 30.0)
	{
		gExtruderVisu.OnFeedbackErr = 1;	
		gAlarm.ExtrV_NotOn = 1;
	}
	else
	{
		gExtruderVisu.OnFeedbackErr = 0;	
	}
	
   gMachineInfo.Extruder       = gMachineOut.ExtrA_On;
   gMachineOut.ExtruderCooling = gMachineInfo.Extruder;
	
   /* ----------------------- Stop extruder ----------------------- */
   /* ----- A extruder ----- */
   gExtruderA.bKeyOff = 0;
   if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat           || 0 == gMachineIn.ParisonPumpRunning   || 
       1 == gMachineIn.ExtrA_MotorErr || 1 == gMachineIn.ExtrA_CoolMotorErr || 1 == gAlarm.A_ExtrMotorTempStop      ||
		 1 == gAlarm.ExtrACurrentAlarm)
   {
		if(1 == gMachineOut.ExtrA_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
//			gAlarm.A_ExtruderMotorOverload = gMachineIn.ExtrA_MotorErr;
			gExtruderA.bKeyOff = 1;
		}
   }
   
   /* ----- B extruder ----- */
   gExtruderB.bKeyOff = 0;
   if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat           || 0 == gMachineIn.ParisonPumpRunning || 
       1 == gMachineIn.ExtrB_MotorErr || 1 == gMachineIn.ExtrB_CoolMotorErr || 1 == gAlarm.ExtrBCurrentAlarm)
   {
		if(1 == gMachineOut.ExtrB_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
//			gAlarm.B_ExtruderMotorOverload = gMachineIn.ExtrB_MotorErr;
			gExtruderB.bKeyOff = 1;
		}
   }
   
   /* ----- Visual extruder ----- */
   gExtruderVisu.bKeyOff = 0;
   if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat || 0 == gMachineIn.ParisonPumpRunning || 
       1 == gMachineIn.ExtrV_MotorErr || 1 == gMachineIn.ExtrV_CoolMotorErr || 0 == gMachineIn.ExtrA_OnOff)
   {
		if(1 == gMachineOut.ExtrVisu_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			gAlarm.VisuExtrMotorOverload = gMachineIn.ExtrV_MotorErr;
			if(0 == gMachineIn.ExtrA_OnOff)gAlarm.ExtrA_NotOn = 1;
			gExtruderVisu.bKeyOff = 1;
		}
   }
   
   /* ------------------------ Auto error stop extruder ---------------------------- */
   if(1 == gPROCESS_info.extruderStopEnable)
   {
     cntExtruderStopDelay ++;
   }
   else
   {
     cntExtruderStopDelay = 0;
   }
   
   if(gMachineFix.ExtruderA.StopTime <= 0.0)gMachineFix.ExtruderA.StopTime = 3.0;
   if(gMachineFix.ExtruderA.StopTime < cntExtruderStopDelay * tTask)
   {
     if(1 == gTKmanual.Status)
     {
         gExtruderA.bKeyOff = 1;
         gExtruderB.bKeyOff = 1;
		 gExtruderD.bKeyOff = 1;
         gExtruderVisu.bKeyOff = 1;
         gPROCESS_info.extruderStopEnable = 0;
         cntExtruderStopDelay = 0;
     }
   }
}
/**************************************************************************************
        				   E X I T   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _EXIT extruderEXIT(void)
{
	/* TODO: Add code here */
}

/* adjust the speed of motor*/
void SpeedUpDw(short ins_des,short command,speedAdjust_typ * pSpeedUpDwData)
{
    unsigned short index;

    if(pSpeedUpDwData->pSpeed != 0)  /*  point is not NULL  */
    {
        index = ins_des + 1;
    
        if (command == 1)
        {
            pSpeedUpDwData->iSpeed[index]++;
            pSpeedUpDwData->iSpeed[index+1]++;
            if (pSpeedUpDwData->iSpeed[index+1] == 1) (*pSpeedUpDwData->pSpeed) = (*pSpeedUpDwData->pSpeed) + pSpeedUpDwData->unitsSlow*ins_des;
            if (pSpeedUpDwData->iSpeed[index]>pSpeedUpDwData->frequence) pSpeedUpDwData->iSpeed[index] = 0;
    
            if (pSpeedUpDwData->iSpeed[index+1]<50) ;
            else if (pSpeedUpDwData->iSpeed[index+1]<pSpeedUpDwData->waitTimeSlow)
            {
                if (pSpeedUpDwData->iSpeed[index]== 0) (*pSpeedUpDwData->pSpeed) = (*pSpeedUpDwData->pSpeed) + pSpeedUpDwData->unitsSlow*ins_des;
            }
            else if (pSpeedUpDwData->iSpeed[index+1]<pSpeedUpDwData->waitTimeFast)
            {
                if (pSpeedUpDwData->iSpeed[index] == 0) (*pSpeedUpDwData->pSpeed) = (*pSpeedUpDwData->pSpeed) + pSpeedUpDwData->unitsFast*ins_des;
            }
            else
            {
                if (pSpeedUpDwData->iSpeed[index] == 0) (*pSpeedUpDwData->pSpeed) = (*pSpeedUpDwData->pSpeed) + pSpeedUpDwData->unitsVFast*ins_des;
                pSpeedUpDwData->iSpeed[index+1] = pSpeedUpDwData->waitTimeFast;
            }
        }
        else
        {
            pSpeedUpDwData->iSpeed[index] = 0;
            pSpeedUpDwData->iSpeed[index+1] = 0;
        }
        if ((*pSpeedUpDwData->pSpeed) >= pSpeedUpDwData->maxVal) (*pSpeedUpDwData->pSpeed) = pSpeedUpDwData->maxVal;
        if ((*pSpeedUpDwData->pSpeed) <= pSpeedUpDwData->minVal) (*pSpeedUpDwData->pSpeed) = pSpeedUpDwData->minVal;
    }/*  if(pSpeedUpDwData->pSpeed != 0) */
}/*end of function*/

/* ---------- Extruder contorl ---------- */
void ExtruderCtrl(Extruder_Ctrl_typ * pExtruder, Extruder_Para_typ * pExtrPara, speedAdjust_typ * pSpeedUpDwData, Lubricate_Check_typ * pLubricCheck)
{
    if(pExtruder->tOnDelay  < pExtruder->tTask * 2.0)pExtruder->tOnDelay = pExtruder->tTask * 2.0;
    if(pExtruder->tOffDelay < pExtruder->tTask * 2.0)pExtruder->tOffDelay= pExtruder->tTask * 2.0;
   
    if(pExtruder->tTask <= 0.0)
    {
        pExtruder->tET_s = 0.0;
        pExtruder->LedOn = 0;
        pExtruder->LedProduction = 0;
            
        pExtruder->doContactor = 0;
        pExtruder->doOn = 0;
        pExtruder->Out = 0.0;
        
        pExtruder->bKeyOnOff = 0;
        pExtruder->bKeyOff = 0;
        pExtruder->bKeyOnOffOld = 0;
        pExtruder->bKeyOffOld = 0;
  
        pExtruder->Step = S$EXTRUDER_WAIT;
        return;
    }

    /*  On Off button edge start */
    if(1 == pExtruder->bKeyOnOff && 0 == pExtruder->bKeyOnOffOld)
    {
        if(S$EXTRUDER_WAIT == pExtruder->Step)
        {
				pExtruder->LedOn = 1;
            pExtruder->Step = S$EXTRUDER_CONTACTOR_ON; 
        }
        else
        {
				pExtruder->LedOn = 0;
            if(pExtruder->Step < S$EXTRUDER_POWER_ON && pExtruder->Step > S$EXTRUDER_WAIT)
            {
               pExtruder->Step = S$EXTRUDER_WAIT; 
            }
            else if(pExtruder->Step >= S$EXTRUDER_POWER_ON && pExtruder->Step < S$EXTRUDER_POWER_OFF_DECEL)
            {
                if(pExtruder->Step > S$EXTRUDER_BASIC_ZONE)
                {
                    pExtruder->Step = S$EXTRUDER_POWER_OFF_DECEL; 
                }
                else
                {
                    pExtruder->Step = S$EXTRUDER_POWER_OFF_PROTECT;  
                }
            }
        }
    }
    pExtruder->bKeyOnOffOld = pExtruder->bKeyOnOff;
   
    /* -------- internal stop -------- */
    if(1 == pExtruder->bKeyOff)
    {
		 pExtruder->LedOn = 0;
       if(pExtruder->Step < S$EXTRUDER_POWER_ON && pExtruder->Step > S$EXTRUDER_WAIT)
       {
          pExtruder->Step = S$EXTRUDER_WAIT; 
       }
       else if(pExtruder->Step >= S$EXTRUDER_POWER_ON && pExtruder->Step < S$EXTRUDER_POWER_OFF_DECEL)
       {
          if(pExtruder->Step > S$EXTRUDER_BASIC_ZONE)
          {
             pExtruder->Step = S$EXTRUDER_POWER_OFF_DECEL; 
          }
          else
          {
             pExtruder->Step = S$EXTRUDER_POWER_OFF_PROTECT;  
          }
       }
    }
    pExtruder->bKeyOffOld = pExtruder->bKeyOff;
	
	if(1 == pExtruder->bKeyStop)
	{
		pExtruder->bKeyStop = 0;
		if(pExtruder->Step < S$EXTRUDER_POWER_ON && pExtruder->Step > S$EXTRUDER_WAIT)
		{
			pExtruder->Step = S$EXTRUDER_WAIT; 
		}
		else if(pExtruder->Step >= S$EXTRUDER_POWER_ON && pExtruder->Step < S$EXTRUDER_POWER_OFF_DECEL)
		{
			if(pExtruder->Step > S$EXTRUDER_BASIC_ZONE)
			{
				pExtruder->Step = S$EXTRUDER_POWER_OFF_DECEL; 
			}
			else
			{
				pExtruder->Step = S$EXTRUDER_POWER_OFF_PROTECT;  
			}
		}	
	}
   
    /*  ------ motor protection ------- */
    if(pExtruder->doOnOld != pExtruder->doOn && pExtruder->tTask > 0.0)
    {
        pExtruder->cntProtect = pExtruder->tProtect /pExtruder->tTask;   
    }
    
    if(pExtruder->cntProtect > 0)pExtruder->cntProtect--;

    /* ----- step machine control ----- */
    switch(pExtruder->Step)
    {
        case S$EXTRUDER_WAIT:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 0;
            pExtruder->doOn = 0;
            pExtruder->doFeedingCylFw = pExtruder->bKeyFeedingCylFw; /* manual test */
            pExtruder->Out = 0.0;
				pLubricCheck->bEnable = 0;
            break;
      
        case S$EXTRUDER_CONTACTOR_ON:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = 0.0;
            pExtruder->Step = S$EXTRUDER_POWER_ON_DELAY;
            break;
      
        case S$EXTRUDER_POWER_ON_DELAY:
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 0;
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = 0.0;
            pExtruder->tET_s = pExtruder->tET_s + pExtruder->tTask;
            if(pExtruder->tET_s >= pExtruder->tOnDelay)
            {
                pExtruder->tET_s = 0.0;
                pExtruder->Step = S$EXTRUDER_POWER_ON;
            }
            break;
      
        case S$EXTRUDER_POWER_ON:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->Out = 0.0;
            pExtruder->doFeedingCylFw = 0;
            
            pExtruder->Step = S$EXTRUDER_READY; 
            break;
      
        case S$EXTRUDER_READY:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
             
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = 0.0;
			
				pLubricCheck->bEnable = 1;
				pLubricCheck->bLubricError = 0;
            
            if(1 == EDGEPOS(pExtruder->bKeyBasic,pExtruder->bKeyBasicOld))
            {
                pExtruder->Step = S$EXTRUDER_BASIC_ZONE;
            }
            break;
		
        case S$EXTRUDER_BASIC_ZONE:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 1;
            pExtruder->LedProduction = 0;
            
            pSpeedUpDwData->maxVal = 50;
            
				pSpeedUpDwData->pSpeed = &pExtrPara->BasicSpeed;
            SpeedUpDw(1   , pExtruder->bKeyInc , pSpeedUpDwData);
            SpeedUpDw(- 1 , pExtruder->bKeyDec , pSpeedUpDwData);
				pExtruder->BasicSpeed = pExtrPara->BasicSpeed;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = pExtruder->BasicSpeed;
            
            if(1 == EDGEPOS(pExtruder->bKeyProduction,pExtruder->bKeyProductionOld))
            {
                pExtruder->Step = S$EXTRUDER_PRODUCTION_ZONE;
            }
            break;
		
        case S$EXTRUDER_PRODUCTION_ZONE:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 1;
            
            pSpeedUpDwData->maxVal = 100;
			
				pSpeedUpDwData->pSpeed = &pExtrPara->ProductionSpeed;
            SpeedUpDw(1   , pExtruder->bKeyInc , pSpeedUpDwData);
            SpeedUpDw(- 1 , pExtruder->bKeyDec , pSpeedUpDwData);
				pExtruder->ProductionSpeed = pExtrPara->ProductionSpeed;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->doFeedingCylFw = 1;
            pExtruder->Out = pExtruder->ProductionSpeed;
            
            if(1 == EDGEPOS(pExtruder->bKeyBasic,pExtruder->bKeyBasicOld))
            {
                pExtruder->Step = S$EXTRUDER_BASIC_ZONE;
            }
            break;
		
        case S$EXTRUDER_DEC_ZONE:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1;
            pExtruder->doFeedingCylFw = 1;
            pExtruder->Out = pExtruder->BasicSpeed;
            
            if(pExtruder->ActSpeed <= pExtruder->BasicSpeed + 0.01)
            {
                pExtruder->Step = S$EXTRUDER_READY; 
            }
            break;
         
        /* power off step  */
        case S$EXTRUDER_POWER_OFF_DECEL:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = pExtruder->BasicSpeed;
            
            if(pExtruder->ActSpeed <= pExtruder->BasicSpeed + 0.01)
            {
                pExtruder->Step = S$EXTRUDER_POWER_OFF_PROTECT; 
            }      
            break;
        
        case S$EXTRUDER_POWER_OFF_PROTECT:
            pExtruder->tET_s = 0.0;
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doOn = 1; 
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = 0.0;

            if(0 == pExtruder->cntProtect)
            {
                pExtruder->Step = S$EXTRUDER_POWER_OFF_DELAY;
            }
            break;
        
        case S$EXTRUDER_POWER_OFF_DELAY:
            pExtruder->LedBaisic = 0;
            pExtruder->LedProduction = 0;
            
            pExtruder->doContactor = 1;
            pExtruder->doFeedingCylFw = 0;
            pExtruder->Out = 0.0;
            pExtruder->tET_s = pExtruder->tET_s + pExtruder->tTask;
            if(pExtruder->tET_s >= pExtruder->tOffDelay)
            {
	            pExtruder->tET_s = 0.0;
					if(0 == pExtruder->LedOn)
					{
						pExtruder->Step = S$EXTRUDER_WAIT;
					}
					else
					{
						pExtruder->Step = S$EXTRUDER_READY;
					}
            }
            break;
    }/* switch( pExtruder->Step) */
	
	
	 if(pLubricCheck->tSetCheckTime < 1.0)pLubricCheck->tSetCheckTime = 1.0;
	
	 if(1 == pLubricCheck->bEnable && (pExtruder->Step > S$EXTRUDER_READY && pExtruder->Step <S$EXTRUDER_DEC_ZONE))
	 {
		if(0 == pLubricCheck->diLubricCheck)
		{
			pLubricCheck->tActCheckTime = pLubricCheck->tActCheckTime + pExtruder->tTask;
			if(pLubricCheck->tActCheckTime >= pLubricCheck->tSetCheckTime * 10.0)pLubricCheck->tActCheckTime=  2.0 * pLubricCheck->tSetCheckTime;
			if(pLubricCheck->tActCheckTime >= pLubricCheck->tSetCheckTime)
			{
				pLubricCheck->bLubricError = 1;
				/*pLubricCheck->bEnable = 0;*/
			}
			else
			{
				pLubricCheck->bLubricError = 0;	
			}
			
			pLubricCheck->tCheckDly = 5.0;
		}
		else
		{
			pLubricCheck->bLubricError = 0;
			pLubricCheck->tActCheckTime = 0.0;
		}
	 }
	 else
	 {
		pLubricCheck->tActCheckTime = 0.0;
		if(pLubricCheck->tCheckDly > 0.0)pLubricCheck->tCheckDly= pLubricCheck->tCheckDly - pExtruder->tTask;
		if(pLubricCheck->tCheckDly < 0.0)pLubricCheck->tCheckDly= 0.0;
		if(0.0 == pLubricCheck->tCheckDly)pLubricCheck->bLubricError = 0;
	 }
	    
    pExtruder->doOnOld = pExtruder->doOn;
    pExtruder->bKeyBasicOld = pExtruder->bKeyBasic;
    pExtruder->bKeyProductionOld = pExtruder->bKeyProduction;
    pExtruder->bKeyFeedingCylFwOld = pExtruder->bKeyFeedingCylFw;
}/*end of function*/
