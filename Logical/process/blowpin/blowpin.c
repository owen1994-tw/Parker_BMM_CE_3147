/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: blowpin
 * File: blowpin.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program blowpin --- up and down 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#define   INSTANCY_STOP_DECEL    3000
#define   INSTANCY_STOP_ACCEL    3000


/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void BlowPinSwitch( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void BlowPinLVDT( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void BlowPinDriverDirect( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara);
void ButtonOffDelay(Mold_typ * pMold , timer_typ * pTimer);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */

	LMold.BlowPin.Step = 30000;
	RMold.BlowPin.Step = 30000;

	LMold.BlowPin.Enable = 1;
	RMold.BlowPin.Enable = 1;
   
	LMold.BlowPin.vRamp.Ts = tTask;
	LMold.BlowPin.vRamp.Accel = 800;
    
	RMold.BlowPin.vRamp.Ts = tTask;
	RMold.BlowPin.vRamp.Accel = 800;
    
	LMold.BlowPin.vTimeProf.Ts = tTask;
	LMold.BlowPin.vTimeProf.accel = 800;
	LMold.BlowPin.vTimeProf.decel = 600;
    
	RMold.BlowPin.vTimeProf.Ts = tTask;
	RMold.BlowPin.vTimeProf.accel = 800;
	RMold.BlowPin.vTimeProf.decel = 600;
   
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
	
	//Prop Valve Output Inverse
	bBlowPinPropValveOutputInverse  = gMachineFix.Option.bBlowpinPropValveOutputInverse;
	
	LMold.TransDIn.BlowPinUpLimit = gMachineIn.LMold.BlowPinUpLimit && gMachineIn.LMold.BlowPinUpLimit_2;		
	RMold.TransDIn.BlowPinUpLimit = gMachineIn.RMold.BlowPinUpLimit && gMachineIn.RMold.BlowPinUpLimit_2;		

	LMold.TransDIn.BlowPinDnLimit = gMachineIn.LMold.BlowPinDnLimit;
	RMold.TransDIn.BlowPinDnLimit = gMachineIn.RMold.BlowPinDnLimit;
	
	
	/*------------------------------------------------------------------------------------------------------*/
	/*2018.7.6 Albert Make gMachinePara.MoldR.Clamp.S_BlowpinOut same as gMachinePara.MoldL.Clamp.S_OpnSlow */
	/*------------------------------------------------------------------------------------------------------*/
	gMachinePara.MoldL.Clamp.S_BlowpinOut = gMachinePara.MoldL.Clamp.S_OpnSlow;
	gMachinePara.MoldR.Clamp.S_BlowpinOut = gMachinePara.MoldR.Clamp.S_OpnSlow;
	/*------------------------------------------------------------------------------------------------------*/

	/* left blow pin velocity  */
	vActHydDT2_L.u = LMold.BlowPin.mmAs;
	HydDT2(&vActHydDT2_L);

	LMold.BlowPin.v_act = vActHydDT2_L.y;

	/* right blow pin velocity  */
	vActHydDT2_R.u = RMold.BlowPin.mmAs;
	HydDT2(&vActHydDT2_R);

	RMold.BlowPin.v_act = vActHydDT2_R.y;
   
	/*  run function  */
	if(ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		if(SENSOR_LVDT == gMachineFix.MoldR.BlowPin.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				BlowPinLVDT(&LMold,&gMachineFix.MoldL.BlowPin,&gMachinePara.MoldL.BlowPin,&gMachinePara.MoldL.Clamp);
			}
			BlowPinLVDT(&RMold,&gMachineFix.MoldR.BlowPin,&gMachinePara.MoldR.BlowPin,&gMachinePara.MoldR.Clamp);
		}
		else if(SENSOR_SWITCH == gMachineFix.MoldR.BlowPin.eSensorType)
		{
			if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
			{
				BlowPinSwitch(&LMold,&gMachineFix.MoldL.BlowPin,&gMachinePara.MoldL.BlowPin,&gMachinePara.MoldL.Clamp);
			}
			BlowPinSwitch(&RMold,&gMachineFix.MoldR.BlowPin,&gMachinePara.MoldR.BlowPin,&gMachinePara.MoldR.Clamp);
		}
	}
	else if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		if(0 == gMachineFix.Option.bHideLeftStation)  // two mold 
		{
			BlowPinDriverDirect(&LMold,&gMachineFix.MoldL.BlowPin,&gMachinePara.MoldL.BlowPin,&gMachinePara.MoldL.Clamp);
		}
		BlowPinDriverDirect(&RMold,&gMachineFix.MoldR.BlowPin,&gMachinePara.MoldR.BlowPin,&gMachinePara.MoldR.Clamp);
	}
	
	
	if(1 == gMachineFix.Option.bHideLeftStation)
	{
		LMold.BlowPin.Error = 0;
		LMold.BlowPin.CalOK = 1;
		LMold.BlowPin.UpPos = 1;
		LMold.BlowPin.DnPos = 1;
	}
	
	gMachineOut.LMold.BlowPinUp = LMold.ValveOut.BlowPinUp;
	gMachineOut.RMold.BlowPinUp = RMold.ValveOut.BlowPinUp;
	
	gMachineOut.LMold.BlowPinDn = LMold.ValveOut.BlowPinDn;
	gMachineOut.RMold.BlowPinDn = RMold.ValveOut.BlowPinDn;
	
	gMachineOut.LMold.BlowPinUpMicro = LMold.ValveOut.BlowPinUpMicro;
	gMachineOut.RMold.BlowPinUpMicro = RMold.ValveOut.BlowPinUpMicro;
	
	gMachineOut.LMold.BlowPinDnHold = LMold.ValveOut.BlowPinDnHold;
	gMachineOut.RMold.BlowPinDnHold = RMold.ValveOut.BlowPinDnHold;
	
	
	if (1==gUserPara.bProductAuxClamp)
	{
		gMachineOut.LMold.ProductAuxClamp = LMold.ValveOut.ProductAuxClamp;
		gMachineOut.RMold.ProductAuxClamp = RMold.ValveOut.ProductAuxClamp;
	}
	else
	{
		gMachineOut.LMold.ProductAuxClamp = 0;
		gMachineOut.RMold.ProductAuxClamp = 0;	
	}
	
	gMachineInfo.RBlowPinPos = RMold.BlowPin.mmAs;/*Albert*/
	gMachineInfo.LBlowPinPos = LMold.BlowPin.mmAs;/*Albert*/
	
	
	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.BlowPin.eActuatorType = gMachineFix.MoldR.BlowPin.eActuatorType;
	
	/*-------------- ValveType----------------- */
	gMachineFix.MoldL.BlowPin.eValveType = gMachineFix.MoldR.BlowPin.eValveType;
	
	/*-------------- SensorType----------------- */
	gMachineFix.MoldL.BlowPin.eSensorType = gMachineFix.MoldR.BlowPin.eSensorType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.BlowPin.ePressueOutputType = gMachineFix.MoldR.BlowPin.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.BlowPin.eFluxOutputType = gMachineFix.MoldR.BlowPin.eFluxOutputType;
	
	/*------------ MechanismType--------------- */
	gMachineFix.MoldL.BlowPin.eMechanismType = gMachineFix.MoldR.BlowPin.eMechanismType;

	/*---------------Button Off Delay - Manual-------------*/
	ButtonOffDelay(&RMold ,&gMacTimer.RBlowPinBtnOffDly);
	ButtonOffDelay(&LMold ,&gMacTimer.LBlowPinBtnOffDly);
	
	
	/*---------------Motor Trand-------------*/
	gMotorTrace.RMold.BlowPin.Position 		= RMold.BlowPin.mmAs;
	gMotorTrace.LMold.BlowPin.Position 		= LMold.BlowPin.mmAs;

} /* end of _CYCLIC */

