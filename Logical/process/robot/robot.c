/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: robot
 * File: robot.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program robot --- robot control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void RobotTransfer( Mold_typ * pMold,Robot_Para_typ * pRobotPara, Robot_Transfer_Fix_typ * pRobotTransferFix, Robot_UpDn_Fix_typ * pRobotUpDnFix );
void RobotTransfer_Drive( Mold_typ * pMold,Robot_Para_typ * pRobotPara, Robot_Transfer_Fix_typ * pRobotTransferFix, Robot_UpDn_Fix_typ * pRobotUpDnFix );

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);
	
	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
	
	LMold.Robot.Step = 30000;
	RMold.Robot.Step = 30000;

} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(LMold.TimeSet.RobotFwAlarmTime  == 0)LMold.TimeSet.RobotFwAlarmTime = 1000;	/*  10s  */
	if(LMold.TimeSet.RobotBwAlarmTime  == 0)LMold.TimeSet.RobotBwAlarmTime = 1000;
	if(LMold.TimeSet.RobotUpAlarmTime  == 0)LMold.TimeSet.RobotUpAlarmTime = 1000;
	if(LMold.TimeSet.RobotDnAlarmTime  == 0)LMold.TimeSet.RobotDnAlarmTime = 1000;

	if(RMold.TimeSet.RobotFwAlarmTime  == 0)RMold.TimeSet.RobotFwAlarmTime = 1000;	/*  10s  */
	if(RMold.TimeSet.RobotBwAlarmTime  == 0)RMold.TimeSet.RobotBwAlarmTime = 1000;
	if(RMold.TimeSet.RobotUpAlarmTime  == 0)RMold.TimeSet.RobotUpAlarmTime = 1000;
	if(RMold.TimeSet.RobotDnAlarmTime  == 0)RMold.TimeSet.RobotDnAlarmTime = 1000;

	LMold.TransDIn.RobotBwLimit = gMachineIn.LMold.RobotBwLimit;
	RMold.TransDIn.RobotBwLimit = gMachineIn.RMold.RobotBwLimit;

	LMold.TransDIn.RobotFwLimit = gMachineIn.LMold.RobotFwLimit;
	RMold.TransDIn.RobotFwLimit = gMachineIn.RMold.RobotFwLimit;
	
	LMold.TransDIn.RobotFwSlowLimit = gMachineIn.LMold.RobotFwSlowLimit;
	RMold.TransDIn.RobotFwSlowLimit = gMachineIn.RMold.RobotFwSlowLimit;

	LMold.TransDIn.RobotUpLimit = gMachineIn.LMold.RobotUpLimit;
	LMold.TransDIn.RobotDnLimit = gMachineIn.LMold.RobotDnLimit;

	RMold.TransDIn.RobotUpLimit = gMachineIn.RMold.RobotUpLimit;
	RMold.TransDIn.RobotDnLimit = gMachineIn.RMold.RobotDnLimit;	
	
	//Ext Robot 
	LMold.TransDIn.ExtRobotUpLmit = gMachineIn.LMold.ExtRobotUpLimit;
	RMold.TransDIn.ExtRobotUpLmit = gMachineIn.RMold.ExtRobotUpLimit;
	
		
	if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.RobotTransfer.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			RobotTransfer_Drive(&LMold,&gMachinePara.MoldL.Robot,&gMachineFix.MoldL.RobotTransfer,&gMachineFix.MoldL.RobotUpDn);
		}
		RobotTransfer_Drive(&RMold,&gMachinePara.MoldR.Robot,&gMachineFix.MoldR.RobotTransfer,&gMachineFix.MoldR.RobotUpDn);
	}
	else
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			RobotTransfer(&LMold,&gMachinePara.MoldL.Robot,&gMachineFix.MoldL.RobotTransfer,&gMachineFix.MoldL.RobotUpDn);
		}
		RobotTransfer(&RMold,&gMachinePara.MoldR.Robot,&gMachineFix.MoldR.RobotTransfer,&gMachineFix.MoldR.RobotUpDn);	
	}
	
	gMachineOut.LMold.RobotFw = LMold.ValveOut.RobotFw;
	gMachineOut.LMold.RobotBw = LMold.ValveOut.RobotBw;

	gMachineOut.RMold.RobotFw = RMold.ValveOut.RobotFw;
	gMachineOut.RMold.RobotBw = RMold.ValveOut.RobotBw;

	gMachineOut.LMold.RobotUp = LMold.ValveOut.RobotUp;
	gMachineOut.LMold.RobotDn = LMold.ValveOut.RobotDn;

	gMachineOut.RMold.RobotUp = RMold.ValveOut.RobotUp;
	gMachineOut.RMold.RobotDn = RMold.ValveOut.RobotDn;
	
	//		gMachineOut.LMold.ProductAuxClamp = LMold.ValveOut.ProductAuxClamp;
	//		gMachineOut.RMold.ProductAuxClamp = RMold.ValveOut.ProductAuxClamp;
	
	
	/*---------------------------------------------------*/
	/*----------------- Robot Transfer-----------------*/
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.RobotTransfer.eActuatorType = gMachineFix.MoldR.RobotTransfer.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.RobotTransfer.eValveType = gMachineFix.MoldR.RobotTransfer.eValveType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.RobotTransfer.ePressueOutputType = gMachineFix.MoldR.RobotTransfer.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.RobotTransfer.eFluxOutputType = gMachineFix.MoldR.RobotTransfer.eFluxOutputType;
	
	
	/*---------------------------------------------------*/
	/*----------------- Robot UpDn-----------------*/
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.RobotUpDn.eActuatorType = gMachineFix.MoldR.RobotUpDn.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.RobotUpDn.eValveType = gMachineFix.MoldR.RobotUpDn.eValveType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.RobotUpDn.ePressueOutputType = gMachineFix.MoldR.RobotUpDn.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.RobotUpDn.eFluxOutputType = gMachineFix.MoldR.RobotUpDn.eFluxOutputType;
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.RobotTransfer.Position 		= RMold.Robot.Transfer.mmAs;
	
	
} /* end of _CYCLIC */

