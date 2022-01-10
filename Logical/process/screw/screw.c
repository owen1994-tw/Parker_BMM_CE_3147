/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: screw
 * File: screw.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program screw --- screw  up and down control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void Screw( Mold_typ * pMold,Screw_Para_typ * pScrewPara);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.Screw.Step = 30000;
 	RMold.Screw.Step = 30000;

 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	RMold.TransDIn.ScrewFwLimit = gMachineIn.RMold.ScrewFwLimit;
	RMold.TransDIn.ScrewBwLimit = gMachineIn.RMold.ScrewBwLimit;

	LMold.TransDIn.ScrewFwLimit = gMachineIn.LMold.ScrewFwLimit;
	LMold.TransDIn.ScrewBwLimit = gMachineIn.LMold.ScrewBwLimit;

	if(!LMold.TimeSet.ScrewBwAlarmTime)LMold.TimeSet.ScrewBwAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.ScrewBwAlarmTime)RMold.TimeSet.ScrewBwAlarmTime = 500;  /* 5s  */

	if(!LMold.TimeSet.ScrewFwAlarmTime)LMold.TimeSet.ScrewFwAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.ScrewFwAlarmTime)RMold.TimeSet.ScrewFwAlarmTime = 500;  /* 5s  */

	Screw(&LMold,&gMachinePara.MoldL.Screw);
	Screw(&RMold,&gMachinePara.MoldR.Screw);

	gMachineOut.LMold.ScrewBw = LMold.ValveOut.ScrewBw;
	gMachineOut.LMold.ScrewFw = LMold.ValveOut.ScrewFw;

	gMachineOut.RMold.ScrewBw = RMold.ValveOut.ScrewBw;
	gMachineOut.RMold.ScrewFw = RMold.ValveOut.ScrewFw;

 } /* end of _CYCLIC */

