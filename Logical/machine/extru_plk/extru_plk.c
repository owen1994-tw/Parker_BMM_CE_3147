/*********************************************************************************
 * Copyright: B&R China Ningbo
 * Author:    wangz
 * Created:   May 29, 2017/10:52 AM 
 *********************************************************************************/ 


#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
** 					               include user files								          **
**************************************************************************************/
#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
#include "pic_id.h"

/**************************************************************************************
** 					 	                 define								                   **
**************************************************************************************/
#define     S$EXTRUDER_WAIT		            	0
#define     S$EXTRUDER_CONTACTOR_ON		      	10
#define     S$EXTRUDER_POWER_ON_DELAY		  	20
#define     S$EXTRUDER_POWER_ON		         	30
#define     S$EXTRUDER_READY		            100
#define     S$EXTRUDER_BASIC_ZONE		      	110
#define     S$EXTRUDER_PRODUCTION_ZONE	      	130
#define     S$EXTRUDER_DEC_ZONE		         	150
#define     S$EXTRUDER_POWER_OFF_DECEL		   	200
#define     S$EXTRUDER_POWER_OFF_PROTECT	   	210
#define     S$EXTRUDER_POWER_OFF_DELAY		   	220
/**************************************************************************************
                               Function Declarations
**************************************************************************************/
void SpeedUpDw(short ins_des,short command,speedAdjust_typ * pSpeedUpDwData);

void ExtruderCtrl(Extruder_Ctrl_typ * pExtruder,Extruder_Para_typ * pExtrPara,speedAdjust_typ * pSpeedUpDwData);

/**************************************************************************************
     	        	       I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT extru_plkINIT(void)
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

	MotorSpeedA.maxVal = 1000;
	MotorSpeedA.minVal = 0;

	MotorSpeedB.minVal  	 = 0;
	MotorSpeedB.frequence	 = 30;
	MotorSpeedB.waitTimeSlow = 200;
	MotorSpeedB.waitTimeFast = 500;
	MotorSpeedB.unitsSlow    = 1;
	MotorSpeedB.unitsFast	 = 5;
	MotorSpeedB.unitsVFast	 = 20;

	MotorSpeedB.maxVal = 1000;
	MotorSpeedB.minVal = 0;

	MotorSpeedVisu.frequence	= 30;
	MotorSpeedVisu.waitTimeSlow = 200;
	MotorSpeedVisu.waitTimeFast = 500;
	MotorSpeedVisu.unitsSlow    = 1;
	MotorSpeedVisu.unitsFast	= 5;
	MotorSpeedVisu.unitsVFast	= 20;

	MotorSpeedVisu.maxVal = 1000;
	MotorSpeedVisu.minVal = 0;
    
	cntExtruderStopDelay = 0;

	gExtruderA.Step = S$EXTRUDER_WAIT;
	gExtruderB.Step = S$EXTRUDER_WAIT;
	gExtruderVisu.Step = S$EXTRUDER_WAIT;
    
	gExtruderA.tTask  = tTask;
	gExtruderB.tTask  = tTask;
	gExtruderVisu.tTask = tTask; 
	
	bHideExtrB = 0;
	bHideExtrV = 1;
	
	
	
	
	/*Ramp Function*/
	
	gExtruderACtrl.InverterFub.fnLCRRamp.enable = 1;
	gExtruderACtrl.InverterFub.fnLCRRamp.dy_up = 100.0;
	gExtruderACtrl.InverterFub.fnLCRRamp.dy_down = 200.0;
	gExtruderACtrl.InverterFub.fnLCRRamp.y_max = 20000.0;
	gExtruderACtrl.InverterFub.fnLCRRamp.y_min = -20000.0;
	gExtruderACtrl.InverterFub.fnLCRRamp.set = 0.0;
	gExtruderACtrl.InverterFub.fnLCRRamp.y_set = 0;
	
	
	
	/*Albert, give set speed value*/
//	gExtruderACtrl.SetSpeed =	ExtruderA_Retain.SetExtrSpeed * ExtruderA_Retain.Pro;
//	gExtruderBCtrl.SetSpeed =	ExtruderB_Retain.SetExtrSpeed * ExtruderB_Retain.Pro;
//	gExtruderCCtrl.SetSpeed =	ExtruderC_Retain.SetExtrSpeed * ExtruderC_Retain.Pro;
//	gExtruderVCtrl.SetSpeed =	ExtruderV_Retain.SetExtrSpeed * ExtruderV_Retain.Pro;
	
}
/**************************************************************************************
        				   C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _CYCLIC extru_plkCYCLIC(void)
{
	/*Command All control*/
	
	if(EnableAll_HMI)
	{
		EnableA_HMI = 1;
		EnableB_HMI = 1;
		EnableC_HMI = 1;
		EnableV_HMI = 1;
		EnableAll_HMI = 0;
	}
	
	if(1==DisableAll_HMI || 1==gMachineInfo.CycleTimeCounReach)
	{
		EnableA_HMI = 0;
		EnableB_HMI = 0;
		EnableC_HMI = 0;
		EnableV_HMI = 0;
		DisableAll_HMI = 0;
		gMachineInfo.CycleTimeCounReach=0;
	}
	

	if ( 0==gExtruderACtrl.InverterCmd.bSwitchOn)	//ipis0213
	{
		if (1== EnableA_HMI)
		{
			gImply.ExtruderA_NotOn =1;
			EnableA_HMI=0;
		} 
	}
	if ( 0==gExtruderBCtrl.InverterCmd.bSwitchOn)	//ipis0213
	{
		if (1== EnableB_HMI)
		{
			gImply.ExtruderB_NotOn =1;
			EnableB_HMI=0;
		} 
	}
	if ( 0==gExtruderCCtrl.InverterCmd.bSwitchOn)	//ipis0213
	{
		if (1== EnableC_HMI)
		{
			gImply.ExtruderC_NotOn =1;
			EnableC_HMI=0;
		} 
	}
	if ( 0==gExtruderVCtrl.InverterCmd.bSwitchOn)	//ipis0213
	{
		if (1== EnableV_HMI)
		{
			gImply.ExtruderV_NotOn =1;
			EnableV_HMI=0;
		} 
	}

	
	
	/*--------------------------------------------------------------*/
	/*----------------------Extruder A------------------------------*/
	/*--------------------------------------------------------------*/
