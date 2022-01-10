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

//void BottomDeflash( Mold_typ * pMold);
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

//	gMachineOut.LMold.BottomDeflashFw = LMold.BottomDeflash.Out;
//	gMachineOut.RMold.BottomDeflashFw = RMold.BottomDeflash.Out;
	
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
			pMold->BottomDeflash.Out      = 0;
			pMold->ActInfo.BottomDeflash  = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
		
			if (0 == gMachineInfo.Auto)
			{
				pMold->ValveOut.BottomDeflashBw = 0;//ipis0726
			}
			
			pMold->BottomDeflash.Timer.IN	= 0;
			break;
		
		
		/*------------------ TopDeflash forward -------------------- */
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
				//	pMold->TimeDis.TopDeflashFwDelay = 0;
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
			if (1 == pMold->BottomDeflash.Timer.Q)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.BottomDeflashFwDelay = 0;
				pMold->BottomDeflash.Step = 300;
			}
			break;
		
		case 300: 	
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
							//	pMold->BottomDeflash.Step = 499;
							pMold->BottomDeflash.Step = 2900; //ipis 0213
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
							//	pMold->BottomDeflash.Step = 499;
							pMold->BottomDeflash.Step = 2900; //ipis 0213
						}
						break;
				}
			}
			else
			{
				pMold->BottomDeflash.Step = 2900; // for no use
			}
			
			break;
		
		case 400:			/* forward time ok */
			pMold->BottomDeflash.Out     = 1;
			pMold->ActInfo.BottomDeflash = 1;
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw;
			
			pMold->TimeDis.BottomDeflashFwTime = pMold->BottomDeflash.Timer.ET;
			
			if ((1 == pMold->BottomDeflash.Timer.Q) )
			{
			
				pMold->BottomDeflash.Out      = 0;
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set = 0;
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->BottomDeflash.LimitTimer.IN = 0;
				//	pMold->TimeDis.BottomDeflashFwTime = 0;
	            		
				pMold->BottomDeflash.Step     = 410;
	
			}
							
			pMold->TimeDis.BottomDeflashFwAlarmTime = pMold->BottomDeflash.LimitTimer.ET;
	
			break;
		
		/*------------------------------------------------------------------------------*/	
		
		case 410:/*Second Forward Delay*/
			
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
					pMold->BottomDeflash.Step = 1300;
					break;
			}
			break;
		case 420: /*Start Second TopDeflash Forward Timer*/
			
			pMold->BottomDeflash.Out      = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = 0;
			
			pMoldUser->TimeDis.BottomDeflashSecDelay = pMold->BottomDeflash.Timer.ET;
			if (1 == pMold->BottomDeflash.Timer.Q)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				pMold->BottomDeflash.LimitTimer.IN 	= 0;
				//	pMoldUser->TimeDis.BottomDeflashSecDelay = 0;
				pMold->BottomDeflash.Step = 430;
			}
			break;
		
		case 430:		/*Start Second TopDeflash Forward Timer*/
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
				pMold->BottomDeflash.Step = 1300;
			}
			break;

		
		case 440:  /*Bottom deflash second forward */
			pMold->BottomDeflash.Out     = 1;
			pMold->ActInfo.BottomDeflash = 1;
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw2nd;
			
			pMoldUser->TimeDis.BottomDeflashSecTime = pMold->BottomDeflash.Timer.ET;
			if ((1 == pMold->BottomDeflash.Timer.Q) || (1 == pMold->TransDIn.BottomDeflashFwLimit))
			{
			
				pMold->BottomDeflash.Out      = 0;
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
				pMold->BottomDeflash.p_set = 0;
				pMold->BottomDeflash.Timer.IN = 0;
				//	pMold->TimeDis.BottomDeflashFwTime = 0;
				//	pMoldUser->TimeDis.BottomDeflashSecTime=0;
				pMold->BottomDeflash.Step     = 1300;			
			}
			pMold->TimeDis.BottomDeflashFwAlarmTime = pMold->BottomDeflash.LimitTimer.ET;

			break;
			
			
			
		
		/*----------------------------------------------*/
		/*No Time Movement-move unitl reach limit sensor*/
		/*----------------------------------------------*/	
		
		case 499:
			pMold->BottomDeflash.Out     = 1;
			pMold->ActInfo.BottomDeflash = 1;
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw;
			
			if (1 == pMold->TransDIn.BottomDeflashFwLimit)
			{
			
				pMold->BottomDeflash.Out      = 0;
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
				pMold->BottomDeflash.p_set = 0;
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->BottomDeflash.LimitTimer.IN = 0;
				//	pMold->TimeDis.BottomDeflashFwTime = 0;
			
				pMold->BottomDeflash.Step     = 2900;
			}
			
			pMold->TimeDis.BottomDeflashFwAlarmTime = pMold->BottomDeflash.LimitTimer.ET;

			if (1 == pMold->BottomDeflash.LimitTimer.Q )
			{
				pMold->Alarm.BottomDeflashNotAtFwPos=1;
				pMold->BottomDeflash.Timer.IN   = 0;
				pMold->BottomDeflash.LimitTimer.IN = 0;
				pMold->BottomDeflash.Step = 40000;
			}
			
			
			
			break;
		
		
		
		
		case 1000:
			pMold->BottomDeflash.Out     = 1;
			pMold->ActInfo.BottomDeflashFw  = 1;
			pMold->ActInfo.BottomDeflashBw  = 0;
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Fw;
			pMold->ValveOut.BottomDeflashFw = 1;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->ActInfo.BottomDeflash = 1;
			break;
	
		/*------- BottomDeflash Bw --------------------------*/	
		
		case 1300: /* BottomDeflash Bw delay*/			
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
							pMold->BottomDeflash.Step 	= 1400;
						}
						else
						{
							pMold->BottomDeflash.Step = 1500;
						}
						
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
						pMold->BottomDeflash.Step = 1500;
						break;
				}
			}
			else
			{
				pMold->BottomDeflash.Step = 1500;
			}
			break;
		case 1400: /* delay ok */
			pMold->TimeDis.BottomDeflashBwDelay = pMold->BottomDeflash.Timer.ET;
			if (1 == pMold->BottomDeflash.Timer.Q)
			{
				pMold->BottomDeflash.Timer.IN       = 0;
				//		pMold->TimeDis.BottomDeflashBwDelay = 0;
				pMold->BottomDeflash.Step = 1500;
			}
			break;	
		
		case 1500:

			if(pMold->Option.BottomDeflash)
			{		
				pMold->BottomDeflash.Timer.IN = 1;
				pMold->BottomDeflash.Timer.PT = pMold->TimeSet.BottomDeflashBwAlarmTime;
				pMold->BottomDeflash.Step = 1600;
			}
			else
			{
				pMold->BottomDeflash.Step = 1700; // for no use 
			}
			break;
		
		case 1600:
			pMold->BottomDeflash.Out     = 0;
			
			pMold->ActInfo.BottomDeflash = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 1;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 1;
			
			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_Bw;
			
			pMold->TimeDis.BottomDeflashBwAlarmTime = pMold->BottomDeflash.Timer.ET;
			
			if (1 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->BottomDeflash.Out      = 0;
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
				pMold->BottomDeflash.p_set=0;
			
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->TimeDis.BottomDeflashBwTime = 0;
				pMold->BottomDeflash.Step     = 2900;
			}
			
			break;
		
		case 1700: // for no use
			
			if (1 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->BottomDeflash.Out      = 0;
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
				pMold->BottomDeflash.p_set=0;
			
				pMold->BottomDeflash.Timer.IN = 0;
				pMold->TimeDis.BottomDeflashBwTime = 0;
				pMold->BottomDeflash.Step     = 2900;
			}
			else	
			{
				pMold->BottomDeflash.Timer.IN   = 0;
				pMold->Alarm.BottomDeflashNotAtBwPos = 1;
				pMold->BottomDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			
			}
				
			
			//			pMold->TimeDis.BottomDeflashBwAlarmTime = pMold->BottomDeflash.Timer.ET;
			//			if(1 == pMold->BottomDeflash.Timer.Q)
			//			{
			//				pMold->ValveOut.BottomDeflashFw  = 0;
			//				pMold->ValveOut.BottomDeflashBw  = 0;
			//					
			//				pMold->BottomDeflash.Timer.IN   = 0;
			//				pMold->Alarm.BottomDeflashNotAtBwPos = 1;
			//				pMold->BottomDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			//			}
				
			break;
		/*----------for handle bottom deflash force backward------------------------------*/
		case 1950: //for auto force backward
			
			pMold->BottomDeflash.Timer.IN = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			pMold->BottomDeflash.Step = 2000;
			
			break;
		
		case 2000:
			pMold->BottomDeflash.Out     = 0;
			pMold->ActInfo.BottomDeflash = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			pMold->BottomDeflash.p_set=0;
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.Step    = 0;
			break;
		
		case 2900:
			
			pMold->BottomDeflash.Timer.IN = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			pMold->BottomDeflash.Out     = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			pMold->BottomDeflash.p_set=0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			pMold->ActInfo.BottomDeflash = 0;
			pMold->BottomDeflash.Step    = 3000;
			break;
		
		case 3000:
			break;

		
		
		case 5000: /* FW*/
			
			//			pMold->ActInfo.BottomDeflashFw  = 1;
			//			pMold->ActInfo.BottomDeflashBw  = 0;
			//			
			//			pMold->ValveOut.BottomDeflashFw = 1;
			//			pMold->ValveOut.BottomDeflashBw = 0;
			//			
			//			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_CalibFw;
			
			if(0 == pMold->TransDIn.BottomDeflashFwLimit)
			{
				pMold->ActInfo.BottomDeflashFw  = 1;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 1;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set = pBottomDeflashFix->pCalibMax; 
				pMold->BottomDeflash.v_set = pBottomDeflashFix->vCalibMax; 
			}
			else		
			{
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;
			
				pMold->BottomDeflash.p_set 		= 0;
				pMold->BottomDeflash.v_set		= 0;
			
				pMold->BottomDeflash.Step    	= 20000;
			}
		
			break;
			
		case 15000: /* BW*/
			
			
			//			pMold->ActInfo.BottomDeflashFw  = 0;
			//			pMold->ActInfo.BottomDeflashBw  = 1;
			//			
			//			pMold->ValveOut.BottomDeflashFw = 0;
			//			pMold->ValveOut.BottomDeflashBw = 1;
			//			
			//			pMold->BottomDeflash.p_set = pBottomDeflashPara->P_CalibBw;
			
			if(0 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 1;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 1;
			
				pMold->BottomDeflash.p_set = pBottomDeflashFix->pCalibZero; 
				pMold->BottomDeflash.v_set = pBottomDeflashFix->vCalibZero;  
				
			}
			else
				
			{
				pMold->ActInfo.BottomDeflashFw  = 0;
				pMold->ActInfo.BottomDeflashBw  = 0;
			
				pMold->ValveOut.BottomDeflashFw = 0;
				pMold->ValveOut.BottomDeflashBw = 0;

				pMold->BottomDeflash.p_set 		= 0;							
				pMold->BottomDeflash.v_set 		= 0;
				pMold->BottomDeflash.Step    = 20000;
			}
					
			
			break;
		

		/*--------------- Manual action stop -----------------*/
		case 20000:
			
			pMold->BottomDeflash.Out      = 0;
			
			pMold->BottomDeflash.Timer.IN   = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set		= 0;
			pMold->BottomDeflash.v_set 		= 0;
			
			pMold->BottomDeflash.Step 	 	= 0;
			break;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->BottomDeflash.Out      = 0;
				
			pMold->BottomDeflash.Timer.IN   = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			
			pMold->BottomDeflash.p_set		= 0;
			pMold->BottomDeflash.v_set 		= 0;
			
			pMold->BottomDeflash.Step     	= 0;
			break;

		/*--------------- error -----------------*/
		case 40000:
			pMold->BottomDeflash.Out      = 0;
				
			pMold->BottomDeflash.Timer.IN   = 0;
			pMold->BottomDeflash.LimitTimer.IN = 0;
			
			pMold->ActInfo.BottomDeflashFw  = 0;
			pMold->ActInfo.BottomDeflashBw  = 0;
			
			pMold->ValveOut.BottomDeflashFw = 0;
			pMold->ValveOut.BottomDeflashBw = 0;
			pMold->BottomDeflash.p_set		= 0;
			pMold->BottomDeflash.v_set 		= 0;
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
	
	/* ----------------- BottomDeflash action limit --------------------  */
	if(pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 500)
	{
		if(0 == pMold->Clamp.ClsPos)
		{
			pMold->Alarm.ClampNotAtClsDI  = !pMold->Clamp.ClsPos;
			pMold->TopDeflash.Step = 40000;
		}
	 	
		if(0 == pMold->Carriage.BwPos)
		{
			pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
			pMold->TopDeflash.Step = 40000;
		}
	}
	
	if((pMold->BottomDeflash.Step > 1300 && pMold->BottomDeflash.Step < 1700))
	{
	
		
	}

}/*end of function */




