/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: HookPin
 * File: HookPin.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program HookPin --- cooling pin control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES 
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void HookPin( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.HookPin.Step = 0;
	RMold.HookPin.Step = 0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{

	LMold.TransDIn.HookPinFwLimit = gMachineIn.LMold.HookPinFwLimit;
	RMold.TransDIn.HookPinFwLimit = gMachineIn.RMold.HookPinFwLimit;
	
	LMold.TransDIn.HookPinBwLimit = gMachineIn.LMold.HookPinBwLimit;
	RMold.TransDIn.HookPinBwLimit = gMachineIn.RMold.HookPinBwLimit;

	HookPin(&LMold);
	HookPin(&RMold);

	gMachineOut.LMold.HookPinFw = LMold.ValveOut.HookPinFw;
	gMachineOut.RMold.HookPinFw = RMold.ValveOut.HookPinFw;

} /* end of _CYCLIC */

void HookPin( Mold_typ * pMold)
{
	if(0 == pMold->TimeSet.HookPinFwAlarmTime)pMold->TimeSet.HookPinFwAlarmTime = 500;
	if(0 == pMold->TimeSet.HookPinBwAlarmTime)pMold->TimeSet.HookPinBwAlarmTime = 500;

	switch ( pMold->HookPin.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
//			pMold->ActInfo.HookPinFw   = 0;		
//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			break;
            
		/*--------------------------------------------------- */
		/*---------------- HookPin Fw  --------------------- */
		/*--------------------------------------------------- */
		case 100: /* HookPin delay*/
			if(pMold->Option.HookPin)
			{
				if(pMold->TimeSet.HookPinFwDelay != 0)
				{
					pMold->HookPin.Timer.IN = 1;
					pMold->HookPin.Timer.PT = pMold->TimeSet.HookPinFwDelay;
					pMold->HookPin.Step = 200;
				}
				else
				{
					pMold->HookPin.Step = 300;
				}	
			}
			else
			{
				pMold->HookPin.Step = 300;
			}
			break;
      
		case 200: /* delay ok */
			pMold->TimeDis.HookPinFwDelay = pMold->HookPin.Timer.ET;
			if (1 == pMold->HookPin.Timer.Q)
			{
				pMold->HookPin.Timer.IN       = 0;
				//               pMold->TimeDis.HookPinFwDelay = 0;
				pMold->HookPin.Step = 300;
			}
			break;
      
		case 300: 
			if(pMold->Option.HookPin)
			{
				pMold->HookPin.LimitTimer.IN = 1;
				pMold->HookPin.LimitTimer.PT = pMold->TimeSet.HookPinFwAlarmTime;
				pMold->HookPin.Step = 400;
			}
			else
			{
				pMold->HookPin.Step = 2900;  // for no use
			}
			
			break;
      
		case 400:
			pMold->ActInfo.HookPinFw  = 1;
			pMold->ValveOut.HookPinFw = 1;
			
			if (1 == pMold->TransDIn.HookPinFwLimit)
			{
				pMold->HookPin.Step = 500;  
			}
            
			break;

		case 500:
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step = 3000;
			break;
		
		case 2900:  // for no use
	
			pMold->ActInfo.HookPinFw   = 0;		
			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step = 3000;
	
			break;
		
		case 3000:
			break;
		
		
		case 5000:
			pMold->ActInfo.HookPinFw  = 1;
			pMold->ValveOut.HookPinFw = 1;
			
			break;
		
		/*--------------------------------------------------- */
		/*---------------- HookPin Bw  --------------------- */
		/*--------------------------------------------------- */
		
		case 10100: /* HookPin delay*/
			if(pMold->Option.HookPin)
			{
				if(pMold->TimeSet.HookPinBwDelay != 0)
				{
					pMold->HookPin.Timer.IN = 1;
					pMold->HookPin.Timer.PT = pMold->TimeSet.HookPinBwDelay;
					pMold->HookPin.Step = 10200;
				}
				else
				{
					pMold->HookPin.Step = 10300;
				}	
			}
			else
			{
				pMold->HookPin.Step = 10300;
			}
			break;
      
		case 10200: /* delay ok */
			pMold->TimeDis.HookPinBwDelay = pMold->HookPin.Timer.ET;
			if (1 == pMold->HookPin.Timer.Q)
			{
				pMold->HookPin.Timer.IN       = 0;
				//               pMold->TimeDis.HookPinFwDelay = 0;
				pMold->HookPin.Step = 10300;
			}
			break;
      
		case 10300: 
			
			if(pMold->Option.HookPin)
			{
				pMold->HookPin.LimitTimer.IN = 1;
				pMold->HookPin.LimitTimer.PT = pMold->TimeSet.HookPinBwAlarmTime;
				pMold->HookPin.Step = 10400;
			}
			else
			{
			
				if (1 == pMold->TransDIn.HookPinBwLimit)
				{
					pMold->HookPin.Step = 12900;  // for no use
				}
				else				
				{
					pMold->Alarm.HookPinNotAtBwPos = 1;
					pMold->HookPin.Step = 40000;           /*  HookPin bw alarm   */
				}
			}
			
			break;
		
		case 10400: 
				
			pMold->ActInfo.HookPinFw  = 0;
			pMold->ValveOut.HookPinFw = 0;
			
			if (1 == pMold->TransDIn.HookPinBwLimit)
			{
				pMold->HookPin.Step = 10500;  
			}
			
			break;
		
		case 10500: 
				
			pMold->ActInfo.HookPinFw   = 0;		
			//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step = 13000;  
			
			break;
		
		case 12900:  // for no use
	
			pMold->ActInfo.HookPinFw   = 0;		
			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step = 13000;
	
			break;
		
		
		case 13000:  
	
			break;
		
		case 15000:
			pMold->ActInfo.HookPinFw  = 0;
			pMold->ValveOut.HookPinFw = 0;
			
			break;
		
      
		case 20000:
//			pMold->ActInfo.HookPinFw   = 0;		
//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step	   = 0;
			break;
            
		case 30000:
			//			pMold->ActInfo.HookPinFw   = 0;		
			//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step	   = 0;
			break;
            
		case 40000:
			//			pMold->ActInfo.HookPinFw   = 0;		
			//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			   
			break;
		
		case 41000:
			//			pMold->ActInfo.HookPinFw   = 0;		
			//			pMold->ValveOut.HookPinFw  = 0;
			
			pMold->HookPin.LimitTimer.IN	   = 0;
			pMold->HookPin.Timer.IN	   = 0;
			
			pMold->HookPin.Step	   = 40000;
			   
			break;
	}/* end of switch */
	
	
	/* ----------------- HookPin Fw / Bw timeout --------------------  */
	if(1 == pMold->HookPin.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->HookPin.Step < 500 && pMold->HookPin.Step > 300)		/* HookPin Fw  */
		{
			pMold->Alarm.HookPinNotAtFwPos = 1;
		}
      
		if(pMold->TopDeflash.Step < 10500 && pMold->TopDeflash.Step > 10300)	/* HookPin Bw  */
		{
			pMold->Alarm.HookPinNotAtBwPos = 1;
		}
      
		pMold->HookPin.Step = 41000;
	}

	/* ----------------- Angle pin Fw  action limit --------------------  */
	if(pMold->HookPin.Step > 300 && pMold->HookPin.Step < 500)
	{	
		if( 0 == pMold->Carriage.BwPos )
		{
			pMold->Alarm.CarriageNotAtBwDI= 1;
			pMold->HookPin.Step = 40000;
		}
	}
	
}/*end of function */