//	if(gExtruderACtrl.InverterStatus.PowerOnReady == 1)
//	{
		gMachineOut.ExtrA_Power = gExtruderACtrl.InverterCmd.bSwitchOn;				// For extruder A fan

	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_A_PLK],PKeyMatric[S$KEYMATRIX_EXTRUDER_A_PLK]))
		{
		if (0==gExtruderTempLow_HMIFlag)  //ipis0719
		{
			if (0==gMachineInfo.bLoweringFlag)
			{
				gImply.TempLow = gMachineInfo.ExtrTLow;
				gImply.HeaterPowerNotOn = !gMachineOut.HeatPower;	
				gImply.HeatNotOn=!gT_Par.GrpSwh.OnOff[0];
				gMachineOut.ExtrA_Enable =!gMachineOut.ExtrA_Enable;

				gExtruderACtrl.InverterCmd.bEnableCtrl = !gExtruderACtrl.InverterCmd.bEnableCtrl; //ipis1214		
				gExtruderACtrl.InverterCmd.bSwitchOn = !gExtruderACtrl.InverterCmd.bSwitchOn;

			}
			else
			{
				gImply.TempLoweringEnable =1;	
			}
		}
		else
		{
		gImply.TempLow =1;	
			
		}		
	}

		
		if (gMachineInfo.ExtrTLow 	 || gAlarm.TempException	|| /*memcmp(&gAlarm.LAlarm, &null, sizeof(gAlarm.LAlarm)) != 0 ||*/
			/*gAlarm.AlarmExtrMeltP 	 ||*/ /*gAlarm.WarnExtrMeltP 	||*/
			gAlarm.A_BarrelCoolFault || gAlarm.A_ExtrCoolFault 		|| gAlarm.ThermocoupleBroken ||
			/*gAlarm.CarriageAxisError ||*/ /*gAlarm.LCarriageLinearBreak ||*/ /*gAlarm.LClampLinearBreak ||*/ /*gAlarm.ParisonLinearBreak ||*/
			gAlarm.EmergencyStop || (0 == gMachineOut.HeatPower) /*||  (0 == gMachineInfo.IsHeat)*/ /*|| 0==gAlarm.A_ExtrMotorOverload*/|| 0==gT_Par.GrpSwh.OnOff[0] )
		{
			EnableA_HMI = 0;
		gMachineOut.ExtrA_Enable=0;
		//	gExtruderACtrl.InverterCmd.bSwitchOn = 0;
		gExtruderACtrl.InverterCmd.bEnableCtrl = 0; //ipis1214		
		gExtruderACtrl.InverterCmd.bSwitchOn = 0;
		}

//		if(gExtruderACtrl.InverterStatus.bSwitchOn == 1)
//		{
///////////////////////////////////////////////////////////////////////////	ipis 1214
//			if(EnableA_HMI == 1)
//			{
//				gExtruderACtrl.InverterCmd.bEnableCtrl = 1;		
//			gExtruderACtrl.InverterCmd.bSwitchOn = 1;//
//		
//			}
//			else
//			{
//				gExtruderACtrl.InverterCmd.bEnableCtrl = 0;
//		gExtruderACtrl.InverterCmd.bSwitchOn = 0;//
//			}
/////////////////////////////////////////////////////////////////////////////////////	
//		}
//		else
//		{	
//			EnableA_HMI = 0;
//			gExtruderACtrl.InverterCmd.bEnableCtrl  = 0;
//		}
//	}
	if (0==gMachineOut.ExtrA_Enable)
	{
		EnableA_HMI = 0;
	}
	if(gExtruderACtrl.InverterStatus.bSwitchOn == 1)
	{
		gAlarm.A_ExtrMotorOverload = gMachineIn.ExtrA_MotorErr;
		if (1==gAlarm.A_ExtrMotorOverload)
		{
			EnableA_HMI = 0;
			EnableB_HMI = 0;
			EnableC_HMI = 0;
			EnableV_HMI = 0;
			
		//	gExtruderACtrl.InverterCmd.bSwitchOn = 0;
			gMachineOut.ExtrA_Enable=0;
			gMachineOut.ExtrB_Enable=0;
			gMachineOut.ExtrC_Enable=0;
			gMachineOut.ExtrV_Enable=0;
		}	
	}
	
	/*--------------------------------------------------------------*/
	/*----------------------Extruder B------------------------------*/
	/*--------------------------------------------------------------*/
