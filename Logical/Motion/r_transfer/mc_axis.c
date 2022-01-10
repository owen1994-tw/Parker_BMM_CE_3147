
#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

//#include "keyap880.h"
#include "keyap2100.h"
#include "edge.h"

#define	STEP_WAIT				0
#define	STEP_PAR_INIT        1
#define	STEP_PAR_INIT_W      2
#define	STEP_PAR_INIT_OK     3
	
#define	STEP_POWER_ON_READY  6
#define	STEP_POWER_ON			7
#define	STEP_AUTO_HOME			8
#define	STEP_HOME				9
	
#define	STEP_READY				10
#define	STEP_HALT				11
#define	STEP_STOP				12
#define	STEP_JOG_POSITIVE		13
#define	STEP_JOG_NEGATIVE		14
#define	STEP_MOVE_ABSOLUTE	15
#define	STEP_MOVE_ADDITIVE	16	
#define	STEP_MOVE_VELOCITY	17
	
#define	STEP_ERROR				100
#define	STEP_ERROR_AXIS		101
#define	STEP_ERROR_RESET		102
	
#define	STEP_WRITE_ID_WAIT		0
#define	STEP_WRITE_ID_READY     10
#define	STEP_WRITE_ID_SCAN      100
#define	STEP_WRITE_ID_RUN       110
#define	STEP_WRITE_ID_OK        200
	
#define INDEX_PAR_WRITE_ID_MAX  13
#define INDEX_PAR_INIT_ID_MAX   3
	
#define	STEP_READ_ID_WAIT		0
#define	STEP_READ_ID_READY      10
#define	STEP_READ_ID_RUN        100
#define	STEP_READ_ID_OK         200
	
#define INDEX_PAR_READ_ID_MAX  	8

/* function declare  */
void ConfigureParID();

void _INIT ProgramInit(void)
{
	mcAxis.AxisID.Address = (UDINT)&(gAxisTransferR); 	
	strcpy(mcAxis.AxisID.Name,"R_Robot transfer");
	mcAxis.AxisID.UnitFactor = 0.01;
	strcpy(mcAxis.AxisID.UnitName, "mm");
	mcAxis.AxisID.AxisType = ncAXIS;

	mcAxis.Parameter.Velocity 	        = 1000; /*velocity for movement*/
	mcAxis.Parameter.Acceleration 		= 4000; /*acceleration for movement*/
	mcAxis.Parameter.Deceleration 		= 4000; /*deceleration for movement*/
	mcAxis.Parameter.JogVelocity		= 400; 	/*velocity for jogging */
	mcAxis.Parameter.OverrideVelocity	= 1.0;
	mcAxis.Parameter.OverrideAcceleration=1.0;

	ConfigureParID();
	
	mcParID_Value.encoderPar.count_dir = ncSTANDARD;
	
	FixTransferR.pidPar.kv_p = 400;   /*  400    */
	FixTransferR.pidPar.kv_s = 1.05;  /*  1.05   */
	FixTransferR.pidPar.tn_s = 0;
	
	FixTransferR.pidPar.kpJunction  = 0.142;  //Kp = (9.2-2.1)/50 = 0.142
	FixTransferR.pidPar.kpMotorTemp = 0.142;  //Kp = (9.2-2.1)/50 = 0.142
	
	//	馬達 3000rpm = 3000rev/min = 50 rev/ s
	//	電動缸 (100 unit/mm) 
	//	旋轉/直線 25 mm / rev  = 2500 unit / rev
	//	V Max 1.5m/s = 1500mm/s = 60 rev/s = 3600 rev/min = 3600 rpm
	//	A Max 15 m/s2  = 15000 mm/s2 = 600 rev/s2
	//
	//	取規格小者 馬達V max = 50 rev/s
	//	25mm/rev 
	//	=> (100 unit  = 1 mm)
	//	=> 25*100 units /rev 
	//	=> 2500 units /rev
	//	X = 50 rev/s * (2500 units/rev) * 0.01( mm/units) * 0.001(m / mm) = 1.25m/s
	//
	//	V Max = 50 rev/s * 2500 (units/ rev) =125,000units/s
	//	A Max = 600 rev/s2 * 2500 (units/ rev) = 1,500,000 units/s2


	FixTransferR.limitPar.a1_pos = 500000.0;
	FixTransferR.limitPar.a2_pos = 500000.0;
	FixTransferR.limitPar.a1_neg = 500000.0;
	FixTransferR.limitPar.a2_neg = 500000.0;  //1500000.0

	FixTransferR.limitPar.v_pos = 125000.0;		
	FixTransferR.limitPar.v_neg = 125000.0;		
}

