/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: HighBlow
 * File: HighBlow.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program HighBlow --- 2 blowing 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void BlowCBM( Mold_typ * pMold);
void BlowStandard( Mold_typ * pMold);
void BlowStandardNew( Mold_typ * pMold, Option_Fix_typ * pOptionFix);
void BlowFreezing( Mold_typ * pMold);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.HighBlow.Step = 0;
 	RMold.HighBlow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	bHideBlowStandard = gMachineFix.Option.bBlowModeCBM;
	bHideBlowCBM      = !gMachineFix.Option.bBlowModeCBM;
	
	bHideLowBlow = !gMachineFix.Option.bLowBlow;
	
	if(0 == gPanelObj.LevelBand_4 && 0 == bHideLowBlow)
	{
		bHideLowBlowIO = 0;
	}
	else
	{
		bHideLowBlowIO = 1;	
	}
	gMachineFix.Option.bBlowModeCBM =0; //ipis0802
	if(1 == gMachineFix.Option.bBlowModeCBM)
	{
		BlowCBM(&LMold);
		BlowCBM(&RMold);
	}
	else
	{
//		BlowStandard(&LMold);
//		BlowStandard(&RMold);
		
		
		switch (gMachineFix.MoldR.Blow.eBlowMode)
		{
			case BLOW_MODE_STANDARD:
       
				BlowStandardNew(&RMold,&gMachineFix.Option);
				BlowStandardNew(&LMold,&gMachineFix.Option);
				
				break;

			case BLOW_MODE_FREEZINGBLOW:
       
				BlowFreezing(&RMold);
				BlowFreezing(&LMold);
				break;
		}
	}

//	switch(LMold.HighBlow.Out)
//	{
//		case 0:		/* 关闭  */
//			gMachineOut.LMold.HighBlow = 0;
//			gMachineOut.LMold.Exhaust = 0;
//				
//			break;
//			
//		case 2:		/* 高壓吹氣 -- 循环吹起  */	
//			gMachineOut.LMold.HighBlow  = 1;
//			gMachineOut.LMold.Exhaust = 0;
//			break;
//			
//		case 3:		/* 循环放气  */
//			gMachineOut.LMold.HighBlow  = 0;
//			gMachineOut.LMold.Exhaust = 1;
//			break;
//			
//		case 4:		/* 高壓吹風延时  */
//			gMachineOut.LMold.HighBlow  = 0;
//			gMachineOut.LMold.Exhaust = 0;
//			break;
//			
//		case 5:		/* 放气  */
//			gMachineOut.LMold.HighBlow  = 0;
//			gMachineOut.LMold.Exhaust = 1;
//			break;
//		
//		case 6:
//			gMachineOut.LMold.HighBlow  = 1;
//			gMachineOut.RMold.Exhaust = 0;
//			break;
//	}/* switch(LMold.HighBlow.Out) */
//	
//	switch(RMold.HighBlow.Out)
//	{
//		case 0:		/* 关闭  */
//			gMachineOut.RMold.HighBlow = 0;
//			gMachineOut.RMold.Exhaust = 0;
//			
//			break;
//			
//		case 2:		/* 高壓吹風 -- 循环吹風  */
//			gMachineOut.RMold.HighBlow = 1;
//			gMachineOut.RMold.Exhaust  = 0;
//			break;
//
//		case 3:		/* 循环放風  */
//			gMachineOut.RMold.HighBlow = 0;
//			gMachineOut.RMold.Exhaust  = 1;
//			break;
//			
//		case 4:		/* 高壓吹風延时  */
//			gMachineOut.RMold.HighBlow  = 0;
//			gMachineOut.RMold.Exhaust = 0;
//			break;
//			
//		case 5:		/* 放气  */
//			gMachineOut.RMold.HighBlow  = 0;
//			gMachineOut.RMold.Exhaust = 1;
//			break;
//		
//		case 6:
//			gMachineOut.RMold.HighBlow  = 1;
//			gMachineOut.RMold.Exhaust = 0;
//			break;
//
//	}/* switch(RMold.HighBlow.Out) */
//
//	gMachineOut.LMold.ExhaustForBlow2 =LMold.ValveOut.ExhaustForBlow2;
//	gMachineOut.RMold.ExhaustForBlow2 = RMold.ValveOut.ExhaustForBlow2;
	
