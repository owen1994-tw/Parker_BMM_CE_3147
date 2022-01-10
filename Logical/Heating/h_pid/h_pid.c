/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: h_pid
 * File: h_pid.c
 * Author: lijianjun
 * Created: February 27, 2013
 ********************************************************************
 * Implementation of program h_pid -- heating and cooling controller
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"               /*edge function    */

#define  S$MODE_OFF					0
#define  S$MODE_PID					1
#define  S$MODE_TUNE					2

#define	C_STEP_WAIT 		   	0
#define	C_STEP_SYNC_HEAT_WAIT 	10
#define	C_STEP_HEAT 		    	20
#define	C_STEP_FREE 				30
#define	C_STEP_FREE_END			40
#define	C_STEP_COOL					50
#define	C_STEP_COOL_END 			60
#define	C_STEP_TUNE_END			70
#define	C_STEP_ERROR				999

void _INIT h_pidINIT(void)
{
	/* TODO: Add code here */
	gT_Par.Status.idxBroken = 0;
	gT_Par.Status.idxHigh   = 0;
	gT_Par.Status.idxLow    = 0;
	gT_Par.Status.idxFreeze = 0;
	gT_Par.Status.idxSwitchoff = 0;
	gT_Par.Status.idxNotNormal = 0;
    
	tProtectCooling = 0.0;
	
	/* ----- get task cycle time ----- */
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
	
	/* ----- setting tuning default value ----- */
	for(ZoneIndex=0; ZoneIndex<MAX_IDX_ZONE; ZoneIndex++)	
	{
		/* parameter for tune   */
		LcrTempSet[ZoneIndex].TuneSet.delta_T_sync_heat = 40.0; 	 	/* Parameter for the synchronization  */
		LcrTempSet[ZoneIndex].TuneSet.delta_dT_sync_heat = 0.1;		/* Parameter for the synchronization d(temp)/dt  */
		LcrTempSet[ZoneIndex].TuneSet.cnt_wait_heat = 100;     		/* Parameter for the synchronization second  */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_heat = 20.0;         	/* Parameter for the heating tuning 20 du */
		LcrTempSet[ZoneIndex].TuneSet.dT_min_heat  = 0.0;      		/* d(temp)/dt, min  */
		LcrTempSet[ZoneIndex].TuneSet.dRatio_heat  = 0.9;      		/* Internal parameter for the heating tuning 0 < dRatio_heat < 1*/
		LcrTempSet[ZoneIndex].TuneSet.cnt_tp_heat  = 180;      		/* 180  second */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_sync_free  = 10.0; 	/* default value  */
		LcrTempSet[ZoneIndex].TuneSet.delta_dT_sync_free = 0.05;		/* default value   */
		LcrTempSet[ZoneIndex].TuneSet.cnt_wait_free      = 300;  	/* default value  300 second */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_free = 10.0;				/* default value  10 du */
		LcrTempSet[ZoneIndex].TuneSet.dT_min_free  = 0.0;				/* default value  */
		LcrTempSet[ZoneIndex].TuneSet.dRatio_free  = 0.9;				/* default value  */
		LcrTempSet[ZoneIndex].TuneSet.cnt_tp_free  = 300;				/* default value  300 second */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_sync_cool  = 10.0;		/* default value  10 du */
		LcrTempSet[ZoneIndex].TuneSet.delta_dT_sync_cool = 0.1;		/* not default value = 0.05  */
		LcrTempSet[ZoneIndex].TuneSet.cnt_wait_cool      = 300;		/* default value  300 second */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_cool = 10.0;				/* default value  10 du */
		LcrTempSet[ZoneIndex].TuneSet.dT_min_cool  = 0.0;				/* default value  */
		LcrTempSet[ZoneIndex].TuneSet.dRatio_cool  = 0.9;				/* default value  */
		LcrTempSet[ZoneIndex].TuneSet.cnt_tp_cool  = 300.0;			/* default value  300 second */
		
		LcrTempSet[ZoneIndex].TuneSet.delta_T_sync_stop = 50.0;     /* Parameter for secure enable of the cooling tuning */
		LcrTempSet[ZoneIndex].TuneSet.filter_base_T = 20.0;         /* average value filter */
		LcrTempSet[ZoneIndex].TuneSet.Temp_amb = 40.0;
		
		LcrTempSet[ZoneIndex].TuneSet.mode = LCRTEMPTune_MODE_EXP;
		
		/* Heating and cooling mode  */
		LcrTempSet[ZoneIndex].enable_cooling  = 0;
		LcrTempSet[ZoneIndex].disable_heating = 0;
		
		/* stable parameter for PID   */
		LcrTempSet[ZoneIndex].PIDSet.hyst   = 2.0;					
		LcrTempSet[ZoneIndex].PIDSet.Kw     = 1.0;
		LcrTempSet[ZoneIndex].PIDSet.delay  = 10.0;
		LcrTempSet[ZoneIndex].PIDSet.Kfbk   = 1.0;
  		
		/* get address  */
		LCRTempTune_Array[ZoneIndex].pSettings = &(LcrTempSet[ZoneIndex]);
		LCRTempPID_Array[ZoneIndex].pSettings  = &(LcrTempSet[ZoneIndex]);
		
		gT_Par.Ctl.OnOff[ZoneIndex] = 0;
		gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
		gT_Par.Ctl.IsHeatingOnPlan  = 0;
		gT_Par.Ctl.PreOnOff[ZoneIndex]= 0;
		gT_Par.Ctl.IsPreHeatingPlan = 0;
		
		OutBuffer[ZoneIndex]= 0.0;
		cntTask[ZoneIndex]  = 0;
		
		gT_Par.Status.TuneOk[ZoneIndex] = 0;
		
		HeatAlarm.tTempLow[ZoneIndex] = 1000.0;
	}/* for(ZoneIndex=0; ZoneIndex<MAX_ZoneIndex_CONTRL_ZONES; ZoneIndex++) */

}

