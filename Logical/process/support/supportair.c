/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: Supportair
 * File: Supportair.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program Supportair --- Supportair 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
//void SupportAir( Mold_typ * pMold,Support_Para_typ * pSupportPara);
void SupportAir( Mold_typ * pMold,Support_Para_typ * pSupportPara,Mold_Para_User_typ * pMoldUser);
void SupportAir_Time( Mold_typ * pMold,Support_Para_typ * pSupportPara,Mold_Para_User_typ * pMoldUser,Support_Fix_typ * pSupportFix);
void SupportSuckAir_Time( Mold_typ * pMold,Support_Para_typ * pSupportPara,Mold_Para_User_typ * pMoldUser);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	gMachinePara.SupportAirEnable[0] =0;  //ipis0305
	gMachinePara.SupportAirEnable[1] =0;
	
	if(gUserPara.FluxSensor[0].Zero < 0.1)gUserPara.FluxSensor[0].Zero = 1.0; 
	if(gUserPara.FluxSensor[0].Max < 0.1 )gUserPara.FluxSensor[0].Max  = 8.0; 
	if(gUserPara.FluxSensor[0].Length < 0.1 )gUserPara.FluxSensor[0].Length = 80.0;
	
	if(gUserPara.FluxSensor[0].Max - gUserPara.FluxSensor[0].Zero > 0.0)
	{	
		RMold.SupportAir.FlowDieA = (RMold.SupportAir.aiFlowDieA - gUserPara.FluxSensor[0].Zero * 3726.7) * gUserPara.FluxSensor[0].Length /
			                         (gUserPara.FluxSensor[0].Max * 3276.7 - gUserPara.FluxSensor[0].Zero * 3276.7);
	}
	else
	{
		RMold.SupportAir.FlowDieA = 0.0; 
	}
	
	LMold.SupportAir.FlowDieA = RMold.SupportAir.FlowDieA;
	
	if(gUserPara.FluxSensor[1].Zero < 0.1)gUserPara.FluxSensor[1].Zero = 1.0; 
	if(gUserPara.FluxSensor[1].Max < 0.1 )gUserPara.FluxSensor[1].Max  = 8.0; 
	if(gUserPara.FluxSensor[1].Length < 0.1 )gUserPara.FluxSensor[1].Length = 80.0;
	
	if(gUserPara.FluxSensor[1].Max - gUserPara.FluxSensor[1].Zero > 0.0)
	{	
		RMold.SupportAir.FlowDieB = (RMold.SupportAir.aiFlowDieB - gUserPara.FluxSensor[1].Zero * 3726.7) * gUserPara.FluxSensor[1].Length /
			                         (gUserPara.FluxSensor[2].Max * 3276.7 - gUserPara.FluxSensor[2].Zero * 3276.7);
	}
	else
	{
		RMold.SupportAir.FlowDieB = 0.0; 
	}
	
	LMold.SupportAir.FlowDieB = RMold.SupportAir.FlowDieB;
	
	if(gUserPara.FluxSensor[2].Zero < 0.1)gUserPara.FluxSensor[2].Zero = 1.0; 
	if(gUserPara.FluxSensor[2].Max < 0.1 )gUserPara.FluxSensor[2].Max  = 8.0;
	if(gUserPara.FluxSensor[2].Length < 0.1 )gUserPara.FluxSensor[2].Length = 80.0;
	
	if(gUserPara.FluxSensor[2].Max - gUserPara.FluxSensor[2].Zero != 0.0)
	{	
		RMold.SupportAir.FlowDieC = (RMold.SupportAir.aiFlowDieC - gUserPara.FluxSensor[2].Zero * 3726.7) * gUserPara.FluxSensor[2].Length /
			                         (gUserPara.FluxSensor[2].Max * 3276.7 - gUserPara.FluxSensor[2].Zero * 3276.7);
	}
	else
	{
		RMold.SupportAir.FlowDieC = 0.0; 
	}
	
	LMold.SupportAir.FlowDieB = RMold.SupportAir.FlowDieB;
		
	if(gUserPara.FluxSensor[3].Zero < 0.1)gUserPara.FluxSensor[3].Zero = 1.0; 
	if(gUserPara.FluxSensor[3].Max < 0.1 )gUserPara.FluxSensor[3].Max  = 8.0; 
	if(gUserPara.FluxSensor[3].Length < 0.1 )gUserPara.FluxSensor[3].Length = 80.0;
	
	if(gUserPara.FluxSensor[3].Max - gUserPara.FluxSensor[3].Zero != 0.0)
	{	
		RMold.SupportAir.FlowDieD = (RMold.SupportAir.aiFlowDieD - gUserPara.FluxSensor[3].Zero * 3726.7) * gUserPara.FluxSensor[3].Length /
			                         (gUserPara.FluxSensor[3].Max * 3276.7 - gUserPara.FluxSensor[3].Zero * 3276.7);
	}
	else
	{
		RMold.SupportAir.FlowDieD = 0.0; 
	}
	
	LMold.SupportAir.FlowDieD = RMold.SupportAir.FlowDieD;
		
	
