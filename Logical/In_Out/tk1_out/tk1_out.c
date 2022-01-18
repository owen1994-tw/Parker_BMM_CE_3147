/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: tk1_out
 * File: tk1_out.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program tk1_out --- task 1 # output 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "pic_id.h"
//#include "keyap880.h"
#include "keyap2100.h"

#define  ABS(X)           (((X) > 0.0) ? (X) : (-1.0*(X))) 
/*************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
*************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
   
	ExtrA_Ramp.Ts = tTask;
	ExtrB_Ramp.Ts = tTask;
	ExtrV_Ramp.Ts = tTask;
	fubP_Ramp.Ts  = tTask;
    
	gHardWare.AOForceNum = 19;
	
	gMachineOut.P_Test = 0.0;
	gMachineOut.Q_Test = 0.0;
	
}/* _INIT void init ( void )  */
/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				   ****
**************************************************************************************/
_CYCLIC void cyclic ( void )
{
	
	/* --------------------- A extruder ramp function  --------------------- */
	//	if(gExtruderA.Step != 200)
	//	{
	//		ExtrA_Ramp.Accel = gMachineFix.ExtruderA.Accel;
	//	}
	//	else
	//	{
	//		ExtrA_Ramp.Accel = 1000;
	//	}
	//	if(ExtrA_Ramp.Accel == 0.0)ExtrA_Ramp.Accel = 10.0; /* 10 %/s  */
	//	ExtrA_Ramp.Set = gExtruderA.Out;
	//    
	//	/* call function */
	//	HydRampCos(&ExtrA_Ramp);
	//    
	//	gExtruderA.ActSpeed  = ExtrA_Ramp.Out;
	//   
	//	gHardWare.AO4632_1[0] = (INT)(ExtrA_Ramp.Out * 327.67) ;
	

	
	/* --------------------- B extruder ramp function  --------------------- */
	//	if(gExtruderB.Step != 200)
	//	{
	//		ExtrB_Ramp.Accel = gMachineFix.ExtruderB.Accel;
	//	}
	//	else
	//	{
	//		ExtrB_Ramp.Accel = 1000;	
	//	}
	//	if(ExtrB_Ramp.Accel == 0.0)ExtrB_Ramp.Accel = 10.0; /* 10%/s  */
	//	ExtrB_Ramp.Set = gExtruderB.Out;
	//    
	//	HydRampCos(&ExtrB_Ramp);
	//    
	//	gExtruderB.ActSpeed  = ExtrB_Ramp.Out;
	//	
	//	gHardWare.AO4632_1[3] = (INT)(ExtrB_Ramp.Out * 327.67) ;
	
	//	if(3 == gHardWare.AOForceNum)gHardWare.AO4632_1[3]  = gHardWare.AOForce[3];

	/* --------------------- visual extruder ramp function  --------------------- */
	//	ExtrV_Ramp.Accel = gMachineFix.ExtruderVisu.Accel; 
	//	if(ExtrV_Ramp.Accel == 0.0)ExtrV_Ramp.Accel = 100.0; /* 100 RPM/s  */
	//	ExtrV_Ramp.Set = gExtruderVisu.Out;
	//   
	//	HydRampCos(&ExtrV_Ramp);
	//    
	//	gExtruderVisu.ActSpeed  = ExtrV_Ramp.Out;
	
	
	/*------------------C Extruder Ao ouput for color contorl---------------*/
	
	//	gHardWare.AO4622_1[3] = gExtruderCCtrl.aoSetSpeed;
	
	/* ------------------------------------------------------------------------------ */
	/* --------------  Servo pump / System Prop valve pressure   -------------------- */
	/* ------------------------------------------------------------------------------ */
	if(1 == gMachineInfo.Auto)
	{
		gMachineOut.P_Test = 0.0;
		gMachineOut.Q_Test = 0.0;
		gMachineOut.P_Test2 = 0.0;
		gMachineOut.Q_Test2 = 0.0;
	}
    
	rMaxP = 0.0;
	rMaxP2 = 0.0;
		
	/*--------------------------------*/
	/*--------First Pressure---------*/
	/*--------------------------------*/
	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{
		if((rMaxP < gMachineOut.P_Test) && (PIC_IO_AO_1 == gPanelObj.GetPicture|| PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture ) && (1 == btnTesetPQ))
		{
			rMaxP = gMachineOut.P_Test;    
		}
	}
	else
	{
		btnTesetPQ = 0;	
	}

	
	//Ðî‰ºÆ÷Ña‰º‰ºÁ¦
	if(rMaxP < gMachineOut.P_Charge)
	{
		rMaxP =	gMachineOut.P_Charge;
	}
	
	//Ì§î^Ðî‰ºÆ÷Ña‰º‰ºÁ¦
	if(rMaxP < gMachineOut.P_Charge_Extrlift)
	{
		rMaxP =	gMachineOut.P_Charge_Extrlift;
	}
	
	if(rMaxP < RMold.Lock.p_set)
	{
		rMaxP = RMold.Lock.p_set;
	}
	
	//Clamp
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Clamp.ePressueOutputType)
	{
		if(rMaxP < RMold.Clamp.p_set)
		{
			rMaxP = RMold.Clamp.p_set;
		}
	}
	
	//Carriage
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Carriage.ePressueOutputType)
	{
		if(rMaxP < RMold.Carriage.p_set)
		{
			rMaxP = RMold.Carriage.p_set;
		}
	}

	//Blowpin
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BlowPin.ePressueOutputType)
	{
		if(rMaxP < RMold.BlowPin.p_set)
		{
			rMaxP = RMold.BlowPin.p_set;
		}
	}
	
	//Submold . Bottom Submold
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.SubMold.ePressueOutputType)
	{
		if(rMaxP < RMold.SubMold.p_set)   
		{
			rMaxP = RMold.SubMold.p_set;
		}
		if(rMaxP < RMold.BottomSubMold.p_set)
		{
			rMaxP = RMold.BottomSubMold.p_set;
		}
	}
	
	//TopDeflash
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.ePressueOutputType)
	{
		if(rMaxP < RMold.TopDeflash.p_set)
		{
			rMaxP = RMold.TopDeflash.p_set;
		}
	}
	
	//BottomDeflash
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.ePressueOutputType)
	{
		if(rMaxP < RMold.BottomDeflash.p_set)
		{
			rMaxP = RMold.BottomDeflash.p_set;
		}
	}
	
	//RobotTransfer
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.RobotTransfer.ePressueOutputType)
	{
		if(rMaxP < RMold.Robot.Transfer.p_set)
		{
			rMaxP = RMold.Robot.Transfer.p_set;
		}
	}
	
	
	
	/*--------------------------------*/
	/*--------Second Pressure---------*/
	/*--------------------------------*/
	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{
		if((rMaxP2 < gMachineOut.P_Test2) && (PIC_IO_AO_1 == gPanelObj.GetPicture || PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture ) && (1 == btnTesetPQ))
		{
			rMaxP2 = gMachineOut.P_Test2;
			//		gMachineOut.LMold.MovementValve = btnTesetPQ;
		}
	}
	else
	{
		btnTesetPQ=0;	
	}

	//Clamp
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.Clamp.ePressueOutputType)
	{
		if(rMaxP2 < LMold.Clamp.p_set)
		{
			rMaxP2 = LMold.Clamp.p_set;
		}
	}
	
	//Carriage
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.Carriage.ePressueOutputType)
	{
		if(rMaxP2 < LMold.Carriage.p_set)
		{
			rMaxP2 = LMold.Carriage.p_set;
		}
	}
	
	//Blowpin
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.BlowPin.ePressueOutputType)
	{
		if(rMaxP2 < LMold.BlowPin.p_set)
		{
			rMaxP2 = LMold.BlowPin.p_set;
		}
	}

	//Submold . Bottom Submold
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.Carriage.ePressueOutputType)
	{
		if(rMaxP2 < LMold.SubMold.p_set)   
		{
			rMaxP2 = LMold.SubMold.p_set;
		}
		if(rMaxP2 < LMold.BottomSubMold.p_set)
		{
			rMaxP2 = LMold.BottomSubMold.p_set;
		}
	}

	//TopDeflash
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.TopDeflash.ePressueOutputType)
	{
		if(rMaxP2 < LMold.TopDeflash.p_set)
		{
			rMaxP2 = LMold.TopDeflash.p_set;
		}
	}
	
	//BottomDeflash
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.BottomDeflash.ePressueOutputType)
	{
		if(rMaxP2 < LMold.BottomDeflash.p_set)
		{
			rMaxP2 = LMold.BottomDeflash.p_set;
		}
	}
	
	//RobotTransfer
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.RobotTransfer.ePressueOutputType)
	{
		if(rMaxP2 < LMold.Robot.Transfer.p_set)
		{
			rMaxP2 = LMold.Robot.Transfer.p_set;
		}
	}

	/*-------------------Special-------------------------------*/
	
	if( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.ePressueOutputType )
	{
		/*-------Plate Up Down AO L------*/
		if ( 5000== RMold.Plate.Step  || 15000==RMold.Plate.Step || 5000== LMold.Plate.Step  || 15000==LMold.Plate.Step )
		{
			if(1 == gMachineFix.Plate.bUpDnPressureOutputLeft)
			{
				if(rMaxP2 < LMold.Plate.p_set)
				{
					rMaxP2 = LMold.Plate.p_set;
				}
				if(rMaxP2 < RMold.Plate.p_set)
				{
					rMaxP2 = RMold.Plate.p_set;
				}
			
			}
			else
			{
				if(rMaxP < LMold.Plate.p_set)
				{
					rMaxP = LMold.Plate.p_set;
				}
				if(rMaxP < RMold.Plate.p_set)
				{
					rMaxP = RMold.Plate.p_set;
				}
			
			}
		}
	
		/*-------Plate Fw Bw AO R------*/
		if (6200 == RMold.Plate.Step ||16200 == RMold.Plate.Step  ||6200 == LMold.Plate.Step ||16200 == LMold.Plate.Step 
			||(RMold.Plate.Step >=6500&& RMold.Plate.Step <=6520 ) ||(LMold.Plate.Step >=6500&& LMold.Plate.Step <=6520)) //ipis0305
		{
			if(1 == gMachineFix.Plate.bFwBwPressureOutputLeft)
			{
				if(rMaxP2 < LMold.Plate.p_set)
				{
					rMaxP2 = LMold.Plate.p_set;
				}
				if(rMaxP2 < RMold.Plate.p_set)
				{
					rMaxP2 = RMold.Plate.p_set;
				}
			}
			else
			{
				if(rMaxP < LMold.Plate.p_set)
				{
					rMaxP = LMold.Plate.p_set;
				}
				if(rMaxP < RMold.Plate.p_set)
				{
					rMaxP = RMold.Plate.p_set;
				}
			}
		}
	}

	/*--------------------------------------------------------*/
	
	
	/* clean valve  */
	if(1 == gMacTimer.ValveCleanTotal.IN || 1 == gMachineInfo.Calib)
	{
		if(1 == gMachineOut.ValveFw || 1 == gMachineOut.ValveBw)
		{
			if(rMaxP < gMachineFix.Pump.P_CleanValve)rMaxP = gMachineFix.Pump.P_CleanValve;
		}
	}
	
	/* Auto running */
	if(1 == gMachineInfo.Auto)
	{
		//		if(rMaxP < gMachineFix.Pump.P_Idle)rMaxP = gMachineFix.Pump.P_Idle;	
	}
	
	if(RMold.Carriage.v_set != 0 || LMold.Carriage.v_set != 0)
	{
		//		if(rMaxP < 1.0)rMaxP = gMachineFix.Pump.P_Idle;	
	}
	
	
	/*---------------Pump Pressure------------------*/
	/* Auto running */
	if(rMaxP > gMachineFix.Pump.MaxP1)rMaxP = gMachineFix.Pump.MaxP1;
	if(rMaxP2 > gMachineFix.Pump.MaxP2) rMaxP2 = gMachineFix.Pump.MaxP2;
	/* Default value */
	if(gMachineFix.Pump.accel_P < 100)gMachineFix.Pump.accel_P = 1000;

	
	/*--First Pressure (Right)--*/
	fubP_Ramp.Set = rMaxP;
	fubP_Ramp.Accel = gMachineFix.Pump.accel_P;
	HydRampCos(&fubP_Ramp);
	aoPressure = fubP_Ramp.Out * 32767/gMachineFix.Pump.MaxP1;
	
	gMachineOut.P_Set = aoPressure / 327.67;
	gMachineOut.P_Act = aoPressure / 3276.7;
	gHardWare.AO4632_1[0] = aoPressure;
	
	
	/*--Second Pressure (Left)--*/
	fubP2_Ramp.Set = rMaxP2; 
	fubP2_Ramp.Accel = gMachineFix.Pump.accel_P;
	/* Call function */
	HydRampCos(&fubP2_Ramp);
   	
	aoPressure2 = fubP2_Ramp.Out * 32767/gMachineFix.Pump.MaxP2;
	
	gMachineOut.P_Set2 = aoPressure2 / 327.67; 
	gMachineOut.P_Act2 = aoPressure2 / 3276.7; 
	//	gHardWare.AO4632_1[1] = aoPressure2;


	

	
	
	/* ------------------------------------------------------------------------------ */
	/* ----------------------------    Servo pump flux     -------------------------- */
	/* ------------------------------------------------------------------------------ */
		
	/*---------------Servo pump flux------------------*/
	
	rMaxQ = 0.0;
	rMaxQ2 = 0.0;
	
	/*--------------------------------*/
	/*--------First Flux--------------*/
	/*--------------------------------*/
	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{
		if((rMaxQ < gMachineOut.Q_Test * 327.67) && (PIC_IO_AO_1 == gPanelObj.GetPicture|| PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture ) && (1 == btnTesetPQ))
		{
			rMaxQ = gMachineOut.Q_Test * 327.67; 
		}
	}
	
	//Ðî‰ºÆ÷Ña‰ºÁ÷Á¿
	if(rMaxQ < gMachineOut.Q_Charge * 327.67)
	{
		rMaxQ = gMachineOut.Q_Charge * 327.67;
	}
	
	//Ì§î^Ðî‰ºÆ÷Ña‰ºÁ÷Á¿
	/*output for charge Extrlift acc pump*/
	if(rMaxQ < gMachineOut.Q_Charge_Extrlift * 327.67)
	{
		rMaxQ = gMachineOut.Q_Charge_Extrlift * 327.67;
	}
	
	//	if(rMaxQ < ABS(RMold.Lock.aoPropValve))
	//	{
	//		rMaxQ = ABS(RMold.Lock.aoPropValve);
	//	}
	
	//	if(rMaxQ < ABS(LMold.Lock.aoPropValve))
	//	{
	//		rMaxQ = ABS(LMold.Lock.aoPropValve);
	//	}
	
	//Clamp
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Clamp.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.Clamp.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ = ABS(RMold.Clamp.v_set * 32.767);
		}
	}

	//Carriage
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Carriage.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.Carriage.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ = ABS(RMold.Carriage.v_set * 32.767);
		}	
	}
	
	//BlowPin
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BlowPin.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.BlowPin.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ = ABS(RMold.BlowPin.v_set * 32.767);
		}
	}
	
	//TopDeflash
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.TopDeflash.v_set * 327.67))
		{
			rMaxQ = ABS(RMold.TopDeflash.v_set * 327.67);
		}
	}
	
	
	//BottomDeflash
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.BottomDeflash.v_set * 327.67))
		{
			rMaxQ = ABS(RMold.BottomDeflash.v_set * 327.67);
		}
	}
	
	//RobotTransfer
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.RobotTransfer.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.Robot.Transfer.v_set * 327.67))
		{
			rMaxQ = ABS(RMold.Robot.Transfer.v_set * 327.67);
		}
	}
	
	//Plate
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.eFluxOutputType)
	{
		if (rMaxQ < ABS(RMold.Plate.v_set * 327.67))
		{
			rMaxQ = ABS(RMold.Plate.v_set * 327.67);
		}
	}
	
	
	/*--------------------------------*/
	/*--------Second Flux--------------*/
	/*--------------------------------*/
	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto)) //ipis0222 Pump start finish
	{
		if((rMaxQ2 < gMachineOut.Q_Test2 * 327.67) && (PIC_IO_AO_1 == gPanelObj.GetPicture|| PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture ) && (1 == btnTesetPQ))
		{
			rMaxQ2 = gMachineOut.Q_Test2 * 327.67; 
		}
	}
	
	//	if(rMaxQ2 < ABS(RMold.Lock.aoPropValve))
	//	{
	//		rMaxQ2 = ABS(RMold.Lock.aoPropValve);
	//	}
	
	//	if(rMaxQ2 < ABS(LMold.Lock.aoPropValve))
	//	{
	//		rMaxQ2 = ABS(LMold.Lock.aoPropValve);
	//	}
	
	//Clamp
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Clamp.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.Clamp.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ2 = ABS(LMold.Clamp.v_set * 32.767);
		}
	}

	//Carriage
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Carriage.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.Carriage.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ2 = ABS(LMold.Carriage.v_set * 32.767);
		}	
	}
	
	//BlowPin
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BlowPin.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.BlowPin.v_set * 32.767))	/* ProfGen.v_set 0-1000*/
		{
			rMaxQ2 = ABS(LMold.BlowPin.v_set * 32.767);
		}	
	}
	
	//TopDeflash
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.TopDeflash.v_set * 327.67))
		{
			rMaxQ2 = ABS(LMold.TopDeflash.v_set * 327.67);
		}
	}
	
	
	//BottomDeflash
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.BottomDeflash.v_set * 327.67))
		{
			rMaxQ2 = ABS(LMold.BottomDeflash.v_set * 327.67);
		}
	}
	
	//RobotTransfer
	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.RobotTransfer.eFluxOutputType)
	{
		if (rMaxQ2 < ABS(LMold.Robot.Transfer.v_set * 327.67))
		{
			rMaxQ2 = ABS(LMold.Robot.Transfer.v_set * 327.67);
		}
	}
	