void BlowPinSwitch( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pBlowPinFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pBlowPinFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	
	/* No LVDT, no calibration false   */
	pMold->BlowPin.CalOK = 1;
	pMold->Alarm.BlowPinCalErr = 0;
	
	/* gAlarm time value    */
	if(0 == pMold->TimeSet.BlowPinUpAlarmTime)pMold->TimeSet.BlowPinUpAlarmTime = 500;
	if(0 == pMold->TimeSet.BlowPinDnAlarmTime)pMold->TimeSet.BlowPinDnAlarmTime = 300;
	
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/

	if(pBlowPinFix->Down.V_Max < 200.0)pBlowPinFix->Down.V_Max = 1000.0;
	if(pBlowPinFix->Up.V_Max < 200.0)pBlowPinFix->Up.V_Max = 1000.0;
    
	if(pBlowPinFix->Down.VoltageMax > -3.0)pBlowPinFix->Down.VoltageMax = -10.0;
	if(pBlowPinFix->Up.VoltageMax < 3.0 )pBlowPinFix->Up.VoltageMax = 10.0; 
   
	pMold->BlowPin.V_Up = pBlowPinPara->V_Up * pBlowPinFix->Up.V_Max/100.0;
	pMold->BlowPin.V_UpMicro = pBlowPinPara->V_UpMicro * pBlowPinFix->Up.V_Max/100.0;
	
	pMold->BlowPin.P_Up		= pBlowPinPara->P_Up ;
	pMold->BlowPin.P_UpMicro	= pBlowPinPara->P_UpMicro ;
	pMold->BlowPin.P_UpReset	= pBlowPinPara->P_UpReset ;
    
	pMold->BlowPin.V_Dn = pBlowPinPara->V_Dn * pBlowPinFix->Down.V_Max/100.0;
	
	pMold->BlowPin.P_Dn		= pBlowPinPara->P_Dn ;
	
	if(0 == pBlowPinPara->V_UpReset)pBlowPinPara->V_UpReset = 2.0;
	pMold->BlowPin.V_UpReset = pBlowPinPara->V_UpReset * pBlowPinFix->Up.V_Max/100.0;
   
	if(pBlowPinFix->Down.accel_stop < 100)pBlowPinFix->Down.accel_stop = INSTANCY_STOP_DECEL; 
	if(pBlowPinFix->Up.accel_stop < 100)pBlowPinFix->Up.accel_stop = INSTANCY_STOP_DECEL;
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	
	/* Up or down  position flag  */
	pMold->BlowPin.UpPos = pMold->TransDIn.BlowPinUpLimit; 
	pMold->BlowPin.DnPos = pMold->BlowPin.bDnPosFlag_Switch; 
	//	pMold->BlowPin.DnPos = pMold->TransDIn.BlowPinDnLimit; 
	
	/* ------------------------------------------------------ */
	/* ---------------- Step Machine Control ---------------- */
	/* ------------------------------------------------------ */
	switch ( pMold->BlowPin.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
			
			pMold->BlowPin.Timer.IN  = 0;
			pMold->BlowPin.LimitTimer.IN  = 0;
			
			break;

		/*------------------ BlowPin move down -------------------- */
		case 100: 	/* down delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowPinDnDelay > 0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnDelay;
					pMold->BlowPin.Step 	= 200;
					
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.BlowPinDnDelay = pMold->TimeSet.BlowPinDnDelay/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.BlowPinDnDelay = pMold->TimeSet.BlowPinDnDelay/100.0;
					}
				}
				else
				{
					pMold->BlowPin.Timer.IN = 0;
					pMold->BlowPin.Step = 300;
				}
			}
			else
			{
				pMold->BlowPin.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.BlowPinDnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//	pMold->TimeDis.BlowPinDnDelay = 0;
				pMold->BlowPin.Step = 300;
			}
			break;
		
		case 300:
			if(pMold->TimeSet.BlowPinDnTime != 0)
			{
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnTime;
			
				pMold->BlowPin.vTimeProf.Ts = tTask;
				pMold->BlowPin.vTimeProf.t_move = pMold->TimeSet.BlowPinDnTime * 0.01;
	            
				pMold->BlowPin.vTimeProf.v_set   = pMold->BlowPin.V_Dn; 
				pMold->BlowPin.vTimeProf.v_start = 0.0; 
				pMold->BlowPin.vTimeProf.v_end   = 0.0;
	            
				pMold->BlowPin.vTimeProf.accel = 2000;
				if(pMold->BlowPin.vTimeProf.accel <= 0.0)pMold->BlowPin.vTimeProf.accel = 1000;
	            
				pMold->BlowPin.vTimeProf.decel = 2000;
				if(pMold->BlowPin.vTimeProf.decel <= 0.0)pMold->BlowPin.vTimeProf.decel = 1000;
	            
				pMold->BlowPin.vTimeProf.init = 1;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowPinDnTime = pMold->TimeSet.BlowPinDnTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowPinDnTime = pMold->TimeSet.BlowPinDnTime/100.0;
				}
	            
				pMold->BlowPin.Step = 400;
			}
			else
			{
				pMold->BlowPin.Step = 500;
			}
			pMold->BlowPin.bDnPosFlag_Switch = 0;
			break;

		case 400:	/*  down  */
			pMold->ActInfo.BlowPinDn  = 1;
			pMold->ActInfo.BlowPinUp  = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 1;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
				pMold->BlowPin.v_set = pMold->BlowPin.V_UpReset;
				pMold->BlowPin.bDnPosFlag_Switch = 1;
			}
			else
			{
				pMold->BlowPin.p_set = pMold->BlowPin.P_Dn;
				pMold->BlowPin.v_set = pMold->BlowPin.V_Dn;
			}
			
			HydvTimeProf(&pMold->BlowPin.vTimeProf);	
			pMold->BlowPin.v_set = pMold->BlowPin.vTimeProf.v_out;

			pMold->TimeDis.BlowPinDnTime = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Timer.IN = 0;
				pMold->BlowPin.Step = 500;
			}
			break;
		
		case 500:
			
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
					
			
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;	
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN      = 0;
			
			pMold->BlowPin.Step = 3000;

			break;
		
		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:

			pMold->ActInfo.BlowPinDn  = 1;
			pMold->ActInfo.BlowPinUp  = 0;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 1;
	
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 1;  //parker special
		
			pMold->BlowPin.p_set = pBlowPinFix->pCalibMax;
			pMold->BlowPin.v_set = pBlowPinFix->vCalibMax*10; 
			
			pMold->BlowPin.bDnPosFlag_Switch = 0;
		
			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* up delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinUpDelay;
				pMold->BlowPin.Step = 10200;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowPinUpDelay = pMold->TimeSet.BlowPinUpDelay/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowPinUpDelay = pMold->TimeSet.BlowPinUpDelay/100.0;
				}
			}
			else
			{
				pMold->BlowPin.Step = 13000;
			}
			break;
		case 10200: 	/* delay ok */
			pMold->TimeDis.BlowPinUpDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//	pMold->TimeDis.BlowPinUpDelay = 0;
				pMold->BlowPin.Step = 10300;
			}
			break;
      
		case 10300:
			if(0 == pMold->TransDIn.BlowPinUpLimit)
			{
				pMold->BlowPin.LimitTimer.IN = 1;
				pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
			
				pMold->BlowPin.Step = 10400;
			}
			else
			{
				pMold->BlowPin.Step = 10500;
			}
			pMold->BlowPin.bDnPosFlag_Switch = 0;
			break;
        
		case 10400:
			pMold->ActInfo.BlowPinDn  = 0;
			pMold->ActInfo.BlowPinUp  = 1;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 1;
			pMold->ValveOut.BlowPinUpMicro = 0;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
				pMold->BlowPin.v_set = pMold->BlowPin.V_UpReset;
			}
			else
			{
				pMold->BlowPin.p_set = pMold->BlowPin.P_Up;
				pMold->BlowPin.v_set = pMold->BlowPin.V_Up;
			}
		
			if(1 == pMold->TransDIn.BlowPinUpLimit)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 10500;
			
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
			
			break;
      
		case 10500:
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
								
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;	
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN      = 0;
					
			pMold->BlowPin.Step = 13000;
			break;
      
		case 13000:
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:
			if(0 == pMold->TransDIn.BlowPinUpLimit)
			{
				pMold->ActInfo.BlowPinDn  = 0;
				pMold->ActInfo.BlowPinUp  = 1;

				//*--VALVE_DIR--*//
				/*-- Down --*/
				pMold->ValveOut.BlowPinDn = 0;
		
				/*-- Up --*/
				pMold->ValveOut.BlowPinUp = 1;
				pMold->ValveOut.BlowPinUpMicro = 0;
			
				pMold->BlowPin.p_set = pBlowPinFix->pCalibZero;
				pMold->BlowPin.v_set = pBlowPinFix->vCalibZero*10; 
		
			}
			else
			{
				pMold->ActInfo.BlowPinDn  = 0;
				pMold->ActInfo.BlowPinUp  = 0;

				//*--VALVE_DIR--*//
				/*-- Down --*/
				pMold->ValveOut.BlowPinDn = 0;
		
				/*-- Up --*/
				pMold->ValveOut.BlowPinUp = 0;
				pMold->ValveOut.BlowPinUpMicro = 0;
			
				pMold->BlowPin.p_set = 0;
				pMold->BlowPin.v_set = 0; 
			}
			pMold->BlowPin.bDnPosFlag_Switch = 0;
			break;

		/*--------------- BlowPin tiny return -----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowpintinyreturnDelay !=0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowpintinyreturnDelay;
					pMold->BlowPin.Step 	= 15200;
					
					//gExSPC
					if(pMold == & LMold)
					{
						gExSPC[ExIndex].LMold.BlowpintinyreturnDelay = pMold->TimeSet.BlowpintinyreturnDelay/100.0;
					}
					else
					{
						gExSPC[ExIndex].RMold.BlowpintinyreturnDelay = pMold->TimeSet.BlowpintinyreturnDelay/100.0;
					}
				}
				else
				{
					pMold->BlowPin.Step = 15300;
				}
			}
			else
			{
				pMold->BlowPin.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */
			pMold->TimeDis.BlowpintinyreturnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				pMold->TimeDis.BlowpintinyreturnDelay = 0;
				pMold->BlowPin.Step = 15300;
			}
			break;
      
		case 15300:
			if(pMold->TimeSet.BlowpintinyreturnTime != 0)
			{
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowpintinyreturnTime;
			
				pMold->BlowPin.vTimeProf.Ts = tTask;
				pMold->BlowPin.vTimeProf.t_move = pMold->TimeSet.BlowpintinyreturnTime * 0.01;
            
				pMold->BlowPin.vTimeProf.v_set   = pMold->BlowPin.V_UpMicro ; 
				pMold->BlowPin.vTimeProf.v_start = 0.0; 
				pMold->BlowPin.vTimeProf.v_end   = 0.0;
            
				pMold->BlowPin.vTimeProf.accel = pBlowPinFix->Up.accel;
				if(pMold->BlowPin.vTimeProf.accel <= 0.0)pMold->BlowPin.vTimeProf.accel = 1000;
            
				pMold->BlowPin.vTimeProf.decel = pBlowPinFix->Up.decel;
				if(pMold->BlowPin.vTimeProf.decel <= 0.0)pMold->BlowPin.vTimeProf.decel = 1000;
            
				pMold->BlowPin.vTimeProf.init = 1;
				pMold->BlowPin.Step = 15400;
			
				//gExSPC
				if(pMold == & LMold)
				{
					gExSPC[ExIndex].LMold.BlowpintinyreturnTime = pMold->TimeSet.BlowpintinyreturnTime/100.0;
				}
				else
				{
					gExSPC[ExIndex].RMold.BlowpintinyreturnTime = pMold->TimeSet.BlowpintinyreturnTime/100.0;
				}
			}
			else
			{
				pMold->BlowPin.Step = 15500;
			}
			break;
              
		case 15400:
			pMold->ActInfo.BlowPinUp  = 1;
			pMold->ActInfo.BlowPinDn  = 0;
					
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 1;
			pMold->ValveOut.BlowPinUpMicro = 1;

			pMold->BlowPin.p_set = pMold->BlowPin.P_UpMicro;
			
			HydvTimeProf(&pMold->BlowPin.vTimeProf);
            
			pMold->BlowPin.v_set = pMold->BlowPin.vTimeProf.v_set;
            
			pMold->TimeDis.BlowpintinyreturnTime = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Timer.IN = 0;
				//	pMold->TimeDis.BlowpintinyreturnTime = 0;
				pMold->BlowPin.Step = 15500;
			}
			break;
      
		case 15500:					
			pMold->ActInfo.BlowPinUp  = 0;
			pMold->ActInfo.BlowPinDn  = 0;
					
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 1;
			pMold->ValveOut.BlowPinUpMicro = 1;
			
			pMold->BlowPin.p_set = 0.0;
			pMold->BlowPin.v_set = 0.0;
			
			pMold->BlowPin.Timer.IN   = 0;
			
			pMold->BlowPin.Step = 18000;  
			break;
      

		case 18000:
			
			break;

		
		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
			pMold->ActInfo.BlowPinUp  = 0;
			pMold->ActInfo.BlowPinDn  = 0;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
		
			pMold->BlowPin.p_set = 0;
			
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
 				
			pMold->BlowPin.vRamp.Ts = tTask;
			if(pMold->BlowPin.v_set > 0)
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Up.accel_stop;
			}
			else
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Down.accel_stop;
			}
 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;

			pMold->BlowPin.Step = 21000;
			break;
        
		case 21000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.p_set =0;
				pMold->BlowPin.Step = 22000;
			}
			break;
    			
		case 22000:
			pMold->BlowPin.Step = 0;
			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
			pMold->ActInfo.BlowPinUp  = 0;
			pMold->ActInfo.BlowPinDn  = 0;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
		
			pMold->BlowPin.p_set = 0;
			
			
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 31000;
			break;
		
		case 31000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 32000;
			}
			break;
	    			
		case 32000:
			pMold->BlowPin.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
			pMold->ActInfo.BlowPinUp  = 0;
			pMold->ActInfo.BlowPinDn  = 0;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
			
			pMold->BlowPin.p_set = 0;
			pMold->BlowPin.v_set = 0;
			break;
		
		case 41000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
			pMold->ActInfo.BlowPinUp  = 0;
			pMold->ActInfo.BlowPinDn  = 0;

			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
		
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 1;
			pMold->ValveOut.BlowPinUpMicro = 1;
		
			pMold->BlowPin.p_set = 0;
			
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 42000;
			break;
	        
		case 42000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 43000;
			}
			break;
	    			
		case 43000:
			pMold->BlowPin.Step = 40000;
			break;
	}/* end of switch */
	
	if(pMold->BlowPin.Step > 10300 && pMold->BlowPin.Step < 10500)
	{
		pMold->TimeDis.BlowPinUpAlarmTime = pMold->BlowPin.LimitTimer.ET;
	}

	/* ----------------- blow pin up / down timeout --------------------  */
	if(1 == pMold->BlowPin.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BlowPin.Step < 1500 && pMold->BlowPin.Step > 200)		/* BlowPin Down  */
		{
			pMold->Alarm.BlowPinNotAtDnPos = 1;
		}
		
		if((pMold->BlowPin.Step < 13000 && pMold->BlowPin.Step > 10200)		/* BlowPin Up  */
			||(pMold->BlowPin.Step < 18000 && pMold->BlowPin.Step > 15300) )	/* BlowPin Up Micro  */
		{
			pMold->Alarm.BlowPinNotAtUpPos = 1;
		}
      
		pMold->BlowPin.LimitTimer.IN = 0;
		pMold->BlowPin.Step = 40000;
	}

	/* ----------------- blow pin down action limit --------------------  */
	if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000)  /*  blow pin down  */
	{
		
		if( 0 == pMold->Carriage.FwPos && 0 == pMold->Carriage.BwPos )
		{
			pMold->Alarm.CarriageNotAtFwBwPos = 1;
			pMold->BlowPin.Step = 40000;
		}
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if(1 == gMachineInfo.Auto && 0 == gMachineInfo.MachineReset)	
		{
			if( 1 == pMold->Option.HookPin)
			{
				if( 0 == pMold->TransDIn.HookPinFwLimit)/*  Hook Pin not at backward limit   */
				{
					pMold->Alarm.HookPinNotAtFwPos = !pMold->TransDIn.HookPinFwLimit;
					pMold->BlowPin.Step = 41000;
				}
			}
		}
		
		//		if(1 == gMachineInfo.Auto)	
		//		{
		//			if(0 == pMold->Carriage.BwPos)
		//			{
		//				pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos
		//		}
	}/* if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000) */
}

