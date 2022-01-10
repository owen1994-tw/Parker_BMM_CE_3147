/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: cutneck
 * File: cutneck.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cutneck --- cut neck control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void CutNeck( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.CutNeck.Step = 30000;
 	RMold.CutNeck.Step = 30000;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	LMold.TransDIn.CutNeckOpnLimit = gMachineIn.LMold.CutNeckOpnLimit = 1;
	RMold.TransDIn.CutNeckOpnLimit = gMachineIn.RMold.CutNeckOpnLimit = 1;

	if(!LMold.TimeSet.CutNeckOpnAlarmTime)LMold.TimeSet.CutNeckOpnAlarmTime = 1000;  /* 10s  */
	if(!RMold.TimeSet.CutNeckOpnAlarmTime)RMold.TimeSet.CutNeckOpnAlarmTime = 1000;  /* 10s  */

	CutNeck(&LMold);
	CutNeck(&RMold);

	gMachineOut.LMold.CutNeckCls = LMold.ValveOut.CutNeckCls;
	gMachineOut.RMold.CutNeckCls = RMold.ValveOut.CutNeckCls;

	gMachineOut.LMold.CutNeck    = LMold.ValveOut.CutNeck;
	gMachineOut.RMold.CutNeck    = RMold.ValveOut.CutNeck;

 } /* end of _CYCLIC */