//	SupportAir(&LMold, &gMachinePara.MoldL.SupportAir,&gUserPara.LMold);
//	SupportAir(&RMold, &gMachinePara.MoldR.SupportAir,&gUserPara.RMold);
	
	/*------------- SupportAirMode--------------- */
	gMachineFix.MoldL.SupportAir.eSupportAirMode = gMachineFix.MoldR.SupportAir.eSupportAirMode;
	
	SupportAir_Time(&LMold, &gMachinePara.MoldL.SupportAir,&gUserPara.LMold,&gMachinePara.MoldL.SupportAir);
	SupportAir_Time(&RMold, &gMachinePara.MoldR.SupportAir,&gUserPara.RMold,&gMachinePara.MoldR.SupportAir);
	
	SupportSuckAir_Time(&LMold, &gMachinePara.MoldL.SupportAir,&gUserPara.LMold);
	SupportSuckAir_Time(&RMold, &gMachinePara.MoldR.SupportAir,&gUserPara.RMold);
	
	
	if(1 == gMachineInfo.Auto)
	{
		gMachineOut.SupportAir_1 = LMold.SupportAir.Out_1 | RMold.SupportAir.Out_1;
		gMachineOut.SupportAir_2 = LMold.SupportAir.Out_2 | RMold.SupportAir.Out_2;
		
		/*----------------------------------------------------*/
		/*-------------------Left side------------------------*/
		/*----------------------------------------------------*/
		
		if(gThickProf.TimSet.CycleTime != 0)
		{			
			/*Albert Start count calculate left*/
			switch (uCntStepL)
			{
				case 0:/*weit step*/					
					if(LMold.SupportAir.cntTime > 0)
					{
						if(tTask != 0)
						{
							uCntTimeL = LMold.SupportAir.cntTime/100/tTask;
							uCntStepL = 1;
						}
						
					}					
					break;
				case 1:/*Start count, since tTask is 4ms so If we need s counting should chagne the counter*/
					if(uCntTimeL > 0)
					{
						uCntTimeL -= tTask;
					}
					else
					{
						LMold.SupportAir.cntTime = 0;
						uCntTimeL = 0;
						uCntStepL = 0;
					}
					
					break;
			}
			//	Make this time from 100ms to 1000ms
			//					|						Make this time from s to 1000ms
			//					|									|			Change to %
			//					|									|				|			dvide by gThickProf cycleTime 100ms
			//					|									|				|					|
			idxThickL = ((gThickProf.TimSet.CycleTime*10) - (uCntTimeL*tTask*1000))*10/gThickProf.TimSet.CycleTime;
			nCurrentPointL = (INT)idxThickL;/*Albert*/
			
			/*----------------------------------------------------*/
			/*-------------------Right side------------------------*/
			/*----------------------------------------------------*/
			switch (uCntStepR)
			{
				case 0:/*weit step*/					
					if(RMold.SupportAir.cntTime > 0)
					{
						if(tTask != 0)
						{
							uCntTimeR = (REAL)RMold.SupportAir.cntTime/100/tTask;
							uCntStepR = 1;
						}
					}					
					break;
				case 1:/*Start count, since tTask is 4ms so If we need s counting should chagne the counter*/
					if(uCntTimeR > 0)
					{
						uCntTimeR-=tTask;
					}
					else
					{
						RMold.SupportAir.cntTime = 0;
						uCntTimeR = 0;
						uCntStepR = 0;
					}					
					break;
			}			
			//	Make this time from 100ms to 1000ms
			//					|						Make this time from s to 1000ms
			//					|									|			Change to %
			//					|									|				|			dvide by gThickProf cycleTime 100ms
			//					|									|				|					|
			idxThickR = ((gThickProf.TimSet.CycleTime*10) - (uCntTimeR*tTask*1000))*10/gThickProf.TimSet.CycleTime;
			nCurrentPointR = (INT)idxThickR;/*Albert*/
		}
	}
	else
	{
		gMachineOut.SupportAir_1 = 0;
		gMachineOut.SupportAir_2 = 0;
		
		LMold.SupportAir.PressureSet = 0.0;
		
		LMold.SupportAir.FlowSetDieA = 0.0;
		LMold.SupportAir.FlowSetDieB = 0.0;
		LMold.SupportAir.FlowSetDieC = 0.0;
		LMold.SupportAir.FlowSetDieD = 0.0;
		
		
		RMold.SupportAir.PressureSet = 0.0;
		
		RMold.SupportAir.FlowSetDieA = 0.0;
		RMold.SupportAir.FlowSetDieB = 0.0;
		RMold.SupportAir.FlowSetDieC = 0.0;
		RMold.SupportAir.FlowSetDieD = 0.0;
		
		
		
		if(gThickProf.TimSet.CycleTime != 0)
		{
			/*----------------------------------------------------*/
			/*-------------------Left side------------------------*/
			/*----------------------------------------------------*/
			
			/*Albert Start count calculate left*/
			switch (uCntStepL)
			{
				case 0:/*weit step*/
					
					if(LMold.SupportAir.cntTime > 0)
					{
						if(tTask != 0)
						{
							uCntTimeL = LMold.SupportAir.cntTime/100/tTask;
							uCntStepL = 1;
						}
						
					}
					
					break;
				case 1:/*Start count, since tTask is 4ms so If we need s counting should chagne the counter*/
					if(uCntTimeL > 0)
					{
						uCntTimeL -= tTask;
					}
					else
					{
						LMold.SupportAir.cntTime = 0;
						uCntTimeL = 0;
						uCntStepL = 0;
					}
					
					break;
			}
			//	Make this time from 100ms to 1000ms
			//					|						Make this time from s to 1000ms
			//					|									|			Change to %
			//					|									|				|			dvide by gThickProf cycleTime 100ms
			//					|									|				|					|
			idxThickL = ((gThickProf.TimSet.CycleTime*10) - (uCntTimeL*tTask*1000))*10/gThickProf.TimSet.CycleTime;
			
//			if(LMold.SupportAir.cntTime > 0)LMold.SupportAir.cntTime--;
//			idxThickL = (gThickProf.TimSet.CycleTime - LMold.SupportAir.cntTime)*100.0/gThickProf.TimSet.CycleTime;
//			idxThickL = gThickData.CurrentTime * 100.0/gThickProf.TimSet.CycleTime;/*Albert*/
			nCurrentPointL = (INT)idxThickL;/*Albert*/
			
			if(idxThickL > LMold.TimeSet.SupportAirStart[0] && idxThickL < LMold.TimeSet.SupportAirStop[0])
			{
				if(gMachinePara.SupportAirEnable[0])
				{
					gMachineOut.SupportAir_1 = 1;
					
					LMold.SupportAir.PressureSet = gMachinePara.MoldL.SupportAir.SetPre_1;
				
					LMold.SupportAir.FlowSetDieA = gMachinePara.MoldL.SupportAir.SetFlowA1;
					LMold.SupportAir.FlowSetDieB = gMachinePara.MoldL.SupportAir.SetFlowB1;
					LMold.SupportAir.FlowSetDieC = gMachinePara.MoldL.SupportAir.SetFlowC1;
					LMold.SupportAir.FlowSetDieD = gMachinePara.MoldL.SupportAir.SetFlowD1;
				}
			}
			else if(idxThickL > LMold.TimeSet.SupportAirStart[1] && idxThickL < LMold.TimeSet.SupportAirStop[1])
			{
				if(gMachinePara.SupportAirEnable[1])
				{
					gMachineOut.SupportAir_2 = 1;
					
					LMold.SupportAir.PressureSet = gMachinePara.MoldL.SupportAir.SetPre_1;
				
					LMold.SupportAir.FlowSetDieA = gMachinePara.MoldL.SupportAir.SetFlowA2;
					LMold.SupportAir.FlowSetDieB = gMachinePara.MoldL.SupportAir.SetFlowB2;
					LMold.SupportAir.FlowSetDieC = gMachinePara.MoldL.SupportAir.SetFlowC2;
					LMold.SupportAir.FlowSetDieD = gMachinePara.MoldL.SupportAir.SetFlowD2;
				}
			}
			
			
			/*----------------------------------------------------*/
			/*-------------------Right side------------------------*/
			/*----------------------------------------------------*/
			
			/*Albert Start count calculate right*/
			switch (uCntStepR)
			{
				case 0:/*weit step*/
					
					if(RMold.SupportAir.cntTime > 0)
					{
						if(tTask != 0)
						{
							uCntTimeR = (REAL)RMold.SupportAir.cntTime/100/tTask;
							uCntStepR = 1;
						}
					}
					
					break;
				case 1:/*Start count, since tTask is 4ms so If we need s counting should chagne the counter*/
					if(uCntTimeR > 0)
					{
						uCntTimeR-=tTask;
					}
					else
					{
						RMold.SupportAir.cntTime = 0;
						uCntTimeR = 0;
						uCntStepR = 0;
					}
					
					break;
			}
			
//			if(RMold.SupportAir.cntTime > 0) /*RMold.SupportAir.cntTime-=tTask;*/	
			
			//	Make this time from 100ms to 1000ms
			//					|						Make this time from s to 1000ms
			//					|									|			Change to %
			//					|									|				|			dvide by gThickProf cycleTime 100ms
			//					|									|				|					|
			idxThickR = ((gThickProf.TimSet.CycleTime*10) - (uCntTimeR*tTask*1000))*10/gThickProf.TimSet.CycleTime;
//			idxThickR = (gThickProf.TimSet.CycleTime - RMold.SupportAir.cntTime)*100.0/gThickProf.TimSet.CycleTime;
//			idxThickR = gThickData.CurrentTime * 100.0/gThickProf.TimSet.CycleTime;/*Albert*/
			nCurrentPointR = (INT)idxThickR;/*Albert*/
			
			if(idxThickR > RMold.TimeSet.SupportAirStart[0] && idxThickR < RMold.TimeSet.SupportAirStop[0])
			{
				if(gMachinePara.SupportAirEnable[0])
				{
					gMachineOut.SupportAir_1 = 1;
					
					RMold.SupportAir.PressureSet = gMachinePara.MoldR.SupportAir.SetPre_1;
				
					RMold.SupportAir.FlowSetDieA = gMachinePara.MoldR.SupportAir.SetFlowA1;
					RMold.SupportAir.FlowSetDieB = gMachinePara.MoldR.SupportAir.SetFlowB1;
					RMold.SupportAir.FlowSetDieC = gMachinePara.MoldR.SupportAir.SetFlowC1;
					RMold.SupportAir.FlowSetDieD = gMachinePara.MoldR.SupportAir.SetFlowD1;
				}
			}
			else if(idxThickR > RMold.TimeSet.SupportAirStart[1] && idxThickR < RMold.TimeSet.SupportAirStop[1])
			{
				if(gMachinePara.SupportAirEnable[1])
				{
					gMachineOut.SupportAir_2 = 1;
					
					RMold.SupportAir.PressureSet = gMachinePara.MoldR.SupportAir.SetPre_2;
				
					RMold.SupportAir.FlowSetDieA = gMachinePara.MoldR.SupportAir.SetFlowA2;
					RMold.SupportAir.FlowSetDieB = gMachinePara.MoldR.SupportAir.SetFlowB2;
					RMold.SupportAir.FlowSetDieC = gMachinePara.MoldR.SupportAir.SetFlowC2;
					RMold.SupportAir.FlowSetDieD = gMachinePara.MoldR.SupportAir.SetFlowD2;
				}
			}
			
//			if(1 == gMachinePara.SupportAirVentEnable)
//			{
//				if((gMachinePara.SupportAirVentingStrart == idxThickL) || (gMachinePara.SupportAirVentingStrart == idxThickR))
//				{
//					RMold.SupportAir.Step = 200;	
//				}
//			}
		}
	
//		if(1 == bTestButton && gMachineInfo.Auto != 1)
//		{
//			gMachineOut.SupportAir_1 = 1;
//			
//			gMachineOut.SuckAir_1 = 1; 
//					
//			RMold.SupportAir.PressureSet = gMachinePara.MoldR.SupportAir.SetPre_1;
//				
//			RMold.SupportAir.FlowSetDieA = gMachinePara.MoldR.SupportAir.SetFlowA1;
//			RMold.SupportAir.FlowSetDieB = gMachinePara.MoldR.SupportAir.SetFlowB1;
//			RMold.SupportAir.FlowSetDieC = gMachinePara.MoldR.SupportAir.SetFlowC1;
//			RMold.SupportAir.FlowSetDieD = gMachinePara.MoldR.SupportAir.SetFlowD1;
//		}
	
//		if(0 == bTestButton && gMachineInfo.Auto != 1)
//		{
//			gMachineOut.SuckAir_1 = 0;	
//		}
	}
	//SupportAir_TimeVersion(脹風)
	gMachineOut.SupportAir_TimeVersion 		= (LMold.ValveOut.SupportAir_TimeVersion| RMold.ValveOut.SupportAir_TimeVersion);
	
	//SuckAir_TimeVersion(模頭抽真空)
	gMachineOut.SuckAir_TimeVersion 		= (LMold.ValveOut.SuckAir_TimeVersion| RMold.ValveOut.SuckAir_TimeVersion);
	gMachineOut.SuckAirRelieve_TimeVersion	= (LMold.ValveOut.SuckAirRelieve_TimeVersion| RMold.ValveOut.SupportAir_TimeVersion);
	
	
	gUserPara.LMold.TimeSet.SupportAirSecDelay = gUserPara.RMold.TimeSet.SupportAirSecDelay;
	gUserPara.LMold.TimeSet.SupportAirSecTime = gUserPara.RMold.TimeSet.SupportAirSecTime;
	
}/* end of _CYCLIC */


