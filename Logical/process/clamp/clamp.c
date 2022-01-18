/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: clamp
 * File: clamp.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program clamp --- open and close 
 ********************************************************************/
#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"               /*edge function                         */
/**************************************************************************************
** 					 	    define								                     **
**************************************************************************************/
#define   INSTANCY_STOP_DECEL    3000
#define   INSTANCY_STOP_ACCEL    3000

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void ClampToggleInit(Clamp_Fix_typ * pClampFix);
void ClampSwitch(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC);
//void ClampLVDT(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC);
void ClampLVDT(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC, Mold_Para_User_typ * pMoldUser);
void ClampDriveToggle_Drive(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC, Mold_Para_User_typ * pMoldUser);
void ButtonOffDelay(Mold_typ * pMold , timer_typ * pTimer);
REAL xTogLinearRTrans(REAL pClampPos);
REAL xTogLinearLTrans(REAL pClampPos);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */

	LMold.Clamp.Step = 0;
	RMold.Clamp.Step = 0;
       
	LMold.Clamp.vRamp.Ts = tTask;
	RMold.Clamp.vRamp.Ts = tTask;
    
	LMold.Clamp.vProfGen.Ts = tTask;
	RMold.Clamp.vProfGen.Ts = tTask;

	/* differentiator for evaluating the actual speed */
	vActHydDT2_L.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2_L.Tv = 1.0; 	   /* derivative time constant [s]   */
	vActHydDT2_L.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_L.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */

	/* differentiator for evaluating the actual speed */
	vActHydDT2_R.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2_R.Tv = 1.0; 	   /* derivative time constant [s]   */
	vActHydDT2_R.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_R.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */
	
	TogLinearL.enable = 1;
	TogLinearL.nNodes = 21;
	TogLinearL.pNodes = &gMachineFix.MoldL.Clamp.ToggleLin[0];
	
	TogLinearR.enable = 1;
	TogLinearR.nNodes = 21;
	TogLinearR.pNodes = &gMachineFix.MoldR.Clamp.ToggleLin[0];
	
	xTogLinearL.enable = 1;
	xTogLinearL.nNodes = 21;
	xTogLinearL.pNodes = &xToggleNodeL[0];

	xTogLinearR.enable = 1;
	xTogLinearR.nNodes = 21;
	xTogLinearR.pNodes = &xToggleNodeR[0];
	
	memcpy(ToggleLOld,gMachineFix.MoldL.Clamp.ToggleLin,sizeof(gMachineFix.MoldL.Clamp.ToggleLin));
	memcpy(ToggleROld,gMachineFix.MoldR.Clamp.ToggleLin,sizeof(gMachineFix.MoldR.Clamp.ToggleLin));
	

	
	rMin = 0.0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	//Prop Valve Output Inverse
	bClampPropValveOutputInverse  = gMachineFix.Option.bClampPropValveOutputInverse;
	
	
	/*--- 機構線性，強制線性化 ----*/
	if( ACTUATOR_ELECTRIC ==  gMachineFix.MoldR.Clamp.eActuatorType)
	{
		int ToggleLinIndex;
		if( MECHANISM_LINEAR ==  gMachineFix.MoldR.Clamp.eMechanismType )
		{
			bToggle_Init = 1;
			for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM - 1); ToggleLinIndex++)
			{
				int iSumR = 0;
				int iSumL = 0;
				iSumR = gMachineFix.MoldR.Clamp.Length / (ToggleLinNodeNum - 1);
				gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].X = iSumR * ToggleLinIndex ;
				gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].Y = iSumR * ToggleLinIndex ;
			
				iSumL = gMachineFix.MoldL.Clamp.Length / (ToggleLinNodeNum - 1);
				gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].X = iSumL * ToggleLinIndex ;
				gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].Y = iSumL * ToggleLinIndex ;
				
				if( ToggleLinIndex >= ToggleLinNodeNum)
				{
					gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldR.Clamp.Length ;
					gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldR.Clamp.Length ;
					
					gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldL.Clamp.Length ;
					gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldL.Clamp.Length ;
				}
	
			}
		}
		else if( MECHANISM_TOGGLE ==  gMachineFix.MoldR.Clamp.eMechanismType  )
		{
			if(1 == bToggle_Init)
			{
				bToggle_Init = 0;
				
				// clear ToggleLin table
				for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM - 1); ToggleLinIndex++)
				{
					
					if(ToggleLinIndex <ToggleLinNodeNum )
					{
						gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].X = 0 ;
						gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].Y = 0 ;
					
						gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].X = 0 ;
						gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].Y = 0 ;
					}
					else
					{
						gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldR.Clamp.Length ;
						gMachineFix.MoldR.Clamp.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldR.Clamp.Length ;
					
						gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldL.Clamp.Length ;
						gMachineFix.MoldL.Clamp.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldL.Clamp.Length ;
					
					}
				}	
			
			}
		}

	}
	
	
	
	int i;
	for(i=0;i<MAX_TOGGLE_LIN_NUM;i++)
	{
		xToggleNodeL[i].X = gMachineFix.MoldL.Clamp.ToggleLin[i].Y;
		xToggleNodeL[i].Y = gMachineFix.MoldL.Clamp.ToggleLin[i].X;
		
		xToggleNodeR[i].X = gMachineFix.MoldR.Clamp.ToggleLin[i].Y;
		xToggleNodeR[i].Y = gMachineFix.MoldR.Clamp.ToggleLin[i].X;
	}
	
	if( 20 > gMachineFix.MoldR.Clamp.ToggleLinNodeNum ) gMachineFix.MoldR.Clamp.ToggleLinNodeNum = 20;
	else if( 30 < gMachineFix.MoldR.Clamp.ToggleLinNodeNum ) gMachineFix.MoldR.Clamp.ToggleLinNodeNum = 30;
	ToggleLinNodeNum = gMachineFix.MoldR.Clamp.ToggleLinNodeNum;
	
	if(memcmp(ToggleLOld,gMachineFix.MoldL.Clamp.ToggleLin,sizeof(gMachineFix.MoldL.Clamp.ToggleLin)) != 0 
		|| oldToggleLinNodeNum != ToggleLinNodeNum)
	{
		TogLinearL.update = 1;
		TogLinearL.nNodes = ToggleLinNodeNum;
		memcpy(ToggleLOld,gMachineFix.MoldL.Clamp.ToggleLin,sizeof(gMachineFix.MoldL.Clamp.ToggleLin));
		
		xTogLinearL.update = 1;
		xTogLinearL.nNodes = ToggleLinNodeNum;
		HydLinear(&xTogLinearL);
	}
	
	
	if(memcmp(ToggleROld,gMachineFix.MoldR.Clamp.ToggleLin,sizeof(gMachineFix.MoldR.Clamp.ToggleLin)) != 0 
		|| oldToggleLinNodeNum != ToggleLinNodeNum)
	{
		TogLinearR.update = 1;
		TogLinearR.nNodes = ToggleLinNodeNum;
		memcpy(ToggleROld,gMachineFix.MoldR.Clamp.ToggleLin,sizeof(gMachineFix.MoldR.Clamp.ToggleLin));
		
		xTogLinearR.update = 1;
		xTogLinearR.nNodes = ToggleLinNodeNum;
		HydLinear(&xTogLinearR);
	}
	
	/*  ExtInMoldSticker limit   */
	RMold.TransDIn.ExtInMoldStickerBwLimit	= gMachineIn.RMold.ExtInMoldStickerBwLimit;	
	LMold.TransDIn.ExtInMoldStickerBwLimit	= gMachineIn.LMold.ExtInMoldStickerBwLimit;
	
	RMold.TransDIn.ExtInMoldStickerAlarm	= gMachineIn.RMold.ExtInMoldStickerAlarm;	
	LMold.TransDIn.ExtInMoldStickerAlarm	= gMachineIn.LMold.ExtInMoldStickerAlarm;
	
	if( 1 == RMold.TransDIn.ExtInMoldStickerBwLimit &&  0 == OldRExtInMoldStickerBwLimit)	RMold.Clamp.ExtInMoldInDone_Flag = 1;	
	if( 1 == LMold.TransDIn.ExtInMoldStickerBwLimit &&  0 == OldLExtInMoldStickerBwLimit)	LMold.Clamp.ExtInMoldInDone_Flag = 1;
		
	OldRExtInMoldStickerBwLimit = RMold.TransDIn.ExtInMoldStickerBwLimit;
	OldLExtInMoldStickerBwLimit = LMold.TransDIn.ExtInMoldStickerBwLimit;
	
	/*  clamp open & close limit   */
	LMold.TransDIn.ClampClsLimit     = gMachineIn.LMold.ClampClsLimit;	
	LMold.TransDIn.ClampClsSlowLimit = gMachineIn.LMold.ClampClsSlowLimit;
	LMold.TransDIn.ClampOpnMicroLimit = gMachineIn.LMold.ClampOpnMicroLimit;
	LMold.TransDIn.ClampOpnSlowLimit = gMachineIn.LMold.ClampOpnSlowLimit;
	if (EDGEPOS(gMachineIn.LMold.ClampOpnLimit,OldLClampOpnDI) || 1==gMachineIn.LMold.ClampOpnLimit)  //ipis1121
	{
		gLClampOpnFlag_Calib =1;
	}
	
	if (EDGEPOS(gMachineIn.RMold.ClampOpnLimit,OldRClampOpnDI) || 1==gMachineIn.RMold.ClampOpnLimit)
	{
		gRClampOpnFlag_Calib =1;
	}
	//	if (0==gMachineInfo.Calib) //ipis1114
	if (1==gMachineInfo.Auto ) //ipis1121
	{
		gLClampOpnFlag_Calib =0;  // ipis1114
		gRClampOpnFlag_Calib =0;
		LMold.TransDIn.ClampOpnLimit     = gMachineIn.LMold.ClampOpnLimit;
	}
	else
	{
		LMold.TransDIn.ClampOpnLimit     = gLClampOpnFlag_Calib;						
	}
	

	RMold.TransDIn.ClampClsLimit     = gMachineIn.RMold.ClampClsLimit;	
	RMold.TransDIn.ClampClsSlowLimit = gMachineIn.RMold.ClampClsSlowLimit;
	RMold.TransDIn.ClampOpnMicroLimit = gMachineIn.RMold.ClampOpnMicroLimit;
	RMold.TransDIn.ClampOpnSlowLimit = gMachineIn.RMold.ClampOpnSlowLimit;
	//	if (0==gMachineInfo.Calib ) //ipis1114
	if (1==gMachineInfo.Auto ) //ipis1121	
	{
		gLClampOpnFlag_Calib =0;  // ipis1114
		gRClampOpnFlag_Calib =0;
		RMold.TransDIn.ClampOpnLimit     = gMachineIn.RMold.ClampOpnLimit;
	}
	else
	{
		RMold.TransDIn.ClampOpnLimit     = gRClampOpnFlag_Calib;						
	}
	
	gMachineFix.MoldL.Clamp.Tolerance = gMachineFix.MoldR.Clamp.Tolerance; 
    
	
	
	/* --------------- */
	/* clamp velocity  */
	/* --------------- */
	if( ACTUATOR_ELECTRIC ==  gMachineFix.MoldR.Clamp.eActuatorType)
	{
		if(TogLinearL.status != 0) //LinNotOK 
		{
			vActHydDT2_L.u = LMold.Clamp.mmAs;
		}
		else
		{
			vActHydDT2_L.u = LMold.Clamp.mmAsDrive;
		}
		
		if(TogLinearR.status != 0)  //LinNotOK
		{
			vActHydDT2_R.u = RMold.Clamp.mmAs;
		}
		else
		{
			vActHydDT2_R.u = RMold.Clamp.mmAsDrive;
		}
	}
	else
	{
		vActHydDT2_L.u = LMold.Clamp.mmAs;
		vActHydDT2_R.u = RMold.Clamp.mmAs;
	}
	
	HydDT2(&vActHydDT2_L);
	LMold.Clamp.v_act = vActHydDT2_L.y;
	
	HydDT2(&vActHydDT2_R);
	RMold.Clamp.v_act = vActHydDT2_R.y;
	
	
	
	if(1 == ToggleInputL)
	{
		ToggleInputL = 0;
		TogLinearL.update = 1;
		gRecipeOperate.DataSave = 1;
	}
	
	if(1 == ToggleInputR)
	{
		ToggleInputR = 0;
		TogLinearR.update = 1;
		gRecipeOperate.DataSave = 1;
	}
	
	/* if(gMachineFix.MoldL.Clamp.ToggleLin[10].Y < 1.0 && 1 == gRecipeOperate.OnceOk)
	{
		ClampToggleInit(&gMachineFix.MoldL.Clamp);
		gRecipeOperate.DataSave = 1;
	}
	
	if(gMachineFix.MoldR.Clamp.ToggleLin[10].Y < 1.0 && 1 == gRecipeOperate.OnceOk)
	{
		ClampToggleInit(&gMachineFix.MoldR.Clamp);
		gRecipeOperate.DataSave = 1;
	}*/

	/*  run function  */
	if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			ClampLVDT(&LMold,&gMachineFix.MoldL.Clamp,&gMachinePara.MoldL.Clamp,&gSPC_Current.MoldL,&gUserPara.LMold);
		}
		ClampLVDT(&RMold,&gMachineFix.MoldR.Clamp,&gMachinePara.MoldR.Clamp,&gSPC_Current.MoldR,&gUserPara.RMold);
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			ClampDriveToggle_Drive(&LMold,&gMachineFix.MoldL.Clamp,&gMachinePara.MoldL.Clamp,&gSPC_Current.MoldL,&gUserPara.LMold);
		}
		ClampDriveToggle_Drive(&RMold,&gMachineFix.MoldR.Clamp,&gMachinePara.MoldR.Clamp,&gSPC_Current.MoldR,&gUserPara.RMold);
	}


	if(1 == gMachineFix.Option.bHideLeftStation)
	{
		LMold.Clamp.Error  = 0;
		LMold.Clamp.CalOK  = 1;
		LMold.Clamp.OpnPos = 1;	
	}
	
	/*  clamp open & close valve out   */
	gMachineOut.LMold.ClampCls  = LMold.ValveOut.ClampCls;
	gMachineOut.LMold.ClampOpn  = LMold.ValveOut.ClampOpn;

	gMachineOut.RMold.ClampCls  = RMold.ValveOut.ClampCls;
	gMachineOut.RMold.ClampOpn  = RMold.ValveOut.ClampOpn;

	gMachineOut.LMold.ClampClsDiff  = LMold.ValveOut.ClampClsDiff;
	gMachineOut.LMold.ClampOpnDiff  = LMold.ValveOut.ClampOpnDiff;

	gMachineOut.RMold.ClampClsDiff  = RMold.ValveOut.ClampClsDiff;
	gMachineOut.RMold.ClampOpnDiff  = RMold.ValveOut.ClampOpnDiff;
	
	gMachineOut.LMold.ClampOpnRelease  = LMold.ValveOut.ClampOpnRelease;
	gMachineOut.RMold.ClampOpnRelease  = RMold.ValveOut.ClampOpnRelease;
	
	gMachineOut.LMold.ClampMoveValve	= LMold.ValveOut.ClampMoveValve;
	gMachineOut.RMold.ClampMoveValve	= RMold.ValveOut.ClampMoveValve;
	
	gMachineOut.LMold.ClampOpnAux   = LMold.ValveOut.ClampOpnAux;
	gMachineOut.RMold.ClampOpnAux   = RMold.ValveOut.ClampOpnAux;
	
	gMachineOut.LMold.ClampClsSlow = LMold.ValveOut.ClampClsSlow;
	gMachineOut.RMold.ClampClsSlow = RMold.ValveOut.ClampClsSlow;	
	
	gMachineOut.LMold.RobotClampOpn = LMold.ValveOut.RobotClampOpn;/*Albert*/
	gMachineOut.RMold.RobotClampOpn = RMold.ValveOut.RobotClampOpn;/*Albert*/
	
	gMachineOut.LMold.RobotClampCls = LMold.ValveOut.RobotClampCls;/*Chaoi*/
	gMachineOut.RMold.RobotClampCls = RMold.ValveOut.RobotClampCls;/*Chaoi*/
	
	// ExtInMoldSticker
	LMold.ValveOut.ExtInMoldStickerAllow = LMold.Option.ExtInMoldSticker && (LMold.Clamp.OpnPos && LMold.Carriage.BwPos && LMold.TransDIn.ExtRobotUpLmit);
	RMold.ValveOut.ExtInMoldStickerAllow = RMold.Option.ExtInMoldSticker && (RMold.Clamp.OpnPos && RMold.Carriage.BwPos && RMold.TransDIn.ExtRobotUpLmit);
	gMachineOut.LMold.ExtInMoldStickerAllow = LMold.ValveOut.ExtInMoldStickerAllow;/*owen*/
	gMachineOut.RMold.ExtInMoldStickerAllow = RMold.ValveOut.ExtInMoldStickerAllow;/*owen*/
    
	if(LMold.Clamp.ExtInMoldInDone_cnt * tTask * 2 > gMachineFix.MoldL.Clamp.ExtInMoldAlarmTime)
	{
		LMold.Clamp.ExtInMoldInDone_cnt = 0;
		LMold.Alarm.ExtInMoldStickerTimeOut = 1;
	}
	
	if(RMold.Clamp.ExtInMoldInDone_cnt * tTask * 2 > gMachineFix.MoldR.Clamp.ExtInMoldAlarmTime)
	{
		RMold.Clamp.ExtInMoldInDone_cnt = 0;
		RMold.Alarm.ExtInMoldStickerTimeOut = 1;
	}
	
	gMachineInfo.LClampPos = LMold.Clamp.mmAs;
	gMachineInfo.RClampPos = RMold.Clamp.mmAs;
	
	gMachineInfo.LClampPos1 = LMold.Clamp.mmAsMax-gMachineInfo.LClampPos;
	gMachineInfo.RClampPos1 = RMold.Clamp.mmAsMax-gMachineInfo.RClampPos;
		
	TON_10ms(&RobotClampTime);
	TON_10ms(&LRobotClampTime);
	
	ButtonOffDelay(&RMold ,&gMacTimer.RClampBtnOffDly);
	ButtonOffDelay(&LMold ,&gMacTimer.LClampBtnOffDly);
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.Clamp.eActuatorType = gMachineFix.MoldR.Clamp.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.Clamp.eValveType = gMachineFix.MoldR.Clamp.eValveType;
	
	/*-------------- SensorType----------------- */
	gMachineFix.MoldL.Clamp.eSensorType = gMachineFix.MoldR.Clamp.eSensorType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.Clamp.ePressueOutputType = gMachineFix.MoldR.Clamp.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.Clamp.eFluxOutputType = gMachineFix.MoldR.Clamp.eFluxOutputType;
	
	/*------------ MechanismType--------------- */
	gMachineFix.MoldL.Clamp.eMechanismType = gMachineFix.MoldR.Clamp.eMechanismType;
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.Clamp.Position 		= RMold.Clamp.mmAs;
	gMotorTrace.LMold.Clamp.Position 		= LMold.Clamp.mmAs;
	
	


} /* end of _CYCLIC */

