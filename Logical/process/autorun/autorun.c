/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: autorun
 * File: autorun.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program autorun --- auto task control 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "edge.h"               /*edge function                         */
#include "pic_id.h"

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void AutoRun( Mold_typ *pMold,Option_Fix_typ * pOptionFix,SPC_Mold_typ * pSPC);
void AutoInit( Mold_typ *pMold);
void AutoRunToSemiAuto();
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	t_init = 1;         	/* Disable init Step: Don't run init Step in first cycle */
	t_stop = 1;				/* Enable stop Step:  Run stop Step in first cycle */

	LMold.Timer.IN       = 0;
	LMold.TimeCycle.IN   = 0;
	LMold.AutoStep  	 = 30000;

	RMold.Timer.IN       = 0;
	RMold.TimeCycle.IN   = 0;
	RMold.AutoStep       = 30000;

	gMachineInfo.LTimeCyc = 0;
	gMachineInfo.RTimeCyc = 0;

	gMachineInfo.CycleTimes = gCycleTimes;

	AutoEnable = 0;

	TimeCounter = 0;
    
	Init = 0;
   
	gMacOption.bAutoStopPinDown = 0;

} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{

	if (0 == t_init)                    /*init Step: run only one time after resume  init auto course data  */
	{
		t_init = 1;				        /* Disable init Step */
		t_stop = 0;
		gTKauto.Status = 1;

		AutoEnable = gMachineIn.AutoEnable;
		AutoStop   = gMachineIn.AutoStop;

		FirstStart = 0;

		if(1 == gMachineFix.Option.bExtParisonCtrl)
		{
			gMachineInfo.ExtParisonCtrlStart = 0;
		}
		
		gThickIO.In.StartThick  = 0;
		gThickIO.Out.ControlEnd = 0;

		OldThickStart = 0;

		gPROCESS_info.extruderStopEnable = 0;

		AutoInit(&LMold);
		AutoInit(&RMold);

		//  	LMold.Carriage.Step = 18000;
		// 	RMold.Carriage.Step = 18000;
		
		gMachineOut.BackGateBypass = 0;
		

		//    	memset(&gMachineOut.LMold,0,sizeof(gMachineOut.LMold));
		//    	memset(&gMachineOut.RMold,0,sizeof(gMachineOut.RMold));

		gMachineOut.CutterFw    = 0;
		gMachineOut.CutterBw    = 0;
		gMachineOut.SupportAir_1  = 0;
		gMachineOut.SupportAir_2  = 0;
		gMachineOut.SupportAir_TimeVersion = 0;
		gMachineOut.ExtrLiftUp    = 0;
		gMachineOut.ExtrLiftUpCheck   = 0;
		gMachineOut.ExtrLiftUpFast    = 0;

		memset(&LMold.Alarm,0,sizeof(LMold.Alarm));
		memset(&RMold.Alarm,0,sizeof(RMold.Alarm));

	}/* if(t_init == 0 ) */

	/*auto solution  */
	gTKauto.Active++;                                /*  task auto status number */
	/*  ----------------------------------------------------------------- */
	if((0 == LMold.AutoStep) && (0 == RMold.AutoStep))
	{
		if(EDGEPOS(gMachineIn.AutoEnable,AutoEnable))
		{
			gMachineInfo.LedAutoStop = 0; 
			//				gMachineOut.AutoRun = 1;
			gThickIO.Out.ControlEnd = 0; 
			if(	1 == gMacOption.RBlowPin && 1 == gMacOption.LBlowPin 
				&& (gMacOption.RBlowPin2 || 0 == gMachineFix.Option.HWEnable.RMold.BlowPin2) 
				&& (gMacOption.LBlowPin2 || 0 == gMachineFix.Option.HWEnable.LMold.BlowPin2) 
				)
			{
				/* double mold    */
				if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
				{
					if((1 == LMold.InBasicPos) && (1 == RMold.InBasicPos) && (0==gMachineInfo.MachineReset))
					{
						if(1 == LMold.Carriage.BwPos && 1 == RMold.Carriage.BwPos)
						{
							if(1 == gMachineFix.Option.bExtParisonCtrl)   /* 外部壁厚控制 */
							{
								RMold.AutorunFirstCycle = 1;  
								LMold.AutorunFirstCycle = 0;  
								
								RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
								LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
							}
							else
							{
								RMold.AutorunFirstCycle = 1;  //Owen 20200923
								LMold.AutorunFirstCycle = 1;  //Owen 20200923
							}
							gMachineOut.AutoRun = 1;
							//					RMold.AutoStep = 900;					/* wait step */
							RMold.AutoStep = 300;					
	
							RMold.TimeCycle.IN = 1;
							RMold.TimeCycle.PT = 10000;
	
		   
							LMold.AutoStep = 900;			/* wait step */
						}
					}
					else
					{
						/*------------------------------------------------------------------------------------------------*/
						//					gImply.LCarriageBw      = !LMold.Carriage.BwPos;
						//					gImply.LClampOpn	      = !LMold.Clamp.OpnPos;
						//					gImply.LPinUp           = !LMold.BlowPin.UpPos;
						//					gImply.LTopDeflashBw    = !LMold.TransDIn.TopDeflashBwLimit;
						//					gImply.LBottomDeflashBw = !LMold.TransDIn.BottomDeflashBwLimit;
						//					gImply.LCoolPinUp       = !LMold.TransDIn.CoolPinUpLimit;
						//					gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
						//					gImply.LClampCalErr     = !LMold.Clamp.CalOK;
						//					gImply.LBlowPinCalErr   = !LMold.BlowPin.CalOK;
						//					gImply.L_RobotTransferCalErr = !LMold.Robot.Transfer.CalOK;
						//					gImply.L_RobotUpDnCalErr  = !LMold.Robot.UpDn.CalOK;
						//					gImply.LInnerPinBw 	   = !LMold.TransDIn.InnerPinBwLimit;
						//					//	gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimit;
						//					gImply.LRobotBw    	   = !LMold.Robot.Transfer.BwPos;
						//					gImply.LRobotUp    	   = !LMold.Robot.UpDn.UpPos;
						//					if (1 == LMold.Option.SubMold )
						//					{
						//						gImply.LSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;
						//					}
						//					if(1 == LMold.Option.BottomSubMold)
						//					{
						//						gImply.LBottomSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;			
						//					}
						//					gImply.LCutNeckBw 	   = !LMold.TransDIn.CutNeckOpnLimit;
						//					gImply.LSealerOpn       = !LMold.TransDIn.SealOpnLimit;
						//					gImply.LPinPartFw       = !LMold.TransDIn.PinPartFwLimit;  
						//							
						//					gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimitR | !LMold.TransDIn.RobotOpnLimitL;
						//					gImply.LTopDeflashOpn   = !LMold.TransDIn.TopDeflashOpnLimit;
						//					
						//							
						//					gImply.RCarriageBw      = !RMold.Carriage.BwPos;
						//					gImply.RClampOpn	      = !RMold.Clamp.OpnPos;
						//					gImply.RPinUp           = !RMold.BlowPin.UpPos;
						//					gImply.RTopDeflashBw    = !RMold.TransDIn.TopDeflashBwLimit;
						//					gImply.RBottomDeflashBw = !RMold.TransDIn.BottomDeflashBwLimit;
						//					gImply.RCoolPinUp       = !RMold.TransDIn.CoolPinUpLimit;
						//					gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
						//					gImply.RClampCalErr     = !RMold.Clamp.CalOK;
						//					gImply.RBlowPinCalErr  = !LMold.BlowPin.CalOK;
						//					gImply.R_RobotTransferCalErr = !RMold.Robot.Transfer.CalOK;
						//					gImply.R_RobotUpDnCalErr  = !RMold.Robot.UpDn.CalOK;
						//					gImply.RInnerPinBw 	   = !RMold.TransDIn.InnerPinBwLimit;
						//					//	gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimit;
						//					gImply.RRobotBw    	   = !RMold.Robot.Transfer.BwPos;
						//					gImply.RRobotUp    	   = !RMold.Robot.UpDn.UpPos;
						//							
						//					if (1 == RMold.Option.SubMold)
						//					{
						//						gImply.RSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;
						//					}
						//					if(1 == RMold.Option.BottomSubMold)
						//					{
						//						gImply.RBottomSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;			
						//					}
						//							
						//					gImply.RCutNeckBw 	   = !RMold.TransDIn.CutNeckOpnLimit;
						//					gImply.RSealerOpn       = !RMold.TransDIn.SealOpnLimit;
						//					gImply.RPinPartFw       = !RMold.TransDIn.PinPartFwLimit;
						//                     
						//					gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimitR | !RMold.TransDIn.RobotOpnLimitL;
						//					gImply.RTopDeflashOpn=		!RMold.TransDIn.TopDeflashOpnLimit;
																				
						/*---------------------------------------------------------------------------------------------*/
						//						gMachineIn.Reset = !gMachineIn.Reset;/*Albert*/
						gMachineIn.Reset = 1;/*Albert*/
						gMachineOut.AutoRun=0; //ipis0221
					}
				}/* if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable) */
	
				/*  right mold  */
	
				if((1 == RMold.Option.Enable) && (0 == LMold.Option.Enable)&& (0==gMachineInfo.MachineReset))
				{			
					if(1 == RMold.InBasicPos)
					{
						RMold.AutorunFirstCycle = 1;  //Owen 20200923
						LMold.AutorunFirstCycle = 0;  //Owen 20200923
						
						RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
//						LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
						
						gMachineOut.AutoRun = 1;
						
						RMold.AutoStep = 300;
	
						RMold.TimeCycle.IN = 1;
						RMold.TimeCycle.PT = 10000;
	
						LMold.AutoStep = 10000;					/* stop and keep step */
					}/*if(1 == RMold.InBasicPos)*/
					else
					{
						/*-------------------------------------------------------------------------------------*/
						//					gImply.RCarriageBw      = !RMold.Carriage.BwPos;
						//					gImply.RClampOpn	      = !RMold.Clamp.OpnPos;
						//					gImply.RPinUp           = !RMold.BlowPin.UpPos;
						//					gImply.RTopDeflashBw    = !RMold.TransDIn.TopDeflashBwLimit;
						//					gImply.RBottomDeflashBw = !RMold.TransDIn.BottomDeflashBwLimit;
						//					gImply.RCoolPinUp       = !RMold.TransDIn.CoolPinUpLimit;
						//					gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
						//					gImply.RClampCalErr     = !RMold.Clamp.CalOK;
						//					gImply.RBlowPinCalErr  = !LMold.BlowPin.CalOK;
						//					gImply.R_RobotTransferCalErr = !RMold.Robot.Transfer.CalOK;
						//					gImply.R_RobotUpDnCalErr  = !RMold.Robot.UpDn.CalOK;
						//					gImply.RInnerPinBw 	   = !RMold.TransDIn.InnerPinBwLimit;
						//					//	gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimit;
						//					gImply.RRobotBw    	   = !RMold.Robot.Transfer.BwPos;
						//					gImply.RRobotUp    	   = !RMold.Robot.UpDn.UpPos;
						//							
						//					if (1 == RMold.Option.SubMold)
						//					{
						//						gImply.RSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;
						//					}
						//					if(1 == RMold.Option.BottomSubMold)
						//					{
						//						gImply.RBottomSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;			
						//					}
						//							
						//					gImply.RCutNeckBw 	   = !RMold.TransDIn.CutNeckOpnLimit;
						//					gImply.RSealerOpn       = !RMold.TransDIn.SealOpnLimit;
						//					gImply.RPinPartFw       = !RMold.TransDIn.PinPartFwLimit;
						//                     
						//					gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimitR | !RMold.TransDIn.RobotOpnLimitL;
						//					gImply.RTopDeflashOpn=		!RMold.TransDIn.TopDeflashOpnLimit;
						
						
						
						/*--------------------------------------------------------------------------------------*/
						gMachineIn.Reset = 1;/*Albert*/
						gMachineOut.AutoRun=0; //ipis0221
					}
				}/* if((1 == RMold.Option.Enable) && (0 == LMold.Option.Enable)) */
	
				/*  left mold   */
				if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable)&& (0==gMachineInfo.MachineReset))
				{
					if(1 == LMold.InBasicPos)
					{
						RMold.AutorunFirstCycle = 0;  //Owen 20200923
						LMold.AutorunFirstCycle = 1;  //Owen 20200923	
						
//						RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
						LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass 外部模內貼完成訊號
						
						gMachineOut.AutoRun = 1;
							
						RMold.AutoStep = 10000;					/* stop and keep step */
						LMold.AutoStep = 300;
	
						LMold.TimeCycle.IN = 1;
						LMold.TimeCycle.PT = 10000;
					}/*if(1 == LMold.InBasicPos)*/
					else
					{
						/*--------------------------------------------------------------------------*/
						//					gImply.LCarriageBw      = !LMold.Carriage.BwPos;
						//					gImply.LClampOpn	      = !LMold.Clamp.OpnPos;
						//					gImply.LPinUp           = !LMold.BlowPin.UpPos;
						//					gImply.LTopDeflashBw    = !LMold.TransDIn.TopDeflashBwLimit;
						//					gImply.LBottomDeflashBw = !LMold.TransDIn.BottomDeflashBwLimit;
						//					gImply.LCoolPinUp       = !LMold.TransDIn.CoolPinUpLimit;
						//					gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
						//					gImply.LClampCalErr     = !LMold.Clamp.CalOK;
						//					gImply.LBlowPinCalErr  = !LMold.BlowPin.CalOK;
						//					gImply.L_RobotTransferCalErr = !LMold.Robot.Transfer.CalOK;
						//					gImply.L_RobotUpDnCalErr  = !LMold.Robot.UpDn.CalOK;
						//					gImply.LInnerPinBw 	   = !LMold.TransDIn.InnerPinBwLimit;
						//					//	gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimit;
						//					gImply.LRobotBw    	   = !LMold.Robot.Transfer.BwPos;
						//					gImply.LRobotUp    	   = !LMold.Robot.UpDn.UpPos;
						//					if (1 == LMold.Option.SubMold)
						//					{
						//						gImply.LSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;
						//					}
						//					if(1 == LMold.Option.BottomSubMold)
						//					{
						//						gImply.LBottomSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;			
						//					}
						//					gImply.LCutNeckBw 	   = !LMold.TransDIn.CutNeckOpnLimit;
						//					gImply.LSealerOpn       = !LMold.TransDIn.SealOpnLimit;
						//					gImply.LPinPartFw       = !LMold.TransDIn.PinPartFwLimit;  
						//							
						//					gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimitR | !LMold.TransDIn.RobotOpnLimitL;
						//					gImply.LTopDeflashOpn   = !LMold.TransDIn.TopDeflashOpnLimit;
						/*--------------------------------------------------------------------------*/	
						gMachineIn.Reset = 1;/*Albert*/
						gMachineOut.AutoRun=0; //ipis0221
					}
				}/* if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable))*/
			}
			else
			{
				gMachineInfo.ManualModeReguest = 1;
				
				//double blowpin
				gImply.RBlowPinEnable     = !gMacOption.RBlowPin;
				gImply.RBlowPin2Enable    = !gMacOption.RBlowPin2;
				gImply.LBlowPinEnable     = !gMacOption.LBlowPin;
				gImply.LBlowPin2Enable    = !gMacOption.LBlowPin2;
			
			}
		}/*  if(EDGEPOS(gMachineIn.AutoEnable,AutoEnable)) */
	}
	else
	{
		gMachineIn.Reset = 0;/*Albert*/
		if(EDGEPOS(gMachineIn.AutoStop,AutoStop))
		{
			gMachineInfo.LedAutoStop = 2;
			gImply.AutoRunStop = 1;
			if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
			{
				if(RMold.AutoStep > 0 && RMold.AutoStep < 10000)
				{
					RMold.SemiAuto = 1;
				}
				if(LMold.AutoStep > 0 && LMold.AutoStep < 10000)
				{
					LMold.SemiAuto = 1;
				}
			}

			if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable)
			{
				if(RMold.AutoStep > 0 && RMold.AutoStep < 10000)
				{
					RMold.SemiAuto = 1;
				}
				LMold.AutoStep = 11000;			/*   stop keep   */
			}

			if(0 == RMold.Option.Enable && 1 == LMold.Option.Enable)
			{
				if(LMold.AutoStep > 0 && LMold.AutoStep < 10000)
				{
					LMold.SemiAuto = 1;
				}
				RMold.AutoStep = 11000;			/*   stop keep   */
			}
		}/* if(EDGEPOS(gMachineIn.AutoStop,AutoStop)) */
	}/* if((0 == LMold.AutoStep) && (0 == RMold.AutoStep)) */

	AutoEnable = gMachineIn.AutoEnable;
	AutoStop   = gMachineIn.AutoStop;

	/* -------------------   cycle exchange  -------------------- */
	if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
	{
		if(1 == RMold.CurCycEnd)
		{
			LMold.NextCycStart = 1;
			RMold.CurCycEnd    = 0;
		}

		if(1 == LMold.CurCycEnd)
		{
			RMold.NextCycStart = 1;
			LMold.CurCycEnd    = 0;
		}

		if(1 == RMold.LengthCtrl.Start)
		{
			RMold.LengthCtrl.Start = 0;
			LMold.LengthCtrl.Step = 100;
		}

		if(1 == LMold.LengthCtrl.Start)
		{
			LMold.LengthCtrl.Start = 0;
			RMold.LengthCtrl.Step = 100;
		}
	}/* if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable) */

	if((1 == RMold.Option.Enable) && (0 == LMold.Option.Enable))
	{
		RMold.NextCycStart = 1;
		RMold.CurCycEnd    = 0;

		if(1 == RMold.LengthCtrl.Start)
		{
			RMold.LengthCtrl.Start = 0;
			RMold.LengthCtrl.Step = 100;
		}
	}

	if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable))
	{
		LMold.NextCycStart = 1;
		LMold.CurCycEnd    = 0;

		if(1 == LMold.LengthCtrl.Start)
		{
			LMold.LengthCtrl.Start = 0;
			LMold.LengthCtrl.Step = 100;
		}
	}
	/* --------------------  工位自动过程 ----------------------- */

	AutoRun(&RMold,&gMachineFix.Option,&gSPC_Current.MoldR);
	AutoRun(&LMold,&gMachineFix.Option,&gSPC_Current.MoldL);
	
	/* -------------- 抬頭異常 自動轉半自動 ---------------- */
