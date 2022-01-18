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

void TopDeflash( Mold_typ * pMold, TopDeflash_Fix_typ * pTopDeflashFix, TopDeflash_Para_typ * pTopDeflashPara,Mold_Para_User_typ * pMoldUser);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.TopDeflash.Step = 0;
	RMold.TopDeflash.Step = 0;
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

	LMold.TransDIn.TopDeflashBwLimit = gMachineIn.LMold.TopDeflashBwLimit;  
	LMold.TransDIn.TopDeflashFwLimit = gMachineIn.LMold.TopDeflashFwLimit;

	RMold.TransDIn.TopDeflashBwLimit = gMachineIn.RMold.TopDeflashBwLimit;
	RMold.TransDIn.TopDeflashFwLimit = gMachineIn.RMold.TopDeflashFwLimit;

	TopDeflash(&LMold,&gMachineFix.MoldL.TopDeflash,&gMachinePara.MoldL.TopDeflash,&gUserPara.LMold);
	TopDeflash(&RMold,&gMachineFix.MoldR.TopDeflash,&gMachinePara.MoldR.TopDeflash,&gUserPara.RMold);
	
	gMachineOut.LMold.TopDeflashFw  = LMold.ValveOut.TopDeflashFw;
	gMachineOut.RMold.TopDeflashFw  = RMold.ValveOut.TopDeflashFw;
	
	gMachineOut.LMold.TopDeflashBw  = LMold.ValveOut.TopDeflashBw;
	gMachineOut.RMold.TopDeflashBw  = RMold.ValveOut.TopDeflashBw;
	
	
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
			
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}	
			
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;
			
			pMold->TopDeflash.Timer.IN	= 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			
			pMold->TopDeflash.AutoTimeOutFlag = 0;
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
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw;/*Albert*/
				
			pMold->TimeDis.TopDeflashPreFwTime = pMold->TopDeflash.Timer.ET;
			if (1 == pMold->TopDeflash.Timer.Q )
			{
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
					
				pMold->TopDeflash.Timer.IN = 0;
				pMold->TimeDis.TopDeflashPreFwTime = 0;
					
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
				pMold->TopDeflash.Step = 100;	/*  Deflash forward */
			}
	
			if(0 == pMold->Option.TopDeflash)
			{
				pMold->CoolPin.Step     = 100;	/*  cool pin down    */
				pMold->CoolPinBlow.Step = 100;	/*  cool pin blowing  */
			}
				
			if(1 == pMold->Option.CoolPin && 1 == pMold->Option.TopDeflash)
			{
				if( 0 == pMold->Option.PunchHandle)
				{
					pMold->CoolPin.Step     = 100;/*  cool pin down      */
					pMold->CoolPinBlow.Step = 100;/*  cool pin blowing  */
				}
				else
				{
					pMold->TopDeflash.Step = 100;/*  Deflash forward    */
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
		
		/*--------------------------------------------------- */
		/*-------------- Topdeflash Fw  --------------------- */
		/*--------------------------------------------------- */
		case 100: 			/* forward delay*/
			
			pMold->CutNeck.Step = 100;   	/* 旋切瓶口 */
			if( pTopDeflashFix->eActuatorType ==  ACTUATOR_PNEU)			
			{
				pMold->CoolDeflash.Step =100; 	/* 打手把吹冷*/
			}
			
			if(pMold->Option.TopDeflash)
			{
				if(pMold->TimeSet.TopDeflashFwDelay != 0)
				{
					pMold->TopDeflash.Timer.IN = 1;
					pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashFwDelay;
					pMold->TopDeflash.Step 	= 200;
				}
				else
				{
					pMold->TopDeflash.Step = 300;
				}
			}
			else
			{
				pMold->TopDeflash.Step = 300;
			}
			break;
             	
		case 200: 			/* forward delay ok */
			pMold->TimeDis.TopDeflashFwDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q || 1 == pMold->TopDeflash.AutoTimeOutFlag)
			{
				pMold->TopDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashFwDelay = 0;
				pMold->TopDeflash.Step = 300;
			}
			break;
            	
		case 300:	/* set time */	
			if(pMold->Option.TopDeflash)
			{
				switch (pTopDeflashFix->eActuatorType)
				{
					/*-------------------------------------------------*/
					case ACTUATOR_HYD: /*1:Oil*/
						/*-------------------------------------------------*/
						if(pMold->TimeSet.TopDeflashFwTime != 0)
						{
							pMold->TopDeflash.LimitTimer.IN = 1; 
							pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashFwAlarmTime;
					
							pMold->TopDeflash.Timer.IN = 1;
							pMold->TopDeflash.Timer.PT = pMold->TimeSet.TopDeflashFwTime;
							pMold->TopDeflash.Step = 400;		
						}
						else
						{
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
							pMold->TopDeflash.Step = 2900; 
						}
						break;
				}
			}
			else			
			{
				pMold->TopDeflash.Step = 2900;  // for no use
			}
			break;
            
		case 400:	/* Forward 1 */
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
			
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw;
			pMold->TopDeflash.v_set = pTopDeflashPara->V_Fw;
			
			pMold->TimeDis.TopDeflashFwTime = pMold->TopDeflash.Timer.ET;

			if (1 == pMold->TopDeflash.Timer.Q || (1 == pMold->TransDIn.TopDeflashFwLimit))
			{
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
		
				pMold->TopDeflash.Step = 410;
			}

			break;
		
		case 410:/* Forward 2 Delay*/
			
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
						
						if( 1 == gMachineInfo.Auto)
						{
							pMold->CoolDeflash.Step =100; 	/* 打手把吹冷*/
						}
					}
					else
					{
						pMold->TopDeflash.Step = 430;
					}

					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					pMold->ActInfo.TopDeflashFw = 1;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->ValveOut.TopDeflashFw  = 1;
					pMold->ValveOut.TopDeflashBw  = 0;
						
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
				
					pMold->TopDeflash.Step = 500;

					break;
			}
						
			break;
		case 420: /* forward 2 delay ok */
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
			
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;
			
			pMoldUser->TimeDis.TopDeflashSecDelay = pMold->TopDeflash.Timer.ET;
			if(pMold->TopDeflash.Timer.Q ||  1 == pMold->TopDeflash.AutoTimeOutFlag)
			{
				pMold->TopDeflash.Timer.IN = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
				//	pMoldUser->TimeDis.TopDeflashSecDelay =0;
				pMold->TopDeflash.Step = 430;
			}			
			break;

		case 430:/* Forward 2 time set*/

			if(pMoldUser->TimeSet.TopDeflashSecTime != 0)
			{
				pMold->TopDeflash.LimitTimer.IN = 1;  //ipis0213
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashFwAlarmTime;
	
				pMold->TopDeflash.Timer.IN = 1;
				pMold->TopDeflash.Timer.PT = pMoldUser->TimeSet.TopDeflashSecTime;
				pMold->TopDeflash.Step = 440;
			}
			else
			{
				pMold->TopDeflash.Step = 500;
			}
				
			break;
		case 440:/* forward 2 */
			pMold->ActInfo.TopDeflashFw = 1;
			pMold->ActInfo.TopDeflashBw = 0;
			
			pMold->ValveOut.TopDeflashFw  = 1;
			pMold->ValveOut.TopDeflashBw  = 0;
		
			pMold->TopDeflash.p_set = pTopDeflashPara->P_Fw2nd;
			pMold->TopDeflash.v_set = pTopDeflashPara->V_Fw2nd;
			
			pMoldUser->TimeDis.TopDeflashSecTime = pMold->TopDeflash.Timer.ET;
			if ((1 == pMold->TopDeflash.Timer.Q )||(1 == pMold->TransDIn.TopDeflashFwLimit))
			{
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->ValveOut.TopDeflashBw = 0;
				
				pMold->TopDeflash.Timer.IN   = 0;
				pMold->TopDeflash.LimitTimer.IN = 0;
	
				if(1 == gMachineInfo.Auto)
				{
					pMold->TopDeflash.Step = 500;
				}
				else
				{
					pMold->TopDeflash.Step = 2900;
				}
			}
			
			break;
		
		case 500:
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ActInfo.TopDeflashFw = 0;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->ValveOut.TopDeflashFw  = 0;
					pMold->ValveOut.TopDeflashBw  = 0;
		
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
		
					pMold->TopDeflash.Timer.IN = 0;
					pMold->TopDeflash.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						if(1 == pMold->Option.PunchHandle)
						{
							pMold->TopDeflash.Step = 2000; // Topdeflash opn/cls + Topdeflash bw
						}
						else
						{
							pMold->TopDeflash.Step = 3000;	// Topdeflash bw
						}
					}
					else
					{
						pMold->TopDeflash.Step = 3000;
					}
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/	
				
					pMold->ActInfo.TopDeflashFw = 1;
					pMold->ActInfo.TopDeflashBw = 0;
			
					pMold->ValveOut.TopDeflashFw  = 1;
					pMold->ValveOut.TopDeflashBw  = 0;
						
					pMold->TopDeflash.p_set = 0;
					pMold->TopDeflash.v_set = 0;
				
					pMold->TopDeflash.Timer.IN = 0;
					pMold->TopDeflash.LimitTimer.IN = 0;
				
					if(1 == gMachineInfo.Auto)
					{
						if(1 == pMold->Option.PunchHandle)
						{
							pMold->TopDeflash.Step = 2000; // Topdeflash opn/cls + Topdeflash fw
						}
						else
						{
							pMold->TopDeflash.Step = 3000; // Topdeflash bw
						}
					}
					else
					{
						pMold->TopDeflash.Step = 3000;
					}
				
					break;
			}
		
			break;
		
		/*---------------------------------------------------------*/
		case 2000:/* Topdeflash opn/cls  */

			if(1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->TopDeflashOpnCls.Step = 100;
				pMold->TopDeflash.Step = 2100;
			}
			else
			{
				pMold->Alarm.TopDeflashTimeShort = 1; // Skip Topdeflash opn/cls
				pMold->TopDeflashOpnCls.Step = 0; // Skip Topdeflash opn/cls
				pMold->TopDeflash.Step = 3000;
			}

			pMold->ChuteDeflashBlow.Step = 100;	/*  吹掉廢胚   */
			
			break;
		
		case 2100:/* Topdeflash opn/cls Done  */
			if(13000 == pMold->TopDeflashOpnCls.Step)
			{
				pMold->TopDeflashOpnCls.Step = 0;
				pMold->TopDeflash.Step = 3000;
			}
			
			if(40000 == pMold->TopDeflashOpnCls.Step )	/* 打手挽故障 */
			{
				pMold->TopDeflash.Step = 41000;
			}
			
			break;
		
		case 2900:  // for no use
	
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
		
			pMold->ValveOut.TopDeflashFw  = 0;
			pMold->ValveOut.TopDeflashBw  = 0;
	
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;
	
			pMold->TopDeflash.Timer.IN = 0;
			pMold->TopDeflash.LimitTimer.IN = 0;
			
			pMold->TopDeflash.Step = 3000;
	
			break;
			

		case 3000:
			
			if(1 == gMachineInfo.Auto)
			{
				pMold->TopDeflash.Step = 10100;
			}
			
			break;
		
		/* ---------------------- calibration Fw ---------------------- */
		case 5000:
				
			if(0 == pMold->TransDIn.TopDeflashFwLimit)
			{	
				pMold->ActInfo.TopDeflashFw = 1;
				pMold->ActInfo.TopDeflashBw = 0;
           	
				pMold->ValveOut.TopDeflashFw  = 1;
				pMold->ValveOut.TopDeflashBw  = 0;
			
				pMold->TopDeflash.p_set = pTopDeflashFix->pCalibMax;
				pMold->TopDeflash.v_set = pTopDeflashFix->vCalibMax;
			}
			else
			{
				pMold->ActInfo.TopDeflashFw = 0;
				pMold->ActInfo.TopDeflashBw = 0;
           	
				pMold->ValveOut.TopDeflashFw  = 0;
				pMold->ValveOut.TopDeflashBw  = 0;
			
				pMold->TopDeflash.p_set = 0;
				pMold->TopDeflash.v_set = 0;
			
			}
			break;
		
		/*--------------------------------------------------- */
		/*-------------- Topdeflash Bw  --------------------- */
		/*--------------------------------------------------- */
		case 10100:
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
							pMold->TopDeflash.Step = 10200;
				
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
							pMold->TopDeflash.Step = 10300;
						}
						break;
					/*-------------------------------------------------*/
					case ACTUATOR_PNEU: /*2:Air*/
						/*-------------------------------------------------*/
						pMold->TopDeflash.Step = 10300;
						break;
				}

			}
			else
			{
				pMold->TopDeflash.Step = 10300;
			}
			break;
				
		case 10200:				/* TopDeflash backward delay ok */
			pMold->TimeDis.TopDeflashBwDelay = pMold->TopDeflash.Timer.ET;  
			if (1 == pMold->TopDeflash.Timer.Q ||  1 == pMold->TopDeflash.AutoTimeOutFlag)
			{
				pMold->TopDeflash.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashBwDelay = 0;
				pMold->TopDeflash.Step = 10300;
			}
			break;
            		
		case 10300:
			if(pMold->Option.TopDeflash)
			{
				pMold->TopDeflash.LimitTimer.IN = 1;
				pMold->TopDeflash.LimitTimer.PT = pMold->TimeSet.TopDeflashBwAlarmTime;
				pMold->TopDeflash.Step = 10400;
			}
			else  // for no use check limit
			{	
				if (1 == pMold->TransDIn.TopDeflashBwLimit)
				{
					pMold->TopDeflash.Step = 12900;  // for no use
				}
				else				
				{
					pMold->Alarm.TopDeflashNotAtBwPos = 1;
					pMold->TopDeflash.Step = 40000;           /*  Topdeflash bw alarm   */
				}
			}
			break;
            	
		case 10400:	
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 1;
            	
			pMold->ValveOut.TopDeflashFw = 0;
			pMold->ValveOut.TopDeflashBw = 1;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->TopDeflash.p_set = pTopDeflashPara->P_BwReset;
				pMold->TopDeflash.v_set = pTopDeflashPara->V_BwReset;
			}
			else
			{
				pMold->TopDeflash.p_set = pTopDeflashPara->P_Bw;
				pMold->TopDeflash.v_set = pTopDeflashPara->V_Bw;
			}


			if (1 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->TopDeflash.Step = 10500;
				
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

			break;
			
		
		case 10500: 
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			pMold->ValveOut.TopDeflashFw = 0;
			pMold->ValveOut.TopDeflashBw = 0;
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
			pMold->TopDeflash.Step = 13000;
			
			pMold->CoolDeflash.Step = 500; 	/* force stop 打手把吹冷*/
		
			break;
		
		case 12900: //for no use	
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			pMold->ValveOut.TopDeflashFw = 0;
			pMold->ValveOut.TopDeflashBw = 0;
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
			pMold->TopDeflash.Step = 13000;
			
			pMold->CoolDeflash.Step = 500; 	/* force stop 打手把吹冷*/
		
			break;
		
		case 13000:
		
			break;
            	
		/* ---------------------- calibration bw ---------------------- */
		case 15000:	
			
			if(0 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->ActInfo.TopDeflashFw = 0;
				pMold->ActInfo.TopDeflashBw = 1;
            	
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->ValveOut.TopDeflashBw = 1;
			
				pMold->TopDeflash.p_set = pTopDeflashFix->pCalibZero;
				pMold->TopDeflash.v_set = pTopDeflashFix->vCalibZero;
			
			}
			else
			{
				pMold->ActInfo.TopDeflashFw = 0;
				pMold->ActInfo.TopDeflashBw = 0;
            	
				pMold->ValveOut.TopDeflashFw = 0;
				pMold->ValveOut.TopDeflashBw = 0;
			
				pMold->TopDeflash.p_set = 0;
				pMold->TopDeflash.v_set = 0;
			
			}
			
			
			break;
		
		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			//TopDeflash Opn Interlock
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
				
			pMold->TopDeflash.Step 	 = 0;
			break;
				
		/*--------------- Instance stop -----------------*/	
		case 30000:
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            
			//TopDeflash Opn Interlock
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/

					break;
			}
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
            	
			pMold->TopDeflash.Step     = 0;
			break;
		
		/*--------------- Error  stop -----------------*/	
		case 40000:
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			//TopDeflash Opn Interlock
			switch (pTopDeflashFix->eActuatorType)
			{
			/*-------------------------------------------------*/
			case ACTUATOR_HYD: /*1:Oil*/
			/*-------------------------------------------------*/
			pMold->ValveOut.TopDeflashFw = 0;
			pMold->ValveOut.TopDeflashBw = 0;
			break;
			/*-------------------------------------------------*/
			case ACTUATOR_PNEU: /*2:Air*/
			/*-------------------------------------------------*/
	
			break;
			}
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
			
			break;
		
		/* ------------- auto run stop ------------- */		
		case 41000:
			pMold->ActInfo.TopDeflashFw = 0;
			pMold->ActInfo.TopDeflashBw = 0;
            	
			//TopDeflash Opn Interlock
			switch (pTopDeflashFix->eActuatorType)
			{
				/*-------------------------------------------------*/
				case ACTUATOR_HYD: /*1:Oil*/
					/*-------------------------------------------------*/
					pMold->ValveOut.TopDeflashFw = 0;
					pMold->ValveOut.TopDeflashBw = 0;
					break;
				/*-------------------------------------------------*/
				case ACTUATOR_PNEU: /*2:Air*/
					/*-------------------------------------------------*/
					break;
			}
		
			pMold->TopDeflash.p_set = 0;
			pMold->TopDeflash.v_set = 0;

			pMold->TopDeflash.Timer.IN   = 0;
			pMold->TopDeflash.LimitTimer.IN   = 0;
			
			pMold->TopDeflash.Step = 40000;
			break;
	}/* end of switch */
	
	/* ----------------- TopDeflash Fw / Bw alarm time display --------------------  */
	if(pMold->TopDeflash.Step > 300 && pMold->TopDeflash.Step  < 500)			/* TopDeflash Fw  */
	{
		pMold->TimeDis.TopDeflashFwAlarmTime = pMold->TopDeflash.LimitTimer.ET;
	}
	else if(pMold->TopDeflash.Step > 10300 && pMold->TopDeflash.Step  < 10500)	/* TopDeflash Bw   */
	{
		pMold->TimeDis.TopDeflashBwAlarmTime = pMold->TopDeflash.LimitTimer.ET;
	}
	
	
	/* ----------------- TopDeflash Fw / Bw timeout --------------------  */
	if(1 == pMold->TopDeflash.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->TopDeflash.Step < 500 && pMold->TopDeflash.Step > 300)		/* TopDeflash Fw  */
		{
			pMold->Alarm.TopDeflashNotAtFwPos = 1;
			pMold->CoolDeflash.Step = 500; 	/* force stop 打手把吹冷*/
		}
      
		if(pMold->TopDeflash.Step < 10500 && pMold->TopDeflash.Step > 10200)	/* TopDeflash Bw  */
		{
			pMold->Alarm.TopDeflashNotAtBwPos = 1;
		}
      
		pMold->TopDeflash.Step = 41000;
	}
	
	
	/* ----------------- TopDeflash Fw / Bw  action limit --------------------  */
	if( pMold->TopDeflash.Step > 100 && pMold->TopDeflash.Step < 500 ) 	 /* TopDeflash Fw  */
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
		
		if (0 == pMold->TransDIn.TopDeflashOpnLimit ) 
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
			pMold->TopDeflash.Step = 40000;
		}	
	}
	 
	if(pMold->TopDeflash.Step > 10100 && pMold->TopDeflash.Step < 10500)  /* TopDeflash Bw  */
	{
		if (0 == pMold->TransDIn.TopDeflashOpnLimit )
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
			pMold->TopDeflash.Step = 40000;
		}
	}
	
	
}/*end of function */