void _CYCLIC h_pidCYCLIC(void)
{
	int i;
	gMachineInfo.bLoweringFlag =	bLowering; //ipis0424
	
	if (EDGEPOS(bLowering_Hmi,bLowering_Hmi_Pos))
	{
		if (1== gMachineOut.ParisonPump || 1== gMachineOut.ExtrA_Enable || 1==gMachineOut.ExtrB_Enable || gMachineOut.ExtrC_Enable || 1== gMachineOut.ExtrV_Enable)
		{
			gImply.ExtruderEnable =1;
		}
		else
		{
			bLowering =1;		
		}				
	}
	
	if (0==gMachineOut.HeatPower) //ipis0815
	{
		bLowering_Hmi =0;
	}
		
	if (0==bLowering_Hmi) 
	{
		bLowering =0;
	}
	
	
	/* -------------------------------------------------------------------------- */
	if(1 == gT_Par.Ctl.IsAlarmConfirm)
	{
		gT_Par.Ctl.IsAlarmConfirm = 0;
		
		/* 非自复位的报警复位  */
		memset(gT_Par.Status.TempFreeze,0,sizeof(gT_Par.Status.TempFreeze));
		memset(gT_Par.Status.TempSwitchOff,0,sizeof(gT_Par.Status.TempSwitchOff));
	}
	
	/* -------------------------------------------------------------------------- */
	gT_Par.Status.idxBroken = 0;
	gT_Par.Status.idxHigh   = 0;
	gT_Par.Status.idxLow    = 0;
	gT_Par.Status.idxFreeze = 0;
	gT_Par.Status.idxSwitchoff = 0;
	gT_Par.Status.idxNotNormal = 0;
	
	gT_Par.Status.bBroken = 0;
	gT_Par.Status.bHigh   = 0;
	gT_Par.Status.bLowInternal = 0;
	gT_Par.Status.bFreeze = 0;
	gT_Par.Status.bSwitchOff = 0;
	gT_Par.Status.bNotNormal = 0;

	/*============================================================================*/
	/* Check group switches                              						         */
	/*============================================================================*/
	//	if(0 == gMachineIn.HeatingPowerNormal)gT_Par.GrpSwh.OnOff[0] = 0;
	if(0 == gMachineOut.HeatPower) gT_Par.GrpSwh.OnOff[0] = 0;
	
	
	if(0 == gT_Par.GrpSwh.OnOff[0] && 0 == gT_Par.GrpSwh.PreOnOff[0])
	{
		gMachineInfo.IsHeat = 0;
	}
	else if(1 == gT_Par.GrpSwh.OnOff[0] && 0 == gT_Par.GrpSwh.PreOnOff[0])
	{
		gMachineInfo.IsHeat = 1;	
	}
	else 
	{
		gMachineInfo.IsHeat = 2;	
	}
	
	grpMaxIndex = sizeof(gT_Par.GrpSwh.OnOff)/sizeof(gT_Par.GrpSwh.OnOff[0]);
	if(grpMaxIndex > MAX_IDX_TEMPGROUP)grpMaxIndex = MAX_IDX_TEMPGROUP; 
	
	for(grp=0; grp<grpMaxIndex; grp++)
	{
		if(gT_Par.Par.ST_GrpPreHeating[grp] < 20)gT_Par.Par.ST_GrpPreHeating[grp]= 20;
		if(gT_Par.Par.tGrpPreHeating[grp] < 10.0)gT_Par.Par.tGrpPreHeating[grp]  = 10.0;	/* 10.0min  */
		
		/* Pre-heating plan control  */
		if(1 == gT_Par.Ctl.IsPreHeatingPlan  && 0 == gT_Par.GrpSwh.PreOnOff[grp])
		{
			gMachineOut.HeatPower =1; //ipis0220
			gT_Par.GrpSwh.PreOnOff[grp] = 1;
			gT_Par.GrpSwh.OnOff[grp]    = 1;
		}
		
		if(1 == gT_Par.Ctl.IsHeatingOffPlan && 1 == gT_Par.GrpSwh.OnOff[grp])
		{
			gT_Par.GrpSwh.PreOnOff[grp] = 0;
		}
		
		if(0==gMachineIn.PreHeatDI)   //ipis0226
		{
			gT_Par.GrpSwh.PreOnOff[grp] = 0;
		}
		
		/* heating plan control  */
		if(1 == gT_Par.Ctl.IsHeatingOnPlan  && (0 == gT_Par.GrpSwh.OnOff[grp] || 1 == gT_Par.GrpSwh.PreOnOff[grp]))
		{
			gMachineOut.HeatPower =1; //ipis0220
			gT_Par.GrpSwh.OnOff[grp] = 1;
			gT_Par.GrpSwh.PreOnOff[grp] = 0;
		}
		
		if(1 == gT_Par.Ctl.IsHeatingOffPlan && 1 == gT_Par.GrpSwh.OnOff[grp])
		{
			gT_Par.GrpSwh.OnOff[grp] = 0;
		}
		
		/* Switch on group heating if group is on from VIS and programme enabled	*/
		if (EDGEPOS(gT_Par.GrpSwh.OnOff[grp],grp_OnOffPos[grp]))
		{
			HeatSequence[grp] = MAX_IDX_CONTRL_ZONES;             
		}
		
		/* Switch off group heating if group is off from VIS and programe disabled	*/
		if (EDGENEG(gT_Par.GrpSwh.OnOff[grp],grp_OnOffNeg[grp]))
		{
			HeatSequence[grp] = 0;             
			for(ZoneIndex=0;ZoneIndex<MAX_IDX_CONTRL_ZONES;ZoneIndex++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)/*if grp nr configed     */
				{
					gT_Par.Ctl.OnOff[ZoneIndex] = 0;
				}
			}
		}
		/* Switch off group pre heating on  */
		if (EDGEPOS(gT_Par.GrpSwh.PreOnOff[grp],grp_PrePos[grp]))
		{
			for(ZoneIndex=0;ZoneIndex<MAX_IDX_CONTRL_ZONES;ZoneIndex++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)/*if grp nr configed     */
				{
					gT_Par.Ctl.PreOnOff[ZoneIndex] = 1;
				}
			}
		}
		
		/* Switch off group pre heating off  */
		if (EDGENEG(gT_Par.GrpSwh.PreOnOff[grp],grp_PreNeg[grp]) || 0==gMachineIn.PreHeatDI) //ipis0221
		{
			for(ZoneIndex=0;ZoneIndex<MAX_IDX_CONTRL_ZONES;ZoneIndex++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)/*if grp nr configed     */
				{
					gT_Par.Ctl.PreOnOff[ZoneIndex] = 0;
				}
			}
		}
		/* Switch off group tuning on  */
		if (EDGEPOS(gT_Par.GrpSwh.TuneOnOff[grp],grp_TunePos[grp]))
		{
			for(ZoneIndex=0;ZoneIndex<MAX_IDX_CONTRL_ZONES;ZoneIndex++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)/*if grp nr configed     */
				{
					gT_Par.Ctl.TuneOnOff[ZoneIndex] = 1;
				}
			}
		}
		
		/* Switch off group tuning off  */
		if (EDGENEG(gT_Par.GrpSwh.TuneOnOff[grp],grp_TuneNeg[grp]))
		{
			for(ZoneIndex=0;ZoneIndex<MAX_IDX_CONTRL_ZONES;ZoneIndex++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)/*if grp nr configed     */
				{
					gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
				}
			}
		}
        
		grp_OnOffPos[grp] = gT_Par.GrpSwh.OnOff[grp];
		grp_OnOffNeg[grp] = gT_Par.GrpSwh.OnOff[grp];
	}/* for(grp=0; grp<grpMaxIndex; grp++) */
		
	/* Heat Sequence,one by one heat zone on */
	grpAct = grpMaxIndex;
	for(grp=0; grp<grpMaxIndex; grp++)
	{
		if(HeatSequence[grp] > 0)
		{
			grpAct = grp;
			break;
		}
	}
	
	if( grpAct < grpMaxIndex) 
	{
		if(HeatSequence[grpAct] > 0)
		{
			if (grpAct == gT_Par.Fix.SynchGroup[MAX_IDX_CONTRL_ZONES - HeatSequence[grpAct]])
			{
				gT_Par.Ctl.OnOff[MAX_IDX_CONTRL_ZONES - HeatSequence[grpAct]] = 1;
			}
			HeatSequence[grpAct]--;
		}
	}	
	
	/* check temperature change time  */
	if(gT_Par.Fix.tCheck < 10.0 )gT_Par.Fix.tCheck = 10.0;  /* 10.0s  */
	if(gT_Par.Fix.tHeatNormal < 20.0) gT_Par.Fix.tHeatNormal = 20.0;  /*  20.0 s */
	if(gT_Par.Fix.AT_MAX < 240.0)gT_Par.Fix.AT_MAX = 240.0; /* 240.0  */
	if(gT_Par.Par.ST_Tune_Same < 100)gT_Par.Par.ST_Tune_Same = 180.0;  /* Tune temperature   */
		
	/*============================================================================*/
	/* actual input && gAlarm check; PID parameters initialization   			  */
	/*============================================================================*/
	gMachineInfo.IsOpt = 0;
	for(ZoneIndex=0; ZoneIndex<MAX_IDX_CONTRL_ZONES; ZoneIndex++)
	{
		/*---UP-----------------------------------------------------------*/
		//		//		if (TempUpLimit_HMI[ZoneIndex]<=gT_Par.Fix.AT_MAX)
		//		//		{
		//		//			TempUpLimit_HMI[ZoneIndex] = gT_Par.Par.ST_PID[ZoneIndex]+  gUserFix_new.TempUpRange;
		//		//		}
		//		//		else
		//		//		{
		//		//			TempUpLimit_HMI[ZoneIndex] =gT_Par.Fix.AT_MAX;
		//		//		}
		//		//ipis022
		//		TempUpLimit_HMI[ZoneIndex] = gT_Par.Par.ST_PID[ZoneIndex]+  gUserFix_new.TempUpRange;
		//		if (gT_Par.Par.AlaUpTol[ZoneIndex] < gT_Par.Par.ST_PID[ZoneIndex] )
		//		{
		//			gT_Par.Par.AlaUpTol[ZoneIndex]=TempUpLimit_HMI[ZoneIndex];
		//		}
		//		//		if (gT_Par.Par.AlaUpTol[ZoneIndex] > gT_Par.Par.ST_PID[ZoneIndex] )
		//		//		{
		//		//			gT_Par.Par.AlaUpTol[ZoneIndex]=TempUpLimit_HMI[ZoneIndex];
		//		//		}
		//		
		//		if (gT_Par.Par.AlaUpTol[ZoneIndex]>=TempUpLimit_HMI[ZoneIndex])
		//		{
		//			gT_Par.Par.AlaUpTol[ZoneIndex]=TempUpLimit_HMI[ZoneIndex];
		//		}
		//ipis0218
		if (gT_Par.Par.AlaUpTol[ZoneIndex] > gUserFix_new.TempUpRange )
		{
			gT_Par.Par.AlaUpTol[ZoneIndex]=gUserFix_new.TempUpRange;
		}
		if (gT_Par.Par.AlaUpTol[ZoneIndex] < 0 )
		{
			gT_Par.Par.AlaUpTol[ZoneIndex]=0;
		}
		/*---Down-----------------------------------------------------------*/
		//		if (TempUpLimit_HMI[ZoneIndex]>=0)
		//		{
		//			TempDwnLimit_HMI[ZoneIndex] = gT_Par.Par.ST_PID[ZoneIndex]- gUserFix_new.TempDwnRange;
		//		}
		//		else
		//		{
		//			TempDwnLimit_HMI[ZoneIndex] =0;
		//		}
		//		if(gT_Par.Par.AlaDwnTol[ZoneIndex]>gT_Par.Par.ST_PID[ZoneIndex])
		//		{
		//			gT_Par.Par.AlaDwnTol[ZoneIndex] = TempDwnLimit_HMI[ZoneIndex];	
		//		}
		//		if(gT_Par.Par.AlaDwnTol[ZoneIndex]<TempDwnLimit_HMI[ZoneIndex])
		//		{
		//			gT_Par.Par.AlaDwnTol[ZoneIndex] = TempDwnLimit_HMI[ZoneIndex];	
		//		}
		//			
		//		if(gT_Par.Par.AlaDwnTol[ZoneIndex]<0)
		//		{
		//			gT_Par.Par.AlaDwnTol[ZoneIndex] = 0;	
		//		}
		
		//ipis0218
		if(gT_Par.Par.AlaDwnTol[ZoneIndex]>gUserFix_new.TempDwnRange)
		{
			gT_Par.Par.AlaDwnTol[ZoneIndex] = gUserFix_new.TempDwnRange;	
		}	
		if(gT_Par.Par.AlaDwnTol[ZoneIndex]<0)
		{
			gT_Par.Par.AlaDwnTol[ZoneIndex] = 0;	
		}
		
		
		/*---UP Same-----------------------------------------------------------*/				
		//		if (TempUpLimit_Same_HMI<gT_Par.Fix.AT_MAX)
		//		{
		//			TempUpLimit_Same_HMI = gT_Par.Par.ST_PID_Same+  gUserFix_new.TempUpRange;
		//		}
		//		else
		//		{
		//			TempUpLimit_Same_HMI =gT_Par.Fix.AT_MAX;
		//		}
		//		TempUpLimit_Same_HMI = gT_Par.Par.ST_PID_Same+  gUserFix_new.TempUpRange;
		//		if (gT_Par.Par.AlaUpTol_Same<gT_Par.Par.ST_PID_Same)
		//		{
		//			gT_Par.Par.AlaUpTol_Same = TempUpLimit_Same_HMI;
		//		}
		//		//		if (gT_Par.Par.AlaUpTol_Same>gT_Par.Par.ST_PID_Same)
		//		//		{
		//		//			gT_Par.Par.AlaUpTol_Same = TempUpLimit_Same_HMI;
		//		//		}
		//		if (gT_Par.Par.AlaUpTol_Same>=TempUpLimit_Same_HMI)
		//		{
		//			gT_Par.Par.AlaUpTol_Same=TempUpLimit_Same_HMI;
		
		
		//ipis0218
		if (gT_Par.Par.AlaUpTol_Same > gUserFix_new.TempUpRange )
		{
			gT_Par.Par.AlaUpTol_Same=gUserFix_new.TempUpRange;
		}
		if (gT_Par.Par.AlaUpTol_Same < 0 )
		{
			gT_Par.Par.AlaUpTol_Same=0;
		}
		
		
		
		/*---Down Same-----------------------------------------------------------*/					
		//		if (TempDwnLimit_Same_HMI>=0)
		//		{
		//			TempDwnLimit_Same_HMI = gT_Par.Par.ST_PID_Same- gUserFix_new.TempDwnRange;
		//		}
		//		else
		//		{
		//			TempDwnLimit_Same_HMI = 0;
		//		}
		//		
		//		if (gT_Par.Par.AlaDwnTol_Same >gT_Par.Par.ST_PID_Same )
		//		{
		//			gT_Par.Par.AlaDwnTol_Same = TempDwnLimit_Same_HMI;
		//		}
		//		if (gT_Par.Par.AlaDwnTol_Same <TempDwnLimit_Same_HMI )
		//		{
		//			gT_Par.Par.AlaDwnTol_Same = TempDwnLimit_Same_HMI;
		//		}	
		//		if (gT_Par.Par.AlaDwnTol_Same <0)
		//		{
		//			gT_Par.Par.AlaDwnTol_Same =0;
		//		}

		//ipis0218
		if(gT_Par.Par.AlaDwnTol_Same>gUserFix_new.TempDwnRange)
		{
			gT_Par.Par.AlaDwnTol_Same = gUserFix_new.TempDwnRange;	
		}	
		if(gT_Par.Par.AlaDwnTol_Same<0)
		{
			gT_Par.Par.AlaDwnTol_Same= 0;	
		}
		
		//		TempUpLimit_HMI[ZoneIndex] = gT_Par.Par.ST_PID[ZoneIndex]+  gUserFix_new.TempUpRange;
		//		
		//		TempDwnLimit_HMI[ZoneIndex] = gT_Par.Par.ST_PID[ZoneIndex]- gUserFix_new.TempDwnRange;
		//		
		//		TempUpLimit_Same_HMI = gT_Par.Par.ST_PID_Same+  gUserFix_new.TempUpRange;
		//		
		//		TempDwnLimit_Same_HMI = gT_Par.Par.ST_PID_Same- gUserFix_new.TempDwnRange;
		
		
		/*--------------------------------------------------------------*/
				
		gT_Par.Fix.bContactor[ZoneIndex] = 1;
		if(gT_Par.Fix.tTempFreeze[ZoneIndex] < 10.0 )gT_Par.Fix.tTempFreeze[ZoneIndex] = 10.0;
		if(gT_Par.Fix.tTempFreeze[ZoneIndex] > 1800.0)gT_Par.Fix.tTempFreeze[ZoneIndex] = 1800.0;

		if(tTask > 0.0 && 1 == gT_Par.Fix.Cooling[ZoneIndex])
		{
			gT_Par.Fix.tPeriodHeat[ZoneIndex] = ((INT)(gT_Par.Fix.tPeriodHeat[ZoneIndex]/tTask + 0.5)) * tTask;
			if(gT_Par.Fix.tPeriodHeat[ZoneIndex] < 10 * tTask)gT_Par.Fix.tPeriodHeat[ZoneIndex] = 10 * tTask;
		}
		
		if(tTask > 0.0 && 1 == gT_Par.Fix.bContactor[ZoneIndex])
		{
			gT_Par.Fix.tPeriodHeat[ZoneIndex] = ((INT)(gT_Par.Fix.tPeriodHeat[ZoneIndex]/tTask + 0.5)) * tTask;
			if(gT_Par.Fix.tPeriodHeat[ZoneIndex] < 10 * tTask)gT_Par.Fix.tPeriodHeat[ZoneIndex] = 10 * tTask;
		}
		
		/* ------ PID OPT value change ------ */
		if (memcmp(&gT_Par.Fix.ParTune[ZoneIndex],&ParTuneOld[ZoneIndex],sizeof(ParTuneOld[ZoneIndex])) != 0)
		{
			LcrTempSet[ZoneIndex].PIDpara = gT_Par.Fix.ParTune[ZoneIndex];
			LCRTempPID_Array[ZoneIndex].update = 1;
			ParTuneOld[ZoneIndex] = gT_Par.Fix.ParTune[ZoneIndex];
		}
		
		/* ---------------------- heating  normal -----------------------*/
		//ByPass gH_IO.HeatNormal(Feedback signal)which for detect heater error
		gH_IO.HeatNormal[ZoneIndex] = gH_IO.do_H[ZoneIndex];
		
		
		if(1 == gH_IO.do_H[ZoneIndex] && 0 == gH_IO.HeatNormal[ZoneIndex])
		{
			if(tHeatNormal[ZoneIndex] > 0.0)tHeatNormal[ZoneIndex] = tHeatNormal[ZoneIndex] - tTask;
			if(tHeatNormal[ZoneIndex] <= 0.0)
			{
				gT_Par.Status.bAlarmHeatNormal[ZoneIndex] = 1;
			}
		}
		else if(0 == gH_IO.do_H[ZoneIndex] && 1 == gH_IO.HeatNormal[ZoneIndex])
		{
			if(tHeatNormal[ZoneIndex] > 0.0)tHeatNormal[ZoneIndex] = tHeatNormal[ZoneIndex] - tTask;
			if(tHeatNormal[ZoneIndex] <= 0.0)
			{
				gT_Par.Status.bAlarmHeatNormal[ZoneIndex] = 1;
			}
		}
		else
		{
			gT_Par.Status.bAlarmHeatNormal[ZoneIndex] = 0;
			tHeatNormal[ZoneIndex] = gT_Par.Fix.tHeatNormal;
		}
		

		////ipis0719/////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		//		if (   0==TempInitOK[ZoneIndex]  )
		//		{   
		//			//ipis0218
		//			if (( gH_IO.ActT[ZoneIndex] < gT_Par.Par.ST_PID[ZoneIndex] - gT_Par.Par.AlaDwnTol[ZoneIndex])&& 1 == gT_Par.Fix.Enable[ZoneIndex]
		//				|| 0==gMachineInfo.ExtrHeatTimeFlag) //ipis0213
		//			{
		//				gExtruderTempLow_HMIFlag =1;
		//			}
		//			else
		//			{
		//				gExtruderTempLow_HMIFlag =0;	
		//			}
		//					
		//		}	

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		
		
		
		/* --- if current zone disabled, clear control bit --- */
		if (0 == gT_Par.Fix.Enable[ZoneIndex])  
		{
			gT_Par.Ctl.OnOff[ZoneIndex] = 0;
			gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
			
			gT_Par.Status.AlmHigh[ZoneIndex]      = 0;     
			gT_Par.Status.AlmLow[ZoneIndex]       = 0;  
			gT_Par.Status.SensorBroken[ZoneIndex] = 0;     /*For error bit show*/
			
			HeatAlarm.tSensorBroken[ZoneIndex] = 0.0;
			HeatAlarm.tTempHigh[ZoneIndex] = 0.0;
			HeatAlarm.tTempLow[ZoneIndex]  = 0.0;
			TempInitOK[ZoneIndex]=1;  //ipis0719
			
			//Bypass for gExtruderTempLow_HMIFlag
			TempProtection[ZoneIndex] =1;
			TempNotLow[ZoneIndex] =1;
		
		}
		else
		{
			if (1==gMachineInfo.IsHeat)
			{
				//	if (gH_IO.ActT[ZoneIndex] >= gT_Par.Par.AlaDwnTol[ZoneIndex])
				//ipis0218
				if (gH_IO.ActT[ZoneIndex] >= gT_Par.Par.ST_PID[ZoneIndex] - gT_Par.Par.AlaDwnTol[ZoneIndex])	
				{
					{
						TempInitOK[ZoneIndex]=1;
					}

				}
				else
				{
					TempInitOK[ZoneIndex]=0;
				}

			
			
				if (1 == gT_Par.GrpSwh.OnOff[0])  //ipis 0128
				{
					gT_Par.Ctl.OnOff[ZoneIndex] = 1;
				}
				/*============================================================================*/
				/* Check gAlarm bit                                 						     */
				/*============================================================================*/
				/*-----------sensor broken or Opencircuit check-----------------------------*/
				if (gH_IO.ActT[ZoneIndex] >= 3000.0 || gH_IO.ActT[ZoneIndex] <= -3000.0)  
				{
					HeatAlarm.tSensorBroken[ZoneIndex] = HeatAlarm.tSensorBroken[ZoneIndex] + tTask;
				
					if(HeatAlarm.tSensorBroken[ZoneIndex] > gT_Par.Fix.tCheck)
					{					
						gT_Par.Status.SensorBroken[ZoneIndex] = 1;
					}
					else
					{
						gT_Par.Status.SensorBroken[ZoneIndex] = 0;
					}
				
					gT_Par.Status.AlmHigh[ZoneIndex] = 0;
					gT_Par.Status.AlmLow[ZoneIndex]  = 0;
					gT_Par.Status.TempFreeze[ZoneIndex] = 0;		
				}
				else
				{
					HeatAlarm.tSensorBroken[ZoneIndex] = 0.0;
					gT_Par.Status.SensorBroken[ZoneIndex] = 0;
				
					if (1==gMachineInfo.IsHeat && (uDisHeaterTime>= gUserFix_new.HeatLowTimeOut*60))
					{
						//	if (gH_IO.ActT[ZoneIndex] < gT_Par.Par.AlaDwnTol[ZoneIndex])
						//ipis0218
						if (gH_IO.ActT[ZoneIndex] < gT_Par.Par.ST_PID[ZoneIndex]- gT_Par.Par.AlaDwnTol[ZoneIndex])
						{
							gAlarm.HeatError=1;	
						}
					
					}
				
				
					if(1==TempInitOK[ZoneIndex] && 0== bLowering && 0==gT_Par.Ctl.IsPreHeatingPlan ) //ipis0304
					{
						/*Actual temperature high temp limit or upper tolerance limit*/
						//	if (gH_IO.ActT[ZoneIndex] > (gT_Par.Par.AlaUpTol[ZoneIndex]))/*Albert*/
						//ipis0211
						if (gH_IO.ActT[ZoneIndex] > (gT_Par.Par.ST_PID[ZoneIndex] +gT_Par.Par.AlaUpTol[ZoneIndex]))/*Albert*/
						{
							HeatAlarm.tTempHigh[ZoneIndex] = HeatAlarm.tTempHigh[ZoneIndex] + tTask;
							if(HeatAlarm.tTempHigh[ZoneIndex] > gT_Par.Fix.tCheck)
							{
								gT_Par.Status.AlmHigh[ZoneIndex] = 1; 
							}
							else
							{
								gT_Par.Status.AlmHigh[ZoneIndex] = 0;
							}
						}
						else
						{
							HeatAlarm.tTempHigh[ZoneIndex] = 0.0;
							gT_Par.Status.AlmHigh[ZoneIndex] = 0;
						}

						/*Actual temperature low temp. limit or down tolerance limit*/
						//	if (gH_IO.ActT[ZoneIndex] < gT_Par.Par.AlaDwnTol[ZoneIndex])
						//ipis0218
						if (gH_IO.ActT[ZoneIndex] < gT_Par.Par.ST_PID[ZoneIndex] - gT_Par.Par.AlaDwnTol[ZoneIndex])
	
						{   
							HeatAlarm.tTempLow[ZoneIndex] = HeatAlarm.tTempLow[ZoneIndex] + tTask;
							if(HeatAlarm.tTempLow[ZoneIndex] > gT_Par.Fix.tCheck)
							{
								gT_Par.Status.AlmLow[ZoneIndex] = 1; 
							}
							else
							{
								gT_Par.Status.AlmLow[ZoneIndex] = 0;
							}     
						}
						else
						{
							HeatAlarm.tTempLow[ZoneIndex] = 0.0;
							gT_Par.Status.AlmLow[ZoneIndex] = 0;
						}
				
						/*Actual temperature > permit max ---> stop heating */
						if ((gH_IO.ActT[ZoneIndex] > gT_Par.Fix.AT_MAX || gH_IO.ActT[ZoneIndex] < -100) &&
							(1 == gT_Par.Ctl.TuneOnOff[ZoneIndex] || 1 == gT_Par.Ctl.OnOff[ZoneIndex] || 1 == gT_Par.Ctl.PreOnOff[ZoneIndex])
							)
						{
							HeatAlarm.tTempSwitchOff[ZoneIndex] = HeatAlarm.tTempSwitchOff[ZoneIndex] + tTask;
				
							if(HeatAlarm.tTempSwitchOff[ZoneIndex] > gT_Par.Fix.tCheck)
							{
								gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
								gT_Par.Ctl.OnOff[ZoneIndex]     = 0;
								gT_Par.Ctl.PreOnOff[ZoneIndex]  = 0;
						
								gT_Par.Status.TempSwitchOff[ZoneIndex] = 1;
							}
							else
							{
								gT_Par.Status.TempSwitchOff[ZoneIndex] = 0;
							}
						}
						else
						{
							HeatAlarm.tTempSwitchOff[ZoneIndex] = 0.0;
							gT_Par.Status.TempSwitchOff[ZoneIndex] = 0;
						
						}

					}
					else
					{
						gT_Par.Status.AlmHigh[ZoneIndex] = 0;	
						gT_Par.Status.AlmLow[ZoneIndex] = 0;
						gT_Par.Status.TempSwitchOff[ZoneIndex] = 0;	
					
					
					}
			
				
				
				
				}
				
				
				
				
			}	
			
			//for gExtruderTempLow_HMIFlag
			if (gH_IO.ActT[ZoneIndex] > gUserFix_new.ProtectionTemp ) //ipis0225
			{
				TempProtection[ZoneIndex] =1;
			}
			else
			{
				TempProtection[ZoneIndex] =0;
			}
		
			if (gH_IO.ActT[ZoneIndex] < gT_Par.Par.ST_PID[ZoneIndex] - gT_Par.Par.AlaDwnTol[ZoneIndex] ) //0226  
			{
				TempNotLow[ZoneIndex] =0;
			}
			else
			{
				TempNotLow[ZoneIndex] =1;
			}
			
			
			
		}/* if (0 == gT_Par.Fix.Enable[ZoneIndex]) */
		
		/* PID and TUNE get set parameter address  */
		LCRTempPID_Array[ZoneIndex].pSettings  = &LcrTempSet[ZoneIndex];
		LCRTempTune_Array[ZoneIndex].pSettings = &LcrTempSet[ZoneIndex];
		
		/* act temperature */
		LCRTempPID_Array[ZoneIndex].Temp  = gH_IO.ActT[ZoneIndex];		
		LCRTempTune_Array[ZoneIndex].Temp = gH_IO.ActT[ZoneIndex];
		
		/*  set temperature */
		if(1 == gT_Par.Ctl.PreOnOff[ZoneIndex])
		{
			for(grp=0; grp<grpMaxIndex; grp++)
			{
				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)
				{
					LCRTempPID_Array[ZoneIndex].Temp_set = gT_Par.Par.ST_GrpPreHeating[grp];	
				}
			}
		}
		else
		{
			/*2018.7.31 Albert add new lowering setting*/
			//			LCRTempPID_Array[ZoneIndex].Temp_set = gT_Par.Par.ST_PID[ZoneIndex];
			if(1 == bLowering)	LCRTempPID_Array[ZoneIndex].Temp_set = gT_Par.Par.ST_PID[ZoneIndex] - gT_Par.Par.Lower;
			else LCRTempPID_Array[ZoneIndex].Temp_set = gT_Par.Par.ST_PID[ZoneIndex];

		}
		
		gT_Par.Par.ST_Tune[ZoneIndex] = gT_Par.Par.ST_Tune_Same;
		LCRTempTune_Array[ZoneIndex].Temp_set = gT_Par.Par.ST_Tune[ZoneIndex];
		
		//		/* Preheating on time --> Preheating off, normal heating*/
		//		if(1 == gT_Par.Ctl.PreOnOff[ZoneIndex])
		//		{
		//			/* reach setting pre-heating temperature - 5.0 */
		//			if( gH_IO.ActT[ZoneIndex] > LCRTempPID_Array[ZoneIndex].Temp_set - 5.0)
		//			{
		//				HeatTest.tPreHeating[ZoneIndex] = HeatTest.tPreHeating[ZoneIndex] + tTask;
		//			}
		//			else
		//			{
		//				HeatTest.tPreHeating[ZoneIndex] = 0.0;	
		//			}
		//			
		//			for(grp=0; grp<grpMaxIndex; grp++)
		//			{
		//				if (gT_Par.Fix.SynchGroup[ZoneIndex] == grp)
		//				{
		//					if(HeatTest.tPreHeating[ZoneIndex] > gT_Par.Par.tGrpPreHeating[grp] * 60)
		//					{
		//						gT_Par.Ctl.OnOff[ZoneIndex]    = 1;
		//						gT_Par.Ctl.PreOnOff[ZoneIndex] = 0;
		//						gT_Par.Status.PreHeatingOk[ZoneIndex] = 1;
		//					}
		//				}
		//			}
		//		}
		//		else
		//		{
		//			HeatTest.tPreHeating[ZoneIndex] = 0;
		//		}
		
		/* Heating Power not Normal, single zone not permit heating  */
		if(0 == gMachineIn.HeatingPowerNormal)
		{
			gT_Par.Ctl.OnOff[ZoneIndex] = 0;
			gT_Par.Ctl.PreOnOff[ZoneIndex] = 0;
		}
		
		/* off or PID or tuning  */
		if ((1 == gT_Par.Status.SensorBroken[ZoneIndex]) || (0 == gT_Par.Fix.Enable[ZoneIndex]))     /*sensor broken*/
		{
			PIDSTEP[ZoneIndex] = S$MODE_OFF;			/* If sensor error or not anble --> switch off zone		*/
			
			gT_Par.Ctl.OnOff[ZoneIndex]     = 0;
			gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
			gT_Par.Ctl.PreOnOff[ZoneIndex]  = 0;
			
			gT_Par.Status.Tuning[ZoneIndex] = 0;
			gT_Par.Status.TuneStatus[ZoneIndex] = 0;
		}
		else if ((1 == gT_Par.Ctl.TuneOnOff[ZoneIndex]) && (1 == gT_Par.Ctl.OnOff[ZoneIndex])) 
		{
			PIDSTEP[ZoneIndex] = S$MODE_TUNE;		/* Tune mode*/
			
			gT_Par.Status.Tuning[ZoneIndex] = 1;
			gMachineInfo.IsOpt  =  1;
		}
		else if ((1 == gT_Par.Ctl.OnOff[ZoneIndex]) || (1 == gT_Par.Ctl.PreOnOff[ZoneIndex]))
		{
			PIDSTEP[ZoneIndex] = S$MODE_PID;			/* PID mode*/ 
			
			gT_Par.Status.Tuning[ZoneIndex] = 0;
			gT_Par.Status.TuneStatus[ZoneIndex] = 0;
		}
		else
		{
			PIDSTEP[ZoneIndex] = S$MODE_OFF;		
			
			gT_Par.Ctl.OnOff[ZoneIndex]     = 0;
			gT_Par.Status.Tuning[ZoneIndex] = 0;
		}
		
		/* step machine for temperature control  */
		switch (PIDSTEP[ZoneIndex])
		{
			case S$MODE_OFF:
					LcrTempSet[ZoneIndex].enable_cooling = 0;
				
				gH_IO.y_heat[ZoneIndex] = 0;
				gH_IO.y_cool[ZoneIndex] = 0;
				
				cntTask[ZoneIndex] = 0;
				OutBuffer[ZoneIndex] = 0.0;
				
				HeatTest.tTempFreeze[ZoneIndex] = 0.0;
				gT_Par.Status.PreHeatingOk[ZoneIndex] = 0;
				
				StepTune[ZoneIndex] = C_STEP_WAIT;
				
				LCRTempTune_Array[ZoneIndex].okToCool    = 0;
				LCRTempTune_Array[ZoneIndex].okToCoolEnd = 0;
				LCRTempTune_Array[ZoneIndex].okToFree    = 0;
				LCRTempTune_Array[ZoneIndex].okToFreeEnd = 0;
				LCRTempTune_Array[ZoneIndex].okToHeat    = 0;
				
				LCRTempPID_Array[ZoneIndex].enable  = 0;
				LCRTempTune_Array[ZoneIndex].enable = 0;
				
				gT_Par.Status.Out[ZoneIndex] = 0.0;
				break;
				
			case S$MODE_PID:
					LCRTempPID_Array[ZoneIndex].enable  = 1;
				LCRTempTune_Array[ZoneIndex].enable = 0;
				
				StepTune[ZoneIndex] = C_STEP_WAIT;
				
				LCRTempPID_Array[ZoneIndex].mode = LCRTEMPPID_MODE_AUTO;
				
				LcrTempSet[ZoneIndex].PIDpara = gT_Par.Fix.ParTune[ZoneIndex];
				
				/* heating default setting pid parameter  */
				if(0.0 == LcrTempSet[ZoneIndex].PIDpara.Kp_h)LcrTempSet[ZoneIndex].PIDpara.Kp_h = 3.0;
				if(0.0 == LcrTempSet[ZoneIndex].PIDpara.Tn_h)LcrTempSet[ZoneIndex].PIDpara.Tn_h = 5.0;
				if(0.0 == LcrTempSet[ZoneIndex].PIDpara.Tv_h)LcrTempSet[ZoneIndex].PIDpara.Tv_h = 1.0;
				
				/* ------ option cooling ------ */
				if(LcrTempSet[ZoneIndex].enable_cooling != gT_Par.Fix.Cooling[ZoneIndex])
				{
					LCRTempPID_Array[ZoneIndex].update = 1;
					LcrTempSet[ZoneIndex].enable_cooling = gT_Par.Fix.Cooling[ZoneIndex];
				}

				/* cooling default setting pid parameter  */
				if(LcrTempSet[ZoneIndex].PIDpara.Kp_c < 0.1)LcrTempSet[ZoneIndex].PIDpara.Kp_c = 6.0;
				LcrTempSet[ZoneIndex].PIDpara.Tn_c = 0.0;
				LcrTempSet[ZoneIndex].PIDpara.Tv_c = 0.0;
				if(0 == ZoneIndex)LcrTempSet[ZoneIndex].PIDpara.Kp_c = 5.0;
				/* Calling fuction LCRTempPID  */
				LCRTempPID(&LCRTempPID_Array[ZoneIndex]);
			
				if(0 == LcrTempSet[ZoneIndex].enable_cooling && 0 == gT_Par.Fix.bContactor[ZoneIndex])	/*  only heating && not contactor heating*/
				{
					gH_IO.y_heat[ZoneIndex] = LCRTempPID_Array[ZoneIndex].y_heat;
					gH_IO.y_cool[ZoneIndex] = LCRTempPID_Array[ZoneIndex].y_cool;
				}
				else   /* heating and cooling */
				{
					cntSamples[ZoneIndex] = (UINT)(gT_Par.Fix.tPeriodHeat[ZoneIndex]/tTask + 0.1);
					if(cntSamples[ZoneIndex] > 0)
					{
						if(0 == (cntTask[ZoneIndex] % cntSamples[ZoneIndex]))
						{
							OutBuffer[ZoneIndex] = OutBuffer[ZoneIndex] + (LCRTempPID_Array[ZoneIndex].y_heat - LCRTempPID_Array[ZoneIndex].y_cool)/cntSamples[ZoneIndex];
							if(OutBuffer[ZoneIndex] >= 100.0)
							{
								OutBuffer[ZoneIndex] = OutBuffer[ZoneIndex] - 100.0;
								gH_IO.y_heat[ZoneIndex] = 100.0;
								gH_IO.y_cool[ZoneIndex] = 0.0;
							}
							else if(OutBuffer[ZoneIndex] <= -100.0)
							{
								OutBuffer[ZoneIndex] = OutBuffer[ZoneIndex] + 100.0;
								gH_IO.y_heat[ZoneIndex] = 0.0;
								gH_IO.y_cool[ZoneIndex] = 100;
							}
							else
							{
								gH_IO.y_heat[ZoneIndex] = 0.0;
								gH_IO.y_cool[ZoneIndex] = 0.0;
							}
						}
						else
						{
							OutBuffer[ZoneIndex] = OutBuffer[ZoneIndex] + (LCRTempPID_Array[ZoneIndex].y_heat - LCRTempPID_Array[ZoneIndex].y_cool)/cntSamples[ZoneIndex];
						}
					}
					else
					{
						gH_IO.y_heat[ZoneIndex] = LCRTempPID_Array[ZoneIndex].y_heat;
						gH_IO.y_cool[ZoneIndex] = LCRTempPID_Array[ZoneIndex].y_cool;
					}
				}
				
				cntTask[ZoneIndex] = cntTask[ZoneIndex] ++;
				if(cntTask[ZoneIndex]>= 10000)cntTask[ZoneIndex]= 1000;
				
				/* zone output percent  */
				if(LCRTempPID_Array[ZoneIndex].y_heat > 0)
				{
					gT_Par.Status.Out[ZoneIndex] = LCRTempPID_Array[ZoneIndex].y_heat;
				}
				else if (LCRTempPID_Array[ZoneIndex].y_cool > 0)
				{
					gT_Par.Status.Out[ZoneIndex] = - LCRTempPID_Array[ZoneIndex].y_cool;
				}
				else
				{
					gT_Par.Status.Out[ZoneIndex] = 0.0; 
				}
				
				/* Heating excption -- sensor   */
				if(100.0 == LCRTempPID_Array[ZoneIndex].y_heat)
				{
					HeatTest.tTempFreeze[ZoneIndex] = HeatTest.tTempFreeze[ZoneIndex] +  tTask;
				}
				else
				{
					HeatTest.tTempFreeze[ZoneIndex] = 0.0;
				}
				
				if(tTask * 2 == HeatTest.tTempFreeze[ZoneIndex])
				{
					HeatTest.LastTemp[ZoneIndex] = LCRTempPID_Array[ZoneIndex].Temp;						
				}
				
				if(gT_Par.Fix.tTempFreeze[ZoneIndex] == HeatTest.tTempFreeze[ZoneIndex] + tTask * 2)
				{
					if(LCRTempPID_Array[ZoneIndex].Temp < HeatTest.LastTemp[ZoneIndex] + 1.0)
					{
						gT_Par.Status.TempFreeze[ZoneIndex] = 1;
						if(1 == gT_Par.Fix.ProtectEna[ZoneIndex])			
						{
							gT_Par.Ctl.OnOff[ZoneIndex]    = 0;
							gT_Par.Ctl.PreOnOff[ZoneIndex] = 0;
						}
					}
				}
				break;
			
			case S$MODE_TUNE:
					LCRTempPID_Array[ZoneIndex].enable  = 0;
				LCRTempTune_Array[ZoneIndex].enable = 1;
				
				/* ------ option cooling ------ */
				if(LcrTempSet[ZoneIndex].enable_cooling != 0)
				{
					LCRTempPID_Array[ZoneIndex].update = 1;
					LcrTempSet[ZoneIndex].enable_cooling = 0;
				}

				cntTask[ZoneIndex] = 0;
				OutBuffer[ZoneIndex] = 0.0;
				
				/* tuning step machine   */
				switch (StepTune[ZoneIndex])
				{
					/* ------- step start ------- */
					case C_STEP_WAIT:
						if(1 == gT_Par.Ctl.TuneOnOff[ZoneIndex])
						{
							gT_Par.Status.TuneOk[ZoneIndex] = 0;
							LCRTempTune_Array[ZoneIndex].start = 1;
							StepTune[ZoneIndex] = C_STEP_SYNC_HEAT_WAIT;
						}
						break;
				
					/* ------- step sync heat ------- */
					case C_STEP_SYNC_HEAT_WAIT:
					
						LCRTempTune_Array[ZoneIndex].okToHeat = 1;
					
						for( i=0; i<MAX_IDX_CONTRL_ZONES;i++)	
						{
							if(1 == gT_Par.Ctl.TuneOnOff[i])
							{
								gT_Par.Status.okToHeat[i] = LCRTempTune_Array[i].rdyToHeat;
							}
							else
							{
								gT_Par.Status.okToHeat[i] = 1;
							}
					
							LCRTempTune_Array[ZoneIndex].okToHeat = LCRTempTune_Array[ZoneIndex].okToHeat & gT_Par.Status.okToHeat[i];
						}
						
						if(1 == LCRTempTune_Array[ZoneIndex].okToHeat)
						{
							StepTune[ZoneIndex] = C_STEP_HEAT;
						}
						break;
				
					/* ------- step  heating ------- */
					case C_STEP_HEAT:
					
						LCRTempTune_Array[ZoneIndex].okToFree = 1;
						
						for( i=0; i<MAX_IDX_CONTRL_ZONES;i++)	
						{
							if(1 == gT_Par.Ctl.TuneOnOff[i])
							{
								gT_Par.Status.okToFree[i] = LCRTempTune_Array[i].rdyToFree;
							}
							else
							{
								gT_Par.Status.okToFree[i] = 1;
							}
					
							LCRTempTune_Array[ZoneIndex].okToFree = LCRTempTune_Array[ZoneIndex].okToFree & gT_Par.Status.okToFree[i];
						}
					
						if(1 == LCRTempTune_Array[ZoneIndex].okToFree)
						{
							StepTune[ZoneIndex] = C_STEP_FREE;
						}
						break;
					
					/* ------- step  heating free ------- */
					case  C_STEP_FREE:
				
						LCRTempTune_Array[ZoneIndex].okToFreeEnd = 1;
						
						for( i=0; i<MAX_IDX_CONTRL_ZONES;i++)	
						{
							if(1 == gT_Par.Ctl.TuneOnOff[i])
							{
								gT_Par.Status.okToFreeEnd[i] = LCRTempTune_Array[i].rdyToFreeEnd;
							}
							else
							{
								gT_Par.Status.okToFreeEnd[i] = 1;
							}
					
							LCRTempTune_Array[ZoneIndex].okToFreeEnd = LCRTempTune_Array[ZoneIndex].okToFreeEnd & gT_Par.Status.okToFreeEnd[i];
						}
					
						if(1 == LCRTempTune_Array[ZoneIndex].okToFreeEnd)
						{
							StepTune[ZoneIndex] = C_STEP_FREE_END;
						}
						break;
					
					/* ------- step  heating free end ------- */
					case C_STEP_FREE_END:
						
						LCRTempTune_Array[ZoneIndex].okToCool = 1;
						
						for( i=0; i<MAX_IDX_CONTRL_ZONES;i++)	
						{
							if(1 == gT_Par.Ctl.TuneOnOff[i])
							{
								gT_Par.Status.okToCool[i] = LCRTempTune_Array[i].rdyToCool;
							}
							else
							{
								gT_Par.Status.okToCool[i] = 1;
							}
					
							LCRTempTune_Array[ZoneIndex].okToCool = LCRTempTune_Array[ZoneIndex].okToCool & gT_Par.Status.okToCool[i];
						}
					
						if(1 == LCRTempTune_Array[ZoneIndex].okToCool)
						{
							StepTune[ZoneIndex] = C_STEP_COOL;
						}
						break;
					/* ------- step  cooling ------- */
					case C_STEP_COOL:
						
						LCRTempTune_Array[ZoneIndex].okToCoolEnd = 1;
						
						for( i=0; i<MAX_IDX_CONTRL_ZONES;i++)	
						{
							if(1 == gT_Par.Ctl.TuneOnOff[i])
							{
								gT_Par.Status.okToCoolEnd[i] = LCRTempTune_Array[i].rdyToCoolEnd;
							}
							else
							{
								gT_Par.Status.okToCoolEnd[i] = 1;
							}
					
							LCRTempTune_Array[ZoneIndex].okToCoolEnd = LCRTempTune_Array[ZoneIndex].okToCoolEnd & gT_Par.Status.okToCoolEnd[i];
						}
						
						if(1 == LCRTempTune_Array[ZoneIndex].okToCoolEnd)
						{
							StepTune[ZoneIndex] = C_STEP_COOL_END;
						}
						break;
					
					/* ------- step  cooling end ------- */
					case C_STEP_COOL_END:
						if(1 == LCRTempTune_Array[ZoneIndex].done)
						{
							gT_Par.Status.TuneOk[ZoneIndex] = 1;
							StepTune[ZoneIndex] = C_STEP_TUNE_END;
						}
						break;
					
					/* ------- step  tune end ------- */
					case C_STEP_TUNE_END:
						LCRTempTune_Array[ZoneIndex].start = 0;
						gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
					
						gT_Par.Fix.ParTune[ZoneIndex] = LcrTempSet[ZoneIndex].PIDpara;
					
						StepTune[ZoneIndex] = C_STEP_WAIT;
						break;
				
					/* ------- step  heating error ------- */
					case C_STEP_ERROR:
						gT_Par.Ctl.TuneOnOff[ZoneIndex] = 0;
						gT_Par.Ctl.OnOff[ZoneIndex]     = 0;
						gT_Par.Ctl.PreOnOff[ZoneIndex]  = 0;
						break;
				
				}/* switch (StepTune[ZoneIndex]) */
				
				/* Calling fuction LCRTempTune  */
				LCRTempTune(&LCRTempTune_Array[ZoneIndex]);
				
				if(LCRTempTune_Array[ZoneIndex].status >= LCR_ERR_LCRPIDTune_ABORT && LCRTempTune_Array[ZoneIndex].status <= LCR_ERR_LCRPIDTune_W_CHANGED)
				{
					StepTune[ZoneIndex] = C_STEP_ERROR;
				}
				
				gT_Par.Status.TuneStatus[ZoneIndex] = StepTune[ZoneIndex];
							
				gH_IO.y_heat[ZoneIndex] = LCRTempTune_Array[ZoneIndex].y_heat;
				gH_IO.y_cool[ZoneIndex] = LCRTempTune_Array[ZoneIndex].y_cool;
				
				/* zone output percent  */
				if(LCRTempTune_Array[ZoneIndex].y_heat > 0)
				{
					gT_Par.Status.Out[ZoneIndex] = LCRTempTune_Array[ZoneIndex].y_heat;
				}
				else if (LCRTempTune_Array[ZoneIndex].y_cool > 0)
				{
					gT_Par.Status.Out[ZoneIndex] = - LCRTempTune_Array[ZoneIndex].y_cool;
				}
				else
				{
					gT_Par.Status.Out[ZoneIndex] = 0.0; 
				}
	
				
				if(100.0 == LCRTempTune_Array[ZoneIndex].y_heat)
				{
					HeatTest.tTempFreeze[ZoneIndex] = HeatTest.tTempFreeze[ZoneIndex] +  tTask;
				}
				else
				{
					HeatTest.tTempFreeze[ZoneIndex] = 0.0;
				}
				
				if(tTask * 2 == HeatTest.tTempFreeze[ZoneIndex])
				{
					HeatTest.LastTemp[ZoneIndex] = LCRTempPID_Array[ZoneIndex].Temp;						
				}
				
				if(gT_Par.Fix.tTempFreeze[ZoneIndex] == HeatTest.tTempFreeze[ZoneIndex] + tTask * 2)
				{
					if(LCRTempPID_Array[ZoneIndex].Temp < HeatTest.LastTemp[ZoneIndex] + 1.0)
					{
						gT_Par.Status.TempFreeze[ZoneIndex] = 1;
						if(1 == gT_Par.Fix.ProtectEna[ZoneIndex])			
						{
							gT_Par.Ctl.OnOff[ZoneIndex] = 0;
						}
					}
				}
				break;			
		}/* switch (PIDSTEP[ZoneIndex]) */
		
		/* ------------------ gAlarm idex information ------------------ */
		if(1 == gT_Par.Status.SensorBroken[ZoneIndex])
		{
			gT_Par.Status.idxBroken = ZoneIndex + 1;
			gT_Par.Status.bBroken = 1;
		}
		
		if(1 == gT_Par.Status.AlmHigh[ZoneIndex])
		{		
			gT_Par.Status.idxHigh = ZoneIndex + 1;
			gT_Par.Status.bHigh = 1;
		}
		
		if(1 == gT_Par.Status.AlmLow[ZoneIndex])
		{
			gT_Par.Status.idxLow = ZoneIndex + 1;
			gT_Par.Status.bLowInternal = 1;
		}
		
		if(1 == gT_Par.Status.TempSwitchOff[ZoneIndex])
		{
			gT_Par.Status.idxSwitchoff = ZoneIndex + 1;
			gT_Par.Status.bSwitchOff = 1;
		}
		
		if(1 == gT_Par.Status.TempFreeze[ZoneIndex])
		{
			gT_Par.Status.idxFreeze = ZoneIndex + 1;
			gT_Par.Status.bFreeze = 1;
		}
		
		if(1 == gT_Par.Status.bAlarmHeatNormal[ZoneIndex])
		{
			gT_Par.Status.bNotNormal = 1;	
			gT_Par.Status.idxNotNormal = ZoneIndex + 1;
		}
		
		
		TempInitOK_Old[ZoneIndex] =1; //ipis0218
		

		
		
	}/* for(ZoneIndex=0; ZoneIndex<MAX_ZoneIndex_CONTRL_ZONES; ZoneIndex++) */
	////ipis0218
	gTempfirstHeatOK = memcmp(&TempInitOK, &TempInitOK_Old , sizeof(TempInitOK));//0= all OK
	TempProtectionFlag = memcmp(&TempProtection, &TempInitOK_Old , sizeof(TempProtection));  //0= all OK
	TempNotLowFlag = memcmp(&TempNotLow, &TempInitOK_Old , sizeof(TempNotLow)); //0= all OK 

	if (0==gMachineInfo.IsHeat)
	{
		TempProtectionFix_HMI =1;
	}
	if (EDGEPOS(gMachineInfo.IsHeat,IsHeatPos))
	{
		if (0==TempProtectionFlag)
		{
			TempProtectionMode =1; //mold
			TempProtectionFix_HMI =1;
		}
		else
		{
			TempProtectionMode =0;	//fix
			TempProtectionFix_HMI =0;
		}
	}		
	
	//	if (0==gTempfirstHeatOK) //ipis0225
	if (1==gMachineInfo.IsHeat && 0==gTempfirstHeatOK)
	{
		if (0==TempProtectionMode) //fix ipis0225
		{
			//			if (tProtectCooling < gT_Par.Fix.tProtectCoolingFix+10)
			//			{
			//				tProtectCooling = tProtectCooling + tTask;
			//			}
			//			if (tProtectCooling > gT_Par.Fix.tProtectCoolingFix ) //ipis0218 force first power on and first heat, must wait 20min
			//			{
			//				gMachineInfo.ExtrHeatTimeFlag =1;
			//					TempProtectionFix_HMI =1;
			//			}
			if (tProtectCooling < gT_Par.Fix.tProtectCoolingFix+10 || tProtectCooling < gT_Par.Fix.tProtectCooling+10)
			{
				tProtectCooling = tProtectCooling + tTask;
				if(tProtectCooling > gT_Par.Fix.tProtectCoolingFix)
				{
					TempProtectionFix_HMI =1;
				}
			}
			if (tProtectCooling > gT_Par.Fix.tProtectCoolingFix  && tProtectCooling > gT_Par.Fix.tProtectCooling ) //ipis0218 force first power on and first heat, must wait 20min
			{
				gMachineInfo.ExtrHeatTimeFlag =1;
				TempProtectionFix_HMI =1;
			}
		}
		else
		{	
			if (tProtectCooling < gT_Par.Fix.tProtectCooling+10)
			{
				tProtectCooling = tProtectCooling + tTask;
			}	
			if (tProtectCooling > gT_Par.Fix.tProtectCooling )
			{	
				gMachineInfo.ExtrHeatTimeFlag =1;	
			}
		}		
	}
	else
	{
		tProtectCooling =0;
		gMachineInfo.ExtrHeatTimeFlag =0;
	}


	//ipis0226  
	//gTempfirstHeatOK  =     
	// TempNotLowFlag = all enable zone temp is in setting range
	// gMachineInfo.ExtrHeatTimeFlag  =
	if (0==gTempfirstHeatOK && 0== TempNotLowFlag && 1==gMachineInfo.ExtrHeatTimeFlag) //ipis0213
	{   
		gExtruderTempLow_HMIFlag =0;									
	}
	else
	{	
		gExtruderTempLow_HMIFlag =1;		
	}
	

	
	//ipis0213 	/* extruder protection cooling   */
	//	if (0==gTempfirstHeatOK)
	//	{
	//		tProtectCooling = tProtectCooling + tTask;
	//						
	//		
	//		if (1== gMachineInfo.ExtrHeatTimeFlag) //=ipis0225
	//		{
	//			if (tProtectCooling < gT_Par.Fix.tProtectCooling+10)
	//			{
	//				tProtectCooling = tProtectCooling + tTask;
	//			}	
	//		}
	//		else
	//		{
	//			if (tProtectCooling < gT_Par.Fix.tProtectCoolingFix+10)
	//			{
	//				tProtectCooling = tProtectCooling + tTask;
	//			}	
	//		}
	//					
	//		
	//		if (1== gMachineInfo.ExtrHeatTimeFlag)
	//		{
	//			if (tProtectCooling > gT_Par.Fix.tProtectCooling )
	//			{
	//				gMachineInfo.ExtrHeatTimeFlag =1;
	//			}
	//		}
	//		else
	//		{	
	//			if (tProtectCooling > gT_Par.Fix.tProtectCoolingFix ) //ipis0218 force first power on and first heat, must wait 20min
	//			{
	//				gMachineInfo.ExtrHeatTimeFlag =1;
	//			}	
	//			
	//		}
	//					
	//	}
	//	else
	//	{
	//		tProtectCooling =0;
	//		gMachineInfo.ExtrHeatTimeFlag =0;
	//	}
	
	
	/* extruder protection cooling   */
	if(0 == gT_Par.Status.bLowInternal)
	{
		//		tProtectCooling = tProtectCooling + tTask;
		//		if(tProtectCooling > 2 * gT_Par.Fix.tProtectCooling)
		//		{
		//			tProtectCooling = 2 * gT_Par.Fix.tProtectCooling + 1.0;
		//		}
		gT_Par.Status.bLow = 0;
		//		if(tProtectCooling >= gT_Par.Fix.tProtectCooling)
		//		{
		//			gT_Par.Status.bLow = 0;
		//		}
		//		else
		//		{
		//			gT_Par.Status.bLow = 1;  
		//		}
	}
	else
	{
		//		tProtectCooling = 0.0;
		gT_Par.Status.bLow = 1;
		//	gT_Par.Status.bLow = 0; //ipis0227
	}
	
	

	
	
	
	
	/*2018.7.24 Albert add timer show heat time*/
	if(gMachineInfo.IsHeat && 0== bLowering)
	{
		rHeaterTime += tTask;
	}
	else
	{
		rHeaterTime = 0;
	}
	uDisHeaterTime = rHeaterTime;
	
}/* end of CYCLIC   */