void _CYCLIC ProgramCyclic(void)
{
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.RobotTransfer.RPM 	= mcAxis.Status.ActVelocity/2500*60;
	gMotorTrace.RMold.RobotTransfer.Current	= ActCurrent/1.414;;
	gMotorTrace.RMold.RobotTransfer.Torque 	= mcAxis.CycRead.torq_act; 
	
	
	memcpy(&mcParID_Value.pidPar,&FixTransferR.pidPar,sizeof(FixTransferR.pidPar));
	memcpy(&mcParID_Value.limitPar,&FixTransferR.limitPar,sizeof(FixTransferR.limitPar));
	
	mcAxis.AxisCmd.Power = RMold.Robot.Transfer.BtnPowerOn;
	
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET],OldAlarmConfirmOK))
		{
		mcAxis.AxisCmd.ErrorAcknowledge = 1;
	}
	
	/***************************************************************/
	/* ------------------ Get setting parameter ------------------ */
	/***************************************************************/
	mcAxis.Parameter.HomePosition = 0.0;	/* homing offset 				*/
	mcAxis.Parameter.HomeMode = mcHOME_ABSOLUTE; /* ncHOME_OFFSET -- nc action	*/
	
	gMachineFix.MoldR.RobotTransfer.accel = FixTransferR.limitPar.a1_pos ;
	gMachineFix.MoldR.RobotTransfer.decel = FixTransferR.limitPar.a1_pos ;

	mcAxis.Parameter.Acceleration = gMachineFix.MoldR.RobotTransfer.accel;	/* acceleration for movement	*/
	mcAxis.Parameter.Deceleration = gMachineFix.MoldR.RobotTransfer.decel;	/* deceleration for movement	*/
	
	mcAxis.Parameter.JogVelocity  = RMold.Robot.Transfer.v_set * FixTransferR.limitPar.v_pos /100.0;	/* velocity for homing movement	*/
	mcAxis.Parameter.Velocity = RMold.Robot.Transfer.v_set * FixTransferR.limitPar.v_pos /100.0;	/* velocity for normal movement */
	
	if(1 == EDGEPOS(RMold.Robot.Transfer.Stop,cmdStopPosOld))
	{
		RMold.Robot.Transfer.Stop = 0;
		mcAxis.AxisCmd.Stop = 1;
	}
	
	if(1 == EDGEPOS(RMold.Robot.Transfer.MoveAbsolute,cmdMoveAbsolutePosOld)&&(gMachineFix.MoldR.RobotTransfer.sHome != 0))
	{
		RMold.Robot.Transfer.MoveAbsolute = 0;
		mcAxis.AxisCmd.MoveAbsolute = 1;
		mcAxis.Parameter.Position = RMold.Robot.Transfer.s_set * 100.0 + gMachineFix.MoldR.RobotTransfer.sHome;
	}
	
	if(mcAxis.Parameter.Position < gMachineFix.MoldR.RobotTransfer.sHome) 
	{
		mcAxis.Parameter.Position = gMachineFix.MoldR.RobotTransfer.sHome;
	}
	if(mcAxis.Parameter.Position > gMachineFix.MoldR.RobotTransfer.sHome + gMachineFix.MoldR.RobotTransfer.sMax * 100)
	{
		mcAxis.Parameter.Position = gMachineFix.MoldR.RobotTransfer.sHome + gMachineFix.MoldR.RobotTransfer.sMax * 100;
	}

	mcAxis.AxisCmd.MoveJogPos = RMold.Robot.Transfer.MoveJogPos;
	mcAxis.AxisCmd.MoveJogNeg = RMold.Robot.Transfer.MoveJogNeg;
	
	/* ------------------ error Acknowledge ------------------ */
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ALARM_RESET],OldAlarmConfirmOK))
		{
		mcAxis.AxisCmd.ErrorAcknowledge = 1;
	}

	if(1 == RMold.Robot.Transfer.InitPower)
	{
		RMold.Robot.Transfer.InitPower = 0;
		mcAxis.AxisCmd.ErrorAcknowledge = 1;
	}
	
	if(1 == EDGENEG(gAxisTransferR.network.init,NegNetworkInit))
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
		case STEP_WAIT:
			if(gAxisTransferR.network.init == ncTRUE)
			{
				mcAxis.Step = STEP_PAR_INIT_OK;
			}
			break;
		
		/******************** INIT PARAMETER *************************/
		case STEP_PAR_INIT:
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
		case STEP_PAR_INIT_W:
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
		case STEP_PAR_INIT_OK:
			MC_Power_0.Enable = 0;
			if(1 == RMold.Robot.Transfer.BtnPowerOn)
			{
				mcAxis.AxisCmd.Power = 1;
				mcAxis.Step = STEP_POWER_ON_READY;
			}
			break;
		
		/******************** POWER ON READY *************************/
		case STEP_POWER_ON_READY: /* STATE: POWER_ON READY */
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
		case STEP_POWER_ON: /* STATE: Power on */
			MC_Power_0.Enable = 1;
			if (MC_Power_0.Status == 1)
			{
			mcAxis.Step = STEP_AUTO_HOME;
			}
			/* if a power error occured go to error state */
			if (MC_Power_0.ErrorID != 0)
			{
				mcAxis.Status.ErrorID = MC_Power_0.ErrorID;
				mcAxis.Step = STEP_ERROR;
			}
			break;
		
		/******************** AUTO HOMING **********************/	
		case STEP_AUTO_HOME:
			if(1 == mcAxis.AxisState.Standstill)
			{
				mcAxis.AxisCmd.Stop = 0;
				mcAxis.AxisCmd.Home = 1;
				mcAxis.Step = STEP_READY;
			}
			break;
	
		/******************** READY **********************/
		case STEP_READY: /* STATE: Waiting for commands */
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
		case STEP_HOME: /* STATE: start homing process */
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
		case STEP_HALT:   /* STATE: Halt movement */
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
		case STEP_STOP: /* STATE: Stop movement */
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
		case STEP_JOG_POSITIVE: /* STATE: Start jog movement in positive direction */
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
		case STEP_JOG_NEGATIVE: /* STATE: Start jog movement in negative direction */
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
		case STEP_MOVE_ABSOLUTE: /* STATE: Start absolute movement */
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
		case STEP_MOVE_ADDITIVE: /* STATE: Start additive movement */
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
		case STEP_MOVE_VELOCITY: /* STATE: Start velocity movement */
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
		case STEP_ERROR: /* STATE: Error */
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
		case STEP_ERROR_AXIS: /* STATE: Axis Error */
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
		case STEP_ERROR_RESET: /* STATE: Wait for reset done */
			
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

	/************************ read par ID step *************************/
	switch(StepReadID)
	{
		case STEP_READ_ID_WAIT:
			if(mcAxis.Step == STEP_READY)
			{
				StepReadID = STEP_READ_ID_READY;
			}
			break;
		
		case STEP_READ_ID_READY:
			mcParID_RW.idxRead = 0;
			StepReadID = STEP_READ_ID_RUN;
			break;	
	
		case STEP_READ_ID_RUN:
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
	
		case STEP_READ_ID_OK:
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
	
	/************************ act velocity *************************/
	MC_BR_CyclicRead_2.Enable = ! MC_BR_CyclicRead_2.Error;
	MC_BR_CyclicRead_2.ParID  = ACP10PAR_PCTRL_V_ACT;
	MC_BR_CyclicRead_2.DataAddress= (UDINT)&mcAxis.CycRead.v_act;
	MC_BR_CyclicRead_2.DataType=ncPAR_TYP_REAL;

	MC_BR_CyclicRead_2.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicRead(&MC_BR_CyclicRead_2);  
	
	/************************ act current *************************/
	MC_BR_CyclicRead_3.Enable = ! MC_BR_CyclicRead_3.Error;
	MC_BR_CyclicRead_3.ParID  = ACP10PAR_ICTRL_ISQ_ACT;
	MC_BR_CyclicRead_3.DataAddress= (UDINT)&ActCurrent;
	MC_BR_CyclicRead_3.DataType=ncPAR_TYP_REAL;

	MC_BR_CyclicRead_3.Axis = mcAxis.AxisID.Address;
	MC_BR_CyclicRead(&MC_BR_CyclicRead_3);  
	
//	/************************ PosHWSwitch For ACOPOS Multi *************************/
	diPosHWSwitch = RMold.Robot.Transfer.FwHWSwitch;
//	
//	MC_BR_CyclicWrite_0.Enable = ! MC_BR_CyclicWrite_0.Error;
//	MC_BR_CyclicWrite_0.ParID = ACP10PAR_VAR_I2_0;
//	MC_BR_CyclicWrite_0.DataAddress = (UDINT)&diPosHWSwitch;
//	MC_BR_CyclicWrite_0.DataType = ncPAR_TYP_INT;
//
//	MC_BR_CyclicWrite_0.Axis = mcAxis.AxisID.Address;
//	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_0); 
//	
//	/************************ NegHWSwitch For ACOPOS Multi*************************/
	diNegHWSwitch =  RMold.Robot.Transfer.BwHWSwitch;
//	
//	MC_BR_CyclicWrite_1.Enable = ! MC_BR_CyclicWrite_1.Error;
//	MC_BR_CyclicWrite_1.ParID = ACP10PAR_VAR_I2_1;
//	MC_BR_CyclicWrite_1.DataAddress = (UDINT)&diNegHWSwitch;
//	MC_BR_CyclicWrite_1.DataType = ncPAR_TYP_INT;
//
//	MC_BR_CyclicWrite_1.Axis = mcAxis.AxisID.Address;
//	MC_BR_CyclicWrite(&MC_BR_CyclicWrite_1); 
	
	
	mcAxis.Status.ActVelocity = mcAxis.CycRead.v_act;

	/***************************************************************/
	/* ------------------ Get axis current data ------------------ */
	/***************************************************************/
	RMold.Robot.Transfer.ecAs    = mcAxis.CycRead.s_encoder;
	RMold.Robot.Transfer.mmAs    = (mcAxis.CycRead.s_encoder - gMachineFix.MoldR.RobotTransfer.sHome) * mcAxis.AxisID.UnitFactor;
	RMold.Robot.Transfer.v_act = mcAxis.CycRead.v_act * mcAxis.AxisID.UnitFactor;
	RMold.Robot.Transfer.p_act = mcAxis.CycRead.torq_act;
	
	RMold.Robot.Transfer.NetworkInit = gAxisTransferR.network.init;
	RMold.Robot.Transfer.PowerOn = mcAxis.DriverStatus.ControllerStatus;
	//	RMold.Robot.Transfer.FwHWSwitch = mcAxis.DriverStatus.PosHWSwitch;
	//	RMold.Robot.Transfer.BwHWSwitch = mcAxis.DriverStatus.NegHWSwitch;
	RMold.Robot.Transfer.AxisReady = mcAxis.AxisState.Standstill;
	RMold.Robot.Transfer.HomingOk = mcAxis.DriverStatus.HomingOk;
	

	//	if(1 == RMold.Robot.Transfer.BtnPowerOn)
	//	{
	//		RMold.Robot.Transfer.Error = mcAxis.AxisState.ErrorStop | mcAxis.AxisState.Disabled | (STEP_ERROR == mcAxis.Step);
	//	}
	//	else
	//	{
	//		RMold.Robot.Transfer.Error = 0;	
	//	}
}

