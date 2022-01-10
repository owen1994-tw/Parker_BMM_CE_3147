/**********************************************************************************
 * COPYRIGHT --  B&R China
 **********************************************************************************
 * Program: parison
 * File: parison.c
 * Author: lijianjun
 * Created: March 06, 2013
 **********************************************************************************
 * Implementation of program parison --- thickness controller
 *********************************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <string.h>

#include "defines.h"
#include "thick.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"
/**********************************************************************************
**                            Function Declarations								      **
**********************************************************************************/
/**********************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				         **
**********************************************************************************/
_INIT    void init()
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
   
	TimePid.e0          = 0;
	TimePid.e1          = 0;
	TimePid.e2          = 0;
	TimePid.OutLast     = 0;
	TimePid.Slew        =  300;
	TimePid.OutMax      =  20000;
	TimePid.OutMin      = -20000;
	TimePid.RepairOut   = -80;
	TimePid.KpGain      = 100;
	TimePid.KiGain      = 5;
	TimePid.KdGain      = 20;
	TimePid.Change      = 30;
	TimePid.Wave        = 30;

	AccuPid.e0          = 0;
	AccuPid.e1          = 0;
	AccuPid.e2          = 0;
	AccuPid.OutLast     = 0;
	AccuPid.Slew        =  300;
	AccuPid.OutMax      =  20000;
	AccuPid.OutMin      = -20000;
	AccuPid.RepairOut   = -80;
	AccuPid.KpGain      = 100;
	AccuPid.KiGain      = 5;
	AccuPid.KdGain      = 20;
	AccuPid.Change      = 30;
	AccuPid.Wave        = 30;

	CalibPid.e0         = 0;
	CalibPid.e1         = 0;
	CalibPid.e2         = 0;
	CalibPid.OutLast    = 0;
	CalibPid.Slew       =  300;
	CalibPid.OutMax     =  20000;
	CalibPid.OutMin     = -20000;
	CalibPid.RepairOut  = -80;
	CalibPid.KpGain     = 50;
	CalibPid.KiGain     = 5;
	CalibPid.KdGain     = 20;
	CalibPid.Change     = 30;
	CalibPid.Wave       = 30;

	Wave = 1;
	gControlInfo.ControlStatus = 0;

	TimeCtrlStep  = 100;
	AccuCtrlStep  = 200;
	CalibrateStep = 0;

	TimeCount     = 0;
	MarkerTime    = 0;
	ContolEndTime = 0;
	gThickIO.In.Purge = 0;
   
	RampCalibSet.Ts = tTask;
	RampCalibSet.Accel = 3000;
	RampCalibSet.Once = 0;
	RampCalibSet.Set  = gServoLinear.Current;
   
	HydSCurveRamp(&RampCalibSet);

	RampSet.Ts = tTask;
	RampSet.Accel = 3000;
	RampSet.Once = 0;
	RampSet.Set  = gServoLinear.Current;
   
	HydRampCos(&RampSet);
   
	/* differentiator for evaluating the actual speed */
	vHydDT2Set.Ts = tTask;	      /* cycle time 0.002s 			    */
	vHydDT2Set.Tv = 1.0; 	      /* derivative time constant [s]   */
	vHydDT2Set.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vHydDT2Set.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */

	/* differentiator for evaluating the actual speed */
	vHydDT2Act.Ts = tTask;	      /* cycle time 0.002s 			  */
	vHydDT2Act.Tv = 1.0; 	      /* derivative time constant [s]   */
	vHydDT2Act.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vHydDT2Act.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */
   
	vSetLimit = 30;
	vActLimit = 30;
	
	gThickData.Offset = 0;
	gThickProf.FunOption.SealMode = 0;
	
	gThickProf.FunOption.Servo = 1;

}/*  end of init  */

/**********************************************************************************
**        			C Y C L I C   P A R T   O F   T H E   T A S K  				      **
**********************************************************************************/

