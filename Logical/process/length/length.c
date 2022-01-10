/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: length
 * File: length.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program length --- parison length control
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void LengthCtrl(Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {

 } /* end of _INIT */

/*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
   /* length control timeout  */
   if(gMachinePara.LengthPara.TimeOut< 1)gMachinePara.LengthPara.TimeOut = 200.0;
   
   /* length incease / decrease RPM  */
   if(gMachinePara.LengthPara.IncRPM < 1)gMachinePara.LengthPara.IncRPM = 10.0;
   if(gMachinePara.LengthPara.DecRPM < 1)gMachinePara.LengthPara.DecRPM = 10.0;

   /* length time limit  */
   if(gMachinePara.LengthPara.SetLength > gThickProf.TimSet.CycleTime/100.0 - 0.5)
   {
      gMachinePara.LengthPara.SetLength = gThickProf.TimSet.CycleTime/100.0 - 0.5; 
   }
   
   if(gMachinePara.LengthPara.SetLength < 2.0)gMachinePara.LengthPara.SetLength = 2.0;
   
 	/* right length control  */
	/* right length control  */
	if(1 == gMachineOut.ExtrA_On && gExtruderA.Out > 5)
	{
		RMold.LengthCtrl.Enable = gMachinePara.LengthPara.LengthEnable;
	}
	else
	{
		RMold.LengthCtrl.Enable = 0;	
	}

 	RMold.LengthCtrl.MagicEye = gMachineIn.ParisonLength;
 	RMold.LengthCtrl.ClampOpn = RMold.Clamp.OpnPos;

 	RMold.LengthCtrl.TolLengthPosTime = gMachinePara.LengthPara.TolLengthPosTime;
 	RMold.LengthCtrl.TolLengthNegTime = gMachinePara.LengthPara.TolLengthNegTime;

	LengthCtrl(&RMold);

	/* left length control  */
	if(1 == gMachineOut.ExtrA_On && gExtruderA.Out > 5)
	{
		LMold.LengthCtrl.Enable = gMachinePara.LengthPara.LengthEnable;
	}
	else
	{
		LMold.LengthCtrl.Enable = 0;	
	}

 	LMold.LengthCtrl.MagicEye = gMachineIn.ParisonLength;
 	LMold.LengthCtrl.ClampOpn = LMold.Clamp.OpnPos;

 	LMold.LengthCtrl.TolLengthPosTime = gMachinePara.LengthPara.TolLengthPosTime;
 	LMold.LengthCtrl.TolLengthNegTime = gMachinePara.LengthPara.TolLengthNegTime;

 	LengthCtrl(&LMold);

 } /* end of _CYCLIC */

void LengthCtrl( Mold_typ * pMold )
{
	if(0 == pMold->LengthCtrl.Enable)
	{
		if(pMold->LengthCtrl.Step > 0 && pMold->LengthCtrl.Step < 20000)pMold->LengthCtrl.Step = 20000;
	}

	switch ( pMold->LengthCtrl.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->LengthCtrl.Out = 0;
			pMold->LengthCtrl.TimerCycle.IN = 0;
			pMold->LengthCtrl.Timer.IN = 0;
			break;
		
		/*------------------ length control   -------------------- */
		case 100:
			if(1 == pMold->LengthCtrl.Enable)
			{
				pMold->LengthCtrl.TimerCycle.IN = 0;
				pMold->LengthCtrl.TimerCycle.PT = 50000;
				pMold->LengthCtrl.Step = 150;
			}
			else
			{
				pMold->LengthCtrl.Step = 3000;
			}
			break;
		
		case 150:
			if(1 == pMold->LengthCtrl.Enable)
			{
				pMold->LengthCtrl.TimerCycle.IN = 1;
				pMold->LengthCtrl.TimerCycle.PT = 50000;
				pMold->LengthCtrl.Step = 200;
			}
			else
			{
				pMold->LengthCtrl.Step = 3000;
			}
			break;

		case 200:
			pMold->LengthCtrl.TimeCycleDis  = pMold->LengthCtrl.TimerCycle.ET;
			if(1 == pMold->Carriage.BwPos && 1 == pMold->OtherMoldBw)
			{
				pMold->LengthCtrl.Step = 300;
			}
			break;

		case 300:
			pMold->LengthCtrl.TimeCycleDis  = pMold->LengthCtrl.TimerCycle.ET;
			if(pMold->LengthCtrl.TimeCycleDis > 500)
			{
				if(1 == pMold->LengthCtrl.MagicEye)
				{
					pMold->LengthCtrl.TimerCycle.IN = 0;
			
					if(1 == pMold->LengthCtrl.Enable)
					{
						pMold->LengthCtrl.Out = gMachinePara.LengthPara.Kp * (pMold->LengthCtrl.TimeCycleDis - gMachinePara.LengthPara.SetLength * 100)/100;
			
						if(pMold->LengthCtrl.Out > gMachinePara.LengthPara.DecRPM)pMold->LengthCtrl.Out =   gMachinePara.LengthPara.IncRPM;
						if(pMold->LengthCtrl.Out < -gMachinePara.LengthPara.DecRPM)pMold->LengthCtrl.Out = -gMachinePara.LengthPara.DecRPM;
			
						gExtruderA.ProductionSpeed = gExtruderA.ProductionSpeed + pMold->LengthCtrl.Out;
						if(gExtruderA.ProductionSpeed > 1000)gExtruderA.ProductionSpeed = 1000;
						if(gExtruderA.ProductionSpeed < 50  )gExtruderA.ProductionSpeed = 50;
					}
					pMold->LengthCtrl.Step = 3000;
				}
			
				if(pMold->LengthCtrl.TimerCycle.ET > gMachinePara.LengthPara.TimeOut * 100)  /* time out */
				{
					pMold->LengthCtrl.TimerCycle.IN = 0;
					pMold->LengthCtrl.Step = 10000;
				}
			}
			break;

		case 3000:
			pMold->LengthCtrl.Timer.IN = 0;
			pMold->LengthCtrl.TimerCycle.IN = 0;
			pMold->LengthCtrl.Step = 0;
			break;
		
		/* --------  Òì³££¬Í£ÂÝ¸Ë --------*/
		case 10000:
			gAlarm.LengthTimeout = 1;
			pMold->LengthCtrl.Step = 0;
			pMold->LengthCtrl.Timer.IN = 0;
			pMold->LengthCtrl.TimerCycle.IN = 0;
			break;

		/* --------  Í£ÂÝ¸Ë --------*/
		case 20000:
		case 30000:
			pMold->LengthCtrl.Step = 0;
			pMold->LengthCtrl.Timer.IN = 0;
			pMold->LengthCtrl.TimerCycle.IN = 0;
			break;
	}/* end of switch */
	
}/*end of function */
