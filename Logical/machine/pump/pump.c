/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: power
 * File: power.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program power --- pump  control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "pic_id.h"
/**************************************************************************************
** 					               include user files								          **
**************************************************************************************/
#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
     	        	           I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
   
	AccuStopCounter = 0;
	AccuStopDelay   = 0;
    
	OilPumpOnDelay  = 0;
	OilPumpOffDelay = 0;
    
	HighSafeV_Keep  = 0;
	LowerSafeV_Keep = 0;
    
	//   gMachineOut.HeatPower = 1;
	
	tParisonOnDelay = 0;
} /* end of _INIT */

/**************************************************************************************
        				   C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	tParisonOnDelay = tParisonOnDelay + tTask;
	if(tParisonOnDelay > 10000)tParisonOnDelay = 1000;
	
	gMachineInfo.Mark = gThickIO.Out.MarkPulse;
	
	gMachineFix.Pump.PumpOnDealy  = 10;			/*  default value 0.1s */
	gMachineFix.Pump.AccuOffDealy = 10;			/*  default value 0.1s */
	gMachineFix.Pump.PumpOffDealy = 10;			/*  default value 0.1s */
			
	/* ----------------------- 油泵启动，加热电源 ------------------------- */
	if(1 == gMachineIn.MainControl && gPanelObj.GetPicture != PIC_INIT && (gPanelObj.GetPicture != PIC_SCREEN_SAVE))
	{
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PENU_VALVE],PKeyMatric[S$KEYMATRIX_PENU_VALVE]))
		{
			gMachineOut.PneumaticValve = !gMachineOut.PneumaticValve;
		}
		
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_HEAT_POWER],PKeyMatric[S$KEYMATRIX_HEAT_POWER]))
		{
			gMachineOut.HeatPower = !gMachineOut.HeatPower;
			gT_Par.GrpSwh.OnOff[0] =  gMachineOut.HeatPower; //1226 ipis
			gT_Par.GrpSwh.PreOnOff[0]=0;
		}
		
		/*  --------------------- Cooling pump --------------------- */
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_COOLING_PUMP],PKeyMatric[S$KEYMATRIX_COOLING_PUMP]))
			{
			if(1 == gMachineOut.OilCoolingPump)
			{
				gMachineOut.OilCoolingPump = 0;
			}
			else
			{
				if(1 == gMachineIn.CoolingPumpEnable)
				{
					gMachineOut.OilCoolingPump = 1;
				}
				else
				{
					gImply.CoolingPumpCondition = 1;
				}
			}
		}
		
		if ( 1 == gMachineFix.ExtruderA.LinkWithMode) 
		{
			/*  --------------------- Parison pump && Extuder A --------------------- */
			if( 1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PARISON_PUMP],PKeyMatric[S$KEYMATRIX_PARISON_PUMP])
				||1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_A_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_A_ABB]))
				{	
				if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0213
				{
					if (0==gMachineInfo.bLoweringFlag)
					{
						if(tParisonOnDelay > gT_Par.Fix.tCheck)
						{
							if(1 == gMachineOut.ParisonPump)
							{
								gMachineOut.ParisonPump = 0;
							}
							else
							{
								if(1 == gExtruderA.Status.OnProtected )  //from  extruder A switch on delay
								{
									if(1 == gMachineIn.ParisonPumpEnable)
									{
										gMachineOut.ParisonPump = 1;
										gThickIO.InitPower = 1;
									}
									else
									{
										gImply.ParisonPumpCondition = 1;
									}
								}
								else
								{
									gImply.ExtrA_OnDelay = 1;
								}
							}
						}
					}
					else
					{
						gImply.TempLoweringEnable =1; 
					}
				}
				else
				{
					gImply.TempLow =1;	
					gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;
				}
			}
		}
		else
		{
			/*  --------------------- Parison pump --------------------- */
			if( 1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_PARISON_PUMP],PKeyMatric[S$KEYMATRIX_PARISON_PUMP]))
				{	
				if(tParisonOnDelay > gT_Par.Fix.tCheck)
				{
					if(1 == gMachineOut.ParisonPump)
					{
						gMachineOut.ParisonPump = 0;
					}
					else
					{
						if(1 == gMachineIn.ParisonPumpEnable)
						{
							gMachineOut.ParisonPump = 1;
							gThickIO.InitPower = 1;
						}
						else
						{
							gImply.ParisonPumpCondition = 1;
						}
					}
				}
			}
		}
	
		
		/*  --------------------- Main pump --------------------- */
		if(1 == EDGEPOS(gMachineIn.OilPumpOnoff,PMachineIn.OilPumpOnoff))
		{
			if((0 == OilPumpOnCounter) && (0 == OilPumpOffCounter))
			{
				if(1 == gMachineOut.OilPump)
				{
					gMachineOut.OilPump = 0;
		     
					AccuStopCounter   = gMachineFix.Pump.AccuOffDealy/2;			/*  off delay     */
					OilPumpOffCounter = gMachineFix.Pump.PumpOffDealy/2;
				}
				else
				{
					OilPumpOnCounter = gMachineFix.Pump.PumpOnDealy/2;
					gMachineOut.OilPumpContactor = 1;                           /*  pump contactor */
				}
			}
		}/* if(1 == EDGEPOS(gMachineIn.OilPumpOnoff,PMachineIn.OilPumpOnoff)) */
	}/* if(1 == gMachineIn.MainControl && gPanelObj.GetPicture != PIC_INIT && gPanelObj.GetPicture != PIC_SCREEN_SAVE) */
	
	/*  油泵延时启动  */
	if(OilPumpOnCounter > 0)OilPumpOnCounter--;
	OilPumpOnDelay = (OilPumpOnCounter * 2)/10;
	if(OilPumpOnCounter == 1 && 1 == gMachineOut.OilPumpContactor)
	{
		if(1 == gMachineIn.OilPumpEnable && 1 == gMachineIn.CoolingPumpRunning)
		{
			gMachineOut.OilPump = 1;
			gMachineOut.AccuRelieveValve = 1;
		}
		else
		{
			if(0 == gMachineIn.OilPumpEnable)gImply.MainPumpCondition = 1;
			if(0 == gMachineIn.CoolingPumpRunning)gImply.CoolingPumpNotOn = 1;
		}
	}
	
	/*  油泵接触器延时关闭  */
	if(OilPumpOffCounter > 0)OilPumpOffCounter--;
	OilPumpOffDelay = (OilPumpOffCounter * 2)/10;
	if(OilPumpOffCounter == 1 && 0 == gMachineOut.OilPump)
	{
		gMachineOut.OilPumpContactor = 0;
	}
	
	/*  储能卸荷延时  */
	if(AccuStopCounter > 0)AccuStopCounter--;
	AccuStopDelay = (AccuStopCounter * 2)/10;
	if(AccuStopCounter == 1 && 0 == gMachineOut.OilPump)
	{
		gMachineOut.AccuRelieveValve = 0;
	}
	
	/* ----------------------------- oil pump off ------------------------------- */
	if(0 == gMachineInfo.Calib)  // Auto. Manual
	{
		if(1 == gMachineIn.InstancyStop || 0 == gMachineIn.SafeGate || 1 == gMachineInfo.SafeInStop )
		{
			gMachineOut.OilPump = 0;
			gAlarm.SaftyDoor = !gMachineIn.SafeGate;
		}
		
		gMachineOut.SafetyGateBypass = 0;
		
		if(1 == gMachineInfo.Manual)
		{
			if(1 == gAlarm.OilTempHigh || 1 == gAlarm.AccuChargeError || 1 == gAlarm.AccuCharge_ExtrliftError)
			{
				gMachineOut.OilPump = 0;
			}
		}
	}
	else
	{
		
		if(1 == gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop || 1 == gAlarm.AccuChargeError || 1 == gAlarm.AccuCharge_ExtrliftError)
		{
			gMachineOut.OilPump = 0;
			gMachineOut.SafetyGateBypass = 0;
		}
		else
		{
			/* 允許安全門開時開油泵  */
			if(gPanelObj.GetPicture != PIC_CALIB_CLAMP_LVDT && gPanelObj.GetPicture != PIC_CALIB_CARRIAGE_LVDT && gPanelObj.GetPicture != PIC_CALIB_BLOWPIN_LVDT && gPanelObj.GetPicture != PIC_CALIB_OTHER_LVDT )
			{
				gMachineOut.SafetyGateBypass = 0;
			}
			else 
			{
				if(1 == gMachineOut.SafetyGateBypass)
				{
						
				}
				else if(0 == gMachineIn.SafeGate )
				{
					
					gAlarm.SaftyDoor = !gMachineIn.SafeGate;
					gMachineOut.OilPump = 0;
				}
			}
		}	
		
		if(1 == gMachineInfo.Calib)
		{
			if(1 == gAlarm.OilTempHigh)
			{
				gMachineOut.OilPump = 0;
			}
		}
	}
	
	/*  main pump feedback check  */
	//	gMachineIn.OilPumpRunning = gMachineOut.OilPump;/*Sinece we don't have feedback, ingore check signal*/
	//	if(gMachineOut.OilPump != gMachineIn.OilPumpRunning)
	//	{
	//		tMainPumpNormal = tMainPumpNormal + tTask;
	//		if(tMainPumpNormal >= 200.0)tMainPumpNormal = 20.0;
	//				
	//		if(tMainPumpNormal >= 10.0)
	//		{
	//			gAlarm.OilPumpErr = 1;
	//		}
	//	}
	//	else
	//	{
	//		gAlarm.OilPumpErr = 0;
	//		tMainPumpNormal = 0;
	//	}


	
	if(1 == gAlarm.OilPumpErr || 0 == gMachineIn.OilPositionNormal || 0 == gMachineIn.OilPumpEnable || 
		0 == gMachineIn.CoolingPumpRunning                          || 0 == gMachineIn.SafeGate)
	{
		if(1 == gMachineOut.OilPump)
		{
			if(0 == gMachineIn.OilPumpEnable)gImply.MainPumpCondition = 1;
			if(0 == gMachineIn.CoolingPumpRunning)gImply.CoolingPumpNotOn = 1;
			if(0 == gMachineIn.SafeGate)gAlarm.SaftyDoor = 1;
			gPROCESS_info.oilpumpStop = 1;   //request manual mode
			gMachineOut.OilPump = 0;
		}
	}
	
	
	/*  -------------------- Parison pump off -------------------- */
	/*  Pariosn pump feedback check  */
	gMachineIn.ParisonPumpRunning = gMachineOut.ParisonPump; 
	if(gMachineOut.ParisonPump != gMachineIn.ParisonPumpRunning)
	{
		tParisonPumpNormal = tParisonPumpNormal + tTask;
		if(tParisonPumpNormal >= 200.0)tParisonPumpNormal = 20.0;
				
		if(tParisonPumpNormal >= 10.0)
		{
			gAlarm.ParisonPumpErr = 1;
		}
	}
	else
	{
		gAlarm.ParisonPumpErr = 0;
		tParisonPumpNormal = 0.0;
	}
	

	
	if (1 == gMachineInfo.ExtrTLow || 0 == gMachineIn.ParisonPumpEnable || 1 == gAlarm.ParisonPumpErr || 1== gAlarm.ParisonMotorOverload || 0 == gMachineOut.HeatPower
		/*||  (0 == gMachineInfo.IsHeat)*/ || 0 == gT_Par.GrpSwh.OnOff[0]
		|| (gMachineFix.Parison.eActuatorType == ACTUATOR_ELECTRIC &&  ( 1 == gAlarm.ParisonAxisErr || 1 == gAlarm.ParisonNetworkErr)))
	{
		if(1 == gMachineOut.ParisonPump)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = gMachineInfo.ExtrTLow;
			if(0 == gMachineIn.ParisonPumpEnable)gImply.ParisonPumpCondition = 1;
			if(0 == gT_Par.GrpSwh.OnOff[0])gImply.HeatNotOn = 1;
			if(0 == gMachineOut.HeatPower) gImply.HeaterPowerNotOn = 1;
			gMachineOut.ParisonPump = 0;
			gMachineInfo.ParisonPump_ErrStop_LinkWith = 1;  //Extruder Link With Selection
		}
	}
	
	/* ------------------------ Pump Link With Selection  ---------------------------- */
	/* ----- ParisonPump ----- */
	if(1 == gMachineFix.ExtruderA.LinkWithMode)
	{
		//		if( EDGENEG(gMachineOut.ParisonPump,oldParisonPump_On))
		//		{
		//			gMachineOut.ExtrA_On = 0;
		//		}
		//		else 
		if(1 == gMachineInfo.ParisonPump_ErrStop_LinkWith)
		{
			gMachineInfo.ParisonPump_ErrStop_LinkWith = 0;
			gMachineOut.ExtrA_On = 0;
		}
	}
	
	oldParisonPump_On = gMachineOut.ParisonPump;
	/*  -------------------- Cooling pump off -------------------- */
	/*  Cooling pump feedback check  */
	//	gMachineIn.CoolingPumpRunning = gMachineOut.OilCoolingPump;
	//	if(gMachineOut.OilCoolingPump != gMachineIn.CoolingPumpRunning)
	//	{
	//		tCoolingPumpNormal = tCoolingPumpNormal + tTask;
	//		if(tCoolingPumpNormal >= 200.0)tCoolingPumpNormal = 20.0;
	//					
	//		if(tCoolingPumpNormal >= 10.0)
	//		{
	//			gAlarm.CoolingPumpErr = 1;		
	//		}
	//	}
	//	else
	//	{
	//		gAlarm.CoolingPumpErr = 0;
	//		tCoolingPumpNormal = 0.0;
	//	}
	
	if (0 == gMachineIn.CoolingPumpEnable || 1 == gAlarm.CoolingPumpErr)
	{
		if(1 == gMachineOut.OilCoolingPump)
		{
			gMachineOut.OilCoolingPump = 0;
			if (0 == gMachineIn.CoolingPumpEnable)gImply.CoolingPumpCondition = 1;
		}
	}
	
	
	/* Oil cooling pump auto cooling  -> OilCoolingValve */
	if(!gMachineFix.OilCooling.OilTempCoolingStart)gMachineFix.OilCooling.OilTempCoolingStart = 40;
	if(!gMachineFix.OilCooling.OilTempCoolingStop)gMachineFix.OilCooling.OilTempCoolingStop = 35;
	
	if(gMachineInfo.OilTemp > gMachineFix.OilCooling.OilTempCoolingStart)
	{
		gMachineOut.WaterCooling = 1;
	}
	
	if(gMachineInfo.OilTemp < gMachineFix.OilCooling.OilTempCoolingStop)
	{
		gMachineOut.WaterCooling = 0;
	}
	gMachineOut.OilCoolingValve = !gMachineOut.WaterCooling;
	
	/* Oil heating pump auto heating  */
	if(!gMachineFix.OilCooling.OilTempHeatingStart)gMachineFix.OilCooling.OilTempHeatingStart = 10;
	if(!gMachineFix.OilCooling.OilTempHeatingStop)gMachineFix.OilCooling.OilTempHeatingStop = 20;
	
	if(gMachineInfo.OilTemp < gMachineFix.OilCooling.OilTempHeatingStart)
	{
		gMachineOut.WaterHeating = 1;
	}
	
	if(gMachineInfo.OilTemp > gMachineFix.OilCooling.OilTempHeatingStop)
	{
		gMachineOut.WaterHeating = 0;
	}
