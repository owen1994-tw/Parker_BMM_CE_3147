/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: tk1_in
 * File: tk1_in.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program tk1_in --- task 1# input
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"               /*edge function                         */

/*************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
*************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
   
	memset(&LCarriageFilter,0,sizeof(LCarriageFilter));
	LCarriageFilter.enable = 1;
	LCarriageFilter.Times  = 20;

	memset(&RCarriageFilter,0,sizeof(RCarriageFilter));
	RCarriageFilter.enable = 1;
	RCarriageFilter.Times  = 20;

	memset(&LClampFilter,0,sizeof(LClampFilter));
	LClampFilter.enable = 1;
	LClampFilter.Times  = 20;

	memset(&RClampFilter,0,sizeof(RClampFilter));
	RClampFilter.enable = 1;
	RClampFilter.Times  = 20;
	
	memset(&LBlowpinFilter,0,sizeof(LBlowpinFilter));
	LBlowpinFilter.enable = 1;
	LBlowpinFilter.Times  = 20;

	memset(&RBlowpinFilter,0,sizeof(RBlowpinFilter));
	RBlowpinFilter.enable = 1;
	RBlowpinFilter.Times  = 20;

	memset(&ServoFilter,0,sizeof(ServoFilter));
	ServoFilter.enable = 1;
	ServoFilter.Times  = 10;
	
	lPT2_MeltP_A.Ts = tTask;
	lPT2_MeltP_A.T1 = tTask * 10;
	lPT2_MeltP_A.T2 = tTask * 10;
	
	lPT2_MeltP_B.Ts = tTask;
	lPT2_MeltP_B.T1 = tTask * 10;
	lPT2_MeltP_B.T2 = tTask * 10;
	
	/*2018.7.10 Albert*/
	/*ACC Pressure*/
	IPT2_ACC_P.Ts = tTask;
	IPT2_ACC_P.T1 = tTask * 10;
	IPT2_ACC_P.T2 = tTask * 10;
	
   
	/* differentiator for evaluating the actual speed */
	vActHydDT2ExtrA.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2ExtrA.Tv = 60.0; 	/* derivative time constant [s]   */
	vActHydDT2ExtrA.T1 = 2.0; 	   /* filter time constant 1 : 0.1s */
	vActHydDT2ExtrA.T2 = 2.0; 	   /* filter time constant 2 : 0.1s */
	
	/* differentiator for evaluating the actual speed */
	vActHydDT2ExtrB.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2ExtrB.Tv = 60.0; 	/* derivative time constant [s]   */
	vActHydDT2ExtrB.T1 = 2.0; 	   /* filter time constant 1 : 0.1s */
	vActHydDT2ExtrB.T2 = 2.0; 	   /* filter time constant 2 : 0.1s */
	
	/* differentiator for evaluating the actual speed */
	vActHydDT2ExtrC.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2ExtrC.Tv = 60.0; 	/* derivative time constant [s]   */
	vActHydDT2ExtrC.T1 = 2.0; 	   /* filter time constant 1 : 0.1s */
	vActHydDT2ExtrC.T2 = 2.0; 	   /* filter time constant 2 : 0.1s */
   
	/* differentiator for evaluating the actual speed */
	vActHydDT2ExtrVisu.Ts = tTask;	/* cycle time 0.002s 			  */
	vActHydDT2ExtrVisu.Tv = 60.0; 	/* derivative time constant [s]   */
	vActHydDT2ExtrVisu.T1 = 2.0; 	   /* filter time constant 1 : 0.1s */
	vActHydDT2ExtrVisu.T2 = 2.0; 	   /* filter time constant 2 : 0.1s */	
	
	

	
	
	/*Servo Flux linear*/
	LinSplineOilFlux.enable = 1;
	LinSplineOilFlux.NbNodes = 2;
	LinSplineOilFlux.Smooth = 0;
	LinSplineOilFlux.Rness = 100.0;
	LinSplineOilFlux.Invert = 0;
	LinSplineOilFlux.DisMode = 0;
	LinSplineOilFlux.pNodeVec = &OilFluxNode[0];
	LinSplineOilFlux.Update	=	1;

	
}/* _INIT void init ( void )  */
/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				           ****
**************************************************************************************/
_CYCLIC void cyclic ( void )
{
	/* ------------- thickness LVDT ------------- */
		if(gHardWare.AI4632_2[0]  >= 0)
		{
			ServoFilter.u= gHardWare.AI4632_2[0];
			HydMovAvgFlt(&ServoFilter);
		}
		gServoLinear.Current = ServoFilter.y;
	
	/* -------------  right carriage LVDT ------------- */
	//		if(gHardWare.AI4632_1[3] >= 0)
	//		{
	//			RCarriageFilter.u = gHardWare.AI4632_1[3];
	//			HydMovAvgFlt(&RCarriageFilter);
	//		}
	//	
	//		RMold.Carriage.As = RCarriageFilter.y;
	
	/* -------------  left carriage LVDT ------------- */
	//		if(gHardWare.AI4632_1[2] >= 0)
	//		{
	//			LCarriageFilter.u = gHardWare.AI4632_1[2];
	//			HydMovAvgFlt(&LCarriageFilter);
	//		}
	//	
	//		LMold.Carriage.As = LCarriageFilter.y;

	/* -------------  right clamp LVDT  ------------- */
	if(gHardWare.AI4632_1[0]>= 0)
	{
		RClampFilter.u = gHardWare.AI4632_1[0];
		HydMovAvgFlt(&RClampFilter);
	}
	
	if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)
	{
		RMold.Clamp.As  = RClampFilter.y;
	}
	
	/* -------------  left clamp LVDT  ------------- */
	if(gHardWare.AI4632_1[1] >= 0)
	{
		LClampFilter.u = gHardWare.AI4632_1[1];
		HydMovAvgFlt(&LClampFilter);
	}
	if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)
	{
		LMold.Clamp.As  = LClampFilter.y;
	}
	
	/* -------------  right blowpin LVDT  ------------- */
	if(gHardWare.AI4632_2[1]>= 0)
	{
		RBlowpinFilter.u = gHardWare.AI4632_2[1];
		HydMovAvgFlt(&RBlowpinFilter);
	}
	
	if(ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		RMold.BlowPin.As  = RBlowpinFilter.y;
	}
	
