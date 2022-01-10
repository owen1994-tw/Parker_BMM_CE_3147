/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: coolpin2
 * File: coolpin2.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program coolpin2 --- cooling pin 2 control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void CoolPin2( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.CoolPin2.Step = 0;
 	RMold.CoolPin2.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
 	LMold.TransDIn.CoolPin2UpLimit = gMachineIn.LMold.CoolPin2UpLimit;
 	RMold.TransDIn.CoolPin2UpLimit = gMachineIn.RMold.CoolPin2UpLimit;

	CoolPin2(&LMold);
	CoolPin2(&RMold);

	gMachineOut.LMold.CoolPin2Dn = LMold.ValveOut.CoolPin2Dn;
	gMachineOut.RMold.CoolPin2Dn = RMold.ValveOut.CoolPin2Dn;

 } /* end of _CYCLIC */

void CoolPin2( Mold_typ * pMold)
{
	if(0 == pMold->TimeSet.CoolPin2UpAlarmTime)pMold->TimeSet.CoolPin2UpAlarmTime = 500;

	switch ( pMold->CoolPin2.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
//			pMold->CoolPin2.Out   	   = 0;
//			pMold->ValveOut.CoolPin2Dn  = 0;
//			pMold->ActInfo.CoolPin2Dn   = 0;
			pMold->CoolPin2.Timer.IN	   = 0;
			pMold->CoolPin2.LimitTimer.IN	   = 0;
			break;
					
		/*------------------ CoolPin2 -------------------- */
		case 100: /* CoolPin2 delay*/	
			if(pMold->Option.CoolPin2)
			{
				if(pMold->TimeSet.CoolPin2DnDelay != 0)
				{
					pMold->CoolPin2.Timer.IN = 1;
					pMold->CoolPin2.Timer.PT = pMold->TimeSet.CoolPin2DnDelay;
					pMold->CoolPin2.Step = 200;
				}
				else
				{
					pMold->CoolPin2.Step = 300;
				}
			}
			else
			{
				pMold->CoolPin2.Step = 500;
			}
			
			break;
		case 200: /* delay ok */
			pMold->TimeDis.CoolPin2DnDelay = pMold->CoolPin2.Timer.ET;
			if (1 == pMold->CoolPin2.Timer.Q)
			{
				pMold->CoolPin2.Timer.IN       = 0;
				//		pMold->TimeDis.CoolPin2DnDelay = 0;
				pMold->CoolPin2.Step = 400;
			}
			break;
		
		case 300:  //沒有轉二下時間， 轉二下直到轉二吹冷結束
			//            	if(pMold->TimeSet.CoolPin2DnTime != 0)
			//            	{
			//            		pMold->CoolPin2.Timer.IN = 1;
			//					pMold->CoolPin2.Timer.PT = pMold->TimeSet.CoolPin2DnTime;
			//					pMold->CoolPin2.Step = 400;
			//            	}
			//            	else
			//            	{
			//            		pMold->CoolPin2.Step = 500;
			//            	}
			pMold->CoolPin2.Step = 400;
			break;
		
		case 400:
			pMold->ActInfo.CoolPin2Dn  = 1;
			pMold->ValveOut.CoolPin2Dn = 1;
			pMold->CoolPin2.Out        = 1;

			//            	pMold->TimeDis.CoolPin2DnTime = pMold->CoolPin2.Timer.ET;
			//            	if (1 == pMold->CoolPin2.Timer.Q)
			//            	{
			//            		pMold->CoolPin2.Out        = 0;
			//            		pMold->ValveOut.CoolPin2Dn = 0;
			//            		pMold->CoolPin2.Timer.IN = 0;
			//            		pMold->TimeDis.CoolPin2DnTime = 0;
			//            		pMold->CoolPin2.Step = 500;
			//            	}
			if (3000==pMold->CoolPin2Blow.Step)
			{
				pMold->CoolPin2.Step = 500;
			}

			break;

		case 500:
			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.Step = 550;
			break;

		case 550:
			if (1 == pMold->Option.CoolPin2)
			{
				pMold->CoolPin2.LimitTimer.IN = 1;
				pMold->CoolPin2.LimitTimer.PT = pMold->TimeSet.CoolPin2UpAlarmTime;
				pMold->CoolPin2.Step = 600;	
			}
			else
			{
				pMold->CoolPin2.Step = 650;	// for no use
			}
								
			break;

		case 600:
			pMold->ActInfo.CoolPin2Dn  = 0;
			pMold->CoolPin2.Out        = 0;
			pMold->ValveOut.CoolPin2Dn = 0;

			if(1 == pMold->TransDIn.CoolPin2UpLimit)
			{
				pMold->CoolPin2.Step = 900;
			}
			pMold->TimeDis.CoolPin2UpAlarmTime = pMold->CoolPin2.LimitTimer.ET;
			
			if (1 == pMold->CoolPin2.LimitTimer.Q)
			{
				pMold->ActInfo.CoolPin2Dn  = 0;
				pMold->CoolPin2.Out        = 0;
				pMold->ValveOut.CoolPin2Dn = 0;

				pMold->CoolPin2.LimitTimer.IN = 0;
				pMold->Alarm.CoolPin2NotAtUpPos = 1;

				pMold->CoolPin2.Step = 40000;
			}
			break;

		case 650: // for no use
			
			if(pMold->TransDIn.CoolPin2UpLimit)
			{
				pMold->CoolPin2.Step = 900;
			}
			else				
			{
				pMold->CoolPin2.LimitTimer.IN = 0;
				pMold->Alarm.CoolPin2NotAtUpPos = 1;
				pMold->CoolPin2.Step = 40000;
			}
											
			//			pMold->TimeDis.CoolPin2UpAlarmTime = pMold->CoolPin2.Timer.ET;
			//			if (1 == pMold->CoolPin2.Timer.Q)
			//			{
			//			//	pMold->ActInfo.CoolPin2Dn  = 0;
			//			//	pMold->CoolPin2.Out        = 0;
			//			//	pMold->ValveOut.CoolPin2Dn = 0;
			//
			//				pMold->CoolPin2.Timer.IN = 0;
			//				pMold->Alarm.CoolPin2NotAtUpPos = 1;
			//
			//				pMold->CoolPin2.Step = 40000;
			//			}
			break;
		
		
		
		case 900:
			if (1==gMachineInfo.Auto)
			{
				if(pMold == &RMold)
				{
					RMold.Option.CoolPin2 = gMacOption.RCoolPin2;
				}
				else
				{
					LMold.Option.CoolPin2 = gMacOption.LCoolPin2;
				}
			}
			pMold->CoolPin2.Out      = 0;
			pMold->ValveOut.CoolPin2Dn  = 0;
			pMold->ActInfo.CoolPin2Dn   = 0;

			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.LimitTimer.IN = 0;
			pMold->CoolPin2.Step = 3000;
			break;
		case 3000:

			break;
		
		case 5000:
			pMold->ActInfo.CoolPin2Dn  = 1;
			pMold->ValveOut.CoolPin2Dn = 1;
				
			break;
		
		case 15000:
			pMold->ActInfo.CoolPin2Dn  = 0;
			pMold->ValveOut.CoolPin2Dn = 0;
				
			break;
		
		case 20000:
//			pMold->CoolPin2.Out        = 0;
//			pMold->ActInfo.CoolPin2Dn  = 0;
//			pMold->ValveOut.CoolPin2Dn = 0;

			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.LimitTimer.IN = 0;
			pMold->CoolPin2.Step    = 0;
			break;

		case 30000:
//			pMold->CoolPin2.Out        = 0;
//			pMold->ActInfo.CoolPin2Dn  = 0;
//			pMold->ValveOut.CoolPin2Dn = 0;
			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.LimitTimer.IN = 0;
			pMold->CoolPin2.Step     = 0;
			break;

		case 40000:
//			pMold->CoolPin2.Out        = 0;
//			pMold->ActInfo.CoolPin2Dn  = 0;
//			pMold->ValveOut.CoolPin2Dn = 0;
			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.LimitTimer.IN = 0;

			break;
		
		case 41000:
			//			pMold->CoolPin2.Out        = 0;
			//			pMold->ActInfo.CoolPin2Dn  = 0;
			//			pMold->ValveOut.CoolPin2Dn = 0;
			pMold->CoolPin2.Timer.IN = 0;
			pMold->CoolPin2.LimitTimer.IN = 0;
			pMold->CoolPin2.Step     = 40000;
			break;

	}/* end of switch */

	/* ----------------- cool pin2 down  action limit --------------------  */
	if(pMold->CoolPin2.Step > 100 && pMold->CoolPin2.Step < 900)
	{	


	}
	


}/*end of function */




