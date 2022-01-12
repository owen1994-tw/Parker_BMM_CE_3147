/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: workmode
 * File: workmode.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program workmode --- work mode   control 
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "defines.h"
#include "pic_id.h"
#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
//#include "keyap880.h"

#define  REQUEST_NOTHING    0
#define  REQUEST_MANUAL     1000
#define  REQUEST_AUTO       2000
#define  REQUEST_ADJUST     3000
#define  REQUEST_RESET      6000
#define  REQUEST_PIN_CLAIB  7000

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void BasicPos(Mold_typ *pMold, HWEnable_Mold_Fix_typ *pHWEnableMoldFix );
void DriverNetworkPhaseReady(void);
/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	Step = 0;

	gMachineInfo.MachineReset = 0;

	DelayTime = 0;
	
	bInit_clear_gUserPara_TimeDis = 1; 
	memset(&RMold.TimeDis,0,sizeof(RMold.TimeDis));
	memset(&LMold.TimeDis,0,sizeof(LMold.TimeDis));
	
	memset(&gUserPara.RMold.TimeDis,0,sizeof(gUserPara.RMold.TimeDis));
	memset(&gUserPara.LMold.TimeDis,0,sizeof(gUserPara.LMold.TimeDis));

} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(!LMold.TimeSet.CycleAlarmTime)LMold.TimeSet.CycleAlarmTime = 5000;
	if(!RMold.TimeSet.CycleAlarmTime)RMold.TimeSet.CycleAlarmTime = 5000;
	
	gMachineOut.PneumaticValve = gMachineIn.PneuForceNormal;
	gMachineOut.InstancyStop = gMachineIn.InstancyStop;
	//Safe Related Stop Mechanism
	gMachineInfo.SafeInStop =  (gMachineIn.SafeModule || ( gMachineInfo.SafeLogicValveDelayDone && gMachineIn.SafeLogicValve && gMachineInfo.SafeLogicValveOffDelayTimeOut && gMachineIn.CalibModeForceDI == 0)) 
								&& gMachineOut.OilPump;
	
	if(1 == bInit_clear_gUserPara_TimeDis && PIC_OVERVIEW == gPanelObj.GetPicture)
	{
		bInit_clear_gUserPara_TimeDis = 0;
		memset(&gUserPara.RMold.TimeDis,0,sizeof(gUserPara.RMold.TimeDis));
		memset(&gUserPara.LMold.TimeDis,0,sizeof(gUserPara.LMold.TimeDis));	
	}
	

	//Safe Logic Valve Check
	if(gUserPara.RMold.TimeSet.SafeLogicValveDelay <=0 ) gUserPara.RMold.TimeSet.SafeLogicValveDelay = 300;
	if(gUserPara.RMold.TimeSet.SafeLogicValveOffDelay <=0 ) gUserPara.RMold.TimeSet.SafeLogicValveOffDelay = 100;
	if(1 == gMachineOut.OilPump )
	{
		//On Check Delay
		if(0 == gMachineIn.CalibModeForceDI)// manual
		{
			if(0 == gMachineInfo.SafeLogicValveDelayDone)
			{
				if(gMacTimer.SafeLogicValveDelay.Q)
				{
					gMacTimer.SafeLogicValveDelay.IN = 0;
					gMachineInfo.SafeLogicValveDelayDone = 1;
				}
				else
				{
					gMacTimer.SafeLogicValveDelay.IN = 1;
					gMacTimer.SafeLogicValveDelay.PT = gUserPara.RMold.TimeSet.SafeLogicValveDelay;
				}
			}
		}
		else
		{
			gMacTimer.SafeLogicValveDelay.IN = 0;
			gMachineInfo.SafeLogicValveDelayDone = 0;
		}
		
		//Off Delay
		
		if(1 == gMachineIn.SafeLogicValve)
		{
			if(0 == gMachineInfo.SafeLogicValveOffDelayTimeOut)
			{
				if(gMacTimer.SafeLogicValveOffDelay.Q)
				{
					gMacTimer.SafeLogicValveOffDelay.IN = 0;
					gMachineInfo.SafeLogicValveOffDelayTimeOut = 1;
				}
				else
				{
					gMacTimer.SafeLogicValveOffDelay.IN = 1;
					gMacTimer.SafeLogicValveOffDelay.PT = gUserPara.RMold.TimeSet.SafeLogicValveOffDelay;
				}
			}
		}
		else
		{
			gMacTimer.SafeLogicValveOffDelay.IN = 0;
			gMachineInfo.SafeLogicValveOffDelayTimeOut = 0;
		}

	}
	else
	{
		gMacTimer.SafeLogicValveDelay.IN = 0;
		gMachineInfo.SafeLogicValveDelayDone = 0;
		gMachineInfo.SafeLogicValveOffDelayTimeOut = 0;
	}
	
	//Safe Related Alarm
	if(gMachineIn.SafeModule && gMachineOut.OilPump)
	{
		gAlarm.SafeModule = 1;
	}
	if(1 == gMachineIn.CalibModeForceDI)
	{
		if(gMachineIn.SafeCalibPressure && gMachineOut.OilPump)
		{
			gAlarm.SafeCalibPressure = 1;
		}
		gAlarm.SafeLogicValve 	= 0;
	}
	else
	{
		gAlarm.SafeCalibPressure = 0;
		if(gMachineInfo.SafeLogicValveDelayDone &&  gMachineIn.SafeLogicValve && gMachineInfo.SafeLogicValveOffDelayTimeOut  && gMachineOut.OilPump)
		{
			gAlarm.SafeLogicValve = 1;
		}
	}
	
	/* --------------------------- 紧急停止 ---------------------------- */
	if(1 == gMachineIn.InstancyStop || 1 == MachineStop || 1 == gMachineInfo.SafeInStop)
	{
		Step = 30000;
	}
	else
	{
		if(1 == LMold.AutoReqStop || 1 == RMold.AutoReqStop)   /*  自动故障停 */
		{
			LMold.AutoReqStop = 0;
			RMold.AutoReqStop = 0;
			Step = REQUEST_MANUAL;
		}
		else if(1 == gPROCESS_info.oilpumpStop)   		   /*  主油泵/電機故障停 */
		{
			gPROCESS_info.oilpumpStop = 0;
			if(1 == gMachineInfo.Auto)
			{
				Step = REQUEST_MANUAL;
			}
		}
		else if (1 == gMachineInfo.ManualModeReguest)
		{
			gMachineInfo.ManualModeReguest = 0;
			if(1 == gMachineInfo.Auto)
			{
				Step = REQUEST_MANUAL;
			}
		}
		
	}/* if(1 == gMachineIn.InstancyStop) */

	BasicPos(&LMold,&gMachineFix.Option.HWEnable.LMold);
	BasicPos(&RMold,&gMachineFix.Option.HWEnable.RMold);
	if( 0 == gMachineOut.OilPump)
	{
		DriverNetworkPhaseReady();
	}
	
	/*------------Batch------------*/
	if( gMachineInfo.Yield >0  &&   gMachinePara.BatchSize >0) 
	{
		if((gMachineInfo.oldYield !=  gMachineInfo.Yield) && ( gMachineInfo.oldYield < gMachineInfo.Yield ) )
		{
			gBatchInfo.Batch=  gBatchInfo.Batch + (gMachineInfo.Yield - gMachineInfo.oldYield );
		}
		if( gBatchInfo.Batch >= gMachinePara.BatchSize )
		{
			gBatchInfo.Batch= gBatchInfo.Batch - gMachinePara.BatchSize ;
			gBatchInfo.BatchDoneCount++;
			gAlarm.BatchDoneCounReach =1;			
		}
	}
	gMachineInfo.oldYield = gMachineInfo.Yield;
	
	
	/*---------------------  自動開始Led燈號模式  -------------------------*/
	//LedAutoRun 20200911 Owen
	if(1 == gMachineInfo.Auto)  //Auto mode
	{
		if (1 == gMachineOut.AutoRun ) //Autorun
		{
			if( 2 == gMachineInfo.LedAutoStop) //one cycle
			{
				gMachineInfo.LedAutoRun = 2;
			}	
			else // auto running
			{
				gMachineInfo.LedAutoRun = 1;
			}
		}
		else if( 1 == gMachineInfo.MachineReset ) //Reseting
		{
			gMachineInfo.LedAutoRun = 1;
		}
		else if ( 0 == gMachineInfo.MachineReset) //After Reset Done
		{
			if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable) //Double mold
			{
				if(1 == LMold.InBasicPos && 1 == RMold.InBasicPos && 1 == LMold.Carriage.BwPos && 1 == RMold.Carriage.BwPos ) gMachineInfo.LedAutoRun = 2;	
				else gMachineInfo.LedAutoRun = 0;
			}
			else if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable) //Right mold
			{
				if(1 == RMold.InBasicPos)gMachineInfo.LedAutoRun = 2;
				else gMachineInfo.LedAutoRun = 0;
			}
			else if(0 == RMold.Option.Enable && 1 == LMold.Option.Enable) //Left mold
			{
				if(1 == LMold.InBasicPos)gMachineInfo.LedAutoRun = 2;
				else gMachineInfo.LedAutoRun = 0;
			}		
		}
		else
		{
			gMachineInfo.LedAutoRun = 0;
		}
	}
	else
	{
		gMachineInfo.LedAutoRun = 0;
	}
	
	//Led Flash State
	
	switch (gMachineInfo.LedAutoRun)
	{
		case 1:  /* On */
			
			gMacTimer.LedAutoRunFlashOn.IN = 0;
			gMacTimer.LedAutoRunFlashOff.IN = 0;
		
			gMachineOut.LedAutoRun = 1;
       
			break;
		
		case 2:  /* Flashing */
			
			if( 1 == gMacTimer.LedAutoRunFlashOn.Q )
			{		
				gMacTimer.LedAutoRunFlashOn.IN = 0;
								
				gMacTimer.LedAutoRunFlashOff.IN = 1;
				gMacTimer.LedAutoRunFlashOff.PT = 50;
						
				gMachineOut.LedAutoRun = 0;
			}
			else if ( 1 == gMacTimer.LedAutoRunFlashOff.Q)
			{
				gMacTimer.LedAutoRunFlashOff.IN = 0;
										
				gMacTimer.LedAutoRunFlashOn.IN = 1;
				gMacTimer.LedAutoRunFlashOn.PT = 100;
						
				gMachineOut.LedAutoRun = 1;
			}
			else if (0 == gMacTimer.LedAutoRunFlashOn.IN && 0 == gMacTimer.LedAutoRunFlashOff.IN )  //First On
			{
			
				gMacTimer.LedAutoRunFlashOn.IN = 1;
				gMacTimer.LedAutoRunFlashOn.PT = 10;
			
				gMachineOut.LedAutoRun = 1;
			}
       
			break;
		
		case 0:  /* Off */	
			gMacTimer.LedAutoRunFlashOn.IN = 0;
			gMacTimer.LedAutoRunFlashOff.IN = 0;
		
			gMachineOut.LedAutoRun = 0;
			
			break;
	}
     
	//	if(1 == gMachineInfo.LedAutoRun) //On
	//	{	
	//		gMacTimer.LedAutoRunFlashOn.IN = 0;
	//		gMacTimer.LedAutoRunFlashOff.IN = 0;
	//		
	//		gMachineOut.LedAutoRun = 1;
	//	}		
	//	else if(2 == gMachineInfo.LedAutoRun) //Flashing
	//	{	
	//		if( 1 == gMacTimer.LedAutoRunFlashOn.Q )
	//		{		
	//			gMacTimer.LedAutoRunFlashOn.IN = 0;
	//								
	//			gMacTimer.LedAutoRunFlashOff.IN = 1;
	//			gMacTimer.LedAutoRunFlashOff.PT = 50;
	//						
	//			gMachineOut.LedAutoRun = 0;
	//		}
	//		else if ( 1 == gMacTimer.LedAutoRunFlashOff.Q)
	//		{
	//			gMacTimer.LedAutoRunFlashOff.IN = 0;
	//										
	//			gMacTimer.LedAutoRunFlashOn.IN = 1;
	//			gMacTimer.LedAutoRunFlashOn.PT = 100;
	//						
	//			gMachineOut.LedAutoRun = 1;
	//		}
	//		else if (0 == gMacTimer.LedAutoRunFlashOn.IN && 0 == gMacTimer.LedAutoRunFlashOff.IN )  //First On
	//		{
	//			
	//			gMacTimer.LedAutoRunFlashOn.IN = 1;
	//			gMacTimer.LedAutoRunFlashOn.PT = 10;
	//			
	//			gMachineOut.LedAutoRun = 1;
	//				
	//		}			
	//	}
	//	else // Off
	//	{
	//		gMacTimer.LedAutoRunFlashOn.IN = 0;
	//		gMacTimer.LedAutoRunFlashOff.IN = 0;
	//		
	//		gMachineOut.LedAutoRun = 0;
	//	}
	
	
	/* --------------------------- 風壓啟動閥 ---------------------------- */
	if(1 == gMachineIn.InstancyStop || 0 == gMachineIn.SafeGate || 1 == gMachineInfo.SafeInStop )/*Chaoi*/
	{
		gMachineOut.LMold.PneumaticOn = 0;
		gMachineOut.RMold.PneumaticOn = 0;
	}else
	{
		gMachineOut.LMold.PneumaticOn = 1;
		gMachineOut.RMold.PneumaticOn = 1;
	}
	/* ------------------------------ 工作燈 ------------------------------ */
	if(1 == gUserFix_new.FrontWorkingLight ) gMachineOut.FrontWorkingLight = 1;      /*Owen*/
	else  gMachineOut.FrontWorkingLight = 0;

	/* ------------------------ manual autorun calibration request ------------------------ */
	//Init activate force CalibModeForceDI change
	if( 0 == CalibModeForceDI_Init && gPanelObj.GetPicture != PIC_INIT) 
	{
		CalibModeForceDI_Init = 1;	
		oldCalibModeForceDI = !gMachineIn.CalibModeForceDI;
	}
	
	if(0 == gMachineIn.CalibModeForceDI ) // manual / auto
	{
		
//		AdjustReq = gPanelObj.KeyMatric[S$KEYMATRIX_MODE_CALIB]; 
		AutoReq   = gPanelObj.KeyMatric[S$KEYMATRIX_MODE_AUTO];
		ManualReq = gPanelObj.KeyMatric[S$KEYMATRIX_MODE_MANUAL]|| (gMachineIn.CalibModeForceDI != oldCalibModeForceDI);
		DrycyleReq = gPanelObj.KeyMatric[S$KEYMATRIX_DRY_CYCLE];
	}
	else   //calib mode
	{
		AdjustReq = gPanelObj.KeyMatric[S$KEYMATRIX_MODE_CALIB] || (gMachineIn.CalibModeForceDI != oldCalibModeForceDI); 
	}
