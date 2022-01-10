/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: tdeflash
 * File: tdeflash.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program tdeflash --- top deflasher control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

//void TopDeflash( Mold_typ * pMold, Punch_Para_typ * pTopDeflashPara);
void TopDeflash( Mold_typ * pMold, TopDeflash_Fix_typ * pTopDeflashFix, TopDeflash_Para_typ * pTopDeflashPara,Mold_Para_User_typ * pMoldUser);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.TopDeflash.Step = 30000;
	RMold.TopDeflash.Step = 30000;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(!LMold.TimeSet.TopDeflashFwAlarmTime)LMold.TimeSet.TopDeflashFwAlarmTime = 1000;  /*  10s  */
	if(!RMold.TimeSet.TopDeflashFwAlarmTime)RMold.TimeSet.TopDeflashFwAlarmTime = 1000;  /*  10s  */
	
	if(!LMold.TimeSet.TopDeflashBwAlarmTime)LMold.TimeSet.TopDeflashBwAlarmTime = 1000;  /*  10s  */
	if(!RMold.TimeSet.TopDeflashBwAlarmTime)RMold.TimeSet.TopDeflashBwAlarmTime = 1000;  /*  10s  */

	if(!LMold.TimeSet.TopDeflashOpnAlarmTime)LMold.TimeSet.TopDeflashOpnAlarmTime = 1000;  /*  10s  */
	if(!RMold.TimeSet.TopDeflashOpnAlarmTime)RMold.TimeSet.TopDeflashOpnAlarmTime = 1000;  /*  10s  */

	LMold.TransDIn.TopDeflashBwLimit = gMachineIn.LMold.TopDeflashBwLimit;  
	LMold.TransDIn.TopDeflashFwLimit = gMachineIn.LMold.TopDeflashFwLimit;

	RMold.TransDIn.TopDeflashBwLimit = gMachineIn.RMold.TopDeflashBwLimit;
	RMold.TransDIn.TopDeflashFwLimit = gMachineIn.RMold.TopDeflashFwLimit;

	LMold.TransDIn.TopDeflashOpnLimit = gMachineIn.LMold.TopDeflashOpnLimit;
	RMold.TransDIn.TopDeflashOpnLimit = gMachineIn.RMold.TopDeflashOpnLimit;

	TopDeflash(&LMold,&gMachineFix.MoldL.TopDeflash,&gMachinePara.MoldL.TopDeflash,&gUserPara.LMold);
	TopDeflash(&RMold,&gMachineFix.MoldR.TopDeflash,&gMachinePara.MoldR.TopDeflash,&gUserPara.RMold);
	

	gMachineOut.LMold.TopDeflashFw  = LMold.ValveOut.TopDeflashFw;
	gMachineOut.LMold.TopDeflashBw  = LMold.ValveOut.TopDeflashBw;

	gMachineOut.LMold.TopDeflashCls = LMold.ValveOut.TopDeflashCls;

	gMachineOut.RMold.TopDeflashFw  = RMold.ValveOut.TopDeflashFw;
	gMachineOut.RMold.TopDeflashBw  = RMold.ValveOut.TopDeflashBw;

	gMachineOut.RMold.TopDeflashCls = RMold.ValveOut.TopDeflashCls;
	
	gMachineOut.RMold.TopDeflashCool = RMold.ValveOut.TopDeflashCool;
	gMachineOut.LMold.TopDeflashCool = LMold.ValveOut.TopDeflashCool;
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.TopDeflash.eActuatorType = gMachineFix.MoldR.TopDeflash.eActuatorType;

	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.TopDeflash.ePressueOutputType = gMachineFix.MoldR.TopDeflash.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.TopDeflash.eFluxOutputType = gMachineFix.MoldR.TopDeflash.eFluxOutputType;
	
	
} /* end of _CYCLIC */

