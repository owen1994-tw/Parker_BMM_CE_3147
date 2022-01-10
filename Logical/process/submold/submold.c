/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: submold
 * File: submold.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program submold --- submold  open and close control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void SubMold( Mold_typ * pMold,SubMold_Para_typ * pSubMoldPara,SubMold_Fix_typ * pSubMoldFix,Clamp_Para_typ * pClampPara);

void BottomSubMold( Mold_typ * pMold,SubMold_Para_typ * pSubMoldPara,SubMold_Fix_typ * pSubMoldFix,Clamp_Para_typ * pClampPara);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
 {
 	LMold.SubMold.Step = 30000;
 	RMold.SubMold.Step = 30000;

 } /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {
	RMold.TransDIn.SubMoldOpnLimit = gMachineIn.RMold.SubMoldOpnLimit;
	RMold.TransDIn.SubMoldClsLimit = gMachineIn.RMold.SubMoldClsLimit;

	LMold.TransDIn.SubMoldOpnLimit = gMachineIn.LMold.SubMoldOpnLimit;
	LMold.TransDIn.SubMoldClsLimit = gMachineIn.LMold.SubMoldClsLimit;
	
	
	RMold.TransDIn.BottomSubMoldOpnLimit = gMachineIn.RMold.BottomSubMoldOpnLimit;
	RMold.TransDIn.BottomSubMoldClsLimit = gMachineIn.RMold.BottomSubMoldClsLimit;

	LMold.TransDIn.BottomSubMoldOpnLimit = gMachineIn.LMold.BottomSubMoldOpnLimit;
	LMold.TransDIn.BottomSubMoldClsLimit = gMachineIn.LMold.BottomSubMoldClsLimit;


	
	if(!LMold.TimeSet.SubMoldOpnAlarmTime)LMold.TimeSet.SubMoldOpnAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.SubMoldOpnAlarmTime)RMold.TimeSet.SubMoldOpnAlarmTime = 500;  /* 5s  */

	if(!LMold.TimeSet.SubMoldClsAlarmTime)LMold.TimeSet.SubMoldClsAlarmTime = 500;  /* 5s  */
	if(!RMold.TimeSet.SubMoldClsAlarmTime)RMold.TimeSet.SubMoldClsAlarmTime = 500;  /* 5s  */

	SubMold(&LMold,&gMachinePara.MoldL.SubMold,&gMachineFix.MoldL.SubMold,&gMachinePara.MoldL.Clamp);
	SubMold(&RMold,&gMachinePara.MoldR.SubMold,&gMachineFix.MoldR.SubMold,&gMachinePara.MoldR.Clamp);

	BottomSubMold(&LMold,&gMachinePara.MoldL.BottomSubMold,&gMachineFix.MoldL.BottomSubMold,&gMachinePara.MoldL.Clamp);
	BottomSubMold(&RMold,&gMachinePara.MoldR.BottomSubMold,&gMachineFix.MoldR.BottomSubMold,&gMachinePara.MoldR.Clamp);
	
	gMachineOut.LMold.SubMoldOpn = LMold.ValveOut.SubMoldOpn;
	gMachineOut.LMold.SubMoldCls = LMold.ValveOut.SubMoldCls;

	gMachineOut.RMold.SubMoldOpn = RMold.ValveOut.SubMoldOpn;
	gMachineOut.RMold.SubMoldCls = RMold.ValveOut.SubMoldCls;
	
	gMachineOut.LMold.BottomSubMoldOpn = LMold.ValveOut.BottomSubMoldOpn;
	gMachineOut.LMold.BottomSubMoldCls = LMold.ValveOut.BottomSubMoldCls;

	gMachineOut.RMold.BottomSubMoldOpn = RMold.ValveOut.BottomSubMoldOpn;
	gMachineOut.RMold.BottomSubMoldCls = RMold.ValveOut.BottomSubMoldCls;
	
	LMold.TimeSet.BottomSubMoldOpnTime = LMold.TimeSet.SubMoldOpnTime;
	RMold.TimeSet.BottomSubMoldOpnTime = RMold.TimeSet.SubMoldOpnTime;
	
	LMold.TimeSet.BottomSubMoldClsDelay = LMold.TimeSet.SubMoldClsDelay;
	RMold.TimeSet.BottomSubMoldClsDelay = RMold.TimeSet.SubMoldClsDelay;
	
	LMold.TimeSet.BottomSubMoldOpnDelay = LMold.TimeSet.SubMoldOpnDelay;
	RMold.TimeSet.BottomSubMoldOpnDelay = RMold.TimeSet.SubMoldOpnDelay;

	LMold.TimeSet.BottomSubMoldClsHoldTime = LMold.TimeSet.SubMoldClsHoldTime;
	RMold.TimeSet.BottomSubMoldClsHoldTime = RMold.TimeSet.SubMoldClsHoldTime;
	