//	gMachineOut.LMold.HighBlow =  LMold.HighBlow.Out;
//	gMachineOut.RMold.HighBlow =  RMold.HighBlow.Out;
	
	gMachineOut.RMold.HighBlow =  RMold.ValveOut.HighBlow;
	gMachineOut.LMold.HighBlow =  LMold.ValveOut.HighBlow;
	
	gMachineOut.RMold.LowBlow =  RMold.ValveOut.LowBlow;
	gMachineOut.LMold.LowBlow =  LMold.ValveOut.LowBlow;
	
	gMachineOut.RMold.FreezingBlow =  RMold.ValveOut.FreezingBlow;
	gMachineOut.LMold.FreezingBlow =  LMold.ValveOut.FreezingBlow;
	
	gMachineOut.RMold.Exhaust =  RMold.ValveOut.Exhaust;
	gMachineOut.LMold.Exhaust =  LMold.ValveOut.Exhaust;
	
	
	/*------------- BlowMode--------------- */
	gMachineFix.MoldL.Blow.eBlowMode = gMachineFix.MoldR.Blow.eBlowMode;
	
	
 } /* end of _CYCLIC */

void BlowCBM( Mold_typ * pMold)
{
	if(pMold->TimeSet.TotalBlowTime * 10 < pMold->TimeSet.BlowingTime)
	{
		pMold->TimeSet.TotalBlowTime = pMold->TimeSet.BlowingTime/10;
	}

	if(pMold->HighBlow.TimerTotal.ET > pMold->LowBlow.TimerTotal.ET)
	{
		pMold->TimeDis.TotalBlowTime = pMold->HighBlow.TimerTotal.ET/10;
	}
	else
	{
		pMold->TimeDis.TotalBlowTime = pMold->LowBlow.TimerTotal.ET/10;
	}
	
	switch ( pMold->HighBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->HighBlow.Out  		= 0;
			pMold->ActInfo.HighBlow   = 0;
			pMold->HighBlow.Timer.IN	= 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100:
			pMold->HighBlow.TimerTotal.IN = 0;
			if(pMold->TimeSet.BlowingDelay != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingDelay;
				pMold->HighBlow.Step 	   = 200;
			}
			else
			{
			//	pMold->HighBlow.Step = 300;
			pMold->HighBlow.Step = 700;
			}
			break;
            
		case 200: /* high blowing delay ok */
			pMold->HighBlow.Out = 4;
			pMold->ActInfo.HighBlow = 1;
			pMold->TimeDis.BlowingDelay = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN      = 0;
			//	pMold->TimeDis.BlowingDelay = 0;
            		
				pMold->HighBlow.Step = 300;  //ipis
		//	pMold->HighBlow.Step = 700;   // ipis
			}
			break;
	
		case 300:
			pMold->HighBlow.TimerTotal.IN = 1;
			pMold->HighBlow.TimerTotal.PT = pMold->TimeSet.TotalBlowTime * 10;

			if(pMold->LowBlow.Step > 300)
			{
				pMold->LowBlow.Step = 470;
			}

			if(pMold->TimeSet.BlowingTime != 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 400;
				

				
			}
			else
			{
				pMold->HighBlow.Step = 450;	
			}
			break;
            
		case 400:	/* high blowing time   */
			pMold->HighBlow.Out     = 2;
			pMold->ActInfo.HighBlow = 1;
			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
			//	pMold->TimeDis.BlowingTime = 0;
				pMold->HighBlow.Step = 450; /* 900; */
			}
            	
			if (1 == pMold->LowBlow.TimerTotal.Q || 1 == pMold->HighBlow.TimerTotal.Q)
			{
				pMold->HighBlow.Out = 0;
				pMold->HighBlow.TimerTotal.IN = 0;
            		
				pMold->HighBlow.Step = 480;
			}
			break;
            	
		case 450:
			pMold->HighBlow.Out     = 2;
			pMold->ActInfo.HighBlow = 1;

			if (1 == pMold->LowBlow.TimerTotal.Q || 1 == pMold->HighBlow.TimerTotal.Q)
			{
				pMold->HighBlow.Out = 0;
				pMold->HighBlow.TimerTotal.IN = 0;
				pMold->HighBlow.Step = 480;
			}
			break;
            	
		case 470:
			pMold->HighBlow.Out = 0;
			break;
            	
		case 480:
			if(pMold->LowBlow.Step < 480 && pMold->LowBlow.Step > 0)pMold->LowBlow.Step = 480;
			pMold->HighBlow.Out = 0;
			pMold->HighBlow.Timer.IN = 0;
			pMold->HighBlow.TimerTotal.IN = 0;
			pMold->HighBlow.Step = 1100;
			break;
			
		case 500:
			if((pMold->TimeSet.TotalBlowTime * 10 - pMold->TimeSet.BlowingTime) > 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.TotalBlowTime * 10 - pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 600;
			}
			else
			{
				pMold->HighBlow.Out  = 0;
				pMold->HighBlow.Step = 1100;	
			}
			break;

		case 600:
			pMold->HighBlow.tCounter++;
            	
			if(pMold->Option.IntervalBlow)
			{
				if((0 == pMold->TimeSet.InterValBlowOnTime) || (0 == pMold->TimeSet.InterValBlowOffTime))
				{
					pMold->HighBlow.Out     = 2;
					pMold->ActInfo.HighBlow = 1;
				}
				else
				{
					pMold->HighBlow.tTemp = pMold->HighBlow.tCounter % (pMold->TimeSet.InterValBlowOnTime + pMold->TimeSet.InterValBlowOffTime);
					if(pMold->HighBlow.tTemp <= pMold->TimeSet.InterValBlowOnTime)
					{
						pMold->HighBlow.Out     = 2;
						pMold->ActInfo.HighBlow = 1;
						pMold->TimeDis.InterValBlowOnTime  = pMold->HighBlow.tTemp;
					//	pMold->TimeDis.InterValBlowOffTime = 0;
					}
					else
					{
						pMold->HighBlow.Out     = 3;
						pMold->ActInfo.HighBlow = 0;
					//	pMold->TimeDis.InterValBlowOnTime = 0;
						pMold->TimeDis.InterValBlowOffTime = pMold->HighBlow.tTemp - pMold->TimeSet.InterValBlowOnTime;
					}
				}
			}/* if(pMold->Option.InterValBlow) */
			else
			{
				pMold->HighBlow.Out     = 2;
				pMold->ActInfo.HighBlow = 1;
			}
	            
			pMold->TimeDis.TotalBlowTime = (pMold->HighBlow.Timer.ET + pMold->TimeSet.BlowingTime)/10;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Out      = 0;
				pMold->HighBlow.Timer.IN = 0;