void ClampToggleInit(Clamp_Fix_typ * pClampFix)
{
	int idx;
	for(idx=0;idx<21;idx++)
	{
		pClampFix->ToggleLin[idx].X = pClampFix->ToggleLin[idx].Y = pClampFix->Length * idx / 20.0;
	}
}

void ClampSwitch(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC)
{
	/* No LVDT, no calibration false   */
	pMold->Clamp.CalOK = 1;
	pMold->Alarm.ClampCalErr = 0;
	
	/* gAlarm time value    */
	if(!pMold->TimeSet.ClampOpnAlarmTime)pMold->TimeSet.ClampOpnAlarmTime = 1000;  /* 10s  */
	if(!pMold->TimeSet.ClampClsAlarmTime)pMold->TimeSet.ClampClsAlarmTime = 1000;  /* 10s  */
	
	pClampFix->Open.V_Max = 1000;
	pMold->Clamp.V_Opn = pClampPara->V_Opn * pClampFix->Open.V_Max /100.0;
	pMold->Clamp.V_OpnSlow = pClampPara->V_OpnSlow * pClampFix->Open.V_Max /100.0;
    
	if(0 == pClampPara->V_OpnReset)pClampPara->V_OpnReset = 2.0;
	pMold->Clamp.V_OpnReset = pClampPara->V_OpnReset * pClampFix->Open.V_Max /100.0;
	
	pClampFix->Close.V_Max = 1000;
	pMold->Clamp.V_Cls = pClampPara->V_Cls * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_ClsSlow = pClampPara->V_ClsSlow * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_Tonnage = pClampPara->V_Tonnage * pClampFix->Close.V_Max /100.0;

	/* clamp open or close position flag  */
	pMold->Clamp.ClsPos = pMold->TransDIn.ClampClsLimit;
	pMold->Clamp.OpnPos = pMold->TransDIn.ClampOpnLimit;
	
	pMold->Clamp.vRamp.Ts = tTask;
	
	/* clamp open or close step  */
	switch ( pMold->Clamp.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->Clamp.p_set = 0;
			pMold->Clamp.v_set = 0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;

			pMold->ValveOut.ClampOpn  = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			
			pMold->Clamp.Timer.IN	   = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			
			RobotClampTime.IN = 0;
			LRobotClampTime.IN = 0;
			break;
		
		/*--------------------------------------------------------------- */
		/*------------------------ Clamp close -------------------------- */
		/*--------------------------------------------------------------- */
		case 100: /* close delay*/
			if(pMold->TimeSet.ClampClsDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampClsDelay;
				pMold->Clamp.Step = 200;
			}
			else
			{
				pMold->Clamp.Step = 300;
			}
			break;

		case 200: /* delay ok */
			pMold->TimeDis.ClampClsDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//				pMold->TimeDis.ClampClsDelay = 0;
				pMold->Clamp.Step = 300;
			}
			break;

		case 300:	/* time limit  */
			pMold->Clamp.LimitTimer.IN = 1;
			pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampClsAlarmTime;
			
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set  = 0.0;
            
			pMold->Clamp.vRamp.Accel = pClampFix->Close.accel;
			if(pMold->Clamp.vRamp.Accel < 1.0)pMold->Clamp.vRamp.Accel = 1000;
			
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
			
			pMold->Clamp.Step = 400;
			break;

		case 400:	/* close quick  */
			pMold->ActInfo.ClampCls  = 1;
			pMold->ValveOut.ClampCls = 1;
			pMold->ValveOut.ClampClsDiff = 1;
            
			pMold->Clamp.vRamp.Set = pMold->Clamp.V_Cls;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pClampPara->P_Cls; 
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			/*  reached target position */
			if(1 == pMold->TransDIn.ClampClsSlowLimit)
			{
				pMold->Clamp.Step = 500;
			}
			
			if(1 == pMold->TransDIn.ClampClsLimit)
			{
				pMold->Clamp.Step = 600;
			}	
			break;

		case 500:	/* close slow  */
			pMold->ActInfo.ClampCls  = 1;
			pMold->ValveOut.ClampCls = 1;
			pMold->ValveOut.ClampClsDiff = 1;/*Keep out sinece use perprotional valve*/
            
			pMold->Clamp.vRamp.Set = pMold->Clamp.V_ClsSlow;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pClampPara->P_ClsSlow;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			/*  reached target position */
			if(1 == pMold->TransDIn.ClampClsLimit)
			{
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step  = 600;
			}
			break;

		case 600:
			pMold->Clamp.p_set = 0.0;
			pMold->Clamp.v_set = 0.0;
			
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ValveOut.ClampOpn = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
				
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				pMold->Clamp.Step = 3000;
			}
			else
			{
				pMold->Clamp.Step = 3000;
			}
			
			pMold->Clamp.Timer.IN = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			break;

		case 3000:
			break;

		/* --------------- 调校关模 --------------- */
		case 5000:
			pMold->ActInfo.ClampCls  = 1;
			pMold->ValveOut.ClampCls = 1;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->Clamp.p_set = pClampFix->pCalibZero;
			pMold->Clamp.v_set = -pClampFix->vCalibZero * 10;
			break;

		/*-------------------------------------------------------------------------------*/
		/*-------------------------------- Clamp open  ----------------------------------*/
		/*-------------------------------------------------------------------------------*/
		case 10100: /* open delay*/
			if(pMold->TimeSet.ClampOpnDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampOpnDelay;
				pMold->Clamp.Step 	  = 10200;
			
			}
			else
			{
				pMold->Clamp.Step = 10300;
			}
			break;

		case 10200: /* delay ok */
			pMold->TimeDis.ClampOpnDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//				pMold->TimeDis.ClampOpnDelay = 0;
				pMold->Clamp.Step = 10300;
			}
			break;

		case 10300:	/* valve open delay  */
			pMold->Clamp.LimitTimer.IN = 1;
			pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;
			
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = 0.0;
            
			pMold->Clamp.vRamp.Accel = pClampFix->Open.accel;
			if(pMold->Clamp.vRamp.Accel < 1.0)pMold->Clamp.vRamp.Accel = 1000;
			
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
			
			pMold->Clamp.Step = 10400;
			break;
        
		case 10400:	/* open   */
			pMold->ActInfo.ClampOpn  = 1;
			pMold->ValveOut.ClampOpn = 1;
			pMold->ValveOut.ClampOpnDiff = 1;

			/*if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)*/  /* 自动复位 */
			if(1 == gMachineInfo.MachineReset)
			{
				pMold->Clamp.p_set = pClampPara->P_OpnReset;
				pMold->Clamp.vRamp.Set = pMold->Clamp.V_OpnReset;
			}
			else
			{
				pMold->Clamp.p_set = pClampPara->P_Opn;
				pMold->Clamp.vRamp.Set = pMold->Clamp.V_Opn;
			}
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			/*  reached target position */
			if(1 == pMold->TransDIn.ClampOpnSlowLimit)
			{
				pMold->Clamp.Step = 10500;
			}
			
			if(1 == pMold->TransDIn.ClampOpnLimit)
			{
				pMold->Clamp.Step = 10600;
			}	
			break;
		
		case 10500:
			pMold->ActInfo.ClampOpn  = 1;
			pMold->ValveOut.ClampOpn = 1;
			pMold->ValveOut.ClampOpnDiff = 1;/*Keep Out since we use perprotional valve to slow down*/

			if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)  /* 自动复位 */
			{
				pMold->Clamp.p_set = pClampPara->P_OpnReset;
				pMold->Clamp.vRamp.Set = pMold->Clamp.V_OpnReset;
			}
			else
			{
				pMold->Clamp.p_set = pClampPara->P_OpnSlow;
				pMold->Clamp.vRamp.Set = pMold->Clamp.V_OpnSlow;
			}
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			/*  reached end position */
			if(1 == pMold->TransDIn.ClampOpnLimit)
			{
				pMold->Clamp.Step = 10600;
			}	
			break;
            
		case 10600:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpOpn = pMold->Clamp.LimitTimer.ET/100.0;
			}
			pMold->Clamp.LimitTimer.IN = 0;
            
			if(pMold->ValveDelay.ClampOpn != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->ValveDelay.ClampOpn;
				pMold->Clamp.Step = 10700;
			}
			else
			{
				pMold->Clamp.Step = 13000;
			}
			break;
      
		case 10700:
			if(1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN = 0;
				pMold->Clamp.v_set  = 0;
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ValveOut.ClampOpn = 0;
				pMold->ValveOut.ClampOpnDiff = 0;
				pMold->Clamp.Step = 13000;
			}
			break;

		/* --------------- 绞牙微开模 --------------- */
		case 11100:
			if(pMold->TimeSet.ScrewMicroOpnTime > 0 && 0 == pMold->TransDIn.ClampOpnMicroLimit)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ScrewMicroOpnTime;
		
				pMold->Clamp.vTimeProf.Ts = tTask;
				pMold->Clamp.vTimeProf.t_move = pMold->TimeSet.ScrewMicroOpnTime * 0.01;
		
				pMold->Clamp.vTimeProf.v_set   = pClampPara->V_OpnMicro * 10.0; 
				pMold->Clamp.vTimeProf.v_start = 0.0; 
				pMold->Clamp.vTimeProf.v_end   = 0.0;
           
				pMold->Clamp.vTimeProf.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vTimeProf.accel <= 0.0)pMold->Clamp.vTimeProf.accel = 1000;
           
				pMold->Clamp.vTimeProf.decel = pClampFix->Open.decel;
				if(pMold->Clamp.vTimeProf.decel <= 0.0)pMold->Clamp.vTimeProf.decel = 1000;
           
				pMold->Clamp.vTimeProf.init = 1;
				pMold->Clamp.Step = 11200;
			}
			else
			{
				pMold->Clamp.Step = 13000;
			}
			break;

		case 11200:
			pMold->ActInfo.ClampOpn  = 1;
			pMold->ValveOut.ClampOpn = 1;
			pMold->ValveOut.ClampOpnDiff = 0;
            
			/* call profile function */
			HydvTimeProf(&pMold->Clamp.vTimeProf);
            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow;
			pMold->Clamp.v_set = pMold->Clamp.vTimeProf.v_out;
         
			pMold->TimeDis.ScrewMicroOpnTime = pMold->Clamp.Timer.ET;
			
			/*  reached target position */
			if(1 == pMold->Clamp.Timer.Q || 1 == pMold->TransDIn.ClampOpnMicroLimit)
			{
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ValveOut.ClampOpn = 0;
				pMold->ValveOut.ClampOpnDiff = 0;
           
				pMold->Clamp.Timer.IN = 0;
				pMold->TimeDis.ScrewMicroOpnTime = 0;
			
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 11300;
			}
			break;

		case 11300:
			pMold->Clamp.Timer.IN = 0;
         
			pMold->Clamp.p_set = 0.0;
			pMold->Clamp.v_set = 0.0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ValveOut.ClampOpn = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->Clamp.Step = 13000;
			break;

		/* --------------- 子模微开模 --------------- */
		case 12100:
			if(pMold->TimeSet.SubMoldMicroOpnTime > 0 && 0 == pMold->TransDIn.ClampOpnMicroLimit)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.SubMoldMicroOpnTime;
			
				pMold->Clamp.vTimeProf.Ts = tTask;
				pMold->Clamp.vTimeProf.t_move = pMold->TimeSet.SubMoldMicroOpnTime * 0.01;
			
				pMold->Clamp.vTimeProf.v_set   = pClampPara->V_OpnMicro * 10.0; 
				pMold->Clamp.vTimeProf.v_start = 0.0; 
				pMold->Clamp.vTimeProf.v_end   = 0.0;
            
				pMold->Clamp.vTimeProf.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vTimeProf.accel <= 0.0)pMold->Clamp.vTimeProf.accel = 1000;
            
				pMold->Clamp.vTimeProf.decel = pClampFix->Open.decel;
				if(pMold->Clamp.vTimeProf.decel <= 0.0)pMold->Clamp.vTimeProf.decel = 1000;
            
				pMold->Clamp.vTimeProf.init = 1;
				pMold->Clamp.Step = 12200;
			}
			else
			{
				pMold->Clamp.Step = 11300;
			}
			break;

		case 12200:
			pMold->ActInfo.ClampOpn  = 1;
			pMold->ValveOut.ClampOpn = 1;
			pMold->ValveOut.ClampOpnDiff = 0;
            
			/* call profile function */
			HydvTimeProf(&pMold->Clamp.vTimeProf);
            
			pMold->Clamp.p_set = pClampPara->P_OpnMicro;
			pMold->Clamp.v_set = pMold->Clamp.vTimeProf.v_out;
         
			pMold->TimeDis.SubMoldMicroOpnTime = pMold->Clamp.Timer.ET;
			
			/*  reached target position */
			if(1 == pMold->Clamp.Timer.Q || 1 == pMold->TransDIn.ClampOpnMicroLimit)
			{
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tSubMoldMicroOpnTime = pMold->Clamp.Timer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tSubMoldMicroOpnTime = pMold->Clamp.Timer.ET/100.0;
				}	
					
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ValveOut.ClampOpn = 0;
				pMold->ValveOut.ClampOpnDiff = 0;
           
				pMold->Clamp.Timer.IN = 0;
				pMold->TimeDis.SubMoldMicroOpnTime = 0;
			
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 11300;
			}
			break;

		case 12300:
			pMold->Clamp.Timer.IN = 0;

			pMold->Clamp.p_set = 0.0;
			pMold->Clamp.v_set = 0.0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ValveOut.ClampOpn = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->Clamp.Step = 13000;
			break;

		case 13000:
			break;

		/* --------------- 调校开模 --------------- */
		case 15000:
			pMold->ActInfo.ClampOpn  = 1;
			pMold->ValveOut.ClampOpn = 1;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->Clamp.p_set = pClampFix->pCalibMax;
			pMold->Clamp.v_set = pClampFix->vCalibMax * 10;
		
			if(pMold->TransDIn.ClampOpnLimit)
			{
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ValveOut.ClampOpn = 0;
				pMold->ValveOut.ClampOpnDiff = 0;
				pMold->Clamp.p_set = 0;
				pMold->Clamp.v_set = 0;
			}
			break;
        
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampOpn  = 0;
                    
			pMold->Clamp.p_old = pMold->Clamp.p_set;
			pMold->Clamp.Step = 21000;
			break;

		case 21000:
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
            
			if(pMold->Clamp.v_set > 0)
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Open.accel_stop;
			}
			else
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Close.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			pMold->Clamp.Step = 22000;
			break;

		case 22000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 0;
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            
			pMold->ValveOut.ClampOpn = 0;
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			
			
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
	            
			pMold->Clamp.vRamp.Ts = tTask;
	            
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			pMold->Clamp.p_old = pMold->Clamp.p_set;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			pMold->Clamp.Step = 31000;
			break;
		
		case 31000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 0;
			}
			break;

		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;

			pMold->Clamp.p_set = 0.0;
			pMold->Clamp.v_set = 0.0;
			pMold->ValveOut.ClampOpn = 0;
			pMold->ValveOut.ClampCls = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			
			break;
		
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
						
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
			
			pMold->Clamp.p_old = pMold->Clamp.p_set;
						
			pMold->Clamp.Step = 42000;
			break;
							
		case 42000:
			pMold->Clamp.vRamp.Internal.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
						
			pMold->Clamp.vRamp.Ts = tTask;
						
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
			pMold->Clamp.Step = 43000;
			break;
						
		case 43000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
					
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
						
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.p_set = 0.0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 40000;
			}
			break;
	}/* end of switch */
   
	if(pMold->Clamp.Step > 300 && pMold->Clamp.Step  < 800)
	{
		pMold->TimeDis.ClampClsAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
	else if(pMold->Clamp.Step > 10300 && pMold->Clamp.Step  < 10800)
	{
		pMold->TimeDis.ClampOpnAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
   
	/* ----------------- clamp open / close timeout --------------------  */
	if(1 == pMold->Clamp.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Clamp.Step < 4000 && pMold->Clamp.Step > 200)		/* clamp close  */
		{
			pMold->Alarm.ClampNotAtClsDI = 1;
		}
      
		if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)	/* clamp open  */
		{
			pMold->Alarm.ClampNotAtOpnDI = 1;
		}
      
		pMold->Clamp.LimitTimer.IN = 0;
		pMold->Clamp.Step = 41000;
	}

	/* ----------------- clamp open / close action limit --------------------  */
   
	if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) 	/* clamp close  */
	{
		/* 子模非开位或合位 */
		if(0 == pMold->TransDIn.SubMoldOpnLimit && 0 == pMold->TransDIn.SubMoldClsLimit)
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->Clamp.Step = 41000;
		}/* if(0 == pMold->TransDIn.SubMoldOpnLimit && 0 == pMold->TransDIn.SubMoldClsLimit) */
      
		
		if(0 == gMachineInfo.Auto )
		{
			if(0 == pMold->Carriage.FwPos)
			{
				pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
				pMold->Clamp.Step = 41000;
			}
		}


		/* 模内吹针在回位   */
		/*if(0 == pMold->TransDIn.InnerPinBwLimit)
		{
		pMold->Alarm.InnerPinNotAtBwPos = !pMold->TransDIn.InnerPinBwLimit;
		pMold->Clamp.Step = 41000;
		}*/
   
		if(1 == gMachineInfo.TimeLimit)
		{
			/* 型胚位  */
			if(0 == pMold->Carriage.FwPos)
			{
				pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
				pMold->Clamp.Step = 41000;
			}
		}
		else
		{
			/* 非架上架下 && 吹针非上位  */
			if(0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos &&
				0 == pMold->BlowPin.UpPos)
			{
				pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
				pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
				pMold->Clamp.Step = 41000;
			}
		}/* if(1 == gMachineInfo.Manual) */
	}/* if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) */

	if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)  /* clamp open  */
	{
		/* 切飞边非退位  */
		if( 0 == pMold->TransDIn.TopDeflashBwLimit)
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Clamp.Step = 41000;
		}
      
	}/* if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200) */
	
	pMold->Clamp.aoPropValve = pMold->Clamp.v_set * 32.767;
	
	
	
}/* end of ClampSwitch() */

