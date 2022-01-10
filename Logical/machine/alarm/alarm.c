/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: gAlarm
 * File: gAlarm.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program gAlarm --- gAlarm and gImply control
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "defines.h"
#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/* gAlarm text display mode  */
#define	 S$ALARM_TEXT_TOP	      0
#define	 S$ALARM_TEXT_LIST	   1

/**************************************************************************************
** 			       function ALARM type -- warning or alarm  				  			    **
**************************************************************************************/
void AlarmTyp(Alarm_typ * pInst);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void alarm_init ( void )
{
	memset(AlarmBit,0,sizeof(AlarmBit));
	memset(OldPBit ,0,sizeof(OldPBit));
	memset(OldNBit ,0,sizeof(OldNBit));

	memset(&gImply,0,sizeof(gImply));
    
	AlarmCount = 0;
	TimeCounter  = 30;
	iFunction = 1;
   
	memset(&AlarmBand,1,sizeof(AlarmBand));
	AlarmTyp(&AlarmBand);

}

/**************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void alarm_cyclic ( void )
{
	int i,j;
	if(TimeCounter > 0)
	{
		memset(&gAlarm,0,sizeof(gAlarm));
		memset(&LMold.Alarm,0,sizeof(LMold.Alarm));
		memset(&RMold.Alarm,0,sizeof(RMold.Alarm));
      
		TimeCounter--;
		return;
	}
   
	pAlarmband = (USINT *)(&AlarmBand);
 
	gAlarm.batteryLow     = ap880Battery;
	gAlarm.EmergencyStop  = gMachineIn.InstancyStop;
	


	IsHeat = 0;
	for (i = 0; i<MAX_IDX_CONTRL_ZONES;i++)
	{
		if (1 == gT_Par.Ctl.OnOff[i])
		{
			IsHeat = 1;
			break;
		}
	}
   
	if(1 == gT_Par.Status.bLow)
	{
		gMachineInfo.ExtrTLow = 1; 
		if(1 == IsHeat)
		{
			gAlarm.ScrewTempLow = 1;  
		}
		else
		{
			gAlarm.ScrewTempLow = 0;  
		}
	}
	else
	{
		gMachineInfo.ExtrTLow = 0;  
		gAlarm.ScrewTempLow   = 0;
	}
   
	gAlarm.ScrewTempHigh	     = gT_Par.Status.bHigh;
	//	if (1==gAlarm.ScrewTempHigh)
	//	{
	//		gMachineOut.HeatPower=0;
	//	}
	
	if (1==gAlarm.HeatError)
	{
		gMachineOut.HeatPower=0;
	}
	
	gAlarm.TempFanErr		= gMachineIn.TempFanErr;
	gAlarm.ThermocoupleBroken = gT_Par.Status.bBroken;
	gAlarm.OilPumpFilter  = !gMachineIn.OilPumpFilter ;
	gAlarm.CoolingPumpFilter = !gMachineIn.CoolingPumpFilter;
	gAlarm.TonnageFilter = !gMachineIn.TonnagePumpFilter;
	gAlarm.PneuForceNormal = !gMachineIn.PneuForceNormal ;
	
	gAlarm.HeatingPowerNormal = !gMachineIn.HeatingPowerNormal;



	gAlarm.OilPumpErr = gMachineIn.OilPumpErr;
//	gAlarm.AirPressureLow = !gMachineIn.LMold.AirPressureLow || !gMachineIn.RMold.AirPressureLow;
	/*20180926 ipis*/
	gAlarm.OilPump1Overload = gMachineIn.OilPump1Overload;
	gAlarm.OilPump2Overload = gMachineIn.OilPump2Overload;
	gAlarm.ParisonMotorOverload = gMachineIn.ParisonPumpMotorErr;
	gAlarm.PlateMotorOverload = gMachineIn.PlateMotorOverload;
	
	gAlarm.WaterPressureLow = gMachineIn.WaterPressureLow;
	
	
	/* -------------- 風壓異常 延時 ----------------*/
	if (1 == gMachineInfo.Auto) //ipis0924
	{
		//		gAlarm.AirPressureLow = gMachineIn.AirPressureLow ;
		
		if( gMachineIn.LMold.AirPressureLow || gMachineIn.RMold.AirPressureLow)  //owen 20201027
		{
			gMacTimer.AirPressureLowAlarmDly.IN = 1;
			gMacTimer.AirPressureLowAlarmDly.PT = gUserFix_new.AirPressureLowAlarmDlyTime;
			gAlarm.AirPressureLowDly = 1;
			if(1 == gMacTimer.AirPressureLowAlarmDly.Q)
			{
				gAlarm.AirPressureLow = 1;
				gAlarm.AirPressureLowDly = 0;
				gMacTimer.AirPressureLowAlarmDly.IN = 0;	
				gMachineInfo.AirPressureLowDlyFlag = 1;
			}
			
			switch (AirPressureLowStatus)
			{
				case 0:
					gMacTimer.AirPressureLowAlarmDly.IN = 1;
					gMacTimer.AirPressureLowAlarmDly.PT = gUserFix_new.AirPressureLowAlarmDlyTime;
					gAlarm.AirPressureLowDly = 1;
					if(1 == gMacTimer.AirPressureLowAlarmDly.Q)
					{
						gMacTimer.AirPressureLowAlarmDly.IN = 0;
						gAlarm.AirPressureLowDly = 0;
						gAlarm.AirPressureLow = 1;
						gMachineInfo.AirPressureLowDlyFlag = 1;
						StatusDelay = 2;
						AirPressureLowStatus = 10;
					}
					break;
				case 10:
					AlarmSound = 1;
					if(StatusDelay > 0 )StatusDelay--;
					if( 0 == StatusDelay)
					{	
						AirPressureLowStatus = 20;
					}
					//reset
						

					break;
				case 20:

						

					break;

			}
     

		}
		else
		{
			gMacTimer.AirPressureLowAlarmDly.IN = 0;
			gAlarm.AirPressureLowDly  = 0;
			AirPressureLowStatus = 0;
		}
	}
	else
	{
		AirPressureLowStatus = 0;
	}


	
	
	gAlarm.Voltage24vNormal = !gMachineIn.Voltage24vNormal;
	gAlarm.L_LubricNormal = !gMachineIn.LMold.LubricNormal;
	gAlarm.R_LubricNormal = !gMachineIn.RMold.LubricNormal;
	
