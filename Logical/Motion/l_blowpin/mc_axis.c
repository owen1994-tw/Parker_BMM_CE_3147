
/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: mc_axis
 * File: mc_axis.c
 * Author: lij
 * Created: December 21, 2015
 ********************************************************************
 * Implementation of program mc_axis
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

//#include "keyap880.h"
#include "keyap2100.h"
#include "edge.h"

#define	STEP_WAIT				   0
#define	STEP_PAR_INIT           1
#define	STEP_PAR_INIT_W         2
#define	STEP_PAR_INIT_OK        3

#define	STEP_POWER_ON_READY		6
#define	STEP_POWER_ON			   7
#define	STEP_HOME				   8

#define	STEP_READY				   10
#define	STEP_HALT					11
#define	STEP_STOP					12
#define	STEP_JOG_POSITIVE			13
#define	STEP_JOG_NEGATIVE			14
#define	STEP_MOVE_ABSOLUTE		15
#define	STEP_MOVE_ADDITIVE		16	
#define	STEP_MOVE_VELOCITY		17

#define	STEP_ERROR					100
#define	STEP_ERROR_AXIS			101
#define	STEP_ERROR_RESET			102

#define	STEP_WRITE_ID_WAIT		0
#define	STEP_WRITE_ID_READY     10
#define	STEP_WRITE_ID_SCAN      100
#define	STEP_WRITE_ID_RUN       110
#define	STEP_WRITE_ID_OK        200

#define INDEX_PAR_WRITE_ID_MAX  	13
#define INDEX_PAR_INIT_ID_MAX   	3

#define	STEP_READ_ID_WAIT			0
#define	STEP_READ_ID_READY      10
#define	STEP_READ_ID_RUN        100
#define	STEP_READ_ID_OK         200

#define INDEX_PAR_READ_ID_MAX  	8
/* function declare  */
void ConfigureParID();

void _INIT ProgramInit(void)
{
	// Insert code here 
	/* TODO: Add code here */
	mcAxis.AxisID.Address = (UDINT)&(gAxisBlowPinL); 	
	strcpy(mcAxis.AxisID.Name,"BlowPinL");
	mcAxis.AxisID.UnitFactor = 0.01;
	strcpy(mcAxis.AxisID.UnitName, "mm");
	mcAxis.AxisID.AxisType = ncAXIS;

	mcAxis.Parameter.Velocity 	        	= 1000; /*velocity for movement*/
	mcAxis.Parameter.Acceleration 		= 4000; /*acceleration for movement*/
	mcAxis.Parameter.Deceleration 		= 4000; /*deceleration for movement*/
	mcAxis.Parameter.JogVelocity		   = 400; 	/*velocity for jogging */
	mcAxis.Parameter.OverrideVelocity	= 1.0;
	mcAxis.Parameter.OverrideAcceleration=1.0;

	ConfigureParID();
	
	mcParID_Value.pidPar.kpJunction  = 0.666;		//Kp = (43-9.7)/50 = 0.666
	mcParID_Value.pidPar.kpMotorTemp = 0.666;
	
	mcParID_Value.pidPar.kv_p = 170;
	mcParID_Value.pidPar.tn_p = 0.0;
	
	mcParID_Value.pidPar.kv_s = 3.25;
	mcParID_Value.pidPar.tn_s = 0.05;
	
	Torque_PT1.Ts = 0.002;
	Torque_PT1.Tf = 0.08;
	
	/*Albert*/
	
	uCountTimer = 100;/*1 second*/

}

