/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: bdeflash
 * File: bdeflash.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program bdeflash --- bottom deflasher control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void BottomDeflash( Mold_typ * pMold,TopDeflash_Fix_typ * pBottomDeflashFix, TopDeflash_Para_typ * pBottomDeflashPara,Mold_Para_User_typ * pMoldUser);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.BottomDeflash.Step = 0;
	RMold.BottomDeflash.Step = 0;
} /* end of _INIT */

 /*************************************************************************************
        		µ×²¿È¥·É±ß		C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(!LMold.TimeSet.BottomDeflashBwAlarmTime)LMold.TimeSet.BottomDeflashBwAlarmTime = 1000;  /*  10s  */
	if(!RMold.TimeSet.BottomDeflashBwAlarmTime)RMold.TimeSet.BottomDeflashBwAlarmTime = 1000;  /*  10s  */

	LMold.TransDIn.BottomDeflashBwLimit = gMachineIn.LMold.BottomDeflashBwLimit;
	LMold.TransDIn.BottomDeflashFwLimit = gMachineIn.LMold.BottomDeflashFwLimit;

	RMold.TransDIn.BottomDeflashBwLimit = gMachineIn.RMold.BottomDeflashBwLimit;
	RMold.TransDIn.BottomDeflashFwLimit = gMachineIn.RMold.BottomDeflashFwLimit;

	BottomDeflash(&LMold,&gMachineFix.MoldL.BottomDeflash,&gMachinePara.MoldL.BottomDeflash,&gUserPara.LMold);
	BottomDeflash(&RMold,&gMachineFix.MoldR.BottomDeflash,&gMachinePara.MoldR.BottomDeflash,&gUserPara.RMold);
	
	gMachineOut.LMold.BottomDeflashFw = LMold.ValveOut.BottomDeflashFw;
	gMachineOut.RMold.BottomDeflashFw = RMold.ValveOut.BottomDeflashFw;

	gMachineOut.LMold.BottomDeflashBw = LMold.ValveOut.BottomDeflashBw;
	gMachineOut.RMold.BottomDeflashBw = RMold.ValveOut.BottomDeflashBw;
	
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.BottomDeflash.eActuatorType = gMachineFix.MoldR.BottomDeflash.eActuatorType;

	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.BottomDeflash.ePressueOutputType = gMachineFix.MoldR.BottomDeflash.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.BottomDeflash.eFluxOutputType = gMachineFix.MoldR.BottomDeflash.eFluxOutputType;
	

} /* end of _CYCLIC */