void BlowPinLVDT( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pBlowPinFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pBlowPinFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	if(0 == pMold->TimeSet.BlowPinUpAlarmTime)pMold->TimeSet.BlowPinUpAlarmTime = 500;
	if(0 == pMold->TimeSet.BlowPinDnAlarmTime)pMold->TimeSet.BlowPinDnAlarmTime = 300;
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pBlowPinFix->Length < 100.0)pBlowPinFix->Length = 100.0;
	if(pBlowPinFix->Tolerance < 0.1)pBlowPinFix->Tolerance = 0.1;
	
	if(bBlowPinPropValveOutputInverse == 0)
	{
		if(pBlowPinFix->Up.VoltageMax > -3.0)pBlowPinFix->Up.VoltageMax = -10.0;
		if(pBlowPinFix->Up.VoltageMin < -7.0 || pBlowPinFix->Up.VoltageMin > 0)pBlowPinFix->Up.VoltageMin = 0;
		
		if(pBlowPinFix->Down.VoltageMax  < 3.0 )pBlowPinFix->Down.VoltageMax  = 10.0;
		if(pBlowPinFix->Down.VoltageMin  > 7.0 || pBlowPinFix->Down.VoltageMin < 0 )pBlowPinFix->Down.VoltageMin  = 0;
	}
	else
	{
		if(pBlowPinFix->Up.VoltageMax  < 3.0 )pBlowPinFix->Up.VoltageMax  = 10.0;
		if(pBlowPinFix->Up.VoltageMin  > 7.0 || pBlowPinFix->Up.VoltageMin < 0 )pBlowPinFix->Up.VoltageMin  = 0;
		
		if(pBlowPinFix->Down.VoltageMax > -3.0  )pBlowPinFix->Down.VoltageMax = -10.0;
		if(pBlowPinFix->Down.VoltageMin < -7.0 || pBlowPinFix->Down.VoltageMin > 0 )pBlowPinFix->Down.VoltageMin = 0;
	}
	
	pMold->BlowPin.Tolerance = pBlowPinFix->Tolerance;
    
	if(pBlowPinFix->Down.V_Max < 200.0)pBlowPinFix->Down.V_Max = 1000.0;
	if(pBlowPinFix->Up.V_Max < 200.0)pBlowPinFix->Up.V_Max = 1000.0;
    
	if(pBlowPinFix->Down.VoltageMax > -3.0)pBlowPinFix->Down.VoltageMax = -10.0;
	if(pBlowPinFix->Up.VoltageMax < 3.0 )pBlowPinFix->Up.VoltageMax = 10.0;
    
	pMold->BlowPin.S_Up = pBlowPinPara->S_Up;
	pMold->BlowPin.S_UpMicro = pBlowPinPara->S_UpMicro;
   
	pMold->BlowPin.V_Up = pBlowPinPara->V_Up * pBlowPinFix->Up.V_Max/100.0;
	pMold->BlowPin.V_UpMicro = pBlowPinPara->V_UpMicro * pBlowPinFix->Up.V_Max/100.0;
	
	pMold->BlowPin.P_Up		= pBlowPinPara->P_Up ;
	pMold->BlowPin.P_UpMicro	= pBlowPinPara->P_UpMicro ;
	pMold->BlowPin.P_UpReset	= pBlowPinPara->P_UpReset ;
	
	pMold->BlowPin.S_Dn = pBlowPinPara->S_Dn;
	pMold->BlowPin.S_DnSlow = pBlowPinPara->S_DnSlow ;
    
	pMold->BlowPin.V_Dn = pBlowPinPara->V_Dn * pBlowPinFix->Down.V_Max/100.0;
	pMold->BlowPin.V_DnSlow = pBlowPinPara->V_DnSlow * pBlowPinFix->Down.V_Max/100.0;
	
	pMold->BlowPin.P_Dn		= pBlowPinPara->P_Dn ;
	pMold->BlowPin.P_DnSlow	= pBlowPinPara->P_DnSlow ;
	
	if(0 == pBlowPinPara->V_UpReset)pBlowPinPara->V_UpReset = 2.0;
	pMold->BlowPin.V_UpReset = pBlowPinPara->V_UpReset * pBlowPinFix->Up.V_Max/100.0;
   
	if(pBlowPinFix->Down.accel_stop < 100)pBlowPinFix->Down.accel_stop = INSTANCY_STOP_DECEL; 
	if(pBlowPinFix->Up.accel_stop < 100)pBlowPinFix->Up.accel_stop = INSTANCY_STOP_DECEL;

	
	/*********************************************************************************
  	**                            linear calibration status                        **
  	*********************************************************************************/
  	/* get calibration data  */
	if(pBlowPinFix->Max < pBlowPinFix->Zero)
	{
		pMold->BlowPin.ZeroPos = pBlowPinFix->Max;
		pMold->BlowPin.MaxPos  = pBlowPinFix->Zero;
	}
	else
	{
		pMold->BlowPin.MaxPos  = pBlowPinFix->Max;
		pMold->BlowPin.ZeroPos = pBlowPinFix->Zero;
	}

	if(pMold->BlowPin.ZeroPos != 0                            &&
		pMold->BlowPin.MaxPos > (pMold->BlowPin.ZeroPos + 3000) &&
		pMold->BlowPin.As     > (pMold->BlowPin.ZeroPos - 500 ) &&
		pMold->BlowPin.As     < (pMold->BlowPin.MaxPos  + 500 ))
	{
		pMold->BlowPin.CalOK = 1;
	}
	else
	{
		pMold->BlowPin.CalOK = 0;
	}
	
	pBlowPinFix->CalibOK = pMold->BlowPin.CalOK;

	if(1 == pMold->BlowPin.CalOK)
	{
		if(pBlowPinFix->Max < pBlowPinFix->Zero)
		{
			pMold->BlowPin.mmAs = 1.0 *(pMold->BlowPin.MaxPos - pMold->BlowPin.As) * pBlowPinFix->Length /
				(pMold->BlowPin.MaxPos - pMold->BlowPin.ZeroPos);
		}
		else
		{
			pMold->BlowPin.mmAs = 1.0 *(pMold->BlowPin.As - pMold->BlowPin.ZeroPos) * pBlowPinFix->Length /
				(pMold->BlowPin.MaxPos - pMold->BlowPin.ZeroPos);
		}
	}
	else
	{
		pMold->BlowPin.mmAs = pMold->BlowPin.mmAs;
		if(0 == gMachineInfo.Calib)	/* 非调校模式  */
		{
			if(pMold->BlowPin.Step > 0 && pMold->BlowPin.Step < 20000)
			{
				pMold->BlowPin.Step = 0;
				pMold->Alarm.BlowPinCalErr = 1;
			}
		}
	}/* if(1 == pMold->BlowPin.CalOK) */
	
	/*  relative position  */
	pMold->BlowPin.Ss = pMold->BlowPin.As;
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	if(1 == pMold->BlowPin.CalOK)
	{
		pMold->BlowPin.UpPos = pMold->TransDIn.BlowPinUpLimit & (pMold->BlowPin.mmAs < (pBlowPinPara->S_Up + pBlowPinFix->Tolerance)); 
		pMold->BlowPin.DnPos = pMold->TransDIn.BlowPinDnLimit & (pMold->BlowPin.mmAs > (pBlowPinPara->S_Dn - pBlowPinFix->Tolerance)); 
	}
	else
	{
		pMold->BlowPin.UpPos = 0;
		pMold->BlowPin.DnPos = 0;
	}
	 
	/* ------------------ function current position ------------------ */
	pMold->BlowPin.vProfGen.s_act = pMold->BlowPin.mmAs;
	pMold->BlowPin.vProfGen.v_act = pMold->BlowPin.v_act;

	switch ( pMold->BlowPin.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/
			
			pMold->BlowPin.Timer.IN  = 0;
			pMold->BlowPin.LimitTimer.IN  = 0;
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
            
			break;
		
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;

		/*------------------ BlowPin move down -------------------- */
		case 100: 	/* down delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowPinDnDelay > 0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnDelay;
					pMold->BlowPin.Step 	= 200;
				}
				else
				{
					pMold->BlowPin.Step = 300;
				}
			}
			else
			{
				pMold->BlowPin.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.BlowPinDnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//				pMold->TimeDis.BlowPinDnDelay = 0;
				pMold->BlowPin.Step = 300;
			}
			break;

		case 300:

			pMold->BlowPin.LimitTimer.IN = 1;
			pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinDnAlarmTime;
				
			pMold->BlowPin.vProfGen.Ts = tTask;
			pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Down.Tj;
			pMold->BlowPin.vProfGen.Td = pBlowPinFix->Down.Td;
				
			pMold->BlowPin.vProfGen.accel = pBlowPinFix->Down.accel;
			if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
				
			pMold->BlowPin.vProfGen.decel = pBlowPinFix->Down.accel;
			if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
				            
			pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Down.decel_end;
			if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
				            
			pMold->BlowPin.vProfGen.v_start = 0.0;
			pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_Dn;
			pMold->BlowPin.vProfGen.v_end 	= 0;
			pMold->BlowPin.vProfGen.s_end 	= pMold->BlowPin.S_Dn;
			pMold->BlowPin.vProfGen.init  	= 1;
					
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin.Step = 500;
			}
			else
			{
				pMold->BlowPin.Step = 400;
			}
			

			break;

		case 400:		/*  quick down  */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			switch (pBlowPinFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Down --*/
					pMold->ValveOut.BlowPinDn = 1;
			
					/*-- Up --*/
					pMold->ValveOut.BlowPinUp = 0;
					pMold->ValveOut.BlowPinUpMicro = 0;
				
					if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
						pMold->BlowPin.v_set = pMold->BlowPin.V_UpReset; 
					}
					else
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_Dn;
						pMold->BlowPin.v_set = pMold->BlowPin.V_Dn; 
					} 

					break;
				
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Down --*/

			
					/*-- Up --*/
				

					pMold->BlowPin.p_set = pMold->BlowPin.P_Dn;
					pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_Dn;
			
						
					/* call profile function */
					HydvProfGen(& pMold->BlowPin.vProfGen);
            
					pMold->BlowPin.p_set = pMold->BlowPin.P_Dn;
					pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
			
					pMold->BlowPin.actTjDn = pMold->BlowPin.vProfGen.Tj;
				
					break;
			}
			
			/*  reached target position */
			if(pMold->BlowPin.mmAs > pMold->BlowPin.S_DnSlow)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				
				pMold->BlowPin.Step = 500;
			}
			
			break;
		
		case 500:		/*  slow down  */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			switch (pBlowPinFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Down --*/
					pMold->ValveOut.BlowPinDn = 1;
					
					/*-- Up --*/
					pMold->ValveOut.BlowPinUp = 0;
					pMold->ValveOut.BlowPinUpMicro = 0;
						
					if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
						pMold->BlowPin.v_set = pMold->BlowPin.V_UpReset;
					}
					else
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_DnSlow;
						pMold->BlowPin.v_set = pMold->BlowPin.V_DnSlow;
					}
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Down --*/
		
					
					/*-- Up --*/
						
					if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
						pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset; 
					}
					else
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_DnSlow;
						pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_DnSlow; 
					}
								
					/* call profile function */
					HydvProfGen(& pMold->BlowPin.vProfGen);
		            
					pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
					
					pMold->BlowPin.actTjDn = pMold->BlowPin.vProfGen.Tj;
					
					break;
			}
			
			/*  reached target position */
			if(pMold->BlowPin.mmAs >= pMold->BlowPin.S_Dn - pBlowPinFix->Tolerance)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
