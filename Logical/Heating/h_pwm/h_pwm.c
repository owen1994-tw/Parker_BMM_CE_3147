/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: h_pwm
 * File: h_pwm.c
 * Author: lijianjun
 * Created: March 04, 2013
 ********************************************************************
 * Implementation of program h_pwm --- heating and cooling PWM mode
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT h_pwmINIT(void)
{
	/* TODO: Add code here */
	gT_Par.GrpSwh.OnOff[0] = 0;
	for(ZoneIndex=0; ZoneIndex<MAX_IDX_CONTRL_ZONES; ZoneIndex++)	
	{
		gT_Par.Ctl.OnOff[ZoneIndex] = 0;
		
		/* Heating time minum and Period  */
		gT_Par.Fix.tMinHeat[ZoneIndex]    = 0.05;        
		gT_Par.Fix.tPeriodHeat[ZoneIndex] = 3.0;
		
        /* Cooling time minum and Period  */
		gT_Par.Fix.tMinCool[ZoneIndex]    = 0.2;
		gT_Par.Fix.tPeriodCool[ZoneIndex] = 3.0;
		
		LCRPWM_HeatArray[ZoneIndex].max_value = 100.0;
		LCRPWM_HeatArray[ZoneIndex].min_value = 0.0;
	
		LCRPWM_CoolArray[ZoneIndex].max_value = 100.0;
		LCRPWM_CoolArray[ZoneIndex].min_value = 0.0;
	}/* for(ZoneIndex=0; ZoneIndex<MAX_IDX_CONTRL_ZONES; ZoneIndex++) */
}

