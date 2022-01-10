/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: coolpin
 * File: coolpin.c
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

void CoolPin( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.CoolPin.Step = 0;
	RMold.CoolPin.Step = 0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{

	LMold.TransDIn.CoolPinUpLimit = gMachineIn.LMold.CoolPinUpLimit;
	RMold.TransDIn.CoolPinUpLimit = gMachineIn.RMold.CoolPinUpLimit;

	CoolPin(&LMold);
	CoolPin(&RMold);

	gMachineOut.LMold.CoolPinDn = LMold.ValveOut.CoolPinDn;
	gMachineOut.RMold.CoolPinDn = RMold.ValveOut.CoolPinDn;

} /* end of _CYCLIC */

void CoolPin( Mold_typ * pMold)
{
	if(0 == pMold->TimeSet.CoolPinUpAlarmTime)pMold->TimeSet.CoolPinUpAlarmTime = 500;

	switch ( pMold->CoolPin.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
//			pMold->ValveOut.CoolPinDn  = 0;
//			pMold->ActInfo.CoolPinDn   = 0;
			pMold->CoolPin.LimitTimer.IN	   = 0;
			pMold->CoolPin.Timer.IN	   = 0;
			break;
            
		/*------------------ CoolPin -------------------- */
		case 100: /* CoolPin delay*/
			if(pMold->Option.CoolPin)
			{
				if ( 1 == gMachineFix.Option.bCoolPinUpNextCycClampCls)    /*  於下一循環關模後轉一上    */
				{
					pMold->CoolPin.Step = 1100; 		/*  強制桿下到下一循環關模後才轉一上 */
				}
				else
				{
					if(pMold->TimeSet.CoolPinDnDelay != 0)
					{
						pMold->CoolPin.Timer.IN = 1;
						pMold->CoolPin.Timer.PT = pMold->TimeSet.CoolPinDnDelay;
						pMold->CoolPin.Step = 200;
					
						//gExSPC
						if(pMold == & LMold)
						{
							gExSPC[ExIndex].LMold.CoolPinDnDelay = pMold->TimeSet.CoolPinDnDelay/100.0;
						}
						else
						{
							gExSPC[ExIndex].RMold.CoolPinDnDelay = pMold->TimeSet.CoolPinDnDelay/100.0;
						}
					}
					else
					{
						pMold->CoolPin.Step = 300;
					}
				}
			}
			else
			{
				pMold->CoolPin.Step = 900;
			}
			break;
      
		case 200: /* delay ok */
			pMold->TimeDis.CoolPinDnDelay = pMold->CoolPin.Timer.ET;
			if (1 == pMold->CoolPin.Timer.Q)
			{
				pMold->CoolPin.Timer.IN       = 0;
				//               pMold->TimeDis.CoolPinDnDelay = 0;
				pMold->CoolPin.Step = 300;
			}
			break;
      
		case 300: //沒有轉一下時間， 轉一下直到轉一吹冷結束
			
			//            if(pMold->TimeSet.CoolPinDnTime != 0)
			//            {
			//               pMold->CoolPin.Timer.IN = 1;
			//               pMold->CoolPin.Timer.PT = pMold->TimeSet.CoolPinDnTime;
			//               pMold->CoolPin.Step = 400;
			//			
			//				//gExSPC
			//				if(pMold == & LMold)
			//				{
			//					gExSPC[ExIndex].LMold.CoolPinDnTime = pMold->TimeSet.CoolPinDnTime/100.0;
			//				}
			//				else
			//				{
			//					gExSPC[ExIndex].RMold.CoolPinDnTime = pMold->TimeSet.CoolPinDnTime/100.0;
			//				}
			//            }
			//            else
			//            {
			//               pMold->CoolPin.Step = 500;
			//            }
			
			pMold->CoolPin.Step = 400;
			break;
      
		case 400:
			pMold->ActInfo.CoolPinDn  = 1;
			pMold->ValveOut.CoolPinDn = 1;
            
			//            pMold->TimeDis.CoolPinDnTime = pMold->CoolPin.Timer.ET;
			//            if (1 == pMold->CoolPin.Timer.Q)
			//            {
			//               pMold->ValveOut.CoolPinDn = 0;
			//               pMold->CoolPin.Timer.IN = 0;
			//               pMold->TimeDis.CoolPinDnTime = 0;
			//               pMold->CoolPin.Step = 500;
			//            }

			if (3000==pMold->CoolPinBlow.Step)
			{
				pMold->CoolPin.Step = 500;
			}
			
			break;

		case 500:
			

			if ((1 == pMold->Option.CoolPin) || (1 == pMold->ValveOut.CoolPinDn))
			{
				pMold->CoolPin.LimitTimer.IN = 1;
				pMold->CoolPin.LimitTimer.PT = pMold->TimeSet.CoolPinUpAlarmTime;
				pMold->CoolPin.Step = 600;	
			}
			else
			{
				pMold->CoolPin.Step = 650;	// for no use
			}
			
			break;

		case 600:
			pMold->ActInfo.CoolPinDn  = 0;
			pMold->ValveOut.CoolPinDn = 0;
            
			if(pMold->TransDIn.CoolPinUpLimit)
			{
				pMold->CoolPin.Step = 900;
			}
			pMold->TimeDis.CoolPinUpAlarmTime = pMold->CoolPin.LimitTimer.ET;
			if (1 == pMold->CoolPin.LimitTimer.Q)
			{
				pMold->ActInfo.CoolPinDn  = 0;
				pMold->ValveOut.CoolPinDn = 0;
               
				pMold->CoolPin.LimitTimer.IN = 0;
				pMold->Alarm.CoolPinNotAtUpPos = 1;
               
				pMold->CoolPin.Step = 40000;
			}
			break;

		case 650: // for no use
			//			pMold->ActInfo.CoolPinDn  = 0;
			//			pMold->ValveOut.CoolPinDn = 0;
			
			if(pMold->TransDIn.CoolPinUpLimit)
			{
				pMold->CoolPin.Step = 900;
			}
			else
			{
				pMold->CoolPin.LimitTimer.IN = 0;
				pMold->Alarm.CoolPinNotAtUpPos = 1;
               
				pMold->CoolPin.Step = 40000;
			}
						
			//			pMold->TimeDis.CoolPinUpAlarmTime = pMold->CoolPin.LimitTimer.ET;
			//			if (1 == pMold->CoolPin.LimitTimer.Q)
			//			{
			//				pMold->CoolPin.LimitTimer.IN = 0;
			//				pMold->Alarm.CoolPinNotAtUpPos = 1;
			//               
			//				pMold->CoolPin.Step = 40000;
			//			}
			break;
		
		case 900:
			
			pMold->ValveOut.CoolPinDn = 0;
			pMold->ActInfo.CoolPinDn  = 0;
			pMold->CoolPin.LimitTimer.IN = 0;
			pMold->CoolPin.Step = 3000;
			break;
		
		case 1100:      /* bCoolPinUpNextCycClampCls  autorun Case-4570 */
			/* 強制桿下到下一循環關模後才轉一上 */
			
			
			pMold->ActInfo.CoolPinDn  = 1;
			pMold->ValveOut.CoolPinDn = 1;
			
			break;
		
		
		case 3000:
			break;
		
		
		case 5000:
			pMold->ActInfo.CoolPinDn  = 1;
			pMold->ValveOut.CoolPinDn = 1;
			
			break;
		
		case 15000:
			pMold->ActInfo.CoolPinDn  = 0;
			pMold->ValveOut.CoolPinDn = 0;
			
			break;
		
      
		case 20000:
//			pMold->ActInfo.CoolPinDn  = 0;
//			pMold->ValveOut.CoolPinDn = 0;
            
			pMold->CoolPin.Timer.IN = 0;
			pMold->CoolPin.LimitTimer.IN = 0;
			pMold->CoolPin.Step = 0;
			break;
            
		case 30000:
//			pMold->ActInfo.CoolPinDn  = 0;
//			pMold->ValveOut.CoolPinDn = 0;
			pMold->CoolPin.Timer.IN = 0;
			pMold->CoolPin.LimitTimer.IN = 0;
			pMold->CoolPin.Step = 0;
			break;
            
		case 40000:
//			pMold->ActInfo.CoolPinDn  = 0;
//			pMold->ValveOut.CoolPinDn = 0;
			pMold->CoolPin.Timer.IN = 0;
			pMold->CoolPin.LimitTimer.IN = 0;
            
			break;
	}/* end of switch */

	/* ----------------- cool pin down  action limit --------------------  */
	if(pMold->CoolPin.Step > 100 && pMold->CoolPin.Step < 900)
	{	


	}
	
}/*end of function */




