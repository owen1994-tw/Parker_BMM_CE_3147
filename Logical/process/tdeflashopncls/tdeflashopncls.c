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

void TopDeflashOpnCls( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.TopDeflashOpnCls.Step = 30000;
	RMold.TopDeflashOpnCls.Step = 30000;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(!LMold.TimeSet.TopDeflashOpnAlarmTime)LMold.TimeSet.TopDeflashOpnAlarmTime = 1000;  /*  10s  */
	if(!RMold.TimeSet.TopDeflashOpnAlarmTime)RMold.TimeSet.TopDeflashOpnAlarmTime = 1000;  /*  10s  */

	LMold.TransDIn.TopDeflashOpnLimit = gMachineIn.LMold.TopDeflashOpnLimit;
	RMold.TransDIn.TopDeflashOpnLimit = gMachineIn.RMold.TopDeflashOpnLimit;
	
	TopDeflashOpnCls(&LMold); 
	TopDeflashOpnCls(&RMold); 
		
	gMachineOut.LMold.TopDeflashCls = LMold.ValveOut.TopDeflashCls;
	gMachineOut.RMold.TopDeflashCls = RMold.ValveOut.TopDeflashCls;
	
	
} /* end of _CYCLIC */

void TopDeflashOpnCls( Mold_typ * pMold)
{
	switch ( pMold->TopDeflashOpnCls.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			if( 1 == pMold->TransDIn.TopDeflashFwLimit )
			{
				pMold->ActInfo.TopDeflashCls = 0;
				pMold->ValveOut.TopDeflashCls  = 0;
			}
			
			pMold->TopDeflashOpnCls.Timer.IN	= 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN = 0;
			
			pMold->TopDeflashOpnCls.AutoTimeOutFlag = 0;
			
			if(pMold == & LMold)
			{
				LHandledeflashCount = 0;
			}
			else
			{
				RHandledeflashCount = 0;
			}
			break;
		
		/*--------------------------------------------------- */
		/*-------------- Topdeflash Cls  -------------------- */
		/*--------------------------------------------------- */
		case 100: 		/* cls delay*/
			if(pMold->Option.PunchHandle)
			{
				if(pMold->TimeSet.TopDeflashClsDelay != 0)
				{
					pMold->TopDeflashOpnCls.Timer.IN = 1;
					pMold->TopDeflashOpnCls.Timer.PT = pMold->TimeSet.TopDeflashClsDelay;
					pMold->TopDeflashOpnCls.Step 	= 200;
				}
				else
				{
					pMold->TopDeflashOpnCls.Step = 300;
				}
			}
			else
			{
				pMold->TopDeflashOpnCls.Step = 300;
			}
			break;
             	
		case 200: 			/* forward delay ok */
			pMold->TimeDis.TopDeflashClsDelay = pMold->TopDeflashOpnCls.Timer.ET;  
			if (1 == pMold->TopDeflashOpnCls.Timer.Q ||  1 == pMold->TopDeflashOpnCls.AutoTimeOutFlag)
			{
				pMold->TopDeflashOpnCls.Timer.IN       = 0;
				//	pMold->TimeDis.TopDeflashFwDelay = 0;
				pMold->TopDeflashOpnCls.Step = 300;
			}
			break;
            	
		case 300:	/* set time */	
			if(pMold->Option.PunchHandle)
			{
				if(pMold->TimeSet.TopDeflashClsTime != 0)
				{
					pMold->TopDeflashOpnCls.Timer.IN = 1;
					pMold->TopDeflashOpnCls.Timer.PT = pMold->TimeSet.TopDeflashClsTime;
					pMold->TopDeflashOpnCls.Step = 400;
				}
				else
				{
					pMold->TopDeflashOpnCls.Step = 500;	
				}
			}
			else			
			{
				pMold->TopDeflashOpnCls.Step = 12900;  // for no use
			}
			break;
            
		case 400:	/*  TopDeflash cls  */
			pMold->ActInfo.TopDeflashCls = 1;

			pMold->ValveOut.TopDeflashCls  = 1;
			
			pMold->TimeDis.TopDeflashClsTime = pMold->TopDeflashOpnCls.Timer.ET;

			if (1 == pMold->TopDeflashOpnCls.Timer.Q || 1 == pMold->TopDeflashOpnCls.AutoTimeOutFlag )
			{
				pMold->TopDeflashOpnCls.Timer.IN  = 0;
		
				pMold->TopDeflashOpnCls.Step = 10100;  
			}

			break;
		
		
		/* ---------------------- calibration Cls ---------------------- */
		case 5000:
				
			if(1 == pMold->TransDIn.TopDeflashFwLimit)
			{	
				pMold->ActInfo.TopDeflashCls   = 1;
				pMold->ValveOut.TopDeflashCls  = 1;
			}
			
			break;
		
		/*--------------------------------------------------- */
		/*-------------- Topdeflash Opn  -------------------- */
		/*--------------------------------------------------- */
		case 10100: 		/* cls delay*/
			if(pMold->Option.PunchHandle)
			{
				pMold->TopDeflashOpnCls.Step = 10200;
			}
			else
			{
				pMold->TopDeflashOpnCls.Step = 10300;
			}
			break;
             	
		case 10200: 	/* cls delay ok */

			pMold->TopDeflashOpnCls.Step = 10300;

			break;
            	
		case 10300:	/* set time */	
			if(pMold->Option.PunchHandle)
			{
				pMold->TopDeflashOpnCls.LimitTimer.IN = 1;
				pMold->TopDeflashOpnCls.LimitTimer.PT = pMold->TimeSet.TopDeflashOpnAlarmTime;
				pMold->TopDeflashOpnCls.Step = 10400;
			}
			else  // for no use check limit
			{	
				if (1 == pMold->TransDIn.TopDeflashOpnLimit)
				{
					pMold->TopDeflashOpnCls.Step = 12900; // for no use
				}
				else				
				{
					pMold->Alarm.TopDeflashNotAtOpnPos = !pMold->TransDIn.TopDeflashOpnLimit;
					pMold->TopDeflashOpnCls.Step = 40000;     /*  TopDeflash Opn alarm   */
				}
			}
			break;
            
		case 10400:	/*  TopDeflash opn  */
			pMold->ActInfo.TopDeflashCls = 0;

			pMold->ValveOut.TopDeflashCls  = 0;
			
			if (1 == pMold->TransDIn.TopDeflashOpnLimit )
			{
				pMold->TopDeflashOpnCls.LimitTimer.IN = 0;
			
				if(pMold == & LMold)
				{
					LHandledeflashCount = LHandledeflashCount+1;
				}
				else
				{
					RHandledeflashCount = RHandledeflashCount+1;
				}
				pMold->TopDeflashOpnCls.Step = 10500;
			}
			break;
		
		case 10500:	/*  Check Topdeflash cls/opn count  */
			
			if(pMold == & LMold)
			{
				if (0==gUserPara.HandledeflashCount || (gUserPara.HandledeflashCount == LHandledeflashCount) || (1 == gMachineInfo.MachineReset )|| 1 == pMold->TopDeflashOpnCls.AutoTimeOutFlag)
				{
					LHandledeflashCount=0;
					pMold->TopDeflashOpnCls.Step = 13000;
				}
				else
				{			
//					pMold->TopDeflashOpnCls.Step = 100; /*  backe to Topdeflash cls delay  */
					pMold->TopDeflashOpnCls.Step = 300; /*  backe to Topdeflash cls (wichout delay)  */
				}
			}
			else
			{
				if (0==gUserPara.HandledeflashCount || (gUserPara.HandledeflashCount == RHandledeflashCount) || (1 == gMachineInfo.MachineReset)|| 1 == pMold->TopDeflashOpnCls.AutoTimeOutFlag)
				{
					RHandledeflashCount=0;
					pMold->TopDeflashOpnCls.Step = 13000;
				}
				else
				{			
//					pMold->TopDeflashOpnCls.Step = 100; /*  backe to Topdeflash cls delay  */
					pMold->TopDeflashOpnCls.Step = 300; /*  backe to Topdeflash cls (wichout delay)  */
				}
			}
			break;
			
			
		case 12900: //for no use	
			pMold->ActInfo.TopDeflashCls = 0;

			pMold->ValveOut.TopDeflashCls  = 0;

			pMold->TopDeflashOpnCls.Timer.IN   = 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN   = 0;
			
			pMold->TopDeflashOpnCls.Step = 13000;
		
			break;
		
		case 13000:
		
			break;
		
		/* ---------------------- calibration Opn ---------------------- */
		case 15000:
				
			if(1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->ActInfo.TopDeflashCls   = 0;
				pMold->ValveOut.TopDeflashCls  = 0;
			}
			break;
		
		
		/*--------------- Manual action stop -----------------*/
		case 20000:

			pMold->ActInfo.TopDeflashCls = 0;

			pMold->ValveOut.TopDeflashCls  = 0;
			
			
			pMold->TopDeflashOpnCls.Timer.IN   = 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN   = 0;
				
			pMold->TopDeflashOpnCls.Step 	 = 0;
			break;
				
		/*--------------- Instance stop -----------------*/	
		case 30000:

			pMold->ActInfo.TopDeflashCls = 0;

			pMold->ValveOut.TopDeflashCls  = 0;

			pMold->TopDeflashOpnCls.Timer.IN   = 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN   = 0;
				
			pMold->TopDeflashOpnCls.Step 	 = 0;
			break;
		
		/*--------------- Error  stop -----------------*/	
		case 40000:
			if( 1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->ActInfo.TopDeflashCls = 0;

				pMold->ValveOut.TopDeflashCls  = 0;
			}
			pMold->TopDeflashOpnCls.Timer.IN   = 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN   = 0;
					
			break;
		
		/* ------------- auto run stop ------------- */		
		case 41000:
			if( 1 == pMold->TransDIn.TopDeflashFwLimit)
			{
				pMold->ActInfo.TopDeflashCls = 0;

				pMold->ValveOut.TopDeflashCls  = 0;
			}
			pMold->TopDeflashOpnCls.Timer.IN   = 0;
			pMold->TopDeflashOpnCls.LimitTimer.IN   = 0;
				
			pMold->TopDeflashOpnCls.Step  = 40000;
			

			break;
	}/* end of switch */
	
	/* ----------------- TopDeflash Opn / Cls alarm time display --------------------  */
	if(pMold->TopDeflashOpnCls.Step > 10300 && pMold->TopDeflashOpnCls.Step  < 10500)	/* TopDeflash Opn   */
	{
		pMold->TimeDis.TopDeflashOpnAlarmTime = pMold->TopDeflash.LimitTimer.ET;
	}
	
	/* ----------------- TopDeflash Opn / Cls timeout --------------------  */
	if(1 == pMold->TopDeflashOpnCls.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{ 
		if(pMold->TopDeflashOpnCls.Step < 10500 && pMold->TopDeflashOpnCls.Step > 10200)	/* TopDeflash Opn  */
		{
			pMold->Alarm.TopDeflashNotAtOpnPos = 1;
		}
      
		pMold->TopDeflashOpnCls.Step = 41000;
	}
	
	
	/* ----------------- TopDeflash Opn / Cls  action limit --------------------  */
	if( pMold->TopDeflashOpnCls.Step > 100 && pMold->TopDeflashOpnCls.Step < 500 ) 	 /* TopDeflash Cls  */
	{
		if (0 == pMold->TransDIn.TopDeflashFwLimit ) 
		{
			pMold->Alarm.TopDeflashNotAtFwPos = !pMold->TransDIn.TopDeflashFwLimit;
			pMold->TopDeflashOpnCls.Step = 40000;
		}	
	}
	 
	if(pMold->TopDeflashOpnCls.Step > 10100 && pMold->TopDeflashOpnCls.Step < 10500)  /* TopDeflash Opn  */
	{
		if (0 == pMold->TransDIn.TopDeflashFwLimit ) 
		{
			pMold->Alarm.TopDeflashNotAtFwPos = !pMold->TransDIn.TopDeflashFwLimit;
			pMold->TopDeflashOpnCls.Step = 40000;
		}
	}
	
	
}/*end of function */



