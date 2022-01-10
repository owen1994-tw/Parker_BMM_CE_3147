/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: extrlift
 * File: extrlift.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program extrlift --- upward and downward 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


#define   INSTANCY_STOP_DECEL       3000

#define   INSTANCY_STOP_UP_DECEL    3000
#define   INSTANCY_STOP_DN_DECEL    3000
/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void ExtrLiftTime( Mold_typ * pMold);
void ExtrLiftInverter( Mold_typ * pMold);
void ExtrLiftSwitch( Mold_typ * pMold);
void ExtrLiftDriveToggle_Drive(Mold_typ * pMold, ExtrLift_Fix_typ * pExtrLiftFix, ExtrLift_Para_typ * pExtrLiftPara);
REAL xTogLinearRTrans(REAL pTogLinPos);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);
	
	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
	
	LMold.ExtrLift.Step = 30000;
	RMold.ExtrLift.Step = 30000;
	
	RMold.ExtrLift.vProfGen.Ts    = tTask;
	RMold.ExtrLift.vProfGen.accel = 800.0;
	RMold.ExtrLift.vProfGen.decel = 600.0;
	RMold.ExtrLift.vRamp.Ts       = tTask;
	RMold.ExtrLift.vRamp.Accel    = 1000;

	LMold.ExtrLift.vProfGen.Ts    = tTask;
	LMold.ExtrLift.vProfGen.accel = 800.0;
	LMold.ExtrLift.vProfGen.decel = 600.0;
	LMold.ExtrLift.vRamp.Ts       = tTask;
	LMold.ExtrLift.vRamp.Accel    = 1000;

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
	
	if( 10 > gMachineFix.ExtrLift.ToggleLinNodeNum ) gMachineFix.ExtrLift.ToggleLinNodeNum = 10;
	ToggleLinNodeNum = gMachineFix.ExtrLift.ToggleLinNodeNum;
	/* extrlift toggle linear */

	TogLinearR.enable = 1;
	TogLinearR.nNodes = ToggleLinNodeNum;
	TogLinearR.pNodes = &gMachineFix.ExtrLift.ToggleLin[0];

	xTogLinearR.enable = 1;
	xTogLinearR.nNodes = ToggleLinNodeNum;
	xTogLinearR.pNodes = &xToggleNodeR[0];
	
	memcpy(ToggleROld,gMachineFix.ExtrLift.ToggleLin,sizeof(gMachineFix.ExtrLift.ToggleLin));

	
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(gMachinePara.ExtrLiftLower.accel < 20.0)gMachinePara.ExtrLiftLower.accel = 20.0;

	if(1 == gMachineIn.ExtrLiftInverterFault)
	{
		cntTime = cntTime + 1; 
		if(cntTime >10000)cntTime =1000;
	}
	else
	{
		cntTime = 0;
	}
	
	if(cntTime > 100)
	{
		gAlarm.SwingInvertErr = 1;
	}
	
	/*--- 機構線性，強制線性化 ----*/
	if( ACTUATOR_ELECTRIC ==  gMachineFix.ExtrLift.eActuatorType)
	{
		int ToggleLinIndex;
		if( MECHANISM_LINEAR ==  gMachineFix.ExtrLift.eMechanismType )
		{
			bToggle_Init = 1;
			for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM); ToggleLinIndex++)
			{
				int iSumR = 0;
				iSumR = gMachineFix.ExtrLift.Length / (ToggleLinNodeNum);
				gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].X = iSumR * ToggleLinIndex ;
				gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].Y = iSumR * ToggleLinIndex ;
				
				if( ToggleLinIndex >= ToggleLinNodeNum)
				{
					gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].X = gMachineFix.ExtrLift.Length ;
					gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].Y = gMachineFix.ExtrLift.Length ;
				}
	
			}
		}
		else if( MECHANISM_TOGGLE ==  gMachineFix.ExtrLift.eMechanismType  )
		{
			if(1 == bToggle_Init)
			{
				bToggle_Init = 0;
				
				// clear ToggleLin table
				for(ToggleLinIndex = 1;ToggleLinIndex < (MAX_TOGGLE_LIN_NUM); ToggleLinIndex++)
				{
					
					if(ToggleLinIndex <ToggleLinNodeNum )
					{
						gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].X = 0 ;
						gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].Y = 0 ;			
					}
					else
					{
						gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].X = gMachineFix.ExtrLift.Length ;
						gMachineFix.ExtrLift.ToggleLin[ToggleLinIndex].Y = gMachineFix.ExtrLift.Length ;			
					}
				}	
			
			}	
		}
	}
	
	int i;
	for(i=0;i<MAX_TOGGLE_LIN_NUM;i++)
	{			
		xToggleNodeR[i].X = gMachineFix.ExtrLift.ToggleLin[i].Y;
		xToggleNodeR[i].Y = gMachineFix.ExtrLift.ToggleLin[i].X;
	}
	
	if( 10 > gMachineFix.ExtrLift.ToggleLinNodeNum ) gMachineFix.ExtrLift.ToggleLinNodeNum = 10;
	else if( 30 < gMachineFix.ExtrLift.ToggleLinNodeNum ) gMachineFix.ExtrLift.ToggleLinNodeNum = 30;
	ToggleLinNodeNum = gMachineFix.ExtrLift.ToggleLinNodeNum;
	//TogLinear  update
	
	if(memcmp(ToggleROld,gMachineFix.ExtrLift.ToggleLin,sizeof(gMachineFix.ExtrLift.ToggleLin)) != 0 
		|| oldToggleLinNodeNum != ToggleLinNodeNum)
	{
		TogLinearR.update = 1;
		TogLinearR.nNodes = ToggleLinNodeNum;
		memcpy(ToggleROld,gMachineFix.ExtrLift.ToggleLin,sizeof(gMachineFix.ExtrLift.ToggleLin));
		
		xTogLinearR.update = 1;
		xTogLinearR.nNodes = ToggleLinNodeNum;
		HydLinear(&xTogLinearR);
	}
	
	oldToggleLinNodeNum = ToggleLinNodeNum;

	
	/*  ExtruderLift Up & Dn limit   */
	RMold.TransDIn.ExtruderLiftUpLimit = gMachineIn.ExtruderLiftUpLimit;
	LMold.TransDIn.ExtruderLiftUpLimit = gMachineIn.ExtruderLiftUpLimit;
	
	RMold.TransDIn.ExtruderLiftDnLimit = gMachineIn.ExtruderLiftDnLimit;
	LMold.TransDIn.ExtruderLiftDnLimit = gMachineIn.ExtruderLiftDnLimit;
	

	/*  valve output*/
	gMachineOut.ExtrLiftUp 		= (LMold.ValveOut.ExtrLiftUp| RMold.ValveOut.ExtrLiftUp);
	gMachineOut.ExtrLiftUpCheck = (LMold.ValveOut.ExtrLiftUpCheck| RMold.ValveOut.ExtrLiftUpCheck);  /* 上止回閥*/
	if(1 == gMachineFix.Option.bExtrLiftUpFastValve)
	{
		gMachineOut.ExtrLiftUpFast  = (LMold.ValveOut.ExtrLiftUpFast | RMold.ValveOut.ExtrLiftUpFast);	 /* 上快速閥*/
	}
	gMachineOut.ExtrLiftDn 		= (LMold.ValveOut.ExtrLiftDn | RMold.ValveOut.ExtrLiftDn);
	
	/* velocity  */
	
	if(ACTUATOR_HYD == gMachineFix.ExtrLift.eActuatorType)
	{
		vActHydDT2_R.u = RMold.ExtrLift.mmAs;
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType)
	{
		if(TogLinearR.status != 0)  //LinNotOK
		{
			vActHydDT2_R.u = RMold.ExtrLift.mmAs;
		}
		else
		{
			vActHydDT2_R.u = RMold.ExtrLift.mmAsDrive;
		}
	}


	HydDT2(&vActHydDT2_R);
	RMold.ExtrLift.v_act = vActHydDT2_R.y;
	
	
	/*  run function  */
	if(ACTUATOR_HYD == gMachineFix.ExtrLift.eActuatorType) //Hydraulic
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			ExtrLiftSwitch(&LMold);
		}
		ExtrLiftSwitch(&RMold);
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) //Electric
	{		
		//		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		//		{
		//			ExtrLiftDriveToggle_Drive(&LMold, &gMachineFix.ExtrLift, &gMachinePara.Extrlift);
		//		}
		ExtrLiftDriveToggle_Drive(&RMold, &gMachineFix.ExtrLift, &gMachinePara.Extrlift);
	}


	
	/*------------- ActuatorType--------------- */
	gMachineFix.ExtrLift.eActuatorType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.ExtrLift.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.ExtrLift.eFluxOutputType;
	
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.Extrlift.Position 		= RMold.ExtrLift.mmAs;

	
	
} /* end of _CYCLIC */