_CYCLIC  void cyc()
{
	
	/*---------Ext Moog Thick signel-------------*/
	if(1 == gMachineFix.Option.bExtParisonCtrl)
	{
		gThickProf.FunOption.Servo = 0;
		if (1 == gMachineInfo.ExtParisonCtrlStart)	/* 外部壁厚触发 */
		{
			gMachineInfo.ExtParisonCtrlStart = 0;
			gMacTimer.ExtParisonCtrlOffDelay.IN = 1;
			gMacTimer.ExtParisonCtrlOffDelay.PT = 100;
			gMachineOut.ExtParisonCtrl = 1;
		}	
		if (1 == gMacTimer.ExtParisonCtrlOffDelay.Q)
		{
			gMacTimer.ExtParisonCtrlOffDelay.IN = 0;
			gMachineOut.ExtParisonCtrl = 0;
		}
	}
	
	/*---------ipis0116-------------------------------------------------------------------*/
	
	if( 0 == gThickData.CurrentTime)   // Parision Cycle End
	{
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)	
		{
			gThickProf.TimSet.CycleTime =gUserPara.ThickTime_TwoMold;
		}
		else
		{
			gThickProf.TimSet.CycleTime = gUserPara.ThickTime_OneMold;
		}
	}
	/*-----------------------------------------------------------------------------------*/
	
	
	gThickIO.enable = gThickProf.FunOption.Servo;
	bHideDieGapMoveFrame = !gThickProf.FunOption.bMoveFrame; 
	/*-----------------------------------------------------------------------------------*/
	
	if(gPanelObj.GetPicture == PIC_GAP_CALIBRATE)
	{
		cmdServoUp = gPanelObj.KeyMatric[S$KEYMATRIX_F5];
		cmdServoDn = gPanelObj.KeyMatric[S$KEYMATRIX_F6];
		cmdAutoCalibrate = gPanelObj.KeyMatric[S$KEYMATRIX_F12]; 
	}
	else
	{
		cmdServoUp = 0;
		cmdServoDn = 0;
		cmdAutoCalibrate = 0; 	
	}
	
	TimePid.Wave   = gThickProf.ShareSet.Wave = 10;
	TimePid.Change = gThickProf.ShareSet.Change;
	TimePid.RepairOut = gThickProf.ShareSet.ServoValveComp;
   
	AccuPid.Wave   = gThickProf.ShareSet.Wave;
	AccuPid.Change = gThickProf.ShareSet.Change;
	AccuPid.RepairOut = gThickProf.ShareSet.ServoValveComp;

	CalibPid.Wave   = gThickProf.ShareSet.Wave;
	CalibPid.Change = gThickProf.ShareSet.Change;
	CalibPid.RepairOut = gThickProf.ShareSet.ServoValveComp;
	CalibPid.Act = gServoLinear.Current;
   
	gThickData.CurrentPos = gAccuLinear.Current;
	gThickData.ServoAct   = gServoLinear.Current;
   
	TimePid.KiGain = 6;
	AccuPid.KiGain = 6;
	CalibPid.KiGain= 6;
	
	if(0 == gMachineOut.ParisonPump)
	{
		TimePid.KiGain = 0;
		AccuPid.KiGain = 0;
		CalibPid.KiGain= 0;
	}
	
	/* velocity  */
	vHydDT2Act.u = gServoLinear.Current;
	HydDT2(&vHydDT2Act);
   
	/* velocity  */
	vHydDT2Set.u = SetValue;
	HydDT2(&vHydDT2Set);
	
	/*  servo LVDT calibration OK or NOT OK  */
	if((gServoLinear.MaxPos  > gServoLinear.ZeroPos + 2000) && (gServoLinear.MaxPos < 32000)&&
		(gServoLinear.ZeroPos > 500))
	{
		gServoLinear.IsReady = 1;
	}
	else
	{
		gServoLinear.IsReady = 0;
	}
	
	if(0 == CalibrateStep)gServoLinear.Status = gServoLinear.IsReady; /* not auto calibration */

	if(gControlInfo.ControlStatus == S$STATUS_AUTORUN) /* 非调校状态 */
		{
		if(1 == gThickIO.In.StartThick)	/* 壁厚触发 */
		{
			if(0 == gThickProf.FunOption.IsAccu)
			{
				if(gThickData.TimePerDot != 0)TimeCtrlStep = 10;
				AccuCtrlStep = 0;
			}
			else
			{
				TimeCtrlStep = 0;
				AccuCtrlStep = 10;
			}
			StartTime = 250;
			gThickIO.In.StartThick = 0;
		}

		if(1 == gThickIO.In.Stop)       /* 停止信号 */
		{
			if(1 == gThickProf.FunOption.IsAccu)
			{
				AccuCtrlStep = 50;
			}
			gThickIO.In.Stop = 0;
		}
		
		if(gThickIO.tSealGapDly > 0.0)
		{
			gThickIO.tSealGapDly = gThickIO.tSealGapDly - tTask; 
			if(gThickIO.tSealGapDly < 0.0)gThickIO.tSealGapDly = 0.0;
			if(gThickIO.tSealGapDly < 2 * tTask)gThickIO.In.SealGap = 1;		
		}
				
		if(1 == gThickIO.In.SealGap)    /* 封口信号 */
		{
			if(0 == gThickProf.FunOption.IsAccu)
			{
				TimeCtrlStep = 110;
				AccuCtrlStep = 0;
			}
			else
			{
				TimeCtrlStep = 0;
				AccuCtrlStep = 100;
			}
			gThickIO.In.SealGap = 0;
		}

		TimePid.Act = gServoLinear.Current;
		AccuPid.Act = gServoLinear.Current;
			
		if(0 == gThickProf.FunOption.IsAccu) 	/* 时间式 */
		{
			switch(TimeCtrlStep)
			{
				case 10:
					gThickData.Counter = 0;
					gThickData.StartServoPos = TimePid.Act ;
					TimePid.OutMax =  gThickProf.ShareSet.StartFlow * 33;
					TimePid.OutMin = -1 * TimePid.OutMax ;
               
					RampSet.Once = 0;
					SetPID = TimePid.Act;
               
					TimeCtrlStep = 20;
					break;
            
				case 20:
					gThickData.Counter++;                    				/* 开口延迟 */

					if((gThickData.SetData[0] - gThickData.StartServoPos != 0) && (gThickData.TimeOpenDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + (gThickData.SetData[0] - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.TimeOpenDly );
					}
					else
					{
						SetPID = gThickData.SetData[0];
					}

					if(gThickData.Counter > gThickData.TimeOpenDly)
					{
						gThickData.Counter = 0;
						TimeCtrlStep = 30;
					}
					break;
            
				case 30:
					memset(gThickData.RealData ,0,sizeof(gThickData.RealData ));
					gThickData.ControlOk = 0;
					gThickData.FocusNum  = 0;
					TimePid.OutMax =  gThickProf.ShareSet.ThickFlow * 33;
					TimePid.OutMin = -1 * TimePid.OutMax ;
               
					SetPID = gThickData.SetData[0];

					//				for (uTIndex = 0; uTIndex < 100; uTIndex++)
					//				{
					//					gThickData.SetData[uTIndex] = gTempData[uTIndex];
					//				}
					TimeCtrlStep = 40;
					break;
            
				case 40:                                   /* Time mode profile */
					if(ABS(vHydDT2Set.y) > vSetLimit || ABS(vHydDT2Act.y) > vActLimit)
					{
						//    TimePid.KiGain = 0;
					}
               
					// Call function  
					
					//				for (uTIndex = 0; uTIndex < 100; uTIndex++)
					//				{
					//					gThickData.SetData[uTIndex] = gTempData[uTIndex];
					//				}	
					
					TimeCurveData(&gThickData);
               
					SetPID = gThickData.SetPID;

					gThickData.CurrentTime = gThickData.Counter/5;
					
					if(1 == gThickData.ControlOk)
					{
						gThickData.CurrentTime = 0;
						gThickData.ControlOk = 0;
						TimeCtrlStep = 50;
					}
					break;
            
				case 50:
					gThickData.StartServoPos = TimePid.Act;
					gThickData.Counter  = 0;
					gThickData.FocusNum = 0;
					TimePid.OutMax =  gThickProf.ShareSet.SealFlow * 33;
					TimePid.OutMin = -1 * TimePid.OutMax ;

					ContolEndTime = 100;
					gThickIO.Out.ControlEnd = 1;
               
					SetPID = TimePid.Act;

					if(0 == gThickProf.FunOption.SealMode)
					{
						SealValue = gThickData.SetData[0];
					}
					else
					{
						SealValue = gThickData.SealPos;
					}
 
					TimeCtrlStep = 60;
					break;
            
				case 60:
					gThickData.Counter++;                    /* 封口时间 */
					if((SealValue - gThickData.StartServoPos != 0) && (gThickData.SealGapDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + ( SealValue - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.SealGapDly);
					}
					else
					{
						SetPID = SealValue;
					}

					if(gThickData.Counter >= gThickData.SealGapDly)
					{
						gThickData.Counter = 0;
						TimeCtrlStep = 100;
					}
					break;
            
				case 100:
					if(0 == gThickProf.FunOption.SealMode)
					{
						SetPID = gThickData.SetData[0];
					}
					else
					{
						SetPID = gThickData.SealPos;
					}
					break;
				
				/*  ----------------- Seal gap -------------------- */
				case 110:
					gThickData.StartServoPos = TimePid.Act;
					gThickData.Counter  = 0;
					gThickData.FocusNum = 0;
					TimePid.OutMax =  gThickProf.ShareSet.SealFlow * 33;
					TimePid.OutMin = -1 * TimePid.OutMax ;

					SetPID = TimePid.Act;
					SealValue = gThickData.SealPos;
 
					TimeCtrlStep = 120;
					break;
				
				case 120:
					gThickData.Counter++;           /* 封口时间 */
					if((SealValue - gThickData.StartServoPos != 0) && (gThickData.SealGapDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + ( SealValue - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.SealGapDly);
					}
					else
					{
						SetPID = SealValue;
					}

					if(gThickData.Counter >= gThickData.SealGapDly)
					{
						gThickData.Counter = 0;
						TimeCtrlStep = 130;
					}
					break;
				
				case 130:
					gThickData.Counter++;
					if(gThickData.Counter > 2000)
					{
						gThickData.Counter = 0;
						TimeCtrlStep = 140;
					}
					break;
				
				case 140:
					gThickData.StartServoPos = TimePid.Act;
					gThickData.Counter  = 0;
					gThickData.FocusNum = 0;
					TimePid.OutMax =  gThickProf.ShareSet.SealFlow * 33;
					TimePid.OutMin = -1 * TimePid.OutMax ;

					SetPID = TimePid.Act;

					if(0 == gThickProf.FunOption.SealMode)
					{
						SealValue = gThickData.SetData[0];
					}
					else
					{
						SealValue = gThickData.SealPos;
					}
 
					TimeCtrlStep = 150;
					break;
				
				case 150:
					gThickData.Counter++;           /* 封口时间 */
					if((SealValue - gThickData.StartServoPos != 0) && (gThickData.SealGapDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + ( SealValue - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.SealGapDly);
					}
					else
					{
						SetPID = SealValue;
					}

					if(gThickData.Counter >= gThickData.SealGapDly)
					{
						gThickData.Counter = 0;
						TimeCtrlStep = 200;
					}
					break;
				
				case 200:
					if(0 == gThickProf.FunOption.SealMode)
					{
						SetPID = gThickData.SetData[0];
					}
					else
					{
						SetPID = gThickData.SealPos;
					}
					break;
				
				default:
					SetPID = gServoLinear.Current;
					break;

			}/* switch(TimeCtrlStep) */
         
			if(0 == gThickIO.In.Purge && 0 == PurgeOld)
			{
				RampSet.Set = SetPID;   
			}
			else if(gThickIO.In.Purge != PurgeOld)
			{
				RampSet.Once = 0;
				RampSet.Set  = gServoLinear.Current;
			}
			else if(1 == gThickIO.In.Purge && 1 == PurgeOld)
			{
				RampSet.Set = gThickData.PurgePos; 
			}
			
			/* Call function  */
			HydRampCos(&RampSet);
			
			if(0 == gThickIO.In.Purge && 0 == PurgeOld)
			{
				TimePid.Set = SetPID;	
			}
			else
			{
				TimePid.Set = RampSet.Out;
			}
			
			PIDCtrl(&TimePid);
			
			SetValue = TimePid.Set;
			ServeOut = TimePid.Out + TimePid.RepairOut;
         
			if(40 == TimeCtrlStep)
			{
				gControlInfo.CurrentPoint = gThickData.FocusNum;
				if(1 ==gThickProf. IsMarker[gThickData.FocusNum])
				{
					MarkerTime = 125;
				}
			}
			else
			{
				gControlInfo.CurrentPoint = 0;
			}
         
			AccuCtrlStep = 100;
		}
		else  /* if(0 == gThickProf.FunOption.IsAccu) */  /* 位置式 */
		{
			switch(AccuCtrlStep)
			{
				case 10:
					gThickData.Counter = 0;
					gThickData.StartServoPos = AccuPid.Act;
					AccuPid.OutMax =  gThickProf.ShareSet.StartFlow * 33;
					AccuPid.OutMin = -1 * AccuPid.OutMax ;
               
					RampSet.Once = 0;
					SetPID = AccuPid.Act;

					AccuCtrlStep = 20;
					break;
            
				case 20:                                 /* 开口延迟 */
					gThickData.Counter++;

					if((gThickData.SetData[0] - gThickData.StartServoPos != 0) && (gThickData.AccuOpenDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + (gThickData.SetData[0] - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.AccuOpenDly );
					}
					else
					{
						SetPID = gThickData.SetData[0];
					}
					if(gThickData.Counter >= gThickData.AccuOpenDly )
					{
						gThickData.Counter = 0;
						AccuCtrlStep = 30;
					}
					break;
            
				case 30:
					gThickData.CurrentTime = 0;
					gThickData.ControlOk   = 0;
					gThickData.FocusNum    = 0;
					memset(gThickData.RealData ,0,sizeof(gThickData.RealData ));
					memset(gThickData.InitLock ,1,sizeof(gThickData.InitLock));
					AccuPid.OutMax =  gThickProf.ShareSet.ThickFlow * 33;
					AccuPid.OutMin = -1 * AccuPid.OutMax ;
               
					SetPID = gThickData.SetData[0];
					
					gThickData.CurrentPos = gAccuLinear.Current - gThickData.Offset;

					AccuCtrlStep = 40;
					break;
            
				case 40:                                 /* 位置式控制 */
					if(ABS(vHydDT2Set.y) > vSetLimit || ABS(vHydDT2Act.y) > vActLimit)
					{
						AccuPid.KiGain = 0;
					}
               
					gThickData.CurrentPos = gAccuLinear.Current - gThickData.Offset;
					
					/* call function  */
					AccuCurveData(&gThickData);
               
					SetPID = gThickData.SetPID;
               
					if(1 == gThickData.ControlOk)
					{
						gThickData.ControlOk = 0;
						gThickData.Counter = 0;
						AccuCtrlStep = 50;
					}
					break;
            
				case 50:
					if(0 == gTKauto.Status)
					{
						gThickData.StartServoPos = AccuPid.Act;
					}
					else
					{
						gThickData.StartServoPos = gThickData.SetData[99];
					}
					gThickData.FocusNum = 0;
					gThickData.Offset = 0;
               
					SetPID = AccuPid.Act;
               
					AccuCtrlStep = 60;
					break;
            
				case 60:
					SetPID = gThickData.StartServoPos;
					break;
            
				case 100:
					gThickData.StartServoPos = AccuPid.Act;
					gThickData.Counter = 0;
					AccuPid.OutMax =  gThickProf.ShareSet.SealFlow * 33;
					AccuPid.OutMin = -1 * AccuPid.OutMax ;
               
					RampSet.Once = 0;
					SetPID = AccuPid.Act;
               
					AccuCtrlStep = 110;
					break;
            
				case 110:                                /* 封口时间 */
					gThickData.Counter++;

					if((gThickData.SealPos - gThickData.StartServoPos != 0) && (gThickData.SealGapDly != 0))
					{
						SetPID = (short)(gThickData.StartServoPos + (gThickData.SealPos - gThickData.StartServoPos)
							* gThickData.Counter / gThickData.SealGapDly);
					}
					else
					{
						SetPID = gThickData.SealPos;
					}

					if(gThickData.Counter >= gThickData.SealGapDly)
					{
						gThickData.Counter = 0;
						AccuCtrlStep = 120;
					}
					break;
            
				case 120:
					SetPID = gThickData.SealPos;
					AccuCtrlStep = 200;
					break;
            
				case 200:
					SetPID = gThickData.SealPos;
					break;
            
				default:
					SetPID = gServoLinear.Current;
					break;
			}/* switch(AccuCtrlStep) */
         
			if(0 == gThickIO.In.Purge && 0 == PurgeOld)
			{
				RampSet.Set = SetPID;
			}
			else if(gThickIO.In.Purge != PurgeOld)
			{
				RampSet.Once = 0;
				RampSet.Set  = gServoLinear.Current;
			}
			else if(1 == gThickIO.In.Purge && 1 == PurgeOld)
			{
				RampSet.Set = gThickData.PurgePos; 
			}
         
			HydRampCos(&RampSet);
			
			if(0 == gThickIO.In.Purge && 0 == PurgeOld)
			{
				AccuPid.Set = SetPID;	
			}
			else
			{
				AccuPid.Set = RampSet.Out;
			}
			
			PIDCtrl(&AccuPid);
			
			SetValue = AccuPid.Set;
			ServeOut = AccuPid.Out + AccuPid.RepairOut;
         
			if(40 == AccuCtrlStep)
			{
				gControlInfo.CurrentPoint = gThickData.FocusNum;
				if(1 == gThickProf.IsMarker[gThickData.FocusNum])
				{
					MarkerTime = 125;
				}
			}
			else
			{
				gControlInfo.CurrentPoint = 0;
			}

			TimeCtrlStep = 50;
		}/* else if(0 == gThickProf.FunOption.IsAccu) */

		if((gThickData.ServoMax - gThickData.ServoMin) != 0)
		{
			if(0 == gThickProf.FunOption.IsZeroDn)
			{
				gThickIO.SetPercent = (SetValue - gThickData.ServoMin            )* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				gThickIO.ActPercent = (gServoLinear.Current - gThickData.ServoMin)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
			}
			else
			{
				gThickIO.SetPercent = (gThickData.ServoMax - SetValue            )* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				gThickIO.ActPercent = (gThickData.ServoMax - gServoLinear.Current)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
			}
		}
		else
		{
			gThickIO.SetPercent = 0.0;
			gThickIO.ActPercent = 0.0;
		}
      
		cmdAutoCalibrate = 0;
		CalibrateStep = 0;
		SetGapValue = gServoLinear.Current;	
		
		/* keep mode */
		SetGapPercent = gThickIO.SetPercent;
		bCalibPosMode = 0;
	}
	else     /* 调校状态 */
	{
		SetPID = gServoLinear.Current;
      
		gThickIO.SetPercent = 0.0;
      
		gThickIO.In.Stop     = 0;
		gThickIO.In.SealGap  = 0;
      
		ServeSetValue = gServoLinear.Current;
		/* thickness time  */
		if(1 == gThickIO.In.StartThick)
		{
			gThickIO.In.StartThick = 0;
			CycleTimeCount = gThickData.CycleTimes + 1;
		}

		if(CycleTimeCount > 0)CycleTimeCount--;
		gThickData.CurrentTime = (gThickData.CycleTimes - CycleTimeCount) / 5;
		if(1 == CycleTimeCount)gThickIO.Out.ControlEnd = 1;
		if(0 == CycleTimeCount)gThickData.CurrentTime = 0;
		
		if(gThickProf.TimSet.CycleTime != 0)
		{
			gThickData.FocusNum = gThickData.CurrentTime * 10.0/gThickProf.TimSet.CycleTime;
		}
		
		/*  判断是否手动校调和自动校调  */
		if(cmdServoUp != 0              || cmdServoDn != 0    || cmdAutoCalibrate != 0       || CalibrateStep != 0 || 
			0 == gMachineOut.ParisonPump || gServoLinear.Current < gThickData.ServoMin - 1635 || gServoLinear.Current > gThickData.ServoMax + 1635)
		{
			bCalibPosMode = 0;
		}

		if(0 == bCalibPosMode)
		{
			if(0 == gThickProf.FunOption.IsAutoCalibrate)  /* 手动调校 */
			{
				if(1 == cmdServoUp)
				{
					ServeOut = -gServoLinear.ManualSpeed ;
				}
   
				if(1 == cmdServoDn)
				{
					ServeOut = gServoLinear.ManualSpeed;
				}
   
				if((0 == cmdServoUp) &&(0 == cmdServoDn))
				{
					ServeOut = 0;
					bCalibPosMode = 1;
					SetGapPercent = gThickIO.ActPercent;
				}
				CalibrateStep = 0;
			}
			else                                           /* 自动调校 */
			{
				if(1 == cmdAutoCalibrate)
				{
					cmdAutoCalibrate = 0;
					if(gServoLinear.AutoSpeed > 0 && 0 == CalibrateStep) 
					{
						CalibrateStep = 10;
					}
				}
            
				/* step machine control  */
				switch(CalibrateStep)
				{
					case 0:
						ServeOut = 0;
						break;
               
					case 10:
						gServoLinear.Status = 2;
						TimeCount = 0;
						CurrentInput = gServoLinear.Current;
						LastInput    = gServoLinear.Current;
						CalibrateStep = 20;
						break;
               
					case 20:
						ServeOut = -gServoLinear.AutoSpeed;
						TimeCount++;
   
						if( 0 == (TimeCount % 1000))
						{
							CurrentInput = gServoLinear.Current;
							if( (CurrentInput - LastInput) > -100 && (CurrentInput - LastInput) < 100 )
							{
								CalibrateStep = 30;
							}
							LastInput = CurrentInput;
						}
						break;
               
					case 30:
						gServoLinear.ZeroPos = gServoLinear.Current;
						ServeOut  = 0;
						TimeCount = 0;
						CurrentInput = gServoLinear.Current;
						LastInput    = gServoLinear.Current;
						CalibrateStep = 40;
						break;
               
					case 40:
						ServeOut = gServoLinear.AutoSpeed;
						TimeCount++;
   
						if( 0 == (TimeCount % 1000))
						{
							CurrentInput = gServoLinear.Current;
							if( (CurrentInput - LastInput) > -100 && (CurrentInput - LastInput) < 100 )
							{
								CalibrateStep = 50;
							}
							LastInput = CurrentInput;
						}
						break;
               
					case 50:
						gServoLinear.MaxPos = gServoLinear.Current;
						ServeOut  = 0;
						TimeCount = 0;
						CalibrateStep = 60;
						break;
               
					case 60:
						gServoLinear.Status = gServoLinear.IsReady;
						gRecipeOperate.OnceSave = 1;
						CalibrateStep = 0;
						break;
               
					default:
						break;
				}/* switch(CalibrateStep) */
			} /* 自动调校 */
      
			if((gThickData.ServoMax - gThickData.ServoMin) != 0)
			{
				if(0 == gThickProf.FunOption.IsZeroDn)
				{
					gThickIO.ActPercent = (gServoLinear.Current - gThickData.ServoMin)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				}
				else
				{
					gThickIO.ActPercent = (gThickData.ServoMax - gServoLinear.Current)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				}
			}
			else
			{
				gThickIO.ActPercent = 0.0;
			}
			SetGapValue = gServoLinear.Current;
		}
		else                    /*  keep mode  */
		{
			if((gThickData.ServoMax - gThickData.ServoMin) != 0)
			{
				if(0 == gThickProf.FunOption.IsZeroDn)
				{
					SetGapValue = gThickData.ServoMin + (gThickData.ServoMax - gThickData.ServoMin)* SetGapPercent/100.0;
					gThickIO.ActPercent = (gServoLinear.Current - gThickData.ServoMin)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				}
				else
				{
					SetGapValue = gThickData.ServoMax - (gThickData.ServoMax - gThickData.ServoMin)* SetGapPercent/100.0;
					gThickIO.ActPercent = (gThickData.ServoMax - gServoLinear.Current)* 100.0 /(gThickData.ServoMax - gThickData.ServoMin);
				}
			}
			else
			{
				gThickIO.ActPercent = 0.0;
			}
			
			CalibPid.Set = (INT)RampCalibSet.Out;
			if(CalibPid.e0 > 100 || CalibPid.e0 < -100)CalibPid.KiGain = 0; 
			PIDCtrl(&CalibPid);
			ServeOut = CalibPid.Out + CalibPid.RepairOut;
		}
		TimeCtrlStep = 50;
		AccuCtrlStep = 100;
		gThickIO.Out.MarkPulse  = 0;
	} /* else 调校状态 */

	RampCalibSet.Set = SetGapValue;
	HydSCurveRamp(&RampCalibSet);
	
	if(1 == bSetCalibPos)
	{
		bSetCalibPos = 0;
		bCalibPosMode = 1;
		CalibPid.Yi = 0;
	}
	
	if(bCalibPosModeOld != bCalibPosMode)
	{
		CalibPid.Yi = 0;
	}
	bCalibPosModeOld = bCalibPosMode;
	
	gControlInfo.CurrentGapSet = SetGapValue;
	gControlInfo.CurrentGapSetPercent = gThickIO.SetPercent;
	
	gControlInfo.CurrentGapPercent = gThickIO.ActPercent; 
	

	if(MarkerTime > 0)
	{
		MarkerTime--;
		gThickIO.Out.MarkPulse  = 1;
	}
	else
	{
		gThickIO.Out.MarkPulse  = 0;
	}

	if(StartTime > 0)
	{
		StartTime --;
		gThickIO.Out.Start = 1;
	}
	else
	{
		gThickIO.Out.Start = 0;
	}

	if(ContolEndTime > 0)
	{
		ContolEndTime--;
		gThickIO.Out.End  = 1;
	}
	else
	{
		gThickIO.Out.End  = 0;
	}

	Wave = -1 * Wave;

	if(gServoLinear.Current < 32767     && gServoLinear.Current > 0 && 
		1 == gThickProf.FunOption.Servo  && 1 == gMachineOut.ParisonPump)
	{
		gThickIO.ServoOut = (short)( ServeOut + Wave * TimePid.Wave);
	}
	else
	{
		gThickIO.ServoOut = 0;
	}
	
	gMachineOut.StartThick = gThickIO.Out.Start;
	PurgeOld = gThickIO.In.Purge;

}/*  end of cycle  */