//	/* -------------  left blowpin LVDT  ------------- */
//	if(gHardWare.AI4632_1[1] >= 0)
//	{
//		LBlowpinFilter.u = gHardWare.AI4632_1[1];
//		HydMovAvgFlt(&LBlowpinFilter);
//	}
//	if(ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType)
//	{
//		LMold.BlowPin.As  = LBlowpinFilter.y;
//	}


	/* ----------------- A extruder speed ----------------- */
	/*------------------*/
	/*-----DI Pulse-----*/
	
	//	gExtruderA.diActRPM = gHardWare.DI9371_3[6];
	//	if(1 == EDGEPOS(gExtruderA.diActRPM,DISpeedExtrA))
	//	{
	//		RotatespeedExtrA = RotatespeedExtrA + 1;
	//	}
	//		
	//	DISpeedExtrA = gExtruderA.diActRPM;
	//
	//	vActHydDT2ExtrA.u = RotatespeedExtrA * 0.1;
	//	
	//	if(vActHydDT2ExtrA.u < 0)vActHydDT2ExtrA.u = -vActHydDT2ExtrA.u;
	//	
	//	/* call function block */
	//	HydDT2(&vActHydDT2ExtrA);
	//  	
	//	gExtruderA.ActRPM = vActHydDT2ExtrA.y;
	//	if(gExtruderA.ActRPM < 0)gExtruderA.ActRPM= -gExtruderA.ActRPM;
   
	//	gExtruderA.aiCurrent = gHardWare.AI4632_1[2];
	//	gExtruderA.aiMeltPressure = gHardWare.AI4632_4[0];
	
	/*-------------------------*/
	/*------AI  0 ~ 32767------*/
	gExtruderA.aiActRPM = gHardWare.AI4632_2[0];
	
	/* ----------------- B extruder speed ----------------- */
	
	/*------------------*/
	/*-----DI Pulse-----*/
	//	gExtruderB.diActRPM = gHardWare.DI9371_3[0];
	//	if(1 == EDGEPOS(gExtruderB.diActRPM,DISpeedExtrB))
	//	{
	//		RotatespeedExtrB = RotatespeedExtrB + 1;
	//	}
	//		
	//	DISpeedExtrB = gExtruderB.diActRPM;
	//	
	//	vActHydDT2ExtrB.u = RotatespeedExtrB * 0.1;
	//	
	//	if(vActHydDT2ExtrB.u < 0)vActHydDT2ExtrB.u = -vActHydDT2ExtrB.u;
	//	
	//	/* call function block */
	//	HydDT2(&vActHydDT2ExtrB);
	//	
	//	gExtruderB.ActRPM = vActHydDT2ExtrB.y;
	//	if(gExtruderB.ActRPM < 0)gExtruderB.ActRPM = -gExtruderB.ActRPM;
	
	//	gExtruderB.aiActRPM  = gHardWare.AI4632_4[3];
	//	gExtruderB.aiCurrent = gHardWare.AI4632_1[3];
	
	/*-------------------------*/
	/*------AI  0 ~ 32767------*/
	gExtruderB.aiActRPM = gHardWare.AI4632_3[2];
   
	/* ----------------- C extruder speed ----------------- */
	
	/*------------------*/
	/*-----DI Pulse-----*/
	//	if(1 == EDGEPOS(gExtruderC.diActRPM,DISpeedExtrC))
	//	{
	//		RotatespeedExtrC = RotatespeedExtrC + 1;
	//	}
	//		
	//	DISpeedExtrC = gExtruderC.diActRPM;
	//	
	//	vActHydDT2ExtrC.u = RotatespeedExtrC * 0.1;
	//	
	//	if(vActHydDT2ExtrC.u < 0)vActHydDT2ExtrC.u = -vActHydDT2ExtrC.u;
	//	
	//	/* call function block */
	//	HydDT2(&vActHydDT2ExtrC);
	//   
	//	gExtruderC.ActRPM = vActHydDT2ExtrC.y;
	//	if(gExtruderC.ActRPM < 0)gExtruderC.ActRPM = -gExtruderC.ActRPM;
	
	/*-------------------------*/
	/*------AI  0 ~ 32767------*/
	gExtruderC.aiActRPM = gHardWare.AI4632_3[3];
	
	/* ----------------- D extruder speed ----------------- */
	
	/*------------------*/
	/*-----DI Pulse-----*/
	//	gExtruderD.diActRPM = gHardWare.DI9371_3[7];
	//	if(1 == EDGEPOS(gExtruderD.diActRPM,DISpeedExtrD))
	//	{
	//		RotatespeedExtrD = RotatespeedExtrD + 1;
	//	}
	//		
	//	DISpeedExtrD = gExtruderD.diActRPM;
	//	
	//	vActHydDT2ExtrD.u = RotatespeedExtrD * 0.1;
	//	
	//	if(vActHydDT2ExtrD.u < 0)vActHydDT2ExtrD.u = -vActHydDT2ExtrD.u;
	//	
	//	/* call function block */
	//	HydDT2(&vActHydDT2ExtrD);
	//   
	//	gExtruderD.ActRPM = vActHydDT2ExtrD.y;
	//	if(gExtruderD.ActRPM < 0)gExtruderD.ActRPM = -gExtruderD.ActRPM;
	//   
	//	gExtruderD.aiActRPM  = gHardWare.AI4632_4[0];
	//	gExtruderD.aiCurrent = gHardWare.AI4632_4[0];

	/*-------------------------*/
	/*------AI  0 ~ 32767------*/
	gExtruderD.aiActRPM = gHardWare.AI4632_3[0];
   
	/* ----------------- Visual extruder speed ----------------- */
	
	/*------------------*/
	/*-----DI Pulse-----*/
	//	gExtruderVisu.diActRPM = gHardWare.DI9371_3[7];
	//	if(1 == EDGEPOS(gExtruderVisu.diActRPM,DISpeedExtrVisu))
	//	{
	//		RotatespeedExtrVisu = RotatespeedExtrVisu + 1;
	//	}
	//		
	//	DISpeedExtrVisu = gExtruderVisu.diActRPM;
	//	
	//	vActHydDT2ExtrVisu.u = RotatespeedExtrVisu * 0.1;
	//	
	//	if(vActHydDT2ExtrVisu.u < 0)vActHydDT2ExtrVisu.u = -vActHydDT2ExtrVisu.u;
	//	
	//	/* call function block */
	//	HydDT2(&vActHydDT2ExtrVisu);
	//   
	//	gExtruderVisu.ActRPM = vActHydDT2ExtrVisu.y;
	//	if(gExtruderVisu.ActRPM < 0)gExtruderVisu.ActRPM = -gExtruderD.ActRPM;
	//   
	//	gExtruderVisu.aiActRPM  = gHardWare.AI4632_4[0];
	//	gExtruderVisu.aiCurrent = gHardWare.AI4632_4[0];
	
	/*-------------------------*/
	/*------AI  0 ~ 32767------*/
	gExtruderVisu.aiActRPM = gHardWare.AI4632_3[1];
	
	/* ------------- A extruder melt pressure ------------- */
	if(0 == gMachineFix.ExtruderA.MeltP10V)gMachineFix.ExtruderA.MeltP10V = 8.0;
	
	lPT2_MeltP_A.u = gExtruderA.aiMeltPressure;
    
	/* call function block */
	HydPT2(&lPT2_MeltP_A);

	if((gMachineFix.ExtruderA.MeltP10V - gMachineFix.ExtruderA.MeltP0V) != 0.0) 
	{
		gExtruderA.MeltPressure = (lPT2_MeltP_A.y/3276.7 - gMachineFix.ExtruderA.MeltP0V)* 560.0 /(gMachineFix.ExtruderA.MeltP10V - gMachineFix.ExtruderA.MeltP0V);
	}
	
	/* ------------- B extruder melt pressure ------------- */
	if(0 == gMachineFix.ExtruderB.MeltP10V)gMachineFix.ExtruderB.MeltP10V = 8.0;
	
	lPT2_MeltP_B.u = gExtruderB.aiMeltPressure;
    
	/* call function block */
	HydPT2(&lPT2_MeltP_B);

	if((gMachineFix.ExtruderB.MeltP10V - gMachineFix.ExtruderB.MeltP0V) != 0.0) 
	{
		gExtruderB.MeltPressure = (lPT2_MeltP_B.y/3276.7 - gMachineFix.ExtruderB.MeltP0V)* 560.0 /(gMachineFix.ExtruderB.MeltP10V - gMachineFix.ExtruderB.MeltP0V);
	}
	/* ----------------------------------------- */
	/* ------------- ACC pressure ------------- */
	/* ----------------------------------------- */
	
	IPT2_ACC_P.u = gHardWare.AI4632_1[2];
	
	/* call function block */
	HydPT2(&IPT2_ACC_P);
	
	//	gMachineInfo.OilAccuPressure = (IPT2_ACC_P.y/32767) * gMachineFix.OilAccu.AccuPressure10V;
	/*--------------------------------------------------------------------------------------------------------------------*/	
	//	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	//	{
	//		if(IPT2_ACC_P.y !=0)
	//		{
	//			if(1==	gMachineInfo.bAccuPumpCalib)
	//			{
	//				gUserPara.AccuPressure_Calib = gMachineFix.OilAccu.AccuPressure10V/IPT2_ACC_P.y;
	//			}		
	//		}	
	//	}
	//	gMachineInfo.OilAccuPressure = IPT2_ACC_P.y*gUserPara.AccuPressure_Calib;
	/*--------------------------------------------------------------------------------------------------------------------*/	
	if (1 == gMacOption.OilPumpStartMode)
	{
		if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
		{			
			if(IPT2_ACC_P.y !=0)
			{
				if(1==	gMachineInfo.bAccuPumpCalib)
				{
					//	gUserPara.AccuPressure_Calib = gMachineFix.OilAccu.AccuPressure10V/IPT2_ACC_P.y;
					//gUserPara.AccuPressure_Calib = IPT2_ACC_P.y;
					gUserFix_new.AccuPressure_Calib = IPT2_ACC_P.y;
				}						
			}	
		}
	}
	else
	{
		if ((1==gMachineIn.OilPumpOk_DI && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
		{			
			if(IPT2_ACC_P.y !=0)
			{
				if(1==	gMachineInfo.bAccuPumpCalib)
				{
					//	gUserPara.AccuPressure_Calib = gMachineFix.OilAccu.AccuPressure10V/IPT2_ACC_P.y;
					//gUserPara.AccuPressure_Calib = IPT2_ACC_P.y;
					gUserFix_new.AccuPressure_Calib = IPT2_ACC_P.y;
		
				}						
			}	
		}
	}
	//	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	//	{			
	//		if(IPT2_ACC_P.y !=0)
	//		{
	//			if(1==	gMachineInfo.bAccuPumpCalib)
	//			{
	//				//	gUserPara.AccuPressure_Calib = gMachineFix.OilAccu.AccuPressure10V/IPT2_ACC_P.y;
	//				//gUserPara.AccuPressure_Calib = IPT2_ACC_P.y;
	//				gUserFix_new.AccuPressure_Calib = IPT2_ACC_P.y;
	//		
	//			}						
	//		}	
	//		
	//		
	//	}
	if ((0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{			
		if(1==	gMachineInfo.bAccuPumpMinCalib)
		{
			//	gUserPara.AccuPumpMinCalib = IPT2_ACC_P.y;
			gUserFix_new.AccuPumpMinCalib = IPT2_ACC_P.y;
		}
	}
	if (IPT2_ACC_P.y-gUserFix_new.AccuPumpMinCalib >=0)
	{
		if (0!=(gUserFix_new.AccuPressure_Calib-gUserFix_new.AccuPumpMinCalib))  //ipis1207
		{
			gMachineInfo.OilAccuPressure=(IPT2_ACC_P.y-gUserFix_new.AccuPumpMinCalib)*((gMachineFix.OilAccu.AccuPressure10V-gUserFix_new.AccuPressure0V)/(gUserFix_new.AccuPressure_Calib-gUserFix_new.AccuPumpMinCalib));			
		}
	}
	else
	{
		gMachineInfo.OilAccuPressure =0;
	}
	
	
	/*--------------------------------------------------------------------------------------------------------------------*/
	
	
	
	/* ------------------------------------------------------ */
	/* ------------- Extrlift ACC pressure ------------------ */
	/* ------------------------------------------------------ */
	
	IPT2_ACC_Extrlift_P.u = gHardWare.AI4632_1[1];
	
	/* call function block */
	HydPT2(&IPT2_ACC_Extrlift_P);
		
	if (1 == gMacOption.OilPumpStartMode)
	{
		if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
		{			
			if(IPT2_ACC_Extrlift_P.y !=0)
			{
				if(1==	gMachineInfo.bAccuPump_Extrlift_Calib)
				{
					//	gUserPara.AccuPressure_Calib = gMachineFix.OilAccu.AccuPressure10V/IPT2_ACC_Extrlift_P.y;
					//gUserPara.AccuPressure_Calib = IPT2_ACC_Extrlift_P.y;
					gUserFix_new.AccuPressure_Extrlift_Calib = IPT2_ACC_Extrlift_P.y;
				}						
			}	
		}
	}
	else
	{
		if ((1==gMachineIn.OilPumpOk_DI && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
		{			
			if(IPT2_ACC_Extrlift_P.y !=0)
			{
				if( 1 == gMachineInfo.bAccuPump_Extrlift_Calib)
				{
					//	gUserPara.AccuPressure_Extrlift_Calib = gMachineFix.OilAccu_Extrlift.AccuPressure10V/IPT2_ACC_Extrlift_P.y;
					//gUserPara.AccuPressure_Extrlift_Calib = IPT2_ACC_Extrlift_P.y;
					gUserFix_new.AccuPressure_Extrlift_Calib = IPT2_ACC_Extrlift_P.y;
		
				}						
			}	
		}
	}

	if ((0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{			
		if(1 == gMachineInfo.bAccuPump_Extrlift_MinCalib)
		{
			//	gUserPara.AccuPumpMinCalib = IPT2_ACC_Extrlift_P.y;
			gUserFix_new.AccuPump_Extrlift_MinCalib = IPT2_ACC_Extrlift_P.y;
		}
	}
	if (IPT2_ACC_Extrlift_P.y-gUserFix_new.AccuPump_Extrlift_MinCalib >=0)
	{
		if (0!=(gUserFix_new.AccuPressure_Extrlift_Calib - gUserFix_new.AccuPump_Extrlift_MinCalib))  //ipis1207
		{
			gMachineInfo.OilAccuPressure_Extrlift=(IPT2_ACC_Extrlift_P.y-gUserFix_new.AccuPump_Extrlift_MinCalib)*((gMachineFix.OilAccu_Extrlift.AccuPressure10V-gUserFix_new.AccuPressure_Extrlift_0V)/(gUserFix_new.AccuPressure_Extrlift_Calib-gUserFix_new.AccuPump_Extrlift_MinCalib));			
		}
	}
	else
	{
		gMachineInfo.OilAccuPressure_Extrlift =0;
	}
	
	
	/*--------------------------------------------------------------------------------------------------------------------*/
	
	
	
	
	/*---------------Servo Flow----------------*/
	
	OilFluxNode[0].X = gUserFix.iMinFluxV;
	OilFluxNode[1].X = gUserFix.iMaxFluxV;
	
	OilFluxNode[0].Y = gUserFix.rMinServoFlow;
	OilFluxNode[1].Y = gUserFix.rMaxServoFlow;
	
	LinSplineOilFlux.Update = gRecipeOperate.OnceSave;
	
	IPT2_OilFlux.u = gHardWare.AI4632_1[3];
	
	/* call function block */
	HydPT2(&IPT2_OilFlux);
	
	gMachineInfo.aiOilFlux = (int)(IPT2_OilFlux.y  * 100/32767) ;
	
	
	LinSplineOilFlux.x = IPT2_OilFlux.y;
	
	HCRLinSpline(&LinSplineOilFlux);
	
	//	gMachineInfo.aiOilFlux = LinSplineOilFlux.y;
	
	//	OilFluxNode[0].X = 
	//	
	//	OilFluxNode[0].Y = 
	

	
	
	
}/* end of cyclic   */


