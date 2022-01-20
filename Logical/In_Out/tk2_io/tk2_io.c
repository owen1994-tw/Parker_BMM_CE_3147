/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: io
 * File: io.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program io --- normal input and output 
 ********************************************************************/

#include <bur/plctypes.h>/*-------------------Special-------------------------------*/
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "defines.h"
//#include "keyap880.h"
#include "edge.h"
#include "keyap2100.h"
#include "pic_id.h"

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void  MachineInit();
void DIFilter(DI_Filter_typ * pDI);
/*************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
*************************************************************************************/
_INIT void init ( void )
{
	ErrCounter = 0;
	
	RTInfo_v.enable = 1;
	RTInfo(&RTInfo_v);
	TC_TIME= RTInfo_v.cycle_time/1000;				/* Task class time [1 ms] */
	tTask  = RTInfo_v.cycle_time/1000.0/1000.0;
	

		
	fubTransferBelt.Ts = tTask;
	
	MachineInit();

	memset(&gHardWare.DOForce[0],0,sizeof(gHardWare.DOForce));
	memset(&gHardWare.AOForce[0],0,sizeof(gHardWare.AOForce));

	memset(&BlowPinUpFilter_L,0,sizeof(BlowPinUpFilter_L));
	BlowPinUpFilter_L.tTime   = 80;			/*  ms  */
	BlowPinUpFilter_L.TC_Time = TC_TIME;
	
	memset(&BlowPinUpFilter_R,0,sizeof(BlowPinUpFilter_R));
	BlowPinUpFilter_R.tTime   = 80;			/*  ms  */
	BlowPinUpFilter_R.TC_Time = TC_TIME;
	
	memset(&BlowPin2UpFilter_L,0,sizeof(BlowPin2UpFilter_L));
	BlowPin2UpFilter_L.tTime   = 80;			/*  ms  */
	BlowPin2UpFilter_L.TC_Time = TC_TIME;
	
	memset(&BlowPin2UpFilter_R,0,sizeof(BlowPin2UpFilter_R));
	BlowPin2UpFilter_R.tTime   = 80;			/*  ms  */
	BlowPin2UpFilter_R.TC_Time = TC_TIME;
	
	memset(&CarriageBwFilter_L,0,sizeof(CarriageBwFilter_L));
	CarriageBwFilter_L.tTime   = 80;			/*  ms  */
	CarriageBwFilter_L.TC_Time = TC_TIME;
	
	memset(&CarriageBwFilter_R,0,sizeof(CarriageBwFilter_R));
	CarriageBwFilter_R.tTime   = 80;			/*  ms  */
	CarriageBwFilter_R.TC_Time = TC_TIME;
	
	memset(&CarriageFwFilter_L,0,sizeof(CarriageFwFilter_L));
	CarriageFwFilter_L.tTime   = 80;			/*  ms  */
	CarriageFwFilter_L.TC_Time = TC_TIME;
	
	memset(&CarriageFwFilter_R,0,sizeof(CarriageFwFilter_R));
	CarriageFwFilter_R.tTime   = 80;			/*  ms  */
	CarriageFwFilter_R.TC_Time = TC_TIME;

	memset(&TopDeflashBwFilter_L,0,sizeof(TopDeflashBwFilter_L));
	TopDeflashBwFilter_L.tTime   = 80;		/*  ms  */
	TopDeflashBwFilter_L.TC_Time = TC_TIME;
	
	memset(&TopDeflashBwFilter_R,0,sizeof(TopDeflashBwFilter_R));
	TopDeflashBwFilter_R.tTime   = 80;		/*  ms  */
	TopDeflashBwFilter_R.TC_Time = TC_TIME;
	
	memset(&TopDeflashFwFilter_L,0,sizeof(TopDeflashFwFilter_L));
	TopDeflashFwFilter_L.tTime   = 80;		/*  ms  */
	TopDeflashFwFilter_L.TC_Time = TC_TIME;
	
	memset(&TopDeflashFwFilter_R,0,sizeof(TopDeflashFwFilter_R));
	TopDeflashFwFilter_R.tTime   = 80;		/*  ms  */
	TopDeflashFwFilter_R.TC_Time = TC_TIME;
	
	memset(&RobotBwFilter_L,0,sizeof(RobotBwFilter_L));
	RobotBwFilter_L.tTime   = 80;				/*  ms  */
	RobotBwFilter_L.TC_Time = TC_TIME;
	
	memset(&RobotBwFilter_R,0,sizeof(RobotBwFilter_R));
	RobotBwFilter_R.tTime   = 80;				/*  ms  */
	RobotBwFilter_R.TC_Time = TC_TIME;
	
	memset(&RobotFwFilter_L,0,sizeof(RobotFwFilter_L));
	RobotFwFilter_L.tTime   = 80;				/*  ms  */
	RobotFwFilter_L.TC_Time = TC_TIME;
	
	memset(&RobotFwFilter_R,0,sizeof(RobotFwFilter_R));
	RobotFwFilter_R.tTime   = 80;				/*  ms  */
	RobotFwFilter_R.TC_Time = TC_TIME;
   
	memset(&RobotUpFilter_L,0,sizeof(RobotUpFilter_L));
	RobotUpFilter_L.tTime   = 80;				/*  ms  */
	RobotUpFilter_L.TC_Time = TC_TIME;
	
	memset(&RobotUpFilter_R,0,sizeof(RobotUpFilter_R));
	RobotUpFilter_R.tTime   = 80;				/*  ms  */
	RobotUpFilter_R.TC_Time = TC_TIME;
	
	memset(&BottomDeflashFwFilter_L,0,sizeof(BottomDeflashFwFilter_L));
	BottomDeflashFwFilter_L.tTime   = 80;				/*  ms  */
	BottomDeflashFwFilter_L.TC_Time = TC_TIME;
	
	memset(&BottomDeflashFwFilter_R,0,sizeof(BottomDeflashFwFilter_R));
	BottomDeflashFwFilter_R.tTime   = 80;				/*  ms  */
	BottomDeflashFwFilter_R.TC_Time = TC_TIME;
	
	memset(&BottomDeflashBwFilter_L,0,sizeof(BottomDeflashBwFilter_L));
	BottomDeflashBwFilter_L.tTime   = 80;				/*  ms  */
	BottomDeflashBwFilter_L.TC_Time = TC_TIME;
	
	memset(&BottomDeflashBwFilter_R,0,sizeof(BottomDeflashBwFilter_R));
	BottomDeflashBwFilter_R.tTime   = 80;				/*  ms  */
	BottomDeflashBwFilter_R.TC_Time = TC_TIME;
	
	memset(&TopDeflashClsFilter_L,0,sizeof(TopDeflashClsFilter_L));
	TopDeflashClsFilter_L.tTime   = 80;				/*  ms  */
	TopDeflashClsFilter_L.TC_Time = TC_TIME;
	
	memset(&TopDeflashClsFilter_R,0,sizeof(TopDeflashClsFilter_R));
	TopDeflashClsFilter_R.tTime   = 80;				/*  ms  */
	TopDeflashClsFilter_R.TC_Time = TC_TIME;
	
	
	
	memset(&gHardWare.ModuleOk,1,sizeof(gHardWare.ModuleOk));
	memset(&gHardWare.ModuleHide,0,sizeof(gHardWare.ModuleHide));
	
	gHardWare.ModuleHide.DO9322_6 = 1;
	
	/*2018.6.21 Albert Add new limit filter of coolpin*/
	/*-------------------------------------------------*/
	memset(&CoolPinUpFilter_L,0,sizeof(CoolPinUpFilter_L));
	CoolPinUpFilter_L.tTime    = 80;	/*  ms  */
	CoolPinUpFilter_L.TC_Time = TC_TIME;
	
	memset(&CoolPinUpFilter_R,0,sizeof(CoolPinUpFilter_R));
	CoolPinUpFilter_R.tTime    = 80;	/*  ms  */
	CoolPinUpFilter_R.TC_Time = TC_TIME;
	/*-------------------------------------------------*/
	
	
	
	
	
	
	
}
/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				           ****
**************************************************************************************/
_CYCLIC void cyclic ( void )
{
	/* when machine state change from calibration to other state, force disable */
	if(0 == gTKcalib.Status && 1 == OldCalibSatus )
	{
		memset(gHardWare.DOForce,0,sizeof(gHardWare.DOForce));
		gHardWare.AOForceNum = 19;
		
	}
	OldCalibSatus = gTKcalib.Status;
	

	
	/* read sensor input value */
	gMachineInfo.OilTemp = gHardWare.ATC402_1[0]/10.0 + gT_Par.Par.SenComT[0];

	gH_IO.ActT[0] = gHardWare.ATC402_1[1]/10.0 + gT_Par.Par.SenComT[1];
	gH_IO.ActT[1] = gHardWare.ATC402_1[2]/10.0 + gT_Par.Par.SenComT[2];
	gH_IO.ActT[2] = gHardWare.ATC402_1[3]/10.0 + gT_Par.Par.SenComT[3];
	gH_IO.ActT[3] = gHardWare.ATC402_1[4]/10.0 + gT_Par.Par.SenComT[4];
	gH_IO.ActT[4] = gHardWare.ATC402_1[5]/10.0 + gT_Par.Par.SenComT[5];
   
	gH_IO.ActT[5] = gHardWare.ATC402_2[0]/10.0 + gT_Par.Par.SenComT[6];
	gH_IO.ActT[6] = gHardWare.ATC402_2[1]/10.0 + gT_Par.Par.SenComT[7];
	gH_IO.ActT[7] = gHardWare.ATC402_2[2]/10.0 + gT_Par.Par.SenComT[8];
	gH_IO.ActT[8] = gHardWare.ATC402_2[3]/10.0 + gT_Par.Par.SenComT[9];
	gH_IO.ActT[9] = gHardWare.ATC402_2[4]/10.0 + gT_Par.Par.SenComT[10];
	gH_IO.ActT[10]= gHardWare.ATC402_2[5]/10.0 + gT_Par.Par.SenComT[11];
   
    gH_IO.ActT[11] = gHardWare.ATC402_3[0]/10.0 + gT_Par.Par.SenComT[12];
	gH_IO.ActT[12] = gHardWare.ATC402_3[1]/10.0 + gT_Par.Par.SenComT[13];
	gH_IO.ActT[13] = gHardWare.ATC402_3[2]/10.0 + gT_Par.Par.SenComT[14];
	gH_IO.ActT[14] = gHardWare.ATC402_3[3]/10.0 + gT_Par.Par.SenComT[15];
	gH_IO.ActT[15] = gHardWare.ATC402_3[4]/10.0 + gT_Par.Par.SenComT[16];
	gH_IO.ActT[16] = gHardWare.ATC402_3[5]/10.0 + gT_Par.Par.SenComT[17];

	gH_IO.ActT[17] = gHardWare.ATC402_4[0]/10.0 + gT_Par.Par.SenComT[18];
	gH_IO.ActT[18] = gHardWare.ATC402_4[1]/10.0 + gT_Par.Par.SenComT[19];
	gH_IO.ActT[19] = gHardWare.ATC402_4[2]/10.0 + gT_Par.Par.SenComT[20];
	gH_IO.ActT[20] = gHardWare.ATC402_4[3]/10.0 + gT_Par.Par.SenComT[21];
	gH_IO.ActT[21] = gHardWare.ATC402_4[4]/10.0 + gT_Par.Par.SenComT[22];
	gH_IO.ActT[22] = gHardWare.ATC402_4[5]/10.0 + gT_Par.Par.SenComT[23];
	
	gH_IO.ActT[23] = gHardWare.ATC402_5[0]/10.0 + gT_Par.Par.SenComT[24];
	gH_IO.ActT[24] = gHardWare.ATC402_5[1]/10.0 + gT_Par.Par.SenComT[25];
	gH_IO.ActT[25] = gHardWare.ATC402_5[2]/10.0 + gT_Par.Par.SenComT[26];
	gH_IO.ActT[26] = gHardWare.ATC402_5[3]/10.0 + gT_Par.Par.SenComT[27];
	gH_IO.ActT[27] = gHardWare.ATC402_5[4]/10.0 + gT_Par.Par.SenComT[28];
	gH_IO.ActT[28] = gHardWare.ATC402_5[5]/10.0 + gT_Par.Par.SenComT[29];
	
	gH_IO.ActT[29] = gHardWare.ATC402_6[0]/10.0 + gT_Par.Par.SenComT[30];
	gH_IO.ActT[30] = gHardWare.ATC402_6[1]/10.0 + gT_Par.Par.SenComT[31];
	gH_IO.ActT[31] = gHardWare.ATC402_6[2]/10.0 + gT_Par.Par.SenComT[32];
	gH_IO.ActT[32] = gHardWare.ATC402_6[3]/10.0 + gT_Par.Par.SenComT[33];

	
	/*gHardWare.ATC402_4[3]/10.0*/
	/*gHardWare.ATC402_4[4]/10.0*/
	
	//	gExtruderA.MeltTemp = gHardWare.ATC402_3[5]/10.0 + gT_Par.Par.SenComT[17]; 
		
	//	gMachineInfo.TempHopper = gHardWare.ATC402_4[0]/10.0 + gT_Par.Par.SenComT[18];
	//	gMachineInfo.TempExtrCoolWater = gHardWare.ATC402_4[1]/10.0 + gT_Par.Par.SenComT[19];
	
	/* --------------------- Good Product Yield  --------------------- */
	if((1 == gMachineIn.ProducutCnt) && (0 == ProductCntOld))
	{
		gMachineInfo.GoodBottles = gMachineInfo.GoodBottles + 1;
	}
	ProductCntOld = gMachineIn.ProducutCnt;
	

	
	
	/* ----------------------- digital input ----------------------- */
	/*---------X20DIF371-1-----------*/
//	gMachineIn.InstancyStop			= !gHardWare.DIF371_1[0];   /* Emergency stop - Normally Close	 */
	gMachineIn.AutoEnable			= gHardWare.DIF371_1[0]; 
	gMachineIn.CalibModeForceDI		= gHardWare.DIF371_1[1]; 
	gMachineIn.AutoStop             = gPanelObj.KeyMatric[S$KEYMATRIX_AUTO_STOP];  //PPC 2100 Key
//		gMachineOut.OilPump             = gPanelObj.KeyMatric[S$KEYMATRIX_MAIN_PUMP];  //PPC 2100 Key
		
		if (0 == gMachineInfo.Auto)
	{
		//		if((0 == gHardWare.DIF371_1[2]) && (0 == gHardWare.DIF371_1[3]))
		//		{
		//			gMacOption.Station = 2;/*Both Mold*/
		//		}
		//		else if((1 == gHardWare.DIF371_1[2]) && (0 == gHardWare.DIF371_1[3])) 
		//		{
		//			gMacOption.Station = 0;/*left Mold*/
		//		}
		//		else if((0 == gHardWare.DIF371_1[2]) && (1 == gHardWare.DIF371_1[3])) 
		//		{
		//			gMacOption.Station = 1;/*Right mold*/
		//		}
		gMacOption.Station = 1;/*Right mold*/
	}	
		
	//	0	= gHardWare.DIF371_1[2];
	//	0	= gHardWare.DIF371_1[3];
	
	gMachineIn.SafeGate_R1			= gHardWare.DIF371_1[4];
	if(0 == gMachineIn.CalibModeForceDI)
	{
		gMachineIn.SafeGate	 			=  gMachineIn.SafeGate_R1;/*安全門-磁力鎖（多DI）*/
	}
	else
	{
		gMachineIn.SafeGate = 1;
	}
	//		0						= gHardWare.DIF371_1[5];
	gMachineIn.SafeLogicValve		= !gHardWare.DIF371_1[6];
	gMachineIn.SafeCalibPressure	= !gHardWare.DIF371_1[7];

	
	gMachineIn.RMold.AirPressureLow	= !gHardWare.DIF371_1[8];
	gMachineIn.ExtruderNormal		= !gHardWare.DIF371_1[9];
	//  0 	 						= gHardWare.DIF371_1[10];
	gMachineIn.SafeModule			= !gHardWare.DIF371_1[11];
	//	0							= gHardWare.DIF371_1[12];
	// 	0							= gHardWare.DIF371_1[13];
	gMachineIn.RMold.HookPinFwLimit	= gHardWare.DIF371_1[14];
	gMachineIn.RMold.HookPinBwLimit	= gHardWare.DIF371_1[15];
	
	
	//	gMachineIn.OilPump2Overload 	= gHardWare.DIF371_1[9];
	//	gMachineIn.ExtrLiftInverterFault= gHardWare.DIF371_1[9]; 			// extruder lift up limit
	//	gMachineIn.ExtrLiftInverterFault= gHardWare.DIF371_1[10]; 			// extruder lift up limit
	//	gMachineIn.ParisonPumpMotorErr	= gHardWare.DIF371_1[10];
	//	gMachineIn.ParisonPumpMotorErr	= gHardWare.DIF371_1[11];
	//	gMachineIn.ExtrA_HopperErr		= gHardWare.DIF371_1[11];
	//	gMachineIn.ExtrA_Wire_MotorErr	= gHardWare.DIF371_1[12];
	//	gMachineIn.ExtrD_Wire_MotorErr	= gHardWare.DIF371_1[13];
	//	gMachineIn.ExtrV_Wire_MotorErr	= gHardWare.DIF371_1[14];
	//	gMachineIn.WaterPressureLow		= gHardWare.DIF371_1[12];
	//	gMachineIn.NoFeed				= gHardWare.DIF371_1[13];
	//	/*Parsion*/
	//	gMachineIn.ParisonPumpFilter  =gHardWare.DIF371_1[14];		
	//	ParisonPumpFilter.In   = gMachineIn.ParisonPumpFilter;	
	//	DIFilter(&ParisonPumpFilter);
	//	gAlarm.ParisonPumpFilter    = ParisonPumpFilter.Out;
	

	
	//	gMachineIn.NoFeed  				= gHardWare.DIF371_1[8];
	//	gMachineIn.ProductAuxClampLimit = gHardWare.DIF371_1[9];
	//	gMachineIn.ExtruderLiftUpLimit	= gHardWare.DIF371_1[10]; 			// extruder lift up limit
	//	gMachineIn.ExtruderLiftDnLimit	= gHardWare.DIF371_1[11];
	//	gMachineIn.PlateUpLimit			= gHardWare.DIF371_1[12];
	//	gMachineIn.PlateDnLimit			= gHardWare.DIF371_1[13];
	//	gMachineIn.PreHeatDI			= gHardWare.DIF371_1[14];
	
	//	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone))
	//	{
	//		gPanelObj.VKeyMatric[S$KEYMATRIX_HANDLE_PUNCHER]=	gHardWare.DIF371_1[15];
	//	}
	/*---------X20DIF371-2-----------*/	
	gMachineIn.ExtrA_Wire_MotorErr	= gHardWare.DIF371_2[0];
	gMachineIn.ExtrD_Wire_MotorErr 	= gHardWare.DIF371_2[1];
	gMachineIn.ExtrV_Wire_MotorErr	= gHardWare.DIF371_2[2];
//	gMachineIn.ExtrB_Wire_MotorErr	= !gHardWare.DIF371_2[3];
//	gMachineIn.ExtrC_Wire_MotorErr	= !gHardWare.DIF371_2[4];
	gMachineIn.OilPumpErr	  		= !gHardWare.DIF371_2[5];
	gMachineIn.ParisonPumpMotorErr	= !gHardWare.DIF371_2[6];
	gMachineIn.ExtrLiftInverterFault = !gHardWare.DIF371_2[7];

	
	gMachineIn.ExtruderLiftUpLimit	= gHardWare.DIF371_2[8];
	gMachineIn.ExtruderLiftDnLimit	= gHardWare.DIF371_2[9];
	gMachineIn.PlateUpLimit			= gHardWare.DIF371_2[10];
	gMachineIn.PlateDnLimit			= gHardWare.DIF371_2[11];
	gMachineIn.RMold.ExtInMoldStickerBwLimit	= gHardWare.DIF371_2[12];
	gMachineIn.RMold.ExtInMoldStickerAlarm		= !gHardWare.DIF371_2[13];
//	0	= !gHardWare.DIF371_2[14];
	gMachineIn.TempFanErr			= !gHardWare.DIF371_2[15];

	//	gMachineIn.ExtruderLiftUpLimit 				= gHardWare.DIF371_2[12];
	//	gMachineIn.ExtruderLiftDnLimit 				= gHardWare.DIF371_2[13];
	//	gMachineIn.PlateUpLimit						= gHardWare.DIF371_2[14];
	//	gMachineIn.PlateDnLimit 					= gHardWare.DIF371_2[15];

		
	
	//	gMachineIn.ExtrA_MotorErr		= gHardWare.DIF371_2[0] || gExtruderACtrl.InverterStatus.bFault;
	//	gMachineIn.ExtrB_MotorErr		= gHardWare.DIF371_2[1] || gExtruderBCtrl.InverterStatus.bFault;
	//	gMachineIn.ExtrC_MotorErr		= gHardWare.DIF371_2[2] || gExtruderCCtrl.InverterStatus.bFault;
	//	gMachineIn.ExtrD_MotorErr		= gHardWare.DIF371_2[3] || gExtruderVCtrl.InverterStatus.bFault;
	//	gAlarm.OilPumpErr				= gHardWare.DIF371_2[4];
	//	gMachineIn.OilPump2Overload		= gHardWare.DI9371_2[5];
	//	gMachineIn.ExtrLiftInverterFault= gHardWare.DIF371_2[6];	
	//	gMachineIn.ParisonPumpMotorErr 	=gHardWare.DIF371_2[7];
	//
	//	/*Parsion*/
	//	gMachineIn.ParisonPumpFilter  =gHardWare.DIF371_2[8];		
	//	ParisonPumpFilter.In   = gMachineIn.ParisonPumpFilter;	
	//	DIFilter(&ParisonPumpFilter);
	//	gAlarm.ParisonPumpFilter    = ParisonPumpFilter.Out;
	//
	//	gMachineIn.LMold.BottomDeflashFwLimit = gHardWare.DIF371_2[9];
	//	gMachineIn.LMold.BottomDeflashBwLimit =	gHardWare.DIF371_2[10];
	//	gMachineIn.RMold.BottomDeflashFwLimit =	gHardWare.DIF371_2[11];
	//	gMachineIn.RMold.BottomDeflashBwLimit =	gHardWare.DIF371_2[12];
	//	gMachineIn.LMold.TopDeflashOpnLimit   =	gHardWare.DIF371_2[13];
	//	gMachineIn.RMold.TopDeflashOpnLimit   =	gHardWare.DIF371_2[14];
	//	gMachineIn.OilPumpOk_DI=gHardWare.DIF371_2[15];
	
	/*---------X20DIF371-3-------------*/
	
	gMachineIn.RMold.ClampOpnLimit	 	= gHardWare.DIF371_3[0];
	gMachineIn.RMold.CarriageFwLimit 	= gHardWare.DIF371_3[1];
	gMachineIn.RMold.CarriageBwLimit 	= gHardWare.DIF371_3[2];
	gMachineIn.RMold.CarriageSlowFwLimit= gHardWare.DIF371_3[3];
	gMachineIn.RMold.CarriageSlowBwLimit= gHardWare.DIF371_3[4];	
	gMachineIn.RMold.BlowPinUpLimit  	= gHardWare.DIF371_3[5];
	gMachineIn.RMold.BlowPinUpLimit_2	= gHardWare.DIF371_3[6];
//	gMachineIn.RMold.BottomDeflashFwLimit	= gHardWare.DIF371_3[7];
	
	gMachineIn.RMold.TopDeflashFwLimit  = gHardWare.DIF371_3[8];
	gMachineIn.RMold.TopDeflashBwLimit	= gHardWare.DIF371_3[9];
	gMachineIn.RMold.RobotFwLimit		= gHardWare.DIF371_3[10];
	gMachineIn.RMold.RobotBwLimit  		= gHardWare.DIF371_3[11];
	gMachineIn.RMold.RobotOpnLimitL		= gHardWare.DIF371_3[12];
	gMachineIn.RMold.RobotOpnLimitR		= gHardWare.DIF371_3[13];
//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_3[14];
//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_3[15];
	
	
	//	CarriageBwFilter_L.In 				= gHardWare.DIF371_3[3];
	//	DIFilter(&CarriageBwFilter_L);
	//	gMachineIn.LMold.CarriageBwLimit    = CarriageBwFilter_L.Out;

	//	TopDeflashFwFilter_L.In				= gHardWare.DIF371_3[4];
	//	DIFilter(&TopDeflashFwFilter_L);
	//	gMachineIn.LMold.TopDeflashFwLimit  = TopDeflashFwFilter_L.Out;
	//	gMachineIn.LMold.TopDeflashFwLimit	= gHardWare.DIF371_3[4];
		
	//	TopDeflashBwFilter_L.In				= gHardWare.DIF371_3[5];
	//	DIFilter(&TopDeflashBwFilter_L);
	//	gMachineIn.LMold.TopDeflashBwLimit  = TopDeflashBwFilter_L.Out;	
	//	gMachineIn.LMold.TopDeflashBwLimit  = gHardWare.DIF371_3[5];
	
	//	gMachineIn.LMold.RobotFwLimit		= gHardWare.DIF371_3[6];
	//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_3[7];
	
	//	gMachineIn.LMold.RobotOpnLimitL		= gHardWare.DIF371_3[8];
	//	gMachineIn.LMold.RobotOpnLimitR		= gHardWare.DIF371_3[9];
	
	//	gMachineIn.LMold.CoolPinUpLimit		= gHardWare.DIF371_3[10];
	//	gMachineIn.LMold.CoolPin2UpLimit	= gHardWare.DIF371_3[11];
	//	gMachineIn.LMold.CoolPin3UpLimit	= gHardWare.DIF371_3[12];
	//	gMachineIn.LMold.SubMoldClsLimit		= gHardWare.DIF371_3[13];
	//	gMachineIn.LMold.BottomSubMoldClsLimit	= gHardWare.DIF371_3[13]; //ipis1204
	//	gMachineIn.LMold.ClampOpnLimit		= gHardWare.DIF371_3[14];
	//	gMachineIn.LMold.ExtRobotUpLimit 		= gHardWare.DIF371_3[15];
	
	//	gMachineIn.LMold.RobotBwLimit       = gMachineIn.LMold.RobotOrigin;/*Chaoi*/		
	
	/*---------X20DIF371-4-------------*/
	
	//	gMachineIn.LMold.BlowPinUpLimit 	= gHardWare.DIF371_4[0];
	//	gMachineIn.LMold.BlowPin2UpLimit	= gHardWare.DIF371_4[1];
	//	gMachineIn.LMold.CarriageFwLimit	= gHardWare.DIF371_4[2];
	//	gMachineIn.LMold.CarriageBwLimit	= gHardWare.DIF371_4[3];
	//	gMachineIn.LMold.ClampOpnLimit  	= gHardWare.DIF371_4[4];	
	//	gMachineIn.LMold.TopDeflashFwLimit  = gHardWare.DIF371_4[5];
	//	gMachineIn.LMold.TopDeflashBwLimit	= gHardWare.DIF371_4[6];
	//	gMachineIn.LMold.BottomDeflashFwLimit	= gHardWare.DIF371_4[7];
	//	
	//	gMachineIn.LMold.BottomDeflashBwLimit  = gHardWare.DIF371_4[8];
	//	gMachineIn.LMold.SubMoldClsLimit	= gHardWare.DIF371_4[9];
	//	gMachineIn.LMold.ExtRobotUpLimit	= gHardWare.DIF371_4[10];
	//	gMachineIn.LMold.TopDeflashBwLimit  = gHardWare.DIF371_4[11];
	//	gMachineIn.LMold.RobotFwLimit		= gHardWare.DIF371_4[12];
	//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_4[13];
	//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_4[14];
	//	gMachineIn.LMold.RobotBwLimit		= gHardWare.DIF371_4[15];
	
	//	gMachineIn.RMold.BlowPinUpLimit = gHardWare.DIF371_4[0];
	//	gMachineIn.RMold.BlowPinDnLimit	= gHardWare.DIF371_4[1];
	//	gMachineIn.RMold.AirPressureLow	= gHardWare.DIF371_4[1];
	
	//	CarriageFwFilter_R.In				= gHardWare.DIF371_4[2];
	//	DIFilter(&CarriageFwFilter_R);
	//	gMachineIn.RMold.CarriageFwLimit    = CarriageFwFilter_R.Out;
	
	//	CarriageBwFilter_R.In 				= gHardWare.DIF371_4[3];
	//	DIFilter(&CarriageBwFilter_R);
	//	gMachineIn.RMold.CarriageBwLimit    = CarriageBwFilter_R.Out;
	
	//	TopDeflashFwFilter_R.In				= gHardWare.DIF371_4[4];
	//	DIFilter(&TopDeflashFwFilter_R);
	//	gMachineIn.RMold.TopDeflashFwLimit  = TopDeflashFwFilter_R.Out;
	//	gMachineIn.RMold.TopDeflashFwLimit	= gHardWare.DIF371_4[4];
	
	//	TopDeflashBwFilter_R.In				= gHardWare.DIF371_4[5];
	//	DIFilter(&TopDeflashBwFilter_R);
	//	gMachineIn.RMold.TopDeflashBwLimit  = TopDeflashBwFilter_R.Out;	
	//	gMachineIn.RMold.TopDeflashBwLimit	= gHardWare.DIF371_4[5];
	
	//	gMachineIn.RMold.RobotFwLimit		= gHardWare.DIF371_4[6];
	//	gMachineIn.RMold.RobotBwLimit		= gHardWare.DIF371_4[7];
	
	//	gMachineIn.RMold.RobotOpnLimitL		= gHardWare.DIF371_4[8];
	//	gMachineIn.RMold.RobotOpnLimitR		= gHardWare.DIF371_4[9];
	
	//	gMachineIn.RMold.CoolPinUpLimit		= gHardWare.DIF371_4[10];
	//	gMachineIn.RMold.CoolPin2UpLimit	= gHardWare.DIF371_4[11];
	//	gMachineIn.RMold.CoolPin3UpLimit	= gHardWare.DIF371_4[12];
	//	gMachineIn.RMold.BottomSubMoldClsLimit			= gHardWare.DIF371_4[13];
	//	gMachineIn.RMold.SubMoldClsLimit			= gHardWare.DIF371_4[13];

	//	gMachineIn.RMold.ClampOpnLimit		= gHardWare.DIF371_4[14];	
	//	gMachineIn.RMold.ExtRobotUpLimit 	= gHardWare.DIF371_4[15];
	
	//	gMachineIn.RMold.RobotBwLimit		= gMachineIn.RMold.RobotOrigin; /*Chaoi*/
	
	//	gHardWare.DIF371_4[15];
	//	/*CoolPin*/
	//	DIFilter(&CoolPinUpFilter_R);
	//	gMachineIn.RMold.CoolPinUpLimit		= CoolPinUpFilter_R.Out;
	//	
	//	DIFilter(&CoolPinUpFilter_L);
	//	gMachineIn.LMold.CoolPinUpLimit		= CoolPinUpFilter_L.Out;
	
	/* --------------------  DI9371-1  -------------------- */
	//	gMachineIn.ExtrA_MotorErr 			= !gHardWare.DI9371_1[0];	/* A extruder motor fault */
	//	gMachineIn.OilPumpMotorErr 	   = !gHardWare.DI9371_1[1];	/* Main pump motor fault */
	//	gMachineIn.ExtrB_MotorErr 			= !gHardWare.DI9371_1[2];	/* B extruder motor fault */
	//	gMachineIn.OilPumpOnoff          = gHardWare.DI9371_1[3];
	//	/*   = gHardWare.DI9371_1[4];*/
	//	gMachineIn.ParisonPumpMotorErr   = gHardWare.DI9371_1[5];
	//	gMachineIn.AutoEnable				= gHardWare.DI9371_1[6];	/* Auto start   */
	//	gMachineIn.InstancyStop				= !gHardWare.DI9371_1[7];	/* instancy stop */
	//	gPanelObj.MKeyMatric[S$KEYMATRIX_PLATE_UP] = gHardWare.DI9371_1[8];
	//	gPanelObj.MKeyMatric[S$KEYMATRIX_PLATE_DN] = gHardWare.DI9371_1[9];
	//	/*   = gHardWare.DI9371_1[10];*/
	//	gMachineIn.SafeGate        		= gHardWare.DI9371_1[11];	/* safety gate */
	
	/* --------------------  DI9371-2  -------------------- */
	//	gMachineIn.LMold.btnBlowPinUp 		= gHardWare.DI9371_2[0];
	//	gMachineIn.LMold.btnBlowPinDn 		= gHardWare.DI9371_2[1];
	//	gMachineIn.RMold.btnBlowPinUp 		= gHardWare.DI9371_2[2];
	//	gMachineIn.RMold.btnBlowPinDn 		= gHardWare.DI9371_2[3];
	//	gMachineIn.LMold.ClampOpnLimit     	= gHardWare.DI9371_2[4];
	//	gMachineIn.LMold.ClampOpnSlowLimit 	= gHardWare.DI9371_2[5];
	//	gMachineIn.LMold.ClampClsSlowLimit 	= gHardWare.DI9371_2[6];
	//	gMachineIn.LMold.ClampClsLimit     	= gHardWare.DI9371_2[7];
	//	CarriageBwFilter_L.In               = gHardWare.DI9371_2[8];	/* left carriage backward limit */
	//	DIFilter(&CarriageBwFilter_L);
	//	gMachineIn.LMold.CarriageBwLimit  	= CarriageBwFilter_L.Out;
	//	CarriageFwFilter_L.In             	= gHardWare.DI9371_2[9];	/* left carriage forkward limit */
	//	DIFilter(&CarriageFwFilter_L);
	//	gMachineIn.LMold.CarriageFwLimit  	= CarriageFwFilter_L.Out;
	//	gMachineIn.LMold.BlowPinUpLimit   	= gHardWare.DI9371_2[10]; 	/* left blow pin up limit */
	//	TopDeflashBwFilter_L.In           	= gHardWare.DI9371_2[11];	/* left top deflash backward limit */
	//	DIFilter(&TopDeflashBwFilter_L);
	//	gMachineIn.LMold.TopDeflashBwLimit	= TopDeflashBwFilter_L.Out;
	
	/* --------------------  DI9371-3  -------------------- */
	//	gMachineIn.RMold.ClampOpnLimit     	= gHardWare.DI9371_3[0];
	//	gMachineIn.RMold.ClampOpnSlowLimit 	= gHardWare.DI9371_3[1];
	//	gMachineIn.RMold.ClampClsSlowLimit 	= gHardWare.DI9371_3[2];
	//	gMachineIn.RMold.ClampClsLimit     	= gHardWare.DI9371_3[3];
	//	CarriageBwFilter_R.In             	= gHardWare.DI9371_3[4];	/* right carriage backward limit */
	//	DIFilter(&CarriageBwFilter_R);
	//	gMachineIn.RMold.CarriageBwLimit  	= CarriageBwFilter_R.Out;
	//	CarriageFwFilter_R.In             	= gHardWare.DI9371_3[5];	/* right carriage forkward limit */
	//	DIFilter(&CarriageFwFilter_R);
	//	gMachineIn.RMold.CarriageFwLimit  	= CarriageFwFilter_R.Out;
	//	gMachineIn.RMold.BlowPinUpLimit   	= gHardWare.DI9371_3[6]; 	/* right blow pin up limit */
	//	TopDeflashBwFilter_R.In           	= gHardWare.DI9371_3[7];	/* top deflash backward limit */
	//	DIFilter(&TopDeflashBwFilter_R);
	//	gMachineIn.RMold.TopDeflashBwLimit	= TopDeflashBwFilter_R.Out;
	//	gMachineIn.RMold.SubMoldOpnLimit  	= gHardWare.DI9371_3[8];
	//	gMachineIn.RMold.SubMoldClsLimit  	= gHardWare.DI9371_3[9];
	//	gMachineIn.LMold.SubMoldOpnLimit  	= gHardWare.DI9371_3[10];
	//	gMachineIn.LMold.SubMoldClsLimit  	= gHardWare.DI9371_3[11];

	/* --------------------  DI9371-4  -------------------- */
	//	RobotFwFilter_L.In           			= gHardWare.DI9371_4[0];	/* left robot forward limit */
	//	DIFilter(&RobotFwFilter_L);
	//	gMachineIn.LMold.RobotFwLimit    	= RobotFwFilter_L.Out;
	//	RobotBwFilter_L.In           			= gHardWare.DI9371_4[1];	/* left robot backward limit */
	//	DIFilter(&RobotBwFilter_L);
	//	gMachineIn.LMold.RobotBwLimit    	= RobotBwFilter_L.Out;
	//	RobotUpFilter_L.In           			= gHardWare.DI9371_4[2];	/* left robot upward limit */
	//	DIFilter(&RobotUpFilter_L);
	//	gMachineIn.LMold.RobotUpLimit    	= RobotUpFilter_L.Out;		
	//	
	//	RobotFwFilter_R.In           			= gHardWare.DI9371_4[3];	/* right robot forward limit */
	//	DIFilter(&RobotFwFilter_R);
	//	gMachineIn.RMold.RobotFwLimit    	= RobotFwFilter_R.Out;
	//	RobotBwFilter_R.In           			= gHardWare.DI9371_4[4];	/* right robot backward limit */
	//	DIFilter(&RobotBwFilter_R);
	//	gMachineIn.RMold.RobotBwLimit    	= RobotBwFilter_R.Out;
	//	RobotUpFilter_R.In           			= gHardWare.DI9371_4[5];	/* right robot upward limit */
	//	DIFilter(&RobotUpFilter_R);
	//	gMachineIn.RMold.RobotUpLimit    	= RobotUpFilter_R.Out;	
	
	/* ----------------------- left and right has same sensor ----------------------- */
	gMachineIn.LMold.SafeGate = gMachineIn.SafeGate;
	gMachineIn.RMold.SafeGate = gMachineIn.SafeGate;

	//	gMachineIn.LMold.ParisonLength = gMachineIn.ParisonLength;
	//	gMachineIn.RMold.ParisonLength = gMachineIn.ParisonLength;

	//	gMachineIn.AutoStop = gMachineIn.AutoEnable;
	/* ----------------------- no install those sensor ----------------------- */
	gMachineIn.PneuForceNormal      = 1;
	gMachineIn.HeatingPowerNormal	= 1;	/* 加熱電源正常			*/
	gMachineIn.Voltage24vNormal     = 1;	/* 控制電源24V正常	  	*/
	gMachineIn.WaterPressureLow		= 0;	/* 水壓過低				  */
	gMachineIn.RMold.LubricNormal   = 1;	/* 右潤滑油正常 		 */
	gMachineIn.LMold.LubricNormal   = 1;	/* 左潤滑油正常 		 */
	gMachineIn.ExtrA_LubrNormal     = 1;	/* 忽視A押出機潤滑檢測 */
	gMachineIn.ExtrA_ColiTempNormal = 1;
	gMachineIn.ExtrB_LubrNormal     = 1;	/* 忽視B押出機潤滑檢測 */
	gMachineIn.ExtrB_ColiTempNormal = 1;

//	gMachineIn.OilPumpErr			= 0;
//	gMachineIn.ExtrLiftInverterFault = 0;
//	gMachineIn.ParisonPumpMotorErr = 0;
	
	gMachineIn.OilPumpFilter      = 1;
	gMachineIn.CoolingPumpFilter  = 1;
	gMachineIn.TonnagePumpFilter  = 1;
	gMachineIn.OilPositionNormal  = 1;
	
	gMachineIn.ParisonPumpEnable  = 1;
	gMachineIn.OilPumpEnable      = 1;
	
	gMachineIn.CoolingPumpEnable  = 1;
	gMachineIn.CoolingPumpRunning = 1;
	gMachineOut.OilCoolingPump 	  = 1;
	
	gMachineIn.LMold.RobotDnLimit = 1;
	gMachineIn.RMold.RobotDnLimit = 1;

	gMachineIn.LubriFilter        = 0;		/* 润滑泵堵塞  */
	gMachineIn.LubriOverload      = 0;		/* 润滑泵过载   */
	gMachineIn.ExtrA_CoolMotorErr = 0;		/* 1机风机故障 */
	gMachineIn.ExtrB_CoolMotorErr = 0;		/* 2机风机故障 */
	gMachineIn.ExtrV_CoolMotorErr = 0;		/* 透机风机故障 */
    
	gMachineIn.LowerPressureHigh = 0;			/* 压力开关高位 */
	gMachineIn.LowerPressureLow  = 0;			/* 压力开关低位 */
	gMachineIn.MainControl 		 = 1;      		/* 主控钥匙开关 */
	
	
	gMachineIn.LMold.AirPressureLow = 0;	/*風壓異常*/
//	gMachineIn.RMold.AirPressureLow = 0;	/*風壓異常*/
	
	
	gMachineIn.LMold.ClampClsLimit = 1;	
	gMachineIn.RMold.ClampClsLimit = 1;	
	gMachineIn.LMold.ClampOpnMicroLimit =1;
	gMachineIn.RMold.ClampOpnMicroLimit =1;
	
	

	
	gMachineIn.RMold.BlowPinDnLimit  = 1;
	gMachineIn.LMold.BlowPinDnLimit  = 1;
	
	gMachineIn.RMold.BlowPin2DnLimit  = 1;
	gMachineIn.LMold.BlowPin2DnLimit  = 1;
	
	gMachineIn.LMold.SealerOpnLimit = 1;
	gMachineIn.RMold.SealerOpnLimit = 1;
	

	
//	gMachineIn.LMold.RobotOpnLimitL = 1;
//	gMachineIn.LMold.RobotOpnLimitR = 1;
//	gMachineIn.RMold.RobotOpnLimitL = 1;
//	gMachineIn.RMold.RobotOpnLimitR = 1;
		
	gMachineIn.OilPumpOk_DI = 1;

	
	
	/* ----------------------- HardWare Option ----------------------- */
	
	if ( 1 == gMachineFix.Option.bHideLeftStation )
	{
		gMachineFix.Option.HWEnable.LMold.BlowPin2 			= 0;
		gMachineFix.Option.HWEnable.LMold.TopDeflash 		= 0;
		gMachineFix.Option.HWEnable.LMold.BottomDeflash		= 0;
		gMachineFix.Option.HWEnable.LMold.PinPart			= 0;
		gMachineFix.Option.HWEnable.LMold.CoolPin			= 0;
		gMachineFix.Option.HWEnable.LMold.CoolPin2			= 0;
		gMachineFix.Option.HWEnable.LMold.CoolPin3			= 0;
		gMachineFix.Option.HWEnable.LMold.InnerPin			= 0;
		gMachineFix.Option.HWEnable.LMold.CutNeck			= 0;	
		gMachineFix.Option.HWEnable.LMold.SubMold			= 0;
		gMachineFix.Option.HWEnable.LMold.BottomSubmold		= 0;
		gMachineFix.Option.HWEnable.LMold.RobotTransfer		= 0;
		gMachineFix.Option.HWEnable.LMold.RobotUpDn			= 0;
		gMachineFix.Option.HWEnable.LMold.ExtRobot			= 0;
		gMachineFix.Option.HWEnable.LMold.PullBottom		= 0;
		gMachineFix.Option.HWEnable.LMold.HookPin			= 0;
	}
	
	if( ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType)
	{
		gMachineFix.Option.HWEnable.RMold.BlowPin2 			= 0;
		gMachineFix.Option.HWEnable.LMold.BlowPin2 			= 0;
	}
	
	/* ----------------General------------------- */
	
	/* ------- 抬頭/加壓馬達 ------- */
	if ( 0 == gMachineFix.Option.HWEnable.HighPrsMotor )
	{
		gMacOption.HighPrsMotorEnable = 0;
	}
	
	/* ------- 系統蓄壓器 ------- */
	if ( 0 == gMachineFix.Option.HWEnable.ACCPump )
	{
		gMacOption.ACCPumpCharge = 0;
	}
	
	/* ------- 抬頭蓄壓器 ------- */
	if ( 0 == gMachineFix.Option.HWEnable.ACCPump_Extrlift )
	{
		gMacOption.ACCPumpCharge_Extrlift = 0;
	}
	
	if ( 0 == gMachineFix.Option.bSupportAirVentTimeVersion )
	{
		gUserPara.SupportAirVentTimeVersionEnable = 0;
	}
	
	/* ----------------RMold------------------- */
	
	/* ---------BlowPin2 for Electric--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.BlowPin2 )
	{
		gMacOption.RBlowPin = 1;
		gMacOption.RBlowPin2 = 0;
		gMachineIn.RMold.BlowPin2UpLimit = 1; 

		RMold.BlowPin2.Error = 0;
		RMold.BlowPin2.CalOK = 1;
		RMold.BlowPin2.UpPos = 1;
	}
	
	/* ---------TopDeflash--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.TopDeflash )
	{
		gMacOption.RTopDeflash = 0;
		gMachineIn.RMold.TopDeflashBwLimit = 1;
		
		gMachineFix.Option.HWEnable.RMold.PunchHandle = 0;  // Disable PunchHandle(no TopDeflash, no PunchHandle)
	}

	
	/* ---------PunchHandle--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.PunchHandle)
	{
		gMacOption.RPunchHandle = 0;
		gMachineIn.RMold.TopDeflashOpnLimit = 1;
	}
	
	/* ---------BottomDeflash--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.BottomDeflash )
	{
		gMacOption.RBottomDeflash = 0;
		gMachineIn.RMold.BottomDeflashFwLimit =	1;
		gMachineIn.RMold.BottomDeflashBwLimit = 1;
	}
	//	else
	//	{
	//		gMachineFix.Option.HWEnable.RMold.PullBottom = 0;   //Interlock BottomDeflash
	//	}
	/* ---------PinPart--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.PinPart )
	{
		gMacOption.RPinPart = 0;
		gMachineIn.RMold.PinPartFwLimit = 1;
		gMachineIn.RMold.PinPartBwLimit = 1;
	}
	
	/* ---------CoolPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.CoolPin )
	{
		gMacOption.RCoolPin = 0;
		gMachineIn.RMold.CoolPinUpLimit = 1;
	}
	
	/* ---------CoolPin2--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.CoolPin2 )
	{
		gMacOption.RCoolPin2 = 0;
		gMachineIn.RMold.CoolPin2UpLimit = 1;
	}
	
	/* ---------CoolPin3--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.CoolPin3 )
	{
		gMacOption.RCoolPin3 = 0;
		gMachineIn.RMold.CoolPin3UpLimit = 1;
	}
	
	/* ---------InnerPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.InnerPin )
	{
		gMacOption.RInnerPin = 0;
		gMachineIn.RMold.InnerPinBwLimit = 1;
	}
	
	/* ---------CutNeck--------- */ /* 旋切瓶口  */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.CutNeck )
	{
		gMacOption.RCutNeck = 0;
		gMachineIn.RMold.CutNeckOpnLimit = 1;
	}
	
	/* ---------SubMold--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.SubMold )
	{
		gMacOption.RSubMold = 0;
		gMachineIn.RMold.SubMoldClsLimit = 1;
		gMachineIn.RMold.SubMoldOpnLimit = 1;
	}

	/* ---------BottomSubmold--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.BottomSubmold )
	{
		gMacOption.RBottomSubmold = 0;
		gMachineIn.RMold.BottomSubMoldClsLimit = 1;
		gMachineIn.RMold.BottomSubMoldOpnLimit = 1;
	}
	
	/* ---------RobotTransfer--------- */
	gMachineFix.Option.HWEnable.RMold.RobotOpnCls = gMachineFix.Option.HWEnable.RMold.RobotTransfer;  // 機械手開閉強制同動
	gMacOption.RRobotOpnCls = gMacOption.RRobotTransfer;									 		  // 機械手開閉強制同動
	if ( 0 == gMachineFix.Option.HWEnable.RMold.RobotTransfer )
	{
		gMachineFix.Option.HWEnable.RMold.RobotOpnCls = 0;
		gMacOption.RRobotTransfer = 0;
		gMachineIn.RMold.RobotBwLimit  = 1;
		
		RMold.Robot.Transfer.Error = 0;
		RMold.Robot.Transfer.BwPos = 1;
		RMold.Robot.Transfer.CalOK = 1;
	}
	
	/* ---------RobotOpnCls--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.RobotOpnCls )
	{
		gMacOption.RRobotOpnCls = 0;
		gMachineIn.RMold.RobotOpnLimit = 1;
	}
	
	/* ---------RobotUpDn--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.RobotUpDn )
	{
		gMacOption.RRobotUpDn = 0;
		gMachineIn.RMold.RobotUpLimit  = 1;
		RMold.Robot.UpDn.Error = 0;
		RMold.Robot.UpDn.UpPos = 1;
		RMold.Robot.UpDn.CalOK = 1;
	}
	
	/* ---------ExtRobot--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.ExtRobot )
	{
		gMacOption.RExtRobot = 0;
		gMachineIn.RMold.ExtRobotUpLimit = 1;
	}
	
	/* ---------PullBottom--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.PullBottom )
	{
		gMacOption.RPullBottom = 0;
	}
	//	else
	//	{
	//		gMachineFix.Option.HWEnable.RMold.BottomDeflash = 0;   //Interlock PullBottom
	//	}
	
	/* ---------ExtInMoldSticker--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.ExtInMoldSticker )
	{
		gMacOption.RExtInMoldSticker = 0;
		gMachineIn.RMold.ExtInMoldStickerBwLimit = 1;
		gMachineIn.RMold.ExtInMoldStickerAlarm = 1;
	}
	
	
	/* ---------HookPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.RMold.HookPin )
	{
		gMacOption.RHookPin = 0;
		gMachineIn.RMold.HookPinBwLimit = 1;
	}
	
	/* ----------------LMold------------------- */
	
	/* ---------BlowPin2 for Electric--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.BlowPin2 )
	{
		gMacOption.LBlowPin = 1;
		gMacOption.LBlowPin2 = 0;
		gMachineIn.LMold.BlowPin2UpLimit = 1; 
		
		LMold.BlowPin2.Error = 0;
		LMold.BlowPin2.CalOK = 1;
		LMold.BlowPin2.UpPos = 1;
	}
	
	/* ---------TopDeflash--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.TopDeflash )
	{
		gMacOption.LTopDeflash = 0;
		gMachineIn.LMold.TopDeflashBwLimit = 1;
		
		gMachineFix.Option.HWEnable.LMold.PunchHandle = 0;  // Disable PunchHandle(no TopDeflash, no PunchHandle)
	}

	
	/* ---------PunchHandle--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.PunchHandle)
	{
		gMacOption.LPunchHandle = 0;
		gMachineIn.LMold.TopDeflashOpnLimit = 1;
	}
	
	/* ---------BottomDeflash--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.BottomDeflash )
	{
		gMacOption.LBottomDeflash = 0;
		gMachineIn.LMold.BottomDeflashFwLimit =	1;
		gMachineIn.LMold.BottomDeflashBwLimit = 1;
	}
	//	else
	//	{
	//		gMachineFix.Option.HWEnable.LMold.PullBottom = 0;   //Interlock BottomDeflash
	//	}
	
	/* ---------PinPart--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.PinPart )
	{
		gMacOption.LPinPart = 0;
		gMachineIn.LMold.PinPartFwLimit = 1;
		gMachineIn.LMold.PinPartBwLimit = 1;
	}
	
	/* ---------CoolPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.CoolPin )
	{
		gMacOption.LCoolPin = 0;
		gMachineIn.LMold.CoolPinUpLimit = 1;
	}
	
	/* ---------CoolPin2--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.CoolPin2 )
	{
		gMacOption.LCoolPin2 = 0;
		gMachineIn.LMold.CoolPin2UpLimit = 1;
	}
	
	/* ---------CoolPin3--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.CoolPin3 )
	{
		gMacOption.LCoolPin3 = 0;
		gMachineIn.LMold.CoolPin3UpLimit = 1;
	}
	
	/* ---------InnerPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.InnerPin )
	{
		gMacOption.LInnerPin = 0;
		gMachineIn.LMold.InnerPinBwLimit = 1;
	}
	
	/* ---------CutNeck--------- */ /* 旋切瓶口  */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.CutNeck )
	{
		gMacOption.LCutNeck = 0;
		gMachineIn.LMold.CutNeckOpnLimit = 1;
	}
	
	/* ---------SubMold--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.SubMold )
	{
		gMacOption.LSubMold = 0;
		gMachineIn.LMold.SubMoldClsLimit = 1;
		gMachineIn.LMold.SubMoldOpnLimit = 1;
	}

	/* ---------BottomSubmold--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.BottomSubmold )
	{
		gMacOption.LBottomSubmold = 0;
		gMachineIn.LMold.BottomSubMoldClsLimit = 1;
		gMachineIn.LMold.BottomSubMoldOpnLimit = 1;
	}
	
	/* ---------RobotTransfer-------- */
	gMachineFix.Option.HWEnable.LMold.RobotOpnCls = gMachineFix.Option.HWEnable.LMold.RobotTransfer;  // 機械手開閉強制同動
	gMacOption.LRobotOpnCls = gMacOption.LRobotTransfer;									 		  // 機械手開閉強制同動
	if ( 0 == gMachineFix.Option.HWEnable.LMold.RobotTransfer )
	{
		gMachineFix.Option.HWEnable.LMold.RobotOpnCls = 0;
		gMacOption.LRobotTransfer = 0;
		gMachineIn.LMold.RobotBwLimit = 1;
		
		LMold.Robot.Transfer.Error = 0;
		LMold.Robot.Transfer.BwPos = 1;
		LMold.Robot.Transfer.CalOK = 1;
	}

	/* ---------RobotOpnCls--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.RobotOpnCls )
	{
		gMacOption.LRobotOpnCls = 0;
		gMachineIn.LMold.RobotOpnLimit = 1;
	}
	
	/* ---------RobotUpDn--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.RobotUpDn )
	{
		gMacOption.LRobotUpDn = 0;
		gMachineIn.LMold.RobotUpLimit  = 1;
		LMold.Robot.UpDn.Error = 0;
		LMold.Robot.UpDn.UpPos = 1;
		LMold.Robot.UpDn.CalOK = 1;
	}
	
	/* ---------ExtRobot--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.ExtRobot )
	{
		gMacOption.LExtRobot = 0;
		gMachineIn.LMold.ExtRobotUpLimit = 1;
	}
	
	/* ---------PullBottom--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.PullBottom )
	{
		gMacOption.LPullBottom = 0;
	}
	//	else
	//	{
	//		gMachineFix.Option.HWEnable.LMold.BottomDeflash = 0;   //Interlock PullBottom
	//	}
	
	/* ---------ExtInMoldSticker--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.ExtInMoldSticker )
	{
		gMacOption.LExtInMoldSticker = 0;
		gMachineIn.LMold.ExtInMoldStickerBwLimit = 1;
		gMachineIn.LMold.ExtInMoldStickerAlarm = 1;
	}

	/* ---------HookPin--------- */
	if ( 0 == gMachineFix.Option.HWEnable.LMold.HookPin )
	{
		gMacOption.LHookPin = 0;
		gMachineIn.LMold.HookPinBwLimit = 1;
	}

	





	/* AO DO force AT auto state reset, otherwise dangerous */
	if(1 == gTKauto.Status)
	{
		memset(&gHardWare.DOForce[0],0,sizeof(gHardWare.DOForce));
		memset(&gHardWare.AOForce[0],0,sizeof(gHardWare.AOForce));
		gHardWare.AOForceNum = 19;
	}

	/* three color led control */
	if(1 == gMachineInfo.Calib && 1 == gMachineIn.CalibMode)
	{
		if(0 == gMachineOut.AlarmRed)
		{
			CalibLed = 1;
		}
		else
		{
			CalibLed = 0;
		}
	}
	else
	{
		CalibLed = 0;
	}

	if(1 == gMachineInfo.Manual || 1 == gMachineInfo.Auto)
	{
		if(0 == gMachineOut.AlarmRed)
		{
			gMachineOut.Running = 1;
		}
		else
		{
			gMachineOut.Running = 0;
		}
	}
	else
	{
		gMachineOut.Running = 0;
	}


   
	/* 自动判断干循环条件  *//*Albert change the condition of extruder on*/
	//	if((1 == gMachineInfo.Auto) && (0 == gMachineOut.ExtrA_On))
	//	if((1 == gMachineInfo.Auto) && (0 == gExtruderACtrl.InverterCmd.bEnableCtrl) &&  (0 == gExtruderBCtrl.InverterCmd.bEnableCtrl) && (0 == gExtruderCCtrl.InverterCmd.bEnableCtrl)
	//		&& (0 == gExtruderVCtrl.InverterCmd.bEnableCtrl))
	//	{
	//		gMacOption.bDryCycle = 1;
	//	}
	//	else
	//	{
	//		if(0 == gMachineInfo.Auto)
	//		{
	//			gMacOption.bDryCycle = 0; 
	//		}
	//		else
	//		{
	//		//	if(gExtruderA.ActSpeed > 0)
	//				if((gExtruderACtrl.rActRPM>0) || (gExtruderBCtrl.rActRPM>0) ||  (gExtruderCCtrl.rActRPM>0) || (gExtruderVCtrl.rActRPM>0)   )
	//			{
	//				gMacOption.bDryCycle = 0;   
	//			}
	//		}
	//	}
	
	/* 干循环，不吹气，吹针不下  */
	/*if(1 == gMacOption.bDryCycle)
	{
	gMachineOut.LMold.LowBlow = 0;  
	gMachineOut.LMold.HighBlow = 0; 
	gMachineOut.RMold.LowBlow = 0;
	gMachineOut.RMold.HighBlow = 0;  
	gMachineOut.LMold.BlowPinDn = 0;
	gMachineOut.LMold.BlowPinDnFast = 0;
	gMachineOut.RMold.BlowPinDn = 0;
	gMachineOut.RMold.BlowPinDnFast = 0; 
		
	LMold.ActInfo.BlowPinUp = 0;
	LMold.ActInfo.BlowPinDn = 0;
		
	RMold.ActInfo.BlowPinUp = 0;
	RMold.ActInfo.BlowPinDn = 0;
	}*/
	
	
	/*2018.7.14 Albert assign diffent action for different action*/
	if(1 == gMachineInfo.Manual  || 1 == gMachineInfo.Auto )
	{
		gMachineOut.RMold.MovementValve = ((gMachineOut.RMold.ClampCls | gMachineOut.RMold.ClampOpn)&& (ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)) 
			| ((gMachineOut.RMold.TopDeflashBw 	| gMachineOut.RMold.TopDeflashFw ) && (ACTUATOR_HYD == gMachineFix.MoldR.TopDeflash.eActuatorType))	
			| ((gMachineOut.RMold.CarriageFw 		| gMachineOut.RMold.CarriageBw 		) && (ACTUATOR_HYD == gMachineFix.MoldR.Carriage.eActuatorType))
			| ((gMachineOut.RMold.BlowPinUp 		| gMachineOut.RMold.BlowPinDn		| gMachineOut.RMold.BlowPinUpMicro )&& (ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType))
			| ((gMachineOut.RMold.BottomDeflashFw | gMachineOut.RMold.BottomDeflashBw) && (ACTUATOR_HYD == gMachineFix.MoldR.BottomDeflash.eActuatorType))
			/*| gMachineOut.AccuChargeValve*/ |gMachineOut.AccuChargeValve_Extrlift
			|  ( 0 == gMachineFix.Plate.bUpDnPressureOutputLeft && (gMachineOut.PlateDn |gMachineOut.PlateUp ))
			|  ( 0 == gMachineFix.Plate.bFwBwPressureOutputLeft && (gMachineOut.PlateFw |gMachineOut.PlateBw ))
	/*	| gMachineOut.RMold.SubMoldOpn 		| gMachineOut.RMold.SubMoldCls 	 
		| gMachineOut.RMold.BottomSubMoldOpn 		| gMachineOut.LMold.BottomSubMoldCls */  //ipis1113
	/*	| gMachineOut.RMold.BlowPinDnHold*/
			| ((0 < gMachineOut.P_Test) && (PIC_IO_AO_1 == gPanelObj.GetPicture || PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture) && (1 == btnTesetPQ)
			&&(1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto));
	
	
		gMachineOut.LMold.MovementValve = ((gMachineOut.LMold.ClampCls | gMachineOut.LMold.ClampOpn)&& (ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)) 
			| ((gMachineOut.LMold.TopDeflashBw 	| gMachineOut.LMold.TopDeflashFw ) && (ACTUATOR_HYD == gMachineFix.MoldL.TopDeflash.eActuatorType))	
			| ((gMachineOut.LMold.CarriageFw 		| gMachineOut.LMold.CarriageBw 		) && (ACTUATOR_HYD == gMachineFix.MoldR.Carriage.eActuatorType))
			| ((gMachineOut.LMold.BlowPinUp 		| gMachineOut.LMold.BlowPinDn		| gMachineOut.LMold.BlowPinUpMicro )&& (ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType))
			| ((gMachineOut.LMold.BottomDeflashFw | gMachineOut.LMold.BottomDeflashBw) && (ACTUATOR_HYD == gMachineFix.MoldL.BottomDeflash.eActuatorType))
			|  ( 1 == gMachineFix.Plate.bUpDnPressureOutputLeft && (gMachineOut.PlateDn |gMachineOut.PlateUp ))
			|  ( 1 == gMachineFix.Plate.bFwBwPressureOutputLeft && (gMachineOut.PlateFw |gMachineOut.PlateBw ))
			/*| gMachineOut.AccuChargeValve*/ |gMachineOut.AccuChargeValve_Extrlift
	/*	| gMachineOut.LMold.SubMoldOpn 		| gMachineOut.LMold.SubMoldCls 		
		| gMachineOut.LMold.BottomSubMoldOpn 		| gMachineOut.LMold.BottomSubMoldCls */ //ipis1113
	/*	| gMachineOut.LMold.BlowPinDnHold*/
	/*	| gMachineOut.PlateDn |	gMachineOut.PlateUp*/
			|((0 < gMachineOut.P_Test2) && (PIC_IO_AO_1 == gPanelObj.GetPicture || PIC_PRESSURE_CALIBRATION == gPanelObj.GetPicture) && (1 == btnTesetPQ)
			&&(1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone && 0== gMachineInfo.Auto));
	}
	else
	{
		gMachineOut.RMold.MovementValve = 0 ;
		gMachineOut.LMold.MovementValve = 0 ;
	}
	gMachineOut.RMold.CalibValve_1 = (1 == gMachineInfo.Calib) && (((gMachineOut.RMold.ClampCls | gMachineOut.RMold.ClampOpn) && (ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType)));

	gMachineOut.RMold.CalibValve_2 = (1 == gMachineInfo.Calib) && (((gMachineOut.RMold.BlowPinUp | gMachineOut.RMold.BlowPinDn | gMachineOut.RMold.BlowPinUpMicro )&& (ACTUATOR_HYD == gMachineFix.MoldR.BlowPin.eActuatorType))
		| ((gMachineOut.RMold.TopDeflashBw 	| gMachineOut.RMold.TopDeflashFw ) && (ACTUATOR_HYD == gMachineFix.MoldR.TopDeflash.eActuatorType))
		| ((gMachineOut.RMold.CarriageFw | gMachineOut.RMold.CarriageBw ) && (ACTUATOR_HYD == gMachineFix.MoldR.Carriage.eActuatorType)));
	
	if(0 == gMachineIn.SafeGate)
	{
		gMachineOut.SafePLC_DO =  gMachineOut.RMold.CalibValve_1 && gMachineOut.RMold.CalibValve_2 ; 
	}
	else
	{
		gMachineOut.SafePLC_DO = 1 ;
	}
	
	/* ------------------------------  DO PART --------------------------------- */
	DOInternal[0]  = 0;
	/* -----------------------  DO8332-A ------------------------ */
	
	DOInternal[1] = gMachineOut.ExtrLiftUp					| gHardWare.DOForce[0];
	DOInternal[2] = gMachineOut.ExtrLiftUpCheck				| gHardWare.DOForce[1];
	DOInternal[3] = gMachineOut.ExtrLiftUp | (gMachineOut.ExtrLiftUpCheck && PRESSURE_OUTPUT_ACC_EXTRLIFT != gMachineFix.ExtrLift.ePressueOutputType ) | (gMachineOut.ExtrLiftUpCheck && PRESSURE_OUTPUT_ACC_EXTRLIFT == gMachineFix.ExtrLift.ePressueOutputType )  | gHardWare.DOForce[2];
	DOInternal[4] = gMachineOut.ExtrLiftUpFast				| gHardWare.DOForce[3];
	DOInternal[5] = gMachineOut.ChargeHold_Extrlift			| gHardWare.DOForce[4];
	DOInternal[6] = gMachineOut.AccuChargeValve_Extrlift 	| gHardWare.DOForce[5];
	DOInternal[7] = gMachineOut.PlateUp						| gHardWare.DOForce[6];
	DOInternal[8] = gMachineOut.PlateDn						| gHardWare.DOForce[7];

	
	/* -----------------------  DO8332-B ------------------------ */
	DOInternal[9]  = 0 	| gHardWare.DOForce[8];
	DOInternal[10] = gMachineOut.RMold.ClampOpnDiff | gMachineOut.RMold.ClampClsDiff	| gHardWare.DOForce[9];
	DOInternal[11] = gMachineOut.RMold.ClampOpn			| gHardWare.DOForce[10];
	DOInternal[12] = gMachineOut.RMold.ClampCls			| gHardWare.DOForce[11];
	DOInternal[13] = gMachineOut.RMold.ClampOpnRelease 	| gHardWare.DOForce[12];
	DOInternal[14] = gMachineOut.RMold.ClampClsSlow		| gHardWare.DOForce[13];	
	DOInternal[15] = gMachineOut.RMold.CarriageFw		| gHardWare.DOForce[14];   
	DOInternal[16] = gMachineOut.RMold.CarriageBw 		| gHardWare.DOForce[15];  
		
	/* -----------------------  DO8332-C ------------------------ */
	
	DOInternal[17]  = gMachineOut.RMold.CarriageAux		| gHardWare.DOForce[16]; 
	DOInternal[18]  = gMachineOut.RMold.CarriageFwHold	| gHardWare.DOForce[17]; 
	DOInternal[19]  = 0	| gHardWare.DOForce[18]; 
	DOInternal[20]  = gMachineOut.RMold.BlowPinUp		| gHardWare.DOForce[19]; 
	DOInternal[21]  = gMachineOut.RMold.BlowPinDn		| gHardWare.DOForce[20];
	DOInternal[22]  = gMachineOut.RMold.BlowPinUpMicro	| gHardWare.DOForce[21];
	DOInternal[23]  = gMachineOut.RMold.TopDeflashFw	| gHardWare.DOForce[22]; 
	DOInternal[24] 	= gMachineOut.RMold.TopDeflashBw	| gHardWare.DOForce[23]; 


	/* -----------------------  DO8332-D ------------------------ */
	
	DOInternal[25] = 0	| gHardWare.DOForce[24];
	DOInternal[26] = 0 	| gHardWare.DOForce[25];
	DOInternal[27] = 0	| gHardWare.DOForce[26];
	DOInternal[28] = 0	| gHardWare.DOForce[27]; 
	DOInternal[29] = 0	| gHardWare.DOForce[28];
	DOInternal[30] = 0	| gHardWare.DOForce[29];
	DOInternal[31] = gMachineOut.RMold.CalibValve_1	| gHardWare.DOForce[30];
	DOInternal[32] = gMachineOut.RMold.CalibValve_2	| gHardWare.DOForce[31];

		
	
	/* -----------------------  DO8332-E ------------------------ */
	
	DOInternal[33] = gMachineOut.CutterFw 		| gHardWare.DOForce[32];
	DOInternal[34] = gMachineOut.SupportAir_TimeVersion	| gHardWare.DOForce[33];
	DOInternal[35] = gMachineOut.AlarmYellow	| gHardWare.DOForce[34];
	DOInternal[36] = gMachineOut.AlarmGreen		| gHardWare.DOForce[35];
	DOInternal[37] = (gMachineOut.AlarmBuzzer || gMachineOut.ImplyBuzzer) 	| gHardWare.DOForce[36];
	DOInternal[38] = gMachineOut.LedAutoRun		| gHardWare.DOForce[37];
	DOInternal[39] = gMachineOut.SafePLC_DO	| gHardWare.DOForce[38];
	DOInternal[40] = gMachineOut.SafePLC_DO	| gHardWare.DOForce[39];
	
	DOInternal[41] = gMachineOut.RMold.HighBlow		| gHardWare.DOForce[40];
	DOInternal[42] = gMachineOut.RMold.LowBlow		| gHardWare.DOForce[41];
	DOInternal[43] = gMachineOut.RMold.Exhaust		| gHardWare.DOForce[42];
	DOInternal[44] = gMachineOut.RMold.BlowDeflash	| gHardWare.DOForce[43]; 
	DOInternal[45] = gMachineOut.RMold.CoolDeflash	| gHardWare.DOForce[44]; 
	DOInternal[46] = gMachineOut.RMold.RobotFw  	| gHardWare.DOForce[45]; 
	DOInternal[47] = gMachineOut.RMold.RobotCls 	| gHardWare.DOForce[46];
	DOInternal[48] = gMachineOut.WaterHeating		| gHardWare.DOForce[47];
	
	DOInternal[49] = gMachineOut.ExtrA_On		| gHardWare.DOForce[48];
	DOInternal[50] = gMachineOut.ExtrD_On		| gHardWare.DOForce[49];
	DOInternal[51] = gMachineOut.ExtrVisu_On	| gHardWare.DOForce[50];
	DOInternal[52] = 0	| gHardWare.DOForce[51];
	DOInternal[53] = 0	| gHardWare.DOForce[52];
	DOInternal[54] = gMachineOut.OilPump		| gHardWare.DOForce[53];
	DOInternal[55] = gMachineOut.ParisonPump	| gHardWare.DOForce[54];
	DOInternal[56] = gMachineOut.HighPrsMotorOn	| gHardWare.DOForce[55];
	
	DOInternal[57] = gH_IO.do_C[0] 	| gHardWare.DOForce[56];
	DOInternal[58] = gH_IO.do_C[2]	| gHardWare.DOForce[57]; 
	DOInternal[59] = gH_IO.do_C[3]	| gHardWare.DOForce[58];
	DOInternal[60] = gH_IO.do_C[4]	| gHardWare.DOForce[59];
	DOInternal[61] = gMachineOut.RMold.ExtInMoldStickerAllow	| gHardWare.DOForce[60];
	DOInternal[62] = gMachineOut.HeatPower	| gHardWare.DOForce[61];
	DOInternal[63] = gH_IO.do_H[0]	| gHardWare.DOForce[62];
	DOInternal[64] = gH_IO.do_H[1] 	| gHardWare.DOForce[63];

	DOInternal[65] = gH_IO.do_H[2]	| gHardWare.DOForce[64];
	DOInternal[66] = gH_IO.do_H[3]	| gHardWare.DOForce[65];
	DOInternal[67] = gH_IO.do_H[4]	| gHardWare.DOForce[66]; 
	DOInternal[68] = gH_IO.do_H[5]	| gHardWare.DOForce[67];
	DOInternal[69] = gH_IO.do_H[6]	| gHardWare.DOForce[68];
	DOInternal[70] = gH_IO.do_H[7]	| gHardWare.DOForce[69];
	DOInternal[71] = gH_IO.do_H[8]	| gHardWare.DOForce[70];
	DOInternal[72] = gH_IO.do_H[9]	| gHardWare.DOForce[71];
	
	DOInternal[73] = gH_IO.do_H[10]	| gHardWare.DOForce[72];  
	DOInternal[74] = gH_IO.do_H[11]	| gHardWare.DOForce[73];
	DOInternal[75] = gH_IO.do_H[12]	| gHardWare.DOForce[74];
	DOInternal[76] = gH_IO.do_H[13]	| gHardWare.DOForce[75];
	DOInternal[77] = gH_IO.do_H[14]	| gHardWare.DOForce[76];
	DOInternal[78] = gH_IO.do_H[15]	| gHardWare.DOForce[77];
	DOInternal[79] = gH_IO.do_H[16]	| gHardWare.DOForce[78]; 
	DOInternal[80] = gMachineOut.RMold.HookPinFw	| gHardWare.DOForce[79];  

	//	DOInternal[81] = gMachineOut.RMold.HighBlow			| gHardWare.DOForce[80];
	//	DOInternal[82] = gMachineOut.RMold.BlowDeflash		| gHardWare.DOForce[81];
	////	DOInternal[83] = gMachineOut.RMold.TopDeflashCool	| gHardWare.DOForce[82];
	//	DOInternal[83] = gMachineOut.RMold.CoolDeflash	    | gHardWare.DOForce[82]; 
	//	
	////	DOInternal[84] = gMachineOut.RMold.BlowPinDnHold	| gHardWare.DOForce[83];
	////	DOInternal[85] = gMachineOut.RMold.ProductAuxClamp	| gHardWare.DOForce[84];
	//	DOInternal[84] = gMachineOut.RMold.ProductAuxClamp	| gHardWare.DOForce[83];  //ipis20200730
	//	DOInternal[85] = gMachineOut.RMold.TopDeflashCls	| gHardWare.DOForce[84];  //ipis20200730
	//	
	//	DOInternal[86] = gMachineOut.RMold.LowBlow			| gHardWare.DOForce[85];
	//	DOInternal[87] = gMachineOut.RMold.RobotFw			| gHardWare.DOForce[86];
	//	DOInternal[88] = gMachineOut.RMold.RobotCls			| gHardWare.DOForce[87];
	//	
	//	DOInternal[89] = gMachineOut.RMold.CoolPinDn		| gHardWare.DOForce[88];
	//	DOInternal[90] = gMachineOut.RMold.CoolPinBlow		| gHardWare.DOForce[89];
	//	DOInternal[91] = gMachineOut.RMold.CoolPin2Dn		| gHardWare.DOForce[90];
	//	DOInternal[92] = gMachineOut.RMold.CoolPin2Blow		| gHardWare.DOForce[91];
	//	DOInternal[93] = gMachineOut.RMold.CoolPin3Dn 		| gHardWare.DOForce[92];
	//	DOInternal[94] = gMachineOut.RMold.CoolPin3Blow		| gHardWare.DOForce[93];
	//	DOInternal[95] = gMachineOut.RMold.RobotCarriageDn	| gHardWare.DOForce[94];  //ipis20200730
	//	DOInternal[96] = gMachineOut.RMold.RobotCarriageUp	| gHardWare.DOForce[95];  //ipis20200730
			
	
	/* -----------------------  DOF322-C ------------------------ */

	//	DOInternal[65] = gMachineOut.LedAutoRun			| gHardWare.DOForce[64];
	//	DOInternal[66] = gMachineOut.FrontWorkingLight	| gHardWare.DOForce[65];
	//	DOInternal[67] = gMachineOut.InstancyStop		| gHardWare.DOForce[66];
	//	DOInternal[68] = gMachineOut.CutterFw			| gHardWare.DOForce[67];
	//	DOInternal[69] = gMachineOut.SupportAir_TimeVersion	| gHardWare.DOForce[68];
	//	DOInternal[70] = (gMachineOut.AlarmBuzzer || gMachineOut.ImplyBuzzer)		| gHardWare.DOForce[69];
	//	DOInternal[71] = gMachineOut.AlarmRed		| gHardWare.DOForce[70];
	//	DOInternal[72] = 0	| gHardWare.DOForce[71];
	//	DOInternal[73] = gMachineOut.ExtrA_On		| gHardWare.DOForce[72];  
	//	DOInternal[74] = gMachineOut.ExtrD_On		| gHardWare.DOForce[73];
	//	DOInternal[75] = gMachineOut.ExtrVisu_On	| gHardWare.DOForce[74];
	//	DOInternal[76] = gMachineOut.ExtrB_On		| gHardWare.DOForce[75];
	//	DOInternal[77] = gMachineOut.ExtrC_On		| gHardWare.DOForce[76];
	//	DOInternal[78] = gMachineOut.OilPump		| gHardWare.DOForce[77];
	//	DOInternal[79] = gMachineOut.HighPrsMotorOn	| gHardWare.DOForce[78];  
	//	DOInternal[80] = gMachineOut.ParisonPump	| gHardWare.DOForce[79];  
	
	//	DOInternal[97] 	= gExtruderACtrl.InverterCmd.bSwitchOn		| gHardWare.DOForce[96];
	//	DOInternal[98] 	= gExtruderBCtrl.InverterCmd.bSwitchOn		| gHardWare.DOForce[97];
	//	DOInternal[99] 	= gExtruderCCtrl.InverterCmd.bSwitchOn      | gHardWare.DOForce[98];
	//	DOInternal[100] = gExtruderVCtrl.InverterCmd.bSwitchOn		| gHardWare.DOForce[99];
	//	DOInternal[97] 	= 	gMachineOut.ExtrA_Enable		| gHardWare.DOForce[96];
	//	DOInternal[98] 	= gMachineOut.ExtrB_Enable	| gHardWare.DOForce[97];
	//	DOInternal[99] 	= gMachineOut.ExtrC_Enable      | gHardWare.DOForce[98];
	//	DOInternal[100] = gMachineOut.ExtrV_Enable		| gHardWare.DOForce[99];
 
	/*---------ipis0616 for ABB inveter------------------------------------------*/
	//	DOInternal[97] 	= gMachineOut.LedAutoRun		| gHardWare.DOForce[96]; 
	//	DOInternal[98] 	= 0	| gHardWare.DOForce[97];  
	//	DOInternal[99] 	= gMachineOut.CutterFw      | gHardWare.DOForce[98];   
	//	DOInternal[100] = gMachineOut.SupportAir_1	| gMachineOut.SupportAir_2  | gMachineOut.SuckAir_1	| gHardWare.DOForce[99];  /*Support air*/	
	/*---------------------------------------------------------------------------*/
	
	//	DOInternal[101] = (gMachineOut.AlarmBuzzer || gMachineOut.ImplyBuzzer)	| gHardWare.DOForce[100];
	//	DOInternal[102] = gMachineOut.AlarmRed						| gHardWare.DOForce[101];
	//	DOInternal[103] = gMachineOut.AlarmYellow					| gHardWare.DOForce[102];
	//	DOInternal[104] = gMachineOut.AlarmGreen					| gHardWare.DOForce[103]; 
	//	DOInternal[105] = gMachineOut.InstancyStop 					| gHardWare.DOForce[104];
	//	DOInternal[106] = gMachineOut.LMold.PneumaticOn				| gHardWare.DOForce[105];
	//	DOInternal[107] = gMachineOut.RMold.PneumaticOn				| gHardWare.DOForce[106];
	//	DOInternal[108] = gMachineOut.LMold.RobotClampCls 			| gHardWare.DOForce[107];
	//	DOInternal[109] = gMachineOut.RMold.RobotClampCls			| gHardWare.DOForce[108];
	//	DOInternal[110] = 0 | gHardWare.DOForce[109];
	//	DOInternal[111] = 0	| gHardWare.DOForce[110]; 
	//	DOInternal[112] = 0	| gHardWare.DOForce[111]; 


	
	/* -----------------------  DOF322-D ------------------------ */
	//	DOInternal[81] = gH_IO.do_C[0]	| gHardWare.DOForce[80];
	//	DOInternal[82] = gH_IO.do_C[1]	| gHardWare.DOForce[81];
	//	DOInternal[83] = gH_IO.do_C[2]	| gHardWare.DOForce[82];
	//	DOInternal[84] = gH_IO.do_C[3]	| gHardWare.DOForce[83];
	//	DOInternal[85] = gH_IO.do_C[4]	| gHardWare.DOForce[84]; 
	//	DOInternal[86] = gH_IO.do_C[5]	| gHardWare.DOForce[85];
	//	DOInternal[87] = gH_IO.do_C[6]	| gHardWare.DOForce[86];
	//	DOInternal[88] = gH_IO.do_C[7]	| gHardWare.DOForce[87];
	//	DOInternal[89] = gH_IO.do_C[8]	| gHardWare.DOForce[88];
	//	DOInternal[90] = gH_IO.do_C[9]	| gHardWare.DOForce[89];
	//	DOInternal[91] = 0	| gHardWare.DOForce[90];
	//	DOInternal[92] = 0	| gHardWare.DOForce[91];
	//	DOInternal[93] = gH_IO.do_H[0]	| gHardWare.DOForce[92];
	//	DOInternal[94] = gH_IO.do_H[1]	| gHardWare.DOForce[93];
	//	DOInternal[95] = gH_IO.do_H[2]	| gHardWare.DOForce[94];  
	//	DOInternal[96] = gH_IO.do_H[3]	| gHardWare.DOForce[95];  
	
	
	//	DOInternal[113] = 0		| gHardWare.DOForce[112];	
	//	DOInternal[114] = 0		| gHardWare.DOForce[113];
	//	DOInternal[115] = 0		| gHardWare.DOForce[114];
	//	DOInternal[116] = 0		| gHardWare.DOForce[115];
	//	DOInternal[117] = gMachineOut.ExtrVisu_On		| gHardWare.DOForce[116];
	//	DOInternal[118] = gMachineOut.OilPump			| gHardWare.DOForce[117];
	//	DOInternal[119] = gMachineOut.ExtrLiftMotorRun  | gHardWare.DOForce[118];    /*coolDO01 --> heatZone2	*/
	//	DOInternal[120] = gMachineOut.ParisonPump  		| gHardWare.DOForce[119];
	
	//	DOInternal[121] = gH_IO.do_C[0]		| gHardWare.DOForce[120];
	//	DOInternal[122] = gH_IO.do_C[1]		| gHardWare.DOForce[121];
	//	DOInternal[123] = gH_IO.do_C[2]		| gHardWare.DOForce[122];
	//	DOInternal[124] = gH_IO.do_C[3]		| gHardWare.DOForce[123];
	//	DOInternal[125] = gH_IO.do_C[4]		| gHardWare.DOForce[124];
	//	DOInternal[126] = gH_IO.do_C[5]		| gHardWare.DOForce[125];
	//	DOInternal[127] = gH_IO.do_C[6]    	| gHardWare.DOForce[126];
	//	DOInternal[128] = gMachineOut.FrontWorkingLight    	| gHardWare.DOForce[127];

	//	if (0==gMachineIn.InstancyStop && 1==gMachineIn.SafeGate ) //ipis0805
	//	{
	//		gMachineOut.PneuValveOff =1;
	//	}
	//	else
	//	{
	//		gMachineOut.PneuValveOff =0;
	//	}
	
	
		
	/* -----------------------  DOF322-E ------------------------ */
	//	DOInternal[97] 	= gH_IO.do_H[4]		| gHardWare.DOForce[96];
	//	DOInternal[98] 	= gH_IO.do_H[5]		| gHardWare.DOForce[97];
	//	DOInternal[99] 	= gH_IO.do_H[6] 	| gHardWare.DOForce[98];
	//	DOInternal[100] = gH_IO.do_H[7]		| gHardWare.DOForce[99];
	//	DOInternal[101] = gH_IO.do_H[8]		| gHardWare.DOForce[100];
	//	DOInternal[102] = gH_IO.do_H[9]		| gHardWare.DOForce[101];
	//	DOInternal[103] = gH_IO.do_H[10]	| gHardWare.DOForce[102];
	//	DOInternal[104] = gH_IO.do_H[11]	| gHardWare.DOForce[103]; 
	//	DOInternal[105] = gH_IO.do_H[12] 	| gHardWare.DOForce[104];
	//	DOInternal[106] = gH_IO.do_H[13]	| gHardWare.DOForce[105];
	//	DOInternal[107] = gH_IO.do_H[14]	| gHardWare.DOForce[106];
	//	DOInternal[108] = gH_IO.do_H[15]	| gHardWare.DOForce[107];
	//	DOInternal[109] = gH_IO.do_H[16]	| gHardWare.DOForce[108];
	//	DOInternal[110] = gH_IO.do_H[17] 	| gHardWare.DOForce[109];
	//	DOInternal[111] = gH_IO.do_H[18]	| gHardWare.DOForce[110]; 
	//	DOInternal[112] = gH_IO.do_H[19]	| gHardWare.DOForce[111]; 
	
	/* -----------------------  DOF322-F ------------------------ */
	
	//	DOInternal[113] = gH_IO.do_H[20]	| gHardWare.DOForce[112];	
	//	DOInternal[114] = gH_IO.do_H[21]	| gHardWare.DOForce[113];
	//	DOInternal[115] = gH_IO.do_H[22]	| gHardWare.DOForce[114];
	//	DOInternal[116] = gH_IO.do_H[23]	| gHardWare.DOForce[115];
	//	DOInternal[117] = gH_IO.do_H[24]	| gHardWare.DOForce[116];
	//	DOInternal[118] = gH_IO.do_H[25]	| gHardWare.DOForce[117];
	//	DOInternal[119] = gH_IO.do_H[26] 	| gHardWare.DOForce[118];    
	//	DOInternal[120] = gH_IO.do_H[27]	| gHardWare.DOForce[119];
	//	DOInternal[121] = gH_IO.do_H[28]	| gHardWare.DOForce[120];
	//	DOInternal[122] = gH_IO.do_H[29]	| gHardWare.DOForce[121];
	//	DOInternal[123] = gH_IO.do_H[30]	| gHardWare.DOForce[122];
	//	DOInternal[124] = gH_IO.do_H[31]	| gHardWare.DOForce[123];
	//	DOInternal[125] = gH_IO.do_H[32]	| gHardWare.DOForce[124];
	//	DOInternal[126] = 0		| gHardWare.DOForce[125];
	//	DOInternal[127] = 0		| gHardWare.DOForce[126];
	//	DOInternal[128] = 0   	| gHardWare.DOForce[127];

	
	
	//	DOInternal[145] = gH_IO.do_H[16]		| gHardWare.DOForce[144]; 	
	//	DOInternal[146] = gH_IO.do_H[17]		| gHardWare.DOForce[145];	
	//	DOInternal[147] = gH_IO.do_H[18]		| gHardWare.DOForce[146];	
	//	DOInternal[148] = gH_IO.do_H[19]		| gHardWare.DOForce[147];	
	//	DOInternal[149] = gH_IO.do_H[20]		| gHardWare.DOForce[148];	
	//	DOInternal[150] = gH_IO.do_H[21]		| gHardWare.DOForce[149];	
	//	DOInternal[151] = gH_IO.do_H[22]   		| gHardWare.DOForce[150];
	//	DOInternal[152] = 0 |gHardWare.DOForce[151];	
	//	
	//	DOInternal[153] = 0	| gHardWare.DOForce[152];	
	//	DOInternal[154] = 0	| gHardWare.DOForce[153];	
	//	DOInternal[155] = 0	| gHardWare.DOForce[154];
	//	DOInternal[156] = 0	| gHardWare.DOForce[155];
	//	DOInternal[157] = 0	| gHardWare.DOForce[156];
	//	DOInternal[158] = 0	| gHardWare.DOForce[157];
	//	DOInternal[159] = 0 | gHardWare.DOForce[158];
	//	DOInternal[160] = gMachineOut.HeatPower  	| gHardWare.DOForce[159];	
	
	//	DOInternal[161] = 0 | gHardWare.DOForce[160];
	//	DOInternal[162] = 0 | gHardWare.DOForce[161];
	//	DOInternal[163] = 0	| gHardWare.DOForce[162];
	//	DOInternal[164] = 0 | gHardWare.DOForce[163];
	
	/* -----------------------  SI8110_1 ------------------------ */	
	/* -----------------------  SO4110_1 ------------------------ */
	

	
	/*--------------------------------------------------------------------*/
	//	/* -----------------------  XP0101_1------------------------ */
	//
	//	if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone) && (0 == btnTesetPQ))
	//	{
	//		gPanelObj.VKeyMatric[S$KEYMATRIX_BLOWPINDN_HOLD]	= gHardWare.XP0101_1[0] || gPanelObj.XP0101_1[0];
	//		
	//			if ((1==LMold.Option.SubMold&& 0==LMold.Option.BottomSubMold) || (1==RMold.Option.SubMold&& 0==RMold.Option.BottomSubMold))    //20190123 topsubmold
	//		{
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_SUBMOLD_OPN]	= gHardWare.XP0101_1[2] || gPanelObj.XP0101_1[2]; //dn
	//				gPanelObj.VKeyMatric[S$KEYMATRIX_SUBMOLD_CLS]	= gHardWare.XP0101_1[1] || gPanelObj.XP0101_1[1]; //up		 
	//				}	
	//		if ((1==LMold.Option.BottomSubMold&&0==LMold.Option.SubMold) || (1==RMold.Option.BottomSubMold&&0==RMold.Option.SubMold)) //20190123 BottomSubMold
	//		{
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_SUBMOLD_OPN]	= gHardWare.XP0101_1[1] || gPanelObj.XP0101_1[1]; //dn
	//				gPanelObj.VKeyMatric[S$KEYMATRIX_SUBMOLD_CLS]	= gHardWare.XP0101_1[2] || gPanelObj.XP0101_1[2]; //up
	//				}
	//
	//		
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_PLATE_UP]		= gHardWare.XP0101_1[3] || gPanelObj.XP0101_1[3];
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_PLATE_DN]		= gHardWare.XP0101_1[4] || gPanelObj.XP0101_1[4];
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_PLATE_FW]	= gHardWare.XP0101_1[5] || gPanelObj.XP0101_1[5];
	//			gPanelObj.VKeyMatric[S$KEYMATRIX_PLATE_BW]	= gHardWare.XP0101_1[6] || gPanelObj.XP0101_1[6];		
	//	}
	//			gPanelObj.VKeyMatric[S$PAGE_FAST_SEETING]		= gHardWare.XP0101_1[7] || gPanelObj.XP0101_1[7];
	//	
	//	if ((0==gMachineIn.OilPumpOk_DI || 0==gMachineInfo.OilPumpDelayDone))
	//	{
	//
	//		for(idx_DI=0;idx_DI<7;idx_DI++)  
	//		{
	//			if (EDGEPOS(gHardWare.XP0101_1[idx_DI],RKeyMatric[idx_DI]) )
	//			{
	//				gImply.OilPumpNotOn=1;
	//			}
	//		}
	//	}
	/* -------------------------- DOInternal to Module DO -------------------------- */
	for(idx=0;idx<8;idx++) //DO Module has 8 point 
	{
		gHardWare.DO8332_1[idx] = DOInternal[idx+1];
		gHardWare.DO8332_2[idx] = DOInternal[idx+9];
		gHardWare.DO8332_3[idx] = DOInternal[idx+17];
		gHardWare.DO8332_4[idx] = DOInternal[idx+25];
		//		gHardWare.DO8332_5[idx] = DOInternal[idx+33];//
		//		gHardWare.DO8332_6[idx] = DOInternal[idx+41];//
		//		gHardWare.DO8332_7[idx] = DOInternal[idx+49];//
		//		gHardWare.DO8332_8[idx] = DOInternal[idx+57];//
	}
	/*2018.6.22 Albert*/
	for(idx=0;idx<16;idx++)//DO Module has 16 point 
	{
		gHardWare.DOF322_1[idx] = DOInternal[idx+33];
		gHardWare.DOF322_2[idx] = DOInternal[idx+49];
		gHardWare.DOF322_3[idx] = DOInternal[idx+65];
		//		gHardWare.DOF322_4[idx] = DOInternal[idx+49];
		//		gHardWare.DOF322_5[idx] = DOInternal[idx+97];
		//		gHardWare.DOF322_6[idx] = DOInternal[idx+113];
		
		//		gHardWare.DOF322_1[idx] = DOInternal[idx+33];
		//		gHardWare.DOF322_2[idx] = DOInternal[idx+49];
		//		gHardWare.DOF322_3[idx] = DOInternal[idx+65];
		//		gHardWare.DOF322_4[idx] = DOInternal[idx+81];
		//		gHardWare.DOF322_5[idx] = DOInternal[idx+97];
		//		gHardWare.DOF322_6[idx] = DOInternal[idx+113];
	}
	
	
	//	for(idx=0;idx<8;idx++) //DO Module has 8 point 
	//	{
	//		gHardWare.DO8332_1[idx] = DOInternal[idx+1];
	//		gHardWare.DO8332_2[idx] = DOInternal[idx+9];
	//		gHardWare.DO8332_3[idx] = DOInternal[idx+17];
	//		gHardWare.DO8332_4[idx] = DOInternal[idx+25];
	//		gHardWare.DO8332_5[idx] = DOInternal[idx+33];//Reserve
	//		gHardWare.DO8332_6[idx] = DOInternal[idx+41];//Reserve
	//		gHardWare.DO8332_7[idx] = DOInternal[idx+49];//Reserve
	//		gHardWare.DO8332_8[idx] = DOInternal[idx+57];//Reserve
	//	}
	//	/*2018.6.22 Albert*/
	//	for(idx=0;idx<16;idx++)//DO Module has 16 point 
	//	{
	//		gHardWare.DOF322_1[idx] = DOInternal[idx+65];
	//		gHardWare.DOF322_2[idx] = DOInternal[idx+81];
	//		gHardWare.DOF322_3[idx] = DOInternal[idx+97];
	//		gHardWare.DOF322_4[idx] = DOInternal[idx+113];
	//		gHardWare.DOF322_5[idx] = DOInternal[idx+129];
	//		gHardWare.DOF322_6[idx] = DOInternal[idx+145];
	//	}
	
    
	/* ---------------------- 模块出错，自动停机,进入保护模式 ----------------------- */
	Power_Error =  gHardWare.ModuleOk.BC0083_1 * gHardWare.ModuleOk.PS9400_1 
		/** gHardWare.ModuleOk.BC0083_2 * gHardWare.ModuleOk.PS9400_2  *  gHardWare.ModuleOk.PS3300_1*/ ;
      
	Power_Error = ! Power_Error;
    
	DI_Error = gHardWare.ModuleOk.DIF371_1 * gHardWare.ModuleOk.DIF371_2 /**  gHardWare.ModuleOk.DIF371_3 * gHardWare.ModuleOk.DIF371_4*/;
 
	DI_Error = ! DI_Error;
    
	DO_Error = gHardWare.ModuleOk.DOF322_1 * gHardWare.ModuleOk.DOF322_2 * gHardWare.ModuleOk.DOF322_3 * gHardWare.ModuleOk.DOF322_4 
	*gHardWare.ModuleOk.DO8332_1 * gHardWare.ModuleOk.DO8332_2 * gHardWare.ModuleOk.DO8332_3 /** gHardWare.ModuleOk.DO8332_4*/ ;
    
	DO_Error = ! DO_Error;
    
	AI_Error = gHardWare.ModuleOk.AI4632_1 * gHardWare.ModuleOk.AI4632_2 /** gHardWare.ModuleOk.AI4632_3*/;
    
	AI_Error = ! AI_Error;
    
	AO_Error =  gHardWare.ModuleOk.AO4632_1 * gHardWare.ModuleOk.AO4632_2 /** gHardWare.ModuleOk.AO4632_3 * gHardWare.ModuleOk.AO4632_4*/;
    
	AO_Error = ! AO_Error;
    
	AT_Error = gHardWare.ModuleOk.ATC402_1 * gHardWare.ModuleOk.ATC402_2 * gHardWare.ModuleOk.ATC402_3 /** gHardWare.ModuleOk.ATC402_4 * gHardWare.ModuleOk.ATC402_5 * gHardWare.ModuleOk.ATC402_6*/;
    
	AT_Error = ! AT_Error;
	
	//	AP_Error = gHardWare.ModuleOk.AP3131_1 * gHardWare.ModuleOk.AP3131_2;
	//	
	//	AP_Error = !AP_Error;
    
	//	SEFETY_Error = gHardWare.ModuleOk.SL8100_1 * gHardWare.ModuleOk.SI8110_1 * gHardWare.ModuleOk.SI8110_2 *
	//					gHardWare.ModuleOk.SO4110_1;
	//	
	//	SEFETY_Error = !SEFETY_Error;
	
	if(1 == Power_Error || 1 == DI_Error || 1 == DO_Error  ||
		1 == AI_Error    || 1 == AO_Error || 1 ==  AT_Error /*||1 == AP_Error || SEFETY_Error*/)
	{
		ErrCounter ++;
		if(ErrCounter > 10000)ErrCounter= 100;
	}
	else
	{
		ErrCounter = 0;
	}

	if(ErrCounter > 50)
	{
		gPROCESS_info.moduleErr = 1;

		MachineInit();
	}
	else
	{
		gPROCESS_info.moduleErr = 0;
	}

	if(1 == gTKauto.Status)
	{
		if(ErrCounter > 50)gPROCESS_info.mode_req = MODE_MANUAL;
	}

}/* end of cyclic   */
/* -------------------------------------- machine DO init ---------------------------------------- */
void MachineInit()
{
	memset(DOInternal,0,sizeof(DOInternal));
	
	for(idx=0;idx<8;idx++) // each DO Module has 8 point
	{
		gHardWare.DO8332_1[idx]  = 0;
		gHardWare.DO8332_2[idx]  = 0;
		gHardWare.DO8332_3[idx]  = 0;
		gHardWare.DO8332_4[idx]  = 0;
		//		gHardWare.DO8332_5[idx]  = 0;
		//		gHardWare.DO8332_6[idx]  = 0;
		//		gHardWare.DO8332_7[idx]  = 0;
		//		gHardWare.DO8332_8[idx]  = 0;
		//		gHardWare.DO8332_9[idx]  = 0;
		//		gHardWare.DO8332_10[idx] = 0;
		//		gHardWare.DO8332_11[idx] = 0;
		//		gHardWare.DO8332_12[idx] = 0;
	}
	/*2018.6.22 Albert*/
	for(idx=0;idx<16;idx++) // each DO Module has 16 point 
	{
		gHardWare.DOF322_1[idx]	= 0;
		gHardWare.DOF322_2[idx]	= 0;
		gHardWare.DOF322_3[idx]	= 0;
		//		gHardWare.DOF322_4[idx]	= 0;
		//		gHardWare.DOF322_5[idx]	= 0;
		//		gHardWare.DOF322_6[idx]	= 0;
	}
    
	for(idx=0;idx<4;idx++)
	{
		gHardWare.AO4632_1[idx] = 0;
		gHardWare.AO4632_2[idx] = 0;
		//		gHardWare.AO4632_3[idx] = 0;
		//		gHardWare.AO4632_4[idx] = 0;
	}
}/* void MachineInit() */
/* ----------------------------------------------------------------------------------------------*/