void TopDeflash( Mold_typ * pMold,TopDeflash_Fix_typ * pTopDeflashFix, TopDeflash_Para_typ * pTopDeflashPara,Mold_Para_User_typ * pMoldUser)
{
		
	switch ( pMold->TopDeflash.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			
			pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			
			//			if(1 == gMachineInfo.Auto)
			//			{
			//				pMold->ValveOut.TopDeflashFw = 0;
			//				pMold->ValveOut.TopDeflashBw = 1;
			//			}
			
			pMold->ActInfo.TopDeflashPuncher = 0;
			//	pMold->ActInfo.TopDeflashFw = pMold->ValveOut.TopDeflashFw;
			//	pMold->ActInfo.TopDeflashBw = pMold->ValveOut.TopDeflashBw;
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			if (0 == gMachineInfo.Auto) //ipis0726
			{
				pMold->ValveOut.TopDeflashBw = 0;	
			}
			
			
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;
			
			pMold->TopDeflash.Timer.IN	= 0;
			break;
            
		/*------------------ TopDeflash prepare forward -------------------- */
		case 10:			/* TopDeflash prepare forward */
			if(pMold->Option.TopDeflash)
			{
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashPreFwTime;
				pMold->TopDeflash.Step = 20;
			}
			else
			{
				pMold->TopDeflash.Step = 30;
			}
			break;
            	
		case 20:
			
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw;/*Albert*/
				
			pMold->TimeDis.TopDeflashPreFwTime = pMold->TopDeflash.Timer.ET;
			if (1 == pMold->TopDeflash.Timer.Q )
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
					
				pMold->TopDeflash.Timer.IN = 0;
				pMold->TimeDis.TopDeflashPreFwTime = 0;
					
				//		pMold->CoolDeflash.Step = 100;		/* cooling deflash  */ //ipis0731
				pMold->TopDeflash.Step = 30;
			}
            	
			if (1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TimeDis.TopDeflashPreFwTime = 0;
				pMold->TopDeflash.Step = 40000;
			}
			break;
            	
		case 30:
			if(0 == pMold->Option.CoolPin)
			{
				pMold->TopDeflash.Step = 100;	/*  defalsh forward */
			}
	
			if(0 == pMold->Option.TopDeflash)
			{
				pMold->CoolPin.Step     = 100;	/*  «á§Ncool pin down    */
				pMold->CoolPinBlow.Step = 100;	/*  «á§Ncool pin blowing§j®ð  */
			}
				
			if(1 == pMold->Option.CoolPin && 1 == pMold->Option.TopDeflash)
			{
				if( 0 == pMold->Option.PunchHandle)
				{
					pMold->CoolPin.Step     = 100;/*  «á§Ncool pin down      */
					pMold->CoolPinBlow.Step = 100;/*  «á§Ncool pin blowing§j®ð  */
				}
				else
				{
					pMold->TopDeflash.Step = 100;/*  defalsh forward    */
				}
			}
			break;

		/*------------------ TopDeflash forward -------------------- */
		case 90: 			/* forward  Extra delay for Extrilift Acc charge */
			if(pMold->Option.TopDeflash)
			{
				if(pMoldUser->TimeSet.TopDeflashExtraDelay != 0)
				{
					pMold->TopDeflash.Timer.IN = 1;
					pMold->TopDeflash.Timer.PT = pMoldUser->TimeSet.TopDeflashExtraDelay;
					pMold->TopDeflash.Step 	= 95;
				}
				else
				{
					pMold->TopDeflash.Step = 100;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 100;
			}
			
			break;
		
		case 95: 	/*Extara delay*/
			pMoldUser->TimeDis.TopDeflashExtraDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashFwDelay = 0;
				pMold->TopDeflash.Step = 100;
			}
		
			break;
		
		/*-------------------------------------- */
		case 100: 			/* forward delay*/
			//		pMold->ValveOut.TopDeflashFw  = 0;
			//		pMold->ValveOut.TopDeflashBw  = 0;
			//		pMold->ValveOut.TopDeflashCls = 0;
			//		
			
			pMold->CutNeck.Step = 100;   	/* ÐýÇÐÆ¿¿Ú */
			pMold->CoolDeflash.Step =100; 	/* ´òÊÖ°Ñ´µÀä*/
			if(pMold->Option.TopDeflash)
			{
				//		pMold->CoolDeflash.Step = 100;		/* cooling deflash  */ //ipis0731
				if(pMold->TimeSet.TopDeflashFwDelay != 0)
				{
					pMold->TopDeflash.Timer.IN = 1;
					pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashFwDelay;
					pMold->TopDeflash.Step 	= 200;
					
					
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.TopDeflashFwDelay = pMold->TimeSet.TopDeflashFwDelay/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.TopDeflashFwDelay = pMold->TimeSet.TopDeflashFwDelay/100.0;
					}
				}
				else
				{
					pMold->TopDeflash.Step = 300;
				}
			}
			else
			{
				//	pMold->ActInfo.TopDeflashFw = 0;
				//	pMold->ActInfo.TopDeflashBw = 0;
				pMold->TopDeflash.Step = 300;
			}
			break;
             	
		case 200: 			/* forward delay ok */
			pMold->TimeDis.TopDeflashFwDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashFwDelay = 0;
				pMold->TopDeflash.Step = 300;
			}
			break;
            	
		case 300:
			
			if(pMold->Option.TopDeflash)
			{
				switch (pTopDeflashFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.TopDeflashFwTime != 0)
						{
							pMold->TopDeflash.LimitTimer.IN = 1; //ipis0213
							pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashFwAlarmTime;
					
							pMold->TopDeflash.Timer.IN = 1;
							pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashFwTime;
							pMold->TopDeflash.Step = 400;
					
							//gExSPC
							if(pMold == & LMold)
							{
								gExSPC[ExIndex].LMold.TopDeflashFwTime = pMold->TimeSet.TopDeflashFwTime/100.0;
							}
							else
							{
								gExSPC[ExIndex].RMold.TopDeflashFwTime = pMold->TimeSet.TopDeflashFwTime/100.0;
							}
						}
						else
						{
							//	pMold->TopDeflash.Step = 499;/*If no timer then direct forward until reach limit sensor*/	
							//							pMold->TopDeflash.Step = 2900;/*If no timer then direct forward until reach limit sensor*/ //ipis2190131
							pMold->TopDeflash.Step = 410;
						}
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.TopDeflashFwTime != 0 )
						{	
							pMold->TopDeflash.Timer.IN = 1;
							pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashFwTime;
							pMold->TopDeflash.Step = 400;
						}
						else
						{
							//	pMold->TopDeflash.Step = 499;
							pMold->TopDeflash.Step = 2900; //ipis 0213
						}
						break;
				}
			}
			else			
			{
				pMold->TopDeflash.Step = 2900;  // for no use
			}
			break;
            
		case 400:			/* forward time ok */
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw;/*Albert*/
			pMold->TopDeflash.v_set = pTopDeflashPara->V_Fw;
			
			pMold->TimeDis.TopDeflashFwTime = pMold->TopDeflash.Timer.ET;

			if (1 == pMold->TopDeflash.Timer.Q || (1 == pMold->TransDIn.TopDeflashFwLimit))
			{
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tTopDeflash1 = pMold->TopDeflash.LimitTimer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tTopDeflash1 = pMold->TopDeflash.LimitTimer.ET/100.0;
				}
				
			
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
				//			pMold->TimeDis.TopDeflashFwTime = 0;
			
				pMold->TopDeflash.p_set = 0;/*Albert*/
				pMold->TopDeflash.v_set = 0;
			
				pMold->TopDeflash.Step = 410;
		
			}
			
			pMold->TimeDis.TopDeflashFwAlarmTime = pMold->TopDeflash.LimitTimer.ET;

			//			if (1 == pMold->TopDeflash.LimitTimer.Q )
			//			{
			//			
			//				pMold->Alarm.TopDeflashNotAtFwPos=1;
			//				pMold->TopDeflash.Timer.IN   = 0;
			//				pMold->TopDeflash.LimitTimer.IN = 0;
			//				pMold->TopDeflash.Step = 40000;
			//			}
			
			
			
			break;
		
		case 410:/*Second Forward Delay*/
			
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					if(pMoldUser->TimeSet.TopDeflashSecDelay != 0)
					{
						pMold->TopDeflash.Timer.IN = 1;
						pMold->TopDeflash.Timer.PT = pMoldUser->TimeSet.TopDeflashSecDelay;
						pMold->TopDeflash.Step = 420;
			
						//gExSPC
						if(pMold == & LMold)
						{
							gExSPC[ExIndex].LMold.TopDeflashSecDelay = pMoldUser->TimeSet.TopDeflashSecDelay/100.0;
						}
						else
						{
							gExSPC[ExIndex].RMold.TopDeflashSecDelay = pMoldUser->TimeSet.TopDeflashSecDelay/100.0;
						}
					}
					else
					{
						pMold->CoolDeflash.Step = 450;  //ipis0731
						pMold->TopDeflash.Step = 430;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->CoolDeflash.Step = 450;  //ipis0731	
					if(1 == gMachineInfo.Auto)
					{
						pMold->TopDeflash.Step = 500;
					}
					else
					{
						pMold->TopDeflash.Step = 2900;
					}
				
					break;
			}
						
			break;
		case 420:/*wait delay for cool deflash*/
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
			
			pMold->TopDeflash.p_set = 0;/*Albert*/
			pMold->ValveOut.TopDeflashCool = 1;
			
			pMoldUser->TimeDis.TopDeflashSecDelay = pMold->TopDeflash.Timer.ET;
			if(pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
				//	pMoldUser->TimeDis.TopDeflashSecDelay =0;
				pMold->TopDeflash.Step = 430;
			}			
			break;

		case 430:/*Start Second TopDeflash Forward Timer*/

			if(pMoldUser->TimeSet.TopDeflashSecTime != 0)
			{
				pMold->TopDeflash.LimitTimer.IN = 1;  //ipis0213
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashFwAlarmTime;
	
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMoldUser->TimeSet.TopDeflashSecTime;
				pMold->TopDeflash.Step = 440;
	
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.TopDeflashSecTime = pMoldUser->TimeSet.TopDeflashSecTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.TopDeflashSecTime = pMoldUser->TimeSet.TopDeflashSecTime/100.0;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 500;
			}
				
			break;
		case 440:/*Topdeflash second forward */
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
		
			
			pMold->ValveOut.TopDeflashCool = 1;/*Albert*/
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw2nd;/*Albert*/
			pMold->TopDeflash.v_set = pTopDeflashPara->V_Fw2nd;/*Albert*/
			
			pMoldUser->TimeDis.TopDeflashSecTime = pMold->TopDeflash.Timer.ET;
			
			if (1== pMold->Option.PunchHandle)
			{
				if ((1 == pMold->TopDeflash.Timer.Q )|| (1 == pMold->TransDIn.TopDeflashFwLimit))
				{
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
					//					pMold->CoolDeflash.Step =500; //ipis0731  //owen1021
					pMold->TopDeflash.Timer.IN   = 0;
					pMold->TopDeflash.LimitTimer.IN = 0;
					//	pMoldUser->TimeDis.TopDeflashSecTime =0;
					pMold->TopDeflash.p_set = 0;/*Albert*/
					pMold->TopDeflash.v_set = 0;
					pMold->TopDeflash.Step = 450;
					//				if ((1 == pMold->TransDIn.TopDeflashFwLimit))
					//				{		
					//					if(1 == gMachineInfo.Auto)
					//					{
					//						pMold->TopDeflash.Step = 500;
					//					}
					//					else
					//					{
					//						pMold->TopDeflash.Step = 2900;
					//					}
					//				}

				}
			}
			else
			{
				if ((1 == pMold->TopDeflash.Timer.Q )||(1 == pMold->TransDIn.TopDeflashFwLimit))
				{
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.tTopDeflash2 = pMold->TopDeflash.LimitTimer.ET/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.tTopDeflash2 = pMold->TopDeflash.LimitTimer.ET/100.0;
					}
					
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
					pMold->TopDeflash.Timer.IN   = 0;
					pMold->TopDeflash.LimitTimer.IN = 0;
					//	pMoldUser->TimeDis.TopDeflashSecTime =0;
					pMold->TopDeflash.p_set = 0;/*Albert*/
					pMold->TopDeflash.Step = 450;
				
					if(1 == gMachineInfo.Auto)
					{
						pMold->TopDeflash.Step = 500;
					}
					else
					{
						pMold->TopDeflash.Step = 2900;
					}
			

				}
			
			
			}
	
			
			pMold->TimeDis.TopDeflashFwAlarmTime = pMold->TopDeflash.LimitTimer.ET;

			//			if (1 == pMold->TopDeflash.LimitTimer.Q )
			//			{
			//				pMold->ValveOut.TopDeflashFw = 0;
			//				pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			//				pMold->CoolDeflash.Step =500; //ipis0731
			//				pMold->TopDeflash.p_set = 0;/*Albert*/
			//			
			//				pMold->Alarm.TopDeflashNotAtFwPos=1;
			//				pMold->TopDeflash.Timer.IN   = 0;
			//				pMold->TopDeflash.LimitTimer.IN = 0;
			//				pMold->TopDeflash.Step = 40000;
			//			}
			
			break;
		
		case 450:
			
			if ((1 == pMold->TransDIn.TopDeflashFwLimit))
			{		
				if(1 == gMachineInfo.Auto)
				{
					pMold->TopDeflash.Step = 500;
				}
				else
				{
					pMold->TopDeflash.Step = 2900;
				}
			}
			pMold->TimeDis.TopDeflashFwAlarmTime = pMold->TopDeflash.LimitTimer.ET;

			//			if (1 == pMold->TopDeflash.LimitTimer.Q )
			//			{
			//				pMold->ValveOut.TopDeflashFw = 0;
			//				pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			//				pMold->CoolDeflash.Step =500; //ipis0731
			//				pMold->TopDeflash.p_set = 0;/*Albert*/
			//			
			//				pMold->Alarm.TopDeflashNotAtFwPos=1;
			//				pMold->TopDeflash.Timer.IN   = 0;
			//				pMold->TopDeflash.LimitTimer.IN = 0;
			//				pMold->TopDeflash.Step = 40000;
			//			}
		
			break;
		
		/*----------------------------------------------*/
		/*No Time Movement-move unitl reach limit sensor*/
		/*----------------------------------------------*/
		/*----------------------------*/
		case 499:
			/*----------------------------*/
			
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw;/*Albert*/
			
			if(1 == pMold->TransDIn.TopDeflashFwLimit) /*2018.7.16 Albert*/
			{
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
				//	pMold->TimeDis.TopDeflashFwTime = 0;
			
				pMold->TopDeflash.p_set = 0;/*Albert*/
				if(1 == gMachineInfo.Auto)
				{
					pMold->TopDeflash.Step = 500;
				}
				else
				{
					pMold->TopDeflash.Step = 2900;
				}
			}
			
			pMold->TimeDis.TopDeflashFwAlarmTime = pMold->TopDeflash.LimitTimer.ET;

			if (1 == pMold->TopDeflash.LimitTimer.Q )
			{
				pMold->Alarm.TopDeflashNotAtFwPos=1;
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
				pMold->TopDeflash.Step = 40000;
			}
			
			
			break;
		
		/* ------------------------- handle forward ------------------------- */	
		case 500:
			HandledeflashCount=0;
			if(pMold->Option.PunchHandle)
			{	
				if(pMold->TimeSet.TopDeflashClsDelay != 0)
				{
					pMold->TopDeflash.Timer.IN = 1;
					pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashClsDelay;
					pMold->TopDeflash.Step 	= 600;
				}
				else
				{
					pMold->TopDeflash.Step = 700;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 900;
			}
			break;
								
		case 600:				/* handle delay ok */
			pMold->TimeDis.TopDeflashClsDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN        = 0;
				//	pMold->TimeDis.TopDeflashClsDelay = 0;
				pMold->TopDeflash.Step = 700;
			}
			break;
            		
		case 700:
			if(pMold->TimeSet.TopDeflashClsTime != 0)
			{
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashClsTime;
				pMold->TopDeflash.Step = 800;
			}
			else
			{
				pMold->TopDeflash.Step = 900;	
			}
			break;
            	
		case 800:				/* TopDeflash handle time ok */
            	
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashCls = 1;
				
			pMold->TimeDis.TopDeflashClsTime = pMold->TopDeflash.Timer.ET;
			if (1 == pMold->TopDeflash.Timer.Q )
			{
				pMold->TopDeflash.Timer.IN = 0;
				//	pMold->TimeDis.TopDeflashClsTime = 0;
            		
				//					if(1 == gMachineInfo.TimeLimit)
				//					{
				//						pMold->TopDeflash.Step = 900;
				//					}
				//					else
				//					{
				//						pMold->TopDeflash.Step = 2800;
				//					}
				pMold->TopDeflash.Step = 1100;
				
			}
			break;
		
		/* ------------------------- handle back ------------------------- */
		case 900:
			if(pMold->Option.PunchHandle)
			{
				if(pMold->TimeSet.TopDeflashOpnDelay != 0)
				{
					pMold->TopDeflash.Timer.IN = 1;
					pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashOpnDelay;
					pMold->TopDeflash.Step 	= 1000;
				}
				else
				{
					pMold->TopDeflash.Step = 1100;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 1100;
			}
			break;
				
		case 1000:
			pMold->TimeDis.TopDeflashOpnDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN        = 0;
				pMold->TimeDis.TopDeflashOpnDelay = 0;
				pMold->TopDeflash.Step = 1100;
			}
			break;
            		
		case 1100:

			
			if (1==pMold->Option.PunchHandle)
			{
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashOpnAlarmTime;
				pMold->TopDeflash.Step = 1200;
			}
			else
			{
				//	pMold->TopDeflash.Step = 1210;
				pMold->TopDeflash.Step = 1300; //ipis0213
			}
		
			break;
				
		case 1200: 
				
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;

			if (1 == pMold->TransDIn.TopDeflashOpnLimit)
			{
				pMold->TopDeflash.Timer.IN = 0;

				//				if(1==gMachineFix.Option.bHandledeflashRelpy )   /* Handledeflash 2 time*/
				//				{
				//					pMold->TopDeflash.Step = 1250;	
				//				}
				//				else
				//				{
				//					pMold->TopDeflash.Step = 1300;
				//				}
				HandledeflashCount = HandledeflashCount+1;
			
				if (0==gUserPara.HandledeflashCount || (gUserPara.HandledeflashCount == HandledeflashCount))
				{
					HandledeflashCount=0;
					pMold->TopDeflash.Step = 1300;
				}
				else
				{			
					pMold->TopDeflash.Step = 700;
				}
					
			
				//	pMold->TopDeflash.Step = 1300;
			
			}
		
			pMold->TimeDis.TopDeflashOpnAlarmTime = pMold->TopDeflash.Timer.ET;
				
			if(1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN = 0;
				pMold->Alarm.TopDeflashNotAtOpnPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			}
			break;
		
		case 1210: // for no use 
				
			

			
			if (1 == pMold->TransDIn.TopDeflashOpnLimit)
			{
				pMold->TopDeflash.Timer.IN = 0;
				if(pMold->Option.TopDeflash)
				{
					pMold->TopDeflash.Step = 1300;		 // topdeflash bw
				}
				else
				{
					pMold->TopDeflash.Step = 3000;	
				}
							
		
			}
			else		
			{	
				pMold->TopDeflash.Timer.IN = 0;
				pMold->Alarm.TopDeflashNotAtOpnPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			
			}
	
		
			break;
		
		
		case 1250:
			if(pMold->TimeSet.TopDeflashClsTime != 0)
			{
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashClsTime;
				pMold->TopDeflash.Step = 1260;
			}
			else
			{
				pMold->TopDeflash.Step = 1270;	
			}
			break;
            	
		case 1260:				/* TopDeflash handle time ok */
            	
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashCls = 1;
				
			pMold->TimeDis.TopDeflashClsTime = pMold->TopDeflash.Timer.ET;
			if (1 == pMold->TopDeflash.Timer.Q )
			{
				pMold->TopDeflash.Timer.IN = 0;
				//	pMold->TimeDis.TopDeflashClsTime = 0;
            		
				pMold->TopDeflash.Step = 1270;
				
			}
			break;
		
		case 1270:
			pMold->TopDeflash.Timer.IN = 1;
			pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashOpnAlarmTime;
			pMold->TopDeflash.Step = 1280;
			break;
				
		case 1280:
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;

			if (1 == pMold->TransDIn.TopDeflashOpnLimit)
			{
				pMold->TopDeflash.Timer.IN = 0;
							

				//	pMold->TopDeflash.Step = 2800;
							
				pMold->TopDeflash.Step = 1300;
			
			}
		
			pMold->TimeDis.TopDeflashOpnAlarmTime = pMold->TopDeflash.Timer.ET;
				
			if(1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN = 0;
				pMold->Alarm.TopDeflashNotAtOpnPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			}
			break;

		
		
	
		
		
		
		
		/* ------------------------- topdeflash backward ------------------------- */
		case 1300:
			if(pMold->Option.TopDeflash)
			{
				switch (pTopDeflashFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.TopDeflashBwDelay != 0)
						{
							pMold->TopDeflash.Timer.IN = 1;
							pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashBwDelay;
							pMold->TopDeflash.Step = 1400;
					
							//gExSPC
							if(pMold == & LMold)
							{
								gExSPC[ExIndex].LMold.TopDeflashBwDelay = pMold->TimeSet.TopDeflashBwDelay/100.0;
							}
							else
							{
								gExSPC[ExIndex].RMold.TopDeflashBwDelay = pMold->TimeSet.TopDeflashBwDelay/100.0;
							}
						}
						else
						{
							pMold->TopDeflash.Step = 1500;
						}
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						pMold->TopDeflash.Step = 1500;
						break;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 1500;
			}
			break;
				
		case 1400:				/* TopDeflash backward delay ok */
			pMold->TimeDis.TopDeflashBwDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->TopDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashBwDelay = 0;
				pMold->TopDeflash.Step = 1500;
			}
			break;
            		
		case 1500:
			if(pMold->Option.TopDeflash)
			{
				pMold->TopDeflash.LimitTimer.IN = 1; //ipis0213
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashBwAlarmTime;
				pMold->TopDeflash.Step = 1600;
			}
			else
			{
				pMold->TopDeflash.Step = 1700;   //for no use 
			}
			break;
            	
		case 1600:
				
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 1;
            	
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 1;
			pMold->ValveOut.TopDeflashCls = 0;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->TopDeflash.p_set = pTopDeflashPara->P_BwReset;
				pMold->TopDeflash.v_set = pTopDeflashPara->V_BwReset;
			}
			else
			{
				pMold->TopDeflash.p_set = pTopDeflashPara->P_Bw;/*Albert*/
				pMold->TopDeflash.v_set = pTopDeflashPara->V_Bw;
			}


			
			if (1 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
			
				pMold->CoolDeflash.Step =500;  //owen1021
			
				pMold->TopDeflash.p_set = 0;/*Albert*/
				pMold->TopDeflash.v_set = 0;

				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.Step = 2900;
				
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.tTopDeflashBw = pMold->TopDeflash.Timer.ET/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.tTopDeflashBw = pMold->TopDeflash.Timer.ET/100.0;
				}
			}

			pMold->TimeDis.TopDeflashBwAlarmTime = pMold->TopDeflash.LimitTimer.ET;
				
			//			if(1 == pMold->TopDeflash.LimitTimer.Q)
			//			{
			//				pMold->ValveOut.TopDeflashFw  = 0;
			//				pMold->ValveOut.TopDeflashBw  = 0;
			//				pMold->ValveOut.TopDeflashCls = 0;
			//					
			//				pMold->TopDeflash.Timer.IN   = 0;
			//				pMold->Alarm.TopDeflashNotAtBwPos = 1;
			//				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			//			}
			break;
			
		
		case 1700: // for no use
			
			if (1 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
			
				pMold->TopDeflash.p_set = 0;/*Albert*/

				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.Step = 2900;
			}
			else				
			{
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->Alarm.TopDeflashNotAtBwPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
				
			}
			pMold->TimeDis.TopDeflashBwAlarmTime = pMold->TopDeflash.LimitTimer.ET;
				
			//			if(1 == pMold->TopDeflash.Timer.Q)
			//			{
			//				pMold->ValveOut.TopDeflashFw  = 0;
			//				pMold->ValveOut.TopDeflashBw  = 0;
			//				pMold->ValveOut.TopDeflashCls = 0;
			//					
			//				pMold->TopDeflash.Timer.IN   = 0;
			//				pMold->Alarm.TopDeflashNotAtBwPos = 1;
			//				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			//			}
		
		
			break;
		
		/*----------for handle Top deflash force backward------------------------------*/
		case 1950: //for auto force backward
			
			pMold->TopDeflash.Timer.IN = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			pMold->TopDeflash.Step = 2000;
			break;
	
		case 2000: 
			// Top deflash Open
			if (1 == pMold->Option.PunchHandle)
			{
				pMold->TopDeflash.LimitTimer.IN = 1;
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashOpnAlarmTime;
				pMold->TopDeflash.Step = 2050;
			}
			else
			{
				pMold->TopDeflash.Step = 2100; //ipis0213
			}
			
			
			break;
		
		case 2050:
				
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;

			if (1 == pMold->TransDIn.TopDeflashOpnLimit)
			{
				pMold->TopDeflash.Timer.IN = 0;

				HandledeflashCount=0;
				pMold->TopDeflash.Step = 2100;			
	
			}
		
			pMold->TimeDis.TopDeflashOpnAlarmTime = pMold->TopDeflash.LimitTimer.ET;
				
			if(1 == pMold->TopDeflash.LimitTimer.Q)
			{
				pMold->TopDeflash.LimitTimer.IN = 0;
				pMold->Alarm.TopDeflashNotAtOpnPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			}					
			
			break;
		
		case 2100:
			// Top deflash backward
			
			if(pMold->Option.TopDeflash)
			{
				pMold->TopDeflash.LimitTimer.IN = 1;
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashBwAlarmTime;
				pMold->TopDeflash.Step = 2150;
			}
			else
			{
				pMold->TopDeflash.Step = 2900;   //for no use 
			}
			
			break;
							
		case 2150:
				
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 1;
            	
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 1;
			pMold->ValveOut.TopDeflashCls = 0;

			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Bw;/*Albert*/
			pMold->TopDeflash.v_set = pTopDeflashPara->V_Bw;
			
			if (1 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
			
				pMold->TopDeflash.p_set = 0;/*Albert*/
				pMold->TopDeflash.v_set = 0;

				pMold->TopDeflash.LimitTimer.IN   = 0;
				pMold->TopDeflash.Step = 2900;
			}

			pMold->TimeDis.TopDeflashBwAlarmTime = pMold->TopDeflash.LimitTimer.ET;
				
			if(1 == pMold->TopDeflash.Timer.Q)
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
					
				pMold->TopDeflash.LimitTimer.IN   = 0;
				pMold->Alarm.TopDeflashNotAtBwPos = 1;
				pMold->TopDeflash.Step = 40000;           /*  TopDeflash backward alarm   */
			}
			
			break;
		
		

		
		
		/*----------------------------------------------------------------------------*/	
		
		
		/* ------------------------- deflash off ------------------------- */    
		
		case 2800:
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			pMold->ValveOut.TopDeflashCls = 0;
			
					
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 0;
	
			pMold->TopDeflash.Timer.IN = 0;
			pMold->TopDeflash.Step = 3000;
			break;
		
		
		case 2900:
			if(1 == gMachineInfo.Auto)
			{
				//				if(pMold == &RMold)
				//				{
				//					RMold.Option.TopDeflash  = gMacOption.RTopDeflash;
				//					RMold.Option.PunchHandle = gMacOption.RPunchHandle;
				//				}
				//				
				//				if(pMold == &LMold)
				//				{
				//					LMold.Option.TopDeflash  = gMacOption.LTopDeflash;
				//					LMold.Option.PunchHandle = gMacOption.LPunchHandle;					
				//				}
			
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
					
				pMold->ActInfo.TopDeflashFw    = 0;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashBw = 0;
	

				/*Albert*/
				//				pMold->BlowDeflash.Step = 500;		/*  ´µÀä·É±ß½áÊø  */
				//				pMold->CoolDeflash.Step = 500;		/*  cool deflash end§ô  */ //ipis0731
				pMold->ChuteDeflashBlow.Step = 100;	/*  ´µµôUÅß   */
			
			}
			else
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
					
				pMold->ActInfo.TopDeflashFw    = 0;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashBw = 0;
			
			}
			
			pMold->TopDeflash.Timer.IN = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Step = 3000;
	
			break;
			

		case 3000:
			break;
		
		case 5000:
				
			//			pMold->ActInfo.TopDeflashFw    = 1;
			//			pMold->ActInfo.TopDeflashPuncher = 0;
			//			pMold->ActInfo.TopDeflashBw = 0;
			//            	
			//			pMold->ValveOut.TopDeflashFw  = 1;
			//			pMold->ValveOut.TopDeflashBw  = 0;
			//			pMold->ValveOut.TopDeflashCls = 0;
			//			pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			//			
			//			pMold->TopDeflash.p_set = pTopDeflashPara->P_CalibFw;/*Albert*/
			
			
			if(0 == pMold->TransDIn.TopDeflashFwLimit)
			{	
				pMold->ActInfo.TopDeflashFw    = 1;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashBw = 0;
           	
				pMold->ValveOut.TopDeflashFw  = 1;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
				pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			
				//	pMold->TopDeflash.p_set = pTopDeflashPara->P_CalibFw;/*Albert*/
				pMold->TopDeflash.p_set = pTopDeflashFix->pCalibMax;
				pMold->TopDeflash.v_set = pTopDeflashFix->vCalibMax;
			}
			else
			{
				pMold->ActInfo.TopDeflashFw    = 0;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashBw = 0;
            	
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
			
				pMold->TopDeflash.p_set = 0;
				pMold->TopDeflash.v_set = 0;
			
				pMold->TopDeflash.Step = 20000;
			}
			
			
			break;
            	
		case 8000:							
				
			pMold->ActInfo.TopDeflashFw    = 0;
			pMold->ActInfo.TopDeflashPuncher = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 1;
			pMold->ValveOut.TopDeflashCls = 0;
			break;
            	
		case 15000:
			//				
			//			pMold->ActInfo.TopDeflashBw    = 1;
			//			pMold->ActInfo.TopDeflashPuncher = 0;
			//			pMold->ActInfo.TopDeflashFw = 0;
			//            	
			//			pMold->ValveOut.TopDeflashFw  = 0;
			//			pMold->ValveOut.TopDeflashBw  = 1;
			//			pMold->ValveOut.TopDeflashCls = 0;
			//			pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			//			
			//			//pMold->TopDeflash.p_set = pTopDeflashPara->P_Bw;/*Albert*/
			//			pMold->TopDeflash.p_set = pTopDeflashPara->P_CalibBw;/*Albert*/
			//			
			
			
			if(0 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->ActInfo.TopDeflashBw    = 1;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashFw = 0;
            	
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 1;
				pMold->ValveOut.TopDeflashCls = 0;
				pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			
				//pMold->TopDeflash.p_set = pTopDeflashPara->P_Bw;/*Albert*/
				//	pMold->TopDeflash.p_set = pTopDeflashPara->P_CalibBw;/*Albert*/
				pMold->TopDeflash.p_set = pTopDeflashFix->pCalibZero;/*ipis20190131*/
				pMold->TopDeflash.v_set = pTopDeflashFix->vCalibZero;/*ipis20190131*/
			
			}
			else
			{
				
				pMold->ActInfo.TopDeflashBw    = 0;
				pMold->ActInfo.TopDeflashPuncher = 0;
				pMold->ActInfo.TopDeflashFw = 0;
           	
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
				pMold->ValveOut.TopDeflashCls = 0;
				pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
			
				pMold->TopDeflash.p_set = 0;/*Albert*/
				pMold->TopDeflash.v_set = 0;
			
				pMold->TopDeflash.Step = 20000;
			}
			
			
			break;
		
		/*--------------- Manual action stop -----------------*/
		case 20000:
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
				
					pMold->ValveOut.TopDeflashFw  = 0;
					pMold->ValveOut.TopDeflashBw  = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
            	
					pMold->ActInfo.TopDeflashFw    = 0;
					pMold->ActInfo.TopDeflashPuncher = 0;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					
					break;
			}
			
			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
				
			pMold->TopDeflash.Step 	 = 0;
			break;
				
		/*--------------- Instance stop -----------------*/	
		case 30000:
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
				
					pMold->ValveOut.TopDeflashFw  = 0;
					pMold->ValveOut.TopDeflashBw  = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
            	
					pMold->ActInfo.TopDeflashFw    = 0;
					pMold->ActInfo.TopDeflashPuncher = 0;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					
					break;
			}
			
			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
            	
			pMold->TopDeflash.Step     = 0;
			break;
		
		/*--------------- Error  stop -----------------*/	
		case 40000:
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
				
					pMold->ValveOut.TopDeflashFw  = 0;
					pMold->ValveOut.TopDeflashBw  = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
            	
					pMold->ActInfo.TopDeflashFw    = 0;
					pMold->ActInfo.TopDeflashPuncher = 0;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					
					break;
			}
			
			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			
			break;
			
		case 41000:
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
				
					pMold->ValveOut.TopDeflashFw  = 0;
					pMold->ValveOut.TopDeflashBw  = 0;
					pMold->ValveOut.TopDeflashCool = 0;/*Albert*/
            	
					pMold->ActInfo.TopDeflashFw    = 0;
					pMold->ActInfo.TopDeflashPuncher = 0;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					
					break;
			}
			
			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			
			break;
	}/* end of switch */
	
	
	/* ----------------- TopDeflash Fw / Bw timeout --------------------  */
	if(1 == pMold->TopDeflash.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->TopDeflash.Step < 500 && pMold->TopDeflash.Step > 300)		/* TopDeflash Fw  */
		{
			pMold->Alarm.TopDeflashNotAtFwPos = 1;
			pMold->CoolDeflash.Step =500; //ipis0731
		}
      
		if(pMold->TopDeflash.Step < 1700 && pMold->TopDeflash.Step > 1400)	/* TopDeflash Bw  */
		{
			pMold->Alarm.TopDeflashNotAtBwPos = 1;
		}
      
		pMold->TopDeflash.Timer.IN   = 0;
		pMold->TopDeflash.LimitTimer.IN = 0;
		pMold->TopDeflash.Step = 40000;
		
	}
	
	
	/* ----------------- TopDeflash action limit --------------------  */
	if((pMold->TopDeflash.Step > 100 && pMold->TopDeflash.Step < 500) /*|| (5000 == pMold->TopDeflash.Step) */)
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
	 	
		if(0 == pMold->TransDIn.TopDeflashOpnLimit)
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
			pMold->TopDeflash.Step = 40000;
		}
	 	
		/* robot up or forward */
		//		if(0 == pMold->TransDIn.RobotUpLimit && 0 == pMold->TransDIn.RobotBwLimit)
		//		{
		//			pMold->Alarm.RobotNotAtUpPos = ! pMold->TransDIn.RobotUpLimit;
		//			pMold->Alarm.RobotNotAtBwPos = ! pMold->TransDIn.RobotBwLimit;
		//			pMold->TopDeflash.Step = 40000;
		//		}
		/*Chaoi*/
		if(1 == gMachineFix.Option.bRobotFwAfterTopdeflash)
		{
			if(0 == pMold->Robot.Transfer.BwPos)
			{
				pMold->Alarm.RobotNotAtBwPos = ! pMold->Robot.Transfer.BwPos;
				pMold->TopDeflash.Step = 40000;
			}
		}

		
		if (0 == pMold->TransDIn.TopDeflashOpnLimit ) 
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
			pMold->TopDeflash.Step = 40000;
		}
		/*----------------------------------------------------------*/
		
		
	}/* if(pMold->TopDeflash.Step > 100 && pMold->TopDeflash.Step < 900) */
	 
	
	if((pMold->TopDeflash.Step > 1300 && pMold->TopDeflash.Step < 1700))
	{
	
		if (0 == pMold->TransDIn.TopDeflashOpnLimit )
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
			pMold->TopDeflash.Step = 40000;
		}
	
	}
	
	
}/*end of function */