void CutNeck( Mold_typ * pMold)
{

	 switch ( pMold->CutNeck.Step )
    {
   		/*------------------ stop all the output ------------------*/
         case 0:
         	pMold->CutNeck.Out = 0;

         	pMold->ActInfo.CutNeck    = 0;
         	pMold->ActInfo.CutNeckCls = 0;

         	pMold->CutNeck.Timer.IN	= 0;
         	break;

   		/*------------------ CutNeck forward -------------------- */
   		case 100: 					/* CutNeck forward delay*/
   			if(pMold->Option.CutNeck)
   			{
   				if(pMold->TimeSet.CutNeckClsDelay != 0)
    			   {
       				pMold->CutNeck.Timer.IN = 1;
   			      pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckClsDelay;
       				pMold->CutNeck.Step = 200;
       			}
       			else
       			{
       				pMold->CutNeck.Step = 300;
       			}
       		}
       		else
       		{
       			pMold->CutNeck.Step = 3000;
       		}
          	break;
         case 200: 					/* CutNeck forward delay ok */
         	pMold->TimeDis.CutNeckClsDelay = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->CutNeck.Timer.IN     = 0;
         		pMold->TimeDis.CutNeckClsDelay = 0;
         		pMold->CutNeck.Step = 300;
         	}
         	break;
         case 300:
         	if(pMold->TimeSet.CutNeckClsTime != 0)
         	{
         		pMold->CutNeck.Timer.IN = 1;
		         pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckClsTime;
		         pMold->CutNeck.Step = 400;
         	}
         	else
         	{
         		pMold->CutNeck.Step = 500;
         	}
         	break;
         case 400:					/* CutNeck forward time ok */
         	pMold->ActInfo.CutNeck    = 0;
         	pMold->ActInfo.CutNeckCls = 1;

         	pMold->ValveOut.CutNeckCls = 1;
         	pMold->ValveOut.CutNeck    = 0;

         	pMold->CutNeck.Out = 1;

         	pMold->TimeDis.CutNeckClsTime = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->CutNeck.Timer.IN = 0;
         		pMold->TimeDis.CutNeckClsTime = 0;
         		pMold->CutNeck.Step = 500;
         	}
         	break;
   		case 500: /* CutNeck delay*/
   			if(pMold->TimeSet.CutNeckDelay != 0)
   			{
   				pMold->CutNeck.Timer.IN = 1;
		         pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckDelay;
   				pMold->CutNeck.Step = 600;
   			}
   			else
   			{
   				pMold->CutNeck.Step = 700;
   			}
          	break;
         case 600: 					/* CutNeck close delay ok */
         	pMold->TimeDis.CutNeckDelay = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->CutNeck.Timer.IN     = 0;
         		pMold->TimeDis.CutNeckDelay = 0;
         		pMold->CutNeck.Step = 700;
         	}
         	break;
         case 700:
         	if(pMold->TransDIn.RobotOpnLimit)			/*  机械手开位   */
         	{
             	if(pMold->TimeSet.CutNeckTime != 0)
             	{
             		pMold->CutNeck.Timer.IN = 1;
   			      pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckTime;
   			      pMold->CutNeck.Step = 800;
             	}
             	else
             	{
             		pMold->CutNeck.Step = 900;
             	}
            }
         	break;
         case 800:
         	pMold->ActInfo.CutNeck    = 1;
         	pMold->ActInfo.CutNeckCls = 1;

         	pMold->ValveOut.CutNeckCls = 1;
         	pMold->ValveOut.CutNeck    = 1;

         	pMold->CutNeck.Out = 2;

         	pMold->TimeDis.CutNeckTime = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->ValveOut.CutNeck    = 0;
		         pMold->CutNeck.Out = 0;

         		pMold->CutNeck.Timer.IN    = 0;
         		pMold->TimeDis.CutNeckTime = 0;
         		pMold->CutNeck.Step = 900;
         	}
         	break;
         case 900:
         	if(pMold->TimeSet.CutNeckOpnDelay != 0)
    		   {
   				pMold->CutNeck.Timer.IN = 1;
		         pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckOpnDelay;
   				pMold->CutNeck.Step = 1000;
   			}
   			else
   			{
   				pMold->CutNeck.Step = 1100;
   			}
         	break;
         case 1000:
         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 1;

         	pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 1;

         	pMold->CutNeck.Out = 3;

         	pMold->TimeDis.CutNeckOpnDelay = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->ValveOut.CutNeckCls = 0;
         		pMold->CutNeck.Timer.IN    = 0;
         		pMold->TimeDis.CutNeckOpnDelay = 0;
         		pMold->CutNeck.Step = 1100;
         	}
         	break;
         case 1100:
         	pMold->CutNeck.Timer.IN = 1;
	         pMold->CutNeck.Timer.PT = pMold->TimeSet.CutNeckOpnAlarmTime;
	         pMold->CutNeck.Step = 1200;
         	break;
         case 1200:
         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 0;

	         pMold->CutNeck.Out = 0;

         	pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 0;

         	if(pMold->TransDIn.CutNeckOpnLimit)
         	{
         		pMold->CutNeck.Timer.IN = 0;
         		pMold->CutNeck.Step     = 1300;
         	}
         	pMold->TimeDis.CutNeckOpnAlarmTime = pMold->CutNeck.Timer.ET;
         	if (1 == pMold->CutNeck.Timer.Q)
         	{
         		pMold->CutNeck.Timer.IN = 0;
         		pMold->Alarm.CutNeckFwNotAtOpnPos = 1;
         		pMold->CutNeck.Step = 40000;
         	}
         	break;
         case 1300:
         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 0;

         	pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 0;

	         pMold->CutNeck.Out = 0;

         	pMold->CutNeck.Timer.IN = 0;
         	pMold->ActInfo.CutNeck  = 0;
         	pMold->CutNeck.Step = 3000;
         	break;
         case 3000:
         	break;

         /*--------------- Manual action stop -----------------*/
         case 20000:
         	pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 0;

         	pMold->CutNeck.Out      = 0;
	         pMold->CutNeck.Timer.IN = 0;

         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 0;

	         pMold->CutNeck.Step    = 0;
	         break;

			/*--------------- Instance stop -----------------*/
			case 30000:
	         pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 0;

	         pMold->CutNeck.Out      = 0;
         	pMold->CutNeck.Timer.IN = 0;

         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 0;

	         pMold->CutNeck.Step     = 0;
	         break;

         /*--------------- Error  stop -----------------*/
         case 40000:
         	pMold->ValveOut.CutNeckCls = 0;
         	pMold->ValveOut.CutNeck    = 0;

	         pMold->CutNeck.Out      = 0;
	         pMold->CutNeck.Timer.IN = 0;

         	pMold->ActInfo.CutNeckCls = 0;
         	pMold->ActInfo.CutNeck    = 0;
	         break;
	 }/* end of switch */

	if(pMold->CutNeck.Step > 100 && pMold->CutNeck.Step < 1300)
	{
	 	if(!pMold->Option.CutNeck && 1 == gMachineInfo.Auto)
	 	{
			pMold->CutNeck.Step = 1300;
		}
	}
}/*end of function */