void ExtrLiftTime( Mold_typ * pMold)
{

	/* ------------------ Step Machine Control ------------------ */
	switch ( pMold->ExtrLift.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ValveOut.ExtrLiftUp  = 0;
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;

			pMold->ExtrLift.Timer.IN = 0;
			break;

		/*------------------ Extr Lift -------------------- */
		case 100: 	/* ExtrLift delay*/

			if(pMold->TimeSet.ExtrLiftUpDelay != 0)
			{
				pMold->ExtrLift.Timer.IN = 1;
				pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpDelay;
				pMold->ExtrLift.Step 	= 200;
			}
			else
			{
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.ExtrLiftUpDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN     = 0;
				//				pMold->TimeDis.ExtrLiftUpDelay = 0;
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 300:
			if(pMold->TimeSet.ExtrLiftUpHoldTime != 0)
			{
				pMold->ExtrLift.Timer.IN = 1;
				pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpHoldTime;
				pMold->ExtrLift.Step = 400;
			}
			else
			{
				pMold->ExtrLift.Step = 2900;
			}
			break;

		case 400:
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 1;

			pMold->TimeDis.ExtrLiftUpHoldTime = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ValveOut.ExtrLiftUp     = 0;
				pMold->ExtrLift.Timer.IN = 0;
				//				pMold->TimeDis.ExtrLiftUpHoldTime = 0;
				pMold->ExtrLift.Step = 500;
			}
			break;

		case 500:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;/*Test*/
			pMold->ValveOut.ExtrLiftUp       = 0;

			pMold->ExtrLift.Step = 2900;
			break;

		case 2900:
			pMold->ExtrLift.Timer.IN    = 0;

			pMold->TimeDis.ExtrLiftUpDelay = 0;
			pMold->TimeDis.ExtrLiftUpHoldTime = 0;
			pMold->TimeDis.ExtrLiftSlowTime = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;

			pMold->ExtrLift.Step = 3000;
			break;
		case 3000:
			break;

		case 5000:					/*  Manual up */
			
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			
			if((0 == pMold->TransDIn.ExtruderLiftUpLimit) && (0 == pMold->TransDIn.ExtruderLiftDnLimit))
			{
				pMold->ValveOut.ExtrLiftUp       = 0;
			}
			else
			{
				if(1 == pMold->TransDIn.ExtruderLiftUpLimit)
				{
					pMold->ValveOut.ExtrLiftUp       = 0;
				}
				else
				{
					pMold->ValveOut.ExtrLiftUp       = 1;
				}
			}
			break;

		case 15000:					/*  Manual down */
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;

			break;

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ExtrLift.Timer.IN = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;

			pMold->ExtrLift.Step = 0;
			break;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->ExtrLift.Timer.IN = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;

			pMold->ExtrLift.Step = 0;
			break;

		/*--------------- error -----------------*/
		case 40000:
			pMold->ExtrLift.Timer.IN  = 0;
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;

			pMold->ExtrLift.Step = 0;
			break;
	}/* end of switch */
}/*end of function */