//	if(1 == gAlarm.RAlarm.ExtrLiftNotAtUpPos || 1 == gAlarm.LAlarm.ExtrLiftNotAtUpPos || 1 == gAlarm.RAlarm.ExtrLiftNotAtDnPos || 1 == gAlarm.LAlarm.ExtrLiftNotAtDnPos 
//		&& 1 == gMachineInfo.ExtrliftAlarmReqSemiAutoFlag)
//	{
//		gMachineInfo.ExtrliftAlarmReqSemiAutoFlag = 0 ;
//		AutoRunToSemiAuto();
//	}
		
	/* -------------- 機台異常 自動轉半自動 ---------------- */
	if(1 == gAlarm.CutterHeatNormal || 1 == gAlarm.SwingInvertErr || 1 == gAlarm.OilTempHigh || 1 == gAlarm.OilPosition 
		|| 1 == gAlarm.AccuChargeError || 1 == gAlarm.AccuCharge_ExtrliftError || 
		((0 == gMacOption.bDryCycle) && (1 == bDryCycleOld)))
	{
		gPROCESS_info.extruderStopEnable = 1;
		AutoRunToSemiAuto();
	}
		
	bDryCycleOld = gMacOption.bDryCycle;
		
	/* -------------- 風壓異常 自動轉半自動 ---------------- */
	if(1 == gAlarm.AirPressureLow && 1 == gMachineInfo.AirPressureLowDlyFlag )
	{
		gMachineInfo.AirPressureLowDlyFlag  = 0;
		AutoRunToSemiAuto();
	}
	

	
	/* ---------------- 油泵运行检测无自动停止 ---------------- */
	if(0 == gMachineOut.OilPump && gTKauto.Active != 1)
	{
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
		{
			gAlarm.OilPumpOff= 1;
			if(LMold.AutoStep > 0 && LMold.AutoStep  < 40000)LMold.AutoStep  = 40000;
			if(RMold.AutoStep > 0 && RMold.AutoStep  < 40000)RMold.AutoStep  = 40000;
		}
		else if (1 == RMold.Option.Enable )
		{
			gAlarm.OilPumpOff = 1;
			if(RMold.AutoStep > 0 && RMold.AutoStep  < 40000)RMold.AutoStep   = 40000;
		}
		else
		{
			gAlarm.OilPumpOff = 1;
			if(LMold.AutoStep > 0 && LMold.AutoStep  < 40000)LMold.AutoStep   = 40000;
		}
	}

	if(1 == gMachineOut.OilPump && 1 == gAlarm.OilPumpErr && gTKauto.Active > 500 ) /*  自动运行5s检测  */
	{
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
		{
			gAlarm.OilPumpErr = 1;
			if(LMold.AutoStep > 0 && LMold.AutoStep  < 40000)LMold.AutoStep  = 40000;
			if(RMold.AutoStep > 0 && RMold.AutoStep  < 40000)RMold.AutoStep  = 40000;
		}
		else if (1 == RMold.Option.Enable )
		{
			if(RMold.AutoStep > 0 && RMold.AutoStep  < 40000)RMold.AutoStep  = 40000;
		}
		else
		{
			if(LMold.AutoStep > 0 && LMold.AutoStep  < 40000)LMold.AutoStep  = 40000;
		}
	}

	/* ---------------------  周期及超时处理  -------------------------*/

	gMachineInfo.LTimeCyc = LMold.TimeCycle.ET/10;
	gMachineInfo.RTimeCyc = RMold.TimeCycle.ET/10;

	if(0 == gMachineInfo.MachineReset)
	{
		if(LMold.TimeCycle.ET >= LMold.TimeSet.CycleAlarmTime)
		{
			if(LMold.AutoStep > 0 && LMold.AutoStep < 40000)
			{	
				if(LMold.AutoStep > 0 && LMold.AutoStep < 15000)
				{
					gAlarm.LCycleTime = 1;
				}	
				LMold.AutoStep  = 40000;
			}
		}
	
		if(RMold.TimeCycle.ET >= RMold.TimeSet.CycleAlarmTime)
		{
			if(RMold.AutoStep > 0 && RMold.AutoStep < 40000)
			{	
				if(RMold.AutoStep > 0 && RMold.AutoStep < 15000)
				{
					gAlarm.RCycleTime = 1;
				}	
				RMold.AutoStep  = 40000;
			}
			
		}
		
	}


	if(0 == gMachineIn.RMold.SafeGate || 0 == gMachineIn.LMold.SafeGate )
	{
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
		{
			gAlarm.SaftyDoor = ! gMachineIn.RMold.SafeGate;
			if(LMold.AutoStep > 0 && LMold.AutoStep < 40000)LMold.AutoStep = 40000;
			RMold.AutoStep  = 40000;
		}
		else if (1 == RMold.Option.Enable )
		{
			gAlarm.SaftyDoor = ! gMachineIn.RMold.SafeGate;
			if(RMold.AutoStep > 0 && RMold.AutoStep < 40000)RMold.AutoStep = 40000;
		}
		else
		{
			gAlarm.SaftyDoor = ! gMachineIn.LMold.SafeGate;
			if(LMold.AutoStep > 0 && LMold.AutoStep < 40000)LMold.AutoStep = 40000;
		}
	}/* if(0 == gMachineIn.SafeGate) */
	
	/* ---------------------  清除gExSPC內容  -------------------------*/
	if(1 == gExSPC_Clear )
	{
		gExSPC_Clear = 0;
		ExIndex = 0;
		memset(&gExSPC,0,sizeof(gExSPC));
	}
		
	
	gMachineInfo.CycleTimes  = gCycleTimes;
	gMachineInfo.CycleTimesL = gCycleTimesL;
	gMachineInfo.CycleTimesR = gCycleTimesR;
	/*  ----------------------------------------------------------------- */

	if (1 == gTKauto.Stop)   /* enable stop Step */
	{
		t_stop      = 1;
		gTKauto.Stop = 0;
	}
	/* auto solution end */
	if(1 == t_stop)
	{
		gImply.AutoRunStop = 0;
		
		gMachineInfo.LedAutoStop = 0;
		gMachineInfo.LedAutoRun = 0;
		gMachineOut.AutoRun = 0;

		gMachineInfo.LTimeCyc = 0;
		gMachineInfo.RTimeCyc = 0;

		if(1 == gMachineFix.Option.bExtParisonCtrl)
		{
			gMachineInfo.ExtParisonCtrlStart = 0;
		}
		gThickIO.In.StartThick  = 0;
		gThickIO.Out.ControlEnd = 0;

		AutoInit(&LMold);
		AutoInit(&RMold);

		//    	memset(&gMachineOut.LMold,0,sizeof(gMachineOut.LMold));
		//    	memset(&gMachineOut.RMold,0,sizeof(gMachineOut.RMold));

		LMold.Carriage.v_set = 0;
		RMold.Carriage.v_set = 0;

		LMold.Clamp.v_set = 0;
		RMold.Clamp.v_set = 0;
      
		LMold.BlowPin.v_set = 0;
		RMold.BlowPin.v_set = 0;

		gMachineOut.SupportAir_1 = 0;
		gMachineOut.SupportAir_2 = 0;
		gMachineOut.SupportAir_TimeVersion = 0;
		LMold.SupportAir.Out_1  = 0;
		LMold.SupportAir.Out_2  = 0;
		RMold.SupportAir.Out_1  = 0;
		RMold.SupportAir.Out_2  = 0;
		LMold.OnceSupport     = 0;
		RMold.OnceSupport     = 0;

		LMold.CutNeck.Timer.IN = 0;
		RMold.CutNeck.Timer.IN = 0;

		t_init        = 0;                 /* next resume ,enable init Step */
		gTKauto.Status = 0;
		ST_tmp_suspend(0);

	} /* if (t_stop==1) */

} /* end of _CYCLIC */
void AutoInit( Mold_typ *pMold)
{
	pMold->NextCycStart = 0;
	pMold->CurCycEnd	= 0;

	pMold->TimeCycle.IN = 0;
	pMold->Timer.IN     = 0;

	pMold->AutoStep			= 0;
	pMold->ResetStep	    = 0;
	pMold->LowBlow.Step		= 0;
	pMold->HighBlow.Step	= 0;
	pMold->BlowPin.Step  	= 0;
	pMold->BlowPin2.Step  	= 0;
	pMold->BottomPin.Step	= 0;
	pMold->Carriage.Step 	= 0;
	pMold->Clamp.Step	   	= 0;
	//	pMold->CoolPin.Step		= 0;
	pMold->CoolPinBlow.Step	= 0;
	//	pMold->CoolPin2.Step	= 0;
	pMold->CoolPin2Blow.Step= 0;
	//	pMold->CoolPin3.Step	= 0;
	pMold->CoolPin3Blow.Step= 0;
	pMold->BlowDeflash.Step	= 0;
	pMold->Cutter.Step		= 0;
	pMold->TopDeflash.Step	= 0;
	pMold->BottomDeflash.Step = 0;
	pMold->SupportAir.Step	= 0;
	pMold->ExtrLift.Step	= 0;
	pMold->Lock.Step		= 0;
	pMold->PullBottom.Step	= 0;
	pMold->Robot.Step	    = 0;
	pMold->RobotOpnCls.Step	   = 0;
	pMold->ChuteDeflashBlow.Step	  = 0;
	pMold->SubMold.Step		= 0;
	pMold->PinPart.Step     = 0;
	pMold->Screw.Step       = 0;
	pMold->LengthCtrl.Step  = 0;
	pMold->HookPin.Step  = 0;

	pMold->SemiAuto = 0;
	
	pMold->OnceSubMoldIn = 0;
	pMold->OncePin2ndDn  = 0;

	memset(&pMold->ValveOut.ClampCls,0,sizeof(pMold->ValveOut)-12);
	
	pMold->ValveOut.RobotCarriageUp = 0;// Reset 外部機械手訊號
	pMold->ValveOut.RobotCarriageDn = 0;// Reset 外部機械手訊號
	pMold->ValveOut.RobotClampOpn = 0;// Reset 外部機械手訊號
	pMold->ValveOut.RobotClampCls = 0;// Reset 外部機械手訊號

}/* void AutoInit( Mold_typ *pMold) */