void ClampLVDT(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC, Mold_Para_User_typ * pMoldUser)
{
   /*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pClampFix->Length < 100.0)pClampFix->Length = 320.0;
	if(pClampFix->Tolerance < 0.1)pClampFix->Tolerance = 0.1;

	if(pClampFix->Close.V_Max < 500.0)pClampFix->Close.V_Max = 1000.0;
	if(pClampFix->Open.V_Max  < 500.0)pClampFix->Open.V_Max  = 1000.0;
	
	if(pClampFix->Close.VoltageMax > -3.0)pClampFix->Close.VoltageMax = -10.0;
	if(pClampFix->Open.VoltageMax  < 3.0 )pClampFix->Open.VoltageMax  = 10.0;
	
	if(bClampPropValveOutputInverse == 0)
	{
		if(pClampFix->Close.VoltageMax > -3.0)pClampFix->Close.VoltageMax = -10.0;
		if(pClampFix->Close.VoltageMin < -7.0 || pClampFix->Close.VoltageMin > 0)pClampFix->Close.VoltageMin = 0;
		
		if(pClampFix->Open.VoltageMax  < 3.0 )pClampFix->Open.VoltageMax  = 10.0;
		if(pClampFix->Open.VoltageMin  > 7.0 || pClampFix->Open.VoltageMin < 0 )pClampFix->Open.VoltageMin  = 0;
	}
	else
	{
		if(pClampFix->Close.VoltageMax  < 3.0 )pClampFix->Close.VoltageMax  = 10.0;
		if(pClampFix->Close.VoltageMin  > 7.0 || pClampFix->Close.VoltageMin < 0 )pClampFix->Close.VoltageMin  = 0;
		
		if(pClampFix->Open.VoltageMax > -3.0  )pClampFix->Open.VoltageMax = -10.0;
		if(pClampFix->Open.VoltageMin < -7.0 || pClampFix->Open.VoltageMin > 0 )pClampFix->Open.VoltageMin = 0;
	}

	pMold->Clamp.Tolerance = pClampFix->Tolerance;
    
	pMold->Clamp.S_Cls = pClampPara->S_Cls;
	pMold->Clamp.S_ClsEnd = pClampPara->S_ClsEnd;
	
	pMold->Clamp.S_CutterCls = pClampPara->S_CutterCls;
	pMold->Clamp.S_BlowPinUp = pClampPara->S_BlowPinUp;
    
	pMold->Clamp.V_Cls = pClampPara->V_Cls * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_ClsSlow = pClampPara->V_ClsSlow * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_Tonnage = pClampPara->V_Tonnage * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_ClsLock = pClampPara->V_Lock * pClampFix->Close.V_Max/100.0;/*2018.7.17 Albert */
	
	pMold->Clamp.S_OpnSlow2   = pClampPara->S_OpnSlow2;
	pMold->Clamp.S_OpnSlow    = pClampPara->S_OpnSlow;
	pMold->Clamp.S_OpnEnd     = pClampPara->S_OpnEnd;
	
	pMold->Clamp.S_ScrewBw    = pClampPara->S_ScrewBw;
	pMold->Clamp.S_SubMoldOut = pClampPara->S_SubMoldOut;
	
	if(	pClampPara->S_SubMoldIn <= pClampPara->S_OpnSlow2)	pClampPara->S_SubMoldIn   = pClampPara->S_OpnSlow2;
	if(	pClampPara->S_SubMoldIn >= pClampPara->S_OpnEnd)	pClampPara->S_SubMoldIn   = pClampPara->S_OpnEnd;
	pMold->Clamp.S_SubMoldIn   = pClampPara->S_SubMoldIn;
    
	pMold->Clamp.V_Opn = pClampPara->V_Opn * pClampFix->Open.V_Max /100.0;
	pMold->Clamp.V_OpnSlow = pClampPara->V_OpnSlow * pClampFix->Open.V_Max /100.0;
	pMold->Clamp.V_OpnSlow2 = pClampPara->V_OpnSlow2 * pClampFix->Open.V_Max /100.0;
    
	if(0 == pClampPara->V_OpnReset)pClampPara->V_OpnReset = 2.0;
	pMold->Clamp.V_OpnReset = pClampPara->V_OpnReset * pClampFix->Open.V_Max /100.0;
	
	if(pClampFix->Open.accel_stop < 100)pClampFix->Open.accel_stop = INSTANCY_STOP_DECEL; 
	if(pClampFix->Close.accel_stop < 100)pClampFix->Close.accel_stop = INSTANCY_STOP_DECEL;
	/*********************************************************************************
  	**                            linear calibration status                        **
  	*********************************************************************************/
  	/* get calibration data  */
	if(pClampFix->Max < pClampFix->Zero)
	{
		pMold->Clamp.ZeroPos = pClampFix->Max;
		pMold->Clamp.MaxPos = pClampFix->Zero;
	}
	else
	{
		pMold->Clamp.MaxPos  = pClampFix->Max;
		pMold->Clamp.ZeroPos = pClampFix->Zero;
	}

	if(pMold->Clamp.ZeroPos != 0                           && 
		pMold->Clamp.MaxPos > (pMold->Clamp.ZeroPos + 10000) &&
		pMold->Clamp.As     > (pMold->Clamp.ZeroPos - 5000 ) &&
		pMold->Clamp.As     < (pMold->Clamp.MaxPos  + 2000 ))
	{
		pMold->Clamp.CalOK = 1;
	}
	else
	{
		pMold->Clamp.CalOK = 0;
	}

	pClampFix->CalibOK = pMold->Clamp.CalOK;

	if(1 == pMold->Clamp.CalOK)
	{
		if(pClampFix->Max < pClampFix->Zero)
		{
			pMold->Clamp.mmAs = 1.0 *(pMold->Clamp.MaxPos - pMold->Clamp.As) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			//			pMold->Clamp.mmDriveTonnage = 1.0 *(pMold->Clamp.MaxPos - pClampFix->ecMaxForce) * pClampFix->Length /
			//														  (pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			//			if(pMold->Clamp.mmDriveTonnage > 20 || pMold->Clamp.mmDriveTonnage < -20)
			//			{
			//				pMold->Clamp.mmDriveTonnage = 0.0;	
			//			}
		}
		else
		{
			pMold->Clamp.mmAs = 1.0 *(pMold->Clamp.As - pMold->Clamp.ZeroPos) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			//			pMold->Clamp.mmDriveTonnage = 1.0 *(pClampFix->ecMaxForce - pMold->Clamp.ZeroPos) * pClampFix->Length /
			//				                                (pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			//			if(pMold->Clamp.mmDriveTonnage > 20 || pMold->Clamp.mmDriveTonnage < -20)
			//			{
			//				pMold->Clamp.mmDriveTonnage = 0.0;	
			//			}
		}
	}
	else
	{
		pMold->Clamp.mmAs = pMold->Clamp.mmAs;
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->Clamp.Step > 0 && pMold->Clamp.Step < 20000)
			{
				pMold->Clamp.Step = 0;
				pMold->Alarm.ClampCalErr = 1;
			}
		}
	}/* if(1 == pMold->Clamp.CalOK) */
	
	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pClampFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pClampFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	/*  relative position  */
	pMold->Clamp.Ss = pMold->Clamp.As;
	pMold->Clamp.mmAsMax = pClampFix->Length + 10;

	/* gAlarm time value    */
	if(!pMold->TimeSet.ClampOpnAlarmTime)pMold->TimeSet.ClampOpnAlarmTime = 1000;  /* 10s  */
	if(!pMold->TimeSet.ClampClsAlarmTime)pMold->TimeSet.ClampClsAlarmTime = 1000;  /* 10s  */

	/* clamp open or close position flag  */
	pMold->Clamp.ClsPos = pMold->TransDIn.ClampClsLimit &&
		(pMold->Clamp.mmAs < (pMold->Clamp.S_ClsEnd + pMold->Clamp.Tolerance));
	pMold->Clamp.OpnPos = pMold->TransDIn.ClampOpnLimit 
		&& (pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance));	
	
	pMold->Clamp.OpnOverHalf = pMold->Clamp.mmAs >= pClampPara->S_SubMoldOut;
	
	if(pMold->Clamp.mmAsDrive > pMold->Clamp.mmDriveTonnage - 0.03 && pMold->Clamp.mmAsDrive < pMold->Clamp.mmDriveTonnage + 0.03)
	{
		pMold->Clamp.TonnagePos = 1;
	}
	else
	{
		pMold->Clamp.TonnagePos = 0;
	}
	

	/* ------------------ function current position ------------------ */
	pMold->Clamp.vProfGen.s_act = pMold->Clamp.mmAs;
	pMold->Clamp.vProfGen.v_act = pMold->Clamp.v_act;

	pMold->Clamp.OpnHWSwitch =0; //ipis0319
	pMold->Clamp.ClsHWSwitch=0;
	/* clamp open or close step  */
	switch ( pMold->Clamp.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->Clamp.v_set = 0;
			pMold->Clamp.p_set = 0;

			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
								
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			
			
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;
			pMold->Clamp.Timer.IN	   = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			
			break;
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;
		/*--------------------------------------------------------------- */
		/*------------------------ Clamp close -------------------------- */
		/*--------------------------------------------------------------- */
		case 100: /* close delay*/			
			if(pMold->TimeSet.ClampClsDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampClsDelay;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.ClampClsDelay = pMold->TimeSet.ClampClsDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.ClampClsDelay = pMold->TimeSet.ClampClsDelay/100.0;
				}
				//				pMold->TimeDis.ClampClsDelay = 0;
				pMold->Clamp.Step = 200;
			}
			else
			{
				pMold->Clamp.Step = 300;
			}
			break;

		case 200: /* delay ok */
			pMold->TimeDis.ClampClsDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//				pMold->TimeDis.ClampClsDelay = 0;
				pMold->Clamp.Step = 300;
			}
			break;

		case 300:	/* time limit  */
			if(0 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->Clamp.LimitTimer.IN = 1;
				pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampClsAlarmTime;
		
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Close.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Close.Td;
		
				pMold->Clamp.vProfGen.accel = pClampFix->Close.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
		
				pMold->Clamp.vProfGen.decel = pClampFix->Close.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
		            
				pMold->Clamp.vProfGen.decel_end = pClampFix->Close.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
		            
				pMold->Clamp.vProfGen.v_start = 0.0;
			
				pMold->Clamp.vProfGen.v_end	= 0;
				pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ClsEnd;
				pMold->Clamp.vProfGen.init  = 1;
			
				if(pMold->Clamp.S_Cls > 0)  //close quick
				{
					pMold->Clamp.Step = 400;				
				}
				else                        //close slow
				{					
					pMold->Clamp.Step = 500;
				}

			}
			else
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}
			break;

		case 400:	/* close quick  */
			pMold->ActInfo.ClampCls  = 1;
			
			switch (pClampFix->eValveType )
			{
			
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
			/*-- Close --*/
			pMold->ValveOut.ClampCls = 1;
			pMold->ValveOut.ClampClsDiff = 1;
			pMold->ValveOut.ClampClsSlow = 0;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 1;		
			pMold->ValveOut.ClampClsAux = 1;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
				
			pMold->Clamp.p_set = pClampPara->P_Cls; 
			pMold->Clamp.v_set = pMold->Clamp.V_Cls;  

			break;
				
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
			pMold->ValveOut.ClampMoveValve = 1;
				
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 1;
			pMold->ValveOut.ClampClsSlow = 0;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 1;		
			pMold->ValveOut.ClampClsAux = 1;
				
			/*-- Open --*/	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
				
						
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_Cls;
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_Cls; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
	
			pMold->Clamp.actTjCls = pMold->Clamp.vProfGen.Tj;
				
			break;
			}
			
			/*  reached target position */
			if(pMold->Clamp.mmAs < pMold->Clamp.S_Cls)
			{   
				pMold->Clamp.Step = 500;
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 500:	/* close slow  */
			
			//			ValveDelay = ValveDelay+tTask;
			//			if (ValveDelay >0.2)
			//			{
			//			pMold->ValveOut.ClampClsDiff = 0;  /*20181008 IPIS*/
			//				ValveDelay=0;
			//			}
			
			pMold->ActInfo.ClampCls  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
			/*-- Close --*/
			pMold->ValveOut.ClampCls = 1;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 1;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 0;		
			pMold->ValveOut.ClampClsAux = 0;
					
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;

			pMold->Clamp.p_set = pClampPara->P_ClsSlow;
			pMold->Clamp.v_set = pMold->Clamp.V_ClsSlow;  

			break;
				
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
			pMold->ValveOut.ClampMoveValve = 1;
						
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 1;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 0;		
			pMold->ValveOut.ClampClsAux = 0;
						
			/*-- Open --*/	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
						
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_ClsSlow;
            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
            
			pMold->Clamp.p_set = pClampPara->P_ClsSlow;
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
					
			break;
			}
            
			/*  reached target position */
			if(pMold->Clamp.mmAs <= pMold->Clamp.S_ClsEnd + pMold->Clamp.Tolerance)
			{
				pMold->Clamp.v_set = 0;
				//				pMold->Clamp.p_set = 0;
				//				pMold->Clamp.Step  = 510;/*510*/
				if(gMachineInfo.Manual)
				{
					pMold->Clamp.Step = 3400;
				}
				else if(gMachineInfo.Auto)
				{
					pMold->Clamp.Step = 600;
					//					pMold->Clamp.Step = 510;
				}
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 510:/*Holding start*/
			
			if(pMoldUser->TimeSet.ClampHoldingTime != 0) 
			{
				pMold->ActInfo.ClampCls  = 1;
				
				switch (pClampFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/		
						/*-- Close --*/
						pMold->ValveOut.ClampCls = 1;
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;
					
						/*-- Open --*/
						pMold->ValveOut.ClampOpn  = 0;
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;

						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						pMold->ValveOut.ClampMoveValve = 1;
						
						/*-- Close --*/
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;			
								
						/*-- Open --*/
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;
											
						break;
				}

				pMoldUser->Clamp.Timer.IN = 1;/*Start Timer count for holding*/
				pMoldUser->Clamp.Timer.PT = pMoldUser->TimeSet.ClampHoldingTime;
	
				pMold->Clamp.Step = 520;
			}
			else
			{
				pMold->Clamp.Step = 600;
			}
			break;
		case 520:
			
			pMold->ActInfo.ClampCls  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
				
			pMold->ValveOut.ClampCls = 1;
			pMold->Clamp.p_set = pClampPara->P_Lock;/*2018.7.17 Albert Locking Pressure*/
			pMold->Clamp.v_set = pMold->Clamp.V_ClsLock; 
		
			break;
					
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/	
									
			pMold->Clamp.p_set = pClampPara->P_Lock;/*2018.7.17 Albert Locking Pressure*/
			pMold->Clamp.v_set = -pMold->Clamp.V_ClsLock; 
							
			break;
			}
			
			pMoldUser->TimeDis.ClampHoldingTime = pMoldUser->Clamp.Timer.ET; /*Start Timer count for holding*/
			if (1 == pMoldUser->Clamp.Timer.Q)
			{	
				//*--VALVE_DIR--*//
				/*-- Close --*/
				pMold->ValveOut.ClampCls  = 0;
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;
			
			
				//*--VALVE_PROP--*//
				/*-- Close --*/
				pMold->ValveOut.ClampMoveValve = 0;
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;	
			
				pMold->Clamp.p_set = 0;
				pMold->Clamp.v_set = 0;
				
				pMoldUser->Clamp.Timer.IN    = 0;
				pMold->Clamp.Step = 600;
			}
			
			break;
		
		case 600:
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;	
								
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			pMold->Clamp.p_set = 0;
			pMold->Clamp.v_set = 0;
					
			pMold->Clamp.Timer.IN = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				pMold->Clamp.Step = 3000;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				}
			
			}
			else
			{
				pMold->Clamp.Step = 3000;
			}
			break;
 
		
		/*hold and lock Start*/
		/*---------*/
		case 3400:
			/*---------*/
			//lock
			pMold->Lock.Step  = 100;  /*Clamp locking step*/
			//hold   
			if(pMoldUser->TimeSet.ClampHoldingTime != 0) 
			{
				pMold->ActInfo.ClampCls = 1;
				
				switch (pClampFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/		
						/*-- Close --*/
						pMold->ValveOut.ClampCls = 1;
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;
					
						/*-- Open --*/
						pMold->ValveOut.ClampOpn  = 0;
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;

						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						pMold->ValveOut.ClampMoveValve = 1;
						
						/*-- Close --*/
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;	
								
						/*-- Open --*/
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;
											
						break;
				}

				pMoldUser->Clamp.Timer.IN = 1;/*Start Timer count for holding*/
				pMoldUser->Clamp.Timer.PT = pMoldUser->TimeSet.ClampHoldingTime;
	
				pMold->Clamp.Step = 3500;
			}
			else
			{
				pMold->Clamp.Step = 3600;
			}
			break;
		/*---------*/
		case 3500:
			/*---------*/	
			
			pMold->ActInfo.ClampCls  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
				
			pMold->ValveOut.ClampCls = 1;
			pMold->Clamp.p_set = pClampPara->P_Lock;/*2018.7.17 Albert Locking Pressure*/
			pMold->Clamp.v_set = pMold->Clamp.V_ClsLock; 
		
			break;
					
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/	
									
			pMold->Clamp.p_set = pClampPara->P_Lock;/*2018.7.17 Albert Locking Pressure*/
			pMold->Clamp.v_set = -pMold->Clamp.V_ClsLock; 
							
			break;
			}
			
			pMoldUser->TimeDis.ClampHoldingTime = pMoldUser->Clamp.Timer.ET; /*Start Timer count for holding*/
			if (1 == pMoldUser->Clamp.Timer.Q)
			{	
				//*--VALVE_DIR--*//
				/*-- Close --*/
				pMold->ValveOut.ClampCls  = 0;
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;
			
			
				//*--VALVE_PROP--*//
				/*-- Close --*/
				pMold->ValveOut.ClampMoveValve = 0;
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;	
			
				pMold->Clamp.p_set = 0;
				pMold->Clamp.v_set = 0;
				
				pMoldUser->Clamp.Timer.IN    = 0;
				pMold->Clamp.Step = 3600;
			}
			
			break;
		
		/*---------*/
		case 3600:
			/*---------*/	
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			pMold->Clamp.p_set = 0;
			pMold->Clamp.v_set = 0;
					
			pMold->Clamp.Timer.IN = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;
			
			//	pMold->Clamp.Step = 3000;
			pMold->Clamp.Step = 3900;
			break;
		
		case 3000:
			RobotClampTime.IN = 0;
			LRobotClampTime.IN = 0;
			break;

		case 3900:
			
			RobotClampTime.IN = 0;
			LRobotClampTime.IN = 0;
			break;
		

		/* --------------- 调校关模 --------------- */
		case 5000:
			if(0 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->ActInfo.ClampCls  = 1;
			
				switch (pClampFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/
						/*-- Close --*/
						pMold->ValveOut.ClampCls = 1;
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;
					
						/*-- Open --*/
						pMold->ValveOut.ClampOpn  = 0;
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;
						
						pMold->Clamp.p_set = pClampFix->pCalibZero;  
						pMold->Clamp.v_set = pClampFix->vCalibZero*10;
						
						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						pMold->ValveOut.ClampMoveValve = 1;
						
						/*-- Close --*/
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 1;
						
						/*-- Open --*/	
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 0;
						
						pMold->Clamp.p_set = pClampFix->pCalibZero;  
						pMold->Clamp.v_set = -pClampFix->vCalibZero;

						break;
				}
			 	
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		/*-------------------------------------------------------------------------------*/
		/*-------------------------------- 開模洩壓  ----------------------------------*/
		/*-------------------------------------------------------------------------------*/		
		case 12100:/*UnLock Process   開模洩壓*/			
			if(pMoldUser->TimeSet.ClampUnLockTime != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMoldUser->TimeSet.ClampUnLockTime;
				pMold->ValveOut.ClampOpnRelease = 1; /*Open Clamp Release valve*/
				pMold->Clamp.Step 	  = 12110;
			}
			else
			{
				pMold->Clamp.Step = 12150;
			}			
			break;
		
		
		case 12110:/*Wait UnLock Finish*/
			pMoldUser->TimeDis.ClampUnLockTime = pMold->Clamp.Timer.ET;
			if(pMold->Clamp.Timer.Q)
			{
				pMold->ValveOut.ClampOpnRelease = 1; /*Open Clamp Release valve*/
				pMold->Clamp.Timer.IN = 0;
				//				pMold->Clamp.Step = 12130;  // 微開模延遲不使用
				pMold->Clamp.Step = 12150;
			}
			break;
		
		//		case 12130: /* open delay */
		//	
		//			if(pMold->TimeSet.ClampMircoOpnDelay != 0)
		//			{
		//				pMold->Clamp.Timer.IN = 1;
		//				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampMircoOpnDelay;
		//				pMold->Clamp.Step 	  = 12140;
		//			}
		//			else
		//			{
		//				pMold->Clamp.Step = 12150;
		//			}
		//			break;
		//
		//		case 12140: /* delay ok 微開模*/
		//			pMold->TimeDis.ClampMircoOpnDelay = pMold->Clamp.Timer.ET;
		//			if (1 == pMold->Clamp.Timer.Q)
		//			{
		//				pMold->Clamp.Timer.IN        = 0;
		//				pMold->Clamp.Step = 12150;
		//			}
		//			break;
		
		
		case 12150:
			pMold->Clamp.LimitTimer.IN = 1;
			pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;

			pMold->Clamp.vProfGen.Ts = tTask;
			pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
			pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
            
			pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
			if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
            
			pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
			if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
            
			pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
			if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
    
			pMold->Clamp.vProfGen.v_start = 0.0;
			pMold->Clamp.vProfGen.v_end	= 0.0;
			pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnSlow;  
			pMold->Clamp.vProfGen.init  = 1;

			pMold->Clamp.Step = 12200;
			break;
		
		
		case 12200:		/* delay ok 微開模 開到慢轉快位置*/

			pMold->ActInfo.ClampOpn  = 1;
			
			switch (pClampFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Close --*/
					pMold->ValveOut.ClampCls  = 0;
					pMold->ValveOut.ClampClsDiff = 0;
					pMold->ValveOut.ClampClsSlow = 0;
							
					/*-- Open --*/
					pMold->ValveOut.ClampOpn  = 1;	
					pMold->ValveOut.ClampOpnDiff = 0;
					pMold->ValveOut.ClampOpnRelease = 1;
						
					pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
					pMold->Clamp.v_set = pMold->Clamp.V_OpnSlow;   
	
				break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					pMold->ValveOut.ClampMoveValve = 1;
									
					/*-- Close --*/
					pMold->ValveOut.ClampClsDiff = 0;
					pMold->ValveOut.ClampClsSlow = 0;			
														
					/*-- Open --*/
					pMold->ValveOut.ClampOpnDiff = 0;
					pMold->ValveOut.ClampOpnRelease = 1;
						
					pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
				            
					/* call profile function */
					HydvProfGen(& pMold->Clamp.vProfGen);
				            
					pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
					pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
				            
					pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
					
				break;
			}
				

			/*  reached target position */
			
//			if( 1 == gMachineInfo.Auto ) 
//			{
				if( 1 == pMold->Option.SubMold)
				{
					if(pMold->Clamp.mmAs > pMold->Clamp.S_SubMoldOut - 0.1)  // 微開模
					{		     
						pMold->Clamp.v_set  = 0;
						pMold->Clamp.p_set  = 0;
						pMold->Clamp.Step = 12300;
					}
				}
				else
				{
					if(pMold->Clamp.mmAs > pMold->Clamp.S_OpnSlow - 0.1)	 // 慢轉快
					{		     
						pMold->Clamp.v_set  = 0;
						pMold->Clamp.p_set  = 0;
						pMold->Clamp.Step = 12300;
					}
				}
//			}
//			else
//			{
//				//手動不走模芯微開
//				if(pMold->Clamp.mmAs > pMold->Clamp.S_OpnSlow - 0.1)	 // 慢轉快
//				{		     
//					pMold->Clamp.v_set  = 0;
//					pMold->Clamp.p_set  = 0;
//					pMold->Clamp.Step = 10300; // 轉接開模慢快慢
//				}
//				
//			}

			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 12300:
//			pMold->Clamp.LimitTimer.IN = 0;
			pMold->Clamp.Timer.IN = 0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;		
			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;			
										
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			pMold->Clamp.Step = 13000;
			break;

		
		/*-------------------------------------------------------------------------------*/
		/*-------------------------------- Clamp open  ----------------------------------*/
		/*-------------------------------------------------------------------------------*/		
				
		case 10100: /* open delay*/
			if(pMold->TimeSet.ClampOpnDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampOpnDelay;
				pMold->Clamp.Step 	  = 10200;
			
				//gExSPCS
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.ClampOpnDelay = pMold->TimeSet.ClampOpnDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.ClampOpnDelay = pMold->TimeSet.ClampOpnDelay/100.0;
				}
			
			}
			else
			{
				pMold->Clamp.Step = 10300;
			}
			break;

		case 10200: /* delay ok */
			pMold->TimeDis.ClampOpnDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//	pMold->TimeDis.ClampOpnDelay = 0;
				pMold->Clamp.Step = 10300;
			}
			break;

		case 10300:	/* valve open delay  */
			if(0 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->Clamp.LimitTimer.IN = 1;
				if(1 == gMachineInfo.MachineReset)
					/*if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)*/  /* 自動復歸  */
				{
					pMold->Clamp.LimitTimer.PT = pMold->TimeSet.CycleAlarmTime;
				}
				else
				{
					pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;
				}
						
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
			 
				pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
			 
				pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
			 
				pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
			
			
				pMold->Clamp.vProfGen.v_start = 0.0;
			
				/*if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)*/  /* 自動復歸  */
				if(1 == gMachineInfo.MachineReset)
				{
					pMold->Clamp.vProfGen.decel_end = 5.0 * pMold->Clamp.vProfGen.v_set;
					if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
					
					pMold->Clamp.vProfGen.v_end	= 0;
					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEnd;
					pMold->Clamp.vProfGen.init  = 1;		
					pMold->Clamp.Step = 10420;
					
				}
				else if(1 == gMachineInfo.PinCalibration && 1 == gMachineInfo.Calib)
				{
					pMold->Clamp.vProfGen.decel_end = 5.0 * pMold->Clamp.vProfGen.v_set;
					if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
					
					pMold->Clamp.vProfGen.v_end	= 0;
					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEnd;
					pMold->Clamp.vProfGen.init  = 1;
					
					pMold->Clamp.Step = 10420;
				}
				else	
				{
					pMold->Clamp.vProfGen.v_end	= 0;
					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEnd;
					pMold->Clamp.vProfGen.init  = 1;
					
					if (pMold->Clamp.S_OpnSlow > 0)  //open slow
					{		
						pMold->Clamp.Step = 10400;
					}
					else if(pMold->Clamp.S_OpnSlow2 > 0)  //open quick
					{
						pMold->Clamp.Step = 10410;				
					}
					else                                 //open slow 2
					{		
						pMold->Clamp.Step = 10420;
					}
						
				}

			}
			else
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}
			break;
		//		case 10310:/*UnLock Process*/			
		//			if(pMoldUser->TimeSet.ClampUnLockTime != 0)
		//			{
		//				pMold->Clamp.Timer.IN = 1;
		//				pMold->Clamp.Timer.PT = pMoldUser->TimeSet.ClampUnLockTime;
		//				pMold->ValveOut.ClampOpnRelease = 1; /*Open Clamp Release valve*/
		//				pMold->Clamp.Step 	  = 10320;
		//			}
		//			else
		//			{
		//				pMold->Clamp.Step = 10400;
		//			}			
		//			break;
		//		
		//		case 10320:/*Wait UnLock Finish*/
		//			pMoldUser->TimeDis.ClampUnLockTime = pMold->Clamp.Timer.ET;
		//			if(pMold->Clamp.Timer.Q)
		//			{
		//				pMold->Clamp.Timer.IN = 0;
		//				pMold->Clamp.Step = 10400;
		//			}
		//			break;
		case 10400:  /* open slow */
			pMold->ActInfo.ClampOpn  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
					
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 1;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 1;
				
			pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
			pMold->Clamp.v_set = pMold->Clamp.V_OpnSlow;   

			break;
				
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
			pMold->ValveOut.ClampMoveValve = 1;
		
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;			
														
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 1;
				
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
		            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
		            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
				
			break;
			}
            
			/*  reached target position */
			if(pMold->Clamp.mmAs > pMold->Clamp.S_OpnSlow)
			{
				if(pMold->Clamp.S_OpnSlow2>0) 
				{							
					pMold->Clamp.Step = 10410;
				}
				else
				{			
					pMold->Clamp.Step = 10420;								
				}	
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		
		case 10410:  /* open quick  */
			pMold->ActInfo.ClampOpn  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
							
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 1;	
			pMold->ValveOut.ClampOpnDiff = 1;
			pMold->ValveOut.ClampOpnRelease = 1;				
				
			pMold->Clamp.p_set = pClampPara->P_Opn; 
			pMold->Clamp.v_set = pMold->Clamp.V_Opn;   

			break;
				
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
			pMold->ValveOut.ClampMoveValve = 1;
									
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;			
																
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 1;
			pMold->ValveOut.ClampOpnRelease = 1;	
				
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_Opn;
		            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_Opn; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
		            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
				
			break;
			}
            
			/*  reached target position */
			if(pMold->Clamp.mmAs > pMold->Clamp.S_OpnSlow2)
			{    
				pMold->Clamp.Step = 10420;
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		
		case 10420:  /* open slow2  */
			pMold->ActInfo.ClampOpn  = 1;
			
			switch (pClampFix->eValveType )
			{
			case VALVE_DIR: /*0:Direct*/  /*no profile*/
				/*-- Close --*/
				pMold->ValveOut.ClampCls  = 0;
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;
										
				/*-- Open --*/
				pMold->ValveOut.ClampOpn  = 1;	
				pMold->ValveOut.ClampOpnDiff = 0;
				pMold->ValveOut.ClampOpnRelease = 1;			
					
				if(1 == gMachineInfo.MachineReset)
				{
					pMold->Clamp.p_set = pClampPara->P_OpnReset; 
					pMold->Clamp.v_set = pMold->Clamp.V_OpnReset;  
				}
				else
				{
					pMold->Clamp.p_set = pClampPara->P_OpnSlow2; 
					pMold->Clamp.v_set = pMold->Clamp.V_OpnSlow2;  
				}
				

			break;
				
			case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
				pMold->ValveOut.ClampMoveValve = 1;
												
				/*-- Close --*/
				pMold->ValveOut.ClampClsDiff = 0;
				pMold->ValveOut.ClampClsSlow = 0;			
																	
				/*-- Open --*/
				pMold->ValveOut.ClampOpnDiff = 0;
				pMold->ValveOut.ClampOpnRelease = 1;		
					
					
				if(1 == gMachineInfo.MachineReset)
				{
					pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnReset;
					pMold->Clamp.p_set = pClampPara->P_OpnReset; 
				}
				else
				{
					pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow2;
					pMold->Clamp.p_set = pClampPara->P_OpnSlow2; 
				}
			            
				/* call profile function */
				HydvProfGen(& pMold->Clamp.vProfGen);
					
				pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
	            
				pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;

			break;
			}
            
			/*  reached target position */
			if(pMold->Clamp.mmAs > pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance)
			{
				pMold->Clamp.v_set = 0;
				pMold->Clamp.Step = 10500;
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		
            
		case 10500:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpOpn = pMold->Clamp.LimitTimer.ET/100.0;
			
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tClampOpn = pMold->Clamp.LimitTimer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tClampOpn = pMold->Clamp.LimitTimer.ET/100.0;
				}
			
			}
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->Clamp.Step = 10600;
			//			if(pMold->ValveDelay.ClampOpn != 0)
			//			{
			//				pMold->Clamp.Timer.IN = 1;
			//				pMold->Clamp.Timer.PT = pMold->ValveDelay.ClampOpn;
			//				pMold->Clamp.Step = 10600;
			//			}
			//			else
			//			{
			//				pMold->Clamp.Step = 13000;
			//			}
			
			break;
      
		case 10600:

			pMold->Clamp.Timer.IN = 0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;		
			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
							
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;			
												
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			pMold->Clamp.v_set  = 0;
			pMold->Clamp.p_set = 0;
		
			pMold->Clamp.Step = 13000;

			break;

		/* --------------- 绞牙微开模 --------------- */
		case 11100:
			if(1 == pMold->Option.Screw)
			{
				pMold->Clamp.LimitTimer.IN = 1;
				pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;
                
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
                
				pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
                
				pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
                
				pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
                
				pMold->Clamp.vProfGen.v_start = 0.0;
				pMold->Clamp.vProfGen.v_end	= 0.0;
				pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ScrewBw;
				pMold->Clamp.vProfGen.init  = 1;
                
				pMold->Clamp.Step = 11200;
			}
			else
			{
				pMold->Clamp.Step = 13000;
			}
			break;

		case 11200:
			pMold->ActInfo.ClampOpn  = 1;
           
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;/*Close Clamp Release*/


			pMold->ValveOut.ClampOpn  = 1;		
			pMold->ValveOut.ClampOpnDiff = 1;
			pMold->ValveOut.ClampOpnRelease = 0;/*Open Clamp Release valve*/
	
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow;
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
            
			/*  reached target position */
			if(pMold->Clamp.mmAs > pMold->Clamp.S_ScrewBw)
			{
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ValveOut.ClampOpn = 0;
				pMold->ValveOut.ClampOpnDiff = 0;
                
				pMold->Clamp.v_set = 0;
				pMold->Clamp.Step = 11300;
			}
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 11300:
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->Clamp.v_set  = 0;
			pMold->Clamp.p_set =0;
			pMold->ActInfo.ClampOpn  = 0;
			
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;/*Close Clamp Release*/


			pMold->ValveOut.ClampOpn  = 0;		
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;/*Open Clamp Release valve*/
		
			pMold->Clamp.Step = 13000;
			break;
		

		case 13000:
			break;

		/* --------------- 调校开模 --------------- */
		case 15000:
			if(0 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->ActInfo.ClampOpn  = 1;
			
				switch (pClampFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/
						/*-- Close --*/
						pMold->ValveOut.ClampCls = 0;
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 0;
					
						/*-- Open --*/
						pMold->ValveOut.ClampOpn  = 1;
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 1;
						
						pMold->Clamp.p_set = pClampFix->pCalibMax;  
						pMold->Clamp.v_set = pClampFix->vCalibMax*10;
						
						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						pMold->ValveOut.ClampMoveValve = 1;
						
						/*-- Close --*/
						pMold->ValveOut.ClampClsDiff = 0;
						pMold->ValveOut.ClampClsSlow = 0;			
												
						/*-- Open --*/
						pMold->ValveOut.ClampOpnDiff = 0;
						pMold->ValveOut.ClampOpnRelease = 1;
						
						pMold->Clamp.p_set = pClampFix->pCalibMax;  
						pMold->Clamp.v_set = -pClampFix->vCalibMax;

						break;
				}
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
        
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;/*Albert*/

			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            			
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 0;		
			pMold->ValveOut.ClampClsAux = 0;

			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;	
								
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
					
			
			
			pMold->Clamp.p_old = pMold->Clamp.p_set;
			pMold->Clamp.Step = 21000;
			break;

		case 21000:
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
            
			pMold->Clamp.vRamp.Ts = tTask;
            
			if(pMold->Clamp.v_set > 0)
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Open.accel_stop;
			}
			else
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Close.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			pMold->Clamp.Step = 22000;
			break;

		case 22000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{	
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.p_set =0;
				pMold->Clamp.Step = 0;
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;/*Albert*/
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            					
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 0;		
			pMold->ValveOut.ClampClsAux = 0;

			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;	
								
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
		
			
			
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
	            
			pMold->Clamp.vRamp.Ts = tTask;
	            
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			pMold->Clamp.Step = 31000;
			break;
		
		case 31000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.p_set =0;
				pMold->Clamp.Step = 0;
			}
			break;

		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;

			pMold->Clamp.v_set = 0.0;
			pMold->Clamp.p_set =0;
						
			//*--VALVE_DIR--*//
			/*-- Close --*/
			pMold->ValveOut.ClampCls  = 0;
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;
			/*-- Close_Tonnage_additional --*/
			pMold->ValveOut.Lock = 0;		
			pMold->ValveOut.ClampClsAux = 0;

			
			/*-- Open --*/
			pMold->ValveOut.ClampOpn  = 0;	
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
			
			
			//*--VALVE_PROP--*//
			pMold->ValveOut.ClampMoveValve = 0;
			
			/*-- Close --*/
			pMold->ValveOut.ClampClsDiff = 0;
			pMold->ValveOut.ClampClsSlow = 0;	
								
			/*-- Open --*/
			pMold->ValveOut.ClampOpnDiff = 0;
			pMold->ValveOut.ClampOpnRelease = 0;
	
			
			
			break;
		
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
						
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
						
			pMold->Clamp.Step = 42000;
			break;
							
		case 42000:
			pMold->Clamp.vRamp.Internal.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
						
			pMold->Clamp.vRamp.Ts = tTask;
						
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
			pMold->Clamp.Step = 43000;
			break;
						
		case 43000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
					
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
						
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.p_set =0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 40000;
			}
			break;
	}/* end of switch */
   
	if(pMold->Clamp.Step > 300 && pMold->Clamp.Step  < 800)
	{
		pMold->TimeDis.ClampClsAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
	else if((pMold->Clamp.Step > 10300 && pMold->Clamp.Step  < 10800) || (pMold->Clamp.Step > 12100 && pMold->Clamp.Step  < 12300))
	{
		pMold->TimeDis.ClampOpnAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
   
	/* ----------------- clamp open / close timeout --------------------  */
	if(1 == pMold->Clamp.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Clamp.Step < 4000 && pMold->Clamp.Step > 200)		/* clamp close  */
		{
			pMold->Alarm.ClampNotAtClsDI = 1;
		}
      
		if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)	/* clamp open  */
		{
			pMold->Alarm.ClampNotAtOpnDI = 1;
		}
      
		pMold->Clamp.LimitTimer.IN = 0;
		pMold->Clamp.Step = 41000;
	}

	/* ----------------- clamp open / close action limit --------------------  */
   
	if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) 	/* clamp close  */
	{
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if(1 == gMachineInfo.Auto && 1 == gMachineFix.Option.bCoolPinUpNextCycClampCls)   /* 特殊模式  "於下一循環關模後轉一上"  */
		{
			//			if ( 0 == pMold->TransDIn.CoolPin2UpLimit || 0 == pMold->TransDIn.CoolPin3UpLimit )
			//			{						
			//				pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
			//				pMold->Alarm.CoolPin3NotAtUpPos	= !pMold->TransDIn.CoolPin3UpLimit;	
			//				pMold->Clamp.Step = 41000;
			//			}	
		}
		else
		{
			if (0 == pMold->TransDIn.CoolPinUpLimit )
			{						
				pMold->Alarm.CoolPinNotAtUpPos	= !pMold->TransDIn.CoolPinUpLimit;
				pMold->Clamp.Step = 41000;
			}
		
		}

		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->Robot.Transfer.BwPos)
			{
				pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
				pMold->Clamp.Step = 41000;
			}
		}
		
		if(1 == gMachineInfo.Auto )
		{
			if(0 == pMold->Carriage.FwPos)
			{
				pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
				pMold->Clamp.Step = 41000;
			}
		}
		
		if(0 == pMold->TransDIn.ExtInMoldStickerBwLimit || 0 == pMold->TransDIn.ExtInMoldStickerAlarm)	
		{
			pMold->Alarm.ExtInMoldStickerNotAtBw = !pMold->TransDIn.ExtInMoldStickerBwLimit;
			pMold->Alarm.ExtInMoldStickerAlarm 	 = !pMold->TransDIn.ExtInMoldStickerAlarm;
			pMold->Clamp.Step = 41000;
		}
		
		/* 模内吹针在回位   */
		//      if(0 == pMold->TransDIn.InnerPinBwLimit)
		//      {
		//         pMold->Alarm.InnerPinNotAtBwPos = !pMold->TransDIn.InnerPinBwLimit;
		//         pMold->Clamp.Step = 41000;
		//      }
   
		//      if(1 == gMachineInfo.TimeLimit)
		//      {
		//         if(0 == pMold->Carriage.FwPos)
		//         {
		//            pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
		//            pMold->Clamp.Step = 41000;
		//         }
		//      }
		//      else
		//      {
		//         if(0 == pMold->Carriage.BwPos 
		//			&& 0 == pMold->Carriage.FwPos 
		//			&& 0 == pMold->BlowPin.UpPos)
		//         {
		//         	pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//         	pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
		//         	pMold->Clamp.Step = 41000;
		//         }
		//      }/* if(1 == gMachineInfo.Manual) */
	}/* if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) */


	if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)  /* clamp open  */
	{

		/* 切飞边非退位  */
		if( 0 == pMold->TransDIn.TopDeflashBwLimit )
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->TransDIn.ExtRobotUpLmit )	
			{
				pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
				pMold->Clamp.Step = 41000;
			}
		}

		
	}/* if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200) */

	
	
	//	if((13000 >= pMold->Clamp.Step) && (10200 < pMold->Clamp.Step))
	//	{
	//		if(pMold == &RMold)
	//		{
	//			RobotClampTime.IN = pMold->Clamp.OpnPos;/*Albert RobotClampOpn*/
	//			RobotClampTime.PT = RobotClampTimePara;
	//		}
	//		if(pMold == & LMold)
	//		{
	//			LRobotClampTime.IN = pMold->Clamp.OpnPos;
	//			LRobotClampTime.PT = LRobotClampTimePara;
	//		}
	//	}
	//	else
	//	{
	////		RobotClampTime.IN = 0;/*Albert RobotClampOpn*/
	//	}
	//	if(pMold == &RMold)
	//	{
	//		if(RobotClampTime.Q)
	//		{
	//			RobotClampTime.IN = 0;
	//		}
	//		pMold->ValveOut.RobotClampOpn = RobotClampTime.Q;
	//		
	//	}
	//	if(pMold == &LMold)
	//	{
	//		if(LRobotClampTime.Q)
	//		{
	//			LRobotClampTime.IN = 0;
	//		}
	//		pMold->ValveOut.RobotClampOpn = LRobotClampTime.Q;
	//	}
	
	
	/* ------------------------------------------------------------------------------ */
	/* ----------------- clamp output and proportional valve linear  ---------------- */
	/* ------------------------------------------------------------------------------ */
	
	if (pClampFix->eValveType == VALVE_PROP)
	{
		if(bClampPropValveOutputInverse == 0 )
		{
			if((pClampFix->Open.VoltageMax - pClampFix->Open.VoltageMin > 5.0) &&
				(pClampFix->Close.VoltageMin - pClampFix->Close.VoltageMax > 5.0))
			{
				pMold->Clamp.PropValveOk   = 1;
				pMold->Alarm.ClampValveErr = 0;
			}
			else
			{
				pMold->Clamp.PropValveOk   = 0;
				pMold->Alarm.ClampValveErr = 1;
			}
		}
		else
		{
			if((pClampFix->Open.VoltageMin - pClampFix->Open.VoltageMax > 5.0) &&
				(pClampFix->Close.VoltageMax - pClampFix->Close.VoltageMin > 5.0))
			{
				pMold->Clamp.PropValveOk   = 1;
				pMold->Alarm.ClampValveErr = 0;
			}
			else
			{
				pMold->Clamp.PropValveOk   = 0;
				pMold->Alarm.ClampValveErr = 1;
			}
		}
		
		pClampFix->Close.V_Max = 1000;
		pClampFix->Open.V_Max  = 1000;
   
		if(pMold->Clamp.v_set > pClampFix->Open.V_Max) pMold->Clamp.v_set = pClampFix->Open.V_Max;
		if(pMold->Clamp.v_set < -pClampFix->Close.V_Max) pMold->Clamp.v_set = -pClampFix->Close.V_Max;
		
		if(5000 == pMold->Clamp.Step || 15000 == pMold->Clamp.Step || 8500 == pMold->Clamp.Step)
		{
			if(bClampPropValveOutputInverse == 0)
			{
				pMold->Clamp.aoPropValve = pMold->Clamp.v_set * 32767/100;   /* 电压输出  0 -- 100 ---> 0.0 -- 10.0v*/
			}
			else
			{
				pMold->Clamp.aoPropValve = -(pMold->Clamp.v_set * 32767/100);   /* 电压输出  0 -- 100 ---> 0.0 -- 10.0v*/
			}
			
		}
		else
		{
			if(pMold->Clamp.v_set > 0 && pMold->Clamp.v_set <= pClampFix->Open.V_Max)
			{
				if(bClampPropValveOutputInverse == 0)
				{
					//Open +V
					pMold->Clamp.aoPropValve = 3276 * (pClampFix->Open.VoltageMin + pMold->Clamp.v_set/pClampFix->Open.V_Max *
						(pClampFix->Open.VoltageMax - pClampFix->Open.VoltageMin));  
				}
				else
				{
					//Open -V
					pMold->Clamp.aoPropValve = 3276 * (pClampFix->Open.VoltageMin + (-pMold->Clamp.v_set/pClampFix->Open.V_Max) *
						(pClampFix->Open.VoltageMin - pClampFix->Open.VoltageMax));	   
				}
			}
			else if(pMold->Clamp.v_set < 0 && pMold->Clamp.v_set >= -pClampFix->Close.V_Max)
			{
				if(bClampPropValveOutputInverse == 0)
				{
					//Close -V
					pMold->Clamp.aoPropValve = 3276 * (pClampFix->Close.VoltageMin + pMold->Clamp.v_set/pClampFix->Close.V_Max *
						(pClampFix->Close.VoltageMin - pClampFix->Close.VoltageMax));
				}
				else
				{
					//Close +V
					pMold->Clamp.aoPropValve = 3276 * (pClampFix->Close.VoltageMin + (-pMold->Clamp.v_set/pClampFix->Open.V_Max) *
						(pClampFix->Close.VoltageMax - pClampFix->Close.VoltageMin));	   
				}
			}
			else
			{
				pMold->Clamp.aoPropValve = 0;
			}	
		}
	}
	else
	{
		pMold->Alarm.ClampValveErr = 0;
	}
	
}/*end of function */


