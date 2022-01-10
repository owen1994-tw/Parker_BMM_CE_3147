/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: clean
 * File: clean.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program clean --- clean proportional  valve
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "defines.h"
#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void CleanValveCtrl (CleanValve_typ * pCleanValveFix,CleanCtrl_typ * pCtrl, timer_typ *tValveCleanTotal,timer_typ *tValveClean,UINT *pMold_Step);

/**************************************************************************************
     	        		         I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT cleanvlave_init(void)
{
	RClampCleanValveCtrl.Step = 3000;
	RCarriageCleanValveCtrl.Step = 3000;
}  /* end of _INIT */
/**************************************************************************************
****				         C Y C L I C   P A R T   O F   T H E   T A S K  				 
**************************************************************************************/
void _CYCLIC cleanvlave_cyclic(void)
{
	/*----------------------------------*/
	/*-------------Clamp----------------*/
	/*----------------------------------*/
	if(VALVE_PROP == gMachineFix.MoldR.Clamp.eValveType)
	{
		if(RClampCleanValveCtrl.Step >= 10 && RClampCleanValveCtrl.Step <= 3000)
		{
			//Valve Out
			RMold.ValveOut.ClampOpn 		= RClampCleanValveCtrl.ValveOut.ValveFw1; 
			RMold.ValveOut.ClampOpnRelease 	= RClampCleanValveCtrl.ValveOut.ValveFw2;
		
			RMold.ValveOut.ClampCls 	= RClampCleanValveCtrl.ValveOut.ValveBw1; 
			//Pressure
			RMold.Clamp.p_set = RClampCleanValveCtrl.p_set;
		}
		if(LClampCleanValveCtrl.Step >= 10 && LClampCleanValveCtrl.Step <= 3000)
		{
			//Valve Out
			LMold.ValveOut.ClampOpn 		= LClampCleanValveCtrl.ValveOut.ValveFw1; 
			LMold.ValveOut.ClampOpnRelease 	= LClampCleanValveCtrl.ValveOut.ValveFw2;
		
			LMold.ValveOut.ClampCls 	= LClampCleanValveCtrl.ValveOut.ValveBw1; 
			//Pressure
			LMold.Clamp.p_set = LClampCleanValveCtrl.p_set;
		}
		CleanValveCtrl(&gMachineFix.CleanValve.MoldR.Clamp,&RClampCleanValveCtrl,&gMacTimer.RClampValveCleanTotal,&gMacTimer.RClampValveClean,&RMold.Clamp.Step);
		CleanValveCtrl(&gMachineFix.CleanValve.MoldL.Clamp,&LClampCleanValveCtrl,&gMacTimer.LClampValveCleanTotal,&gMacTimer.LClampValveClean,&LMold.Clamp.Step);
	}
	
	/*----------------------------------*/
	/*-------------Carriage-------------*/
	/*----------------------------------*/
	if(VALVE_PROP == gMachineFix.MoldR.Carriage.eValveType)
	{
		if(RCarriageCleanValveCtrl.Step >= 10 && RCarriageCleanValveCtrl.Step <= 3000)
		{
			//Valve Out
			RMold.ValveOut.CarriageFw = RCarriageCleanValveCtrl.ValveOut.ValveFw1; 
			RMold.ValveOut.CarriageBw = RCarriageCleanValveCtrl.ValveOut.ValveBw1; 
			//Pressure
			RMold.Carriage.p_set = RCarriageCleanValveCtrl.p_set;
		}
		if(LCarriageCleanValveCtrl.Step >= 10 && LCarriageCleanValveCtrl.Step <= 3000)
		{
			//Valve Out
			LMold.ValveOut.CarriageFw = LCarriageCleanValveCtrl.ValveOut.ValveFw1; 
			LMold.ValveOut.CarriageBw = LCarriageCleanValveCtrl.ValveOut.ValveBw1; 
			//Pressure
			LMold.Carriage.p_set = LCarriageCleanValveCtrl.p_set;
		}
		CleanValveCtrl(&gMachineFix.CleanValve.MoldR.Carriage,&RCarriageCleanValveCtrl,&gMacTimer.RCarriageValveCleanTotal,&gMacTimer.RCarriageValveClean,&RMold.Carriage.Step);
		CleanValveCtrl(&gMachineFix.CleanValve.MoldL.Carriage,&LCarriageCleanValveCtrl,&gMacTimer.LCarriageValveCleanTotal,&gMacTimer.LCarriageValveClean,&LMold.Carriage.Step);
	}
	
	/*----------------------------------*/
	/*-------------Blowpin--------------*/
	/*----------------------------------*/
//	if(RBlowPinCleanValveCtrl.Step >= 10 && RBlowPinCleanValveCtrl.Step <= 3000)
//	{
//		//Valve Out
//		RMold.ValveOut.BlowPinDn = RBlowPinCleanValveCtrl.ValveOut.ValveFw1; 
//		RMold.ValveOut.BlowPinUp = RBlowPinCleanValveCtrl.ValveOut.ValveBw1; 
//		//Pressure
//		RMold.BlowPin.p_set = RBlowPinCleanValveCtrl.p_set;
//	}
//	if(LBlowPinCleanValveCtrl.Step >= 10 && LBlowPinCleanValveCtrl.Step <= 3000)
//	{
//		//Valve Out
//		LMold.ValveOut.BlowPinDn = LBlowPinCleanValveCtrl.ValveOut.ValveFw1; 
//		LMold.ValveOut.BlowPinUp = LBlowPinCleanValveCtrl.ValveOut.ValveBw1; 
//		//Pressure
//		LMold.BlowPin.p_set = LBlowPinCleanValveCtrl.p_set;
//	}
//	CleanValveCtrl(&gMachineFix.CleanValve.MoldR.BlowPin,&RBlowPinCleanValveCtrl,&gMacTimer.RBlowPinValveCleanTotal,&gMacTimer.RBlowPinValveClean,&RMold.BlowPin.Step);
//	CleanValveCtrl(&gMachineFix.CleanValve.MoldL.BlowPin,&LBlowPinCleanValveCtrl,&gMacTimer.LBlowPinValveCleanTotal,&gMacTimer.LBlowPinValveClean,&LMold.BlowPin.Step);
	
	
}  /* end of _CYCLIC */