void SupportAir( Mold_typ * pMold, Support_Para_typ * pSupportPara, Mold_Para_User_typ * pMoldUser)
{
	/* ---------------- 模頭吹風 --------------------- */
	
	/*Albert, use nCurrentPoint get actual thick data*/
//	if(gMachineInfo.Auto == 1)
//	{
//		if(pMold == &RMold)
//		{	
//			nCurrentPointR = gThickData.FocusNum;/*Albert*/
//		}
//		
//		if(pMold == &LMold)
//		{
//			nCurrentPointL = gThickData.FocusNum;/*Albert*/
//		}
//	}
	
	if(1 == pMold->OnceSupport)
	{
		pMold->SupportAir.Out_1 = 0;
		pMold->SupportAir.Out_2 = 0;
		
		pMold->SupportAir.PressureSet = 0.0;
		
		pMold->SupportAir.FlowSetDieA = 0.0;
		pMold->SupportAir.FlowSetDieB = 0.0;
		pMold->SupportAir.FlowSetDieC = 0.0;
		pMold->SupportAir.FlowSetDieD = 0.0;
		
		if(gThickData.FocusNum > pMold->TimeSet.SupportAirStart[0] &&
	 	   gThickData.FocusNum < pMold->TimeSet.SupportAirStop[0])
	 	{
			if(gMachinePara.SupportAirEnable[0])
			{
				pMold->SupportAir.Out_1 = 1;
				
				pMold->SupportAir.PressureSet = pSupportPara->SetPre_1;
				
				pMold->SupportAir.FlowSetDieA = pSupportPara->SetFlowA1;
				pMold->SupportAir.FlowSetDieB = pSupportPara->SetFlowB1;
				pMold->SupportAir.FlowSetDieC = pSupportPara->SetFlowC1;
				pMold->SupportAir.FlowSetDieD = pSupportPara->SetFlowD1;
			}
		}
		else if(gThickData.FocusNum > pMold->TimeSet.SupportAirStart[1]&&
	 	   	  gThickData.FocusNum < pMold->TimeSet.SupportAirStop[1])
		{
			if(gMachinePara.SupportAirEnable[1])
			{
				pMold->SupportAir.Out_2 = 1;
				
				pMold->SupportAir.PressureSet = pSupportPara->SetPre_2;
				
				pMold->SupportAir.FlowSetDieA = pSupportPara->SetFlowA2;
				pMold->SupportAir.FlowSetDieB = pSupportPara->SetFlowB2;
				pMold->SupportAir.FlowSetDieC = pSupportPara->SetFlowC2;
				pMold->SupportAir.FlowSetDieD = pSupportPara->SetFlowD2;
			}
		}/* else if */
	}

	if(99 == gThickData.FocusNum)
	{
		pMold->OnceSupport = 0;
	}

	if(0 == gMachineInfo.Auto)
	{
		pMold->OnceSupport = 0;
		pMold->SupportAir.Out_1 = 0;
		pMold->SupportAir.Out_2 = 0;
	}

	/*  模頭排氣/吸氣  */
	if(pMold == &RMold)
	{											
		switch(pMold->SupportAir.Step)
		{
			case 100:
				if(1 == gMachinePara.SupportAirVentEnable)
				{
//					if(gThickData.FocusNum == gMachinePara.SupportAirVentingStrart)
//					{
						gMachineOut.SuckAir_1 = 0;
						gMachineOut.SuckAir_2 = 0;
						pMold->SupportAir.Timer.IN = 0;
						pMold->SupportAir.Step = 200;	/*   排气启动   */
//					}
				}
				else
				{
					pMold->SupportAir.Step = 3000;
				}
				break;

			case 200:
				gMachineOut.SuckAir_1 = 0;
				gMachineOut.SuckAir_2 = 0;
				pMold->SupportAir.Timer.IN = 0;

				if(gMachinePara.SupportAirVentingDelay != 0)
				{
					pMold->SupportAir.Step = 300;
				}
				else
				{
					pMold->SupportAir.Step = 500;
				}
				break;

			case 300:
				pMold->SupportAir.Timer.IN = 1;
				pMold->SupportAir.Timer.PT = gMachinePara.SupportAirVentingDelay;
				pMold->SupportAir.Step = 400;
				break;

			case 400:
				SuckAirDelayDis = pMold->SupportAir.Timer.ET;
				if(1 == pMold->SupportAir.Timer.Q)
				{
					pMold->SupportAir.Timer.IN = 0;
				//	SuckAirDelayDis = 0;
					pMold->SupportAir.Step = 500;
				}
				break;

			case 500:
				if(gMachinePara.SupportAirVentingTime != 0)
				{
					pMold->SupportAir.Step = 600;
				}
				else
				{
					pMold->SupportAir.Timer.IN = 0;
			//		pMold->SupportAir.Step = 3000;
				pMold->SupportAir.Step = 800; //20190130 ipis
				}
				break;

			case 600:
				pMold->SupportAir.Timer.IN = 1;
				pMold->SupportAir.Timer.PT = gMachinePara.SupportAirVentingTime;
				pMold->SupportAir.Step = 700;
				break;

			case 700:
				gMachineOut.SuckAir_1 = 1;
				gMachineOut.SuckAir_2 = 1;
				SuckAirTimeDis = pMold->SupportAir.Timer.ET;
				if(1 == pMold->SupportAir.Timer.Q)
				{
					gMachineOut.SuckAir_1 = 0;
					gMachineOut.SuckAir_2 = 0;
					pMold->SupportAir.Timer.IN = 0;
			//		SuckAirTimeDis = 0;
					pMold->SupportAir.Step = 800;
				}
				break;
			
			case 800:/*Start second suckair delay time*/
				
				if(pMoldUser->TimeSet.SupportAirSecDelay != 0)
				{
					pMold->SupportAir.Timer.IN = 1;
					pMold->SupportAir.Timer.PT = pMoldUser->TimeSet.SupportAirSecDelay;
					pMold->SupportAir.Step = 900;
				}
				else
				{
					pMold->SupportAir.Step = 1000;
				}
				break;
			case 900:
				gMachineOut.SuckAir_1 = 0;
				gMachineOut.SuckAir_2 = 0;
				
				pMoldUser->TimeDis.SupportAirSecDelay= pMold->SupportAir.Timer.ET;
				if(pMold->SupportAir.Timer.Q)
				{
					pMold->SupportAir.Timer.IN = 0;
					pMold->SupportAir.Step = 1000;
				}
				break;
			
			case 1000:/*Start Second Support air time*/
				
				if(pMoldUser->TimeSet.SupportAirSecTime != 0)
				{
					pMold->SupportAir.Timer.IN = 1;
					pMold->SupportAir.Timer.PT = pMoldUser->TimeSet.SupportAirSecTime;
					pMold->SupportAir.Step = 1100;
				}
				else
				{
					pMold->SupportAir.Step = 3000;
				}
				break;
			
			case 1100:
				gMachineOut.SuckAir_1 = 1;
				gMachineOut.SuckAir_2 = 1;
				
				pMoldUser->TimeDis.SupportAirSecTime = pMold->SupportAir.Timer.ET;
				
				if(pMold->SupportAir.Timer.Q)
				{
					pMold->SupportAir.Timer.IN = 0;
					pMold->SupportAir.Step = 3000;
				}
							
				break;
			
			case 3000:
				gMachineOut.SuckAir_1 = 0;
				gMachineOut.SuckAir_2 = 0;
				pMold->SupportAir.Timer.IN = 0;

//				if(1 == gMachineInfo.Auto)
//				{
//					pMold->SupportAir.Step = 100;
//				}
//				else
//				{
//					pMold->SupportAir.Step = 0;
//				}
				break;

			case 20000:
				gMachineOut.SupportAir_1 = 0;
				gMachineOut.SuckAir_1  = 0;
				gMachineOut.SuckAir_2  = 0;

				pMold->SupportAir.Timer.IN = 0;
				SuckAirDelayDis = 0;
				SuckAirTimeDis  = 0;

				pMold->SupportAir.Step = 0;
				break;

			case 30000:
				gMachineOut.SupportAir_1 = 0;
				gMachineOut.SuckAir_1  = 0;
				gMachineOut.SuckAir_2  = 0;

				pMold->SupportAir.Timer.IN = 0;
				SuckAirDelayDis = 0;
				SuckAirTimeDis  = 0;

				pMold->SupportAir.Step = 0;
				break;
		}/* switch(SuckAirStep) */
	 }
 }


