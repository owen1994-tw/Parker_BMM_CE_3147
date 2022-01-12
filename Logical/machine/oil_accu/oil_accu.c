/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: oil_accu
 * File: oil_accu.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program oil_accu--- accumulator  control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	/* ----- get task cycle time ----- */
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
   
	ChargeState = 0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
_CYCLIC void cyclic ( void )
{
	/*  ------------ 设定值问题 ------------ */
	if(0.0 == gMachineFix.OilAccu.AccuTimeout)gMachineFix.OilAccu.AccuTimeout = 10;             /*   s   */
	if(0.0 == gMachineFix.OilAccu_Extrlift.AccuTimeout)gMachineFix.OilAccu_Extrlift.AccuTimeout = 10;       /*   s   */
	if(0.0 == gMachineFix.OilAccu.AccuPressureStart)gMachineFix.OilAccu.AccuPressureStart = 70.0;  /*  bar  */
	if(0.0 == gMachineFix.OilAccu.AccuPressureStop)gMachineFix.OilAccu.AccuPressureStop = 90.0;    /*  bar  */
	if(0.0 == gMachineFix.OilAccu.AccuPressure10V)gMachineFix.OilAccu.AccuPressure10V = 200.0;     /*  bar  */
	if(0.0 == gMachineFix.OilAccu.AccuFlow) gMachineFix.OilAccu.AccuFlow	= 0.0; /*Perstange*/
	
	if(gMachineFix.OilAccu.AccuPressureStop < gMachineFix.OilAccu.AccuPressureStart)
	{
		gMachineFix.OilAccu.AccuPressureStop = gMachineFix.OilAccu.AccuPressureStart + 20;
	}

	/* ------------ 压力传感器问题 ------------ */
	if(gMachineFix.OilAccu.AccuPressure10V != 0)
	{
		if(gMachineFix.OilAccu.AccuPressure10V == gMachineInfo.OilAccuPressure)
		{
			/*gAlarm.AccuSensorError = 1;*/
		}
		else
		{
			gAlarm.AccuSensorError = 0;
		}
	}
	else
	{
		gAlarm.AccuSensorError = 0;
	}

	/*Chaoi*/
	gUserPara.OilPumpDelayTime = 3;
	if (1==gMacOption.OilPumpStartMode)
	{	
		if (1==gMachineIn.OilPumpOk_DI)
		{
			if (OilPumpDelayTime >= gUserPara.OilPumpDelayTime)
			{
				gMachineInfo.OilPumpDelayDone =1;
			}	
			else
			{
				OilPumpDelayTime = OilPumpDelayTime+tTask;
			}	
		}
		else
		{
			gMachineInfo.OilPumpDelayDone =0;
			OilPumpDelayTime =0;	
		}		
	}
	else
	{
		gMachineInfo.OilPumpDelayDone = 1;
		OilPumpDelayTime =0;	
	}

	/* -------------------------------------------- */
	/* ----------------系統蓄壓器------------------ */
	/* ---------------------------------------------*/
	
	/* ------------ 蓄能器充压控制 ------------ */

	if((1 == gMachineOut.OilPump) && (1 == gMacOption.ACCPumpCharge) 
		&& ( 0==gMacOption.OilPumpStartMode || (1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone)/*  开启蓄能器  */))  
	{
		gMachineOut.ChargeHold	=	gMachineOut.OilPump;/*When Oil pump on then it need hold pressure by enable Hold Valve*/
		

		gMachineInfo.OilAccuChargeAuto = 0;
		if(gMachineInfo.OilAccuPressure < gMachineFix.OilAccu.AccuPressureStart)
		{
			ChargeFlag = 1;
		}
		else
		{
			ChargeFlag = 0;
		}

		
		switch (ChargeState)
		{
			case 0:	//StandStill
				gMachineOut.Q_Charge	=	0;
				gMachineOut.P_Charge	=   0;
				gMachineOut.AccuChargeValve = 0;
				timeCount = 0;
				accChargeValveDelayTimeCount = 0;
				accChargeValveTimeCount = 0;  
				if(1 == ChargeFlag)
				{
					ChargeState = 1;
				}
				break;
			
			
			case 1: //Charging
						
				accChargeValveDelayTimeCount++;
				if(accChargeValveDelayTimeCount * tTask > gUserFix_new.AccChargeValveDelayTime || 1 == accChargeValveDelayDoneOnce )
				{
					accChargeValveDelayDoneOnce = 1;
						
					gMachineOut.AccuChargeValve	= 1;
					gMachineOut.Q_Charge		= gMachineFix.OilAccu.AccuFlow;	
					gMachineOut.P_Charge		= gMachineFix.OilAccu.AccuChargePressure;	
					timeCount ++;
					accChargeValveTimeCount ++;
					if(timeCount * tTask > gMachineFix.OilAccu.AccuTimeout)
					{
						gAlarm.AccuChargeError = 1;
					}
				}
		

				if(gMachineInfo.OilAccuPressure > gMachineFix.OilAccu.AccuPressureStop)
				{
					ChargeState = 0;
				}
       
				break;

		}
	
		RelieveCount = 15000;   /* time = 15000 * tTask this task means 300s */
	
	}
	else
	{
		ChargeState = 0;
		timeCount = 0;
		accChargeValveDelayTimeCount = 0;
		accChargeValveDelayDoneOnce = 0;
		gMachineOut.AccuChargeValve = 0;
		gMachineOut.Q_Charge	=	0;
		gMachineOut.P_Charge	=	0;
		gMachineOut.ChargeHold = 0;/*Reset valve*/
		if(RelieveCount > 0)RelieveCount--;

		if(1 == RelieveCount)
		{
			if(gMachineInfo.OilAccuPressure > 20)
			{
				/*gAlarm.AccuRelieveError = 1;*/
			}
		}
	}/* if(1 == gMachineOut.OilPump)*/



	oldChargeState = ChargeState;
	chargeTime = timeCount * tTask;
	accChargeValveDelayTime = accChargeValveDelayTimeCount * tTask;
	accChargeValveTime = accChargeValveTimeCount * tTask;

	
	//	gMachineOut.AccuChargeValve = gMachineIn.OilPumpRunning;


	/* ------------ 蓄能器压力可以 ------------ */
	if(gMachineInfo.OilAccuPressure > gMachineFix.OilAccu.AccuPressureStart)
	{
		gMachineInfo.IsAccuOK = 1;
	}
	else
	{
		gMachineInfo.IsAccuOK = 0;
	}
	
	
	/* -------------------------------------------- */
	/* ----------------抬頭蓄壓器------------------ */
	/* -------------------------------------------- */
	
	/* ------------ 抬頭蓄壓器充压控制 ------------ */

	if((1 == gMachineOut.OilPump) && (1 == gMacOption.ACCPumpCharge_Extrlift)
		&& ( 0==gMacOption.OilPumpStartMode || (1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone)/*  开启蓄能器  */))  
	{
		gMachineOut.ChargeHold_Extrlift	=	gMachineOut.OilPump;/*When Oil pump on then it need hold pressure by enable Hold Valve*/
	
		if (1 == gMachineInfo.Auto)
		{
			if(1 ==  gMachineInfo.OilAccuChargeAuto_Extrlift && (PRESSURE_OUTPUT_ACC_EXTRLIFT == gMachineFix.ExtrLift.ePressueOutputType && RMold.ExtrLift.Step  == 13000 ) ) /*關模保持結束 && 抬頭結束*/
			{
				gMachineInfo.OilAccuChargeAuto_Extrlift = 0;
				if(gMachineInfo.OilAccuPressure_Extrlift < gMachineFix.OilAccu_Extrlift.AccuPressureStop)
				{
					ChargeFlag_Extrlift = 1;
				}
				else
				{
					ChargeFlag_Extrlift = 0;
				}
			}
		}
		else
		{
			if(( 0 == gMachineFix.Option.bOilAccu_Extrlift_DisChar_Calib && 1 == gMachineInfo.Calib )  || 1 == gMachineInfo.Manual )
			{
				if(gMachineInfo.OilAccuPressure_Extrlift < gMachineFix.OilAccu_Extrlift.AccuPressureStart)
				{
					ChargeFlag_Extrlift = 1;
				}
				else
				{
					ChargeFlag_Extrlift = 0;
				}
			}
			else if( 1 == gMachineFix.Option.bOilAccu_Extrlift_DisChar_Calib && 1 == gMachineInfo.Calib )
			{
				ChargeFlag_Extrlift = 0;
				ChargeState_Extrlift = 0;
			}
		}

	
		switch (ChargeState_Extrlift)
		{
			case 0:	//StandStill
				gMachineOut.Q_Charge_Extrlift	=	0;
				gMachineOut.P_Charge_Extrlift	=   0;
				gMachineOut.AccuChargeValve_Extrlift = 0;
				timeCount_Extr = 0;
				accChargeValveDelayTimeCnt_Extr = 0;
				accChargeValveTimeCount_Extr = 0;
			
				if(1 == ChargeFlag_Extrlift)
				{
					ChargeFlag_Extrlift = 0;
					ChargeState_Extrlift = 1;
				}
				break;
		
		
			case 1: //Charging
					
				accChargeValveDelayTimeCnt_Extr ++;
				if(accChargeValveDelayTimeCnt_Extr * tTask > gUserFix_new.AccChargeValve_Extrlfit_DlyTime || 1 == accChargeValveDelayDoneOnce_Extr )
				{
					accChargeValveDelayDoneOnce_Extr = 1;
					
					gMachineOut.AccuChargeValve_Extrlift	= 1;
					gMachineOut.Q_Charge_Extrlift			= gMachineFix.OilAccu_Extrlift.AccuFlow;	
					gMachineOut.P_Charge_Extrlift			= gMachineFix.OilAccu_Extrlift.AccuChargePressure;	
					timeCount_Extr ++;
					accChargeValveTimeCount_Extr ++;
					if(timeCount_Extr * tTask > gMachineFix.OilAccu_Extrlift.AccuTimeout)
					{
						gAlarm.AccuCharge_ExtrliftError = 1;
					}
				}

				if(gMachineInfo.OilAccuPressure_Extrlift > gMachineFix.OilAccu_Extrlift.AccuPressureStop)
				{
					ChargeState_Extrlift = 0;
				}

				break;

		}

		RelieveCount_Extr = 15000;   /* time = 15000 * tTask this task means 300s */

	}
	else
	{
		ChargeState_Extrlift = 0;
		timeCount_Extr = 0;
		accChargeValveDelayTimeCnt_Extr = 0;
		accChargeValveDelayDoneOnce_Extr = 0;
		gMachineOut.AccuChargeValve_Extrlift  = 0;
		gMachineOut.Q_Charge_Extrlift	= 0;
		gMachineOut.P_Charge_Extrlift	= 0;
		gMachineOut.ChargeHold_Extrlift = 0;/*Reset valve*/
		if(RelieveCount_Extr > 0)RelieveCount_Extr--;

		if(1 == RelieveCount_Extr)
		{
			if(gMachineInfo.OilAccuPressure_Extrlift > 20)
			{
				/*gAlarm.AccuRelieveError = 1;*/
			}
		}
	}/* if(1 == gMachineOut.OilPump)*/



	oldChargeState_Extrlift = ChargeState_Extrlift;
	chargeTime_Extr = timeCount_Extr * tTask;
	accChargeValveDelayTime_Extr = accChargeValveDelayTimeCnt_Extr * tTask;
	accChargeValveTime_Extr = accChargeValveTimeCount_Extr * tTask;

	
	/* 2018.7.11 Albert High Pressure motor enable*/
	//	if(1 == gMacOption.HighPrsMotorEnable)  
	//		gMachineOut.HighPrsMotorOn = gMachineOut.OilPump;
	//	else
	//		gMachineOut.HighPrsMotorOn = 0;
	gMachineOut.HighPrsMotorOn =1;
	
	//	gMachineOut.AccuChargeValve = gMachineIn.OilPumpRunning;


	/* ------------ 抬頭蓄壓器压力可以 ------------ */
	if(gMachineInfo.OilAccuPressure_Extrlift > gMachineFix.OilAccu_Extrlift.AccuPressureStart)
	{
		gMachineInfo.IsAccuOK_Extrlift = 1;
	}
	else
	{
		gMachineInfo.IsAccuOK_Extrlift = 0;
	}
	
	
	/* ------------------------------------------------ */
	
	if (1==gMacOption.OilPumpStartMode)
	{
		if ((1 == gMachineOut.OilPump) && (0==gMachineIn.OilPumpOk_DI))
		{
			OilPumpDetectTime=	tTask +OilPumpDetectTime;
			if (OilPumpDetectTime >= gUserPara.OilPumpDetectTime)
			{
				gMachineOut.OilPump=0;
				gAlarm.OilPumpErr = 1;
			}
		
		}
		else	
		{
			OilPumpDetectTime=0;
		}
	}
	else
	{
		OilPumpDetectTime=0; //ipis0315
	}
	
	
	/* ------------ 抬頭/加壓馬達 ------------ */
	if(1 == gMacOption.HighPrsMotorEnable)
	{
		gMachineOut.HighPrsMotorOn = gMachineOut.OilPump;
	}
	else
	{
		gMachineOut.HighPrsMotorOn = 0;
	}
	
}/* end of _CYCLIC */


