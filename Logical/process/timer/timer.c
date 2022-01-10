/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: timer
 * File: timer.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of programtimer --- timer counter 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
**                            Function Declarations                                  **
**************************************************************************************/
void timer_10ms(timer_typ *pTimer);
void MoldTime(Mold_typ *pMold);

/***************************************************************************************
****	        		I N I T U P  O F   T H E   T A S K 		 				    ****
***************************************************************************************/
void _INIT Timer_init(void)
{
	RTInfo_v.enable = 1;
	RTInfo(&RTInfo_v);
	TC_TIME	= RTInfo_v.cycle_time/1000;	/* Task class time [1 ms] */
}  /* end of _INIT */

/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				   ****
**************************************************************************************/
void _CYCLIC Timer_cyclic(void)
{
	MoldTime(&LMold);
	MoldTime(&RMold);

	timer_10ms(&gMacTimer.ValveClean);
	timer_10ms(&gMacTimer.ValveCleanTotal);

	timer_10ms(&gMacTimer.LubrWork);
	timer_10ms(&gMacTimer.LubrStop);
	
	
	timer_10ms(&gMacTimer.WasteBelt);
	
	timer_10ms(&gMacTimer.LedAutoRunFlashOn);
	timer_10ms(&gMacTimer.LedAutoRunFlashOff);
	
	timer_10ms(&gMacTimer.ExtrA_OnDelay);
	timer_10ms(&gMacTimer.ExtrB_OnDelay);
	timer_10ms(&gMacTimer.ExtrC_OnDelay);
	timer_10ms(&gMacTimer.ExtrD_OnDelay);
	timer_10ms(&gMacTimer.ExtrVisu_OnDelay);
	
	
	timer_10ms(&gUserPara.RMold.Clamp.Timer);/*2018.7.17 Albert add new timer for clamp lock and holding*/
	timer_10ms(&gUserPara.LMold.Clamp.Timer);/*2018.7.17 Albert add new timer for clamp lock and holding*/
	
	
	timer_10ms(&gMacTimer.AirPressureLowAlarmDly);  // 20201027  owen

	
	timer_10ms(&gMacTimer.ExtParisonCtrlOffDelay);
	
	timer_10ms(&gMacTimer.RClampOpnValveDly);
	timer_10ms(&gMacTimer.RClampClsValveDly);
	timer_10ms(&gMacTimer.LClampOpnValveDly);
	timer_10ms(&gMacTimer.LClampClsValveDly);
	
	timer_10ms(&gMacTimer.RCarriageBtnOffDly);
	timer_10ms(&gMacTimer.LCarriageBtnOffDly);
	
	timer_10ms(&gMacTimer.RBlowPinBtnOffDly);
	timer_10ms(&gMacTimer.LBlowPinBtnOffDly);
	
	timer_10ms(&gMacTimer.RClampBtnOffDly);
	timer_10ms(&gMacTimer.LClampBtnOffDly);
	
	
	/*Clean Valve*/
	timer_10ms(&gMacTimer.RClampValveCleanTotal);
	timer_10ms(&gMacTimer.RClampValveClean);
	timer_10ms(&gMacTimer.LClampValveCleanTotal);
	timer_10ms(&gMacTimer.LClampValveClean);
	
	timer_10ms(&gMacTimer.RCarriageValveCleanTotal);
	timer_10ms(&gMacTimer.RCarriageValveClean);
	timer_10ms(&gMacTimer.LCarriageValveCleanTotal);
	timer_10ms(&gMacTimer.LCarriageValveClean);
	
	timer_10ms(&gMacTimer.RBlowPinValveCleanTotal);
	timer_10ms(&gMacTimer.RBlowPinValveClean);
	timer_10ms(&gMacTimer.LBlowPinValveCleanTotal);
	timer_10ms(&gMacTimer.LBlowPinValveClean);
	
	/*Ext Parison Ctrl*/
	timer_10ms(&gMacTimer.ExtParison);
	
	/*SafeLogicValve Delay*/
	timer_10ms(&gMacTimer.SafeLogicValveDelay);
	timer_10ms(&gMacTimer.SafeLogicValveOffDelay);

}  /* end of _CYCLIC */