void RobotTransfer( Mold_typ * pMold,Robot_Para_typ * pRobotPara, Robot_Transfer_Fix_typ * pRobotTransferFix, Robot_UpDn_Fix_typ * pRobotUpDnFix )
{
	if(pMold == & LMold)
	{
		pRobotTransferFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pRobotTransferFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	pMold->Robot.Transfer.FwPos = pMold->TransDIn.RobotFwLimit;
	pMold->Robot.Transfer.BwPos = pMold->TransDIn.RobotBwLimit;
	
	pMold->Robot.UpDn.UpPos = pMold->TransDIn.RobotUpLimit;
	pMold->Robot.UpDn.DnPos = pMold->TransDIn.RobotDnLimit;
	
	switch ( pMold->Robot.Step )
	{	
		/*------------------ stop all the output ------------------*/
		case 0:		
			
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/

					pMold->ActInfo.RobotFw	= 0;
					pMold->ActInfo.RobotBw	= 0;
			
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/


					break;
			}
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;

			break;
		
		
		/*--------------------------------------------------- */
		/*------------------ Robot Fw ----------------------- */
		/*--------------------------------------------------- */
		case 100:		/* Fw delay */
			if(1 == pMold->Option.RobotTransfer)
			{
				if(pMold->TimeSet.RobotFwDly != 0)
				{
					pMold->Robot.Timer.IN = 1;
					pMold->Robot.Timer.PT = pMold->TimeSet.RobotFwDly;
					
					if(1 == gMachineInfo.Auto)
					{
						if(13000 == pMold->RobotOpnCls.Step || 0 == pMold->RobotOpnCls.Step)  // Opn too Slow wait, Opn Done
						{
							pMold->Robot.Step = 200;
						}
					}
					else
					{
						pMold->Robot.Step = 200;
					}
				}
				else
				{
					if(1 == gMachineInfo.Auto)
					{
						if(13000 == pMold->RobotOpnCls.Step || 0 == pMold->RobotOpnCls.Step)  // Opn too Slow wait, Opn Done
						{
							pMold->Robot.Step = 300;
						}
					}
					else
					{
						pMold->Robot.Step = 300;
					}
				}
			}
			else
			{
				pMold->Robot.Step = 300;
			}
	
			break;
			
		case 200:  		/* delay ok */
			pMold->TimeDis.RobotFwDly = pMold->Robot.Timer.ET;
			if (1 == pMold->Robot.Timer.Q)
			{
				pMold->Robot.Timer.IN        = 0;
				pMold->Robot.Step = 300;
			}
			break;
		
		case 300:  		/* set time */	
			
			if(1 == pMold->Option.RobotTransfer)
			{
				pMold->Robot.LimitTimer.IN = 1;
				pMold->Robot.LimitTimer.PT = pMold->TimeSet.RobotFwAlarmTime;
			
				pMold->Robot.Step = 400;
			}
			else
			{
				pMold->Robot.Step = 2900; // for no use
			}
			break;
	
		case 400: // Fw quick
			pMold->ActInfo.RobotFw  = 1;
			pMold->ActInfo.RobotBw  = 0;
			
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 1;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = pRobotPara->ParaTransfer.P_Fw;
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_Fw;
				
					if(1 == pMold->TransDIn.RobotFwLimit || 1 == pMold->TransDIn.RobotFwSlowLimit)
					{			
						pMold->Robot.Step = 500;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 1;
					pMold->ValveOut.RobotBw = 0;
				
					if(1 == pMold->TransDIn.RobotFwLimit )
					{			
						pMold->Robot.Step = 1000;
					}

					break;
			}
			
			break;
		
		case 500: // Fw slow
			pMold->ActInfo.RobotFw  = 1;
			pMold->ActInfo.RobotBw  = 0;
			
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 1;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = pRobotPara->ParaTransfer.P_FwSlow;
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_FwSlow;
				
					if(1 == pMold->TransDIn.RobotFwLimit)
					{
						pMold->Robot.Timer.IN = 0;
						pMold->Robot.LimitTimer.IN   = 0;
			
						pMold->Robot.Step = 1000;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
			
			break;
		
		case 1000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotBw	= 0;
					pMold->ActInfo.RobotFw	= 0;
				
					pMold->ValveOut.RobotBw = 0;
					pMold->ValveOut.RobotFw = 0;
				
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
		
					pMold->Robot.Timer.IN = 0;
					pMold->Robot.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						pMold->RobotOpnCls.Step = 100;  //Robot Cls
						pMold->Robot.Step = 2000;
					}
					else
					{
						pMold->Robot.Step = 3000;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotBw	= 0;
					pMold->ActInfo.RobotFw	= 1;
					
					pMold->ValveOut.RobotBw = 0;
					pMold->ValveOut.RobotFw = 1;
				
					pMold->Robot.Timer.IN = 0;
					pMold->Robot.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						pMold->RobotOpnCls.Step = 100;  //Robot Cls
						pMold->Robot.Step = 2000;
					}
					else
					{
						pMold->Robot.Step = 3000;
					}
				
					break;
			}
			break;
		
		case 2000:  // wait  Robot Cls
			
			if( 3000 == pMold->RobotOpnCls.Step) 
			{
				pMold->Robot.Step = 3000;
			}
			
			if( 40000 == pMold->RobotOpnCls.Step) 
			{	
				pMold->Robot.Step = 40000;
			}
			
			break;
		
		case 2900:  // for no use
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotFw	= 0;
				
			pMold->ValveOut.RobotBw = 0;
			pMold->ValveOut.RobotFw = 0;
				
			pMold->Robot.Transfer.p_set = 0;
			pMold->Robot.Transfer.v_set = 0;
		
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 100;  //Robot Cls
			}

			
			pMold->Robot.Step = 3000;
			
			break;
			
		case 3000:
			break;
			
		/* ---------------------- calibration Fw ---------------------- */	
		case 5000:
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotFw	= 1;
			
			pMold->ValveOut.RobotBw = 0;
			pMold->ValveOut.RobotFw = 1;
			
			pMold->Robot.Transfer.p_set = pRobotTransferFix->pCalibMax;
			pMold->Robot.Transfer.v_set = pRobotTransferFix->vCalibMax;
		
			break;
		
		/*--------------------------------------------------- */
		/*------------------ Robot Bw  ---------------------- */
		/*--------------------------------------------------- */
		case 10100:		/* Bw delay */
			if(1 == pMold->Option.RobotTransfer)
			{
				switch (pRobotTransferFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.RobotBwDly != 0)
						{
							pMold->Robot.Timer.IN = 1;
							pMold->Robot.Timer.PT = pMold->TimeSet.RobotBwDly;
		
							pMold->Robot.Step = 10200;
						}
						else
						{
							pMold->Robot.Step = 10300;
						}

						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.RobotBwDly != 0)
						{
							pMold->Robot.Timer.IN = 1;
							pMold->Robot.Timer.PT = pMold->TimeSet.RobotBwDly;
		
							pMold->Robot.Step = 10200;
						}
						else
						{
							pMold->Robot.Step = 10300;
						}
						
						break;
				}
			}
			else
			{
				pMold->Robot.Step = 10300;
			}
	
			break;
			
		case 10200:  		/* delay ok */
			pMold->TimeDis.RobotBwDly = pMold->Robot.Timer.ET;
			if (1 == pMold->Robot.Timer.Q)
			{
				pMold->Robot.Timer.IN  = 0;
				pMold->Robot.Step = 10300;
			}
			break;
		
		case 10300:  		
			if(pMold->Option.RobotTransfer)
			{
				pMold->Robot.LimitTimer.IN = 1; 
				pMold->Robot.LimitTimer.PT = pMold->TimeSet.RobotBwAlarmTime;
				pMold->Robot.Step = 10400;
			}
			else  // for no use check limit
			{	
				if (1 == pMold->Robot.Transfer.BwPos)
				{
					pMold->Robot.Step = 12900; // for no use
				}
				else				
				{
					pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
					pMold->Robot.Step = 40000;     /*  Robot Bw alarm   */
				}
			}
			break;
	
	
		case 10400: // Bw quick
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 1;
			
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 1;
			
					pMold->Robot.Transfer.p_set = pRobotPara->ParaTransfer.P_Bw;
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_Bw;
				
					if(1 == pMold->TransDIn.RobotBwLimit || 1 == pMold->TransDIn.RobotBwSlowLimit)
					{			
						pMold->Robot.Step = 10500;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 1;
				
					if(1 == pMold->TransDIn.RobotBwLimit )
					{			
						pMold->Robot.Step = 11000;
					}

					break;
			}

			
			break;
		
		case 10500: // Bw slow
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 1;
			
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 1;
			
					pMold->Robot.Transfer.p_set = pRobotPara->ParaTransfer.P_BwSlow;
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_BwSlow;
				
					if(1 == pMold->TransDIn.RobotBwLimit)
					{
						pMold->Robot.Timer.IN = 0;
						pMold->Robot.LimitTimer.IN   = 0;
			
						pMold->Robot.Step = 11000;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
			
			break;
		
		case 11000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotBw	= 0;
					pMold->ActInfo.RobotFw	= 0;
				
					pMold->ValveOut.RobotBw = 0;
					pMold->ValveOut.RobotFw = 0;
				
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
		
					pMold->Robot.Timer.IN = 0;
					pMold->Robot.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						pMold->RobotOpnCls.Step = 10100;  //Robot Opn
						pMold->Robot.Step = 12000;
					
						if(0 == gMachineInfo.MachineReset)
						{
							pMold->CoolPin2.Step = 100;         /*  轉二下		 */
							pMold->CoolPin2Blow.Step =100;      /*  轉二下吹冷  */
							pMold->CoolPin3.Step = 100;	       	/*  轉三下		 */
							pMold->CoolPin3Blow.Step =100;    	/*  轉三下吹冷  */
						}
					}
					else
					{
						pMold->Robot.Step = 13000;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotBw	= 0;
					pMold->ActInfo.RobotFw	= 0;
					
					pMold->ValveOut.RobotBw = 0;
					pMold->ValveOut.RobotFw = 0;
				
					pMold->Robot.Timer.IN = 0;
					pMold->Robot.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						pMold->RobotOpnCls.Step = 10100;  //Robot Opn
						pMold->Robot.Step = 12000;
					
						if(0 == gMachineInfo.MachineReset)
						{
							pMold->CoolPin2.Step = 100;         /*  轉二下		 */
							pMold->CoolPin2Blow.Step =100;      /*  轉二下吹冷  */
							pMold->CoolPin3.Step = 100;	       	/*  轉三下		 */
							pMold->CoolPin3Blow.Step =100;    	/*  轉三下吹冷  */
						}
					}
					else
					{
						pMold->Robot.Step = 13000;
					}
				
					break;
			}
			
			break;
		
		case 12000:  // wait Robot Opn
			
			if( 13000 == pMold->RobotOpnCls.Step) 
			{	
				pMold->RobotOpnCls.Step = 0;
				pMold->Robot.Step = 13000;
			}
			
			if( 40000 == pMold->RobotOpnCls.Step) 
			{	
				pMold->Robot.Step = 40000;
			}	

			break;
		
		case 12900:  // for no use
			
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotFw	= 0;
					
			pMold->ValveOut.RobotBw = 0;
			pMold->ValveOut.RobotFw = 0;
			
			pMold->Robot.Transfer.p_set = 0;
			pMold->Robot.Transfer.v_set = 0;
				
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 10100;  //Robot Opn
			}
				
			pMold->Robot.Step = 13000;

			break;
		
			
		case 13000:
			break;
			
		/* ---------------------- calibration Bw ---------------------- */	
		case 15000:
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 1;
			
			pMold->ValveOut.RobotFw = 0;
			pMold->ValveOut.RobotBw = 1;
			
			pMold->Robot.Transfer.p_set = pRobotTransferFix->pCalibZero;
			pMold->Robot.Transfer.v_set = pRobotTransferFix->vCalibZero;
		
			break;
		
      
		/* ------------- Normal stop ------------- */
		case 20000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/

					pMold->ActInfo.RobotFw	= 0;
					pMold->ActInfo.RobotBw	= 0;
			
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/


					break;
			}
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			pMold->Robot.Step = 0;
			
			break;
		
		/* ------------- Instancy stop ------------- */	
		case 30000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/

					pMold->ActInfo.RobotFw	= 0;
					pMold->ActInfo.RobotBw	= 0;
			
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/


					break;
			}
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			pMold->Robot.Step = 0;
			
			break;
		
		/* ------------- Error stop ------------- */	
		case 40000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/

					pMold->ActInfo.RobotFw	= 0;
					pMold->ActInfo.RobotBw	= 0;
			
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/


					break;
			}
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			pMold->Robot.Step = 0;
			
			break;
		
		/* ------------- auto run stop ------------- */	
		case 41000:
			switch (pRobotTransferFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/

					pMold->ActInfo.RobotFw	= 0;
					pMold->ActInfo.RobotBw	= 0;
			
					pMold->ValveOut.RobotFw = 0;
					pMold->ValveOut.RobotBw = 0;
			
					pMold->Robot.Transfer.p_set = 0;
					pMold->Robot.Transfer.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/


					break;
			}
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			pMold->Robot.Step = 40000;
			
			break;
	}/* end of switch */
	
	
	/* ----------------- Robot Fw/Bw alarm time display --------------------  */
	if(pMold->Robot.Step > 300 && pMold->Robot.Step  < 1000)			/* Robot Fw  */
	{
		pMold->TimeDis.RobotFwAlarmTime = pMold->Robot.LimitTimer.ET;
	}
	else if(pMold->Robot.Step > 10300 && pMold->Robot.Step  < 11000)	/* Robot Bw   */
	{
		pMold->TimeDis.RobotBwAlarmTime = pMold->Robot.LimitTimer.ET;
	}
	
	
	/* ----------------- Robot Fw/Bw timeout --------------------  */
	if(1 == pMold->Robot.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Robot.Step < 3000 && pMold->Robot.Step > 200)		/* Robot Fw  */
		{
			pMold->Alarm.RobotNotAtFwPos = 1;
		}
      
		if(pMold->Robot.Step < 13000 && pMold->Robot.Step > 10200)	/* Robot Bw   */
		{
			pMold->Alarm.RobotNotAtBwPos = 1;
		}
   
		pMold->Robot.Step = 41000;
	}/* if(1 == pMold->Robot.LimitTimer.Q) */

	/* ----------------- Robot Fw/Bw action limit --------------------  */
	if(pMold->Robot.Step < 3000 && pMold->Robot.Step > 200)		/* Robot Fw  */
	{
		if(0 == pMold->Carriage.BwPos)
		{
			pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
			pMold->Robot.Step = 40000;
		}
		
		if(0 == gMachineInfo.Auto || (1 == gMachineInfo.Auto && 1 == gMachineFix.Option.bRobotFwAfterTopdeflash))
		{
			if( 0 == pMold->TransDIn.TopDeflashBwLimit )
			{
				pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
				pMold->Robot.Step = 41000;
			}
		}
		
		if( 0 == pMold->TransDIn.RobotOpnLimit)
		{
			pMold->Alarm.RobotNotAtOpnPos = !pMold->TransDIn.RobotOpnLimit;
			pMold->Robot.Step = 41000;
		}
		
	}
	
	if(pMold->Robot.Step < 13000 && pMold->Robot.Step > 10200)	/* Robot Bw   */
	{
		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Robot.Step = 41000;
			}
		}
		else if(1 == gMachineInfo.Auto)
		{
			if(0 == (pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance)) && 0 == pMold->Carriage.FwPos )
			{
				pMold->Alarm.ClampNotAtOpnDI = !(pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance));
				pMold->Alarm.CarriageNotAtFwDI = !pMold->Carriage.FwPos;
				pMold->Robot.Step = 41000;
			}
		}
		
		if( 0 == pMold->TransDIn.CoolPinUpLimit)
		{
			pMold->Alarm.CoolPinNotAtUpPos  = !pMold->TransDIn.CoolPinUpLimit;
			pMold->Robot.Step = 41000;
		}
			
		if(0 == pMold->TransDIn.CoolPin2UpLimit)
		{
			pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
			pMold->Robot.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.CoolPin3UpLimit)
		{
			pMold->Alarm.CoolPin3NotAtUpPos = !pMold->TransDIn.CoolPin3UpLimit;
			pMold->Robot.Step = 41000;
		}
		
	}
	
}/*end of function */

