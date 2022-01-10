/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: Tonnage
 * File: Tonnage.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program Tonnage --- mold Tonnage 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void Sensor( Mold_typ * pMold,Robot_Para_typ * pRobotPara);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */

 	LMold.Sensor.Step = 30000;
 	RMold.Sensor.Step = 30000;
 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	LMold.TransDIn.SensorUp = gMachineIn.LMold.SensorUp = 1;
	RMold.TransDIn.SensorUp = gMachineIn.RMold.SensorUp = 1;
	
	Sensor(&LMold,&gMachinePara.MoldL.Robot);
	Sensor(&RMold,&gMachinePara.MoldR.Robot);
	
 } /* end of _CYCLIC */

void Sensor( Mold_typ * pMold,Robot_Para_typ * pRobotPara)
{
	pMold->Sensor.UpDn.CalOK = pMold->Sensor.UpDn.HomingOk = 1;
	
	pRobotPara->ParaUpDn.sSensorBottle = 0.0;
	pRobotPara->ParaUpDn.sSensorWaste  = 0.0;
	
	pMold->Sensor.UpDn.mmAs = 0.0;
	
	if(pMold->TimeSet.SensorUpAlarmTime < 10)pMold->TimeSet.SensorUpAlarmTime = 1000;
	if(pMold->TimeSet.SensorDnAlarmTime < 10)pMold->TimeSet.SensorDnAlarmTime = 1000;
	
	if((1 == pMold->Sensor.UpDn.HomingOk) &&  (1 == pMold->TransDIn.SensorUp) &&
		(pMold->Sensor.UpDn.mmAs > -0.2  ) && (pMold->Sensor.UpDn.mmAs < 0.2))
	{
		pMold->Sensor.ZeroPos = 1;	
	}
	else
	{
		pMold->Sensor.ZeroPos = 0;
	}
	
	if((1 == pMold->Sensor.UpDn.HomingOk) && 
		(pMold->Sensor.UpDn.mmAs > pRobotPara->ParaUpDn.sSensorBottle - 0.2) && 
		(pMold->Sensor.UpDn.mmAs < pRobotPara->ParaUpDn.sSensorBottle + 0.2    ))
	{
		pMold->Sensor.BottlePos = 1;	
	}
	else
	{
		pMold->Sensor.BottlePos = 0;
	}
	
	if((1 == pMold->Sensor.UpDn.HomingOk) && 
		(pMold->Sensor.UpDn.mmAs > pRobotPara->ParaUpDn.sSensorWaste - 0.2) && 
		(pMold->Sensor.UpDn.mmAs < pRobotPara->ParaUpDn.sSensorWaste + 0.2    ))
	{
		pMold->Sensor.WastePos = 1;	
	}
	else
	{
		pMold->Sensor.WastePos = 0;
	}
	
	switch ( pMold->Sensor.Step )
   {
   		/*------------------ stop all the output ------------------*/
         case 0:
				 pMold->Sensor.UpDn.v_set = 0;
         	 pMold->Sensor.Timer.IN	= 0;
         	break;
   
   		/*------------------ Sensor down -------------------- */
   		case 100: 	/* delay */
				if(1 == pMold->Option.HookPin)
				{
					pMold->Sensor.Timer.IN = 1;
					pMold->Sensor.Timer.PT = 1;
					pMold->Sensor.Step = 200;
				}
				else
				{
					pMold->Sensor.Step = 3000;
				}
            break;
   
         case 200: 	/* Sensoring delay */
            if (1 == pMold->Sensor.Timer.Q)
            {
               pMold->Sensor.Timer.IN = 0;
               pMold->Sensor.Step = 300;
            }
            break;
   
         case 300: 	/* scan Sensor bottle */
				pMold->Sensor.LimitTimer.IN = 1;
				pMold->Sensor.LimitTimer.PT = pMold->TimeSet.SensorDnAlarmTime;
			
				pMold->Sensor.UpDn.s_set = pRobotPara->ParaUpDn.sSensorBottle;
				pMold->Sensor.UpDn.MoveAbsolute = 1;
				pMold->Sensor.Step = 400;
            break;
		
         case 400:
				pMold->TimeDis.SensorDnAlarmTime = pMold->Sensor.LimitTimer.ET;
			
				if(1 == pMold->Sensor.LimitTimer.Q)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 40000;
				}
			
				if(pMold->Sensor.UpDn.mmAs > pRobotPara->ParaUpDn.sSensorBottle - 0.02 && pMold->Sensor.UpDn.mmAs < pRobotPara->ParaUpDn.sSensorBottle + 0.02)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 3000;
				}
			
            break;
   
			/*------------------ Sensor down -------------------- */
			case 1100: 	/* delay */
				if(1 == pMold->Option.HookPin)
				{
					pMold->Sensor.Timer.IN = 1;
					pMold->Sensor.Timer.PT = 1;
					pMold->Sensor.Step = 1200;
				}
				else
				{
					pMold->Sensor.Step = 3000;
				}
				break;
	   
			case 1200: 	/* Sensoring delay */
				if (1 == pMold->Sensor.Timer.Q)
				{
					pMold->Sensor.Timer.IN = 0;
					pMold->Sensor.Step = 1300;
				}
				break;
	   
			case 1300: 	/* scan Sensor bottle */
				pMold->Sensor.LimitTimer.IN = 1;
				pMold->Sensor.LimitTimer.PT = pMold->TimeSet.SensorDnAlarmTime;

				pMold->Sensor.UpDn.s_set = pRobotPara->ParaUpDn.sSensorWaste;
				pMold->Sensor.UpDn.MoveAbsolute = 1;
				pMold->Sensor.Step = 1400;
				break;
			
			case 1400:
				pMold->TimeDis.SensorDnAlarmTime = pMold->Sensor.LimitTimer.ET;
				if(1 == pMold->Sensor.LimitTimer.Q)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 40000;
				}
			
				if(pMold->Sensor.UpDn.mmAs > pRobotPara->ParaUpDn.sSensorBottle - 0.02 && pMold->Sensor.UpDn.mmAs < pRobotPara->ParaUpDn.sSensorBottle + 0.02)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 3000;
				}
				break;
	   
			case 3000:
				break;
		
			/*  sensor calibration down    */
			case 5000:
				if(0 == pMold->Sensor.UpDn.BwHWSwitch)
				{
					pMold->Sensor.UpDn.v_set = pRobotPara->ParaUpDn.vSensorHome;
				}
				
				if(1 == pMold->Sensor.UpDn.BwHWSwitch)
				{
					pMold->HwAlarm.SensorDnHWSwitch = 1;
					pMold->BlowPin.Step 	= 25000;
				}
				break;
		
			/*  sensor up   */
         case 10100:
				if(1 == pMold->Option.HookPin)
				{
					pMold->Sensor.Timer.IN = 1;
					pMold->Sensor.Timer.PT = 1;
					pMold->Sensor.Step = 10200;
				}
				else
				{
					pMold->Sensor.Step = 13000;
				}
            break;
         
         case 10200:
				if (1 == pMold->Sensor.Timer.Q)
				{
					pMold->Sensor.Timer.IN = 0;
					pMold->Sensor.Step = 10300;
				}
            break;
   
         case 10300:
				pMold->Sensor.LimitTimer.IN = 1;
				pMold->Sensor.LimitTimer.PT = pMold->TimeSet.SensorUpAlarmTime;

				pMold->Sensor.UpDn.s_set = 0;
				pMold->Sensor.UpDn.MoveAbsolute = 1;
				pMold->Sensor.Step = 10400;
            break;
   
         case 10400:
				pMold->TimeDis.SensorUpAlarmTime = pMold->Sensor.LimitTimer.ET;
				if(1 == pMold->Sensor.LimitTimer.Q)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 40000;
				}
				
				if(pMold->Sensor.UpDn.mmAs > -0.02 && pMold->Sensor.UpDn.mmAs < 0.02)
				{
					pMold->Sensor.LimitTimer.IN = 0;
					pMold->Sensor.Step = 13000;
				}
            break;
   
         case 13000:
            break;
		
			/*  Sensor calibration up    */
			case 15000:
				if(0 == pMold->Sensor.UpDn.FwHWSwitch)
				{
					pMold->Sensor.UpDn.v_set = -pRobotPara->ParaUpDn.vSensorHome;
				}
					
				if(1 == pMold->Sensor.UpDn.BwHWSwitch)
				{
					pMold->HwAlarm.SensorUpHWSwitch = 1;
					pMold->BlowPin.Step = 25000;
				}
				break;

        /*--------------- Manual action stop -----------------*/
         case 20000:
				pMold->Sensor.UpDn.MoveAbsolute = 0;
				pMold->Sensor.UpDn.Stop = 1;
				pMold->Sensor.Timer.IN = 0;
         	pMold->Sensor.Step = 0;
            break;
		
			case 25000:
				pMold->Sensor.vRamp.Ts = tTask;
				pMold->Sensor.vRamp.Once = 0;
				pMold->Sensor.vRamp.Set = pMold->Sensor.UpDn.v_set;
				pMold->Sensor.vRamp.Accel = 1000;
				
				/* call profile function */
				HydSCurveRamp(&pMold->Sensor.vRamp);
				pMold->Sensor.UpDn.v_set = pMold->Sensor.vRamp.Out;
			
				pMold->Sensor.Step = 26000;
				break;
		
			case 26000:
				pMold->Sensor.vRamp.Set = 0;
				pMold->Sensor.vRamp.Accel = 1000;
				
				/* call profile function */
				HydSCurveRamp(&pMold->Sensor.vRamp);
				pMold->Sensor.UpDn.v_set = pMold->Sensor.vRamp.Out;
			
				if(pMold->Sensor.UpDn.v_set < 0.1 && pMold->Sensor.UpDn.v_set > -0.1)
				{
					pMold->Sensor.UpDn.v_set = 0;
					pMold->Sensor.Step = 0;
				}
				break;
			/*--------------- Instance stop -----------------*/
			case 30000:
				pMold->Sensor.UpDn.v_set = 0;
				pMold->Sensor.UpDn.MoveJogPos = 0;
				pMold->Sensor.UpDn.MoveJogNeg = 0;
           	pMold->Sensor.Timer.IN = 0;
           	pMold->Sensor.Step = 0;
            break;
		
			/*--------------- Instance stop -----------------*/
			case 40000:
				pMold->Sensor.UpDn.Stop = 1;
				pMold->Sensor.UpDn.v_set = 0;
				pMold->Sensor.Timer.IN = 0;
				pMold->Sensor.Step = 0;
				break;
	}/* end of switch */
}/*end of function */