void SupportAir_Time( Mold_typ * pMold,Support_Para_typ * pSupportPara,Mold_Para_User_typ * pMoldUser,Support_Fix_typ * pSupportFix)
{
	/* ---------------- 模頭漲風  時間版本  --------------------- */		
	switch(pMold->SupportAir.Step)
	{
		case 0:
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			
			pMold->SupportAir.Timer.IN = 0;
				
			break;
		
		case 100:
			if(1 == gMachinePara.SupportAirTimeVersionEnable)
			{
				pMold->ValveOut.SupportAir_TimeVersion = 0;
				pMold->SupportAir.Timer.IN = 0;
				pMold->SupportAir.Step = 200;	/*   排气启动   */
			}
			else
			{
				pMold->SupportAir.Step = 3000;
			}
			break;

		case 200: /*Start first suport air delay time*/
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			pMold->SupportAir.Timer.IN = 0;

			if(pMold->TimeSet.SupportAirDelay)
			{
				pMold->SupportAir.Step = 300;
			}
			else
			{
				pMold->SupportAir.Step = 500;
			}
			break;

		case 300:
			pMold->SupportAir.Timer.IN = 1;
			pMold->SupportAir.Timer.PT = pMold->TimeSet.SupportAirDelay;
			pMold->SupportAir.Step = 400;
			break;

		case 400:
			pMold->TimeDis.SupportAirDelay = pMold->SupportAir.Timer.ET;
			if(1 == pMold->SupportAir.Timer.Q)
			{
				pMold->SupportAir.Timer.IN = 0;
				pMold->SupportAir.Step = 500;
			}
			break;

		case 500: /*Start first suport air time*/
			if(pMold->TimeSet.SupportAirTime != 0)
			{
				pMold->SupportAir.Step = 600;
			}
			else
			{
				pMold->SupportAir.Timer.IN = 0;
				pMold->SupportAir.Step = 800;
			}
			break;

		case 600:
			pMold->SupportAir.Timer.IN = 1;
			pMold->SupportAir.Timer.PT = pMold->TimeSet.SupportAirTime;
			pMold->SupportAir.Step = 700;
			
			break;

		case 700:
			pMold->ValveOut.SupportAir_TimeVersion = 1;
			pMold->TimeDis.SupportAirTime = pMold->SupportAir.Timer.ET;
			
			if(1 == pMold->SupportAir.Timer.Q)
			{
				pMold->ValveOut.SupportAir_TimeVersion = 0;
				pMold->SupportAir.Timer.IN = 0;
				
				if( SUPPORTAIR_MODE_STANDARD == pSupportFix->eSupportAirMode)
				{
					pMold->SupportAir.Step = 800;
				}
				else if(SUPPORTAIR_MODE_AFTER_CLAMP_CLS == pSupportFix->eSupportAirMode)
				{
					pMold->SupportAir.Step = 3000;  // only one time support air
				}
			}
			break;
		
		case 800:/*Start second suport air delay time*/
			
			if(pMold->TimeSet.SupportAirSecDelay != 0)
			{
				pMold->SupportAir.Timer.IN = 1;
				pMold->SupportAir.Timer.PT = pMold->TimeSet.SupportAirSecDelay;
				pMold->SupportAir.Step = 900;
			}
			else
			{
				pMold->SupportAir.Step = 1000;
			}
			break;
		case 900:
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			pMold->TimeDis.SupportAirSecDelay = pMold->SupportAir.Timer.ET;
			if(pMold->SupportAir.Timer.Q)
			{
				pMold->SupportAir.Timer.IN = 0;
				pMold->SupportAir.Step = 1000;
			}
			break;
		
		case 1000:/*Start Second Support air time*/
			
			if(pMold->TimeSet.SupportAirSecTime != 0)
			{
				pMold->SupportAir.Timer.IN = 1;
				pMold->SupportAir.Timer.PT = pMold->TimeSet.SupportAirSecTime;
				pMold->SupportAir.Step = 1100;
			}
			else
			{
				pMold->SupportAir.Step = 3000;
			}
			break;
		
		case 1100:
			pMold->ValveOut.SupportAir_TimeVersion = 1;
			pMold->TimeDis.SupportAirSecTime = pMold->SupportAir.Timer.ET;
			
			if(pMold->SupportAir.Timer.Q)
			{
				pMold->SupportAir.Timer.IN = 0;
				pMold->SupportAir.Step = 3000;
			}
						
			break;
		
		case 3000:
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			pMold->SupportAir.Timer.IN = 0;

			//				if(1 == gMachineInfo.Auto)
			//				{
			//					pMold->SupportAir.Step = 100;
			//				}
			//				else
			//				{
			//					pMold->SupportAir.Step = 0;
			//				}
			break;

		case 20000:
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			pMold->SupportAir.Timer.IN = 0;
			pMold->SupportAir.Step = 0;
			
			break;

		case 30000:
			pMold->ValveOut.SupportAir_TimeVersion = 0;
			pMold->SupportAir.Timer.IN = 0;
			pMold->SupportAir.Step = 0;
			
			break;
	}/* switch(SupportAir) */

}