void RobotTransfer_Drive( Mold_typ * pMold,Robot_Para_typ * pRobotPara, Robot_Transfer_Fix_typ * pRobotTransferFix, Robot_UpDn_Fix_typ * pRobotUpDnFix )
{
	if(pMold == & LMold)
	{
		pRobotTransferFix->pMax = 100;
	}
	else
	{
		pRobotTransferFix->pMax = 100;
	}
	
	
	if(1 == gMachineOut.OilPump)
	{
		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		if(tErrorDelay < tTask)
		{
			if(pMold == & LMold)
			{
				LMold.HwAlarm.RobotTransferAxisErr = !LMold.Robot.Transfer.NetworkInit;
				LMold.HwAlarm.RobotTransferPowerOn = !LMold.Robot.Transfer.PowerOn;			
			}
			else
			{
				RMold.HwAlarm.RobotTransferAxisErr = !RMold.Robot.Transfer.NetworkInit;
				RMold.HwAlarm.RobotTransferPowerOn = !RMold.Robot.Transfer.PowerOn;	
			}
		}
		
		if((RMold.HwAlarm.RobotTransferAxisErr ||RMold.HwAlarm.RobotTransferPowerOn)||(LMold.HwAlarm.RobotTransferAxisErr ||LMold.HwAlarm.RobotTransferPowerOn))
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
			//			LMold.HwAlarm.RobotTransferPowerOn = 0;
			//			LMold.HwAlarm.RobotTransferPowerOn = 0;		
		}
		else
		{
			//			RMold.HwAlarm.RobotTransferPowerOn = 0;
			//			RMold.HwAlarm.RobotTransferPowerOn = 0;
		}
	}
	
	
	/* ------------- robot transfer axis alarms ------------- */
	
	pMold->HwAlarm.RobotTransferAxisErr    = pMold->Robot.Transfer.Error;
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pRobotTransferFix->Tolerance < 0.3)pRobotTransferFix->Tolerance = 2.0;
	if(0 == pRobotPara->ParaTransfer.V_BwReset)pRobotPara->ParaTransfer.V_BwReset = 2.0;
	
	if(0 == pMold->Robot.Transfer.Error && 1 == pMold->Robot.Transfer.HomingOk && pRobotTransferFix->sHome != 0 &&
		pMold->Robot.Transfer.mmAs > -15 && pMold->Robot.Transfer.mmAs < pRobotTransferFix->sMax + 15)
	{
		pMold->Robot.Transfer.CalOK = 1;
	}
	else
	{
		pMold->Robot.Transfer.CalOK = 0;
	}
	
	//	if(0 == pMold->Robot.UpDn.Error && 1 == pMold->Robot.UpDn.HomingOk && pRobotUpDnFix->sHome != 0 &&
	//		pMold->Robot.UpDn.mmAs > -15 && pMold->Robot.UpDn.mmAs < pRobotUpDnFix->sMax + 15)
	//	{
	//		pMold->Robot.UpDn.CalOK = 1;
	//	}
	//	else
	//	{
	//		pMold->Robot.UpDn.CalOK = 0;
	//	}
	

	if(1 == pMold->Robot.Transfer.CalOK)
	{
		pMold->Robot.Transfer.BwPos = pMold->TransDIn.RobotBwLimit & (pMold->Robot.Transfer.mmAs <= pRobotPara->ParaTransfer.S_Bw + pRobotTransferFix->Tolerance) &
			(pMold->Robot.Transfer.mmAs >= pRobotPara->ParaTransfer.S_Bw - pRobotTransferFix->Tolerance);
		pMold->Robot.Transfer.FwPos = pMold->TransDIn.RobotFwLimit & (pMold->Robot.Transfer.mmAs >= pRobotPara->ParaTransfer.S_Fw - pRobotTransferFix->Tolerance) &
			(pMold->Robot.Transfer.mmAs <= pRobotPara->ParaTransfer.S_Fw + pRobotTransferFix->Tolerance);
	 	
		pMold->Robot.Transfer.WastePos = (pMold->Robot.Transfer.mmAs > pRobotPara->ParaTransfer.S_WasteFw - pRobotTransferFix->Tolerance) &
			(pMold->Robot.Transfer.mmAs < pRobotPara->ParaTransfer.S_WasteFw + pRobotTransferFix->Tolerance);
				
	}
	else
	{
		pMold->Robot.Transfer.BwPos = 0;
		pMold->Robot.Transfer.FwPos = 0;
		pMold->Robot.Transfer.WastePos = 0;
	}
	 
	//	pRobotUpDnFix->Tolerance = 0.3;
	//
	//	if(1 == pMold->Robot.UpDn.CalOK)
	//	{
	//		pMold->Robot.UpDn.UpPos = pMold->TransDIn.RobotUpLimit & (pMold->Robot.UpDn.mmAs <= pRobotPara->ParaUpDn.sUp + pRobotUpDnFix->Tolerance) &
	//			(pMold->Robot.UpDn.mmAs >= pRobotPara->ParaUpDn.sUp - pRobotUpDnFix->Tolerance);
	//	}
	//	else
	//	{
	//		pMold->Robot.UpDn.UpPos = 0;
	//	}
	 

	if(0 == pMold->Robot.Transfer.CalOK )
	{
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->Robot.Step > 0 && pMold->Robot.Step < 20000)
			{
				pMold->Alarm.RobotTransferCalErr = ! pMold->Robot.Transfer.CalOK;
				pMold->Robot.Step = 20000;
			}
		}
	}
	
	//	if(1 == pMold->Option.RobotUpDn && 0 == pMold->Robot.UpDn.CalOK)
	//	{
	//		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
	//		{
	//			if(pMold->Robot.Step > 0 && pMold->Robot.Step < 20000)
	//			{
	//				pMold->Alarm.RobotUpDnCalErr = ! pMold->Robot.UpDn.CalOK;
	//				pMold->Robot.Step = 20000;
	//			}
	//		}
	//	}
	
	 
	switch(pMold->Robot.Step)
	{
		case 0:
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			pMold->Robot.Transfer.Stop = 0;
	
			pMold->Robot.UpDn.MoveAbsolute = 0;
			pMold->Robot.UpDn.MoveJogPos = 0;
			pMold->Robot.UpDn.MoveJogNeg = 0;
			pMold->Robot.UpDn.Stop = 0;
			
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotUp	= 0;
			pMold->ActInfo.RobotDn	= 0;
		
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			break;
		
		/*------------------------------------------------------- */
		/*---------------------- Robot Fw ----------------------- */
		/*------------------------------------------------------- */
		case 100:		/* fw delay */
			if(1 == pMold->Option.RobotTransfer)
			{
				if(pMold->TimeSet.RobotFwDly != 0)
				{
					pMold->Robot.Timer.IN = 1;
					pMold->Robot.Timer.PT = pMold->TimeSet.RobotFwDly;
					
					if(1 == gMachineInfo.Auto)
					{
						if(13000 == pMold->RobotOpnCls.Step || 0 == pMold->RobotOpnCls.Step)  // Opn too Slow wait, Opn Done
						{
							pMold->Robot.Step = 200;
						}
					}
					else
					{
						pMold->Robot.Step = 200;
					}
				}
				else
				{
					if(1 == gMachineInfo.Auto)
					{
						if(13000 == pMold->RobotOpnCls.Step || 0 == pMold->RobotOpnCls.Step)  // Opn too Slow wait, Opn Done
						{
							pMold->Robot.Step = 300;
						}
					}
					else
					{
						pMold->Robot.Step = 300;
					}
				}
			}
			else
			{
				pMold->Robot.Step = 300; // for no use
			}
			break;
			
		case 200:  		/* delay ok */
			pMold->TimeDis.RobotFwDly = pMold->Robot.Timer.ET;
			if (1 == pMold->Robot.Timer.Q)
			{
				pMold->Robot.Timer.IN        = 0;

				pMold->Robot.Step = 300;
			}
			break;
			
		case 300:		/* time limit  */
			
			if(1 == pMold->Option.RobotTransfer)
			{
				pMold->Robot.Transfer.MoveAbsolute = 1;
				pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_Fw;
				pMold->Robot.Transfer.s_set = pRobotPara->ParaTransfer.S_Fw;
				
				pMold->Robot.LimitTimer.IN = 1;
				pMold->Robot.LimitTimer.PT = pMold->TimeSet.RobotFwAlarmTime;
				pMold->Robot.Step = 400;
			}
			else
			{
				pMold->Robot.Step = 2900; // for no use
			}
			
			if(1 == pMold->Robot.Transfer.FwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferFwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			break;
			
		case 400:
			pMold->ActInfo.RobotFw  = 1;
			pMold->ActInfo.RobotBw  = 0;
			
			if(pMold->Robot.Transfer.mmAs >= pRobotPara->ParaTransfer.S_Fw - pRobotTransferFix->Tolerance) 	/*  機械手進位置到  */
			{
				pMold->Robot.Timer.IN = 0;
				pMold->Robot.LimitTimer.IN = 0;
			
				pMold->Robot.Step = 500;
			}
			
			if(1 == pMold->Robot.Transfer.FwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferFwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			
			break;
			
		case 500:		
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.Stop = 1;
			
			pMold->Robot.Timer.IN   = 0;
			pMold->Robot.LimitTimer.IN = 0;
		
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 100;  //Robot Cls	
			}

			pMold->Robot.Step = 3000;
			
			break;
			
		
		case 2900:	// for no use	
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.Stop = 1;
			
			pMold->Robot.Timer.IN   = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 100;  //Robot Cls
			}

			
			pMold->Robot.Step = 3000;
			
			break;
			
		case 3000:
			break;
			
		/* ---------------------- calibration forward ---------------------- */	
		case 5000:
			pMold->ActInfo.RobotFw  = 1;
			pMold->ActInfo.RobotBw  = 0;
		
			pMold->Robot.Transfer.v_set = pRobotTransferFix->vCalibMax;
			pMold->Robot.Transfer.MoveJogPos = 1;
			
			if(1 == pMold->Robot.Transfer.FwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferFwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			
			break;
		
		/* ---------------------- calibration down ---------------------- */	
		//		case 5500:
		//			pMold->Robot.UpDn.MoveJogPos = 1;
		//			if(1 == pMold->TransDIn.RobotDnLimit)
		//			{
		//				pMold->Robot.Step = 20000;
		//			}
		//			break;
		
		/*------------------------------------------------------- */
		/*---------------------- Robot Bw ----------------------- */
		/*------------------------------------------------------- */
		case 10100:		/* bw delay */
			if(1 == pMold->Option.RobotTransfer)
			{
				if(pMold->TimeSet.RobotBwDly != 0)
				{
					pMold->Robot.Timer.IN = 1;
					pMold->Robot.Timer.PT = pMold->TimeSet.RobotBwDly;
		
					pMold->Robot.Step = 10200;
				}
				else
				{
					pMold->Robot.Step = 10300;
				}
			}
			else
			{
				pMold->Robot.Step = 10300;
			}
			break;
			
		case 10200:  		/* delay ok */
			pMold->TimeDis.RobotBwDly = pMold->Robot.Timer.ET;
			if (1 == pMold->Robot.Timer.Q)
			{
				pMold->Robot.Timer.IN   = 0;
				pMold->Robot.Step = 10300;
			}
			break;
			
		case 10300:		/* time limit  */
			
			if(1 == pMold->Option.RobotTransfer)
			{
				pMold->Robot.Transfer.MoveAbsolute = 1;
			
				if(1 == gMachineInfo.MachineReset )
				{
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_BwReset;
				}
				else
				{
					pMold->Robot.Transfer.v_set = pRobotPara->ParaTransfer.V_Bw;
				}
				pMold->Robot.Transfer.s_set = pRobotPara->ParaTransfer.S_Bw;
				pMold->Robot.LimitTimer.IN = 1;
				pMold->Robot.LimitTimer.PT = pMold->TimeSet.RobotBwAlarmTime;
				pMold->Robot.Step = 10400;
			}
			else   // for no use check limit
			{
				if (1 == pMold->Robot.Transfer.BwPos)
				{
					pMold->Robot.Step = 12900; // for no use
				}
				else				
				{
					pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
					pMold->Robot.Step = 40000;           
				}
			}
			
			if(1 == pMold->Robot.Transfer.BwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferBwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			
			break;
			
		case 10400:
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 1;
			
			if(pMold->Robot.Transfer.mmAs <= pRobotPara->ParaTransfer.S_Bw + pRobotTransferFix->Tolerance) 	/*  機械手退位置到  */
			{
				pMold->Robot.Timer.IN = 0;
				pMold->Robot.LimitTimer.IN = 0;
			
				pMold->Robot.Step = 10500;
			}
			
			if(1 == pMold->Robot.Transfer.BwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferBwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			
			
			break;
			
		case 10500:		
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
	
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.Stop = 1;
			
			pMold->Robot.Timer.IN   = 0;
			pMold->Robot.LimitTimer.IN = 0;

			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 10100;  //Robot Opn
			
				if(0 == gMachineInfo.MachineReset)
				{
					pMold->CoolPin2.Step = 100;         /*  轉二下		 */
					pMold->CoolPin2Blow.Step =100;      /*  轉二下吹冷  */
					pMold->CoolPin3.Step = 100;	       	/*  轉三下		 */
					pMold->CoolPin3Blow.Step =100;    	/*  轉三下吹冷  */
				}
			}
			pMold->Robot.Step = 13000;
			
			break;
		
		
		case 12900:  // for no use
			
			
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
	
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.Stop = 1;
			
			pMold->Robot.Timer.IN   = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->RobotOpnCls.Step = 10100;  //Robot Opn
			}

			
			pMold->Robot.Step = 13000;
				
			break;


		case 13000:

			break;
			
		/* ---------------------- calibration backward ---------------------- */	
		case 15000:
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 1;
			
			pMold->Robot.Transfer.v_set = pRobotTransferFix->vCalibZero;
			pMold->Robot.Transfer.MoveJogNeg = 1;
			
			
			if(1 == pMold->Robot.Transfer.BwHWSwitch)
			{
				pMold->HwAlarm.RobotTransferBwHWSwitch = 1;
				pMold->Robot.Step = 40000;
			}
			break;
		
		
		/* ---------------------- calibration upward ---------------------- */	
		//		case 15500:
		//			pMold->Robot.UpDn.MoveJogNeg = 1;
		//			break;

		/* ------------- Normal stop ------------- */
		case 20000:
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 0;
			
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			pMold->Robot.Transfer.Stop = 1;
	
			//			pMold->Robot.UpDn.MoveAbsolute = 0;
			//			pMold->Robot.UpDn.MoveJogPos = 0;
			//			pMold->Robot.UpDn.MoveJogNeg = 0;
			//			pMold->Robot.UpDn.Stop = 1;
			
		
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotUp	= 0;
			pMold->ActInfo.RobotDn	= 0;
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			pMold->Robot.Step = 0;
			break;
		
		/* ------------- Calib stop ------------- */
		case 25000:
			pMold->ActInfo.RobotFw  = 0;
			pMold->ActInfo.RobotBw  = 0;
			
			//			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			//			pMold->Robot.Transfer.Stop = 1;
	
			//			pMold->Robot.UpDn.MoveAbsolute = 0;
			//			pMold->Robot.UpDn.MoveJogPos = 0;
			//			pMold->Robot.UpDn.MoveJogNeg = 0;
			//			pMold->Robot.UpDn.Stop = 1;
			
		
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotUp	= 0;
			pMold->ActInfo.RobotDn	= 0;
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			pMold->Robot.Step = 0;
			break;
		
		/* ------------- Instancy stop ------------- */	
		case 30000:
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			pMold->Robot.Transfer.Stop = 1;
	
			//			pMold->Robot.UpDn.MoveAbsolute = 0;
			//			pMold->Robot.UpDn.MoveJogPos = 0;
			//			pMold->Robot.UpDn.MoveJogNeg = 0;
			//			pMold->Robot.UpDn.Stop = 1;

			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			//			pMold->ActInfo.RobotUp	= 0;
			//			pMold->ActInfo.RobotDn	= 0;
			
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			pMold->Robot.Step = 0;
			break;
		
		/* ------------- Error stop ------------- */	
		case 40000:
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			pMold->Robot.Transfer.Stop = 1;
	
			//			pMold->Robot.UpDn.MoveAbsolute = 0;
			//			pMold->Robot.UpDn.MoveJogPos = 0;
			//			pMold->Robot.UpDn.MoveJogNeg = 0;
			//			pMold->Robot.UpDn.Stop = 1;
			
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			pMold->ActInfo.RobotUp	= 0;
			pMold->ActInfo.RobotDn	= 0;
		
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			//			gImply.RobotReset = 1;
			break;
		
		
		/* ------------- auto run stop ------------- */	
		case 41000:
			pMold->Robot.Transfer.MoveAbsolute = 0;
			pMold->Robot.Transfer.MoveJogPos = 0;
			pMold->Robot.Transfer.MoveJogNeg = 0;
			pMold->Robot.Transfer.Stop = 1;
	
			//			pMold->Robot.UpDn.MoveAbsolute = 0;
			//			pMold->Robot.UpDn.MoveJogPos = 0;
			//			pMold->Robot.UpDn.MoveJogNeg = 0;
			//			pMold->Robot.UpDn.Stop = 1;
			
			pMold->ActInfo.RobotFw	= 0;
			pMold->ActInfo.RobotBw	= 0;
			//			pMold->ActInfo.RobotUp	= 0;
			//			pMold->ActInfo.RobotDn	= 0;
		
			pMold->Robot.Timer.IN = 0;
			pMold->Robot.LimitTimer.IN = 0;
			
			pMold->Robot.Step = 40000;
			break;
	}/* switch(pMold->Robot.Step) */
	
	/* ----------------- Robot Fw/Bw alarm time display --------------------  */
	if(pMold->Robot.Step > 300 && pMold->Robot.Step  < 1000)			/* Robot Fw  */
	{
		pMold->TimeDis.RobotFwAlarmTime = pMold->Robot.LimitTimer.ET;
	}
	else if(pMold->Robot.Step > 10300 && pMold->Robot.Step  < 11000)	/* Robot Bw   */
	{
		pMold->TimeDis.RobotBwAlarmTime = pMold->Robot.LimitTimer.ET;
	}
	
	
	/* ----------------- Robot Fw/Bw timeout --------------------  */
	if(1 == pMold->Robot.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->Robot.Step < 3000 && pMold->Robot.Step > 200)		/* Robot Fw  */
		{
			pMold->Alarm.RobotNotAtFwPos = 1;
		}
      
		if(pMold->Robot.Step < 13000 && pMold->Robot.Step > 10200)	/* Robot Bw   */
		{
			pMold->Alarm.RobotNotAtBwPos = 1;
		}
   
		pMold->Robot.Step = 41000;
	}/* if(1 == pMold->Robot.LimitTimer.Q) */

	/* ----------------- Robot Fw/Bw action limit --------------------  */
	if(pMold->Robot.Step < 3000 && pMold->Robot.Step > 200)		/* Robot Fw  */
	{
		if(0 == pMold->Carriage.BwPos)
		{
			pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
			pMold->Robot.Step = 40000;
		}
		
		if(0 == gMachineInfo.Auto || (1 == gMachineInfo.Auto && 1 == gMachineFix.Option.bRobotFwAfterTopdeflash))
		{
			if( 0 == pMold->TransDIn.TopDeflashBwLimit )
			{
				pMold->Alarm.TopDeflashNotAtBwPos = !pMold->TransDIn.TopDeflashBwLimit;
				pMold->Robot.Step = 41000;
			}
		}
		
		if( 0 == pMold->TransDIn.RobotOpnLimit)
		{
			pMold->Alarm.RobotNotAtOpnPos = !pMold->TransDIn.RobotOpnLimit;
			pMold->Robot.Step = 41000;
		}
		
	}
	
	if(pMold->Robot.Step < 13000 && pMold->Robot.Step > 10200)	/* Robot Bw   */
	{
		if(0 == gMachineInfo.Auto)
		{
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Robot.Step = 41000;
			}
		}
		else if(1 == gMachineInfo.Auto)
		{
			//			if(0 == (pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance)) && 0 == pMold->Carriage.FwPos )
			//			{
			//				pMold->Alarm.ClampNotAtOpnDI = !(pMold->Clamp.mmAs > (pMold->Clamp.S_OpnEnd - pMold->Clamp.Tolerance));
			//				pMold->Alarm.CarriageNotAtFwDI = !pMold->Carriage.FwPos;
			//				pMold->Robot.Step = 41000;
			//			}
			
			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.FwPos )
			{
				pMold->Alarm.ClampNotAtOpnDI = !pMold->Clamp.OpnPos;
				pMold->Alarm.CarriageNotAtFwDI = !pMold->Carriage.FwPos;
				pMold->Robot.Step = 41000;
			}
		}
		
			
		if(0 == pMold->TransDIn.CoolPin2UpLimit)
		{
			pMold->Alarm.CoolPin2NotAtUpPos = !pMold->TransDIn.CoolPin2UpLimit;
			pMold->Robot.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.CoolPin3UpLimit)
		{
			pMold->Alarm.CoolPin3NotAtUpPos = !pMold->TransDIn.CoolPin3UpLimit;
			pMold->Robot.Step = 41000;
		}
		
	}
	
}/* void Robot(Mold_typ * pMold) */