//	if(gExtruderBCtrl.InverterStatus.PowerOnReady == 1)
//	{
		gMachineOut.ExtrB_Power = gExtruderBCtrl.InverterCmd.bSwitchOn;				// For extruder B fan

	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_B_PLK],PKeyMatric[S$KEYMATRIX_EXTRUDER_B_PLK]))
		{	
		if (0==gExtruderTempLow_HMIFlag) //ipis0719
		{
			if (0==gMachineInfo.bLoweringFlag)
			{
				gImply.TempLow = gMachineInfo.ExtrTLow;
				gImply.HeaterPowerNotOn = !gMachineOut.HeatPower;
				gImply.HeatNotOn=!gT_Par.GrpSwh.OnOff[0];
				gMachineOut.ExtrB_Enable =!gMachineOut.ExtrB_Enable;

				gExtruderBCtrl.InverterCmd.bEnableCtrl = !gExtruderBCtrl.InverterCmd.bEnableCtrl;  //ipis1214
				gExtruderBCtrl.InverterCmd.bSwitchOn	=!gExtruderBCtrl.InverterCmd.bSwitchOn;
			}
			else
			{
				gImply.TempLoweringEnable =1; 
			}
		}
		else
		{
			gImply.TempLow =1;	
			
		}
			
					
	}
		
		if (gMachineInfo.ExtrTLow 	 || gAlarm.TempException	|| /*memcmp(&gAlarm.LAlarm, &null, sizeof(gAlarm.LAlarm)) != 0 ||*/
			/*gAlarm.AlarmExtrMeltP 	 ||*/ /*gAlarm.WarnExtrMeltP 	||*/
			gAlarm.B_BarrelCoolFault || gAlarm.B_ExtrCoolFault 		|| gAlarm.ThermocoupleBroken ||
			/*gAlarm.CarriageAxisError ||*/ /*gAlarm.LCarriageLinearBreak ||*/ /*gAlarm.LClampLinearBreak ||*/ /*gAlarm.ParisonLinearBreak ||*/
			gAlarm.EmergencyStop || (0 == gMachineOut.HeatPower) /*||  (0 == gMachineInfo.IsHeat)*/ /*|| 0==gAlarm.B_ExtrMotorOverload*/|| 0==gT_Par.GrpSwh.OnOff[0] )
		{
			EnableB_HMI = 0;
		gMachineOut.ExtrB_Enable=0;
		gExtruderBCtrl.InverterCmd.bEnableCtrl = 0;  //ipis1214
		gExtruderBCtrl.InverterCmd.bSwitchOn	=0;
		}

	//	if(gExtruderBCtrl.InverterStatus.bSwitchOn == 1)
	//	{
/////////////////////////////////////////////////////////////////////////////////////	
//			if(EnableB_HMI == 1)
//			{
//				gExtruderBCtrl.InverterCmd.bEnableCtrl = 1;
//		gExtruderBCtrl.InverterCmd.bSwitchOn=1;
//			}
//			else
//			{
//				gExtruderBCtrl.InverterCmd.bEnableCtrl = 0;
//		gExtruderBCtrl.InverterCmd.bSwitchOn=0;
//			}
////////////////////////////////////////////////////////////////////////////////////////////	
	//	}
	//	else
	//	{	
	//		EnableB_HMI = 0;
	//		gExtruderBCtrl.InverterCmd.bEnableCtrl  = 0;
	//	}
//	}
	if (0==gMachineOut.ExtrB_Enable)
	{
		EnableB_HMI = 0;
	}
	if(gExtruderBCtrl.InverterStatus.bSwitchOn == 1)
	{
		gAlarm.B_ExtrMotorOverload = gMachineIn.ExtrB_MotorErr;

		if (1==gAlarm.B_ExtrMotorOverload)
		{
		//	EnableB_HMI = 0;
			EnableA_HMI = 0;
			EnableB_HMI = 0;
			EnableC_HMI = 0;
			EnableV_HMI = 0;
		//	gExtruderBCtrl.InverterCmd.bSwitchOn = 0;
		//	gMachineOut.ExtrB_Enable=0;
			gMachineOut.ExtrA_Enable=0;
			gMachineOut.ExtrB_Enable=0;
			gMachineOut.ExtrC_Enable=0;
			gMachineOut.ExtrV_Enable=0;
		}	
	}
	
	/*--------------------------------------------------------------*/
	/*----------------------Extruder C------------------------------*/
	/*--------------------------------------------------------------*/
//	if(gExtruderCCtrl.InverterStatus.PowerOnReady == 1)
//	{
		gMachineOut.ExtrC_Power = gExtruderCCtrl.InverterCmd.bSwitchOn;				// For extruder C fan

	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_C_PLK],PKeyMatric[S$KEYMATRIX_EXTRUDER_C_PLK]))
		{	
		if (0==gExtruderTempLow_HMIFlag)  //ipis0719
		{
			if (0==gMachineInfo.bLoweringFlag)
			{
				gImply.TempLow = gMachineInfo.ExtrTLow;
				gImply.HeaterPowerNotOn = !gMachineOut.HeatPower;
				gImply.HeatNotOn=!gT_Par.GrpSwh.OnOff[0];
				gMachineOut.ExtrC_Enable =!gMachineOut.ExtrC_Enable;
	
				gExtruderCCtrl.InverterCmd.bEnableCtrl 	=	!gExtruderCCtrl.InverterCmd.bEnableCtrl;
				gExtruderCCtrl.InverterCmd.bSwitchOn	=	!gExtruderCCtrl.InverterCmd.bSwitchOn;
			
			}
			else
			{
				gImply.TempLoweringEnable =1; 
			}	
		}
		else
		{
			gImply.TempLow =1;	
			
		}
					
	}
		
		if (gMachineInfo.ExtrTLow 	 || gAlarm.TempException	|| /*memcmp(&gAlarm.LAlarm, &null, sizeof(gAlarm.LAlarm)) != 0 ||*/
			/*gAlarm.AlarmExtrMeltP 	 ||*/ /*gAlarm.WarnExtrMeltP 	||*/
			gAlarm.C_BarrelCoolFault || gAlarm.C_ExtrCoolFault 	 	|| gAlarm.ThermocoupleBroken ||
			/*gAlarm.CarriageAxisError ||*/ /*gAlarm.LCarriageLinearBreak ||*//* gAlarm.LClampLinearBreak ||*/ /*gAlarm.ParisonLinearBreak ||*/
			gAlarm.EmergencyStop || (0 == gMachineOut.HeatPower) /*||  (0 == gMachineInfo.IsHeat)*/ /*|| 0==gAlarm.C_ExtrMotorOverload*/ || 0==gT_Par.GrpSwh.OnOff[0])
		{
			EnableC_HMI = 0;
		gMachineOut.ExtrC_Enable=0;

		gExtruderCCtrl.InverterCmd.bEnableCtrl = 0;
		gExtruderCCtrl.InverterCmd.bSwitchOn=0;
		}
	//	if(gExtruderCCtrl.InverterStatus.bSwitchOn == 1)
	//	{