void MoldTime(Mold_typ * pMold)
{
	timer_10ms(&pMold->Clamp.ExtRobotTimer);
	timer_10ms(&pMold->Carriage.ExtRobotTimer);
	timer_10ms(&pMold->Timer);
	timer_10ms(&pMold->TimeCycle);
	timer_10ms(&pMold->Carriage.Timer);
	timer_10ms(&pMold->Carriage.LimitTimer);
	timer_10ms(&pMold->Clamp.Timer);
	timer_10ms(&pMold->Clamp.LimitTimer);
	timer_10ms(&pMold->BlowPin.Timer);
	timer_10ms(&pMold->BlowPin2.Timer);
	timer_10ms(&pMold->BottomPin.Timer);
	timer_10ms(&pMold->BlowPin.LimitTimer);
	timer_10ms(&pMold->BlowPin2.LimitTimer);
	timer_10ms(&pMold->PinPart.Timer);
	timer_10ms(&pMold->PinPart.LimitTimer);
	timer_10ms(&pMold->SubMold.Timer);
	timer_10ms(&pMold->SubMold.LimitTimer);
	timer_10ms(&pMold->Screw.Timer);
	timer_10ms(&pMold->Screw.LimitTimer);
	timer_10ms(&pMold->Cutter.Timer);
	timer_10ms(&pMold->ExtrLift.Timer);
	timer_10ms(&pMold->ExtrLift.LimitTimer); //ipis0219
	timer_10ms(&pMold->LowBlow.Timer);
	timer_10ms(&pMold->HighBlow.Timer);
	timer_10ms(&pMold->LowBlow.TimerTotal);
	timer_10ms(&pMold->HighBlow.TimerTotal);
	timer_10ms(&pMold->SupportAir.Timer);
	timer_10ms(&pMold->SupportSuckAir.Timer);
	timer_10ms(&pMold->Lock.Timer);
	timer_10ms(&pMold->CoolPin.Timer);
	timer_10ms(&pMold->CoolPin.LimitTimer);
	timer_10ms(&pMold->CoolPinBlow.Timer);
	timer_10ms(&pMold->CoolPin2.Timer);
	timer_10ms(&pMold->CoolPin2.LimitTimer);
	timer_10ms(&pMold->CoolPin2Blow.Timer);
	timer_10ms(&pMold->CoolPin3.Timer);
	timer_10ms(&pMold->CoolPin3.LimitTimer);
	timer_10ms(&pMold->CoolPin3Blow.Timer);
	timer_10ms(&pMold->BlowDeflash.Timer);
	timer_10ms(&pMold->CoolDeflash.Timer);
	timer_10ms(&pMold->TopDeflash.Timer);
	timer_10ms(&pMold->TopDeflash.LimitTimer);
	timer_10ms(&pMold->BottomDeflash.Timer);
	timer_10ms(&pMold->BottomDeflash.LimitTimer);
	timer_10ms(&pMold->PullBottom.Timer);
	timer_10ms(&pMold->InnerPin.Timer);
	timer_10ms(&pMold->CutNeck.Timer);
	timer_10ms(&pMold->Robot.Timer);
	timer_10ms(&pMold->Robot.LimitTimer);
	timer_10ms(&pMold->RobotOpnCls.Timer);
	timer_10ms(&pMold->RobotOpnCls.LimitTimer);
	timer_10ms(&pMold->Sensor.Timer);
	timer_10ms(&pMold->Sensor.LimitTimer);
	timer_10ms(&pMold->ChuteDeflashBlow.Timer);
	timer_10ms(&pMold->Lubricate.Timer);
	
	timer_10ms(&pMold->BottomSubMold.Timer);
	timer_10ms(&pMold->BottomSubMold.LimitTimer);
	
	timer_10ms(&pMold->HookPin.Timer);
	timer_10ms(&pMold->HookPin.LimitTimer);


	timer_10ms(&pMold->Plate.Timer);/*2018.7.13 Albert*/
	timer_10ms(&pMold->LengthCtrl.Timer);
	timer_10ms(&pMold->LengthCtrl.TimerCycle);
    
}/* end of timer_MOLD()*/

void timer_10ms(timer_typ *pTimer)
{
	pTimer->PT_ms = pTimer->PT * 10;

	if(1 == pTimer->IN)
	{
		if(pTimer->PT_ms > pTimer->ET_ms)
		{
			pTimer->ET_ms = pTimer->ET_ms + TC_TIME;
			pTimer->Q = 0;
		}
		else
		{
			pTimer->Q = 1;
		}
	}
	else
	{
		pTimer->Q  = 0;
		pTimer->ET_ms = 0;
	}

	pTimer->ET = pTimer->ET_ms/10;
}/* end of timer_10ms*/