void ClampDriveToggle_Drive(Mold_typ * pMold, Clamp_Fix_typ * pClampFix,Clamp_Para_typ * pClampPara,SPC_Mold_typ * pSPC, Mold_Para_User_typ * pMoldUser)
{	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pClampFix->pMax = 100;
	}
	else
	{
		pClampFix->pMax = 100;
	}
	
	if(1 == gMachineOut.OilPump)
	{
		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		if(tErrorDelay < tTask)
		{
			if(pMold == & LMold)
			{
				LMold.HwAlarm.ClampNetworkErr = !LMold.Clamp.NetworkInit;
				LMold.HwAlarm.ClampPowerOn    = !LMold.Clamp.PowerOn;			
			}
			else
			{
				RMold.HwAlarm.ClampNetworkErr = !RMold.Clamp.NetworkInit;
				RMold.HwAlarm.ClampPowerOn    = !RMold.Clamp.PowerOn;	
			}
		}
		
		if((RMold.HwAlarm.ClampNetworkErr ||RMold.HwAlarm.ClampPowerOn)||(LMold.HwAlarm.ClampNetworkErr ||LMold.HwAlarm.ClampPowerOn))
		{
			gMachineOut.OilPump = 0;
			gPROCESS_info.oilpumpStop = 1;   //request manual mode
		}
	}
	else
	{
		tErrorDelay = 0.1;
		if(pMold == & LMold)
		{
			//			LMold.HwAlarm.ClampNetworkErr = 0;
			//			LMold.HwAlarm.ClampPowerOn    = 0;		
		}
		else
		{
			//			RMold.HwAlarm.ClampNetworkErr = 0;
			//			RMold.HwAlarm.ClampPowerOn    = 0;
		}
	}
	
	
	/* ------------- clamp axis alarms ------------- */
	
	pMold->HwAlarm.ClampAxisErr    = pMold->Clamp.Error;
	
	
   /*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pClampFix->Length < 100.0)pClampFix->Length = 320.0;
	if(pClampFix->Tolerance < 0.1)pClampFix->Tolerance = 0.1;

	if(pClampFix->Close.V_Max < 500.0)pClampFix->Close.V_Max = 1000.0;
	if(pClampFix->Open.V_Max  < 500.0)pClampFix->Open.V_Max  = 1000.0;
	
	if(pClampFix->Close.VoltageMax > -3.0)pClampFix->Close.VoltageMax = -10.0;
	if(pClampFix->Open.VoltageMax  < 3.0 )pClampFix->Open.VoltageMax  = 10.0;
	
	if(pClampFix->pPress2Hold  < 10 )pClampFix->pPress2Hold  = 10.0;
	if(pClampFix->pPress2Hold  > 70 )pClampFix->pPress2Hold  = 70;

	pMold->Clamp.Tolerance = pClampFix->Tolerance;
    
	pMold->Clamp.S_Cls = pClampPara->S_Cls;
	pMold->Clamp.S_ClsEnd = pClampPara->S_ClsEnd;
	
	pMold->Clamp.S_CutterCls = pClampPara->S_CutterCls;
	pMold->Clamp.S_BlowPinUp = pClampPara->S_BlowPinUp;
    
	pMold->Clamp.V_Cls = pClampPara->V_Cls * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_ClsSlow = pClampPara->V_ClsSlow * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_Tonnage = pClampPara->V_Tonnage * pClampFix->Close.V_Max /100.0;
	pMold->Clamp.V_ClsLock = pClampPara->V_Lock * pClampFix->Close.V_Max/100.0;/*2018.7.17 Albert */
	
	pMold->Clamp.P_Cls		= pClampPara->P_Cls 	= 100;
	pMold->Clamp.P_ClsSlow	= pClampPara->P_ClsSlow = 100;
	pMold->Clamp.P_Tonnage	= pClampPara->P_Tonnage;
	pMold->Clamp.P_Lock		= pClampPara->P_Lock;

	
	pMold->Clamp.S_OpnSlow2   = pClampPara->S_OpnSlow2;
	pMold->Clamp.S_OpnSlow    = pClampPara->S_OpnSlow;
	pMold->Clamp.S_OpnEnd     = pClampPara->S_OpnEnd;
	
	pMold->Clamp.S_ScrewBw    = pClampPara->S_ScrewBw;
	pMold->Clamp.S_SubMoldOut = pClampPara->S_SubMoldOut;
	
	if(	pClampPara->S_SubMoldIn <= pClampPara->S_OpnSlow2)	pClampPara->S_SubMoldIn   = pClampPara->S_OpnSlow2;
	if(	pClampPara->S_SubMoldIn >= pClampPara->S_OpnEnd)	pClampPara->S_SubMoldIn   = pClampPara->S_OpnEnd;
	pMold->Clamp.S_SubMoldIn   = pClampPara->S_SubMoldIn;
    
	pMold->Clamp.V_Opn = pClampPara->V_Opn * pClampFix->Open.V_Max /100.0;
	pMold->Clamp.V_OpnSlow = pClampPara->V_OpnSlow * pClampFix->Open.V_Max /100.0;
	pMold->Clamp.V_OpnSlow2 = pClampPara->V_OpnSlow2 * pClampFix->Open.V_Max /100.0;
	
	pMold->Clamp.P_Opn		= pClampPara->P_Opn			= 100;
	pMold->Clamp.P_OpnSlow	= pClampPara->P_OpnSlow 	= 100;
	pMold->Clamp.P_OpnSlow2	= pClampPara->P_OpnSlow2	= 100;
    
	if(0 == pClampPara->V_OpnReset)pClampPara->V_OpnReset = 2.0;
	pMold->Clamp.V_OpnReset = pClampPara->V_OpnReset * pClampFix->Open.V_Max /100.0;
	pClampPara->P_OpnReset	= 100;
	
	if(pClampFix->Open.accel_stop < 100)pClampFix->Open.accel_stop = INSTANCY_STOP_DECEL; 
	if(pClampFix->Close.accel_stop < 100)pClampFix->Close.accel_stop = INSTANCY_STOP_DECEL;
	/*********************************************************************************
  	**                            linear calibration status                        **
  	*********************************************************************************/
  	/* get calibration data  */
	if(pClampFix->Max < pClampFix->Zero)
	{
		pMold->Clamp.ZeroPos = pClampFix->Max;
		pMold->Clamp.MaxPos = pClampFix->Zero;
	}
	else
	{
		pMold->Clamp.MaxPos  = pClampFix->Max;
		pMold->Clamp.ZeroPos = pClampFix->Zero;
	}

	if(pMold->Clamp.ZeroPos != 0                           && 
		pMold->Clamp.MaxPos > (pMold->Clamp.ZeroPos + 10000) &&
		pMold->Clamp.As     > (pMold->Clamp.ZeroPos - 5000 ) &&
		pMold->Clamp.As     < (pMold->Clamp.MaxPos  + 2000 ))
	{
		pMold->Clamp.CalOK = pMold->Clamp.NetworkInit;
	}
	else
	{
		pMold->Clamp.CalOK = 0;
	}

	pClampFix->CalibOK = pMold->Clamp.CalOK;

	if(1 == pMold->Clamp.CalOK)
	{
		if(pClampFix->Max < pClampFix->Zero)
		{
			pMold->Clamp.mmAsDrive = 1.0 *(pMold->Clamp.MaxPos - pMold->Clamp.As) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			pMold->Clamp.mmDriveTonnage = 1.0 *(pMold->Clamp.MaxPos - pClampFix->ecMaxForce) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			if(pMold->Clamp.mmDriveTonnage > 20 || pMold->Clamp.mmDriveTonnage < -20)
			{
				pMold->Clamp.mmDriveTonnage = 0.0;	
			}
		}
		else
		{
			pMold->Clamp.mmAsDrive = 1.0 *(pMold->Clamp.As - pMold->Clamp.ZeroPos) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			pMold->Clamp.mmDriveTonnage = 1.0 *(pClampFix->ecMaxForce - pMold->Clamp.ZeroPos) * pClampFix->Length /
				(pMold->Clamp.MaxPos - pMold->Clamp.ZeroPos);
			
			if(pMold->Clamp.mmDriveTonnage > 20 || pMold->Clamp.mmDriveTonnage < -20)
			{
				pMold->Clamp.mmDriveTonnage = 0.0;	
			}
		}
	}
	else
	{
		pMold->Clamp.mmAsDrive = pMold->Clamp.mmAsDrive;

		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->Clamp.Step > 0 && pMold->Clamp.Step < 20000)
			{
				pMold->Clamp.Step = 0;
				pMold->Alarm.ClampCalErr = 1;
			}
		}
	}/* if(1 == pMold->Clamp.CalOK) */
	
	/*  toggle clamp linear  */
	pClampFix->ToggleLin[0].X = 0.0;
	pClampFix->ToggleLin[0].Y = 0.0;
	
	pClampFix->ToggleLin[ToggleLinNodeNum-1].X = pClampFix->Length;
	pClampFix->ToggleLin[ToggleLinNodeNum-1].Y = pClampFix->Length;
	
	
	
	if(pMold == &LMold)
	{
		TogLinearL.Y = pMold->Clamp.mmAsDrive;
		HydLinear(&TogLinearL);

		if(TogLinearL.status != 0)
		{
			pMold->Alarm.ClampLinNotOK = 1;
			pMold->Clamp.mmAs = pMold->Clamp.mmAsDrive;
		}
		else
		{
			pMold->Clamp.mmAs = TogLinearL.X;
		}
		
		//		if(pMold->Clamp.As < pMold->Clamp.ZeroPos)
		//		{
		//			pMold->Clamp.mmAs = pMold->Clamp.mmAsDrive;
		//		}
	}
	else if(pMold == &RMold)
	{
		TogLinearR.Y = pMold->Clamp.mmAsDrive;
		HydLinear(&TogLinearR);
	
		if(TogLinearR.status != 0)
		{
			pMold->Alarm.ClampLinNotOK = 1;
			pMold->Clamp.mmAs = pMold->Clamp.mmAsDrive;
		}
		else
		{
			pMold->Clamp.mmAs = TogLinearR.X;
		}
		
		//		if(pMold->Clamp.As < pMold->Clamp.ZeroPos)
		//		{
		//			pMold->Clamp.mmAs = pMold->Clamp.mmAsDrive;
		//		}
	}
	
	/*  relative position  */
	pMold->Clamp.Ss = pMold->Clamp.As;
	pMold->Clamp.mmAsMax = pClampFix->Length + 10;

	/* gAlarm time value    */
	if(!pMold->TimeSet.ClampOpnAlarmTime)pMold->TimeSet.ClampOpnAlarmTime = 1000;  /* 10s  */
	if(!pMold->TimeSet.ClampClsAlarmTime)pMold->TimeSet.ClampClsAlarmTime = 1000;  /* 10s  */

	/* clamp open or close position flag  */
	pMold->Clamp.ClsPos = pMold->TransDIn.ClampClsLimit &&
		(pMold->Clamp.mmAs < (pMold->Clamp.S_ClsEnd + pMold->Clamp.Tolerance));
	pMold->Clamp.OpnPos = pMold->TransDIn.ClampOpnLimit &&
		(pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance));	
	
	pMold->Clamp.OpnOverHalf = pMold->Clamp.mmAs >= pClampPara->S_SubMoldOut;
	
	if(pMold->Clamp.mmAsDrive > pMold->Clamp.mmDriveTonnage - 0.03 && pMold->Clamp.mmAsDrive < pMold->Clamp.mmDriveTonnage + 0.03)
	{
		pMold->Clamp.TonnagePos = 1;
	}
	else
	{
		pMold->Clamp.TonnagePos = 0;
	}
	

	/* ------------------ function current position ------------------ */
	pMold->Clamp.vProfGen.s_act = pMold->Clamp.mmAsDrive;
	pMold->Clamp.vProfGen.v_act = pMold->Clamp.v_act; // mmAsDrive -> v_act

	/* clamp open or close step  */
	switch ( pMold->Clamp.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->Clamp.v_set = 0;
			pMold->Clamp.p_set = 0;

			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;
			
			pMold->Clamp.Timer.IN	   = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			
			break;
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;
		/*--------------------------------------------------------------- */
		/*------------------------ Clamp close -------------------------- */
		/*--------------------------------------------------------------- */
		case 100: /* close delay*/			
			if(pMold->TimeSet.ClampClsDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampClsDelay;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.ClampClsDelay = pMold->TimeSet.ClampClsDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.ClampClsDelay = pMold->TimeSet.ClampClsDelay/100.0;
				}
				//				pMold->TimeDis.ClampClsDelay = 0;
				pMold->Clamp.Step = 200;
			}
			else
			{
				pMold->Clamp.Step = 300;
			}
			break;

		case 200: /* delay ok */
			pMold->TimeDis.ClampClsDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//				pMold->TimeDis.ClampClsDelay = 0;
				pMold->Clamp.Step = 300;
			}
			break;

		case 300:	/* time limit  */
			if(0 == pMold->Clamp.ClsHWSwitch)
			{
				if(pMold == &LMold)
				{
					pMold->Clamp.S_ClsDrive  = xTogLinearLTrans(pClampPara->S_Cls);
					pMold->Clamp.S_ClsEndDrive = xTogLinearLTrans(pClampPara->S_ClsEnd);
				}
				else
				{
					pMold->Clamp.S_ClsDrive  = xTogLinearRTrans(pClampPara->S_Cls);
					pMold->Clamp.S_ClsEndDrive = xTogLinearRTrans(pClampPara->S_ClsEnd);
				}
			
				pMold->Clamp.LimitTimer.IN = 1;
				pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampClsAlarmTime;
		
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Close.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Close.Td;
		
				pMold->Clamp.vProfGen.accel = pClampFix->Close.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
		
				pMold->Clamp.vProfGen.decel = pClampFix->Close.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
		            
				pMold->Clamp.vProfGen.decel_end = pClampFix->Close.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
		            
				pMold->Clamp.vProfGen.v_start = 0.0;

			
				if (pMold->Clamp.S_Cls >0) //close quick
				{
					//					pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_ClsSlow;
					//					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ClsDrive;
					//					pMold->Clamp.vProfGen.init  = 1;
					
					if(pMold->TimeSet.ClampLockTime == 0 || 1 == gMachineInfo.Manual )
					{
						pMold->Clamp.vProfGen.v_end = 0;
//						pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Tonnage/2;
					}
					else
					{
						pMold->Clamp.vProfGen.v_end = 0;
//						pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Tonnage/2;
					}
					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ClsEndDrive;
					pMold->Clamp.vProfGen.init  = 1;
						
					pMold->Clamp.Step = 400;	
				}
				else   							//close slow
				{
					if(pMold->TimeSet.ClampLockTime == 0 || 1 == gMachineInfo.Manual )
					{
						pMold->Clamp.vProfGen.v_end = 0;
//						pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Tonnage/2;
					}
					else
					{
						pMold->Clamp.vProfGen.v_end = 0;
//						pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Tonnage/2;
					}
					pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ClsEndDrive;
					pMold->Clamp.vProfGen.init  = 1;	
						
					pMold->Clamp.Step = 500;
				}

			}
			else
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}
			break;

		case 400:	/* close quick  */
			pMold->ActInfo.ClampCls  = 1;
									
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_Cls;
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_Cls = 100.0; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
	
			pMold->Clamp.actTjCls = pMold->Clamp.vProfGen.Tj;
				

			/*  reached target position */
			if(pMold->Clamp.vProfGen.s_act <= pMold->Clamp.S_ClsDrive)
			{   
				//				pMold->Clamp.vProfGen.v_start = pMold->Clamp.v_set;
				//				if(pMold->TimeSet.ClampLockTime == 0 || 1 == gMachineInfo.Manual )
				//				{
				//					pMold->Clamp.vProfGen.v_end = 0;
				//				}
				//				else
				//				{
				//					pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Tonnage/2;
				//				}
				//				pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ClsEndDrive;
				//				pMold->Clamp.vProfGen.init  = 1;
			
			
				pMold->Clamp.Step = 500;
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 500:	/* close slow  */
			
			pMold->ActInfo.ClampCls  = 1;
						
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_ClsSlow;
            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
            
			pMold->Clamp.p_set = pClampPara->P_ClsSlow;
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;

            
			/*  reached target position */
			if(pMold->Clamp.vProfGen.s_act <= pMold->Clamp.vProfGen.s_end + pMold->Clamp.Tolerance)
			{
				pMold->Clamp.v_set = 0;
				pMold->Clamp.LimitTimer.IN = 0;
				if(gMachineInfo.Manual)
				{
					pMold->Clamp.Step = 3400;
				}
				else if(gMachineInfo.Auto)
				{
					pMold->Clamp.Step = 600;
				}
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
	
		case 600:
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;
			
			pMold->Clamp.p_set = 0;
			pMold->Clamp.v_set = 0;
					
			pMold->Clamp.Timer.IN = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				pMold->Clamp.Step = 3000;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tCalmpCls = pMold->Clamp.LimitTimer.ET/100.0;
				}
			
			}
			else
			{
				pMold->Clamp.Step = 3000;
			}
			break;
 
		
		/* lock Start （加壓）*/
		/*---------*/
		case 3400:
			/*---------*/
			//lock
			if(pMold->TimeSet.ClampLockTime != 0)
			{
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ActInfo.ClampCls  = 1;

	            
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampLockTime;
	            
				pMold->Clamp.vTimeProf.Ts = tTask;
				pMold->Clamp.vTimeProf.t_move = pMold->TimeSet.ClampLockTime * 0.01;
	            
				pMold->Clamp.vTimeProf.v_set   = pMold->Clamp.V_Tonnage; 
				pMold->Clamp.vTimeProf.v_start = pMold->Clamp.v_set; 
				pMold->Clamp.vTimeProf.v_end   = 0.0;
	            
				pMold->Clamp.vTimeProf.accel = 1000;
				if(pMold->Clamp.vTimeProf.accel <= 0.0)pMold->Clamp.vTimeProf.accel = 1000;
	            
				pMold->Clamp.vTimeProf.decel = 1000;
				if(pMold->Clamp.vTimeProf.decel <= 0.0)pMold->Clamp.vTimeProf.decel = 1000;
	            
				pMold->Clamp.vTimeProf.init = 1;
				pMold->Clamp.Step = 3500;
			}
			else
			{
				pMold->Clamp.Step = 3800;  //3900
			}
			break;
		
		/*---------*/
		case 3500:	 /*(Press torque procedure)*/
			/*---------*/	
			
			pMold->ActInfo.ClampCls  = 1;


			HydvTimeProf(&pMold->Clamp.vTimeProf);
				
			pMold->Clamp.p_set = pMold->Clamp.P_Tonnage;
			pMold->Clamp.v_set = -pMold->Clamp.vTimeProf.v_out;

			pMold->TimeDis.ClampLockTime = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
