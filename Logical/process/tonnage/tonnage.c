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

void Lock( Mold_typ * pMold,Clamp_Para_typ * pClampPara);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	LMold.Lock.Step = 30000;
	RMold.Lock.Step = 30000;
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{

	Lock(&LMold,&gMachinePara.MoldL.Clamp);
	Lock(&RMold,&gMachinePara.MoldR.Clamp);

	
	gMachineOut.LMold.ClampLocking = LMold.ValveOut.Lock;
	gMachineOut.RMold.ClampLocking = RMold.ValveOut.Lock;
	
	gMachineOut.LMold.ClampClsAux  = LMold.ValveOut.ClampClsAux;
	gMachineOut.RMold.ClampClsAux  = RMold.ValveOut.ClampClsAux;
	

} /* end of _CYCLIC */

void Lock( Mold_typ * pMold,Clamp_Para_typ * pClampPara)
{

	switch ( pMold->Lock.Step )
	{
		/*------------------ stop all the output ------------------*/
		case 0:
			pMold->Lock.Timer.IN = 0;
			
			if(pMold->Clamp.Step <= 300 && pMold->Clamp.Step >= 500 )  // Clamp Close Fast Do not Close valve
			{
				pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
				pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/		
			}
			
			break;
   
		/*------------------ Lock  -------------------- */
		case 100: 	/* delay */
			
			if(pMold->TimeSet.ClampLockDly != 0)
			{
				pMold->Lock.Timer.IN = 1;
				pMold->Lock.Timer.PT = pMold->TimeSet.ClampLockDly;
				pMold->Lock.Step = 200;
			}
			else
			{
				pMold->Lock.Step = 300;
			}
			
			break;
   
		case 200: 	/* locking delay */
			
			pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
			pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/
			
			pMold->TimeDis.ClampLockDly = pMold->Lock.Timer.ET;
			if (1 == pMold->Lock.Timer.Q)
			{
				pMold->Lock.Timer.IN = 0;
				pMold->TimeDis.ClampLockDly = 0;
				pMold->Lock.Step = 300;
			}
			break;
   
		case 300: 	/* set lock time */
			
			if(pMold->TimeSet.ClampLockTime != 0)
			{
				pMold->Lock.Timer.IN = 1;
				pMold->Lock.Timer.PT = pMold->TimeSet.ClampLockTime;
				pMold->Lock.Step = 400;
			}
			else
			{
				pMold->Lock.Step = 500;
			}
			
			break;
   
		case 400: 	/* locking */

			pMold->ValveOut.Lock = 1;		/*Clamp Locking valve*/
			pMold->ValveOut.ClampClsAux = 1;/*Clamp Close aux. valve*/
			
			//			pMold->Lock.p_set = pClampPara->P_Tonnage;
			//			pMold->Lock.v_set = pClampPara->V_Tonnage;
				
			pMold->TimeDis.ClampLockTime = pMold->Lock.Timer.ET;
			if (1 == pMold->Lock.Timer.Q)
			{
				pMold->Lock.Timer.IN = 0;
				//	pMold->TimeDis.ClampLockTime = 0;
			
				pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
				pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/
			
				//				pMold->Lock.p_set = 0.0;
				//				pMold->Lock.v_set = 0.0;
			
				pMold->Lock.Step = 500;
			}
			break;
   
		case 500:

			pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
			pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/
			
			pMold->Lock.Timer.IN = 0;
			pMold->Lock.Step = 3000;
			break;
   
		case 3000:
			break;
   
		case 10100:

			pMold->Lock.Step = 13000;
			break;
   
		case 13000:
			break;
   
		/*--------------- Manual action stop -----------------*/
		case 20000:
			pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
			pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/
			
			
			pMold->Lock.p_set = 0.0;
			pMold->Lock.v_set = 0.0;

			pMold->Lock.Timer.IN         = 0;
			//   pMold->TimeDis.ClampLockTime = 0;
			pMold->Lock.Step = 0;
			break;

		/*--------------- Instance stop -----------------*/
		case 30000:
			pMold->ValveOut.Lock = 0;		/*Clamp Locking valve*/
			pMold->ValveOut.ClampClsAux = 0;/*Clamp Close aux. valve*/
			
			
			pMold->Lock.p_set = 0.0;
			pMold->Lock.v_set = 0.0;
            
			pMold->Lock.Timer.IN = 0;
			//	pMold->TimeDis.ClampLockTime = 0;
			pMold->Lock.Step = 0;
				
			break;
	}/* end of switch */
	
	//	pMold->Lock.aoPropValve = (INT)(pMold->Lock.v_set * 327.67);
	
}/*end of function */