////////////////////////////////////////////////////////////////////////////////////	
//			if(EnableC_HMI == 1)
//			{
//				gExtruderCCtrl.InverterCmd.bEnableCtrl = 1;
//		gExtruderCCtrl.InverterCmd.bSwitchOn=1;
//			}
//			else
//			{
//				gExtruderCCtrl.InverterCmd.bEnableCtrl = 0;
//		gExtruderCCtrl.InverterCmd.bSwitchOn=0;
//			}
/////////////////////////////////////////////////////////////////////////////////////	
	//	}
	//	else
	//	{	
	//		EnableC_HMI = 0;
	//		gExtruderCCtrl.InverterCmd.bEnableCtrl  = 0;
	//	}
//	}	
	if (0==gMachineOut.ExtrC_Enable)
	{
		EnableC_HMI = 0;
	}
	if(gExtruderCCtrl.InverterStatus.bSwitchOn == 1)
	{
		gAlarm.C_ExtrMotorOverload = gMachineIn.ExtrC_MotorErr;

		if (1==gAlarm.C_ExtrMotorOverload)
		{
		//	EnableC_HMI = 0;
			EnableA_HMI = 0;
			EnableB_HMI = 0;
			EnableC_HMI = 0;
			EnableV_HMI = 0;
		//	gExtruderCCtrl.InverterCmd.bSwitchOn = 0;
		//	gMachineOut.ExtrC_Enable=0;
			gMachineOut.ExtrA_Enable=0;
			gMachineOut.ExtrB_Enable=0;
			gMachineOut.ExtrC_Enable=0;
			gMachineOut.ExtrV_Enable=0;
		}	
	}
	
	/*--------------------------------------------------------------*/
	/*----------------------Extruder V------------------------------*/
	/*--------------------------------------------------------------*/
//	if(gExtruderVCtrl.InverterStatus.PowerOnReady == 1)
//	{

		gMachineOut.VisuExtrPower = gExtruderVCtrl.InverterCmd.bSwitchOn;				// For extruder V fan

	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_VISU_EXTRUDER_PLK],PKeyMatric[S$KEYMATRIX_VISU_EXTRUDER_PLK]))
		{	
		if (0==gExtruderTempLow_HMIFlag) //ipis0213
		{
			if (0==gMachineInfo.bLoweringFlag)
			{
				gImply.TempLow = gMachineInfo.ExtrTLow;
				gImply.HeaterPowerNotOn = !gMachineOut.HeatPower;
				gImply.HeatNotOn=!gT_Par.GrpSwh.OnOff[0];
				gMachineOut.ExtrV_Enable =!gMachineOut.ExtrV_Enable;
				//	gExtruderVCtrl.InverterCmd.bSwitchOn = !gExtruderVCtrl.InverterCmd.bSwitchOn;
				gExtruderVCtrl.InverterCmd.bEnableCtrl 	=	!gExtruderVCtrl.InverterCmd.bEnableCtrl;
				gExtruderVCtrl.InverterCmd.bSwitchOn	=	!gExtruderVCtrl.InverterCmd.bSwitchOn;
			}
			else
			{
				gImply.TempLoweringEnable =1; 
			}
		}
		else
		{
			gImply.TempLow =1;	
			
		}	
			
	}
		
		if (gMachineInfo.ExtrTLow 	 || gAlarm.TempException	|| /*memcmp(&gAlarm.LAlarm, &null, sizeof(gAlarm.LAlarm)) != 0 ||*/
			/*gAlarm.AlarmExtrMeltP 	 ||*/ /*gAlarm.WarnExtrMeltP 	||*/
			gAlarm.V_BarrelCoolFault || gAlarm.V_ExtrCoolFault 		|| gAlarm.ThermocoupleBroken ||
			/*gAlarm.CarriageAxisError ||*/ /*gAlarm.LCarriageLinearBreak || gAlarm.LClampLinearBreak ||*/ /*gAlarm.ParisonLinearBreak ||*/
			gAlarm.EmergencyStop || (0 == gMachineOut.HeatPower) /*||  (0 == gMachineInfo.IsHeat)*/ /*|| 0==gAlarm.V_ExtrMotorOverload*/ || 0==gT_Par.GrpSwh.OnOff[0] )
		{
			EnableV_HMI = 0;
		gMachineOut.ExtrV_Enable=0;
	
		gExtruderVCtrl.InverterCmd.bEnableCtrl = 0;
		gExtruderVCtrl.InverterCmd.bSwitchOn=0;
		}

	//	if(gExtruderVCtrl.InverterStatus.bSwitchOn == 1)
	//	{
