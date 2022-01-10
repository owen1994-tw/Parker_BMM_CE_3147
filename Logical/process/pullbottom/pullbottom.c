/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: bullbottom
 * File: pullbottom.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program coolpin --- pull bottom control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES 
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void PullBottom( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
	LMold.PullBottom.Step = 0;
 	RMold.PullBottom.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {

	PullBottom(&LMold);
	PullBottom(&RMold);

	gMachineOut.LMold.PullBottom = LMold.ValveOut.PullBottom;
	gMachineOut.RMold.PullBottom = RMold.ValveOut.PullBottom;

 } /* end of _CYCLIC */

void PullBottom( Mold_typ * pMold)
{
   if(0 == pMold->TimeSet.CoolPinUpAlarmTime)pMold->TimeSet.CoolPinUpAlarmTime = 500;

   switch ( pMold->PullBottom.Step )
   {
         /*------------------ stop all the output ------------------*/
         case 0:
            pMold->ValveOut.PullBottom  = 0;
            pMold->ActInfo.PullBottom   = 0;
			pMold->PullBottom.Timer.IN	   = 0;
            break;
            
      	/*------------------ PullBottom -------------------- */
         case 100: /* PullBottom delay*/
            if(pMold->Option.PullBottom)
            {
				if(pMold->TimeSet.PullBottomDelay != 0)
				{
					pMold->PullBottom.Timer.IN = 1;
					pMold->PullBottom.Timer.PT = pMold->TimeSet.PullBottomDelay;
					pMold->PullBottom.Step = 200;
				}
				else
				{
					pMold->PullBottom.Step = 300;
				}
				
            }
            else
            {
            	pMold->PullBottom.Step = 900;
            }
          	break;
      
         case 200: /* delay ok */
            pMold->TimeDis.PullBottomDelay = pMold->PullBottom.Timer.ET;
            if (1 == pMold->PullBottom.Timer.Q)
            {
               pMold->PullBottom.Timer.IN       = 0;
               pMold->TimeDis.CoolPinDnDelay = 0;
               pMold->PullBottom.Step = 300;
            }
            break;
      
         case 300:
			
            if(pMold->TimeSet.PullBottomTime != 0)
            {
               pMold->PullBottom.Timer.IN = 1;
               pMold->PullBottom.Timer.PT = pMold->TimeSet.PullBottomTime;
               pMold->PullBottom.Step = 400;
            }
            else
            {
               pMold->PullBottom.Step = 500;
            }
            break;
      
         case 400: /*PullBottom time*/
            pMold->ActInfo.PullBottom  = 1;
            pMold->ValveOut.PullBottom = 1;
            
            pMold->TimeDis.PullBottomTime = pMold->PullBottom.Timer.ET;
            if (1 == pMold->PullBottom.Timer.Q)
            {
               pMold->ValveOut.PullBottom = 0;
               pMold->PullBottom.Timer.IN = 0;
//               pMold->TimeDis.PullBottomTime = 0;
               pMold->PullBottom.Step = 500;
            }
			
            break;

		case 500:
			pMold->ActInfo.PullBottom  = 0;
			pMold->ValveOut.PullBottom = 0;
	
			pMold->PullBottom.Step = 900;
		
			break;

		
         case 900:
			
			pMold->ActInfo.PullBottom  = 0;
			pMold->ValveOut.PullBottom = 0;

			pMold->PullBottom.Timer.IN = 0;

			pMold->PullBottom.Step = 3000;
			
            break;
		
		case 1100:      /* bCoolPinUpNextCycClampCls  autorun Case-4570 */
						/* 強制桿下到下一循環關模後才轉一上 */
			
			
			pMold->ActInfo.CoolPinDn  = 1;
			pMold->ValveOut.CoolPinDn = 1;
			
			break;
		
		
         case 3000:
            break;
		
		
		case 5000:
			pMold->ActInfo.PullBottom  = 1;
			pMold->ValveOut.PullBottom = 1;
			
			break;
		
      
         case 20000:
            pMold->ActInfo.PullBottom  = 0;
            pMold->ValveOut.PullBottom = 0;
            
			pMold->PullBottom.Timer.IN = 0;
            pMold->PullBottom.Step = 0;
            break;
            
		case 30000:
			pMold->ActInfo.PullBottom  = 0;
			pMold->ValveOut.PullBottom = 0;
            
			pMold->PullBottom.Timer.IN = 0;
			pMold->PullBottom.Step = 0;
            break;
            
   }/* end of switch */

	/* ----------------- pull bottom  action limit --------------------  */
	if(pMold->PullBottom.Step > 100 && pMold->PullBottom.Step < 900)
	{	


	}
	
}/*end of function */