void AutoRunToSemiAuto()
{
	if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
	{
		if(RMold.AutoStep > 0 && RMold.AutoStep < 10000)
		{
			RMold.SemiAuto = 1;
		}
		if(LMold.AutoStep > 0 && LMold.AutoStep < 10000)
		{
			LMold.SemiAuto = 1;
		}
	}
		
	if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable)
	{
		if(RMold.AutoStep > 0 && RMold.AutoStep < 10000)
		{
			RMold.SemiAuto = 1;
		}
		if(LMold.AutoStep > 0 && LMold.AutoStep <= 10000)
		{
			LMold.AutoStep = 11000;			/*   stop keep   */
		}
		
	}
		
	if(0 == RMold.Option.Enable && 1 == LMold.Option.Enable)
	{
		if(LMold.AutoStep > 0 && LMold.AutoStep < 10000)
		{
			LMold.SemiAuto = 1;
		}
		
		if(RMold.AutoStep > 0 && RMold.AutoStep <= 10000)
		{
			RMold.AutoStep = 11000;			/*   stop keep   */
		}
	}
	
	
}
/* ----------------------------------- Auto run course ---------------------------  */
void AutoRun( Mold_typ *pMold,Option_Fix_typ * pOptionFix,SPC_Mold_typ * pSPC)
{
	switch(pMold->AutoStep)
	{		
		/* ----- first station auto start action ------ */
		case 100:
			if(0 == gMacOption.Cut1stCycle)
			{
				pMold->Cutter.Step = 100;		   /*   切刀启动   */
				pMold->AutoStep = 200;
			}
			else
			{
				if(1 == gMachineFix.Option.bExtParisonCtrl)
				{
					gMachineInfo.ExtParisonCtrlStart = 1;
				}
				else
				{
					gThickIO.In.StartThick = 1;      /*   壁厚启动   */
				}
				pMold->LengthCtrl.Step = 100; 	/* 	长度控制  */
				pMold->AutoStep = 300;
			}
			break;

		case 200:
			if(3000 == pMold->Cutter.Step)		/*   切刀完成   */
			{
				pMold->Cutter.Step =0;
				pMold->AutoStep = 300;
			}
			break;

		case 300:
			gThickIO.Out.ControlEnd = 1;
			pMold->AutoStep = 3050;          	
			break;
      
		/* ----------- second station start --------------- */	
		case 900:
			/*pMold->Carriage.Step = 18000;*/	/*  保持另一模回位  */
			if(pMold == &LMold)
			{
				if(0 == RMold.BlowPin.Step && 0 == RMold.BlowPin2.Step )		/*  吹针下结束  */
				{
					if(1 == pMold->NextCycStart)
					{	
						gMachineOut.LeftStation = 1;
						pMold->NextCycStart = 0;
						pMold->AutoStep = 3000;    /*  去料长控制选择分支  */
					}
					else
					{
						if(1 == pMold->SemiAuto)	/*  自动停止 */
						{
							pMold->AutoStep = 0;
						}
					}
				}
			}
		
			if(pMold == &RMold)
			{
				if(0 == LMold.BlowPin.Step && 0 == LMold.BlowPin2.Step)/*  吹针下结束  */
				{
					if(1 == pMold->NextCycStart)
					{
						gMachineOut.LeftStation = 0;
						pMold->NextCycStart = 0;
						pMold->AutoStep = 3000;    /*  去料长控制选择分支  */
					}
					else
					{
						if(1 == pMold->SemiAuto)	/*  自动停止 */
						{
							pMold->AutoStep = 0;
						}
					}
				}
			}	
			break;
      
		/* -------------------------------------------------- */
		/*                     Cycle start                    */
		/* -------------------------------------------------- */	
		case 1000:
		
			/*pMold->Carriage.Step = 18000;*/		/*  保持另一模回位  */											
			if(1 == pMold->NextCycStart)		/*  等待状态  */	
			{
				if (pMold == &RMold)
				{
					gMachineOut.LeftStation = 0;
				}
				if (pMold == &LMold)
				{
					gMachineOut.LeftStation = 1;
				}
				pMold->NextCycStart = 0;
				pMold->AutoStep = 1200;

			}
			else
			{
				if(1 == pMold->SemiAuto)
				{
					if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
					{
						if(pMold == &LMold)
						{
							if(0 == RMold.AutoStep)
							{
								pMold->AutoStep = 1200;
							}
						}
						
						if(pMold == &RMold)
						{
							if(0 == LMold.AutoStep)
							{
								pMold->AutoStep = 1200;
							}
						}
					}/* if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable) */
				}/* if(1 == pMold->SemiAuto) */
			}/* if(1 == pMold->NextCycStart) */
			break;
      
		case 1200:
			
			//			if(pMold->Option.BlowPin)
			//			{
			//				pMold->BlowPin.Step = 15300;		/* 吹针微抽  	*/
			//			}
			//			
			//			if(pMold->Option.BlowPin2)
			//			{
			//				pMold->BlowPin2.Step = 15300;		/* 吹针微抽  	*/
			//			}
			
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType )	/*  電動關模加壓超时 */
			{
				if( 3900 == pMold->Clamp.Step)
				{
					pMold->Clamp.Step = 0;
				}
			}
			pMold->BlowPin.Step = 15300;		/* 吹针微抽  	*/
			pMold->BlowPin2.Step = 15300;		/* 吹针微抽  	*/
			
			pMold->AutoStep = 1300;
			break;

		case 1300:			
			
			//			if(18000 == pMold->BlowPin.Step)	/* 吹针微抽结束  */
			//			{
			//				pMold->BlowPin.Step = 0;
			//				if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//				{
			//					pMold->ValveOut.BlowPinDnHold =1;
			//				}
			//				pMold->AutoStep = 1400;
			//				pMold->PinPart.Step = 10100;		/*  吹针块退   	*/  
			//			}
			
			//			if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
			//			{
			//				if(18000 == pMold->BlowPin.Step)	/* 吹针微抽结束  */
			//				{
			//					pMold->BlowPin.Step = 0;
			//					if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//					{
			//						pMold->ValveOut.BlowPinDnHold =1;
			//					}
			//					pMold->AutoStep = 1400;
			//					pMold->PinPart.Step = 10100;		/*  吹针块退   	*/  
			//				}
			//
			//			}
			//			else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			//				if(18000 == pMold->BlowPin2.Step)	/* 吹针微抽结束  */
			//				{
			//					pMold->BlowPin2.Step = 0;
			//					if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//					{
			//						pMold->ValveOut.BlowPinDnHold =1;
			//					}
			//					pMold->AutoStep = 1400;
			//					pMold->PinPart.Step = 10100;		/*  吹针块退   	*/  
			//				}
			//
			//			}
			//			else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			if(18000 == pMold->BlowPin.Step && 18000 == pMold->BlowPin2.Step)	/* 吹针微抽结束  */
			{
				pMold->BlowPin.Step = 0;
				pMold->BlowPin2.Step = 0;
				if(1 == pMold->Option.BottomSubMold)   // ipis1112
				{
					pMold->ValveOut.BlowPinDnHold =1;
				}
				pMold->AutoStep = 1400;
				pMold->PinPart.Step = 10100;		/*  吹针块退   	*/  
			}

			//			}
			
			
			break;

		case 1400:
			if(13000 == pMold->PinPart.Step)	/*  吹针块退结束 */
			{
				pMold->PinPart.Step = 18000;
				pMold->AutoStep = 1900;
			}

			if(40000 == pMold->PinPart.Step )	/*  吹针块退超时 */
			{
				pMold->PinPart.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		/*---------------no use--------------------------------------------------------------*/    
		/*  绞牙微开模 -- 绞牙 -- 子模微开 -- */
		case 1500:
			pMold->Clamp.Step = 11100;		  	/*  绞牙微开模 */
			pMold->AutoStep = 1600;
			break;

		case 1600:
			if(13000 == pMold->Clamp.Step)	/*  绞牙微开模结束  */
			{
				pMold->Clamp.Step = 0;
				pMold->AutoStep   = 1700;
			}

			if(40000 == pMold->Clamp.Step)  	/*  绞牙微开模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 1700:	
			pMold->Screw.Step = 10100;			/* 绞牙退    	*/
			pMold->AutoStep   = 1800;
			break;

		case 1800:
			if(13000 == pMold->Screw.Step)	/*  绞牙退结束  */
			{
				if(pMold == &RMold)
				{
					RMold.Option.Screw = gMacOption.RScrew;
				}
				
				if(pMold == &LMold)
				{
					LMold.Option.Screw = gMacOption.LScrew;					
				}

				pMold->Screw.Step = 0;
				pMold->AutoStep   = 1900;
			}

			if(40000 == pMold->Screw.Step)  	/*  绞牙退超时   */
			{
				pMold->Screw.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		/*-----------------------------------------------------------------------------*/ 
		
		case 1900:
			if(3000 == pMold->Robot.Step && 3000 == pMold->RobotOpnCls.Step ) /*  機械手進+關結束  */
			{
				if(ACTUATOR_PNEU  == gMachineFix.MoldR.RobotTransfer.eActuatorType)
				{
					//Hold step
				}
				else
				{
					pMold->Robot.Step = 0;
				}
			
				if(ACTUATOR_PNEU  == gMachineFix.MoldR.RobotOpnCls.eActuatorType)
				{
					//Hold step
				}
				else
				{
					pMold->RobotOpnCls.Step = 0;
				}
			
				pMold->AutoStep = 2000; 
				
			}
			else
			{
				if(3000 != pMold->Robot.Step)   // Robot Fw too slow
				{
					pMold->Alarm.RobotNotAtFwPos = !pMold->Robot.Transfer.FwPos;
					pMold->Alarm.RobotFwTimeout  = 1;
				}
				else if (3000 == pMold->Robot.Step && 3000 != pMold->RobotOpnCls.Step) // Robot Cls too slow
				{
					pMold->Alarm.RobotNotAtClsPos = 1;
					pMold->Alarm.RobotClsTimeout  = 1;
				}
			
			
				pMold->Robot.Step = 0;
				pMold->RobotOpnCls.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			
			if(40000 == pMold->Robot.Step)	/* 機械手進故障   */
			{
				pMold->Robot.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->RobotOpnCls.Step)/* 機械手關故障   */
			{
				pMold->RobotOpnCls.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}

			break;

		
		case 2000:
			if ( 1 == pMold->TransDIn.TopDeflashBwLimit && 1 == pMold->TransDIn.BottomDeflashBwLimit )  // ipis wait all bw
			{
				pMold->Clamp.Step = 12100;		  	/*  開模洩壓 + 微開模 */
				pMold->AutoStep = 2050;
			}		
			break;
				
		case 2050:

			if(13000 == pMold->Clamp.Step)		/*  開模洩壓结束  */
			{
				pMold->AutoStep = 2100;
			}	
						
			if(40000 == pMold->Clamp.Step)  	/*  開模洩壓超时	*/
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2100:
	
			pMold->SubMold.Step 	  = 10100;  /*  Topsubmold open        */
			pMold->BottomSubMold.Step = 10100;	/*  Bottomsubmold open     */
			pMold->Clamp.Step 		  = 10100;	/*  Clamp open */
			
			pMold->AutoStep = 2200; 

			break;

		case 2200:
			if(13000 == pMold->SubMold.Step && 13000 == pMold->BottomSubMold.Step )	/*  子模开结束  */
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step =0;
				pMold->AutoStep = 2300;
			}

			if(40000 == pMold->SubMold.Step)  /*  子模开超时  */
			{
				pMold->SubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->BottomSubMold.Step)  /*  下子模开超时  */
			{
				pMold->BottomSubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Clamp.Step)  	/*  開模超时		*/
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;

		case 2300:
			if (pMold->Clamp.mmAs > pMold->Clamp.S_SubMoldIn )  //ipis1113
			{
				pMold->SubMold.Step  = 100;  		/*  子模关闭 有上子模的时候必须先关闭子模可以架上*/ //ipis1113
				pMold->BottomSubMold.Step  = 100;  	
				pMold->AutoStep = 2400; //ipis1113
			
			}
			if(40000 == pMold->Clamp.Step)  	/*  开模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
		
			break;
		
		
		case 2400:   /***Cycle End***/
			if(13000 == pMold->Clamp.Step )	/*  开模结束  */
			{
				pMold->Clamp.Step = 0;
				
				if (1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotClampOpn = 1;	/*  外部機械手  */
				}
				pMold->Robot.Step = 10100;		/*  機械手退+開  */
				pMold->HookPin.Step = 10100;		/*  掛勾吹針退  */
			
				pMold->AutoStep   = 2450;
			}
		
			if(40000 == pMold->Clamp.Step)  	/*  开模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2450: //top submold cls must check before carriage fw  ipis1112
			if( 3000 == pMold->SubMold.Step && 3000 == pMold->BottomSubMold.Step )		/*  子模关结束  */
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step = 0;
				pMold->AutoStep   = 2500;
			}
			
			if(40000 == pMold->SubMold.Step)  	/*  子模关超时   */
			{
				pMold->SubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		case 2500:
			if(1 == pMold->Option.PinOpenUp)	/*  开模后吹针上，模内贴标用,此项目不用 */
			{
				pMold->BlowPin.Step = 10100;
				pMold->AutoStep     = 2600;
			}
			else
			{
				pMold->AutoStep = 2650;
			}
			break;

		case 2600:				/* 吹针上过程 */
			if( 13000 == pMold->BlowPin.Step)
			{
				pSPC->tBlowPinUp = pMold->TimeDis.BlowPinUpAlarmTime/100.0;
				pMold->BlowPin.Step = 0;
				pMold->AutoStep = 2650;
			}

			if(40000 == pMold->BlowPin.Step)/*  吹针上超时  */
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2650:	
			if(1 == pMold->SemiAuto)		/*  自动停止 */						
			{
				pMold->CurCycEnd = 1;
				if (13000 == pMold->Robot.Step && (13000 == pMold->RobotOpnCls.Step || 0 == pMold->RobotOpnCls.Step ) )
				{	
					pMold->Robot.Step = 0;
					pMold->RobotOpnCls.Step = 0;
					pMold->ValveOut.ProductAuxClamp =0; //ipis0219
					pMold->SemiAuto = 0;
					pMold->AutoStep = 15450; //15100
				}
				
				if(40000 == pMold->Robot.Step)	/* 機械手退故障   */
				{
					pMold->Robot.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->RobotOpnCls.Step)/* 機械手開故障   */
				{
					pMold->RobotOpnCls.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				pMold->AutoStep = 2700;
			}									
			break;

		
		case 2700:
			if(1 == pMold->Clamp.OpnPos)		/* 开模判断    */
			{
				pMold->AutoStep = 3000;
			}
			break;

		case 3000:
			if(pMold->LengthCtrl.Enable)
			{
				if(1 == pMold->LengthCtrl.MagicEye)
				{
					pMold->AutoStep = 3050;
				}
			}
			else
			{
				pMold->AutoStep = 3050;
			}
			break;
		
		case 3050:

			
			pMold->AutoStep   = 3060;
			
			break;
		
		case 3060:

			pMold->AutoStep   = 3100;
			break;

		
		/***********************************************************************/		
			
		/* ------------ carriage tansport start --------------- */
		case 3100:
			
			pMold->InnerPin.Step = 30000;  		/*  模内吹针复位 */
			pMold->Sealer.Step   = 30000;    	/*  封口汽缸复位 */
			pMold->Screw.Step    = 100;			/*  绞牙进       */
			pMold->Carriage.Step = 100;         /*  架上启动   	 */
			pMold->AutoStep = 3200;
			
			pMold->ValveOut.RobotCarriageDn = 0;// Reset 外部機械手訊號

			break;

		case 3200:								      /*  架上过程   */
			if(3000 == pMold->Carriage.Step)
			{
				if (1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotCarriageUp = 1;
				}
			
				if(pMold->Robot.Step > 10100 && pMold->Robot.Step < 10300) // 強制機械手退結束 (開不強制)
				{
					pMold->Alarm.RobotBwTimeout  = 1;
					pMold->RobotOpnCls.RobotBwTimeOutFlag = 1;	/*  機械手開 force 	*/
					pMold->Robot.Step  = 10300;					/*  機械手退 force 	*/
					pMold->Robot.Timer.IN = 0;	
				}
//				else if ( 13000 == pMold->Robot.Step && (pMold->RobotOpnCls.Step > 10100 &&  pMold->RobotOpnCls.Step < 10300 ))
//				{
//					pMold->Alarm.RobotOpnTimeout  = 1;
//					pMold->RobotOpnCls.Step = 10300; /*  機械手開 force 	*/
//					pMold->RobotOpnCls.Timer.IN = 0;
//				}
				

				pMold->Timer.IN = 1;
				pMold->Timer.PT = 2000;
			
				pMold->Carriage.Step = 0; /*8000; 保持架上位 */
//				pMold->AutoStep = 3300;
				pMold->AutoStep = 3250;
			}

			if(40000 == pMold->Carriage.Step)   /*  架上超时 */
			{
				pMold->Carriage.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		//*** ExtInMoldSticker  ***// 
		case 3250:
					
			if(1 == pMold->Option.ExtInMoldSticker)
			{
				if(1 == pMold->Clamp.ExtInMoldInDone_Flag )		/*  外部模內貼  退回原點完成  */
				{
					pMold->Clamp.ExtInMoldInDone_Flag = 0;
					pMold->AutoStep = 3260;
					pMold->Clamp.ExtInMoldInDone_cnt = 0;
				}
				else if(0 == pMold->Clamp.ExtInMoldInDone_Flag )
				{
					pMold->Clamp.ExtInMoldInDone_cnt = pMold->Clamp.ExtInMoldInDone_cnt + 1; 
				}
	
				if(1 == pMold->Alarm.ExtInMoldStickerTimeOut)
				{	
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
			}
			else
			{
				pMold->Clamp.ExtInMoldInDone_Flag = 0;
				pMold->AutoStep = 3260;
			}
			
			break;
		
		case 3260:
			
			pMold->AutoStep   = 3300;
			break;
		
		case 3300:
			pMold->TimeDis.MoldClsWait = pMold->Timer.ET;
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)   /* 外部壁厚控制 */
			{
				//Ext Parison Internal Timer
				if(1 == gMacTimer.ExtParison.Q || 1 == pMold->AutorunFirstCycle)
				{
					if( 1 == pMold->AutorunFirstCycle) //Parison Alarm Bypass
					{
						//						pMold->AutorunFirstCycle = 0;
						pMold->Alarm.ParisonTimeSmall = 0;
						pMold->AutoStep = 3400;	
					}
					else
					{
						pMold->Alarm.ParisonTimeSmall = 1;
						pMold->AutoStep = 3400;				
					}
				
				}		
				else
				{
					pMold->Alarm.ParisonTimeSmall = 0;
					pMold->AutoStep = 3400;
				}
			
			}
			else
			{
				if(1 == gThickIO.Out.ControlEnd)	/*  壁厚时间超时 */
				{
					if( 1 == pMold->AutorunFirstCycle) /*Parison Alarm Bypass*/  //owen 20200923
					{
						pMold->AutorunFirstCycle = 0;
						pMold->Alarm.ParisonTimeSmall = 0;
						pMold->AutoStep = 3400;	
					}
					else
					{
						pMold->Alarm.ParisonTimeSmall = 1;
						pMold->AutoStep = 3400;				
					}
				}
				else
				{
					pMold->Alarm.ParisonTimeSmall = 0;
					pMold->AutoStep = 3400;
				}
			}
			break;

		case 3400:
			pMold->TimeDis.MoldClsWait = pMold->Timer.ET;
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)   /* 外部壁厚控制 */
			{
				//ExtParison Internal Timer
				if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)	
				{
					gMacTimer.ExtParison.PT = gUserPara.ThickTime_TwoMold;
				}
				else
				{
					gMacTimer.ExtParison.PT = gUserPara.ThickTime_OneMold;
				}
			
				if(gMacTimer.ExtParison.PT != 0)	/*  壁厚时间设置非零时处理 */
				{
					if(1 == gMacTimer.ExtParison.Q || 1 == pMold->AutorunFirstCycle)
					{
						pMold->AutorunFirstCycle = 0;
						gMacTimer.ExtParison.IN = 0;
						pMold->AutoStep = 3500;
					}
				}
				else
				{
					pMold->Alarm.ParisonTimeSmall = 1;
					pMold->AutoStep = 3500;
				}
			}
			else
			{
				if(gThickProf.TimSet.CycleTime != 0)	/*  壁厚时间设置非零时处理 */
				{
					if(1 == gThickIO.Out.ControlEnd)
					{
						gThickIO.Out.ControlEnd = 0;
						pMold->AutoStep = 3500;
					}
				}
				else
				{
					pMold->Alarm.ParisonTimeSmall = 1;
					pMold->AutoStep = 3500;
				}
			}
			break;

		case 3500:
			pMold->TimeDis.MoldClsWait = pMold->Timer.ET;
			pMold->Timer.IN = 0;
			pMold->TimeDis.CycleAlarmTime = pMold->TimeCycle.ET;  /*  周期时间收集  */
			pSPC->tCycle = pMold->TimeDis.CycleAlarmTime/100.0;
			pMold->TimeCycle.IN = 0;
			pMold->AutoStep = 3700;
			
			//gExSPC
			if(pMold == & LMold)
			{
				gExSPC[ExIndex].LMold.MoldClsWait = pMold->Timer.ET/100.0;
			}
			else
			{
				gExSPC[ExIndex].RMold.MoldClsWait = pMold->Timer.ET/100.0;
			}
			
			
			//gExSPC
			if(pMold == & LMold)
			{
				gExSPC[ExIndex].LMold.tCycle = pMold->TimeDis.CycleAlarmTime/100.0;
			}
			else
			{
				gExSPC[ExIndex].RMold.tCycle = pMold->TimeDis.CycleAlarmTime/100.0;
			}
		
			//gExSPC
			if(pMold == & LMold)
			{
				gExSPC[ExIndex].LMold.tTotalCycle = gExSPC[ExIndex].LMold.ClampClsDelay + gExSPC[ExIndex].LMold.tCalmpCls + 
					gExSPC[ExIndex].LMold.ClampHoldTime + gExSPC[ExIndex].LMold.ClampLockTime +
					gExSPC[ExIndex].LMold.BlowPinUpDelay + gExSPC[ExIndex].LMold.CarriageBwDelay +
				
					gExSPC[ExIndex].LMold.tCarriageBw + gExSPC[ExIndex].LMold.CarriageBwHoldTime +
					gExSPC[ExIndex].LMold.BlowPinDnDelay + gExSPC[ExIndex].LMold.BlowPinDnTime + 
					gExSPC[ExIndex].LMold.BlowingDelay + 
				
					gExSPC[ExIndex].LMold.BlowingTime +gExSPC[ExIndex].LMold.AirVentingTime + 
					gExSPC[ExIndex].LMold.BlowingTime2 +gExSPC[ExIndex].LMold.AirVentingTime2 +
				
													 
				
					gExSPC[ExIndex].LMold.CoolPinBlowDelay + 
					gExSPC[ExIndex].LMold.CoolPinBlowTime + gExSPC[ExIndex].LMold.CoolPinDnDelay +
					gExSPC[ExIndex].LMold.CoolPinDnTime + gExSPC[ExIndex].LMold.tBlowPinUp +		
				
					gExSPC[ExIndex].LMold.TopDeflashFwDelay + gExSPC[ExIndex].LMold.tTopDeflash1 +	
					gExSPC[ExIndex].LMold.TopDeflashSecDelay + gExSPC[ExIndex].LMold.tTopDeflash2 +	
					gExSPC[ExIndex].LMold.TopDeflashBwDelay + gExSPC[ExIndex].LMold.tTopDeflashBw +	
				
					gExSPC[ExIndex].LMold.BlowpintinyreturnDelay + gExSPC[ExIndex].LMold.BlowpintinyreturnTime +	
					gExSPC[ExIndex].LMold.tSubMoldMicroOpnTime + 
													
					gExSPC[ExIndex].LMold.ClampOpnDelay + gExSPC[ExIndex].LMold.tClampOpn +
													
					gExSPC[ExIndex].LMold.CarriageFwDelay + gExSPC[ExIndex].LMold.tCarriageFw +	
													
					gExSPC[ExIndex].LMold.CarriageFwHoldTime + gExSPC[ExIndex].LMold.MoldClsWait; 
	
				
			}
			else
			{
				gExSPC[ExIndex].RMold.tTotalCycle = gExSPC[ExIndex].RMold.ClampClsDelay + gExSPC[ExIndex].RMold.tCalmpCls + 
					gExSPC[ExIndex].RMold.ClampHoldTime + gExSPC[ExIndex].RMold.ClampLockTime +
					gExSPC[ExIndex].RMold.BlowPinUpDelay + gExSPC[ExIndex].RMold.CarriageBwDelay +
												
					gExSPC[ExIndex].RMold.tCarriageBw + gExSPC[ExIndex].RMold.CarriageBwHoldTime +
					gExSPC[ExIndex].RMold.BlowPinDnDelay + gExSPC[ExIndex].RMold.BlowPinDnTime + 
					gExSPC[ExIndex].RMold.BlowingDelay + 
												
					gExSPC[ExIndex].RMold.BlowingTime +gExSPC[ExIndex].RMold.AirVentingTime + 
					gExSPC[ExIndex].RMold.BlowingTime2 +gExSPC[ExIndex].RMold.AirVentingTime2 +
												
																					 
												
					gExSPC[ExIndex].RMold.CoolPinBlowDelay + 
					gExSPC[ExIndex].RMold.CoolPinBlowTime + gExSPC[ExIndex].RMold.CoolPinDnDelay +
					gExSPC[ExIndex].RMold.CoolPinDnTime + gExSPC[ExIndex].RMold.tBlowPinUp +		
												
					gExSPC[ExIndex].RMold.TopDeflashFwDelay + gExSPC[ExIndex].RMold.tTopDeflash1 +	
					gExSPC[ExIndex].RMold.TopDeflashSecDelay + gExSPC[ExIndex].RMold.tTopDeflash2 +	
					gExSPC[ExIndex].RMold.TopDeflashBwDelay + gExSPC[ExIndex].RMold.tTopDeflashBw +	
												
					gExSPC[ExIndex].RMold.BlowpintinyreturnDelay + gExSPC[ExIndex].RMold.BlowpintinyreturnTime +	
					gExSPC[ExIndex].RMold.tSubMoldMicroOpnTime + 
																					
					gExSPC[ExIndex].RMold.ClampOpnDelay + gExSPC[ExIndex].RMold.tClampOpn +
																					
					gExSPC[ExIndex].RMold.CarriageFwDelay + gExSPC[ExIndex].RMold.tCarriageFw +	
																					
					gExSPC[ExIndex].RMold.CarriageFwHoldTime + gExSPC[ExIndex].RMold.MoldClsWait; 
			}
			
			// 雙模增加ExIndex條件  //單模增加ExIndex條件        
			if(pMold == & LMold  || (pMold == & RMold && LMold.AutoStep == 10000) )
			{
				if (ExIndex< 19)		ExIndex = ExIndex+1;
				else  ExIndex = 0;	
			}
			
			
			break;
		
		case 3700:
			//			if(3000 == pMold->Screw.Step)		/*  绞牙进结束  */
			//			{
			//				pMold->Screw.Step = 0;
			//		pMold->AutoStep   = 3800;   // ipis1112
			pMold->AutoStep   = 3800;
			//			}

			//			if(40000 == pMold->Screw.Step)  	/*  绞牙进超时   */
			//			{
			//				pMold->Screw.Step = 0;
			//				pMold->StopAutoStep = pMold->AutoStep;
			//				pMold->AutoStep = 40000;
			//			}
			break;
		
		case 3800:
			pMold->Sealer.Step   = 100;			/*   封口       */
			pMold->Clamp.Step    = 100;         /*   合模       */
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				RMold.ExtrLift.Step = 100;			/*   抬头       */
			}
			else
			{
				pMold->ExtrLift.Step = 100;			/*   抬头       */
			}
			pMold->InnerPin.Step = 100;			/*  模内小吹针  */
			pMold->SupportSuckAir.Step = 100;   /*  模頭抽真空  */
			pMold->Cutter.Step = 100;			/*   切刀启动   */
			
			if( SUPPORTAIR_MODE_AFTER_CLAMP_CLS == gMachineFix.MoldR.SupportAir.eSupportAirMode) // 脹風啟動（以關模為啟動條件）
			{
				pMold->SupportAir.Step = 100;// air support
			}
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)
			{
				gMachineInfo.ExtParisonCtrlStart = 1; /*   外部壁厚启动   */
				//ExtParison Internal Timer
				gMacTimer.ExtParison.IN = 1;
				if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)	
				{
					gMacTimer.ExtParison.PT = gUserPara.ThickTime_TwoMold;
				}
				else
				{
					gMacTimer.ExtParison.PT = gUserPara.ThickTime_OneMold;
				}
			}
			else
			{
				gThickIO.In.StartThick = 1;      /*   壁厚启动   */
				pMold->LengthCtrl.Start = 1; 	 /*   长度控制   */
				//	RMold.SupportAir.Step = 100;/*Albert Mold Pre-Blow*/
				pMold->SupportAir.cntTime = gThickProf.TimSet.CycleTime;/*Albert*/
			}
			pMold->ValveOut.RobotClampOpn = 0; // Reset 外部機械手訊號
			
			pMold->OnceBlowPinUp = 1;    /*桿下次數計算*/ 
			
			pMold->TimeCycle.IN   = 1;
			pMold->TimeCycle.PT   = pMold->TimeSet.CycleAlarmTime;
			
			
			/////////////////////////////////////////////////////////////
			gCycleTimes++;
			//SPC CleanCycle
			if(pMold == &RMold)
			{
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tCycle;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tBlowPinUp;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tCalmpCls;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tCalmpOpn;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tCarriageFw;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.tCarriageBw;
				//				gSPC_Current.MoldR.tBlowPinUp = gSPC_Current_Buffer.MoldR.sCarriageFw;
				//				memset(&gSPC_Current_Buffer.MoldR,0,sizeof(gSPC_Current_Buffer.MoldR));
				gSPC_Current.MoldR.bCleanCycle = 1;
			}
			else if(pMold == &LMold)
			{
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tCycle;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tBlowPinUp;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tCalmpCls;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tCalmpOpn;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tCarriageFw;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.tCarriageBw;
				//				gSPC_Current.MoldL.tBlowPinUp = gSPC_Current_Buffer.MoldL.sCarriageFw;
				//				memset(&gSPC_Current_Buffer.MoldL,0,sizeof(gSPC_Current_Buffer.MoldL));
				gSPC_Current.MoldL.bCleanCycle = 1;
			}
			
			gMachineInfo.CycleTimes = gCycleTimes;
			gMachineInfo.Yield = gMachineInfo.CycleTimes * gMachinePara.DieheadNum;	
			if((gMachinePara.StopYield > 0) && (0 == gMachinePara.bReachYieldStop)) 
			{	
				if(gMachineInfo.Yield >= gMachinePara.StopYield)
				{
					gAlarm.CycleTimeCounReach =1;			
				}	
			}

			if(pMold == &LMold)
			{
				gCycleTimesL++;
			}
			
			if(pMold == &RMold)
			{
				gCycleTimesR++;
			}
			///////////////////////////////////////////////////////////////////////////////
			
			
			pMold->AutoStep = 3900;
			break;

		case 3900:   /*   合模过程   */
			if ( 1 == pOptionFix->bBlowUpDuringClampCls)
			{
				if(pMold->Clamp.mmAs < pMold->Clamp.S_BlowPinUp)	/*  合模中吹针上    */
				{
					if(1 == pMold->OnceBlowPinUp)
					{
						pMold->OnceBlowPinUp = 0;
						if(0 == pMold->Option.PinOpenUp)	
						{
							//							pMold->BlowPin.Step = 10100;   
							//							if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
							//							{
							//								pMold->BlowPin.Step = 10100;  
							//							}
							//							else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
							//							{
							//								pMold->BlowPin.Step = 10100;  
							//							}
							//							else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
							//							{
							pMold->BlowPin.Step = 10100; 
							pMold->BlowPin2.Step = 10100; 
							//							}
						}
					}
				}
			}
			
			if(3000 == pMold->Clamp.Step)
			{		
				if (0 == pOptionFix->bBlowUpDuringClampCls)
				{
					if(1 == pMold->OnceBlowPinUp)
					{
						pMold->OnceBlowPinUp = 0;
						if(0 == pMold->Option.PinOpenUp)	/*  合模后吹针上    */
						{
							//							pMold->BlowPin.Step = 10100;    		
							//							if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
							//							{
							//								pMold->BlowPin.Step = 10100;  
							//							}
							//							else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
							//							{
							//								pMold->BlowPin.Step = 10100;  
							//							}
							//							else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
							//							{
							pMold->BlowPin.Step = 10100; 
							pMold->BlowPin2.Step = 10100; 
							//							}
						}
					}
				}
			
				pMold->Clamp.Step = 3400; /*Clamp holding and locking step*/
				pMold->AutoStep = 3950;   
			
				if ( 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  於下一循環關模後轉一上    */
				{
					pMold->CoolPin.Step = 500; /*force stop*/
				}
			}

			if(40000 == pMold->Clamp.Step)      /*  合模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}

			
			if(40000 == pMold->BlowPin.Step)    
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->BlowPin2.Step)    
			{
				pMold->BlowPin2.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;
 
		case 3950:   /* 關模保持   &&  吹针上过程  &&  (於下一循環關模後轉一上 && 轉一上超時)  */
			
			if (pMold->Clamp.Step > 3400 && pMold->Clamp.Step < 4000)
			{
				pMold->AutoStep = 4000; 
			}
			
			if(40000 == pMold->BlowPin.Step )    
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->BlowPin2.Step )    
			{
				pMold->BlowPin2.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Clamp.Step)      /*  合模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Clamp.Step)      /*  合模超时   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if ( 40000 == pMold->CoolPin.Step && 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  於下一循環關模後轉一上 && 轉一上超時   */  
			{
				pMold->CoolPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
		
			break;
		
		case 4000:

			pMold->AutoStep = 4100;
			
			break;

		case 4100:								      /*  吹针上过程  */
			if(0 == pMold->Option.PinOpenUp)
			{
				//				if( 13000 == pMold->BlowPin.Step)
				//				{
				//					pSPC->tBlowPinUp = pMold->TimeDis.BlowPinUpAlarmTime/100.0;
				//					pMold->BlowPin.Step = 0;
				//					//	pMold->AutoStep = 4200; // ipis1114
				//					pMold->AutoStep = 4150;
				//					
				//					//gExSPC
				//					if(pMold == & LMold)
				//					{
				//						gExSPC[ExIndex].LMold.tBlowPinUp = pMold->BlowPin.LimitTimer.ET/100.0;
				//					}
				//					else
				//					{
				//						gExSPC[ExIndex].RMold.tBlowPinUp = pMold->BlowPin.LimitTimer.ET/100.0;
				//					}
				//				}
				
				//				if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
				//				{
				//					if( 13000 == pMold->BlowPin.Step)
				//					{
				//						pMold->BlowPin.Step = 0;
				//						pMold->AutoStep = 4150;
				//					}  
				//				}
				//				else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
				//				{
				//					if( 13000 == pMold->BlowPin2.Step)
				//					{
				//						pMold->BlowPin2.Step = 0;
				//						pMold->AutoStep = 4150;
				//					}  
				//				}
				//				else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
				//				{
				if( 13000 == pMold->BlowPin.Step && 13000 == pMold->BlowPin2.Step)
				{
					pSPC->tBlowPinUp = pMold->TimeDis.BlowPinUpAlarmTime/100.0;
					pMold->BlowPin.Step = 0;
					pMold->BlowPin2.Step = 0;
					//	pMold->AutoStep = 4200; // ipis1114
					pMold->AutoStep = 4150;
					
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.tBlowPinUp = pMold->BlowPin.LimitTimer.ET/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.tBlowPinUp = pMold->BlowPin.LimitTimer.ET/100.0;
					}
				}   
				//				}
			


				if(40000 == pMold->BlowPin.Step)/*  吹针上超时  */
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->BlowPin2.Step)/*  吹针上超时  */
				{
					pMold->BlowPin2.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->Clamp.Step)      /*  合模超时   */
				{
					pMold->Clamp.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if ( 40000 == pMold->CoolPin.Step && 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  於下一循環關模後轉一上 && 轉一上超時   */ 
				{
					pMold->CoolPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				pMold->AutoStep = 4150;
			}
			break;

		case 4150:
			
			if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)  //Hyd Clamp
			{ 
				if( 3900 == pMold->Clamp.Step )  /*  關模保持結束   */
				{
					pMold->Clamp.Step = 0;
				
					if(1 == pOptionFix->bPlateFwAfterClampHold)
					{
						pMold->Plate.Step = 6500;  // for Plate Fw in auto mode (timer)
					}
				
//					gMachineInfo.OilAccuChargeAuto_Extrlift = 1;  // for  Extrlift accu charge in auto mode
					pMold->AutoStep = 4170;		
				}
				
				if(40000 == pMold->Clamp.Step)      /*  合模超时   */
				{
					pMold->Clamp.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType) //Elec Clamp
			{
				//check Electric Lock Done in case 6000 
				if(1 == pOptionFix->bPlateFwAfterClampHold)
				{
					pMold->Plate.Step = 6500;  // for Plate Fw in auto mode (timer)
				}
				
//				gMachineInfo.OilAccuChargeAuto_Extrlift = 1;  // for  Extrlift accu charge in auto mode
				pMold->AutoStep = 4170;	
					
			} 
			break;
		
		case 4170:
			
			if(1 == pOptionFix->bCoolPinUpNextCycClampCls)
			{
				if( 3000 == pMold->CoolPin.Step )  	/*  轉一上結束   */
				{
					pMold->CoolPin.Step = 0;
					pMold->AutoStep = 4200;		
				}
			
				if ( 40000 == pMold->CoolPin.Step )    /*  轉一上超時   */ 
				{
					pMold->CoolPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				pMold->AutoStep = 4200;	
			}
			
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if(40000 == RMold.ExtrLift.Step)     // 移下前確認抬頭偵測時間超時
				{
					RMold.ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				if(40000 == pMold->ExtrLift.Step)     // 移下前確認抬頭偵測時間超時
				{
					pMold->ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			break;
		
		
		case 4200:  
			// 移下前確認機械手退位(沒有確認開)
			if( 1 == pMold->Robot.Transfer.BwPos &&  1 == pMold->TransDIn.ExtRobotUpLmit)
			{	
				if (( pMold->Robot.Step > 11000 && pMold->Robot.Step <= 13000) || 0 == pMold->Robot.Step )  /*  機械手退結束  */
				{
					pMold->Robot.Step = 0;
					
					pMold->Carriage.Step = 10100;	/*  移下啟動   */
					pMold->ValveOut.RobotCarriageUp = 0;  // Reset 外部機械手訊號
					pMold->PinPart.Step  = 100;		/*  吹针块进   */
					pMold->AutoStep = 4250;	
				}
			}
			else
			{	
				pMold->Alarm.RobotNotAtBwPos = !pMold->Robot.Transfer.BwPos;
				pMold->Alarm.ExtRobotNotAtUp = !pMold->TransDIn.ExtRobotUpLmit;
				pMold->Alarm.RobotBwTimeout  = 1;
			
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Robot.Step)   /*  機械手退超時 */
			{
				pMold->Robot.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;
		
		case 4250:  
			// 移下前確認掛勾吹針退位
			if( 1 == pMold->TransDIn.HookPinBwLimit )
			{	
				if (13000 == pMold->HookPin.Step || 0 == pMold->HookPin.Step )  /*  掛勾吹針退結束  */
				{
					pMold->HookPin.Step = 0;
					pMold->AutoStep = 4300;	
				}
			}
			else
			{	
				pMold->Alarm.HookPinNotAtBwPos = 1;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->HookPin.Step)   /*  掛勾吹針退超時 */
			{
				pMold->HookPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;

		case 4300:								      /*  架下过程  */
			if((13000 == pMold->Carriage.Step) )
			{
				pMold->CurCycEnd = 1;				 /*另外一模準備*/
				pMold->Carriage.Step = 0; /*18000;	 保持架下位 */
			
				if(1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotCarriageDn = 1;
				}
			
				pMold->AutoStep = 4400;	          
			}
			
			if(40000 == pMold->Carriage.Step)   /*  架下超时 */
			{
				pMold->Carriage.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;

		case 4400:
			if(3000 == pMold->PinPart.Step)
			{
				pMold->PinPart.Step = 0;
				pMold->AutoStep = 4410; //ipis0308
			}

			if(40000 == pMold->PinPart.Step)   /*  吹针块进超时 */
			{
				pMold->PinPart.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			} 
			break;
		
		case 4410:
			pMold->HookPin.Step = 100;			/*  掛勾吹針進  */
			pMold->AutoStep = 4420; 

			break;
		
		case 4420:
			if(3000 == pMold->HookPin.Step)   
			{
				pMold->HookPin.Step = 0;
				pMold->AutoStep = 4500; 
			}

			if(40000 == pMold->HookPin.Step)   /*  掛勾吹超时 */
			{
				pMold->HookPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		case 4500:	

			//			if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
			//			{
			//				pMold->BlowPin.Step = 100;			/*  吹针下  	 */
			//			}
			//			else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			//				pMold->BlowPin2.Step = 100;			/*  吹针下  	 */
			//			}
			//			else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			/*************/
			pMold->BlowPin.Step = 100;			/*  吹针下  	 */
			pMold->BlowPin2.Step = 100;			/*  吹针下  	 */
			//			}
			//			pMold->BlowPin.Step = 100;			/*  吹针下  	 */
			/*************/
			pMold->LowBlow.Step = 100;			/*  吹气1动作   */
			pMold->HighBlow.Step = 100;			/*  吹气2动作   */
			pMold->BlowDeflash.Step = 100;		/*  吹桿吹冷    */
			/*************/
//			pMold->CoolPin2.Step = 100;         /*  轉二下		 */
//			pMold->CoolPin2Blow.Step =100;      /*  轉二下吹冷  */
//			pMold->CoolPin3.Step = 100;	       	/*  轉三下		 */
//			pMold->CoolPin3Blow.Step =100;    	/*  轉三下吹冷  */
			//			pMold->CoolDeflash.Step = 100;		/*  打手把吹冷     */
			
			
			pMold->AutoStep = 4510;
			if( 0 == pOptionFix->bRobotFwAfterTopdeflash)
			{
//				pMold->Robot.Step = 100;			/*  機械手進+關  */
				if (  pMold->RobotOpnCls.Step >= 10100  &&  pMold->RobotOpnCls.Step < 13000) // Robot Opn too slow
				{
					pMold->Alarm.RobotOpnTimeout  = 1;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
				else
				{
					pMold->Robot.Step = 100;			/*  機械手進+關  */
				}
			}
		
			
			break;

		case 4510:

			if (0 == pOptionFix->bTopDeflashMode ||  1 == pOptionFix->bCoolPinUpNextCycClampCls) // topdeflash before/after cooling   /*如果使用於下一循環關模後轉一上，就不能走轉一手打手把*/
			{
				if(1 == gMacOption.ACCPumpCharge_Extrlift)
				{
					pMold->TopDeflash.Step     = 90;	/*  打手把(工程頁面額外延遲時間)    */  
					pMold->BottomDeflash.Step  = 90;	/*  打瓶底(工程頁面額外延遲時間)    */ 
					pMold->PullBottom.Step  = 100; 		/*  拉底启动  */
				}
				else
				{
					pMold->TopDeflash.Step    = 100;	/*  打手把    */
					pMold->BottomDeflash.Step = 100;	/*  打瓶底    */
					pMold->PullBottom.Step  = 100; 		/*  拉底启动  */
				}
				
				pMold->AutoStep = 4550;
			}
			else
			{
				pMold->CoolPin.Step     = 100;		/*  后冷却      */
				pMold->CoolPinBlow.Step = 100;		/*  后冷却吹气  */
				pMold->AutoStep = 4580;
			}
			
		
			break;
		

		case 4550:
						
			/* 打手把完成 -> 機械手進+關  */
			if(13000 == pMold->TopDeflash.Step)
			{
				pMold->TopDeflash.Step = 0;
				if( 1 == pOptionFix->bRobotFwAfterTopdeflash)
				{
					pMold->Robot.Step = 100;			/*  機械手進+關  */
					if ( pMold->RobotOpnCls.Step >= 10100  &&  pMold->RobotOpnCls.Step < 13000) // Robot Opn too slow
					{
						pMold->Alarm.RobotOpnTimeout  = 1;
					}
				}
				pMold->AutoStep = 4560;
			}
			
			if( 40000 == pMold->TopDeflash.Step  || 40000 == pMold->BottomDeflash.Step )	/* 打手把+打手挽 / 打瓶底 超時  */
			{
				pMold->TopDeflash.Step = 0;
				pMold->BottomDeflash.Step = 0;
				pMold->CoolDeflash.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
	
			// detect TopDeflashBwLimit BottomDeflashBwLimit
			if( pMold->HighBlow.Step >=900 &&  pMold->HighBlow.Step<=3000 )/*  AirVentingTime2 finish  */					
			{
				/*--------- 打手把+打手挽-----打瓶底-----拉瓶底---------- */
				/*  打手把+打手挽  */
				if( pMold->TopDeflash.Step > 100 && pMold->TopDeflash.Step < 2100 )			// Topdeflash Fw Not Done 
				{
					pMold->Alarm.TopDeflashTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay
				}
				else if ( 2100 == pMold->TopDeflash.Step  &&  pMold->TopDeflashOpnCls.Step < 10500 )  // Topdeflash Fw Done, but doing Topdeflash Opn/Cls
				{
					if(0 == pMold->Alarm.TopDeflashTimeout)pMold->Alarm.TopDeflashOpnClsTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay	
				}
				else if( pMold->TopDeflash.Step > 10100 && pMold->TopDeflash.Step < 10300 )			// Topdeflash Fw and Opn/Cls Done, Topdeflash BwDelay Not Done 
				{
					if(0 == pMold->Alarm.TopDeflashOpnClsTimeout)pMold->Alarm.TopDeflashTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay	
				}
				
				/*  打瓶底  */
				if( pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 2100 )	// BottomDeflash Fw Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
					pMold->Alarm.BottomDeflashTimeout = 1;
				}
				else if( pMold->BottomDeflash.Step > 10100 && pMold->BottomDeflash.Step < 10300 )	// BottomDeflash Fw Done, but doing BottomDeflash BwDelay Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
				}
			
				/*  拉瓶底  */
				if(pMold->PullBottom.Step > 100 && pMold->PullBottom.Step < 500)
				{
					pMold->Alarm.PullBottomTimeOut = 1;
					pMold->PullBottom.Step = 500;   //  Force PullBottom 
				}
				/*--------- 打手把+打手挽-----打瓶底-----拉瓶底---------- */
				if( 3000 == pMold->HighBlow.Step)
				{
					pMold->AutoStep  = 4600;
				}
				
			}
							
			break;

		

		case 4560:

			if (0 == pOptionFix->bTopDeflashMode) 
			{		
				pMold->AutoStep = 4570;
			}
			else
			{
				pMold->AutoStep = 4600;	
			}
			
			break;
		
		
		case 4570:

			pMold->CoolPin.Step     = 100;		/*  后冷却      */
			pMold->CoolPinBlow.Step = 100;		/*  后冷却吹气  */
		

			pMold->AutoStep = 4600;
			break;
		
		/*----------------------------------*/		
		
		case 4580:   
					
			if(3000 == pMold->CoolPinBlow.Step)
			{
				pMold->CoolPin.Step = 500;    
				pMold->AutoStep = 4590;
			}	
			
			/*----ipis0221-----------------------------------------------------------------------*/	
			if(3000 == pMold->HighBlow.Step )/*  吹气结束  */
			{
				if(pMold->BlowDeflash.Step < 500 && pMold->BlowDeflash.Step > 0)pMold->BlowDeflash.Step = 500;
				
				pMold->Alarm.TopDeflashTimeout = 1;
			
				pMold->Lock.Step  = 30000;	/* 锁模结束  */
				pMold->Clamp.Step = 0;
				//			pMold->LowBlow.Step = 0;
				//			pMold->HighBlow.Step = 0;
				pMold->CoolPin.Timer.IN = 0;
				pMold->CoolPin2.Timer.IN = 0;
				pMold->CoolPin3.Timer.IN = 0;
			
				if ( 0 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  不使用"於下一循環關模後轉一上"    */
				{
					pMold->CoolPin.Step     = 500;		/*  后冷却   force stop   */
				}
				pMold->CoolPinBlow.Step = 900;		/*  后冷却吹气 force stop */
			
				pMold->CoolPin2.Step     = 500;		/*  后冷却   force stop   */
				pMold->CoolPin2Blow.Step = 900;		/*  后冷却吹气 force stop */
			
				pMold->CoolPin3.Step     = 500;		/*  后冷却   force stop   */
				pMold->CoolPin3Blow.Step = 900;		/*  后冷却吹气 force stop */
			
			
				pMold->InnerPin.Step = 10100;	   /* 模内吹针退    */
				//			pMold->Lock.Step    = 10100;		/*  卸压         */
			
				//	pMold->BlowPin.Step = 0;         /* 吹针保持下结束*/

			
	
	
				pMold->AutoStep  = 4600;
			
			}
			/*---------------------------------------------------------------------------*/	
		

			break;
		
		case 4590:
					
			if(3000 == pMold->CoolPin.Step)
			{  
				pMold->TopDeflash.Step  = 100;		/*  顶打飞边    */
				pMold->AutoStep = 4550;
			}			
			if(40000 == pMold->CoolPin.Step) /*后冷却上超时   */
			{
				pMold->CoolPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		/*----------------------------------------------*/
				
		case 4600:
			if( ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType )	
			{
				if(3000 == pMold->BlowPin.Step && 3000 == pMold->BlowPin2.Step  && 1== pMold->TransDIn.TopDeflashBwLimit ) /*  吹针下结束 */	
				{
					pMold->BlowPin.Step = 0;
					pMold->BlowPin2.Step = 0;
					
					pMold->AutoStep = 4700;
				}

				if(40000 == pMold->BlowPin.Step )	/*  吹针下超时 */
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->BlowPin2.Step )	/*  吹针下超时 */
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	
			{
				gMachineInfo.OilAccuChargeAuto_Extrlift = 1;  // for  Extrlift accu charge in auto mode
				pMold->AutoStep = 4700;
			}
			
			break;

		case 4700:		
			if( pMold->HighBlow.Step >=900 &&  pMold->HighBlow.Step<=3000 ) /*  AirVentingTime2 finish  */	
			{	
				/*--------- 打手把+打手挽-----打瓶底-----拉瓶底---------- */
				/*  打手把+打手挽  */
				if( pMold->TopDeflash.Step > 100 && pMold->TopDeflash.Step < 2100 )			// Topdeflash Fw Not Done 
				{
					pMold->Alarm.TopDeflashTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay
				}
				else if ( 2100 == pMold->TopDeflash.Step  &&  pMold->TopDeflashOpnCls.Step < 10500 )  // Topdeflash Fw Done, but doing Topdeflash Opn/Cls
				{
					if(0 == pMold->Alarm.TopDeflashTimeout)pMold->Alarm.TopDeflashOpnClsTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay	
				}
				else if( pMold->TopDeflash.Step > 10100 && pMold->TopDeflash.Step < 10300 )			// Topdeflash Fw and Opn/Cls Done, Topdeflash BwDelay Not Done 
				{
					if(0 == pMold->Alarm.TopDeflashOpnClsTimeout)pMold->Alarm.TopDeflashTimeout = 1;
					pMold->TopDeflash.AutoTimeOutFlag = 1;  		  // TopDeflash Pass Delay
					pMold->TopDeflashOpnCls.AutoTimeOutFlag = 1;	  // TopDeflashOpnCls Pass Delay	
				}
				
				/*  打瓶底  */
				if( pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 2100 )	// BottomDeflash Fw Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
					pMold->Alarm.BottomDeflashTimeout = 1;
				}
				else if( pMold->BottomDeflash.Step > 10100 && pMold->BottomDeflash.Step < 10300 )	// BottomDeflash Fw Done, but doing BottomDeflash BwDelay Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
				}
			
				/*  拉瓶底  */
				if(pMold->PullBottom.Step > 100 && pMold->PullBottom.Step < 500)
				{
					pMold->Alarm.PullBottomTimeOut = 1;
					pMold->PullBottom.Step = 500;   //  Force PullBottom 
				}
				/*--------- 打手把+打手挽-----打瓶底-----拉瓶底---------- */

				if (pMold->CoolPinBlow.Step!=3000)
				{
					pMold->Alarm.TopDeflashTimeout = 1;	
				}
			
				pMold->BlowDeflash.Step = 500;		/*  轉一下   	force stop	*/
				
				if ( 0 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  不使用"於下一循環關模後轉一上"    */
				{
					pMold->CoolPin.Step     = 500;		/*  后冷却   force stop   */
				}
				pMold->CoolPinBlow.Step = 900;		/*  轉一下吹冷 force stop  */
			
				pMold->CoolPin2.Step     = 500;		/*  轉二下   	force stop	*/
				pMold->CoolPin2Blow.Step = 900;		/*  轉二下吹冷 force stop  */
			
				pMold->CoolPin3.Step     = 500;		/*  轉三下   	force stop	*/
				pMold->CoolPin3Blow.Step = 900;		/*  轉三下吹冷 force stop  */
			
				if( pMold->Robot.Step > 100 && pMold->Robot.Step < 300)
				{
					pMold->Alarm.RobotFwTimeout  = 1;
					pMold->RobotOpnCls.RobotFwTimeOutFlag = 1;	/*  機械手關 force 	*/
					pMold->Robot.Step = 300;     		/*  機械手進 force 	*/
					pMold->Robot.Timer.IN = 0;
				}
				else if ( 3000 == pMold->Robot.Step && (pMold->RobotOpnCls.Step > 100 &&  pMold->RobotOpnCls.Step < 300 ))
				{
					pMold->Alarm.RobotClsTimeout  = 1;
					pMold->RobotOpnCls.Step = 300; /*  機械手關 force 	*/
					pMold->RobotOpnCls.Timer.IN = 0;
				}
			
				
				pMold->InnerPin.Step = 10100;	   /* 模内吹针退    */
				//			pMold->Lock.Step    = 10100;		/*  卸压         */
			
				//	pMold->AutoStep  = 4800;
			
				// Elec Blowpin
				if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  電動吹桿2段低壓S超时 */
				{
					if( pMold->BlowPin.Step > 1300 && pMold->BlowPin.Step < 1500)
					{
						pMold->BlowPin.Step = 1500;
					}
			
					if(40000 == pMold->BlowPin.Step)     // 吹氣完成後確認電動吹桿2段低壓超時
					{
						pMold->BlowPin.Step = 0;
						pMold->StopAutoStep = pMold->AutoStep;
						pMold->AutoStep = 40000;
					}
				}
			
				if(3000 == pMold->HighBlow.Step)
				{
					pMold->AutoStep  = 4710;
					
//					//Blowpin
//					if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  電動關模加壓超时 */
//					{
//						if( pMold->BlowPin.Step > 1300 && pMold->BlowPin.Step < 1500)
//						{
//							pMold->BlowPin.Step = 1500;
//						}
//			
//						if(40000 == pMold->BlowPin.Step)     // 吹氣完成後確認電動關模加壓超時
//						{
//							pMold->BlowPin.Step = 0;
//							pMold->StopAutoStep = pMold->AutoStep;
//							pMold->AutoStep = 40000;
//						}
//					}
				}
			
			}
			break;
		
		
		case 4710:
	
			if( (0 == pMold->TopDeflash.Step || 13000 == pMold->TopDeflash.Step )  && 13000 == pMold->BottomDeflash.Step )	/*  打手把+打手挽 && 打瓶底 结束  */
			{    			
				pMold->TopDeflash.Step = 0;
				pMold->BottomDeflash.Step = 0;
			
				//Reset AutoTimeOutFlag
				//打手把+打手挽
				pMold->TopDeflash.AutoTimeOutFlag = 0;  	// TopDeflash Pass Delay
				pMold->TopDeflashOpnCls.AutoTimeOutFlag = 0;// TopDeflashOpnCls Pass Delay
				//打瓶底+轉一下+轉一關
				pMold->BottomDeflash.AutoTimeOutFlag = 0;	// BottomDeflash Pass Delay

			
				pMold->AutoStep = 4750;
			}
			
			
			break;
		
      
		case 4750:
			pMold->AutoStep  = 6000;
			
			//			if(3000 == pMold->CoolPin.Step)
			//			{
			//				//				if(pMold == &RMold)
			//				//				{
			//				//					RMold.Option.CoolPin = gMacOption.RCoolPin;
			//				//				}
			//				//				else
			//				//				{
			//				//					LMold.Option.CoolPin = gMacOption.LCoolPin;
			//				//				}	
			//				
			//				//	pMold->AutoStep  = 6000;
			//			}	
			//			if(40000 == pMold->CoolPin.Step) /*后冷却上超时   */
			//			{
			//				pMold->CoolPin.Step = 0;
			//				pMold->StopAutoStep = pMold->AutoStep;
			//				pMold->AutoStep = 40000;
			//			}
			
			
			break;
		
		
		
		
		//		case 4800:
		//			if(3000 == pMold->CoolPin.Step)
		//			{
		//				pMold->CoolPin.Step = 0;
		//         
		//				if(pMold == &RMold)
		//				{
		//					RMold.Option.CoolPin = gMacOption.RCoolPin;
		//				}
		//				else
		//				{
		//					LMold.Option.CoolPin = gMacOption.LCoolPin;
		//				}
		//				pMold->AutoStep = 4850;
		//			}
		//			else
		//			{
		//				if(40000 == pMold->CoolPin.Step) /*后冷却上超时   */
		//				{
		//					pMold->CoolPin.Step = 0;
		//					pMold->StopAutoStep = pMold->AutoStep;
		//					pMold->AutoStep = 40000;
		//				}
		//			}
		//			break;
		
		case 4850:
			//			pMold->TopDeflash.Step  = 100;		/*  顶打飞边    */
			//			pMold->BottomDeflash.Step = 100;	/*  底打飞边       */
			//			pMold->AutoStep = 4900;
			pMold->AutoStep = 5100;
			//			if(3000 == pMold->CoolPin2.Step)
			//			{
			//				pMold->CoolPin2.Step = 0;
			//         
			//				if(pMold == &RMold)
			//				{
			//					RMold.Option.CoolPin2 = gMacOption.RCoolPin2;
			//				}
			//				else
			//				{
			//					LMold.Option.CoolPin2 = gMacOption.LCoolPin2;
			//				}
			//				pMold->AutoStep = 4900;
			//			}
			//			else
			//			{
			//				if(40000 == pMold->CoolPin2.Step) /*后冷却上超时   */
			//				{
			//					pMold->CoolPin2.Step = 0;
			//					pMold->StopAutoStep = pMold->AutoStep;
			//					pMold->AutoStep = 40000;
			//				}
			//			}
			
			break;
		
		
		case 5100:
			
			pMold->CoolPin.Step     = 100;		/*  后冷却      */
			pMold->CoolPinBlow.Step = 100;		/*  后冷却吹气  */
			pMold->AutoStep = 5300;
			break;
		
		case 5200:
					
			if(3000 == pMold->CoolPinBlow.Step)
			{
				pMold->CoolPin.Step = 500;
         

				pMold->AutoStep = 5300;
			}

			break;
		
		case 5300:
			
			if(3000 == pMold->CoolPin.Step)
			{
				//				if(pMold == &RMold)
				//				{
				//					RMold.Option.CoolPin = gMacOption.RCoolPin;
				//				}
				//				else
				//				{
				//					LMold.Option.CoolPin = gMacOption.LCoolPin;
				//				}	
		
				pMold->AutoStep = 6000;
			}	
			if(40000 == pMold->CoolPin.Step) /*后冷却上超时   */
			{
				pMold->CoolPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			
			break;
		
		
		
		
         
		case 6000:
			//			gCycleTimes++;
			//			
			//        	 gMachineInfo.CycleTimes = gCycleTimes;
			//			gMachineInfo.Yield = gMachineInfo.CycleTimes * gMachinePara.DieheadNum;	
			//			if((gMachinePara.StopYield > 0) && (0 == gMachinePara.bReachYieldStop)) 
			//			{	
			//			if(gMachineInfo.Yield >= gMachinePara.StopYield)
			//			{
			//				gAlarm.CycleTimeCounReach =1;			
			//			}	
			//			}
			//
			//			if(pMold == &LMold)
			//			{
			//			gCycleTimesL++;
			//			}
			//			
			//			if(pMold == &RMold)
			//			{
			//				gCycleTimesR++;
			//			}
			pMold->AutoStep = 1000;				/*  下一过程的等待  */
			
			//clamp
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType )	/*  電動關模加壓超时 */
			{
				if( pMold->Clamp.Step > 3400 && pMold->Clamp.Step < 3800)
				{
					pMold->Clamp.Step = 3800;
				}
			
				if(40000 == pMold->Clamp.Step)     // 吹氣完成後確認電動關模加壓超時
				{
					pMold->Clamp.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if(40000 == RMold.ExtrLift.Step)     // 吹氣完成後確認抬頭偵測時間超時
				{
					RMold.ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				if(40000 == pMold->ExtrLift.Step)     // 吹氣完成後確認抬頭偵測時間超時
				{
					pMold->ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			//Blowpin
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  電動關模加壓超时 */
			{
				if( 3000 == pMold->BlowPin.Step )
				{
					pMold->BlowPin.Step = 0;
				}
				
				if(40000 == pMold->BlowPin.Step)     // 吹氣完成後確認電動吹桿2段低壓超時
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			
			//coolpin1/2/3 
			if(40000 == pMold->CoolPin.Step || 40000 == pMold->CoolPin2.Step || 40000 == pMold->CoolPin3.Step)     // 吹氣完成後轉123偵測時間超時
			{
				pMold->CoolPin.Step = 0;
				pMold->CoolPin2.Step = 0;
				pMold->CoolPin3.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			
			break;

		/*  ------------------------------ 单模运行  ------------------------------ */
		case 10000:
			pMold->Carriage.Step = 0;  /* 18000;  保持另一模回位  */
			break;

		case 11000:
			pMold->Carriage.Step = 30000;
			pMold->AutoStep = 0;
			break;

		/*  ------------------------------ 自动停止  ------------------------------ */
		case 15100:
			if(0 == gMacOption.bAutoStopPinDown)   /*  开模后吹针保持下位  */
			{
				pMold->BlowPin.Step = 10100;    		/*  开模后吹针上    */
				pMold->AutoStep     = 15200;
			}
			else
			{
				pMold->AutoStep = 15300;
			}
				
			break;

		case 15200:
			if( 13000 == pMold->BlowPin.Step)
			{
				pMold->BlowPin.Step = 0;
				pMold->AutoStep = 15300;
			}

			if(40000 == pMold->BlowPin.Step)/*  吹针上超时  */
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 15300:
			pMold->Robot.Step = 10100;		   /*  机械手退松  */
			if(pMold == &RMold)
			{
				gMachineOut.LeftStation = 1;
			}
			else
			{
				gMachineOut.LeftStation = 0;
			}
			pMold->AutoStep   = 15400;
			break;

		case 15400:
			if(13000 == pMold->Robot.Step)
			{
				pMold->Robot.Step = 0;
				pMold->AutoStep   = 15500;
			}

			if(40000 == pMold->Robot.Step) /* 機械手退+開故障   */
			{
				pMold->Robot.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		case 15450:
			pMold->AutoStep   = 15460;	
			
			break;

		case 15460:

			if((0 == pMold->CoolPin2.Step|| 3000 == pMold->CoolPin2.Step) && (0 == pMold->CoolPin3.Step|| 3000 == pMold->CoolPin3.Step))
			{
				pMold->CoolPin2.Step = 0;
				pMold->CoolPin3.Step = 0;
			
				pMold->AutoStep   = 15500;
			}


			break;

		
		case 15500:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;
			
			pMold->AutoStep = 0;

			if(0 == LMold.AutoStep && 0 == RMold.AutoStep)
			{
				pMold->AutoReqStop  = 1;
				if(1 == gAlarm.OilTempHigh || 1 == gAlarm.OilPosition || 1 == gAlarm.AirPressureLow )gPROCESS_info.extruderStopEnable = 1;
			}
			break;

		/* ------------------------------  手动按自动停止   ----------------------------- */
		case 20000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;
			pMold->AutoStep = 0;
			break;

		/* ------------------------------    自动急停      ------------------------------ */
		case 30000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;
			pMold->AutoStep = 0;
			break;

		/* ------------------------------    故障自动停    -------------------------------- */
		case 40000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;

			gPROCESS_info.extruderStopEnable = 1;

			pMold->AutoReqStop  = 1;
			pMold->AutoStep = 0;
			break;

	}/* switch(pMold->AutoStep)  */


}/*end of function */