/////////////////////////////////////////////////////////////////////////////////	
//			if(EnableV_HMI == 1)
//			{
//				gExtruderVCtrl.InverterCmd.bEnableCtrl = 1;
//		gExtruderVCtrl.InverterCmd.bSwitchOn=1;
//			}
//			else
//			{
//				gExtruderVCtrl.InverterCmd.bEnableCtrl = 0;
//		gExtruderVCtrl.InverterCmd.bSwitchOn=0;
//			}
///////////////////////////////////////////////////////////////////////////////////	
	//	}
	//	else
	//	{	
	//		EnableV_HMI = 0;
	//		gExtruderVCtrl.InverterCmd.bEnableCtrl  = 0;
	//	}
//	}	
	if (0==gMachineOut.ExtrV_Enable)
	{
		EnableV_HMI = 0;
	}
	if(gExtruderVCtrl.InverterStatus.bSwitchOn == 1)
	{
		gAlarm.V_ExtrMotorOverload = gMachineIn.ExtrV_MotorErr;

		if (1==gAlarm.V_ExtrMotorOverload)
		{
		//	EnableV_HMI = 0;
			EnableA_HMI = 0;
			EnableB_HMI = 0;
			EnableC_HMI = 0;
			EnableV_HMI = 0;
		//	gExtruderVCtrl.InverterCmd.bSwitchOn = 0;
		//	gMachineOut.ExtrV_Enable=0;
			gMachineOut.ExtrA_Enable=0;
			gMachineOut.ExtrB_Enable=0;
			gMachineOut.ExtrC_Enable=0;
			gMachineOut.ExtrV_Enable=0;
		}	
	}
	// Parison pump is followed by Extruder A //
//	if (gExtruderACtrl.InverterStatus.bSwitchOn)  //Chao-i 20180402
//	{
//		if (gMachineInfo.ExtrTLow == 0)
//		{ 
//			uParisonPumpEn = 1;
//		}
//		else
//		{
//			uParisonPumpEn = 0;
////			gMachineOut.ParisonPump = 0;
//		}
//	}
//	else
//	{
//		uParisonPumpEn = 0;
////		gMachineOut.ParisonPump = 0;
//	}	
	
	if (ExtruderA_Retain.Pro == 0)
	{
		ExtruderA_Retain.Pro = 1;
	}
	if (ExtruderB_Retain.Pro == 0)
	{
		ExtruderB_Retain.Pro = 1;
	}
	if (ExtruderC_Retain.Pro == 0)
	{
		ExtruderC_Retain.Pro = 1;
	}
	if (ExtruderV_Retain.Pro == 0)
	{
		ExtruderV_Retain.Pro = 1;
	}
	for(Index=0 ; Index<3 ; Index++)
	{
		if (ExtruderA_Retain.AccTime[Index] == 0)
		{
			ExtruderA_Retain.AccTime[Index] = 1.0;
		}
		if (ExtruderB_Retain.AccTime[Index] == 0)
		{
			ExtruderB_Retain.AccTime[Index] = 1.0;
		}
		if (ExtruderC_Retain.AccTime[Index] == 0)
		{
			ExtruderC_Retain.AccTime[Index] = 1.0;
		}
		if (ExtruderV_Retain.AccTime[Index] == 0)
		{
			ExtruderV_Retain.AccTime[Index] = 1.0;
		}
	}

	ExtrASpeedLmt					= (int)MotorSpeedLmtA / ExtruderA_Retain.Pro;
	ExtrBSpeedLmt					= (int)MotorSpeedLmtB / ExtruderB_Retain.Pro;
	ExtrCSpeedLmt					= (int)MotorSpeedLmtC / ExtruderC_Retain.Pro;
	ExtrVSpeedLmt					= (int)MotorSpeedLmtV / ExtruderV_Retain.Pro;
	ExtruderA_Retain.ActExtrSpeed 	= (int)(ExtruderA_Retain.ActMotorSpeed / ExtruderA_Retain.Pro);
	ExtruderB_Retain.ActExtrSpeed 	= (int)(ExtruderB_Retain.ActMotorSpeed / ExtruderB_Retain.Pro);
	ExtruderC_Retain.ActExtrSpeed 	= (int)(ExtruderC_Retain.ActMotorSpeed / ExtruderC_Retain.Pro);
	ExtruderV_Retain.ActExtrSpeed 	= (int)(ExtruderV_Retain.ActMotorSpeed / ExtruderV_Retain.Pro);
	for(Index=0 ; Index<3 ; Index++)
	{
		ExtruderA_Retain.Acc[Index]		= (700.0)/ExtruderA_Retain.AccTime[Index]*tTask;//*ExtruderA_Retain.Pro;
		ExtruderB_Retain.Acc[Index]		= (700.0)/ExtruderB_Retain.AccTime[Index]*tTask;//*ExtruderB_Retain.Pro;
		ExtruderC_Retain.Acc[Index]		= (700.0)/ExtruderC_Retain.AccTime[Index]*tTask;//*ExtruderA_Retain.Pro;
		ExtruderV_Retain.Acc[Index]		= (700.0)/ExtruderV_Retain.AccTime[Index]*tTask;//*ExtruderB_Retain.Pro;
		
	}
	
	/*Exturder A*/
	/*if(ExtruderA_Retain.OperateEnable == 1)*/
