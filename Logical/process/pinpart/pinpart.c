/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: pinpart
 * File: pinpart.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program pinpart --- pinpart control 
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void PinPart( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.PinPart.Step = 30000;
 	RMold.PinPart.Step = 30000;

 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	RMold.TransDIn.PinPartFwLimit = gMachineIn.RMold.PinPartFwLimit;
	RMold.TransDIn.PinPartBwLimit = gMachineIn.RMold.PinPartBwLimit;

	LMold.TransDIn.PinPartFwLimit = gMachineIn.LMold.PinPartFwLimit;
	LMold.TransDIn.PinPartBwLimit = gMachineIn.LMold.PinPartBwLimit;

	if(!LMold.TimeSet.PinPartBwAlarmTime)LMold.TimeSet.PinPartBwAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.PinPartBwAlarmTime)RMold.TimeSet.PinPartBwAlarmTime = 500;  /* 5s  */

	if(!LMold.TimeSet.PinPartFwAlarmTime)LMold.TimeSet.PinPartFwAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.PinPartFwAlarmTime)RMold.TimeSet.PinPartFwAlarmTime = 500;  /* 5s  */

	PinPart(&LMold);
	PinPart(&RMold);

	gMachineOut.LMold.PinPartBw = LMold.ValveOut.PinPartBw;
	gMachineOut.LMold.PinPartFw = LMold.ValveOut.PinPartFw;

	gMachineOut.RMold.PinPartBw = RMold.ValveOut.PinPartBw;
	gMachineOut.RMold.PinPartFw = RMold.ValveOut.PinPartFw;

 } /* end of _CYCLIC */

