/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: ChuteDeflashBlow
 * File: ChuteDeflashBlow.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program ChuteDeflashBlow --- parison ChuteDeflashBlow control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void ChuteDeflashBlow( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.ChuteDeflashBlow.Step = 0;
 	RMold.ChuteDeflashBlow.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {

	ChuteDeflashBlow(&LMold);
	ChuteDeflashBlow(&RMold);

	gMachineOut.LMold.ChuteDeflashBlow = LMold.ChuteDeflashBlow.Out;
	gMachineOut.RMold.ChuteDeflashBlow = RMold.ChuteDeflashBlow.Out;

 } /* end of _CYCLIC */

void ChuteDeflashBlow( Mold_typ * pMold)
{

	 switch ( pMold->ChuteDeflashBlow.Step )
    {
   		/*------------------ stop all the output ------------------*/
         case 0:
         	pMold->ChuteDeflashBlow.Out = 0;
         	pMold->ActInfo.ChuteDeflashBlow  = 0;
         	pMold->ChuteDeflashBlow.Timer.IN = 0;
         	break;

   		/*------------------ ChuteDeflashBlow  -------------------- */
   		case 100: 		/* ChuteDeflashBlow delay*/
  				if(pMold->TimeSet.ChuteDeflashBlowDelay != 0)
      		{
      			pMold->ChuteDeflashBlow.Timer.IN = 1;
  			      pMold->ChuteDeflashBlow.Timer.PT = pMold->TimeSet.ChuteDeflashBlowDelay;
      			pMold->ChuteDeflashBlow.Step = 200;
      		}
      		else
      		{
      			pMold->ChuteDeflashBlow.Step = 300;
      		}
       		break;
         case 200: 		/* ChuteDeflashBlow delay ok */
         	pMold->TimeDis.ChuteDeflashBlowDelay = pMold->ChuteDeflashBlow.Timer.ET;
         	if (1 == pMold->ChuteDeflashBlow.Timer.Q)
         	{
         		pMold->ChuteDeflashBlow.Timer.IN     = 0;
         		pMold->TimeDis.ChuteDeflashBlowDelay = 0;
         		pMold->ChuteDeflashBlow.Step = 300;
         	}
         	break;
         case 300:		/* ChuteDeflashBlow time */
         	if(pMold->TimeSet.ChuteDeflashBlowTime != 0)
         	{
         		pMold->ChuteDeflashBlow.Timer.IN = 1;
		         pMold->ChuteDeflashBlow.Timer.PT = pMold->TimeSet.ChuteDeflashBlowTime;
		         pMold->ChuteDeflashBlow.Step = 400;
         	}
         	else
         	{
         		pMold->ChuteDeflashBlow.Step = 500;
         	}
         	break;
         case 400:
         	pMold->ActInfo.ChuteDeflashBlow = 1;
         	pMold->ChuteDeflashBlow.Out     = 1;
         	pMold->TimeDis.ChuteDeflashBlowTime = pMold->ChuteDeflashBlow.Timer.ET;
         	if (1 == pMold->ChuteDeflashBlow.Timer.Q)
         	{
         		pMold->ChuteDeflashBlow.Out      = 0;
         		pMold->ChuteDeflashBlow.Timer.IN = 0;
         		pMold->TimeDis.ChuteDeflashBlowTime = 0;
         		pMold->ChuteDeflashBlow.Step = 500;
         	}
         	break;
         case 500:
         	pMold->ChuteDeflashBlow.Timer.IN = 0;
         	pMold->ChuteDeflashBlow.Out     = 0;
         	pMold->ActInfo.ChuteDeflashBlow = 0;
         	pMold->ChuteDeflashBlow.Step = 3000;
         	break;
         case 3000:
         	break;

         /* ------------- Normal stop ------------- */
         case 20000:
         	pMold->ChuteDeflashBlow.Out      = 0;
            pMold->ChuteDeflashBlow.Timer.IN = 0;
            
            pMold->ActInfo.ChuteDeflashBlow = 0;
            pMold->ChuteDeflashBlow.Step    = 0;
            break;

         /* ------------- Instance stop ------------- */
         case 30000:
         	pMold->ChuteDeflashBlow.Out      = 0;
         	pMold->ChuteDeflashBlow.Timer.IN = 0;
         
         	pMold->ActInfo.ChuteDeflashBlow  = 0;
         	pMold->ChuteDeflashBlow.Step     = 0;
         	break;
         
         /* ------------- Error stop ------------- */
         case 40000:
         	pMold->ChuteDeflashBlow.Out      = 0;
         	pMold->ChuteDeflashBlow.Timer.IN = 0;
         
         	pMold->ActInfo.ChuteDeflashBlow  = 0;
         	break;
         
	 }/* end of switch */
}/*end of function */