//				pMold->BlowPin.LimitTimer.IN = 0;
				pMold->BlowPin.Step = 700;
				if(1 == gMachineInfo.Auto && 0 == gMachineInfo.MachineReset )
				{
					gMachineInfo.OilAccuChargeAuto_Extrlift = 1;  // for  Extrlift accu charge in auto mode
				}
			}
			
			break;

		case 700:
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/		
			
			
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;	
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
			pMold->BlowPin.Step = 3000;
			break;

		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:
		
			pMold->ActInfo.BlowPinDn  = 1;
			pMold->ActInfo.BlowPinUp  = 0;

			switch (pBlowPinFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Down --*/
					pMold->ValveOut.BlowPinDn = 1;
			
					/*-- Up --*/
					pMold->ValveOut.BlowPinUp = 0;
					pMold->ValveOut.BlowPinUpMicro = 1; //parker special
				
					pMold->BlowPin.p_set = pBlowPinFix->pCalibMax;
					pMold->BlowPin.v_set = pBlowPinFix->vCalibMax*10; 

					break;
			
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Down --*/

			
					/*-- Up --*/

				
					pMold->BlowPin.p_set = pBlowPinFix->pCalibMax;
					pMold->BlowPin.v_set = -pBlowPinFix->vCalibMax; 
			
					break;
			}
		

		
			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* open delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinUpDelay;
				pMold->BlowPin.Step = 10200;
			}
			else
			{
				pMold->BlowPin.Step = 13000;
			}
			break;
		case 10200: 	/* delay ok */
			pMold->TimeDis.BlowPinUpDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
//				pMold->TimeDis.BlowPinUpDelay = 0;
				pMold->BlowPin.Step = 10300;
			}
			break;
      
		case 10300:

			pMold->BlowPin.LimitTimer.IN = 1;
			pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
				
			pMold->BlowPin.vProfGen.Ts = tTask;
			pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Up.Tj;
			pMold->BlowPin.vProfGen.Td = pBlowPinFix->Up.Td;
				
			pMold->BlowPin.vProfGen.accel = pBlowPinFix->Up.accel;
			if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
				
			pMold->BlowPin.vProfGen.decel = pBlowPinFix->Up.accel;
			if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
				            
			pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
			if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
				            
			pMold->BlowPin.vProfGen.v_start = 0.0;
			pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_Up;
			pMold->BlowPin.vProfGen.v_end 	= 0.0;
			pMold->BlowPin.vProfGen.s_end 	= pMold->BlowPin.S_Up;
			pMold->BlowPin.vProfGen.init  	= 1;
					
			pMold->BlowPin.Step = 10400;

			break;
        
		case 10400:
			pMold->ActInfo.BlowPinUp  = 1;
			pMold->ActInfo.BlowPinDn  = 0;
			
			switch (pBlowPinFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Down --*/
					pMold->ValveOut.BlowPinDn = 0;
					
					/*-- Up --*/
					pMold->ValveOut.BlowPinUp = 1;
					pMold->ValveOut.BlowPinUpMicro = 0;
				
					if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
						pMold->BlowPin.v_set = pMold->BlowPin.V_UpReset; 
					}
					else
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_Up;
						pMold->BlowPin.v_set = pMold->BlowPin.V_Up; 
					} 

					break;
				
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Down --*/
		
					
					/*-- Up --*/
						
					if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_UpReset;
						pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset; 
					}
					else
					{
						pMold->BlowPin.p_set = pMold->BlowPin.P_Up;
						pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_Up;
					} 
								
					/* call profile function */
					HydvProfGen(& pMold->BlowPin.vProfGen);
		            
					pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
					
					pMold->BlowPin.actTjUp = pMold->BlowPin.vProfGen.Tj;
					
					break;
			}
			
			/*  reached target position */
			if(pMold->BlowPin.mmAs <= pMold->BlowPin.S_Up + pBlowPinFix->Tolerance)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 10500;
			}
			
			break;
      
		case 10500:
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/		
			
			
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;	
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->BlowPin.Step = 13000;
			break;
      
		case 13000:
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:

			if(0 == pMold->TransDIn.BlowPinUpLimit)
			{
				pMold->ActInfo.BlowPinDn  = 0;
				pMold->ActInfo.BlowPinUp  = 1;

				switch (pBlowPinFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/
						/*-- Down --*/
						pMold->ValveOut.BlowPinDn = 0;
				
						/*-- Up --*/
						pMold->ValveOut.BlowPinUp = 1;
						pMold->ValveOut.BlowPinUpMicro = 0;
					
						pMold->BlowPin.p_set = pBlowPinFix->pCalibZero;
						pMold->BlowPin.v_set = pBlowPinFix->vCalibZero*10; 

						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						/*-- Down --*/

				
						/*-- Up --*/

					
						pMold->BlowPin.p_set = pBlowPinFix->pCalibZero;
						pMold->BlowPin.v_set = pBlowPinFix->vCalibZero; 
				
						break;
				}
		
			}
			else
			{
				pMold->ActInfo.BlowPinDn  = 0;
				pMold->ActInfo.BlowPinUp  = 0;

				switch (pBlowPinFix->eValveType )
				{
					case VALVE_DIR: /*0:Direct*/  /*no profile*/
						/*-- Down --*/
						pMold->ValveOut.BlowPinDn = 0;
				
						/*-- Up --*/
						pMold->ValveOut.BlowPinUp = 0;
						pMold->ValveOut.BlowPinUpMicro = 0;
					
						pMold->BlowPin.p_set = 0;
						pMold->BlowPin.v_set = 0; 

						break;
				
					case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
						/*-- Down --*/

				
						/*-- Up --*/

					
						pMold->BlowPin.p_set = 0;
						pMold->BlowPin.v_set = 0; 
				
						break;
				}
			}

			break;

		/*--------------- BlowPin tiny return -----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowpintinyreturnDelay > 0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowpintinyreturnDelay;
					pMold->BlowPin.Step 	= 15200;
				}
				else
				{
					if(0 == gMacOption.bDryCycle)
					{
						pMold->BlowPin.Step = 15300;
					}
					else
					{
						pMold->BlowPin.Step = 18000;
					}
				}
			}
			else
			{
				pMold->BlowPin.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */
			pMold->TimeDis.BlowpintinyreturnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//				pMold->TimeDis.BlowpintinyreturnDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin.Step = 15300;
				}
				else
				{
					pMold->BlowPin.Step = 18000;
				}
			}
			break;
      
		case 15300:

			pMold->BlowPin.LimitTimer.IN = 1;
			pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
					
			pMold->BlowPin.vProfGen.Ts = tTask;
			//				pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Up.Tj;
			//				pMold->BlowPin.vProfGen.Td = pBlowPinFix->Up.Td;
			//						
			//				pMold->BlowPin.vProfGen.accel = pBlowPinFix->Up.accel;
			//				if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
			//						
			//				pMold->BlowPin.vProfGen.decel = pBlowPinFix->Up.accel;
			//				if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
			//						            
			//				pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
			//				if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
				
			pMold->BlowPin.vProfGen.Tj = 0;
			pMold->BlowPin.vProfGen.Td = 0;
				
			pMold->BlowPin.vProfGen.accel = 1000;
			pMold->BlowPin.vProfGen.decel = 1000;
			pMold->BlowPin.vProfGen.decel_end = 100;
					            
			pMold->BlowPin.vProfGen.v_start = 0.0;
			pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_UpMicro ;
			pMold->BlowPin.vProfGen.v_end 	= 0.0;
			pMold->BlowPin.vProfGen.s_end 	= pMold->BlowPin.vProfGen.s_act - pMold->BlowPin.S_UpMicro;
			pMold->BlowPin.vProfGen.init 	= 1;
						
			pMold->BlowPin.Step = 15400;

			break;
              
		case 15400:
			pMold->ActInfo.BlowPinUp  = 1;
			pMold->ActInfo.BlowPinDn  = 0;
					
			switch (pBlowPinFix->eValveType )
			{
				case VALVE_DIR: /*0:Direct*/  /*no profile*/
					/*-- Down --*/
					pMold->ValveOut.BlowPinDn = 0;
					
					/*-- Up --*/
					pMold->ValveOut.BlowPinUp = 1;
					pMold->ValveOut.BlowPinUpMicro = 1;
						
					pMold->BlowPin.p_set = pMold->BlowPin.P_UpMicro;
					pMold->BlowPin.v_set = pMold->BlowPin.V_UpMicro; 
	
					break;
					
				case VALVE_PROP: /*1:Proportion valve*/ /*profile*/
					/*-- Down --*/
		
					
					/*-- Up --*/
						
								
					pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpMicro;
					/* call profile function */
					HydvProfGen(& pMold->BlowPin.vProfGen);
		            
					pMold->BlowPin.p_set = pMold->BlowPin.P_UpMicro;
					pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
					
					pMold->BlowPin.actTjUp = pMold->BlowPin.vProfGen.Tj;
					
					break;
			}
		
			/*  reached target position */
			if(pMold->BlowPin.mmAs <= pMold->BlowPin.vProfGen.s_end + pBlowPinFix->Tolerance)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
