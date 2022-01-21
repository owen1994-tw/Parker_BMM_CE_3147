/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: reset
 * File: reset.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program reset --- machine reset control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void MoldReset( Mold_typ * pMold);
void MoldReset_Hybrid(Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.ResetStep = 0;
	RMold.ResetStep = 0;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	if(LMold.TimeSet.ResetAlarmTime < 1)LMold.TimeSet.ResetAlarmTime = 2000;
	if(RMold.TimeSet.ResetAlarmTime < 1)RMold.TimeSet.ResetAlarmTime = 2000;
	
	MoldReset_Hybrid(&RMold);
	MoldReset_Hybrid(&LMold);


} /* end of _CYCLIC */

void MoldReset_Hybrid( Mold_typ * pMold)
{
	if(0 == gMachineIn.SafeGate)
	{
		if(pMold->ResetStep > 100 && pMold->ResetStep < 3000 )
		{
			pMold->ResetStep = 20000;
		}
	}

	switch (pMold->ResetStep)
	{				
		case 100:/*Blow Pin Up, extruderlift down*/
			pMold->TimeCycle.IN   = 1;
			pMold->TimeCycle.PT   = pMold->TimeSet.ResetAlarmTime;	/*  限时  */
			pMold->ValveOut.ProductAuxClamp =0;
						
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				RMold.ExtrLift.Step = 10300;/*Exturderlif down*/
			}
			else
			{
				pMold->ExtrLift.Step = 10300;/*Exturderlif down*/
			}
			
			if(pMold->Carriage.BwPos                 
				&& pMold->Clamp.OpnPos              
				&& pMold->TransDIn.CoolPinUpLimit && pMold->TransDIn.CoolPin2UpLimit && pMold->TransDIn.CoolPin3UpLimit  
				&& pMold->TransDIn.TopDeflashBwLimit && pMold->TransDIn.BottomDeflashBwLimit 
				&& pMold->TransDIn.TopDeflashOpnLimit
				&& pMold->TransDIn.RobotBwLimit && pMold->TransDIn.RobotUpLimit 
				&& pMold->TransDIn.RobotOpnLimit
				&& pMold->TransDIn.SubMoldClsLimit && pMold->TransDIn.BottomSubMoldClsLimit   
				&& pMold->TransDIn.HookPinBwLimit
				//		&& pMold->TransDIn.PinPartFwLimit 
				//	    && pMold->TransDIn.RobotOpnLimit     
				//	    && pMold->TransDIn.CutNeckOpnLimit      
				//		&& pMold->TransDIn.SealOpnLimit                
				&& pMold->Robot.Transfer.BwPos 
				&& (pMold->Robot.Transfer.CalOK || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eValveType)    
				&& pMold->Robot.UpDn.UpPos              
				&& (pMold->Robot.UpDn.CalOK || ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotUpDn.eValveType) 
				&& pMold->TransDIn.RobotOpnLimit
				//		&& pMold->TransDIn.RobotOpnLimitL 
				//		&& pMold->TransDIn.RobotOpnLimitR 
				) 
			{
				pMold->ResetStep = 1290;  /* jump reset process*/
			}
			else if(0 == pMold->BlowPin.UpPos || 0 == pMold->BlowPin2.UpPos)
			{
				pMold->BlowPin.Step = 10300;    /*  吹针上    */
				pMold->BlowPin2.Step = 10300;    /*  吹针上    */
			
				pMold->ResetStep = 200;
			}
			else 
			{
				pMold->ResetStep = 210;
			}
			break;

		case 200:/*wait blow pin up finished*/
			if( 13000 == pMold->BlowPin.Step && 13000 == pMold->BlowPin2.Step )	/*  吹针上   */
			{
				pMold->BlowPin.Step = 0;
				pMold->BlowPin2.Step = 0;
				pMold->ResetStep = 210;
			}

			if(40000 == pMold->BlowPin.Step)
			{
				pMold->BlowPin.Step = 0;
				pMold->ResetStep = 40000;
			}
			
			if(40000 == pMold->BlowPin2.Step)
			{
				pMold->BlowPin2.Step = 0;
				pMold->ResetStep = 40000;
			}
			
			break;

		case 210:
		
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if (13000==pMold->ExtrLift.Step)
				{
					RMold.ExtrLift.Step =0;
					pMold->ResetStep = 250;
				}	
			
				if (40000==pMold->ExtrLift.Step)
				{
					RMold.ExtrLift.Step =0;
					pMold->ResetStep = 40000;
				}
			}
			else
			{
				if (13000==pMold->ExtrLift.Step)
				{
					pMold->ExtrLift.Step =0;
					pMold->ResetStep = 250;
				}	
			
				if (40000==pMold->ExtrLift.Step)
				{
					pMold->ExtrLift.Step =0;
					pMold->ResetStep = 40000;
				}
			}				
			break;
		
		
		case 250:/*TopDeflash open (handle backward)  打手挽退*/ 
			if(0 == pMold->TransDIn.TopDeflashOpnLimit)
			{		
				pMold->TopDeflashOpnCls.Step = 10100;
				pMold->ResetStep = 260;
			}
			else
			{
				pMold->ResetStep = 300;	
			}	
			
			break;
		
		case 260: /*wait topdeflash open finish*/
			if(13000 == pMold->TopDeflashOpnCls.Step)
			{
				pMold->TopDeflashOpnCls.Step = 0;
				pMold->ResetStep = 300;
			}
			
			if(40000 == pMold->TopDeflashOpnCls.Step)
			{
				pMold->TopDeflashOpnCls.Step = 0;
				pMold->ResetStep = 40000;
			}					
		
			break;
					
		case 300:/*TopDeflash Backward*/
			if(0 == pMold->TransDIn.TopDeflashBwLimit)
			{
				pMold->TopDeflash.Step = 10100;
				pMold->ResetStep = 310;
			}
			else
			{
				pMold->ResetStep = 400;
			}
			break;

		case 310:/*wait topdeflash backward finish*/
			if(13000 == pMold->TopDeflash.Step)
			{
				pMold->TopDeflash.Step = 0;
				pMold->ResetStep = 400;
			}
			
			if(40000 == pMold->TopDeflash.Step)
			{
				pMold->TopDeflash.Step = 0;
				pMold->ResetStep = 40000;
			}
			break;
		
		case 400:/*BottomDeflash Backward*/
			if(0 == pMold->TransDIn.BottomDeflashBwLimit)
			{
				pMold->BottomDeflash.Step = 10100;
				pMold->ResetStep = 410;
			}
			else
			{
				pMold->ResetStep = 500;
			}
			break;

		case 410:/*wait BottomDeflash backward finish*/
			if(13000 == pMold->BottomDeflash.Step)
			{
				pMold->BottomDeflash.Step = 0;
				pMold->ResetStep = 500;
			}
			
			if(40000 == pMold->BottomDeflash.Step)
			{
				pMold->BottomDeflash.Step = 0;
				pMold->ResetStep = 40000;
			}
			break;
		
		case 500:/*CoolPin UP*/
			pMold->CoolPin.Step = 500;					
			pMold->CoolPin2.Step = 500;	
			pMold->CoolPin3.Step = 500;	
									
			pMold->ResetStep = 510;						
			break;

		case 510:/*wait CoolPin UP finish*/
			if(3000 == pMold->CoolPin.Step && 3000 == pMold->CoolPin2.Step && 3000 == pMold->CoolPin3.Step )
			{
				pMold->CoolPin.Step = 0;
				pMold->CoolPin2.Step = 0;
				pMold->CoolPin3.Step = 0;
				pMold->ResetStep = 600;
			}
			
			if(40000 == pMold->CoolPin.Step || 40000 == pMold->CoolPin2.Step || 40000 == pMold->CoolPin3.Step)
			{
				pMold->CoolPin.Step = 0;
				pMold->CoolPin2.Step = 0;
				pMold->CoolPin3.Step = 0;
				pMold->ResetStep = 40000;
			}
			
			break;
		
		case 600:/*HookPin Backward*/
			if(0 == pMold->TransDIn.HookPinBwLimit)
			{
				pMold->HookPin.Step = 1300;
				pMold->ResetStep = 610;
			}
			else
			{
				pMold->ResetStep = 700;
			}
			break;

		case 610:/*wait HookPin backward finish*/
			if(3000 == pMold->HookPin.Step)
			{
				pMold->BottomDeflash.Step = 0;
				pMold->ResetStep = 700;
			}
			
			if(40000 == pMold->HookPin.Step)
			{
				pMold->HookPin.Step = 0;
				pMold->ResetStep = 40000;
			}
			break;
		
		case 700:/*Clamp UnLock*/
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Clamp.Step = 12100;		/*UnLock Process  開模洩壓 */
				pMold->ResetStep  = 710;
			}
			else
			{

				pMold->ResetStep  = 800;
			}
			break;
		
		case 710:/*wait UnLock finish*/
			if(13000 == pMold->Clamp.Step)
			{
				pMold->Clamp.Step = 0;
				pMold->ResetStep  = 750;
			}

			if(40000 == pMold->Clamp.Step)
			{
				pMold->Clamp.Step = 0;
				pMold->ResetStep  = 40000;
			}
			break;	
		
		case 750:/*Clamp Open*/
			if(0 == pMold->Clamp.OpnPos)
			{
				pMold->Clamp.Step = 10300;		/*Clamp Open*/
				pMold->ResetStep  = 760;
			}
			else
			{
				pMold->ResetStep  = 900;
			}
			break;
		
		case 760:/*wait clamp open finish*/
			if(13000 == pMold->Clamp.Step)
			{
				pMold->Clamp.Step = 0;
				pMold->ResetStep  = 800;
			}

			if(40000 == pMold->Clamp.Step)
			{
				pMold->Clamp.Step = 0;
				pMold->ResetStep  = 40000;
			}
			break;		
		
		case 800:  // Submold cls

			pMold->SubMold.Step  = 300;
			pMold->BottomSubMold.Step  = 300;
			pMold->ResetStep  = 810;

			break;
		
		case 810:
			if(3000 == pMold->SubMold.Step && 3000 == pMold->BottomSubMold.Step )
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step =0;
				pMold->ResetStep  = 900;
			}

			if(40000 == pMold->SubMold.Step || 40000==pMold->BottomSubMold.Step)
			{
				pMold->SubMold.Step = 0;
				pMold->BottomSubMold.Step =0;
				pMold->ResetStep  = 40000;
			}
			break;
		
			
		case 900: // robot bw
			if(0 == pMold->Robot.Transfer.BwPos)
			{
				pMold->Robot.Step = 10300;
				pMold->ResetStep = 910;
			}
			else
			{
				pMold->ResetStep = 1000;
			}
			break;
		
		case 910:
			if(13000 == pMold->Robot.Step)
			{
				pMold->Robot.Step = 0;
				pMold->ResetStep  = 1000;
			}

			if(40000 == pMold->Robot.Step)
			{
				pMold->Robot.Step = 0;
				pMold->ResetStep  = 40000;
			}			
			break;
		
		case 1000: // robot opn 
			if(0 == pMold->TransDIn.RobotOpnLimit)
			{
				pMold->RobotOpnCls.Step = 10300;
				pMold->ResetStep = 1010;
			}
			else
			{
				pMold->ResetStep = 1100;
			}
			break;
			
		case 1010:
			if(13000 == pMold->RobotOpnCls.Step)
			{
				pMold->RobotOpnCls.Step = 0;
				pMold->ResetStep  = 1100;
			}

			if(40000 == pMold->RobotOpnCls.Step)
			{
				pMold->RobotOpnCls.Step = 0;
				pMold->ResetStep  = 40000;
			}			
			break;
				
		
		case 1100:/*Carriage down backward*/
			if(0 == pMold->Carriage.BwPos)
			{
				pMold->Carriage.Step = 10300;
				pMold->ResetStep = 1110;
			}
			else
			{
				pMold->ResetStep = 1300;
			}
			break;
		
		case 1110:/*wait carriage down finish*/
			if(13000 == pMold->Carriage.Step)
			{
				pMold->Carriage.Step = 0;
				pMold->ResetStep = 1300;
			}

			if(40000 == pMold->Carriage.Step)
			{
				pMold->Carriage.Step = 0;
				pMold->ResetStep = 40000;
			}
			break;
			
		case 1290: // jump reset process
			
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) 
			{
				if (13000==pMold->ExtrLift.Step)
				{
					RMold.ExtrLift.Step =0;
					pMold->ResetStep = 1300;
				}	
			
				if (40000==pMold->ExtrLift.Step)
				{
					RMold.ExtrLift.Step =0;
					pMold->ResetStep = 40000;
				}
			}
			else
			{
				if (13000==pMold->ExtrLift.Step)
				{
					pMold->ExtrLift.Step =0;
					pMold->ResetStep = 1300;
				}	
			
				if (40000==pMold->ExtrLift.Step)
				{
					pMold->ExtrLift.Step =0;
					pMold->ResetStep = 40000;
				}
			}	
			
			break;
			
		
		case 1300:/*blowpin down*/
			
			if(0 == pMold->BlowPin.DnPos || 0 == pMold->BlowPin2.DnPos)
			{
				pMold->BlowPin.Step = 300;
				pMold->BlowPin2.Step = 300;
				pMold->ResetStep = 1310;
			}
			else
			{
				pMold->ResetStep = 1400;
			}
		
			

			break;
		
		case 1310:/*wait blowpin down finish*/


			if(3000 == pMold->BlowPin.Step && 3000 == pMold->BlowPin2.Step)
			{
				pMold->BlowPin.Step = 0;
				pMold->BlowPin2.Step = 0;
				pMold->ResetStep = 1400;
			}	

			if(40000 == pMold->BlowPin.Step)
			{
				pMold->BlowPin.Step = 0;
				pMold->ResetStep = 40000;
			}
			
			if(40000 == pMold->BlowPin2.Step)
			{
				pMold->BlowPin2.Step = 0;
				pMold->ResetStep = 40000;
			}
			break;
		
		
		case 1400:

			pMold->TimeCycle.IN = 0;
			pMold->ResetStep = 3000;
			break;

		case 3000:
			break;

		case 20000:
			pMold->TimeCycle.IN = 0;

			pMold->LowBlow.Step			= 30000;
			pMold->HighBlow.Step			= 30000;
			pMold->BlowPin.Step			= 30000;
			pMold->BlowPin2.Step			= 30000;
			pMold->BottomPin.Step		= 30000;
			pMold->Carriage.Step		   = 20000;
			pMold->Clamp.Step			   = 30000;
			//	pMold->CoolPin.Step			= 30000;
			pMold->CoolPinBlow.Step	   = 30000;
			//	pMold->CoolPin2.Step		   = 30000;
			pMold->CoolPin2Blow.Step	= 30000;
			//	pMold->CoolPin3.Step	      = 30000;
			pMold->CoolPin3Blow.Step	= 30000;
			pMold->BlowDeflash.Step		= 30000;
			pMold->CoolDeflash.Step		= 30000;
			pMold->Cutter.Step			= 30000;
			pMold->TopDeflash.Step		= 30000;
			pMold->BottomDeflash.Step	= 30000;
			pMold->SupportAir.Step		= 30000;
			pMold->SupportSuckAir.Step		= 30000;
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) RMold.ExtrLift.Step		   = 30000;
			else  pMold->ExtrLift.Step		   = 30000;
			pMold->Lock.Step			   = 30000;
			pMold->PullBottom.Step		= 30000;
			pMold->InnerPin.Step	      = 30000;
			pMold->CutNeck.Step   		= 30000;
			//	pMold->Robot.Step	    	   = 30000;
			pMold->ChuteDeflashBlow.Step = 30000;
			pMold->SubMold.Step		   = 30000;
			pMold->PinPart.Step		   = 30000;
			pMold->Screw.Step		      = 30000;
			pMold->HookPin.Step		   = 30000;

			pMold->ResetStep = 0;
			break;

		case 30000:
			pMold->TimeCycle.IN = 0;
			pMold->ResetStep = 0;
			break;

		case 40000:
			pMold->TimeCycle.IN = 0;

			pMold->LowBlow.Step			= 30000;
			pMold->HighBlow.Step		= 30000;
			pMold->BlowPin.Step			= 30000;
			pMold->BlowPin2.Step		= 30000;
			pMold->BottomPin.Step		= 30000;
			pMold->Carriage.Step		= 20000;
			pMold->Clamp.Step			= 30000;
			pMold->CoolPin.Step			= 30000;
			pMold->CoolPinBlow.Step	    = 30000;
			pMold->CoolPin2.Step		= 30000;
			pMold->CoolPin2Blow.Step	= 30000;
			pMold->CoolPin3.Step	    = 30000;
			pMold->CoolPin3Blow.Step	= 30000;
			pMold->BlowDeflash.Step		= 30000;
			pMold->CoolDeflash.Step		= 30000;
			pMold->Cutter.Step			= 30000;
			pMold->TopDeflash.Step		= 30000;
			pMold->BottomDeflash.Step	= 30000;
			pMold->SupportAir.Step		= 30000;
			pMold->SupportSuckAir.Step	= 30000;
			if(ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType) RMold.ExtrLift.Step		   = 30000;
			else  pMold->ExtrLift.Step	= 30000;
			pMold->Lock.Step			= 30000;
			pMold->PullBottom.Step		= 30000;
			pMold->InnerPin.Step	    = 30000;
			pMold->CutNeck.Step   		= 30000;
			pMold->Robot.Step	    	= 30000;
			pMold->ChuteDeflashBlow.Step = 30000;
			pMold->SubMold.Step		    = 30000;
			pMold->PinPart.Step		    = 30000;
			pMold->Screw.Step		    = 30000;
			pMold->HookPin.Step		    = 30000;

			pMold->ResetStep = 60000;
			break;

		case 60000:
			break;
	}/* end of switch */
		
	if(1 == gMachineInfo.MachineReset)
	{
		pMold->TimeDis.ResetAlarmTime = pMold->TimeCycle.ET;
	}
	
		
	//	if(1 == gMachineInfo.Manual && 1 == pMold->TimeCycle.IN)
	//	{
	//		pMold->TimeDis.CycleAlarmTime = pMold->TimeCycle.ET;
	//	}

	/* 单模复位限时到  */
	if(1 == pMold->TimeCycle.Q)
	{
		pMold->TimeCycle.IN = 0;
		pMold->ResetStep = 40000;
	}
}/*end of function */





