/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: LowBlow
 * File: LowBlow.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program LowBlow --- 1 blowing 
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

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 		LMold.LowBlow.Step = 0;
 		RMold.LowBlow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	if(1 == gMachineFix.Option.bBlowModeCBM)
	{
		BlowCBM(&LMold);
		BlowCBM(&RMold);
	}
	else
	{
		BlowStandard(&LMold);
		BlowStandard(&RMold);
	}
	
//	gMachineOut.LMold.LowBlow = LMold.LowBlow.Out;
//	gMachineOut.RMold.LowBlow = RMold.LowBlow.Out;
} /* end of _CYCLIC */

void BlowCBM( Mold_typ * pMold)
{
	switch ( pMold->LowBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->LowBlow.Out  		= 0;
			pMold->ActInfo.LowBlow   = 0;
			pMold->LowBlow.Timer.IN	= 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100: /* blow delay*/
			pMold->LowBlow.TimerTotal.IN = 0;
			if(pMold->TimeSet.LowBlowDelay != 0)
			{
				pMold->LowBlow.Timer.IN = 1;
				pMold->LowBlow.Timer.PT = pMold->TimeSet.LowBlowDelay;
				pMold->LowBlow.Step 	   = 200;
			}
			else
			{
				pMold->LowBlow.Step = 300;
			}
			break;
             	
		case 200: /* delay ok */
			pMold->TimeDis.LowBlowDelay = pMold->LowBlow.Timer.ET;  
			if (1 == pMold->LowBlow.Timer.Q)
			{
				pMold->LowBlow.Timer.IN     = 0;
			//	pMold->TimeDis.LowBlowDelay = 0;
            		
				pMold->LowBlow.Step = 300;
			}
			break;
            	
		case 300:
			pMold->LowBlow.TimerTotal.IN = 1;
			pMold->LowBlow.TimerTotal.PT = pMold->TimeSet.TotalBlowTime * 10;

			if(pMold->HighBlow.Step > 300)
			{
				pMold->HighBlow.Step = 470;
			}

			if(pMold->TimeSet.LowBlowTime != 0)
			{
				pMold->LowBlow.Timer.IN = 1;
				pMold->LowBlow.Timer.PT = pMold->TimeSet.LowBlowTime;
				pMold->LowBlow.Step = 400;
			}
			else
			{
				pMold->LowBlow.Step = 450;
			}
			break;
            	
		case 400:	/* pre blow time   */
			pMold->LowBlow.Out     = 1;
			pMold->ActInfo.LowBlow = 1;
			pMold->TimeDis.LowBlowTime = pMold->LowBlow.Timer.ET;  
			if (1 == pMold->LowBlow.Timer.Q)
			{
				pMold->LowBlow.Out      = 0;
				pMold->LowBlow.Timer.IN = 0;
			//	pMold->TimeDis.LowBlowTime = 0;
				pMold->LowBlow.Step = 450;
			}
            	
			if (1 == pMold->LowBlow.TimerTotal.Q || 1 == pMold->HighBlow.TimerTotal.Q)
			{
				pMold->LowBlow.Out = 0;
				pMold->LowBlow.TimerTotal.IN = 0;
            		
				pMold->LowBlow.Step = 480;
			}
			break;
            	
		case 450:
			pMold->LowBlow.Out     = 1;
			pMold->ActInfo.LowBlow = 1;

			if (1 == pMold->LowBlow.TimerTotal.Q || 1 == pMold->HighBlow.TimerTotal.Q)
			{
				pMold->LowBlow.Out = 0;
				pMold->LowBlow.TimerTotal.IN = 0;
				pMold->LowBlow.Step = 480;
			}
			break;
            	
		case 470:
			pMold->LowBlow.Out = 0;
			break;
            	
		case 480:
			if(pMold->HighBlow.Step < 480 && pMold->HighBlow.Step > 0)pMold->HighBlow.Step = 480;
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
			pMold->LowBlow.TimerTotal.IN = 0;
			pMold->LowBlow.Step = 900;
			break;
            	
		case 900:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;
				
			pMold->ActInfo.LowBlow = 0;
				
			pMold->LowBlow.Step = 3000;
			break;
            	
		case 3000:
			break;
		
		case 5000:
			pMold->LowBlow.Out     = 1;
			pMold->ActInfo.LowBlow = 1;
			break;
            	
		case 20000:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
			pMold->LowBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;
				
			pMold->ActInfo.LowBlow= 0;
			pMold->LowBlow.Step 	 = 0;
			break;
					
		case 30000:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
			pMold->LowBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;

			pMold->ActInfo.LowBlow  = 0;
			pMold->LowBlow.Step     = 0;
			break;

	}/* end of switch */
}/*end of function */

void BlowStandard( Mold_typ * pMold)
{
	switch ( pMold->LowBlow.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->LowBlow.Out  		= 0;
			pMold->ActInfo.LowBlow   = 0;
			pMold->LowBlow.Timer.IN	= 0;
			break;
            
		/*------------------ Blow  -------------------- */
		case 100: /* blow delay*/
			pMold->LowBlow.TimerTotal.IN = 0;
			if(pMold->TimeSet.LowBlowDelay != 0)
			{
				pMold->LowBlow.Timer.IN = 1;
				pMold->LowBlow.Timer.PT = pMold->TimeSet.LowBlowDelay;
				pMold->LowBlow.Step 	   = 200;
			}
			else
			{
				pMold->LowBlow.Step = 300;
			}
			break;
             	
		case 200: /* delay ok */
			pMold->TimeDis.LowBlowDelay = pMold->LowBlow.Timer.ET;  
			if (1 == pMold->LowBlow.Timer.Q)
			{
				pMold->LowBlow.Timer.IN     = 0;
		//		pMold->TimeDis.LowBlowDelay = 0;
            		
				pMold->LowBlow.Step = 300;
			}
			break;
            	
		case 300:
			if(pMold->TimeSet.LowBlowTime != 0)
			{
				pMold->LowBlow.Timer.IN = 1;
				pMold->LowBlow.Timer.PT = pMold->TimeSet.LowBlowTime;
				pMold->LowBlow.Step = 400;
			}
			else
			{
				pMold->LowBlow.Step = 900;
			}
			break;
            	
		case 400:	/* pre blow time   */
			pMold->LowBlow.Out     = 1;
			pMold->ActInfo.LowBlow = 1;
			pMold->TimeDis.LowBlowTime = pMold->LowBlow.Timer.ET;  
			if (1 == pMold->LowBlow.Timer.Q)
			{
				pMold->LowBlow.Out      = 0;
				pMold->LowBlow.Timer.IN = 0;
			//	pMold->TimeDis.LowBlowTime = 0;
				pMold->LowBlow.Step = 900;
			}
			break;
            	
		case 900:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;
				
			pMold->ActInfo.LowBlow = 0;
				
			pMold->LowBlow.Step = 3000;
			break;
            	
		case 3000:
			break;
            	
		case 20000:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
			pMold->LowBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;
				
			pMold->ActInfo.LowBlow= 0;
			pMold->LowBlow.Step 	 = 0;
			break;
					
		case 30000:
			pMold->LowBlow.Out      = 0;
			pMold->LowBlow.Timer.IN = 0;
			pMold->LowBlow.TimerTotal.IN = 0;
				
//			pMold->TimeDis.LowBlowTime  = 0;
//			pMold->TimeDis.LowBlowDelay = 0;

			pMold->ActInfo.LowBlow  = 0;
			pMold->LowBlow.Step     = 0;
			break;

	}/* end of switch */
}/*end of function */



