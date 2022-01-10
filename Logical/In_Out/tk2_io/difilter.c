#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/*****************************   DI filter  ***************************************/
void DIFilter(DI_Filter_typ * pDI)
{
	unsigned long	BaseTimes;
	if(0 == pDI->tTime)pDI->tTime = 200;              /*  ms  */
	if(0 == pDI->Percent)pDI->Percent = 80;           /*  0 -- 100 */
	if(pDI->TC_Time != 0 && pDI->tTime > 2 * pDI->TC_Time)
  	{
  		BaseTimes = pDI->tTime/pDI->TC_Time;
  		pDI->TimeCounter++;
  		if(pDI->TimeCounter > 100 * BaseTimes)pDI->TimeCounter = pDI->TimeCounter - 100 * BaseTimes;

 		if(pDI->In != 0)
		{
			pDI->Counter++;
		}

		if(0 == (pDI->TimeCounter % BaseTimes))
	  	{
			if(	pDI->Counter > BaseTimes * pDI->Percent /100)
	  		{
				pDI->Out = 1;
	 		}
			else
	 		{
				pDI->Out = 0;
			}
			pDI->Counter = 0;
		}
 	}
	else
  	{
		if(pDI->In != 0)
	  	{
			pDI->Out = 1;
		}
		else
 		{
			pDI->Out = 0;
		}
	}
}