void PinPart( Mold_typ * pMold)
{
   
   switch ( pMold->PinPart.Step )
   {
      /*------------------ stop all the output ------------------*/
  	   case 0:
     		pMold->ActInfo.PinPartFw  = 0;
     		pMold->ActInfo.PinPartBw  = 0;
			pMold->ValveOut.PinPartFw = 0;
			pMold->ValveOut.PinPartBw = 0;

         pMold->PinPart.Out  		= 0;
         pMold->PinPart.Timer.IN	= 0;
         break;


     /*--------------- PinPart forward -----------------*/
   	case 100: 	/* forward delay*/
   		if(1 == pMold->Option.PinPart)
   		{
       		pMold->PinPart.Timer.IN = 1;
   	      pMold->PinPart.Timer.PT = pMold->TimeSet.PinPartFwDelay;
       		pMold->PinPart.Step = 200;
   		}
   	    else
   		{
   			pMold->PinPart.Step = 3000;
   		}
         break;

      case 200: 	/* delay ok */
         pMold->TimeDis.PinPartFwDelay = pMold->PinPart.Timer.ET;
         if (1 == pMold->PinPart.Timer.Q)
         {
         	pMold->PinPart.Timer.IN        = 0;
         	pMold->TimeDis.PinPartFwDelay = 0;
         	pMold->PinPart.Step = 300;
         }
         break;

      case 300:
			pMold->PinPart.LimitTimer.IN = 1;
			pMold->PinPart.LimitTimer.PT = pMold->TimeSet.PinPartFwAlarmTime;
			pMold->PinPart.Step = 400;
         break;

      case 400:
         pMold->ActInfo.PinPartFw  = 1;
     		pMold->ActInfo.PinPartBw  = 0;

			pMold->ValveOut.PinPartFw = 1;

			pMold->TimeDis.PinPartFwAlarmTime = pMold->PinPart.LimitTimer.ET;

			if(1 == pMold->TransDIn.PinPartFwLimit)
			{
				pMold->PinPart.LimitTimer.IN = 0;
				pMold->PinPart.Step = 500;
			}

			if(1 == pMold->PinPart.LimitTimer.Q)
	        {
				pMold->PinPart.LimitTimer.IN = 0;
				pMold->Alarm.PinPartNotAtFwPos = 1;
				pMold->PinPart.Step = 40000;
			}
         break;

      case 500:
			pMold->PinPart.Timer.IN      = 0;
			pMold->PinPart.LimitTimer.IN = 0;

			pMold->ActInfo.PinPartFw  = 0;
			pMold->ValveOut.PinPartFw = 0;
			pMold->PinPart.Step = 3000;
			break;

      case 3000:
         pMold->ActInfo.PinPartFw  = 0;
         break;


     /*------------------ PinPart backward -------------------- */
  	   case 10100: 	/* Backward delay*/
   	   if(1 == pMold->Option.PinPart)
   		{
   	      if(pMold->TimeSet.PinPartBwDelay != 0)
   			{
   				pMold->PinPart.Timer.IN = 1;
		         pMold->PinPart.Timer.PT = pMold->TimeSet.PinPartBwDelay;
   				pMold->PinPart.Step 	= 10200;
   			}
   	      else
   			{
   				pMold->PinPart.Step = 10300;
   			}
    	   }
   	  	else
   		{
   			pMold->PinPart.Step = 13000;
   		}
         break;

    	case 10200: 	/* delay ok */
         pMold->TimeDis.PinPartBwDelay = pMold->PinPart.Timer.ET;
         if (1 == pMold->PinPart.Timer.Q)
         {
         	pMold->PinPart.Timer.IN       = 0;
         	pMold->TimeDis.PinPartBwDelay = 0;
         	pMold->PinPart.Step = 10300;
         }
         break;

    	case 10300:
         pMold->PinPart.LimitTimer.IN = 1;
         pMold->PinPart.LimitTimer.PT = pMold->TimeSet.PinPartBwAlarmTime;
         pMold->PinPart.Step = 10400;
         break;

    	case 10400:
			pMold->ActInfo.PinPartFw  = 0;
     		pMold->ActInfo.PinPartBw  = 1;

			pMold->ValveOut.PinPartBw = 1;
			pMold->ValveOut.PinPartFw = 0;

			if(1 == pMold->TransDIn.PinPartBwLimit)
         {
            pMold->ActInfo.PinPartBw  = 0;
				pMold->ValveOut.PinPartBw = 0;

				pMold->PinPart.Timer.IN = 0;
            pMold->PinPart.Step = 10500;
         }

         pMold->TimeDis.PinPartBwAlarmTime = pMold->PinPart.LimitTimer.ET;

         if(1 == pMold->PinPart.LimitTimer.Q)
         {
         	pMold->PinPart.Timer.IN   = 0;
         	pMold->Alarm.PinPartNotAtBwPos = 1;
         	pMold->PinPart.Step = 40000;
         }
			break;

		case 10500:
			pMold->ActInfo.PinPartFw  = 0;
     		pMold->ActInfo.PinPartBw  = 0;

			pMold->ValveOut.PinPartBw = 0;

			pMold->PinPart.Timer.IN = 0;
			pMold->PinPart.LimitTimer.IN = 0;
			pMold->PinPart.Step = 13000;
			break;

      case 13000:
         break;
		
		case 18000:
			pMold->ActInfo.PinPartFw  = 0;
			pMold->ActInfo.PinPartBw  = 1;
			pMold->ValveOut.PinPartBw = 1;
			break;

		/*--------------- Manual action stop -----------------*/
      case 20000:
			pMold->PinPart.Timer.IN = 0;
			pMold->PinPart.LimitTimer.IN = 0;

			pMold->ActInfo.PinPartFw = 0;
			pMold->ActInfo.PinPartBw = 0;

			pMold->ValveOut.PinPartFw = 0;
			pMold->ValveOut.PinPartBw = 0;

			pMold->PinPart.Step = 0;
			break;

		/*--------------- Instancy stop -----------------*/
		case 30000:
			pMold->PinPart.Timer.IN = 0;
			pMold->PinPart.LimitTimer.IN = 0;

			pMold->ActInfo.PinPartFw = 0;
			pMold->ActInfo.PinPartBw = 0;

			pMold->ValveOut.PinPartFw = 0;
			pMold->ValveOut.PinPartBw = 0;

			pMold->PinPart.Step = 0;
			break;

		/*--------------- Auto error stop -----------------*/
		case 40000:
			pMold->PinPart.Timer.IN = 0;
			pMold->PinPart.LimitTimer.IN = 0;

			pMold->ActInfo.PinPartFw = 0;
			pMold->ActInfo.PinPartBw = 0;

			pMold->ValveOut.PinPartFw = 0;
			pMold->ValveOut.PinPartBw = 0;
			break;
	}/* switch ( pMold->PinPart.Step )  */
}/* end of function  */