void _EXIT ProgramExit(void)
{
	// Insert code here 
	
}

/* ------------- configure function ------------- */
void ConfigureParID()
{	
	/* -------------------- configure mc par id init -------------------- */	
	/* ------------- VCTRL_ENCOD_COUNT_DIR ---------------- */
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
		
	/* -------------------- configure mc par id write -------------------- */	
	/* ------------- AXLIM_V_POS ---------------- */
	mcParID_RW.Write[0].DataAddress = (UDINT) & mcParID_Value.limitPar.v_pos;
	mcParID_RW.Write[0].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Write[0].ParID       = ACP10PAR_AXLIM_V_POS;	
	
	/* ------------- AXLIM_V_NEG ---------------- */
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

	/* -------------------- configure mc par id read -------------------- */
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
	mcParID_RW.Read[5].ParID       = ACP10PAR_TEMP_MOTOR;

	/* ------------- TEMP_JUNCTION ---------------- */
	mcParID_RW.Read[6].DataAddress = (UDINT) & mcParID_Value.readPar.tempJunction;
	mcParID_RW.Read[6].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[6].ParID       = ACP10PAR_TEMP_JUNCTION;
	
	/* ------------- MOTOR_TORQ_MAX ---------------- */
	mcParID_RW.Read[7].DataAddress = (UDINT) & mcParID_Value.readPar.torqueMax;
	mcParID_RW.Read[7].DataType    = ncPAR_TYP_REAL;
	mcParID_RW.Read[7].ParID       = ACP10PAR_MOTOR_TORQ_MAX;

}/* void ConfigureParID() */