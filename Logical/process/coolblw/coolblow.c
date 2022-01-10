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

void CoolPinBlow( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.CoolPinBlow.Step = 0;
 	RMold.CoolPinBlow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	CoolPinBlow(&LMold);
	CoolPinBlow(&RMold);

	gMachineOut.LMold.CoolPinBlow = LMold.ValveOut.CoolPinBlow;
	gMachineOut.RMold.CoolPinBlow = RMold.ValveOut.CoolPinBlow;

 } /* end of _CYCLIC */

void CoolPinBlow( Mold_typ * pMold)
{

	 switch ( pMold->CoolPinBlow.Step )
     {
      		/*------------------ stop all the output ------------------*/
            case 0:
            	pMold->ValveOut.CoolPinBlow   = 0;
            	pMold->ActInfo.CoolPinBlow  = 0;
            	pMold->CoolPinBlow.Timer.IN	= 0;
            	break;

      		/*------------------ CoolPin -------------------- */
            case 100:
            	if(pMold->Option.CoolPin)
      			{
	            	if(pMold->TimeSet.CoolPinBlowDelay != 0)
		      		{
	      				pMold->CoolPinBlow.Timer.IN = 1;
						pMold->CoolPinBlow.Timer.PT = pMold->TimeSet.CoolPinBlowDelay;
	      				pMold->CoolPinBlow.Step = 200;
					
						
						//gExSPC
						if(pMold == & LMold)
						{
							gExSPC[ExIndex].LMold.CoolPinBlowDelay = pMold->TimeSet.CoolPinBlowDelay/100.0;
						}
						else
						{
							gExSPC[ExIndex].RMold.CoolPinBlowDelay = pMold->TimeSet.CoolPinBlowDelay/100.0;
						}
	      			}
	      			else
	      			{
	      				pMold->CoolPinBlow.Step = 300;
	      			}
	      		}
	      		else
	      		{
	      			pMold->CoolPinBlow.Step = 900;
	      		}
            	break;
            case 200:
            	pMold->TimeDis.CoolPinBlowDelay = pMold->CoolPinBlow.Timer.ET;
            	if (1 == pMold->CoolPinBlow.Timer.Q)
            	{
            		pMold->CoolPinBlow.Timer.IN       = 0;
            	//	pMold->TimeDis.CoolPinBlowDelay = 0;
            		pMold->CoolPinBlow.Step = 300;
            	}
            	break;
            case 300:
            	if(pMold->TimeSet.CoolPinBlowTime != 0)
            	{
            		pMold->CoolPinBlow.Timer.IN = 1;
					pMold->CoolPinBlow.Timer.PT = pMold->TimeSet.CoolPinBlowTime;
					pMold->CoolPinBlow.Step = 400;
			
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.CoolPinBlowTime = pMold->TimeSet.CoolPinBlowTime/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.CoolPinBlowTime = pMold->TimeSet.CoolPinBlowTime/100.0;
					}
					
            	}
            	else
            	{
            		pMold->CoolPinBlow.Step = 900;
            	}
            	break;
            case 400:
            	pMold->ActInfo.CoolPinBlow  = 1;
            	pMold->ValveOut.CoolPinBlow = 1;

            	pMold->TimeDis.CoolPinBlowTime = pMold->CoolPinBlow.Timer.ET;
            	if (1 == pMold->CoolPinBlow.Timer.Q)
            	{
            		pMold->CoolPinBlow.Timer.IN = 0;
            	//	pMold->TimeDis.CoolPinBlowTime = 0;

            		pMold->CoolPinBlow.Step = 500;
            	}
            	break;
		
			case 500:
				pMold->ActInfo.CoolPinBlow  = 0;
				pMold->ValveOut.CoolPinBlow = 0;
	
				pMold->CoolPinBlow.Step = 900;
		
				break;
		
            case 900:
//			if (1==gMachineInfo.Auto)
//			{
//				if(pMold == &RMold)
//				{
//					RMold.Option.CoolPin = gMacOption.RCoolPin;
//				}
//				else
//				{
//					LMold.Option.CoolPin = gMacOption.LCoolPin;
//				}
//			}
            	pMold->ActInfo.CoolPinBlow  = 0;
            	pMold->ValveOut.CoolPinBlow = 0;

            	pMold->CoolPinBlow.Timer.IN = 0;

            	pMold->CoolPinBlow.Step = 3000;
            	break;

            case 3000:
            	break;
		
			case 5000:
				pMold->ActInfo.CoolPinBlow  = 1;
				pMold->ValveOut.CoolPinBlow = 1;
				
				break;

            case 20000:
            	pMold->ActInfo.CoolPinBlow  = 0;
            	pMold->ValveOut.CoolPinBlow = 0;

				pMold->CoolPinBlow.Timer.IN = 0;
				pMold->CoolPinBlow.Step    = 0;
				break;

			case 30000:
            	pMold->ActInfo.CoolPinBlow  = 0;
            	pMold->ValveOut.CoolPinBlow = 0;

				pMold->CoolPinBlow.Timer.IN = 0;
				pMold->CoolPinBlow.Step     = 0;
				break;

	 }/* end of switch */

	/* ----------------- cool pin blow  action limit --------------------  */
	 if(pMold->CoolPinBlow.Step > 100 && pMold->CoolPinBlow.Step < 900)
	 {
	 	if( 0 == pMold->Option.CoolPin && 1 == gMachineInfo.Auto)
	 	{
			pMold->CoolPinBlow.Step  = 900;
		}
	}
}/*end of function */




