/***************************************************************************************
 * COPYRIGHT --  B&R China
 ***************************************************************************************
 * Program: carriage
 * File: carriage.c
 * Author: lijianjun
 * Created: March 06, 2015
 **************************************************************************************
 * Implementation of program carriage --- forward and backward 
 *************************************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif
#include <string.h>
/**************************************************************************************
** 					 	    define								                               **
**************************************************************************************/
#define   INSTANCY_STOP_DECEL       3000
#define   INSTANCY_STOP_ACCEL       3000


#define  ABS(X)   (((X) > 0.0) ? (X) : (-1.0*(X))) 

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void PropVlaveInit(Carriage_Fix_typ * pCarriageFix);
void TransportLVDT(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser);
void TransportSwitch(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser);
void TransportDriveToggle_Drive(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser);
void ButtonOffDelay(Mold_typ * pMold , timer_typ * pTimer);
REAL xTogLinearRTrans(REAL pCarriagePos);
REAL xTogLinearLTrans(REAL pCarriagePos);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */

	LMold.Carriage.Step = 30000;
	RMold.Carriage.Step = 30000;

	LMold.Carriage.Enable = 1;
	RMold.Carriage.Enable = 1;

	RMold.Carriage.vProfGen.Ts    = tTask;
	RMold.Carriage.vProfGen.accel = 800.0;
	RMold.Carriage.vProfGen.decel = 600.0;
	RMold.Carriage.vRamp.Ts       = tTask;
	RMold.Carriage.vRamp.Accel    = 1000;

	LMold.Carriage.vProfGen.Ts    = tTask;
	LMold.Carriage.vProfGen.accel = 800.0;
	LMold.Carriage.vProfGen.decel = 600.0;
	LMold.Carriage.vRamp.Ts       = tTask;
	LMold.Carriage.vRamp.Accel    = 1000;

	/* differentiator for evaluating the actual speed */
	vActHydDT2_L.Ts = tTask;	/* cycle time tTasks 			  */
	vActHydDT2_L.Tv = 1.0; 	/* derivative time constant [s]   */
	vActHydDT2_L.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_L.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */

	/* differentiator for evaluating the actual speed */
	vActHydDT2_R.Ts = tTask;	/* cycle time tTasks 			  */
	vActHydDT2_R.Tv = 1.0; 	/* derivative time constant [s]   */
	vActHydDT2_R.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_R.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */
   
	
	if( 20 > gMachineFix.MoldR.Carriage.ToggleLinNodeNum ) gMachineFix.MoldR.Carriage.ToggleLinNodeNum = 20;
	ToggleLinNodeNum = gMachineFix.MoldR.Carriage.ToggleLinNodeNum;
	/* Carriage toggle linear */
	TogLinearL.enable = 1;
	TogLinearL.nNodes = ToggleLinNodeNum;
	TogLinearL.pNodes = &gMachineFix.MoldL.Carriage.ToggleLin[0];

	TogLinearR.enable = 1;
	TogLinearR.nNodes = ToggleLinNodeNum;
	TogLinearR.pNodes = &gMachineFix.MoldR.Carriage.ToggleLin[0];
	
	xTogLinearL.enable = 1;
	xTogLinearL.nNodes = ToggleLinNodeNum;
	xTogLinearL.pNodes = &xToggleNodeL[0];

	xTogLinearR.enable = 1;
	xTogLinearR.nNodes = ToggleLinNodeNum;
	xTogLinearR.pNodes = &xToggleNodeR[0];
	
	memcpy(ToggleLOld,gMachineFix.MoldL.Carriage.ToggleLin,sizeof(gMachineFix.MoldL.Carriage.ToggleLin));
	memcpy(ToggleROld,gMachineFix.MoldR.Carriage.ToggleLin,sizeof(gMachineFix.MoldR.Carriage.ToggleLin));
	
	
	/* Left carriage proportional valve linear */
	if(0.0 == gMachineFix.MoldL.Carriage.ValveLin[0].X)
	{
		gMachineFix.MoldL.Carriage.ValveLin[0].X = -10.0; gMachineFix.MoldL.Carriage.ValveLin[0].Y = -1000.0;  /* v --- mm/s  */
		gMachineFix.MoldL.Carriage.ValveLin[1].X = -0.90; gMachineFix.MoldL.Carriage.ValveLin[1].Y = -50.0;
		gMachineFix.MoldL.Carriage.ValveLin[2].X = -0.72; gMachineFix.MoldL.Carriage.ValveLin[2].Y = -20.0;
		gMachineFix.MoldL.Carriage.ValveLin[3].X = -0.4; gMachineFix.MoldL.Carriage.ValveLin[3].Y = -0.5;
		gMachineFix.MoldL.Carriage.ValveLin[4].X =   0.0; gMachineFix.MoldL.Carriage.ValveLin[4].Y =  0.0;
		gMachineFix.MoldL.Carriage.ValveLin[5].X =  0.34; gMachineFix.MoldL.Carriage.ValveLin[5].Y =  0.5;
		gMachineFix.MoldL.Carriage.ValveLin[6].X =  0.62; gMachineFix.MoldL.Carriage.ValveLin[6].Y =  20.0;
		gMachineFix.MoldL.Carriage.ValveLin[7].X =  0.78; gMachineFix.MoldL.Carriage.ValveLin[7].Y =  50.0;
		gMachineFix.MoldL.Carriage.ValveLin[8].X =  10.0; gMachineFix.MoldL.Carriage.ValveLin[8].Y =  1000.0;
	}

	LMold.Carriage.ValveLinear.nNodes = 9;
	LMold.Carriage.ValveLinear.pNodes = &gMachineFix.MoldL.Carriage.ValveLin[0];
	LMold.Carriage.ValveLinear.update = 1;

	/* Right carriage proportional valve linear */
	if(0.0 == gMachineFix.MoldR.Carriage.ValveLin[0].X)
	{
		gMachineFix.MoldR.Carriage.ValveLin[0].X = -10.0; gMachineFix.MoldR.Carriage.ValveLin[0].Y = -1000.0;   /* v --- mm/s  */
		gMachineFix.MoldR.Carriage.ValveLin[1].X = -0.90; gMachineFix.MoldR.Carriage.ValveLin[1].Y = -50.0;
		gMachineFix.MoldR.Carriage.ValveLin[2].X = -0.72; gMachineFix.MoldR.Carriage.ValveLin[2].Y = -20.0;
		gMachineFix.MoldR.Carriage.ValveLin[3].X = -0.40; gMachineFix.MoldR.Carriage.ValveLin[3].Y = -0.5;
		gMachineFix.MoldR.Carriage.ValveLin[4].X =   0.0; gMachineFix.MoldR.Carriage.ValveLin[4].Y =  0.0;
		gMachineFix.MoldR.Carriage.ValveLin[5].X =  0.34; gMachineFix.MoldR.Carriage.ValveLin[5].Y =  0.5;
		gMachineFix.MoldR.Carriage.ValveLin[6].X =  0.62; gMachineFix.MoldR.Carriage.ValveLin[6].Y =  20.0;
		gMachineFix.MoldR.Carriage.ValveLin[7].X =  0.78; gMachineFix.MoldR.Carriage.ValveLin[7].Y =  50.0;
		gMachineFix.MoldR.Carriage.ValveLin[8].X =  10.0; gMachineFix.MoldR.Carriage.ValveLin[8].Y =  1000.0;
	}
   
	RMold.Carriage.ValveLinear.nNodes = 9;
	RMold.Carriage.ValveLinear.pNodes = &gMachineFix.MoldR.Carriage.ValveLin[0];
	RMold.Carriage.ValveLinear.update = 1;
   
	rMin0 = 0.0;
	
	
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	
	/*Servo On Function*/
	
	//	if(1 == gMachineOut.MotionPower)
	//	{
	//		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
	//		
	//		if(tErrorDelay < tTask)
	//		{
	//			LMold.HwAlarm.CarrNetworkErr = !LMold.Carriage.NetworkInit;
	//			LMold.HwAlarm.CarrPowerOn    = !LMold.Carriage.PowerOn;
	//		
	//			RMold.HwAlarm.CarrNetworkErr = !RMold.Carriage.NetworkInit;
	//			RMold.HwAlarm.CarrPowerOn    = !RMold.Carriage.PowerOn;
	//		}
	//	}
	//	else
	//	{
	//		tErrorDelay = 0.1;
	//		
	//		LMold.HwAlarm.CarrNetworkErr = 0;
	//		LMold.HwAlarm.CarrPowerOn    = 0;
	//		
	//		RMold.HwAlarm.CarrNetworkErr = 0;
	//		RMold.HwAlarm.CarrPowerOn    = 0;
	//
	//	}
	

	
	/*--- 機構線性，強制線性化 ----*/
	if( ACTUATOR_ELECTRIC ==  gMachineFix.MoldR.Carriage.eActuatorType)
	{
		int ToggleLinIndex;
		if( MECHANISM_LINEAR ==  gMachineFix.MoldR.Carriage.eMechanismType )
		{
			bToggle_Init = 1;
			for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM - 1); ToggleLinIndex++)
			{
				int iSumR = 0;
				int iSumL = 0;
				iSumR = gMachineFix.MoldR.Carriage.Length / (ToggleLinNodeNum - 1);
				gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].X = iSumR * ToggleLinIndex ;
				gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].Y = iSumR * ToggleLinIndex ;
			
				iSumL = gMachineFix.MoldL.Carriage.Length / (ToggleLinNodeNum - 1);
				gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].X = iSumL * ToggleLinIndex ;
				gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].Y = iSumL * ToggleLinIndex ;
				
				if( ToggleLinIndex >= ToggleLinNodeNum)
				{
					gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldR.Carriage.Length ;
					gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldR.Carriage.Length ;
					
					gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldL.Carriage.Length ;
					gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldL.Carriage.Length ;
				}
	
			}
		}
		else if( MECHANISM_TOGGLE ==  gMachineFix.MoldR.Carriage.eMechanismType  )
		{
			if(1 == bToggle_Init)
			{
				bToggle_Init = 0;
				
				// clear ToggleLin table
				for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM - 1); ToggleLinIndex++)
				{
					
					if(ToggleLinIndex <ToggleLinNodeNum )
					{
						gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].X = 0 ;
						gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].Y = 0 ;
					
						gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].X = 0 ;
						gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].Y = 0 ;
					}
					else
					{
						gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldR.Carriage.Length ;
						gMachineFix.MoldR.Carriage.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldR.Carriage.Length ;
					
						gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].X = gMachineFix.MoldL.Carriage.Length ;
						gMachineFix.MoldL.Carriage.ToggleLin[ToggleLinIndex].Y = gMachineFix.MoldL.Carriage.Length ;
					
					}
				}	
			
			}

			
			
		}

	}


	
	int i;
	for(i=0;i<MAX_TOGGLE_LIN_NUM;i++)
	{	
		xToggleNodeL[i].X = gMachineFix.MoldL.Carriage.ToggleLin[i].Y;
		xToggleNodeL[i].Y = gMachineFix.MoldL.Carriage.ToggleLin[i].X;
		
		xToggleNodeR[i].X = gMachineFix.MoldR.Carriage.ToggleLin[i].Y;
		xToggleNodeR[i].Y = gMachineFix.MoldR.Carriage.ToggleLin[i].X;
	}
	
	if( 20 > gMachineFix.MoldR.Carriage.ToggleLinNodeNum ) gMachineFix.MoldR.Carriage.ToggleLinNodeNum = 20;
	else if( 30 < gMachineFix.MoldR.Carriage.ToggleLinNodeNum ) gMachineFix.MoldR.Carriage.ToggleLinNodeNum = 30;
	ToggleLinNodeNum = gMachineFix.MoldR.Carriage.ToggleLinNodeNum;
	//TogLinear  update
	if( (memcmp(ToggleLOld,gMachineFix.MoldL.Carriage.ToggleLin,sizeof(gMachineFix.MoldL.Carriage.ToggleLin)) != 0 )
		|| oldToggleLinNodeNum != ToggleLinNodeNum)
	{
		TogLinearL.update = 1;
		TogLinearL.nNodes = ToggleLinNodeNum;
		memcpy(ToggleLOld,gMachineFix.MoldL.Carriage.ToggleLin,sizeof(gMachineFix.MoldL.Carriage.ToggleLin));
		
		xTogLinearL.update = 1;
		xTogLinearL.nNodes = ToggleLinNodeNum;
		HydLinear(&xTogLinearL);
	}
	
	
	if(memcmp(ToggleROld,gMachineFix.MoldR.Carriage.ToggleLin,sizeof(gMachineFix.MoldR.Carriage.ToggleLin)) != 0 
		|| oldToggleLinNodeNum != ToggleLinNodeNum)
	{
		TogLinearR.update = 1;
		TogLinearR.nNodes = ToggleLinNodeNum;
		memcpy(ToggleROld,gMachineFix.MoldR.Carriage.ToggleLin,sizeof(gMachineFix.MoldR.Carriage.ToggleLin));
		
		xTogLinearR.update = 1;
		xTogLinearR.nNodes = ToggleLinNodeNum;
		HydLinear(&xTogLinearR);
	}
	
	oldToggleLinNodeNum = ToggleLinNodeNum;
	
	
	/*  carriage Fw & Bw limit   */
	LMold.TransDIn.CarriageFwLimit = gMachineIn.LMold.CarriageFwLimit;
	LMold.TransDIn.CarriageBwLimit = gMachineIn.LMold.CarriageBwLimit;
	
	LMold.TransDIn.CarriageSlowFwLimit = gMachineIn.LMold.CarriageSlowFwLimit;
	LMold.TransDIn.CarriageSlowBwLimit = gMachineIn.LMold.CarriageSlowBwLimit;
   
	RMold.TransDIn.CarriageFwLimit = gMachineIn.RMold.CarriageFwLimit;
	RMold.TransDIn.CarriageBwLimit = gMachineIn.RMold.CarriageBwLimit;
	
	RMold.TransDIn.CarriageSlowFwLimit = gMachineIn.RMold.CarriageSlowFwLimit;
	RMold.TransDIn.CarriageSlowBwLimit = gMachineIn.RMold.CarriageSlowBwLimit;
		
	
	LMold.Carriage.bForwardLockUp = gMacOption.bForwardLockUp;		/*  left carriage forward keeping  */
	RMold.Carriage.bForwardLockUp = gMacOption.bForwardLockUp;		/*  right carriage forward keeping  */
    
	gMachineFix.MoldL.Carriage.Tolerance = gMachineFix.MoldR.Carriage.Tolerance;   /* only setting right tolerance */
	
	rMinCarriageBwL = gMachineFix.MoldL.Carriage.Length - 0.5;
	rMaxCarriageBwL = gMachineFix.MoldL.Carriage.Length + 0.5;
	rMinCarriageBwR = gMachineFix.MoldR.Carriage.Length - 0.5;
	rMaxCarriageBwR = gMachineFix.MoldR.Carriage.Length + 0.5;
   
	
	rCarriageFwL_HMI =2;
	rCarriageBwL_HMI =gMachineFix.MoldL.Carriage.Length-2;	
	rCarriageFwR_HMI =2;
	rCarriageBwR_HMI =gMachineFix.MoldR.Carriage.Length-2;
	
	
	/*  valve configure parameter change  */
	if(bValveChangeL)
	{
		bValveChangeL = 0;
		gRecipeOperate.DataSave = 1;
		LMold.Carriage.ValveLinear.update = 1;
	}
   
	if(bValveChangeR)
	{
		bValveChangeR = 0;
		gRecipeOperate.DataSave = 1;
		RMold.Carriage.ValveLinear.update = 1;
	}
	
	
	/* velocity  */
	if( ACTUATOR_ELECTRIC ==  gMachineFix.MoldR.Carriage.eActuatorType)
	{
		if(TogLinearL.status != 0) //LinNotOK 
		{
			vActHydDT2_L.u = LMold.Carriage.mmAs;
		}
		else
		{
			vActHydDT2_L.u = LMold.Carriage.mmAsDrive;
		}
		
		if(TogLinearR.status != 0)  //LinNotOK
		{
			vActHydDT2_R.u = RMold.Carriage.mmAs;
		}
		else
		{
			vActHydDT2_R.u = RMold.Carriage.mmAsDrive;
		}
	}
	else
	{
		vActHydDT2_L.u = LMold.Carriage.mmAs;
		vActHydDT2_R.u = RMold.Carriage.mmAs;
	}
	
	/*  run function  */
	if(ACTUATOR_HYD == gMachineFix.MoldR.Carriage.eActuatorType)
	{
		if(SENSOR_LVDT == gMachineFix.MoldR.Carriage.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				TransportLVDT(&LMold,&gMachineFix.MoldL.Carriage,&gMachinePara.MoldL.Carriage,&gSPC_Current.MoldL,&gUserPara.LMold);
			}
			TransportLVDT(&RMold,&gMachineFix.MoldR.Carriage,&gMachinePara.MoldR.Carriage,&gSPC_Current.MoldR,&gUserPara.RMold);
		}
		else if(SENSOR_SWITCH == gMachineFix.MoldR.Carriage.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				TransportSwitch(&LMold,&gMachineFix.MoldL.Carriage,&gMachinePara.MoldL.Carriage,&gSPC_Current.MoldL,&gUserPara.LMold);
			}
			TransportSwitch(&RMold,&gMachineFix.MoldR.Carriage,&gMachinePara.MoldR.Carriage,&gSPC_Current.MoldR,&gUserPara.RMold);
		}
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Carriage.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			TransportDriveToggle_Drive(&LMold,&gMachineFix.MoldL.Carriage,&gMachinePara.MoldL.Carriage,&gSPC_Current.MoldL,&gUserPara.LMold);
		}
		TransportDriveToggle_Drive(&RMold,&gMachineFix.MoldR.Carriage,&gMachinePara.MoldR.Carriage,&gSPC_Current.MoldR,&gUserPara.RMold);
	}
	
	
	/* other mold backward  */
	LMold.OtherMoldBw = RMold.Carriage.BwPos;
	RMold.OtherMoldBw = LMold.Carriage.BwPos;

	if(1 == gMachineFix.Option.bHideLeftStation)
	{
		LMold.Carriage.Error = 0;
		LMold.Carriage.CalOK = 1;
		LMold.Carriage.BwPos = 1;
		
		RMold.OtherMoldBw = LMold.Carriage.BwPos;
	}
   
	/* valve output*/
	gMachineOut.LMold.CarriageFw = LMold.ValveOut.CarriageFw;
	gMachineOut.LMold.CarriageBw = LMold.ValveOut.CarriageBw;
   
	gMachineOut.RMold.CarriageFw = RMold.ValveOut.CarriageFw;
	gMachineOut.RMold.CarriageBw = RMold.ValveOut.CarriageBw;
	
	gMachineOut.RMold.RobotCarriageDn = RMold.ValveOut.RobotCarriageDn;/*Albert RobotCarraigeDn*/
	gMachineOut.LMold.RobotCarriageDn = LMold.ValveOut.RobotCarriageDn;/*Albert RobotCarraigeDn*/
	
	gMachineOut.RMold.RobotCarriageUp = RMold.ValveOut.RobotCarriageUp;/*Chaoi RobotCarraigeUp*/
	gMachineOut.LMold.RobotCarriageUp = LMold.ValveOut.RobotCarriageUp;/*Chaoi RobotCarraigeUp*/
    
	gMachineInfo.LCarriagePos = LMold.Carriage.mmAs;
	gMachineInfo.RCarriagePos = RMold.Carriage.mmAs;
	gMachineInfo.LCarriagePos1 = gMachineFix.MoldL.Carriage.Length-LMold.Carriage.mmAs;
	
	
	gMachineOut.LMold.CarriageAux = LMold.ValveOut.CarriageAux;
	gMachineOut.RMold.CarriageAux = RMold.ValveOut.CarriageAux;
	
	gMachineOut.LMold.CarriageFwCalib = LMold.ValveOut.CarriageFwCalib;
	gMachineOut.LMold.CarriageBwCalib = LMold.ValveOut.CarriageBwCalib;

	gMachineOut.RMold.CarriageFwCalib = RMold.ValveOut.CarriageFwCalib;
	gMachineOut.RMold.CarriageBwCalib = RMold.ValveOut.CarriageBwCalib;
	
	gMachineOut.LMold.CarriageFwHold = LMold.ValveOut.CarriageFwHold;
	gMachineOut.LMold.CarriageBwHold = LMold.ValveOut.CarriageBwHold;
	
	gMachineOut.RMold.CarriageFwHold = RMold.ValveOut.CarriageFwHold;
	gMachineOut.RMold.CarriageBwHold = RMold.ValveOut.CarriageBwHold;
	
	ButtonOffDelay(&RMold ,&gMacTimer.RCarriageBtnOffDly);
	ButtonOffDelay(&LMold ,&gMacTimer.LCarriageBtnOffDly);
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.Carriage.eActuatorType = gMachineFix.MoldR.Carriage.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.Carriage.eValveType = gMachineFix.MoldR.Carriage.eValveType;
	
	/*-------------- SensorType----------------- */
	gMachineFix.MoldL.Carriage.eSensorType = gMachineFix.MoldR.Carriage.eSensorType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.Carriage.ePressueOutputType = gMachineFix.MoldR.Carriage.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.Carriage.eFluxOutputType = gMachineFix.MoldR.Carriage.eFluxOutputType;
	
	/*------------ MechanismType--------------- */
	gMachineFix.MoldL.Carriage.eMechanismType = gMachineFix.MoldR.Carriage.eMechanismType;

	
	/*Albert*/
	TON_10ms(&RobotCarriageTime);
	TON_10ms(&LRobotCarriageTime);
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.Carriage.Position 		= RMold.Carriage.mmAs;
	gMotorTrace.LMold.Carriage.Position 		= LMold.Carriage.mmAs;

} /* end of _CYCLIC */