//	if(1 == gExtruderACtrl.InverterCmd.bEnableCtrl)
//	{
//		if(gExtruderACtrl.SetSpeed < (ExtruderA_Retain.SetExtrSpeed * ExtruderA_Retain.Pro)-0.5)
//		{
//			if(gExtruderACtrl.SetSpeed <= 700*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL += ExtruderA_Retain.Acc[0];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderACtrl.SetSpeed > 700*ExtruderA_Retain.Pro && gExtruderACtrl.SetSpeed <= 1400*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL += ExtruderA_Retain.Acc[1];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderACtrl.SetSpeed > 1400*ExtruderA_Retain.Pro && gExtruderACtrl.SetSpeed <= 2100*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL += ExtruderA_Retain.Acc[2];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//		}
//		else if(gExtruderACtrl.SetSpeed > (ExtruderA_Retain.SetExtrSpeed * ExtruderA_Retain.Pro)+0.5)	
//		{
//			if(gExtruderACtrl.SetSpeed <= 700*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL -= ExtruderA_Retain.Acc[0];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderACtrl.SetSpeed > 700*ExtruderA_Retain.Pro && gExtruderACtrl.SetSpeed <= 1400*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL -= ExtruderA_Retain.Acc[1];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderACtrl.SetSpeed > 1400*ExtruderA_Retain.Pro && gExtruderACtrl.SetSpeed <= 2100*ExtruderA_Retain.Pro)
//			{
//				ExtruderA_Retain.SetMotorSpeedREAL -= ExtruderA_Retain.Acc[2];
//				gExtruderACtrl.SetSpeed = (int)ExtruderA_Retain.SetMotorSpeedREAL;
//			}
//		}	
//	}
//	else
//	{
//		ExtruderA_Retain.SetMotorSpeedREAL = 0;
//		gExtruderACtrl.SetSpeed = 0;
//	}
		
	

	
	/*Exturder B*/
//	if(1 == gExtruderBCtrl.InverterCmd.bEnableCtrl)
//	{
//		if(gExtruderBCtrl.SetSpeed < (ExtruderB_Retain.SetExtrSpeed * ExtruderB_Retain.Pro)-0.5)
//		{
//			if(gExtruderBCtrl.SetSpeed <= 700*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL += ExtruderB_Retain.Acc[0];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderBCtrl.SetSpeed > 700*ExtruderB_Retain.Pro && gExtruderBCtrl.SetSpeed <= 1400*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL += ExtruderB_Retain.Acc[1];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderBCtrl.SetSpeed > 1400*ExtruderB_Retain.Pro && gExtruderBCtrl.SetSpeed <= 2100*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL += ExtruderB_Retain.Acc[2];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//		}
//		else if(gExtruderBCtrl.SetSpeed > (ExtruderB_Retain.SetExtrSpeed * ExtruderB_Retain.Pro)+0.5)	
//		{
//			if(gExtruderBCtrl.SetSpeed <= 700*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL -= ExtruderB_Retain.Acc[0];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderBCtrl.SetSpeed > 700*ExtruderB_Retain.Pro && gExtruderBCtrl.SetSpeed <= 1400*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL -= ExtruderB_Retain.Acc[1];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderBCtrl.SetSpeed > 1400*ExtruderB_Retain.Pro && gExtruderBCtrl.SetSpeed <= 2100*ExtruderB_Retain.Pro)
//			{
//				ExtruderB_Retain.SetMotorSpeedREAL -= ExtruderB_Retain.Acc[2];
//				gExtruderBCtrl.SetSpeed = (int)ExtruderB_Retain.SetMotorSpeedREAL;
//			}
//		}	
//	}
//	else
//	{
//		ExtruderB_Retain.SetMotorSpeedREAL = 0;
//		gExtruderBCtrl.SetSpeed = 0;
//	}	
			

	/*Exturder C*/
//	if(1 == gExtruderCCtrl.InverterCmd.bEnableCtrl)
//	{
//		if(gExtruderCCtrl.SetSpeed < (ExtruderC_Retain.SetExtrSpeed * ExtruderC_Retain.Pro)-0.5)
//		{
//			if(gExtruderCCtrl.SetSpeed <= 700*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL += ExtruderC_Retain.Acc[0];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderCCtrl.SetSpeed > 700*ExtruderC_Retain.Pro && gExtruderCCtrl.SetSpeed <= 1400*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL += ExtruderC_Retain.Acc[1];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderCCtrl.SetSpeed > 1400*ExtruderC_Retain.Pro && gExtruderCCtrl.SetSpeed <= 2100*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL += ExtruderC_Retain.Acc[2];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//		}
//		else if(gExtruderCCtrl.SetSpeed > (ExtruderC_Retain.SetExtrSpeed * ExtruderC_Retain.Pro)+0.5)	
//		{
//			if(gExtruderCCtrl.SetSpeed <= 700*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL -= ExtruderC_Retain.Acc[0];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderCCtrl.SetSpeed > 700*ExtruderC_Retain.Pro && gExtruderCCtrl.SetSpeed <= 1400*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL -= ExtruderC_Retain.Acc[1];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderCCtrl.SetSpeed > 1400*ExtruderC_Retain.Pro && gExtruderCCtrl.SetSpeed <= 2100*ExtruderC_Retain.Pro)
//			{
//				ExtruderC_Retain.SetMotorSpeedREAL -= ExtruderC_Retain.Acc[2];
//				gExtruderCCtrl.SetSpeed = (int)ExtruderC_Retain.SetMotorSpeedREAL;
//			}
//		}	
//	}
//	else
//	{
//		ExtruderC_Retain.SetMotorSpeedREAL = 0;
//		gExtruderCCtrl.SetSpeed = 0;
//	}
	
	
	/*Exturder V*/
