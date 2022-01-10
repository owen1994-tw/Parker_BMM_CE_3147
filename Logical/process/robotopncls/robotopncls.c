/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: robotopncls
 * File: robotopncls.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program coolpin --- cooling pin control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES 
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void RobotOpnCls( Mold_typ * pMold,Robot_OpnCls_Para_typ * pRobotOpnClsPara,Robot_OpnCls_Fix_typ * pRobotOpnClsFix);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.RobotOpnCls.Step = 0;
	RMold.RobotOpnCls.Step = 0;
} /* end of _INIT */

/*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{

	if(LMold.TimeSet.RobotOpnAlarmTime == 0)LMold.TimeSet.RobotOpnAlarmTime= 1000;	/*  10s  */
	if(RMold.TimeSet.RobotOpnAlarmTime == 0)RMold.TimeSet.RobotOpnAlarmTime= 1000;	/*  10s  */
	
	if(LMold.TimeSet.RobotClsAlarmTime == 0)LMold.TimeSet.RobotClsAlarmTime= 1000;	/*  10s  */
	if(RMold.TimeSet.RobotClsAlarmTime == 0)RMold.TimeSet.RobotClsAlarmTime= 1000;	/*  10s  */
	
	LMold.TransDIn.RobotOpnLimit = ((0 == gMachineFix.MoldR.RobotOpnCls.bEnableRobotTwoOpnLimit && gMachineIn.LMold.RobotOpnLimit) || 
		( 1 == gMachineFix.MoldR.RobotOpnCls.bEnableRobotTwoOpnLimit &&  gMachineIn.LMold.RobotOpnLimitR && gMachineIn.LMold.RobotOpnLimitL));
	RMold.TransDIn.RobotOpnLimit = ((0 == gMachineFix.MoldR.RobotOpnCls.bEnableRobotTwoOpnLimit && gMachineIn.RMold.RobotOpnLimit) || 
		( 1 == gMachineFix.MoldR.RobotOpnCls.bEnableRobotTwoOpnLimit &&  gMachineIn.RMold.RobotOpnLimitR && gMachineIn.RMold.RobotOpnLimitL));

	//	LMold.TransDIn.RobotOpnLimitL = gMachineIn.LMold.RobotOpnLimitL;
	//	RMold.TransDIn.RobotOpnLimitL = gMachineIn.RMold.RobotOpnLimitL;
	//
	//	LMold.TransDIn.RobotOpnLimitR = gMachineIn.LMold.RobotOpnLimitR;
	//	RMold.TransDIn.RobotOpnLimitR = gMachineIn.RMold.RobotOpnLimitR;
	
	LMold.TransDIn.RobotClsLimit = gMachineIn.LMold.RobotClsLimit;
	RMold.TransDIn.RobotClsLimit = gMachineIn.RMold.RobotClsLimit;
	
	
	if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
	{
		RobotOpnCls(&LMold,&gMachinePara.MoldL.RobotOpnCls,&gMachineFix.MoldL.RobotOpnCls);		
	}
	RobotOpnCls(&RMold,&gMachinePara.MoldR.RobotOpnCls,&gMachineFix.MoldR.RobotOpnCls);

	gMachineOut.LMold.RobotCls = LMold.ValveOut.RobotCls;
	gMachineOut.RMold.RobotCls = RMold.ValveOut.RobotCls;
	
	gMachineOut.LMold.RobotOpn = LMold.ValveOut.RobotOpn;
	gMachineOut.RMold.RobotOpn = RMold.ValveOut.RobotOpn;
	
	
	/*-----------------------------------------------*/
	/*----------------- Robot OpnCls-----------------*/
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.RobotOpnCls.eActuatorType = gMachineFix.MoldR.RobotOpnCls.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.RobotOpnCls.eValveType = gMachineFix.MoldR.RobotOpnCls.eValveType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.RobotOpnCls.ePressueOutputType = gMachineFix.MoldR.RobotOpnCls.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.RobotOpnCls.eFluxOutputType = gMachineFix.MoldR.RobotOpnCls.eFluxOutputType;
	
	/*------------ bEnableRobotClsLimit--------------- */
	gMachineFix.MoldL.RobotOpnCls.bEnableRobotClsLimit = gMachineFix.MoldR.RobotOpnCls.bEnableRobotClsLimit;

} /* end of _CYCLIC */