void _CYCLIC h_pwmCYCLIC(void)
{
	/* TODO: Add code here */
	
		/* quick setting parameters  */
		if(1 == gT_Par.Ctl.IsQuickSetValue && 0 == IsQuickSetValueOld)
		{
			gT_Par.Ctl.IsQuickSetValue = 0;
			for(ZoneIndex=0; ZoneIndex<MAX_IDX_ZONE; ZoneIndex++)
			{
				if(gT_Par.Par.ST_PID_Same > 0&& gT_Par.Par.ST_PID_Same <=gT_Par.Fix.AT_MAX)
				{
					gT_Par.Par.ST_PID[ZoneIndex] = gT_Par.Par.ST_PID_Same;
				}
         
				if(gT_Par.Par.AlaUpTol_Same > 0&& gT_Par.Par.AlaUpTol_Same <=gUserFix_new.TempUpRange)
				{
					gT_Par.Par.AlaUpTol[ZoneIndex] = gT_Par.Par.AlaUpTol_Same;
				}
         
				if(gT_Par.Par.AlaDwnTol_Same > 0&& gT_Par.Par.AlaDwnTol_Same <=gUserFix_new.TempDwnRange)
				{
					gT_Par.Par.AlaDwnTol[ZoneIndex] = gT_Par.Par.AlaDwnTol_Same;
				}
			}
		}/* if(1 == gT_Par.Ctl.IsQuickSetValue && 0 == IsQuickSetValueOld) */
    
		IsQuickSetValueOld = gT_Par.Ctl.IsQuickSetValue;
    
		bHeating = 0;
		for(ZoneIndex=0; ZoneIndex<MAX_IDX_CONTRL_ZONES; ZoneIndex++)
		{
			if(1 == gT_Par.Ctl.PreOnOff[ZoneIndex] || gT_Par.Ctl.OnOff[ZoneIndex])bHeating = 1;
			
			/* default heating pwm parameter */
			if(gT_Par.Fix.tMinHeat[ZoneIndex] < 0.05  )gT_Par.Fix.tMinHeat[ZoneIndex] = 0.05;
			if(1 == gT_Par.Fix.bContactor[ZoneIndex] && gT_Par.Fix.tMinHeat[ZoneIndex] < 0.3)gT_Par.Fix.tMinHeat[ZoneIndex] = 0.3;
			
			LCRPWM_HeatArray[ZoneIndex].enable = gT_Par.Fix.Enable[ZoneIndex];
			LCRPWM_HeatArray[ZoneIndex].t_min_pulse = gT_Par.Fix.tMinHeat[ZoneIndex];
			LCRPWM_HeatArray[ZoneIndex].t_period = gT_Par.Fix.tPeriodHeat[ZoneIndex];
			LCRPWM_HeatArray[ZoneIndex].x = gH_IO.y_heat[ZoneIndex];
			LCRPWM(&LCRPWM_HeatArray[ZoneIndex]);
			
			/* default cooling pwm parameter */
			if(gT_Par.Fix.tMinCool[ZoneIndex] < 0.3)gT_Par.Fix.tMinCool[ZoneIndex] = 0.3;
			gT_Par.Fix.tPeriodCool[ZoneIndex] = gT_Par.Fix.tPeriodHeat[ZoneIndex];

			LCRPWM_CoolArray[ZoneIndex].enable = gT_Par.Fix.Cooling[ZoneIndex];
			LCRPWM_CoolArray[ZoneIndex].t_min_pulse = gT_Par.Fix.tMinCool[ZoneIndex];
			LCRPWM_CoolArray[ZoneIndex].t_period = gT_Par.Fix.tPeriodCool[ZoneIndex];
			LCRPWM_CoolArray[ZoneIndex].x = gH_IO.y_cool[ZoneIndex];
			LCRPWM(&LCRPWM_CoolArray[ZoneIndex]);

			/* digital output  */
			gH_IO.do_H[ZoneIndex] = LCRPWM_HeatArray[ZoneIndex].pulse;
			gH_IO.do_C[ZoneIndex] = LCRPWM_CoolArray[ZoneIndex].pulse;
		
			/* analog output  */
			gH_IO.ao_H[ZoneIndex] = (INT)(gH_IO.y_heat[ZoneIndex] * 32767/100.0);
			gH_IO.ao_C[ZoneIndex] = (INT)(gH_IO.y_cool[ZoneIndex] * 32767/100.0);
        
			/* zones color  */
             
			if(1 == gT_Par.Status.SensorBroken[ZoneIndex])
			{
				gH_IO.ZoneColor[ZoneIndex] = 0x332A;	/* ºì×Ö»Æµ×  *//*Word Red , Back Color Yellow*/
			}
			else if(1 == gT_Par.Status.AlmHigh[ZoneIndex])			
			{
				gH_IO.ZoneColor[ZoneIndex] = 0x0137;   /* À¶×Öºìµ×  */	//*Word Blue , Back Color Red*/	
			}
			else if(1 == gT_Par.Status.AlmLow[ZoneIndex])
			{
				gH_IO.ZoneColor[ZoneIndex] = 0x0110;	/* À¶×Ö°×µ×  */	//*Word Blue , Back Color White*/	
			}
			else
			{
				gH_IO.ZoneColor[ZoneIndex] = 0x0122;   /* Ë{×ÖüSµ×  */	//*Word Blue , Back Color Yellow*/
			}
        
			if(1 == gT_Par.Ctl.PreOnOff[ZoneIndex] )  /* ±£ÎÂ¹ý³Ì   */
			{
				gH_IO.ZoneColor[ZoneIndex] = 0x012F;   /* À¶×Ö»Æµ×  */  //*Word Blue , Back Color Yellow*/
			}
			/***************************************************************************/
			/********************ÎÂÇø×´Ì¬Í¼×éÖÐµÄÍ¼Æ¬¿ØÖÆ*******************************/
			/***************************************************************************/
			TON_StatusBG[ZoneIndex].IN = 1;
      
			if ((1 == gH_IO.do_H[ZoneIndex]) && (gH_IO.HC_sta[ZoneIndex] > 4)) 
			{
				gH_IO.HC_sta[ZoneIndex] = 4;
			}
			else if ((1 == gH_IO.do_C[ZoneIndex]) && (gH_IO.HC_sta[ZoneIndex] < 5)) 
			{
				gH_IO.HC_sta[ZoneIndex] = 5;
			}
      
			if (TON_StatusBG[ZoneIndex].Q == 1) 		/* Ã¿1ÃëÖÓ, Ë¢ÐÂÍ¼Æ¬ */
			{
				TON_StatusBG[ZoneIndex].IN = 0;
      
				if (1 ==gT_Par.Status.SensorBroken[ZoneIndex]) 
				{
					gH_IO.HC_sta[ZoneIndex] = 7;
				}
				else if (1 == gH_IO.do_H[ZoneIndex]) 
				{
					gH_IO.HC_sta[ZoneIndex] = gH_IO.HC_sta[ZoneIndex] + 1;
      
					if (gH_IO.HC_sta[ZoneIndex] > 4) 
					{
						gH_IO.HC_sta[ZoneIndex] = 1;
					}	
				}	
				else if (1 == gH_IO.do_C[ZoneIndex]) 
				{
					gH_IO.HC_sta[ZoneIndex] = gH_IO.HC_sta[ZoneIndex] + 1;
      
					if (gH_IO.HC_sta[ZoneIndex] > 6) 
					{
						gH_IO.HC_sta[ZoneIndex] = 5;
					}	
				}		
				else
				{ 
					gH_IO.HC_sta[ZoneIndex] = 0;
				}
			}/* if (TON_StatusBG[ZoneIndex].Q == 1) */
      
			/*********************************************************************************/
			/**************TON_StatusBG¶¨Ê±Æ÷ÊµÏÖ*********************************************/
			/*********************************************************************************/
			TON_StatusBG[ZoneIndex].PT = 1000;				/*Éè¶¨Ê±¼äÎª1Ãë*/
			TON(&TON_StatusBG[ZoneIndex]);        
		}/* for(ZoneIndex=0; ZoneIndex<MAX_IDX_CONTRL_ZONES; ZoneIndex++) */
	
		/* -------- ³¬œØ±£×oé_êP -------- */
		if(1 == gT_Par.Status.bSwitchOff)
		{
			gMachineOut.TempProtection = 1;	
		}
		else
		{
			gMachineOut.TempProtection = 0;
		}
	}/* end of cyclic  */

void _EXIT h_pwmEXIT(void)
{
	/* TODO: Add code here */
}
