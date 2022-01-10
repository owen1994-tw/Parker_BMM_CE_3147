/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: inerpin
 * File: inerpin.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program inerpin ---inner pin  control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void InnerPin( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.InnerPin.Step = 0;
 	RMold.InnerPin.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
 	if(!LMold.TimeSet.InnerPinBwAlarmTime)LMold.TimeSet.InnerPinBwAlarmTime = 1000;
 	if(!RMold.TimeSet.InnerPinBwAlarmTime)RMold.TimeSet.InnerPinBwAlarmTime = 1000;

 	LMold.TransDIn.InnerPinBwLimit = gMachineIn.LMold.InnerPinBwLimit;
	RMold.TransDIn.InnerPinBwLimit = gMachineIn.RMold.InnerPinBwLimit;

	InnerPin(&LMold);
	InnerPin(&RMold);

	switch(LMold.InnerPin.Out)
	{
		case 0:
			gMachineOut.LMold.InnerPinFw   = 0;
			gMachineOut.LMold.InnerPinBlow = 0;
			break;
		case 1:
			gMachineOut.LMold.InnerPinFw   = 1;
			gMachineOut.LMold.InnerPinBlow = 0;
			break;
		case 2:
			gMachineOut.LMold.InnerPinFw   = 1;
			gMachineOut.LMold.InnerPinBlow = 1;
			break;
	}/* switch(LMold.InnerPin.Out) */

	switch(RMold.InnerPin.Out)
	{
		case 0:
			gMachineOut.RMold.InnerPinFw   = 0;
			gMachineOut.RMold.InnerPinBlow = 0;
			break;
		case 1:
			gMachineOut.RMold.InnerPinFw   = 1;
			gMachineOut.RMold.InnerPinBlow = 0;
			break;
		case 2:
			gMachineOut.RMold.InnerPinFw   = 1;
			gMachineOut.RMold.InnerPinBlow = 1;
			break;
	}/* switch(LMold.InnerPin.Out) */

 } /* end of _CYCLIC */

