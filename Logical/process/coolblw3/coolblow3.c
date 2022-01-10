/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: coolblow
 * File: coolblow.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program coolblow --- cooling blow control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void CoolPin3Blow( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.CoolPin3Blow.Step = 0;
 	RMold.CoolPin3Blow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	CoolPin3Blow(&LMold);
	CoolPin3Blow(&RMold);

	gMachineOut.LMold.CoolPin3Blow = LMold.ValveOut.CoolPin3Blow;
	gMachineOut.RMold.CoolPin3Blow = RMold.ValveOut.CoolPin3Blow;

 } /* end of _CYCLIC */

void CoolPin3Blow( Mold_typ * pMold)
{

	 switch ( pMold->CoolPin3Blow.Step )
     {
      		/*------------------ stop all the output ------------------*/
            case 0:
            	pMold->CoolPin3Blow.Out   	= 0;
            	pMold->ActInfo.CoolPin3Blow  = 0;
            	pMold->CoolPin3Blow.Timer.IN	= 0;
            	break;

      		/*------------------ CoolPin3 -------------------- */
            case 100:
            	if(pMold->Option.CoolPin3)
      			{
	            	if(pMold->TimeSet.CoolPin3BlowDelay != 0)
		      		{
	      				pMold->CoolPin3Blow.Timer.IN = 1;
						pMold->CoolPin3Blow.Timer.PT = pMold->TimeSet.CoolPin3BlowDelay;
	      				pMold->CoolPin3Blow.Step = 200;
	      			}
	      			else
	      			{
	      				pMold->CoolPin3Blow.Step = 300;
	      			}
	      		}
	      		else
	      		{
	      			pMold->CoolPin3Blow.Step = 900;
	      		}
            	break;
            case 200:
            	pMold->TimeDis.CoolPin3BlowDelay = pMold->CoolPin3Blow.Timer.ET;
            	if (1 == pMold->CoolPin3Blow.Timer.Q)
            	{
            		pMold->CoolPin3Blow.Timer.IN       = 0;
            	//	pMold->TimeDis.CoolPin3BlowDelay = 0;
            		pMold->CoolPin3Blow.Step = 300;
            	}
            	break;
            case 300:
            	if(pMold->TimeSet.CoolPin3BlowTime != 0)
            	{
            		pMold->CoolPin3Blow.Timer.IN = 1;
					pMold->CoolPin3Blow.Timer.PT = pMold->TimeSet.CoolPin3BlowTime;
					pMold->CoolPin3Blow.Step = 400;
            	}
            	else
            	{
            		pMold->CoolPin3Blow.Step = 900;
            	}
            	break;
            case 400:
            	pMold->CoolPin3Blow.Out      = 1;
            	pMold->ActInfo.CoolPin3Blow  = 1;
            	pMold->ValveOut.CoolPin3Blow = 1;

            	pMold->TimeDis.CoolPin3BlowTime = pMold->CoolPin3Blow.Timer.ET;
            	if (1 == pMold->CoolPin3Blow.Timer.Q)
            	{
            		pMold->CoolPin3Blow.Out      = 0;
            		pMold->ActInfo.CoolPin3Blow  = 0;
            		pMold->ValveOut.CoolPin3Blow = 0;

            		pMold->CoolPin3Blow.Timer.IN = 0;
            	//	pMold->TimeDis.CoolPin3BlowTime = 0;

            		pMold->CoolPin3Blow.Step = 900;
            	}
            	break;
            case 900:
//			if (1==gMachineInfo.Auto)
//			{
//				if(pMold == &RMold)
//				{
//					RMold.Option.CoolPin3 = gMacOption.RCoolPin3;
//				}
//				else
//				{
//					LMold.Option.CoolPin3 = gMacOption.LCoolPin3;
//				}
//			}
            	pMold->CoolPin3Blow.Out      = 0;
            	pMold->ActInfo.CoolPin3Blow  = 0;
            	pMold->ValveOut.CoolPin3Blow = 0;

            	pMold->CoolPin3Blow.Timer.IN = 0;

            	pMold->CoolPin3Blow.Step = 3000;
            	break;

            case 3000:
            	break;
		
			case 5000:
				pMold->ActInfo.CoolPin3Blow  = 1;
				pMold->ValveOut.CoolPin3Blow = 1;
						
				break;

            case 20000:
            	pMold->CoolPin3Blow.Out      = 0;
            	pMold->ActInfo.CoolPin3Blow  = 0;
            	pMold->ValveOut.CoolPin3Blow = 0;

				pMold->CoolPin3Blow.Timer.IN = 0;
				pMold->CoolPin3Blow.Step    = 0;
				break;

			case 30000:
            	pMold->CoolPin3Blow.Out      = 0;
            	pMold->ActInfo.CoolPin3Blow  = 0;
            	pMold->ValveOut.CoolPin3Blow = 0;

				pMold->CoolPin3Blow.Timer.IN = 0;
				pMold->CoolPin3Blow.Step     = 0;
				break;

	 }/* end of switch */

	 if(pMold->CoolPin3Blow.Step > 100 && pMold->CoolPin3Blow.Step < 500)
	 {
	 	if(!pMold->Option.CoolPin3 && 1 == gMachineInfo.Auto)
	 	{
			pMold->CoolPin3Blow.Step  = 900;
		}
	}
}/*end of function */




