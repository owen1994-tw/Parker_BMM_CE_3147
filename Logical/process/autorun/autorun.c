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
							if(1 == gMachineFix.Option.bExtParisonCtrl)   /* �ⲿ�ں���� */
							{
								RMold.AutorunFirstCycle = 1;  
								LMold.AutorunFirstCycle = 0;  
								
								RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
								LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
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
						
						RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
//						LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
						
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
						
//						RMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
						LMold.Clamp.ExtInMoldInDone_Flag = 1;  // ByPass �ⲿģ���N���Ӎ̖
						
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
	/* --------------------  ��λ�Զ����� ----------------------- */

	AutoRun(&RMold,&gMachineFix.Option,&gSPC_Current.MoldR);
	AutoRun(&LMold,&gMachineFix.Option,&gSPC_Current.MoldL);
	
	/* -------------- ̧�^���� �Ԅ��D���Ԅ� ---------------- */
//	if(1 == gAlarm.RAlarm.ExtrLiftNotAtUpPos || 1 == gAlarm.LAlarm.ExtrLiftNotAtUpPos || 1 == gAlarm.RAlarm.ExtrLiftNotAtDnPos || 1 == gAlarm.LAlarm.ExtrLiftNotAtDnPos 
//		&& 1 == gMachineInfo.ExtrliftAlarmReqSemiAutoFlag)
//	{
//		gMachineInfo.ExtrliftAlarmReqSemiAutoFlag = 0 ;
//		AutoRunToSemiAuto();
//	}
		
	/* -------------- �C̨���� �Ԅ��D���Ԅ� ---------------- */
	if(1 == gAlarm.CutterHeatNormal || 1 == gAlarm.SwingInvertErr || 1 == gAlarm.OilTempHigh || 1 == gAlarm.OilPosition 
		|| 1 == gAlarm.AccuChargeError || 1 == gAlarm.AccuCharge_ExtrliftError || 
		((0 == gMacOption.bDryCycle) && (1 == bDryCycleOld)))
	{
		gPROCESS_info.extruderStopEnable = 1;
		AutoRunToSemiAuto();
	}
		
	bDryCycleOld = gMacOption.bDryCycle;
		
	/* -------------- �L������ �Ԅ��D���Ԅ� ---------------- */
	if(1 == gAlarm.AirPressureLow && 1 == gMachineInfo.AirPressureLowDlyFlag )
	{
		gMachineInfo.AirPressureLowDlyFlag  = 0;
		AutoRunToSemiAuto();
	}
	

	
	/* ---------------- �ͱ����м�����Զ�ֹͣ ---------------- */
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

	if(1 == gMachineOut.OilPump && 1 == gAlarm.OilPumpErr && gTKauto.Active > 500 ) /*  �Զ�����5s���  */
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

	/* ---------------------  ���ڼ���ʱ����  -------------------------*/

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
	
	/* ---------------------  ���gExSPC����  -------------------------*/
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
	
	pMold->ValveOut.RobotCarriageUp = 0;// Reset �ⲿ�Cе��Ӎ̖
	pMold->ValveOut.RobotCarriageDn = 0;// Reset �ⲿ�Cе��Ӎ̖
	pMold->ValveOut.RobotClampOpn = 0;// Reset �ⲿ�Cе��Ӎ̖
	pMold->ValveOut.RobotClampCls = 0;// Reset �ⲿ�Cе��Ӎ̖

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
				pMold->Cutter.Step = 100;		   /*   �е�����   */
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
					gThickIO.In.StartThick = 1;      /*   �ں�����   */
				}
				pMold->LengthCtrl.Step = 100; 	/* 	���ȿ���  */
				pMold->AutoStep = 300;
			}
			break;

		case 200:
			if(3000 == pMold->Cutter.Step)		/*   �е����   */
			{
				pMold->Cutter.Step =0;
				pMold->AutoStep = 300;
			}
			break;

		case 300:
			gThickIO.Out.ControlEnd = 1;
			pMold->AutoStep = 3050;          	
			break;

		case 400:
       	
			break;
      
		/* ----------- second station start --------------- */	
		case 900:
			/*pMold->Carriage.Step = 18000;*/	/*  ������һģ��λ  */
			if(pMold == &LMold)
			{
				if(0 == RMold.BlowPin.Step && 0 == RMold.BlowPin2.Step )		/*  �����½���  */
				{
					if(1 == pMold->NextCycStart)
					{	
						gMachineOut.LeftStation = 1;
						pMold->NextCycStart = 0;
						pMold->AutoStep = 3000;    /*  ȥ�ϳ�����ѡ���֧  */
					}
					else
					{
						if(1 == pMold->SemiAuto)	/*  �Զ�ֹͣ */
						{
							pMold->AutoStep = 0;
						}
					}
				}
			}
		
			if(pMold == &RMold)
			{
				if(0 == LMold.BlowPin.Step && 0 == LMold.BlowPin2.Step)/*  �����½���  */
				{
					if(1 == pMold->NextCycStart)
					{
						gMachineOut.LeftStation = 0;
						pMold->NextCycStart = 0;
						pMold->AutoStep = 3000;    /*  ȥ�ϳ�����ѡ���֧  */
					}
					else
					{
						if(1 == pMold->SemiAuto)	/*  �Զ�ֹͣ */
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
		
			/*pMold->Carriage.Step = 18000;*/		/*  ������һģ��λ  */											
			if(1 == pMold->NextCycStart)		/*  �ȴ�״̬  */	
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
			//				pMold->BlowPin.Step = 15300;		/* ����΢��  	*/
			//			}
			//			
			//			if(pMold->Option.BlowPin2)
			//			{
			//				pMold->BlowPin2.Step = 15300;		/* ����΢��  	*/
			//			}
			
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType )	/*  늄��Pģ�Ӊ���ʱ */
			{
				if( 3900 == pMold->Clamp.Step)
				{
					pMold->Clamp.Step = 0;
				}
			}
			pMold->BlowPin.Step = 15300;		/* ����΢��  	*/
			pMold->BlowPin2.Step = 15300;		/* ����΢��  	*/
			
			pMold->AutoStep = 1300;
			break;

		case 1300:			
			
			//			if(18000 == pMold->BlowPin.Step)	/* ����΢�����  */
			//			{
			//				pMold->BlowPin.Step = 0;
			//				if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//				{
			//					pMold->ValveOut.BlowPinDnHold =1;
			//				}
			//				pMold->AutoStep = 1400;
			//				pMold->PinPart.Step = 10100;		/*  �������   	*/  
			//			}
			
			//			if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
			//			{
			//				if(18000 == pMold->BlowPin.Step)	/* ����΢�����  */
			//				{
			//					pMold->BlowPin.Step = 0;
			//					if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//					{
			//						pMold->ValveOut.BlowPinDnHold =1;
			//					}
			//					pMold->AutoStep = 1400;
			//					pMold->PinPart.Step = 10100;		/*  �������   	*/  
			//				}
			//
			//			}
			//			else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			//				if(18000 == pMold->BlowPin2.Step)	/* ����΢�����  */
			//				{
			//					pMold->BlowPin2.Step = 0;
			//					if(1 == pMold->Option.BottomSubMold)   // ipis1112
			//					{
			//						pMold->ValveOut.BlowPinDnHold =1;
			//					}
			//					pMold->AutoStep = 1400;
			//					pMold->PinPart.Step = 10100;		/*  �������   	*/  
			//				}
			//
			//			}
			//			else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			if(18000 == pMold->BlowPin.Step && 18000 == pMold->BlowPin2.Step)	/* ����΢�����  */
			{
				pMold->BlowPin.Step = 0;
				pMold->BlowPin2.Step = 0;
				if(1 == pMold->Option.BottomSubMold)   // ipis1112
				{
					pMold->ValveOut.BlowPinDnHold =1;
				}
				pMold->AutoStep = 1400;
				pMold->PinPart.Step = 10100;		/*  �������   	*/  
			}

			//			}
			
			
			break;

		case 1400:
			if(13000 == pMold->PinPart.Step)	/*  ������˽��� */
			{
				pMold->PinPart.Step = 18000;
				pMold->AutoStep = 1900;
			}

			if(40000 == pMold->PinPart.Step )	/*  ������˳�ʱ */
			{
				pMold->PinPart.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		/*---------------no use--------------------------------------------------------------*/    
		/*  ����΢��ģ -- ���� -- ��ģ΢�� -- */
		case 1500:
			pMold->Clamp.Step = 11100;		  	/*  ����΢��ģ */
			pMold->AutoStep = 1600;
			break;

		case 1600:
			if(13000 == pMold->Clamp.Step)	/*  ����΢��ģ����  */
			{
				pMold->Clamp.Step = 0;
				pMold->AutoStep   = 1700;
			}

			if(40000 == pMold->Clamp.Step)  	/*  ����΢��ģ��ʱ   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 1700:	
			pMold->Screw.Step = 10100;			/* ������    	*/
			pMold->AutoStep   = 1800;
			break;

		case 1800:
			if(13000 == pMold->Screw.Step)	/*  �����˽���  */
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

			if(40000 == pMold->Screw.Step)  	/*  �����˳�ʱ   */
			{
				pMold->Screw.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		/*-----------------------------------------------------------------------------*/ 
		
		case 1900:
			if(3000 == pMold->Robot.Step && 3000 == pMold->RobotOpnCls.Step ) /*  �Cе���M+�P�Y��  */
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
			
			
			if(40000 == pMold->Robot.Step)	/* �Cе���M����   */
			{
				pMold->Robot.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->RobotOpnCls.Step)/* �Cе���P����   */
			{
				pMold->RobotOpnCls.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}

			break;

		
		case 2000:
			if ( 1 == pMold->TransDIn.TopDeflashBwLimit && 1 == pMold->TransDIn.BottomDeflashBwLimit )  // ipis wait all bw
			{
				pMold->Clamp.Step = 12100;		  	/*  �_ģ���� + ΢�_ģ */
				pMold->AutoStep = 2050;
			}		
			break;
				
		case 2050:

			if(13000 == pMold->Clamp.Step)		/*  �_ģ��������  */
			{
				pMold->AutoStep = 2100;
			}	
						
			if(40000 == pMold->Clamp.Step)  	/*  �_ģ������ʱ	*/
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2100:
	
//			pMold->SubMold.Step 	  = 10100;  /*  Topsubmold open        */
//			pMold->BottomSubMold.Step = 10100;	/*  Bottomsubmold open     */
			pMold->Clamp.Step 		  = 10100;	/*  Clamp open */
			
			pMold->AutoStep = 2200; 

			break;

		case 2200:
			if(13000 == pMold->SubMold.Step && 13000 == pMold->BottomSubMold.Step )	/*  ��ģ������  */
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step =0;
				pMold->AutoStep = 2300;
			}

			if(40000 == pMold->SubMold.Step)  /*  ��ģ����ʱ  */
			{
				pMold->SubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->BottomSubMold.Step)  /*  ����ģ����ʱ  */
			{
				pMold->BottomSubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Clamp.Step)  	/*  �_ģ��ʱ		*/
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;

		case 2300:
			if (pMold->Clamp.mmAs > pMold->Clamp.S_SubMoldIn )  //ipis1113
			{
				pMold->SubMold.Step  = 100;  		/*  ��ģ�ر� ������ģ��ʱ������ȹر���ģ���Լ���*/ //ipis1113
				pMold->BottomSubMold.Step  = 100;  	
				pMold->AutoStep = 2400; //ipis1113
			
			}
			if(40000 == pMold->Clamp.Step)  	/*  ��ģ��ʱ   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
		
			break;
		
		
		case 2400:   /***Cycle End***/
			if(13000 == pMold->Clamp.Step )	/*  ��ģ����  */
			{
				pMold->Clamp.Step = 0;
				
				if (1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotClampOpn = 1;	/*  �ⲿ�Cе��  */
				}
				pMold->Robot.Step = 10100;		/*  �Cе����+�_  */
				pMold->HookPin.Step = 10100;		/*  �카�����  */
			
				pMold->AutoStep   = 2450;
			}
		
			if(40000 == pMold->Clamp.Step)  	/*  ��ģ��ʱ   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2450: //top submold cls must check before carriage fw  ipis1112
			if( 3000 == pMold->SubMold.Step && 3000 == pMold->BottomSubMold.Step )		/*  ��ģ�ؽ���  */
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step = 0;
				pMold->AutoStep   = 2500;
			}
			
			if(40000 == pMold->SubMold.Step)  	/*  ��ģ�س�ʱ   */
			{
				pMold->SubMold.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		case 2500:
			if(1 == pMold->Option.PinOpenUp)	/*  ��ģ�����ϣ�ģ��������,����Ŀ���� */
			{
				pMold->BlowPin.Step = 10100;
				pMold->AutoStep     = 2600;
			}
			else
			{
				pMold->AutoStep = 2650;
			}
			break;

		case 2600:				/* �����Ϲ��� */
			if( 13000 == pMold->BlowPin.Step)
			{
				pSPC->tBlowPinUp = pMold->TimeDis.BlowPinUpAlarmTime/100.0;
				pMold->BlowPin.Step = 0;
				pMold->AutoStep = 2650;
			}

			if(40000 == pMold->BlowPin.Step)/*  �����ϳ�ʱ  */
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 2650:	
			if(1 == pMold->SemiAuto)		/*  �Զ�ֹͣ */						
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
				
				if(40000 == pMold->Robot.Step)	/* �Cе���˹���   */
				{
					pMold->Robot.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->RobotOpnCls.Step)/* �Cе���_����   */
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
			if(1 == pMold->Clamp.OpnPos)		/* ��ģ�ж�    */
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
			
			pMold->InnerPin.Step = 30000;  		/*  ģ�ڴ��븴λ */
			pMold->Sealer.Step   = 30000;    	/*  ������׸�λ */
			pMold->Screw.Step    = 100;			/*  ������       */
			pMold->Carriage.Step = 100;         /*  ��������   	 */
			pMold->AutoStep = 3200;
			
			pMold->ValveOut.RobotCarriageDn = 0;// Reset �ⲿ�Cе��Ӎ̖

			break;

		case 3200:								      /*  ���Ϲ���   */
			if(3000 == pMold->Carriage.Step)
			{
				if (1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotCarriageUp = 1;
				}
			
				if(pMold->Robot.Step > 10100 && pMold->Robot.Step < 10300) // ���ƙCе���˽Y�� (�_������)
				{
					pMold->Alarm.RobotBwTimeout  = 1;
					pMold->RobotOpnCls.RobotBwTimeOutFlag = 1;	/*  �Cе���_ force 	*/
					pMold->Robot.Step  = 10300;					/*  �Cе���� force 	*/
					pMold->Robot.Timer.IN = 0;	
				}
//				else if ( 13000 == pMold->Robot.Step && (pMold->RobotOpnCls.Step > 10100 &&  pMold->RobotOpnCls.Step < 10300 ))
//				{
//					pMold->Alarm.RobotOpnTimeout  = 1;
//					pMold->RobotOpnCls.Step = 10300; /*  �Cе���_ force 	*/
//					pMold->RobotOpnCls.Timer.IN = 0;
//				}
				

				pMold->Timer.IN = 1;
				pMold->Timer.PT = 2000;
			
				pMold->Carriage.Step = 0; /*8000; ���ּ���λ */
//				pMold->AutoStep = 3300;
				pMold->AutoStep = 3250;
			}

			if(40000 == pMold->Carriage.Step)   /*  ���ϳ�ʱ */
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
				if(1 == pMold->Clamp.ExtInMoldInDone_Flag )		/*  �ⲿģ���N  �˻�ԭ�c���  */
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
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)   /* �ⲿ�ں���� */
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
				if(1 == gThickIO.Out.ControlEnd)	/*  �ں�ʱ�䳬ʱ */
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
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)   /* �ⲿ�ں���� */
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
			
				if(gMacTimer.ExtParison.PT != 0)	/*  �ں�ʱ�����÷���ʱ���� */
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
				if(gThickProf.TimSet.CycleTime != 0)	/*  �ں�ʱ�����÷���ʱ���� */
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
			pMold->TimeDis.CycleAlarmTime = pMold->TimeCycle.ET;  /*  ����ʱ���ռ�  */
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
			
			// �pģ����ExIndex�l��  //��ģ����ExIndex�l��        
			if(pMold == & LMold  || (pMold == & RMold && LMold.AutoStep == 10000) )
			{
				if (ExIndex< 19)		ExIndex = ExIndex+1;
				else  ExIndex = 0;	
			}
			
			
			break;
		
		case 3700:
			//			if(3000 == pMold->Screw.Step)		/*  ����������  */
			//			{
			//				pMold->Screw.Step = 0;
			//		pMold->AutoStep   = 3800;   // ipis1112
			pMold->AutoStep   = 3800;
			//			}

			//			if(40000 == pMold->Screw.Step)  	/*  ��������ʱ   */
			//			{
			//				pMold->Screw.Step = 0;
			//				pMold->StopAutoStep = pMold->AutoStep;
			//				pMold->AutoStep = 40000;
			//			}
			break;
		
		case 3800:
			pMold->Sealer.Step   = 100;			/*   ���       */
			pMold->Clamp.Step    = 100;         /*   ��ģ       */
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				RMold.ExtrLift.Step = 100;			/*   ̧ͷ       */
			}
			else
			{
				pMold->ExtrLift.Step = 100;			/*   ̧ͷ       */
			}
			pMold->InnerPin.Step = 100;			/*  ģ��С����  */
			pMold->SupportSuckAir.Step = 100;   /*  ģ�^�����  */
			pMold->Cutter.Step = 100;			/*   �е�����   */
			
			if( SUPPORTAIR_MODE_AFTER_CLAMP_CLS == gMachineFix.MoldR.SupportAir.eSupportAirMode) // Û�L���ӣ����Pģ�醢�ӗl����
			{
				pMold->SupportAir.Step = 100;// air support
			}
			
			if(1 == gMachineFix.Option.bExtParisonCtrl)
			{
				gMachineInfo.ExtParisonCtrlStart = 1; /*   �ⲿ�ں�����   */
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
				gThickIO.In.StartThick = 1;      /*   �ں�����   */
				pMold->LengthCtrl.Start = 1; 	 /*   ���ȿ���   */
				//	RMold.SupportAir.Step = 100;/*Albert Mold Pre-Blow*/
				pMold->SupportAir.cntTime = gThickProf.TimSet.CycleTime;/*Albert*/
			}
			pMold->ValveOut.RobotClampOpn = 0; // Reset �ⲿ�Cе��Ӎ̖
			
			pMold->OnceBlowPinUp = 1;    /*�U�´Δ�Ӌ��*/ 
			
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

		case 3900:   /*   ��ģ����   */
			if ( 1 == pOptionFix->bBlowUpDuringClampCls)
			{
				if(pMold->Clamp.mmAs < pMold->Clamp.S_BlowPinUp)	/*  ��ģ�д�����    */
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
						if(0 == pMold->Option.PinOpenUp)	/*  ��ģ������    */
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
			
				if ( 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  ���һѭ�h�Pģ���Dһ��    */
				{
					pMold->CoolPin.Step = 500; /*force stop*/
				}
			}

			if(40000 == pMold->Clamp.Step)      /*  ��ģ��ʱ   */
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
 
		case 3950:   /* �Pģ����   &&  �����Ϲ���  &&  (���һѭ�h�Pģ���Dһ�� && �Dһ�ϳ��r)  */
			
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
			
			if(40000 == pMold->Clamp.Step)      /*  ��ģ��ʱ   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->Clamp.Step)      /*  ��ģ��ʱ   */
			{
				pMold->Clamp.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if ( 40000 == pMold->CoolPin.Step && 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  ���һѭ�h�Pģ���Dһ�� && �Dһ�ϳ��r   */  
			{
				pMold->CoolPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
		
			break;
		
		case 4000:

			pMold->AutoStep = 4100;
			
			break;

		case 4100:								      /*  �����Ϲ���  */
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
			


				if(40000 == pMold->BlowPin.Step)/*  �����ϳ�ʱ  */
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->BlowPin2.Step)/*  �����ϳ�ʱ  */
				{
					pMold->BlowPin2.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->Clamp.Step)      /*  ��ģ��ʱ   */
				{
					pMold->Clamp.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if ( 40000 == pMold->CoolPin.Step && 1 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  ���һѭ�h�Pģ���Dһ�� && �Dһ�ϳ��r   */ 
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
				if( 3900 == pMold->Clamp.Step )  /*  �Pģ���ֽY��   */
				{
					pMold->Clamp.Step = 0;
				
					if(1 == pOptionFix->bPlateFwAfterClampHold)
					{
						pMold->Plate.Step = 6500;  // for Plate Fw in auto mode (timer)
					}
				
//					gMachineInfo.OilAccuChargeAuto_Extrlift = 1;  // for  Extrlift accu charge in auto mode
					pMold->AutoStep = 4170;		
				}
				
				if(40000 == pMold->Clamp.Step)      /*  ��ģ��ʱ   */
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
				if( 3000 == pMold->CoolPin.Step )  	/*  �Dһ�ϽY��   */
				{
					pMold->CoolPin.Step = 0;
					pMold->AutoStep = 4180;		
				}
			
				if ( 40000 == pMold->CoolPin.Step )    /*  �Dһ�ϳ��r   */ 
				{
					pMold->CoolPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				pMold->AutoStep = 4180;	
			}
			
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if(40000 == RMold.ExtrLift.Step)     // ����ǰ�_�J̧�^�ɜy�r�g���r
				{
					RMold.ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				if(40000 == pMold->ExtrLift.Step)     // ����ǰ�_�J̧�^�ɜy�r�g���r
				{
					pMold->ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			break;
		
		case 4180:  
			// ����ǰ�_�J�카�����λ
			if( 1 == pMold->TransDIn.HookPinBwLimit )
			{	
				if (13000 == pMold->HookPin.Step || 0 == pMold->HookPin.Step )  /*  �카����˽Y��  */
				{
					pMold->HookPin.Step = 0;
					pMold->AutoStep = 4200;	
				}
			}
			else
			{	
				pMold->Alarm.HookPinNotAtBwPos = 1;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			if(40000 == pMold->HookPin.Step)   /*  �카����˳��r */
			{
				pMold->HookPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;
		
		
		case 4200:  
			// ����ǰ�_�J�Cе����λ(�]�д_�J�_)
			if( 1 == pMold->Robot.Transfer.BwPos &&  1 == pMold->TransDIn.ExtRobotUpLmit)
			{	
				if (( pMold->Robot.Step > 11000 && pMold->Robot.Step <= 13000) || 0 == pMold->Robot.Step )  /*  �Cе���˽Y��  */
				{
					pMold->Robot.Step = 0;
					
					pMold->Carriage.Step = 10100;	/*  ������   */
					pMold->ValveOut.RobotCarriageUp = 0;  // Reset �ⲿ�Cе��Ӎ̖
					pMold->PinPart.Step  = 100;		/*  ������   */
					pMold->AutoStep = 4300;	
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
			
			if(40000 == pMold->Robot.Step)   /*  �Cе���˳��r */
			{
				pMold->Robot.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			break;

		case 4300:								      /*  ���¹���  */
			if((13000 == pMold->Carriage.Step) )
			{
				pMold->CurCycEnd = 1;				 /*����һģ�ʂ�*/
				pMold->Carriage.Step = 0; /*18000;	 ���ּ���λ */
			
				if(1 == pMold->Option.ExtRobot)
				{
					pMold->ValveOut.RobotCarriageDn = 1;
				}
			
				pMold->AutoStep = 4400;	          
			}
			
			if(40000 == pMold->Carriage.Step)   /*  ���³�ʱ */
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

			if(40000 == pMold->PinPart.Step)   /*  ��������ʱ */
			{
				pMold->PinPart.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			} 
			break;
		
		case 4410:
			pMold->HookPin.Step = 100;			/*  �카����M  */
			pMold->AutoStep = 4420; 

			break;
		
		case 4420:
			if(3000 == pMold->HookPin.Step)   
			{
				pMold->HookPin.Step = 0;
				pMold->AutoStep = 4500; 
			}

			if(40000 == pMold->HookPin.Step)   /*  �카����ʱ */
			{
				pMold->HookPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;
		
		case 4500:	

			//			if( 1 == pMold->Option.BlowPin && 0 == pMold->Option.BlowPin2  )
			//			{
			//				pMold->BlowPin.Step = 100;			/*  ������  	 */
			//			}
			//			else if( 0 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			//				pMold->BlowPin2.Step = 100;			/*  ������  	 */
			//			}
			//			else if( 1 == pMold->Option.BlowPin && 1 == pMold->Option.BlowPin2  )
			//			{
			/*************/
			pMold->BlowPin.Step = 100;			/*  ������  	 */
			pMold->BlowPin2.Step = 100;			/*  ������  	 */
			//			}
			//			pMold->BlowPin.Step = 100;			/*  ������  	 */
			/*************/
			pMold->LowBlow.Step = 100;			/*  ����1����   */
			pMold->HighBlow.Step = 100;			/*  ����2����   */
			pMold->BlowDeflash.Step = 100;		/*  ���U����    */
			/*************/
//			pMold->CoolPin2.Step = 100;         /*  �D����		 */
//			pMold->CoolPin2Blow.Step =100;      /*  �D���´���  */
//			pMold->CoolPin3.Step = 100;	       	/*  �D����		 */
//			pMold->CoolPin3Blow.Step =100;    	/*  �D���´���  */
			//			pMold->CoolDeflash.Step = 100;		/*  ���ְѴ���     */
			
			
			pMold->AutoStep = 4510;
			if( 0 == pOptionFix->bRobotFwAfterTopdeflash)
			{
//				pMold->Robot.Step = 100;			/*  �Cе���M+�P  */
				if (  pMold->RobotOpnCls.Step >= 10100  &&  pMold->RobotOpnCls.Step < 13000) // Robot Opn too slow
				{
					pMold->Alarm.RobotOpnTimeout  = 1;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
				else
				{
					pMold->Robot.Step = 100;			/*  �Cе���M+�P  */
				}
			}
		
			
			break;

		case 4510:

			if (0 == pOptionFix->bTopDeflashMode ||  1 == pOptionFix->bCoolPinUpNextCycClampCls) // topdeflash before/after cooling   /*���ʹ�����һѭ�h�Pģ���Dһ�ϣ��Ͳ������Dһ�ִ��ְ�*/
			{
				if(1 == gMacOption.ACCPumpCharge_Extrlift)
				{
					pMold->TopDeflash.Step     = 90;	/*  ���ְ�(��������~�����t�r�g)    */  
					pMold->BottomDeflash.Step  = 90;	/*  ��ƿ��(��������~�����t�r�g)    */ 
					pMold->PullBottom.Step  = 100; 		/*  ��������  */
				}
				else
				{
					pMold->TopDeflash.Step    = 100;	/*  ���ְ�    */
					pMold->BottomDeflash.Step = 100;	/*  ��ƿ��    */
					pMold->PullBottom.Step  = 100; 		/*  ��������  */
				}
				
				pMold->AutoStep = 4550;
			}
			else
			{
				pMold->CoolPin.Step     = 100;		/*  ����ȴ      */
				pMold->CoolPinBlow.Step = 100;		/*  ����ȴ����  */
				pMold->AutoStep = 4580;
			}
			
		
			break;
		

		case 4550:
						
			/* ���ְ���� -> �Cе���M+�P  */
			if(13000 == pMold->TopDeflash.Step)
			{
				pMold->TopDeflash.Step = 0;
				if( 1 == pOptionFix->bRobotFwAfterTopdeflash)
				{
					pMold->Robot.Step = 100;			/*  �Cе���M+�P  */
					if ( pMold->RobotOpnCls.Step >= 10100  &&  pMold->RobotOpnCls.Step < 13000) // Robot Opn too slow
					{
						pMold->Alarm.RobotOpnTimeout  = 1;
					}
				}
				pMold->AutoStep = 4560;
			}
			
			if( 40000 == pMold->TopDeflash.Step  || 40000 == pMold->BottomDeflash.Step )	/* ���ְ�+������ / ��ƿ�� ���r  */
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
				/*--------- ���ְ�+������-----��ƿ��-----��ƿ��---------- */
				/*  ���ְ�+������  */
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
				
				/*  ��ƿ��  */
				if( pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 2100 )	// BottomDeflash Fw Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
					pMold->Alarm.BottomDeflashTimeout = 1;
				}
				else if( pMold->BottomDeflash.Step > 10100 && pMold->BottomDeflash.Step < 10300 )	// BottomDeflash Fw Done, but doing BottomDeflash BwDelay Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
				}
			
				/*  ��ƿ��  */
				if(pMold->PullBottom.Step > 100 && pMold->PullBottom.Step < 500)
				{
					pMold->Alarm.PullBottomTimeOut = 1;
					pMold->PullBottom.Step = 500;   //  Force PullBottom 
				}
				/*--------- ���ְ�+������-----��ƿ��-----��ƿ��---------- */
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

			pMold->CoolPin.Step     = 100;		/*  ����ȴ      */
			pMold->CoolPinBlow.Step = 100;		/*  ����ȴ����  */
		

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
			if(3000 == pMold->HighBlow.Step )/*  ��������  */
			{
				if(pMold->BlowDeflash.Step < 500 && pMold->BlowDeflash.Step > 0)pMold->BlowDeflash.Step = 500;
				
				pMold->Alarm.TopDeflashTimeout = 1;
			
				pMold->Lock.Step  = 30000;	/* ��ģ����  */
				pMold->Clamp.Step = 0;
				//			pMold->LowBlow.Step = 0;
				//			pMold->HighBlow.Step = 0;
				pMold->CoolPin.Timer.IN = 0;
				pMold->CoolPin2.Timer.IN = 0;
				pMold->CoolPin3.Timer.IN = 0;
			
				if ( 0 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  ��ʹ��"���һѭ�h�Pģ���Dһ��"    */
				{
					pMold->CoolPin.Step     = 500;		/*  ����ȴ   force stop   */
				}
				pMold->CoolPinBlow.Step = 900;		/*  ����ȴ���� force stop */
			
				pMold->CoolPin2.Step     = 500;		/*  ����ȴ   force stop   */
				pMold->CoolPin2Blow.Step = 900;		/*  ����ȴ���� force stop */
			
				pMold->CoolPin3.Step     = 500;		/*  ����ȴ   force stop   */
				pMold->CoolPin3Blow.Step = 900;		/*  ����ȴ���� force stop */
			
			
				pMold->InnerPin.Step = 10100;	   /* ģ�ڴ�����    */
				//			pMold->Lock.Step    = 10100;		/*  жѹ         */
			
				//	pMold->BlowPin.Step = 0;         /* ���뱣���½���*/

			
	
	
				pMold->AutoStep  = 4600;
			
			}
			/*---------------------------------------------------------------------------*/	
		

			break;
		
		case 4590:
					
			if(3000 == pMold->CoolPin.Step)
			{  
				pMold->TopDeflash.Step  = 100;		/*  ����ɱ�    */
				pMold->AutoStep = 4550;
			}			
			if(40000 == pMold->CoolPin.Step) /*����ȴ�ϳ�ʱ   */
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
				if(3000 == pMold->BlowPin.Step && 3000 == pMold->BlowPin2.Step  && 1== pMold->TransDIn.TopDeflashBwLimit ) /*  �����½��� */	
				{
					pMold->BlowPin.Step = 0;
					pMold->BlowPin2.Step = 0;
					
					pMold->AutoStep = 4700;
				}

				if(40000 == pMold->BlowPin.Step )	/*  �����³�ʱ */
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			
				if(40000 == pMold->BlowPin2.Step )	/*  �����³�ʱ */
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
				/*--------- ���ְ�+������-----��ƿ��-----��ƿ��---------- */
				/*  ���ְ�+������  */
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
				
				/*  ��ƿ��  */
				if( pMold->BottomDeflash.Step > 100 && pMold->BottomDeflash.Step < 2100 )	// BottomDeflash Fw Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
					pMold->Alarm.BottomDeflashTimeout = 1;
				}
				else if( pMold->BottomDeflash.Step > 10100 && pMold->BottomDeflash.Step < 10300 )	// BottomDeflash Fw Done, but doing BottomDeflash BwDelay Not Done 
				{
					pMold->BottomDeflash.AutoTimeOutFlag = 1;	// BottomDeflash Pass Delay
				}
			
				/*  ��ƿ��  */
				if(pMold->PullBottom.Step > 100 && pMold->PullBottom.Step < 500)
				{
					pMold->Alarm.PullBottomTimeOut = 1;
					pMold->PullBottom.Step = 500;   //  Force PullBottom 
				}
				/*--------- ���ְ�+������-----��ƿ��-----��ƿ��---------- */

				if (pMold->CoolPinBlow.Step!=3000)
				{
					pMold->Alarm.TopDeflashTimeout = 1;	
				}
			
				pMold->BlowDeflash.Step = 500;		/*  �Dһ��   	force stop	*/
				
				if ( 0 == pOptionFix->bCoolPinUpNextCycClampCls)    /*  ��ʹ��"���һѭ�h�Pģ���Dһ��"    */
				{
					pMold->CoolPin.Step     = 500;		/*  ����ȴ   force stop   */
				}
				pMold->CoolPinBlow.Step = 900;		/*  �Dһ�´��� force stop  */
			
				pMold->CoolPin2.Step     = 500;		/*  �D����   	force stop	*/
				pMold->CoolPin2Blow.Step = 900;		/*  �D���´��� force stop  */
			
				pMold->CoolPin3.Step     = 500;		/*  �D����   	force stop	*/
				pMold->CoolPin3Blow.Step = 900;		/*  �D���´��� force stop  */
			
				if( pMold->Robot.Step > 100 && pMold->Robot.Step < 300)
				{
					pMold->Alarm.RobotFwTimeout  = 1;
					pMold->RobotOpnCls.RobotFwTimeOutFlag = 1;	/*  �Cе���P force 	*/
					pMold->Robot.Step = 300;     		/*  �Cе���M force 	*/
					pMold->Robot.Timer.IN = 0;
				}
				else if ( 3000 == pMold->Robot.Step && (pMold->RobotOpnCls.Step > 100 &&  pMold->RobotOpnCls.Step < 300 ))
				{
					pMold->Alarm.RobotClsTimeout  = 1;
					pMold->RobotOpnCls.Step = 300; /*  �Cе���P force 	*/
					pMold->RobotOpnCls.Timer.IN = 0;
				}
			
				
				pMold->InnerPin.Step = 10100;	   /* ģ�ڴ�����    */
				//			pMold->Lock.Step    = 10100;		/*  жѹ         */
			
				//	pMold->AutoStep  = 4800;
			
				// Elec Blowpin
				if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  늄Ӵ��U2�ε͉�S��ʱ */
				{
					if( pMold->BlowPin.Step > 1300 && pMold->BlowPin.Step < 1500)
					{
						pMold->BlowPin.Step = 1500;
					}
			
					if(40000 == pMold->BlowPin.Step)     // ���������_�J늄Ӵ��U2�ε͉����r
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
//					if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  늄��Pģ�Ӊ���ʱ */
//					{
//						if( pMold->BlowPin.Step > 1300 && pMold->BlowPin.Step < 1500)
//						{
//							pMold->BlowPin.Step = 1500;
//						}
//			
//						if(40000 == pMold->BlowPin.Step)     // ���������_�J늄��Pģ�Ӊ����r
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
	
			if( (0 == pMold->TopDeflash.Step || 13000 == pMold->TopDeflash.Step )  && 13000 == pMold->BottomDeflash.Step )	/*  ���ְ�+������ && ��ƿ�� ����  */
			{    			
				pMold->TopDeflash.Step = 0;
				pMold->BottomDeflash.Step = 0;
			
				//Reset AutoTimeOutFlag
				//���ְ�+������
				pMold->TopDeflash.AutoTimeOutFlag = 0;  	// TopDeflash Pass Delay
				pMold->TopDeflashOpnCls.AutoTimeOutFlag = 0;// TopDeflashOpnCls Pass Delay
				//��ƿ��+�Dһ��+�Dһ�P
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
			//			if(40000 == pMold->CoolPin.Step) /*����ȴ�ϳ�ʱ   */
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
		//				if(40000 == pMold->CoolPin.Step) /*����ȴ�ϳ�ʱ   */
		//				{
		//					pMold->CoolPin.Step = 0;
		//					pMold->StopAutoStep = pMold->AutoStep;
		//					pMold->AutoStep = 40000;
		//				}
		//			}
		//			break;
		
		case 4850:
			//			pMold->TopDeflash.Step  = 100;		/*  ����ɱ�    */
			//			pMold->BottomDeflash.Step = 100;	/*  �״�ɱ�       */
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
			//				if(40000 == pMold->CoolPin2.Step) /*����ȴ�ϳ�ʱ   */
			//				{
			//					pMold->CoolPin2.Step = 0;
			//					pMold->StopAutoStep = pMold->AutoStep;
			//					pMold->AutoStep = 40000;
			//				}
			//			}
			
			break;
		
		
		case 5100:
			
			pMold->CoolPin.Step     = 100;		/*  ����ȴ      */
			pMold->CoolPinBlow.Step = 100;		/*  ����ȴ����  */
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
			if(40000 == pMold->CoolPin.Step) /*����ȴ�ϳ�ʱ   */
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
			pMold->AutoStep = 1000;				/*  ��һ���̵ĵȴ�  */
			
			//clamp
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType )	/*  늄��Pģ�Ӊ���ʱ */
			{
				if( pMold->Clamp.Step > 3400 && pMold->Clamp.Step < 3800)
				{
					pMold->Clamp.Step = 3800;
				}
			
				if(40000 == pMold->Clamp.Step)     // ���������_�J늄��Pģ�Ӊ����r
				{
					pMold->Clamp.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if(40000 == RMold.ExtrLift.Step)     // ���������_�J̧�^�ɜy�r�g���r
				{
					RMold.ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			else
			{
				if(40000 == pMold->ExtrLift.Step)     // ���������_�J̧�^�ɜy�r�g���r
				{
					pMold->ExtrLift.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			//Blowpin
			if( ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType )	/*  늄��Pģ�Ӊ���ʱ */
			{
				if( 3000 == pMold->BlowPin.Step )
				{
					pMold->BlowPin.Step = 0;
				}
				
				if(40000 == pMold->BlowPin.Step)     // ���������_�J늄Ӵ��U2�ε͉����r
				{
					pMold->BlowPin.Step = 0;
					pMold->StopAutoStep = pMold->AutoStep;
					pMold->AutoStep = 40000;
				}
			}
			
			
			//coolpin1/2/3 
			if(40000 == pMold->CoolPin.Step || 40000 == pMold->CoolPin2.Step || 40000 == pMold->CoolPin3.Step)     // ����������D123�ɜy�r�g���r
			{
				pMold->CoolPin.Step = 0;
				pMold->CoolPin2.Step = 0;
				pMold->CoolPin3.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			
			
			break;

		/*  ------------------------------ ��ģ����  ------------------------------ */
		case 10000:
			pMold->Carriage.Step = 0;  /* 18000;  ������һģ��λ  */
			break;

		case 11000:
			pMold->Carriage.Step = 30000;
			pMold->AutoStep = 0;
			break;

		/*  ------------------------------ �Զ�ֹͣ  ------------------------------ */
		case 15100:
			if(0 == gMacOption.bAutoStopPinDown)   /*  ��ģ���뱣����λ  */
			{
				pMold->BlowPin.Step = 10100;    		/*  ��ģ������    */
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

			if(40000 == pMold->BlowPin.Step)/*  �����ϳ�ʱ  */
			{
				pMold->BlowPin.Step = 0;
				pMold->StopAutoStep = pMold->AutoStep;
				pMold->AutoStep = 40000;
			}
			break;

		case 15300:
			pMold->Robot.Step = 10100;		   /*  ��е������  */
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

			if(40000 == pMold->Robot.Step) /* �Cе����+�_����   */
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

		/* ------------------------------  �ֶ����Զ�ֹͣ   ----------------------------- */
		case 20000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;
			pMold->AutoStep = 0;
			break;

		/* ------------------------------    �Զ���ͣ      ------------------------------ */
		case 30000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;
			pMold->AutoStep = 0;
			break;

		/* ------------------------------    �����Զ�ͣ    -------------------------------- */
		case 40000:
			pMold->Timer.IN     = 0;
			pMold->TimeCycle.IN = 0;

			gPROCESS_info.extruderStopEnable = 1;

			pMold->AutoReqStop  = 1;
			pMold->AutoStep = 0;
			break;

	}/* switch(pMold->AutoStep)  */


}/*end of function */