//				pMold->TimeDis.TotalBlowTime       = 0;
//				pMold->TimeDis.InterValBlowOnTime  = 0;
//				pMold->TimeDis.InterValBlowOffTime = 0;

				pMold->HighBlow.Step = 1100;
			}
			break;
            
		/*--------------------------------------------------------------------------------------*/
		
		
//		case 700:
//			if(pMold->TimeSet.BlowingTime != 0)
//			{
//				pMold->HighBlow.tCounter = 0;
//				pMold->HighBlow.Timer.IN = 1;
//				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
//				pMold->HighBlow.Step = 710;
//			}
//			else
//			{
//				pMold->HighBlow.Step = 720;	
//			}
//				
//		break;
//		
//		case 710:/* high blowing time   */
//			pMold->HighBlow.Out     = 2;
//			pMold->ActInfo.HighBlow = 1;
//			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
//			if (1 == pMold->HighBlow.Timer.Q)
//			{
//				pMold->HighBlow.Timer.IN = 0;
//				//	pMold->TimeDis.BlowingTime = 0;
//				pMold->HighBlow.Step = 720; 
//			}		
//			
//		break;
//		
//		case 720: /*AirVentingTime*/
//			
//			if(pMold->TimeSet.AirVentingTime != 0)
//			{
//				
//				pMold->HighBlow.Timer.IN = 1;
//				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;
//				pMold->HighBlow.Step = 730;
//			}
//			else
//			{
//				pMold->HighBlow.Step = 760;	
//			}
//		break;
//		
//		case 730:/* AirVentingTime   */
//			pMold->HighBlow.Out     = 3;
//			pMold->ActInfo.HighBlow = 0;
//			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET; 
//			if (1 == pMold->HighBlow.Timer.Q)
//			{
//				pMold->HighBlow.Timer.IN = 0;
//				//	pMold->TimeDis.AirVentingTime = 0;
//				pMold->HighBlow.Step = 760; 
//			}		
//			
//			break;
//		
//			
//		case 760: /* high blowing time2   */
//			if(pMold->TimeSet.BlowingTime2 != 0)
//			{
//				pMold->HighBlow.tCounter = 0;
//				pMold->HighBlow.Timer.IN = 1;
//				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime2;
//				pMold->HighBlow.Step = 770;
//			}
//			else
//			{
//				pMold->HighBlow.Step = 780;	
//			}
//				
//			break;
//		
//		case 770:/* high blowing time2   */
//			pMold->HighBlow.Out     = 2;
//			pMold->ActInfo.HighBlow = 1;
//			pMold->TimeDis.BlowingTime2 = pMold->HighBlow.Timer.ET; 
//			if (1 == pMold->HighBlow.Timer.Q)
//			{
//				pMold->HighBlow.Timer.IN = 0;
//				//	pMold->TimeDis.BlowingTime2 = 0;
//				pMold->HighBlow.Step = 780; 
//			}		
//			
//			break;
//		
//		case 780: /*AirVentingTime2*/
//			
//			if(pMold->TimeSet.AirVentingTime2 != 0)
//			{
//				
//				pMold->HighBlow.Timer.IN = 1;
//				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime2;
//				pMold->HighBlow.Step = 790;
//			}
//			else
//			{
//				pMold->HighBlow.Step = 1300;	
//			}
//			break;
//		
//		case 790:/* AirVentingTime2  */
//			pMold->HighBlow.Out     = 3;
//			pMold->ActInfo.HighBlow = 0;
//			pMold->TimeDis.AirVentingTime2 = pMold->HighBlow.Timer.ET; 
//			if (1 == pMold->HighBlow.Timer.Q)
//			{
//				pMold->HighBlow.Timer.IN = 0;
//				//	pMold->TimeDis.AirVentingTime2 = 0;
//				pMold->HighBlow.Step = 1300; 
//			}		
//			
//			break;
		
		
		
		
		
		case 1100:
			if(1 == gMachineInfo.Auto)
			{ 
//				pMold->PullBottom.Step  = 100; 		/*  拉底启动  */
            		
				/*if(pMold->BlowPin.Step < 500 && pMold->BlowPin.Step > 100)pMold->BlowPin.Step = 500;  blow pin down force end  */
            		
				pMold->LowBlow.Step = 900;				/*  低壓吹風強制結束 */
			}
            	
			pMold->ActInfo.HighBlow = 0;
            	
			if(pMold->TimeSet.AirVentingTime != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;	/*  排气时间  */

				pMold->HighBlow.Step = 1200;
			}
			else
			{
				pMold->HighBlow.Step = 1300;	
			}
			break;
            	
		case 1200:
			pMold->HighBlow.Out = 5;
			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
			//	pMold->TimeDis.AirVentingTime = 0;
				pMold->HighBlow.Step = 1300;
			}
			break;
            	
		case 1300:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
				
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//			pMold->TimeDis.AirVentingTime = 0;
				
			pMold->ActInfo.HighBlow  = 0;
				
			pMold->Lock.Step  = 10100;	/*  卸压 */
			pMold->HighBlow.Step = 3000;
			break;
		case 3000:
			break;
            	
		case 5000:
			pMold->HighBlow.Out = 6;
			break;
            	
		case 20000:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
			pMold->HighBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.InterValBlowOnTime  = 0;
