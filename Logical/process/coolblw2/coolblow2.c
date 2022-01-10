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

void CoolPin2Blow( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.CoolPin2Blow.Step = 0;
 	RMold.CoolPin2Blow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	CoolPin2Blow(&LMold);
	CoolPin2Blow(&RMold);

	gMachineOut.LMold.CoolPin2Blow = LMold.ValveOut.CoolPin2Blow;
	gMachineOut.RMold.CoolPin2Blow = RMold.ValveOut.CoolPin2Blow;

 } /* end of _CYCLIC */

void CoolPin2Blow( Mold_typ * pMold)
{

	 switch ( pMold->CoolPin2Blow.Step )
     {
      		/*------------------ stop all the output ------------------*/
            case 0:
            	pMold->CoolPin2Blow.Out   	= 0;
            	pMold->ActInfo.CoolPin2Blow  = 0;
            	pMold->CoolPin2Blow.Timer.IN	= 0;
            	break;

      		/*------------------ CoolPin2 -------------------- */
            case 100:
            	if(pMold->Option.CoolPin2)
      			{
	            	if(pMold->TimeSet.CoolPin2BlowDelay != 0)
		      		{
	      				pMold->CoolPin2Blow.Timer.IN = 1;
						pMold->CoolPin2Blow.Timer.PT = pMold->TimeSet.CoolPin2BlowDelay;
	      				pMold->CoolPin2Blow.Step = 200;
	      			}
	      			else
	      			{
	      				pMold->CoolPin2Blow.Step = 300;
	      			}
	      		}
	      		else
	      		{
	      			pMold->CoolPin2Blow.Step = 900;
	      		}
            	break;
            case 200:
            	pMold->TimeDis.CoolPin2BlowDelay = pMold->CoolPin2Blow.Timer.ET;
            	if (1 == pMold->CoolPin2Blow.Timer.Q)
            	{
            		pMold->CoolPin2Blow.Timer.IN       = 0;
            //		pMold->TimeDis.CoolPin2BlowDelay = 0;
            		pMold->CoolPin2Blow.Step = 300;
            	}
            	break;
            case 300:
            	if(pMold->TimeSet.CoolPin2BlowTime != 0)
            	{
            		pMold->CoolPin2Blow.Timer.IN = 1;
					pMold->CoolPin2Blow.Timer.PT = pMold->TimeSet.CoolPin2BlowTime;
					pMold->CoolPin2Blow.Step = 400;
            	}
            	else
            	{
            		pMold->CoolPin2Blow.Step = 900;
            	}
            	break;
            case 400:
            	pMold->CoolPin2Blow.Out      = 1;
            	pMold->ActInfo.CoolPin2Blow  = 1;
            	pMold->ValveOut.CoolPin2Blow = 1;

            	pMold->TimeDis.CoolPin2BlowTime = pMold->CoolPin2Blow.Timer.ET;
            	if (1 == pMold->CoolPin2Blow.Timer.Q)
            	{
            		pMold->CoolPin2Blow.Out      = 0;
            		pMold->ActInfo.CoolPin2Blow  = 0;
            		pMold->ValveOut.CoolPin2Blow = 0;

            		pMold->CoolPin2Blow.Timer.IN = 0;
            	//	pMold->TimeDis.CoolPin2BlowTime = 0;

            		pMold->CoolPin2Blow.Step = 900;
            	}
            	break;
            case 900:
//			if (1==gMachineInfo.Auto)
//			{
//				if(pMold == &RMold)
//				{
//					RMold.Option.CoolPin2 = gMacOption.RCoolPin2;
//				}
//				else
//				{
//					LMold.Option.CoolPin2 = gMacOption.LCoolPin2;
//				}
//			}
            	pMold->CoolPin2Blow.Out      = 0;
            	pMold->ActInfo.CoolPin2Blow  = 0;
            	pMold->ValveOut.CoolPin2Blow = 0;

            	pMold->CoolPin2Blow.Timer.IN = 0;

            	pMold->CoolPin2Blow.Step = 3000;
            	break;

            case 3000:
            	break;
		
			case 5000:
				pMold->ActInfo.CoolPin2Blow  = 1;
				pMold->ValveOut.CoolPin2Blow = 1;
					
				break;

            case 20000:
            	pMold->CoolPin2Blow.Out      = 0;
            	pMold->ActInfo.CoolPin2Blow  = 0;
            	pMold->ValveOut.CoolPin2Blow = 0;

				pMold->CoolPin2Blow.Timer.IN = 0;
				pMold->CoolPin2Blow.Step    = 0;
				break;

			case 30000:
            	pMold->CoolPin2Blow.Out      = 0;
            	pMold->ActInfo.CoolPin2Blow  = 0;
            	pMold->ValveOut.CoolPin2Blow = 0;

				pMold->CoolPin2Blow.Timer.IN = 0;
				pMold->CoolPin2Blow.Step     = 0;
				break;

	 }/* end of switch */

	 if(pMold->CoolPin2Blow.Step > 100 && pMold->CoolPin2Blow.Step < 500)
	 {
	 	if(!pMold->Option.CoolPin2 && 1 == gMachineInfo.Auto)
	 	{
			pMold->CoolPin2Blow.Step  = 900;
		}
	}
}/*end of function */




