/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: cutter
 * File: cutter.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cutter --- cutter control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void Cutter( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.Cutter.Step = 0;
 	RMold.Cutter.Step = 0;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {

	Cutter(&LMold);
	Cutter(&RMold);

//	if(1 == gMachineInfo.Auto)
//	{
//		if( 1 == LMold.Option.CutterMode)	/*  双向切刀 */
//		{
//			if(0 == LMold.ValveOut.Cutter && 0 == RMold.ValveOut.Cutter)
//			{
//				gMachineOut.CutterFw = 0;
//				gMachineOut.CutterBw = 0;
//			}
//			else if(1 == LMold.ValveOut.Cutter || 1 == RMold.ValveOut.Cutter)
//			{
//				gMachineOut.CutterFw = 1;
//				gMachineOut.CutterBw = 0;
//			}
//			else
//			{
//				gMachineOut.CutterFw = 0;
//				gMachineOut.CutterBw = 1;
//			}
//		}
//		else								/*  单向切刀 */
//		{
//			if (EDGEPOS(LMold.ValveOut.Cutter ,Cutter_L_P) || EDGEPOS(RMold.ValveOut.Cutter ,Cutter_R_P))
//			{
//				if(0 == gMachineOut.CutterBw)
//				{
//					gMachineOut.CutterFw = !gMachineOut.CutterFw;
//				}
//	
//				if(0 == gMachineOut.CutterFw)
//				{
//					gMachineOut.CutterBw = !gMachineOut.CutterBw;
//					if(0 == gMachineOut.CutterBw)gMachineOut.CutterFw = ! gMachineOut.CutterFw;
//				}
//			}
//		}
//	}
//	else
//	{
//		if(0 == LMold.ValveOut.Cutter && 0 == RMold.ValveOut.Cutter)
//		{
//			gMachineOut.CutterFw = 0;
//			gMachineOut.CutterBw = 0;
//		}
//		else if(1 == LMold.ValveOut.Cutter || 1 == RMold.ValveOut.Cutter)
//		{
//			gMachineOut.CutterFw = 1;
//			gMachineOut.CutterBw = 0;
//		}
//		else
//		{
//			gMachineOut.CutterFw = 0;
//			gMachineOut.CutterBw = 1;
//		}	
//	}
	
//	Cutter_L_P = LMold.ValveOut.Cutter;
//	Cutter_R_P = RMold.ValveOut.Cutter;
	

	
	gMachineOut.CutterFw = LMold.ValveOut.Cutter | RMold.ValveOut.Cutter;
 } /* end of _CYCLIC */

void Cutter( Mold_typ * pMold)
{
	 switch ( pMold->Cutter.Step )
     {
      		/*------------------ stop all the output ------------------*/
            case 0:
//            	pMold->ValveOut.Cutter  	   = 0;
//            	pMold->ActInfo.Cutter  = 0;

            	pMold->Cutter.Timer.IN	= 0;
            	break;

      		/*------------------ Cutter course -------------------- */
      		case 100: /* close delay*/
			if ( pMold->Option.Cutter)
			{
				if(pMold->TimeSet.CutterDelay != 0)
				{
					pMold->Cutter.Timer.IN = 1;
					pMold->Cutter.Timer.PT = pMold->TimeSet.CutterDelay;
					pMold->Cutter.Step 	   = 200;
				}
				else
				{
					pMold->Cutter.Step = 300;
				}
			}
			else
			{
				pMold->Cutter.Step = 3000;
			}

			
			
             	break;

            case 200: /* delay ok */
            	pMold->TimeDis.CutterDelay = pMold->Cutter.Timer.ET;
            	if (1 == pMold->Cutter.Timer.Q)
            	{
            		pMold->Cutter.Timer.IN     = 0;
            	//	pMold->TimeDis.CutterDelay = 0;
            		pMold->Cutter.Step = 300;
            	}
            	break;

            case 300:/*Determine Cutter mode , if double cutter goes to 400, single cutter goes to 1100*/
					/*   涨气启动   */
//				RMold.SupportAir.Step = 100;
				if(1 == gMachineInfo.Auto)
				{
//						if(RMold.SupportAir.Step == 0)RMold.SupportAir.Step = 100;
					
					if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
					{
						if(pMold == &RMold)
						{
							LMold.OnceSupport  = 1;
						}
						else
						{
							RMold.OnceSupport  = 1;
						}
					}
					else
					{
						pMold->OnceSupport  = 1;
					}
				}

            	if( 1 == pMold->Option.CutterMode)/*Double Cutter*/
            	{
					if( 0 ==  pMold->ValveOut.Cutter)
					{
						if( SUPPORTAIR_MODE_STANDARD == gMachineFix.MoldR.SupportAir.eSupportAirMode)
						{
							pMold->SupportAir.Step = 100;// air support， only for double cutter
						}
					
						if(pMold->TimeSet.CutterTime != 0)
						{
							pMold->Cutter.Timer.IN = 1;
							pMold->Cutter.Timer.PT = pMold->TimeSet.CutterTime;
							pMold->Cutter.Step = 400;
						}
						else
						{
							/*   cutter time gAlarm   */
							pMold->Cutter.Step = 450;
						}
					
					}
					else /*If cutter is retain on Fw position , then do Single Cutter again to go back Bw Position*/
					{
						pMold->Cutter.Step = 1100;
					}
			
	            }
	            else/*Single Cutter*/
	            {
	            	pMold->Cutter.Step = 1100;
	            }
            	break;

            case 400:/*Double cutter, move forward until time expired*/
            	pMold->ActInfo.Cutter = 1;
				pMold->ValveOut.Cutter  = 1;
			
            	pMold->TimeDis.CutterTime = pMold->Cutter.Timer.ET;
            	if (1 == pMold->Cutter.Timer.Q)
            	{
            		pMold->Cutter.Timer.IN    = 0;
            //		pMold->TimeDis.CutterTime = 0;
					pMold->Cutter.Step = 450;/*Change to case 450*/
            	}
            	break;

			case 450:/*Time expire or no cutter time setting*/
				pMold->ValveOut.Cutter = 0;
				pMold->ActInfo.Cutter  = 0;
			
				pMold->Cutter.Step 	  = 3000;
			break;
		
            case 500:/*Current no used case*/
            	pMold->Cutter.Timer.IN = 1;
					pMold->Cutter.Timer.PT = pMold->TimeSet.CutterBwTime;
					pMold->Cutter.Step = 600;
            	break;

            case 600:/*Current no used case*/
            	pMold->ValveOut.Cutter     = 2;
            	pMold->ActInfo.Cutter = 1;

            	pMold->TimeDis.CutterBwTime = pMold->Cutter.Timer.ET;
            	if (1 == pMold->Cutter.Timer.Q)
            	{
            		pMold->Cutter.Timer.IN      = 0;
            	//	pMold->TimeDis.CutterBwTime = 0;
            		pMold->Cutter.Step = 700;
            	}
				break;

            case 700:/*Current no used case*/
            	pMold->ValveOut.Cutter = 0;
            	pMold->ActInfo.Cutter  = 0;
			
				pMold->Cutter.Step = 800;
            	break;

            case 800:/*Current no used case*/
            	pMold->ValveOut.Cutter     = 0;
            	pMold->ActInfo.Cutter = 0;

            	pMold->Cutter.Step = 3000;
            	break;

            case 1100:/*Single cutter , inverse output*/
            	pMold->ValveOut.Cutter = !pMold->ValveOut.Cutter;/*Albert*/
            	pMold->ActInfo.Cutter = pMold->ValveOut.Cutter;
            	pMold->Cutter.Step = 3000;
            	break;

            case 1200:/*Current no used case*/
            	pMold->Cutter.Step = 1300;
            	break;

            case 1300:/*Current no used case*/
               	pMold->ValveOut.Cutter     = 0;
            	pMold->ActInfo.Cutter = 0;

            	pMold->Cutter.Step = 3000;
            	break;

            case 3000:
            	break;
		      
			case 5000:
				pMold->ValveOut.Cutter     = 1;
				pMold->ActInfo.Cutter = 1;
				break;
		
            /*--------------- Manual action stop -----------------*/
            case 20000:
				if(pMold->Option.CutterMode)/*Albert Double cutter*/
				{
					pMold->ValveOut.Cutter      = 0;
				}
			
					pMold->Cutter.Timer.IN = 0;
	
				//	pMold->TimeDis.CutterTime  = 0;
				//	pMold->TimeDis.CutterDelay = 0;
	
					pMold->ActInfo.Cutter  = 0;
					pMold->Cutter.Step 	   = 0;
					break;

			/*--------------- Instance stop -----------------*/
			case 30000:
				if(pMold->Option.CutterMode)/*Albert Double Cutter*/
				{
					pMold->ValveOut.Cutter      = 0;
				}
				pMold->Cutter.Timer.IN = 0;

			//	pMold->TimeDis.CutterTime  = 0;
			//	pMold->TimeDis.CutterDelay = 0;

				pMold->ActInfo.Cutter  = 0;
				pMold->Cutter.Step     = 0;
				break;

	 }/* end of switch */
}/*end of function */