void InnerPin( Mold_typ * pMold)
{
   /* ------ step machine control ----- */
	switch ( pMold->InnerPin.Step )
   {
   		/*------------------ stop all the output ------------------*/
         case 0:
         	pMold->InnerPin.Out  	 = 0;
         	pMold->ActInfo.InnerPin  = 0;
         	pMold->InnerPin.Timer.IN = 0;
         	break;

   		/*------------------ InnerPin close -------------------- */
   		case 100: /* InnerPin delay*/
   			if(pMold->Option.InnerPin)
   			{
   				if(pMold->TimeSet.InnerPinFwDelay != 0)
       			{
       				pMold->InnerPin.Timer.IN = 1;
   			      pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinFwDelay;
       				pMold->InnerPin.Step = 200;
       			}
       			else
       			{
       				pMold->InnerPin.Step = 300;
       			}
       		}
       		else
       		{
       			pMold->InnerPin.Step = 3000;
       		}
          	break;
         case 200: /* delay ok */
         	pMold->TimeDis.InnerPinFwDelay = pMold->InnerPin.Timer.ET;
         	if (1 == pMold->InnerPin.Timer.Q)
         	{
         		pMold->InnerPin.Timer.IN     = 0;
         		pMold->TimeDis.InnerPinFwDelay = 0;
         		pMold->InnerPin.Step = 300;
         	}
         	break;
         case 300:
         	pMold->ActInfo.InnerPin = 1;
         	pMold->InnerPin.Out     = 1;
	         pMold->InnerPin.Step = 3000;
         	break;

         /*case 300:
         	if(pMold->TimeSet.InnerPinFwTime != 0)
         	{
         		pMold->InnerPin.Timer.IN = 1;
		         pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinFwTime;
		         pMold->InnerPin.Step = 400;
         	}
         	else
         	{
         		pMold->InnerPin.Step = 500;
         	}
         	break;
         case 400:
         	pMold->ActInfo.InnerPin = 1;
         	pMold->InnerPin.Out     = 1;
         	pMold->TimeDis.InnerPinFwTime = pMold->InnerPin.Timer.ET;
         	if (1 == pMold->InnerPin.Timer.Q)
         	{
         		pMold->InnerPin.Timer.IN = 0;
         		pMold->TimeDis.InnerPinFwTime = 0;
         		pMold->InnerPin.Step = 500;
         	}
         	break;
         case 500:
         	if(pMold->TimeSet.InnerPinBlowDelay != 0)
    		   {
   				pMold->InnerPin.Timer.IN = 1;
		         pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinBlowDelay;
   				pMold->InnerPin.Step = 600;
   			}
   			else
   			{
   				pMold->InnerPin.Step = 700;
   			}
         	break;
         case 600:
         	pMold->TimeDis.InnerPinBlowDelay = pMold->InnerPin.Timer.ET;
         	if (1 == pMold->InnerPin.Timer.Q)
         	{
         		pMold->InnerPin.Timer.IN     = 0;
         		pMold->TimeDis.InnerPinBlowDelay = 0;
         		pMold->InnerPin.Step = 300;
         	}
         	break;
         case 700:
         	if(pMold->TimeSet.InnerPinBlowTime != 0)
    		   {
   				pMold->InnerPin.Timer.IN = 1;
		         pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinBlowTime;
   				pMold->InnerPin.Step = 800;
   			}
   			else
   			{
   				pMold->InnerPin.Step = 900;
   			}
         	break;
         case 800:
         	pMold->ActInfo.InnerPin = 1;
         	pMold->InnerPin.Out     = 2;
         	pMold->TimeDis.InnerPinBlowTime = pMold->InnerPin.Timer.ET;
         	if (1 == pMold->InnerPin.Timer.Q)
         	{
         		pMold->InnerPin.Out      = 0;
         		pMold->InnerPin.Timer.IN = 0;
         		pMold->TimeDis.InnerPinBlowTime = 0;
         		pMold->InnerPin.Step = 900;
         	}
         	break;
         case 900:
         	pMold->InnerPin.Timer.IN = 0;
         	pMold->InnerPin.Out     = 0;
         	pMold->ActInfo.InnerPin = 0;
         	pMold->InnerPin.Step = 3000;
         	break;*/
         case 3000:
         	break;

         case 10100: 	/* InnerPin backward delay*/
   			if(pMold->Option.InnerPin)
   			{
   				if(pMold->TimeSet.InnerPinBwDelay != 0)
       			{
       				pMold->InnerPin.Timer.IN = 1;
   			      pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinBwDelay;
       				pMold->InnerPin.Step = 10200;
       			}
       			else
       			{
       				pMold->InnerPin.Step = 10300;
       			}
    		   }
    		   else
    		   {
    			   pMold->InnerPin.Step = 13000;
    		   }
          	break;
         case 10200: 	/* backward delay ok */
         	pMold->TimeDis.InnerPinBwDelay = pMold->InnerPin.Timer.ET;
         	if (1 == pMold->InnerPin.Timer.Q)
         	{
         		pMold->InnerPin.Timer.IN     = 0;
         		pMold->TimeDis.InnerPinBwDelay = 0;
         		pMold->InnerPin.Step = 10300;
         	}
         	break;
         case 10300:
         	pMold->InnerPin.Timer.IN = 1;
	         pMold->InnerPin.Timer.PT = pMold->TimeSet.InnerPinBwAlarmTime;
    		   pMold->InnerPin.Step = 10400;
         	break;
			case 10400:
				pMold->ActInfo.InnerPin = 0;
				pMold->InnerPin.Out     = 0;

				if(pMold->TransDIn.InnerPinBwLimit)
				{
					pMold->InnerPin.Timer.IN = 0;
					pMold->InnerPin.Step = 13000;
				}

				pMold->TimeDis.InnerPinBwAlarmTime = pMold->InnerPin.Timer.ET;

				if (1 == pMold->InnerPin.Timer.Q)
				{
					pMold->Alarm.InnerPinNotAtBwPos = 1;
					pMold->InnerPin.Step = 40000;
				}
				break;
			case 13000:
				break;

			/*  normal stop  */
         case 20000:
            pMold->InnerPin.Out      = 0;
				pMold->InnerPin.Timer.IN = 0;

				pMold->ActInfo.InnerPin = 0;
				pMold->InnerPin.Step    = 0;
				break;

			/*--------------- Instancy stop -----------------*/
			case 30000:
				pMold->InnerPin.Out      = 0;
				pMold->InnerPin.Timer.IN = 0;

				pMold->ActInfo.InnerPin  = 0;
				pMold->InnerPin.Step     = 0;
				break;

			/*--------------- Error stop -----------------*/
			case 40000:
				pMold->InnerPin.Out      = 0;
				pMold->InnerPin.Timer.IN = 0;

				pMold->ActInfo.InnerPin  = 0;
				break;
	 }/* end of switch */

	if(pMold->InnerPin.Step > 100 && pMold->InnerPin.Step < 900)
	{
	 	if(!pMold->Option.InnerPin && 1 == gMachineInfo.Auto)
	 	{
			pMold->InnerPin.Step  = 20000;
		}
	}

}/*end of function */




