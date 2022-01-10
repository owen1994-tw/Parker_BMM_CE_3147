/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: blowpin2
 * File: blowpin2.c
 * Author: wu yun hung
 * Created: March 31, 2021
 ********************************************************************
 * Implementation of program blowpin2 --- up and down 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#define   INSTANCY_STOP_DECEL    3000

#define   INSTANCY_STOP_DOWN_DECEL    3000
#define   INSTANCY_STOP_UP_DECEL    3000


/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void BlowPin2Switch( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void BlowPin2LVDT( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void BlowPin2DriverDirect( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void ButtonOffDelay(Mold_typ * pMold , timer_typ * pTimer);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */

	LMold.BlowPin2.Step = 30000;
	RMold.BlowPin2.Step = 30000;
   
	LMold.BlowPin2.vRamp.Ts = tTask;
	LMold.BlowPin2.vRamp.Accel = 800;
    
	RMold.BlowPin2.vRamp.Ts = tTask;
	RMold.BlowPin2.vRamp.Accel = 800;
    
	LMold.BlowPin2.vTimeProf.Ts = tTask;
	LMold.BlowPin2.vTimeProf.accel = 800;
	LMold.BlowPin2.vTimeProf.decel = 600;
    
	RMold.BlowPin2.vTimeProf.Ts = tTask;
	RMold.BlowPin2.vTimeProf.accel = 800;
	RMold.BlowPin2.vTimeProf.decel = 600;
   
	/* differentiator for evaluating the actual speed */
	vActHydDT2_L.Ts = tTask;	/* cycle time tTasks 			  */
	vActHydDT2_L.Tv = 1.0; 	   /* derivative time constant [s]   */
	vActHydDT2_L.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_L.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */

	/* differentiator for evaluating the actual speed */
	vActHydDT2_R.Ts = tTask;	/* cycle time tTasks 			  */
	vActHydDT2_R.Tv = 1.0; 	   /* derivative time constant [s]   */
	vActHydDT2_R.T1 = 5 * tTask; 	/* filter time constant 1 : 0.01s */
	vActHydDT2_R.T2 = 5 * tTask; 	/* filter time constant 2 : 0.01s */
	
   
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	
	LMold.BlowPin2.Enable = gMachineFix.Option.HWEnable.LMold.BlowPin2;
	RMold.BlowPin2.Enable = gMachineFix.Option.HWEnable.RMold.BlowPin2;
	
	//	if(1 == gMachineOut.OilPump)
	//	{
	//		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
	//		
	//		if(tErrorDelay < tTask)
	//		{
	//			LMold.HwAlarm.BlowPinNetworkErr = !LMold.BlowPin.NetworkInit;
	//			LMold.HwAlarm.BlowPinPowerOn    = !LMold.BlowPin.PowerOn;
	//		
	//			RMold.HwAlarm.BlowPinNetworkErr = !RMold.BlowPin.NetworkInit;
	//			RMold.HwAlarm.BlowPinPowerOn    = !RMold.BlowPin.PowerOn;
	//		}
	//	}
	//	else
	//	{
	//		tErrorDelay = 0.1;
	//		
	//		LMold.HwAlarm.BlowPinNetworkErr = 0;
	//		LMold.HwAlarm.BlowPinPowerOn    = 0;
	//		
	//		RMold.HwAlarm.BlowPinNetworkErr = 0;
	//		RMold.HwAlarm.BlowPinPowerOn   = 0;
	//	}
	
	LMold.TransDIn.BlowPin2UpLimit = gMachineIn.LMold.BlowPin2UpLimit;		
	RMold.TransDIn.BlowPin2UpLimit = gMachineIn.RMold.BlowPin2UpLimit;		

	LMold.TransDIn.BlowPin2DnLimit = gMachineIn.LMold.BlowPin2DnLimit;
	RMold.TransDIn.BlowPin2DnLimit = gMachineIn.RMold.BlowPin2DnLimit;
	
	

	
	if(gMachineFix.MoldR.BlowPin2.Tolerance < 0.1)gMachineFix.MoldR.BlowPin2.Tolerance = 0.2;
	gMachineFix.MoldL.BlowPin2.Tolerance = gMachineFix.MoldR.BlowPin2.Tolerance;

	/* left blow pin velocity  */
	vActHydDT2_L.u = LMold.BlowPin2.mmAs;
	HydDT2(&vActHydDT2_L);

	LMold.BlowPin2.v_act = vActHydDT2_L.y;

	/* right blow pin velocity  */
	vActHydDT2_R.u = RMold.BlowPin2.mmAs;
	HydDT2(&vActHydDT2_R);

	RMold.BlowPin2.v_act = vActHydDT2_R.y;
   

	if(ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		if(SENSOR_LVDT == gMachineFix.MoldR.BlowPin.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				BlowPin2LVDT(&LMold,&gMachineFix.MoldL.BlowPin2,&gMachinePara.MoldL.BlowPin2,&gMachinePara.MoldL.Clamp);
			}
			BlowPin2LVDT(&RMold,&gMachineFix.MoldR.BlowPin2,&gMachinePara.MoldR.BlowPin2,&gMachinePara.MoldR.Clamp);
		}
		else if(SENSOR_SWITCH == gMachineFix.MoldR.BlowPin.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				BlowPin2Switch(&LMold,&gMachineFix.MoldL.BlowPin2,&gMachinePara.MoldL.BlowPin2,&gMachinePara.MoldL.Clamp);
			}
			BlowPin2Switch(&RMold,&gMachineFix.MoldR.BlowPin2,&gMachinePara.MoldR.BlowPin2,&gMachinePara.MoldR.Clamp);
		}
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			BlowPin2DriverDirect(&LMold,&gMachineFix.MoldL.BlowPin2,&gMachinePara.MoldL.BlowPin2,&gMachinePara.MoldL.Clamp);
		}
		BlowPin2DriverDirect(&RMold,&gMachineFix.MoldR.BlowPin2,&gMachinePara.MoldR.BlowPin2,&gMachinePara.MoldR.Clamp);
	}

	
	
	if(1 == gMachineFix.Option.bHideLeftStation)
	{
		LMold.BlowPin2.Error = 0;
		LMold.BlowPin2.CalOK = 1;
		LMold.BlowPin2.UpPos = 1;
	}
	
	gMachineInfo.RBlowPin2Pos = RMold.BlowPin2.mmAs;/*Albert*/
	gMachineInfo.LBlowPin2Pos = LMold.BlowPin2.mmAs;/*Albert*/
	
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldR.BlowPin2.eActuatorType = gMachineFix.MoldR.BlowPin.eActuatorType;
	gMachineFix.MoldL.BlowPin2.eActuatorType = gMachineFix.MoldR.BlowPin.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldR.BlowPin2.eValveType = gMachineFix.MoldR.BlowPin.eValveType;
	gMachineFix.MoldL.BlowPin2.eValveType = gMachineFix.MoldR.BlowPin.eValveType;
	
	/*-------------- SensorType----------------- */
	gMachineFix.MoldR.BlowPin2.eSensorType = gMachineFix.MoldR.BlowPin.eSensorType;
	gMachineFix.MoldL.BlowPin2.eSensorType = gMachineFix.MoldR.BlowPin.eSensorType;
	
	/*------------ MechanismType--------------- */
	gMachineFix.MoldR.BlowPin2.eMechanismType = gMachineFix.MoldR.BlowPin.eMechanismType;
	gMachineFix.MoldL.BlowPin2.eMechanismType = gMachineFix.MoldR.BlowPin.eMechanismType;

	/*---------------Button Off Delay - Manual-------------*/
	ButtonOffDelay(&RMold ,&gMacTimer.RBlowPinBtnOffDly);
	ButtonOffDelay(&LMold ,&gMacTimer.LBlowPinBtnOffDly);
	
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.BlowPin2.Position 		= RMold.BlowPin2.mmAs;
	gMotorTrace.LMold.BlowPin2.Position 		= LMold.BlowPin2.mmAs;

} /* end of _CYCLIC */
void BlowPin2Switch( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	/* No LVDT, no calibration false   */
	pMold->BlowPin2.CalOK = 1;
	pMold->Alarm.BlowPin2CalErr = 0;
	
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
   
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	
	/* Up or down  position flag  */
	pMold->BlowPin2.UpPos = pMold->TransDIn.BlowPin2UpLimit; 
	pMold->BlowPin2.DnPos = pMold->TransDIn.BlowPin2DnLimit; 
	
	/* ------------------------------------------------------ */
	/* ---------------- Step Machine Control ---------------- */
	/* ------------------------------------------------------ */
	switch ( pMold->BlowPin2.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			
			pMold->BlowPin2.v_set = 0;
			pMold->BlowPin2.p_set = 0;
			
			pMold->BlowPin2.Timer.IN  = 0;
			pMold->BlowPin2.LimitTimer.IN  = 0;
			break;

		/*------------------ BlowPin move down -------------------- */
		case 100: 	
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step = 200;
			}
			else
			{
				pMold->BlowPin2.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */

			pMold->BlowPin2.Step = 300;

			break;
		
		case 300:

			pMold->BlowPin2.Step = 500;
			break;

		case 400:	/*  down  */

			break;
		
		case 500:
			
			pMold->BlowPin2.Step = 3000;

			break;
		
		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:
			
			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* up delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step = 10200;
			}
			else
			{
				pMold->BlowPin2.Step = 13000;
			}
			break;
		case 10200: 	/* delay ok */

			pMold->BlowPin2.Step = 10300;	
			break;
      
		case 10300:

			pMold->BlowPin2.Step = 10400;
			break;
        
		case 10400:

			pMold->BlowPin2.Step = 10500;
			break;
      
		case 10500:
			
			pMold->BlowPin2.Step = 13000;
			break;
      
		case 13000:
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:
			
			break;

		/*--------------- BlowPin tiny return -----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step = 15200;
			}
			else
			{
				pMold->BlowPin2.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */
			
			pMold->BlowPin2.Step = 15300;
			break;
      
		case 15300:

			pMold->BlowPin2.Step = 15400;
			break;
              
		case 15400:

			pMold->BlowPin2.Step = 15500;	
			break;
      
		case 15500:					
			
			pMold->BlowPin2.Step = 18000;  
			break;
      
		case 18000:
			
			break;

		
		/*--------------- Manual action stop -----------------*/
		case 20000:

			pMold->BlowPin2.Step = 22000;
			break;
    			
		case 22000:
			
			pMold->BlowPin2.Step = 0;
			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
	
			pMold->BlowPin2.Step = 32000;
			break;
	    			
		case 32000:
			
			pMold->BlowPin2.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:

			break;
		
		case 41000:
			
			pMold->BlowPin2.Step = 43000;
			break;	   
	    			
		case 43000:
			
			pMold->BlowPin2.Step = 40000;
			break;
	}/* end of switch */


}
void BlowPin2LVDT( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	
	/* No LVDT, no calibration false   */
	pMold->BlowPin2.CalOK = 1;
	pMold->Alarm.BlowPin2CalErr = 0;
	
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
   
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	
	/* Up or down  position flag  */
	pMold->BlowPin2.UpPos = pMold->TransDIn.BlowPin2UpLimit; 
	pMold->BlowPin2.DnPos = pMold->TransDIn.BlowPin2DnLimit; 
	 
	/* ------------------ function current position ------------------ */

	switch ( pMold->BlowPin2.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->BlowPin2.v_set = 0;
			pMold->BlowPin2.p_set = 0;
			

			
			pMold->BlowPin2.Timer.IN  = 0;
			pMold->BlowPin2.LimitTimer.IN  = 0;
			break;
		
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;

		/*------------------ BlowPin move down -------------------- */
		case 100: 	/* down delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step 	= 200;
			}
			else
			{
				pMold->BlowPin2.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */

			pMold->BlowPin2.Step = 300;
			break;

		case 300:
		
			pMold->BlowPin2.Step = 400;
			break;

		case 400:		/*  quick down  */

			pMold->BlowPin2.Step = 500;
			break;
		
		case 500:		/*  slow down  */

			pMold->BlowPin2.Step = 700;
			break;

		case 700:
			
			pMold->BlowPin2.Step = 3000;
			break;

		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:

			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* open delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step = 10200;
			}
			else
			{
				pMold->BlowPin2.Step = 13000;
			}
			break;
		case 10200: 	/* delay ok */

			pMold->BlowPin2.Step = 10300;
			break;
      
		case 10300:

			pMold->BlowPin2.Step = 10400;
			break;
        
		case 10400:

			pMold->BlowPin2.Step = 10500;
			break;
      
		case 10500:

			pMold->BlowPin2.Step = 13000;
			break;
      
		case 13000:
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:

			break;

		/*--------------- BlowPin tiny return -----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				pMold->BlowPin2.Step = 15200;
			}
			else
			{
				pMold->BlowPin2.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */

			pMold->BlowPin2.Step = 15300;
			break;
      
		case 15300:
						
			pMold->BlowPin2.Step = 15400;
			break;
              
		case 15400:
	
			pMold->BlowPin2.Step = 15500;
			break;
      
		case 15500:	
            
			pMold->BlowPin2.Step = 18000;
			break;
      
		case 18000:

			break;

		/*--------------- Manual action stop -----------------*/
		case 20000:

			pMold->BlowPin2.Step = 22000;
			break;
    			
		case 22000:
			
			pMold->BlowPin2.Step = 0;
			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
	
			pMold->BlowPin2.Step = 32000;
			break;
	    			
		case 32000:
			
			pMold->BlowPin2.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:

			break;
		
		case 41000:
			
			pMold->BlowPin2.Step = 43000;
			break;	   
	    			
		case 43000:
			
			pMold->BlowPin2.Step = 40000;
			break;
		
	}/* end of switch */
	
    
	/* ------------------------------------------------------------------------------ */
	/* ---------------- blow pin output and proportional valve linear --------------- */
	/* ------------------------------------------------------------------------------ */
	pMold->BlowPin2.aoPropValve = 0;
	pMold->BlowPin2.PropValveOk = 1;
    
}
void BlowPin2DriverDirect( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	if(1 == pMold->BlowPin2.Enable)
	{
		if(1 == gMachineOut.OilPump)
		{
			if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		
			if(tErrorDelay < tTask)
			{
				if(pMold == & LMold)
				{
					LMold.HwAlarm.BlowPin2NetworkErr = !LMold.BlowPin2.NetworkInit;
					LMold.HwAlarm.BlowPin2PowerOn    = !LMold.BlowPin2.PowerOn;			
				}
				else
				{
					RMold.HwAlarm.BlowPin2NetworkErr = !RMold.BlowPin2.NetworkInit;
					RMold.HwAlarm.BlowPin2PowerOn    = !RMold.BlowPin2.PowerOn;	
				}			
			}
		
			if((RMold.HwAlarm.BlowPin2NetworkErr ||RMold.HwAlarm.BlowPin2PowerOn || RMold.HwAlarm.BlowPin2AxisErr)||(LMold.HwAlarm.BlowPin2NetworkErr ||LMold.HwAlarm.BlowPin2PowerOn || LMold.HwAlarm.BlowPin2AxisErr))
			{
				gMachineOut.OilPump = 0;
				gPROCESS_info.oilpumpStop = 1;   //request manual mode
			}
		}
		else
		{
			tErrorDelay = 0.5;

			if(pMold == & LMold)
			{
//				LMold.HwAlarm.BlowPin2NetworkErr = 0;
//				LMold.HwAlarm.BlowPin2PowerOn    = 0;			
			}
			else
			{
//				RMold.HwAlarm.BlowPin2NetworkErr = 0;
//				RMold.HwAlarm.BlowPin2PowerOn   = 0;
			}	
		}
	}
	
	if(0 == pMold->TimeSet.BlowPin2UpAlarmTime)pMold->TimeSet.BlowPin2UpAlarmTime = 500;
	if(0 == pMold->TimeSet.BlowPin2DnAlarmTime)pMold->TimeSet.BlowPin2DnAlarmTime = 300;
	
	if(1 == pMold->BlowPin2.Enable)
	{
		/* ------------- Blowpin2 axis alarms ------------- */
		pMold->HwAlarm.BlowPin2AxisErr = pMold->BlowPin2.Error;
	}
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pBlowPinFix->Length < 100.0)pBlowPinFix->Length = 100.0;
	if(pBlowPinFix->Tolerance < 0.1)pBlowPinFix->Tolerance = 2.0;
	pMold->BlowPin2.Tolerance = pBlowPinFix->Tolerance;
    
	if(pBlowPinFix->Down.V_Max < 200.0)pBlowPinFix->Down.V_Max = 1000.0;
	if(pBlowPinFix->Up.V_Max < 200.0)pBlowPinFix->Up.V_Max = 1000.0;
    
	if(pBlowPinFix->Down.VoltageMax > -3.0)pBlowPinFix->Down.VoltageMax = -10.0;
	if(pBlowPinFix->Up.VoltageMax < 3.0 )pBlowPinFix->Up.VoltageMax = 10.0;
	
	if(pBlowPinFix->vRelease < 5)pBlowPinFix->vRelease = 5.0;
	if(pBlowPinFix->vRelease > 20 )pBlowPinFix->vRelease = 20.0;
    
	pMold->BlowPin2.S_Up = pBlowPinPara->S_Up;
	pMold->BlowPin2.S_UpMicro = pBlowPinPara->S_UpMicro;

   
	pMold->BlowPin2.V_Up = pBlowPinPara->V_Up * pBlowPinFix->Up.V_Max/100.0;
	pMold->BlowPin2.V_UpMicro = pBlowPinPara->V_UpMicro * pBlowPinFix->Up.V_Max/100.0;
	
	pMold->BlowPin2.P_Up		= pBlowPinPara->P_Up = 100;
	pMold->BlowPin2.P_UpMicro	= pBlowPinPara->P_UpMicro = 100;
	
	pMold->BlowPin2.S_Dn = pBlowPinPara->S_Dn;
	pMold->BlowPin2.S_DnSlow = pBlowPinPara->S_DnSlow ;
    
	pMold->BlowPin2.V_Dn = pBlowPinPara->V_Dn * pBlowPinFix->Down.V_Max/100.0;
	pMold->BlowPin2.V_DnSlow = pBlowPinPara->V_DnSlow * pBlowPinFix->Down.V_Max/100.0;
	
	pMold->BlowPin2.P_Dn		= pBlowPinPara->P_Dn = 100;
	pMold->BlowPin2.P_DnSlow		= pBlowPinPara->P_DnSlow;
	
	if(0 == pBlowPinPara->V_UpReset)pBlowPinPara->V_UpReset = 2.0;
	pMold->BlowPin2.V_UpReset = pBlowPinPara->V_UpReset * pBlowPinFix->Up.V_Max/100.0;
   
	if(pBlowPinFix->Down.accel_stop < 100)pBlowPinFix->Down.accel_stop = INSTANCY_STOP_DOWN_DECEL; 
	if(pBlowPinFix->Up.accel_stop < 100)pBlowPinFix->Up.accel_stop = INSTANCY_STOP_UP_DECEL;

	/*********************************************************************************
  	**                            linear calibration status                        **
  	*********************************************************************************/
  	/* get calibration data  */
	if(pBlowPinFix->Max < pBlowPinFix->Zero)
	{
		pMold->BlowPin2.ZeroPos = pBlowPinFix->Max;
		pMold->BlowPin2.MaxPos  = pBlowPinFix->Zero;
	}
	else
	{
		pMold->BlowPin2.MaxPos  = pBlowPinFix->Max;
		pMold->BlowPin2.ZeroPos = pBlowPinFix->Zero;
	}

	if(1 == pMold->BlowPin2.Enable)
	{
		if(pMold->BlowPin2.ZeroPos != 0                            &&
			pMold->BlowPin2.MaxPos > (pMold->BlowPin2.ZeroPos + 3000) &&
			pMold->BlowPin2.As     > (pMold->BlowPin2.ZeroPos - 2500 ) &&
			pMold->BlowPin2.As     < (pMold->BlowPin2.MaxPos  + 2500 ))
		{
			pMold->BlowPin2.CalOK = pMold->BlowPin2.NetworkInit;
		}
		else
		{
			pMold->BlowPin2.CalOK = 0;
		}
	
		pBlowPinFix->CalibOK = pMold->BlowPin2.CalOK;

		if(1 == pMold->BlowPin2.CalOK)
		{
			if(pBlowPinFix->Max < pBlowPinFix->Zero)
			{
				pMold->BlowPin2.mmAs = 1.0 *(pMold->BlowPin2.MaxPos - pMold->BlowPin2.As) * pBlowPinFix->Length /
					(pMold->BlowPin2.MaxPos - pMold->BlowPin2.ZeroPos);
			}
			else
			{
				pMold->BlowPin2.mmAs = 1.0 *(pMold->BlowPin2.As - pMold->BlowPin2.ZeroPos) * pBlowPinFix->Length /
					(pMold->BlowPin2.MaxPos - pMold->BlowPin2.ZeroPos);
			}
		}
		else
		{
			pMold->BlowPin2.mmAs = pMold->BlowPin2.mmAs;
			if(0 == gMachineInfo.Calib)	/* Not calibration mode  */
			{
				if(pMold->BlowPin2.Step > 0 && pMold->BlowPin2.Step < 20000)
				{
					pMold->BlowPin2.Step = 0;
					pMold->Alarm.BlowPinCalErr = 1;
				}
			}
			else if (0 == pMold->BlowPin2.NetworkInit)
			{
				if(pMold->BlowPin2.Step > 0 && pMold->BlowPin2.Step < 20000)
				{
					pMold->BlowPin2.Step = 0;
				}
			}
		}/* if(1 == pMold->BlowPin2.CalOK) */
	
		/* network and calibration is ok, axis has an error   */
		if(0 == pMold->BlowPin2.AxisReady) 		  
		{
			if(pMold->BlowPin2.Step > 0 && pMold->BlowPin2.Step < 20000)
			{
				pMold->BlowPin2.Step = 30000;
			}
		}
	}
	/*  relative position  */
	pMold->BlowPin2.Ss = pMold->BlowPin2.As;
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	if(1 == pMold->BlowPin2.Enable)
	{
		if(1 == pMold->BlowPin2.CalOK)
		{
			pMold->BlowPin2.UpPos = pMold->TransDIn.BlowPin2UpLimit & (pMold->BlowPin2.mmAs < (pBlowPinPara->S_Up + pBlowPinFix->Tolerance)); 
			pMold->BlowPin2.DnPos = pMold->TransDIn.BlowPin2DnLimit & (pMold->BlowPin2.mmAs > (pBlowPinPara->S_Dn - pBlowPinFix->Tolerance)); 
		}
		else
		{
			pMold->BlowPin2.UpPos = 0;
			pMold->BlowPin2.DnPos = 0;
		}
	}
	 
	/* ------------------ function current position ------------------ */
	pMold->BlowPin2.vProfGen.s_act = pMold->BlowPin2.mmAs;
	pMold->BlowPin2.vProfGen.v_act = pMold->BlowPin2.v_act;

	switch ( pMold->BlowPin2.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->BlowPin2.v_set = 0;
			pMold->BlowPin2.p_set = 0;
			pMold->BlowPin2.Timer.IN  = 0;
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
            

			break;

		/*------------------ BlowPin2 move down -------------------- */
		case 100: 	/* down delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				if(pMold->TimeSet.BlowPin2DnDelay > 0)
				{
					pMold->BlowPin2.Timer.IN = 1;
					pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2DnDelay;
					pMold->BlowPin2.Step 	= 200;
				}
				else
				{
					if(0 == gMacOption.bDryCycle)
					{
						pMold->BlowPin2.Step = 300;
					}
					else
					{
						pMold->BlowPin2.Step = 3000;
					}
				}
			}
			else
			{
				pMold->BlowPin2.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.BlowPin2DnDelay = pMold->BlowPin2.Timer.ET;
			if (1 == pMold->BlowPin2.Timer.Q)
			{
				pMold->BlowPin2.Timer.IN       = 0;
				//		pMold->TimeDis.BlowPin2DnDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin2.Step = 300;
				}
				else
				{
					pMold->BlowPin2.Step = 3000;
				}
			}
			break;

		case 300:
			if(1 == pMold->Option.BlowPin2)
			{
				if(0 == pMold->BlowPin2.DnHWSwitch)
				{
					pMold->BlowPin2.LimitTimer.IN = 1;
					pMold->BlowPin2.LimitTimer.PT = pMold->TimeSet.BlowPin2DnAlarmTime;
					
					pMold->BlowPin2.vProfGen.Ts = tTask;
					pMold->BlowPin2.vProfGen.Tj = pBlowPinFix->Down.Tj;
					pMold->BlowPin2.vProfGen.Td = pBlowPinFix->Down.Td;
					
					pMold->BlowPin2.vProfGen.accel = pBlowPinFix->Down.accel;
					if(pMold->BlowPin2.vProfGen.accel <= 0.0)pMold->BlowPin2.vProfGen.accel = 1000;
					
					pMold->BlowPin2.vProfGen.decel = pBlowPinFix->Down.accel;
					if(pMold->BlowPin2.vProfGen.decel <= 0.0)pMold->BlowPin2.vProfGen.decel = 1000;
					            
					pMold->BlowPin2.vProfGen.decel_end = pBlowPinFix->Down.decel_end;
					if(pMold->BlowPin2.vProfGen.decel_end <= 0.0)pMold->BlowPin2.vProfGen.decel_end = 100;
					            
					pMold->BlowPin2.vProfGen.v_start = 0.0;
					pMold->BlowPin2.vProfGen.v_set   = pMold->BlowPin2.V_Dn;
			
					if(pMold->TimeSet.BlowPin2DnPressTime == 0 || 1 == gMacOption.bDryCycle || 1 == gMachineInfo.Manual )
					{
						pMold->BlowPin2.vProfGen.v_end = 0;
					}
					else
					{
						pMold->BlowPin2.vProfGen.v_end	= pMold->BlowPin2.V_DnSlow/2;
					}
					pMold->BlowPin2.vProfGen.s_end = pMold->BlowPin2.S_Dn;
					pMold->BlowPin2.vProfGen.init  = 1;
						
					pMold->BlowPin2.Step = 400;
				}
				else
				{
					pMold->HwAlarm.BlowPin2DnHWSwitch = 1;
					pMold->BlowPin2.Step 	= 41000;
				}
			}
			else
			{
				pMold->BlowPin2.Step = 3000;
			}
			break;

		case 400:		/*  quick down  */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin2.vProfGen.v_set = pMold->BlowPin2.V_UpReset;
			}
			else
			{
				pMold->BlowPin2.vProfGen.v_set = pMold->BlowPin2.V_Dn;
			}
			
            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin2.vProfGen);
            
			pMold->BlowPin2.p_set = pMold->BlowPin2.P_Dn;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vProfGen.v_out;
			
			pMold->BlowPin2.actTjDn = pMold->BlowPin2.vProfGen.Tj;
			
			/*  reached target position */
			if(pMold->BlowPin2.mmAs >= pMold->BlowPin2.S_Dn - pMold->BlowPin2.Tolerance) //ipis0713
			{
				
				if(0 == gMacOption.bDryCycle)
				{
					if(1 == gMachineInfo.Auto)   //auto
					{
						if(1 == gMachineInfo.MachineReset)  //auto in reset
						{
							pMold->BlowPin2.Step = 700;
						}
						else
						{
							pMold->BlowPin2.Step = 500;
						}	  
					}
					else
					{
						pMold->BlowPin2.Step = 700;  
					}
					
				}
				else
				{
					pMold->BlowPin2.Step = 700;  /* to end  */
				}
			}
            
			pMold->TimeDis.BlowPin2DnAlarmTime = pMold->BlowPin2.LimitTimer.ET;

			
			if(1 == pMold->BlowPin2.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPin2DnHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
     
		case 500:
			if(pMold->TimeSet.BlowPin2DnPressTime != 0)
			{
				pMold->ActInfo.BlowPinDn  = LED_ON;
				pMold->ActInfo.BlowPinUp  = LED_OFF;

	            
				pMold->BlowPin2.Timer.IN = 1;
				pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2DnPressTime;
	            
				pMold->BlowPin2.vTimeProf.Ts = tTask;
				pMold->BlowPin2.vTimeProf.t_move = pMold->TimeSet.BlowPin2DnPressTime * 0.01;
	            
				pMold->BlowPin2.vTimeProf.v_set   = pMold->BlowPin2.V_DnSlow; 
				pMold->BlowPin2.vTimeProf.v_start = pMold->BlowPin2.v_set; 
				pMold->BlowPin2.vTimeProf.v_end   = 0.0;
	            
				pMold->BlowPin2.vTimeProf.accel = 2000;
				if(pMold->BlowPin2.vTimeProf.accel <= 0.0)pMold->BlowPin2.vTimeProf.accel = 1000;
	            
				pMold->BlowPin2.vTimeProf.decel = 2000;
				if(pMold->BlowPin2.vTimeProf.decel <= 0.0)pMold->BlowPin2.vTimeProf.decel = 1000;
	            
				pMold->BlowPin2.vTimeProf.init = 1;
				pMold->BlowPin2.Step = 600;
			}
			else
			{
				pMold->BlowPin2.Step = 700;
			}
			break;

		case 600:		/*  quick down (Press torque procedure) */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			HydvTimeProf(&pMold->BlowPin2.vTimeProf);
				
			pMold->BlowPin2.p_set = pMold->BlowPin2.P_DnSlow;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vTimeProf.v_out;

			pMold->TimeDis.BlowPin2DnPressTime = pMold->BlowPin2.Timer.ET;
			if (1 == pMold->BlowPin2.Timer.Q)
			{
				pMold->BlowPin2.v_set = 0;
				pMold->BlowPin2.Timer.IN      = 0;
				//				pMold->TimeDis.BlowPin2DnPressTime = 0;
				pMold->BlowPin2.Step = 700;
			}
			

			pMold->TimeDis.BlowPin2DnAlarmTime = pMold->BlowPin2.LimitTimer.ET;

			
			if(1 == pMold->BlowPin2.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPin2DnHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;

		case 700:
			pMold->BlowPin2.v_set = 0;
			
			pMold->ActInfo.BlowPinDn  = LED_OFF;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			
			pMold->BlowPin2.Timer.IN      = 0;
			if(1 == gMachineInfo.Auto)
			{
				if(1 == gMachineInfo.MachineReset)  //auto in reset
				{
					pMold->BlowPin2.LimitTimer.IN = 0;
					pMold->BlowPin2.Step = 3000;
				}
				else
				{
					pMold->BlowPin2.Step = 1300;  /*Blow pin up to S_Dn*/
				}
			}
			else
			{
				pMold->BlowPin2.LimitTimer.IN = 0;
				pMold->BlowPin2.Step = 3000;  
			}

			
			break;
		

      
		/*  ------------------- Blow pin up to S_Dn ------------------- */ 
		case 1300:

			if(0 == pMold->BlowPin2.UpHWSwitch)
			{
				//					pMold->BlowPin2.LimitTimer.IN = 1;
				//					pMold->BlowPin2.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
					
				pMold->BlowPin2.vProfGen.Ts = tTask;
				//				pMold->BlowPin2.vProfGen.Tj = pBlowPinFix->Up.Tj;
				//				pMold->BlowPin2.vProfGen.Td = pBlowPinFix->Up.Td;
			
				pMold->BlowPin2.vProfGen.Tj = 0;
				pMold->BlowPin2.vProfGen.Td = 0;
					
				//				pMold->BlowPin2.vProfGen.accel = pBlowPinFix->Up.accel;
				//				if(pMold->BlowPin2.vProfGen.accel <= 0.0)pMold->BlowPin2.vProfGen.accel = 1000;
				//					
				//				pMold->BlowPin2.vProfGen.decel = pBlowPinFix->Up.accel;
				//				if(pMold->BlowPin2.vProfGen.decel <= 0.0)pMold->BlowPin2.vProfGen.decel = 1000;
				//				
				//				pMold->BlowPin2.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
				//				if(pMold->BlowPin2.vProfGen.decel_end <= 0.0)pMold->BlowPin2.vProfGen.decel_end = 100;
				
				pMold->BlowPin2.vProfGen.accel = 1000;
				pMold->BlowPin2.vProfGen.decel = 1000;
				pMold->BlowPin2.vProfGen.decel_end = 100;
					            

					            
				pMold->BlowPin2.vProfGen.v_start = 0.0;
				pMold->BlowPin2.vProfGen.v_set = pBlowPinFix->vRelease*10;
				//				pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset;
				pMold->BlowPin2.vProfGen.v_end = 0.0;
				pMold->BlowPin2.vProfGen.s_end = pMold->BlowPin2.S_Up;
				pMold->BlowPin2.vProfGen.init  = 1;
						
				pMold->BlowPin2.Step = 1400;
			}
			else
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}

			break;
              
		case 1400:
			pMold->ActInfo.BlowPinDn  = LED_OFF;
			pMold->ActInfo.BlowPinUp  = LED_ON;

			pMold->BlowPin.vProfGen.v_set = pBlowPinFix->vRelease*10;
			//			pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset;
	            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin2.vProfGen);
	 			
			pMold->BlowPin2.p_set = pMold->BlowPin2.P_Dn;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vProfGen.v_out;
					
			pMold->BlowPin2.actTjDn = pMold->BlowPin2.vProfGen.Tj;
				 
			/*  reached target position */
			//	if(pMold->BlowPin2.mmAs <= pMold->BlowPin2.vProfGen.s_end + 0.01)
			//			if(pMold->BlowPin2.mmAs <= pMold->BlowPin2.S_Dn + pBlowPinFix->Tolerance)	//ipis0724
			if(pMold->BlowPin2.mmAs <= pMold->BlowPin2.S_Up + 0.01 || pMold->BlowPin2.p_act <= pBlowPinFix->pRelease)	//ipis0724
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.Step = 1500;
			}
				
			//			pMold->TimeDis.BlowPin2UpAlarmTime = pMold->BlowPin2.LimitTimer.ET;

				
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			else if(1 == pMold->BlowPin2.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
      
		case 1500:
			pMold->BlowPin2.Timer.IN   = 0;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			pMold->BlowPin2.LimitTimer.IN = 0;

			pMold->BlowPin2.v_set = 0;
            
			pMold->BlowPin2.Step = 3000;
			break;
		

		//		/*  ------------------- Blow pin 2nd down ------------------- */
		//		case 2100:
		//			if(pMold->Clamp.mmAs >= pClampPara->S_BlowpinOut)
		//			{
		//				if(1 == pMold->OncePin2ndDn)
		//				{
		//					pMold->OncePin2ndDn = 0;
		//					pMold->BlowPin2.Step = 2200;
		//				}
		//			}
		//			break;
		//
		//		case 2200:
		//			pMold->BlowPin2.Timer.IN = 1;
		//			pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2DnDelay;
		//            
		//			pMold->BlowPin2.Step = 2300;
		//			break;
		//
		//		case 2300:
		//			pMold->TimeDis.BlowPin2DnDelay = pMold->BlowPin2.Timer.ET;
		//			if (1 == pMold->BlowPin2.Timer.Q)
		//			{
		//				pMold->BlowPin2.Timer.IN = 0;
		//				pMold->TimeDis.BlowPin2DnDelay = 0;
		//				pMold->BlowPin2.Step = 2400;
		//			}
		//			break;
		//         
		//		case 2400:
		//			pMold->BlowPin2.Timer.IN = 1;
		//			pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2DnTime;
		//            
		//			pMold->BlowPin2.vTimeProf.Ts = tTask;
		//			pMold->BlowPin2.vTimeProf.t_move = pMold->TimeSet.BlowPin2DnTime * 0.01;
		//            
		//			pMold->BlowPin2.vTimeProf.v_set   = pBlowPinPara->V_Dn2nd * 10.0; 
		//			pMold->BlowPin2.vTimeProf.v_start = 0.0; 
		//			pMold->BlowPin2.vTimeProf.v_end   = 0.0;
		//            
		//			pMold->BlowPin2.vTimeProf.accel = pBlowPinFix->Down.accel;
		//			if(pMold->BlowPin2.vTimeProf.accel <= 0.0)pMold->BlowPin2.vTimeProf.accel = 1000;
		//            
		//			pMold->BlowPin2.vTimeProf.decel = pBlowPinFix->Down.decel;
		//			if(pMold->BlowPin2.vTimeProf.decel <= 0.0)pMold->BlowPin2.vTimeProf.decel = 1000;
		//            
		//			pMold->BlowPin2.vTimeProf.init = 1;
		//			pMold->BlowPin2.Step = 2500;
		//			break;
		//
		//		case 2500:
		//			pMold->ActInfo.BlowPinDn  = LED_ON;
		//                
		//			HydvTimeProf(&pMold->BlowPin2.vTimeProf);
		//            
		//			pMold->BlowPin2.p_set = pBlowPinPara->P_Dn2nd;
		//			pMold->BlowPin2.v_set = pMold->BlowPin2.vTimeProf.v_set;
		//            
		//			pMold->TimeDis.BlowPin2DnTime = pMold->BlowPin2.Timer.ET;
		//			if (1 == pMold->BlowPin2.Timer.Q)
		//			{
		//				pMold->BlowPin2.v_set = 0;
		//				pMold->BlowPin2.Timer.IN = 0;
		//				pMold->TimeDis.BlowPin2DnTime = 0;
		//				pMold->BlowPin2.Step = 2600;
		//			}
		//			
		//			if(1 == pMold->BlowPin2.DnHWSwitch)
		//			{
		//				pMold->HwAlarm.BlowPin2DnHWSwitch = 1;
		//				pMold->BlowPin2.Step 	= 41000;
		//			}
		//			break;
		//
		//		case 2600:
		//			pMold->BlowPin2.Timer.IN = 0;
		//			pMold->BlowPin2.LimitTimer.IN = 0;
		//            
		//			pMold->ActInfo.BlowPinUp  = LED_OFF;
		//			pMold->ActInfo.BlowPinDn  = LED_OFF;
		//            
		//			pMold->BlowPin2.v_set = 0;
		//            
		//			pMold->BlowPin2.Step = 3000;
		//			break;

		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:
			if(0 == pMold->BlowPin2.DnHWSwitch)
			{
				pMold->ActInfo.BlowPinDn  = LED_ON;

				pMold->BlowPin2.p_set = pBlowPinFix->pCalibMax;
				pMold->BlowPin2.v_set = pBlowPinFix->vCalibMax;
				
			}
			
			if(1 == pMold->BlowPin2.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPin2DnHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* open delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				//				if(pMold->TimeSet.BlowPin2UpDelay > 0)
				//				{
				//					pMold->BlowPin2.Timer.IN = 1;
				//					pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2UpDelay;
				//					pMold->BlowPin2.Step = 10200;
				//				}
				//				else
				//				{
				//					if(0 == gMacOption.bDryCycle)
				//					{
				//						pMold->BlowPin2.Step = 10300;
				//					}
				//					else
				//					{
				//						pMold->BlowPin2.Step = 13000;
				//					}
				//				}
			
				if(0 == gMacOption.bDryCycle)  //ipis0719
				{
					if(pMold->TimeSet.BlowPin2UpDelay > 0)
					{
						pMold->BlowPin2.Timer.IN = 1;
						pMold->BlowPin2.Timer.PT = pMold->TimeSet.BlowPin2UpDelay;
						pMold->BlowPin2.Step = 10200;
					}
					else
					{
						pMold->BlowPin2.Step = 10300;
					}	
					
				}
				else
				{
					pMold->BlowPin2.Step = 13000;
				}	
			}
			else
			{
				pMold->BlowPin2.Step = 13000;
			}
			break;
		
		case 10200: 	/* delay ok */
			pMold->TimeDis.BlowPin2UpDelay = pMold->BlowPin2.Timer.ET;
			if (1 == pMold->BlowPin2.Timer.Q)
			{
				pMold->BlowPin2.Timer.IN       = 0;
				//		pMold->TimeDis.BlowPin2UpDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin2.Step = 10300;
				}
				else
				{
					pMold->BlowPin2.Step = 13000;
				}
			}
			break;
      
		case 10300:
			if(1 == pMold->Option.BlowPin2)
			{

				pMold->BlowPin2.LimitTimer.IN = 1;
				pMold->BlowPin2.LimitTimer.PT = pMold->TimeSet.BlowPin2UpAlarmTime;
					
				pMold->BlowPin2.vProfGen.Ts = tTask;
				pMold->BlowPin2.vProfGen.Tj = pBlowPinFix->Up.Tj;
				pMold->BlowPin2.vProfGen.Td = pBlowPinFix->Up.Td;
					
				pMold->BlowPin2.vProfGen.accel = pBlowPinFix->Up.accel;
				if(pMold->BlowPin2.vProfGen.accel <= 0.0)pMold->BlowPin2.vProfGen.accel = 1000;
					
				pMold->BlowPin2.vProfGen.decel = pBlowPinFix->Up.accel;
				if(pMold->BlowPin2.vProfGen.decel <= 0.0)pMold->BlowPin2.vProfGen.decel = 1000;
					            
				pMold->BlowPin2.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
				if(pMold->BlowPin2.vProfGen.decel_end <= 0.0)pMold->BlowPin2.vProfGen.decel_end = 100;
			
		
				pMold->BlowPin2.vProfGen.v_start = 0.0;
				pMold->BlowPin2.vProfGen.v_set   = pMold->BlowPin2.V_Up ;
				pMold->BlowPin2.vProfGen.v_end = 0.0;
				pMold->BlowPin2.vProfGen.s_end = pMold->BlowPin2.S_Up;
				pMold->BlowPin2.vProfGen.init  = 1;	

				pMold->BlowPin2.Step = 10400;
			}
			else // for no use check limit
			{
				if (1 == pMold->BlowPin2.UpPos)
				{
					pMold->BlowPin2.Step = 10500;
				}
				else				
				{
					pMold->Alarm.BlowPin2NotAtUpPos = !pMold->BlowPin2.UpPos;
					pMold->BlowPin2.Step = 40000;           
				}
			}
			
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
        
		case 10400:
			pMold->ActInfo.BlowPinUp  = LED_ON;
			pMold->ActInfo.BlowPinDn  = LED_OFF;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin2.vProfGen.v_set = pMold->BlowPin2.V_UpReset;
			}
			else
			{
				pMold->BlowPin2.vProfGen.v_set = pMold->BlowPin2.V_Up;
			}

			
            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin2.vProfGen);
 			
			pMold->BlowPin2.p_set = pMold->BlowPin2.P_Up;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vProfGen.v_out;
				
			pMold->BlowPin2.actTjUp = pMold->BlowPin2.vProfGen.Tj;
			
			/*  reached target position */
			if((pMold->BlowPin2.mmAs <= pMold->BlowPin2.S_Up + pMold->BlowPin2.Tolerance)  /*&& (1 == pMold->TransDIn.BlowPinUpLimit)*/) //ipis0713
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.LimitTimer.IN = 0;
				pMold->BlowPin2.Step = 10500;
			}
			
			pMold->TimeDis.BlowPin2UpAlarmTime = pMold->BlowPin2.LimitTimer.ET;
			
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
      
		case 10500:
			pMold->BlowPin2.Timer.IN      = 0;
			pMold->BlowPin2.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp  = LED_OFF;
			pMold->ActInfo.BlowPinDn  = LED_OFF;

			pMold->BlowPin2.v_set = 0;

			pMold->BlowPin2.Step = 13000;
			break;
      
		case 13000:
			
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:
			if(0 == pMold->BlowPin2.UpHWSwitch)
			{

				pMold->ActInfo.BlowPinUp  = LED_ON;
				pMold->ActInfo.BlowPinDn  = LED_OFF;
			

				pMold->BlowPin2.p_set = pBlowPinFix->pCalibZero;
				pMold->BlowPin2.v_set = -pBlowPinFix->vCalibZero;
			
			}
			
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
		
		


		/*--------------- BlowPin tiny return (PreLift)-----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->Option.BlowPin2)
			{
				if(pMold->TimeSet.Blowpin2tinyreturnDelay > 0)
				{
					pMold->BlowPin2.Timer.IN = 1;
					pMold->BlowPin2.Timer.PT = pMold->TimeSet.Blowpin2tinyreturnDelay;
					pMold->BlowPin2.Step 	= 15200;
				}
				else
				{
					if(0 == gMacOption.bDryCycle)
					{
						pMold->BlowPin2.Step = 15300;
					}
					else
					{
						pMold->BlowPin2.Step = 18000;
					}
				}
			}
			else
			{
				pMold->BlowPin2.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */
			pMold->TimeDis.Blowpin2tinyreturnDelay = pMold->BlowPin2.Timer.ET;
			if (1 == pMold->BlowPin2.Timer.Q)
			{
				pMold->BlowPin2.Timer.IN       = 0;
				pMold->TimeDis.Blowpin2tinyreturnDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin2.Step = 15300;
				}
				else
				{
					pMold->BlowPin2.Step = 18000;
				}
			}
			break;
      
		case 15300:
			if(1 == pMold->Option.BlowPin2)
			{
				if(0 == gMacOption.bDryCycle) //ipis0719
				{
					if(0 == pMold->BlowPin2.UpHWSwitch)
					{
						pMold->BlowPin2.LimitTimer.IN = 1;
						pMold->BlowPin2.LimitTimer.PT = pMold->TimeSet.BlowPin2UpAlarmTime;
						
						pMold->BlowPin2.vProfGen.Ts = tTask;
						//					pMold->BlowPin2.vProfGen.Tj = pBlowPin2Fix->Up.Tj;
						//					pMold->BlowPin2.vProfGen.Td = pBlowPin2Fix->Up.Td;
						//						
						//					pMold->BlowPin2.vProfGen.accel = pBlowPin2Fix->Up.accel;
						//					if(pMold->BlowPin2.vProfGen.accel <= 0.0)pMold->BlowPin2.vProfGen.accel = 1000;
						//						
						//					pMold->BlowPin2.vProfGen.decel = pBlowPin2Fix->Up.accel;
						//					if(pMold->BlowPin2.vProfGen.decel <= 0.0)pMold->BlowPin2.vProfGen.decel = 1000;
						//						            
						//					pMold->BlowPin2.vProfGen.decel_end = pBlowPin2Fix->Up.decel_end;
						//					if(pMold->BlowPin2.vProfGen.decel_end <= 0.0)pMold->BlowPin2.vProfGen.decel_end = 100;
					
						pMold->BlowPin2.vProfGen.Tj = 0;
						pMold->BlowPin2.vProfGen.Td = 0;
					
						pMold->BlowPin2.vProfGen.accel = 1000;
						pMold->BlowPin2.vProfGen.decel = 1000;
						pMold->BlowPin2.vProfGen.decel_end = 100;
					
						            
						pMold->BlowPin2.vProfGen.v_start = 0.0;
						pMold->BlowPin2.vProfGen.v_set   = pMold->BlowPin2.V_UpMicro;
						pMold->BlowPin2.vProfGen.v_end = 0.0;
						pMold->BlowPin2.vProfGen.s_end = pMold->BlowPin2.vProfGen.s_act - pMold->BlowPin2.S_UpMicro;
						pMold->BlowPin2.vProfGen.init  = 1;
							
						pMold->BlowPin2.Step = 15400;
					}
					else
					{
						pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
						pMold->BlowPin2.Step 	= 41000;
					}
				}
				else
				{
					pMold->BlowPin2.Step = 18000; 
				}
			}
			else
			{
				pMold->BlowPin2.Step = 18000; 
			}
			
			
			break;
              
		case 15400:
			pMold->ActInfo.BlowPinUp  = LED_ON;
			pMold->ActInfo.BlowPinDn  = LED_OFF;

			pMold->BlowPin2.vProfGen.v_set = pMold->BlowPin2.V_UpMicro ;
	            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin2.vProfGen);
	 			
			pMold->BlowPin2.p_set = pMold->BlowPin2.P_UpMicro;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vProfGen.v_out;
					
			pMold->BlowPin2.actTjUp = pMold->BlowPin2.vProfGen.Tj;
				
			/*  reached target position */
			//	if(pMold->BlowPin2.mmAs <= pMold->BlowPin2.vProfGen.s_end + 0.01)
			if(pMold->BlowPin2.mmAs <= pMold->BlowPin2.vProfGen.s_end + pBlowPinFix->Tolerance)	//ipis0724
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.LimitTimer.IN = 0;
				pMold->BlowPin2.Step = 15500;
			}
				
			pMold->TimeDis.BlowPin2UpAlarmTime = pMold->BlowPin2.LimitTimer.ET;

				
			if(1 == pMold->BlowPin2.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPin2UpHWSwitch = 1;
				pMold->BlowPin2.Step 	= 41000;
			}
			break;
      
		case 15500:
			pMold->BlowPin2.Timer.IN   = 0;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			pMold->BlowPin2.v_set = 0;
            
			pMold->BlowPin2.Step = 18000;
			break;
      
		case 18000:
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			break;

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->BlowPin2.Timer.IN      = 0;
			pMold->BlowPin2.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin2.p_old = pMold->BlowPin2.p_set;
			pMold->BlowPin2.Step = 21000;
			break;
        
		case 21000:	
			pMold->BlowPin2.vRamp.Internal.Once = 0;
			pMold->BlowPin2.vRamp.Set = pMold->BlowPin2.v_set;
 				
			pMold->BlowPin2.vRamp.Ts = tTask;
			
			if(pMold->BlowPin2.v_set > 0)
			{
				pMold->BlowPin2.vRamp.Accel = pBlowPinFix->Down.accel_stop;
			}
			else
			{
				pMold->BlowPin2.vRamp.Accel = pBlowPinFix->Up.accel_stop;
			}
				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
 				
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
			pMold->BlowPin2.p_set = pMold->BlowPin2.p_old;
 				
			pMold->BlowPin2.Step = 22000;

			break;
    			
		case 22000:
			pMold->BlowPin2.vRamp.Ts = tTask;
			pMold->BlowPin2.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
            
			pMold->BlowPin2.p_set = pMold->BlowPin2.p_old;
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
			
			if(pMold->BlowPin2.v_set < 0.1 && pMold->BlowPin2.v_set > -0.1)
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.Step = 0;
			
				pMold->BlowPin2.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch
			}

			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->BlowPin2.Timer.IN      = 0;
			pMold->BlowPin2.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin2.vRamp.Internal.Once = 0;
			pMold->BlowPin2.vRamp.Set = pMold->BlowPin2.v_set;
	 				
			pMold->BlowPin2.vRamp.Ts = tTask;
			pMold->BlowPin2.vRamp.Accel = pBlowPinFix->Down.accel_stop;
	 				
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
	
			pMold->BlowPin2.Step = 31000;
			break;
		
		case 31000:
			pMold->BlowPin2.vRamp.Ts  = tTask;
			pMold->BlowPin2.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
	 				
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
	 				
			if(pMold->BlowPin2.v_set < 0.1 && pMold->BlowPin2.v_set > -0.1)
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.Step = 32000;
			
				pMold->BlowPin2.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch
			}
			break;
	    			
		case 32000:
			pMold->BlowPin2.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:
			pMold->BlowPin2.Timer.IN      = 0;
			pMold->BlowPin2.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin2.p_set = 0;
			pMold->BlowPin2.v_set = 0;
			break;
		
		case 41000:
			pMold->BlowPin2.Timer.IN      = 0;
			pMold->BlowPin2.LimitTimer.IN = 0;
	
			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
	            
			pMold->BlowPin2.vRamp.Internal.Once = 0;
			pMold->BlowPin2.vRamp.Set = pMold->BlowPin2.v_set;
	 				
			pMold->BlowPin2.vRamp.Ts = tTask;
			pMold->BlowPin2.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
	
			pMold->BlowPin2.Step = 42000;
			break;
	        
		case 42000:
			pMold->BlowPin2.vRamp.Ts  = tTask;
			pMold->BlowPin2.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin2.vRamp);
	 				
			pMold->BlowPin2.v_set = pMold->BlowPin2.vRamp.Out;
	 				
			if(pMold->BlowPin2.v_set < 0.1 && pMold->BlowPin2.v_set > -0.1)
			{
				pMold->BlowPin2.v_set = 0.0;
				pMold->BlowPin2.Step = 43000;
			}
			break;
	    			
		case 43000:
			pMold->BlowPin2.Step = 40000;
			break;
	}/* end of switch */
	
	
	/* ----------------- blow pin up / down timeout --------------------  */
	if(1 == pMold->BlowPin2.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BlowPin2.Step < 1500 && pMold->BlowPin2.Step > 200)		/* BlowPin Down  */
		{
			pMold->Alarm.BlowPin2NotAtDnPos = 1;
		}
		
		if((pMold->BlowPin2.Step < 13000 && pMold->BlowPin2.Step > 10200)		/* BlowPin Up  */
			||(pMold->BlowPin2.Step < 18000 && pMold->BlowPin2.Step > 15300) )	/* BlowPin Up Micro  */
		{
			pMold->Alarm.BlowPin2NotAtUpPos = 1;
		}
      
		pMold->BlowPin2.LimitTimer.IN = 0;
		pMold->BlowPin2.Step = 40000;
	}

	/* ----------------- blow pin down action limit --------------------  */
	if( pMold->BlowPin2.Step > 100 && pMold->BlowPin2.Step < 3000)  /*  blow pin down  */
	{
		
		//		if(0 == pMold->Clamp.OpnPos && 0 == pMold->Clamp.ClsPos )
		//		{
		//			pMold->Alarm.ClampNotAtOpnClsPos = 1;
		//			pMold->BlowPin2.Step = 40000;
		//		}
		
		if( 0 == pMold->Carriage.FwPos && 0 == pMold->Carriage.BwPos )
		{
			pMold->Alarm.CarriageNotAtFwBwPos = 1;
			pMold->BlowPin2.Step = 40000;
		}
		
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->Clamp.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->Clamp.Step = 41000;
		}
		
		//		if(1 == gMachineInfo.Auto)	
		//		{
		//			if(0 == pMold->Carriage.BwPos)
		//			{
		//				pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//				pMold->BlowPin2.Step = 40000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin2.Step = 40000;
		//			}
		//			
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin2.Step = 40000;
		//			}
		//			
		//			0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos
		//		}
	}/* if( pMold->BlowPin2.Step > 100 && pMold->BlowPin2.Step < 3000) */
	
    
}/*end of function */

void ButtonOffDelay(Mold_typ * pMold,timer_typ * pTimer)
{
	if(  1 == pMold->BlowPin2.BtnOffDlyFlag )
	{
		pTimer->IN = 1;
		pTimer->PT = 100;
	
		if( 1 == pTimer->Q)
		{
			pMold->BlowPin2.BtnOffDlyFlag = 0;
		}
	}
	else
	{
		pTimer->IN = 0;
	}
	
}