void _CYCLIC ProgramCyclic(void)
{
	/*---------------Motor Trand-------------*/
	gMotorTrace.LMold.BlowPin.RPM 		= mcAxis.Status.ActVelocity*1/2000*60;
	gMotorTrace.LMold.BlowPin.Current	= ActCurrent/1.414;
	gMotorTrace.LMold.BlowPin.Torque 	= mcAxis.CycRead.torq_act; 
	
	
	/*Albert Add power consumption function*/
	
	switch (uStepPower)
	{
		case 0:/*Weit step*/
			if(gMachineInfo.Auto)
				//			if(bTestAuto)
			{
				fnTON_10ms.IN = 1;
				fnTON_10ms.PT = uCountTimer;
				uStepPower = 10;
			}
			break;
		case 10:/*Start Timer*/
			if(fnTON_10ms.Q)
			{
				fnTON_10ms.IN = 0;
				uIndex++;
				uStepPower = 20;
			}
			if(0 == gMachineInfo.Auto)
				//if(0 == bTestAuto)
			{
				uIndex	=	0;
				fnTON_10ms.IN = 0;
				uStepPower = 0;
			}
			break;
		case 20:/*Get value*/
			if(uIndex > 59) /*if it is save 60s then get the value*/
			{
				uIndex = 0;
				rPowerTotal += (rPowerTotalPerSec/3600);/*3600 = 60(times) * 60(min)*/
			}
			rPowerTotalPerSec = 0;
			
			rActCurrent[uIndex] = mcParID_Value.readPar.actCurrent/1.414;
			rActVoltage[uIndex] = mcParID_Value.readPar.actUDC;
			
//			rActCurrent[uIndex] = rTestCur;
//			rActVoltage[uIndex] = rTestVol;
			
			uStepPower = 30;
			
			break;
		case 30:/*Calculate*/
			rPower[uIndex] = (rActCurrent[uIndex] * rActVoltage[uIndex])/1000;
			for(i=0;i<60;i++)
			{
				rPowerTotalPerSec += rPower[i];
			}
			rPowerPerMin = rPowerTotalPerSec/60;
			uStepPower = 0;
			break;
	}
	
	
	/*Function call*/
	TON_10ms(&fnTON_10ms);
	
		
	/*Reset Function*/
	if(bCleanPower)
	{
		iClickTime--;
		if(iClickTime <= 0)
		{
			bCleanPower = 0;
			rPowerTotal = 0;
			rPowerPerMin = 0;
		}
	}
	else
	{
		iClickTime = 500;
	}
	
	// Insert code here 
	mcAxis.AxisCmd.Power = LMold.BlowPin.BtnPowerOn;
	
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET],OldAlarmConfirmOK))
	{
		mcAxis.AxisCmd.ErrorAcknowledge = 1;
	}
	
	mcAxis.AxisID.UnitFactor = 0.01;
	
	/* ------------------ Get setting parameter ------------------ */
	if(LMold.BlowPin.v_set > 1000)LMold.BlowPin.v_set=1000;
	if(LMold.BlowPin.v_set < -1000)LMold.BlowPin.v_set=-1000;
	
	
	/*-----v_set -------*/
	//	mcAxis.CycWrite.v_set = LMold.Carriage.v_set  * X / mcAxis.AxisID.UnitFactor;	
	//	?R?_ 
	//	3000rpm = 3000rev/min = 50 rev/ s
	//			
	//	?????? 
	//	???D/???? 20 mm / rev 
	//	V Max 0.65 m/s = 650 mm/s = 32.5 rev/s = 1950 rev / min
	//	A Max 15 m/s2  = 15000 mm/s2 = 750 rev/s2
	//
	//	?????????? ??????V max = 32.5 rev/s
	//	20mm/rev 
	//	=> (100 unit  = 1 mm)
	//	=> 20*100 units /rev 
	//	2000 uints /rev
	//	X = 32.5 rev/s * (2000 units/rev) * 0.01( mm/units) * 0.001(m / mm) = 0.65m/s
	
	/*------torq_set------*/
	//	mcAxis.CycWrite.torq_set = mcParID_Value.readPar.torqueRated * X * Torque_PT1.y /100.0;
	//	?R?_????????   43     ??????????????63.7    ??????
	//	???????????R?_????????  
	//		X	= (?R?_???????e/?p????) / ?R?_?~?????e
	//		= (43/1)/9.7
	//		=  4.43
	
	
	if(mcAxis.AxisID.UnitFactor != 0)
	{
		mcAxis.CycWrite.v_set = (LMold.BlowPin.v_set * 0.65 / mcAxis.AxisID.UnitFactor)/2000;   // 0.65
	}
	else
	{
		mcAxis.CycWrite.v_set = LMold.BlowPin.v_set;	
	}
	
	if(LMold.BlowPin.p_set > 100)LMold.BlowPin.p_set = 100;
	if(LMold.BlowPin.p_set < 5 )LMold.BlowPin.p_set = 5;
	
	Torque_PT1.u  = LMold.BlowPin.p_set;
	HydPT1(&Torque_PT1);
	
	mcAxis.CycWrite.torq_set = mcParID_Value.readPar.torqueRated * 3.98 * Torque_PT1.y /100.0;    //3.98 =(43*0.9/1)/9.7  gearbox max.T*safefactor(0.9) / gear ratio / nominal T 
		
	/* ------------------ error Acknowledge ------------------ */
	if(1 == EDGENEG(gAxisBlowPinL.network.init,NegNetworkInit))
	{
		mcAxis.AxisCmd.Init = 0;
		mcAxis.AxisCmd.Stop = 0;
		mcAxis.AxisCmd.Home = 0;
		mcAxis.AxisCmd.MoveJogPos = 0;
		mcAxis.AxisCmd.MoveJogNeg = 0;
		mcAxis.AxisCmd.MoveVelocity = 0;
		mcAxis.AxisCmd.MoveAbsolute = 0;
		mcAxis.AxisCmd.MoveAdditive = 0;
		mcAxis.AxisCmd.ErrorAcknowledge = 0;
		return;
	}
	
	if(mcAxis.Step > STEP_WAIT && mcAxis.Step < STEP_ERROR)mcAxis.AxisCmd.ErrorAcknowledge = 0;
	
	/**************** CHECK FOR GENERAL AXIS ERROR ******************/
	if ((MC_ReadAxisError_0.AxisErrorID != 0) && (MC_ReadAxisError_0.Valid == 1)) 
	{
		if(mcAxis.Step > STEP_WAIT && mcAxis.Step < STEP_ERROR)mcAxis.Step = STEP_ERROR;
	}
	else if(mcAxis.AxisCmd.Power == 0) 
	{
		if((MC_ReadStatus_0.Errorstop == 1) && (MC_ReadStatus_0.Valid == 1)) 
		{
			mcAxis.Step = STEP_ERROR_RESET;
		}
		else
		{
			if(mcAxis.Step >= STEP_POWER_ON_READY)mcAxis.Step = STEP_WAIT;
		}
	}
	else
	{
		if((MC_ReadStatus_0.Errorstop == 1) && (MC_ReadStatus_0.Valid == 1))
		{
			if(1 == mcAxis.AxisCmd.ErrorAcknowledge)
			{
				mcAxis.AxisCmd.ErrorAcknowledge = 0;
				mcAxis.Step = STEP_ERROR_RESET;
			}
		}
	}
	
	/**************** step machine logic control ******************/
	switch(mcAxis.Step)
	{
		/******************** WAIT *************************/
		case STEP_WAIT:  /* 0 */
			if(gAxisBlowPinL.network.init == ncTRUE)
			{
				mcAxis.Step = STEP_PAR_INIT_OK;
			}
			break;
		
		/******************** INIT PARAMETER *************************/
		case STEP_PAR_INIT:  /* 1 */
			MC_BR_WriteParID_0.Execute = 1;
			MC_BR_WriteParID_0.ParID   = mcParID_RW.init[mcParID_RW.idxInit].ParID;
			MC_BR_WriteParID_0.DataAddress = mcParID_RW.init[mcParID_RW.idxInit].DataAddress;
			MC_BR_WriteParID_0.DataType    = mcParID_RW.init[mcParID_RW.idxInit].DataType;

			if (1 == MC_BR_WriteParID_0.Done)
			{
				MC_BR_WriteParID_0.Execute = 0;
				mcAxis.Step = STEP_PAR_INIT_W;
			}
			else if (MC_BR_WriteParID_0.Error)
			{  
				mcAxis.Status.ErrorID = MC_BR_WriteParID_0.ErrorID;
				MC_BR_WriteParID_0.Execute = 0;
				mcAxis.Step = STEP_PAR_INIT_W;
			}
			break;
		
		/******************** INIT PARAMETER WAIT *************************/
		case STEP_PAR_INIT_W: /* 2 */
			mcParID_RW.idxInit ++;
			if(mcParID_RW.idxInit < INDEX_PAR_INIT_ID_MAX) 
			{
				mcAxis.Step = STEP_PAR_INIT;
			}
			else
			{
				mcParID_RW.idxInit   = 0;
				mcAxis.Step = STEP_PAR_INIT_OK;
			}
			break;
		
		/******************** INIT PARAMETER OK *************************/
		case STEP_PAR_INIT_OK: /* 3 */
			MC_Power_0.Enable = 0;
			if(1 == LMold.BlowPin.BtnPowerOn)
			{
				mcAxis.AxisCmd.Power = 1;
				mcAxis.Step = STEP_POWER_ON_READY;
			}
			break;
		
		/******************** POWER ON READY *************************/
		case STEP_POWER_ON_READY: /* 6 */ /* STATE: POWER_ON READY */
			if (mcAxis.AxisCmd.Power == 1)
			{
				mcAxis.Step = STEP_POWER_ON;
			}
			else
			{
				MC_Power_0.Enable = 0;
			}
	
			/* reset all fb execute inputs we use */
			MC_Home_0.Execute 		  = 0;
			MC_Stop_0.Execute 		  = 0;
			MC_MoveAbsolute_0.Execute = 0;
			MC_MoveAdditive_0.Execute = 0;
			MC_MoveVelocity_0.Execute = 0;
			MC_ReadAxisError_0.Acknowledge = 0;
			MC_Reset_0.Execute = 0;
	
			/* reset user commands */
			mcAxis.AxisCmd.Init = 0;
			mcAxis.AxisCmd.Stop = 0;
			mcAxis.AxisCmd.Home = 0;
			mcAxis.AxisCmd.MoveJogPos = 0;
			mcAxis.AxisCmd.MoveJogNeg = 0;
			mcAxis.AxisCmd.MoveVelocity = 0;
			mcAxis.AxisCmd.MoveAbsolute = 0;
			mcAxis.AxisCmd.MoveAdditive = 0;
	
			mcAxis.Status.ErrorID = 0;
			break;
			
		/******************** POWER ON **********************/
		case STEP_POWER_ON:  /* 7 *//* STATE: Power on */
			MC_Power_0.Enable = 1;
			if (MC_Power_0.Status == 1)
			{
			mcAxis.Step = STEP_READY;
			}
			/* if a power error occured go to error state */
			if (MC_Power_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_Power_0.ErrorID;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** READY **********************/
		case STEP_READY: /* 10 *//* STATE: Waiting for commands */
			if (mcAxis.AxisCmd.Init == 1)
			{
				mcAxis.AxisCmd.Init = 0;
				mcAxis.Step = STEP_WAIT;	
			}
			else if(mcAxis.AxisCmd.Power == 0)
			{
				mcAxis.Step = STEP_PAR_INIT_OK;	
			}	
			else if (mcAxis.AxisCmd.Home == 1)
			{
				mcAxis.AxisCmd.Home = 0;
				mcAxis.Step = STEP_HOME;
			}
			else if (mcAxis.AxisCmd.Stop == 1)
			{
				mcAxis.AxisCmd.Stop = 0;
				mcAxis.Step = STEP_STOP;
			}
			else if (mcAxis.AxisCmd.MoveJogPos == 1)
			{
				mcAxis.Step = STEP_JOG_POSITIVE;
			}
			else if (mcAxis.AxisCmd.MoveJogNeg == 1)
			{
				mcAxis.Step = STEP_JOG_NEGATIVE;
			}
			else if (mcAxis.AxisCmd.MoveAbsolute == 1)
			{
				mcAxis.AxisCmd.MoveAbsolute = 0;
				mcAxis.Step = STEP_MOVE_ABSOLUTE;
			}
			else if (mcAxis.AxisCmd.MoveAdditive == 1)
			{
				mcAxis.AxisCmd.MoveAdditive = 0;
				mcAxis.Step = STEP_MOVE_ADDITIVE;
			}
			else if (mcAxis.AxisCmd.MoveVelocity == 1)
			{
				mcAxis.AxisCmd.MoveVelocity = 0;
				mcAxis.Step = STEP_MOVE_VELOCITY;
			}
			break;
	
		/******************** HOME **********************/
		case STEP_HOME: /* 8 *//* STATE: start homing process */
			MC_Home_0.Position 		= mcAxis.Parameter.HomePosition;
			MC_Home_0.HomingMode 	= mcAxis.Parameter.HomeMode;
			MC_Home_0.Execute = 1;
			if (mcAxis.AxisCmd.Stop == 1)
			{
				mcAxis.AxisCmd.Stop = 0;
				MC_Home_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			else if (MC_Home_0.Done == 1)
			{
				MC_Home_0.Execute = 0;
				mcAxis.Step = STEP_READY;
			}
			/* if a homing error occured go to error state */
			if (MC_Home_0.ErrorID != 0)
			{
				MC_Home_0.Execute = 0;
				mcAxis.Status.ErrorID = MC_Home_0.ErrorID;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/***********************HALT_MOVEMENT***************************/
		case STEP_HALT:  /* 11 */ /* STATE: Halt movement */
			MC_Halt_0.Deceleration = mcAxis.Parameter.Deceleration;
			MC_Halt_0.Execute = 1;
			if(MC_Halt_0.Done == 1)
			{
				MC_Halt_0.Execute = 0;
				mcAxis.Step = STEP_READY;        
			}
			
			/* check if error occured */
			if(MC_Halt_0.Error == 1)
			{
				mcAxis.Status.ErrorID = MC_Halt_0.ErrorID;
				MC_Halt_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}                                        
			break;
		
		/*********************** STOP MOVEMENT *************************/
		case STEP_STOP: /* 12 *//* STATE: Stop movement */
			MC_Stop_0.Deceleration= mcAxis.Parameter.Deceleration;
			MC_Stop_0.Execute = 1;
			/* if axis is stopped go to state wait */
			if (MC_Stop_0.Done == 1)
			{
				MC_Stop_0.Execute = 0;
				mcAxis.Step = STEP_READY;
			}
			/* check if error occured */
			if (MC_Stop_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_Stop_0.ErrorID;
				MC_Stop_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** START JOG MOVEMENT POSITVE **********************/
		case STEP_JOG_POSITIVE: /* 13 *//* STATE: Start jog movement in positive direction */
			MC_MoveVelocity_0.Velocity		= mcAxis.Parameter.JogVelocity;
			MC_MoveVelocity_0.Acceleration	= mcAxis.Parameter.Acceleration;
			MC_MoveVelocity_0.Deceleration	= mcAxis.Parameter.Deceleration;
			MC_MoveVelocity_0.Direction		= mcPOSITIVE_DIR;
			MC_MoveVelocity_0.Execute = 1;
			/* check if jog movement stop */
			if (mcAxis.AxisCmd.MoveJogPos == 0)
			{
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			/* check if error occured */
			if (MC_MoveVelocity_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_MoveVelocity_0.ErrorID;
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** START JOG MOVEMENT NEGATIVE **********************/
		case STEP_JOG_NEGATIVE: /* 14 *//* STATE: Start jog movement in negative direction */
			MC_MoveVelocity_0.Velocity		= mcAxis.Parameter.JogVelocity;
			MC_MoveVelocity_0.Acceleration	= mcAxis.Parameter.Acceleration;
			MC_MoveVelocity_0.Deceleration	= mcAxis.Parameter.Deceleration;
			MC_MoveVelocity_0.Direction		= mcNEGATIVE_DIR;
			MC_MoveVelocity_0.Execute = 1;
			/* check if jog movement stop */
			if (mcAxis.AxisCmd.MoveJogNeg == 0)
			{
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			/* check if error occured */
			if (MC_MoveVelocity_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_MoveVelocity_0.ErrorID;
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** START ABSOLUTE MOVEMENT **********************/
		case STEP_MOVE_ABSOLUTE: /* 15 *//* STATE: Start absolute movement */
			MC_MoveAbsolute_0.Position		= mcAxis.Parameter.Position;
			MC_MoveAbsolute_0.Velocity		= mcAxis.Parameter.Velocity;
			MC_MoveAbsolute_0.Acceleration	= mcAxis.Parameter.Acceleration;
			MC_MoveAbsolute_0.Deceleration	= mcAxis.Parameter.Deceleration;
			MC_MoveAbsolute_0.Direction		= mcSHORTEST_WAY;
			MC_MoveAbsolute_0.Execute = 1;
			/* check if commanded position is reached */
			if (mcAxis.AxisCmd.Stop == 1)
			{
				mcAxis.AxisCmd.Stop = 0;
				MC_MoveAbsolute_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			else if (MC_MoveAbsolute_0.Done == 1)
			{
				MC_MoveAbsolute_0.Execute = 0;
				mcAxis.Step = STEP_READY;
			}
			/* check if error occured */
			if (MC_MoveAbsolute_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_MoveAbsolute_0.ErrorID;
				MC_MoveAbsolute_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** START ADDITIVE MOVEMENT **********************/
		case STEP_MOVE_ADDITIVE: /* 16 *//* STATE: Start additive movement */
			MC_MoveAdditive_0.Distance		= mcAxis.Parameter.Distance;
			MC_MoveAdditive_0.Velocity		= mcAxis.Parameter.Velocity;
			MC_MoveAdditive_0.Acceleration	= mcAxis.Parameter.Acceleration;
			MC_MoveAdditive_0.Deceleration	= mcAxis.Parameter.Deceleration;
			MC_MoveAdditive_0.Execute = 1;
			/* check if commanded distance is reached */
			if (mcAxis.AxisCmd.Stop == 1)
			{
				mcAxis.AxisCmd.Stop = 0;
				MC_MoveAdditive_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			else if (MC_MoveAdditive_0.Done == 1)
			{
				MC_MoveAdditive_0.Execute = 0;
				mcAxis.Step = STEP_READY;
			}
			/* check if error occured */
			if (MC_MoveAdditive_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_MoveAdditive_0.ErrorID;
				MC_MoveAdditive_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** START VELOCITY MOVEMENT **********************/
		case STEP_MOVE_VELOCITY: /* 17 *//* STATE: Start velocity movement */
			MC_MoveVelocity_0.Velocity		= mcAxis.Parameter.Velocity;
			MC_MoveVelocity_0.Acceleration	= mcAxis.Parameter.Acceleration;
			MC_MoveVelocity_0.Deceleration	= mcAxis.Parameter.Deceleration;
			MC_MoveVelocity_0.Direction		= mcAxis.Parameter.Direction;
			MC_MoveVelocity_0.Execute = 1;
			/* check if commanded velocity is reached */
			if (mcAxis.AxisCmd.Stop == 1)
			{
				mcAxis.AxisCmd.Stop = 0;
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_STOP;
			}
			else if (MC_MoveVelocity_0.InVelocity == 1)
			{
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_READY;
			}
			/* check if error occured */
			if (MC_MoveVelocity_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_MoveVelocity_0.ErrorID;
				MC_MoveVelocity_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;
			}
			break;
	
		/******************** FB-ERROR OCCURED *************************/
		case STEP_ERROR: /* 100 *//* STATE: Error */
			/* check if fb shows a readaxiserror */
			if(mcAxis.Status.ErrorID == 0)mcAxis.Status.ErrorID = MC_ReadAxisError_0.AxisErrorID;
			if (mcAxis.Status.ErrorID == 29226)
			{
				mcAxis.Step = STEP_ERROR_AXIS;
			}
			else
			{				
				if (mcAxis.AxisCmd.ErrorAcknowledge == 1)
				{
					mcAxis.AxisCmd.ErrorAcknowledge = 0;
					mcAxis.Status.ErrorID = 0;
					/* reset axis if in errorstop mode */
					if ((MC_ReadStatus_0.Errorstop == 1) && (MC_ReadStatus_0.Valid == 1))
					{
						mcAxis.Step = STEP_ERROR_RESET;
					}
					else
					{
						if (MC_ReadAxisError_0.AxisErrorID != 0)
						{
							MC_ReadAxisError_0.Acknowledge = 1;
						}
						mcAxis.Step = STEP_PAR_INIT_OK;
					}
				}
			}
			break;
	
		/******************** AXIS-ERROR OCCURED *************************/
		case STEP_ERROR_AXIS: /* 101 *//* STATE: Axis Error */
			if (MC_ReadAxisError_0.Valid == 1)
			{
				if (MC_ReadAxisError_0.AxisErrorID != 0)
				{
					mcAxis.Status.ErrorID = MC_ReadAxisError_0.AxisErrorID;
				}
				MC_ReadAxisError_0.Acknowledge = 0;
				if (mcAxis.AxisCmd.ErrorAcknowledge == 1)
				{
					mcAxis.AxisCmd.ErrorAcknowledge = 0;
					/* acknowledge axis error */
					if (MC_ReadAxisError_0.AxisErrorID != 0)
					{
						MC_ReadAxisError_0.Acknowledge = 1;
					}
				}
				if (MC_ReadAxisError_0.AxisErrorCount == 0)
				{
					mcAxis.Status.ErrorID = 0;
					/* reset axis if in errorstop mode */
					if ((MC_ReadStatus_0.Errorstop == 1) && (MC_ReadStatus_0.Valid == 1))
					{
						mcAxis.Step = STEP_ERROR_RESET;
					}
					else
					{
						mcAxis.Step = STEP_PAR_INIT_OK;
					}
				}
			}
			break;
	
		/******************** RESET DONE *************************/
		case STEP_ERROR_RESET: /* 102 *//* STATE: Wait for reset done */
			
			MC_Reset_0.Execute = 1;	/* reset MC_Power.Enable if this FB is in Error*/
				
			if(MC_Power_0.Error == 1)
			{
				MC_Power_0.Enable = 0;
			}
			if(MC_Reset_0.Done == 1)
			{
				MC_Reset_0.Execute = 0;
				mcAxis.Step = STEP_PAR_INIT_OK;
			}
			else if (MC_Reset_0.Error == 1)
			{
				MC_Reset_0.Execute = 0;
				mcAxis.Step = STEP_ERROR;   
			}
			
			
			break;
		/******************** SEQUENCE END *************************/
	}/* switch(mcAxis.Step) */
	
	/***************************************************************
	Function Block Calls
	***************************************************************/
	/************************** MC_POWER ****************************/
	MC_Power_0.Axis = mcAxis.AxisID.Address; /* pointer to axis */
	MC_Power(&MC_Power_0);
	
	/************************** MC_HOME *****************************/
	MC_Home_0.Axis = mcAxis.AxisID.Address;
	MC_Home(&MC_Home_0);
	
	/********************** MC_MOVEABSOLUTE *************************/
	MC_MoveAbsolute_0.Axis= mcAxis.AxisID.Address;
	MC_MoveAbsolute(&MC_MoveAbsolute_0);
	
	/********************** MC_MOVEADDITIVE *************************/
	MC_MoveAdditive_0.Axis= mcAxis.AxisID.Address;
	MC_MoveAdditive(&MC_MoveAdditive_0);
	
	/********************** MC_MOVEVELOCITY *************************/
	MC_MoveVelocity_0.Axis= mcAxis.AxisID.Address;
	MC_MoveVelocity(&MC_MoveVelocity_0);
	
	/************************** MC_STOP *****************************/
	MC_Stop_0.Axis = mcAxis.AxisID.Address;
	MC_Stop(&MC_Stop_0);
	
	/***************************MC_HALT******************************/
	MC_Halt_0.Axis = mcAxis.AxisID.Address;
	MC_Halt(&MC_Halt_0);

	/************************** MC_RESET ****************************/
	MC_Reset_0.Axis= mcAxis.AxisID.Address;
	MC_Reset(&MC_Reset_0);
	
	/************************ MC_READAXISERROR **********************/
	MC_ReadAxisError_0.Enable = ! MC_ReadAxisError_0.Error;
	MC_ReadAxisError_0.Axis = mcAxis.AxisID.Address;
	MC_ReadAxisError_0.DataAddress = (UDINT)&(mcAxis.Status.ErrorText);
	MC_ReadAxisError_0.DataLength = sizeof(mcAxis.Status.ErrorText);
	strcpy((void*)&MC_ReadAxisError_0.DataObjectName,"acp10etxen");
	MC_ReadAxisError(&MC_ReadAxisError_0);
	
	/************************ MC_READSTATUS *************************/
	MC_ReadStatus_0.Enable = ! MC_ReadStatus_0.Error;
	MC_ReadStatus_0.Axis = mcAxis.AxisID.Address;
	MC_ReadStatus(&MC_ReadStatus_0);
	if(MC_ReadStatus_0.Valid == 1)
	{
		mcAxis.AxisState.Disabled = MC_ReadStatus_0.Disabled;
		mcAxis.AxisState.Standstill = MC_ReadStatus_0.StandStill;
		mcAxis.AxisState.Stopping = MC_ReadStatus_0.Stopping;
		mcAxis.AxisState.Homing = MC_ReadStatus_0.Homing;
		mcAxis.AxisState.MotionDiscrete	= MC_ReadStatus_0.DiscreteMotion;
		mcAxis.AxisState.MotionContinous	= MC_ReadStatus_0.ContinuousMotion;
		mcAxis.AxisState.MotionSynchronized= MC_ReadStatus_0.SynchronizedMotion;
		mcAxis.AxisState.ErrorStop = MC_ReadStatus_0.Errorstop;
	}

	/************************ MC_BR_READDRIVERSTATUS *************************/
	MC_BR_ReadDriveStatus_0.Enable = ! MC_BR_ReadDriveStatus_0.Error;
	MC_BR_ReadDriveStatus_0.Axis   = mcAxis.AxisID.Address;
	MC_BR_ReadDriveStatus_0.AdrDriveStatus = (UDINT)&mcDriverStatus;
	MC_BR_ReadDriveStatus(&MC_BR_ReadDriveStatus_0);
	if(MC_BR_ReadDriveStatus_0.Valid == 1)
	{
		memcpy(&mcAxis.DriverStatus ,&mcDriverStatus,sizeof(mcDriverStatus));
	}

	/******************** MC_READACTUALVELOCITY *********************/
	MC_ReadActualVelocity_0.Enable = ! MC_ReadActualVelocity_0.Error;
	MC_ReadActualVelocity_0.Axis   = mcAxis.AxisID.Address;
	MC_ReadActualVelocity(&MC_ReadActualVelocity_0);
	if(MC_ReadActualVelocity_0.Valid == 1)
	{
		mcAxis.Status.ActVelocity = MC_ReadActualVelocity_0.Velocity;
		mcAxis.CycRead.v_act      = mcAxis.Status.ActVelocity;
	}
	
	/************************ read par ID step *************************/
	
	switch(StepReadID)
	{
		case STEP_READ_ID_WAIT: /*0*/
			if(mcAxis.Step == STEP_READY)
			{
				StepReadID = STEP_READ_ID_READY;
			}
			break;
		
		case STEP_READ_ID_READY:/*10*/
			mcParID_RW.idxRead = 0;
			StepReadID = STEP_READ_ID_RUN;
			break;	
	
		case STEP_READ_ID_RUN:/*100*/
			MC_BR_ReadParID_0.Execute = 1;
			MC_BR_ReadParID_0.ParID   = mcParID_RW.Read[mcParID_RW.idxRead].ParID;
			MC_BR_ReadParID_0.DataAddress =(UDINT)mcParID_RW.Read[mcParID_RW.idxRead].DataAddress;
			MC_BR_ReadParID_0.DataType    = mcParID_RW.Read[mcParID_RW.idxRead].DataType;
			
			if (1 == MC_BR_ReadParID_0.Done)
			{
				MC_BR_ReadParID_0.Execute=0;
				StepReadID = STEP_READ_ID_OK;
			}
			else if (MC_BR_ReadParID_0.Error)
			{ 
				mcAxis.Status.ErrorID = MC_BR_ReadParID_0.ErrorID;
				MC_BR_ReadParID_0.Execute=0;
				StepReadID = STEP_READ_ID_WAIT;
			}
			break;
	
		case STEP_READ_ID_OK:/*200*/
			mcParID_RW.idxRead ++;
			mcParID_RW.idxRead = mcParID_RW.idxRead % INDEX_PAR_READ_ID_MAX;
			StepReadID = STEP_READ_ID_RUN;
			break;
	}/* switch(StepReadID) */
	
	MC_BR_ReadParID_0.Axis = mcAxis.AxisID.Address;
	MC_BR_ReadParID(&MC_BR_ReadParID_0);

	/************************ write par ID step *************************/
	switch(StepWriteID)
	{
		case STEP_WRITE_ID_WAIT:
			if(mcAxis.Step == STEP_READY)
			{
				StepWriteID = STEP_WRITE_ID_READY;
			}
			break;
		
		case STEP_WRITE_ID_READY:
			mcParID_RW.idxWrite = 0;
			StepWriteID = STEP_WRITE_ID_SCAN;
			break;
		
		case STEP_WRITE_ID_SCAN:
			switch(mcParID_RW.Write[mcParID_RW.idxWrite].DataType)
			{
				case 8:	/* ncPAR_TYP_REAL */
					if((*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) != (*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)))
					{
						if( ((*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) > 0.0) ||  /* 9 and 7 Tn of PID */
							((*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) == 0.0 && ((9 == mcParID_RW.idxWrite) || (7 == mcParID_RW.idxWrite)))
							)
						{
							StepWriteID = STEP_WRITE_ID_RUN;
						}
					}
					(*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)) = (*((REAL *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress));
					break;
			
				case 5:	/* ncPAR_TYP_USINT */
					if((*((USINT *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) != (*((USINT *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)))
					{
						StepWriteID = STEP_WRITE_ID_RUN;
					}
					(*((USINT *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)) = (*((USINT *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress));			
					break;
			
				case 7:	/* ncPAR_TYP_UDINT */
					if((*((UDINT *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) != (*((UDINT *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)))
					{
						if((*((UDINT *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress)) > 0)
						{
							StepWriteID = STEP_WRITE_ID_RUN;
						}
					}
					(*((UDINT *)mcParID_RW.Write[mcParID_RW.idxWrite].pValue)) = (*((UDINT *)mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress));			
					break;
			}/* switch(mcParID_RW.Write[mcParID_RW.idxWrite].DataType) */
			
			if(StepWriteID == STEP_WRITE_ID_SCAN) /* value no change  */
			{
				StepWriteID = STEP_WRITE_ID_OK;
			}
			break;		
	
		case STEP_WRITE_ID_RUN:
			MC_BR_WriteParID_0.Execute = 1;
			MC_BR_WriteParID_0.ParID   = mcParID_RW.Write[mcParID_RW.idxWrite].ParID;
			MC_BR_WriteParID_0.DataAddress = mcParID_RW.Write[mcParID_RW.idxWrite].DataAddress;
			MC_BR_WriteParID_0.DataType    = mcParID_RW.Write[mcParID_RW.idxWrite].DataType;

			if (1 == MC_BR_WriteParID_0.Done)
			{
				MC_BR_WriteParID_0.Execute=0;
				StepWriteID = STEP_WRITE_ID_OK;
			}
			else if (MC_BR_WriteParID_0.Error)
			{  
				mcAxis.Status.ErrorID = MC_BR_WriteParID_0.ErrorID;
				MC_BR_WriteParID_0.Execute = 0;
				StepWriteID = STEP_WRITE_ID_WAIT;
			}
			break;
		
		case STEP_WRITE_ID_OK:
			mcParID_RW.idxWrite ++;	
			mcParID_RW.idxWrite = mcParID_RW.idxWrite % INDEX_PAR_WRITE_ID_MAX ;
			StepWriteID = STEP_WRITE_ID_SCAN;
			break;
	}/* switch(StepWriteID) */
	
	MC_BR_WriteParID_0.Axis = mcAxis.AxisID.Address;
	MC_BR_WriteParID(&MC_BR_WriteParID_0);

	/************************ act position *************************/
	MC_BR_CyclicRead_0.Enable = ! MC_BR_CyclicRead_0.Error;
	MC_BR_CyclicRead_0.ParID  = ACP10PAR_ENCOD1_S_ACT;
	MC_BR_CyclicRead_0.DataAddress= (UDINT)&mcAxis.CycRead.s_encoder;
	MC_BR_CyclicRead_0.DataType=ncPAR_TYP_DINT;

	MC_BR_CyclicRead_0.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicRead(&MC_BR_CyclicRead_0);  

	mcAxis.Status.ActPosition = mcAxis.CycRead.s_encoder * mcAxis.AxisID.UnitFactor;
	
	/************************ act troque *************************/
	MC_BR_CyclicRead_1.Enable = ! MC_BR_CyclicRead_1.Error;
	MC_BR_CyclicRead_1.ParID  = ACP10PAR_TORQUE_ACT;
	MC_BR_CyclicRead_1.DataAddress= (UDINT)&mcAxis.CycRead.torq_act;
	MC_BR_CyclicRead_1.DataType=ncPAR_TYP_REAL;

	MC_BR_CyclicRead_1.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicRead(&MC_BR_CyclicRead_1);  
	
	mcAxis.Status.ActTorq = mcAxis.CycRead.torq_act;
	
	/************************ act current *************************/
	MC_BR_CyclicRead_2.Enable = ! MC_BR_CyclicRead_2.Error;
	MC_BR_CyclicRead_2.ParID  = ACP10PAR_ICTRL_ISQ_ACT;
	MC_BR_CyclicRead_2.DataAddress= (UDINT)&ActCurrent;
	MC_BR_CyclicRead_2.DataType=ncPAR_TYP_REAL;

	MC_BR_CyclicRead_2.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicRead(&MC_BR_CyclicRead_2);  
	
	/************************ v_set *************************/
	MC_BR_CyclicWrite_0.Enable = ! MC_BR_CyclicWrite_0.Error;
	MC_BR_CyclicWrite_0.ParID = ACP10PAR_VAR_R4_0;
	MC_BR_CyclicWrite_0.DataAddress = (UDINT)&mcAxis.CycWrite.v_set;
	MC_BR_CyclicWrite_0.DataType = ncPAR_TYP_REAL;

	MC_BR_CyclicWrite_0.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_0); 
	
	/************************ torque_set *************************/
	MC_BR_CyclicWrite_1.Enable = ! MC_BR_CyclicWrite_1.Error;
	MC_BR_CyclicWrite_1.ParID = ACP10PAR_VAR_R4_1;
	MC_BR_CyclicWrite_1.DataAddress = (UDINT)&mcAxis.CycWrite.torq_set;
	MC_BR_CyclicWrite_1.DataType = ncPAR_TYP_REAL;

	MC_BR_CyclicWrite_1.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_1); 
	
	/************************ PosHWSwitch For ACOPOS Multi *************************/
	diPosHWSwitch = LMold.BlowPin.DnHWSwitch;
	
	MC_BR_CyclicWrite_2.Enable = ! MC_BR_CyclicWrite_2.Error;
	MC_BR_CyclicWrite_2.ParID = ACP10PAR_VAR_I2_0;
	MC_BR_CyclicWrite_2.DataAddress = (UDINT)&diPosHWSwitch;
	MC_BR_CyclicWrite_2.DataType = ncPAR_TYP_INT;

	MC_BR_CyclicWrite_2.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_2); 
	
	/************************ NegHWSwitch For ACOPOS Multi*************************/
	diNegHWSwitch = LMold.BlowPin.UpHWSwitch;
	
	MC_BR_CyclicWrite_3.Enable = ! MC_BR_CyclicWrite_3.Error;
	MC_BR_CyclicWrite_3.ParID = ACP10PAR_VAR_I2_1;
	MC_BR_CyclicWrite_3.DataAddress = (UDINT)&diNegHWSwitch;
	MC_BR_CyclicWrite_3.DataType = ncPAR_TYP_INT;

	MC_BR_CyclicWrite_3.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_3); 
	
	/* ------------------ Get axis current data ------------------ */	
	LMold.BlowPin.As    = mcAxis.CycRead.s_encoder;
	LMold.BlowPin.v_act = mcAxis.Status.ActVelocity * mcAxis.AxisID.UnitFactor;
	LMold.BlowPin.p_act = mcAxis.CycRead.torq_act;
	
	LMold.BlowPin.NetworkInit = gAxisBlowPinL.network.init;
	LMold.BlowPin.PowerOn = mcAxis.DriverStatus.ControllerStatus;
//	LMold.BlowPin.UpHWSwitch = mcAxis.DriverStatus.NegHWSwitch;
//	LMold.BlowPin.DnHWSwitch = mcAxis.DriverStatus.PosHWSwitch;
	
	/*2018.7.12 Albert*/
//	LMold.BlowPin.UpHWSwitch = mcAxis.DriverStatus.PosHWSwitch;
//	LMold.BlowPin.DnHWSwitch = mcAxis.DriverStatus.NegHWSwitch;
	
	LMold.BlowPin.AxisReady = mcAxis.AxisState.Standstill;
	LMold.BlowPin.Error    = mcAxis.AxisState.ErrorStop;
}

void _EXIT ProgramExit(void)
{
	// Insert code here 
	
}

/* ------------- configure function ------------- */
void ConfigureParID()
{	
	/* ------------------- configure mc par id init ------------------- */
	mcParID_RW.init[0].DataAddress = (UDINT) & mcParID_Value.encoderPar.count_dir;
	mcParID_RW.init[0].DataType    = ncPAR_TYP_USINT;
	mcParID_RW.init[0].ParID       = ACP10PAR_VCTRL_ENCOD_COUNT_DIR;	
	
	/* ------------- VCTRL_SCALE_LOAD_UNITS ---------------- */
	mcParID_RW.init[1].DataAddress = (UDINT) & mcParID_Value.encoderPar.load_units;
	mcParID_RW.init[1].DataType    = ncPAR_TYP_UDINT;
	mcParID_RW.init[1].ParID       = ACP10PAR_VCTRL_SCALE_LOAD_UNITS;	

	/* ------------- VCTRL_SCALE_LOAD_MOTREV ---------------- */
	mcParID_RW.init[2].DataAddress = (UDINT) & mcParID_Value.encoderPar.encoder_rev;
	mcParID_RW.init[2].DataType    = ncPAR_TYP_UDINT;
	mcParID_RW.init[2].ParID       = ACP10PAR_VCTRL_SCALE_LOAD_MOTREV;
		
	/* ------------- AXLIM_V_POS ---------------- */
	mcParID_Value.limitPar.v_pos = mcParID_Value.v_Max;
	mcParID_RW.Write[0].DataAddress = (UDINT) & mcParID_Value.limitPar.v_pos;
	mcParID_RW.Write[0].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[0].ParID       = ACP10PAR_AXLIM_V_POS;	
	
	/* ------------- AXLIM_V_NEG ---------------- */
	mcParID_Value.limitPar.v_neg = -mcParID_Value.v_Max;
	mcParID_RW.Write[1].DataAddress = (UDINT) & mcParID_Value.limitPar.v_neg;
	mcParID_RW.Write[1].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[1].ParID       = ACP10PAR_AXLIM_V_NEG;	

	/* ------------- AXLIM_A1_POS ---------------- */
	mcParID_RW.Write[2].DataAddress = (UDINT) & mcParID_Value.limitPar.a1_pos;
	mcParID_RW.Write[2].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[2].ParID       = ACP10PAR_AXLIM_A1_POS;	

	/* ------------- AXLIM_A1_NEG ---------------- */
	mcParID_RW.Write[3].DataAddress = (UDINT) & mcParID_Value.limitPar.a1_neg;
	mcParID_RW.Write[3].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[3].ParID       = ACP10PAR_AXLIM_A1_NEG;	

	/* ------------- AXLIM_A2_POS ---------------- */
	mcParID_RW.Write[4].DataAddress = (UDINT) & mcParID_Value.limitPar.a2_pos;
	mcParID_RW.Write[4].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[4].ParID       = ACP10PAR_AXLIM_A2_POS;	

	/* ------------- AXLIM_A2_NEG ---------------- */
	mcParID_RW.Write[5].DataAddress = (UDINT) & mcParID_Value.limitPar.a2_neg;
	mcParID_RW.Write[5].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[5].ParID       = ACP10PAR_AXLIM_A2_NEG;	
	
	/* ------------- POS_CTRL_KV ---------------- */
	mcParID_RW.Write[6].DataAddress = (UDINT) & mcParID_Value.pidPar.kv_p;
	mcParID_RW.Write[6].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[6].ParID       = ACP10PAR_POS_CTRL_KV;	

	/* ------------- POS_CTRL_TN ---------------- */
	mcParID_RW.Write[7].DataAddress = (UDINT) & mcParID_Value.pidPar.tn_p;
	mcParID_RW.Write[7].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[7].ParID       = ACP10PAR_POS_CTRL_TN;	
	
	/* ------------- SCTRL_KV ---------------- */
	mcParID_RW.Write[8].DataAddress = (UDINT) & mcParID_Value.pidPar.kv_s;
	mcParID_RW.Write[8].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[8].ParID       = ACP10PAR_SCTRL_KV;	
	
	/* ------------- SCTRL_TN ---------------- */
	mcParID_RW.Write[9].DataAddress = (UDINT) & mcParID_Value.pidPar.tn_s;
	mcParID_RW.Write[9].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[9].ParID       = ACP10PAR_SCTRL_TN;
	
	/* ------------- SCTRL_TI_FIL ---------------- */
	mcParID_RW.Write[10].DataAddress = (UDINT) & mcParID_Value.pidPar.tf_s;
	mcParID_RW.Write[10].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[10].ParID       = ACP10PAR_SCTRL_TI_FIL;
	
	/* ------------- VAR_R4_2+0 ---------------- */
	mcParID_RW.Write[11].DataAddress = (UDINT) & mcParID_Value.pidPar.kpJunction;
	mcParID_RW.Write[11].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[11].ParID       = ACP10PAR_VAR_R4_2+0;

	/* ------------- VAR_R4_3+0 ---------------- */
	mcParID_RW.Write[12].DataAddress = (UDINT) & mcParID_Value.pidPar.kpMotorTemp;
	mcParID_RW.Write[12].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[12].ParID       = ACP10PAR_VAR_R4_3+0;

	/* ------------- MOTOR_TORQ_RATED ---------------- */
	mcParID_RW.Read[0].DataAddress = (UDINT) & mcParID_Value.readPar.torqueRated;
	mcParID_RW.Read[0].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[0].ParID       = ACP10PAR_MOTOR_TORQ_RATED;
	
	/* ------------- MOTOR_CURR_RATED ---------------- */
	mcParID_RW.Read[1].DataAddress = (UDINT) & mcParID_Value.readPar.currentRated;
	mcParID_RW.Read[1].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[1].ParID       = ACP10PAR_MOTOR_CURR_RATED;
	
	/* ------------- MOTOR_COMMUT_OFFSET ---------------- */
	mcParID_RW.Read[2].DataAddress = (UDINT) & mcParID_Value.readPar.offset;
	mcParID_RW.Read[2].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[2].ParID       = ACP10PAR_MOTOR_COMMUT_OFFSET;
	
	/* ------------- UDC_ACT ---------------- */
	mcParID_RW.Read[3].DataAddress = (UDINT) & mcParID_Value.readPar.actUDC;
	mcParID_RW.Read[3].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[3].ParID       = ACP10PAR_UDC_ACT;
	
	/* ------------- ICTRL_ISQ_ACT ---------------- */
	mcParID_RW.Read[4].DataAddress = (UDINT) & mcParID_Value.readPar.actCurrent;
	mcParID_RW.Read[4].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[4].ParID       = ACP10PAR_ICTRL_ISQ_ACT;
	
	/* ------------- TEMP_MOTOR ---------------- */
	mcParID_RW.Read[5].DataAddress = (UDINT) & mcParID_Value.readPar.tempMotor;
	mcParID_RW.Read[5].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[5].ParID       = ACP10PAR_TEMP_MOTOR_MODELL;
	
	/* ------------- LIM_T1_POS ---------------- */
	mcParID_RW.Read[6].DataAddress = (UDINT) & mcParID_Value.readPar.actLimitTorque;
	mcParID_RW.Read[6].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[6].ParID       = ACP10PAR_LIM_T1_POS;
	
	/* ------------- TEMP_JUNCTION ---------------- */
	mcParID_RW.Read[7].DataAddress = (UDINT) & mcParID_Value.readPar.tempJunction;
	mcParID_RW.Read[7].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[7].ParID       = ACP10PAR_TEMP_HEATSINK;
}