//	if(1 == LMold.Option.RobotTransfer)
//	{
//		gAlarm.ServoTransferErrL = LMold.Robot.Transfer.Error;		
//		gAlarm.ServoTransProtectL = LMold.Robot.Transfer.BwHWSwitch | LMold.Robot.Transfer.FwHWSwitch;
//	}
//	else
//	{
//		gAlarm.ServoTransferErrL = 0;
//		gAlarm.ServoTransProtectL = 0;
//	}
   	
	if(1 == LMold.Option.RobotUpDn)
	{
		gAlarm.ServoUpDnErrL = LMold.Robot.UpDn.Error;
		gAlarm.ServoUpDnProtectL  = LMold.Robot.UpDn.BwHWSwitch | LMold.Robot.UpDn.FwHWSwitch;
	}
	else
	{
		gAlarm.ServoUpDnErrL = 0;
		gAlarm.ServoUpDnProtectL = 0;
	}
	
//	if(1 == RMold.Option.RobotTransfer)
//	{
//		gAlarm.ServoTransferErrR = RMold.Robot.Transfer.Error;   	
//		gAlarm.ServoTransProtectR = RMold.Robot.Transfer.BwHWSwitch | RMold.Robot.Transfer.FwHWSwitch;
//	}
//	else
//	{
//		gAlarm.ServoTransferErrR = 0;
//		gAlarm.ServoTransProtectR = 0;
//	}
	
	if(1 == RMold.Option.RobotUpDn)
	{
		gAlarm.ServoUpDnErrR = RMold.Robot.UpDn.Error;
		gAlarm.ServoUpDnProtectR  = RMold.Robot.UpDn.BwHWSwitch | RMold.Robot.UpDn.FwHWSwitch;
	}
	else
	{
		gAlarm.ServoUpDnErrR = 0;
		gAlarm.ServoUpDnProtectR =0;
	}
   
   
	gMachineIn.LMold.CarriageLockLimit = ! LMold.Option.Enable;
	if(LMold.Option.Enable == gMachineIn.LMold.CarriageLockLimit)
	{
		gAlarm.CarriageLockL = 1;
	}
	else
	{
		gAlarm.CarriageLockL = 0;
	}

	gMachineIn.RMold.CarriageLockLimit = ! RMold.Option.Enable;
	if(RMold.Option.Enable == gMachineIn.RMold.CarriageLockLimit)
	{
		gAlarm.CarriageLockR = 1;
	}
	else
	{
		gAlarm.CarriageLockR = 0;
	}

   
	gAlarm.ModuleErr = gPROCESS_info.moduleErr;
   
	gAlarm.OilPosition = !gMachineIn.OilPositionNormal;
   
	gAlarm.LubriFilter   = gMachineIn.LubriFilter;
	gAlarm.LubriOverload = gMachineIn.LubriOverload;
   
	if(0 == gMachineFix.OilCooling.OilTempAlarm)  gMachineFix.OilCooling.OilTempAlarm = 40;
	if(gMachineInfo.OilTemp > gMachineFix.OilCooling.OilTempAlarm)
	{
		gAlarm.OilTempHigh = 1;
	}
	else
	{
		gAlarm.OilTempHigh = 0;
	}

	if(gServoLinear.Current > -32767 && gServoLinear.Current < 32767)
	{
		gAlarm.ParisonLinearBreak = 0;
	}
	else
	{
		gAlarm.ParisonLinearBreak = 1;
	}

	memcpy(&gAlarm.LAlarm,&LMold.Alarm,sizeof(gAlarm.LAlarm));
	memcpy(&gAlarm.RAlarm,&RMold.Alarm,sizeof(gAlarm.RAlarm));
	
	memcpy(&gAlarm.LHwAlarm,&LMold.HwAlarm,sizeof(gAlarm.LHwAlarm));
	memcpy(&gAlarm.RHwAlarm,&RMold.HwAlarm,sizeof(gAlarm.RHwAlarm));

	memcpy(AlarmBit,&gAlarm,sizeof(gAlarm));

	IsAlarm   = 0;
	IsWarning = 0;
   
	/* ------------------------ 当前报警 ----------------------------- */

	for(i=0;i<sizeof(gAlarm);i++)
	{
		if(1 == EDGEPOS(AlarmBit[i],OldPBit[i]))
		{
			gMachineInfo.AlarmIndex = i+1;
			if(1 == pAlarmband[i])
			{
				AlarmSound = 1;
			}
		}
		else if((1 == EDGENEG(AlarmBit[i],OldNBit[i]))&&(0 == gAlarm.EmergencyStop))
		{
			gMachineInfo.AlarmIndex  = 0;
			AlarmSound = 0;
		}
		else
		{
			if(1 == AlarmBit[i])
			{
				gMachineInfo.AlarmIndex = i+1;
			}
		}

		OldPBit[i] = AlarmBit[i];
		OldNBit[i] = AlarmBit[i];

		if(1 == AlarmBit[i])
		{
			if(1 == pAlarmband[i])
			{
				IsAlarm = 1;
			}
			else
			{
				IsWarning = 1;
			}
		}
	}/* for(i=0;i<sizeof(gAlarm);i++) */

	AlarmCount ++;

	/* ------------------------- 警报闪动图标 ---------------------------- */
	if(1 == IsAlarm || 1 == IsWarning)
	{
		if((AlarmCount%10) < 5)
		{
			gMachineInfo.IsAlarm = 1;
		}
		else
		{
			gMachineInfo.IsAlarm = 0;
		}
	}
	else
	{
		gMachineInfo.IsAlarm = 0;
	}

	/* ------------------------- 输出警报文字 ---------------------------- */
	if (gPanelObj.vcHandle == 0)
	{

	}
	else if (IsAlarm || IsWarning)
	{
		if((AlarmCount%3) == 0)
		{
			if (!VA_Saccess (1, gPanelObj.vcHandle))
			{
				if(iFunction > 2 || iFunction < 1)iFunction = 1;
				AlarmLength = sizeof(sAlarmText)/sizeof(sAlarmText[0]);
				Status_wcGetAlarmList = VA_wcGetActAlarmList(1, gPanelObj.vcHandle, (UDINT)sAlarmTmp, (UDINT)&AlarmLength, iFunction, '|', 2);

				if (Status_wcGetAlarmList == 240)
				{
					iFunction = 1;
					lengthTotal = record;
					record = 0;
					memcpy(sAlarmTotal,sAlarmTotalTmp,sizeof(sAlarmTotalTmp));
				}
				else if(Status_wcGetAlarmList == 0)
				{
					if(S$ALARM_TEXT_TOP == gMachineFix.Option.AlarmTextMode )
						{
						iFunction = 1;
					}
					else
					{
						iFunction = 2;
					}

					if(iFunctionOld == 1 && iFunction == 2)
					{
						memset(sAlarmTotalTmp,0,sizeof(sAlarmTotalTmp));
					}

					j = 0;
					for(i=0;i<sizeof(sAlarmText)/sizeof(sAlarmText[0]);i++)
					{
						if(sAlarmTmp[i] == '|')
						{
							IndexArray[j] = i;
							j++;
						}

						if(sAlarmTmp[i] == 0)
						{
							break;
						}
					}/* for(i=0;i<sizeof(sAlarmText)/sizeof(sAlarmText[0]);i++) */

					memset(sAlarmText,0,sizeof(sAlarmText));
					if(IndexArray[3] > IndexArray[2] + 2)
					{
						for(i=IndexArray[2]+1;i<IndexArray[3];i++)
						{
							sAlarmText[i - IndexArray[2] - 1] = sAlarmTmp[i];
							if(record < 180)
							{
								sAlarmTotalTmp[record + i - IndexArray[2] - 1] = sAlarmTmp[i];
							}
						}

						sAlarmTmp[IndexArray[3] - IndexArray[2] - 1] = 0;

						if(record < 180)
						{
							sAlarmTotalTmp[record + IndexArray[3] - IndexArray[2] - 1] = '/';
							record = record + IndexArray[3] - IndexArray[2];
						}
					}/* if(IndexArray[1] > IndexArray[2] + 2) */
				}/* else if(Status_wcGetAlarmList == 0) */
				VA_Srelease (1, gPanelObj.vcHandle);
			}/* if (!VA_Saccess (1, vcHandle)) */
		}/* if((AlarmCount%3) == 0) */
	}
	else  /* else if (IsAlarm) */
	{
		memset(sAlarmText,0,sizeof(sAlarmText));
	}/* if (vcHandle == 0) */

	iFunctionOld = iFunction;

	/* ------------ gAlarm led mode ----------- */

	if (1 == IsAlarm )
	{
		if( 1 == gAlarm.AirPressureLowDly )
		{
			gMachineInfo.AlarmLight.Red = 0;
			gMachineInfo.AlarmLight.Yellow = 1;
			gMachineInfo.AlarmLight.Green = 0;
		}
		else
		{
			gMachineInfo.AlarmLight.Red = 1;
			gMachineInfo.AlarmLight.Yellow = 0;
			gMachineInfo.AlarmLight.Green = 0;
		}
	}
	else
	{
		gMachineInfo.AlarmLight.Red = 0;
		if(1 == gMachineInfo.IsImply ||  (1 == gExtruderTempLow_HMIFlag && 1 == gMachineOut.HeatPower) ) // 加溫啟動 但溫度未到（與壓出機啟動條件相同）
		{
			gMachineInfo.AlarmLight.Yellow = 1;
			gMachineInfo.AlarmLight.Green = 0;
		}
		else
		{
			gMachineInfo.AlarmLight.Yellow = 0;
			gMachineInfo.AlarmLight.Green = 1;
		}
	
	}
	
	gMachineOut.AlarmRed 	= gMachineInfo.AlarmLight.Red; 
	gMachineOut.AlarmYellow = gMachineInfo.AlarmLight.Yellow; 
	gMachineOut.AlarmGreen 	= gMachineInfo.AlarmLight.Green; 
	
	//	if(0 == gMachineFix.Option.AlarmLedMode)
	//	{
	//		gMachineOut.AlarmRed = IsAlarm;
	//	}
	//	else
	//	{
	//		gMachineOut.AlarmRed = gMachineInfo.IsAlarm;
	//	}
	
	//	gMachineOut.AlarmYellow = IsWarning;
	
	/* ------------ gAlarm sound mode ----------- */
	
	if(1 == AlarmSound)
	{
//		if((AlarmCount%8) < 4)
//		{
//			gMachineOut.AlarmBuzzer = 1;
//		}
//		else
//		{
//			gMachineOut.AlarmBuzzer = 0;
//		}
		
		//Alarm Buzzer Echo Mode
		AlarmBuzzerCount ++;
		if((AlarmBuzzerCount < 5))
		{
			gMachineOut.AlarmBuzzer = 1;
		}
		else
		{ 	
			if(AlarmBuzzerCount > 7) AlarmBuzzerCount = 0;
			gMachineOut.AlarmBuzzer = 0;
		}
		
//		gMachineOut.AlarmBuzzer = 1;
		
		
	}
	else
	{
		gMachineOut.AlarmBuzzer  = 0;
		AlarmBuzzerCount = 0;
	}
	/* ------------------------ 消除报警 -------------------------------- */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET],OldAlarmConfirmOK))
	{
		memset(&gAlarm,0,sizeof(gAlarm));
		memset(&LMold.Alarm,0,sizeof(LMold.Alarm));
		memset(&RMold.Alarm,0,sizeof(RMold.Alarm));
		memset(&LMold.HwAlarm,0,sizeof(LMold.HwAlarm));
		memset(&RMold.HwAlarm,0,sizeof(RMold.HwAlarm));
		/*gExtruderACtrl.InverterCmd.bResetFault = 1*/
	}
	
	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET])
	{
		AlarmSound = 0;
		gPROCESS_info.extruderStopEnable = 0;
	}
	OldAlarmConfirmOK = gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET];

	/*Albert P84 Error reset by Visual Key*/
	gExtruderACtrl.InverterCmd.bResetFault = gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET];
	gExtruderBCtrl.InverterCmd.bResetFault = gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET];
	gExtruderCCtrl.InverterCmd.bResetFault = gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET];
	gExtruderVCtrl.InverterCmd.bResetFault = gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET];
		
		/* ------------------------- 系统提示信息  ------------------------- */
		for(i=0;i<sizeof(gImply);i++)
	{
		if((1 == ((USINT *)&gImply)[i]  ) && 
			(0 == ((USINT*)&OldImply)[i]))    /* edge positive */
		{
			gMachineInfo.ImplyTime  = 51;     /*  10 second gImply time */
			gMachineInfo.IsImply    = 1;
			gMachineInfo.ImplyIdex  = i + 1;
		}
		((USINT *)&OldImply)[i] = (( USINT *)&gImply)[i];
	}

	if(gMachineInfo.ImplyTime > 0)
	{
		gMachineInfo.ImplyTime --;
	}

	
	if(1 == gMachineInfo.ImplyTime)
	{
		memset(&gImply,0,sizeof(gImply));

		gMachineInfo.IsImply   = 0;
		gMachineInfo.ImplyIdex = 0;
	}
	
	
	for(i=0;i<32;i++)
	{
		if(1 == gPanelObj.KeyMatric[i] )
		{
			if(1 == gMachineInfo.Manual || 1 == gMachineInfo.Calib )
			{
				if ( 1 == gMachineInfo.IsImply ) gMachineOut.ImplyBuzzer  = 1;
			}	
		}
		
		
//		if(1 == EDGEPOS(gPanelObj.KeyMatric[i],PKeyMatric[i]) )
//		{
//			if(1 == gMachineInfo.Manual || 1 == gMachineInfo.Calib )
//			{
//				if ( 1 == gMachineInfo.IsImply ) gMachineOut.ImplyBuzzer  = 1;
//			}
//			
//		}
		
		if(1 ==  EDGENEG(gPanelObj.KeyMatric[i],NKeyMatric[i]) )
		{
			gMachineOut.ImplyBuzzer  = 0;
			gMachineInfo.ImplyTime = 2 ;
		}
	}
}/* task end */