//				pMold->BlowPin.LimitTimer.IN = 0;
				pMold->BlowPin.Step = 15500;
			}

			break;
      
		case 15500:
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/		
			
			
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;	
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			
            
			pMold->BlowPin.Step = 18000;
			break;
      
		case 18000:

			break;
	

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;

			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/
			
			pMold->BlowPin.p_set = 0;
            
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
 				
			pMold->BlowPin.vRamp.Ts = tTask;
			if(pMold->BlowPin.v_set > 0)
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Up.accel_stop;
			}
			else
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Down.accel_stop;
			}
 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;

			pMold->BlowPin.Step = 21000;
			break;
        
		case 21000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 22000;
			}
			break;
    			
		case 22000:
			pMold->BlowPin.Step = 0;
			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
	
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/
			
			pMold->BlowPin.p_set = 0;
            
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 31000;
			break;
		
		case 31000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 32000;
			}
			break;
	    			
		case 32000:
			pMold->BlowPin.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;

			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/
			
			
            
			pMold->BlowPin.p_set = 0;
			pMold->BlowPin.v_set = 0;
			break;
		
		case 41000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
	
			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
	
			
			//*--VALVE_DIR--*//
			/*-- Down --*/
			pMold->ValveOut.BlowPinDn = 0;
			
			/*-- Up --*/
			pMold->ValveOut.BlowPinUp = 0;
			pMold->ValveOut.BlowPinUpMicro = 0;
				
			
			//*--VALVE_PROP--*//
			/*-- Down --*/

			
			/*-- Up --*/
			
			
			pMold->BlowPin.p_set = 0;
			
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 42000;
			break;
	        
		case 42000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 43000;
			}
			break;
	    			
		case 43000:
			pMold->BlowPin.Step = 40000;
			break;
	}/* end of switch */
	
	if(pMold->BlowPin.Step > 300 && pMold->BlowPin.Step < 700)
	{
		pMold->TimeDis.BlowPinDnAlarmTime = pMold->BlowPin.LimitTimer.ET;
	}
	else if((pMold->BlowPin.Step > 10300 && pMold->BlowPin.Step < 10500)
		||(pMold->BlowPin.Step > 15300 && pMold->BlowPin.Step < 15500)
		)
	{
		pMold->TimeDis.BlowPinUpAlarmTime = pMold->BlowPin.LimitTimer.ET;
	}
	
	/* ----------------- blow pin up / down timeout --------------------  */
	if(1 == pMold->BlowPin.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BlowPin.Step < 1500 && pMold->BlowPin.Step > 200)		/* BlowPin Down  */
		{
			pMold->Alarm.BlowPinNotAtDnPos = 1;
		}
		
		if((pMold->BlowPin.Step < 13000 && pMold->BlowPin.Step > 10200)		/* BlowPin Up  */
			||(pMold->BlowPin.Step < 18000 && pMold->BlowPin.Step > 15300) )	/* BlowPin Up Micro  */
		{
			pMold->Alarm.BlowPinNotAtUpPos = 1;
		}
      
		pMold->BlowPin.LimitTimer.IN = 0;
		pMold->BlowPin.Step = 40000;
	}

	/* ----------------- blow pin down action limit --------------------  */
	if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000)  /*  blow pin down  */
	{
		
		if( 0 == pMold->Carriage.FwPos && 0 == pMold->Carriage.BwPos )
		{
			pMold->Alarm.CarriageNotAtFwBwPos = 1;
			pMold->BlowPin.Step = 40000;
		}
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if(1 == gMachineInfo.Auto && 0 == gMachineInfo.MachineReset)	
		{
			if( 1 == pMold->Option.HookPin)
			{
				if( 0 == pMold->TransDIn.HookPinFwLimit)/*  Hook Pin not at backward limit   */
				{
					pMold->Alarm.HookPinNotAtFwPos = !pMold->TransDIn.HookPinFwLimit;
					pMold->BlowPin.Step = 41000;
				}
			}
		}
		
		//		if(1 == gMachineInfo.Auto)	
		//		{
		//			if(0 == pMold->Carriage.BwPos)
		//			{
		//				pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos
		//		}
	}/* if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000) */
    
	/* ------------------------------------------------------------------------------ */
	/* ---------------- blow pin output and proportional valve linear --------------- */
	/* ------------------------------------------------------------------------------ */
	
	if (pBlowPinFix->eValveType == VALVE_PROP)
	{
		if(bBlowPinPropValveOutputInverse == 0 )
		{
			if((pBlowPinFix->Down.VoltageMax - pBlowPinFix->Down.VoltageMin > 5.0) &&
				(pBlowPinFix->Up.VoltageMin - pBlowPinFix->Up.VoltageMax > 5.0))
			{
				pMold->BlowPin.PropValveOk   = 1;
				pMold->Alarm.BlowPinValveErr = 0;
			}
			else
			{
				pMold->BlowPin.PropValveOk   = 0;
				pMold->Alarm.BlowPinValveErr = 1;
			}
		}
		else
		{
			if((pBlowPinFix->Down.VoltageMin - pBlowPinFix->Down.VoltageMax > 5.0) &&
				(pBlowPinFix->Up.VoltageMax - pBlowPinFix->Up.VoltageMin > 5.0))
			{
				pMold->BlowPin.PropValveOk   = 1;
				pMold->Alarm.BlowPinValveErr = 0;
			}
			else
			{
				pMold->BlowPin.PropValveOk   = 0;
				pMold->Alarm.BlowPinValveErr = 1;
			}
		}
		
		pBlowPinFix->Up.V_Max = 1000;
		pBlowPinFix->Down.V_Max  = 1000;
   
		if(pMold->BlowPin.v_set > pBlowPinFix->Down.V_Max) pMold->BlowPin.v_set = pBlowPinFix->Down.V_Max;
		if(pMold->BlowPin.v_set < -pBlowPinFix->Up.V_Max) pMold->BlowPin.v_set = -pBlowPinFix->Up.V_Max;
		
		if(5000 == pMold->BlowPin.Step || 15000 == pMold->BlowPin.Step || 8500 == pMold->BlowPin.Step)
		{
			if(bBlowPinPropValveOutputInverse == 0)
			{
				pMold->BlowPin.aoPropValve = pMold->BlowPin.v_set * 32767/100;   /* 电压输出  0 -- 100 ---> 0.0 -- 10.0v*/
			}
			else
			{
				pMold->BlowPin.aoPropValve = -(pMold->BlowPin.v_set * 32767/100);   /* 电压输出  0 -- 100 ---> 0.0 -- 10.0v*/
			}
			
		}
		else
		{
			if(pMold->BlowPin.v_set > 0 && pMold->BlowPin.v_set <= pBlowPinFix->Down.V_Max)
			{
				if(bBlowPinPropValveOutputInverse == 0)
				{
					//Down +V
					pMold->BlowPin.aoPropValve = 3276 * (pBlowPinFix->Down.VoltageMin + pMold->BlowPin.v_set/pBlowPinFix->Down.V_Max *
						(pBlowPinFix->Down.VoltageMax - pBlowPinFix->Down.VoltageMin));  
				}
				else
				{
					//Down -V
					pMold->BlowPin.aoPropValve = 3276 * (pBlowPinFix->Down.VoltageMin + (-pMold->BlowPin.v_set/pBlowPinFix->Down.V_Max) *
						(pBlowPinFix->Down.VoltageMin - pBlowPinFix->Down.VoltageMax));	   
				}
			}
			else if(pMold->BlowPin.v_set < 0 && pMold->BlowPin.v_set >= -pBlowPinFix->Up.V_Max)
			{
				if(bBlowPinPropValveOutputInverse == 0)
				{
					//Up -V
					pMold->BlowPin.aoPropValve = 3276 * (pBlowPinFix->Up.VoltageMin + pMold->BlowPin.v_set/pBlowPinFix->Up.V_Max *
						(pBlowPinFix->Up.VoltageMin - pBlowPinFix->Up.VoltageMax));
				}
				else
				{
					//Up +V
					pMold->BlowPin.aoPropValve = 3276 * (pBlowPinFix->Up.VoltageMin + (-pMold->BlowPin.v_set/pBlowPinFix->Down.V_Max) *
						(pBlowPinFix->Up.VoltageMax - pBlowPinFix->Up.VoltageMin));	   
				}
			}
			else
			{
				pMold->BlowPin.aoPropValve = 0;
			}	
		}
	}
	else
	{
		pMold->Alarm.BlowPinValveErr = 0;
	}
    
}