void CleanValveCtrl(CleanValve_typ * pCleanValveFix,CleanCtrl_typ * pCtrl, timer_typ *tValveCleanTotal,timer_typ *tValveClean,UINT *pMold_Step)
{

		if( 1 == pCtrl->Enable && 0 == pCtrl->Enable)
		{
			if(PIC_OEM_SETTING_10 == gPanelObj.GetPicture)
			{
				pCtrl->Enable = 1;
			}
		}
		/* Start clean valve  */
		if(gPanelObj.PasswordLevel > 2 && 1 == gTKcalib.Status)
		{
			if(1 == pCtrl->Enable)
			{
				pCtrl->Enable = 0;
	         
				if(pCtrl->Step != 0)
				{
					pCtrl->Step = 3000;
					*pMold_Step = 0;	
				}
				else
				{
					pCtrl->Step = 10;
					*pMold_Step = 5;  /*Clean Valve Step*/
				}
			}
		}/* if(gPanelObj.PasswordLevel > 3) */

		pCtrl->oldEnable  =  pCtrl->Enable;
	
	
	/*  step machine clear valve */
	switch(pCtrl->Step)
	{
		case 0: 	// Off

			
			break;
		
		case 10: 	// total time start
			tValveCleanTotal->IN = 1;
			tValveCleanTotal->PT = pCleanValveFix->CleanValveTotalTime*60;
			pCtrl->Step = 100;
			break;
      
		case 100:	//Fw Timer Setting
			tValveClean->IN = 1;
			tValveClean->PT = pCleanValveFix->CleanValveFwTime;
			pCtrl->Step = 200;
			break;
      
		case 200:	//Fw 
			pCtrl->ValveOut.ValveFw1 = 1;
			pCtrl->ValveOut.ValveFw2 = 1;
			
			pCtrl->p_set = pCtrl->Para.P_FwClean;
			pCtrl->TimeDis.CleanValveFwDis = tValveClean->ET;
			if(1 == tValveClean->Q)
			{
				pCtrl->TimeDis.CleanValveFwDis = 0;
				tValveClean->IN = 0;
				pCtrl->Step = 300;
			}
			break;
      
		case 300:	//Fw End
			pCtrl->ValveOut.ValveFw1 = 0;
			pCtrl->ValveOut.ValveFw2 = 0;
			
			pCtrl->p_set = 0;
			
			pCtrl->Step = 400;
			break;
      
		case 400:	//Interval Timer Setting
			tValveClean->IN = 1;
			tValveClean->PT = pCleanValveFix->CleanValveInterval;
			pCtrl->Step = 500;
			break;
      
		case 500:	//Interval
			pCtrl->TimeDis.CleanValveIntervalDis = tValveClean->ET;
			if(1 == tValveClean->Q)
			{
				pCtrl->TimeDis.CleanValveIntervalDis = 0;
				tValveClean->IN = 0;
				pCtrl->Step = 600;
			}
			
			break;
      
		case 600:
			pCtrl->Step = 700;
			break;
      
		case 700:	//Bw Timer Setting
			tValveClean->IN = 1;
			tValveClean->PT = pCleanValveFix->CleanValveBwTime;
			pCtrl->Step = 800;
			break;
      
		case 800:	//Bw
			pCtrl->ValveOut.ValveBw1 = 1;
			pCtrl->ValveOut.ValveBw2 = 1;
			
			pCtrl->p_set = pCtrl->Para.P_BwClean;
			pCtrl->TimeDis.CleanValveBwDis = tValveClean->ET;
			if(1 == tValveClean->Q)
			{
				pCtrl->TimeDis.CleanValveBwDis = 0;
				tValveClean->IN = 0;
				pCtrl->Step = 900;
			}
			break;
      
		case 900:	//Bw End
			pCtrl->ValveOut.ValveBw1 = 0;
			pCtrl->ValveOut.ValveBw2 = 0;
			
			pCtrl->p_set = 0;
			RClampCleanValveCtrl.p_set = 0;
			pCtrl->Step = 1000;
			break;
      
		case 1000:	//Inerval Timer Setting
			tValveClean->IN = 1;
			tValveClean->PT = pCleanValveFix->CleanValveInterval;
			pCtrl->Step = 1100;
			break;
      
		case 1100:	//Interval
			pCtrl->TimeDis.CleanValveIntervalDis = tValveClean->ET;
			if(1 == tValveClean->Q)
			{
				pCtrl->TimeDis.CleanValveIntervalDis = 0;
				tValveClean->IN = 0;
				pCtrl->Step = 1200;
			}
			break;
      
		case 1200:
			pCtrl->Step = 100;
			break;
      
		case 3000:
			tValveClean->IN      = 0;
			tValveCleanTotal->IN = 0;
			
			pCtrl->p_set = 0;
			pCtrl->ValveOut.ValveFw1 = 0;
			pCtrl->ValveOut.ValveFw2 = 0;
			
			pCtrl->ValveOut.ValveBw1 = 0;
			pCtrl->ValveOut.ValveBw2 = 0;
			
			pCtrl->Step = 0;
			break;
	}/* switch(pCtrl->Step) */

	pCtrl->TimeDis.CleanValveTotalDis = tValveCleanTotal->ET/60;

	if(1 == tValveCleanTotal->Q)
	{
		pCtrl->Step = 3000;
	}
	
	
}
