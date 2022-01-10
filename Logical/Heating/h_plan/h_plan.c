/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: h_plan
 * File: h_plan.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program h_plan --- heating  plan 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT h_planINIT(void)
{
	/* TODO: Add code here */
	
}

void _CYCLIC h_planCYCLIC(void)
{
	/* TODO: Add code here */
	
	RTC_gettime(&rtGetTime);
	
	/*   week index     */
	if(rtGetTime.reserve > 7 || rtGetTime.reserve < 1)rtGetTime.reserve = 1;
	
	indexWeek = rtGetTime.reserve - 1;

	/*  îAŸáé_   */
	if(1 == gHeatingPlan.Enable[indexWeek] && 1==gMachineIn.PreHeatDI)//ipis0220
	{
		if((rtGetTime.hour   == gHeatingPlan.HeatingOn[indexWeek].Hour  )&&
		   (rtGetTime.minute == gHeatingPlan.HeatingOn[indexWeek].Minute)&&
			(rtGetTime.second >0 && rtGetTime.second<3)
		  )
		{
			gT_Par.Ctl.IsPreHeatingPlan = 1;
		}
		else
		{
			gT_Par.Ctl.IsPreHeatingPlan = 0;
		}   
	}/* if(1 == gHeatingPlan.Enable[indexWeek]) */
	else
	{
		gT_Par.Ctl.IsPreHeatingPlan = 0;
	}
	
	/*  ¼ÓŸáé_   */
	if(1 == gHeatingPlan.Enable[7 + indexWeek])
	{
		if((rtGetTime.hour   == gHeatingPlan.HeatingOff[indexWeek].Hour  )&&
		   (rtGetTime.minute == gHeatingPlan.HeatingOff[indexWeek].Minute)&&
			(rtGetTime.second >0 && rtGetTime.second<3)
		  )
		{
			gT_Par.Ctl.IsHeatingOnPlan = 1;
		}
		else
		{
			gT_Par.Ctl.IsHeatingOnPlan = 0;
		}   
	}/* if(1 == gHeatingPlan.Enable[7 + indexWeek]) */
	else
	{
		gT_Par.Ctl.IsHeatingOnPlan = 0;
	}
	
}