//	if(Step == REQUEST_NOTHING) oldCalibModeForceDI = gMachineIn.CalibModeForceDI;

	//		gMachineIn.AutoStop = gPanelObj.KeyMatric[S$KEYMATRIX_AUTO_STOP];
		
	/* ------ dry cycle ------- */
	if(1 == DrycyleReq && 0 == PosDrycyleReq)
	{
		gMacOption.bDryCycle = !gMacOption.bDryCycle;
	}

	/* machine auto reset request  */
	//	if(1 == gTKmanual.Status) 
	//	{
	//		gMachineIn.Reset = gPanelObj.KeyMatric[S$KEYMATRIX_AUTO_RESET];
	//	}
	//	else
	//	{
	//		gMachineIn.Reset = 0;
	//	}
	
	if(DelayTime > 0)DelayTime--;
	/* ------------------------ manual --- autorun --- calibration step ------------------------ */
	switch(Step)
	{
		case REQUEST_NOTHING:
			if(0 == gMachineIn.MainControl || gPanelObj.GetPicture == PIC_INIT || gPanelObj.GetPicture == PIC_SCREEN_SAVE)
			{
				break;
			}
			
			oldCalibModeForceDI = gMachineIn.CalibModeForceDI;
			/* switch to manual mode  */
			if(1 == EDGEPOS(ManualReq,OldManualReq))
			{
				if(1 == gTKauto.Status)
				{
					Step = REQUEST_MANUAL;
				}
				else
				{
					if(0 == gTKmanual.Status)
					{
						Step = REQUEST_MANUAL;
					}
				}
			}

			/* switch to calibration mode  */
			if(1 == EDGEPOS(AdjustReq,OldAdjustReq))
			{
				//	gPanelObj.SetPicture =PIC_CALIB_SETTING;
				if(1 == gTKauto.Status)
				{
					Step = REQUEST_ADJUST;
				}

				if(1 == gTKmanual.Status)
				{
					Step = REQUEST_ADJUST + 100;
				}
			}

			/* switch to autorun mode  */
			if(1 == EDGEPOS(AutoReq,OldAutoReq))
			{
				if(1 == gTKmanual.Status|| 1 == gTKcalib.Status)
				{
					/*if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)*/
					if(1 == gTKmanual.Status || 1 == gTKcalib.Status)				// Both manual and calibration mode can switch to Auto
					{
						if((0 == gMachineInfo.TimeStopMachine)&&
							/*(1 == LMold.InBasicPos)             &&*//*Albert*/
							/*(1 == RMold.InBasicPos)             &&*//*Albert*/
							(1 == gMachineOut.OilPump)          &&
							(1 == gMachineOut.PneumaticValve))
							/*   && (1 == gMachineOut.MotionPower))*/
						{
							if(gMachineInfo.OilTemp < gMachineFix.OilCooling.OilTempAlarm)  // Avoid Oil temperature too  high
							{
								Step = REQUEST_AUTO;
							}
						}
						else
						{
							gImply.TimeStopMachine = gMachineInfo.TimeStopMachine;

							gImply.OilPumpNotOn = !gMachineOut.OilPump;
							gImply.PneumaticValveOff = !gMachineOut.PneumaticValve;

							//							gImply.LCarriageBw      = !LMold.Carriage.BwPos;
							//							gImply.LClampOpn	      = !LMold.Clamp.OpnPos;
							//							gImply.LPinUp           = !LMold.BlowPin.UpPos;
							//							gImply.LTopDeflashBw    = !LMold.TransDIn.TopDeflashBwLimit;
							//							gImply.LBottomDeflashBw = !LMold.TransDIn.BottomDeflashBwLimit;
							//							gImply.LCoolPinUp       = !LMold.TransDIn.CoolPinUpLimit;
							//							gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
							//							gImply.LClampCalErr     = !LMold.Clamp.CalOK;
							//							gImply.LBlowPinCalErr   = !LMold.BlowPin.CalOK;
							//							gImply.L_RobotTransferCalErr = !LMold.Robot.Transfer.CalOK;
							//							gImply.L_RobotUpDnCalErr  = !LMold.Robot.UpDn.CalOK;
							//							gImply.LInnerPinBw 	   = !LMold.TransDIn.InnerPinBwLimit;
							//						//	gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimit;
							//							gImply.LRobotBw    	   = !LMold.Robot.Transfer.BwPos;
							//							gImply.LRobotUp    	   = !LMold.Robot.UpDn.UpPos;
							//							if (1 == LMold.Option.SubMold )
							//							{
							//	 							gImply.LSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;
							//							}
							//							if(1 == LMold.Option.BottomSubMold)
							//							{
							//								gImply.LBottomSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;			
							//							}
							//							gImply.LCutNeckBw 	   = !LMold.TransDIn.CutNeckOpnLimit;
							//							gImply.LSealerOpn       = !LMold.TransDIn.SealOpnLimit
							//							gImply.LPinPartFw       = !LMold.TransDIn.PinPartFwLimit;  
							//							
							//							gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimitR | !LMold.TransDIn.RobotOpnLimitL;
							//							gImply.LTopDeflashOpn   = !LMold.TransDIn.TopDeflashOpnLimit;
							//					
							//							
							//							gImply.RCarriageBw      = !RMold.Carriage.BwPos;
							//							gImply.RClampOpn	      = !RMold.Clamp.OpnPos;
							//							gImply.RPinUp           = !RMold.BlowPin.UpPos;
							//							gImply.RTopDeflashBw    = !RMold.TransDIn.TopDeflashBwLimit;
							//							gImply.RBottomDeflashBw = !RMold.TransDIn.BottomDeflashBwLimit;
							//							gImply.RCoolPinUp       = !RMold.TransDIn.CoolPinUpLimit;
							//							gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
							//							gImply.RClampCalErr     = !RMold.Clamp.CalOK;
							//							gImply.RBlowPinCalErr   = !RMold.BlowPin.CalOK;
							//							gImply.R_RobotTransferCalErr = !RMold.Robot.Transfer.CalOK;
							//							gImply.R_RobotUpDnCalErr  = !RMold.Robot.UpDn.CalOK;
							//							gImply.RInnerPinBw 	   = !RMold.TransDIn.InnerPinBwLimit;
							//						//	gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimit;
							//							gImply.RRobotBw    	   = !RMold.Robot.Transfer.BwPos;
							//							gImply.RRobotUp    	   = !RMold.Robot.UpDn.UpPos;
							//							
							//							if (1 == RMold.Option.SubMold )
							//							{
							//								gImply.RSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;
							//							}
							//							if(1 == RMold.Option.BottomSubMold)
							//							{
							//								gImply.RBottomSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;			
							//							}
							//							
							//							gImply.RCutNeckBw 	   = !RMold.TransDIn.CutNeckOpnLimit;
							//							gImply.RSealerOpn       = !RMold.TransDIn.SealOpnLimit;
							//							gImply.RPinPartFw       = !RMold.TransDIn.PinPartFwLimit;
							//                     
							//							gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimitR | !RMold.TransDIn.RobotOpnLimitL;
							//							gImply.RTopDeflashOpn=		!RMold.TransDIn.TopDeflashOpnLimit;
					
							
							if(1 == gMachineInfo.PermitAuto)gPanelObj.SetPicture = PIC_AUTO_START;
						}
					}/* if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable && 1 == gMachineOut.OilPump) */
					else if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable)
					{
						if((0 == gMachineInfo.TimeStopMachine) &&
							/*(1 == RMold.InBasicPos  )           &&*/ /*Albert*/
							(1 == gMachineOut.OilPump)          &&
							(1 == gMachineOut.PneumaticValve))
						{
							if(1 == LMold.Carriage.BwPos)
							{
								Step = REQUEST_AUTO;
							}
							else
							{
								gImply.LCarriageBw = !LMold.Carriage.BwPos;
							}
						}
						else
						{
							gImply.TimeStopMachine = gMachineInfo.TimeStopMachine;

							gImply.OilPumpNotOn = !gMachineOut.OilPump;
							gImply.PneumaticValveOff = !gMachineOut.PneumaticValve;

							//							gImply.RCarriageBw      = !RMold.Carriage.BwPos;
							//							gImply.RClampOpn	      = !RMold.Clamp.OpnPos;
							//							gImply.RPinUp           = !RMold.BlowPin.UpPos;
							//							gImply.RTopDeflashBw    = !RMold.TransDIn.TopDeflashBwLimit;
							//							gImply.RBottomDeflashBw = !RMold.TransDIn.BottomDeflashBwLimit;
							//							gImply.RCoolPinUp       = !RMold.TransDIn.CoolPinUpLimit;
							//							gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
							//							gImply.RClampCalErr     = !RMold.Clamp.CalOK;
							//							gImply.RBlowPinCalErr  = !LMold.BlowPin.CalOK;
							//							gImply.R_RobotTransferCalErr = !RMold.Robot.Transfer.CalOK;
							//							gImply.R_RobotUpDnCalErr  = !RMold.Robot.UpDn.CalOK;
							//                     gImply.RInnerPinBw 	   = !RMold.TransDIn.InnerPinBwLimit;
							//						//	gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimit;
							//							gImply.RRobotBw    	   = !RMold.Robot.Transfer.BwPos;
							//							gImply.RRobotUp    	   = !RMold.Robot.UpDn.UpPos;
							//						
							//							if (1 == RMold.Option.SubMold )
							//							{
							//								gImply.RSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;
							//							}
							//							if(1 == RMold.Option.BottomSubMold)
							//							{
							//								gImply.RBottomSubMoldFw      = !RMold.TransDIn.SubMoldClsLimit;			
							//							}
							//							gImply.RCutNeckBw 	   = !RMold.TransDIn.CutNeckOpnLimit;
							//							gImply.RSealerOpn       = !RMold.TransDIn.SealOpnLimit;
							//							gImply.RPinPartFw       = !RMold.TransDIn.PinPartFwLimit;
							//                     
							//							gImply.RRobotOpn   	   = !RMold.TransDIn.RobotOpnLimitR || !RMold.TransDIn.RobotOpnLimitL;
							//							gImply.RTopDeflashOpn=		!RMold.TransDIn.TopDeflashOpnLimit;
							
							if(1 == gMachineInfo.PermitAuto)gPanelObj.SetPicture = PIC_AUTO_START;

						}/* if((1 == RMold.InBasicPos )&& (1 == gMachineOut.OilPump))*/
					}/* else if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable) */
					else if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable))
					{
						if((0 == gMachineInfo.TimeStopMachine) &&
							/*(1 == LMold.InBasicPos)             &&*/ /*Albert*/
							(1 == gMachineOut.OilPump)          &&
							(1 == gMachineOut.PneumaticValve))
						{
							if(1 == RMold.Carriage.BwPos)
							{
								Step = REQUEST_AUTO;
							}
							else
							{
								gImply.RCarriageBw = !LMold.Carriage.BwPos;
							}
						}
						else
						{
							gImply.TimeStopMachine = gMachineInfo.TimeStopMachine;

							gImply.OilPumpNotOn  = !gMachineOut.OilPump;
							gImply.PneumaticValveOff = !gMachineOut.PneumaticValve;

							//							gImply.LCarriageBw      = !LMold.Carriage.BwPos;
							//							gImply.LClampOpn	      = !LMold.Clamp.OpnPos;
							//							gImply.LPinUp           = !LMold.BlowPin.UpPos;
							//							gImply.LTopDeflashBw    = !LMold.TransDIn.TopDeflashBwLimit;
							//							gImply.LBottomDeflashBw = !LMold.TransDIn.BottomDeflashBwLimit;
							//							gImply.LCoolPinUp       = !LMold.TransDIn.CoolPinUpLimit;
							//							gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
							//							gImply.LClampCalErr     = !LMold.Clamp.CalOK;
							//							gImply.LBlowPinCalErr  = !LMold.BlowPin.CalOK;
							//							gImply.L_RobotTransferCalErr = !LMold.Robot.Transfer.CalOK;
							//							gImply.L_RobotUpDnCalErr  = !LMold.Robot.UpDn.CalOK;
							//                     gImply.LInnerPinBw 	   = !LMold.TransDIn.InnerPinBwLimit;
							//						//	gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimit;
							//							
							//							gImply.LRobotBw    	   = !LMold.Robot.Transfer.BwPos;
							//							gImply.LRobotUp    	   = !LMold.Robot.UpDn.UpPos;
							//							
							//							if (1 == LMold.Option.SubMold)
							//							{
							//								gImply.LSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;
							//							}
							//							if(1 == RMold.Option.BottomSubMold)
							//							{
							//								gImply.LBottomSubMoldFw      = !LMold.TransDIn.SubMoldClsLimit;			
							//							}
							//							gImply.LCutNeckBw 	   = !LMold.TransDIn.CutNeckOpnLimit;
							//							gImply.LSealerOpn	      = !LMold.TransDIn.SealOpnLimit;
							//							gImply.LPinPartFw       = !LMold.TransDIn.PinPartFwLimit;
							//                     
							//						gImply.LRobotOpn   	   = !LMold.TransDIn.RobotOpnLimitR || !LMold.TransDIn.RobotOpnLimitL;
							//							gImply.LTopDeflashOpn=		!LMold.TransDIn.TopDeflashOpnLimit;
							
							if(1 == gMachineInfo.PermitAuto)gPanelObj.SetPicture = PIC_AUTO_START;

						}/* if(1 == LMold.InBasicPos && 1 == gMachineOut.OilPump) */
					}/* else if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable))*/
				}/* if(1 == gTKmanual.Status) */
			}/* if(1 == EDGEPOS(AutolReq,OldAutolReq)) */

			/* machine auto reset  */
			if((1 == EDGEPOS(gMachineIn.Reset,NegReset)) && (0 == gMachineOut.AutoRun))/*Albert chage positive edge*/
			{
				gMachineIn.Reset = 0;/*Albert*/
				if(/*1 == gTKmanual.Status    	&&*/ 
					1 == gMachineIn.SafeGate 	&& 
					0 == DelayTime           	&&
					1 == gMachineOut.OilPump &&
					1 == RMold.Carriage.CalOK	&&
					1 == LMold.Carriage.CalOK  &&
					1 == RMold.Clamp.CalOK     &&
					1 == LMold.Clamp.CalOK 		&&
					(1 == gMacOption.RBlowPin && 1 == gMacOption.LBlowPin
					&& (gMacOption.RBlowPin2 || 0 == gMachineFix.Option.HWEnable.RMold.BlowPin2) 
					&& (gMacOption.LBlowPin2 || 0 == gMachineFix.Option.HWEnable.LMold.BlowPin2))
					)
				{
					Step = REQUEST_RESET;
				}
				else
				{
					gImply.OilPumpNotOn     = !gMachineOut.OilPump;
					gImply.SafeGate         = !gMachineIn.SafeGate;
					gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
					gImply.RClampCalErr     = !RMold.Clamp.CalOK;
					gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
					gImply.LClampCalErr     = !LMold.Clamp.CalOK;
					
					//double blowpin
					gImply.RBlowPinEnable     = !gMacOption.RBlowPin;
					gImply.RBlowPin2Enable    = !gMacOption.RBlowPin2;
					gImply.LBlowPinEnable     = !gMacOption.LBlowPin;
					gImply.LBlowPin2Enable    = !gMacOption.LBlowPin2;
					
				}
			}
			
			/* machine blow pin calibration */
			if(1 == EDGENEG(CalibPinReq,NegCalibPinReq))
			{
				if(1 == gTKcalib.Status 		&& 
					1 == gMachineIn.SafeGate 	&& 
					0 == DelayTime             &&
					1 == RMold.Carriage.CalOK	&&
					1 == LMold.Carriage.CalOK  &&
					1 == RMold.Clamp.CalOK     &&
					1 == LMold.Clamp.CalOK)
				{
					Step = REQUEST_PIN_CLAIB;
				}
				else
				{
					gImply.SafeGate         = !gMachineIn.SafeGate;
					gImply.RCarriageCalErr  = !RMold.Carriage.CalOK;
					gImply.RClampCalErr     = !RMold.Clamp.CalOK;
					gImply.LCarriageCalErr  = !LMold.Carriage.CalOK;
					gImply.LClampCalErr     = !LMold.Clamp.CalOK;
				}
			}
			break;
		
		/* ---------------------- Switch to manual mode ------------------- */
		case REQUEST_MANUAL:						/*  自动内部故障停止 */
			LMold.LowBlow.Step		   = 20000;
			LMold.HighBlow.Step		   = 20000;
			LMold.BlowPin.Step		= 20000;
			LMold.BlowPin2.Step		= 20000;
			LMold.BottomPin.Step    = 20000;
			LMold.Carriage.Step		= 20000;
			LMold.Clamp.Step		   = 20000;
			LMold.CoolPin.Step	   = 20000;
			LMold.CoolPinBlow.Step	= 20000;
			LMold.CoolPin2.Step	   = 20000;
			LMold.CoolPin2Blow.Step	= 20000;
			LMold.CoolPin3.Step	   = 20000;
			LMold.CoolPin3Blow.Step	= 20000;
			LMold.BlowDeflash.Step	= 20000;
			LMold.CoolDeflash.Step	= 20000;
			LMold.Cutter.Step		   = 20000;
			LMold.TopDeflash.Step   = 20000;
			LMold.BottomDeflash.Step= 20000;
			LMold.SupportAir.Step	= 20000;
			LMold.SupportSuckAir.Step	= 20000;
			LMold.ExtrLift.Step		= 20000;
			LMold.Lock.Step			= 20000;
			LMold.PullBottom.Step	= 20000;
			LMold.InnerPin.Step	   = 20000;
			LMold.CutNeck.Step	   = 20000;
			LMold.Screw.Step        = 20000;
			LMold.Robot.Step	      = 20000;
			LMold.RobotOpnCls.Step	   = 20000;
			LMold.ChuteDeflashBlow.Step = 20000;
			LMold.SubMold.Step	   = 20000;
			LMold.HookPin.Step		= 20000;

			RMold.LowBlow.Step		   = 20000;
			RMold.HighBlow.Step		   = 20000;
			RMold.BottomPin.Step    = 20000;
			RMold.BlowPin.Step		= 20000;
			RMold.BlowPin2.Step		= 20000;
			RMold.Carriage.Step		= 20000;
			RMold.Clamp.Step		   = 20000;
			RMold.CoolPin.Step	   = 20000;
			RMold.CoolPinBlow.Step	= 20000;
			RMold.CoolPin2.Step	   = 20000;
			RMold.CoolPin2Blow.Step	= 20000;
			RMold.CoolPin3.Step	   = 20000;
			RMold.CoolPin3Blow.Step	= 20000;
			RMold.BlowDeflash.Step	= 20000;
			RMold.CoolDeflash.Step	= 20000;
			RMold.Cutter.Step		   = 20000;
			RMold.TopDeflash.Step	= 20000;
			RMold.BottomDeflash.Step= 20000;
			RMold.SupportAir.Step	= 20000;
			RMold.SupportSuckAir.Step	= 20000;
			RMold.ExtrLift.Step		= 20000;
			RMold.Lock.Step			= 20000;
			RMold.PullBottom.Step	= 20000;
			RMold.InnerPin.Step		= 20000;
			RMold.CutNeck.Step	   = 20000;
			RMold.Screw.Step        = 20000;
			RMold.Robot.Step	      = 20000;
			RMold.RobotOpnCls.Step    = 20000;
			RMold.ChuteDeflashBlow.Step = 20000;
			RMold.SubMold.Step	   = 20000;
			RMold.HookPin.Step		= 20000;
		
				
			Step = REQUEST_MANUAL + 100;
			break;

		case REQUEST_MANUAL + 100:
			gPROCESS_info.mode_req = MODE_MANUAL;
			Step = REQUEST_MANUAL + 200;
			break;

		case REQUEST_MANUAL + 200:
			if(1 == gTKmanual.Status)
			{
				LMold.AutoStep = 0;
				RMold.AutoStep = 0;

				Step = REQUEST_NOTHING;
			}
			break;

		/*  ------------------- Switch to auto mode ---------------------- */
		case REQUEST_AUTO:
			gPROCESS_info.mode_req = MODE_AUTO;
			Step = REQUEST_NOTHING;
			break;

		case REQUEST_ADJUST:
			LMold.LowBlow.Step		= 20000;
			LMold.BottomPin.Step    = 20000;
			LMold.HighBlow.Step		= 20000;
			LMold.BlowPin.Step		= 20000;
			LMold.BlowPin2.Step		= 20000;
			LMold.BottomPin.Step    = 20000;
			LMold.Carriage.Step		= 20000;
			LMold.Clamp.Step		   = 20000;
			LMold.CoolPin.Step	   = 20000;
			LMold.CoolPinBlow.Step	= 20000;
			LMold.CoolPin2.Step	   = 20000;
			LMold.CoolPin2Blow.Step	= 20000;
			LMold.CoolPin3.Step	   = 20000;
			LMold.CoolPin3Blow.Step	= 20000;

			LMold.BlowDeflash.Step	= 20000;
			LMold.CoolDeflash.Step	= 20000;
			LMold.Cutter.Step		   = 20000;
			LMold.TopDeflash.Step   = 20000;
			LMold.BottomDeflash.Step= 20000;
			LMold.SupportAir.Step	= 20000;
			LMold.SupportSuckAir.Step	= 20000;
			LMold.ExtrLift.Step		= 20000;
			LMold.Lock.Step			= 20000;
			LMold.PullBottom.Step	= 20000;
			LMold.InnerPin.Step	   = 20000;
			LMold.CutNeck.Step	   = 20000;
			LMold.Screw.Step        = 20000;
			LMold.Robot.Step	      = 20000;
			LMold.RobotOpnCls.Step	  = 20000;
			LMold.ChuteDeflashBlow.Step = 20000;
			LMold.SubMold.Step		= 20000;
			LMold.LengthCtrl.Step	= 20000;
			LMold.HookPin.Step		= 20000;

			RMold.LowBlow.Step		= 20000;
			RMold.HighBlow.Step		= 20000;
			RMold.BlowPin.Step		= 20000;
			RMold.BlowPin2.Step		= 20000;
			RMold.BottomPin.Step    = 20000;
			RMold.Carriage.Step		= 20000;
			RMold.Clamp.Step		   = 20000;
			RMold.CoolPin.Step	   = 20000;
			RMold.CoolPinBlow.Step	= 20000;
			RMold.CoolPin2.Step	   = 20000;
			RMold.CoolPin2Blow.Step	= 20000;
			RMold.CoolPin3.Step	   = 20000;
			RMold.CoolPin3Blow.Step	= 20000;
			RMold.BlowDeflash.Step	= 20000;
			RMold.CoolDeflash.Step	= 20000;
			RMold.Cutter.Step		   = 20000;
			RMold.TopDeflash.Step	= 20000;
			RMold.BottomDeflash.Step= 20000;
			RMold.SupportAir.Step	= 20000;
			RMold.SupportSuckAir.Step	= 20000;
			RMold.ExtrLift.Step		= 20000;
			RMold.Lock.Step			= 20000;
			RMold.PullBottom.Step	= 20000;
			RMold.InnerPin.Step		= 20000;
			RMold.CutNeck.Step	   = 20000;
			RMold.Screw.Step        = 20000;
			RMold.Robot.Step	      = 20000;
			RMold.RobotOpnCls.Step	  = 20000;
			RMold.ChuteDeflashBlow.Step = 20000;
			RMold.SubMold.Step		= 20000;
			RMold.LengthCtrl.Step	= 20000;
			RMold.HookPin.Step		= 20000;

			Step = REQUEST_ADJUST + 100;
			break;

		case REQUEST_ADJUST + 100:
			LMold.SupportAir.Step	= 20000;
			RMold.SupportAir.Step	= 20000;
			LMold.SupportSuckAir.Step	= 20000;
			RMold.SupportSuckAir.Step	= 20000;
			gPROCESS_info.mode_req = MODE_ADJUST;

			Step = REQUEST_ADJUST + 200;
			break;

		case REQUEST_ADJUST + 200:
			if(1 == gTKcalib.Status)
			{
				LMold.AutoStep = 0;
				RMold.AutoStep = 0;

				Step = REQUEST_NOTHING;
			}
			break;

		/*  ------------------- Switch to reset mode ---------------------- */
		case REQUEST_RESET :
			if(1 == RMold.Option.Enable)
			{
				RMold.ResetStep = 100;
			}
			else
			{
				RMold.ResetStep = 3000;
			}
			gMachineInfo.MachineReset = 1;
			Step = REQUEST_RESET + 100;
			break;

		case REQUEST_RESET + 100:
			gMachineInfo.MachineReset = 1;
			
			if(3000 == RMold.ResetStep)
			{
			RMold.ResetStep = 0;
			Step = REQUEST_RESET + 200;
			}

			if(60000 == RMold.ResetStep)
			{
			gMachineInfo.MachineReset = 0;
			RMold.ResetStep = 0;

			gImply.ResetFalure = 1;
			//	Step = REQUEST_NOTHING;
			gPanelObj.SetPicture = PIC_AUTO_START;  //ipis1115
			Step = REQUEST_MANUAL;
			}
			
			/* switch to manual mode  */
			if(1 == EDGEPOS(ManualReq,OldManualReq))
			{
				gMachineInfo.MachineReset = 0;
				LMold.ResetStep = 0;

				gImply.ResetFalure = 1;
				Step = REQUEST_MANUAL;
			}

			/* switch to calibration mode  */
			if(1 == EDGEPOS(AdjustReq,OldAdjustReq))
			{
				gMachineInfo.MachineReset = 0;
				LMold.ResetStep = 0;

				gImply.ResetFalure = 1;
				Step = REQUEST_ADJUST;
			}
			break;

		case REQUEST_RESET + 200:
			if(1 == LMold.Option.Enable)
			{
				LMold.ResetStep = 100;
			}
			else
			{
				LMold.ResetStep = 3000;
			}
			Step = REQUEST_RESET + 300;
			break;

		case REQUEST_RESET + 300:
			gMachineInfo.MachineReset = 1;
			if(3000 == LMold.ResetStep)
			{
			LMold.ResetStep = 0;
			Step = REQUEST_RESET + 400;
			}

			if(60000 == LMold.ResetStep)
			{
			gMachineInfo.MachineReset = 0;
			LMold.ResetStep = 0;

			gImply.ResetFalure = 1;
			//	Step = REQUEST_NOTHING;
			gPanelObj.SetPicture = PIC_AUTO_START;  //ipis1115
			Step = REQUEST_MANUAL;
			}
			
			/* switch to manual mode  */
			if(1 == EDGEPOS(ManualReq,OldManualReq))
			{
				gMachineInfo.MachineReset = 0;
				LMold.ResetStep = 0;

				gImply.ResetFalure = 1;
				Step = REQUEST_MANUAL;
			}

			/* switch to calibration mode  */
			if(1 == EDGEPOS(AdjustReq,OldAdjustReq))
			{
				gMachineInfo.MachineReset = 0;
				LMold.ResetStep = 0;

				gImply.ResetFalure = 1;
				Step = REQUEST_ADJUST;
			}
			break;

		case REQUEST_RESET + 400:
			gMachineInfo.MachineReset = 0;
			gImply.ResetSuccess = 1;
			Step = REQUEST_NOTHING;
			break;
		
		/*  ------------------- Switch to blow pin calibration mode ---------------------- */
		case REQUEST_PIN_CLAIB :
			if(1 == RMold.Option.Enable)
			{
				RMold.PinCalibStep = 100;
			}
			else
			{
				RMold.PinCalibStep = 3000;
			}
			gMachineInfo.PinCalibration =1;
			
			Step = REQUEST_PIN_CLAIB + 100;
			break;
		
		case REQUEST_PIN_CLAIB + 100:
			gMachineInfo.PinCalibration = 1;
			
			if(3000 == RMold.PinCalibStep)
			{
				RMold.PinCalibStep = 0;
				Step = REQUEST_PIN_CLAIB + 200;
			}

			if(60000 == RMold.PinCalibStep)
			{
				gMachineInfo.PinCalibration = 0;
				RMold.PinCalibStep = 0;

				gImply.PinCalibFalse = 1;
				Step = REQUEST_NOTHING;
			}
			break;

		case REQUEST_PIN_CLAIB + 200:
			if(1 == LMold.Option.Enable)
			{
				LMold.PinCalibStep = 100;
			}
			else
			{
				LMold.PinCalibStep = 3000;
			}
			Step = REQUEST_PIN_CLAIB + 300;
			break;

		case REQUEST_PIN_CLAIB + 300:
			gMachineInfo.PinCalibration = 1;
			
			if(3000 == LMold.PinCalibStep)
			{
				LMold.PinCalibStep = 0;
				Step = REQUEST_PIN_CLAIB + 400;
			}

			if(60000 == LMold.PinCalibStep)
			{
				gMachineInfo.PinCalibration = 0;
				LMold.PinCalibStep = 0;

				gImply.PinCalibFalse = 1;
				Step = REQUEST_NOTHING;
			}
			break;
		
		case REQUEST_PIN_CLAIB + 400:
			gMachineInfo.PinCalibration = 0;
			gImply.PinCalibSuccess = 1;
			Step = REQUEST_NOTHING;
			break;

		/*  ------------------- Switch to stop mode ---------------------- */
		case 30000:
			gMachineInfo.MachineReset = 0;
			
			memset(&LMold.ValveOut,0,sizeof(LMold.ValveOut));
			memset(&RMold.ValveOut,0,sizeof(RMold.ValveOut));

			LMold.AutoStep			= 30000;
			LMold.ResetStep		= 30000;
			LMold.LowBlow.Step	= 30000;
			LMold.HighBlow.Step	= 30000;
			LMold.BottomPin.Step = 30000;
         
			if(LMold.BlowPin.Step > 0 && LMold.BlowPin.Step < 20000)
			{
				LMold.BlowPin.Step = 30000;
			}
			if(LMold.BlowPin2.Step > 0 && LMold.BlowPin2.Step < 20000)
			{
				LMold.BlowPin2.Step = 30000;
			}
			if(LMold.Carriage.Step > 0 && LMold.Carriage.Step < 20000)
			{
				LMold.Carriage.Step= 30000;
			}
			if(LMold.Clamp.Step > 0 && LMold.Clamp.Step < 20000)
			{
				LMold.Clamp.Step = 30000;
			}

			LMold.CoolPin.Step		= 30000;
			LMold.CoolPinBlow.Step	= 30000;
			LMold.CoolPin2.Step	   	= 30000;
			LMold.CoolPin2Blow.Step	= 30000;
			LMold.CoolPin3.Step	   	= 30000;
			LMold.CoolPin3Blow.Step	= 30000;
			LMold.BlowDeflash.Step	= 30000;
			LMold.CoolDeflash.Step	= 30000;
			LMold.Cutter.Step		= 30000;
			LMold.TopDeflash.Step	= 30000;
			LMold.BottomDeflash.Step= 30000;
			LMold.SupportAir.Step	= 30000;
			LMold.SupportSuckAir.Step	= 30000;
			LMold.ExtrLift.Step		= 30000;
			LMold.Lock.Step			= 30000;
			LMold.PullBottom.Step	= 30000;
			LMold.InnerPin.Step		= 30000;
			LMold.CutNeck.Step   	= 30000;
			LMold.Screw.Step        = 30000;
			LMold.Robot.Step	    = 30000;
			LMold.RobotOpnCls.Step	= 30000;
			LMold.ChuteDeflashBlow.Step = 30000;
			LMold.SubMold.Step	   	= 30000;
			LMold.LengthCtrl.Step	= 30000;
			LMold.HookPin.Step		= 30000;

			RMold.AutoStep			= 30000;
			RMold.ResetStep			= 30000;
			RMold.LowBlow.Step		= 30000;
			RMold.HighBlow.Step		= 30000;
			RMold.BottomPin.Step    = 30000;
         
			if(RMold.BlowPin.Step > 0 && RMold.BlowPin.Step < 20000)
			{
				RMold.BlowPin.Step	= 30000;
			}
			if(RMold.BlowPin2.Step > 0 && RMold.BlowPin2.Step < 20000)
			{
				RMold.BlowPin2.Step	= 30000;
			}
			if(RMold.Carriage.Step > 0 && RMold.Carriage.Step < 20000)
			{
				RMold.Carriage.Step	= 30000;
			}
			if(RMold.Clamp.Step > 0 && RMold.Clamp.Step < 20000)
			{
				RMold.Clamp.Step    = 30000;
			}

			RMold.CoolPin.Step	   	= 30000;
			RMold.CoolPinBlow.Step	= 30000;
			RMold.CoolPin2.Step	   	= 30000;
			RMold.CoolPin2Blow.Step	= 30000;
			RMold.CoolPin3.Step	   	= 30000;
			RMold.CoolPin3Blow.Step	= 30000;
			RMold.BlowDeflash.Step	= 30000;
			RMold.CoolDeflash.Step	= 30000;
			RMold.Cutter.Step		= 30000;
			RMold.TopDeflash.Step   = 30000;
			RMold.BottomDeflash.Step= 30000;
			RMold.SupportAir.Step	= 30000;
			RMold.SupportSuckAir.Step	= 30000;
			RMold.ExtrLift.Step		= 30000;
			RMold.Lock.Step			= 30000;
			RMold.PullBottom.Step	= 30000;
			RMold.InnerPin.Step		= 30000;
			RMold.CutNeck.Step	   	= 30000;
			RMold.Screw.Step        = 30000;
			RMold.Robot.Step	    = 30000;
			RMold.RobotOpnCls.Step	= 30000;
			RMold.ChuteDeflashBlow.Step = 30000;
			RMold.SubMold.Step		= 30000;
			RMold.LengthCtrl.Step	= 20000;
			RMold.HookPin.Step		= 30000;

			gMachineOut.OilPump = 0;
			gMachineOut.OilPumpContactor = 0;
			
			gMachineOut.ParisonPump = 0;
			gMachineOut.OilCoolingPump = 0;

			gMachineOut.AccuRelieveValve = 0;

			if(1 == gTKauto.Status)
			{
				LMold.AutoReqStop = 0;
				RMold.AutoReqStop = 0;
				gPROCESS_info.mode_req = MODE_MANUAL;
			}

			if(1 == MachineStop)
			{
				MachineStop = 0;
				Step = REQUEST_ADJUST;
			}
			else
			{
				if(0 == gMachineIn.InstancyStop && 0 == gMachineInfo.SafeInStop)
				{
					Step = REQUEST_NOTHING;
				}
			}
			break;
	}/* switch(Step) */

	/* --------------------- Auto reset stop --------------------- */
	//	if(Step > REQUEST_RESET && Step < REQUEST_RESET + 400)
	//	{
	//		if(1 == EDGEPOS(gMachineIn.Reset,PosReset))
	//		{
	//			if(LMold.ResetStep > 100 && LMold.ResetStep <3000)LMold.ResetStep = 20000;
	//			if(RMold.ResetStep > 100 && RMold.ResetStep <3000)RMold.ResetStep = 20000;
	//
	//			gMachineInfo.MachineReset = 0;
	//			DelayTime = 200;
	//
	//			Step = REQUEST_NOTHING;
	//		}
	//	}/* if(Step >= REQUEST_RESET && Step < REQUEST_RESET + 400) */

	gImply.Reset = gMachineInfo.MachineReset;
	
	/* --------------------- pin calibration request stop --------------------- */
	if(Step > REQUEST_PIN_CLAIB && Step < REQUEST_PIN_CLAIB + 400)
	{
		if(1 == EDGEPOS(CalibPinReq,PosCalibPinReq))
		{
			if(LMold.PinCalibStep > 100 && LMold.PinCalibStep <3000)LMold.PinCalibStep = 20000;
			if(RMold.PinCalibStep > 100 && RMold.PinCalibStep <3000)RMold.PinCalibStep = 20000;

			gMachineInfo.PinCalibration = 0;
			DelayTime = 200;

			Step = REQUEST_NOTHING;
		}
	}/* if(Step >= REQUEST_PIN_CLAIB && Step < REQUEST_PIN_CLAIB + 400) */
	
	gImply.PinCalib = gMachineInfo.PinCalibration;

	/*  ----------------------  Run mode led ------------------- */
	gMachineInfo.RunStatus = 0;

	if(1 == gTKmanual.Status)
	{
		gMachineInfo.Manual = 1;
		gMachineInfo.Calib  = 0;
		gMachineInfo.Auto   = 0;
		
		gMachineInfo.TimeLimit = 1;  //手動中也要偵測時間

		//		if(Step >= REQUEST_RESET && Step < REQUEST_RESET + 400)
		//		{
		//			gMachineInfo.TimeLimit = 1;
		//		}
		//		else
		//		{
		//			gMachineInfo.TimeLimit = 0;
		//		}

		gMachineInfo.RunStatus = 1;
	}

	if(1 == gTKauto.Status)
	{
		gMachineInfo.Manual = 0;
		gMachineInfo.Calib  = 0;
		gMachineInfo.Auto   = 1;

		gMachineInfo.TimeLimit= 1;

		gMachineInfo.RunStatus = 2;
	}

	if(1 == gTKcalib.Status)
	{
		gMachineInfo.Manual = 0;
		gMachineInfo.Calib  = 1;
		gMachineInfo.Auto   = 0;

		gMachineInfo.TimeLimit = 0;

		if(1 == gMachineIn.CalibMode)
		{
			gMachineInfo.RunStatus = 3;
		}
		else
		{
			gMachineInfo.RunStatus = 4;
		}
		
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
		{
			if(18500 == LMold.Carriage.Step && 8500 == LMold.Clamp.Step &&
				18500 == RMold.Carriage.Step && 8500 == RMold.Clamp.Step)
			{
				gMachineInfo.LedPinCalibMode = 1;
			}
			else
			{
				if(1 == gMachineInfo.PinCalibration)
				{
					gMachineInfo.LedPinCalibMode = 2;
				}
				else
				{
					gMachineInfo.LedPinCalibMode = 0;
				}
			}
		}
		else if(1 == RMold.Option.Enable && 0 == LMold.Option.Enable)
		{
			if(18500 == RMold.Carriage.Step && 8500 == RMold.Clamp.Step)
			{
				gMachineInfo.LedPinCalibMode = 1;
			}
			else
			{
				if(1 == gMachineInfo.PinCalibration)
				{
					gMachineInfo.LedPinCalibMode = 2;
				}
				else
				{
					gMachineInfo.LedPinCalibMode = 0;
				}
			}
		}
		else
		{
			if(18500 == LMold.Carriage.Step && 8500 == LMold.Clamp.Step)
			{
				gMachineInfo.LedPinCalibMode = 1;
			}
			else
			{
				if(1 == gMachineInfo.PinCalibration)
				{
					gMachineInfo.LedPinCalibMode = 2;
				}
				else
				{
					gMachineInfo.LedPinCalibMode = 0;
				}
			}
		}
	}
	else
	{
		gMachineInfo.LedPinCalibMode = 0;
	}

	if(1 == gTKmanual.Status)
	{
		if(1 == RMold.Option.Enable && 1 == LMold.Option.Enable)
		{
			if(1 == LMold.InBasicPos && 1 == RMold.InBasicPos && 1 == gMachineOut.OilPump)
			{
				gMachineInfo.PermitAuto = 2;
			}
			else
			{
				gMachineInfo.PermitAuto = 1;
			}
			
			if(1 == LMold.InBasicPos && 1 == RMold.InBasicPos)
			{
				gMachineInfo.LedReset = 1;
			}
			else
			{
				if(Step >= REQUEST_RESET && Step < REQUEST_RESET + 400)
				{
					gMachineInfo.LedReset = 2;
				}
				else
				{
					gMachineInfo.LedReset = 0;
				}
			}
		}

		if((1 == RMold.Option.Enable) && (0 == LMold.Option.Enable))
		{
			if((1 == RMold.InBasicPos ) && (1 ==LMold.Carriage.BwPos))
			{
				gMachineInfo.PermitAuto = 2;
			}
			else
			{
				gMachineInfo.PermitAuto = 1;
			}
			
			if(1 == RMold.InBasicPos && 1 ==LMold.Carriage.BwPos)
			{
				gMachineInfo.LedReset = 1;
			}
			else
			{
				if(Step >= REQUEST_RESET && Step < REQUEST_RESET + 400)
				{
					gMachineInfo.LedReset = 2;
				}
				else
				{
					gMachineInfo.LedReset = 0;
				}
			}
		}

		if((0 == RMold.Option.Enable) && (1 == LMold.Option.Enable))
		{
			if((1 == LMold.InBasicPos )&&(1 == RMold.Carriage.BwPos))
			{
				gMachineInfo.PermitAuto = 2;
			}
			else
			{
				gMachineInfo.PermitAuto = 1;
			}
			
			if((1 == LMold.InBasicPos )&&(1 == RMold.Carriage.BwPos))
			{
				gMachineInfo.LedReset = 1;
			}
			else
			{
				if(Step >= REQUEST_RESET && Step < REQUEST_RESET + 400)
				{
					gMachineInfo.LedReset = 2;
				}
				else
				{
					gMachineInfo.LedReset = 0;
				}
			}
		}
	}
	else
	{
		gMachineInfo.PermitAuto = 0;
		gMachineInfo.LedReset = 0;
	}/*  if(1 == gTKmanual.Status) */
	
	/*  ----------------------  other ------------------- */
	OldManualReq = ManualReq;
	OldAutoReq   = AutoReq;
	OldAdjustReq = AdjustReq;
	
	PosCalibPinReq = CalibPinReq;
	NegCalibPinReq = CalibPinReq;

	PosReset = gMachineIn.Reset;
	NegReset = gMachineIn.Reset;
	
	PosDrycyleReq = DrycyleReq;
	NegDrycyleReq = DrycyleReq;

}/*  end of cycle   */

