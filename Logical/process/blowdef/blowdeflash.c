/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: blowdeflash
 * File: blowdeflash.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program blowdeflash --- blowing air to  deflasher control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void BlowDeflash( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.BlowDeflash.Step = 0;
 	RMold.BlowDeflash.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	BlowDeflash(&LMold);
	BlowDeflash(&RMold);

	gMachineOut.LMold.BlowDeflash = LMold.BlowDeflash.Out;
	gMachineOut.RMold.BlowDeflash = RMold.BlowDeflash.Out;

 } /* end of _CYCLIC */

void BlowDeflash( Mold_typ * pMold)
{
    /* ------ step machine control ------ */
	 switch (pMold->BlowDeflash.Step)
    {
   		/*------------------ stop all the output ------------------*/
         case 0:
         	pMold->BlowDeflash.Out  	= 0;
         	pMold->ActInfo.BlowDeflash  = 0;
         	pMold->BlowDeflash.Timer.IN	= 0;
         	break;

   		/*------------------ BlowDeflash close -------------------- */
   		case 100: /* BlowDeflash delay*/
				if(pMold->TimeSet.BlowDeflashDelay != 0)
    			{
    				pMold->BlowDeflash.Timer.IN = 1;
			   	 	pMold->BlowDeflash.Timer.PT = pMold->TimeSet.BlowDeflashDelay;
    				pMold->BlowDeflash.Step = 200;
    			}
    			else
    			{
    				pMold->BlowDeflash.Step = 300;
    			}
          	break;
         case 200: /* delay ok */
         	pMold->TimeDis.BlowDeflashDelay = pMold->BlowDeflash.Timer.ET;
         	if (1 == pMold->BlowDeflash.Timer.Q)
         	{
         		pMold->BlowDeflash.Timer.IN     = 0;
         	//	pMold->TimeDis.BlowDeflashDelay = 0;
         		pMold->BlowDeflash.Step = 300;
         	}
         	break;
         case 300:
         	if(pMold->TimeSet.BlowDeflashTime != 0)
         	{
         		pMold->BlowDeflash.Timer.IN = 1;
		         pMold->BlowDeflash.Timer.PT = pMold->TimeSet.BlowDeflashTime;
		         pMold->BlowDeflash.Step = 400;
         	}
         	else
         	{
         		pMold->BlowDeflash.Step = 500;
         	}
         	break;
         case 400:
         	pMold->ActInfo.BlowDeflash = 1;
         	pMold->BlowDeflash.Out     = 1;

         	pMold->TimeDis.BlowDeflashTime = pMold->BlowDeflash.Timer.ET;
         	if (1 == pMold->BlowDeflash.Timer.Q)
         	{
         		pMold->BlowDeflash.Out      = 0;
         		pMold->BlowDeflash.Timer.IN = 0;
         	//	pMold->TimeDis.BlowDeflashTime = 0;
         		pMold->BlowDeflash.Step = 500;
         	}
         	break;
         case 500:
         	pMold->BlowDeflash.Timer.IN = 0;
         	pMold->BlowDeflash.Out     = 0;
         	pMold->ActInfo.BlowDeflash = 0;
         	pMold->BlowDeflash.Step = 3000;
         	break;
         case 3000:
         	break;

         case 20000:
         	pMold->BlowDeflash.Out      = 0;
				pMold->BlowDeflash.Timer.IN = 0;

				pMold->ActInfo.BlowDeflash = 0;
				pMold->BlowDeflash.Step    = 0;
	         break;

			case 30000:
				pMold->BlowDeflash.Out      = 0;
				pMold->BlowDeflash.Timer.IN = 0;

				pMold->ActInfo.BlowDeflash  = 0;
				pMold->BlowDeflash.Step     = 0;
				break;
	 }/* end of switch */
}/*end of function */




