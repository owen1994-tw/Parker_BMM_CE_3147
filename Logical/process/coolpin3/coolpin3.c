/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: coolpin3
 * File: coolpin3.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program coolpin3 --- cooling pin 3 control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void CoolPin3( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.CoolPin3.Step = 0;
	RMold.CoolPin3.Step = 0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	LMold.TransDIn.CoolPin3UpLimit = gMachineIn.LMold.CoolPin3UpLimit;
	RMold.TransDIn.CoolPin3UpLimit = gMachineIn.RMold.CoolPin3UpLimit;

	CoolPin3(&LMold);
	CoolPin3(&RMold);

	gMachineOut.LMold.CoolPin3Dn = LMold.ValveOut.CoolPin3Dn;
	gMachineOut.RMold.CoolPin3Dn = RMold.ValveOut.CoolPin3Dn;

} /* end of _CYCLIC */

void CoolPin3( Mold_typ * pMold)
{

	if(0 == pMold->TimeSet.CoolPin3UpAlarmTime)pMold->TimeSet.CoolPin3UpAlarmTime = 500;

	switch ( pMold->CoolPin3.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
//			pMold->ActInfo.CoolPin3Dn   = 0;
//			pMold->ValveOut.CoolPin3Dn  = 0;
			
			pMold->CoolPin3.LimitTimer.IN = 0;
			pMold->CoolPin3.Timer.IN	= 0;
			break;

		/*------------------ CoolPin3 -------------------- */
		case 100: /* CoolPin3 delay*/
			if(pMold->Option.CoolPin3)
			{
				if(pMold->TimeSet.CoolPin3DnDelay != 0)
				{
					pMold->CoolPin3.Timer.IN = 1;
					pMold->CoolPin3.Timer.PT = pMold->TimeSet.CoolPin3DnDelay;
					pMold->CoolPin3.Step = 200;
				}
				else
				{
					pMold->CoolPin3.Step = 400;
				}
			}
			else
			{
				pMold->CoolPin3.Step = 900;
			}
			break;
		case 200: /* delay ok */
			pMold->TimeDis.CoolPin3DnDelay = pMold->CoolPin3.Timer.ET;
			if (1 == pMold->CoolPin3.Timer.Q)
			{
				pMold->CoolPin3.Timer.IN       = 0;
				//	pMold->TimeDis.CoolPin3DnDelay = 0;
				pMold->CoolPin3.Step = 400;
			}
			break;
		case 300:  //沒有轉三下時間， 轉三下直到轉三吹冷結束
			//            	if(pMold->TimeSet.CoolPin3DnTime != 0)
			//            	{
			//            		pMold->CoolPin3.Timer.IN = 1;
			//					pMold->CoolPin3.Timer.PT = pMold->TimeSet.CoolPin3DnTime;
			//					pMold->CoolPin3.Step = 400;
			//            	}
			//            	else
			//            	{
			//            		pMold->CoolPin3.Step = 500;
			//            	}
			pMold->CoolPin3.Step = 400;
			
			break;
		case 400:
			pMold->ActInfo.CoolPin3Dn  = 1;
			pMold->ValveOut.CoolPin3Dn = 1;


			//            	pMold->TimeDis.CoolPin3DnTime = pMold->CoolPin3.Timer.ET;
			//            	if (1 == pMold->CoolPin3.Timer.Q)
			//            	{
			//            		pMold->ValveOut.CoolPin3Dn = 0;
			//            		pMold->CoolPin3.Timer.IN = 0;
			//            		pMold->TimeDis.CoolPin3DnTime = 0;
			//            		pMold->CoolPin3.Step = 450;
			//            	}
			if (3000==pMold->CoolPin3Blow.Step)
			{
				pMold->CoolPin3.Step = 500;
			}
			break;

		case 500:
			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.Step = 550;
	
			break;
		
		case 550:
			if (1 == pMold->Option.CoolPin3)
			{
				pMold->CoolPin3.LimitTimer.IN = 1;
				pMold->CoolPin3.LimitTimer.PT = pMold->TimeSet.CoolPin3UpAlarmTime;
				pMold->CoolPin3.Step = 600;	
			}
			else
			{
				pMold->CoolPin3.Step = 650;	// for no use
			}

			break;

		case 600:
			pMold->ActInfo.CoolPin3Dn  = 0;
			pMold->ValveOut.CoolPin3Dn = 0;

			if(pMold->TransDIn.CoolPin3UpLimit)
			{
				pMold->CoolPin3.Step = 900;
			}
			pMold->TimeDis.CoolPin3UpAlarmTime = pMold->CoolPin3.LimitTimer.ET;
			if (1 == pMold->CoolPin3.LimitTimer.Q)
			{
				pMold->ActInfo.CoolPin3Dn  = 0;
				pMold->ValveOut.CoolPin3Dn = 0;

				pMold->CoolPin3.LimitTimer.IN = 0;
				pMold->Alarm.CoolPin3NotAtUpPos = 1;

				pMold->CoolPin3.Step = 40000;
			}
			break;
		
		case 650: // for no use
			
			if(pMold->TransDIn.CoolPin3UpLimit)
			{
				pMold->CoolPin3.Step = 900;
			}
			else				
			{
				pMold->CoolPin3.LimitTimer.IN = 0;
				pMold->Alarm.CoolPin3NotAtUpPos = 1;
				pMold->CoolPin3.Step = 40000;
			}
											
			break;
		
		case 900:
			if (1==gMachineInfo.Auto)
			{
				if(pMold == &RMold)
				{
					RMold.Option.CoolPin3 = gMacOption.RCoolPin3;
				}
				else
				{
					LMold.Option.CoolPin3 = gMacOption.LCoolPin3;
				}
			}
			
			pMold->ValveOut.CoolPin3Dn  = 0;
			pMold->ActInfo.CoolPin3Dn   = 0;
			
			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.LimitTimer.IN = 0;
			pMold->CoolPin3.Step = 3000;
			
			break;
		
		case 3000:

			break;
		
		case 5000:
			pMold->ActInfo.CoolPin3Dn  = 1;
			pMold->ValveOut.CoolPin3Dn = 1;
					
			break;
		
		case 15000:
			pMold->ActInfo.CoolPin3Dn  = 0;
			pMold->ValveOut.CoolPin3Dn = 0;
				
			break;
		
		case 20000:
//			pMold->ActInfo.CoolPin3Dn  = 0;
//			pMold->ValveOut.CoolPin3Dn = 0;

			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.LimitTimer.IN = 0;
			pMold->CoolPin3.Step    = 0;
			break;

		case 30000:
//			pMold->ActInfo.CoolPin3Dn  = 0;
//			pMold->ValveOut.CoolPin3Dn = 0;
			
			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.LimitTimer.IN = 0;
			pMold->CoolPin3.Step     = 0;
			break;

		case 40000:
//			pMold->ActInfo.CoolPin3Dn  = 0;
//			pMold->ValveOut.CoolPin3Dn = 0;
			
			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.LimitTimer.IN = 0;

			break;
		
		case 41000:
			//			pMold->ActInfo.CoolPin3Dn  = 0;
			//			pMold->ValveOut.CoolPin3Dn = 0;
			
			pMold->CoolPin3.Timer.IN = 0;
			pMold->CoolPin3.LimitTimer.IN = 0;
			pMold->CoolPin3.Step     = 40000;
			break;

	}/* end of switch */

	/* ----------------- cool pin3 down  action limit --------------------  */
	if(pMold->CoolPin3.Step > 100 && pMold->CoolPin3.Step < 900)
	{

	}
	
}/*end of function */