void BottomDeflash( Mold_typ * pMold,TopDeflash_Fix_typ * pBottomDeflashFix, TopDeflash_Para_typ * pBottomDeflashPara,Mold_Para_User_typ * pMoldUser)
{

	switch ( pMold->BottomDeflash.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;	
			switch (pBottomDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.BottomDeflashFw = 0;
					pMold->ValveOut.BottomDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			pMold->BottomDeflash.AutoTimeOutFlag = 0;
			break;
		
		
		/*------------------  Bottom Deflash forward -------------------- */
		case 90: 			/* forward  Extra delay for Extrilift Acc charge */
			if(pMold->Option.BottomDeflash)
			{
				if(pMoldUser->TimeSet.BottomDeflashExtraDelay != 0)
				{
					pMold->BottomDeflash.Timer.IN = 1;
					pMold->BottomDeflash.Timer.PT = pMoldUser->TimeSet.BottomDeflashExtraDelay;
					pMold->BottomDeflash.Step 	= 95;
				}
				else
				{
					pMold->BottomDeflash.Step = 100;
				}
			}
			else
			{
				pMold->BottomDeflash.Step = 100;
			}
			
			break;
		
		case 95: 	/*Extara delay*/
			pMoldUser->TimeDis.BottomDeflashExtraDelay = pMold->BottomDeflash.Timer.ET;  
			if (1 == pMold->BottomDeflash.Timer.Q)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.BottomDeflashExtraDelay = 0;
				pMold->BottomDeflash.Step = 100;
			}
		
			break;

		/*------------------ BottomDeflash forward -------------------- */
		case 100: /* BottomDeflash delay*/
     
			if(pMold->Option.BottomDeflash)
			{
				if(pMold->TimeSet.BottomDeflashFwDelay != 0)
				{
					pMold->BottomDeflash.Timer.IN = 1;
					pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashFwDelay;
					pMold->BottomDeflash.Step 	= 200;
				}
				else
				{
					pMold->BottomDeflash.Step = 300;
				}
			}
			else
			{	
				pMold->BottomDeflash.Step = 300; 
			}
			break;
		
		case 200: 		/* delay ok */
			pMold->TimeDis.BottomDeflashFwDelay = pMold->BottomDeflash.Timer.ET;
			if (1 == pMold->BottomDeflash.Timer.Q ||  1 == pMold->BottomDeflash.AutoTimeOutFlag)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.BottomDeflashFwDelay = 0;
				pMold->BottomDeflash.Step = 300;
			}
			break;
		
		case 300: 	/* set time */	
			if(pMold->Option.BottomDeflash)
			{
				switch (pBottomDeflashFix->eActuatorType )
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.BottomDeflashFwTime != 0 )
						{
							pMold->BottomDeflash.LimitTimer.IN = 1; //ipis0213
							pMold->BottomDeflash.LimitTimer.PT = pMold->TimeSet.BottomDeflashFwAlarmTime;	
						
							pMold->BottomDeflash.Timer.IN = 1;
							pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashFwTime;
							pMold->BottomDeflash.Step = 400;
						}
						else
						{
							pMold->BottomDeflash.Step = 2900; 
						}
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.BottomDeflashFwTime != 0 )
						{	
							pMold->BottomDeflash.Timer.IN = 1;
							pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashFwTime;
							pMold->BottomDeflash.Step = 400;
						}
						else
						{
							pMold->BottomDeflash.Step = 2900; //ipis 0213
						}
						break;
				}
			}
			else
			{
				pMold->BottomDeflash.Step = 2900; 
			}
			
			break;
		
		case 400:	/* forward 1  */
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw;
			pMold->BottomDeflash.v_set = pBottomDeflashPara->V_Fw;
			
			pMold->TimeDis.BottomDeflashFwTime = pMold->BottomDeflash.Timer.ET;
			
			if ((1 == pMold->BottomDeflash.Timer.Q) || (1 == pMold->TransDIn.BottomDeflashFwLimit) )
			{
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->BottomDeflash.LimitTimer.IN = 0;
	            		
				pMold->BottomDeflash.Step     = 410;
			}
	
			break;
		
		case 410:/* Forward 2 Delay*/
			
			switch (pBottomDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					if(pMoldUser->TimeSet.BottomDeflashSecDelay != 0)
					{
						pMold->BottomDeflash.Timer.IN = 1;
						pMold->BottomDeflash.Timer.PT = pMoldUser->TimeSet.BottomDeflashSecDelay;
						pMold->BottomDeflash.Step = 420;
					}
					else
					{
						pMold->BottomDeflash.Step = 430;
					}
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ActInfo.BottomDeflashFw = 1;
					pMold->ActInfo.BottomDeflashBw = 0;
			
					pMold->ValveOut.BottomDeflashFw  = 1;
					pMold->ValveOut.BottomDeflashBw  = 0;
						
					pMold->BottomDeflash.p_set = 0;
					pMold->BottomDeflash.v_set = 0;
				
					pMold->BottomDeflash.Step = 500;
					break;
			}
			break;
		
		case 420: /* forward 2 delay ok */
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
				
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMoldUser->TimeDis.BottomDeflashSecDelay = pMold->BottomDeflash.Timer.ET;
			if (1 == pMold->BottomDeflash.Timer.Q ||  1 == pMold->BottomDeflash.AutoTimeOutFlag)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				pMold->BottomDeflash.LimitTimer.IN 	= 0;
				//	pMoldUser->TimeDis.BottomDeflashSecDelay = 0;
				pMold->BottomDeflash.Step = 430;
			}
			break;
		
		case 430:	/* Forward 2 time set*/
			if(pMoldUser->TimeSet.BottomDeflashSecTime != 0)
			{
				pMold->BottomDeflash.LimitTimer.IN = 1; //ipis0213
				pMold->BottomDeflash.LimitTimer.PT = pMold->TimeSet.BottomDeflashFwAlarmTime;
	
				pMold->BottomDeflash.Timer.IN = 1;
				pMold->BottomDeflash.Timer.PT = pMoldUser->TimeSet.BottomDeflashSecTime;
				pMold->BottomDeflash.Step = 440;
			}
			else
			{
				pMold->BottomDeflash.Step = 500;
			}
			break;

		
		case 440:  /* forward 2 */
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw2nd;
			pMold->BottomDeflash.v_set = pBottomDeflashPara->V_Fw2nd;
			
			pMoldUser->TimeDis.BottomDeflashSecTime = pMold->BottomDeflash.Timer.ET;
			if ((1 == pMold->BottomDeflash.Timer.Q) || (1 == pMold->TransDIn.BottomDeflashFwLimit))
			{
			
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->BottomDeflash.LimitTimer.IN = 0;
			
				if(1 == gMachineInfo.Auto)
				{
					pMold->BottomDeflash.Step = 500;
				}
				else
				{
					pMold->BottomDeflash.Step = 2900;
				}		
			}

			break;
		
		case 500:
			switch (pBottomDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ActInfo.BottomDeflashFw  = 0;
					pMold->ActInfo.BottomDeflashBw  = 0;
			
					pMold->ValveOut.BottomDeflashFw = 0;
					pMold->ValveOut.BottomDeflashBw = 0;
				
					pMold->BottomDeflash.p_set = 0;
					pMold->BottomDeflash.v_set = 0;
			
					pMold->BottomDeflash.Timer.IN = 0;
					pMold->BottomDeflash.LimitTimer.IN = 0;
				

					pMold->BottomDeflash.Step = 3000;
				
					
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/	
				
					pMold->ActInfo.BottomDeflashFw  = 1;
					pMold->ActInfo.BottomDeflashBw  = 0;
			
					pMold->ValveOut.BottomDeflashFw = 1;
					pMold->ValveOut.BottomDeflashBw = 0;
				
					pMold->BottomDeflash.p_set = 0;
					pMold->BottomDeflash.v_set = 0;
			
					pMold->BottomDeflash.Timer.IN = 0;
					pMold->BottomDeflash.LimitTimer.IN = 0;


					pMold->BottomDeflash.Step = 3000; // BottomDeflash bw

					break;
			}

			break;	
		
		case 2900:	// for no use
			
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;	
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			pMold->BottomDeflash.Step = 3000;
			
			break;
		
		case 3000:
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->BottomDeflash.Step = 10100;
			}
			
			break;

		
		
		/* ---------------------- calibration fw ---------------------- */
		case 5000: 
			
			if(0 == pMold->TransDIn.BottomDeflashFwLimit)
			{
				pMold->ActInfo.BottomDeflashFw = 1;
				pMold->ActInfo.BottomDeflashBw = 0;
            	
				pMold->ValveOut.BottomDeflashFw = 1;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set = pBottomDeflashFix->pCalibMax;
				pMold->BottomDeflash.v_set = pBottomDeflashFix->vCalibMax;
			
			}
			else
			{
				pMold->ActInfo.BottomDeflashFw = 0;
				pMold->ActInfo.BottomDeflashBw = 0;
            	
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set = 0;
				pMold->BottomDeflash.v_set = 0;
			}
		
			break;
		
		
		/*--------------------------------------------------- */
		/*-------------- BottomDeflash Bw  ------------------ */
		/*--------------------------------------------------- */	
		
		case 10100: /* BottomDeflash Bw delay*/			
			if(pMold->Option.BottomDeflash)
			{
				switch (pBottomDeflashFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.BottomDeflashBwDelay != 0)
						{
							pMold->BottomDeflash.Timer.IN = 1;
							pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashBwDelay;
							pMold->BottomDeflash.Step 	= 10200;
						}
						else
						{
							pMold->BottomDeflash.Step = 10300;
						}
						
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						pMold->BottomDeflash.Step = 10300;
						break;
				}
			}
			else
			{
				pMold->BottomDeflash.Step = 10300;
			}
			break;
		case 10200: /* delay ok */
			pMold->TimeDis.BottomDeflashBwDelay = pMold->BottomDeflash.Timer.ET;
			if (1 == pMold->BottomDeflash.Timer.Q ||  1 == pMold->BottomDeflash.AutoTimeOutFlag)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				//		pMold->TimeDis.BottomDeflashBwDelay = 0;
				pMold->BottomDeflash.Step = 10300;
			}
			break;	
		
		case 10300:
			if(pMold->Option.BottomDeflash)
			{		
				pMold->BottomDeflash.Timer.IN = 1;
				pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashBwAlarmTime;
				pMold->BottomDeflash.Step = 10400;
			}
			else  // for no use check limit
			{	
				if (1 == pMold->TransDIn.BottomDeflashBwLimit)
				{
					pMold->BottomDeflash.Step = 12900;  // for no use
				}
				else				
				{
					pMold->Alarm.BottomDeflashNotAtBwPos = 1;
					pMold->BottomDeflash.Step = 40000;           /*  BottomDeflash bw alarm   */
				}
			}
			break;
		
		case 10400:
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 1;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 1;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BottomDeflash.p_set = pBottomDeflashPara->P_BwReset;
				pMold->BottomDeflash.v_set = pBottomDeflashPara->V_BwReset;
			}
			else
			{
				pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Bw;
				pMold->BottomDeflash.v_set = pBottomDeflashPara->V_Bw;
			}
			
			pMold->TimeDis.BottomDeflashBwAlarmTime = pMold->BottomDeflash.Timer.ET;
			
			if(1 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->BottomDeflash.Step     = 10500;
			}
			
			break;
		
		case 10500: 
			
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
		
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
		
			pMold->BottomDeflash.Timer.IN = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			
			pMold->BottomDeflash.Step = 13000;
				
			break;
		
		case 12900: //for no use	
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
		
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;

			pMold->BottomDeflash.Timer.IN = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			pMold->BottomDeflash.Step     = 13000;
		
			break;
		
		case 13000:
		
			break;
		
		/* ---------------------- calibration bw ---------------------- */
		case 15000: 
			
			if(0 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->ActInfo.BottomDeflashFw = 0;
				pMold->ActInfo.BottomDeflashBw = 1;
            	
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 1;
			
				pMold->BottomDeflash.p_set = pBottomDeflashFix->pCalibZero;
				pMold->BottomDeflash.v_set = pBottomDeflashFix->vCalibZero;
			
			}
			else
			{
				pMold->ActInfo.BottomDeflashFw = 0;
				pMold->ActInfo.BottomDeflashBw = 0;
            	
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set = 0;
				pMold->BottomDeflash.v_set = 0;
			}
					
			
			break;
		

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			switch (pBottomDeflashFix->eActuatorType)
			{
			/*-------------------------------------------------*/
			case ACTUATOR_HYD: /*1:Oil*/
			/*-------------------------------------------------*/
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			break;
			/*-------------------------------------------------*/
			case ACTUATOR_PNEU: /*2:Air*/
			/*-------------------------------------------------*/

			break;
			}	
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			pMold->BottomDeflash.Step     = 0;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			switch (pBottomDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.BottomDeflashFw = 0;
					pMold->ValveOut.BottomDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			pMold->BottomDeflash.Step     = 0;
			
			break;

		/*--------------- Error  stop -----------------*/	
		case 40000:
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			switch (pBottomDeflashFix->eActuatorType)
			{
			/*-------------------------------------------------*/
			case ACTUATOR_HYD: /*1:Oil*/
			/*-------------------------------------------------*/
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			break;
			/*-------------------------------------------------*/
			case ACTUATOR_PNEU: /*2:Air*/
			/*-------------------------------------------------*/

			break;
			}	
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			break;
		
		/* ------------- auto run stop ------------- */		
		case 41000:
			pMold->ActInfo.BottomDeflashFw = 0;
			pMold->ActInfo.BottomDeflashBw = 0;
			
			switch (pBottomDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.BottomDeflashFw = 0;
					pMold->ValveOut.BottomDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}	
			
			pMold->BottomDeflash.p_set = 0;
			pMold->BottomDeflash.v_set = 0;
			
			pMold->BottomDeflash.Timer.IN	= 0;
			pMold->BottomDeflash.LimitTimer.IN	= 0;
			
			pMold->BottomDeflash.Step = 40000;
			break;
	}/* end of switch */
	
	
	/* ----------------- BottomDeflash Fw / Bw timeout --------------------  */
	if(1 == pMold->BottomDeflash.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BottomDeflash.Step < 500 && pMold->BottomDeflash.Step > 300)		/* BottomDeflash Fw  */
		{
			pMold->Alarm.BottomDeflashNotAtFwPos = 1;
		}
	     
		if(pMold->BottomDeflash.Step < 1700 && pMold->BottomDeflash.Step > 1400)	/* BottomDeflash Bw  */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = 1;
		}
	     
		pMold->BottomDeflash.Timer.IN   = 0;
		pMold->BottomDeflash.LimitTimer.IN = 0;
		pMold->BottomDeflash.Step = 40000;
		
	}
	
	/* ----------------- BottomDeflash Fw / Bw alarm time display --------------------  */
	if(pMold->BottomDeflash.Step > 300 && pMold->BottomDeflash.Step  < 500)			/* BottomDeflash Fw  */
	{
		pMold->TimeDis.BottomDeflashFwAlarmTime = pMold->BottomDeflash.LimitTimer.ET;
	}
	else if(pMold->BottomDeflash.Step > 10300 && pMold->BottomDeflash.Step  < 10500)	/* BottomDeflash Bw   */
	{
		pMold->TimeDis.BottomDeflashBwAlarmTime = pMold->BottomDeflash.LimitTimer.ET;
	}
	
	/* ----------------- BottomDeflash Fw / Bw timeout --------------------  */
	if(1 == pMold->BottomDeflash.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BottomDeflash.Step < 500 && pMold->BottomDeflash.Step > 300)		/* BottomDeflash Fw  */
		{
			pMold->Alarm.BottomDeflashNotAtFwPos = 1;
		}
      
		if(pMold->BottomDeflash.Step < 10500 && pMold->BottomDeflash.Step > 10200)	/* BottomDeflash Bw  */
		{
			pMold->Alarm.BottomDeflashNotAtBwPos = 1;
		}
      
		pMold->BottomDeflash.Step = 41000;
	}
	
	/* ----------------- BottomDeflash action limit --------------------  */
	if(pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 500)	/* BottomDeflash Fw  */
	{
		if(0 == pMold->Clamp.ClsPos)
		{
			pMold->Alarm.ClampNotAtClsDI  = !pMold->Clamp.ClsPos;
			pMold->BottomDeflash.Step = 41000;
		}
	 	
		if(0 == pMold->Carriage.BwPos)
		{
			pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
			pMold->BottomDeflash.Step = 41000;
		}
		
		if(0 == pMold->TransDIn.CoolPinUpLimit)
		{
			pMold->Alarm.CarriageNotAtBwDI = !pMold->TransDIn.CoolPinUpLimit;
			pMold->BottomDeflash.Step = 41000;
		}
	}
	
	if((pMold->BottomDeflash.Step > 10100 && pMold->BottomDeflash.Step < 10500))	/* BottomDeflash Bw  */
	{
	
		
	}

}/*end of function */