void Screw( Mold_typ * pMold,Screw_Para_typ * pScrewPara)
{
	 switch ( pMold->Screw.Step )
    {
      	/*------------------ stop all the output ------------------*/
     	   case 0:
     		   pMold->ActInfo.ScrewFw  = 0;
     		   pMold->ActInfo.ScrewBw  = 0;
            pMold->Screw.Timer.IN	= 0;
            break;

        /*--------------- Screw forward -----------------*/
      	case 100: 	/* forward delay*/
      		if(1 == pMold->Option.Screw)
      		{
	      		pMold->Screw.Timer.IN = 1;
				   pMold->Screw.Timer.PT = pMold->TimeSet.ScrewFwDelay;
	      		pMold->Screw.Step = 200;
      		}
      	    else
      		{
      			pMold->Screw.Step = 3000;
      		}
            break;

         case 200: 	/* delay ok */
            pMold->TimeDis.ScrewFwDelay = pMold->Screw.Timer.ET;
            if (1 == pMold->Screw.Timer.Q)
            {
            	pMold->Screw.Timer.IN        = 0;
            	pMold->TimeDis.ScrewFwDelay = 0;
            	pMold->Screw.Step = 300;
            }
            break;

         case 300:
   			pMold->Screw.LimitTimer.IN = 1;
   			pMold->Screw.LimitTimer.PT = pMold->TimeSet.ScrewFwAlarmTime;
   			pMold->Screw.Step = 400;
            break;

         case 400:
            pMold->ActInfo.ScrewFw  = 1;
     		   pMold->ActInfo.ScrewBw  = 0;

			   pMold->ValveOut.ScrewFw = 1;
				pMold->ValveOut.ScrewBw = 0;
			
				pMold->Screw.p_set = pScrewPara->SpFw;
				pMold->Screw.v_set = pScrewPara->SvFw;

			   pMold->TimeDis.ScrewFwAlarmTime = pMold->Screw.LimitTimer.ET;

   			if(1 == pMold->TransDIn.ScrewFwLimit) 
   			{
					pMold->Screw.p_set = 0;
					pMold->Screw.v_set = 0;
	
   				pMold->Screw.LimitTimer.IN = 0;
   				pMold->Screw.Step = 500;
   			}
   
   			if(1 == pMold->Screw.LimitTimer.Q)
   	        {
   				pMold->Screw.LimitTimer.IN = 0;
   				pMold->Alarm.ScrewNotAtFwPos = 1;
   				pMold->Screw.Step = 40000;
   			}
			   break;

		   case 500:
   			pMold->Screw.Timer.IN      = 0;
   			pMold->Screw.LimitTimer.IN = 0;
   
   			pMold->ActInfo.ScrewFw  = 0;
   			pMold->ValveOut.ScrewFw = 0;
   			pMold->Screw.Step = 3000;
   			break;

        case 3000:
            pMold->ActInfo.ScrewFw  = 0;
            break;


        /*------------------ Screw backward -------------------- */
     	  case 10100: 	/* Backward delay*/
      	   if(1 == pMold->Option.Screw)
      		{
      	      if(pMold->TimeSet.ScrewBwDelay != 0)
      			{
      				pMold->Screw.Timer.IN = 1;
					   pMold->Screw.Timer.PT = pMold->TimeSet.ScrewBwDelay;
      				pMold->Screw.Step 	= 10200;
      			}
      	      else
      			{
      				pMold->Screw.Step = 10300;
      			}
	      	}
      	  	else
      		{
      			pMold->Screw.Step = 13000;
      		}
            break;

       	case 10200: 	/* delay ok */
            pMold->TimeDis.ScrewBwDelay = pMold->Screw.Timer.ET;
            if (1 == pMold->Screw.Timer.Q)
            {
            	pMold->Screw.Timer.IN       = 0;
            	pMold->TimeDis.ScrewBwDelay = 0;
            	pMold->Screw.Step = 10300;
            }
            break;

       	case 10300:
			   pMold->Screw.LimitTimer.IN = 1;
            pMold->Screw.LimitTimer.PT = pMold->TimeSet.ScrewBwAlarmTime;
            pMold->Screw.Step = 10400;
			   break;

       	case 10400:
   			pMold->ActInfo.ScrewFw  = 0;
        		pMold->ActInfo.ScrewBw  = 1;
   
   			pMold->ValveOut.ScrewBw = 1;
   			pMold->ValveOut.ScrewFw = 0;
			
				pMold->Screw.p_set = pScrewPara->SpFw;
				pMold->Screw.v_set = pScrewPara->SvFw;

   			if(1 == pMold->TransDIn.ScrewBwLimit)
            {
					pMold->Screw.p_set = 0;
					pMold->Screw.v_set = 0;
			
               pMold->ActInfo.ScrewBw  = 0;
   				pMold->ValveOut.ScrewBw = 0;
   
   				pMold->Screw.Timer.IN = 0;
               pMold->Screw.Step = 10500;
            }

            pMold->TimeDis.ScrewBwAlarmTime = pMold->Screw.LimitTimer.ET;

            if(1 == pMold->Screw.LimitTimer.Q)
            {
            	pMold->Screw.Timer.IN   = 0;
            	pMold->Alarm.ScrewNotAtBwPos = 1;
            	pMold->Screw.Step = 40000;
            }
			   break;

		   case 10500:
   			pMold->ActInfo.ScrewFw  = 0;
        		pMold->ActInfo.ScrewBw  = 0;
   
   			pMold->ValveOut.ScrewBw = 0;
   
   			pMold->Screw.Timer.IN = 0;
   			pMold->Screw.LimitTimer.IN = 0;
   			pMold->Screw.Step = 13000;
   			break;

       	case 13000:
            break;

   		/*--------------- Manual action stop -----------------*/
           case 20000:
   			pMold->Screw.Timer.IN = 0;
   			pMold->Screw.LimitTimer.IN = 0;
   
   			pMold->ActInfo.ScrewFw = 0;
   			pMold->ActInfo.ScrewBw = 0;
   
   			pMold->ValveOut.ScrewFw = 0;
   			pMold->ValveOut.ScrewBw = 0;
			
				pMold->Screw.p_set = 0;
				pMold->Screw.v_set = 0;
   
   			pMold->Screw.Step = 0;
   			break;
   
   		/*--------------- Instancy stop -----------------*/
   		case 30000:
   			pMold->Screw.Timer.IN = 0;
   			pMold->Screw.LimitTimer.IN = 0;
   
   			pMold->ActInfo.ScrewFw = 0;
   			pMold->ActInfo.ScrewBw = 0;
   
   			pMold->ValveOut.ScrewFw = 0;
   			pMold->ValveOut.ScrewBw = 0;
			
				pMold->Screw.p_set = 0;
				pMold->Screw.v_set = 0;
   
   			pMold->Screw.Step = 0;
   			break;
   
   		/*--------------- Auto error stop -----------------*/
   		case 40000:
   			pMold->Screw.Timer.IN = 0;
   			pMold->Screw.LimitTimer.IN = 0;
   
   			pMold->ActInfo.ScrewFw = 0;
   			pMold->ActInfo.ScrewBw = 0;
   
   			pMold->ValveOut.ScrewFw = 0;
   			pMold->ValveOut.ScrewBw = 0;
			
				pMold->Screw.p_set = 0;
				pMold->Screw.v_set = 0;
   			break;
	}/* switch ( pMold->Screw.Step )  */
}