//			pMold->TimeDis.InterValBlowOffTime = 0;
//
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//				
//			pMold->TimeDis.TotalBlowTime  = 0;
//			pMold->TimeDis.AirVentingTime = 0;
				
			pMold->ActInfo.HighBlow= 0;
			pMold->HighBlow.Step 	 = 0;
			break;
		
		case 30000:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
			pMold->HighBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.InterValBlowOnTime  = 0;
//			pMold->TimeDis.InterValBlowOffTime = 0;
//
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//				
//			pMold->TimeDis.TotalBlowTime  = 0;
//			pMold->TimeDis.AirVentingTime = 0;

			pMold->ActInfo.HighBlow  = 0;
			pMold->HighBlow.Step     = 0;
			break;

	}/* end of switch */
}/*end of function */

void BlowStandard( Mold_typ * pMold)
{
	if(pMold->TimeSet.TotalBlowTime * 10 < pMold->TimeSet.BlowingTime)
	{
		pMold->TimeSet.TotalBlowTime = pMold->TimeSet.BlowingTime/10;
	}

	pMold->TimeDis.TotalBlowTime = pMold->HighBlow.TimerTotal.ET/10;
	
	switch ( pMold->HighBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->HighBlow.Out  		= 0;
			pMold->ActInfo.HighBlow   = 0;
			pMold->HighBlow.Timer.IN	= 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100:
			pMold->HighBlow.TimerTotal.IN = 0;
			if(pMold->TimeSet.BlowingDelay != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingDelay;
				pMold->HighBlow.Step 	   = 200;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowingDelay = pMold->TimeSet.BlowingDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowingDelay = pMold->TimeSet.BlowingDelay/100.0;
				}
			
			}
			else
			{
				pMold->HighBlow.Step = 700;
			}
			break;
            
		case 200: /* high blowing delay ok */
			pMold->HighBlow.Out = 4;
			pMold->ActInfo.HighBlow = 1;
			pMold->TimeDis.BlowingDelay = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN      = 0;
		//		pMold->TimeDis.BlowingDelay = 0;
            		
			//	pMold->HighBlow.Step = 300;
				pMold->HighBlow.Step = 700;
			}
			break;
	
		case 300:
			
			pMold->HighBlow.Out = 4;
			pMold->ActInfo.HighBlow = 1;
			pMold->HighBlow.TimerTotal.IN = 1;
			pMold->HighBlow.TimerTotal.PT = pMold->TimeSet.TotalBlowTime * 10;

			if(pMold->TimeSet.BlowingTime != 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 400;
			}
			else
			{
				pMold->HighBlow.Step = 500;	
			}
			break;
            
		case 400:	/* high blowing time   */
			pMold->HighBlow.Out     = 2;
			pMold->ActInfo.HighBlow = 1;
			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
			//	pMold->TimeDis.BlowingTime = 0;
				pMold->HighBlow.Step = 500; 
			}
            	
			if (1 == pMold->HighBlow.TimerTotal.Q)
			{
				pMold->HighBlow.Out = 0;
				pMold->HighBlow.TimerTotal.IN = 0;
            		
				pMold->HighBlow.Step = 1100;
			}
			break;
			
		case 500:
			if((pMold->TimeSet.TotalBlowTime * 10 - pMold->TimeSet.BlowingTime) > 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.TotalBlowTime * 10 - pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 600;
			}
			else
			{
				pMold->HighBlow.Out  = 0;
				pMold->HighBlow.Step = 1100;	
			}
			break;

		case 600:
			pMold->HighBlow.tCounter++;
            	
			if(pMold->Option.IntervalBlow)
			{
				if((0 == pMold->TimeSet.InterValBlowOnTime) || (0 == pMold->TimeSet.InterValBlowOffTime))
				{
					pMold->HighBlow.Out     = 2;
					pMold->ActInfo.HighBlow = 1;
				}
				else
				{
					pMold->HighBlow.tTemp = pMold->HighBlow.tCounter % (pMold->TimeSet.InterValBlowOnTime + pMold->TimeSet.InterValBlowOffTime);
					if(pMold->HighBlow.tTemp <= pMold->TimeSet.InterValBlowOnTime)
					{
						pMold->HighBlow.Out     = 2;
						pMold->ActInfo.HighBlow = 1;
						pMold->TimeDis.InterValBlowOnTime  = pMold->HighBlow.tTemp;
					//	pMold->TimeDis.InterValBlowOffTime = 0;
					}
					else
					{
						pMold->HighBlow.Out     = 3;
						pMold->ActInfo.HighBlow = 0;
					//	pMold->TimeDis.InterValBlowOnTime = 0;
						pMold->TimeDis.InterValBlowOffTime = pMold->HighBlow.tTemp - pMold->TimeSet.InterValBlowOnTime;
					}
				}
			}/* if(pMold->Option.InterValBlow) */
			else
			{
				pMold->HighBlow.Out     = 2;
				pMold->ActInfo.HighBlow = 1;
			}
	            
			pMold->TimeDis.TotalBlowTime = (pMold->HighBlow.Timer.ET + pMold->TimeSet.BlowingTime)/10;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Out      = 0;
				pMold->HighBlow.Timer.IN = 0;