//	//Plate
//	if(FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.eFluxOutputType)
//	{
//		if (rMaxQ2 < ABS(LMold.Plate.v_set * 327.67))
//		{
//			rMaxQ2 = ABS(LMold.Plate.v_set * 327.67);
//		}
//	}
	
	/*----------------------------------------------*/
	/*---------------Pump Flux Out------------------*/
	/*----------------------------------------------*/
	
	/*-----First Flux------*/
	if(rMaxQ > 32767)rMaxQ = 32767;
	aoFlux = (INT)rMaxQ;
	gMachineOut.Q_Set = aoFlux / 327.67;
	
	gHardWare.AO4632_1[1] = (INT)aoFlux;
//	gHardWare.AO4622_1[1] = aoFlux;
	
	/*-----Second Flux------*/
	if(rMaxQ2 > 32767)rMaxQ2 = 32767;
	aoFlux2 = (INT)rMaxQ2;
	gMachineOut.Q_Set2 = aoFlux2 / 327.67;
	
	//	gHardWare.AO4632_1[3] = (INT)aoFlux2;
	//	gHardWare.AO4622_1[1] = aoFlux2;
	
	/* ------------------------------------------------------------------------------ */
	/* ---------------------------- thickness valve output -------------------------- */
	/* ------------------------------------------------------------------------------ */
	
		if(1 == gMachineOut.ParisonPump)
		{
			gHardWare.AO4632_1[2] = -gThickIO.ServoOut;/*Test*/
		}
		else
		{
			gHardWare.AO4632_1[2]  = 0;
		}
	
	
	/* ------------------------------------------------------------------------------ */
	/*  ----------------------------- Flux Prop Valve  ------------------------------ */
	/* ------------------------------------------------------------------------------ */
	
	/* ----------------------*/
	/* ---------right--------*/
	/* ----------------------*/
	//Clamp
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.Clamp.eFluxOutputType)
	{
		//		gHardWare.AO4632_3[0] = RMold.Clamp.aoPropValve; 	
	}
   
	//Carriage
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.Carriage.eFluxOutputType)
	{
		//		gHardWare.AO4622_3[2] = RMold.Carriage.aoPropValve; 	
	}
	
	//BlowPin
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.BlowPin.eFluxOutputType)
	{
		//		gHardWare.AO4622_3[2] = RMold.BlowPin.aoPropValve; 	
	}
	
	/* ----------------------*/
	/* ---------right--------*/
	/* ----------------------*/
	
	//Clamp
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldL.Clamp.eFluxOutputType)
	{
		//		gHardWare.AO4632_3[1] = RMold.Clamp.aoPropValve; 	
	}
   
	//Carriage
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldL.Carriage.eFluxOutputType)
	{
		//		gHardWare.AO4622_3[2] = RMold.Carriage.aoPropValve; 	
	}
	
	//BlowPin
	if( FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldL.BlowPin.eFluxOutputType)
	{
		//		gHardWare.AO4622_3[2] = RMold.BlowPin.aoPropValve; 	
	}
	
	/* ------------------------------------------------------------------------------ */
	/* -------------------------- right temp SCR die lip---------------------------- */
	/* ------------------------------------------------------------------------------ */
	if(0 == gMachineOut.HeatPower)
	{
		bEnableRightDie = 0;
	}
	else if(oldHeatPower != gMachineOut.HeatPower) 
	{
		bEnableRightDie = gMachineOut.HeatPower;
	}
	
	if(bEnableRightDie)
	{

		if(gMachineInfo.bLoweringFlag) gMachineOut.aoRightDieLipTemp = (INT)(gUserPara.RightDieLipTempSCR - gUserPara.DieLipTempSCR_Lower);		
		else gMachineOut.aoRightDieLipTemp = (INT)gUserPara.RightDieLipTempSCR; 
	}
	else
	{
		gMachineOut.aoRightDieLipTemp = 0;
	}
	
	if(gMachineOut.aoRightDieLipTemp <= 0) gMachineOut.aoRightDieLipTemp = 0;
	gHardWare.AO4632_2[0] = gMachineOut.aoRightDieLipTemp;
	
	
	/* ------------------------------------------------------------------------------ */
	/* -------------------------- left temp SCR die lip---------------------------- */
	/* ------------------------------------------------------------------------------ */
	if(0 == gMachineOut.HeatPower)
	{
		bEnableLeftDie = 0;
	}
	else if(oldHeatPower != gMachineOut.HeatPower) 
	{
		bEnableLeftDie = gMachineOut.HeatPower;
	}
	
	
	if(bEnableLeftDie)
	{	
		
		if(gMachineInfo.bLoweringFlag) gMachineOut.aoLiftDieLipTemp = (INT)(gUserPara.LiftDieLipTempSCR - gUserPara.DieLipTempSCR_Lower);
		else gMachineOut.aoLiftDieLipTemp = (INT)gUserPara.LiftDieLipTempSCR; 
	}
	else
	{
		gMachineOut.aoLiftDieLipTemp = 0;
	}
	
	if(gMachineOut.aoLiftDieLipTemp <= 0) gMachineOut.aoLiftDieLipTemp = 0;
	gHardWare.AO4632_2[1] = gMachineOut.aoLiftDieLipTemp;

	
	
	/* ------------------------------------------------------------------------------ */
	/* --------------------------  temp SCR die lip_3    ---------------------------- */
	/* ------------------------------------------------------------------------------ */
	if(0 == gMachineOut.HeatPower)
	{
		bEnableDie3 = 0;
	}
	else if(oldHeatPower != gMachineOut.HeatPower) 
	{
		bEnableDie3 = gMachineOut.HeatPower;
	}
	
	if(bEnableDie3)
	{
		if(gMachineInfo.bLoweringFlag) gMachineOut.aoDieLipTemp3 = (INT)(gUserPara.DieLipTempSCR3 - gUserPara.DieLipTempSCR_Lower);
		else gMachineOut.aoDieLipTemp3 = (INT)gUserPara.DieLipTempSCR3; 
	}
	else
	{
		gMachineOut.aoDieLipTemp3 = 0;
	}
	
	if(gMachineOut.aoDieLipTemp3 <= 0) gMachineOut.aoDieLipTemp3 = 0;
	gHardWare.AO4632_2[2] = gMachineOut.aoDieLipTemp3;
	
	/* ------------------------------------------------------------------------------ */
	/* -------------------------- temp SCR die lip_4     ---------------------------- */
	/* ------------------------------------------------------------------------------ */
	if(0 == gMachineOut.HeatPower)
	{
		bEnableDie4 = 0;
	}
	else if(oldHeatPower != gMachineOut.HeatPower) 
	{
		bEnableDie4 = gMachineOut.HeatPower;
	}
	
	if(bEnableDie4)
	{
		if(gMachineInfo.bLoweringFlag) gMachineOut.aoDieLipTemp4 = (INT)(gUserPara.DieLipTempSCR4 - gUserPara.DieLipTempSCR_Lower);
		else gMachineOut.aoDieLipTemp4 = (INT)gUserPara.DieLipTempSCR4; 
	}
	else
	{
		gMachineOut.aoDieLipTemp4 = 0;
	}
	
	if(gMachineOut.aoDieLipTemp4 <= 0) gMachineOut.aoDieLipTemp4 = 0;
	gHardWare.AO4632_2[3] = gMachineOut.aoDieLipTemp4;
	
	
	/* ------------------------------------------------------------------------------ */
	/* ------------------------- temp SCR die lip Common ---------------------------- */
	/* ------------------------------------------------------------------------------ */
	if( (gMachineFix.Option.bDieLip[0] &&(/*0 == gUserPara.RightDieLipTempSCR ||*/ 0 == bEnableRightDie))||
		(gMachineFix.Option.bDieLip[1] &&(/*0 == gUserPara.LiftDieLipTempSCR ||*/ 0 == bEnableLeftDie))	||
		(gMachineFix.Option.bDieLip[2] &&(/*0 == gUserPara.DieLipTempSCR3 ||*/ 0 == bEnableDie3))||
		(gMachineFix.Option.bDieLip[3] &&(/*0 == gUserPara.DieLipTempSCR4 ||*/ 0 == bEnableDie4)))
	{
		gMachineInfo.DieLipTempLow = 1;
	}
	else
	{
		gMachineInfo.DieLipTempLow = 0;
	}
	
	
	/* ------------------------------------------------------------------------------ */
	/* -------------------- hot cutter heating  ŸáÇÐµ¶    --------------------------- */
	/* ------------------------------------------------------------------------------ */
	
	if(1 == gMachineFix.Option.bCutterHeating &&  1 == gMacOption.Cutter && 1 == gMachineOut.ExtrA_On)
	{
		if(bEnableCutterHeating)
		{
			gMachineOut.CutterHeating = (INT)gUserPara.CutterHeatingSCR; 
		}
		else
		{
			gMachineOut.CutterHeating = 0;
		}	
	}
	else
	{
		if( 1 == bEnableCutterHeating)
		{
			gImply.NoFunction = !gMacOption.Cutter;
			gImply.FrequencyNotOn = !gMachineOut.ExtrA_On;
			
		}

		bEnableCutterHeating = 0;
		gMachineOut.CutterHeating = 0;
	}
	if(gMachineOut.CutterHeating <= 0) gMachineOut.CutterHeating = 0;
	gHardWare.AO4632_1[3] = gMachineOut.CutterHeating;
	
	
	oldHeatPower = gMachineOut.HeatPower;
	/* ------------------------------------------------------------------------------ */
	/*  ----------------------------- Extruder AO ----------------------------------- */
	/* ------------------------------------------------------------------------------ */
	
	if(CTRL_VF == gMachineFix.ExtruderA.eCtrlType )// Analog VF Control
	{
		// Extruder A
		gHardWare.AO4632_3[3] = gExtruderA.aoSetRPM;
		// Extruder B
		gHardWare.AO4632_4[2] = gExtruderB.aoSetRPM;
		// Extruder C
		gHardWare.AO4632_4[3] = gExtruderC.aoSetRPM;
		// Extruder D
		gHardWare.AO4632_4[0] = gExtruderD.aoSetRPM;
		// Extruder Visu
		gHardWare.AO4632_4[1] = gExtruderVisu.aoSetRPM;
	}
	
	/* --------------------------------  AO Force-------------------------------- */
	//	if(1 == gHardWare.AOForceNum[0])gHardWare.AO4632_1[0]  = gHardWare.AOForce[0];
	//	if(1 == gHardWare.AOForceNum[1])gHardWare.AO4632_1[1]  = gHardWare.AOForce[1];
	//	if(1 == gHardWare.AOForceNum[2])gHardWare.AO4632_1[2]  = gHardWare.AOForce[2];
	//	if(1 == gHardWare.AOForceNum[3])gHardWare.AO4632_1[3]  = gHardWare.AOForce[3];
	//	if(1 == gHardWare.AOForceNum[4])gHardWare.AO4632_2[0]  = gHardWare.AOForce[4];
	//	if(1 == gHardWare.AOForceNum[5])gHardWare.AO4632_2[1]  = gHardWare.AOForce[5];
	//	if(1 == gHardWare.AOForceNum[6])gHardWare.AO4632_2[2]  = gHardWare.AOForce[6];
	//	if(1 == gHardWare.AOForceNum[7])gHardWare.AO4632_2[3]  = gHardWare.AOForce[7];
	//	if(1 == gHardWare.AOForceNum[8])gHardWare.AO4632_3[0]  = gHardWare.AOForce[8];
	//	if(1 == gHardWare.AOForceNum[9])gHardWare.AO4632_3[1]  = gHardWare.AOForce[9];
	//	if(1 == gHardWare.AOForceNum[10])gHardWare.AO4632_3[2]  = gHardWare.AOForce[10];/*LMold.Clamp.aoPropValve*/
	//	if(1 == gHardWare.AOForceNum[11])gHardWare.AO4632_3[3]  = gHardWare.AOForce[11];/*RMold.Clamp.aoPropValve*/
	
	if(0 == gHardWare.AOForceNum)gHardWare.AO4632_1[0]  = gHardWare.AOForce[0];
	if(1 == gHardWare.AOForceNum)gHardWare.AO4632_1[1]  = gHardWare.AOForce[1];
	if(2 == gHardWare.AOForceNum)gHardWare.AO4632_1[2]  = gHardWare.AOForce[2];
	if(3 == gHardWare.AOForceNum)gHardWare.AO4632_1[3]  = gHardWare.AOForce[3];
	
	if(4 == gHardWare.AOForceNum)gHardWare.AO4632_2[0]  = gHardWare.AOForce[4];
	if(5 == gHardWare.AOForceNum)gHardWare.AO4632_2[1]  = gHardWare.AOForce[5];
	if(6 == gHardWare.AOForceNum)gHardWare.AO4632_2[2]  = gHardWare.AOForce[6];
	if(7 == gHardWare.AOForceNum)gHardWare.AO4632_2[3]  = gHardWare.AOForce[7];
	
	if(8 == gHardWare.AOForceNum)gHardWare.AO4632_3[0]  = gHardWare.AOForce[8];
	if(9 == gHardWare.AOForceNum)gHardWare.AO4632_3[1]  = gHardWare.AOForce[9];
	if(10 == gHardWare.AOForceNum)gHardWare.AO4632_3[2]  = gHardWare.AOForce[10];
	if(11 == gHardWare.AOForceNum)gHardWare.AO4632_3[3]  = gHardWare.AOForce[11];
	
	if(12 == gHardWare.AOForceNum)gHardWare.AO4632_4[0]  = gHardWare.AOForce[12];
	if(13 == gHardWare.AOForceNum)gHardWare.AO4632_4[1]  = gHardWare.AOForce[13];
	if(14 == gHardWare.AOForceNum)gHardWare.AO4632_4[2]  = gHardWare.AOForce[14];
	if(15 == gHardWare.AOForceNum)gHardWare.AO4632_4[3]  = gHardWare.AOForce[15];
	
	
	/* ------------------------------------------------------------------------------------- */
	/* --------------------------------     Module error    -------------------------------- */
	/* ------------------------------------------------------------------------------------- */	
	if(1 == gPROCESS_info.moduleErr)
	{
		gHardWare.AO4632_1[0]  = 0;
		gHardWare.AO4632_1[1]  = 0;
		gHardWare.AO4632_1[2]  = 0;
		gHardWare.AO4632_1[3]  = 0;
       
		gHardWare.AO4632_2[0]  = 0;
		gHardWare.AO4632_2[1]  = 0;
		gHardWare.AO4632_2[2]  = 0;
		gHardWare.AO4632_2[3]  = 0;
		
		gHardWare.AO4632_3[0]  = 0;
		gHardWare.AO4632_3[1]  = 0;
		gHardWare.AO4632_3[2]  = 0;
		gHardWare.AO4632_3[3]  = 0;
	
		gHardWare.AO4632_4[0]  = 0;
		gHardWare.AO4632_4[1]  = 0;
		gHardWare.AO4632_4[2]  = 0;
		gHardWare.AO4632_4[3]  = 0;
	}/* if(1 == gPROCESS_info.moduleErr) */
}/* end of cyclic   */