//	if(1 == gExtruderVCtrl.InverterCmd.bEnableCtrl)
//	{
//		if(gExtruderVCtrl.SetSpeed < (ExtruderV_Retain.SetExtrSpeed * ExtruderV_Retain.Pro)-0.5)
//		{
//			if(gExtruderVCtrl.SetSpeed <= 700*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL += ExtruderV_Retain.Acc[0];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderVCtrl.SetSpeed > 700*ExtruderV_Retain.Pro && gExtruderVCtrl.SetSpeed <= 1400*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL += ExtruderV_Retain.Acc[1];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderVCtrl.SetSpeed > 1400*ExtruderV_Retain.Pro && gExtruderVCtrl.SetSpeed <= 2100*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL += ExtruderV_Retain.Acc[2];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//		}
//		else if(gExtruderVCtrl.SetSpeed > (ExtruderV_Retain.SetExtrSpeed * ExtruderV_Retain.Pro)+0.5)	
//		{
//			if(gExtruderVCtrl.SetSpeed <= 700*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL -= ExtruderV_Retain.Acc[0];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderVCtrl.SetSpeed > 700*ExtruderV_Retain.Pro && gExtruderVCtrl.SetSpeed <= 1400*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL -= ExtruderV_Retain.Acc[1];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//			else if(gExtruderVCtrl.SetSpeed > 1400*ExtruderV_Retain.Pro && gExtruderVCtrl.SetSpeed <= 2100*ExtruderV_Retain.Pro)
//			{
//				ExtruderV_Retain.SetMotorSpeedREAL -= ExtruderV_Retain.Acc[2];
//				gExtruderVCtrl.SetSpeed = (int)ExtruderV_Retain.SetMotorSpeedREAL;
//			}
//		}	
//	}
//	else
//	{
//		ExtruderV_Retain.SetMotorSpeedREAL = 0;
//		gExtruderVCtrl.SetSpeed = 0;
//	}
	
	

	
	/*Exturder A*/
	if((1 == gExtruderACtrl.InverterCmd.bEnableCtrl) && (1==EnableA_HMI))
	{	
//		ExtruderA_Retain.SetExtrSpeed * ExtruderA_Retain.Pro
		
//		gExtruderACtrl.SetSpeed =	ExtruderA_Retain.SetExtrSpeed * ExtruderA_Retain.Pro;
//		gExtruderACtrl.SetSpeed =	ExtruderA_Retain.SetExtrSpeed;/*Change to the motor speed*/ //ipis0423
		gExtruderACtrl.SetSpeed =gUserPara.SetExtrSpeed_A;/*Change to the motor speed*/ //ipis0423
	}
	else
	{
		gExtruderACtrl.SetSpeed = 0;
	}
	
	
	
	/*Exturder B*/
	if(1 == gExtruderBCtrl.InverterCmd.bEnableCtrl && (1==EnableB_HMI))
	{		
//		gExtruderBCtrl.SetSpeed =	ExtruderB_Retain.SetExtrSpeed * ExtruderB_Retain.Pro;
//		gExtruderBCtrl.SetSpeed =	ExtruderB_Retain.SetExtrSpeed;/*Change to the motor speed*/
		gExtruderBCtrl.SetSpeed =gUserPara.SetExtrSpeed_B;/*Change to the motor speed*/ //ipis0423
	}
	else
	{
		gExtruderBCtrl.SetSpeed = 0;
	}
	
	/*Exturder C*/
	if(1 == gExtruderCCtrl.InverterCmd.bEnableCtrl && (1==EnableC_HMI))
	{		
//		gExtruderCCtrl.SetSpeed =	ExtruderC_Retain.SetExtrSpeed * ExtruderC_Retain.Pro;
//		gExtruderCCtrl.SetSpeed =	ExtruderC_Retain.SetExtrSpeed;
		gExtruderCCtrl.SetSpeed =gUserPara.SetExtrSpeed_C;/*Change to the motor speed*/ //ipis0423
	}
	else
	{
		gExtruderCCtrl.SetSpeed = 0;
	}
	
	/*Albert Add color speed depend on the extruder speed*/
	rCExtruderSpeed = gExtruderCCtrl.SetSpeed;
		
	if(ExtrCSpeedLmt != 0)
		gExtruderCCtrl.aoSetSpeed = (INT)(rCExtruderSpeed/ExtrCSpeedLmt * 32767);
	
	
	/*Exturder V*/
	if(1 == gExtruderVCtrl.InverterCmd.bEnableCtrl && (1==EnableV_HMI))
	{		
//		gExtruderVCtrl.SetSpeed =	ExtruderV_Retain.SetExtrSpeed * ExtruderV_Retain.Pro;
//		gExtruderVCtrl.SetSpeed =	ExtruderV_Retain.SetExtrSpeed;
		gExtruderVCtrl.SetSpeed =gUserPara.SetExtrSpeed_V;/*Change to the motor speed*/ //ipis0423
	}
	else
	{
		gExtruderVCtrl.SetSpeed = 0;
	}
	
	if(ExtruderA_Retain.Pro != 0)
		gExtruderACtrl.rActRPM = gExtruderACtrl.SetSpeed/ExtruderA_Retain.Pro;
	if(ExtruderB_Retain.Pro != 0)
		gExtruderBCtrl.rActRPM = gExtruderBCtrl.SetSpeed/ExtruderB_Retain.Pro;
	if(ExtruderC_Retain.Pro != 0)
		gExtruderCCtrl.rActRPM = gExtruderCCtrl.SetSpeed/ExtruderC_Retain.Pro;
	if(ExtruderV_Retain.Pro != 0)
		gExtruderVCtrl.rActRPM = gExtruderVCtrl.SetSpeed/ExtruderV_Retain.Pro;