//				pMold->TimeDis.TotalBlowTime       = 0;
//				pMold->TimeDis.InterValBlowOnTime  = 0;
//				pMold->TimeDis.InterValBlowOffTime = 0;

				pMold->HighBlow.Step = 1100;
			}
			break;
            
		
		/*--------------------------------------------------------------------------------------*/
		
		
		case 700:
			if(pMold->TimeSet.BlowingTime != 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 710;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowingTime = pMold->TimeSet.BlowingTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowingTime = pMold->TimeSet.BlowingTime/100.0;
				}
			}
			else
			{
				pMold->HighBlow.Step = 720;	
			}
				
			break;
		
		case 710:/* high blowing time   */
			pMold->HighBlow.Out     = 2;
			pMold->ActInfo.HighBlow = 1;
			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.BlowingTime = 0;
				pMold->HighBlow.Step = 720; 
			}		
			
			break;
		
		case 720: /*AirVentingTime*/
			
			if(pMold->TimeSet.AirVentingTime != 0)
			{
				
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;
				pMold->HighBlow.Step = 730;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.AirVentingTime = pMold->TimeSet.AirVentingTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.AirVentingTime = pMold->TimeSet.AirVentingTime/100.0;
				}
			}
			else
			{
				pMold->HighBlow.Step = 760;	
			}
			break;
		
		case 730:/* AirVentingTime   */
			pMold->HighBlow.Out     = 3;
			pMold->ActInfo.HighBlow = 0;
			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime = 0;
				pMold->HighBlow.Step = 760; 
			}		
			
			break;
		
			
		case 760: /* high blowing time2   */
			if(pMold->TimeSet.BlowingTime2 != 0)
			{
				pMold->HighBlow.tCounter = 0;
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime2;
				pMold->HighBlow.Step = 770;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowingTime2 = pMold->TimeSet.BlowingTime2/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowingTime2 = pMold->TimeSet.BlowingTime2/100.0;
				}
				
			}
			else
			{
				pMold->HighBlow.Step = 780;	
			}
				
			break;
		
		case 770:/* high blowing time2   */
			pMold->HighBlow.Out     = 2;
			pMold->ActInfo.HighBlow = 1;
			pMold->ValveOut.ExhaustForBlow2   = 1; //ipis0219
			pMold->TimeDis.BlowingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
		//s	pMold->ValveOut.ExhaustForBlow2   = 0; //ipis0219
				//	pMold->TimeDis.BlowingTime2 = 0;
				pMold->HighBlow.Step = 780; 
			}		
			
			break;
		
		case 780: /*AirVentingTime2*/
			
			if(pMold->TimeSet.AirVentingTime2 != 0)
			{
				
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime2;
				pMold->HighBlow.Step = 790;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.AirVentingTime2 = pMold->TimeSet.AirVentingTime2/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.AirVentingTime2 = pMold->TimeSet.AirVentingTime2/100.0;
				}
			}
			else
			{
				pMold->HighBlow.Step = 1300;	
			}
			break;
		
		case 790:/* AirVentingTime2  */
			pMold->HighBlow.Out     = 3;
			pMold->ActInfo.HighBlow = 0;
			pMold->TimeDis.AirVentingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime2 = 0;
				pMold->HighBlow.Step = 1300; 
			}		
			
			break;
		
		
		
		
		
		
		
		
		
		
		
		case 1100:
			if(1 == gMachineInfo.Auto)
			{ 
				pMold->PullBottom.Step  = 100; 		/*  拉底启动  */
            		
				/*if(pMold->BlowPin.Step < 500 && pMold->BlowPin.Step > 100)pMold->BlowPin.Step = 500;   blow pin down force end  */
            		
				pMold->LowBlow.Step = 900;				/*  低壓吹風強制結束 */
			}
            	
			pMold->ActInfo.HighBlow = 0;
            	
			if(pMold->TimeSet.AirVentingTime != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;	/*  排气时间  */

				pMold->HighBlow.Step = 1200;
			}
			else
			{
				pMold->HighBlow.Step = 1300;	
			}
			break;
            	
		case 1200:
			pMold->HighBlow.Out = 5;
			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
		//		pMold->TimeDis.AirVentingTime = 0;
				pMold->HighBlow.Step = 1300;
			}
			break;
            	
		case 1300:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
				pMold->ValveOut.ExhaustForBlow2   = 0; //ipis0219
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//			pMold->TimeDis.AirVentingTime = 0;
				
			pMold->ActInfo.HighBlow  = 0;
				
			pMold->Lock.Step  = 10100;	/*  卸压 */
			pMold->HighBlow.Step = 3000;
			break;
		case 3000:
			break;
            	
		case 5000:
			pMold->HighBlow.Out = 6;
			break;
            	
		case 20000:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
			pMold->HighBlow.TimerTotal.IN = 0;
			pMold->ValveOut.ExhaustForBlow2   = 0; //ipis0219