void SupportSuckAir_Time( Mold_typ * pMold, Support_Para_typ * pSupportPara, Mold_Para_User_typ * pMoldUser)
{
	/* ---------------- 模頭抽真空  時間版本  --------------------- */		
	switch(pMold->SupportSuckAir.Step)
	{
		case 0:
			pMold->ValveOut.SuckAir_TimeVersion = 0;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
			pMold->SupportSuckAir.Timer.IN = 0;
			
			break;
		
		case 100:
			if(1 == gUserPara.SupportAirVentTimeVersionEnable)
			{
				pMold->ValveOut.SuckAir_TimeVersion = 0;
				pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
				pMold->SupportSuckAir.Timer.IN = 0;
				pMold->SupportSuckAir.Step = 200;	/*   吸氣启动   */
			}
			else
			{
				pMold->SupportSuckAir.Step = 3000;
			}
			break;

		case 200:
			pMold->ValveOut.SuckAir_TimeVersion = 0;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
			pMold->SupportSuckAir.Timer.IN = 0;

			if(gMachinePara.SupportAirVentingDelay != 0)
			{
				pMold->SupportSuckAir.Step = 300;
			}
			else
			{
				pMold->SupportSuckAir.Step = 500;
			}
			break;

		case 300:
			pMold->SupportSuckAir.Timer.IN = 1;
			pMold->SupportSuckAir.Timer.PT = gMachinePara.SupportAirVentingDelay;
			pMold->SupportSuckAir.Step = 400;
			break;

		case 400:   // SupportAirVenting  Delay
			SuckAirDelayDis = pMold->SupportSuckAir.Timer.ET;
			if(1 == pMold->SupportSuckAir.Timer.Q)
			{
				pMold->SupportSuckAir.Timer.IN = 0;
//				SuckAirDelayDis = 0;
				pMold->SupportSuckAir.Step = 500;
			}
			break;

		case 500:
			if(gMachinePara.SupportAirVentingTime != 0)
			{
				pMold->SupportSuckAir.Step = 600;
			}
			else
			{
				pMold->SupportSuckAir.Timer.IN = 0;
				pMold->SupportSuckAir.Step = 3000;
			}
			break;

		case 600:  
			pMold->SupportSuckAir.Timer.IN = 1;
			pMold->SupportSuckAir.Timer.PT = gMachinePara.SupportAirVentingTime;
			pMold->SupportSuckAir.Step = 700;
			
			break;

		case 700:    //SupportAirVenting Time
			pMold->ValveOut.SuckAir_TimeVersion = 1;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 1;
			SuckAirTimeDis = pMold->SupportSuckAir.Timer.ET;
			if(1 == pMold->SupportSuckAir.Timer.Q)
			{
				pMold->ValveOut.SuckAir_TimeVersion = 0;
				pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
				pMold->SupportSuckAir.Timer.IN = 0;
//				SuckAirTimeDis = 0;
				pMold->SupportSuckAir.Step = 3000;
			}
			break;

		
		case 3000:
			pMold->ValveOut.SuckAir_TimeVersion = 0;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
			pMold->SupportSuckAir.Timer.IN = 0;

			break;

		case 20000:
			pMold->ValveOut.SuckAir_TimeVersion = 0;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
			pMold->SupportSuckAir.Timer.IN = 0;
			pMold->SupportSuckAir.Step = 0;
			
			break;

		case 30000:
			pMold->ValveOut.SuckAir_TimeVersion = 0;
			pMold->ValveOut.SuckAirRelieve_TimeVersion = 0;
			pMold->SupportSuckAir.Timer.IN = 0;
			pMold->SupportSuckAir.Step = 0;
			
			break;
	}/* switch(SupportSuckAir) */

}



