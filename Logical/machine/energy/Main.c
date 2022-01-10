
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	// Insert code here 
	RTInfo_v.enable = 1;
	RTInfo(&RTInfo_v);
	tTask  = RTInfo_v.cycle_time/1000.0/1000.0;
	
	tEnergyTime_1 = 0.0; 
	tEnergyTime_2 = 0.0;
	
	cntRunTime = 0;

}

void _CYCLIC ProgramCyclic(void)
{
	// Insert code here 
	UDINT  Temp;
	
	/* ---------------------------- Energy monitor ------------------------- */
	if(gUserPara.SetTimePower < 1)gUserPara.SetTimePower = 10;
	Temp  = (UDINT)(gUserPara.SetTimePower * 60 / tTask);
	
	if(1 == gMachineInfo.Auto)
	{
		if(cntRunTime >= Temp * 100 )cntRunTime = 0;
		if(0 == (cntRunTime % Temp))
		{
//			gMachineInfo.AvgEnergy = (gMachineInfo.AenergyT_1 - AvgEnergyOld) * 60.0 /gUserPara.SetTimePower;
//			gMachineInfo.AvgEnergyHeating = (gMachineInfo.AenergyT_2 - AvgEnergyHeatingOld)* 60.0 /gUserPara.SetTimePower;
			
			gMachineInfo.EnergyInfo[0].AvgEnergy = (gMachineInfo.EnergyInfo[0].AenergyT - AvgEnergyOld) * 60.0 /gUserPara.SetTimePower;/*Albert*/
			gMachineInfo.EnergyInfo[1].AvgEnergy = (gMachineInfo.EnergyInfo[1].AenergyT - AvgEnergyHeatingOld)* 60.0 /gUserPara.SetTimePower;/*Albert*/
			gMachineInfo.EnergyInfo[2].AvgEnergy = (gMachineInfo.EnergyInfo[2].AenergyT - AvgEnergyPumpOld) * 60.0/gUserPara.SetTimePower;/*Albert*/
			
//			AvgEnergyOld = gMachineInfo.AenergyT_1 ;
//			AvgEnergyHeatingOld = gMachineInfo.AenergyT_2 ;
			
			AvgEnergyOld = gMachineInfo.EnergyInfo[0].AenergyT;
			AvgEnergyHeatingOld = gMachineInfo.EnergyInfo[1].AenergyT;
			AvgEnergyPumpOld = gMachineInfo.EnergyInfo[2].AenergyT;/*Albert for pump consumption*/
		}
		cntRunTime = cntRunTime + 1;
	}
	else
	{
		cntRunTime = 0;
	}
	
	/* First AP3131 Old*/
	if(1 == gHardWare.AP3131_1.ClearEnergy)
	{
		tEnergyTime_1 = 0.0;
		AvgEnergyOld = 0;/*Albert*/
	}
	else
	{
		tEnergyTime_1 = tEnergyTime_1 + tTask;	
	}
	
	tEnergyTimeDis_1 = tEnergyTime_1/60.0;
	
	gHardWare.AP3131_1.EnableEnergy = 1;
	if(gUserPara.Current32767_A < 1.0)gUserPara.Current32767_A = 300.0;
	gUserPara.Power32767_A   = 4.0;
	
	gMachineInfo.Current_A1 = gHardWare.AP3131_1.IrmsA * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.Current_B1 = gHardWare.AP3131_1.IrmsB * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.Current_C1 = gHardWare.AP3131_1.IrmsC * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.Power_1    = gHardWare.AP3131_1.PmeanT * gUserPara.Power32767_A/1000.0;
	gMachineInfo.AenergyT_1 = gHardWare.AP3131_1.AenergyT * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.SumAenergyT_1 = gHardWare.AP3131_1.SAenergyT * gUserPara.Current32767_A/1000.0/50.0;
	
	

	
	
	
	/*First AP3131 New*/
	
	
	gMachineInfo.EnergyInfo[0].Current_A = gHardWare.AP3131_1.IrmsA * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.EnergyInfo[0].Current_B = gHardWare.AP3131_1.IrmsB * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.EnergyInfo[0].Current_C = gHardWare.AP3131_1.IrmsC * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.EnergyInfo[0].Power    = gHardWare.AP3131_1.PmeanT * gUserPara.Power32767_A/1000.0;
	gMachineInfo.EnergyInfo[0].AenergyT = gHardWare.AP3131_1.AenergyT * gUserPara.Current32767_A/1000.0/50.0;
	gMachineInfo.EnergyInfo[0].SumAenergyT = gHardWare.AP3131_1.SAenergyT * gUserPara.Current32767_A/1000.0/50.0;
	
	/* Second AP3131 old */
	if(1 == gHardWare.AP3131_2.ClearEnergy)
	{
		tEnergyTime_2 = 0.0;
		AvgEnergyHeatingOld = 0; /*Albert*/
	}
	else
	{
		tEnergyTime_2 = tEnergyTime_2 + tTask;
	}
	
	tEnergyTimeDis_2 = tEnergyTime_2/60.0;
	
	gHardWare.AP3131_2.EnableEnergy = 1;
	if(gUserPara.Current32767_B < 1.0)gUserPara.Current32767_B = 100.0;
	gUserPara.Power32767_B = 4.0;
	
	gMachineInfo.Current_A2 = gHardWare.AP3131_2.IrmsA * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.Current_B2 = gHardWare.AP3131_2.IrmsB * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.Current_C2 = gHardWare.AP3131_2.IrmsC * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.Power_2    = gHardWare.AP3131_2.PmeanT * gUserPara.Power32767_B/1000.0;
	gMachineInfo.AenergyT_2 = gHardWare.AP3131_2.AenergyT * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.SumAenergyT_2 = gHardWare.AP3131_2.SAenergyT * gUserPara.Current32767_B/1000.0/50.0;
	
	
	/* Second AP3131 New*/
	
	gMachineInfo.EnergyInfo[1].Current_A = gHardWare.AP3131_2.IrmsA * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.EnergyInfo[1].Current_B = gHardWare.AP3131_2.IrmsB * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.EnergyInfo[1].Current_C = gHardWare.AP3131_2.IrmsC * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.EnergyInfo[1].Power    = gHardWare.AP3131_2.PmeanT * gUserPara.Power32767_B/1000.0;
	gMachineInfo.EnergyInfo[1].AenergyT = gHardWare.AP3131_2.AenergyT * gUserPara.Current32767_B/1000.0/50.0;
	gMachineInfo.EnergyInfo[1].SumAenergyT = gHardWare.AP3131_2.SAenergyT * gUserPara.Current32767_B/1000.0/50.0;
	
	
	/* Third AP3131 */
	if(1 == gHardWare.AP3131_3.ClearEnergy)
	{
		tEnergyTime_3 = 0.0;
		AvgEnergyPumpOld = 0;/*Albert*/
	}
	else
	{
		tEnergyTime_3 = tEnergyTime_3 + tTask;
	}
	
	tEnergyTimeDis_3 = tEnergyTime_3/60.0;
	
	gHardWare.AP3131_3.EnableEnergy = 1;
	if(gUserPara.Current32767_C < 1.0)gUserPara.Current32767_C = 100.0;
	gUserPara.Power32767_C = 4.0;
	
	gMachineInfo.EnergyInfo[2].Current_A = gHardWare.AP3131_3.IrmsA * gUserPara.Current32767_C/1000.0/50.0;
	gMachineInfo.EnergyInfo[2].Current_B = gHardWare.AP3131_3.IrmsB * gUserPara.Current32767_C/1000.0/50.0;
	gMachineInfo.EnergyInfo[2].Current_C = gHardWare.AP3131_3.IrmsC * gUserPara.Current32767_C/1000.0/50.0;
	gMachineInfo.EnergyInfo[2].Power    = gHardWare.AP3131_3.PmeanT * gUserPara.Power32767_C/1000.0;
	gMachineInfo.EnergyInfo[2].AenergyT = gHardWare.AP3131_3.AenergyT * gUserPara.Current32767_C/1000.0/50.0;
	gMachineInfo.EnergyInfo[2].SumAenergyT = gHardWare.AP3131_3.SAenergyT * gUserPara.Current32767_C/1000.0/50.0;
}

void _EXIT ProgramExit(void)
{
	// Insert code here 

}