void ExtrLiftInverter( Mold_typ * pMold)
{
	if(0 == pMold->TimeSet.ExtrLiftSlowTime)pMold->TimeSet.ExtrLiftSlowTime = 1000;
	
	switch ( pMold->ExtrLift.Step)
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ValveOut.ExtrLiftUp   = 0;
			pMold->ExtrLift.v_set = 0.0;
			
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			
			pMold->ExtrLift.Timer.IN = 0;
			break;

		/*------------------ Extr Lift -------------------- */
		case 100: 	/* ExtrLift delay*/
			if(pMold->TimeSet.ExtrLiftUpDelay != 0)
			{
				pMold->ExtrLift.Timer.IN = 1;
				pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpDelay;
				pMold->ExtrLift.Step 	= 200;
			}
			else
			{
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.ExtrLiftUpDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN     = 0;
				pMold->TimeDis.ExtrLiftUpDelay = 0;
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 300:
			pMold->ExtrLift.Timer.IN = 1;
			pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftSlowTime;
			
			pMold->ExtrLift.vRamp.Internal.Once = 0;
			pMold->ExtrLift.vRamp.Set = 0.0;
		            
			pMold->ExtrLift.vRamp.Ts = 0.002;
			pMold->ExtrLift.vRamp.Accel = gMachinePara.ExtrLiftLower.accel;
			if(pMold->ExtrLift.vRamp.Accel < 10)pMold->ExtrLift.vRamp.Accel = 100;
		            
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
			
			pMold->ExtrLift.Step = 400;
			break;

		case 400:
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 1;
			
			pMold->ExtrLift.vRamp.Set = gMachinePara.ExtrLiftLower.vLowerQuick;
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
				
			pMold->TimeDis.ExtrLiftUpHoldTime = pMold->ExtrLift.Timer.ET;
			if(1 == gMachineIn.ExtrLowerSlowLimit || pMold->TimeDis.ExtrLiftUpHoldTime >= pMold->TimeSet.ExtrLiftUpHoldTime )
			{
				pMold->ExtrLift.Step = 500;
			}
			
					
			pMold->TimeDis.ExtrLiftSlowTime = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ValveOut.ExtrLiftUp      = 0;
				pMold->ExtrLift.Timer.IN = 0;
				pMold->ExtrLift.Step = 40000;
			}
			break;
		
		case 500:
			pMold->ValveOut.ExtrLiftUp = 1;
			
			pMold->ExtrLift.vRamp.Set = gMachinePara.ExtrLiftLower.vLowerSlow;
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
				
			pMold->TimeDis.ExtrLiftSlowTime = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ValveOut.ExtrLiftUp      = 0;
				pMold->ExtrLift.Timer.IN = 0;
				pMold->ExtrLift.Step = 40000;
			}
			
			if(1 == gMachineIn.ExtrLowerLimit)
			{
				pMold->ExtrLift.Timer.IN = 0;
				pMold->ExtrLift.Step = 600;
			}
			break;
		
		case 600:
			pMold->ExtrLift.vRamp.Accel = 1600;
			pMold->ExtrLift.vRamp.Set = 0.0;
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;

			if(pMold->ExtrLift.v_set < 0.1 && pMold->ExtrLift.v_set > -0.1)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ExtrLift.Step = 700;
			}
			break;
			
		case 700:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			pMold->ExtrLift.v_set     = 0.0;
			
			pMold->ExtrLift.Step = 2900;
			break;

		case 2900:
			pMold->ExtrLift.Timer.IN    = 0;
	
			pMold->TimeDis.ExtrLiftUpDelay = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			pMold->ExtrLift.v_set     = 0.0;
				
			pMold->ExtrLift.Step = 3000;
			break;
		case 3000:
			break;

		case 5000:					/*  Manual up */
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 1;
			
			pMold->ExtrLift.v_set = gMachinePara.ExtrLiftLower.vLowerSlow;
			break;

		case 15000:					/*  Manual down */
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			break;

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ExtrLift.Timer.IN = 0;
	
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			
			pMold->ExtrLift.vRamp.Internal.Once = 0;
			pMold->ExtrLift.vRamp.Set = pMold->ExtrLift.v_set;
				            
			pMold->ExtrLift.vRamp.Ts = 0.002;
			pMold->ExtrLift.vRamp.Accel = gMachinePara.ExtrLiftLower.accel;
			if(pMold->ExtrLift.vRamp.Accel < 10)pMold->ExtrLift.vRamp.Accel = 100;
				            
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
	
			pMold->ExtrLift.Step = 21000;
			break;
		
		case 21000:
			pMold->ExtrLift.vRamp.Accel = 2000;
			pMold->ExtrLift.vRamp.Set = 0.0;
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
		
			if(pMold->ExtrLift.v_set < 0.1 && pMold->ExtrLift.v_set > -0.1)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ExtrLift.Step = 0;
			}
			break;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->ExtrLift.Timer.IN = 0;
	
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp   = 0;
			pMold->ExtrLift.v_set = 0.0;
	
			pMold->ExtrLift.Step = 0;
			break;

		/*--------------- error -----------------*/
		case 40000:
			pMold->ExtrLift.Timer.IN  = 0;
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp   = 0;
			pMold->ExtrLift.v_set = 0.0;
	
			pMold->ExtrLift.Step = 0;
			break;
	}/* end of switch */


}/*end of function */