void PropVlaveInit(Carriage_Fix_typ * pCarriageFix)
{
	pCarriageFix->ValveLin[0].X = -10.0; pCarriageFix->ValveLin[0].Y = -1000.0;  /* v --- mm/s  */
	pCarriageFix->ValveLin[1].X = -1.80; pCarriageFix->ValveLin[1].Y = -100.0;
	pCarriageFix->ValveLin[2].X = -1.60; pCarriageFix->ValveLin[2].Y = -50.0;
	pCarriageFix->ValveLin[3].X = -0.40; pCarriageFix->ValveLin[3].Y = -0.1;
	pCarriageFix->ValveLin[4].X =   0.0; pCarriageFix->ValveLin[4].Y =  0.0;
	pCarriageFix->ValveLin[5].X =  0.40; pCarriageFix->ValveLin[5].Y =  0.1;
	pCarriageFix->ValveLin[6].X =  1.60; pCarriageFix->ValveLin[6].Y =  50.0;
	pCarriageFix->ValveLin[7].X =  1.80; pCarriageFix->ValveLin[7].Y =  100.0;
	pCarriageFix->ValveLin[8].X =  10.0; pCarriageFix->ValveLin[8].Y =  1000.0;
}

REAL xTogLinearLTrans(REAL pCarriagePos)
{
	xTogLinearL.Y = pCarriagePos;
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

REAL xTogLinearRTrans(REAL pCarriagePos)
{
	xTogLinearR.Y = pCarriagePos;
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
	if(  1 == pMold->Carriage.BtnOffDlyFlag )
	{
		pTimer->IN = 1;
		pTimer->PT = 200;
	
		if( 1 == pTimer->Q)
		{
			pMold->Carriage.BtnOffDlyFlag = 0;
		}
	}
	else
	{
		pTimer->IN = 0;
	}
	
}

void TransportSwitch(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser)
{
	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pCarriageFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pCarriageFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	/* No LVDT, no calibration false   */
	pMold->Carriage.CalOK = 1;
	pMold->Alarm.CarriageCalErr = 0;
	
	/* gAlarm time value    */
	if(!pMold->TimeSet.CarriageFwAlarmTime)pMold->TimeSet.CarriageFwAlarmTime = 1000;   /* 10s  */
	if(!pMold->TimeSet.CarriageBwAlarmTime)pMold->TimeSet.CarriageBwAlarmTime = 1000;   /* 10s  */
	
	/*********************************************************************************
	**                            HMI data value to PLC                             **
	*********************************************************************************/
  
	if(pCarriageFix->Fw.V_Max < 500.0)pCarriageFix->Fw.V_Max = 1000.0;
	if(pCarriageFix->Bw.V_Max < 500.0)pCarriageFix->Bw.V_Max = 1000.0;
    
	if(pCarriageFix->Fw.VoltageMax > -3.0)pCarriageFix->Fw.VoltageMax = -10.0;
	if(pCarriageFix->Bw.VoltageMax < 3.0 )pCarriageFix->Bw.VoltageMax = 10.0;   

	pMold->Carriage.V_Fw = pCarriagePara->V_Fw * pCarriageFix->Fw.V_Max/100.0;
	pMold->Carriage.V_FwSlow = pCarriagePara->V_FwSlow * pCarriageFix->Fw.V_Max/100.0;
	pMold->Carriage.V_FwHold = pCarriagePara->V_FwHold * pCarriageFix->Fw.V_Max/100.0;
	
	pMold->Carriage.P_Fw = pCarriagePara->P_Fw;
	pMold->Carriage.P_FwSlow = pCarriagePara->P_FwSlow;
	pMold->Carriage.P_FwHold = pCarriagePara->P_FwHold;
     
	pMold->Carriage.V_Bw = pCarriagePara->V_Bw * pCarriageFix->Bw.V_Max/100.0;
	pMold->Carriage.V_BwSlow = pCarriagePara->V_BwSlow * pCarriageFix->Bw.V_Max/100.0;
	pMold->Carriage.V_BwHold = pCarriagePara->V_BwHold * pCarriageFix->Fw.V_Max/100.0;
	
	pMold->Carriage.P_Bw = pCarriagePara->P_Bw;
	pMold->Carriage.P_BwSlow = pCarriagePara->P_BwSlow;
	pMold->Carriage.P_BwHold = pCarriagePara->P_BwHold;
	
	if(0 == pCarriagePara->V_BwReset)pCarriagePara->V_BwReset = 2.0;
	pMold->Carriage.V_BwReset = pCarriagePara->V_BwReset * pCarriageFix->Bw.V_Max/100.0;
	
	pMold->Carriage.P_BwReset = pCarriagePara->P_BwReset;
	
	if(pCarriageFix->Fw.accel_stop < 100)pCarriageFix->Fw.accel_stop = INSTANCY_STOP_DECEL; 
	if(pCarriageFix->Bw.accel_stop < 100)pCarriageFix->Bw.accel_stop = INSTANCY_STOP_DECEL;
	/*********************************************************************************
  	**                             carriage status                                  **
  	*********************************************************************************/
	
	/* Up or down  position flag  */
	pMold->Carriage.FwPos = pMold->TransDIn.CarriageFwLimit;
	pMold->Carriage.BwPos = pMold->TransDIn.CarriageBwLimit;

	/* Up or down move step */
	switch ( pMold->Carriage.Step )
	{
		/*------------------ stop all the Output ------------------*/
		case 0:
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageFw  = 0;
			
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			
			RobotCarriageTime.IN = 0;
			LRobotCarriageTime.IN = 0;
			break;

		/*------------------ Carriage move up -------------------- */
		case 100: /* forward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT = pMold->TimeSet.CarriageFwDelay;
				pMold->Carriage.Step = 200;
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;

		case 200: /*delay ok*/
			pMold->TimeDis.CarriageFwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.Timer.IN = 0;
				//				pMold->TimeDis.CarriageFwDelay = 0;
				pMold->Carriage.Step = 300;
			}
			break;

		case 300:
			pMold->Carriage.LimitTimer.IN = 1;
			pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageFwAlarmTime;
			
			pMold->Carriage.vRamp.Once = 0;
			pMold->Carriage.vRamp.Set  = 0.0;
            
			pMold->Carriage.vRamp.Accel = pCarriageFix->Fw.accel;
			if(pMold->Carriage.vRamp.Accel < 1.0)pMold->Carriage.vRamp.Accel = 1000;
			
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
			
			pMold->Carriage.Step = 400;
			
			break;

		case 400:	/* forward quick  */
			pMold->ActInfo.CarriageFw  = 1;
			pMold->ActInfo.CarriageBw  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 1;
			pMold->ValveOut.CarriageFwHold  = 1;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			

			pMold->Carriage.vRamp.Set = pMold->Carriage.V_Fw;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.P_Fw;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(1 == pMold->TransDIn.CarriageSlowFwLimit)
			{
				pMold->Carriage.Step = 500;
			}
			break;
		
		case 500:	/* forward slow  */
			pMold->ActInfo.CarriageFw  = 1;
			pMold->ActInfo.CarriageBw  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 1;
			pMold->ValveOut.CarriageFwHold  = 1;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			
			/*2018.7.16 Albert Add condition if carriage not in backward position speed is slow speed*/

			pMold->Carriage.p_set = pMold->Carriage.P_FwSlow;
			pMold->Carriage.vRamp.Set = pMold->Carriage.V_FwSlow;
			
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
    
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(1 == pMold->Carriage.FwPos)
			{
				pMold->Carriage.p_set = 0.0;
				pMold->Carriage.Step = 600;
			}
			break;
		
		case 600:  /* forward hold  */
			if(pMoldUser->TimeSet.CarriageFwHoldTime != 0)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT =  pMoldUser->TimeSet.CarriageFwHoldTime;							
				pMold->Carriage.Step = 700;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageFwHoldTime = pMoldUser->TimeSet.CarriageFwHoldTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageFwHoldTime = pMoldUser->TimeSet.CarriageFwHoldTime/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 800;
			}
			break;
		
		case 700:
			pMoldUser->TimeDis.CarriageFwHoldTime = pMold->Carriage.Timer.ET;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 1;
			pMold->ValveOut.CarriageFwHold  = 1;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
				
			pMold->Carriage.p_set = pMold->Carriage.P_FwHold;
			pMold->Carriage.v_set = pMold->Carriage.V_FwHold;  
			
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.p_set = 0;
			
				pMold->Carriage.Timer.IN = 0;
//				pMold->Carriage.LimitTimer.IN = 0;
			
				pMold->Carriage.Step = 750;
			}
			break;	
		
		case 750:
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
			pMold->Carriage.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 0.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 800;
			}
			break;

		case 800:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCarriageFw = pMold->Carriage.LimitTimer.ET/100.0;
			}
			
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0; 
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 3000;
			break;
		
		case 3000:
			break;
			
		/*  --------------- 调校移模进 --------------- */
		case 5000:
			
			if(0 == pMold->TransDIn.CarriageFwLimit)
			{
				pMold->ActInfo.CarriageFw  = 1;
				pMold->ActInfo.CarriageBw  = 0;
			
				//*--VALVE_DIR--*//
				/*-- Fw --*/
				pMold->ValveOut.CarriageFw  = 1;
				pMold->ValveOut.CarriageFwHold  = 1;
				pMold->ValveOut.CarriageFwCalib  = 1;
				
				/*-- Bw --*/
				pMold->ValveOut.CarriageBw  = 0;
				pMold->ValveOut.CarriageBwHold  = 0;
				pMold->ValveOut.CarriageBwCalib  = 0;
				pMold->ValveOut.CarriageAux  = 0;
			
				pMold->Carriage.p_set = pCarriageFix->pCalibZero;
				pMold->Carriage.v_set = -pCarriageFix->vCalibZero * 10;
			}
			else
			{
				pMold->ActInfo.CarriageFw  = 0;
				pMold->ActInfo.CarriageBw  = 0;
			
				//*--VALVE_DIR--*//
				/*-- Fw --*/
				pMold->ValveOut.CarriageFw  = 0;
				pMold->ValveOut.CarriageFwHold  = 0;
				pMold->ValveOut.CarriageFwCalib  = 0;
				
				/*-- Bw --*/
				pMold->ValveOut.CarriageBw  = 0;
				pMold->ValveOut.CarriageBwHold  = 0;
				pMold->ValveOut.CarriageBwCalib  = 0;
				pMold->ValveOut.CarriageAux  = 0;
			
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0;
			}

			break;
		
		case 8000:
			break;
		
		/*--------------- Carriage move backward -----------------*/
		case 10100: /* backward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN  = 1;
				pMold->Carriage.Timer.PT  = pMold->TimeSet.CarriageBwDelay;
				pMold->Carriage.Step = 10200;
			
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageBwDelay = pMold->TimeSet.CarriageBwDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageBwDelay = pMold->TimeSet.CarriageBwDelay/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;
         
		case 10200: /*delay ok*/
			pMold->TimeDis.CarriageBwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.Timer.IN = 0;
				//				pMold->TimeDis.CarriageBwDelay = 0;
				pMold->Carriage.Step = 10300;
			}
			break;
         
		case 10300:
			pMold->Carriage.LimitTimer.IN = 1;
			pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageBwAlarmTime;
			
			pMold->Carriage.vRamp.Once = 0;
			pMold->Carriage.vRamp.Set  = 0.0;
            
			pMold->Carriage.vRamp.Accel = pCarriageFix->Bw.accel;
			if(pMold->Carriage.vRamp.Accel < 1.0)pMold->Carriage.vRamp.Accel = 1000;
			
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
			
			if( 1 == gMachineInfo.MachineReset)  // Reset  slow
			{
				pMold->Carriage.ResetSlowFlag = 1;
				pMold->Carriage.Step = 10500;
			}
			else
			{
				pMold->Carriage.Step = 10400;
			}
			
			break;
		
		case 10400:	/* backward quick  */
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 1;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 1;
			pMold->ValveOut.CarriageBwHold  = 1;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 1;
			
			pMold->Carriage.vRamp.Set = pMold->Carriage.V_Bw;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.P_Bw;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(1 == pMold->TransDIn.CarriageSlowBwLimit)
			{
				pMold->Carriage.Step = 10500;
			}
			break;
         
		case 10500:	/* backward slow  */
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 1;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 1;
			pMold->ValveOut.CarriageBwHold  = 1;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			if(1 == gMachineInfo.MachineReset || 1 == pMold->Carriage.ResetSlowFlag)
			{
				pMold->Carriage.p_set = pMold->Carriage.P_BwReset;
				pMold->Carriage.vRamp.Set = pMold->Carriage.V_BwReset;
			}
			else
			{	
				pMold->Carriage.p_set = pMold->Carriage.P_BwSlow;
				pMold->Carriage.vRamp.Set = pMold->Carriage.V_BwSlow;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(1 == pMold->Carriage.BwPos)
			{
				pMold->Carriage.p_set = 0.0;
				pMold->Carriage.Step = 10600;
			}
			break;
		
		case 10600:  /* backward hold  */
			if(pMoldUser->TimeSet.CarriageBwHoldTime != 0)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT =  pMoldUser->TimeSet.CarriageBwHoldTime;							
				pMold->Carriage.Step = 10700;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageBwHoldTime = pMoldUser->TimeSet.CarriageBwHoldTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageBwHoldTime = pMoldUser->TimeSet.CarriageBwHoldTime/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 10800;
			}
			break;
		
		case 10700:

			pMoldUser->TimeDis.CarriageBwHoldTime = pMold->Carriage.Timer.ET;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 1;
			pMold->ValveOut.CarriageBwHold  = 1;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			pMold->Carriage.p_set = pMold->Carriage.P_BwHold;
			pMold->Carriage.v_set = pMold->Carriage.V_BwHold;  
			
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.p_set = 0;
			
				pMold->Carriage.Timer.IN = 0;
