
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/
void Platform( Mold_typ * pMold, Plate_Para_typ * PlatePara,Plate_Fix_typ * pPlateFix);


void _INIT ProgramInit(void)
{
	// Insert code here 
	LMold.Plate.Step = 30000;
}

void _CYCLIC ProgramCyclic(void)
{
	// Insert code here 
	Platform(&LMold, &gMachinePara.Plate,&gMachineFix.Plate);
	Platform(&RMold, &gMachinePara.Plate,&gMachineFix.Plate);
	
	gMachineOut.PlateUp = LMold.ValveOut.PlateUp | RMold.ValveOut.PlateUp;
	gMachineOut.PlateDn = LMold.ValveOut.PlateDn | RMold.ValveOut.PlateDn;
	
	gMachineOut.PlateFw = LMold.ValveOut.PlateFw | RMold.ValveOut.PlateFw;
	gMachineOut.PlateBw = LMold.ValveOut.PlateBw | RMold.ValveOut.PlateBw;
	
	
	
	
	
}


void _EXIT ProgramExit(void)
{
	// Insert code here 

}

void Platform( Mold_typ *pMold, Plate_Para_typ *PlatePara,Plate_Fix_typ * pPlateFix)
{ 	 
	switch ( pMold->Plate.Step )  
	{
		case 0:
			pMold->Plate.p_set = 0.0;
			pMold->Plate.v_set = 0.0;
			pMold->ValveOut.PlateUp = 0;
			pMold->ValveOut.PlateDn = 0;
			
			break;
  	
		
		case 5000:
			
			if(1 == gMachineIn.PlateUpLimit)
			{
				pMold->ValveOut.PlateUp = 0;
				pMold->ValveOut.PlateDn = 0;
			
				pMold->Plate.p_set = 0.0;
				pMold->Plate.v_set = 0.0;
			
				gImply.PlateUpLimit = gMachineIn.PlateUpLimit;
			}
			else
			{
				pMold->Plate.Out = 1;
				pMold->ValveOut.PlateUp = 1;
				pMold->ValveOut.PlateDn = 0;
	
				pMold->Plate.p_set = pPlateFix->pCalibUp;
				pMold->Plate.v_set = pPlateFix->vCalibUp;		
			
			}
			
			break;
		
		
		
		
		case 6100:
		//	if(0 == gMachineIn.PlateUpLimit)
		//	{
				pMold->Plate.Step = 6200;
		//	}
		//	else
		//	{
		//		pMold->Plate.Step = 3000;
		//	}
		
			break;
		
		case 6200:
			if(0 == gMachineIn.PlateFwBwLimit)
			{
				pMold->Plate.Out = 1;
			
				pMold->ValveOut.PlateFw = 1;
				pMold->ValveOut.PlateBw = 0;
			
				pMold->Plate.p_set = pPlateFix->pCalibFw;
				pMold->Plate.v_set = pPlateFix->vCalibFw;
			}
			else
			{
				pMold->Plate.Out = 0;
			
				pMold->ValveOut.PlateFw = 0;
				pMold->ValveOut.PlateBw = 0;
			
				pMold->Plate.p_set = 0;
				pMold->Plate.v_set = 0;
			
				gImply.PlateFwBwLimit = gMachineIn.PlateFwBwLimit;
			}
		
			break;
		
		
		
		case 6500:
			if(gUserFix_new.PlateFwTime != 0)
			{
				pMold->Plate.Timer.IN  = 1;
				pMold->Plate.Timer.PT  = gUserFix_new.PlateFwTime; 
				pMold->Plate.Step = 6510;
			}
			else
			{
				pMold->Plate.Step = 6520;
			}
			break;
		
		case 6510:
		
			pMold->Plate.Out = 1;
			
			pMold->ValveOut.PlateFw = 1;
			pMold->ValveOut.PlateBw = 0;
	
			pMold->Plate.p_set = PlatePara->P_Fw;
			pMold->Plate.v_set = PlatePara->V_Fw;
			
			if (1 == pMold->Plate.Timer.Q)
			{
				pMold->ValveOut.PlateFw = 0;
				
				pMold->Plate.p_set = 0;
				pMold->Plate.v_set =0;
				pMold->Plate.Timer.IN  = 0;
				pMold->Plate.Step = 6520;
			}		
			break;
		
		case 6520:
			pMold->Plate.Out = 0;
			
			pMold->ValveOut.PlateFw = 0;
			pMold->ValveOut.PlateBw = 0;

			pMold->Plate.Timer.IN  = 0;
			pMold->Plate.Step = 0;
		
			break;
		
		
		
		case 15000:
			
		
			if(1 == gMachineIn.PlateDnLimit)
			{
				pMold->ValveOut.PlateUp = 0;
				pMold->ValveOut.PlateDn = 0;
				
				pMold->Plate.p_set = 0.0;
				pMold->Plate.v_set = 0.0; 
				
			
				gImply.PlateDnLimit = gMachineIn.PlateDnLimit;
			}
			else
			{
				pMold->Plate.Out = 1;
			
				pMold->ValveOut.PlateUp = 0;
				pMold->ValveOut.PlateDn = 1;
			
				pMold->Plate.p_set = pPlateFix->pCalibDn;
				pMold->Plate.v_set = pPlateFix->vCalibDn;
			}			
			break;
	
		case 16100:
			//	if(0 == gMachineIn.PlateUpLimit)
			//	{
			pMold->Plate.Step = 16200;
			//	}
			//	else
			//	{
			//		pMold->Plate.Step = 3000;
			//	}
		
			break;
		
		case 16200:
			
			if(0 == gMachineIn.PlateFwBwLimit)
			{
				pMold->Plate.Out = 1;
			
				pMold->ValveOut.PlateFw = 0;
				pMold->ValveOut.PlateBw = 1;
			
				pMold->Plate.p_set = pPlateFix->pCalibBw;
				pMold->Plate.v_set = pPlateFix->vCalibBw;
			}
			else
			{
				pMold->Plate.Out = 0;
			
				pMold->ValveOut.PlateFw = 0;
				pMold->ValveOut.PlateBw = 0;
			
				pMold->Plate.p_set = 0;
				pMold->Plate.v_set = 0;
			
				gImply.PlateFwBwLimit = gMachineIn.PlateFwBwLimit;
			}
		
		
			break;
		
		
		
		
		
		/*---------------    Í£Ö¹³ÌÐò    ---------------- */
		case 20000:
			pMold->ValveOut.PlateUp = 0;
			pMold->ValveOut.PlateDn = 0;
			
			pMold->Plate.v_set = 0.0;
			pMold->Plate.p_set = 0.0;
			
			pMold->Plate.Step = 21000;
			break;
	        
		case 21000:
			pMold->Plate.Timer.IN = 1;
			pMold->Plate.Timer.PT = 20; 
			pMold->Plate.Step = 22000;
			break;
			
		case 22000: 
			if (1 == pMold->Plate.Timer.Q)
			{
				pMold->Plate.Timer.IN = 0;
				pMold->ValveOut.PlateUp = 0;
				pMold->ValveOut.PlateDn = 0;
				pMold->ValveOut.PlateFw = 0;
				pMold->ValveOut.PlateBw = 0;
				pMold->Plate.Step = 23000;  
			}
			break;
			
		case 23000:
			pMold->Plate.Out = 0;
			
			pMold->Plate.p_set = 0.0;
			pMold->Plate.v_set = 0.0;

			pMold->ValveOut.PlateUp = 0;
			pMold->ValveOut.PlateDn = 0;
			pMold->Plate.Step = 0;  
			break;
			
		case 30000:
			pMold->Plate.Out = 0;
			
			pMold->Plate.p_set = 0.0;
			pMold->Plate.v_set = 0.0;

			pMold->ValveOut.PlateUp = 0;
			pMold->ValveOut.PlateDn = 0;
			pMold->ValveOut.PlateFw = 0;
			pMold->ValveOut.PlateBw = 0;
			
			pMold->Plate.Step = 0;
			break;
		
		case 40000:
			pMold->Plate.Out = 0;
			
			pMold->Plate.p_set = 0.0;
			pMold->Plate.v_set = 0.0;

			pMold->ValveOut.PlateUp = 0;
			pMold->ValveOut.PlateDn = 0;
			pMold->ValveOut.PlateFw = 0;
			pMold->ValveOut.PlateBw = 0;
			
			break;
		
		
		
		
	}/* end of switch */

}/*end of function */

