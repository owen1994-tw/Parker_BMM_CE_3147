/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: sealer
 * File: sealer.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program sealer --- parison sealer control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void Sealer( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.Sealer.Step = 0;
 	RMold.Sealer.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
 	LMold.TransDIn.SealOpnLimit = gMachineIn.LMold.SealerOpnLimit;
 	RMold.TransDIn.SealOpnLimit = gMachineIn.RMold.SealerOpnLimit;

	Sealer(&LMold);
	Sealer(&RMold);

	gMachineOut.LMold.SealerFw = LMold.Sealer.Out;
	gMachineOut.RMold.SealerFw = RMold.Sealer.Out;

 } /* end of _CYCLIC */

void Sealer( Mold_typ * pMold)
{

	 switch ( pMold->Sealer.Step )
    {
   		/*------------------ stop all the output ------------------*/
         case 0:
         	pMold->Sealer.Out = 0;
         	pMold->ActInfo.Sealer  = 0;
         	pMold->Sealer.Timer.IN = 0;
         	break;

   		/*------------------ Sealer  -------------------- */
   		case 100: 		/* Sealer delay*/
   			if(pMold->Option.Sealer)
   			{
   				if(pMold->TimeSet.SealerDelay != 0)
       			{
       				pMold->Sealer.Timer.IN = 1;
   			      pMold->Sealer.Timer.PT = pMold->TimeSet.SealerDelay;
       				pMold->Sealer.Step = 200;
       			}
       			else
       			{
       				pMold->Sealer.Step = 300;
       			}
       		}
       		else
       		{
       			pMold->Sealer.Step = 3000;
       		}
          	break;
         case 200: 		/* Sealer delay ok */
         	pMold->TimeDis.SealerDelay = pMold->Sealer.Timer.ET;
         	if (1 == pMold->Sealer.Timer.Q)
         	{
         		pMold->Sealer.Timer.IN     = 0;
         		pMold->TimeDis.SealerDelay = 0;
         		pMold->Sealer.Step = 300;
         	}
         	break;
         case 300:		/* Sealer time */
         	if(pMold->TimeSet.SealerTime != 0)
         	{
         		pMold->Sealer.Timer.IN = 1;
		         pMold->Sealer.Timer.PT = pMold->TimeSet.SealerTime;
		         pMold->Sealer.Step = 400;
         	}
         	else
         	{
         		pMold->Sealer.Step = 500;
         	}
         	break;
         case 400:
         	pMold->ActInfo.Sealer = 1;
         	pMold->Sealer.Out     = 1;
         	pMold->TimeDis.SealerTime = pMold->Sealer.Timer.ET;
         	if (1 == pMold->Sealer.Timer.Q)
         	{
         		pMold->Sealer.Out      = 0;
         		pMold->Sealer.Timer.IN = 0;
         		pMold->TimeDis.SealerTime = 0;
         		pMold->Sealer.Step = 500;
         	}
         	break;
         case 500:
         	pMold->Sealer.Timer.IN = 0;
         	pMold->Sealer.Out     = 0;
         	pMold->ActInfo.Sealer = 0;
         	pMold->Sealer.Step = 3000;
         	break;
         case 3000:
         	break;

         /* ------------- Normal stop ------------- */
         case 20000:
         	pMold->Sealer.Out      = 0;
            pMold->Sealer.Timer.IN = 0;
            
            pMold->ActInfo.Sealer = 0;
            pMold->Sealer.Step    = 0;
            break;

         /* ------------- Instance stop ------------- */
         case 30000:
         	pMold->Sealer.Out      = 0;
         	pMold->Sealer.Timer.IN = 0;
         
         	pMold->ActInfo.Sealer  = 0;
         	pMold->Sealer.Step     = 0;
         	break;
         
         /* ------------- Error stop ------------- */
         case 40000:
         	pMold->Sealer.Out      = 0;
         	pMold->Sealer.Timer.IN = 0;
         
         	pMold->ActInfo.Sealer  = 0;
         	break;
         
	 }/* end of switch */

	if(pMold->Sealer.Step > 100 && pMold->Sealer.Step < 500)
	{
	 	if(!pMold->Option.Sealer && 1 == gMachineInfo.Auto)
	 	{
			pMold->Sealer.Step  = 500;
		}
	}
}/*end of function */




