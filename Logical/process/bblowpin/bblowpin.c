/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: bblowpin
 * File: bblowpin.c
 * Author: lij
 * Created: September 25, 2015
 ********************************************************************
 * Implementation of program bblowpin
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/*********************************************************************
                       Function Declarations
*********************************************************************/
void BottomPin(Mold_typ * pMold);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT bblowpinINIT(void)
{
	/* TODO: Add code here */
   LMold.BottomPin.Step = 30000;
   RMold.BottomPin.Step = 30000;
}
/*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _CYCLIC bblowpinCYCLIC(void)
{
	/* TODO: Add code here */
   LMold.TransDIn.BottomPinUpLimit = gMachineIn.LMold.BottomPinUpLimit;
   RMold.TransDIn.BottomPinUpLimit = gMachineIn.RMold.BottomPinUpLimit;

   LMold.TransDIn.BottomPinDnLimit = gMachineIn.LMold.BottomPinDnLimit = 1;    /* No install this sensor */
   RMold.TransDIn.BottomPinDnLimit = gMachineIn.RMold.BottomPinDnLimit = 1;    /* No install this sensor */
   
   /* call function */
   BottomPin(&LMold);
   BottomPin(&RMold);
   
   gMachineOut.LMold.BottomPinUp = LMold.ValveOut.BottomPinUp;
   gMachineOut.LMold.BottomPinDn = LMold.ValveOut.BottomPinDn;

   gMachineOut.RMold.BottomPinUp = RMold.ValveOut.BottomPinUp;
   gMachineOut.RMold.BottomPinDn = RMold.ValveOut.BottomPinDn;
   
}

void _EXIT bblowpinEXIT(void)
{
	/* TODO: Add code here */
}

void BottomPin(Mold_typ * pMold)
{
   switch (pMold->BottomPin.Step)  
   {
      /*------------------ stop all the output ------------------*/
      case 0:
         pMold->BottomPin.Out  		= 0;
         pMold->BottomPin.Timer.IN	= 0;
         break;
            
      /*------------------ Bottom Pin move down -------------------- */
      case 100: 	/* down delay*/
         if(1 == pMold->Option.BottomPin)
         {	
            if(pMold->TimeSet.BottomPinDnDelay > 0)
            {
               pMold->BottomPin.Timer.IN = 1;
               pMold->BottomPin.Timer.PT = pMold->TimeSet.BottomPinDnDelay;
               pMold->BottomPin.Step 	= 400;
            }
            else
            {
               pMold->BottomPin.Step = 300;
            }
         }
         else
         {
            pMold->BottomPin.Step = 3000;
         }		
         break;
      
      case 200: 	/* delay ok */
         pMold->TimeDis.BottomPinDnDelay = pMold->BottomPin.Timer.ET;  
         if (1 == pMold->BottomPin.Timer.Q)
         {
            pMold->BottomPin.Timer.IN       = 0;
            pMold->TimeDis.BottomPinDnDelay = 0;
            pMold->BottomPin.Step = 400;
         }
         break;
      
      case 300:
         pMold->BottomPin.Step = 400;
         break;
      
      case 400:
         pMold->ActInfo.BottomPinDn  = 1;
         pMold->ValveOut.BottomPinDn = 1;
         pMold->ActInfo.BottomPinUp  = 0;
         pMold->ValveOut.BottomPinUp = 0;	
         
         break;
      
      case 500:
         pMold->BottomPin.Timer.IN = 0;
				
         pMold->ActInfo.BottomPinDn  = 0;
         pMold->ValveOut.BottomPinDn = 0;
         pMold->BottomPin.Step = 3000;
         break;
      
      case 3000:
         break;
            	
      /*--------------- Bottom Pin move up -----------------*/
      case 10100: 	/* up delay*/
         if(1 == pMold->Option.BottomPin)
         {	
            pMold->BottomPin.Timer.IN = 1;
            pMold->BottomPin.Timer.PT = pMold->TimeSet.BottomPinUpDelay;
            pMold->BottomPin.Step = 10200;
         }
         else
         {
            pMold->BottomPin.Step = 13000;
         }		
         break;
      
      case 10200: 	/* delay ok */
         pMold->TimeDis.BottomPinUpDelay = pMold->BottomPin.Timer.ET;  
         if (1 == pMold->BottomPin.Timer.Q)
         {
            pMold->BottomPin.Timer.IN       = 0;
            pMold->TimeDis.BottomPinUpDelay = 0;
            pMold->BottomPin.Step = 10300;
         }
         break;
      
      case 10300:
         pMold->BottomPin.Step = 10400;
         break;
      
      case 10400:
         pMold->ActInfo.BottomPinUp  = 1;
         pMold->ValveOut.BottomPinUp = 1;
         pMold->ActInfo.BottomPinDn  = 0;
         pMold->ValveOut.BottomPinDn = 0;
         break;
      
      case 10500:
         pMold->BottomPin.Timer.IN = 0;
         pMold->ActInfo.BottomPinUp  = 0;
         pMold->ValveOut.BottomPinUp = 0;
         pMold->BottomPin.Step = 13000;
         break;
      
      case 13000:
         break;
            	
      case 15000:
         if(1 == pMold->Option.BottomPin)
         {
            pMold->ActInfo.BottomPinUp  = 1;
            pMold->ValveOut.BottomPinUp = 1;
         }
         break;
            	
            
      /*--------------- Manual action stop -----------------*/	
      case 20000:
         pMold->BottomPin.Timer.IN = 0;
				
         pMold->ActInfo.BottomPinUp = 0;
         pMold->ActInfo.BottomPinDn = 0;

         pMold->ValveOut.BottomPinUp = 0;
         pMold->ValveOut.BottomPinDn = 0;

         pMold->BottomPin.Step = 0;
         break;
      
      /*--------------- Instance stop -----------------*/
      case 30000:
         pMold->BottomPin.Timer.IN = 0;
				
         pMold->ActInfo.BottomPinUp = 0;
         pMold->ActInfo.BottomPinDn = 0;
					
         pMold->ValveOut.BottomPinUp = 0;
         pMold->ValveOut.BottomPinDn = 0;

         pMold->BottomPin.Step = 0;
         break;
      
      /*----------------- Error step -----------------*/
      case 40000:
         pMold->BottomPin.Timer.IN = 0;
				
         pMold->ActInfo.BottomPinUp = 0;
         pMold->ActInfo.BottomPinDn = 0;
					
         pMold->ValveOut.BottomPinUp = 0;
         pMold->ValveOut.BottomPinDn = 0;
         
         pMold->BottomPin.Step = 0;
         break;
	}/* switch (pMold->BottomPin.Step */
}
