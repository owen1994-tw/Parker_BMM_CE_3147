/********************************************************************
 * COPYRIGHT -- B&R China
 ********************************************************************
 * Program: cooldef
 * File: cooldef.c
 * Author: lij
 * Created: December 08, 2014
 ********************************************************************
 * Implementation of program cooldef
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void CoolDeflash( Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K 		 				   
**************************************************************************************/
void _INIT DeflashCINIT(void)
{
	/* TODO: Add code here */
   LMold.CoolDeflash.Step = 0;
   RMold.CoolDeflash.Step = 0;    
}

/*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K  				   
**************************************************************************************/
void _CYCLIC DeflashCCYCLIC(void)
{
	/* TODO: Add code here */
   
   CoolDeflash(&LMold);
   CoolDeflash(&RMold);
	
   gMachineOut.LMold.CoolDeflash = LMold.CoolDeflash.Out;
   gMachineOut.RMold.CoolDeflash = RMold.CoolDeflash.Out;
	
}
/*************************************************************************************
        				E X I T   P A R T   O F   T H E   T A S K  				   
**************************************************************************************/
void _EXIT DeflashCEXIT(void)
{
	/* TODO: Add code here */
}

void CoolDeflash( Mold_typ * pMold)
{
   /* ------ step machine control ------ */
	switch ( pMold->CoolDeflash.Step )  
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->CoolDeflash.Out  	 = 0;
			pMold->ActInfo.CoolDeflash  = 0;
			pMold->CoolDeflash.Timer.IN = 0;
			break;
            
		/*------------------ CoolDeflash close -------------------- */
		case 100: /* CoolDeflash delay*/
			if(pMold->TimeSet.CoolDeflashDelay != 0)
			{
				pMold->CoolDeflash.Timer.IN = 1;
				pMold->CoolDeflash.Timer.PT = pMold->TimeSet.CoolDeflashDelay;
				pMold->CoolDeflash.Step = 200;
			}
			else
			{
				//     pMold->CoolDeflash.Step = 300;
				pMold->CoolDeflash.Step = 450;  //ipis0731
			}
			break;
             	
		case 200: /* delay ok */
			pMold->TimeDis.CoolDeflashDelay = pMold->CoolDeflash.Timer.ET;  
			if (1 == pMold->CoolDeflash.Timer.Q)
			{
				pMold->CoolDeflash.Timer.IN     = 0;
				//   pMold->TimeDis.CoolDeflashDelay = 0;
				pMold->CoolDeflash.Step = 450;  //ipis0731
				//   pMold->CoolDeflash.Step = 300;
			}
			break;
            	
		case 300:
			pMold->CoolDeflash.Timer.IN = 1;
			pMold->CoolDeflash.Timer.PT = pMold->TimeSet.CoolDeflashTime;
			pMold->CoolDeflash.Step = 400;
			
		case 400:
			pMold->ActInfo.CoolDeflash = 1;
			pMold->CoolDeflash.Out     = 1;
			pMold->TimeDis.CoolDeflashTime = pMold->CoolDeflash.Timer.ET;  
			if (1 == pMold->CoolDeflash.Timer.Q)
			{
				pMold->CoolDeflash.Timer.IN     = 0;
				pMold->TimeDis.CoolDeflashTime = 0;
				pMold->CoolDeflash.Step = 500;
			}
			break;
          
		case 450:
			pMold->ActInfo.CoolDeflash = 1;
			pMold->CoolDeflash.Out     = 1;
					
			break;
		
		case 500:
			pMold->CoolDeflash.Timer.IN = 0;
			pMold->CoolDeflash.Out     = 0;
			pMold->ActInfo.CoolDeflash = 0;
			pMold->CoolDeflash.Step = 3000;
			break;
            	
		case 3000:
			break;
            	
		case 20000:
			pMold->CoolDeflash.Out      = 0;
			pMold->CoolDeflash.Timer.IN = 0;
				
			pMold->ActInfo.CoolDeflash = 0;
			pMold->CoolDeflash.Step    = 0;
			break;
					
		case 30000:
			pMold->CoolDeflash.Out      = 0;
			pMold->CoolDeflash.Timer.IN = 0;
				
			pMold->ActInfo.CoolDeflash  = 0;
			pMold->CoolDeflash.Step     = 0;
			break;
	}/* end of switch */
}/*end of function */