//				pMold->Carriage.LimitTimer.IN = 0;
			
				pMold->Carriage.Step = 10750;
			}
			break;
		
		case 10750:
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
			pMold->Carriage.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 0.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 10800;
			}
			break;
		
		case 10800:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCarriageBw = pMold->Carriage.LimitTimer.ET/100.0;
			}
			
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 0;
			
			pMold->Carriage.ResetSlowFlag = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0; 
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 13000;
			break;
      
		case 13000:
			break;
        
		/*  --------------- 校调移模退 --------------- */
		case 15000:
			
			if(0 == pMold->TransDIn.CarriageBwLimit)
			{
				pMold->ActInfo.CarriageFw  = 0;
				pMold->ActInfo.CarriageBw  = 1;
			
				//*--VALVE_DIR--*//
				/*-- Fw --*/
				pMold->ValveOut.CarriageFw  = 0;
				pMold->ValveOut.CarriageFwHold  = 0;
				pMold->ValveOut.CarriageFwCalib  = 0;
				
				/*-- Bw --*/
				pMold->ValveOut.CarriageBw  = 1;
				pMold->ValveOut.CarriageBwHold  = 1;
				pMold->ValveOut.CarriageBwCalib  = 1;
				pMold->ValveOut.CarriageAux  = 0;
			
				pMold->Carriage.p_set = pCarriageFix->pCalibMax;
				pMold->Carriage.v_set = pCarriageFix->vCalibMax * 10;
			}
			else
			{
				pMold->ActInfo.CarriageFw  = 0;
				pMold->ActInfo.CarriageBw  = 0;
			
				//*--VALVE_DIR--*//
				/*-- Fw --*/
				pMold->ValveOut.CarriageFw  = 0;
				pMold->ValveOut.CarriageFwHold  = 0;
				pMold->ValveOut.CarriageFwCalib  = 0;
				
				/*-- Bw --*/
				pMold->ValveOut.CarriageBw  = 0;
				pMold->ValveOut.CarriageBwHold  = 0;
				pMold->ValveOut.CarriageBwCalib  = 0;
				pMold->ValveOut.CarriageAux  = 0;
			
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0;
			}

			break;
		
		case 18000:
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 1;
			
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 1;
			
			if(gMachineOut.P_Set < 1.0)pMold->Carriage.p_set = pCarriageFix->pKeep;
			if(gMachineOut.Q_Set < 1.0)pMold->Carriage.v_set = pCarriageFix->vKeep;
			if(0 == gMachineOut.OilPump)
			{
				pMold->ActInfo.CarriageBw  = 0;
				pMold->ValveOut.CarriageBw = 0;
			
				pMold->Carriage.p_set = 0.0; 
				pMold->Carriage.v_set = 0.0;
			
				pMold->Carriage.Step = 0;
			}
			break;
		
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;

			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
            
			pMold->Carriage.p_old = pMold->Carriage.p_set;

			pMold->Carriage.Step = 21000;
			break;

		case 21000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			if(pMold->Carriage.v_set > 0)
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Bw.accel_stop;
			}
			else
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Fw.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 22000;
			break;

		case 22000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.p_set = 0.0;
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 0;
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
            
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 0;
            
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 0.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 31000;
			break;
        
		case 31000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.p_set = 0.0;
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step  = 0;
			}
			break;
            
		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
            
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 0;
         
			pMold->Carriage.p_set = 0.0;
			pMold->Carriage.v_set = 0.0;
			break;
      
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
					
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
					
			pMold->Carriage.Step = 42000;
			break;
						
		case 42000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
					
			pMold->Carriage.vRamp.Ts = tTask;
					
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
			pMold->Carriage.Step = 43000;
			break;
					
		case 43000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
				
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
					
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 40000;
			}
			break;
	}/* end of switch */

	if(pMold->Carriage.Step > 300 && pMold->Carriage.Step < 700)
	{
		pMold->TimeDis.CarriageFwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
	else if(pMold->Carriage.Step > 10300 && pMold->Carriage.Step < 10700)
	{
		pMold->TimeDis.CarriageBwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
   
	/* ----------------- Carriage Up / Down timeout --------------------  */
	if(1 == pMold->Carriage.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)		/* Carriage move up  */
		{
			pMold->Alarm.CarriageNotAtFwDI = 1;
		}
      
		if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)	/* Carriage move down  */
		{
			pMold->Alarm.CarriageNotAtBwDI = 1;
		}
   
		pMold->Carriage.Step = 41000;
	}/* if(1 == pMold->Carriage.LimitTimer.Q) */

	/* ----------------- Carriage forward / backward action limit --------------------  */
	if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)	  /* Carriage move up  */
	{
		if(0 == pMold->OtherMoldBw )				/*  The other mold not at backward  */
		{
			if(pMold == & LMold)
			{
				RMold.Alarm.CarriageNotAtBwDI  = !RMold.Carriage.BwPos;
			}
			else
			{
				LMold.Alarm.CarriageNotAtBwDI  = !LMold.Carriage.BwPos;
			}
         
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 41000;
		}/* if(0 == pMold->OtherMoldBw ) */
   
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit)
		{
			pMold->Alarm.SubMoldNotAtClsPos  = !pMold->TransDIn.SubMoldClsLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.TopDeflashBwLimit)/*  Top deflash not at backward limit   */
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}

		
		if( 1 == gMachineInfo.Auto)				/*  Auto or AutoReset mode  */
		{
			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
			if(0 == pMold->Clamp.OpnPos  &&  300 == pMold->Carriage.Step )  // while carriage up start ，OpnPos need active. When carriage is moving，the OpnPos no need any more  //owen 
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
		}
		else   // in calib or manual mode the OpnPos will support by  gRClampOpnFlag_Calib (parker special flag)
		{
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
			
			if( 0 == pMold->TransDIn.HookPinBwLimit)/*  Hook Pin not at backward limit   */
			{
				pMold->Alarm.HookPinNotAtBwPos = !pMold->TransDIn.HookPinBwLimit;
				pMold->Carriage.Step = 41000;
			}
		}
		
		
		
	
		
		//		if( 1 == gMachineInfo.TimeLimit)				/*  Auto or AutoReset mode  */
		//		{
		//			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
		//			if(0 == pMold->Clamp.OpnPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos  && 0 == pMold->BlowPin.UpPos) /* Clamp not at open position & Blowpin not at up position */
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = !pMold->Clamp.OpnPos;
		//				pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		
	}/* if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200) */
   
	if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)  /* Carriage move down  */
	{
		/*  Top deflash not at backward limit   */	// 2018.05.29 Jack: No need this action limit due to top deflash and carriage are at the same structure
		if( 0 == pMold->TransDIn.TopDeflashBwLimit) 
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
      
		if(0 == pMold->Robot.Transfer.BwPos)
		{
			pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
			pMold->Carriage.Step = 41000;
		}
		
		
		
		if( 0 == pMold->TransDIn.CoolPinUpLimit)
		{
			pMold->Alarm.CoolPinNotAtUpPos  = !pMold->TransDIn.CoolPinUpLimit;
			pMold->Carriage.Step = 41000;
		}
			
//		if(0 == pMold->TransDIn.CoolPin2UpLimit)
//		{
//			pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
//		
//		if(0 == pMold->TransDIn.CoolPin3UpLimit)
//		{
//			pMold->Alarm.CoolPin3NotAtUpPos = !pMold->TransDIn.CoolPin3UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
		
		if(0 == pMold->BlowPin.UpPos )	/* Blowpin not at up position */
		{
			pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->BlowPin2.UpPos )	/* Blowpin2 not at up position */
		{
			pMold->Alarm.BlowPin2NotAtUpPos = !pMold->BlowPin2.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.ExtRobotUpLmit )	
		{
			pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.HookPinBwLimit)/*  Hook Pin not at backward limit   */
		{
			pMold->Alarm.HookPinNotAtBwPos = !pMold->TransDIn.HookPinBwLimit;
			pMold->Carriage.Step = 41000;
		}

      
		//		if(1 == gMachineInfo.Auto)
		//		{
		//			if(0 == pMold->Clamp.ClsPos)			      /* Clamp not at close position */
		//			{
		//				pMold->Alarm.ClampNotAtClsDI = !pMold->Clamp.ClsPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//			
		//		}
		//		else
		//		{
		//			
		//		}/* if(1 == gMachineInfo.Auto) */
	}/* if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200) */	

}/* end of function */