//	LMold.TimeSet.BottomSubMoldOpnAlarmTime = LMold.TimeSet.SubMoldOpnAlarmTime;
//	RMold.TimeSet.BottomSubMoldOpnAlarmTime = RMold.TimeSet.SubMoldOpnAlarmTime;
//
//	LMold.TimeSet.BottomSubMoldClsAlarmTime = LMold.TimeSet.SubMoldClsAlarmTime;
//	RMold.TimeSet.BottomSubMoldClsAlarmTime = RMold.TimeSet.SubMoldClsAlarmTime;

	/*------------- ActuatorType--------------- */
	gMachineFix.MoldL.SubMold.eActuatorType = gMachineFix.MoldR.SubMold.eActuatorType;
	
	/*------------ PressueOutputType--------------- */
	gMachineFix.MoldL.SubMold.ePressueOutputType = gMachineFix.MoldR.SubMold.ePressueOutputType;
	
	/*------------ FluxOutputType--------------- */
	gMachineFix.MoldL.SubMold.eFluxOutputType = gMachineFix.MoldR.SubMold.eFluxOutputType;

 } /* end of _CYCLIC */

void SubMold( Mold_typ * pMold,SubMold_Para_typ * pSubMoldPara,SubMold_Fix_typ * pSubMoldFix,Clamp_Para_typ * pClampPara)
{
	if(pMold == & LMold)
	{
		pSubMoldFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pSubMoldFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	if(0 == pSubMoldPara->V_Reset)pSubMoldPara->V_Reset = 5;
	if(0 == pSubMoldPara->P_Reset)pSubMoldPara->P_Reset = 5;
	
	switch ( pMold->SubMold.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;

			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.Timer.IN	= 0;
			pMold->SubMold.LimitTimer.IN = 0;
			
			break;

		/*--------------- SubMold close -----------------*/
		case 100: 	/* close delay*/
			if(1 == pMold->Option.SubMold)
			{
				pMold->SubMold.Timer.IN = 1;
				pMold->SubMold.Timer.PT = pMold->TimeSet.SubMoldClsDelay;
				pMold->SubMold.Step = 200;
			}
			else
			{
				pMold->SubMold.Step = 300;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.SubMoldClsDelay = pMold->SubMold.Timer.ET;
			if (1 == pMold->SubMold.Timer.Q)
			{
				pMold->SubMold.Timer.IN        = 0;
				//	pMold->TimeDis.SubMoldClsDelay = 0;
				pMold->SubMold.Step = 300;
			}
			break;

		case 300:
			if(1 == pMold->Option.SubMold)
			{
				pMold->SubMold.LimitTimer.IN = 1;
				pMold->SubMold.LimitTimer.PT = pMold->TimeSet.SubMoldClsAlarmTime;
				pMold->SubMold.Step = 400;
			}
			else
			{
				if(1 == pMold->TransDIn.SubMoldClsLimit)
				{		
					pMold->SubMold.Step = 2900; // for no use
				}
				else
				{
					pMold->Alarm.SubMoldNotAtClsPos = 1;
					pMold->SubMold.Step = 40000;           /*  Submold close alarm   */
				}
			}
			
			break;

		case 400:

			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 1;

			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 1;
			
			if( 1 == gMachineInfo.MachineReset)  // Reset
			{
				pMold->SubMold.p_set = pSubMoldPara->P_Reset;
				pMold->SubMold.v_set = pSubMoldPara->V_Reset;
			}
			else
			{
				pMold->SubMold.p_set = pSubMoldPara->P_Cls;
				pMold->SubMold.v_set = pSubMoldPara->V_Cls;
			}
			pMold->TimeDis.SubMoldClsAlarmTime = pMold->SubMold.LimitTimer.ET;
   
			if(1 == pMold->TransDIn.SubMoldClsLimit)
			{		
				pMold->SubMold.Step = 500;
			}
   
			break;


		case 500:   // valve cls delay
			pMold->SubMold.Timer.IN = 1;
			pMold->SubMold.Timer.PT = pMold->TimeSet.SubMoldClsHoldTime;
			pMold->SubMold.Step = 460;
			
			break;
		
		case 510:
			pMold->TimeDis.SubMoldClsHoldTime = pMold->SubMold.Timer.ET;
			pMold->TimeDis.SubMoldClsAlarmTime = pMold->SubMold.LimitTimer.ET;
			
			if (1== pMold->SubMold.Timer.Q)
			{
				pMold->SubMold.Timer.IN = 0;
				pMold->SubMold.LimitTimer.IN = 0;
				pMold->SubMold.Step = 600;
			}
		
			break;
		
		case 600:
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;

			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;		
			
			pMold->SubMold.Timer.IN      = 0;
			pMold->SubMold.LimitTimer.IN = 0;

			pMold->SubMold.Step = 3000;
		
			break;
		
		
	
		case 2900: // for no use		

			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;

			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;		
			
			pMold->SubMold.Timer.IN      = 0;
			pMold->SubMold.LimitTimer.IN = 0;

			pMold->SubMold.Step = 3000;
			
			break;

		case 3000:
			break;
		
		
		/*--------------- Calib Cls -----------------*/
		case 5000:
			if(0 == pMold->TransDIn.SubMoldClsLimit)
			{	
				pMold->ActInfo.SubMoldOpn  = 0;
				pMold->ActInfo.SubMoldCls  = 1;
	
				pMold->ValveOut.SubMoldOpn = 0;
				pMold->ValveOut.SubMoldCls = 1;
			
				pMold->SubMold.p_set = pSubMoldFix->pCalibZero;
				pMold->SubMold.v_set = pSubMoldFix->vCalibZero;
			}
			else
			{
				pMold->ActInfo.SubMoldOpn  = 0;
				pMold->ActInfo.SubMoldCls  = 0;
	
				pMold->ValveOut.SubMoldOpn = 0;
				pMold->ValveOut.SubMoldCls = 0;
			
				pMold->SubMold.p_set = 0;
				pMold->SubMold.v_set = 0;
			}
			
			break;

		
		/*------------------ SubMold open -------------------- */
  		
		case 10100: 	/* Open delay*/
			if(1 == pMold->Option.SubMold)
			{
				if(pMold->TimeSet.SubMoldOpnDelay != 0)
				{
					pMold->SubMold.Timer.IN = 1;
					pMold->SubMold.Timer.PT = pMold->TimeSet.SubMoldOpnDelay;
					pMold->SubMold.Step 	= 10200;
				}
				else
				{
					pMold->SubMold.Step = 10300;
				}					
			}
			else
			{
				pMold->SubMold.Step = 10300;
			}
			break;

		case 10200: 	/* delay ok */
			pMold->TimeDis.SubMoldOpnDelay = pMold->SubMold.Timer.ET;
			if (1 == pMold->SubMold.Timer.Q)
			{
				pMold->SubMold.Timer.IN       = 0;
				//	pMold->TimeDis.SubMoldOpnDelay = 0;
				pMold->SubMold.Step = 10300;
			}
			break;

		case 10300:
			if(1 == pMold->Option.SubMold)
			{	
				if(pMold->TimeSet.SubMoldOpnTime != 0)
				{
					pMold->SubMold.LimitTimer.IN = 1;
					pMold->SubMold.LimitTimer.PT = pMold->TimeSet.SubMoldOpnAlarmTime;
					
					pMold->SubMold.Timer.IN = 1;
					pMold->SubMold.Timer.PT = pMold->TimeSet.SubMoldOpnTime;
			
					pMold->SubMold.Step = 10400;
				}
				else
				{
					pMold->SubMold.Step = 10500;
				}
			}
			else
			{
				pMold->SubMold.Step = 12900; // for no use
			}
			
			break;
	
		case 10400:
			
			pMold->ActInfo.SubMoldOpn  = 1;
			pMold->ActInfo.SubMoldCls  = 0;
	
			pMold->ValveOut.SubMoldOpn = 1;
			pMold->ValveOut.SubMoldCls = 0;

			if( 1 == gMachineInfo.MachineReset)  // Reset
			{
				pMold->SubMold.p_set = pSubMoldPara->P_Reset;
				pMold->SubMold.v_set = pSubMoldPara->V_Reset;
			}
			else
			{
				pMold->SubMold.p_set = pSubMoldPara->P_Opn;
				pMold->SubMold.v_set = pSubMoldPara->V_Opn;
			}
			
			pMold->TimeDis.SubMoldOpnAlarmTime = pMold->SubMold.LimitTimer.ET;

			pMold->TimeDis.SubMoldOpnTime = pMold->SubMold.Timer.ET;
			if (1== pMold->SubMold.Timer.Q)
			{
				pMold->SubMold.Timer.IN = 0;
				pMold->SubMold.LimitTimer.IN = 0;
				//	pMold->TimeDis.SubMoldOpnTime =0;
				pMold->SubMold.Step  = 10500;
			}

			break;
		
		case 10500:

			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
			
			pMold->SubMold.Step = 13000;
			break;
		
		case 12900: // for no use

			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
			
			pMold->SubMold.Step = 13000;
			
			break;
   
		case 13000:
			break;
		
		
		/*--------------- Calib Open -----------------*/
		case 15000:
			
			pMold->ActInfo.SubMoldOpn  = 1;
			pMold->ActInfo.SubMoldCls  = 0;
		
			pMold->ValveOut.SubMoldOpn = 1;
			pMold->ValveOut.SubMoldCls = 0;
				
			pMold->SubMold.p_set = pSubMoldFix->pCalibMax;
			pMold->SubMold.v_set = pSubMoldFix->vCalibMax;
			
			break;


		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
   
			pMold->SubMold.Step = 0;
			break;

		/*--------------- Instancy stop -----------------*/
		case 30000:
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
   
			pMold->SubMold.Step = 0;
			break;

		/*--------------- Auto error stop -----------------*/
		case 40000:
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
			
			break;
		
		case 41000:
			pMold->ActInfo.SubMoldOpn  = 0;
			pMold->ActInfo.SubMoldCls  = 0;
   
			pMold->ValveOut.SubMoldOpn = 0;
			pMold->ValveOut.SubMoldCls = 0;
			
			pMold->SubMold.p_set = 0;
			pMold->SubMold.v_set = 0;
			
			pMold->SubMold.Timer.IN = 0;
			pMold->SubMold.LimitTimer.IN = 0;
			
			pMold->SubMold.Step = 41000;
			
			break;
	}/* switch ( pMold->SubMold.Step )  */
	
	
	
	if(1 == pMold->SubMold.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->SubMold.Step < 3000 && pMold->SubMold.Step > 300)		/* SubMold close  */
		{
			pMold->Alarm.SubMoldNotAtClsPos = 1;
		}
      
		if(pMold->SubMold.Step < 13000 && pMold->SubMold.Step > 10300)	/* SubMold open  */
		{
			pMold->Alarm.SubMoldNotAtOpnPos = 1;
		}
      
		pMold->SubMold.LimitTimer.IN = 0;
		pMold->SubMold.Step = 40000;
	}
	
	
	pMold->SubMold.aoPropValve = pMold->SubMold.v_set * 32767/100.0;
}


void BottomSubMold( Mold_typ * pMold,SubMold_Para_typ * pBottomSubMoldPara,SubMold_Fix_typ * pBottomSubMoldFix,Clamp_Para_typ * pClampPara)
{
	
	if(pMold == & LMold)
	{
		pBottomSubMoldFix->pMax = gMachineFix.Pump.MaxP2;
	}
	else
	{
		pBottomSubMoldFix->pMax = gMachineFix.Pump.MaxP1;
	}
	
	if(0 == pBottomSubMoldPara->V_Reset)pBottomSubMoldPara->V_Reset = 5;
	if(0 == pBottomSubMoldPara->P_Reset)pBottomSubMoldPara->P_Reset = 5;
	
	switch ( pMold->BottomSubMold.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;

			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.Timer.IN	= 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
			
			break;

		/*--------------- BottomSubMold close -----------------*/
		case 100: 	/* close delay*/
			if(1 == pMold->Option.BottomSubMold)
			{
				pMold->BottomSubMold.Timer.IN = 1;
				pMold->BottomSubMold.Timer.PT = pMold->TimeSet.BottomSubMoldClsDelay;
				pMold->BottomSubMold.Step = 200;
			}
			else
			{
				pMold->BottomSubMold.Step = 300;
			}
			break;

		case 200: 	/* delay ok */
			pMold->TimeDis.BottomSubMoldClsDelay = pMold->BottomSubMold.Timer.ET;
			if (1 == pMold->BottomSubMold.Timer.Q)
			{
				pMold->BottomSubMold.Timer.IN        = 0;
				//	pMold->TimeDis.BottomSubMoldClsDelay = 0;
				pMold->BottomSubMold.Step = 300;
			}
			break;

		case 300:
			if(1 == pMold->Option.BottomSubMold)
			{
				pMold->BottomSubMold.LimitTimer.IN = 1;
				pMold->BottomSubMold.LimitTimer.PT = pMold->TimeSet.BottomSubMoldClsAlarmTime;
				pMold->BottomSubMold.Step = 400;
			}
			else
			{
				if(1 == pMold->TransDIn.BottomSubMoldClsLimit)
				{		
					pMold->BottomSubMold.Step = 2900; // for no use
				}
				else
				{
					pMold->Alarm.BottomSubMoldNotAtClsPos = 1;
					pMold->BottomSubMold.Step = 40000;           /*  Submold close alarm   */
				}
			}
			
			break;

		case 400:

			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 1;

			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 1;
			
			if( 1 == gMachineInfo.MachineReset)  // Reset
			{
				pMold->BottomSubMold.p_set = pBottomSubMoldPara->P_Reset;
				pMold->BottomSubMold.v_set = pBottomSubMoldPara->V_Reset;
			}
			else
			{
				pMold->BottomSubMold.p_set = pBottomSubMoldPara->P_Cls;
				pMold->BottomSubMold.v_set = pBottomSubMoldPara->V_Cls;
			}
			pMold->TimeDis.BottomSubMoldClsAlarmTime = pMold->BottomSubMold.LimitTimer.ET;
   
			if(1 == pMold->TransDIn.BottomSubMoldClsLimit)
			{		
				pMold->BottomSubMold.Step = 500;
			}
   
			break;


		case 500:   // valve cls delay
			pMold->BottomSubMold.Timer.IN = 1;
			pMold->BottomSubMold.Timer.PT = pMold->TimeSet.BottomSubMoldClsHoldTime;
			pMold->BottomSubMold.Step = 460;
			
			break;
		
		case 510:
			pMold->TimeDis.BottomSubMoldClsHoldTime = pMold->BottomSubMold.Timer.ET;
			pMold->TimeDis.BottomSubMoldClsAlarmTime = pMold->BottomSubMold.LimitTimer.ET;
			
			if (1== pMold->BottomSubMold.Timer.Q)
			{
				pMold->BottomSubMold.Timer.IN = 0;
				pMold->BottomSubMold.LimitTimer.IN = 0;
				pMold->BottomSubMold.Step = 600;
			}
		
			break;
		
		case 600:
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;

			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;		
			
			pMold->BottomSubMold.Timer.IN      = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;

			pMold->BottomSubMold.Step = 3000;
		
			break;
		
		
	
		case 2900: // for no use		

			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;

			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;		
			
			pMold->BottomSubMold.Timer.IN      = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;

			pMold->BottomSubMold.Step = 3000;
			
			break;

		case 3000:
			break;
		
		
		/*--------------- Calib Cls -----------------*/
		case 5000:
			if(0 == pMold->TransDIn.BottomSubMoldClsLimit)
			{	
				pMold->ActInfo.BottomSubMoldOpn  = 0;
				pMold->ActInfo.BottomSubMoldCls  = 1;
	
				pMold->ValveOut.BottomSubMoldOpn = 0;
				pMold->ValveOut.BottomSubMoldCls = 1;
			
				pMold->BottomSubMold.p_set = pBottomSubMoldFix->pCalibZero;
				pMold->BottomSubMold.v_set = pBottomSubMoldFix->vCalibZero;
			}
			else
			{
				pMold->ActInfo.BottomSubMoldOpn  = 0;
				pMold->ActInfo.BottomSubMoldCls  = 0;
	
				pMold->ValveOut.BottomSubMoldOpn = 0;
				pMold->ValveOut.BottomSubMoldCls = 0;
			
				pMold->BottomSubMold.p_set = 0;
				pMold->BottomSubMold.v_set = 0;
			}
			
			break;

		
		/*------------------ BottomSubMold open -------------------- */
  		
		case 10100: 	/* Open delay*/
			if(1 == pMold->Option.BottomSubMold)
			{
				if(pMold->TimeSet.BottomSubMoldOpnDelay != 0)
				{
					pMold->BottomSubMold.Timer.IN = 1;
					pMold->BottomSubMold.Timer.PT = pMold->TimeSet.BottomSubMoldOpnDelay;
					pMold->BottomSubMold.Step 	= 10200;
				}
				else
				{
					pMold->BottomSubMold.Step = 10300;
				}					
			}
			else
			{
				pMold->BottomSubMold.Step = 10300;
			}
			break;

		case 10200: 	/* delay ok */
			pMold->TimeDis.BottomSubMoldOpnDelay = pMold->BottomSubMold.Timer.ET;
			if (1 == pMold->BottomSubMold.Timer.Q)
			{
				pMold->BottomSubMold.Timer.IN       = 0;
				//	pMold->TimeDis.BottomSubMoldOpnDelay = 0;
				pMold->BottomSubMold.Step = 10300;
			}
			break;

		case 10300:
			if(1 == pMold->Option.BottomSubMold)
			{	
				if(pMold->TimeSet.BottomSubMoldOpnTime != 0)
				{
					pMold->BottomSubMold.LimitTimer.IN = 1;
					pMold->BottomSubMold.LimitTimer.PT = pMold->TimeSet.BottomSubMoldOpnAlarmTime;
					
					pMold->BottomSubMold.Timer.IN = 1;
					pMold->BottomSubMold.Timer.PT = pMold->TimeSet.BottomSubMoldOpnTime;
			
					pMold->BottomSubMold.Step = 10400;
				}
				else
				{
					pMold->BottomSubMold.Step = 10500;
				}
			}
			else
			{
				pMold->BottomSubMold.Step = 12900; // for no use
			}
			
			break;
	
		case 10400:
			
			pMold->ActInfo.BottomSubMoldOpn  = 1;
			pMold->ActInfo.BottomSubMoldCls  = 0;
	
			pMold->ValveOut.BottomSubMoldOpn = 1;
			pMold->ValveOut.BottomSubMoldCls = 0;

			if( 1 == gMachineInfo.MachineReset)  // Reset
			{
				pMold->BottomSubMold.p_set = pBottomSubMoldPara->P_Reset;
				pMold->BottomSubMold.v_set = pBottomSubMoldPara->V_Reset;
			}
			else
			{
				pMold->BottomSubMold.p_set = pBottomSubMoldPara->P_Opn;
				pMold->BottomSubMold.v_set = pBottomSubMoldPara->V_Opn;
			}
			
			pMold->TimeDis.BottomSubMoldOpnAlarmTime = pMold->BottomSubMold.LimitTimer.ET;

			pMold->TimeDis.BottomSubMoldOpnTime = pMold->BottomSubMold.Timer.ET;
			if (1== pMold->BottomSubMold.Timer.Q)
			{
				pMold->BottomSubMold.Timer.IN = 0;
				pMold->BottomSubMold.LimitTimer.IN = 0;
				//	pMold->TimeDis.BottomSubMoldOpnTime =0;
				pMold->BottomSubMold.Step  = 10500;
			}

			break;
		
		case 10500:

			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
			
			pMold->BottomSubMold.Step = 13000;
			break;
		
		case 12900: // for no use

			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
			
			pMold->BottomSubMold.Step = 13000;
			
			break;
   
		case 13000:
			break;
		
		
		/*--------------- Calib Open -----------------*/
		case 15000:
			
			pMold->ActInfo.BottomSubMoldOpn  = 1;
			pMold->ActInfo.BottomSubMoldCls  = 0;
		
			pMold->ValveOut.BottomSubMoldOpn = 1;
			pMold->ValveOut.BottomSubMoldCls = 0;
				
			pMold->BottomSubMold.p_set = pBottomSubMoldFix->pCalibMax;
			pMold->BottomSubMold.v_set = pBottomSubMoldFix->vCalibMax;
			
			break;


		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
   
			pMold->BottomSubMold.Step = 0;
			break;

		/*--------------- Instancy stop -----------------*/
		case 30000:
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
   
			pMold->BottomSubMold.Step = 0;
			break;

		/*--------------- Auto error stop -----------------*/
		case 40000:
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
			
			break;
		
		case 41000:
			pMold->ActInfo.BottomSubMoldOpn  = 0;
			pMold->ActInfo.BottomSubMoldCls  = 0;
   
			pMold->ValveOut.BottomSubMoldOpn = 0;
			pMold->ValveOut.BottomSubMoldCls = 0;
			
			pMold->BottomSubMold.p_set = 0;
			pMold->BottomSubMold.v_set = 0;
			
			pMold->BottomSubMold.Timer.IN = 0;
			pMold->BottomSubMold.LimitTimer.IN = 0;
			
			pMold->BottomSubMold.Step = 41000;
			
			break;
	}/* switch ( pMold->BottomSubMold.Step )  */
	
	
	
	if(1 == pMold->BottomSubMold.LimitTimer.Q && 1 == gMachineInfo.TimeLimit)
	{
		if(pMold->BottomSubMold.Step < 3000 && pMold->BottomSubMold.Step > 300)		/* BottomSubMold close  */
		{
			pMold->Alarm.BottomSubMoldNotAtClsPos = 1;
		}
      
		if(pMold->BottomSubMold.Step < 13000 && pMold->BottomSubMold.Step > 10300)	/* BottomSubMold open  */
		{
			pMold->Alarm.BottomSubMoldNotAtOpnPos = 1;
		}
      
		pMold->BottomSubMold.LimitTimer.IN = 0;
		pMold->BottomSubMold.Step = 40000;
	}
	
	
	pMold->BottomSubMold.aoPropValve = pMold->BottomSubMold.v_set * 32767/100.0;
}