void RobotOpnCls( Mold_typ * pMold,Robot_OpnCls_Para_typ * pRobotOpnClsPara,Robot_OpnCls_Fix_typ * pRobotOpnClsFix)
{

	switch ( pMold->RobotOpnCls.Step )
	{	
		/*------------------ stop all the output ------------------*/
		case 0:			

			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN   = 0;
			
			break;
		/*--------------------------------------------------- */
		/*------------------ Robot Close  ------------------- */
		/*--------------------------------------------------- */
		case 100:		/* cls delay */
			if(1 == pMold->Option.RobotOpnCls)
			{
				if(0 == pMold->RobotOpnCls.RobotFwTimeOutFlag)
				{
					if(pMold->TimeSet.RobotClsDly != 0)
					{
						pMold->RobotOpnCls.Timer.IN = 1;
						pMold->RobotOpnCls.Timer.PT = pMold->TimeSet.RobotClsDly;
			
						pMold->RobotOpnCls.Step = 200;
					}
					else
					{
						pMold->RobotOpnCls.Step = 300;
					}
				}
				else
				{
					pMold->RobotOpnCls.RobotFwTimeOutFlag = 0;
					pMold->RobotOpnCls.Step = 300;
				}

			}
			else
			{
				pMold->RobotOpnCls.Step = 300; // for no use
			}
	
			break;
			
		case 200:  		/* delay ok */
			pMold->TimeDis.RobotClsDly = pMold->RobotOpnCls.Timer.ET;
			if (1 == pMold->RobotOpnCls.Timer.Q)
			{
				pMold->RobotOpnCls.Timer.IN        = 0;
				pMold->RobotOpnCls.Step = 300;
			}
			break;
		
		case 300:  		/* set time */	
			if( 1 == pMold->Option.RobotOpnCls)
			{					
				pMold->RobotOpnCls.Timer.PT = pMold->TimeSet.RobotClsTime;
				pMold->RobotOpnCls.Timer.IN = 1;
			
				pMold->RobotOpnCls.LimitTimer.PT = pMold->TimeSet.RobotClsAlarmTime;
				pMold->RobotOpnCls.LimitTimer.IN = 1;
				
				pMold->RobotOpnCls.Step = 400;
			}
			else
			{
				pMold->RobotOpnCls.Step = 2900; // for no use
			}

			break;
	
		case 400: 
			
			pMold->ActInfo.RobotOpn	= 0;
			pMold->ActInfo.RobotCls	= 1;
			
			switch (pRobotOpnClsFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotOpn = 0;
					pMold->ValveOut.RobotCls = 1;
			
					pMold->RobotOpnCls.p_set = pRobotOpnClsPara->P_Cls;
					pMold->RobotOpnCls.v_set = pRobotOpnClsPara->V_Cls;
				
					if(1 == pMold->RobotOpnCls.Timer.Q)
					{
						pMold->ValveOut.RobotOpn = 0;
						pMold->ValveOut.RobotCls = 0;
					
						pMold->RobotOpnCls.p_set = 0;
						pMold->RobotOpnCls.v_set = 0;
			
						pMold->RobotOpnCls.Timer.IN = 0;
						pMold->RobotOpnCls.LimitTimer.IN   = 0;
			
						pMold->RobotOpnCls.Step = 500;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ValveOut.RobotOpn = 0;
					pMold->ValveOut.RobotCls = 1;
				
					if(0 == pRobotOpnClsFix->bEnableRobotClsLimit ||  1 == pMold->TransDIn.RobotClsLimit )
					{
						pMold->RobotOpnCls.Timer.IN = 0;
						pMold->RobotOpnCls.LimitTimer.IN   = 0;
			
						pMold->RobotOpnCls.Step = 500;
					}

					break;
			}
			
			break;
		
		case 500:
			switch (pRobotOpnClsFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotOpn	= 0;
					pMold->ActInfo.RobotCls	= 0;
				
					pMold->ValveOut.RobotOpn = 0;
					pMold->ValveOut.RobotCls = 0;
				
					pMold->RobotOpnCls.p_set = 0;
					pMold->RobotOpnCls.v_set = 0;
		
					pMold->RobotOpnCls.Timer.IN = 0;
					pMold->RobotOpnCls.LimitTimer.IN   = 0;
					pMold->RobotOpnCls.Step = 3000;

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ActInfo.RobotOpn	= 0;
					pMold->ActInfo.RobotCls	= 1;
					
					pMold->ValveOut.RobotOpn = 0;
					pMold->ValveOut.RobotCls = 1;
				
					pMold->RobotOpnCls.Timer.IN = 0;
					pMold->RobotOpnCls.LimitTimer.IN   = 0;
					pMold->RobotOpnCls.Step = 3000;
					break;
			}
			
			break;
		
		case 2900:  // for no use
			
			pMold->ActInfo.RobotOpn	= 0;
			pMold->ActInfo.RobotCls	= 0;
			
			pMold->ValveOut.RobotOpn = 0;
			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
		
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN   = 0;
			pMold->RobotOpnCls.Step = 3000;
			
			break;
			
		case 3000:
			break;
			
		/* ---------------------- calibration Cls ---------------------- */	
		case 5000:
			pMold->ActInfo.RobotOpn	= 0;
			pMold->ActInfo.RobotCls	= 1;
			
			pMold->ValveOut.RobotOpn = 0;
			pMold->ValveOut.RobotCls = 1;
			
			pMold->RobotOpnCls.p_set = pRobotOpnClsFix->pCalibCls;
			pMold->RobotOpnCls.v_set = pRobotOpnClsFix->vCalibCls;
		
			break;
		
		/*--------------------------------------------------- */
		/*------------------ Robot Open  -------------------- */
		/*--------------------------------------------------- */
		case 10100:		/* opn delay */
			if(1 == pMold->Option.RobotOpnCls)
			{
				switch (pRobotOpnClsFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.RobotOpnDly != 0)
						{
							pMold->RobotOpnCls.Timer.IN = 1;
							pMold->RobotOpnCls.Timer.PT = pMold->TimeSet.RobotOpnDly;
		
							pMold->RobotOpnCls.Step = 10200;
						}
						else
						{
							pMold->RobotOpnCls.Step = 10300;
						}

						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.RobotOpnDly != 0)
						{
							pMold->RobotOpnCls.Timer.IN = 1;
							pMold->RobotOpnCls.Timer.PT = pMold->TimeSet.RobotOpnDly;
		
							pMold->RobotOpnCls.Step = 10200;
						}
						else
						{
							pMold->RobotOpnCls.Step = 10300;
						}
						
						break;
				}
			}
			else
			{
				pMold->RobotOpnCls.Step = 10300;
			}
	
			break;
			
		case 10200:  		/* delay ok */
			pMold->TimeDis.RobotOpnDly = pMold->RobotOpnCls.Timer.ET;
			if (1 == pMold->RobotOpnCls.Timer.Q)
			{
				pMold->RobotOpnCls.Timer.IN  = 0;
				pMold->RobotOpnCls.Step = 10300;
			}
			break;
		
		case 10300:  		
			if(pMold->Option.RobotOpnCls)
			{
				pMold->RobotOpnCls.LimitTimer.IN = 1; 
				pMold->RobotOpnCls.LimitTimer.PT = pMold->TimeSet.RobotOpnAlarmTime;
				pMold->RobotOpnCls.Step = 10400;
			}
			else  // for no use check limit
			{	
				if (1 == pMold->TransDIn.RobotOpnLimit)
				{
					pMold->RobotOpnCls.Step = 12900;  // for no use
				}
				else				
				{
					pMold->Alarm.RobotNotAtOpnPos = 1;
					pMold->RobotOpnCls.Step = 40000;           /*  Robot Open alarm   */
				}
			}

			break;
	
		case 10400: 			
			pMold->ActInfo.RobotOpn	= 1;
			pMold->ActInfo.RobotCls	= 0;
			
			pMold->ValveOut.RobotOpn = 1;
			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = pRobotOpnClsPara->P_Opn;
			pMold->RobotOpnCls.v_set = pRobotOpnClsPara->V_Opn;
			
			if (1 == pMold->TransDIn.RobotOpnLimit)
			{
				pMold->RobotOpnCls.p_set = 0;
				pMold->RobotOpnCls.v_set = 0;
			
				pMold->RobotOpnCls.Timer.IN = 0;
				pMold->RobotOpnCls.LimitTimer.IN   = 0;
			
				pMold->RobotOpnCls.Step = 10500;
			}
			break;
		
		case 10500: 			/* ¼ÐÊ±¼ä */
			
			if (1==gMachineInfo.Auto)
			{
				if(pMold == &RMold)
				{
					RMold.Option.RobotOpnCls = gMacOption.RRobotOpnCls;
				}
				else
				{
					LMold.Option.RobotOpnCls = gMacOption.LRobotOpnCls;
				}
			}

			
			pMold->ActInfo.RobotOpn	= 0;
			pMold->ActInfo.RobotCls	= 0;
			
			pMold->ValveOut.RobotOpn = 0;
			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
			
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.Step = 13000;

			break;
		
		case 12900: //for no use
			pMold->ActInfo.RobotOpn	= 0;
			pMold->ActInfo.RobotCls	= 0;
			
			pMold->ValveOut.RobotOpn = 0;
			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
			
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.Step = 13000;

			break;
			
		case 13000:
			break;
			
		/* ---------------------- calibration Opn ---------------------- */	
		case 15000:
			pMold->ActInfo.RobotOpn	= 1;
			pMold->ActInfo.RobotCls	= 0;
			
			pMold->ValveOut.RobotOpn = 1;
			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = pRobotOpnClsFix->pCalibOpn;
			pMold->RobotOpnCls.v_set = pRobotOpnClsFix->vCalibOpn;
		
			break;
		
      
		/* ------------- Normal stop ------------- */
		case 20000:

//			pMold->ActInfo.RobotOpn	= 0;
//			pMold->ActInfo.RobotCls	= 0;
//			
//			pMold->ValveOut.RobotOpn = 0;
//			pMold->ValveOut.RobotCls = 0;
			
			pMold->TimeDis.RobotClsDly 	= 0;
			pMold->TimeDis.RobotOpnDly 	= 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
			
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Step = 0;
			
			break;
		
		/* ------------- Instancy stop ------------- */	
		case 30000:
//			pMold->ActInfo.RobotOpn	= 0;
//			pMold->ActInfo.RobotCls	= 0;
//			
//			pMold->ValveOut.RobotOpn = 0;
//			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
			
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Step = 0;
			
			break;
		
		/* ------------- Error stop ------------- */	
		case 40000:
//			pMold->ActInfo.RobotOpn	= 0;
//			pMold->ActInfo.RobotCls	= 0;
//			
//			pMold->ValveOut.RobotOpn = 0;
//			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.p_set = 0;
			pMold->RobotOpnCls.v_set = 0;
			
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Step = 0;
			
			break;
		
		/* ------------- auto run stop ------------- */	
		case 41000:
//			pMold->ActInfo.RobotOpn	= 0;
//			pMold->ActInfo.RobotCls	= 0;
//			
//			pMold->ValveOut.RobotOpn = 0;
//			pMold->ValveOut.RobotCls = 0;
			
			pMold->RobotOpnCls.Timer.IN = 0;
			pMold->RobotOpnCls.LimitTimer.IN = 0;
			pMold->RobotOpnCls.Step = 40000;
			
			break;
	}/* end of switch */
	
	/* ----------------- Robot Open / Close alarm time display --------------------  */
	if(pMold->RobotOpnCls.Step > 300 && pMold->RobotOpnCls.Step  < 500)			/* Robot Close  */
	{
		pMold->TimeDis.RobotClsAlarmTime = pMold->RobotOpnCls.LimitTimer.ET;
	}
	else if(pMold->RobotOpnCls.Step > 10300 && pMold->RobotOpnCls.Step  < 10500)	/* Robot Open   */
	{
		pMold->TimeDis.RobotOpnAlarmTime = pMold->RobotOpnCls.LimitTimer.ET;
	}
	
	/* ----------------- Robot Open / Close timeout --------------------  */
	if(1 == pMold->RobotOpnCls.LimitTimer.Q  && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->RobotOpnCls.Step < 3000 && pMold->RobotOpnCls.Step > 200)		/* Robot Close  */
		{
			pMold->Alarm.RobotNotAtClsPos = 1;
		}
      
		if(pMold->RobotOpnCls.Step < 13000 && pMold->RobotOpnCls.Step > 10200)	/* Robot Open   */
		{
			pMold->Alarm.RobotNotAtOpnPos = 1;
		}
   
		pMold->RobotOpnCls.Step = 41000;
	}/* if(1 == pMold->RobotOpnCls.LimitTimer.Q) */

	/* ----------------- Robot Open / Close action limit --------------------  */
	if(pMold->RobotOpnCls.Step < 3000 && pMold->RobotOpnCls.Step > 200)		/* Robot Close  */
	{
		
	}
	
	if(pMold->RobotOpnCls.Step < 13000 && pMold->RobotOpnCls.Step > 10200)	/* Robot Open   */
	{

	}
	
}/*end of function */