//	if (ExtruderA_Retain.InverseHmi == 1)
//	{
//		gExtruderACtrl.InverterCmd.bInverse = 1;
//	}
//	else
//	{
//		gExtruderACtrl.InverterCmd.bInverse = 0;
//	}
//	if (ExtruderB_Retain.InverseHmi == 1)
//	{
//		ExtruderB_Retain.Inverse = 1;
//	}
//	else
//	{
//		ExtruderB_Retain.Inverse = 0;
//	}
//	if (ExtruderC_Retain.InverseHmi == 1)
//	{
//		ExtruderC_Retain.Inverse = 1;
//	}
//	else
//	{
//		ExtruderC_Retain.Inverse = 0;
//	}
//	
//	if (ExtruderV_Retain.InverseHmi == 1)
//	{
//		ExtruderV_Retain.Inverse = 1;
//	}
//	else
//	{
//		ExtruderV_Retain.Inverse = 0;
//	}
	
	// emergency stop 
	if (gExtruderACtrl.InverterStatus.ActTorque*0.1 >= ExtruderA_Retain.AlarmTorque)
	{
		//gAxis04_ModuleOk = 1;
//		gExtruderACtrl.InverterCmd.bAskStop = 1;
	}
	else
	{
		//gAxis04_ModuleOk = 0;
//		gExtruderACtrl.InverterCmd.bAskStop = 0;
	}
	
	//emergency stop
	if (ExtruderB_Retain.ActTorque*0.1 >= ExtruderB_Retain.AlarmTorque)
	{
		//gAxis05_ModuleOk = 1;
//		ExtruderB_Retain.FastStop = 1;
	}
	else
	{
		//gAxis05_ModuleOk = 0;
//		ExtruderB_Retain.FastStop = 0;
	}

	if (ExtruderC_Retain.ActTorque*0.1 >= ExtruderC_Retain.AlarmTorque)
	{
		//gAxis05_ModuleOk = 1;
//		ExtruderC_Retain.FastStop = 1;
	}
	else
	{
		//gAxis05_ModuleOk = 0;
//		ExtruderC_Retain.FastStop = 0;
	}
	
	if (ExtruderV_Retain.ActTorque*0.1 >= ExtruderV_Retain.AlarmTorque)
	{
		//gAxis05_ModuleOk = 1;
//		ExtruderV_Retain.FastStop = 1;
	}
	else
	{
		//gAxis05_ModuleOk = 0;
//		ExtruderV_Retain.FastStop = 0;
	}
	
	if (ExtruderA_Retain.AlarmTorque == 0)
	{
		ExtruderA_Retain.AlarmTorque = 90;
	}
	if (ExtruderB_Retain.AlarmTorque == 0)
	{
		ExtruderB_Retain.AlarmTorque = 90;
	}
	if (ExtruderC_Retain.AlarmTorque == 0)
	{
		ExtruderC_Retain.AlarmTorque = 90;
	}
	if (ExtruderV_Retain.AlarmTorque == 0)
	{
		ExtruderV_Retain.AlarmTorque = 90;
	}
	
	
	
	
	/*2018.7.31 Albert*/
	/*Limit Extruder Set Speed*/
	
//	if(ExtruderA_Retain.SetExtrSpeed > ExtrASpeedLmt) ExtruderA_Retain.SetExtrSpeed = ExtrASpeedLmt;
//	if(ExtruderB_Retain.SetExtrSpeed > ExtrBSpeedLmt) ExtruderB_Retain.SetExtrSpeed = ExtrBSpeedLmt;
//	if(ExtruderC_Retain.SetExtrSpeed > ExtrCSpeedLmt) ExtruderC_Retain.SetExtrSpeed = ExtrCSpeedLmt;
//	if(ExtruderV_Retain.SetExtrSpeed > ExtrVSpeedLmt) ExtruderV_Retain.SetExtrSpeed = ExtrVSpeedLmt;
	
	if(ExtruderA_Retain.SetExtrSpeed > MotorSpeedLmtA) ExtruderA_Retain.SetExtrSpeed = MotorSpeedLmtA;
	if(ExtruderB_Retain.SetExtrSpeed > MotorSpeedLmtB) ExtruderB_Retain.SetExtrSpeed = MotorSpeedLmtB;
	if(ExtruderC_Retain.SetExtrSpeed > MotorSpeedLmtC) ExtruderC_Retain.SetExtrSpeed = MotorSpeedLmtC;
	if(ExtruderV_Retain.SetExtrSpeed > MotorSpeedLmtV) ExtruderV_Retain.SetExtrSpeed = MotorSpeedLmtV;
	
	if(gUserPara.SetExtrSpeed_A > MotorSpeedLmtA) gUserPara.SetExtrSpeed_A = MotorSpeedLmtA;
	if(gUserPara.SetExtrSpeed_B > MotorSpeedLmtB) gUserPara.SetExtrSpeed_B = MotorSpeedLmtB;
	if(gUserPara.SetExtrSpeed_C > MotorSpeedLmtC) gUserPara.SetExtrSpeed_C = MotorSpeedLmtC;
	if(gUserPara.SetExtrSpeed_V > MotorSpeedLmtV) gUserPara.SetExtrSpeed_V = MotorSpeedLmtV;
	
	
	LCRRamp(&gExtruderACtrl.InverterFub.fnLCRRamp);
	
	
	

	
}
/**************************************************************************************
        				   E X I T   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _EXIT extru_plkEXIT(void)
{
	/* TODO: Add code here */
}