void TransportLVDT(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser)
{
	
	
   /*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pCarriageFix->Length < 100.0)pCarriageFix->Length = 400.0;
	if(pCarriageFix->Tolerance < 0.1)pCarriageFix->Tolerance = 0.1;
	pMold->Carriage.Tolerance = pCarriageFix->Tolerance;
    
	if(pCarriageFix->Fw.V_Max < 500.0)pCarriageFix->Fw.V_Max = 1000.0;
	if(pCarriageFix->Bw.V_Max < 500.0)pCarriageFix->Bw.V_Max = 1000.0;
    
	if(pCarriageFix->Fw.VoltageMax > -3.0)pCarriageFix->Fw.VoltageMax = -10.0;
	if(pCarriageFix->Bw.VoltageMax < 3.0 )pCarriageFix->Bw.VoltageMax = 10.0;
	
	pMold->Carriage.Tolerance = pCarriageFix->Tolerance;
    
	pMold->Carriage.S_Fw = pCarriagePara->S_Fw;
	pMold->Carriage.S_FwCreep = pCarriagePara->S_FwCreep;
   
	pMold->Carriage.V_Fw = pCarriagePara->V_Fw * pCarriageFix->Fw.V_Max/100.0;
	pMold->Carriage.V_FwSlow = pCarriagePara->V_FwSlow * pCarriageFix->Fw.V_Max/100.0;
	pMold->Carriage.V_FwHold = pCarriagePara->V_FwHold * pCarriageFix->Fw.V_Max/100.0;
	
	pMold->Carriage.P_Fw = pCarriagePara->P_Fw;
	pMold->Carriage.P_FwSlow = pCarriagePara->P_FwSlow;
	pMold->Carriage.P_FwHold = pCarriagePara->P_FwHold;
    
	pMold->Carriage.S_Bw = pCarriagePara->S_Bw;
	pMold->Carriage.S_BwCreep = pCarriagePara->S_BwCreep ;
     
	pMold->Carriage.V_Bw = pCarriagePara->V_Bw * pCarriageFix->Bw.V_Max/100.0;
	pMold->Carriage.V_BwSlow = pCarriagePara->V_BwSlow * pCarriageFix->Bw.V_Max/100.0;
	pMold->Carriage.V_BwHold = pCarriagePara->V_BwHold * pCarriageFix->Fw.V_Max/100.0;
	
	pMold->Carriage.P_Bw = pCarriagePara->P_Bw;
	pMold->Carriage.P_BwSlow = pCarriagePara->P_BwSlow;
	pMold->Carriage.P_BwHold = pCarriagePara->P_BwHold;
	
	if(0 == pCarriagePara->V_BwReset)pCarriagePara->V_BwReset = 2.0;
	pMold->Carriage.V_BwReset = pCarriagePara->V_BwReset * pCarriageFix->Bw.V_Max/100.0;
	
	pMold->Carriage.P_BwReset = pCarriagePara->P_BwReset;
   
	if(pCarriageFix->Fw.accel_stop < 100)pCarriageFix->Fw.accel_stop = INSTANCY_STOP_DECEL; 
	if(pCarriageFix->Bw.accel_stop < 100)pCarriageFix->Bw.accel_stop = INSTANCY_STOP_DECEL;
    /*********************************************************************************
   **                            linear calibration status                        **
   *********************************************************************************/
   /* get calibration data  */
	if(pCarriageFix->Max < pCarriageFix->Zero)
	{
		pMold->Carriage.ZeroPos = pCarriageFix->Max;
		pMold->Carriage.MaxPos  = pCarriageFix->Zero;
	}
	else
	{
		pMold->Carriage.MaxPos  = pCarriageFix->Max;
		pMold->Carriage.ZeroPos = pCarriageFix->Zero;
	}
   
	if(pMold->Carriage.ZeroPos != 0                              &&
		pMold->Carriage.MaxPos > (pMold->Carriage.ZeroPos + 20000)  &&
		pMold->Carriage.As     > (pMold->Carriage.ZeroPos - 5000)   &&
		pMold->Carriage.As     < (pMold->Carriage.MaxPos  + 5000))
	{
		pMold->Carriage.CalOK = 1;
	}
	else
	{
		pMold->Carriage.CalOK = 0;
	}
	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pCarriageFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pCarriageFix->pMax = gMachineFix.Pump.MaxP1;
	}
   
	pCarriageFix->CalibOK = pMold->Carriage.CalOK;
   
	if(1 == pMold->Carriage.CalOK)
	{
		if(pCarriageFix->Max < pCarriageFix->Zero)
		{
			pMold->Carriage.mmAs = 1.0 *(pMold->Carriage.MaxPos - pMold->Carriage.As) * pCarriageFix->Length /
				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
		}
		else
		{
			pMold->Carriage.mmAs = 1.0 *(pMold->Carriage.As - pMold->Carriage.ZeroPos) * pCarriageFix->Length /
				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
		}
	}
	else
	{
		pMold->Carriage.mmAs = pMold->Carriage.mmAs;
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->Carriage.Step > 0 && pMold->Carriage.Step < 20000)
			{
				pMold->Carriage.Step = 0;
				pMold->Alarm.CarriageCalErr = 1;
			}
		}
	}/* if(1 == pMold->Carriage.CalOK) */
	
	/*  relative position  */
	pMold->Carriage.Ss = pMold->Carriage.As;
	pMold->Carriage.mmAsMax = pCarriageFix->Length + 10;

	pMold->Carriage.mmAsMax_HMI = pCarriageFix->Length + 50;
	
	
	/* gAlarm time value    */
	if(!pMold->TimeSet.CarriageFwAlarmTime)pMold->TimeSet.CarriageFwAlarmTime = 1000;   /* 10s  */
	if(!pMold->TimeSet.CarriageBwAlarmTime)pMold->TimeSet.CarriageBwAlarmTime = 1000;   /* 10s  */

	/* Fw or Bw  position flag  */
	if(1 == pMold->Carriage.CalOK)
	{
		pMold->Carriage.FwPos = pMold->TransDIn.CarriageFwLimit 
			&& (pMold->Carriage.mmAs > (pCarriagePara->S_FwCreep - pMold->Carriage.Tolerance))
			&& (pMold->Carriage.mmAs < (pCarriagePara->S_FwCreep + pMold->Carriage.Tolerance));
		pMold->Carriage.BwPos = pMold->TransDIn.CarriageBwLimit 
			&& (pMold->Carriage.mmAs < (pCarriagePara->S_BwCreep + pMold->Carriage.Tolerance))
			&& (pMold->Carriage.mmAs > (pCarriagePara->S_BwCreep - pMold->Carriage.Tolerance));
	}
	else
	{
		pMold->Carriage.FwPos = 0;
		pMold->Carriage.BwPos = 0;
	}
   
	/* ------------------ current position ------------------ */
	pMold->Carriage.vProfGen.s_act = pMold->Carriage.mmAs;
	pMold->Carriage.vProfGen.v_act = pMold->Carriage.v_act;
	

	/* Up or down move step */
	switch ( pMold->Carriage.Step )
	{
		/*------------------ stop all the Output ------------------*/
		case 0:
			pMold->Carriage.v_set = 0;
			pMold->Carriage.p_set = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
		
			
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageFw  = 0;
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			break;
		
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;

		/*------------------ Carriage move up -------------------- */
		case 100: /* forward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT = pMold->TimeSet.CarriageFwDelay;
				pMold->Carriage.Step = 200;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageFwDelay = pMold->TimeSet.CarriageFwDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageFwDelay = pMold->TimeSet.CarriageFwDelay/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;

		case 200: /*delay ok*/
			pMold->TimeDis.CarriageFwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.Timer.IN = 0;
				//   pMold->TimeDis.CarriageFwDelay = 0;
				pMold->Carriage.Step = 300;
			}
			break;

		case 300:

			pMold->Carriage.LimitTimer.IN = 1;
			pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageFwAlarmTime;
	            
			pMold->Carriage.vProfGen.Ts = tTask;
			pMold->Carriage.vProfGen.Tj = pCarriageFix->Fw.Tj;
			pMold->Carriage.vProfGen.Td = pCarriageFix->Fw.Td;
	            
			pMold->Carriage.vProfGen.accel = pCarriageFix->Fw.accel;
			if(pMold->Carriage.vProfGen.accel <= 0.0)pMold->Carriage.vProfGen.accel = 1000;
	            
			pMold->Carriage.vProfGen.decel = pCarriageFix->Fw.accel;
			if(pMold->Carriage.vProfGen.decel <= 0.0)pMold->Carriage.vProfGen.decel = 1000;
	            
			pMold->Carriage.vProfGen.decel_end = pCarriageFix->Fw.decel_end;
			if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
				
			pMold->Carriage.vProfGen.v_start = 0;			

			pMold->Carriage.vProfGen.v_end = 0.0;
			pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_FwCreep;
			pMold->Carriage.vProfGen.init = 1;
		
			if(pMold->Carriage.S_Fw > 0)	// fw quick 
			{
				pMold->Carriage.Step = 400;  
			}
			else
			{
				pMold->Carriage.Step = 500; // fw slow 
			}
			
		
			break;

		case 400:	/* forward quick  */
			pMold->ActInfo.CarriageFw  = 1;

			switch (pCarriageFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 1;
					pMold->ValveOut.CarriageFwHold  = 1;
					pMold->ValveOut.CarriageFwCalib  = 0;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 0;
					pMold->ValveOut.CarriageBwHold  = 0;
					pMold->ValveOut.CarriageBwCalib  = 0;
					pMold->ValveOut.CarriageAux  = 0;

						
					pMold->Carriage.p_set = pMold->Carriage.P_Fw; 
					pMold->Carriage.v_set = pMold->Carriage.V_Fw;  
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
				
							
					/*-- Bw --*/
						
								
					pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_Fw;
					/* call profile function */
					HydvProfGen(& pMold->Carriage.vProfGen);
				        
					pMold->Carriage.p_set = pMold->Carriage.P_Fw; 
					pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;  
			
					pMold->Carriage.actTjFw = pMold->Carriage.vProfGen.Tj;
					
					break;
			}

			if(pMold->Carriage.mmAs <= pMold->Carriage.S_Fw )
			{

				pMold->Carriage.Step = 500;
			}
			
			break;
		
		
		case 500:	/* forward slow  */
			pMold->ActInfo.CarriageFw  = 1;
			
			switch (pCarriageFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 1;
					pMold->ValveOut.CarriageFwHold  = 1;
					pMold->ValveOut.CarriageFwCalib  = 0;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 0;
					pMold->ValveOut.CarriageBwHold  = 0;
					pMold->ValveOut.CarriageBwCalib  = 0;
					pMold->ValveOut.CarriageAux  = 0;
						
				

					pMold->Carriage.p_set = pMold->Carriage.P_FwSlow; 
					pMold->Carriage.v_set = pMold->Carriage.V_FwSlow; 
					
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
				
							
					/*-- Bw --*/
						
								

					pMold->Carriage.p_set = pMold->Carriage.P_FwSlow; 
					pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_FwSlow;
					 
					pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_FwSlow;
					/* call profile function */
					HydvProfGen(& pMold->Carriage.vProfGen);
				        
					pMold->Carriage.p_set = pMold->Carriage.P_FwSlow; 
					pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;  
			
					pMold->Carriage.actTjFw = pMold->Carriage.vProfGen.Tj;
					
					break;
			}
			
			
			if(pMold->Carriage.mmAs <= pMold->Carriage.S_FwCreep + pMold->Carriage.Tolerance )
			{

				pMold->Carriage.Step = 600;
			}
					
			break;
		
		case 600:  /* forward hold  */
			if(pMoldUser->TimeSet.CarriageFwHoldTime != 0)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT =  pMoldUser->TimeSet.CarriageFwHoldTime;							
				pMold->Carriage.Step = 700;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageFwHoldTime = pMoldUser->TimeSet.CarriageFwHoldTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageFwHoldTime = pMoldUser->TimeSet.CarriageFwHoldTime/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 800;
			}
			break;
		
		
		case 700:

			pMoldUser->TimeDis.CarriageFwHoldTime = pMold->Carriage.Timer.ET;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 1;
			pMold->ValveOut.CarriageFwHold  = 1;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
			
			pMold->Carriage.p_set = pMold->Carriage.P_FwHold;
			pMold->Carriage.v_set = pMold->Carriage.V_FwHold;  
			
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0; 
			
				pMold->Carriage.Timer.IN = 0;
				pMold->Carriage.LimitTimer.IN = 0;
			
				pMold->Carriage.Step = 800;
			}
			break;	
		
		case 800:
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
			
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0; 
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 3000;
			break;
		
		
		
		case 3000:
			RobotCarriageTime.IN = 0;
			LRobotCarriageTime.IN = 0;
			
			break;
			
		/*  --------------- 调校移模进 --------------- */
		case 5000:
			pMold->ActInfo.CarriageFw  = 1;
			
			switch (pCarriageFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 1;
					pMold->ValveOut.CarriageFwHold  = 1;
					pMold->ValveOut.CarriageFwCalib  = 1;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 0;
					pMold->ValveOut.CarriageBwHold  = 0;
					pMold->ValveOut.CarriageBwCalib  = 0;
					pMold->ValveOut.CarriageAux  = 0;
						
					pMold->Carriage.p_set = pCarriageFix->pCalibZero; 
					pMold->Carriage.v_set = pCarriageFix->vCalibZero*10;  
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
				
							
					/*-- Bw --*/
						
								
					pMold->Carriage.p_set = pCarriageFix->pCalibZero; 
					pMold->Carriage.v_set = -pCarriageFix->vCalibZero; 
					
					break;
			}
			break;
		
		/*--------------- Carriage move backward -----------------*/
		case 10100: /* backward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN  = 1;
				pMold->Carriage.Timer.PT  = pMold->TimeSet.CarriageBwDelay;
				pMold->Carriage.Step = 10200;
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;
         
		case 10200: /*delay ok*/
			pMold->TimeDis.CarriageBwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.Timer.IN = 0;
				//    pMold->TimeDis.CarriageBwDelay = 0;
				pMold->Carriage.Step = 10300;
			}
			break;
         
		case 10300:

			pMold->Carriage.LimitTimer.IN = 1;
			if(1 == gMachineInfo.MachineReset )  /* 自動復歸  */
			{
			pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageBwAlarmTime;
			}
			else
			{
			pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageBwAlarmTime;
			} 
	
			pMold->Carriage.vProfGen.Ts = tTask;
			pMold->Carriage.vProfGen.Tj = pCarriageFix->Bw.Tj;
			pMold->Carriage.vProfGen.Td = pCarriageFix->Bw.Td;
	   
			pMold->Carriage.vProfGen.accel = pCarriageFix->Bw.accel;
			if(pMold->Carriage.vProfGen.accel <= 0.0)pMold->Carriage.vProfGen.accel = 1000;
	            
			pMold->Carriage.vProfGen.decel = pCarriageFix->Bw.accel;
			if(pMold->Carriage.vProfGen.decel <= 0.0)pMold->Carriage.vProfGen.decel = 1000;
	            
			pMold->Carriage.vProfGen.decel_end = pCarriageFix->Bw.decel_end;
			if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
	         
			pMold->Carriage.vProfGen.v_start = 0;
				
		
			if(1 == gMachineInfo.MachineReset)
			{
				pMold->Carriage.ResetSlowFlag = 1;
				pMold->Carriage.vProfGen.decel_end = 5.0 * pMold->Carriage.vProfGen.v_set;
				if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
				
				pMold->Carriage.vProfGen.v_end	= 0;
				pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreep;
				pMold->Carriage.vProfGen.init  = 1;		
				pMold->Carriage.Step = 10500;
			}
			else if(1 == gMachineInfo.PinCalibration && 1 == gMachineInfo.Calib)
			{
				pMold->Carriage.vProfGen.decel_end = 5.0 * pMold->Carriage.vProfGen.v_set;
				if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
				
				pMold->Carriage.vProfGen.v_end	= 0;
				pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreep;
				pMold->Carriage.vProfGen.init  = 1;		
				pMold->Carriage.Step = 10500;
			}
			else
			{
				pMold->Carriage.vProfGen.v_end	= 0;
				pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreep;
				pMold->Carriage.vProfGen.init  = 1;
				
				if(pMold->Carriage.S_Bw > 0)	// bw quick 
				{
					pMold->Carriage.Step = 10400;  
				}
				else
				{
					pMold->Carriage.Step = 10500; // bw slow 
				}	
			}

			break;
         
		case 10400:	/* backward quick  */
			pMold->ActInfo.CarriageBw  = 1;

			switch (pCarriageFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 0;
					pMold->ValveOut.CarriageFwHold  = 0;
					pMold->ValveOut.CarriageFwCalib  = 0;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 1;
					pMold->ValveOut.CarriageBwHold  = 1;
					pMold->ValveOut.CarriageBwCalib  = 0;
					pMold->ValveOut.CarriageAux  = 1;
								
					pMold->Carriage.p_set = pMold->Carriage.P_Bw; 
					pMold->Carriage.v_set = pMold->Carriage.V_Bw;  
		
					break;
						
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
						
									
					/*-- Bw --*/
							
									
					pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_Bw;
					/* call profile function */
					HydvProfGen(& pMold->Carriage.vProfGen);
						        
					pMold->Carriage.p_set = pMold->Carriage.P_Bw; 
					pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;  
					
					pMold->Carriage.actTjBw = pMold->Carriage.vProfGen.Tj;
						
					break;
			}

			if(pMold->Carriage.mmAs >= pMold->Carriage.S_Bw )
			{
				pMold->Carriage.Step = 10500;
			}
			
			break;
		
		case 10500: /* backward slow  */
			pMold->ActInfo.CarriageBw  = 1;
			
			switch (pCarriageFix->eValveType )
			{		
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 0;
					pMold->ValveOut.CarriageFwHold  = 0;
					pMold->ValveOut.CarriageFwCalib  = 0;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 1;
					pMold->ValveOut.CarriageBwHold  = 1;
					pMold->ValveOut.CarriageBwCalib  = 0;
					pMold->ValveOut.CarriageAux  = 0;
						
					pMold->Carriage.p_set = pMold->Carriage.P_BwSlow; 
					pMold->Carriage.v_set = pMold->Carriage.V_BwSlow; 
				
					if(1 == gMachineInfo.MachineReset || 1 == pMold->Carriage.ResetSlowFlag)
					{
						pMold->Carriage.p_set = pMold->Carriage.P_BwReset; 
						pMold->Carriage.v_set = pMold->Carriage.V_BwReset; 
					}
					else
					{
						pMold->Carriage.p_set = pMold->Carriage.P_BwSlow; 
						pMold->Carriage.v_set = pMold->Carriage.V_BwSlow; 
					}
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
				
							
					/*-- Bw --*/
						
				
					if(1 == gMachineInfo.MachineReset || 1 == pMold->Carriage.ResetSlowFlag)
					{
						pMold->Carriage.p_set = pMold->Carriage.P_BwReset; 
						pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_BwReset;
					}
					else
					{
						pMold->Carriage.p_set = pMold->Carriage.P_BwSlow; 
						pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_BwSlow;
					}
								
					/* call profile function */
					HydvProfGen(& pMold->Carriage.vProfGen);

					pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;  
			
					pMold->Carriage.actTjBw = pMold->Carriage.vProfGen.Tj;
					
					break;
			}
			
			
			if(pMold->Carriage.mmAs >= pMold->Carriage.S_BwCreep - pMold->Carriage.Tolerance )
			{

				pMold->Carriage.Step = 10600;
			}
			
			break;
      
						
		case 10600:   /* backward hold  */
			
			if(pMoldUser->TimeSet.CarriageBwHoldTime != 0)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT =  pMoldUser->TimeSet.CarriageBwHoldTime;							
				pMold->Carriage.Step = 10700;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.CarriageBwHoldTime = pMoldUser->TimeSet.CarriageBwHoldTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.CarriageBwHoldTime = pMoldUser->TimeSet.CarriageBwHoldTime/100.0;
				}
			}
			else
			{
				pMold->Carriage.Step = 10800;
			}
			break;

		case 10700:
			
			//TopDeflash force back
			/*----------------------------------------------------------------------------------*/
			if(1 == gMachineInfo.Auto) 
			{
				if ( ACTUATOR_HYD == gMachineFix.MoldR.TopDeflash.eActuatorType 
					&& 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash 
					&& 0 == pMold->Option.TopDeflash)
				{
					pMold->ValveOut.TopDeflashBw  = 1;
				}
			
				if ( ACTUATOR_HYD == gMachineFix.MoldR.BottomDeflash.eActuatorType 
					&& 1 == gMachineFix.Option.HWEnable.RMold.BottomDeflash 
					&& 0 == pMold->Option.BottomDeflash)
				{		
					pMold->ValveOut.BottomDeflashBw  = 1;
				}
			}
			/*----------------------------------------------------------------------------------*/
			
			pMoldUser->TimeDis.CarriageBwHoldTime = pMold->Carriage.Timer.ET;
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 1;
			pMold->ValveOut.CarriageBwHold  = 1;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
			
			pMold->Carriage.p_set = pMold->Carriage.P_BwHold;
			pMold->Carriage.v_set = pMold->Carriage.V_BwHold;  
			
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0; 
			
				pMold->Carriage.Timer.IN = 0;
				pMold->Carriage.LimitTimer.IN = 0;
			
				pMold->Carriage.Step = 10800;
			}
			break;
		
		case 10800:
			
			//TopDeflash force back
			/*----------------------------------------------------------------------------------*/
			if(1 == gMachineInfo.Auto) 
			{
				if ( ACTUATOR_HYD == gMachineFix.MoldR.TopDeflash.eActuatorType 
					&& 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash 
					&& 0 == pMold->Option.TopDeflash)
				{
					pMold->ValveOut.TopDeflashBw  = 0;
				}
			
				if ( ACTUATOR_HYD == gMachineFix.MoldR.BottomDeflash.eActuatorType 
					&& 1 == gMachineFix.Option.HWEnable.RMold.BottomDeflash 
					&& 0 == pMold->Option.BottomDeflash)
				{		
					pMold->ValveOut.BottomDeflashBw  = 0;
				}
			}
			/*----------------------------------------------------------------------------------*/
			
			
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCarriageBw = pMold->Carriage.LimitTimer.ET/100.0;
			}
			
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 0;
			
			pMold->Carriage.ResetSlowFlag = 0;
			
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
			
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0; 
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 13000;		
			break;
			
		case 13000:

			break;
        
		/*  --------------- 校调移模退 --------------- */
		case 15000:
			pMold->ActInfo.CarriageBw  = 1;
			
			switch (pCarriageFix->eValveType )
			{
			
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Fw --*/
					pMold->ValveOut.CarriageFw  = 0;
					pMold->ValveOut.CarriageFwHold  = 0;
					pMold->ValveOut.CarriageFwCalib  = 0;
				
					/*-- Bw --*/
					pMold->ValveOut.CarriageBw  = 1;
					pMold->ValveOut.CarriageBwHold  = 1;
					pMold->ValveOut.CarriageBwCalib  = 1;
					pMold->ValveOut.CarriageAux  = 0;
						
					pMold->Carriage.p_set = pCarriageFix->pCalibMax; 
					pMold->Carriage.v_set = pCarriageFix->vCalibMax*10;  
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Fw --*/
				
							
					/*-- Bw --*/
						
								
					pMold->Carriage.p_set = pCarriageFix->pCalibMax; 
					pMold->Carriage.v_set = pCarriageFix->vCalibMax; 
					
					break;
			}
			break;
		
		case 18000:
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 1;
			
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 1;
			
			if(gMachineOut.P_Set < 1.0)pMold->Carriage.p_set = pCarriageFix->pKeep;
			if(gMachineOut.Q_Set < 1.0)pMold->Carriage.v_set = pCarriageFix->vKeep;
			if(0 == gMachineOut.OilPump)
			{
				pMold->ActInfo.CarriageBw  = 0;
				pMold->ValveOut.CarriageBw = 0;
			
				pMold->Carriage.p_set = 0.0; 
				pMold->Carriage.v_set = 0.0;
			
				pMold->Carriage.Step = 0;
			}
			break;
		
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;

			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
            
			pMold->Carriage.p_old = pMold->Carriage.p_set;
			pMold->Carriage.Step = 21000;
			break;

		case 21000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			if(pMold->Carriage.v_set > 0)
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Bw.accel_stop;
			}
			else
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Fw.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 22000;
			break;

		case 22000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 0;
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;			
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
            
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 31000;
			break;
        
		case 31000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			//	pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.p_set = 0;
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step  = 0;
			}
			break;
            
		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
			//*--VALVE_DIR--*//
			/*-- Fw --*/
			pMold->ValveOut.CarriageFw  = 0;
			pMold->ValveOut.CarriageFwHold  = 0;
			pMold->ValveOut.CarriageFwCalib  = 0;
				
			/*-- Bw --*/
			pMold->ValveOut.CarriageBw  = 0;
			pMold->ValveOut.CarriageBwHold  = 0;
			pMold->ValveOut.CarriageBwCalib  = 0;
			pMold->ValveOut.CarriageAux  = 0;		
			
			//*--VALVE_PROP--*//
			/*-- Fw --*/
			
			/*-- Bw --*/
            
			pMold->Carriage.v_set = 0.0;
			break;
      
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
					
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
			
			pMold->Carriage.Step = 42000;
			break;
						
		case 42000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
					
			pMold->Carriage.vRamp.Ts = tTask;
					
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
			pMold->Carriage.Step = 43000;
			break;
					
		case 43000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
				
			//	pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
					
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.p_set = 0;
				pMold->Carriage.Step = 40000;
			}
			break;
	}/* end of switch */

	if(pMold->Carriage.Step > 300 && pMold->Carriage.Step < 800)
	{
		pMold->TimeDis.CarriageFwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
	else if(pMold->Carriage.Step > 10300 && pMold->Carriage.Step < 10800)
	{
		pMold->TimeDis.CarriageBwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
   
	/* ----------------- Carriage Up / Down timeout --------------------  */
	if(1 == pMold->Carriage.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)		/* Carriage move up  */
		{
			pMold->Alarm.CarriageNotAtFwDI = 1;
		}
      
		if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)	/* Carriage move down  */
		{
			pMold->Alarm.CarriageNotAtBwDI = 1;
		}
   
		pMold->Carriage.Step = 41000;
	}/* if(1 == pMold->Carriage.LimitTimer.Q) */

	/* ----------------- Carriage forward / backward action limit --------------------  */
	if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)	  /* Carriage move up  */
	{
		if(0 == pMold->OtherMoldBw )				/*  The other mold not at backward  */
		{
			if(pMold == & LMold)
			{
				RMold.Alarm.CarriageNotAtBwDI  = !RMold.Carriage.BwPos;
			}
			else
			{
				LMold.Alarm.CarriageNotAtBwDI  = !LMold.Carriage.BwPos;
			}
         
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 41000;
		}/* if(0 == pMold->OtherMoldBw ) */
   
		if( 0 == pMold->TransDIn.SubMoldClsLimit)
		{
			pMold->Alarm.SubMoldNotAtClsPos  = !pMold->TransDIn.SubMoldClsLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.TopDeflashBwLimit)/*  Top deflash not at backward limit   */
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		
		if( 1 == gMachineInfo.Auto)				/*  Auto or AutoReset mode  */
		{
			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
			if(0 == pMold->Clamp.OpnPos  &&  300 == pMold->Carriage.Step )  // while carriage up start ，OpnPos need active. When carriage is moving，the OpnPos no need any more  //owen 
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
		}
		else   // in calib or manual mode the OpnPos will support by  gRClampOpnFlag_Calib (parker special flag)
		{
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
			
			if( 0 == pMold->TransDIn.HookPinBwLimit)/*  Hook Pin not at backward limit   */
			{
				pMold->Alarm.HookPinNotAtBwPos = !pMold->TransDIn.HookPinBwLimit;
				pMold->Carriage.Step = 41000;
			}
		}
		
		
		
	
		
		//		if( 1 == gMachineInfo.TimeLimit)				/*  Auto or AutoReset mode  */
		//		{
		//			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
		//			if(0 == pMold->Clamp.OpnPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos  && 0 == pMold->BlowPin.UpPos) /* Clamp not at open position & Blowpin not at up position */
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = !pMold->Clamp.OpnPos;
		//				pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		
	}/* if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200) */
   
	if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)  /* Carriage move down  */
	{
		/*  Top deflash not at backward limit   */	// 2018.05.29 Jack: No need this action limit due to top deflash and carriage are at the same structure
		if( 0 == pMold->TransDIn.TopDeflashBwLimit) 
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
      
		if(0 == pMold->Robot.Transfer.BwPos)
		{
			pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
			pMold->Carriage.Step = 41000;
		}
		
		
		
		if( 0 == pMold->TransDIn.CoolPinUpLimit)
		{
			pMold->Alarm.CoolPinNotAtUpPos  = !pMold->TransDIn.CoolPinUpLimit;
			pMold->Carriage.Step = 41000;
		}

			
//		if(0 == pMold->TransDIn.CoolPin2UpLimit)
//		{
//			pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
//		
//		if(0 == pMold->TransDIn.CoolPin3UpLimit)
//		{
//			pMold->Alarm.CoolPin3NotAtUpPos = !pMold->TransDIn.CoolPin3UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
		
		if(0 == pMold->BlowPin.UpPos )	/* Blowpin not at up position */
		{
			pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->BlowPin2.UpPos )	/* Blowpin2 not at up position */
		{
			pMold->Alarm.BlowPin2NotAtUpPos = !pMold->BlowPin2.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.ExtRobotUpLmit )	
		{
			pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
			pMold->Carriage.Step = 41000;
		}

		if( 0 == pMold->TransDIn.HookPinBwLimit)/*  Hook Pin not at backward limit   */
		{
			pMold->Alarm.HookPinNotAtBwPos = !pMold->TransDIn.HookPinBwLimit;
			pMold->Carriage.Step = 41000;
		}
      
		//		if(1 == gMachineInfo.Auto)
		//		{
		//			if(0 == pMold->Clamp.ClsPos)			      /* Clamp not at close position */
		//			{
		//				pMold->Alarm.ClampNotAtClsDI = !pMold->Clamp.ClsPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//			
		//		}
		//		else
		//		{
		//			
		//		}/* if(1 == gMachineInfo.Auto) */
	}/* if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200) */	
		
	
	/* ------------------------------------------------------------------------------ */
	/* ---------------  carriage output and proportional valve linear --------------- */
	/* ------------------------------------------------------------------------------ */
	if (pCarriageFix->eValveType == VALVE_PROP)
	{
		//		if(pMold->Carriage.v_set < 0.0 )           
		//		{
		//			if(1 == pCarriageFix->Bw.bCloseLoop)   
		//			{
		//				pMold->Carriage.v_add = pCarriageFix->Bw.Kp * (pMold->Carriage.v_set - pMold->Carriage.v_act);
		//				if(pMold->Carriage.v_add > 0.5 * pMold->Carriage.v_set)pMold->Carriage.v_add = 0.5 * pMold->Carriage.v_set;
		//				if(pMold->Carriage.v_add < -0.5 * pMold->Carriage.v_set)pMold->Carriage.v_add = -0.5 * pMold->Carriage.v_set;
		//				pMold->Carriage.ValveLinear.Y = pMold->Carriage.v_set + pMold->Carriage.v_add;
		//			}
		//			else
		//			{
		//				pMold->Carriage.ValveLinear.Y = pMold->Carriage.v_set;  
		//			}
		//		}
		//		else if(pMold->Carriage.v_set > 0.0)      
		//		{
		//			if(1 == pCarriageFix->Fw.bCloseLoop)   
		//			{
		//				pMold->Carriage.v_add = pCarriageFix->Fw.Kp * (pMold->Carriage.v_set - pMold->Carriage.v_act);
		//				if(pMold->Carriage.v_add < 0.5 * pMold->Carriage.v_set)pMold->Carriage.v_add = 0.5 * pMold->Carriage.v_set;
		//				if(pMold->Carriage.v_add > -0.5 * pMold->Carriage.v_set)pMold->Carriage.v_add = -0.5 * pMold->Carriage.v_set;
		//				pMold->Carriage.ValveLinear.Y = pMold->Carriage.v_set + pMold->Carriage.v_add;
		//			}
		//			else
		//			{
		//				pMold->Carriage.ValveLinear.Y = pMold->Carriage.v_set;  
		//			}   
		//		}
		//		else                                      /* carriage standstill */       
		//		{
		//			pMold->Carriage.ValveLinear.Y = 0.0;
		//		}
		//   
		//		/* --- call function --- */
		//		pMold->Carriage.ValveLinear.enable = 1;
		//		if(pCarriageFix->ValveLin[0].X <= 0.1)PropVlaveInit(pCarriageFix);
		//		HydLinear(&pMold->Carriage.ValveLinear);
		//   
		//		if(pMold->Carriage.ValveLinear.status == 0) /* Proportional valve linear ok  */
		//		{
		//			pMold->Carriage.PropValveOk   = 1;
		//			pMold->Alarm.CarriageValveErr = 0;
		//      
		//			if((5000 == pMold->Carriage.Step) || (15000 == pMold->Carriage.Step) || 
		//				(8500 == pMold->Carriage.Step) || (18500 == pMold->Carriage.Step) ||
		//				(pMold->Carriage.Step >= 25000 && pMold->Carriage.Step <= 27000))
		//			{
		//				pMold->Carriage.aoPropValve = -pMold->Carriage.v_set * 327/10; 
		//			}
		//			else
		//			{
		//				pMold->Carriage.aoPropValve = (INT)(-3276.7 * pMold->Carriage.ValveLinear.X);
		//			}
		//		}
		//		else
		//		{
		//			pMold->Carriage.PropValveOk   = 0;
		//			pMold->Alarm.CarriageValveErr = 1;
		//      
		//			pMold->Carriage.aoPropValve = 0;
		//		}
		
		if((pCarriageFix->Fw.VoltageMax - pCarriageFix->Fw.VoltageMin > 5.0) &&
			(pCarriageFix->Bw.VoltageMin - pCarriageFix->Bw.VoltageMax > 5.0))
		{
			pMold->Carriage.PropValveOk   = 1;
			pMold->Alarm.CarriageValveErr = 0;
		}
		else
		{
			pMold->Carriage.PropValveOk   = 0;
			pMold->Alarm.CarriageValveErr = 1;
		}
    
		pCarriageFix->Bw.V_Max = 1000;
		pCarriageFix->Fw.V_Max  = 1000;
   
		if(pMold->Carriage.v_set > pCarriageFix->Fw.V_Max) pMold->Carriage.v_set = pCarriageFix->Fw.V_Max;
		if(pMold->Carriage.v_set < -pCarriageFix->Bw.V_Max) pMold->Carriage.v_set = -pCarriageFix->Bw.V_Max;
		
		if(5000 == pMold->Carriage.Step || 15000 == pMold->Carriage.Step )
		{
			pMold->Carriage.aoPropValve = pMold->Carriage.v_set * 32767/100;   /* 电压输出  0 -- 100 ---> 0.0 -- 10.0v*/
		}
		else
		{
			if(pMold->Carriage.v_set > 0 && pMold->Carriage.v_set <= pCarriageFix->Fw.V_Max)
			{
				pMold->Carriage.aoPropValve = 3276 * (pCarriageFix->Fw.VoltageMin + pMold->Carriage.v_set/pCarriageFix->Fw.V_Max *
					(pCarriageFix->Fw.VoltageMax - pCarriageFix->Fw.VoltageMin));
			}
			else if(pMold->Carriage.v_set < 0 && pMold->Carriage.v_set >= -pCarriageFix->Bw.V_Max)
			{
				pMold->Carriage.aoPropValve = 3276 * (pCarriageFix->Bw.VoltageMin + pMold->Carriage.v_set/pCarriageFix->Bw.V_Max *
					(pCarriageFix->Bw.VoltageMin - pCarriageFix->Bw.VoltageMax));
			}
			else
			{
				pMold->Carriage.aoPropValve = 0;
			}	
		}
	}
	else
	{
		pMold->Alarm.CarriageValveErr = 0;
	}

}/*end of function */