void ExtrLiftSwitch( Mold_typ * pMold)
{
	switch ( pMold->ExtrLift.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp     = 0;
			pMold->ValveOut.ExtrLiftUpCheck    = 0;
			pMold->ValveOut.ExtrLiftUpFast     = 0;
			pMold->ValveOut.ExtrLiftDn 	 = 0;
			
			pMold->ExtrLift.Timer.IN = 0;
			pMold->ExtrLift.LimitTimer.IN = 0; //ipis0219 
			break;

		/*------------------ Extr Lift up-------------------- */
		case 100: 	/* ExtrLift delay*/
			if(pMold->Option.ExtrLift)
			{
				if(pMold->TimeSet.ExtrLiftUpDelay != 0)
				{
					pMold->ExtrLift.Timer.IN = 1;
					pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpDelay;
					pMold->ExtrLift.Step 	= 200;
				}
				else
				{
					pMold->ExtrLift.Step = 300;
				}
			}
			else
			{
				pMold->ExtrLift.Step = 2900;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.ExtrLiftUpDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN     = 0;
				//	pMold->TimeDis.ExtrLiftUpDelay = 0;
			
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 300:		
			if (0 == pMold->TransDIn.ExtruderLiftUpLimit)
			{
				pMold->ExtrLift.LimitTimer.IN = 1; //ipis0219 
				pMold->ExtrLift.LimitTimer.PT = pMold->TimeSet.ExtrLiftUpAlarmTime; //ipis0219
			
				pMold->ExtrLift.Step = 400;
			}
			else
			{
				pMold->ExtrLift.Step = 500;
			}
			break;

		case 400:
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp   = 1;
			pMold->ValveOut.ExtrLiftUpCheck    = 0;
			pMold->ValveOut.ExtrLiftUpFast     = 1;
			pMold->ValveOut.ExtrLiftDn 	 = 0;
 		
			pMold->TimeDis.ExtrLiftUpAlarmTime = pMold->ExtrLift.LimitTimer.ET;
					
			if (1 == pMold->TransDIn.ExtruderLiftUpLimit)
			{
			
				if(pMold->TimeSet.ExtrLiftUpHoldTime != 0)
				{
					pMold->ExtrLift.Timer.IN = 1;
					pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpHoldTime;
					pMold->ExtrLift.Step = 450;
				}
				else
				{
					pMold->ExtrLift.Step = 500;
				}
			}
			break;
		case 450:
			pMold->ValveOut.ExtrLiftUpFast     = 0;
			
			pMold->TimeDis.ExtrLiftUpHoldTime = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ValveOut.ExtrLiftUp      = 0;
				pMold->ExtrLift.Timer.IN = 0;
			
				pMold->ExtrLift.Step = 500;
			}
			
			break;

		case 500:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp      = 0;
			pMold->ValveOut.ExtrLiftUpCheck    = 0;
			pMold->ValveOut.ExtrLiftUpFast     = 0;
			pMold->ValveOut.ExtrLiftDn 	  = 0;
			
			pMold->ExtrLift.LimitTimer.IN = 0;
			if (1 == gMachineInfo.Auto)
			{
				pMold->ExtrLift.Step = 10100;
			}
			else
			{
				pMold->ExtrLift.Step = 2900;
			}
				
			break;
	
		case 2900:
			pMold->ExtrLift.Timer.IN    = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
			
			//			//gExSPC
			//			if(pMold == & LMold)
			//			{
			//				gExSPC[ExIndex].LMold.tExtrLift = pMold->ExtrLift.LimitTimer.ET/100.0;
			//			}
			//			else
			//			{
			//				gExSPC[ExIndex].RMold.tExtrLift = pMold->ExtrLift.LimitTimer.ET/100.0;
			//			}
			
			//	pMold->TimeDis.ExtrLiftUpDelay = 0;
			//	pMold->TimeDis.ExtrLiftUpHoldTime = 0;
			//	pMold->TimeDis.ExtrLiftDnHoldTime = 0;
	
			//	pMold->TimeDis.ExtrLiftUpHoldTime = 0;
			//	pMold->TimeDis.ExtrLiftDnHoldTime = 0;
	
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp      = 0;
			pMold->ValveOut.ExtrLiftUpCheck = 0;
			pMold->ValveOut.ExtrLiftUpFast  = 0;
			pMold->ValveOut.ExtrLiftDn 	  = 0;
		
			pMold->ExtrLift.Step = 3000;
			break;
			
		case 3000:
			break;
	
		case 5000:					/*  Manual up */
			pMold->ActInfo.ExtrLiftUp = 1;
			pMold->ActInfo.ExtrLiftDn = 0;
			
			pMold->ValveOut.ExtrLiftUp    = 1;
			pMold->ValveOut.ExtrLiftUpCheck  = 0;
			pMold->ValveOut.ExtrLiftUpFast   = 0;
			pMold->ValveOut.ExtrLiftDn 	  = 0;			
			
			break;
		
		/*------------------ Extr Lift down -------------------- */
		case 10100:
			if (pMold->TimeSet.ExtrLiftDnDelay != 0)
			{
				pMold->ExtrLift.Timer.IN = 1;
				pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftDnDelay;
				pMold->ExtrLift.Step = 10200;
			}
			else
			{
				pMold->ExtrLift.Step = 10300;
			}
			break;
		case 10200:
			pMold->TimeDis.ExtrLiftDnDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN = 0;
				//	pMold->TimeDis.ExtrLiftDnDelay = 0;
				pMold->ExtrLift.Step = 10300;
			}
			break;
		case 10300:
			if ( 0 == gMachineIn.ExtruderLiftDnLimit)
			{
				pMold->ExtrLift.LimitTimer.IN = 1; 
				pMold->ExtrLift.LimitTimer.PT = pMold->TimeSet.ExtrLiftDnAlarmTime; 
				
				pMold->ExtrLift.Step = 10400;
			}
			else
			{
				pMold->ExtrLift.Step = 10500;
			}
			break;
		case 10400:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 1;
			pMold->ValveOut.ExtrLiftUp  	= 0;
			pMold->ValveOut.ExtrLiftUpCheck = 1;
			pMold->ValveOut.ExtrLiftUpFast  = 0;
			pMold->ValveOut.ExtrLiftDn 	    = 1;
			
			pMold->TimeDis.ExtrLiftDnAlarmTime = pMold->ExtrLift.LimitTimer.ET;
			
			if ( 1 == gMachineIn.ExtruderLiftDnLimit)
			{
				if(pMold->TimeSet.ExtrLiftDnHoldTime != 0)
				{
					pMold->ExtrLift.Timer.IN = 1;
					pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftDnHoldTime;
					pMold->ExtrLift.Step = 10450;
				}
				else
				{
					pMold->ExtrLift.Step = 10500;
				}
			}
			break;
		case 10450:
			pMold->TimeDis.ExtrLiftDnHoldTime = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ValveOut.ExtrLiftUp    = 0;
				pMold->ValveOut.ExtrLiftUpCheck = 0;
				pMold->ValveOut.ExtrLiftUpFast  = 0;
				pMold->ValveOut.ExtrLiftDn 	  = 0;
				pMold->ExtrLift.Timer.IN = 0;
				//				pMold->TimeDis.ExtrLiftDnHoldTime = 0;
				pMold->ExtrLift.Step = 10500;
			}
			break;
		case 10500:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp      = 0;
			pMold->ValveOut.ExtrLiftUpCheck = 0;
			pMold->ValveOut.ExtrLiftUpFast  = 0;
			pMold->ValveOut.ExtrLiftDn 	  = 0;
			pMold->ExtrLift.Timer.IN    = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
			
			pMold->ExtrLift.Step 	  = 13000;
			break;
		
		case 13000:	
			
			break;
		
		case 15000:					/*  Manual down */
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 1;
			pMold->ValveOut.ExtrLiftUp    = 0;
			pMold->ValveOut.ExtrLiftUpCheck  = 1;
			pMold->ValveOut.ExtrLiftUpFast   = 0;
			pMold->ValveOut.ExtrLiftDn 	  = 1;	
	
			break;
	
		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ExtrLift.Timer.IN = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			pMold->ValveOut.ExtrLiftUpFast 	 = 0; 
			
			pMold->ValveOut.ExtrLiftUpCheck = 0;
			pMold->ValveOut.ExtrLiftDn 	  	= 0;
			
		
			pMold->ExtrLift.Step = 0;
			break;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->ExtrLift.Timer.IN = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			pMold->ValveOut.ExtrLiftUpFast 	 = 0; 
			
			pMold->ValveOut.ExtrLiftUpCheck = 0;
			pMold->ValveOut.ExtrLiftDn 	  	= 0;
			
			pMold->ExtrLift.Step = 0;
			break;

		/*--------------- error -----------------*/
		case 40000:
			pMold->ExtrLift.Timer.IN  = 0;
			
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			pMold->ValveOut.ExtrLiftUp       = 0;
			pMold->ValveOut.ExtrLiftUpFast 	 = 0; 
			
			pMold->ValveOut.ExtrLiftUpCheck = 0;
			pMold->ValveOut.ExtrLiftDn 	  	= 0;
			
			pMold->ExtrLift.Step = 0;
			break;
	}/* end of switch */
	
	
	
	/* ----------------- ExtrLift Up / down timeout --------------------  */
	if(1 == pMold->ExtrLift.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->ExtrLift.Step < 500 && pMold->ExtrLift.Step > 200)		/* ExtrLift Up  */
		{
			pMold->Alarm.ExtrLiftNotAtUpPos = 1;
		}
		
		
		if(pMold->ExtrLift.Step < 10500 && pMold->ExtrLift.Step > 10200)		/* ExtrLift Dn  */
		{
			pMold->Alarm.ExtrLiftNotAtDnPos = 1;
		}
      
		pMold->ExtrLift.LimitTimer.IN = 0;
		pMold->ExtrLift.Step = 40000;
		
		if(gMachineInfo.Auto)
		{
			gMachineInfo.ExtrliftAlarmReqSemiAutoFlag = 1;
		}
	}
}