//			pMold->TimeDis.InterValBlowOnTime  = 0;
//			pMold->TimeDis.InterValBlowOffTime = 0;
//
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//				
//			pMold->TimeDis.TotalBlowTime  = 0;
//			pMold->TimeDis.AirVentingTime = 0;
				
			pMold->ActInfo.HighBlow= 0;
			pMold->HighBlow.Step 	 = 0;
			break;
		
		case 30000:
			pMold->HighBlow.Out      = 0;
			pMold->HighBlow.Timer.IN = 0;
			pMold->HighBlow.TimerTotal.IN = 0;
				pMold->ValveOut.ExhaustForBlow2   = 0; //ipis0219
//			pMold->TimeDis.InterValBlowOnTime  = 0;
//			pMold->TimeDis.InterValBlowOffTime = 0;
//
//			pMold->TimeDis.BlowingDelay   = 0;
//			pMold->TimeDis.BlowingTime    = 0;
//				
//			pMold->TimeDis.TotalBlowTime  = 0;
//			pMold->TimeDis.AirVentingTime = 0;

			pMold->ActInfo.HighBlow  = 0;
			pMold->HighBlow.Step     = 0;
			break;
	}/* end of switch */
}/*end of function */


void BlowStandardNew( Mold_typ * pMold, Option_Fix_typ * pOptionFix)
{

	switch ( pMold->HighBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ActInfo.HighBlow   	= 0;
			pMold->ValveOut.HighBlow  	= 0;
			pMold->ValveOut.LowBlow 	= 0;

			pMold->HighBlow.Timer.IN	= 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100:
			if(pMold->TimeSet.BlowingDelay != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingDelay;
				pMold->HighBlow.Step 	   = 200;
			}
			else
			{
				pMold->HighBlow.Step = 210;
			}
			break;
            
		case 200: /* high blowing delay ok */
			pMold->TimeDis.BlowingDelay = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				pMold->HighBlow.Step = 210;
			}
			break;
		
		case 210: /*  pre Low blowing ok */
			if(pMold->TimeSet.PreBlowTime != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.PreBlowTime;
				pMold->HighBlow.Step = 220;
			}
			else
			{
				pMold->HighBlow.Step = 300;	
			}
				
			break;
		
		case 220:/* pre low blowing time   */	
			pMold->ActInfo.HighBlow = 1;
			
			pMold->ValveOut.LowBlow = 1;
			pMold->TimeDis.PreBlowTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ActInfo.HighBlow = 0;
				pMold->ValveOut.LowBlow = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.PreBlowTime = 0;
				pMold->HighBlow.Step = 300; 
			}		
			
			break;
		
		case 300:
			if(pMold->TimeSet.BlowingTime != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 400;
			}
			else
			{
				pMold->HighBlow.Step = 500;	
			}
				
			break;
		
		case 400:/* high blowing time   */	
			pMold->ActInfo.HighBlow = 1;
			
			pMold->ValveOut.HighBlow = 1;
			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ActInfo.HighBlow  = 0;
				pMold->ValveOut.HighBlow = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.BlowingTime = 0;
				pMold->HighBlow.Step = 500; 
			}		
			
			break;
		
		case 500: /*AirVentingTime Set*/
			if(pMold->TimeSet.AirVentingTime != 0)
			{	
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;
				pMold->HighBlow.Step = 600;
			}
			else
			{
				pMold->HighBlow.Step = 700;	
			}
			break;
		
		case 600:/* AirVentingTime   */
			pMold->ActInfo.HighBlow = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.LowBlow = 0;
			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime = 0;
				pMold->HighBlow.Step = 700; 
			}		
			
			break;
		
			
		case 700: /* high blowing time2 Set  */
			if(pMold->TimeSet.BlowingTime2 != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime2;
				pMold->HighBlow.Step = 800;
			}
			else
			{
				pMold->HighBlow.Step = 900;	
			}
				
			break;
		
		case 800:/* high blowing time2   */
			pMold->ActInfo.HighBlow = 1;
	
			if( 0 == pOptionFix->bBlowingTime2LowBlow)
			{
				pMold->ValveOut.HighBlow = 1;
			}
			else
			{
				pMold->ValveOut.LowBlow = 1;
			}
			
			pMold->TimeDis.BlowingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ActInfo.HighBlow  = 0;
				pMold->ValveOut.HighBlow = 0;
				pMold->ValveOut.LowBlow  = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.BlowingTime2 = 0;
				pMold->HighBlow.Step = 900; 
			}		
			
			break;
		
		case 900: /*AirVentingTime2 Set*/
			
			if(pMold->TimeSet.AirVentingTime2 != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime2;
				pMold->HighBlow.Step = 1000;
			}
			else
			{
				pMold->HighBlow.Step = 1300;	
			}
			break;
		
		case 1000:/* AirVentingTime2  */
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.LowBlow = 0;
			pMold->ActInfo.HighBlow = 0;
			pMold->TimeDis.AirVentingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime2 = 0;
				pMold->HighBlow.Step = 1300; 
			}		
			break;

            	
		case 1300:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->HighBlow.Timer.IN = 0;
			
			pMold->HighBlow.Step = 3000;
			break;
		
		case 3000:
			break;
            	
		case 5000:
			pMold->ValveOut.HighBlow = 1;

			break;
            	
		case 20000:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->HighBlow.Timer.IN = 0;

			pMold->HighBlow.Step     = 0;
			break;
		
		case 30000:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->HighBlow.Timer.IN = 0;

			pMold->HighBlow.Step     = 0;
			break;
	}/* end of switch */
}/*end of function */