//				pMold->Clamp.v_set = 0;
				pMold->Clamp.Timer.IN      = 0;
				pMold->Clamp.Step = 3600;
			}
			
//			pMold->TimeDis.ClampClsAlarmTime = pMold->Clamp.LimitTimer.ET;

			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}
			break;
		
		/*  --------------------------------------------------------------- */ 
		/*  -------------- Press torque 2 procedure Set ------------------- */
		case 3600:	/*(Press torque 2 procedure Set)*/
			/*---------*/
			//lock
			if(pMold->TimeSet.ClampLockTime != 0)
			{
				pMold->ActInfo.ClampOpn  = 0;
				pMold->ActInfo.ClampCls  = 1;

	            
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampClsPress2HoldTime;
	            
				pMold->Clamp.vTimeProf.Ts = tTask;
				pMold->Clamp.vTimeProf.t_move = pMold->TimeSet.ClampClsPress2HoldTime * 0.01;
	            
				pMold->Clamp.vTimeProf.v_set   = pMold->Clamp.V_Tonnage; 
				pMold->Clamp.vTimeProf.v_start = pMold->Clamp.v_set; 
				pMold->Clamp.vTimeProf.v_end   = 0.0;
	            
				pMold->Clamp.vTimeProf.accel = Test_vTimeProf_accel;
				if(pMold->Clamp.vTimeProf.accel <= 0.0)pMold->Clamp.vTimeProf.accel = 1000;
	            
				pMold->Clamp.vTimeProf.decel = Test_vTimeProf_decel;
				if(pMold->Clamp.vTimeProf.decel <= 0.0)pMold->Clamp.vTimeProf.decel = 1000;
	            
				pMold->Clamp.vTimeProf.init = 1;
				pMold->Clamp.Step = 3700;
			}
			else
			{
				pMold->Clamp.Step = 3800;  //3900
			}
			break;
		
		/*---------*/
		case 3700:	 /*(Press torque 2 procedure)*/
		/*---------*/	
			
			pMold->ActInfo.ClampCls  = 1;

			pMold->TimeDis.ClampClsPress2HoldTime = pMold->Clamp.Timer.ET;
			HydvTimeProf(&pMold->Clamp.vTimeProf);
				
			pMold->Clamp.p_set = pClampFix->pPress2Hold;
			pMold->Clamp.v_set = -pMold->Clamp.vTimeProf.v_out;