void BlowPinDriverDirect( Mold_typ * pMold,Blowpin_Fix_typ * pBlowPinFix,Blowpin_Para_typ * pBlowPinPara,Clamp_Para_typ * pClampPara)
{
	/*  pMax set  */
	if(pMold == & LMold)
	{
		pBlowPinFix->pMax = 100;
	}
	else
	{
		pBlowPinFix->pMax = 100;
	}
	
	if(1 == gMachineOut.OilPump)
	{
		if(tErrorDelay >= tTask)tErrorDelay = tErrorDelay - tTask;
		
		if(tErrorDelay < tTask)
		{
			if(pMold == & LMold)
			{
				LMold.HwAlarm.BlowPinNetworkErr = !LMold.BlowPin.NetworkInit;
				LMold.HwAlarm.BlowPinPowerOn    = !LMold.BlowPin.PowerOn;			
			}
			else
			{
				RMold.HwAlarm.BlowPinNetworkErr = !RMold.BlowPin.NetworkInit;
				RMold.HwAlarm.BlowPinPowerOn    = !RMold.BlowPin.PowerOn;	
			}			
		}
		
		if((RMold.HwAlarm.BlowPinNetworkErr ||RMold.HwAlarm.BlowPinPowerOn)||(LMold.HwAlarm.BlowPinNetworkErr ||LMold.HwAlarm.BlowPinPowerOn))
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
			//			LMold.HwAlarm.BlowPinNetworkErr = 0;
			//			LMold.HwAlarm.BlowPinPowerOn    = 0;			
		}
		else
		{
			//			RMold.HwAlarm.BlowPinNetworkErr = 0;
			//			RMold.HwAlarm.BlowPinPowerOn   = 0;
		}	
	}
	
	
	if(0 == pMold->TimeSet.BlowPinUpAlarmTime)pMold->TimeSet.BlowPinUpAlarmTime = 500;
	if(0 == pMold->TimeSet.BlowPinDnAlarmTime)pMold->TimeSet.BlowPinDnAlarmTime = 300;
	
	/* ------------- Blowpin axis alarms ------------- */
	pMold->HwAlarm.BlowPinAxisErr = pMold->BlowPin.Error;
	
	
	/*********************************************************************************
  	**                            HMI data value to PLC                             **
  	*********************************************************************************/
	if(pBlowPinFix->Length < 100.0)pBlowPinFix->Length = 100.0;
	if(pBlowPinFix->Tolerance < 0.1)pBlowPinFix->Tolerance = 0.1;
	pMold->BlowPin.Tolerance = pBlowPinFix->Tolerance;
    
	if(pBlowPinFix->Down.V_Max < 200.0)pBlowPinFix->Down.V_Max = 1000.0;
	if(pBlowPinFix->Up.V_Max < 200.0)pBlowPinFix->Up.V_Max = 1000.0;
    
	if(pBlowPinFix->Down.VoltageMax > -3.0)pBlowPinFix->Down.VoltageMax = -10.0;
	if(pBlowPinFix->Up.VoltageMax < 3.0 )pBlowPinFix->Up.VoltageMax = 10.0;
    
	pMold->BlowPin.S_Up = pBlowPinPara->S_Up;
	pMold->BlowPin.S_UpMicro = pBlowPinPara->S_UpMicro;

   
	pMold->BlowPin.V_Up = pBlowPinPara->V_Up * pBlowPinFix->Up.V_Max/100.0;
	pMold->BlowPin.V_UpMicro = pBlowPinPara->V_UpMicro * pBlowPinFix->Up.V_Max/100.0;
	
	pMold->BlowPin.P_Up		= pBlowPinPara->P_Up = 100;
	if(pBlowPinPara->P_UpMicro<=10) pBlowPinPara->P_UpMicro = 10;
	pMold->BlowPin.P_UpMicro	= pBlowPinPara->P_UpMicro;
	
	pMold->BlowPin.S_Dn = pBlowPinPara->S_Dn;
	pMold->BlowPin.S_DnSlow = pBlowPinPara->S_DnSlow ;
    
	pMold->BlowPin.V_Dn = pBlowPinPara->V_Dn * pBlowPinFix->Down.V_Max/100.0;
	pMold->BlowPin.V_DnSlow = pBlowPinPara->V_DnSlow * pBlowPinFix->Down.V_Max/100.0;
	
	pMold->BlowPin.P_Dn		= pBlowPinPara->P_Dn = 100;
	pMold->BlowPin.P_DnSlow		= pBlowPinPara->P_DnSlow ;
	
	if(0 == pBlowPinPara->V_UpReset)pBlowPinPara->V_UpReset = 2.0;
	pMold->BlowPin.V_UpReset = pBlowPinPara->V_UpReset * pBlowPinFix->Up.V_Max/100.0;
   
	if(pBlowPinFix->Down.accel_stop < 100)pBlowPinFix->Down.accel_stop = INSTANCY_STOP_DECEL; 
	if(pBlowPinFix->Up.accel_stop < 100)pBlowPinFix->Up.accel_stop = INSTANCY_STOP_DECEL;

	/*********************************************************************************
  	**                            linear calibration status                        **
  	*********************************************************************************/
  	/* get calibration data  */
	if(pBlowPinFix->Max < pBlowPinFix->Zero)
	{
		pMold->BlowPin.ZeroPos = pBlowPinFix->Max;
		pMold->BlowPin.MaxPos  = pBlowPinFix->Zero;
	}
	else
	{
		pMold->BlowPin.MaxPos  = pBlowPinFix->Max;
		pMold->BlowPin.ZeroPos = pBlowPinFix->Zero;
	}

	if(pMold->BlowPin.ZeroPos != 0                            &&
		pMold->BlowPin.MaxPos > (pMold->BlowPin.ZeroPos + 3000) &&
		pMold->BlowPin.As     > (pMold->BlowPin.ZeroPos - 2500 ) &&
		pMold->BlowPin.As     < (pMold->BlowPin.MaxPos  + 2500 ))
	{
		pMold->BlowPin.CalOK = pMold->BlowPin.NetworkInit;
	}
	else
	{
		pMold->BlowPin.CalOK = 0;
	}
	
	pBlowPinFix->CalibOK = pMold->BlowPin.CalOK;

	if(1 == pMold->BlowPin.CalOK)
	{
		if(pBlowPinFix->Max < pBlowPinFix->Zero)
		{
			pMold->BlowPin.mmAs = 1.0 *(pMold->BlowPin.MaxPos - pMold->BlowPin.As) * pBlowPinFix->Length /
				(pMold->BlowPin.MaxPos - pMold->BlowPin.ZeroPos);
		}
		else
		{
			pMold->BlowPin.mmAs = 1.0 *(pMold->BlowPin.As - pMold->BlowPin.ZeroPos) * pBlowPinFix->Length /
				(pMold->BlowPin.MaxPos - pMold->BlowPin.ZeroPos);
		}
	}
	else
	{
		pMold->BlowPin.mmAs = pMold->BlowPin.mmAs;
		if(0 == gMachineInfo.Calib)	/* Not calibration mode  */
		{
			if(pMold->BlowPin.Step > 0 && pMold->BlowPin.Step < 20000)
			{
				pMold->BlowPin.Step = 0;
				pMold->Alarm.BlowPinCalErr = 1;
			}
		}
		else if (0 == pMold->BlowPin.NetworkInit)
		{
			if(pMold->BlowPin.Step > 0 && pMold->BlowPin.Step < 20000)
			{
				pMold->BlowPin.Step = 0;
			}
		}
	}/* if(1 == pMold->BlowPin.CalOK) */
	
	/* network and calibration is ok, axis has an error   */
	if(0 == pMold->BlowPin.AxisReady) 		  
	{
		if(pMold->BlowPin.Step > 0 && pMold->BlowPin.Step < 20000)
		{
			pMold->BlowPin.Step = 30000;
		}
	}

	/*  relative position  */
	pMold->BlowPin.Ss = pMold->BlowPin.As;
	
	/*********************************************************************************
  	**                            blow pin status                                   **
  	*********************************************************************************/
	if(1 == pMold->BlowPin.CalOK)
	{
		pMold->BlowPin.UpPos = pMold->TransDIn.BlowPinUpLimit & (pMold->BlowPin.mmAs < (pBlowPinPara->S_Up + pBlowPinFix->Tolerance)); 
		pMold->BlowPin.DnPos = pMold->TransDIn.BlowPinDnLimit & (pMold->BlowPin.mmAs > (pBlowPinPara->S_Dn - pBlowPinFix->Tolerance)); 
	}
	else
	{
		pMold->BlowPin.UpPos = 0;
		pMold->BlowPin.DnPos = 0;
	}
	 
	/* ------------------ function current position ------------------ */
	pMold->BlowPin.vProfGen.s_act = pMold->BlowPin.mmAs;
	pMold->BlowPin.vProfGen.v_act = pMold->BlowPin.v_act;

	switch ( pMold->BlowPin.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;
			pMold->BlowPin.Timer.IN  = 0;
			pMold->ActInfo.BlowPinUp = 0;
			pMold->ActInfo.BlowPinDn = 0;
            

			break;
		
		/*------------------ Clean Valve ------------------*/
		case 5:
			/*Clean Valve*/
			break;

		/*------------------ BlowPin move down -------------------- */
		case 100: 	/* down delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowPinDnDelay > 0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnDelay;
					pMold->BlowPin.Step 	= 200;
				}
				else
				{
					if(0 == gMacOption.bDryCycle)
					{
						pMold->BlowPin.Step = 300;
					}
					else
					{
						pMold->BlowPin.Step = 3000;
					}
				}
			}
			else
			{
				pMold->BlowPin.Step = 3000;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.BlowPinDnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//		pMold->TimeDis.BlowPinDnDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin.Step = 300;
				}
				else
				{
					pMold->BlowPin.Step = 3000;
				}
			}
			break;

		case 300:
			if(0 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->BlowPin.LimitTimer.IN = 1;
				pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinDnAlarmTime;
					
				pMold->BlowPin.vProfGen.Ts = tTask;
				pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Down.Tj;
				pMold->BlowPin.vProfGen.Td = pBlowPinFix->Down.Td;
					
				pMold->BlowPin.vProfGen.accel = pBlowPinFix->Down.accel;
				if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
					
				pMold->BlowPin.vProfGen.decel = pBlowPinFix->Down.accel;
				if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
					            
				pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Down.decel_end;
				if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
					            
				pMold->BlowPin.vProfGen.v_start = 0.0;
				pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_Dn;
			
				if(pMold->TimeSet.BlowPinDnPressTime == 0 || 1 == gMacOption.bDryCycle || 1 == gMachineInfo.Manual )
				{
					pMold->BlowPin.vProfGen.v_end = 0;
				}
				else
				{
					pMold->BlowPin.vProfGen.v_end	= pMold->BlowPin.V_DnSlow/2;
				}
				pMold->BlowPin.vProfGen.s_end = pMold->BlowPin.S_Dn;
				pMold->BlowPin.vProfGen.init  = 1;
						
				pMold->BlowPin.Step = 400;
			}
			else
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;

		case 400:		/*  quick down  */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset;
			}
			else
			{
				pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_Dn;
			}
			
            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin.vProfGen);
            
			pMold->BlowPin.p_set = pMold->BlowPin.P_Dn;
			pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
			
			pMold->BlowPin.actTjDn = pMold->BlowPin.vProfGen.Tj;
			
			/*  reached target position */
			if(pMold->BlowPin.mmAs >= pMold->BlowPin.S_Dn - pMold->BlowPin.Tolerance) //ipis0713
			{
				
				if(0 == gMacOption.bDryCycle)
				{
					if(1 == gMachineInfo.Auto)   //auto
					{
						if(1 == gMachineInfo.MachineReset)  //auto in reset
						{
							pMold->BlowPin.Step = 700;
						}
						else
						{
							pMold->BlowPin.Step = 500;
						}	  
					}
					else
					{
						pMold->BlowPin.Step = 700;  
					}
					
				}
				else
				{
					pMold->BlowPin.Step = 700;  /* to end  */
				}
			}

			
			if(1 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
     
		case 500:
			if(pMold->TimeSet.BlowPinDnPressTime != 0)
			{
				pMold->ActInfo.BlowPinDn  = LED_ON;
				pMold->ActInfo.BlowPinUp  = LED_OFF;

	            
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnPressTime;
	            
				pMold->BlowPin.vTimeProf.Ts = tTask;
				pMold->BlowPin.vTimeProf.t_move = pMold->TimeSet.BlowPinDnPressTime * 0.01;
	            
				pMold->BlowPin.vTimeProf.v_set   = pMold->BlowPin.V_DnSlow; 
				pMold->BlowPin.vTimeProf.v_start = pMold->BlowPin.v_set; 
				pMold->BlowPin.vTimeProf.v_end   = 0.0;
	            
				pMold->BlowPin.vTimeProf.accel = 2000;
				if(pMold->BlowPin.vTimeProf.accel <= 0.0)pMold->BlowPin.vTimeProf.accel = 1000;
	            
				pMold->BlowPin.vTimeProf.decel = 2000;
				if(pMold->BlowPin.vTimeProf.decel <= 0.0)pMold->BlowPin.vTimeProf.decel = 1000;
	            
				pMold->BlowPin.vTimeProf.init = 1;
				pMold->BlowPin.Step = 600;
			}
			else
			{
				pMold->BlowPin.Step = 700;
			}
			break;

		case 600:		/*  quick down (Press torque procedure) */
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			HydvTimeProf(&pMold->BlowPin.vTimeProf);
				
			pMold->BlowPin.p_set = pMold->BlowPin.P_DnSlow;
			pMold->BlowPin.v_set = pMold->BlowPin.vTimeProf.v_out;

			pMold->TimeDis.BlowPinDnPressTime = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.v_set = 0;
				pMold->BlowPin.Timer.IN      = 0;
				//				pMold->TimeDis.BlowPinDnPressTime = 0;
				pMold->BlowPin.Step = 700;
			}

			if(1 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;

		case 700:
			pMold->BlowPin.v_set = 0;
			
			pMold->ActInfo.BlowPinDn  = LED_OFF;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			
			pMold->BlowPin.Timer.IN      = 0;
			if(1 == gMachineInfo.Auto)
			{
				if(1 == gMachineInfo.MachineReset)  //auto in reset
				{
					pMold->BlowPin.LimitTimer.IN = 0;
					pMold->BlowPin.Step = 3000;
				}
				else
				{
					pMold->BlowPin.LimitTimer.IN = 0;
					if(pMold->TimeSet.BlowPinDnPressTime != 0 )
					{
						pMold->BlowPin.Step = 1300;  /*Press torque 2*/
					}
					else
					{
						pMold->BlowPin.Step = 3000; 
					}
				}
			}
			else
			{
				pMold->BlowPin.LimitTimer.IN = 0;
				pMold->BlowPin.Step = 3000;  
			}

			
			break;
		

      
		/*  ------------------- (Press torque 2 procedure Set) ------------------- */ 
		case 1300: //(Press torque 2 procedure Set)

			if(pMold->TimeSet.BlowPinDnPress2HoldTime != 0)
			{
				pMold->ActInfo.BlowPinDn  = LED_ON;
				pMold->ActInfo.BlowPinUp  = LED_OFF;

	            
				pMold->BlowPin.Timer.IN = 1;
				pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinDnPress2HoldTime;
	            
				pMold->BlowPin.vTimeProf.Ts = tTask;
				pMold->BlowPin.vTimeProf.t_move = pMold->TimeSet.BlowPinDnPress2HoldTime * 0.01;
	            
				pMold->BlowPin.vTimeProf.v_set   = pMold->BlowPin.V_DnSlow; 
				pMold->BlowPin.vTimeProf.v_start = pMold->BlowPin.v_set; 
				pMold->BlowPin.vTimeProf.v_end   = 0.0;
	            
				pMold->BlowPin.vTimeProf.accel = 2000;
				if(pMold->BlowPin.vTimeProf.accel <= 0.0)pMold->BlowPin.vTimeProf.accel = 1000;
	            
				pMold->BlowPin.vTimeProf.decel = 2000;
				if(pMold->BlowPin.vTimeProf.decel <= 0.0)pMold->BlowPin.vTimeProf.decel = 1000;
	            
				pMold->BlowPin.vTimeProf.init = 1;
				pMold->BlowPin.Step = 1400;
			}
			else
			{
				pMold->BlowPin.Step = 1500;
			}
			break;

			break;
              
		case 1400: //(Press torque 2 procedure )
			
			pMold->ActInfo.BlowPinDn  = LED_ON;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			pMold->TimeDis.BlowPinDnPress2HoldTime = pMold->BlowPin.Timer.ET;
			HydvTimeProf(&pMold->BlowPin.vTimeProf);
				
			pMold->BlowPin.p_set = pBlowPinFix->pPress2Hold;
			pMold->BlowPin.v_set = pMold->BlowPin.vTimeProf.v_out;

			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.v_set = 0;
				pMold->BlowPin.Timer.IN      = 0;
				//				pMold->TimeDis.BlowPinDnPressTime = 0;
				pMold->BlowPin.Step = 1500;
			}
			
				
			if(1 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			else if(1 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
      
		case 1500:
			pMold->BlowPin.Timer.IN   = 0;
			pMold->ActInfo.BlowPinUp  = LED_OFF;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->BlowPin.v_set = 0;
            
			pMold->BlowPin.Step = 3000;
			break;
		

		//		/*  ------------------- Blow pin 2nd down ------------------- */
		//		case 2100:
		//			if(pMold->Clamp.mmAs >= pClampPara->S_BlowpinOut)
		//			{
		//				if(1 == pMold->OncePin2ndDn)
		//				{
		//					pMold->OncePin2ndDn = 0;
		//					pMold->BlowPin.Step = 2200;
		//				}
		//			}
		//			break;
		//
		//		case 2200:
		//			pMold->BlowPin.Timer.IN = 1;
		//			pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPin2DnDelay;
		//            
		//			pMold->BlowPin.Step = 2300;
		//			break;
		//
		//		case 2300:
		//			pMold->TimeDis.BlowPin2DnDelay = pMold->BlowPin.Timer.ET;
		//			if (1 == pMold->BlowPin.Timer.Q)
		//			{
		//				pMold->BlowPin.Timer.IN = 0;
		//				pMold->TimeDis.BlowPin2DnDelay = 0;
		//				pMold->BlowPin.Step = 2400;
		//			}
		//			break;
		//         
		//		case 2400:
		//			pMold->BlowPin.Timer.IN = 1;
		//			pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPin2DnTime;
		//            
		//			pMold->BlowPin.vTimeProf.Ts = tTask;
		//			pMold->BlowPin.vTimeProf.t_move = pMold->TimeSet.BlowPin2DnTime * 0.01;
		//            
		//			pMold->BlowPin.vTimeProf.v_set   = pBlowPinPara->V_Dn2nd * 10.0; 
		//			pMold->BlowPin.vTimeProf.v_start = 0.0; 
		//			pMold->BlowPin.vTimeProf.v_end   = 0.0;
		//            
		//			pMold->BlowPin.vTimeProf.accel = pBlowPinFix->Down.accel;
		//			if(pMold->BlowPin.vTimeProf.accel <= 0.0)pMold->BlowPin.vTimeProf.accel = 1000;
		//            
		//			pMold->BlowPin.vTimeProf.decel = pBlowPinFix->Down.decel;
		//			if(pMold->BlowPin.vTimeProf.decel <= 0.0)pMold->BlowPin.vTimeProf.decel = 1000;
		//            
		//			pMold->BlowPin.vTimeProf.init = 1;
		//			pMold->BlowPin.Step = 2500;
		//			break;
		//
		//		case 2500:
		//			pMold->ActInfo.BlowPinDn  = LED_ON;
		//                
		//			HydvTimeProf(&pMold->BlowPin.vTimeProf);
		//            
		//			pMold->BlowPin.p_set = pBlowPinPara->P_Dn2nd;
		//			pMold->BlowPin.v_set = pMold->BlowPin.vTimeProf.v_set;
		//            
		//			pMold->TimeDis.BlowPin2DnTime = pMold->BlowPin.Timer.ET;
		//			if (1 == pMold->BlowPin.Timer.Q)
		//			{
		//				pMold->BlowPin.v_set = 0;
		//				pMold->BlowPin.Timer.IN = 0;
		//				pMold->TimeDis.BlowPin2DnTime = 0;
		//				pMold->BlowPin.Step = 2600;
		//			}
		//			
		//			if(1 == pMold->BlowPin.DnHWSwitch)
		//			{
		//				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
		//				pMold->BlowPin.Step 	= 41000;
		//			}
		//			break;
		//
		//		case 2600:
		//			pMold->BlowPin.Timer.IN = 0;
		//			pMold->BlowPin.LimitTimer.IN = 0;
		//            
		//			pMold->ActInfo.BlowPinUp  = LED_OFF;
		//			pMold->ActInfo.BlowPinDn  = LED_OFF;
		//            
		//			pMold->BlowPin.v_set = 0;
		//            
		//			pMold->BlowPin.Step = 3000;
		//			break;

		case 3000:
			break;

		/*--------------- BlowPin calibration down -----------------*/
		case 5000:
			if(0 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->ActInfo.BlowPinDn  = LED_ON;

				pMold->BlowPin.p_set = pBlowPinFix->pCalibMax;
				pMold->BlowPin.v_set = pBlowPinFix->vCalibMax;
				
			}
			
			if(1 == pMold->BlowPin.DnHWSwitch)
			{
				pMold->HwAlarm.BlowPinDnHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
      
		/*--------------- BlowPin move up -----------------*/
		case 10100: 	/* open delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				//				if(pMold->TimeSet.BlowPinUpDelay > 0)
				//				{
				//					pMold->BlowPin.Timer.IN = 1;
				//					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinUpDelay;
				//					pMold->BlowPin.Step = 10200;
				//				}
				//				else
				//				{
				//					if(0 == gMacOption.bDryCycle)
				//					{
				//						pMold->BlowPin.Step = 10300;
				//					}
				//					else
				//					{
				//						pMold->BlowPin.Step = 13000;
				//					}
				//				}
			
				if(0 == gMacOption.bDryCycle)  //ipis0719
				{
					if(pMold->TimeSet.BlowPinUpDelay > 0)
					{
						pMold->BlowPin.Timer.IN = 1;
						pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowPinUpDelay;
						pMold->BlowPin.Step = 10200;
					}
					else
					{
						pMold->BlowPin.Step = 10300;
					}	
					
				}
				else
				{
					pMold->BlowPin.Step = 13000;
				}
			
			}
			break;
		
		case 10200: 	/* delay ok */
			pMold->TimeDis.BlowPinUpDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//		pMold->TimeDis.BlowPinUpDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin.Step = 10300;
				}
				else
				{
					pMold->BlowPin.Step = 13000;
				}
			}
			break;
      
		case 10300:
			if(0 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->BlowPin.LimitTimer.IN = 1;
				pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
					
				pMold->BlowPin.vProfGen.Ts = tTask;
				pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Up.Tj;
				pMold->BlowPin.vProfGen.Td = pBlowPinFix->Up.Td;
					
				pMold->BlowPin.vProfGen.accel = pBlowPinFix->Up.accel;
				if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
					
				pMold->BlowPin.vProfGen.decel = pBlowPinFix->Up.accel;
				if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
					            
				pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
				if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
			
		
				pMold->BlowPin.vProfGen.v_start = 0.0;
				pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_Up ;
				pMold->BlowPin.vProfGen.v_end = 0.0;
				pMold->BlowPin.vProfGen.s_end = pMold->BlowPin.S_Up;
				pMold->BlowPin.vProfGen.init  = 1;	

				pMold->BlowPin.Step = 10400;

			}
			else
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
        
		case 10400:
			pMold->ActInfo.BlowPinUp  = LED_ON;
			pMold->ActInfo.BlowPinDn  = LED_OFF;
			
			if(1 == gMachineInfo.MachineReset)  /* AutoReset  */
			{
				pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpReset;
			}
			else
			{
				pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_Up;
			}

			
            
			/* call profile function */
			HydvProfGen(& pMold->BlowPin.vProfGen);
 			
			pMold->BlowPin.p_set = pMold->BlowPin.P_Up;
			pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
				
			pMold->BlowPin.actTjUp = pMold->BlowPin.vProfGen.Tj;
			
			/*  reached target position */
			if((pMold->BlowPin.mmAs <= pMold->BlowPin.S_Up + pMold->BlowPin.Tolerance)  /*&& (1 == pMold->TransDIn.BlowPinUpLimit)*/) //ipis0713
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 10500;
			}
			
			if(1 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
      
		case 10500:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp  = LED_OFF;
			pMold->ActInfo.BlowPinDn  = LED_OFF;

			pMold->BlowPin.v_set = 0;

			pMold->BlowPin.Step = 13000;
			break;
      
		case 13000:
			
			if(1 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;

		/*--------------- BlowPin calibration up -----------------*/
		case 15000:
			if(0 == pMold->BlowPin.UpHWSwitch)
			{

				pMold->ActInfo.BlowPinUp  = LED_ON;
				pMold->ActInfo.BlowPinDn  = LED_OFF;
			

				pMold->BlowPin.p_set = pBlowPinFix->pCalibZero;
				pMold->BlowPin.v_set = -pBlowPinFix->vCalibZero;
			
			}
			
			if(1 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
		
		


		/*--------------- BlowPin tiny return (PreLift)-----------------*/

		case 15100: 		/* micro up  delay*/
			if(1 == pMold->BlowPin.Enable)
			{
				if(pMold->TimeSet.BlowpintinyreturnDelay > 0)
				{
					pMold->BlowPin.Timer.IN = 1;
					pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowpintinyreturnDelay;
					pMold->BlowPin.Step 	= 15200;
				}
				else
				{
					if(0 == gMacOption.bDryCycle)
					{
						pMold->BlowPin.Step = 15300;
					}
					else
					{
						pMold->BlowPin.Step = 18000;
					}
				}
			}
			else
			{
				pMold->BlowPin.Step = 18000;
			}
			break;
      
		case 15200: 		/* delay ok */
			pMold->TimeDis.BlowpintinyreturnDelay = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.Timer.IN       = 0;
				//				pMold->TimeDis.BlowpintinyreturnDelay = 0;
				if(0 == gMacOption.bDryCycle)
				{
					pMold->BlowPin.Step = 15300;
				}
				else
				{
					pMold->BlowPin.Step = 18000;
				}
			}
			break;
		
		case 15300:
			if(0 == gMacOption.bDryCycle) //ipis0719
			{
				if(0 == pMold->BlowPin.UpHWSwitch)
				{
					if(pMold->TimeSet.BlowpintinyreturnTime != 0)
					{
						pMold->BlowPin.Timer.IN = 1;
						pMold->BlowPin.Timer.PT = pMold->TimeSet.BlowpintinyreturnTime;
						
						pMold->BlowPin.Step = 15400;	
					}
					else
					{
						pMold->BlowPin.Step = 15500;
					}
					
				}
				else
				{
					pMold->HwAlarm.BlowPinUpHWSwitch = 1;
					pMold->BlowPin.Step 	= 41000;
				}
			}
			else
			{
				pMold->BlowPin.Step = 18000;  //ipis0719
			
			}
			
			break;
              
		case 15400:
			pMold->ActInfo.BlowPinUp  = LED_ON;
			pMold->ActInfo.BlowPinDn  = LED_OFF;
	
			pMold->BlowPin.p_set = pMold->BlowPin.P_UpMicro;
			pMold->BlowPin.v_set = -pMold->BlowPin.V_UpMicro;
					
			pMold->TimeDis.BlowpintinyreturnTime = pMold->BlowPin.Timer.ET;
			if (1 == pMold->BlowPin.Timer.Q)
			{
				pMold->BlowPin.p_set = 0.0;
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Timer.IN = 0;
				
				pMold->BlowPin.Step = 15500;
			}

			if(1 == pMold->BlowPin.UpHWSwitch)
			{
				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
				pMold->BlowPin.Step 	= 41000;
			}
			break;
      
		//		case 15300:
		//			if(0 == gMacOption.bDryCycle) //ipis0719
		//			{
		//				if(0 == pMold->BlowPin.UpHWSwitch)
		//				{
		//					pMold->BlowPin.LimitTimer.IN = 1;
		//					pMold->BlowPin.LimitTimer.PT = pMold->TimeSet.BlowPinUpAlarmTime;
		//						
		//					pMold->BlowPin.vProfGen.Ts = tTask;
		//					//					pMold->BlowPin.vProfGen.Tj = pBlowPinFix->Up.Tj;
		//					//					pMold->BlowPin.vProfGen.Td = pBlowPinFix->Up.Td;
		//					//						
		//					//					pMold->BlowPin.vProfGen.accel = pBlowPinFix->Up.accel;
		//					//					if(pMold->BlowPin.vProfGen.accel <= 0.0)pMold->BlowPin.vProfGen.accel = 1000;
		//					//						
		//					//					pMold->BlowPin.vProfGen.decel = pBlowPinFix->Up.accel;
		//					//					if(pMold->BlowPin.vProfGen.decel <= 0.0)pMold->BlowPin.vProfGen.decel = 1000;
		//					//						            
		//					//					pMold->BlowPin.vProfGen.decel_end = pBlowPinFix->Up.decel_end;
		//					//					if(pMold->BlowPin.vProfGen.decel_end <= 0.0)pMold->BlowPin.vProfGen.decel_end = 100;
		//					
		//					pMold->BlowPin.vProfGen.Tj = 0;
		//					pMold->BlowPin.vProfGen.Td = 0;
		//					
		//					pMold->BlowPin.vProfGen.accel = 1000;
		//					pMold->BlowPin.vProfGen.decel = 1000;
		//					pMold->BlowPin.vProfGen.decel_end = 1000;
		//					
		//						            
		//					pMold->BlowPin.vProfGen.v_start = 0.0;
		//					pMold->BlowPin.vProfGen.v_set   = pMold->BlowPin.V_UpMicro;
		//					pMold->BlowPin.vProfGen.v_end = 0.0;
		//					pMold->BlowPin.vProfGen.s_end = pMold->BlowPin.S_Dn - pMold->BlowPin.S_UpMicro;
		//					pMold->BlowPin.vProfGen.init  = 1;
		//							
		//					pMold->BlowPin.Step = 15400;
		//				}
		//				else
		//				{
		//					pMold->HwAlarm.BlowPinUpHWSwitch = 1;
		//					pMold->BlowPin.Step 	= 41000;
		//				}
		//			}
		//			else
		//			{
		//				pMold->BlowPin.Step = 18000;  //ipis0719
		//			
		//			}
		//			
		//			
		//			break;
		//              
		//		case 15400:
		//			pMold->ActInfo.BlowPinUp  = LED_ON;
		//			pMold->ActInfo.BlowPinDn  = LED_OFF;
		//
		//			pMold->BlowPin.vProfGen.v_set = pMold->BlowPin.V_UpMicro ;
		//	            
		//			/* call profile function */
		//			HydvProfGen(& pMold->BlowPin.vProfGen);
		//	 			
		//			pMold->BlowPin.p_set = pMold->BlowPin.P_UpMicro;
		//			pMold->BlowPin.v_set = pMold->BlowPin.vProfGen.v_out;
		//					
		//			pMold->BlowPin.actTjUp = pMold->BlowPin.vProfGen.Tj;
		//				
		//			/*  reached target position */
		//			//	if(pMold->BlowPin.mmAs <= pMold->BlowPin.vProfGen.s_end + 0.01)
		//			if(pMold->BlowPin.mmAs <= pMold->BlowPin.vProfGen.s_end + pBlowPinFix->Tolerance)	//ipis0724
		//			{
		//				pMold->BlowPin.v_set = 0.0;
		//				pMold->BlowPin.Step = 15500;
		//			}
		//
		//			if(1 == pMold->BlowPin.UpHWSwitch)
		//			{
		//				pMold->HwAlarm.BlowPinUpHWSwitch = 1;
		//				pMold->BlowPin.Step 	= 41000;
		//			}
		//			break;
      
		case 15500:
			pMold->BlowPin.Timer.IN   = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
			pMold->ActInfo.BlowPinUp  = LED_OFF;

			pMold->BlowPin.v_set = 0;
			pMold->BlowPin.p_set = 0;
            
			pMold->BlowPin.Step = 18000;
			break;
      
		case 18000:
			break;

		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin.p_old = pMold->BlowPin.p_set;
			pMold->BlowPin.Step = 21000;
			break;
        
		case 21000:	
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
 				
			pMold->BlowPin.vRamp.Ts = tTask;
			
			if(pMold->BlowPin.v_set > 0)
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Down.accel_stop;
			}
			else
			{
				pMold->BlowPin.vRamp.Accel = pBlowPinFix->Up.accel_stop;
			}
				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
			pMold->BlowPin.p_set = pMold->BlowPin.p_old;
 				
			pMold->BlowPin.Step = 22000;

			break;
    			
		case 22000:
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
            
			/* call profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
            
			pMold->BlowPin.p_set = pMold->BlowPin.p_old;
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
			
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 0;
			
				pMold->BlowPin.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch
			}

			break;
        
		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = pBlowPinFix->Down.accel_stop;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 31000;
			break;
		
		case 31000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 32000;
			
				pMold->BlowPin.BtnOffDlyFlag = 1;  //owen Avoid Motor Fw/Bw quick switch
			}
			break;
	    			
		case 32000:
			pMold->BlowPin.Step = 0;
			break;
      
		/*----------------- Error step -----------------*/
		case 40000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;

			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
            
			pMold->BlowPin.p_set = 0;
			pMold->BlowPin.v_set = 0;
			break;
		
		case 41000:
			pMold->BlowPin.Timer.IN      = 0;
			pMold->BlowPin.LimitTimer.IN = 0;
	
			pMold->ActInfo.BlowPinUp = LED_OFF;
			pMold->ActInfo.BlowPinDn = LED_OFF;
	            
			pMold->BlowPin.vRamp.Internal.Once = 0;
			pMold->BlowPin.vRamp.Set = pMold->BlowPin.v_set;
	 				
			pMold->BlowPin.vRamp.Ts = tTask;
			pMold->BlowPin.vRamp.Accel = INSTANCY_STOP_DECEL;
	 				
			HydSCurveRamp(&pMold->BlowPin.vRamp);
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	
			pMold->BlowPin.Step = 42000;
			break;
	        
		case 42000:
			pMold->BlowPin.vRamp.Ts  = tTask;
			pMold->BlowPin.vRamp.Set = 0.0;
	 				
			/* call s profile function */
			HydSCurveRamp(&pMold->BlowPin.vRamp);
	 				
			pMold->BlowPin.v_set = pMold->BlowPin.vRamp.Out;
	 				
			if(pMold->BlowPin.v_set < 0.1 && pMold->BlowPin.v_set > -0.1)
			{
				pMold->BlowPin.v_set = 0.0;
				pMold->BlowPin.Step = 43000;
			}
			break;
	    			
		case 43000:
			pMold->BlowPin.Step = 40000;
			break;
	}/* end of switch */
	
	if(pMold->BlowPin.Step > 300 && pMold->BlowPin.Step < 700)
	{
		pMold->TimeDis.BlowPinDnAlarmTime = pMold->BlowPin.LimitTimer.ET;
	}
	else if((pMold->BlowPin.Step > 10300 && pMold->BlowPin.Step < 10500)
		||(pMold->BlowPin.Step > 15300 && pMold->BlowPin.Step < 15500)
		)
	{
		pMold->TimeDis.BlowPinUpAlarmTime = pMold->BlowPin.LimitTimer.ET;
	}
	
	
	/* ----------------- blow pin up / down timeout --------------------  */
	if(1 == pMold->BlowPin.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BlowPin.Step < 1500 && pMold->BlowPin.Step > 200)		/* BlowPin Down  */
		{
			pMold->Alarm.BlowPinNotAtDnPos = 1;
		}
		
		if((pMold->BlowPin.Step < 13000 && pMold->BlowPin.Step > 10200)		/* BlowPin Up  */
			||(pMold->BlowPin.Step < 18000 && pMold->BlowPin.Step > 15300) )	/* BlowPin Up Micro  */
		{
			pMold->Alarm.BlowPinNotAtUpPos = 1;
		}
      
		pMold->BlowPin.LimitTimer.IN = 0;
		pMold->BlowPin.Step = 40000;
	}

	/* ----------------- blow pin down action limit --------------------  */
	if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000)  /*  blow pin down  */
	{
		
		if( 0 == pMold->Carriage.FwPos && 0 == pMold->Carriage.BwPos )
		{
			pMold->Alarm.CarriageNotAtFwBwPos = 1;
			pMold->BlowPin.Step = 40000;
		}
		
		if( 0 == pMold->TransDIn.SubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.SubMoldOpnLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if( 0 == pMold->TransDIn.BottomSubMoldClsLimit) 	
		{
			pMold->Alarm.SubMoldNotAtOpnPos  = !pMold->TransDIn.BottomSubMoldClsLimit;
			pMold->BlowPin.Step = 41000;
		}
		
		if(1 == gMachineInfo.Auto && 0 == gMachineInfo.MachineReset)	
		{
			if( 1 == pMold->Option.HookPin)
			{
				if( 0 == pMold->TransDIn.HookPinFwLimit)/*  Hook Pin not at backward limit   */
				{
					pMold->Alarm.HookPinNotAtFwPos = !pMold->TransDIn.HookPinFwLimit;
					pMold->BlowPin.Step = 41000;
				}
			}
		}
		
		//		if(1 == gMachineInfo.Auto)	
		//		{
		//			if(0 == pMold->Carriage.BwPos)
		//			{
		//				pMold->Alarm.CarriageNotAtBwDI = !pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//		}
		//		else
		//		{
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			if(0 == pMold->Clamp.OpnPos && 0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos)
		//			{
		//				pMold->Alarm.ClampNotAtOpnDI   = ! pMold->Clamp.OpnPos;
		//				pMold->Alarm.CarriageNotAtBwDI = ! pMold->Carriage.BwPos;
		//				pMold->BlowPin.Step = 40000;
		//			}
		//			
		//			0 == pMold->Carriage.BwPos && 0 == pMold->Carriage.FwPos
		//		}
	}/* if( pMold->BlowPin.Step > 100 && pMold->BlowPin.Step < 3000) */
	
    
}/*end of function */

void ButtonOffDelay(Mold_typ * pMold,timer_typ * pTimer)
{
	if(  1 == pMold->BlowPin.BtnOffDlyFlag )
	{
		pTimer->IN = 1;
		pTimer->PT = 100;
	
		if( 1 == pTimer->Q)
		{
			pMold->BlowPin.BtnOffDlyFlag = 0;
		}
	}
	else
	{
		pTimer->IN = 0;
	}
	
}