void AlarmTyp(Alarm_typ * pInst)
{
	pInst->batteryLow     = 0;				  
	pInst->ModuleErr      = 1;               
	pInst->SaftyDoor      = 1;                
	pInst->EmergencyStop  = 1;            
	
	pInst->ScrewTempLow   = 0;             
	pInst->ScrewTempHigh  = 0; 		      		
	pInst->ThermocoupleBroken = 1;		  
	pInst->TempException      = 1;			  
	
	pInst->OilPumpErr      = 1;		  
	pInst->CoolingPumpErr  = 0;	  
	pInst->VisuExtrMotorOverload = 1;	  
	pInst->A_ExtruderMotorOverload = 1;  
	pInst->B_ExtruderMotorOverload = 1;

	pInst->ParisonLinearBreak   = 1;       
	pInst->ParisonLinearCalErr  = 1;
	pInst->ParisonValuebeyond   = 0;
	pInst->OilPumpFilter        = 0;      	  	  

	memset(&pInst->LAlarm,1,sizeof(pInst->LAlarm));
	memset(&pInst->RAlarm,1,sizeof(pInst->RAlarm));
//	pInst->LAlarm.ParisonTimeSmall = 0;
//	pInst->RAlarm.ParisonTimeSmall = 0;

	pInst->LCycleTime = 1;
	pInst->RCycleTime = 1;
	
	pInst->OilPosition  = 0;
	pInst->LubrTimeZero    = 0;

	
	pInst->OilTempHigh = 0;
	pInst->OilPumpOff  = 1;
	
	pInst->LCarriageLinearBreak = 1;
	pInst->RCarriageLinearBreak = 1;
	pInst->LClampLinearBreak = 1;
	pInst->RClampLinearBreak = 1;
	
	pInst->LowSafeValvePumpOff = 1;
	pInst->HighSafeValvePumpOff = 1;
	pInst->LubriFilter   = 0;
	pInst->LubriOverload = 0;
	
	pInst->VisuExtrCoolOverload = 0;
	pInst->CoolingPumpErr  = 0;
	pInst->AccuSensorError  = 1;
	pInst->AccuChargeError  = 1;

	pInst->ExtrAMeltP_Alarm  = 1;
	pInst->ExtrBMeltP_Alarm = 1;
	pInst->ExtrACurrentAlarm  = 1;
	pInst->ExtrBCurrentAlarm = 1;
	pInst->A_ExtrMotorOverload =1;
	pInst->B_ExtrMotorOverload =1;
	pInst->C_ExtrMotorOverload =1;
	pInst->V_ExtrMotorOverload =1;
	
	
	pInst->HeatError=1;

	pInst->AccuChargeRPressure =1;
	pInst->AirPressureLow = 1;
	pInst->AirPressureLowDly = 1;
	pInst->OilTempHigh	= 1;

	
	pInst->AirPressureLow=1; //ipis0924
	pInst->WaterPressureLow=1;//ipis0924
	
	
}/*  void AlarmTyp(Alarm_typ * pInst) */