//	gMachineOut.OilCoolingValve = !gMachineOut.WaterHeating;
	
	
	/* ------------------------ hot cutter heating  熱切刀 DO -------------------------- */
	//	if(1 == EDGEPOS(gMachineIn.Key.HotCutter,PMachineIn.Key.HotCutter))
	//	{
	//		gMachineOut.CutterHeating = !gMachineOut.CutterHeating;
	//	}
	//
	//	if(1 == gMachineOut.CutterHeating)
	//	{
	//		if(gMachineOut.CutterHeating != gMachineIn.HotCutterNormal)
	//		{
	//			tCutterHeatNormal = tCutterHeatNormal + tTask;
	//			if(tCutterHeatNormal >= 200.0)tCutterHeatNormal = 20.0;
	//		}
	//		else
	//		{
	//			tCutterHeatNormal = 0;
	//		}
	//	}
	//	else
	//	{
	//		tCutterHeatNormal = 0;	
	//	}
	//		
	//	if(tCutterHeatNormal >= 10.0)
	//	{
	//		gAlarm.CutterHeatNormal = 1;  /*  熱切刀加熱異常  */
	//		if(gMachineOut.CutterHeating != 0)gMachineOut.CutterHeating = 0;
	//	}
	//	else
	//	{
	//		gAlarm.CutterHeatNormal = 0;
	//	}
	
	
      
	gMachineOut.LowerRelieveValve  = gMachineOut.OilPump;
	gMachineOut.HigherRelieveValve = gMachineOut.OilPump;
	
	if(1 == gMachineIn.LowerPressureHigh && 0 == gMachineIn.LowerPressureLow)
	{
		gMachineOut.LowerRelieveValve = 0;
	}

	/* ----------------- high pressure/low pressure enable valve --------------- */
	if(0 == gMachineInfo.Calib)
	{
		gMachineOut.HighPressureEnable = !gMachineIn.InstancyStop | gMachineIn.SafeGate | gMachineIn.OilPumpRunning | !gMachineInfo.SafeInStop;         
		gMachineOut.LowPressureEnable  = !gMachineIn.InstancyStop | gMachineIn.SafeGate | gMachineIn.OilPumpRunning | !gMachineInfo.SafeInStop; 
	}
	else
	{
		gMachineOut.HighPressureEnable = !gMachineIn.InstancyStop | gMachineIn.OilPumpRunning | !gMachineInfo.SafeInStop;            
		gMachineOut.LowPressureEnable  = !gMachineIn.InstancyStop | gMachineIn.OilPumpRunning | !gMachineInfo.SafeInStop; 
	}
	
	/*  -------------------------Safe Calib Pressure --------------------- */
	if(1 == gMachineIn.LMold.SafeGate && 1 == gMachineIn.RMold.SafeGate)
	{
		if(gMachineOut.OilPump == 1)
		{
			if(0 == gMachineInfo.Calib)
			{
				gMachineOut.SafeCalibPressure      = 1;
			}
			else
			{
				gMachineOut.SafeCalibPressure      = gMachineIn.CalibMode;
			}
		}
		else
		{
			gMachineOut.SafeCalibPressure     = 0;
		}
	}
	else
	{
		gMachineOut.SafeCalibPressure      = gMachineOut.OilPump;
	}

	gMachineOut.InerloclResetEnable =  gMachineIn.HydPressureSW | gMachineIn.PnePressureSW;
	
	/* ----------------------------- Safety reset lamp  ------------------------------- */
	cntFlasher++;
	if(cntFlasher >= 20000)cntFlasher=0;
		
	if(0 == gMachineIn.InstancyStop && 1 == gMachineIn.SafeGate && 0 == gMachineInfo.SafeInStop)
	{
		gMachineOut.InerloclResetLed = 1;
	}
	else
	{
		if(1 == gMachineIn.InstancyStop || 0 == gMachineIn.SafeGate  || 1 == gMachineInfo.SafeInStop)
		{
			/* lamp falsher   */
			if((cntFlasher%100) < 50)
			{
				gMachineOut.InerloclResetLed = 1;
			}
			else
			{
				gMachineOut.InerloclResetLed = 0;
			}
		}
		else
		{
			gMachineOut.InerloclResetLed = 0;
		}
	}/* if(0 == gMachineIn.InstancyStop && 1 == gMachineIn.SafeGate) */
	
	gThickIO.BtnPowerOn = gMachineOut.ParisonPump;
	
	if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_MAIN_PUMP],PKeyMatric[S$KEYMATRIX_MAIN_PUMP]))
		{
		if(1 == gMachineIn.MainControl && gPanelObj.GetPicture != PIC_INIT && gPanelObj.GetPicture != PIC_SCREEN_SAVE)
		{
			if (1 == gMachineIn.AccuChargeON && 0 == gMachineOut.OilPump)
			{/*2020.08.27 Chaoi*/
				gAlarm.AccuChargeRPressure = 1;
			}
			else
			{
				if(  1 == gMachineInfo.DriverNetworkPhaseReady  )
				{
					if( 1 == gMachineIn.SafeGate )
					{
						if(0 == gMachineOut.OilPump)
						{
							gMachineOut.OilPump = 1;
							
							RMold.ExtrLift.InitPower = 1;
							
							LMold.Clamp.InitPower = 1;
							RMold.Clamp.InitPower = 1;
							
							LMold.Carriage.InitPower = 1;
							RMold.Carriage.InitPower = 1;
							
							LMold.BlowPin.InitPower = 1;
							RMold.BlowPin.InitPower = 1;
							
							LMold.BlowPin2.InitPower = 1;
							RMold.BlowPin2.InitPower = 1;
							
							LMold.Robot.Transfer.InitPower = 1;
							RMold.Robot.Transfer.InitPower = 1;
			
							LMold.Robot.UpDn.InitPower = 1;
							RMold.Robot.UpDn.InitPower = 1;
										
							LMold.Sensor.UpDn.InitPower = 1;
							RMold.Sensor.UpDn.InitPower = 1;
						}
						else
						{
							if((0.0 == LMold.Clamp.v_set   ) && (0.0 == RMold.Clamp.v_set   ) && 
								(0.0 == LMold.Carriage.v_set) && (0.0 == RMold.Carriage.v_set) &&
								(0.0 == LMold.BlowPin.v_set ) && (0.0 == RMold.BlowPin.v_set )&&
								(0.0 == LMold.BlowPin2.v_set ) && (0.0 == RMold.BlowPin2.v_set ))
							{
								gMachineOut.OilPump = 0;	
							}
						}	
					}
					else
					{
						gImply.SafeGate = !gMachineIn.SafeGate;
					}
					
				}
				else
				{
					gImply.DriverNetworkPhaseNotReady = !gMachineInfo.DriverNetworkPhaseReady;
				}
			}	
		}
	}

	RMold.ExtrLift.BtnPowerOn = gMachineOut.OilPump;
	
	LMold.Clamp.BtnPowerOn = gMachineOut.OilPump;
	RMold.Clamp.BtnPowerOn = gMachineOut.OilPump;

	LMold.Carriage.BtnPowerOn = gMachineOut.OilPump;
	RMold.Carriage.BtnPowerOn = gMachineOut.OilPump;
	
	LMold.BlowPin.BtnPowerOn = gMachineOut.OilPump;
	RMold.BlowPin.BtnPowerOn = gMachineOut.OilPump;
	
	LMold.BlowPin2.BtnPowerOn = gMachineOut.OilPump;
	RMold.BlowPin2.BtnPowerOn = gMachineOut.OilPump;
	
	LMold.Robot.UpDn.BtnPowerOn =  gMachineOut.OilPump;
	RMold.Robot.UpDn.BtnPowerOn =  gMachineOut.OilPump;
	
	LMold.Robot.Transfer.BtnPowerOn =  gMachineOut.OilPump;
	RMold.Robot.Transfer.BtnPowerOn =  gMachineOut.OilPump;
	
	LMold.Sensor.UpDn.BtnPowerOn = gMachineOut.OilPump;
	RMold.Sensor.UpDn.BtnPowerOn = gMachineOut.OilPump;
	
	gMachineOut.CarriagePower = gMachineOut.OilPump;
	gMachineOut.ClampPower    = gMachineOut.OilPump;
	gMachineOut.BlowPinPower  = gMachineOut.OilPump;
	
	/* ----------------------------- safety gate lock ------------------------------- */
	if(0 == gMachineOut.OilPump && 1 == gMachineIn.PnePressureSW)
	{
		gMachineOut.SafetyGateUnlock = 1;
	}
	else
	{
		gMachineOut.SafetyGateUnlock = gMachineInfo.Calib;	
	}

	/* ----------------------------- waste belt control ------------------------------- */
	if(0 == gMachineInfo.Calib)
	{
		if(0 == StepWasteBelt)StepWasteBelt = 10;	
	}
	else
	{
		StepWasteBelt = 0;	
	}
	
	switch(StepWasteBelt)
	{
		case 0:
			gMachineOut.WasteBelt  = 0;
			gMacTimer.WasteBelt.IN = 0; 
			break;
		
		case 10:
			if(gMachinePara.WateBeltOffTime > 0.0)
			{
				gMacTimer.WasteBelt.IN = 1;
				gMacTimer.WasteBelt.PT = (UDINT)(gMachinePara.WateBeltOffTime * 100);
				StepWasteBelt = 20;
			}
			else
			{
				StepWasteBelt = 30;
			}
			break;
	
		case 20:
			gMachineOut.WasteBelt = 0;
			
			if(1 == gMacTimer.WasteBelt.Q)
			{
				gMacTimer.WasteBelt.IN = 0;
				StepWasteBelt = 30;
			}
			break;
		
		case 30:
			gMacTimer.WasteBelt.IN = 0; 
			StepWasteBelt = 40;
			break;
		
		case 40:
			if(gMachinePara.WateBeltOnTime > 0.0)
			{
				gMacTimer.WasteBelt.IN = 1;
				gMacTimer.WasteBelt.PT = (UDINT)(gMachinePara.WateBeltOnTime * 100);
				StepWasteBelt = 50;
			}
			else
			{
				StepWasteBelt = 60;
			}
			break;
	
		case 50:
			gMachineOut.WasteBelt = 1;
			
			if(1 == gMacTimer.WasteBelt.Q)
			{
				gMacTimer.WasteBelt.IN = 0;
				StepWasteBelt = 60;
			}
			break;
		
		case 60:
			gMachineOut.WasteBelt  = 0;
			gMacTimer.WasteBelt.IN = 0; 
			StepWasteBelt = 10;
			break;
	}/* switch(StepWasteBelt) */
	
	/* ----------------------------- key edge ------------------------------- */
	PKeyMatric[S$KEYMATRIX_HEAT_POWER]  = gPanelObj.KeyMatric[S$KEYMATRIX_HEAT_POWER];
		PKeyMatric[S$KEYMATRIX_PENU_VALVE]  = gPanelObj.KeyMatric[S$KEYMATRIX_PENU_VALVE];

		PMachineIn = gMachineIn;
	NMachineIn = gMachineIn;

} /* end of _CYCLIC */