void TransportDriveToggle_Drive(Mold_typ * pMold, Carriage_Fix_typ * pCarriageFix,Carriage_Para_typ * pCarriagePara,SPC_Mold_typ * pSPC,Mold_Para_User_typ * pMoldUser)
{	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pCarriageFix->pMax = 100;
	}
	else
	{
		pCarriageFix->pMax = 100;
	}
	
	if(1 == gMachineOut.OilPump)
	{
		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		if(tErrorDelay < tTask)
		{
			if(pMold == & LMold)
			{
				LMold.HwAlarm.CarrNetworkErr = !LMold.Carriage.NetworkInit;
				LMold.HwAlarm.CarrPowerOn    = !LMold.Carriage.PowerOn;			
			}
			else
			{
				RMold.HwAlarm.CarrNetworkErr = !RMold.Carriage.NetworkInit;
				RMold.HwAlarm.CarrPowerOn    = !RMold.Carriage.PowerOn;	
			}
		}
		
		if((RMold.HwAlarm.CarrNetworkErr ||RMold.HwAlarm.CarrPowerOn)||(LMold.HwAlarm.CarrNetworkErr ||LMold.HwAlarm.CarrPowerOn))
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
			//			LMold.HwAlarm.CarrNetworkErr = 0;
			//			LMold.HwAlarm.CarrPowerOn    = 0;		
		}
		else
		{
			//			RMold.HwAlarm.CarrNetworkErr = 0;
			//			RMold.HwAlarm.CarrPowerOn    = 0;
		}
	}
	
	
	/* ------------- carriage axis alarms ------------- */
	
	pMold->HwAlarm.CarrAxisErr    = pMold->Carriage.Error;

    /*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pCarriageFix->Length < 100.0)pCarriageFix->Length = 600.0;
	if(pCarriageFix->Tolerance < 0.1)pCarriageFix->Tolerance = 0.1;
	pMold->Carriage.Tolerance = pCarriageFix->Tolerance;
    
	if(pCarriageFix->Fw.V_Max < 200.0)pCarriageFix->Fw.V_Max = 1000.0;
	if(pCarriageFix->Bw.V_Max < 200.0)pCarriageFix->Bw.V_Max = 1000.0;
    
	if(pCarriageFix->Fw.VoltageMax > -3.0)pCarriageFix->Fw.VoltageMax = -10.0;
	if(pCarriageFix->Bw.VoltageMax < 3.0 )pCarriageFix->Bw.VoltageMax = 10.0;
    
	pMold->Carriage.S_Fw = pCarriagePara->S_Fw;
	pMold->Carriage.S_FwCreep = pCarriagePara->S_FwCreep;
   
	pMold->Carriage.V_Fw = pCarriagePara->V_Fw * pCarriageFix->Fw.V_Max/100.0;
    
	pMold->Carriage.S_Bw = pCarriagePara->S_Bw;
	pMold->Carriage.S_BwCreep = pCarriagePara->S_BwCreep ;
    
	pMold->Carriage.V_Bw = pCarriagePara->V_Bw * pCarriageFix->Bw.V_Max/100.0;
	
	pMold->Carriage.V_BwSlow = pCarriagePara->V_BwSlow * pCarriageFix->Bw.V_Max/100.0;
	
	pMold->Carriage.V_FwSlow = pCarriagePara->V_FwSlow * pCarriageFix->Fw.V_Max/100.0;
		
	if(0 == pCarriagePara->V_BwReset)pCarriagePara->V_BwReset = 2.0;
	pMold->Carriage.V_BwReset = pCarriagePara->V_BwReset * pCarriageFix->Bw.V_Max/100.0;
   
	if(pCarriageFix->Fw.accel_stop < 100)pCarriageFix->Fw.accel_stop = INSTANCY_STOP_DECEL; 
	if(pCarriageFix->Bw.accel_stop < 100)pCarriageFix->Bw.accel_stop = INSTANCY_STOP_DECEL;
    /*********************************************************************************
   **                            linear calibration status                        **
   *********************************************************************************/
   /* get calibration data  */
	if(pCarriageFix->Max < pCarriageFix->Zero)
	{
		pMold->Carriage.ZeroPos = pCarriageFix->Max;
		pMold->Carriage.MaxPos  = pCarriageFix->Zero;
	}
	else
	{
		pMold->Carriage.MaxPos  = pCarriageFix->Max;
		pMold->Carriage.ZeroPos = pCarriageFix->Zero;
	}
   
	if(pMold->Carriage.ZeroPos != 0                              &&
		pMold->Carriage.MaxPos > (pMold->Carriage.ZeroPos + 20000)  &&
		pMold->Carriage.As     > (pMold->Carriage.ZeroPos - 5000)   &&
		pMold->Carriage.As     < (pMold->Carriage.MaxPos  + 5000))
	{
		pMold->Carriage.CalOK = pMold->Carriage.NetworkInit;
	}
	else
	{
		pMold->Carriage.CalOK = 0;
	}
   
	pCarriageFix->CalibOK = pMold->Carriage.CalOK;
   
	if(1 == pMold->Carriage.CalOK)
	{
		if(pCarriageFix->Max < pCarriageFix->Zero)
		{
			pMold->Carriage.mmAsDrive = 1.0 *(pMold->Carriage.MaxPos - pMold->Carriage.As) * pCarriageFix->Length /
				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
			
			
			//			pMold->Carriage.mmAs = 1.0 *(pMold->Carriage.MaxPos - pMold->Carriage.As) * pCarriageFix->Length /
			//				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
		}
		else
		{
			pMold->Carriage.mmAsDrive = 1.0 *(pMold->Carriage.As - pMold->Carriage.ZeroPos) * pCarriageFix->Length /
				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
		
			//			pMold->Carriage.mmAs = 1.0 *(pMold->Carriage.As - pMold->Carriage.ZeroPos) * pCarriageFix->Length /
			//				(pMold->Carriage.MaxPos - pMold->Carriage.ZeroPos);
		}
	}
	else
	{
		pMold->Carriage.mmAsDrive = pMold->Carriage.mmAsDrive;
		//      pMold->Carriage.mmAs = pMold->Carriage.mmAs;
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->Carriage.Step > 0 && pMold->Carriage.Step < 20000)
			{
				pMold->Carriage.Step = 0;
				pMold->Alarm.CarriageCalErr = 1;
			}
		}
		else if(0 == pMold->Carriage.NetworkInit)
		{
			if(pMold->Carriage.Step > 0 && pMold->Carriage.Step < 20000)
			{
				pMold->Carriage.Step = 0;
			}
		}
	}/* if(1 == pMold->Carriage.CalOK) */
	
	/*  toggle carriage linear  */
	
	pCarriageFix->ToggleLin[0].X = 0.0;
	pCarriageFix->ToggleLin[0].Y = 0.0;
	
	pCarriageFix->ToggleLin[ToggleLinNodeNum-1].X = pCarriageFix->Length;
	pCarriageFix->ToggleLin[ToggleLinNodeNum-1].Y = pCarriageFix->Length;


	
	if(pMold == &LMold)
	{
		TogLinearL.Y = pMold->Carriage.mmAsDrive;
		HydLinear(&TogLinearL);

		if(TogLinearL.status != 0)
		{

			pMold->Alarm.CarriageLinNotOK = 1;
			pMold->Carriage.mmAs = pMold->Carriage.mmAsDrive;
		}
		else
		{
			pMold->Carriage.mmAs = TogLinearL.X;
		}
	}
	else if(pMold == &RMold)
	{
		TogLinearR.Y = pMold->Carriage.mmAsDrive;
		HydLinear(&TogLinearR);
	
		if(TogLinearR.status != 0)
		{
			pMold->Alarm.CarriageLinNotOK = 1;
			pMold->Carriage.mmAs = pMold->Carriage.mmAsDrive;
		}
		else
		{
			pMold->Carriage.mmAs = TogLinearR.X;
		}
	}
	
	/* network and calibration is ok, axis has an error   */ 
	if(0 == pMold->Carriage.AxisReady)	 
	{
		if(pMold->Carriage.Step > 0 && pMold->Carriage.Step < 20000)
		{
			pMold->Carriage.Step = 30000;
		}
	}
   
	/*  relative position  */
	pMold->Carriage.Ss = pMold->Carriage.As;
	pMold->Carriage.mmAsMax = pCarriageFix->Length + 10;
	pMold->Carriage.mmAsMax_HMI = pCarriageFix->Length + 50;  //ipis0619

	/* gAlarm time value    */
	if(!pMold->TimeSet.CarriageFwAlarmTime)pMold->TimeSet.CarriageFwAlarmTime = 1000;   /* 10s  */
	if(!pMold->TimeSet.CarriageBwAlarmTime)pMold->TimeSet.CarriageBwAlarmTime = 1000;   /* 10s  */

	/* Fw or Bw  position flag  */
	if(1 == pMold->Carriage.CalOK)
	{
		pMold->Carriage.FwPos = pMold->TransDIn.CarriageFwLimit 
			&& (pMold->Carriage.mmAs > (pCarriagePara->S_FwCreep - pMold->Carriage.Tolerance))
			&& (pMold->Carriage.mmAs < (pCarriagePara->S_FwCreep + pMold->Carriage.Tolerance));
		pMold->Carriage.BwPos = pMold->TransDIn.CarriageBwLimit 
			&& (pMold->Carriage.mmAs < (pCarriagePara->S_BwCreep + pMold->Carriage.Tolerance))
			&& (pMold->Carriage.mmAs > (pCarriagePara->S_BwCreep - pMold->Carriage.Tolerance));
	}
	else
	{
		pMold->Carriage.FwPos = 0;
		pMold->Carriage.BwPos = 0;
	}

	/* ------------------ current position ------------------ */
	pMold->Carriage.vProfGen.s_act = pMold->Carriage.mmAsDrive;
	pMold->Carriage.vProfGen.v_act = pMold->Carriage.v_act;  // mmAsDrive -> v_act

	
	/* Fw or Bw move step */
	switch ( pMold->Carriage.Step )
	{
		/*------------------ stop all the Output ------------------*/
		case 0:
			pMold->Carriage.p_set = 0;
			pMold->Carriage.v_set = 0;
			
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 0;
			
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ActInfo.CarriageBw  = 0;
			
			pMold->Carriage.Timer.IN = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			break;
		
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;

		/*------------------ Carriage move up -------------------- */
		case 100: /* forward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN = 1;
				pMold->Carriage.Timer.PT = pMold->TimeSet.CarriageFwDelay;
				pMold->Carriage.Step = 200;
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;

		case 200: /*delay ok*/
			pMold->TimeDis.CarriageFwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				pMold->Carriage.Timer.IN = 0;
				//		pMold->TimeDis.CarriageFwDelay = 0;
				pMold->Carriage.Step = 300;
			}
			break;

		case 300:
			if(0 == pMold->Carriage.FwHWSwitch)
			{
				if(pMold == &LMold)
				{
					//					xTogLinearL.Y = pCarriagePara->S_FwCreep;
					//					HydLinear(&xTogLinearL);
					//		
					//					if(0 == xTogLinearL.status)
					//					{
					//						pMold->Carriage.S_FwCreepDrive = xTogLinearL.X;	
					//					}
					pMold->Carriage.S_FwCreepDrive = xTogLinearLTrans(pCarriagePara->S_FwCreep);
				}
				else
				{
					//					xTogLinearR.Y = pCarriagePara->S_FwCreep;
					//					HydLinear(&xTogLinearR);
					//	
					//					if(0 == xTogLinearR.status)
					//					{
					//						pMold->Carriage.S_FwCreepDrive = xTogLinearR.X;	
					//					}
					pMold->Carriage.S_FwCreepDrive = xTogLinearRTrans(pCarriagePara->S_FwCreep);
				}
			
				pMold->Carriage.LimitTimer.IN = 1;
				pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageFwAlarmTime;
	            
				pMold->Carriage.vProfGen.Ts = tTask;
				pMold->Carriage.vProfGen.Tj = pCarriageFix->Fw.Tj;
				pMold->Carriage.vProfGen.Td = pCarriageFix->Fw.Td;
	            
				pMold->Carriage.vProfGen.accel = pCarriageFix->Fw.accel;
				if(pMold->Carriage.vProfGen.accel <= 0.0)pMold->Carriage.vProfGen.accel = 1000;
	            
				pMold->Carriage.vProfGen.decel = pCarriageFix->Fw.accel;
				if(pMold->Carriage.vProfGen.decel <= 0.0)pMold->Carriage.vProfGen.decel = 1000;
	            
				pMold->Carriage.vProfGen.decel_end = pCarriageFix->Fw.decel_end;
				if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
	            
				pMold->Carriage.vProfGen.v_start = 0;	
				if( 0 == pMold->Carriage.BwPos)  // Reset  slow
				{
					pMold->Carriage.ResetSlowFlag = 1;
					pMold->Carriage.vProfGen.decel_end = 5.0 * pMold->Carriage.vProfGen.v_set;
					if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
					
					pMold->Carriage.vProfGen.v_end = 0;  
					pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_FwCreepDrive; 
					pMold->Carriage.vProfGen.init  = 1;
					pMold->Carriage.Step = 370;
				}
				else
				{
					pMold->Carriage.vProfGen.v_end	= 0.0;
					pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_FwCreepDrive;
					pMold->Carriage.vProfGen.init = 1;	
					
					pMold->Carriage.Step = 370;	
				}
			}
			else
			{
				pMold->HwAlarm.CarrFwHWSwitch = 1;
				pMold->Carriage.Step = 40000;
			}
			break;
		
		
		case 370:
			pMold->ActInfo.CarriageFw  = 1;
			pMold->ValveOut.CarriageFw = 1;
			if(1 == pMold->Carriage.ResetSlowFlag)
			{
				pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_BwReset;
			}
			else
			{
				pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_Fw;
			}
	
			
			/* call profile function */
			HydvProfGen(& pMold->Carriage.vProfGen);
			
			pMold->Carriage.p_set = pCarriagePara->P_Fw = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;
			
			pMold->Carriage.actTjFw = pMold->Carriage.vProfGen.Tj;
		
			//	if(pMold->Carriage.vProfGen.s_act <= pMold->Carriage.vProfGen.s_end + pCarriageFix->TolTimePos) //ipis0611
			if(pMold->Carriage.vProfGen.s_act <= pMold->Carriage.vProfGen.s_end + pCarriageFix->Tolerance) //ipis0722							
			{
				if(1 == gMachineInfo.Auto)
				{
					pSPC->tCarriageFw = pMold->Carriage.LimitTimer.ET/100.0;
				}
				pMold->Carriage.v_set = 0; 
				pMold->Carriage.Step = 500;
			}
			
			if(1 == pMold->Carriage.FwHWSwitch)
			{
				pMold->HwAlarm.CarrFwHWSwitch = 1;
				pMold->Carriage.Step = 40000;
			}
			
			break;

		case 500:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCarriageFw = pMold->Carriage.LimitTimer.ET/100.0;
				pSPC->sCarriageFw = pMold->Carriage.mmAs;
			}
			pMold->Carriage.LimitTimer.IN = 0; //ipis0719
			
			pMold->Carriage.ResetSlowFlag = 0;
			pMold->ActInfo.CarriageFw  = 0;
			pMold->ValveOut.CarriageFw = 0;
			pMold->Carriage.v_set = 0;
			pMold->Carriage.Step = 3000;
			break;
		
		case 3000:
			break;
		
		
		
		/*  --------------- Calibration forward --------------- */
		case 5000:
			if(0 == pMold->Carriage.FwHWSwitch)
			{

				pMold->ActInfo.CarriageFw  = 1;
				pMold->ValveOut.CarriageFw = 1;
	
				pMold->Carriage.p_set = pCarriageFix->pCalibZero;
				pMold->Carriage.v_set = -pCarriageFix->vCalibZero;  
					
			}
		
			if(1 == pMold->Carriage.FwHWSwitch)
			{
				pMold->HwAlarm.CarrFwHWSwitch = 1;
				pMold->Carriage.Step = 40000;
			}
			break;
		
		/*--------------- Carriage move backward -----------------*/
		case 10100: /* backward delay*/
			if(1 == pMold->Carriage.Enable)
			{
				pMold->Carriage.Timer.IN  = 1;
				pMold->Carriage.Timer.PT  = pMold->TimeSet.CarriageBwDelay;
				pMold->Carriage.Step = 10200;
			}
			else
			{
				pMold->Carriage.Step = 0;
			}
			break;
         
		case 10200: /*delay ok*/
			pMold->TimeDis.CarriageBwDelay = pMold->Carriage.Timer.ET;
			if (1 == pMold->Carriage.Timer.Q)
			{
				//       pMold->TimeDis.CarriageBwDelay = 0;
				pMold->Carriage.Timer.IN = 0;
				pMold->Carriage.Step = 10300;
			}
			break;
         
		case 10300:
			if(0 == pMold->Carriage.BwHWSwitch)
			{
				if(pMold == &LMold)
				{
					//					xTogLinearL.Y = pCarriagePara->S_BwCreep;
					//					HydLinear(&xTogLinearL);
					//		
					//					if(0 == xTogLinearL.status)
					//					{
					//						pMold->Carriage.S_BwCreepDrive = xTogLinearL.X;	
					//					}
					
					pMold->Carriage.S_BwCreepDrive = xTogLinearLTrans(pCarriagePara->S_BwCreep);
				}
				else
				{
					//					xTogLinearR.Y = pCarriagePara->S_BwCreep;
					//					HydLinear(&xTogLinearR);
					//	
					//					if(0 == xTogLinearR.status)
					//					{
					//						pMold->Carriage.S_BwCreepDrive = xTogLinearR.X;	
					//					}
					
					pMold->Carriage.S_BwCreepDrive = xTogLinearRTrans(pCarriagePara->S_BwCreep);
				}
				if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual)  /* AutoReset  */
				{
					pMold->Carriage.LimitTimer.IN = 0;
					pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageBwAlarmTime;
				}
				else
				{
					pMold->Carriage.LimitTimer.IN = 1;
					pMold->Carriage.LimitTimer.PT = pMold->TimeSet.CarriageBwAlarmTime;
				} 
		
				pMold->Carriage.vProfGen.Ts = tTask;
				pMold->Carriage.vProfGen.Tj = pCarriageFix->Bw.Tj;
				pMold->Carriage.vProfGen.Td = pCarriageFix->Bw.Td;
		   
				pMold->Carriage.vProfGen.accel = pCarriageFix->Bw.accel;
				if(pMold->Carriage.vProfGen.accel <= 0.0)pMold->Carriage.vProfGen.accel = 1000;
		            
				pMold->Carriage.vProfGen.decel = pCarriageFix->Bw.accel;
				if(pMold->Carriage.vProfGen.decel <= 0.0)pMold->Carriage.vProfGen.decel = 1000;
		            
				pMold->Carriage.vProfGen.decel_end = pCarriageFix->Bw.decel_end;
				if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
		         
				pMold->Carriage.vProfGen.v_start = 0;
					
				///	if(1 == gMachineInfo.MachineReset && 1 == gMachineInfo.Manual) //ipis0715
				if(1 == gMachineInfo.MachineReset || 0 == pMold->Carriage.FwPos)
				{
					pMold->Carriage.ResetSlowFlag = 1;
					pMold->Carriage.vProfGen.decel_end = 5.0 * pMold->Carriage.vProfGen.v_set;
					if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
					
					pMold->Carriage.vProfGen.v_end = 0;  
					pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreepDrive; 
					pMold->Carriage.vProfGen.init = 1;
					pMold->Carriage.Step = 10370;
				}
				else if(1 == gMachineInfo.PinCalibration && 1 == gMachineInfo.Calib)
				{
					
					pMold->Carriage.vProfGen.decel_end = 5.0 * pMold->Carriage.vProfGen.v_set;
					if(pMold->Carriage.vProfGen.decel_end <= 0.0)pMold->Carriage.vProfGen.decel_end = 100;
					
					pMold->Carriage.vProfGen.v_end = 0;  
					pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreepDrive; 
					pMold->Carriage.vProfGen.init = 1;

					pMold->Carriage.Step = 10370;
				}
				else
				{		
					pMold->Carriage.vProfGen.v_end	= 0;
					pMold->Carriage.vProfGen.s_end = pMold->Carriage.S_BwCreepDrive;
					pMold->Carriage.vProfGen.init = 1;	
					
					pMold->Carriage.Step = 10370;
				}
			}
			else
			{
				pMold->HwAlarm.CarrBwHWSwitch = 1;
				pMold->Carriage.Step = 41000;
			}
			break;
		
		
		case 10370:
			
			pMold->ActInfo.CarriageBw  = 1;
			pMold->ValveOut.CarriageBw = 1;
			if(1 == gMachineInfo.MachineReset || 1 == pMold->Carriage.ResetSlowFlag)
			{
				pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_BwReset;
			}
			else
			{
				pMold->Carriage.vProfGen.v_set = pMold->Carriage.V_Bw;
			}
			/* call profile function */
			HydvProfGen(& pMold->Carriage.vProfGen);
			
			pMold->Carriage.p_set = pCarriagePara->P_Bw = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vProfGen.v_out;
			
			pMold->Carriage.actTjBw = pMold->Carriage.vProfGen.Tj;
			
			
			/////////////////////////////////////////////////////////////////////////////////////////
			//	 if(pMold->Carriage.vProfGen.s_act >= pMold->Carriage.vProfGen.s_end - pCarriageFix->TolTimePos) //ipis0612
			if(pMold->Carriage.vProfGen.s_act >= pMold->Carriage.vProfGen.s_end - pCarriageFix->Tolerance) //ipis0612
		
			{	
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 10500;
					
			}
			//////////////////////////////////////////////////////////////////////////////////////////
			if(1 == pMold->Carriage.BwHWSwitch)
			{
				pMold->HwAlarm.CarrBwHWSwitch = 1;
				pMold->Carriage.Step = 41000;
			}	
					
			break;
		
		case 10500:
			if(1 == gMachineInfo.Auto)
			{
				pSPC->tCarriageBw = pMold->Carriage.LimitTimer.ET/100.0;
			}
			pMold->Carriage.ResetSlowFlag = 0;
			pMold->Carriage.v_set = 0.0;
			pMold->ActInfo.CarriageBw  = 0;
			pMold->ValveOut.CarriageBw = 0;
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 13000;
			break;
      
		case 13000:
			break;
		
		
		/*  --------------- Calibration backward --------------- */
		case 15000:
			if(0 == pMold->Carriage.BwHWSwitch)
			{
				pMold->ActInfo.CarriageBw  = 1;
				pMold->Carriage.p_set = pCarriageFix->pCalibMax;
				pMold->Carriage.v_set = pCarriageFix->vCalibMax;     
			}
			
			if(1 == pMold->Carriage.BwHWSwitch)
			{
				pMold->HwAlarm.CarrBwHWSwitch = 1;
				pMold->Carriage.Step = 40000;
			}
			break;
		
		
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;

			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
            
			pMold->Carriage.p_old = pMold->Carriage.p_set;

			pMold->Carriage.Step = 21000;
			break;

		case 21000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			if(pMold->Carriage.v_set > 0)
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Bw.accel_stop;
			}
			else
			{
				pMold->Carriage.vRamp.Accel = pCarriageFix->Fw.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 22000;
			break;

		case 22000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = pMold->Carriage.p_old;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 0;
			
				pMold->Carriage.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch  
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
            
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 0;
            
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
            
			pMold->Carriage.vRamp.Ts = tTask;
            
			pMold->Carriage.vRamp.Accel = pCarriageFix->Bw.accel_stop;
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			pMold->Carriage.Step = 31000;
			break;
        
		case 31000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
            
			HydSCurveRamp(&pMold->Carriage.vRamp);
            
			pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
            
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step  = 0;
				
				pMold->Carriage.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch 
			}
			break;
            
		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
            
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
            
			pMold->ValveOut.CarriageFw = 0;
			pMold->ValveOut.CarriageBw = 0;
            
			pMold->Carriage.v_set = 0.0;
			break;
      
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->Carriage.Timer.IN      = 0;
			pMold->Carriage.LimitTimer.IN = 0;
					
			pMold->ActInfo.CarriageFw = 0;
			pMold->ActInfo.CarriageBw = 0;
					
			pMold->Carriage.Step = 42000;
			break;
						
		case 42000:
			pMold->Carriage.vRamp.Internal.Once = 0;
			pMold->Carriage.vRamp.Set = pMold->Carriage.v_set;
					
			pMold->Carriage.vRamp.Ts = tTask;
					
			pMold->Carriage.vRamp.Accel = INSTANCY_STOP_DECEL;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
			pMold->Carriage.Step = 43000;
			break;
					
		case 43000:
			pMold->Carriage.vRamp.Ts = tTask;
			pMold->Carriage.vRamp.Set = 0.0;
					
			/* call profile function */
			HydSCurveRamp(&pMold->Carriage.vRamp);
				
			pMold->Carriage.p_set = 100.0;
			pMold->Carriage.v_set = pMold->Carriage.vRamp.Out;
					
			if(pMold->Carriage.v_set < 0.1 && pMold->Carriage.v_set > -0.1)
			{
				pMold->Carriage.v_set = 0.0;
				pMold->Carriage.Step = 40000;
			}
			break;
	}/* end of switch */
	
	if(pMold->Carriage.Step > 300 && pMold->Carriage.Step < 500)
	{
		pMold->TimeDis.CarriageFwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
	else if(pMold->Carriage.Step > 10300 && pMold->Carriage.Step < 10450)
	{
		pMold->TimeDis.CarriageBwAlarmTime = pMold->Carriage.LimitTimer.ET;
	}
   
	/* ----------------- Carriage Up / Down timeout --------------------  */
	if(1 == pMold->Carriage.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)		/* Carriage move up  */
		{
			pMold->Alarm.CarriageNotAtFwDI = 1;
		}
      
		if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)	/* Carriage move down  */
		{
			pMold->Alarm.CarriageNotAtBwDI = 1;
		}
   
		pMold->Carriage.Step = 41000;
	}/* if(1 == pMold->Carriage.LimitTimer.Q) */

	/* ----------------- Carriage up / down action limit --------------------  */
	if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200)	  /* Carriage move up  */
	{
		if(0 == pMold->OtherMoldBw )				/*  The other mold not at backward  */
		{
			if(pMold == & LMold)
			{
				RMold.Alarm.CarriageNotAtBwDI  = !RMold.Carriage.BwPos;
			}
			else
			{
				LMold.Alarm.CarriageNotAtBwDI  = !LMold.Carriage.BwPos;
			}
         
			pMold->Carriage.LimitTimer.IN = 0;
			pMold->Carriage.Step = 41000;
		}/* if(0 == pMold->OtherMoldBw ) */
   
		if( 0 == pMold->TransDIn.SubMoldClsLimit)
		{
			pMold->Alarm.SubMoldNotAtClsPos  = !pMold->TransDIn.SubMoldClsLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.TopDeflashBwLimit)/*  Top deflash not at backward limit   */
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		
		
		if( 1 == gMachineInfo.Auto)				/*  Auto or AutoReset mode  */
		{
			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
			if(0 == pMold->Clamp.OpnPos  &&  300 == pMold->Carriage.Step )  // while carriage up start ，OpnPos need active. When carriage is moving，the OpnPos no need any more  //owen 
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
		}
		else   // in calib or manual mode the OpnPos will support by  gRClampOpnFlag_Calib (parker special flag)
		{
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Carriage.Step = 41000;
			}
		}
		
		
		
	
		
		//		if( 1 == gMachineInfo.TimeLimit)				/*  Auto or AutoReset mode  */
		//		{
		//			//  if(pMold->Clamp.mmAs  < pMold->Clamp.S_OpnEnd - gMachineFix.MoldR.Clamp.TolerForClampOpn) //ipis0719	
		//			if(0 == pMold->Clamp.OpnPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos  && 0 == pMold->BlowPin.UpPos) /* Clamp not at open position & Blowpin not at up position */
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = !pMold->Clamp.OpnPos;
		//				pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//		}
		
	}/* if(pMold->Carriage.Step < 3000 && pMold->Carriage.Step > 200) */
   
	if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200)  /* Carriage move down  */
	{
		/*  Top deflash not at backward limit   */	// 2018.05.29 Jack: No need this action limit due to top deflash and carriage are at the same structure
		if( 0 == pMold->TransDIn.TopDeflashBwLimit) 
		{
			pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomDeflashBwLimit)/*  Botton deflash not at backward limit   */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = !pMold->TransDIn.BottomDeflashBwLimit;
			pMold->Carriage.Step = 41000;
		}
      
		if(0 == pMold->Robot.Transfer.BwPos)
		{
			pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
			pMold->Carriage.Step = 41000;
		}
			
		if( 0 == pMold->TransDIn.CoolPinUpLimit)
		{
			pMold->Alarm.CoolPinNotAtUpPos  = !pMold->TransDIn.CoolPinUpLimit;
			pMold->Carriage.Step = 41000;
		}
			