void BlowFreezing( Mold_typ * pMold)
{

	switch ( pMold->HighBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.FreezingBlow = 0;
			pMold->ValveOut.Exhaust = 0;
			pMold->HighBlow.Timer.IN = 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100:
			if(pMold->TimeSet.BlowingDelay != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingDelay;
				pMold->HighBlow.Step 	   = 200;
			}
			else
			{
				pMold->HighBlow.Step = 300;
			}
			break;
            
		case 200: /* high blowing delay ok */
			pMold->TimeDis.BlowingDelay = pMold->HighBlow.Timer.ET;  
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				pMold->HighBlow.Step = 300;
			}
			break;	
		
		case 300:
			if(pMold->TimeSet.BlowingTime != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime;
				pMold->HighBlow.Step = 400;
			}
			else
			{
				pMold->HighBlow.Step = 500;	
			}
				
			break;
		
		case 400:/* high blowing time   */	
			pMold->ActInfo.HighBlow = 1;
			
			pMold->ValveOut.HighBlow = 1;
			pMold->TimeDis.BlowingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ActInfo.HighBlow  = 0;
				pMold->ValveOut.HighBlow = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.BlowingTime = 0;
				pMold->HighBlow.Step = 500; 
			}		
			
			break;
		
		case 500: /*AirVentingTime Set*/
			if(pMold->TimeSet.AirVentingTime != 0)
			{	
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime;
				pMold->HighBlow.Step = 600;
			}
			else
			{
				pMold->HighBlow.Step = 700;	
			}
			break;
		
		case 600:/* AirVentingTime   */
			pMold->ActInfo.HighBlow = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.Exhaust  = 1;
			pMold->TimeDis.AirVentingTime = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ValveOut.Exhaust  = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime = 0;
				pMold->HighBlow.Step = 700; 
			}		
			
			break;
		
			
		case 700: /* high blowing time2 Set  */
			
			pMold->ValveOut.Exhaust  = 0;
			
			if(pMold->TimeSet.BlowingTime2 != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.BlowingTime2;
				pMold->HighBlow.Step = 800;
			}
			else
			{
				pMold->HighBlow.Step = 900;	
			}
				
			break;
		
		case 800:/* high blowing time2   */
			pMold->ActInfo.HighBlow = 1;
	
			pMold->ValveOut.FreezingBlow = 1;
			pMold->TimeDis.BlowingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->ActInfo.HighBlow = 0;
				pMold->ValveOut.FreezingBlow = 0;
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.BlowingTime2 = 0;
				pMold->HighBlow.Step = 900; 
			}		
			
			break;
		
		case 900: /*AirVentingTime2 Set*/
			pMold->ValveOut.FreezingBlow = 0;
			if(pMold->TimeSet.AirVentingTime2 != 0)
			{
				pMold->HighBlow.Timer.IN = 1;
				pMold->HighBlow.Timer.PT = pMold->TimeSet.AirVentingTime2;
				pMold->HighBlow.Step = 1000;
			}
			else
			{
				pMold->HighBlow.Step = 1300;	
			}
			break;
		
		case 1000:/* AirVentingTime2  */
			pMold->ActInfo.HighBlow = 0;
			pMold->ValveOut.Exhaust = 1;
			pMold->TimeDis.AirVentingTime2 = pMold->HighBlow.Timer.ET; 
			if (1 == pMold->HighBlow.Timer.Q)
			{
				pMold->HighBlow.Timer.IN = 0;
				//	pMold->TimeDis.AirVentingTime2 = 0;
				pMold->HighBlow.Step = 1300; 
			}		
			break;

            	
		case 1300:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.FreezingBlow = 0;
			pMold->ValveOut.Exhaust = 0;
			pMold->HighBlow.Timer.IN = 0;
			
			pMold->HighBlow.Step = 3000;
			break;
		
		case 3000:
			break;
            	
		case 5000:
			pMold->ValveOut.HighBlow = 1;

			break;
            	
		case 20000:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.FreezingBlow = 0;
			pMold->ValveOut.Exhaust = 0;
			pMold->HighBlow.Timer.IN = 0;

			pMold->HighBlow.Step     = 0;
			break;
		
		case 30000:
			pMold->ActInfo.HighBlow  = 0;
			
			pMold->ValveOut.HighBlow = 0;
			pMold->ValveOut.FreezingBlow = 0;
			pMold->ValveOut.Exhaust = 0;
			pMold->HighBlow.Timer.IN = 0;

			pMold->HighBlow.Step     = 0;
			break;
	}/* end of switch */
}/*end of function */