//			pMold->TimeDis.ClampLockTime = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.v_set = 0;
				pMold->Clamp.Timer.IN      = 0;
				pMold->Clamp.Step = 3800;
			}
			
//			pMold->TimeDis.ClampClsAlarmTime = pMold->Clamp.LimitTimer.ET;

			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step 	= 41000;
			}
			else if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step 	= 41000;
			}
			break;
			
		case 3800: //End 
			pMold->ActInfo.ClampCls  = 0;
			pMold->ActInfo.ClampOpn  = 0;
			
			pMold->Clamp.Timer.IN   = 0;
			pMold->Clamp.LimitTimer.IN = 0;

			pMold->Clamp.v_set = 0;
            
			pMold->Clamp.Step = 3900;
			break;
		
		
		case 3000:
			RobotClampTime.IN = 0;
			LRobotClampTime.IN = 0;
			break;

		case 3900:
			
			RobotClampTime.IN = 0;
			LRobotClampTime.IN = 0;
			break;
		

		/* --------------- 调校关模 --------------- */
		case 5000:
			if(0 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->ActInfo.ClampCls  = 1;
		
				pMold->Clamp.p_set = pClampFix->pCalibZero;  
				pMold->Clamp.v_set = -pClampFix->vCalibZero*10;
			}
			
			if(1 == pMold->Clamp.ClsHWSwitch)
			{
				pMold->HwAlarm.ClampClsHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		/* --------------- 绞牙微开模 --------------- */
		case 11100:
			if(1 == pMold->Option.Screw)
			{
				pMold->Clamp.LimitTimer.IN = 1;
				pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;
                
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
                
				pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
                
				pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
                
				pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
                
				pMold->Clamp.vProfGen.v_start = 0.0;
				pMold->Clamp.vProfGen.v_end	= 0.0;
				pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_ScrewBw;
				pMold->Clamp.vProfGen.init  = 1;
                
				pMold->Clamp.Step = 11200;
			}
			else
			{
				pMold->Clamp.Step = 13000;
			}
			break;

		case 11200:
			pMold->ActInfo.ClampOpn  = 1;
           
	
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow;
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
            
			/*  reached target position */
			if(pMold->Clamp.mmAs > pMold->Clamp.S_ScrewBw)
			{
				pMold->ActInfo.ClampOpn  = 0;

                
				pMold->Clamp.v_set = 0;
				pMold->Clamp.Step = 11300;
			}
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 11300:
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->Clamp.v_set  = 0;
			pMold->Clamp.p_set =0;
			pMold->ActInfo.ClampOpn  = 0;
			

		
			pMold->Clamp.Step = 13000;
			break;
		
		/*-------------------------------------------------------------------------------*/
		/*----------------------- 開模洩壓 + 子模微開模 ---------------------------------*/
		/*-------------------------------------------------------------------------------*/		
		case 12100:/*UnLock Process   開模洩壓*/			
			//			if(pMoldUser->TimeSet.ClampUnLockTime != 0)
			//			{
			//				pMold->Clamp.Timer.IN = 1;
			//				pMold->Clamp.Timer.PT = pMoldUser->TimeSet.ClampUnLockTime;
			//				pMold->Clamp.Step 	  = 12110;
			//			}
			//			else
			//			{
			if( 1 == pMold->Option.SubMold)
			{
				pMold->Clamp.Step = 12130; //微開延遲+ 微開
			}
			else
			{
				pMold->Clamp.Step = 12300; //微開延遲+ 微開 跳過
			}
				
			//			}			
			break;
		
		
		//		case 12110:/*Wait UnLock Finish*/
		//			pMoldUser->TimeDis.ClampUnLockTime = pMold->Clamp.Timer.ET;
		//			if(pMold->Clamp.Timer.Q)
		//			{
		//				pMold->Clamp.Timer.IN = 0;
		//				//				pMold->Clamp.Step = 12130;  // 微開模延遲不使用
		//				pMold->Clamp.Step = 12150;
		//			}
		//			break;
		
		case 12130: /* open delay */
			if(pMold->TimeSet.ClampMircoOpnDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampMircoOpnDelay;
				pMold->Clamp.Step 	  = 12140;
			}
			else
			{
				pMold->Clamp.Step = 12150;
			}
			break;

		case 12140: /* delay ok 微開模*/
			pMold->TimeDis.ClampMircoOpnDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				pMold->Clamp.Step = 12150;
			}
			break;
		
		
		case 12150:
			pMold->Clamp.LimitTimer.IN = 1;
			pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;

			pMold->Clamp.vProfGen.Ts = tTask;
			pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
			pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
            
			pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
			if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
            
			pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
			if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
            
			pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
			if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
    
			pMold->Clamp.vProfGen.v_start = 0.0;
			pMold->Clamp.vProfGen.v_end	= 0.0;
			
			pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_SubMoldOut; // 微開模

			pMold->Clamp.vProfGen.init  = 1;

			pMold->Clamp.Step = 12200;
			break;
		
		
		case 12200:		/* 開到 微開模 位置*/

			pMold->ActInfo.ClampOpn  = 1;

			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
		            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
		            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;

			/*  reached target position */
			
			if(pMold->Clamp.mmAs > pMold->Clamp.S_SubMoldOut - 0.1)  // 微開模
			{		     
				pMold->Clamp.v_set  = 0;
				pMold->Clamp.p_set  = 0;
				pMold->Clamp.Step = 12300;
			}

			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;

		case 12300:
			pMold->Clamp.LimitTimer.IN = 0;
			pMold->Clamp.Timer.IN = 0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;		
			
			pMold->Clamp.v_set  = 0;
			pMold->Clamp.p_set  = 0;
			
			pMold->Clamp.Step = 13000;
			break;

		
		/*-------------------------------------------------------------------------------*/
		/*-------------------------------- Clamp open  ----------------------------------*/
		/*-------------------------------------------------------------------------------*/		
		case 10100: /* open delay*/
			if(pMold->TimeSet.ClampOpnDelay != 0)
			{
				pMold->Clamp.Timer.IN = 1;
				pMold->Clamp.Timer.PT = pMold->TimeSet.ClampOpnDelay;
				pMold->Clamp.Step 	  = 10200;
			
				//gExSPCS
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.ClampOpnDelay = pMold->TimeSet.ClampOpnDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.ClampOpnDelay = pMold->TimeSet.ClampOpnDelay/100.0;
				}
			
			}
			else
			{
				pMold->Clamp.Step = 10300;
			}
			break;

		case 10200: /* delay ok */
			pMold->TimeDis.ClampOpnDelay = pMold->Clamp.Timer.ET;
			if (1 == pMold->Clamp.Timer.Q)
			{
				pMold->Clamp.Timer.IN        = 0;
				//	pMold->TimeDis.ClampOpnDelay = 0;
				pMold->Clamp.Step = 10300;
			}
			break;
				
		case 10300:	/* valve open delay  */
			if(0 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->Clamp.rAfterPress2_Tolerance = (pClampFix->pPress2Hold/10) *0.2;
				if(pMold == &LMold)
				{
					pMold->Clamp.S_OpnSlowDrive  = xTogLinearLTrans(pClampPara->S_OpnSlow);
					pMold->Clamp.S_OpnEndDrive 	 = xTogLinearLTrans(pClampPara->S_OpnEnd);
				}
				else
				{
					pMold->Clamp.S_OpnSlowDrive  = xTogLinearRTrans(pClampPara->S_OpnSlow);
					pMold->Clamp.S_OpnEndDrive 	 = xTogLinearRTrans(pClampPara->S_OpnEnd);
				}
			
				pMold->Clamp.LimitTimer.IN = 1;
				if(1 == gMachineInfo.MachineReset)
					/*if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)*/  /* 自動復歸  */
				{
					pMold->Clamp.LimitTimer.PT = pMold->TimeSet.CycleAlarmTime;
				}
				else
				{
					pMold->Clamp.LimitTimer.PT = pMold->TimeSet.ClampOpnAlarmTime;
				}
						
				pMold->Clamp.vProfGen.Ts = tTask;
				pMold->Clamp.vProfGen.Tj = pClampFix->Open.Tj;
				pMold->Clamp.vProfGen.Td = pClampFix->Open.Td;
			 
				pMold->Clamp.vProfGen.accel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.accel <= 0.0)pMold->Clamp.vProfGen.accel = 1000;
			 
				pMold->Clamp.vProfGen.decel = pClampFix->Open.accel;
				if(pMold->Clamp.vProfGen.decel <= 0.0)pMold->Clamp.vProfGen.decel = 1000;
			 
				pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;
				if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
			
			
				pMold->Clamp.vProfGen.v_start = 0.0;
			
				/*if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)*/  /* 自動復歸  */
				if(1 == gMachineInfo.MachineReset)
				{
//					pMold->Clamp.vProfGen.decel_end = 5.0 * pMold->Clamp.vProfGen.v_set;
					pMold->Clamp.vProfGen.decel_end = pClampFix->Open.decel_end;	
					if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
					
					pMold->Clamp.vProfGen.v_end	= 0;
					if(pMold->TimeSet.ClampLockTime == 0 )
					{
						pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive;
					}
					else if(pMold->TimeSet.ClampLockTime != 0 )
					{
						pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive + pMold->Clamp.rAfterPress2_Tolerance;
					}
					pMold->Clamp.vProfGen.init  = 1;		
					pMold->Clamp.Step = 10420;
					
				}
				else if(1 == gMachineInfo.PinCalibration && 1 == gMachineInfo.Calib)
				{
					pMold->Clamp.vProfGen.decel_end = 5.0 * pMold->Clamp.vProfGen.v_set;
					if(pMold->Clamp.vProfGen.decel_end <= 0.0)pMold->Clamp.vProfGen.decel_end = 100;
					
					pMold->Clamp.vProfGen.v_end	= 0;
					if(pMold->TimeSet.ClampLockTime == 0 )
					{
						pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive;
					}
					else if(pMold->TimeSet.ClampLockTime != 0 )
					{
						pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive + pMold->Clamp.rAfterPress2_Tolerance;
					}
					pMold->Clamp.vProfGen.init  = 1;
					
					pMold->Clamp.Step = 10420;
				}
				else	
				{
					if (pMold->Clamp.S_OpnSlow >0) //open slow
					{
						//						pMold->Clamp.vProfGen.v_end	= pMold->Clamp.V_Opn;
						//						pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnSlowDrive;
						//						pMold->Clamp.vProfGen.init  = 1;	
						
						pMold->Clamp.vProfGen.v_end	= 0.0;
						if(pMold->TimeSet.ClampLockTime == 0 )
						{
							pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive;
						}
						else if(pMold->TimeSet.ClampLockTime != 0 )
						{
							pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive + pMold->Clamp.rAfterPress2_Tolerance;
						}
						pMold->Clamp.vProfGen.init  = 1;
						
						pMold->Clamp.Step = 10400;	
					}
					else   					      //open quick
					{
						pMold->Clamp.vProfGen.v_end	= 0.0;
						if(pMold->TimeSet.ClampLockTime == 0 )
						{
							pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive;
						}
						else if(pMold->TimeSet.ClampLockTime != 0 )
						{
							pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive + pMold->Clamp.rAfterPress2_Tolerance;
						}
						pMold->Clamp.vProfGen.init  = 1;	
						
						pMold->Clamp.Step = 10420;
					}		
				}
			}
			else
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}
			break;
		
		case 10400:  /* open slow */
			pMold->ActInfo.ClampOpn  = 1;
				
			pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnSlow;
		            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
		            
			pMold->Clamp.p_set = pClampPara->P_OpnSlow; 
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
		            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;
				
			/*  reached target position */
			if(pMold->Clamp.vProfGen.s_act >= pMold->Clamp.S_OpnSlowDrive)
			{   
				//				pMold->Clamp.vProfGen.v_start = pMold->Clamp.v_set;
				//				pMold->Clamp.vProfGen.v_end	= 0.0;
				//				pMold->Clamp.vProfGen.s_end = pMold->Clamp.S_OpnEndDrive;
				//				pMold->Clamp.vProfGen.init  = 1;
			
				pMold->Clamp.Step = 10420;
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		
		case 10420:  /* open quick  */
			pMold->ActInfo.ClampOpn  = 1;
				
			if(1 == gMachineInfo.MachineReset)
			{
				pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_OpnReset;
				pMold->Clamp.p_set = pClampPara->P_OpnReset; 
			}
			else
			{
				pMold->Clamp.vProfGen.v_set = pMold->Clamp.V_Opn;
				pMold->Clamp.p_set = pClampPara->P_Opn;; 
			}
		            
			/* call profile function */
			HydvProfGen(& pMold->Clamp.vProfGen);
				
			pMold->Clamp.v_set = pMold->Clamp.vProfGen.v_out;
            
			pMold->Clamp.actTjOpn = pMold->Clamp.vProfGen.Tj;

            
			/*  reached target position */
			if(pMold->Clamp.vProfGen.s_act >= pMold->Clamp.vProfGen.s_end - pMold->Clamp.Tolerance * Toggle_ToleranceFactor)
			{
				pMold->Clamp.v_set = 0;
				pMold->Clamp.Step = 10500;
			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
		
            
		case 10500:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCalmpOpn = pMold->Clamp.LimitTimer.ET/100.0;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tClampOpn = pMold->Clamp.LimitTimer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tClampOpn = pMold->Clamp.LimitTimer.ET/100.0;
				}
			
			}
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->Clamp.Step = 10600;
			//			if(pMold->ValveDelay.ClampOpn != 0)
			//			{
			//				pMold->Clamp.Timer.IN = 1;
			//				pMold->Clamp.Timer.PT = pMold->ValveDelay.ClampOpn;
			//				pMold->Clamp.Step = 10600;
			//			}
			//			else
			//			{
			//				pMold->Clamp.Step = 13000;
			//			}
			
			break;
      
		case 10600:

			pMold->Clamp.Timer.IN = 0;
			pMold->ActInfo.ClampOpn  = 0;
			pMold->ActInfo.ClampCls  = 0;		
			
			pMold->Clamp.v_set  = 0;
			pMold->Clamp.p_set = 0;
		
			pMold->Clamp.Step = 13000;

			break;


		

		case 13000:
			break;

		/* --------------- 调校开模 --------------- */
		case 15000:
			if(0 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->ActInfo.ClampOpn  = 1;
			
				pMold->Clamp.p_set = pClampFix->pCalibMax;  
				pMold->Clamp.v_set = pClampFix->vCalibMax*10;

			}
			
			if(1 == pMold->Clamp.OpnHWSwitch)
			{
				pMold->HwAlarm.ClampOpnHWSwitch = 1;
				pMold->Clamp.Step = 41000;
			}	
			break;
        
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;/*Albert*/

			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            			
			
			pMold->Clamp.p_old = pMold->Clamp.p_set;
			pMold->Clamp.Step = 21000;
			break;

		case 21000:
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
            
			pMold->Clamp.vRamp.Ts = tTask;
            
			if(pMold->Clamp.v_set > 0)
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Open.accel_stop;
			}
			else
			{
				pMold->Clamp.vRamp.Accel = pClampFix->Close.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			pMold->Clamp.Step = 22000;
			break;

		case 22000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{	
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.p_set =0;
				pMold->Clamp.Step = 0;
			
				pMold->Carriage.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch  
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
			pMoldUser->Clamp.Timer.IN = 0;/*Albert*/
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
            					
			
			pMold->Clamp.vRamp.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
	            
			pMold->Clamp.vRamp.Ts = tTask;
	            
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			pMold->Clamp.Step = 31000;
			break;
		
		case 31000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
	            
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
	            
			pMold->Clamp.p_set = pMold->Clamp.p_old;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
	            
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.p_set =0;
				pMold->Clamp.Step = 0;
			
				pMold->Carriage.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch  
			}
			break;

		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
            
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;

			pMold->Clamp.v_set = 0.0;
			pMold->Clamp.p_set =0;
						
			
			break;
		
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Clamp.Timer.IN      = 0;
			pMold->Clamp.LimitTimer.IN = 0;
						
			pMold->ActInfo.ClampOpn = 0;
			pMold->ActInfo.ClampCls = 0;
						
			pMold->Clamp.Step = 42000;
			break;
							
		case 42000:
			pMold->Clamp.vRamp.Internal.Once = 0;
			pMold->Clamp.vRamp.Set = pMold->Clamp.v_set;
						
			pMold->Clamp.vRamp.Ts = tTask;
						
			pMold->Clamp.vRamp.Accel = INSTANCY_STOP_ACCEL;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
			pMold->Clamp.Step = 43000;
			break;
						
		case 43000:
			pMold->Clamp.vRamp.Ts = tTask;
			pMold->Clamp.vRamp.Set = 0.0;
						
			/* call profile function */
			HydSCurveRamp(&pMold->Clamp.vRamp);
					
			//	pMold->Clamp.p_set = 100.0;
			pMold->Clamp.v_set = pMold->Clamp.vRamp.Out;
						
			if(pMold->Clamp.v_set < 0.1 && pMold->Clamp.v_set > -0.1)
			{
				pMold->Clamp.p_set =0;
				pMold->Clamp.v_set = 0.0;
				pMold->Clamp.Step = 40000;
			}
			break;
	}/* end of switch */
   
	if(pMold->Clamp.Step > 300 && pMold->Clamp.Step  < 800)
	{
		pMold->TimeDis.ClampClsAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
	else if(pMold->Clamp.Step > 10300 && pMold->Clamp.Step  < 10800)
	{
		pMold->TimeDis.ClampOpnAlarmTime = pMold->Clamp.LimitTimer.ET;
	}
   
	/* ----------------- clamp open / close timeout --------------------  */
	if(1 == pMold->Clamp.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Clamp.Step < 4000 && pMold->Clamp.Step > 200)		/* clamp close  */
		{
			pMold->Alarm.ClampNotAtClsDI = 1;
		}
      
		if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)	/* clamp open  */
		{
			pMold->Alarm.ClampNotAtOpnDI = 1;
		}
      
		pMold->Clamp.LimitTimer.IN = 0;
		pMold->Clamp.Step = 41000;
	}

	/* ----------------- clamp open / close action limit --------------------  */
   
	if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) 	/* clamp close  */
	{
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->Clamp.Step = 41000;
		}
	
		
		if(1 == gMachineInfo.Auto && 1 == gMachineFix.Option.bCoolPinUpNextCycClampCls)   /* 特殊模式  "於下一循環關模後轉一上"  */
		{
//			if ( 0 == pMold->TransDIn.CoolPin2UpLimit || 0 == pMold->TransDIn.CoolPin3UpLimit )
//			{						
//				pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
//				pMold->Alarm.CoolPin3NotAtUpPos	= !pMold->TransDIn.CoolPin3UpLimit;	
//				pMold->Clamp.Step = 41000;
//			}	
		}
		else
		{
			if (0 == pMold->TransDIn.CoolPinUpLimit )
			{						
				pMold->Alarm.CoolPinNotAtUpPos	= !pMold->TransDIn.CoolPinUpLimit;
				pMold->Clamp.Step = 41000;
			}
		
		}

		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->Robot.Transfer.BwPos)
			{
				pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
				pMold->Clamp.Step = 41000;
			}
		}
		
		if(1 == gMachineInfo.Auto )
		{
			if(0 == pMold->Carriage.FwPos)
			{
				pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
				pMold->Clamp.Step = 41000;
			}
		}
		
		if(0 == pMold->TransDIn.ExtInMoldStickerBwLimit || 0 == pMold->TransDIn.ExtInMoldStickerAlarm)	
		{
			pMold->Alarm.ExtInMoldStickerNotAtBw = !pMold->TransDIn.ExtInMoldStickerBwLimit;
			pMold->Alarm.ExtInMoldStickerAlarm 	 = !pMold->TransDIn.ExtInMoldStickerAlarm;
			pMold->Clamp.Step = 41000;
		}
		
		/* 模内吹针在回位   */
		//      if(0 == pMold->TransDIn.InnerPinBwLimit)
		//      {
		//         pMold->Alarm.InnerPinNotAtBwPos = !pMold->TransDIn.InnerPinBwLimit;
		//         pMold->Clamp.Step = 41000;
		//      }
   
		//      if(1 == gMachineInfo.TimeLimit)
		//      {
		//         if(0 == pMold->Carriage.FwPos)
		//         {
		//            pMold->Alarm.CarriageNotAtFwDI  = !pMold->Carriage.FwPos;
		//            pMold->Clamp.Step = 41000;
		//         }
		//      }
		//      else
		//      {
		//         if(0 == pMold->Carriage.BwPos 
		//			&& 0 == pMold->Carriage.FwPos 
		//			&& 0 == pMold->BlowPin.UpPos)
		//         {
		//         	pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//         	pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
		//         	pMold->Clamp.Step = 41000;
		//         }
		//      }/* if(1 == gMachineInfo.Manual) */
	}/* if(pMold->Clamp.Step < 3000 && pMold->Clamp.Step > 200) */


	if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200)  /* clamp open  */
	{

		/* 切飞边非退位  */
		if( 0 == pMold->TransDIn.TopDeflashBwLimit )
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Clamp.Step = 41000;
		}
      
		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->TransDIn.ExtRobotUpLmit )	
			{
				pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
				pMold->Clamp.Step = 41000;
			}
		}

		
	}/* if(pMold->Clamp.Step < 13000 && pMold->Clamp.Step > 10200) */

	
	
	//	if((13000 >= pMold->Clamp.Step) && (10200 < pMold->Clamp.Step))
	//	{
	//		if(pMold == &RMold)
	//		{
	//			RobotClampTime.IN = pMold->Clamp.OpnPos;/*Albert RobotClampOpn*/
	//			RobotClampTime.PT = RobotClampTimePara;
	//		}
	//		if(pMold == & LMold)
	//		{
	//			LRobotClampTime.IN = pMold->Clamp.OpnPos;
	//			LRobotClampTime.PT = LRobotClampTimePara;
	//		}
	//	}
	//	else
	//	{
	////		RobotClampTime.IN = 0;/*Albert RobotClampOpn*/
	//	}
	//	if(pMold == &RMold)
	//	{
	//		if(RobotClampTime.Q)
	//		{
	//			RobotClampTime.IN = 0;
	//		}
	//		pMold->ValveOut.RobotClampOpn = RobotClampTime.Q;
	//		
	//	}
	//	if(pMold == &LMold)
	//	{
	//		if(LRobotClampTime.Q)
	//		{
	//			LRobotClampTime.IN = 0;
	//		}
	//		pMold->ValveOut.RobotClampOpn = LRobotClampTime.Q;
	//	}
	
	
}/*end of function */

REAL xTogLinearLTrans(REAL pClampPos)
{
	xTogLinearL.Y = pClampPos;
	HydLinear(&xTogLinearL);
	if(0 == xTogLinearL.status)
	{
		return xTogLinearL.X;	
	}
	else
	{
		return 0;
	}
}

REAL xTogLinearRTrans(REAL pClampPos)
{
	xTogLinearR.Y = pClampPos;
	HydLinear(&xTogLinearR);
	if(0 == xTogLinearR.status)
	{
		return xTogLinearR.X;	
	}
	else
	{
		return 0;
	}
}

void ButtonOffDelay(Mold_typ * pMold,timer_typ * pTimer)
{
	if(  1 == pMold->Clamp.BtnOffDlyFlag )
	{
		pTimer->IN = 1;
		pTimer->PT = 200;
	
		if( 1 == pTimer->Q)
		{
			pMold->Clamp.BtnOffDlyFlag = 0;
		}
	}
	else
	{
		pTimer->IN = 0;
	}
	
}