//		if(0 == pMold->TransDIn.CoolPin2UpLimit)
//		{
//			pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
//		
//		if(0 == pMold->TransDIn.CoolPin3UpLimit)
//		{
//			pMold->Alarm.CoolPin3NotAtUpPos = !pMold->TransDIn.CoolPin3UpLimit;
//			pMold->Carriage.Step = 41000;
//		}
		
		if(0 == pMold->BlowPin.UpPos )	/* Blow100pin not at up position */
		{
			pMold->Alarm.BlowPinNotAtUpPos = !pMold->BlowPin.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->BlowPin2.UpPos )	/* Blowpin2 not at up position */
		{
			pMold->Alarm.BlowPin2NotAtUpPos = !pMold->BlowPin2.UpPos;
			pMold->Carriage.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.ExtRobotUpLmit )	
		{
			pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
			pMold->Carriage.Step = 41000;
		}

      
		//		if(1 == gMachineInfo.Auto)
		//		{
		//			if(0 == pMold->Clamp.ClsPos)			      /* Clamp not at close position */
		//			{
		//				pMold->Alarm.ClampNotAtClsDI = !pMold->Clamp.ClsPos;
		//				pMold->Carriage.Step = 41000;
		//			}
		//			
		//		}
		//		else
		//		{
		//			
		//		}/* if(1 == gMachineInfo.Auto) */
	}/* if(pMold->Carriage.Step < 13000 && pMold->Carriage.Step > 10200) */	
	
}/*end of function */