/* --------------------------------------------------------------- */
void BasicPos(Mold_typ *pMold, HWEnable_Mold_Fix_typ *pHWEnableMoldFix )
{
	if(pMold->Carriage.BwPos                 
		&& pMold->Clamp.OpnPos              
		&& pMold->BlowPin.DnPos 
		&& (pMold->BlowPin2.DnPos || 0 == pHWEnableMoldFix->BlowPin2 )
		&& pMold->TransDIn.CoolPinUpLimit && pMold->TransDIn.CoolPin2UpLimit && pMold->TransDIn.CoolPin3UpLimit  
		&& pMold->TransDIn.TopDeflashBwLimit && pMold->TransDIn.BottomDeflashBwLimit 
		&& (0 == pMold->Option.PunchHandle || pMold->TransDIn.TopDeflashOpnLimit) 
		&& pMold->TransDIn.ExtruderLiftDnLimit 
		&& ( pMold->ExtrLift.CalOK || ACTUATOR_ELECTRIC != gMachineFix.ExtrLift.eActuatorType)
		//		&& pMold->TransDIn.InnerPinBwLimit 
		&& pMold->TransDIn.SubMoldClsLimit && pMold->TransDIn.BottomSubMoldClsLimit     
		//		&& pMold->TransDIn.PinPartFwLimit 
		//	    && pMold->TransDIn.RobotOpnLimit     
		//	    && pMold->TransDIn.CutNeckOpnLimit      
		//		&& pMold->TransDIn.SealOpnLimit
		//       && pMold->TransDIn.PinHookDnLimit        
		&& pMold->BlowPin.CalOK 
		&& (pMold->BlowPin2.CalOK || 0 == pHWEnableMoldFix->BlowPin2 )
		&& pMold->Carriage.CalOK                 
		&& pMold->Clamp.CalOK           
		&& pMold->Robot.Transfer.BwPos 
		&& (pMold->Robot.Transfer.CalOK || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eValveType)
		&& pMold->Robot.UpDn.UpPos              
		&& (pMold->Robot.UpDn.CalOK || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotUpDn.eValveType)
		&& pMold->TransDIn.RobotOpnLimit
		&& pMold->TransDIn.HookPinBwLimit
		//		&& pMold->TransDIn.RobotOpnLimitL 
		//		&& pMold->TransDIn.RobotOpnLimitR 
		) 
	{
		pMold->InBasicPos = 1;
	}
	else
	{
		pMold->InBasicPos = 0;
	}
}/* void BasicPos(&Mold_typ *pMold) */
void DriverNetworkPhaseReady(void)
{
	if(//Right
		(90 == gAxisBlowPinR.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType)
		&&	(90 == gAxisBlowPin2R.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType ||  0 == gMachineFix.Option.HWEnable.RMold.BlowPin2)
		&&	(90 == gAxisCarriageR.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.Carriage.eActuatorType)
		&&	(90 == gAxisClampR.network.phase 	|| ACTUATOR_ELECTRIC != gMachineFix.MoldR.Clamp.eActuatorType)
		&&	(90 == gAxisTransferR.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eActuatorType)
		&&	(90 == gAxisExtrlift.network.phase	|| ACTUATOR_ELECTRIC != gMachineFix.ExtrLift.eActuatorType)
		)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)
		{
			if(	//Left
				(90 == gAxisBlowPinL.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType)
				&&	(90 == gAxisBlowPin2L.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType ||  0 == gMachineFix.Option.HWEnable.LMold.BlowPin2)
				&&	(90 == gAxisCarriageL.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.Carriage.eActuatorType)
				&&	(90 == gAxisClampL.network.phase 	|| ACTUATOR_ELECTRIC != gMachineFix.MoldR.Clamp.eActuatorType)
				&&	(90 == gAxisTransferL.network.phase || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eActuatorType)
				)
			{
				gMachineInfo.DriverNetworkPhaseReady = 1;
				//Check Hw configuration (Electric)
			}
		}
		else
		{
			gMachineInfo.DriverNetworkPhaseReady = 1;
			//Check Hw configuration (Electric)
		}
	}
	else
	{
		gMachineInfo.DriverNetworkPhaseReady = 0;
	}
}