void ExtrLiftDriveToggle_Drive(Mold_typ * pMold, ExtrLift_Fix_typ * pExtrLiftFix, ExtrLift_Para_typ * pExtrLiftPara)
{
	if(pMold == & LMold)
	{
		pExtrLiftFix->pMax = 100;
	}
	else
	{
		pExtrLiftFix->pMax = 100;
	}
	
	
	if(1 == gMachineOut.OilPump)
	{
		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		if(tErrorDelay < tTask)
		{
			if(pMold == & LMold)
			{
				LMold.HwAlarm.ExtrLiftNetworkErr = !LMold.ExtrLift.NetworkInit;
				LMold.HwAlarm.ExtrLiftPowerOn    = !LMold.ExtrLift.PowerOn;			
			}
			else
			{
				RMold.HwAlarm.ExtrLiftNetworkErr = !RMold.ExtrLift.NetworkInit;
				RMold.HwAlarm.ExtrLiftPowerOn    = !RMold.ExtrLift.PowerOn;	
			}
		}
		
		if((RMold.HwAlarm.ExtrLiftNetworkErr ||RMold.HwAlarm.ExtrLiftPowerOn)/*||(LMold.HwAlarm.ExtrLiftNetworkErr ||LMold.HwAlarm.ExtrLiftPowerOn)*/)
		{
			gMachineOut.OilPump = 0;
			gPROCESS_info.oilpumpStop = 1;   //request manual mode
		}
	}
	else
	{
		tErrorDelay = 0.5;
		//		if(pMold == & LMold)
		//		{
		////			LMold.HwAlarm.ExtrLiftNetworkErr = 0;
		////			LMold.HwAlarm.ExtrLiftPowerOn    = 0;		
		//		}
		//		else
		//		{
		////			RMold.HwAlarm.ExtrLiftNetworkErr = 0;
		////			RMold.HwAlarm.ExtrLiftPowerOn    = 0;
		//		}
	}
	
	
	/* ------------- extrLift axis alarms ------------- */
	
	pMold->HwAlarm.ExtrLiftAxisErr    = RMold.ExtrLift.Error;
	
	 /*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pExtrLiftFix->Length < 10.0)pExtrLiftFix->Length = 60.0;
	if(pExtrLiftFix->Tolerance < 0.1)pExtrLiftFix->Tolerance = 2.0;
	pMold->ExtrLift.Tolerance = pExtrLiftFix->Tolerance;
    
	if(pExtrLiftFix->Up.V_Max < 30.0)pExtrLiftFix->Up.V_Max = 1000.0;
	if(pExtrLiftFix->Dn.V_Max < 30.0)pExtrLiftFix->Dn.V_Max = 1000.0;
    
	if(pExtrLiftFix->Up.VoltageMax > -3.0)pExtrLiftFix->Up.VoltageMax = -10.0;
	if(pExtrLiftFix->Dn.VoltageMax < 3.0 )pExtrLiftFix->Dn.VoltageMax = 10.0;
    
	pMold->ExtrLift.S_Up = pExtrLiftPara->S_Up;
	pMold->ExtrLift.V_Up = pExtrLiftPara->V_Up * pExtrLiftFix->Up.V_Max/100.0;
    
	pMold->ExtrLift.S_Dn = pExtrLiftPara->S_Dn;
	pMold->ExtrLift.V_Dn = pExtrLiftPara->V_Dn * pExtrLiftFix->Dn.V_Max/100.0;

	
	if(0 == pExtrLiftPara->V_DnReset)pExtrLiftPara->V_DnReset = 2.0;
	pMold->ExtrLift.V_DnReset = pExtrLiftPara->V_DnReset * pExtrLiftFix->Dn.V_Max/100.0;
   
	if(pExtrLiftFix->Up.accel_stop < 100)pExtrLiftFix->Up.accel_stop = INSTANCY_STOP_UP_DECEL; 
	if(pExtrLiftFix->Dn.accel_stop < 100)pExtrLiftFix->Dn.accel_stop = INSTANCY_STOP_DN_DECEL;
    /*********************************************************************************
   **                            linear calibration status                        **
   *********************************************************************************/
   /* get calibration data  */
	if(pExtrLiftFix->Max < pExtrLiftFix->Zero)
	{
		pMold->ExtrLift.ZeroPos = pExtrLiftFix->Max;
		pMold->ExtrLift.MaxPos  = pExtrLiftFix->Zero;
	}
	else
	{
		pMold->ExtrLift.MaxPos  = pExtrLiftFix->Max;
		pMold->ExtrLift.ZeroPos = pExtrLiftFix->Zero;
	}
  
	
	if(pMold->ExtrLift.ZeroPos != 0            						&&
		pMold->ExtrLift.MaxPos > (pMold->ExtrLift.ZeroPos + 3000)  	&&
		pMold->ExtrLift.As     > (pMold->ExtrLift.ZeroPos - 1000)   &&
		pMold->ExtrLift.As     < (RMold.ExtrLift.MaxPos  + 5000))
	{
		pMold->ExtrLift.CalOK = pMold->ExtrLift.NetworkInit;
	}
	else
	{
		pMold->ExtrLift.CalOK = 0;
	}
   
	pExtrLiftFix->CalibOK = pMold->ExtrLift.CalOK;
   
	if(1 == pMold->ExtrLift.CalOK)
	{
		if(pExtrLiftFix->Max < pExtrLiftFix->Zero)
		{
			pMold->ExtrLift.mmAsDrive = 1.0 *(pMold->ExtrLift.MaxPos - pMold->ExtrLift.As) * pExtrLiftFix->Length /
				(pMold->ExtrLift.MaxPos - pMold->ExtrLift.ZeroPos);
		}
		else
		{
			pMold->ExtrLift.mmAsDrive = 1.0 *(pMold->ExtrLift.As - pMold->ExtrLift.ZeroPos) * pExtrLiftFix->Length /
				(pMold->ExtrLift.MaxPos - pMold->ExtrLift.ZeroPos);
		}
	}
	else
	{
		pMold->ExtrLift.mmAsDrive = pMold->ExtrLift.mmAsDrive;
		//		pMold->ExtrLift.mmAs = pMold->ExtrLift.mmAs;
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->ExtrLift.Step > 0 && pMold->ExtrLift.Step < 20000)
			{
				pMold->ExtrLift.Step = 0;
				//	pMold->ExtrLift.Step = 40000; //ipis081
				pMold->Alarm.ExtrliftCalErr = 1;
			}
		}
		else if(0 == pMold->ExtrLift.NetworkInit)
		{
			if(pMold->ExtrLift.Step > 0 && pMold->ExtrLift.Step < 20000)
			{
				pMold->ExtrLift.Step = 0;
			}
		}
	}/* if(1 == pMold->ExtrLift.CalOK) */
	
	
	
	/*  toggle carriage linear  */
	pExtrLiftFix->ToggleLin[0].X = 0.0;
	pExtrLiftFix->ToggleLin[0].Y = 0.0;
	
	pExtrLiftFix->ToggleLin[ToggleLinNodeNum-1].X = pExtrLiftFix->Length;
	pExtrLiftFix->ToggleLin[ToggleLinNodeNum-1].Y = pExtrLiftFix->Length;



	TogLinearR.Y = pMold->ExtrLift.mmAsDrive;
	HydLinear(&TogLinearR);

	if(TogLinearR.status != 0)
	{
		pMold->Alarm.ExtrliftLinNotOK = 1;
		pMold->ExtrLift.mmAs = pMold->ExtrLift.mmAsDrive;
	}
	else
	{
		pMold->ExtrLift.mmAs = TogLinearR.X;
	}
	
	
	/* network and calibration is ok, axis has an error   */ 
	if(0 == pMold->ExtrLift.AxisReady)	 
	{
		if(pMold->ExtrLift.Step > 0 && pMold->ExtrLift.Step < 20000)
		{
			pMold->ExtrLift.Step = 30000;
		}
	}
   
	/*  relative position  */
	pMold->ExtrLift.Ss = pMold->ExtrLift.As;
	pMold->ExtrLift.mmAsMax = pExtrLiftFix->Length + 10;

	/* gAlarm time value    */
	if(!pMold->TimeSet.ExtrLiftUpAlarmTime)pMold->TimeSet.ExtrLiftUpAlarmTime = 1000;   /* 10s  */
	if(!pMold->TimeSet.ExtrLiftDnAlarmTime)pMold->TimeSet.ExtrLiftDnAlarmTime = 1000;   /* 10s  */

	/* Up or down  position flag  */
	if(1 == pMold->ExtrLift.CalOK)
	{
		pMold->ExtrLift.UpPos = pMold->TransDIn.ExtruderLiftUpLimit 
			&& (pMold->ExtrLift.mmAs > (pExtrLiftPara->S_Up - pMold->ExtrLift.Tolerance))
			&& (pMold->ExtrLift.mmAs < (pExtrLiftPara->S_Up + pMold->ExtrLift.Tolerance));	
		
		pMold->ExtrLift.DnPos = pMold->TransDIn.ExtruderLiftDnLimit 
			&& (pMold->ExtrLift.mmAs < (pExtrLiftPara->S_Dn + pMold->ExtrLift.Tolerance))
			&& (pMold->ExtrLift.mmAs > (pExtrLiftPara->S_Dn - pMold->ExtrLift.Tolerance));

	}
	else
	{
		pMold->ExtrLift.UpPos = 0;
		pMold->ExtrLift.DnPos = 0;
	}
   
	/* ------------------ current position ------------------ */
	pMold->ExtrLift.vProfGen.s_act = pMold->ExtrLift.mmAsDrive;
	pMold->ExtrLift.vProfGen.v_act = pMold->ExtrLift.v_act;

	
	
	/* Up or down move step */
	switch (pMold->ExtrLift.Step )
	{
		/*------------------ stop all the Output ------------------*/
		case 0:
			
			pMold->ActInfo.ExtrLiftUp  = 0;
			pMold->ActInfo.ExtrLiftDn  = 0;
			
			pMold->ExtrLift.p_set = 0;
			pMold->ExtrLift.v_set = 0;
			
			pMold->ExtrLift.Timer.IN = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
			break;

		/*------------------ Extrlift up -------------------- */
		case 100: /* up delay*/
			if(pMold->Option.ExtrLift)
			{
				if(pMold->TimeSet.ExtrLiftUpDelay != 0)
				{
					pMold->ExtrLift.Timer.IN = 1;
					pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftUpDelay;
					pMold->ExtrLift.Step 	= 200;
				}
				else
				{
					pMold->ExtrLift.Step = 300;
				}
			}
			else
			{
				pMold->ExtrLift.Step = 2900;
			}
			break;

		case 200: /*delay ok*/
			pMold->TimeDis.ExtrLiftUpDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN     = 0;
				//	pMold->TimeDis.ExtrLiftUpDelay = 0;
				pMold->ExtrLift.Step = 300;
			}
			break;

		case 300:
			if(0 == pMold->ExtrLift.UpHWSwitch)
			{
			
				/* xTogLinearLTrans */ 
				pMold->ExtrLift.S_UpDrive = xTogLinearRTrans(pExtrLiftPara->S_Up);

				pMold->ExtrLift.LimitTimer.IN = 1;
				pMold->ExtrLift.LimitTimer.PT = pMold->TimeSet.ExtrLiftUpAlarmTime;
			
				pMold->ExtrLift.vProfGen.Ts = tTask;
				pMold->ExtrLift.vProfGen.Tj = pExtrLiftFix->Up.Tj;
				pMold->ExtrLift.vProfGen.Td = pExtrLiftFix->Up.Td;
	            
				pMold->ExtrLift.vProfGen.accel = pExtrLiftFix->Up.accel;
				if(pMold->ExtrLift.vProfGen.accel <= 0.0)pMold->ExtrLift.vProfGen.accel = 1000;
	            
				pMold->ExtrLift.vProfGen.decel = pExtrLiftFix->Up.accel;
				if(pMold->ExtrLift.vProfGen.decel <= 0.0)pMold->ExtrLift.vProfGen.decel = 1000;
	            
				pMold->ExtrLift.vProfGen.decel_end = pExtrLiftFix->Up.decel_end;
				if(pMold->ExtrLift.vProfGen.decel_end <= 0.0)pMold->ExtrLift.vProfGen.decel_end = 100;
	            
				pMold->ExtrLift.vProfGen.v_start = 0;
				pMold->ExtrLift.vProfGen.v_end = 0.0;
				pMold->ExtrLift.vProfGen.s_end = pMold->ExtrLift.S_UpDrive;
				pMold->ExtrLift.vProfGen.init = 1;
				
				pMold->ExtrLift.Step = 400;
			}
			else
			{
				pMold->HwAlarm.ExtrLiftUpHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}
			break;

		case 400:	/* up */
			pMold->ActInfo.ExtrLiftUp  = 1;
              
			pMold->ExtrLift.vProfGen.v_set = pMold->ExtrLift.V_Up;
			HydvProfGen(&pMold->ExtrLift.vProfGen);
            
			pMold->ExtrLift.p_set = pExtrLiftPara->P_Up = 100.0;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vProfGen.v_out;
            
			pMold->ExtrLift.actTjUp = pMold->ExtrLift.vProfGen.Tj;
			
			pMold->TimeDis.ExtrLiftUpAlarmTime = pMold->ExtrLift.LimitTimer.ET;
            
			if(pMold->ExtrLift.vProfGen.s_act >= pMold->ExtrLift.vProfGen.s_end - pExtrLiftFix->Tolerance)
			{
				pMold->ExtrLift.LimitTimer.IN = 0;
				pMold->ExtrLift.p_set = 0;
				pMold->ExtrLift.v_set = 0;
				pMold->ExtrLift.Step = 500;
			}
			if(1 == pMold->ExtrLift.UpHWSwitch)
			{
				pMold->HwAlarm.ExtrLiftUpHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}
			break;

		case 500:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ExtrLift.p_set = 0;
			pMold->ExtrLift.v_set = 0;
			
			if (1==gMachineInfo.Auto)	
			{
				pMold->ExtrLift.Step = 10100;
			}
			else
			{		
				pMold->ExtrLift.Step = 2900;						
			}
          
			break;
		
		case 2900:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			
			pMold->ExtrLift.p_set = 0;
			pMold->ExtrLift.v_set = 0;
			
			pMold->ExtrLift.Timer.IN    = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;

		
			pMold->ExtrLift.Step = 3000;
			break;
			
		case 3000:
			break;
			
		/*  --------------- Calibration Extrlift up --------------- */
		case 5000:
			if(0 == pMold->ExtrLift.UpHWSwitch)
			{
				pMold->ActInfo.ExtrLiftUp  = 1;
			
				pMold->ExtrLift.p_set = pExtrLiftFix->pCalibMax;
				pMold->ExtrLift.v_set = pExtrLiftFix->vCalibMax;

			}
			if(1 == pMold->ExtrLift.UpHWSwitch)
			{
				pMold->HwAlarm.ExtrLiftUpHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}			
			break;
		
		/*--------------- Extrlift down -----------------*/
		case 10100: /* down delay*/
			if (pMold->TimeSet.ExtrLiftDnDelay != 0)
			{
				pMold->ExtrLift.Timer.IN = 1;
				pMold->ExtrLift.Timer.PT = pMold->TimeSet.ExtrLiftDnDelay;
				pMold->ExtrLift.Step = 10200;
			}
			else
			{
				pMold->ExtrLift.Step = 10300;
			}
			break;
         
		case 10200: /*delay ok*/
			pMold->TimeDis.ExtrLiftDnDelay = pMold->ExtrLift.Timer.ET;
			if (1 == pMold->ExtrLift.Timer.Q)
			{
				pMold->ExtrLift.Timer.IN = 0;
				//	pMold->TimeDis.ExtrLiftDnDelay = 0;
				pMold->ExtrLift.Step = 10300;
			}
			break;
         
		case 10300:
			if(0 == pMold->ExtrLift.DnHWSwitch)
			{
				/* xTogLinearLTrans */ 
				pMold->ExtrLift.S_DnDrive = xTogLinearRTrans(pExtrLiftPara->S_Dn);
			
				pMold->ExtrLift.LimitTimer.IN = 1;
				pMold->ExtrLift.LimitTimer.PT = pMold->TimeSet.ExtrLiftDnAlarmTime;
	
				pMold->ExtrLift.vProfGen.Ts = tTask;
				pMold->ExtrLift.vProfGen.Tj = pExtrLiftFix->Dn.Tj;
				pMold->ExtrLift.vProfGen.Td = pExtrLiftFix->Dn.Td;
			   
				pMold->ExtrLift.vProfGen.accel = pExtrLiftFix->Dn.accel;
				if(pMold->ExtrLift.vProfGen.accel <= 0.0)pMold->ExtrLift.vProfGen.accel = 1000;
			            
				pMold->ExtrLift.vProfGen.decel = pExtrLiftFix->Dn.accel;
				if(pMold->ExtrLift.vProfGen.decel <= 0.0)pMold->ExtrLift.vProfGen.decel = 1000;
			            
				pMold->ExtrLift.vProfGen.decel_end = pExtrLiftFix->Dn.decel_end;
				if(pMold->ExtrLift.vProfGen.decel_end <= 0.0)pMold->ExtrLift.vProfGen.decel_end = 100;
			         
				pMold->ExtrLift.vProfGen.v_start = 0; 					
				pMold->ExtrLift.vProfGen.v_end = 0.0;
				pMold->ExtrLift.vProfGen.s_end = pMold->ExtrLift.S_DnDrive;
				pMold->ExtrLift.vProfGen.init = 1;
			
				pMold->ExtrLift.Step = 10400;
			}
			else
			{
				pMold->HwAlarm.ExtrLiftDnHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}
			break;
         
		case 10400:	/* down */
			pMold->ActInfo.ExtrLiftDn  = 1;
              
			pMold->ExtrLift.vProfGen.v_set = pMold->ExtrLift.V_Dn;
			HydvProfGen(&pMold->ExtrLift.vProfGen);
         
			pMold->ExtrLift.p_set = pExtrLiftPara->P_Dn = 100.0;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vProfGen.v_out;
         
			pMold->ExtrLift.actTjDn = pMold->ExtrLift.vProfGen.Tj;
         
			pMold->TimeDis.ExtrLiftDnAlarmTime = pMold->ExtrLift.LimitTimer.ET;
			
			if(pMold->ExtrLift.vProfGen.s_act <= pMold->ExtrLift.vProfGen.s_end + pExtrLiftFix->Tolerance)
			{
				pMold->ExtrLift.LimitTimer.IN = 0;
				pMold->ExtrLift.v_set = 0;
				pMold->ExtrLift.p_set = 0;
				pMold->ExtrLift.Step = 10500;
			}
			if(1 == pMold->ExtrLift.DnHWSwitch)
			{
				pMold->HwAlarm.ExtrLiftDnHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}
			break;
		
		case 10500:
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
			
			pMold->ExtrLift.p_set = 0;
			pMold->ExtrLift.v_set = 0;
			
			pMold->ExtrLift.Timer.IN    = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
			
			pMold->ExtrLift.Step = 13000;
			break;
      
		case 13000:
			break;
 
		/*  --------------- Calibration Extrlift down --------------- */
		case 15000:
			if(0 == pMold->ExtrLift.DnHWSwitch)
			{
				pMold->ActInfo.ExtrLiftDn  = 1;
			
				pMold->ExtrLift.p_set = pExtrLiftFix->pCalibMax;
				pMold->ExtrLift.v_set = -pExtrLiftFix->vCalibZero;
					
			}
			if(1 == pMold->ExtrLift.DnHWSwitch)
			{
				pMold->HwAlarm.ExtrLiftDnHWSwitch = 1;
				pMold->ExtrLift.Step = 40000;
			}			
			break;
		
		/* ----------------------------  manual stop ------------------------- */
		case 20000:
			pMold->ExtrLift.Timer.IN      = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;

			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
            
			pMold->ExtrLift.p_old = pMold->ExtrLift.p_set;

			pMold->ExtrLift.Step = 21000;
			break;

		case 21000:
			pMold->ExtrLift.vRamp.Internal.Once = 0;
			pMold->ExtrLift.vRamp.Set = pMold->ExtrLift.v_set;
            
			pMold->ExtrLift.vRamp.Ts = tTask;
            
			if(pMold->ExtrLift.v_set > 0)
			{
				pMold->ExtrLift.vRamp.Accel = pExtrLiftFix->Dn.accel_stop;
			}
			else
			{
				pMold->ExtrLift.vRamp.Accel = pExtrLiftFix->Up.accel_stop;
			}
            
			/* call profile function */
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
            
			pMold->ExtrLift.p_set = pMold->ExtrLift.p_old;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
            
			pMold->ExtrLift.Step = 22000;
			break;

		case 22000:
			pMold->ExtrLift.vRamp.Ts = tTask;
			pMold->ExtrLift.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
            
			pMold->ExtrLift.p_set = pMold->ExtrLift.p_old;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
            
			if(pMold->ExtrLift.v_set < 0.1 && pMold->ExtrLift.v_set > -0.1)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ExtrLift.Step = 0;
			}
			break;

		/* ----------------------------  Instance stop ------------------------- */
		case 30000:
			pMold->ExtrLift.Timer.IN      = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
            
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
            
			pMold->ExtrLift.vRamp.Internal.Once = 0;
			pMold->ExtrLift.vRamp.Set = pMold->ExtrLift.v_set;
            
			pMold->ExtrLift.vRamp.Ts = tTask;
            
			pMold->ExtrLift.vRamp.Accel = INSTANCY_STOP_DECEL;
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
            
			pMold->ExtrLift.p_set = 100.0;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
            
			pMold->ExtrLift.Step = 31000;
			break;
        
		case 31000:
			pMold->ExtrLift.vRamp.Ts = tTask;
			pMold->ExtrLift.vRamp.Set = 0.0;
            
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
            
			pMold->ExtrLift.p_set = 100.0;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
            
			if(pMold->ExtrLift.v_set < 0.1 && pMold->ExtrLift.v_set > -0.1)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ExtrLift.Step  = 0;
			}
			break;
            
		/* ----------------------------  Error step ------------------------- */
		case 40000:
			pMold->ExtrLift.Timer.IN      = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
		
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
            
			pMold->ExtrLift.v_set = 0.0;
			break;
      
		/* ----------------------------  auto run stop ------------------------- */
		case 41000:        
			pMold->ExtrLift.Timer.IN      = 0;
			pMold->ExtrLift.LimitTimer.IN = 0;
			
			pMold->ActInfo.ExtrLiftUp = 0;
			pMold->ActInfo.ExtrLiftDn = 0;
					
			pMold->ExtrLift.Step = 42000;
			break;
						
		case 42000:
			pMold->ExtrLift.vRamp.Internal.Once = 0;
			pMold->ExtrLift.vRamp.Set = pMold->ExtrLift.v_set;
					
			pMold->ExtrLift.vRamp.Ts = tTask;
					
			pMold->ExtrLift.vRamp.Accel = INSTANCY_STOP_DECEL;
					
			/* call profile function */
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
			pMold->ExtrLift.Step = 43000;
			break;
					
		case 43000:
			pMold->ExtrLift.vRamp.Ts = tTask;
			pMold->ExtrLift.vRamp.Set = 0.0;
					
			/* call profile function */
			HydSCurveRamp(&pMold->ExtrLift.vRamp);
				
			pMold->ExtrLift.p_set = 100.0;
			pMold->ExtrLift.v_set = pMold->ExtrLift.vRamp.Out;
					
			if(pMold->ExtrLift.v_set < 0.1 && pMold->ExtrLift.v_set > -0.1)
			{
				pMold->ExtrLift.v_set = 0.0;
				pMold->ExtrLift.Step = 40000;
			}
			break;
	}/* end of switch */
	
	
	
	/* ----------------- ExtrLift Up / down timeout --------------------  */
	if(1 == pMold->ExtrLift.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->ExtrLift.Step < 500 && pMold->ExtrLift.Step > 200)		/* ExtrLift Up  */
		{
			pMold->Alarm.ExtrLiftNotAtUpPos = 1;
		}
		
		if(pMold->ExtrLift.Step < 10500 && pMold->ExtrLift.Step > 10200)		/* ExtrLift Dn  */
		{
			pMold->Alarm.ExtrLiftNotAtDnPos = 1;
		}
      
		pMold->ExtrLift.LimitTimer.IN = 0;
		pMold->ExtrLift.Step = 40000;
		
		if(gMachineInfo.Auto)
		{
			gMachineInfo.ExtrliftAlarmReqSemiAutoFlag = 1;
		}
	}
}
REAL xTogLinearRTrans(REAL pTogLinPos)
{
	xTogLinearR.Y = pTogLinPos;
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
