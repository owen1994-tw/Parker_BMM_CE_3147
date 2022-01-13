/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: panel
 * File: panel.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program panel --- display control 
 ********************************************************************/


//#define	PIC_INIT		          0
//
//#define  PIC_OVERVIEW          	1
//
//#define  PIC_TEMP_SET_1			4
//#define  PIC_TEMP_SET_2   		5
//#define  PIC_TEMP_EXTRUDER_2     6
//#define  PIC_TEMP_EXTRUDER_3     7
//#define  PIC_TEMP_EXTRUDER_4     8
//
//#define  PIC_TEMP_DIEHEAD_1      9
//#define  PIC_TEMP_DIEHEAD_2      10
//#define  PIC_TEMP_DIEHEAD_3      11
//#define  PIC_TEMP_DIEHEAD_4      12
//
//#define  PIC_HEATING_PLAN        13
//#define	PIC_TEMP_CONSTRUCT	   	14
//#define	PIC_TEMP_CONSTRUCT_2	15
//
//#define	PIC_TEMP_TRACE_1		16
//#define	PIC_TEMP_TRACE_2		17
//#define	PIC_TEMP_TRACE_3		18
//#define	PIC_TEMP_TRACE_4		19
//#define	PIC_TEMP_TRACE_5		20
//#define	PIC_TEMP_TRACE_6		21
//#define	PIC_TEMP_TRACE_7		22
//#define	PIC_TEMP_TRACE_8		23
//
//#define	PIC_TEMP_OPT_1		   24
//#define	PIC_TEMP_OPT_2		   25
//#define	PIC_TEMP_OPT_3		   26
//#define	PIC_TEMP_OPT_4		   27
//#define	PIC_TEMP_OPT_5		   28
//#define	PIC_TEMP_OPT_6		   29
//#define	PIC_TEMP_OPT_7		   30
//#define	PIC_TEMP_OPT_8		   31
//
//#define	PIC_TEMP_CONFIG_1		32
//#define	PIC_TEMP_CONFIG_2		33
//#define	PIC_TEMP_CONFIG_3		34
//#define	PIC_TEMP_CONFIG_4		35
//#define	PIC_TEMP_CONFIG_5		36
//#define	PIC_TEMP_CONFIG_6		37
//#define	PIC_TEMP_CONFIG_7		38
//#define	PIC_TEMP_CONFIG_8		39
//
//#define  PIC_TIME_1           41
//#define  PIC_TIME_2          	42
//#define  PIC_TIME_3          	43
//#define  PIC_TIME_4          	44
//#define  PIC_TIME_5          	45
//#define  PIC_TIME_6          	46
//#define  PIC_TIME_7          	47
//#define  PIC_TIME_8          	48
//#define  PIC_TIME_9          	49
//
//#define  PIC_CLAMP_L            51
//#define  PIC_CLAMP_R            52
//#define  PIC_TRANSPORT_L        53
//#define  PIC_TRANSPORT_R        54
//#define  PIC_ROBOT				  55
//
//#define  PIC_DEFLASH			58


//#define  PIC_BOTTOM_DEFLASH       60
//#define  PIC_BLOW_PIN_L      	  61
//#define  PIC_BLOW_PIN_R      	  62
//#define  PIC_BLOW_PIN2_L      	  1061
//#define  PIC_BLOW_PIN2_R      	  1062
//#define  PIC_BLOW_PIN_CONFIG    63
//#define  PIC_SUBMOLD            64
//
//#define  PIC_LEFT_CONFIG        66
//#define  PIC_RIGHT_CONFIG       67
//#define  PIC_ACCU_SETTING       68
//#define  PIC_CALIB_SETTING       69
//	
//#define  PIC_OPTION             71
//#define  PIC_RECIPE			     72
//#define  PIC_OPTION_2             73
//
//#define  PIC_SPEED			     75
//#define  PIC_LENGTH			     76
//
//#define PIC_THICK_START			81
//#define	PIC_CURVE_EDIT			82
//#define	PIC_CURVE_TRACE			83
//#define	PIC_MARKER_SET_A		86
//#define PIC_CURVEDATA_A			87
//#define	PIC_MARKER_SET_B		88
//#define PIC_CURVEDATA_B        	89
//#define	PIC_IO_DO_2  			90
//#define	PIC_IO_DI_1  			91
//#define	PIC_IO_DI_2  			92
//#define	PIC_IO_DI_3  			93
//#define	PIC_IO_DO_1  			94
//#define	PIC_IO_DO_3  			95
//#define	PIC_IO_DO_4  			96
//#define	PIC_IO_AI_1  			97
//#define	PIC_IO_AO_1  			98
//#define	PIC_IO_MODULE  			99
//#define	PIC_IO_DRIVER_1    		100
//#define	PIC_IO_DRIVER_2    		101
//#define	PIC_ENERGY_MONITOR_ALL  102
//#define	PIC_ENERGY_MONITOR   	103
//
//#define PIC_AUTO_STEP			104
//#define PIC_AUTO_START			105
//#define	PIC_SPC_CTRL			106
//#define	PIC_SPC_1				107
//#define	PIC_SPC_2				108
//
//
//#define	PIC_ALARM_CURRENT	   	111
//#define	PIC_ALARM_HISTORY		112
//#define	PIC_ALARM_1				113
//#define	PIC_ALARM_2				114
//#define	PIC_ALARM_3				115
//#define	PIC_ALARM_4				116
//#define	PIC_ALARM_5				117
//
//#define	PIC_PASSWORD_LOGIN      120
//#define	PIC_SYSTEM_SET_1		121
//#define	PIC_SYSTEM_SET_2		122
//#define	PIC_SYSTEM_SET_3		123
//
//#define	PIC_INDUSTY_40			  	125
//
//#define	PIC_PASSWORD_SET			130
//#define	PIC_MOLD_CALIBRATION		131
//#define	PIC_MOLD_CALIBRATION_2		132
//#define	PIC_PROPORTION_VALVE		133
////#define	PIC_VALVE_DELAY	    	134
//#define	PIC_PRESSURE_CALIBRATION   	134
//#define	PIC_IP_CONFIG		    	135
//#define	PIC_PV_LOG		        	136
//#define	PIC_LOG_INFO		    	137
//#define	PIC_VNC						138
//#define	PIC_HIGH_PASSWORD       	139
//
//#define	PIC_INIT_SET				140
//#define PIC_GAP_CALIBRATE			141
//#define	PIC_ACCU_CALIBRATE		142
//#define	PIC_DEFAULT_SET			143
//#define	PIC_GAP_CALIBRATE_HELP  145
//
//#define	PIC_SCREEN_CALIBRATE	150
//#define	PIC_MOTOR_TREND  		151
//
//#define PIC_FAST_SETTING		160
//
//
//#define  PIC_CONFIG_HELP        170
//#define  PIC_CLAMP_CONFIG       171
//#define  PIC_TRANSPORT_CONFIG   172
//#define  PIC_BLOW_PIN_CONFIG    173
//#define  PIC_ROBOT_CONFIG   	174
//
//
//#define	PIC_OEM_SETTING_1		180
//#define	PIC_OEM_SETTING_2		181
//#define	PIC_OEM_SETTING_3		182
//
//#define	PIC_OEM_SETTING_10		189
//
//#define  PIC_CLAMP_TOGGLE        190
//#define  PIC_CARRIAGE_TOGGLE     191
//
//
//#define	PIC_SCREEN_SAVE			200
//
//
//#define	PIC_CALIB_CLAMP_ENCODER		220
//#define	PIC_CALIB_CARRIAGE_ENCODER	221
//#define	PIC_CALIB_BLOWPIN_ENCODER	222
//#define	PIC_CALIB_OTHER_ENCODER		223
//#define	PIC_CALIB_CLAMP_LVDT		226
//#define	PIC_CALIB_CARRIAGE_LVDT		227
//#define	PIC_CALIB_BLOWPIN_LVDT		228
//#define	PIC_CALIB_OTHER_LVDT		229
//
//#define  PIC_ROBOT_CALIBRATION  	250
//
//#endif

#include <bur/plctypes.h>

#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "defines.h"
#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"
#include "thick.h"

#define  STATE_INACTIVE    0
#define  STATE_SUB_PAGE    1
#define  STATE_ACTIVE      2
#define  STATE_SUB_BUTTON  3

#define  MAX_PAGE_DO     48
#define  MAX_PAGE_DO48     48
#define  MAX_PAGE_DO32     32
#define  MAX_PAGE_DO24     24
#define  MAX_PAGE_DO16     16
#define  MAX_PAGE_DO8      8


void InitOptionLists(void);
void OptionListsUpdate(Mold_Fix_typ *pRMachineFix, Plate_Fix_typ *pPlateFix, ExtrLift_Fix_typ *pExtrLiftFix);

/**********************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				 **
**********************************************************************************/
_INIT    void init()
{
	bHideFrame = 1;
	


	MarkerDis.Current = 0;
	MarkerDis.Count   = 100;
	MarkerDis.Section = 10;
	MarkerDis.pSource = (unsigned char *)&gThickProf.IsMarker[0];

	//	OptionDis.Current = 0;
	//	OptionDis.Count   = 28;
	//	OptionDis.Section = 7;
	//	OptionDis.pSource = (unsigned char *)&gMacOption.LRobot;
	//    OptionDis.pHideSource = (unsigned char *)&OptionHide.LRobot;

	
	Led     = 0;
	IndexDo = 0;
	ready   = 0;
    

	/*----------Special Movement-------------*/
	
	//	gMachineFix.Option.bHideLeftStation  = 1;
	//	gMachineFix.Option.bExtParisonCtrl  = 1;
	
	//	gMachineFix.Option.bBlowUpDuringClampCls = 1;
	//	gMachineFix.Option.bCoolPinUpNextCycClampCls = 1;
	
	
	
	InitOptionLists();
	
	/*2018.6.22 Albert*/
	for(i=0;i<199;i++)
	{
		DoColor[i] = 0x00FA;
	}

	for(i=0;i<12;i++)
	{
		AoColor[i] = 0x00FA;
	}

	for(i=0;i<10;i++)
	{
		FLed[i] = 0;
	}
    
	cntF7 = 0;
	cntF8 = 0;
   
   
	/* function selection hide or not   */
	memset(&OptionHide,1, sizeof(OptionHide));


   
	/*  找到可以顯示的第一?功能   */
	for(i=0;i<OptionDis.Count;i++)
	{
		if(0 == OptionDis.pHideSource[i])
		{
			OptionDis.Current = i;
			break;
		}
	}
	
	bHideNoUse = 1;
}/*  end of init  */

/**********************************************************************************
**        			C Y C L I C   P A R T   O F   T H E   T A S K  				 **
**********************************************************************************/

_CYCLIC  void cyc()
{
		
	if(1 == gRecipeOperate.OnceSave)
	{
		OptionListsUpdate(&gMachineFix.MoldR,&gMachineFix.Plate,&gMachineFix.ExtrLift);
	}
	if (1==gLevelTooLow)
	{
		gImply.LevelTooLow=1;
	}
	
	
	/*-------------------Hide Reset Dialoag---------------*/
	
	if(gPanelObj.GetPicture != PIC_CALIB_CLAMP_ENCODER &&
		gPanelObj.GetPicture != PIC_CALIB_CARRIAGE_ENCODER &&
		gPanelObj.GetPicture != PIC_CALIB_BLOWPIN_ENCODER &&
		gPanelObj.GetPicture != PIC_CALIB_OTHER_ENCODER &&
		gPanelObj.GetPicture != PIC_CALIB_ROBOT_ENCODER &&
		gPanelObj.GetPicture != PIC_CALIB_CLAMP_LVDT && 
		gPanelObj.GetPicture != PIC_CALIB_CARRIAGE_LVDT && 
		gPanelObj.GetPicture != PIC_CALIB_BLOWPIN_LVDT )
	{
		uHideDiagResetCalib = 1;
		bCmdResetClampR	= 0;
		bCmdResetClampL	= 0;
		bCmdResetBlowPinR = 0;
		bCmdResetBlowPinL = 0;
		bCmdResetCarriageR = 0;
		bCmdResetCarriageL = 0;
		
		bCmdResetBlowPin2R = 0;
		bCmdResetBlowPin2L = 0;
		
		bCmdResetExtrLift = 0;
	}
	
	/* ------------ hide left station ------------ */
	bHideLeftStation = gMachineFix.Option.bHideLeftStation;
	
	/* ------------ hide Parison page ------------ */
	bHideParison = gMachineFix.Option.bExtParisonCtrl;
	
	bHideParisonActuatorElectic  	=  (ACTUATOR_ELECTRIC != gMachineFix.Parison.eActuatorType) || gMachineFix.Option.bExtParisonCtrl;
	bHideParisonActuatorHyd		  	=  (ACTUATOR_HYD != gMachineFix.Parison.eActuatorType) 		|| gMachineFix.Option.bExtParisonCtrl;
	
	/* ------------ hide BlowUpDuringClampCls option ------------ */
	bHideBlowUpDuringClampCls = !gMachineFix.Option.bBlowUpDuringClampCls;
	
	/* ------------ hide ACCPump  ------------ */
	bHideACCPump = !gMachineFix.Option.HWEnable.ACCPump;
	
	/* ------------ hide ACCPump Extrlift   ------------ */
	bHideACCPump_Extrlift 	= !gMachineFix.Option.HWEnable.ACCPump_Extrlift;
	
	/* ------------ hide HighPrsMotor   ------------ */
	bHideHighPrsMotor 	= !gMachineFix.Option.HWEnable.HighPrsMotor;

	/* ------------ hide Cutter Heating   ------------ */
	bHideCutterHeating = !gMachineFix.Option.bCutterHeating;
	
	
	/* ------------ hide Die Lip   ------------ */
	for (Index = 0; Index < 8; Index++)
	{
		bHideDieLip[Index] = !gMachineFix.Option.bDieLip[Index];
	}
	
	
	/* ------------ hide Actuator ------------ */
	/* ---Clamp Actuator--- */
	bHideClampActuatorElectic  		=  (ACTUATOR_ELECTRIC != gMachineFix.MoldR.Clamp.eActuatorType);
	bHideRClampActuatorElectic		=  (ACTUATOR_ELECTRIC != gMachineFix.MoldR.Clamp.eActuatorType);
	bHideLClampActuatorElectic		=  (ACTUATOR_ELECTRIC != gMachineFix.MoldR.Clamp.eActuatorType || bHideLeftStation);
	bHideClampActuatorHyd		  	=  (ACTUATOR_HYD != gMachineFix.MoldR.Clamp.eActuatorType);
	bHideRClampActuatorHyd			=  (ACTUATOR_HYD != gMachineFix.MoldR.Clamp.eActuatorType);
	bHideLClampActuatorHyd			=  (ACTUATOR_HYD != gMachineFix.MoldR.Clamp.eActuatorType || bHideLeftStation);
	bHideClampActuatorHyd_LVDT	  	=  (SENSOR_LVDT != gMachineFix.MoldR.Clamp.eSensorType);
	bHideClampActuatorHyd_Switch  	=  (SENSOR_SWITCH != gMachineFix.MoldR.Clamp.eSensorType);
	
	bHideClampFluxOutput			=  (ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType ||FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Clamp.eFluxOutputType || FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.Clamp.eFluxOutputType);
	
	/* ---Carriage Actuator--- */
	bHideCarriageActuatorElectic  	=  (ACTUATOR_ELECTRIC != gMachineFix.MoldR.Carriage.eActuatorType);
	bHideCarriageActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.MoldR.Carriage.eActuatorType);
	bHideRCarriageActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.MoldR.Carriage.eActuatorType);
	bHideLCarriageActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.MoldR.Carriage.eActuatorType || bHideLeftStation);
	bHideCarriageActuatorHyd_LVDT	=  (SENSOR_LVDT != gMachineFix.MoldR.Carriage.eSensorType);
	bHideCarriageActuatorHyd_Switch 	=  (SENSOR_SWITCH != gMachineFix.MoldR.Carriage.eSensorType);
	bHideLCarriageActuatorHyd_Switch 	=  (SENSOR_SWITCH != gMachineFix.MoldR.Carriage.eSensorType || bHideLeftStation);
	
	bHideCarriageFluxOutput			=  (ACTUATOR_ELECTRIC == gMachineFix.MoldR.Carriage.eActuatorType ||FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.Carriage.eFluxOutputType || FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.Carriage.eFluxOutputType);
	
	/* ---BlowPin Actuator--- */
	bHideBlowPinActuatorElectic  	=  (ACTUATOR_ELECTRIC != gMachineFix.MoldR.BlowPin.eActuatorType);
	bHideBlowPinActuatorHyd		  	=  (ACTUATOR_HYD != gMachineFix.MoldR.BlowPin.eActuatorType);
	bHideBlowPinActuatorHyd_LVDT	=  (SENSOR_LVDT != gMachineFix.MoldR.BlowPin.eSensorType);
	bHideBlowPinActuatorHyd_Switch 	=  (SENSOR_SWITCH != gMachineFix.MoldR.BlowPin.eSensorType);
	bHideLBlowPinActuatorHyd_Switch =  (SENSOR_SWITCH != gMachineFix.MoldR.BlowPin.eSensorType || bHideLeftStation);
	
	bHideBlowPinFluxOutput			=  (ACTUATOR_ELECTRIC == gMachineFix.MoldR.BlowPin.eActuatorType ||FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BlowPin.eFluxOutputType || FLUX_OUTPUT_PROP_VALVE_SINGLE == gMachineFix.MoldR.BlowPin.eFluxOutputType);
		
	/* ---Top Deflash Actuator--- */
	bHideTopDeflashActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.MoldR.TopDeflash.eActuatorType);

	bHideWordTopDeflashAirOpt =  ((ACTUATOR_HYD != gMachineFix.MoldR.TopDeflash.eActuatorType)  |  OptionWordHide.TopDeflash);
	bHideRTopDeflashAirOpt = ((ACTUATOR_HYD != gMachineFix.MoldR.TopDeflash.eActuatorType) | !gMachineFix.Option.HWEnable.RMold.TopDeflash);
	bHideLTopDeflashAirOpt = ((ACTUATOR_HYD != gMachineFix.MoldR.TopDeflash.eActuatorType) | !gMachineFix.Option.HWEnable.LMold.TopDeflash);
	
	/* ---Bottom Deflash Actuator--- */
	bHideBottomDeflashActuatorHyd	=  (ACTUATOR_HYD != gMachineFix.MoldR.BottomDeflash.eActuatorType);

	bHideWordBottomDeflashAirOpt =  ((ACTUATOR_HYD != gMachineFix.MoldR.BottomDeflash.eActuatorType)  | OptionWordHide.BottomDeflash);
	bHideRBottomDeflashAirOpt = ((ACTUATOR_HYD != gMachineFix.MoldR.BottomDeflash.eActuatorType)  | !gMachineFix.Option.HWEnable.RMold.BottomDeflash);
	bHideLBottomDeflashAirOpt = ((ACTUATOR_HYD != gMachineFix.MoldR.BottomDeflash.eActuatorType)  | !gMachineFix.Option.HWEnable.LMold.BottomDeflash);
	
	/* ---SubMold Actuator--- */
	bHideSubmoldActuatorHyd	=  (ACTUATOR_HYD != gMachineFix.MoldR.SubMold.eActuatorType);
	
	/* ---ExtrLift Actuator--- */
	bHideExtrLiftActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.ExtrLift.eActuatorType);
	bHideRExtrLiftActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.ExtrLift.eActuatorType);
	bHideLExtrLiftActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.ExtrLift.eActuatorType || bHideLeftStation);
	bHideExtrLiftActuatorElectric	=  (ACTUATOR_ELECTRIC != gMachineFix.ExtrLift.eActuatorType);

	
	bHideExtrLiftPresOutput			= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.ExtrLift.ePressueOutputType || ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType);
	bHideExtrLiftFluxOutput			= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.ExtrLift.eFluxOutputType		|| ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType);
	
	/* ---Plate Actuator--- */
	bHidePlateActuatorHyd	=  (ACTUATOR_HYD != gMachineFix.Plate.eActuatorType);
	
	/* ---Robot Transfer Actuator--- */
	bHideRobotTransferActuatorHyd	=	(ACTUATOR_HYD != gMachineFix.MoldR.RobotTransfer.eActuatorType);
	bHideRobotTransferActuatorElec	=	(ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eActuatorType);
	bHideRobotTransferActuatorPneu	= 	(ACTUATOR_PNEU != gMachineFix.MoldR.RobotTransfer.eActuatorType);
	
	/* ---Robot UpDn Actuator--- */
	bHideRobotUpDnActuatorHyd		=	(ACTUATOR_HYD != gMachineFix.MoldR.RobotUpDn.eActuatorType);
	bHideRobotUpDnActuatorElectric	=	(ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotUpDn.eActuatorType);
	bHideRobotUpDnActuatorPneu		= 	(ACTUATOR_PNEU != gMachineFix.MoldR.RobotTransfer.eActuatorType);
		
	/* ---Robot OpnCls Actuator--- */
	bHideRobotOpnClsActuatorHyd		=  (ACTUATOR_HYD != gMachineFix.MoldR.RobotOpnCls.eActuatorType);
	bHideRobotOpnClsActuatorPneu	=  (ACTUATOR_PNEU != gMachineFix.MoldR.RobotOpnCls.eActuatorType);
		
	/* ------------ hide Prop Valve page ------------ */
	/* ---Page--- */
	bHidePropValvePage = ((VALVE_PROP != gMachineFix.MoldR.Clamp.eValveType) && (VALVE_PROP != gMachineFix.MoldR.Carriage.eValveType) && (VALVE_PROP != gMachineFix.MoldR.BlowPin.eValveType)) || (gPanelObj.PasswordLevel < 3);
	/* ---Clamp Prop Valve--- */
	bHideClampPropValve 	= (VALVE_PROP != gMachineFix.MoldR.Clamp.eValveType);
	/* ---Carriage Prop Valve--- */
	bHideCarriagePropValve 	= (VALVE_PROP != gMachineFix.MoldR.Carriage.eValveType);
	/* ---BlowPin Prop Valve--- */
	bHideBlowPinPropValve 	= (VALVE_PROP != gMachineFix.MoldR.BlowPin.eValveType);
	
	
	/* ------------ hide Extruder    ------------ */
	bExtrA_Hide = !gMachineFix.ExtruderA.Use;
	bExtrB_Hide = !gMachineFix.ExtruderB.Use;
	bExtrC_Hide = !gMachineFix.ExtruderC.Use;
	bExtrD_Hide = !gMachineFix.ExtruderD.Use;
	bExtrV_Hide = !gMachineFix.ExtruderVisu.Use;
	
	/* ------------ hide Extruder VF_ctrl   ------------ */
	bHideExtruderVFCtrl = (CTRL_VF != gMachineFix.ExtruderA.eCtrlType) ;

	bHideExtruderVFCtrlPara = (CTRL_VF == gMachineFix.ExtruderA.eCtrlType) ;
	bHideExtrA_VFCtrlPara = (CTRL_VF == gMachineFix.ExtruderA.eCtrlType || !gMachineFix.ExtruderA.Use);
	bHideExtrB_VFCtrlPara = (CTRL_VF == gMachineFix.ExtruderB.eCtrlType || !gMachineFix.ExtruderB.Use);
	bHideExtrC_VFCtrlPara = (CTRL_VF == gMachineFix.ExtruderC.eCtrlType || !gMachineFix.ExtruderC.Use);
	bHideExtrD_VFCtrlPara = (CTRL_VF == gMachineFix.ExtruderD.eCtrlType || !gMachineFix.ExtruderD.Use);
	bHideExtrV_VFCtrlPara = (CTRL_VF == gMachineFix.ExtruderVisu.eCtrlType || !gMachineFix.ExtruderVisu.Use);
	
	
	/* ------------ hide AutoLubr ------------ */
	bHideAutoLubr = !gMachineFix.Lubrication.AutoLubrEnable;

	/* password level */
	gMachineInfo.PasswordLevel = gPanelObj.PasswordLevel;
	gMachineInfo.IsZeroDn      = gThickProf.FunOption.IsZeroDn;
	
	OptionHide.LIntervalBlow = !gMachineFix.Option.bIntervalBlow;
	OptionHide.RIntervalBlow = !gMachineFix.Option.bIntervalBlow;
	
	/* action option change  */
	if(gTKauto.Status != 1)
	{
		if(0 == gMacOption.Station)
		{
			LMold.Option.Enable = 1;
			RMold.Option.Enable = 0;
		}
		else if(1 == gMacOption.Station)
		{
			LMold.Option.Enable = 0;
			RMold.Option.Enable = 1;
		}
		else if(2 == gMacOption.Station)
		{
			LMold.Option.Enable = 1;
			RMold.Option.Enable = 1;
		}
		else
		{
			LMold.Option.Enable = 0;
			RMold.Option.Enable = 0;
		}
	}/* if(gTKauto.Status != 1) */

	OldStation = gMacOption.Station;

	LMold.Option.CutterMode = gMacOption.CutterMode;
	RMold.Option.CutterMode = gMacOption.CutterMode;
	
	LMold.Option.Cutter = gMacOption.Cutter;
	RMold.Option.Cutter = gMacOption.Cutter;
	
	LMold.Option.ExtrLift = gMacOption.ExtrLift;
	RMold.Option.ExtrLift = gMacOption.ExtrLift;
	
	LMold.Option.PinOpenUp = 0;
	LMold.Option.PinOpenUp = 0;
   
	LMold.Option.BlowPinClampClosingUp = gMacOption.RBlowPinClampClosingUp;
	RMold.Option.BlowPinClampClosingUp = gMacOption.LBlowPinClampClosingUp;
   
	LMold.Option.Parison = 1;
	RMold.Option.Parison = 1;
   
	/*  no display function --- value = 0  */
	for(i=0;i<OptionDis.Count;i++)
	{
		OptionDis.pSource[i] = OptionDis.pSource[i] * (!OptionDis.pHideSource[i]);
	}
	
	/*  interlock option  */
	if(1 == gMacOption.RPullBottom  && 0 == MacOptionOld.RPullBottom ) 
	{
		if( 1 == gMacOption.RTopDeflash|| 1 == gMacOption.RBottomDeflash )
		{
			gImply.RDeflashAndPullbottomInterlock = 1;
			gMacOption.RTopDeflash = 0;
			gMacOption.RBottomDeflash = 0;	
		}
	}
	else if (1 == gMacOption.RTopDeflash  && 0 == MacOptionOld.RTopDeflash )
	{
		if( 1 == gMacOption.RPullBottom ||  1 == gMacOption.RBottomDeflash)
		{
			gImply.RDeflashAndPullbottomInterlock = 1;
			gMacOption.RPullBottom = 0;
			gMacOption.RBottomDeflash = 0;
		}
	}
	else if(1 == gMacOption.RBottomDeflash  && 0 == MacOptionOld.RBottomDeflash )
	{
		if( 1 == gMacOption.RTopDeflash ||  1 == gMacOption.RPullBottom)
		{
			gImply.RDeflashAndPullbottomInterlock = 1;
			gMacOption.RTopDeflash = 0;
			gMacOption.RPullBottom = 0;
		}
	}
	
	if(1 == gMacOption.LPullBottom  && 0 == MacOptionOld.LPullBottom ) 
	{
		if( 1 == gMacOption.LTopDeflash|| 1 == gMacOption.LBottomDeflash )
		{
			gImply.LDeflashAndPullbottomInterlock = 1;
			gMacOption.LTopDeflash = 0;
			gMacOption.LBottomDeflash = 0;	
		}
	}
	else if (1 == gMacOption.LTopDeflash  && 0 == MacOptionOld.LTopDeflash )
	{
		if( 1 == gMacOption.LPullBottom ||  1 == gMacOption.LBottomDeflash)
		{
			gImply.LDeflashAndPullbottomInterlock = 1;
			gMacOption.LPullBottom = 0;
			gMacOption.LBottomDeflash = 0;
		}
	}
	else if(1 == gMacOption.LBottomDeflash  && 0 == MacOptionOld.LBottomDeflash )
	{
		if( 1 == gMacOption.LTopDeflash ||  1 == gMacOption.LPullBottom)
		{
			gImply.LDeflashAndPullbottomInterlock = 1;
			gMacOption.LTopDeflash = 0;
			gMacOption.LPullBottom = 0;
		}
	}
	
	/*if(0 == gMacOption.LTopDeflash)
	{
	if(1 == gMacOption.LRobot)
	{
	gImply.NoDeflasherL = 1;
	gMacOption.LRobot = 0;
	}
		
	if(1 == gMacOption.LRobotUpDn)
	{
	gImply.NoDeflasherL = 1;
	gMacOption.LRobotUpDn = 0;
	}
	}
	if(0 == gMacOption.RTopDeflash)
	{
	if(1 == gMacOption.RRobot)
	{
	gImply.NoDeflasherR = 1;
	gMacOption.RRobot = 0;
	}
		
	if(1 == gMacOption.RRobotUpDn)
	{
	gImply.NoDeflasherR = 1;
	gMacOption.RRobotUpDn = 0;
	}
	}*/
	
	if(1 == gMacOption.LTopDeflash && 0 == MacOptionOld.LTopDeflash)
	{
		gMacOption.LCutNeck = 0;	
	}
	
	if(1 == gMacOption.RTopDeflash && 0 == MacOptionOld.RTopDeflash)
	{
		gMacOption.RCutNeck = 0;	
	}
	
	if(1 == gMacOption.LCutNeck && 0 == MacOptionOld.LCutNeck)
	{
		gMacOption.LTopDeflash = 0;	
	}
	
	if(1 == gMacOption.RCutNeck && 0 == MacOptionOld.RCutNeck)
	{
		gMacOption.RTopDeflash = 0;	
	}
	
	MacOptionOld = gMacOption;

	/*---------------------ipis1114--------------------------------------------------*/

	if (EDGEPOS(bLSubMold,OldLSubMold))	
	{	
		if (0==gTKauto.Status) //ipis1204
		{
			if (0==gMacOption.LSubMold)
			{
				gUserPara.bProductAuxClamp =1; //ipis0731
				gMacOption.LSubMold=1;
				gMacOption.LBottomSubmold=0;
			}
			else
			{
				gMacOption.LSubMold=0;
					
			}
		}
		else
		{
			gImply.AutoModeLock =1;	
		}
				
	}
					
	if (EDGEPOS(bLBottomSubMold,OldLBottomSubMold))
	{
		if (0==gTKauto.Status) //ipis1204
		{
			if (0==gMacOption.LBottomSubmold)
			{
				gMacOption.LSubMold=0;
				gMacOption.LBottomSubmold=1;
			}
			else
			{
				gMacOption.LBottomSubmold=0;
					
			}	
		}
		else
		{
			gImply.AutoModeLock =1;	
		}
	}
			
	if (EDGEPOS(bRSubMold,OldRSubMold))	
	{	
		if (0==gTKauto.Status) //ipis1204
		{ 
			if (0==gMacOption.RSubMold)
			{
				gUserPara.bProductAuxClamp =1; //ipis0731
				gMacOption.RSubMold=1;
				gMacOption.RBottomSubmold=0;
			}
			else
			{
				gMacOption.RSubMold=0;
					
			}	
		}
		else
		{
			gImply.AutoModeLock =1;	
		}
	}
					
	if (EDGEPOS(bRBottomSubMold,OldRBottomSubMold))
	{
		if (0==gTKauto.Status) //ipis1204
		{
			if (0==gMacOption.RBottomSubmold)
			{
				gMacOption.RSubMold=0;
				gMacOption.RBottomSubmold=1;
			}
			else
			{
				gMacOption.RBottomSubmold=0;
					
			}	
		}
		else
		{
			gImply.AutoModeLock =1;	
		}
	}
	//	if (0==gMacOption.LRobot)
	//	{
	//		if (EDGEPOS(gMacOption.LCoolPin2,bLCoolPin2))	
	//		{
	//			gImply.NoLRobot =1;
	//		}
	//		if (EDGEPOS(gMacOption.LCoolPin3,bLCoolPin3))	
	//		{
	//			gImply.NoLRobot =1;
	//		}		
	//	}
	//	if (0==gMacOption.RRobot)
	//	{
	//		if (EDGEPOS(gMacOption.RCoolPin2,bRCoolPin2))	
	//		{
	//			gImply.NoRRobot =1;
	//		}
	//		if (EDGEPOS(gMacOption.RCoolPin3,bRCoolPin3))	
	//		{
	//			gImply.NoRRobot =1;
	//		}
	//	}
	//			
	//		if (0==gMacOption.LRobot)
	//		{
	//			gMacOption.LCoolPin2=0;	
	//			gMacOption.LCoolPin3=0;		
	//		}
	//	
	//		if (0==gMacOption.RRobot)
	//		{
	//			gMacOption.RCoolPin2=0;
	//			gMacOption.RCoolPin3=0;
	//		}
	
	
	/*-----------------------------------------------------------------------*/	
	
	
	if(0 == gTKauto.Status)	
	{
		//double blow pin option
		LMold.Option.BlowPin = gMacOption.LBlowPin;
		RMold.Option.BlowPin = gMacOption.RBlowPin;
		
		LMold.Option.BlowPin2 = gMacOption.LBlowPin2;
		RMold.Option.BlowPin2 = gMacOption.RBlowPin2;
		
		LMold.Option.RobotTransfer = gMacOption.LRobotTransfer;
		RMold.Option.RobotTransfer = gMacOption.RRobotTransfer;
		
		LMold.Option.RobotOpnCls = gMacOption.LRobotOpnCls;
		RMold.Option.RobotOpnCls = gMacOption.RRobotOpnCls;
		
		LMold.Option.RobotUpDn = gMacOption.LRobotUpDn;
		RMold.Option.RobotUpDn = gMacOption.RRobotUpDn;
		
		LMold.Option.ExtRobot = gMacOption.LExtRobot;
		RMold.Option.ExtRobot = gMacOption.RExtRobot;
		
		LMold.Option.TopDeflash = gMacOption.LTopDeflash;
		RMold.Option.TopDeflash = gMacOption.RTopDeflash;
		
		LMold.Option.PullBottom = gMacOption.LPullBottom;
		RMold.Option.PullBottom = gMacOption.RPullBottom;
			
		LMold.Option.PinPart = gMacOption.LPinPart;	
		RMold.Option.PinPart = gMacOption.RPinPart;
		
		LMold.Option.BottomDeflash = gMacOption.LBottomDeflash;	
		RMold.Option.BottomDeflash = gMacOption.RBottomDeflash;
		
		LMold.Option.CoolPin = gMacOption.LCoolPin;
		RMold.Option.CoolPin = gMacOption.RCoolPin;
		
		LMold.Option.CoolPin2 = gMacOption.LCoolPin2;
		RMold.Option.CoolPin2 = gMacOption.RCoolPin2;
	
		LMold.Option.CoolPin3 = gMacOption.LCoolPin3;
		RMold.Option.CoolPin3 = gMacOption.RCoolPin3;
	
		LMold.Option.SubMold = gMacOption.LSubMold;
		RMold.Option.SubMold = gMacOption.RSubMold;
		
		LMold.Option.BottomSubMold = gMacOption.LBottomSubmold;
		RMold.Option.BottomSubMold = gMacOption.RBottomSubmold;
	
		LMold.Option.PunchHandle = gMacOption.LPunchHandle;	
		RMold.Option.PunchHandle = gMacOption.RPunchHandle;	

		LMold.Option.PinPart = gMacOption.LPinPart;
		RMold.Option.PinPart = gMacOption.RPinPart;
		
		LMold.Option.Screw = gMacOption.LScrew;
		RMold.Option.Screw = gMacOption.RScrew;

		LMold.Option.InnerPin = gMacOption.LInnerPin;		
		RMold.Option.InnerPin = gMacOption.RInnerPin;			
		
		LMold.Option.ExtInMoldSticker = gMacOption.LExtInMoldSticker;	
		RMold.Option.ExtInMoldSticker = gMacOption.RExtInMoldSticker;

		LMold.Option.IntervalBlow = gMacOption.LIntervalBlow;	
		RMold.Option.IntervalBlow = gMacOption.RIntervalBlow;
			
		LMold.Option.CutNeck = gMacOption.LCutNeck;
		RMold.Option.CutNeck = gMacOption.RCutNeck;

		LMold.Option.Sealer = 0;
		RMold.Option.Sealer = 0;
		
		LMold.Option.HookPin = gMacOption.LHookPin;
		RMold.Option.HookPin = gMacOption.RHookPin;
		
	}
	else
	{
			
		
		//	if(3200 == LMold.AutoStep) /* 移模进时 */
		if(3900 == LMold.AutoStep) /* clamp cls */	
		{
			LMold.Option.CoolPin = gMacOption.LCoolPin;
			//	LMold.Option.CoolPin2   = gMacOption.LCoolPin2;
			//	LMold.Option.CoolPin3   = gMacOption.LCoolPin3;
			LMold.Option.TopDeflash  = gMacOption.LTopDeflash;
			LMold.Option.PunchHandle = gMacOption.LPunchHandle;	
			LMold.Option.BottomDeflash = gMacOption.LBottomDeflash;	
			LMold.Option.SubMold = gMacOption.LSubMold;	
			LMold.Option.BottomSubMold = gMacOption.LBottomSubmold;	
			LMold.Option.PullBottom = gMacOption.LPullBottom;
			LMold.Option.ExtInMoldSticker = gMacOption.LExtInMoldSticker;
			//         LMold.Option.InnerPin   = gMacOption.LInnerPin;			
			//         LMold.Option.IntervalBlow  = gMacOption.LIntervalBlow;
			//         LMold.Option.CutNeck = gMacOption.LCutNeck;
			LMold.Option.RobotTransfer = gMacOption.LRobotTransfer;
//			LMold.Option.RobotOpnCls = gMacOption.LRobotOpnCls;
			LMold.Option.HookPin = gMacOption.LHookPin;
		}
		
		//	if(3200 == RMold.AutoStep) /* 移模进时 */
		if(3900 == RMold.AutoStep) /* clamp cls */	
		{
			RMold.Option.CoolPin = gMacOption.RCoolPin;
			//	RMold.Option.CoolPin2   = gMacOption.RCoolPin2;
			//	RMold.Option.CoolPin3   = gMacOption.RCoolPin3;
			RMold.Option.TopDeflash  = gMacOption.RTopDeflash;
			RMold.Option.PunchHandle = gMacOption.RPunchHandle;
			RMold.Option.BottomDeflash = gMacOption.RBottomDeflash;
			RMold.Option.SubMold = gMacOption.RSubMold;
			RMold.Option.BottomSubMold = gMacOption.RBottomSubmold;
			RMold.Option.PullBottom = gMacOption.RPullBottom;
			RMold.Option.ExtInMoldSticker = gMacOption.RExtInMoldSticker;
			//         RMold.Option.InnerPin   = gMacOption.RInnerPin;				
			//         RMold.Option.IntervalBlow = gMacOption.RIntervalBlow;
			//         RMold.Option.CutNeck = gMacOption.RCutNeck;
			RMold.Option.RobotTransfer = gMacOption.RRobotTransfer;
//			RMold.Option.RobotOpnCls = gMacOption.RRobotOpnCls;
			RMold.Option.HookPin = gMacOption.RHookPin;
		}
		
		LMold.Option.Sealer = 0;
		RMold.Option.Sealer = 0;
		
	}
	


	
	gMachineInfo.Station = gMacOption.Station;
	
	
	
	/*---------------------------------------*/
	/*-------------option hide---------------*/
	/*---------------------------------------*/
	/*----Common----*/
	
	OptionHide.ExtrLift				= 0;
	
	bHidePlateUpDnPresOutput			= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.ePressueOutputType);
	bHidePlateUpDnFluxOutput			= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.eFluxOutputType);
	bHidePlateFwBwPresOutput			= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.ePressueOutputType)&& (1 == gMachineFix.Option.bPlateFwBw );
	bHidePlateFwBwFluxOutput			= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.Plate.eFluxOutputType) && (1 == gMachineFix.Option.bPlateFwBw );
	
	
	bHideRSupportAirTimeVersion 		= !gMachinePara.SupportAirTimeVersionEnable;
	bHideLSupportAirTimeVersion 		= !gMachinePara.SupportAirTimeVersionEnable || bHideLeftStation ;
	bHideRSupportAir2TimeVersion 		= !gMachinePara.SupportAirTimeVersionEnable || (SUPPORTAIR_MODE_AFTER_CLAMP_CLS == gMachineFix.MoldR.SupportAir.eSupportAirMode) ;
	bHideLSupportAir2TimeVersion 		= !gMachinePara.SupportAirTimeVersionEnable || bHideLeftStation ||(SUPPORTAIR_MODE_AFTER_CLAMP_CLS == gMachineFix.MoldL.SupportAir.eSupportAirMode) ;
	
	bHideWordSupportAirTimeVersion		= bHideLSupportAirTimeVersion * bHideRSupportAirTimeVersion;
	bHideWordSupportAir2TimeVersion		= bHideLSupportAir2TimeVersion * bHideRSupportAir2TimeVersion;
	
	bHideRSupportSuckAirTimeVersion 	= !gMachineFix.Option.bSupportAirVentTimeVersion;
	bHideLSupportSuckAirTimeVersion 	= !gMachineFix.Option.bSupportAirVentTimeVersion || bHideLeftStation;
	bHideWordSupportSuckAirTimeVer		= bHideRSupportSuckAirTimeVersion * bHideLSupportSuckAirTimeVersion;
	/*----Right Mold----*/
	OptionHide.RBlowPin2			= !gMachineFix.Option.HWEnable.RMold.BlowPin2;
	OptionHide.RRobotTransfer 		= !gMachineFix.Option.HWEnable.RMold.RobotTransfer;
	OptionHide.RRobotUpDn 			= !gMachineFix.Option.HWEnable.RMold.RobotUpDn;
	OptionHide.RRobotOpnCls 		= !gMachineFix.Option.HWEnable.RMold.RobotOpnCls;
	OptionHide.RExtRobot			= !gMachineFix.Option.HWEnable.RMold.ExtRobot;
	OptionHide.RTopDeflash 			= !gMachineFix.Option.HWEnable.RMold.TopDeflash;
	bHideRTopDeflashPresOutput		= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.ePressueOutputType);
	bHideRTopDeflashFluxOutput		= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.eFluxOutputType);
	OptionHide.RBottomDeflash 		= !gMachineFix.Option.HWEnable.RMold.BottomDeflash;
	bHideRBottomDeflashPresOutput	= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.ePressueOutputType);
	bHideRBottomDeflashFluxOutput	= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.eFluxOutputType);
	OptionHide.RCoolPin 			= !gMachineFix.Option.HWEnable.RMold.CoolPin;
	bHideRCoolBlow					= !gMachineFix.Option.HWEnable.RMold.CoolPin || gMachineFix.Option.bCoolPinUpNextCycClampCls;
	OptionHide.RCoolPin2 			= !gMachineFix.Option.HWEnable.RMold.CoolPin2;
	bHideRCoolBlow2					= !gMachineFix.Option.HWEnable.RMold.CoolPin2;
	OptionHide.RCoolPin3 			= !gMachineFix.Option.HWEnable.RMold.CoolPin3;
	bHideRCoolBlow3					= !gMachineFix.Option.HWEnable.RMold.CoolPin3;
	OptionHide.RExtInMoldSticker 	= !gMachineFix.Option.HWEnable.RMold.ExtInMoldSticker;
	OptionHide.RInnerPin 			= !gMachineFix.Option.HWEnable.RMold.InnerPin;
	OptionHide.RIntervalBlow 		= !gMachineFix.Option.HWEnable.RMold.IntervalBlow;
	OptionHide.RPinPart 			= !gMachineFix.Option.HWEnable.RMold.PinPart;
	OptionHide.RPunchHandle 		= !gMachineFix.Option.HWEnable.RMold.PunchHandle;
	OptionHide.RScrew 				= !gMachineFix.Option.HWEnable.RMold.Screw;
	OptionHide.RSubMold 			= !gMachineFix.Option.HWEnable.RMold.SubMold;
	bHideRSubMoldPresOutput			= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.SubMold.ePressueOutputType);
	bHideRSubMoldFluxOutput			= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.SubMold.eFluxOutputType);
	OptionHide.RBottomSubmold		= !gMachineFix.Option.HWEnable.RMold.BottomSubmold;
	OptionHide.RPullBottom			= !gMachineFix.Option.HWEnable.RMold.PullBottom;
	OptionHide.RHookPin			= !gMachineFix.Option.HWEnable.RMold.HookPin;
	
	/*----Left Mold----*/
	OptionHide.LBlowPin2			= !gMachineFix.Option.HWEnable.LMold.BlowPin2;
	OptionHide.LRobotTransfer 		= !gMachineFix.Option.HWEnable.LMold.RobotTransfer;
	OptionHide.LRobotUpDn 			= !gMachineFix.Option.HWEnable.LMold.RobotUpDn;
	OptionHide.LRobotOpnCls 		= !gMachineFix.Option.HWEnable.LMold.RobotOpnCls;
	OptionHide.LExtRobot			= !gMachineFix.Option.HWEnable.LMold.ExtRobot;
	OptionHide.LTopDeflash 			= !gMachineFix.Option.HWEnable.LMold.TopDeflash;
	bHideLTopDeflashPresOutput		= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.TopDeflash.ePressueOutputType);
	bHideLTopDeflashFluxOutput		= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.TopDeflash.eFluxOutputType);
	OptionHide.LBottomDeflash 		= !gMachineFix.Option.HWEnable.LMold.BottomDeflash;
	bHideLBottomDeflashPresOutput	= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.BottomDeflash.ePressueOutputType);
	bHideLBottomDeflashFluxOutput	= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.BottomDeflash.eFluxOutputType);
	OptionHide.LCoolPin 			= !gMachineFix.Option.HWEnable.LMold.CoolPin;
	bHideLCoolBlow					= !gMachineFix.Option.HWEnable.LMold.CoolPin ||(gMachineFix.Option.bCoolPinUpNextCycClampCls || bHideLeftStation);
	OptionHide.LCoolPin2 			= !gMachineFix.Option.HWEnable.LMold.CoolPin2;
	bHideLCoolBlow2					= !gMachineFix.Option.HWEnable.LMold.CoolPin2;
	OptionHide.LCoolPin3 			= !gMachineFix.Option.HWEnable.LMold.CoolPin3;
	bHideLCoolBlow3					= !gMachineFix.Option.HWEnable.LMold.CoolPin3;
	OptionHide.LExtInMoldSticker 	= !gMachineFix.Option.HWEnable.LMold.ExtInMoldSticker;
	OptionHide.LInnerPin 			= !gMachineFix.Option.HWEnable.LMold.InnerPin;
	OptionHide.LIntervalBlow 		= !gMachineFix.Option.HWEnable.LMold.IntervalBlow;
	OptionHide.LPinPart 			= !gMachineFix.Option.HWEnable.LMold.PinPart;
	OptionHide.LPunchHandle 		= !gMachineFix.Option.HWEnable.LMold.PunchHandle;
	OptionHide.LScrew 				= !gMachineFix.Option.HWEnable.LMold.Screw;
	OptionHide.LSubMold 			= !gMachineFix.Option.HWEnable.LMold.SubMold;
	bHideLSubMoldPresOutput			= (PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.SubMold.ePressueOutputType);
	bHideLSubMoldFluxOutput			= (FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldL.SubMold.eFluxOutputType);
	OptionHide.LBottomSubmold		= !gMachineFix.Option.HWEnable.LMold.BottomSubmold;
	OptionHide.LPullBottom			= !gMachineFix.Option.HWEnable.LMold.PullBottom;
	OptionHide.LHookPin			= !gMachineFix.Option.HWEnable.LMold.HookPin;
	
	
	/* option word hide or not   */
	OptionWordHide.ExtrLift			= OptionHide.ExtrLift;
	OptionWordHide.BlowPin2			= OptionHide.LBlowPin2 * OptionHide.RBlowPin2;
	OptionWordHide.RobotTransfer = OptionHide.LRobotTransfer * OptionHide.RRobotTransfer;
	OptionWordHide.RobotUpDn = OptionHide.LRobotUpDn * OptionHide.RRobotUpDn;
	OptionWordHide.RobotOpnCls = OptionHide.LRobotOpnCls * OptionHide.RRobotOpnCls;
	OptionWordHide.ExtRobot = OptionHide.LExtRobot * OptionHide.RExtRobot;
	OptionWordHide.TopDeflash = OptionHide.LTopDeflash * OptionHide.RTopDeflash;
	OptionWordHide.BottomDeflash = OptionHide.LBottomDeflash * OptionHide.RBottomDeflash;
	OptionWordHide.CoolPin = OptionHide.LCoolPin * OptionHide.RCoolPin;
	bHideWordCoolBlow		= bHideLCoolBlow * bHideRCoolBlow;
	OptionWordHide.CoolPin2 = OptionHide.LCoolPin * OptionHide.RCoolPin2;
	bHideWordCoolBlow2		= bHideLCoolBlow2 * bHideRCoolBlow2;
	OptionWordHide.CoolPin3 = OptionHide.LCoolPin * OptionHide.RCoolPin3;
	bHideWordCoolBlow3		= bHideLCoolBlow3 * bHideRCoolBlow3;
	OptionWordHide.SubMold = OptionHide.LSubMold * OptionHide.RSubMold;
	OptionWordHide.PunchHandle = OptionHide.LPunchHandle * OptionHide.RPunchHandle;
	OptionWordHide.PinPart = OptionHide.LPinPart * OptionHide.RPinPart;
	OptionWordHide.Screw = OptionHide.LScrew * OptionHide.RScrew;
	OptionWordHide.InnerPin = OptionHide.LInnerPin * OptionHide.RInnerPin;
	OptionWordHide.IntervalBlow = OptionHide.LIntervalBlow * OptionHide.RIntervalBlow;
	OptionWordHide.CutNeck = OptionHide.LCutNeck * OptionHide.RCutNeck;
	OptionWordHide.PullBottom = OptionHide.LPullBottom * OptionHide.RPullBottom;
	OptionWordHide.HookPin	 = OptionHide.LHookPin * OptionHide.RHookPin;

	if(1 == EDGENEG(gMachineInfo.IsOpt ,OldIsOpt))
	{
		gRecipeOperate.OnceSave = 1;
	}

	if(gPanelObj.PasswordLevel >= 4)  /*20181017 ipis*/
	{
		gPanelObj.LevelBand_1 = 0;
		gPanelObj.LevelBand_2 = 0;
		gPanelObj.LevelBand_3 = 0;
		gPanelObj.LevelBand_4 = 0;
		
		
		gPanelObj.LevelHide1 = 1;
		gPanelObj.LevelHide2 = 1;
		gPanelObj.LevelHide3 = 1;
		gPanelObj.LevelHide4 = 1;
		
	}
	else if(gPanelObj.PasswordLevel == 3) 
	{
		InputColor = 0x003A;

		gPanelObj.LevelBand_1 = 0;
		gPanelObj.LevelBand_2 = 0;
		gPanelObj.LevelBand_3 = 0;
		gPanelObj.LevelBand_4 = 1; /*20181017 ipis*/
		
		gPanelObj.LevelHide1 = 1;
		gPanelObj.LevelHide2 = 1;
		gPanelObj.LevelHide3 = 1;
		gPanelObj.LevelHide4 = 0;
	}
	else
	{
		InputColor = 0x0060;

		if(gPanelObj.PasswordLevel == 2)
		{
			gPanelObj.LevelBand_1 = 0;
			gPanelObj.LevelBand_2 = 0;
			gPanelObj.LevelBand_3 = 1;
			gPanelObj.LevelBand_4 = 1;
			
			gPanelObj.LevelHide1 = 1;
			gPanelObj.LevelHide2 = 1;
			gPanelObj.LevelHide3 = 0;
			gPanelObj.LevelHide4 = 0;
		}
		else if(gPanelObj.PasswordLevel == 1)
		{
			gPanelObj.LevelBand_1 = 0;
			gPanelObj.LevelBand_2 = 1;
			gPanelObj.LevelBand_3 = 1;
			gPanelObj.LevelBand_4 = 1;
			
			gPanelObj.LevelHide1 = 1;
			gPanelObj.LevelHide2 = 0;
			gPanelObj.LevelHide3 = 0;
			gPanelObj.LevelHide4 = 0;
		}
		else
		{
			gPanelObj.LevelBand_1 = 1;
			gPanelObj.LevelBand_2 = 1;
			gPanelObj.LevelBand_3 = 1;
			gPanelObj.LevelBand_4 = 1;
			
			gPanelObj.LevelHide1 = 0;
			gPanelObj.LevelHide2 = 0;
			gPanelObj.LevelHide3 = 0;
			gPanelObj.LevelHide4 = 0;
		}
	}/* if(gPanelObj.PasswordLevel > 2) */

	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
	{
		bThickCalibHide = 0;
	}
	else
	{
		bThickCalibHide = 1;
	}

	if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
	{
		bAutoHide = 0;
	}
	else
	{
		bAutoHide = 1;
	}

	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
	{
		bLevel3CalibHide = 0;
		bHideForceDO = 0;
	}
	else
	{
		bLevel3CalibHide = 1;
		bHideForceDO = 1;
	}

	if(gPanelObj.PasswordLevel > 3 &&  1 == gTKcalib.Status)
	{
		bLevel4CalibHide = 0;
	}
	else
	{
		bLevel4CalibHide = 1;
	}
	/*  hide temperature zone */
	for(i=0;i<MAX_IDX_ZONE;i++)
	{
		bZoneHide[i] = !gT_Par.Fix.Enable[i] ;
		
		if(1 == gT_Par.Fix.Enable[i])
		{
			if((i+1)> gT_Par.Fix.OEMZoneOptionNum)
			{
				gT_Par.Fix.OEMZoneOptionNum = i+1;
			}
		}
		
		if(i < gT_Par.Fix.OEMZoneOptionNum) bOEMZoneOptionHide[i] = 0;
		else bOEMZoneOptionHide[i] = 1;	
	}
	
	/* -------Set SetLanguage -------------- */
	if(gPanelObj.SetLanguage > 3)
	{
		gPanelObj.SetLanguage = 1;
	}

  
	/* ------------------------------ main contorl ------------------------------ */
	if(0 == gMachineIn.MainControl)
	{
		gPanelObj.SetPicture = PIC_INIT;
	}
   
	if(gPanelObj.GetPicture != PIC_IO_AO_1)IndexAo = 0;

	if(gPanelObj.GetPicture != OldGetPicture && OldGetPicture != PIC_SCREEN_SAVE )
	{
		gPanelObj.OldPicture = OldGetPicture;
		if(gPanelObj.OldPicture < 1)gPanelObj.OldPicture = 1;
		ChangePageDly = 30;
	}
	
	if(gPanelObj.GetPicture == PIC_LOG_INFO)
	{
		bHideFunctionKey = 1;	
	}
	else
	{
		bHideFunctionKey = 0;
	}

	/* ---------------- All screen shot is no run can change picture ---------------- */
	if(0 == gScreenShot.ScreenShotRun)
	{
		/*Albert use visual key direct change to fast page*/
		
		/*Fast Page*/
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_FAST_SEETING],KeyMatric[S$PAGE_FAST_SEETING]))
			{
			gPanelObj.SetPicture = PIC_FAST_SETTING;
		}
		
		//		/*IO Page*/
		//		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_MONITOR],KeyMatric[S$PAGE_MONITOR]))
		//		{
		//			gPanelObj.SetPicture = PIC_IO_DI_1;
		//		}
		//		
		//		/*Temperature Page*/
		//		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_TEMP_ALL],KeyMatric[S$PAGE_TEMP_ALL]))
		//		{
		//			gPanelObj.SetPicture = PIC_TEMP_SET_1;
		//		}
		//		
		//		/*Extruder Spedd Page*/
		//		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_EXTRUDER],KeyMatric[S$PAGE_EXTRUDER]))
		//		{
		//			gPanelObj.SetPicture = PIC_SPEED;
		//		}
		
		/* ---------------- Picture change ---------------- */
		switch(gPanelObj.GetPicture)
		{
			case PIC_INIT:
				if(1 == bCmdTouch)
				{
					cntTask++;
				}
				else
				{
					cntTask = 0;	
				}
				
				if(200 == cntTask)
				{
					gPanelObj.vcHandle = VA_Setup(1, gPanelObj.VisualName);

					if (gPanelObj.vcHandle != 0)
					{
						if (!VA_Saccess(1, gPanelObj.vcHandle))
						{
							VA_StartTouchCal (1,gPanelObj.vcHandle);
							VA_Srelease(1, gPanelObj.vcHandle);
						}
					}
				}
				
				if(1 == gRecipeOperate.CurveOk && 1 == gRecipeOperate.NameOk &&
					gRecipeOperate.OnceOk != 0  && 1 == gMachineIn.MainControl &&
					gPanelObj.PasswordLevel > 0)
				{
					Led = 1;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
						{
						gPanelObj.SetLanguage = 2;/*2018.7.5 Albert Set language to traditional chinese*/
						gPanelObj.SetPicture  = PIC_OVERVIEW;
					}
               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
						//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
						
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
						{
						gPanelObj.SetLanguage = 3;
						gPanelObj.SetPicture  = PIC_OVERVIEW;
					}
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
						{
						gPanelObj.SetLanguage = 1;
						gPanelObj.SetPicture  = PIC_OVERVIEW;
					}
				}
				else
				{
					Led = 0;
				}
				break;

			case PIC_OVERVIEW: 
			case PIC_OPTION:
			case PIC_OPTION_2:
			case PIC_SPEED:
			case PIC_AUTO_START:
			case PIC_AUTO_STEP:
				
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;   
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_TIME_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_L;
					if(1 == bHideLeftStation)gPanelObj.SetPicture = PIC_CLAMP_R;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CALIB_SETTING;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
						if (PIC_OPTION == gPanelObj.GetPicture)
						{
							gPanelObj.SetPicture = PIC_OPTION_2;
						}
						else
						{
							gPanelObj.SetPicture = PIC_OPTION;
						}
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == bHideParison)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(0 == bHideParison)gPanelObj.SetPicture = PIC_THICK_START;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == bHideParison)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bHideParison)gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10])) 
					{
					if(gPanelObj.PasswordLevel >= 2)
					{
						gPanelObj.SetPicture = PIC_RECIPE;
					}
					else	
					{
						gImply.LevelTooLow=1;
					}
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_ALARM_CURRENT;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					gPanelObj.SetPicture = PIC_AUTO_STEP;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					gPanelObj.SetPicture = PIC_AUTO_START;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					gPanelObj.SetPicture = PIC_SPC_CTRL;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP]))
					{
					gPanelObj.SetLanguage = 1;
				}
				
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN]))
					{
					gPanelObj.SetLanguage = 0;
				}


				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ESC],KeyMatric[S$KEYMATRIX_ESC]))
					{
					gPwd.PwdLevel = 0;
					gPanelObj.SetPicture = PIC_INIT;
				}
							
				//			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_FAST_SEETING],KeyMatric[S$PAGE_FAST_SEETING]))
				//			{
				//				gPanelObj.SetPicture = PIC_FAST_SETTING;
				//			}
					
				//for PIC_OPTION   //from old  PIC_OPTION  20200915 owen
				//					for(i=0;i<OptionDis.Count;i++)
				//					{
				//						if(1 == HotSpot[i])
				//						{
				//							HotSpot[i]  = 0;
				//							if(0 == OptionDis.pHideSource[i])
				//							{
				//								OptionDis.Current = i;
				//								OptionDis.pSource[OptionDis.Current] = !OptionDis.pSource[OptionDis.Current];
				//								gRecipeOperate.DataSave = 1;
				//							}
				//						} 
				//						OptionDis.Obj[i]= OptionDis.pSource[i];
				//					}
				//					  
				//						OptionDis.Obj[OptionDis.Current] = OptionDis.pSource[OptionDis.Current] + 2;
				
						
				break;

			/*2018.7.3 Albert Add new page for show all temperature setting*/
			case PIC_TEMP_SET_1:
			case PIC_TEMP_SET_2:
			case PIC_TEMP_EXTRUDER_2:
			case PIC_TEMP_EXTRUDER_3:
			case PIC_TEMP_EXTRUDER_4:
			case PIC_TEMP_DIEHEAD_1:
			case PIC_TEMP_DIEHEAD_2:
			case PIC_TEMP_DIEHEAD_3:
			case PIC_TEMP_DIEHEAD_4:
			case PIC_TEMP_CONSTRUCT:
			case PIC_TEMP_CONSTRUCT_2:
			case PIC_HEATING_PLAN:

				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				for(idxZone = 0;idxZone < MAX_IDX_ZONE;idxZone++)
				{
					if(0 == gT_Par.Fix.Enable[idxZone])TempEnableFoucs[idxZone] = 1;
				}
            
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB]))
					{
					for(idxZone = 0;idxZone < MAX_IDX_ZONE;idxZone++)
					{
						if(16384 == TempEnableFoucs[idxZone] && 1 == gT_Par.Fix.Enable[idxZone])
						{
							gT_Par.Ctl.OnOff[idxZone] = ! gT_Par.Ctl.OnOff[idxZone]; 
						}
					}
				}
            
				//					if(16384 == QuickSetValueFoucs)
				//					{
				//						gT_Par.Ctl.IsQuickSetValue = gPanelObj.KeyMatric[S$KEYMATRIX_TAB];
				//							}
				//					else
				//					{
				//						gT_Par.Ctl.IsQuickSetValue = 0;
				//					}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
//					if (PIC_TEMP_CONSTRUCT == gPanelObj.GetPicture)
//					{
//						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT_2;
//					}
//					else
//					{
						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
//					}
				}

				if(PIC_TEMP_CONSTRUCT == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					if(gT_Par.Fix.OEMZoneOptionNum >20)
					{
						if (PIC_TEMP_SET_1 == gPanelObj.GetPicture)
						{
							gPanelObj.SetPicture = PIC_TEMP_SET_2;
						}
						else
						{
							gPanelObj.SetPicture = PIC_TEMP_SET_1;
						}	
					}
					else
					{
						gPanelObj.SetPicture = PIC_TEMP_SET_1;
					}
							
				}			
				if(PIC_TEMP_SET_1 == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					
					
					if (gPanelObj.PasswordLevel>3)  //ipis0305 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
						//						{
						//						gPanelObj.SetPicture = PIC_HEATING_PLAN;
						//					}
						}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_TEMP_TRACE_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					//              gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}
				//				if(PIC_TEMP_CONSTRUCT == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == bHideParison)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bHideParison)gPanelObj.SetPicture = PIC_THICK_START;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
					
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_TEMP_OPT_1;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;

					/*------F12------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_TEMP_CONFIG_1;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					/*------F13------*/    
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					//						{
					//					if (1==gMachineIn.PreHeatDI) //ipis0219
					//					{
					//						gMachineOut.HeatPower =!gMachineOut.HeatPower;
					//					//	gT_Par.GrpSwh.PreOnOff[0] = ! gT_Par.GrpSwh.PreOnOff[0];
					//							gT_Par.GrpSwh.PreOnOff[0] = gMachineOut.HeatPower;
					//						gT_Par.GrpSwh.OnOff[0]    = gT_Par.GrpSwh.PreOnOff[0];
					//					}
					//
					//
					////						if(1 == gMachineOut.HeatPower)
					////						{
					////							gT_Par.GrpSwh.PreOnOff[0] = ! gT_Par.GrpSwh.PreOnOff[0];
					////							gT_Par.GrpSwh.OnOff[0]    = gT_Par.GrpSwh.PreOnOff[0];
					////						}
					////						else
					////						{
					////							gImply.HeaterPowerNotOn = 1;/*Albert*/
					////							gT_Par.GrpSwh.PreOnOff[0] = 0;
					////							gT_Par.GrpSwh.OnOff[0] = 0;
					////						}
					//					}
					/*------F14------*/		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if((1 == gT_Par.GrpSwh.OnOff[0]) && (1 == gT_Par.GrpSwh.PreOnOff[0])) /* pre-heating  */
					{
						gT_Par.GrpSwh.PreOnOff[0] = 0;	
					}
					else
					{
						
						gMachineOut.HeatPower =!gMachineOut.HeatPower; //ipis0219
						//	gT_Par.GrpSwh.OnOff[0] = ! gT_Par.GrpSwh.OnOff[0];
						gT_Par.GrpSwh.OnOff[0] = gMachineOut.HeatPower;
						
						
						//							/*Check Heat power is on then allow to switch heater on*/
						//							if(1 == gMachineOut.HeatPower)
						//							{
						//								gT_Par.GrpSwh.OnOff[0] = ! gT_Par.GrpSwh.OnOff[0];
						//							}
						//							else
						//							{
						//								gImply.HeaterPowerNotOn = 1;/*Albert*/
						//								gT_Par.GrpSwh.OnOff[0] = 0;
						//							
						//							}
						
					}
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_TEMP_TRACE_1:
			case PIC_TEMP_TRACE_2:
			case PIC_TEMP_TRACE_3:
			case PIC_TEMP_TRACE_4:
			case PIC_TEMP_TRACE_5:
			case PIC_TEMP_TRACE_6:
			case PIC_TEMP_TRACE_7:
			case PIC_TEMP_TRACE_8:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_TEMP_SET_1;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.GetPicture < (PIC_TEMP_TRACE_1 + (MAX_IDX_CONTRL_ZONES - 1)/5) && gPanelObj.GetPicture >= PIC_TEMP_TRACE_1)
					{
						gPanelObj.SetPicture = gPanelObj.GetPicture + 1;
					}
					else
					{
						gPanelObj.SetPicture = PIC_TEMP_TRACE_1;
					}
				}
				if(gPanelObj.GetPicture >= PIC_TEMP_TRACE_1 && gPanelObj.GetPicture <= PIC_TEMP_TRACE_8)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
						}
			
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}            
				break;

			case PIC_TEMP_OPT_1:
			case PIC_TEMP_OPT_2:
			case PIC_TEMP_OPT_3:
			case PIC_TEMP_OPT_4:
			case PIC_TEMP_OPT_5:
			case PIC_TEMP_OPT_6:
			case PIC_TEMP_OPT_7:
			case PIC_TEMP_OPT_8:
			case PIC_TEMP_CONFIG_1:
			case PIC_TEMP_CONFIG_2:
			case PIC_TEMP_CONFIG_3:
			case PIC_TEMP_CONFIG_4:
			case PIC_TEMP_CONFIG_5:
			case PIC_TEMP_CONFIG_6:
			case PIC_TEMP_CONFIG_7:
			case PIC_TEMP_CONFIG_8:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_TEMP_SET_1;
				}

				
				if (gPanelObj.PasswordLevel>3)  //ipis0305 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
						//						{
						//						gPanelObj.SetPicture = PIC_HEATING_PLAN;
						//					}
						}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						}
				
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_TEMP_TRACE_1;
				}
	            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//						{
					//						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
					//					}
	            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == bHideParison)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bHideParison)gPanelObj.SetPicture = PIC_THICK_START;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
					
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)
					{
						if(gPanelObj.GetPicture < (PIC_TEMP_OPT_1 + (MAX_IDX_CONTRL_ZONES - 1)/10) && gPanelObj.GetPicture >= PIC_TEMP_OPT_1)
						{
							gPanelObj.SetPicture = gPanelObj.GetPicture + 1;
						}
						else
						{
							gPanelObj.SetPicture = PIC_TEMP_OPT_1;
						}
					}
				}
				if(gPanelObj.GetPicture >= PIC_TEMP_OPT_1 && gPanelObj.GetPicture <= PIC_TEMP_OPT_8)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_ACTIVE;
						}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)
					{
						if(gPanelObj.GetPicture < (PIC_TEMP_CONFIG_1 + (MAX_IDX_CONTRL_ZONES - 1)/10) && gPanelObj.GetPicture >= PIC_TEMP_CONFIG_1 )
						{
							gPanelObj.SetPicture = gPanelObj.GetPicture + 1;
						}
						else
						{
							gPanelObj.SetPicture = PIC_TEMP_CONFIG_1;
						}
					}
				}
				if(gPanelObj.GetPicture >= PIC_TEMP_CONFIG_1 && gPanelObj.GetPicture <= PIC_TEMP_CONFIG_8)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_ACTIVE;
						}
				if(gPanelObj.PasswordLevel < 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if((1 == gT_Par.GrpSwh.OnOff[0]) && (1 == gT_Par.GrpSwh.PreOnOff[0])) /* pre-heating  */
					{
						gT_Par.GrpSwh.PreOnOff[0] = 0;	
					}
					else
					{
						gT_Par.GrpSwh.OnOff[0] = ! gT_Par.GrpSwh.OnOff[0];
					}
				}
			
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			//				case PIC_HEATING_PLAN:
			//					if(ChangePageDly > 0)
			//					{
			//						ChangePageDly--;
			//						break;
			//					}
			//            
			//					for(i=0;i<14;i++)
			//					{
			//					if(16388 == HeatingPlanFoucs[i])
			//					{
			//					HeatingPlanFoucs[i] = 16384;
			//					}
			//					}
			//				
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
			//						{
			//						gPanelObj.SetPicture = PIC_OVERVIEW;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
			//						{
			//						gPanelObj.SetPicture = gPanelObj.OldPicture;
			//					}
			// 
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
			//						{
			//						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
			//						{
			//						gPanelObj.SetPicture = PIC_TEMP_SET_1;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
			//						{
			//						gPanelObj.SetPicture = PIC_HEATING_PLAN;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
			//						{
			//						gPanelObj.SetPicture = PIC_TEMP_TRACE_1;
			//					}
			//	           
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
			//						{
			//						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
			//					}
			//	           
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
			//						{
			//						gPanelObj.SetPicture = PIC_THICK_START;
			//					}
			//	
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
			//						{
			//						gPanelObj.SetPicture = PIC_SPEED;
			//					}
			//				
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
			//						
			//						if(gPanelObj.PasswordLevel >= 1)
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
			//							if((1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB])) ||
			//							(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
			//							)
			//							{
			//							for(i=0;i<14;i++)
			//							{
			//								if(16384 == HeatingPlanFoucs[i])
			//								{
			//									gHeatingPlan.Enable[i] = ! gHeatingPlan.Enable[i];
			//								}
			//							}
			//						}
			//					}
			//					else
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
			//							}
			//					
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
			//						{
			//						gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
			//					}
			//					break;

			case PIC_TIME_1:
			case PIC_TIME_2:
			case PIC_TIME_3:
			case PIC_TIME_4:
			case PIC_TIME_5:
			case PIC_TIME_6:
			case PIC_TIME_7:
			case PIC_TIME_8:
			case PIC_TIME_9:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_TIME_1;
				}
				if(gPanelObj.GetPicture == PIC_TIME_1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_TIME_2;
				}
				if(gPanelObj.GetPicture == PIC_TIME_2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;  
					
					if( 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash || 1 == gMachineFix.Option.HWEnable.RMold.BottomDeflash)	gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					else 	gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash || 1 == gMachineFix.Option.HWEnable.RMold.BottomDeflash) gPanelObj.SetPicture = PIC_TIME_3;
				}
				if(gPanelObj.GetPicture == PIC_TIME_3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;

					if( 1 == gMachineFix.Option.HWEnable.RMold.RobotTransfer || 1 == gMachineFix.Option.HWEnable.RMold.RobotOpnCls)	gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					else 	gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
				
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(1 == gMachineFix.Option.HWEnable.RMold.RobotTransfer || 1 == gMachineFix.Option.HWEnable.RMold.RobotOpnCls) gPanelObj.SetPicture = PIC_TIME_4;
				}
				if(gPanelObj.GetPicture == PIC_TIME_4)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;

					if( 1 == gMachineFix.Option.HWEnable.RMold.CoolPin || 1 == gMachineFix.Option.HWEnable.RMold.CoolPin2 || 1 == gMachineFix.Option.HWEnable.RMold.CoolPin3) gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					else 	gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.CoolPin || 1 == gMachineFix.Option.HWEnable.RMold.CoolPin2 || 1 == gMachineFix.Option.HWEnable.RMold.CoolPin3) gPanelObj.SetPicture = PIC_TIME_5;
				}
				if(gPanelObj.GetPicture == PIC_TIME_5)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
					
					if( 1 == gMachinePara.SupportAirTimeVersionEnable) gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					else 	gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if( 1 == gMachinePara.SupportAirTimeVersionEnable) gPanelObj.SetPicture = PIC_TIME_6;
				}
				if(gPanelObj.GetPicture == PIC_TIME_6)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_ACTIVE;
					
					if( 1 == gMachineFix.Option.HWEnable.RMold.SubMold || 1 == gMachineFix.Option.HWEnable.RMold.HookPin )gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					else 	gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.SubMold ||  1 == gMachineFix.Option.HWEnable.RMold.HookPin )gPanelObj.SetPicture = PIC_TIME_7;
				}
				if(gPanelObj.GetPicture == PIC_TIME_7)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_ACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_TIME_8;
				}
				if(gPanelObj.GetPicture == PIC_TIME_8)gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_ACTIVE;
				
				/*gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
				{
					gPanelObj.SetPicture = PIC_TIME_9;
				}
				if(gPanelObj.GetPicture == PIC_TIME_9)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_ACTIVE;*/
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					gPanelObj.SetPicture = PIC_TIME_9;
				}
				if(gPanelObj.GetPicture == PIC_TIME_9)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_ACTIVE;
			
					
					
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
              
					if(gPanelObj.GetPicture == PIC_TIME_6)
				{
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
					//							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					//							{
					//							gMachinePara.SupportAirEnable[0] = ! gMachinePara.SupportAirEnable[0];
					//						}
					//					
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_BUTTON;
					//							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					//							{
					//							gMachinePara.SupportAirEnable[1] = ! gMachinePara.SupportAirEnable[1];
					//						}
					//					
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
					//							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//							{
					//							gMachinePara.SupportAirVentEnable = !gMachinePara.SupportAirVentEnable;
					//						}
					
					if(1 == bTimeLtoR)
					{
						bTimeLtoR = 0;
						RMold.TimeSet = LMold.TimeSet;
					}
	
					if(1 == bTimeRtoL)
					{
						bTimeRtoL = 0;
						LMold.TimeSet = RMold.TimeSet;
					}
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_CLAMP_L:
			case PIC_CLAMP_R:			
			case PIC_TRANSPORT_L:
			case PIC_TRANSPORT_R:
			case PIC_ROBOT_L:
			case PIC_ROBOT_R:	
			case PIC_BLOW_PIN_L:
			case PIC_BLOW_PIN_R:
			case PIC_BLOW_PIN2_L:
			case PIC_BLOW_PIN2_R:
			case PIC_SUBMOLD:
			case PIC_DEFLASH:
			case PIC_EXTRLIFT:
			case PIC_BOTTOM_DEFLASH:
				//case PIC_ACCU_SETTING:
			case PIC_CALIB_SETTING: 
			case PIC_CALIB_SETTING_2:
				
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				/*-----------F1---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				/*-----------F2---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
				/*-----------F3---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					if(0 == bHideLeftStation)gPanelObj.SetPicture = PIC_CLAMP_L;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					
					/*-----------F4---------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_R;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
            
					/*-------------F5---------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(0 == bHideLeftStation)gPanelObj.SetPicture = PIC_TRANSPORT_L;
				}
				if(gPanelObj.GetPicture == PIC_TRANSPORT_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
			
					/*------------F6-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_TRANSPORT_R;
				}
				if(gPanelObj.GetPicture == PIC_TRANSPORT_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
					
					
					/*------------F7-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(0 == bHideLeftStation)
					{
						if(gPanelObj.GetPicture == PIC_BLOW_PIN_L && 0 == OptionHide.LBlowPin2)gPanelObj.SetPicture = PIC_BLOW_PIN2_L;
						else gPanelObj.SetPicture = PIC_BLOW_PIN_L;
						//						gPanelObj.SetPicture = PIC_BLOW_PIN_L;
					}
				}
				if(gPanelObj.GetPicture == PIC_BLOW_PIN_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
			
					/*-----------F8------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(gPanelObj.GetPicture == PIC_BLOW_PIN_R && 0 == OptionHide.RBlowPin2)gPanelObj.SetPicture = PIC_BLOW_PIN2_R;
					else gPanelObj.SetPicture = PIC_BLOW_PIN_R;
					//						gPanelObj.SetPicture = PIC_BLOW_PIN_R;
				}
				if(gPanelObj.GetPicture == PIC_BLOW_PIN_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_ACTIVE;
			
					/*----------F9-------------*/
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash &&
						( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.TopDeflash.ePressueOutputType )
						)gPanelObj.SetPicture = PIC_DEFLASH;
				}
				if( 0 == gMachineFix.Option.HWEnable.RMold.TopDeflash ||
					(PRESSURE_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.TopDeflash.ePressueOutputType && FLUX_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.TopDeflash.	eFluxOutputType) 
					)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					if(gPanelObj.GetPicture == PIC_DEFLASH)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_ACTIVE;		
					
					/*----------F10-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.BottomDeflash &&
						( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.ePressueOutputType || FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.BottomDeflash.eFluxOutputType)
						)gPanelObj.SetPicture = PIC_BOTTOM_DEFLASH;
				}
				if( 0 == gMachineFix.Option.HWEnable.RMold.BottomDeflash ||
					(PRESSURE_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.BottomDeflash.ePressueOutputType && FLUX_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.BottomDeflash.eFluxOutputType) 
					)gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					if(gPanelObj.GetPicture == PIC_BOTTOM_DEFLASH)gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_ACTIVE;	
					
												
					/*----------F11-------------*/		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.TopDeflash &&
						( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.SubMold.ePressueOutputType || FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.SubMold.eFluxOutputType)
						)gPanelObj.SetPicture = PIC_SUBMOLD;
				}
				if( 0 == gMachineFix.Option.HWEnable.RMold.SubMold ||
					(PRESSURE_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.SubMold.ePressueOutputType && FLUX_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.SubMold.eFluxOutputType) 
					)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					if(gPanelObj.GetPicture == PIC_SUBMOLD)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_ACTIVE;		
         
					/*----------F12-------------*/		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if( ACTUATOR_ELECTRIC == gMachineFix.ExtrLift.eActuatorType	)
					{
						gPanelObj.SetPicture = PIC_EXTRLIFT;
					}
				}
				if(  ACTUATOR_ELECTRIC != gMachineFix.ExtrLift.eActuatorType ) gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				
					if(gPanelObj.GetPicture == PIC_EXTRLIFT)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_ACTIVE;	 

					/*----------F13-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if( 1 == gMachineFix.Option.HWEnable.RMold.RobotTransfer &&
						( PRESSURE_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.RobotTransfer.ePressueOutputType 
						|| FLUX_OUTPUT_SYSTEM_SINGLE == gMachineFix.MoldR.RobotTransfer.eFluxOutputType
						|| ACTUATOR_ELECTRIC == gMachineFix.MoldR.RobotTransfer.eActuatorType)
						)
					{
						if( 0 == bHideLeftStation)
						{
							if(gPanelObj.GetPicture == PIC_ROBOT_R)gPanelObj.SetPicture = PIC_ROBOT_L;
							else gPanelObj.SetPicture = PIC_ROBOT_R;
						}
						else
						{
							gPanelObj.SetPicture = PIC_ROBOT_R;
						}
					}
						
				}
				if( 0 == gMachineFix.Option.HWEnable.RMold.RobotTransfer ||
					( PRESSURE_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.RobotTransfer.ePressueOutputType 
					&& FLUX_OUTPUT_SYSTEM_SINGLE != gMachineFix.MoldR.RobotTransfer.eFluxOutputType
					&& ACTUATOR_ELECTRIC != gMachineFix.MoldR.RobotTransfer.eActuatorType)
					) gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					
					if(gPanelObj.GetPicture == PIC_ROBOT_R ||gPanelObj.GetPicture == PIC_ROBOT_L )gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_ACTIVE;		
				
	
					
					/*----------F14-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
			
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.GetPicture == PIC_CALIB_SETTING )
					{
						gPanelObj.SetPicture = PIC_CALIB_SETTING_2;
					}
					else
					{
						gPanelObj.SetPicture = PIC_CALIB_SETTING;
					}

				}
				if(gPanelObj.GetPicture == PIC_CALIB_SETTING || gPanelObj.GetPicture == PIC_CALIB_SETTING_2 )gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;		
		
					
					
					/*----------F15-------------*/	
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))					
					{
					if(gPanelObj.PasswordLevel > 3) gPanelObj.SetPicture = PIC_CLAMP_CONFIG;
				}	
				if(gPanelObj.PasswordLevel <= 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
    
					/*----------F16-------------*/		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				
				break;

				//				case PIC_OPTION:
				//					if(ChangePageDly > 0)
				//					{
				//						ChangePageDly--;
				//						break;
				//					}
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
				//					{
				//					gPanelObj.SetPicture = PIC_OVERVIEW;
				//				}           
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;   
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
				//					{
				//					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
				//					{
				//					gPanelObj.SetPicture = PIC_TIME_1;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
				//					{
				//					gPanelObj.SetPicture = PIC_CLAMP_L;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
				//					{
				//					gPanelObj.SetPicture = PIC_CALIB_SETTING;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
				//					{
				//					gPanelObj.SetPicture = PIC_OPTION;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
				//					{
				//					gPanelObj.SetPicture = PIC_THICK_START;
				//				}
				//            
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//					{
				//					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				//				}
				//            
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
				//					{
				//					gPanelObj.SetPicture = PIC_SPEED;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10])) 
				//					{
				//					gPanelObj.SetPicture = PIC_RECIPE;
				//				}
				//				
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
				//					{
				//					gPanelObj.SetPicture = PIC_IO_DI_1;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
				//					{
				//					gPanelObj.SetPicture = PIC_ALARM_CURRENT;
				//				}
				//	
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
				//					{
				//					gPanelObj.SetPicture = PIC_AUTO_STEP;
				//				}
				//	
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
				//					{
				//					gPanelObj.SetPicture = PIC_AUTO_START;
				//				}
				//				
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
				//					{
				//					gPanelObj.SetPicture = PIC_SPC_CTRL;
				//				}
				//	
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
				//					{
				//					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				//				}
				//				
				//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP]))
				//					{
				//					gPanelObj.SetLanguage = 1;
				//				}
				//				
				//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN]))
				//					{
				//					gPanelObj.SetLanguage = 0;
				//				}
				//
				//
				//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ESC],KeyMatric[S$KEYMATRIX_ESC]))
				//					{
				//					gPwd.PwdLevel = 0;
				//					gPanelObj.SetPicture = PIC_INIT;
				//				}
				//				for(i=0;i<OptionDis.Count;i++)
				//				{
				//					if(1 == HotSpot[i])
				//					{
				//						HotSpot[i]  = 0;
				//						if(0 == OptionDis.pHideSource[i])
				//						{
				//							OptionDis.Current = i;
				//							OptionDis.pSource[OptionDis.Current] = !OptionDis.pSource[OptionDis.Current];
				//							gRecipeOperate.DataSave = 1;
				//						}
				//					} 
				//					OptionDis.Obj[i]= OptionDis.pSource[i];
				//				}
				//				  
				//					OptionDis.Obj[OptionDis.Current] = OptionDis.pSource[OptionDis.Current] + 2;			
				//				//			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$PAGE_FAST_SEETING],KeyMatric[S$PAGE_FAST_SEETING]))
				//				//			{
				//				//				gPanelObj.SetPicture = PIC_FAST_SETTING;
				//				//			}
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
				////						{
				////						gPanelObj.SetPicture = PIC_OVERVIEW;
				////					}
				////            
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
				////						{
				////						gPanelObj.SetPicture = gPanelObj.OldPicture;
				////					}
				////            
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
				////						{
				////						gMacOption.CutterMode = !gMacOption.CutterMode;
				////						gRecipeOperate.DataSave = 1;
				////					}
				////
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
				////						{
				////						gMacOption.Cut1stCycle = !gMacOption.Cut1stCycle;
				////						gRecipeOperate.DataSave = 1;
				////					}
				////
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
				////						{
				////						gMacOption.bDryCycle = !gMacOption.bDryCycle;
				////						gRecipeOperate.DataSave = 1;
				////					}
				////
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
				////						{
				////						gMacOption.bAutoStopPinDown = !gMacOption.bAutoStopPinDown;
				////						gRecipeOperate.DataSave = 1;
				////					}
				////				
				////					/*2018.7.11 Albert Add option for enable ACC function*/	
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
				////						{
				////						gMacOption.ACCPumpCharge = !gMacOption.ACCPumpCharge;
				////						gRecipeOperate.DataSave = 1;
				////					}		
				////										
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				////						{
				////						gMacOption.HighPrsMotorEnable = !gMacOption.HighPrsMotorEnable;
				////						gRecipeOperate.DataSave = 1;
				////					}		
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;	
				////						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				////						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
				////				
				////						if(0 == gMachineFix.Option.bHideLeftStation)
				////					{
				////						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
				////							gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_BUTTON;
				////							gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
				////							}
				////					else
				////					{
				////						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12]  = STATE_INACTIVE;	
				////							gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				////							gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				////							}
				////					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
				////						
				////						gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
				////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
				////						{
				////						gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				////					}
				////
				////					/***************************选项控制*******************************/
				////					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) /* UP ARROW  */
				////						{
				////						for(i=0;i<OptionDis.Count;i++)
				////						{
				////							if(0 == OptionDis.Current)
				////							{
				////								OptionDis.Current = OptionDis.Count - 1;
				////							}
				////							else
				////							{
				////								OptionDis.Current--;
				////							}
				////
				////							if(0 == OptionDis.pHideSource[OptionDis.Current])
				////							{
				////								break;
				////							}
				////						}
				////					}
				////
				////					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
				////						{
				////						for(i=0;i<OptionDis.Count;i++)
				////						{
				////							if(OptionDis.Current >= (OptionDis.Count-1))
				////							{
				////								OptionDis.Current = 0;
				////							}
				////							else
				////							{
				////								OptionDis.Current++;
				////							}
				////					
				////							if(0 == OptionDis.pHideSource[OptionDis.Current])
				////							{
				////								break;
				////							}
				////						}
				////					}
				////
				////					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
				////						{
				////						for(i=0;i<(OptionDis.Count/OptionDis.Section);i++)
				////						{
				////							OptionDis.Current = OptionDis.Current - OptionDis.Section;
				////							if(OptionDis.Current < 0)
				////							{
				////								OptionDis.Current = OptionDis.Current + OptionDis.Count;
				////							}
				////					
				////							if(0 == OptionDis.pHideSource[OptionDis.Current])
				////							{
				////								break;
				////							}
				////						}
				////					}
				////
				////					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
				////						{
				////						for(i=0;i<(OptionDis.Count/OptionDis.Section);i++)
				////						{
				////							OptionDis.Current = OptionDis.Current + OptionDis.Section;
				////							if(OptionDis.Current > OptionDis.Count-1)
				////							{
				////								OptionDis.Current = OptionDis.Current - OptionDis.Count;
				////							}
				////							if(0 == OptionDis.pHideSource[OptionDis.Current])
				////							{
				////								break;
				////							}
				////						}
				////					}
				////
				////					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB]))
				////						{
				////						/*  hide function no operation  */
				////						if(0 == OptionDis.pHideSource[OptionDis.Current])
				////						{
				////							OptionDis.pSource[OptionDis.Current] = !OptionDis.pSource[OptionDis.Current];
				////							gRecipeOperate.DataSave = 1;
				////						}
				////					}
				////

				break;
			
			
			case PIC_CONFIG_HELP:
			case PIC_CLAMP_CONFIG:
			case PIC_TRANSPORT_CONFIG:
			case PIC_BLOW_PIN_CONFIG:
			case PIC_EXTRLIFT_CONFIG:
			case PIC_CLAMP_TOGGLE:
			case PIC_CARRIAGE_TOGGLE:
			case PIC_EXTRLIFT_TOGGLE:	
				
				
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				/*-----------F1---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				/*-----------F2---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
				/*-----------F3---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_CONFIG;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_CONFIG)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					
					/*-----------F4---------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_TRANSPORT_CONFIG;
				}
				if(gPanelObj.GetPicture == PIC_TRANSPORT_CONFIG)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
            
					/*-------------F5---------*/
					//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_BLOW_PIN_CONFIG;
				}
				if(gPanelObj.GetPicture == PIC_BLOW_PIN_CONFIG)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
			
					/*------------F6-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_EXTRLIFT_CONFIG;
				}
				if(gPanelObj.GetPicture == PIC_EXTRLIFT_CONFIG)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
					
					
					/*------------F7-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;

			
					/*-----------F8------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
			
					/*----------F9-------------*/	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;			
					
					/*----------F10-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					
					
					/*----------F11-------------*/					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_TOGGLE;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_TOGGLE)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_ACTIVE;
         
					/*----------F12-------------*/		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_CARRIAGE_TOGGLE;
				}
				if(gPanelObj.GetPicture == PIC_CARRIAGE_TOGGLE)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_ACTIVE;												
					

					/*----------F13-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					gPanelObj.SetPicture = PIC_EXTRLIFT_TOGGLE;
				}
				if(gPanelObj.GetPicture == PIC_EXTRLIFT_TOGGLE)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_ACTIVE;	
							
					
					/*----------F14-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					gPanelObj.SetPicture = PIC_CONFIG_HELP;
				}
				if(gPanelObj.GetPicture == PIC_CONFIG_HELP)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;		
		
					
					
					/*----------F15-------------*/	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_L;
					if(1 == bHideLeftStation)gPanelObj.SetPicture = PIC_CLAMP_R;
				}	
    
				/*----------F16-------------*/		
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				
				
				break;

			case PIC_RECIPE:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
            
					if(1 == gRecipeOperate.usbDiskOk)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_BUTTON;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_BUTTON;
						}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
						}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_BUTTON;
		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			//				case PIC_SPEED:
			//					if(ChangePageDly > 0)
			//					{
			//						ChangePageDly--;
			//						break;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
			//						{
			//						gPanelObj.SetPicture = PIC_OVERVIEW;
			//					}
			//				
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
			//						{
			//						gPanelObj.SetPicture = gPanelObj.OldPicture;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
			////						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
			////						{
			////						gPanelObj.SetPicture =PIC_LENGTH;
			////					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
			//               
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
			//						{
			//						gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
			//						{
			//						gPanelObj.SetPicture =PIC_THICK_START;
			//					}
			//				
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
			//					
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
			//						{
			//						gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
			//					}
			//					break;
        
			case PIC_LENGTH:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture =PIC_SPEED;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture =PIC_SPEED;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_BUTTON;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gMachinePara.LengthPara.LengthEnable = !gMachinePara.LengthPara.LengthEnable;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
        
			case PIC_THICK_START:
			case PIC_THICK_DIVER:
				
				if(1 == gThickProf.FunOption.SealMode) bHideSealGap = 0;
				else bHideSealGap = 1;

				if(ChangePageDly > 0)
				{
				ChangePageDly--;
				break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CURVE_TRACE;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_MARKER_SET_A;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_CURVEDATA_A;
				}

        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//	if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}					
				break;

			case PIC_CURVE_TRACE:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CURVE_TRACE;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_MARKER_SET_A;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_CURVEDATA_A;
				}

        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)				
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//	if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}										
				break;
         
			case PIC_CURVE_EDIT:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CURVE_TRACE;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_MARKER_SET_A;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_CURVEDATA_A;
				}

        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;   
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}   
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)				
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
			
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE; 
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_BUTTON;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}										
				break;

			case PIC_MARKER_SET_A:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CURVE_TRACE;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_MARKER_SET_A;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_CURVEDATA_A;
				}

        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)				
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
            
				MarkerDis.Current = MarkerDis.Current % MarkerDis.Count;
            
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) /* UP ARROW  */
					{
					if(0 == MarkerDis.Current)
					{
						MarkerDis.Current = MarkerDis.Count - 1;
					}
					else
					{
						MarkerDis.Current--;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(MarkerDis.Current >= (MarkerDis.Count-1))
					{
						MarkerDis.Current = 0;
					}
					else
					{
						MarkerDis.Current++;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
					{
					MarkerDis.Current = MarkerDis.Current - MarkerDis.Section;
					if(MarkerDis.Current < 0)
					{
						MarkerDis.Current = MarkerDis.Current + MarkerDis.Count;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
					{
					MarkerDis.Current = MarkerDis.Current + MarkerDis.Section;
					if(MarkerDis.Current > MarkerDis.Count - 1)
					{
						MarkerDis.Current = MarkerDis.Current - MarkerDis.Count;
					}
				}

				if(MarkerDis.Current >= MarkerDis.Count)MarkerDis.Current = 0;

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB]))
					{
					MarkerDis.pSource[MarkerDis.Current] = !MarkerDis.pSource[MarkerDis.Current];
				}

				for(i=0;i<100;i++)
				{
					MarkerDis.Obj[i] = MarkerDis.pSource[i];
				}

				MarkerDis.Obj[MarkerDis.Current] = MarkerDis.pSource[MarkerDis.Current] + 2;
				break;

			case PIC_CURVEDATA_A:
			case PIC_CURVEDATA_B:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CURVE_EDIT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CURVE_TRACE;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_MARKER_SET_A;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_CURVEDATA_A;
				}

        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_TEMP_CONSTRUCT;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_SPEED;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)				
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_IO_DI_1:
			case PIC_IO_DI_2:
			case PIC_IO_DI_3:
			case PIC_IO_AI_1:
			case PIC_IO_MODULE:
			case PIC_IO_DRIVER_1:
			case PIC_IO_DRIVER_2:
			case PIC_ENERGY_MONITOR_ALL:
			case PIC_ENERGY_MONITOR:
			case PIC_MOTOR_TREND:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
  
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					//					if(PIC_IO_DI_1 == gPanelObj.GetPicture)
					//					{
					//						gPanelObj.SetPicture = PIC_IO_DI_2;
					//					}					
					//					else
					//					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
					//					}
				}
				
														
				if(gPanelObj.GetPicture >= PIC_IO_DI_1 && gPanelObj.GetPicture <= PIC_IO_DI_3)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
						}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}
				if(gPanelObj.GetPicture == PIC_IO_AI_1)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
						}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}
				if(gPanelObj.GetPicture == PIC_IO_MODULE)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
						}
         
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8]  = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;
					
					if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						if(gPanelObj.GetPicture != PIC_ENERGY_MONITOR_ALL)
					//						{
					//							gPanelObj.SetPicture = PIC_ENERGY_MONITOR_ALL;
					//						}
					//						else
					//						{
					//							gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//						}
					//					}
					//					if(gPanelObj.GetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			
			
			//				case PIC_MOTOR_TREND:
			//					if(ChangePageDly > 0)
			//					{
			//						ChangePageDly--;
			//						break;
			//					}
			//	
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
			//						{
			//						gPanelObj.SetPicture = PIC_OVERVIEW;
			//					}
			//	
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
			//						{
			//						gPanelObj.SetPicture = gPanelObj.OldPicture;
			//					}
			//	  
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_DI_1;
			//						//						if(PIC_IO_DI_1==gPanelObj.GetPicture)
			//						//						{
			//						//							gPanelObj.SetPicture = PIC_IO_DI_2;
			//						//						}					
			//						//						else
			//						//						{
			//						//							gPanelObj.SetPicture = PIC_IO_DI_1;
			//						//						}
			//					}
			//					
			//															
			//					if(gPanelObj.GetPicture >= PIC_IO_DI_1 && gPanelObj.GetPicture <= PIC_IO_DI_3)
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
			//							}
			//	            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_DO_1;
			//					}
			//	
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_AI_1;
			//					}
			//					if(gPanelObj.GetPicture == PIC_IO_AI_1)
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
			//							}
			//	            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_AO_1;
			//					}
			//	
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_MODULE;
			//					}
			//					if(gPanelObj.GetPicture == PIC_IO_MODULE)
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
			//							}
			//	         
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8]  = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;
			//						
			//						if (gPanelObj.PasswordLevel <= 3) 
			//					{
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
			//							}	
			//					else
			//					{
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
			//							{
			//							gPanelObj.SetPicture = PIC_MOTOR_TREND;
			//						}
			//					}
			//					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
			//						//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
			//						//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
			//						//						{
			//						//						if(gPanelObj.GetPicture != PIC_ENERGY_MONITOR_ALL)
			//						//						{
			//						//							gPanelObj.SetPicture = PIC_ENERGY_MONITOR_ALL;
			//						//						}
			//						//						else
			//						//						{
			//						//							gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
			//						//						}
			//						//					}
			//						//					if(gPanelObj.GetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
			//					
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
			//	
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
			//						{
			//						gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
			//					}
			//					break;
			//				case PIC_AUTO_START:
			//				case PIC_AUTO_STEP:
			//					if(ChangePageDly > 0)
			//					{
			//						ChangePageDly--;
			//						break;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
			//						{
			//						gPanelObj.SetPicture = PIC_OVERVIEW;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
			//						{
			//						gPanelObj.SetPicture = gPanelObj.OldPicture;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_DI_1;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_DO_1;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_AI_1;
			//					}
			//
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_AO_1;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
			//						{
			//						gPanelObj.SetPicture = PIC_IO_MODULE;
			//					}
			//            
			//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8]  = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
			//					
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
			//						{
			//						gPanelObj.SetPicture = PIC_AUTO_STEP;
			//					}
			//					if(gPanelObj.GetPicture == PIC_AUTO_STEP)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_ACTIVE;
			//
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
			//						{
			//						gPanelObj.SetPicture = PIC_AUTO_START;
			//					}
			//					if(gPanelObj.GetPicture == PIC_AUTO_START)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;	
			//
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
			//	
			//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
			//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
			//						{
			//						gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
			//					}
			//					break;
      
			case PIC_IO_AO_1:			/*  AO FORCE   */
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
 
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
			
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) /* UP ARROW  */
					{
					if(0 == IndexAo)
					{
						IndexAo = 3;
					}
					else
					{
						IndexAo = IndexAo-1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(3 == IndexAo)
					{
						IndexAo = 0;
					}
					else
					{
						IndexAo = IndexAo+1;
					}
				}

				if(IndexAo > 3)IndexAo = 3;
				if(IndexAo < 0)IndexAo = 0;

				for(i=0;i<4;i++)
				{
					AoColor[i] = 0x00FA;
				}
				if(1 == gTKcalib.Status && gPanelObj.PasswordLevel > 2)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
						bHideForceAO = 0;
					AoColor[IndexAo] = 0x22A9;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
						{
						if(gHardWare.AOForceNum > 3)
						{
							gHardWare.AOForceNum = IndexAo;
							gImply.ForceAO = 1;
						}
						else
						{
							gHardWare.AOForceNum = 19;
							gImply.ForceAO = 0;
						}
						
						//							gHardWare.AOForceNum[IndexAo] = !gHardWare.AOForceNum[IndexAo];
							
					}
				}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						bHideForceAO = 1;
					gHardWare.AOForceNum = 19;
					if(1 == gImply.ForceAO)gImply.ForceAO = 0;
				}
				
				if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//					}
					//					if(gPanelObj.SetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			
			case PIC_IO_DO_1:			/*  DO FORCE   */
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_3;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
               
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) 	/* UP ARROW  */
					{
					if(0 == IndexDo)
					{
						IndexDo = MAX_PAGE_DO32 - 1;
					}
					else
					{
						IndexDo = IndexDo-1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(MAX_PAGE_DO48 - 1 == IndexDo)
					{
						IndexDo = 0;
					}
					else
					{
						IndexDo = IndexDo+1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
					{
					IndexDo = IndexDo - 8;
					if(IndexDo < 0)IndexDo = MAX_PAGE_DO32 + IndexDo;
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
					{
					IndexDo = IndexDo + 8;
					if(IndexDo > MAX_PAGE_DO32 - 1)IndexDo = IndexDo - MAX_PAGE_DO32;
				}

				if(IndexDo > MAX_PAGE_DO32 - 1)IndexDo = MAX_PAGE_DO - 1;
				if(IndexDo < 0 )IndexDo = 0;

				for(i=0;i<MAX_PAGE_DO32;i++)
				{
					DoColor[i] = 0x00FA;
				}

				if(1 == gTKcalib.Status && gPanelObj.PasswordLevel > 2)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
						DoColor[IndexDo] = 0x22A9;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
						{
						gHardWare.DOForce[IndexDo] = !gHardWare.DOForce[IndexDo];
					}
				}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						}
				
				if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//					}
					//					if(gPanelObj.SetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_IO_DO_2:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_3;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;

					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) 	/* UP ARROW  */
					{
					if(0 == IndexDo)
					{
						IndexDo = MAX_PAGE_DO - 1;
					}
					else
					{
						IndexDo = IndexDo-1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(MAX_PAGE_DO - 1 == IndexDo)
					{
						IndexDo = 0;
					}
					else
					{
						IndexDo = IndexDo+1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
					{
					IndexDo = IndexDo - 16;
					if(IndexDo < 0)IndexDo = MAX_PAGE_DO + IndexDo;
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
					{
					IndexDo = IndexDo + 16;
					if(IndexDo > MAX_PAGE_DO - 1)IndexDo = IndexDo - MAX_PAGE_DO;
				}

				if(IndexDo > MAX_PAGE_DO - 1)IndexDo = MAX_PAGE_DO - 1;
				if(IndexDo < 0 )IndexDo = 0;

				for(i=0;i<MAX_PAGE_DO;i++)
				{
					DoColor[i + MAX_PAGE_DO] = 0x00FA;
				}

				if(1 == gTKcalib.Status && gPanelObj.PasswordLevel > 2)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
						DoColor[IndexDo + MAX_PAGE_DO] = 0x22A9;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
						{
						gHardWare.DOForce[IndexDo + MAX_PAGE_DO] = !gHardWare.DOForce[IndexDo + MAX_PAGE_DO];
					}
				}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						}
				
				if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//					}
					//					if(gPanelObj.SetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_IO_DO_3:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
               
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) 	/* UP ARROW  */
					{
					if(0 == IndexDo)
					{
						IndexDo = MAX_PAGE_DO48 - 1;
					}
					else
					{
						IndexDo = IndexDo - 1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(MAX_PAGE_DO48 - 1 == IndexDo)
					{
						IndexDo = 0;
					}
					else
					{
						IndexDo = IndexDo+1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
					{
					IndexDo = IndexDo - 16;
					if(IndexDo < 0)IndexDo = MAX_PAGE_DO48 + IndexDo;
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
					{
					IndexDo = IndexDo + 16;
					if(IndexDo > MAX_PAGE_DO48 - 1)IndexDo = IndexDo - MAX_PAGE_DO48;
				}

				if(IndexDo > MAX_PAGE_DO48 - 1)IndexDo = MAX_PAGE_DO48 - 1;
				if(IndexDo < 0 )IndexDo = 0;

				for(i=0;i<MAX_PAGE_DO48;i++)
				{
					DoColor[i + MAX_PAGE_DO32] = 0x00FA;
				}

				if(1 == gTKcalib.Status && gPanelObj.PasswordLevel > 2)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
						DoColor[IndexDo + MAX_PAGE_DO32] = 0x22A9;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
						{
						gHardWare.DOForce[IndexDo + MAX_PAGE_DO32] = !gHardWare.DOForce[IndexDo + MAX_PAGE_DO32];
					}
				}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						}

				if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//					}
					//					if(gPanelObj.SetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			case PIC_IO_DO_4:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_IO_DI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_IO_DO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_IO_AI_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_IO_AO_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					gPanelObj.SetPicture = PIC_IO_MODULE;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
               
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP])) 	/* UP ARROW  */
					{
					if(0 == IndexDo)
					{
						IndexDo = MAX_PAGE_DO - 1;
					}
					else
					{
						IndexDo = IndexDo - 1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN])) /* DOWN ARROW  */
					{
					if(MAX_PAGE_DO - 1 == IndexDo)
					{
						IndexDo = 0;
					}
					else
					{
						IndexDo = IndexDo+1;
					}
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_LEFT],KeyMatric[S$KEYMATRIX_LEFT])) /* LEFT ARROW  */
					{
					IndexDo = IndexDo - 16;
					if(IndexDo < 0)IndexDo = MAX_PAGE_DO + IndexDo;
				}

				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT],KeyMatric[S$KEYMATRIX_RIGHT])) /* RIGHT ARROW  */
					{
					IndexDo = IndexDo + 16;
					if(IndexDo > MAX_PAGE_DO - 1)IndexDo = IndexDo - MAX_PAGE_DO;
				}

				if(IndexDo > MAX_PAGE_DO - 1)IndexDo = MAX_PAGE_DO - 1;
				if(IndexDo < 0 )IndexDo = 0;

				for(i=0;i<MAX_PAGE_DO;i++)
				{
					DoColor[i + MAX_PAGE_DO32 + MAX_PAGE_DO] = 0x00FA;
				}

				if(1 == gTKcalib.Status && gPanelObj.PasswordLevel > 2)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
						DoColor[IndexDo +MAX_PAGE_DO32 + MAX_PAGE_DO] = 0x22A9;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
						{
						gHardWare.DOForce[IndexDo + MAX_PAGE_DO32 + MAX_PAGE_DO] = !gHardWare.DOForce[IndexDo + MAX_PAGE_DO32 + MAX_PAGE_DO];
					}
				}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						}

				if (gPanelObj.PasswordLevel <= 3) 
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						}	
				else
				{
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
						{
						gPanelObj.SetPicture = PIC_MOTOR_TREND;
					}
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					//						{
					//						gPanelObj.SetPicture = PIC_ENERGY_MONITOR;
					//					}
					//					if(gPanelObj.SetPicture == PIC_ENERGY_MONITOR)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			case PIC_ALARM_CURRENT:
			case PIC_ALARM_HISTORY:
			case PIC_ALARM_1:
			case PIC_ALARM_2:
			case PIC_ALARM_3:
			case PIC_ALARM_4:
			case PIC_ALARM_5:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_ALARM_CURRENT;
				}
				if(gPanelObj.GetPicture == PIC_ALARM_CURRENT)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_ALARM_HISTORY;
				}
				if(gPanelObj.GetPicture == PIC_ALARM_HISTORY)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}        
				break;

			case PIC_PASSWORD_LOGIN:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_SYSTEM_SET_1 ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_ACCU_SETTING ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(gPanelObj.PasswordLevel > 3 )gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}
				if(gPanelObj.PasswordLevel <= 3 )gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_PASSWORD_SET;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					/*Chaoi*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PRESSURE_CALIBRATION ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//				{
					//					if(0 == bLevel3CalibHide)
					//					{
					//						gPanelObj.SetPicture = PIC_MOLD_CALIBRATION;
					//					}
					//				}
					//				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bLevel3CalibHide)
					{
						if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
						}
						else if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType )
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
						}
					}
				}
				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;

					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;	
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if(gPanelObj.PasswordLevel > 3 && 1 == gTKcalib.Status)
					{
						gPanelObj.SetPicture = PIC_PROPORTION_VALVE;
					}
				}
				if(1 == bHidePropValvePage)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
						
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				}
					
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PV_LOG;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_LOG_INFO;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_INDUSTY_40;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_IP_CONFIG;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_VNC;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}        

				if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_TAB]&& 1 == gPanelObj.KeyMatric[S$KEYMATRIX_ENTER])
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_DEFAULT_SET  ;
				}
				break;

			case PIC_SYSTEM_SET_1:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_SYSTEM_SET_2;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_ACCU_SETTING;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(gPanelObj.PasswordLevel > 3)gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}
				if(gPanelObj.PasswordLevel <= 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_PASSWORD_SET;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PRESSURE_CALIBRATION ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//				{
					//					if(0 == bLevel3CalibHide)
					//					{
					//						gPanelObj.SetPicture = PIC_MOLD_CALIBRATION;
					//					}
					//				}
					//				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bLevel3CalibHide)
					{
						if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
						}
						else if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType )
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
						}
					}
				}
				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
	
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;	
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if(gPanelObj.PasswordLevel > 3 && 1 == gTKcalib.Status)
					{
						gPanelObj.SetPicture = PIC_PROPORTION_VALVE;
					}
				}
				if(1 == bHidePropValvePage)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
							
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				}
						
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PV_LOG;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
	               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_LOG_INFO;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
	            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_INDUSTY_40;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_IP_CONFIG;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_VNC;
				}
				
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
				//            		gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//						{
				//						gMachineInfo.CycleTimes = 0;
				//						gCycleTimes             = 0;
				//						gMachineInfo.GoodBottles = 0;
				//					}
            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
				//						{
				//						gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				//					}
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}        
				break;

			case PIC_SYSTEM_SET_2:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_SYSTEM_SET_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_ACCU_SETTING;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(gPanelObj.PasswordLevel > 3)gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}
				if(gPanelObj.PasswordLevel <= 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					
				
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_PASSWORD_SET;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PRESSURE_CALIBRATION ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//				{
					//					if(0 == bLevel3CalibHide)
					//					{
					//						gPanelObj.SetPicture = PIC_MOLD_CALIBRATION;
					//					}
					//				}
					//				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bLevel3CalibHide)
					{
						if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
						}
						else if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType )
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
						}
					}
				}
				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
	
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;	
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if(gPanelObj.PasswordLevel > 3 && 1 == gTKcalib.Status)
					{
						gPanelObj.SetPicture = PIC_PROPORTION_VALVE;
					}
				}
				if(1 == bHidePropValvePage)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
							
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				}
						
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PV_LOG;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
	               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_LOG_INFO;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
	            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_INDUSTY_40;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_IP_CONFIG;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_VNC;
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
				//						{
				//						gMachineFix.Option.AlarmTextMode  = !gMachineFix.Option.AlarmTextMode;
				//					}
				//            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//						{
				//						gMachineFix.Option.AlarmLedMode  = !gMachineFix.Option.AlarmLedMode;
				//					}
				//            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
				//						{
				//						/*gPwd.Language = ! gPwd.Language;*/
				//						gPanelObj.SetLanguage = gPanelObj.GetLanguage + 1;
				//						if(gPanelObj.SetLanguage > 2)gPanelObj.SetLanguage = 1;
				//					}
				//            		gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
				//						{
				//						if(1 == gRecipeOperate.usbDiskOk)gScreenShot.CmdScreenShotAll = 1;
				//					}
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
               
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_SYSTEM_SET_3:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_SYSTEM_SET_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_SYSTEM_SET_2;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}
					
				
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_PASSWORD_SET;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PRESSURE_CALIBRATION ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//				{
					//					if(0 == bLevel3CalibHide)
					//					{
					//						gPanelObj.SetPicture = PIC_MOLD_CALIBRATION;
					//					}
					//				}
					//				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bLevel3CalibHide)
					{
						if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
						}
						else if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType )
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
						}
					}
				}
				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
	
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;	
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if(gPanelObj.PasswordLevel > 3 && 1 == gTKcalib.Status)
					{
						gPanelObj.SetPicture = PIC_PROPORTION_VALVE;
					}
				}
				if(1 == bHidePropValvePage)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
							
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				}
						
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PV_LOG;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
	               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_LOG_INFO;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
	            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_INDUSTY_40;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_IP_CONFIG;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_VNC;
				}
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
				//						{
				//						gMachineFix.Option.AlarmTextMode  = !gMachineFix.Option.AlarmTextMode;
				//					}
				//            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//						{
				//						gMachineFix.Option.AlarmLedMode  = !gMachineFix.Option.AlarmLedMode;
				//					}
				//            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
				//						{
				//						/*gPwd.Language = ! gPwd.Language;*/
				//						gPanelObj.SetLanguage = gPanelObj.GetLanguage + 1;
				//						if(gPanelObj.SetLanguage > 2)gPanelObj.SetLanguage = 1;
				//					}
				//            		gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
				//						{
				//						if(1 == gRecipeOperate.usbDiskOk)gScreenShot.CmdScreenShotAll = 1;
				//					}
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
               
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			
			
			
			case PIC_OEM_SETTING_1:  // 180
			case PIC_OEM_SETTING_2:  // 181
			case PIC_OEM_SETTING_3:  // 182
			case PIC_OEM_SETTING_4:  // 183
			case PIC_OEM_SETTING_10:  // 189
				
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_2;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_3;
				}
					
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_4;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_OEM_SETTING_10;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
	
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}        
				break;
			
			
			
			
			case PIC_IP_CONFIG: //135
			case PIC_PRESSURE_CALIBRATION:  //134
			case PIC_LOG_INFO:  //137
			case PIC_INDUSTY_40:	//125
			case PIC_VNC:	//138
			case PIC_PROPORTION_VALVE: //133
			case PIC_ACCU_SETTING: //68
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
		
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
		
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
		            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_SYSTEM_SET_1;
				}
		
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_ACCU_SETTING;
				}
		
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(gPanelObj.PasswordLevel > 3)gPanelObj.SetPicture = PIC_OEM_SETTING_1;
				}
				if(gPanelObj.PasswordLevel <= 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_PASSWORD_SET;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PRESSURE_CALIBRATION ;
				}
				if(gPanelObj.PasswordLevel <= 2)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					//				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					//				{
					//					if(0 == bLevel3CalibHide)
					//					{
					//						gPanelObj.SetPicture = PIC_MOLD_CALIBRATION;
					//					}
					//				}
					//				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
								
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(0 == bLevel3CalibHide)
					{
						if(ACTUATOR_ELECTRIC == gMachineFix.MoldR.Clamp.eActuatorType)
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
						}
						else if(ACTUATOR_HYD == gMachineFix.MoldR.Clamp.eActuatorType )
						{
							gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
						}

					}
				}
				if(1 == bLevel3CalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
			
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;	
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					if(gPanelObj.PasswordLevel > 3 && 1 == gTKcalib.Status)
					{
						gPanelObj.SetPicture = PIC_PROPORTION_VALVE;
					}
				}
				if(1 == bHidePropValvePage)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
									
								
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				}
								
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_PV_LOG;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
			               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					if(gPanelObj.PasswordLevel > 2)gPanelObj.SetPicture = PIC_LOG_INFO;
				}
				if(gPanelObj.PasswordLevel < 3)gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
			            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_INDUSTY_40;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
								
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_IP_CONFIG;
				}
				if(gPanelObj.PasswordLevel < 1)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
			
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					if(gPanelObj.PasswordLevel > 0)gPanelObj.SetPicture = PIC_VNC;
				}
						
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
				//            		gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//						{
				//						gMachineInfo.CycleTimes = 0;
				//						gCycleTimes             = 0;
				//						gMachineInfo.GoodBottles = 0;
				//					}
		            
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
				//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
				//						{
				//						gPanelObj.SetPicture = PIC_SCREEN_CALIBRATE;
				//					}
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				//						gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;
			
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}        
				
				
				break;

			case PIC_PASSWORD_SET: 
				//	case PIC_VALVE_DELAY:
			case PIC_PV_LOG: 
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;   
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
         
			case PIC_SPC_CTRL:
			case PIC_SPC_1:
			case PIC_SPC_2:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_SPC_CTRL;
				}
				
				if(PIC_SPC_CTRL == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3]=STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_SPC_1; 
				}
				
				if(PIC_SPC_1 == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4]=STATE_ACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_SPC_2; 
				}
				
				if(PIC_SPC_2 == gPanelObj.GetPicture)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5]=STATE_ACTIVE;
            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE; 
				
					if(gPanelObj.GetPicture != PIC_SPC_CTRL)
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
						}
				else
				{
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
						}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;


			case PIC_HIGH_PASSWORD:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					if(PIC_HIGH_PASSWORD == gPanelObj.GetPicture)
					{
						gPwd.Option[0] = !gPwd.Option[0];
						gPwd.wSavePassword = 1;
					}
				}
				if(gPanelObj.GetPicture == PIC_HIGH_PASSWORD)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_BUTTON;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(PIC_HIGH_PASSWORD == gPanelObj.GetPicture)
					{
						gPwd.Option[1] = !gPwd.Option[1];
						gPwd.wSavePassword = 1;
					}
				}
				if(gPanelObj.GetPicture == PIC_HIGH_PASSWORD)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_BUTTON;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if(PIC_HIGH_PASSWORD == gPanelObj.GetPicture)
					{
						gPwd.Option[2] = !gPwd.Option[2];
						gPwd.wSavePassword = 1;
					}
				}
				if(gPanelObj.GetPicture == PIC_HIGH_PASSWORD)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if(PIC_HIGH_PASSWORD == gPanelObj.GetPicture)
					{
						gMachineStopDays = 0;

						gPwd.OK_3 = 0;
						gPwd.OK_2 = 0;
						gPwd.OK_1 = 0;

						gPwd.wSavePassword = 1;
					}
				}
				if(gPanelObj.GetPicture == PIC_HIGH_PASSWORD)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
               
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					if(gPanelObj.GetPicture == PIC_INDUSTY_40)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_ACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_INIT_SET:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;

					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_THICK_DIVER;
				}
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
				//					{
				//					gThickProf.FunOption.bLengthUnit = !gThickProf.FunOption.bLengthUnit;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				//            
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
				//					{
				//					gThickProf.FunOption.IsTraceInEdit = !gThickProf.FunOption.IsTraceInEdit;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
				//					{
				//					gThickProf.FunOption.Is100_300 = !gThickProf.FunOption.Is100_300;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
				//					{
				//					gThickProf.FunOption.IsZeroDn = !gThickProf.FunOption.IsZeroDn;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				//
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
				//					{
				//					gThickProf.FunOption.SealMode = !gThickProf.FunOption.SealMode;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				//			
				//				gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
				//					
				//					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_BUTTON;
				//					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
				//					{
				//					gThickProf.FunOption.Servo = !gThickProf.FunOption.Servo;
				//					gRecipeOperate.OnceSave = 1;
				//				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//	if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				
				if(gThickProf.ShareSet.StartFlow > 998)gThickProf.ShareSet.StartFlow = 998;
				if(gThickProf.ShareSet.SealFlow  > 998)gThickProf.ShareSet.SealFlow  = 998;
				if(gThickProf.ShareSet.ThickFlow > 998)gThickProf.ShareSet.ThickFlow = 998;

				if(gThickProf.ShareSet.StartFlow < 100)gThickProf.ShareSet.StartFlow = 100;
				if(gThickProf.ShareSet.SealFlow  < 100)gThickProf.ShareSet.SealFlow  = 100;
				if(gThickProf.ShareSet.ThickFlow < 100)gThickProf.ShareSet.ThickFlow = 100;
				break;

			case PIC_GAP_CALIBRATE:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
        
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_THICK_START;
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4]  = STATE_INACTIVE;
					
		
					
					if(gMachineFix.Parison.eActuatorType == ACTUATOR_ELECTRIC)
				{
					//Electric
				
					if (gPanelObj.GetPicture ==PIC_GAP_CALIBRATE )
					{
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_BUTTON;
							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickIO.Step = STEP_THICK_CALIB_DOWN;
							}
						}
			
						if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F5],keyF5N))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickIO.Step = STEP_THICK_STOP;
							}
						}

						gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickIO.Step = STEP_THICK_CALIB_UP;
							}
						}
			
						if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F6],keyF6N))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickIO.Step = STEP_THICK_STOP;
							}
						}
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickFix.ecMin = gThickIO.ecAs;
								gRecipeOperate.OnceSave = 1;
							}
						}

						gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
							if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
							{
							if(1 == gMachineInfo.Calib)
							{
								gThickFix.ecMax = gThickIO.ecAs;
								gRecipeOperate.OnceSave = 1;
							}
						}
					}
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
						{
						gPanelObj.SetPicture = PIC_THICK_DIVER;
					}
				
				}
				else if(gMachineFix.Parison.eActuatorType == ACTUATOR_HYD)
				{
					//Hydraulic
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_BUTTON;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_BUTTON;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_BUTTON;
						if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_F7])
						{
						cntF7 = cntF7 + 1;
					}
					else
					{
						cntF7 = 0;
					}
					if(50 == cntF7)
					{
						if(gServoLinear.Current > 0 && gServoLinear.Current < 32767)
						{
							gServoLinear.ZeroPos     = gServoLinear.Current;
							gRecipeOperate.OnceSave = 1;
						}
					}
		
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
						if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_F8])
						{
						cntF8 = cntF8 + 1;
					}
					else
					{
						cntF8 = 0;
					}
						
					if(50 == cntF8)
					{
						if(gServoLinear.Current > 0 && gServoLinear.Current < 32767)
						{
							if(gServoLinear.Current > gServoLinear.ZeroPos)
							{
								gServoLinear.MaxPos      = gServoLinear.Current;
								gRecipeOperate.OnceSave = 1;
							}
						}
					}
				
					// auto calibration
					if(1 == gThickProf.FunOption.IsAutoCalibrate)
					{
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_SUB_BUTTON;
							bHideParisonAutoCalib = 0;
					
						if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_F5] || 1 == gPanelObj.KeyMatric[S$KEYMATRIX_F6] || 
							1 == gPanelObj.KeyMatric[S$KEYMATRIX_F7] || 1 == gPanelObj.KeyMatric[S$KEYMATRIX_F8])
							{
							gImply.ParisonCalibNotManual = 1;
						}
					}
					else
					{
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
							bHideParisonAutoCalib = 1;
					}
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9]  = STATE_INACTIVE;
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					
						gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
				
						}
				
				/*-------------------F13----------------------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					
					{
						gPanelObj.SetPicture = PIC_INIT_SET  ;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
	
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
						
					/*-------------------F14----------------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGENEG(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					//	if(gPanelObj.PasswordLevel > 2 &&  1 == gTKcalib.Status)
					if(gPanelObj.PasswordLevel > 2 &&  0 == gTKauto.Status)
					{
						gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
						OldSet    = gThickProf.ShareSet;
						OldFunSet = gThickProf.FunOption;
					}
				}
				//if(1 == bThickCalibHide)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
				if(1 == gTKauto.Status)gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					gPanelObj.SetPicture = PIC_GAP_CALIBRATE_HELP;
				}           
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_GAP_CALIBRATE_HELP:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;

			case PIC_DEFAULT_SET:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_GAP_CALIBRATE;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
            
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}         
				break;

			
			case PIC_CALIB_CLAMP_ENCODER:
			case PIC_CALIB_CARRIAGE_ENCODER:
			case PIC_CALIB_BLOWPIN_ENCODER:
			case PIC_CALIB_OTHER_ENCODER:	
			case PIC_CALIB_ROBOT_ENCODER:		
			case PIC_CALIB_CLAMP_LVDT:
			case PIC_CALIB_CARRIAGE_LVDT:
			case PIC_CALIB_BLOWPIN_LVDT:
				
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}

				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					gPanelObj.SetPicture = PIC_CALIB_CLAMP_ENCODER;
				}
				if( 1 == bHideClampActuatorElectic )gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CALIB_CARRIAGE_ENCODER;

				}
				if( 1 == bHideCarriageActuatorElectic )gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;	

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					gPanelObj.SetPicture = PIC_CALIB_BLOWPIN_ENCODER;
				}
				if( 1 == bHideBlowPinActuatorElectic)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;	
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					if( 0 == bHideClampActuatorHyd_LVDT)gPanelObj.SetPicture = PIC_CALIB_CLAMP_LVDT;
				}
				if( 1 == bHideClampActuatorHyd_LVDT)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;	
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if( 0 == bHideCarriageActuatorHyd_LVDT)gPanelObj.SetPicture = PIC_CALIB_CARRIAGE_LVDT;
				}
				if( 1 == bHideCarriageActuatorHyd_LVDT)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;	
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					if( 0 == bHideBlowPinActuatorHyd_LVDT)gPanelObj.SetPicture = PIC_CALIB_BLOWPIN_LVDT;
				}
				if( 1 == bHideBlowPinActuatorHyd_LVDT)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_INACTIVE;	
					
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_CALIB_OTHER_ENCODER;
				}
				if( 1 == bHideExtrLiftActuatorElectric)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;	
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					{
					gPanelObj.SetPicture = PIC_CALIB_ROBOT_ENCODER;
				}
				if( 1 == bHideRobotTransferActuatorElec)gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;	
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					
					
					/*----Reset Calibrate Function---------*/
					/* check commands */
					if (((bCmdResetClampR) || (bCmdResetClampL) || (bCmdResetBlowPinR) || (bCmdResetBlowPinL) || (bCmdResetCarriageR) || (bCmdResetCarriageL)|| (bCmdResetBlowPin2R) || (bCmdResetBlowPin2L) || (bCmdResetExtrLift) )&& (gPanelObj.PasswordLevel > 3))
				{
					uHideDiagResetCalib = 0;
				}

				/*Dialog Pop up contorl*/		

				if(0 == uHideDiagResetCalib)
				{
					if(bConfirmReset)
					{
						if(bCmdResetClampR)
						{
							gMachineFix.MoldR.Clamp.Zero = 0;
							gMachineFix.MoldR.Clamp.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetClampR = 0;
						}
						else if(bCmdResetClampL)
						{
							gMachineFix.MoldL.Clamp.Zero = 0;
							gMachineFix.MoldL.Clamp.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetClampL = 0;
						}
						else if(bCmdResetBlowPinR)
						{
							gMachineFix.MoldR.BlowPin.Zero = 0;
							gMachineFix.MoldR.BlowPin.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetBlowPinR = 0;
						}
						else if(bCmdResetBlowPinL)
						{
							gMachineFix.MoldL.BlowPin.Zero = 0;
							gMachineFix.MoldL.BlowPin.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetBlowPinL = 0;
						}
						else if(bCmdResetCarriageR)
						{
							gMachineFix.MoldR.Carriage.Zero = 0;
							gMachineFix.MoldR.Carriage.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetCarriageR = 0;
						}
						else if(bCmdResetCarriageL)
						{
							gMachineFix.MoldL.Carriage.Zero = 0;
							gMachineFix.MoldL.Carriage.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetCarriageL = 0;
						}
						else if(bCmdResetBlowPin2R)
						{
							gMachineFix.MoldR.BlowPin2.Zero = 0;
							gMachineFix.MoldR.BlowPin2.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetBlowPin2R = 0;
						}
						else if(bCmdResetBlowPin2L)
						{
							gMachineFix.MoldL.BlowPin2.Zero = 0;
							gMachineFix.MoldL.BlowPin2.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetBlowPin2L = 0;
						}
						else if(bCmdResetExtrLift)
						{
							gMachineFix.ExtrLift.Zero = 0;
							gMachineFix.ExtrLift.Max = 0;
							gRecipeOperate.OnceSave = 1;
							bCmdResetExtrLift = 0;
						}
						uHideDiagResetCalib = 1;
						bCancelReset = 	0;
						bConfirmReset = 0;
					}			
					else if (bCancelReset)
					{
						bCancelReset = 	0;
						bConfirmReset = 0;
						uHideDiagResetCalib = 1;
						bCmdResetClampR = 0;
						bCmdResetClampL = 0;
						bCmdResetBlowPinR = 0;
						bCmdResetBlowPinL = 0;
						bCmdResetBlowPin2R = 0;
						bCmdResetBlowPin2L = 0;
						bCmdResetCarriageR = 0;
						bCmdResetCarriageL = 0;
						bCmdResetExtrLift  =0;
					}		
				}
				else
				{
					bCancelReset = 	0;
					bConfirmReset = 0;
					bCmdResetClampR = 0;
					bCmdResetClampL = 0;
					bCmdResetBlowPinR = 0;
					bCmdResetBlowPinL = 0;
					bCmdResetCarriageR = 0;
					bCmdResetCarriageL = 0;
					bCmdResetExtrLift  =0;
				}	
					
				/*-------------------Calibration Confirm----------------------*/
				for(i=0;i<sizeof(CalibList)/2;i++)
				{
					CalibList[i] = 0;
				}

				CalibList[IndexCalib] = 1;
					
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_BUTTON;
					if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_F13] && 1 == gTKcalib.Status)
					{
					cntF13 = cntF13 + 1;
				}
				else
				{
					cntF13 = 0;
				}

				if(cntF13 > 20)
				{
					if(1 == gMachineInfo.Calib && PIC_CALIB_CLAMP_ENCODER == gPanelObj.GetPicture )	/* 调校模式  */
					{
						switch(IndexCalib)
						{
							case 0:
								gMachineFix.MoldL.Clamp.Zero = LMold.Clamp.As;
								gRecipeOperate.OnceSave= 1;

								break;
							case 1:
								gMachineFix.MoldR.Clamp.Zero = RMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							case 2:

								break;
							case 3:
								
								break;							
							case 4:
								gMachineFix.MoldL.Clamp.Max = LMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							
							case 5:
								gMachineFix.MoldR.Clamp.Max = RMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							
							case 6:

								break;
							case 7:
								
								break;	
								

						}/* switch(IndexCalib) */
					}/* if(1 == gMachineInfo.Calib)*/
					else if(1 == gMachineInfo.Calib && PIC_CALIB_CARRIAGE_ENCODER == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:	
								if(1 == LMold.TransDIn.CarriageFwLimit)
								{
									gMachineFix.MoldL.Carriage.Zero = LMold.Carriage.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.LCarriageFw = 1;
								}
								break;
							case 1:
								if(1 == RMold.TransDIn.CarriageFwLimit)
								{
									gMachineFix.MoldR.Carriage.Zero = RMold.Carriage.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.RCarriageFw = 1;  
								}
								break;
							
							case 2:

								break;
							case 3:
		
								break;							
							case 4:
								if(1 == LMold.TransDIn.CarriageBwLimit)
								{
									gMachineFix.MoldL.Carriage.Max = LMold.Carriage.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.LCarriageBw = 1;
								}
								break;
							
							case 5:
								if(1 == RMold.TransDIn.CarriageBwLimit)
								{
									gMachineFix.MoldR.Carriage.Max = RMold.Carriage.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.RCarriageBw = 1;
								}
								break;
							
							case 6:

								break;
							case 7:
								
								break;	
								

						}/* switch(IndexCalib) */
					}
					else if(1 == gMachineInfo.Calib && PIC_CALIB_BLOWPIN_ENCODER == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:
								if(1 == LMold.TransDIn.BlowPin2UpLimit)
								{
									gMachineFix.MoldL.BlowPin2.Zero = LMold.BlowPin2.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.LPin2Up = 1;
								}
								break;
							case 1:
								if(1 == LMold.TransDIn.BlowPinUpLimit)
								{
									gMachineFix.MoldL.BlowPin.Zero = LMold.BlowPin.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.LPinUp = 1;
								}
								break;
							
							case 2:
								if(1 == RMold.TransDIn.BlowPin2UpLimit)
								{
									gMachineFix.MoldR.BlowPin2.Zero = RMold.BlowPin2.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.RPin2Up = 1;
								}
								break;
							case 3:
								if(1 == RMold.TransDIn.BlowPinUpLimit)
								{
									gMachineFix.MoldR.BlowPin.Zero = RMold.BlowPin.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.RPinUp = 1;
								}
								break;							
							case 4:
								gMachineFix.MoldL.BlowPin2.Max = LMold.BlowPin2.As;
								gRecipeOperate.OnceSave= 1;

								break;
							case 5:
								gMachineFix.MoldL.BlowPin.Max = LMold.BlowPin.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							
							case 6:
								gMachineFix.MoldR.BlowPin2.Max = RMold.BlowPin2.As;
								gRecipeOperate.OnceSave = 1;

								break;
							case 7:
								gMachineFix.MoldR.BlowPin.Max = RMold.BlowPin.As;
								gRecipeOperate.OnceSave = 1;
								
								break;	
								

						}/* switch(IndexCalib) */
					}
					else if(1 == gMachineInfo.Calib && PIC_CALIB_OTHER_ENCODER == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:

								break;
							case 1:
								if(1 == RMold.TransDIn.ExtruderLiftDnLimit)
								{
									gMachineFix.ExtrLift.Zero = RMold.ExtrLift.As;
									gRecipeOperate.OnceSave = 1;
								}
								else
								{
									gImply.ExtrLiftDn = 1;
								}

								break;
							
							case 2:

								break;
							case 3:

								break;							
							case 4:

								break;
							case 5:
								//								if(1 == RMold.TransDIn.ExtruderLiftUpLimit)
								//								{
								gMachineFix.ExtrLift.Max = RMold.ExtrLift.As;
								gRecipeOperate.OnceSave = 1;
								//								}
								//								else
								//								{
								//									gImply.ExtrLiftUp = 1;
								//								}
								
								break;
							
							case 6:

								break;
							case 7:
								
								break;	
								

						}/* switch(IndexCalib) */
					}
					else if(1 == gMachineInfo.Calib && PIC_CALIB_CLAMP_LVDT == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:
								gMachineFix.MoldL.Clamp.Zero = LMold.Clamp.As;
								gRecipeOperate.OnceSave= 1;

								break;
							case 1:
								gMachineFix.MoldR.Clamp.Zero = RMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							case 2:

								break;
							
							case 3:
								
								break;							
							case 4:
								gMachineFix.MoldL.Clamp.Max = LMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;

								break;
							case 5:
								gMachineFix.MoldR.Clamp.Max = RMold.Clamp.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							case 6:

								break;
							case 7:

								break;	
								

						}/* switch(IndexCalib) */
					}
					else if(1 == gMachineInfo.Calib && PIC_CALIB_CARRIAGE_LVDT == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:
								gMachineFix.MoldL.Carriage.Zero = LMold.Carriage.As;
								gRecipeOperate.OnceSave= 1;

								break;
							case 1:
								gMachineFix.MoldR.Carriage.Zero = RMold.Carriage.As;
								gRecipeOperate.OnceSave = 1;
								
								break;		
							case 2:

								break;
							case 3:
								
								break;										
							case 4:
								gMachineFix.MoldL.Carriage.Max = LMold.Carriage.As;
								gRecipeOperate.OnceSave = 1;

								break;
							case 5:
								gMachineFix.MoldR.Carriage.Max = RMold.Carriage.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							case 6:

								break;
							case 7:
								
								break;	
								

						}/* switch(IndexCalib) */
					}
					else if(1 == gMachineInfo.Calib && PIC_CALIB_BLOWPIN_LVDT == gPanelObj.GetPicture)
					{
						switch(IndexCalib)
						{
							case 0:
								gMachineFix.MoldL.BlowPin.Zero = LMold.BlowPin.As;
								gRecipeOperate.OnceSave= 1;

								break;
							case 1:
								gMachineFix.MoldR.BlowPin.Zero = RMold.BlowPin.As;
								gRecipeOperate.OnceSave = 1;
								break;
							
							case 2:

								break;
							case 3:
								
								break;							
							case 4:
								gMachineFix.MoldL.BlowPin.Max = LMold.BlowPin.As;
								gRecipeOperate.OnceSave = 1;

								break;
							
							case 5:
								gMachineFix.MoldR.BlowPin.Max = RMold.BlowPin.As;
								gRecipeOperate.OnceSave = 1;
								
								break;
							
							case 6:

								break;
							case 7:
								
								break;	
								

						}/* switch(IndexCalib) */
					}
					else
					{
						gImply.CalMode = !gMachineInfo.Calib;
					}
					
				}/* if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEY_F13],KeyMatric[S$KEY_F13])) */
			
				// Robot Calib
				if(PIC_CALIB_ROBOT_ENCODER == gPanelObj.GetPicture)
				{
					if(1 == bRRobotHorZero)
					{
						cntRRobotHorZero++;
					}
					else
					{
						RMold.Robot.Transfer.Home = 0;
						cntRRobotHorZero =0;
					}
			
					if(1 == bRRobotVerZero)
					{
						RMold.Robot.UpDn.Home = 0;
						cntRRobotVerZero++;
					}
					else
					{
						cntRRobotVerZero =0;
					}
				
				
					if(1 == bLRobotHorZero)
					{
						cntLRobotHorZero++;
					}
					else
					{
						LMold.Robot.Transfer.Home = 0;
						cntLRobotHorZero =0;
					}
			
					if(1 == bLRobotVerZero)
					{
						cntLRobotVerZero++;
					}
					else
					{
						LMold.Robot.UpDn.Home =0;
						cntLRobotVerZero =0;
					}

					if(1 == gMachineInfo.Calib)	/* clibration mode  */
					{
						if(cntRRobotHorZero > 20)
						{
							if(1 == gMachineIn.RMold.RobotBwLimit)
							{
								gMachineFix.MoldR.RobotTransfer.sHome = RMold.Robot.Transfer.ecAs;
							}
							else
							{
								gImply.RRobotBw = 1;
							}
						}
			
						if(cntRRobotVerZero > 20)
						{
							if(1 == gMachineIn.RMold.RobotUpLimit)
							{
								gMachineFix.MoldR.RobotUpDn.sHome = RMold.Robot.UpDn.ecAs;
							}
							else
							{
								gImply.RRobotUp = 1;
							}
						}
						
						if(cntLRobotHorZero > 20)
						{
							if(1 == gMachineIn.LMold.RobotBwLimit)
							{
								gMachineFix.MoldL.RobotTransfer.sHome = LMold.Robot.Transfer.ecAs;
							}
							else
							{
								gImply.LRobotBw = 1;
							}
						}
			
						if(cntLRobotVerZero > 20)
						{
							if(1 == gMachineIn.LMold.RobotUpLimit)
							{        
								gMachineFix.MoldL.RobotUpDn.sHome = LMold.Robot.UpDn.ecAs;
							}
							else
							{
								gImply.LRobotUp = 1;
							}
						}
					}
				}
								

				


				gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;              
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				break;
			
			
			case PIC_SCREEN_CALIBRATE:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
            
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2]))
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_BUTTON;
				
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.vcHandle = VA_Setup(1, gPanelObj.VisualName);

					if (gPanelObj.vcHandle != 0)
					{
						if (!VA_Saccess(1, gPanelObj.vcHandle))
						{
							VA_StartTouchCal (1,gPanelObj.vcHandle);
							VA_Srelease(1, gPanelObj.vcHandle);
						}
					}
				}
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_INACTIVE;
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_INACTIVE;     
				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				if(ready == 0)
				{
					VC_HANDLE = VA_Setup(1 , gPanelObj.VisualName);

					if(VC_HANDLE != 0)ready = 1;
				}

				if(1 == ready)
				{
					if (!VA_Saccess(1,VC_HANDLE))
					{
						VA_GetTouchAction (1,VC_HANDLE,0,(UDINT)(&touch));
						VA_Ellipse (1,VC_HANDLE,touch.x,touch.y,2,2,0,0);
						VA_Srelease(1,VC_HANDLE);
					}
				}
				break;
			
			/*2018.8.3 Albert add new page for fast setting*/
			case PIC_FAST_SETTING:
				if(ChangePageDly > 0)
				{
					ChangePageDly--;
					break;
				}
				
				/*-----------F1---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F1] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F1],KeyMatric[S$KEYMATRIX_F1]))
					{
					gPanelObj.SetPicture = PIC_OVERVIEW;
				}
				/*-----------F2---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F2] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F2],KeyMatric[S$KEYMATRIX_F2])) 
					{
					gPanelObj.SetPicture = gPanelObj.OldPicture;
				}
				/*-----------F3---------*/
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F3],KeyMatric[S$KEYMATRIX_F3]))
					{
					if(0 == bHideLeftStation)gPanelObj.SetPicture = PIC_CLAMP_L;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F3] = STATE_ACTIVE;
						
					/*-----------F4---------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],KeyMatric[S$KEYMATRIX_F4]))
					{
					gPanelObj.SetPicture = PIC_CLAMP_R;
				}
				if(gPanelObj.GetPicture == PIC_CLAMP_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F4] = STATE_ACTIVE;
	            
					/*-------------F5---------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],KeyMatric[S$KEYMATRIX_F5]))
					{
					if(0 == bHideLeftStation)gPanelObj.SetPicture = PIC_TRANSPORT_L;
				}
				if(gPanelObj.GetPicture == PIC_TRANSPORT_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F5] = STATE_ACTIVE;
				
					/*------------F6-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],KeyMatric[S$KEYMATRIX_F6]))
					{
					gPanelObj.SetPicture = PIC_TRANSPORT_R;
				}
				if(gPanelObj.GetPicture == PIC_TRANSPORT_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F6] = STATE_ACTIVE;
						
						
					/*------------F7-----------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_SUB_PAGE;
					if(1 == bHideLeftStation)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_INACTIVE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))
					{
					if(0 == bHideLeftStation)gPanelObj.SetPicture = PIC_BLOW_PIN_L;
				}
				if(gPanelObj.GetPicture == PIC_BLOW_PIN_L)gPanelObj.idxKeyPicture[S$KEYMATRIX_F7] = STATE_ACTIVE;
				
					/*-----------F8------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
					gPanelObj.SetPicture = PIC_BLOW_PIN_R;
				}
				if(gPanelObj.GetPicture == PIC_BLOW_PIN_R)gPanelObj.idxKeyPicture[S$KEYMATRIX_F8] = STATE_ACTIVE;
				
					/*----------F9-------------*/
						
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_SUB_PAGE;		
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F9],KeyMatric[S$KEYMATRIX_F9]))
					{
					gPanelObj.SetPicture = PIC_DEFLASH;
				}
				if(gPanelObj.GetPicture == PIC_DEFLASH)gPanelObj.idxKeyPicture[S$KEYMATRIX_F9] = STATE_ACTIVE;		
						
					/*----------F10-------------*/
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_INACTIVE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F10],KeyMatric[S$KEYMATRIX_F10]))
					//						{
					//						gPanelObj.SetPicture = PIC_BOTTOM_DEFLASH;
					//					}
					//					if(gPanelObj.GetPicture == PIC_BOTTOM_DEFLASH)gPanelObj.idxKeyPicture[S$KEYMATRIX_F10] = STATE_ACTIVE;	

				
					gPanelObj.idxKeyPicture[S$KEYMATRIX_F11] = STATE_INACTIVE;
					//						if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],KeyMatric[S$KEYMATRIX_F11]))
					//						{
					//						gPanelObj.SetPicture = PIC_IO_DI_1;
					//					}

					gPanelObj.idxKeyPicture[S$KEYMATRIX_F12] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],KeyMatric[S$KEYMATRIX_F12]))
					{
					gPanelObj.SetPicture = PIC_ALARM_CURRENT;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F13] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],KeyMatric[S$KEYMATRIX_F13]))
					{
					gPanelObj.SetPicture = PIC_AUTO_STEP;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F14] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],KeyMatric[S$KEYMATRIX_F14]))
					{
					gPanelObj.SetPicture = PIC_AUTO_START;
				}
				
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F15] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F15],KeyMatric[S$KEYMATRIX_F15]))
					{
					gPanelObj.SetPicture = PIC_SPC_CTRL;
				}
	
				gPanelObj.idxKeyPicture[S$KEYMATRIX_F16] = STATE_SUB_PAGE;
					if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F16],KeyMatric[S$KEYMATRIX_F16]))
					{
					gPanelObj.SetPicture = PIC_PASSWORD_LOGIN;
				}
				
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_UP],KeyMatric[S$KEYMATRIX_UP]))
					{
					gPanelObj.SetLanguage = 1;
				}
				
				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_DOWN],KeyMatric[S$KEYMATRIX_DOWN]))
					{
					gPanelObj.SetLanguage = 0;
				}


				if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_ESC],KeyMatric[S$KEYMATRIX_ESC]))
					{
					gPwd.PwdLevel = 0;
					gPanelObj.SetPicture = PIC_INIT;
				}
				
				//					/*Albert Machine Option*/
				//					for(i=0;i<OptionDis.Count;i++)
				//					{
				//						if(1 == HotSpot[i])
				//						{
				//							HotSpot[i]  = 0;
				//							if(0 == OptionDis.pHideSource[i])
				//							{
				//								OptionDis.Current = i;
				//								OptionDis.pSource[OptionDis.Current] = !OptionDis.pSource[OptionDis.Current];
				//								gRecipeOperate.DataSave = 1;
				//							}
				//						} 
				//						OptionDis.Obj[i]= OptionDis.pSource[i];
				//					}
								
				break;	
			
			
			default:
				break;

		}/* switch(gPanelObj.GetPicture) */
	}

	/* 离开DO DI AI AO MODULE 页面，强制复位  */
	//	if(((gPanelObj.GetPicture > PIC_IO_MODULE) || (gPanelObj.GetPicture < PIC_IO_DI_1))&&
	//		((OldGetPicture >= PIC_IO_DI_1       ) || (OldGetPicture < PIC_IO_MODULE      )))
	if(((gPanelObj.GetPicture > PIC_IO_MODULE) || (gPanelObj.GetPicture < PIC_IO_DO_2))&&
		((OldGetPicture >= PIC_IO_DO_2       ) || (OldGetPicture < PIC_IO_MODULE      ))&& 	
		gPanelObj.GetPicture !=PIC_SCREEN_SAVE &&
		gPanelObj.GetPicture !=PIC_PRESSURE_CALIBRATION )
	{
		memset(&gHardWare.DOForce[0],0,sizeof(gHardWare.DOForce));
		memset(&gHardWare.AOForce[0],0,sizeof(gHardWare.AOForce));
		gHardWare.AOForceNum = 19;
		btnTesetPQ = 0; 
	}

	OldGetPicture = gPanelObj.GetPicture;

	KeyMatric[S$KEYMATRIX_F5] = gPanelObj.KeyMatric[S$KEYMATRIX_F5];
		KeyMatric[S$KEYMATRIX_F6] = gPanelObj.KeyMatric[S$KEYMATRIX_F6];
		KeyMatric[S$KEYMATRIX_F7] = gPanelObj.KeyMatric[S$KEYMATRIX_F7];
		KeyMatric[S$KEYMATRIX_F8] = gPanelObj.KeyMatric[S$KEYMATRIX_F8];

		/* 调校不合理，进入调教状态 */
		if((PIC_INIT_SET       == gPanelObj.GetPicture)||
		(PIC_GAP_CALIBRATE  == gPanelObj.GetPicture)||
		(PIC_ACCU_CALIBRATE == gPanelObj.GetPicture)
		)
	{
		gControlInfo.ControlStatus = S$STATUS_CALIBRATE;
		gAlarm.ParisonLinearCalErr = 0;
	}
	else
	{
		gControlInfo.ControlStatus = S$STATUS_AUTORUN;
		bHidePurge = 0;

		if(0 == gServoLinear.IsReady)
		{
			bHidePurge = 1;
			gControlInfo.ControlStatus = S$STATUS_NEED_CALIBRATE;

			if(1 == gThickProf.FunOption.Servo)
			{
				gAlarm.ParisonLinearCalErr = !gServoLinear.IsReady;
			}
			else
			{
				gAlarm.ParisonLinearCalErr = 0;
			}
		}
	}/* A|B|C */
	
	if((gServoLinear.Current < gServoLinear.ZeroPos - 1000  ) || (gServoLinear.Current > gServoLinear.MaxPos + 1000))
	{
		if(1 == gThickProf.FunOption.Servo && 1 == gServoLinear.IsReady)
		{
			gAlarm.ParisonValuebeyond = 1;
		}
		else
		{
			gAlarm.ParisonValuebeyond = 0;	
		}
	}
	else
	{
		gAlarm.ParisonValuebeyond = 0;
	}

	gMachineInfo.ReceipeNum = gRecipeOperate.Index;


	
	
}/*  end of cycle  */


void InitOptionLists(void)
{
	
	/*=================================================Parison=================================================*/
	/*ActuatorType*/
	memset(uOptActorParison, 2, sizeof(uOptActorParison));
	uOptActorParison[ACTUATOR_UNDEFINED]  = 0;
	uOptActorParison[ACTUATOR_HYD]  	= 0;
	uOptActorParison[ACTUATOR_ELECTRIC]  = 0;
	
	/*=================================================Clamp=================================================*/
	/*ActuatorType*/
	memset(uOptActorClamp, 2, sizeof(uOptActorClamp));
	uOptActorClamp[ACTUATOR_UNDEFINED]  = 0;
	uOptActorClamp[ACTUATOR_HYD]  	= 0;
	uOptActorClamp[ACTUATOR_ELECTRIC]  = 0;

	/*PressueOutputType*/
	memset(uOptPresOutClamp, 2, sizeof(uOptPresOutClamp));
	uOptPresOutClamp[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutClamp[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutClamp, 2, sizeof(uOptFluxOutClamp));
	uOptFluxOutClamp[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutClamp[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutClamp[FLUX_OUTPUT_PROP_VALVE_SINGLE]  = 0;
	uOptFluxOutClamp[FLUX_OUTPUT_NONE]  = 0;
	
	
	/*=================================================Carriage=================================================*/
	/*ActuatorType*/
	memset(uOptActorCarriage, 2, sizeof(uOptActorCarriage));
	uOptActorCarriage[ACTUATOR_UNDEFINED]  = 0;
	uOptActorCarriage[ACTUATOR_HYD]  	= 0;
	uOptActorCarriage[ACTUATOR_ELECTRIC]  = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutCarriage, 2, sizeof(uOptPresOutCarriage));
	uOptPresOutCarriage[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutCarriage[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutCarriage, 2, sizeof(uOptFluxOutCarriage));
	uOptFluxOutCarriage[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutCarriage[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutCarriage[FLUX_OUTPUT_PROP_VALVE_SINGLE]  	= 0;
	uOptFluxOutCarriage[FLUX_OUTPUT_NONE]  = 0;

	
	
	/*=================================================BlowPin=================================================*/
	/*ActuatorType*/
	memset(uOptActorBlowPin, 2, sizeof(uOptActorBlowPin));
	uOptActorBlowPin[ACTUATOR_UNDEFINED]  = 0;
	uOptActorBlowPin[ACTUATOR_HYD]  	 = 0;
	uOptActorBlowPin[ACTUATOR_ELECTRIC]  = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutBlowPin, 2, sizeof(uOptPresOutBlowPin));
	uOptPresOutBlowPin[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutBlowPin[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutBlowPin, 2, sizeof(uOptFluxOutBlowPin));
	uOptFluxOutBlowPin[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutBlowPin[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutBlowPin[FLUX_OUTPUT_PROP_VALVE_SINGLE]  	= 0;
	uOptFluxOutBlowPin[FLUX_OUTPUT_NONE]  = 0;
	
	
	/*=================================================Top Deflash=================================================*/
	/*ActuatorType*/
	memset(uOptActorTopDeflash, 2, sizeof(uOptActorTopDeflash));
	uOptActorTopDeflash[ACTUATOR_UNDEFINED]  = 0;
	uOptActorTopDeflash[ACTUATOR_HYD]  	= 0;
	uOptActorTopDeflash[ACTUATOR_PNEU]  = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutTopDeflash, 2, sizeof(uOptPresOutTopDeflash));
	uOptPresOutTopDeflash[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutTopDeflash[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutTopDeflash[PRESSURE_OUTPUT_ACC]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutTopDeflash, 2, sizeof(uOptFluxOutTopDeflash));
	uOptFluxOutTopDeflash[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutTopDeflash[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutTopDeflash[FLUX_OUTPUT_ACC]  	= 0;
	
	
	
	
	/*=================================================Bottom Deflash=================================================*/
	/*ActuatorType*/
	memset(uOptActorBottomDeflash, 2, sizeof(uOptActorBottomDeflash));
	uOptActorBottomDeflash[ACTUATOR_UNDEFINED]  = 0;
	uOptActorBottomDeflash[ACTUATOR_HYD]  	= 0;
	uOptActorBottomDeflash[ACTUATOR_PNEU]  = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutBottomDeflash, 2, sizeof(uOptPresOutBottomDeflash));
	uOptPresOutBottomDeflash[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutBottomDeflash[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutBottomDeflash[PRESSURE_OUTPUT_ACC]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutBottomDeflash, 2, sizeof(uOptFluxOutBottomDeflash));
	uOptFluxOutBottomDeflash[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutBottomDeflash[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutBottomDeflash[FLUX_OUTPUT_ACC]  	= 0;

	
	
	/*=================================================SubMold=================================================*/
	/*ActuatorType*/
	memset(uOptActorSubMold, 2, sizeof(uOptActorSubMold));
	uOptActorSubMold[ACTUATOR_UNDEFINED]  = 0;
	uOptActorSubMold[ACTUATOR_HYD]  	= 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutSubMold, 2, sizeof(uOptPresOutSubMold));
	uOptPresOutSubMold[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutSubMold[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutSubMold[PRESSURE_OUTPUT_ACC]  	= 0;
	uOptPresOutSubMold[PRESSURE_OUTPUT_NONE]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutSubMold, 2, sizeof(uOptFluxOutSubMold));
	uOptFluxOutSubMold[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutSubMold[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutSubMold[FLUX_OUTPUT_ACC]  	= 0;
	uOptFluxOutSubMold[FLUX_OUTPUT_NONE]  	= 0;
	
	/*=================================================Plate=================================================*/
	/*ActuatorType*/
	memset(uOptActorPlate, 2, sizeof(uOptActorPlate));
	uOptActorPlate[ACTUATOR_UNDEFINED]  = 0;
	uOptActorPlate[ACTUATOR_HYD]  	= 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutPlate, 2, sizeof(uOptPresOutPlate));
	uOptPresOutPlate[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutPlate[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutPlate[PRESSURE_OUTPUT_ACC]  	= 0;
	uOptPresOutPlate[PRESSURE_OUTPUT_NONE]  = 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutPlate, 2, sizeof(uOptFluxOutPlate));
	uOptFluxOutPlate[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutPlate[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutPlate[FLUX_OUTPUT_ACC]  	= 0;
	uOptFluxOutPlate[FLUX_OUTPUT_NONE]  = 0;
	
	
	/*=================================================ExtrLift=================================================*/
	/*ActuatorType*/
	memset(uOptActorExtrLift, 2, sizeof(uOptActorExtrLift));
	uOptActorExtrLift[ACTUATOR_UNDEFINED]  = 0;
	uOptActorExtrLift[ACTUATOR_HYD]  	   = 0;
	uOptActorExtrLift[ACTUATOR_ELECTRIC]   = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutExtrLift, 2, sizeof(uOptPresOutExtrLift));
	uOptPresOutExtrLift[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutExtrLift[PRESSURE_OUTPUT_ACC_EXTRLIFT]  	= 0;
	uOptPresOutExtrLift[PRESSURE_OUTPUT_NONE]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutExtrLift, 2, sizeof(uOptFluxOutExtrLift));
	uOptFluxOutExtrLift[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutExtrLift[FLUX_OUTPUT_ACC_EXTRLIFT]  	= 0;
	uOptFluxOutExtrLift[FLUX_OUTPUT_NONE]  	= 0;
	
	
	
	/*=================================================Robot Transfer=================================================*/
	/*ActuatorType*/
	memset(uOptActorRobotTransfer, 2, sizeof(uOptActorRobotTransfer));
	uOptActorRobotTransfer[ACTUATOR_UNDEFINED]  = 0;
	uOptActorRobotTransfer[ACTUATOR_HYD]  	   = 0;
	uOptActorRobotTransfer[ACTUATOR_ELECTRIC]   = 0;
	uOptActorRobotTransfer[ACTUATOR_PNEU]    = 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutRobotTransfer, 2, sizeof(uOptPresOutRobotTransfer));
	uOptPresOutRobotTransfer[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutRobotTransfer[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutRobotTransfer[PRESSURE_OUTPUT_NONE]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutRobotTranfer, 2, sizeof(uOptFluxOutRobotTranfer));
	uOptFluxOutRobotTranfer[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutRobotTranfer[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutRobotTranfer[FLUX_OUTPUT_NONE]  	= 0;
	
	
	/*=================================================Robot UpDn=================================================*/
	/*ActuatorType*/
	memset(uOptActorRobotUpDn, 2, sizeof(uOptActorRobotUpDn));
	uOptActorRobotUpDn[ACTUATOR_UNDEFINED]  = 0;
	uOptActorRobotUpDn[ACTUATOR_HYD]  	   = 0;
	uOptActorRobotUpDn[ACTUATOR_ELECTRIC]   = 0;
	uOptActorRobotUpDn[ACTUATOR_PNEU]    = 0;

	/*PressueOutputType*/
	memset(uOptPresOutRobotUpDn, 2, sizeof(uOptPresOutRobotUpDn));
	uOptPresOutRobotUpDn[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutRobotUpDn[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutRobotUpDn[PRESSURE_OUTPUT_NONE]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutRobotUpDn, 2, sizeof(uOptFluxOutRobotUpDn));
	uOptFluxOutRobotUpDn[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutRobotUpDn[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutRobotUpDn[FLUX_OUTPUT_NONE]  	= 0;
	
	/*=================================================Robot OpnCls=================================================*/
	/*ActuatorType*/
	memset(uOptActorRobotOpnCls, 2, sizeof(uOptActorRobotOpnCls));
	uOptActorRobotOpnCls[ACTUATOR_UNDEFINED]  = 0;
	uOptActorRobotOpnCls[ACTUATOR_HYD]  	   = 0;
	uOptActorRobotOpnCls[ACTUATOR_PNEU]   	= 0;
	
	/*PressueOutputType*/
	memset(uOptPresOutRobotOpnCls, 2, sizeof(uOptPresOutRobotOpnCls));
	uOptPresOutRobotOpnCls[PRESSURE_OUTPUT_UNDEFINED]  	= 0;
	uOptPresOutRobotOpnCls[PRESSURE_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptPresOutRobotOpnCls[PRESSURE_OUTPUT_NONE]  	= 0;
	
	/*FluxOutputType*/
	memset(uOptFluxOutRobotOpnCls, 2, sizeof(uOptFluxOutRobotOpnCls));
	uOptFluxOutRobotOpnCls[FLUX_OUTPUT_UNDEFINED]  	= 0;
	uOptFluxOutRobotOpnCls[FLUX_OUTPUT_SYSTEM_SINGLE]  = 0;
	uOptFluxOutRobotOpnCls[FLUX_OUTPUT_NONE]  	= 0;
}

void OptionListsUpdate(Mold_Fix_typ *pRMachineFix, Plate_Fix_typ *pPlateFix, ExtrLift_Fix_typ *pExtrLiftFix)
{
	UDINT old_Clamp_eActuatorType;
	/*=================================================Clamp=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->Clamp.eActuatorType)  // Hydraulic
	{
		pRMachineFix->Clamp.eMechanismType = MECHANISM_LINEAR;
	}
	else if (ACTUATOR_ELECTRIC == pRMachineFix->Clamp.eActuatorType) // Electric
	{
		pRMachineFix->Clamp.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->Clamp.eSensorType 		= SENSOR_UNDEFINED;
		pRMachineFix->Clamp.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->Clamp.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->Clamp.eActuatorType) // Undefined
	{
		pRMachineFix->Clamp.eMechanismType = MECHANISM_LINEAR;
		pRMachineFix->Clamp.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->Clamp.eSensorType 		= SENSOR_UNDEFINED;
		pRMachineFix->Clamp.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->Clamp.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*ValveType*/
	if(VALVE_PROP != pRMachineFix->Clamp.eValveType || ACTUATOR_HYD != pRMachineFix->Clamp.eActuatorType)  // Dir
	{
		gMachineFix.Option.bClampPropValveOutputInverse = 0;
	}
	
	/*=================================================Carriage=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->Carriage.eActuatorType)  // Hydraulic
	{
		pRMachineFix->Carriage.eMechanismType = MECHANISM_LINEAR;
	}
	else if (ACTUATOR_ELECTRIC == pRMachineFix->Carriage.eActuatorType) // Electric
	{
		pRMachineFix->Carriage.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->Carriage.eSensorType 			= SENSOR_UNDEFINED;
		pRMachineFix->Carriage.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->Carriage.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->Carriage.eActuatorType) // Undefined
	{
		pRMachineFix->Carriage.eMechanismType = MECHANISM_LINEAR;
		pRMachineFix->Carriage.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->Carriage.eSensorType 			= SENSOR_UNDEFINED;
		pRMachineFix->Carriage.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->Carriage.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================BlowPin=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->BlowPin.eActuatorType)  // Hydraulic
	{
		pRMachineFix->BlowPin.eMechanismType = MECHANISM_LINEAR;
	}
	else if (ACTUATOR_ELECTRIC == pRMachineFix->BlowPin.eActuatorType) // Electric
	{
		pRMachineFix->BlowPin.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->BlowPin.eSensorType 			= SENSOR_UNDEFINED;
		pRMachineFix->BlowPin.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->BlowPin.eFluxOutputType		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->BlowPin.eActuatorType) // Undefined
	{
		pRMachineFix->BlowPin.eMechanismType = MECHANISM_LINEAR;
		pRMachineFix->BlowPin.eValveType 			= VALVE_UNDEFINED;
		pRMachineFix->BlowPin.eSensorType 			= SENSOR_UNDEFINED;
		pRMachineFix->BlowPin.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->BlowPin.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*ValveType*/
	if(VALVE_PROP != pRMachineFix->BlowPin.eValveType || ACTUATOR_HYD != pRMachineFix->BlowPin.eActuatorType)  // Dir
	{
		gMachineFix.Option.bBlowpinPropValveOutputInverse = 0;
	}
	/*=================================================Top Deflash=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->TopDeflash.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_PNEU == pRMachineFix->TopDeflash.eActuatorType) // Pneumatic
	{
		pRMachineFix->TopDeflash.ePressueOutputType = PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->TopDeflash.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->TopDeflash.eActuatorType) // Undefined
	{
		pRMachineFix->TopDeflash.ePressueOutputType = PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->TopDeflash.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}

	/*=================================================Bottom Deflash=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->BottomDeflash.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_PNEU == pRMachineFix->BottomDeflash.eActuatorType) // Pneumatic
	{
		pRMachineFix->BottomDeflash.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->BottomDeflash.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->BottomDeflash.eActuatorType) // Undefined
	{
		pRMachineFix->BottomDeflash.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->BottomDeflash.eFluxOutputType 	= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================SubMold=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->SubMold.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if (ACTUATOR_UNDEFINED == pRMachineFix->SubMold.eActuatorType) // Undefined
	{
		pRMachineFix->SubMold.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->SubMold.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}

	/*=================================================Robot Transfer=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->RobotTransfer.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_PNEU == pRMachineFix->RobotTransfer.eActuatorType) // Pneumatic
	{
		pRMachineFix->RobotTransfer.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotTransfer.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_ELECTRIC == pRMachineFix->RobotTransfer.eActuatorType) // Electric
	{
		pRMachineFix->RobotTransfer.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotTransfer.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->RobotTransfer.eActuatorType) // Undefined
	{
		pRMachineFix->RobotTransfer.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotTransfer.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================Robot UpDn=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->RobotUpDn.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_PNEU == pRMachineFix->RobotUpDn.eActuatorType) // Pneumatic
	{
		pRMachineFix->RobotUpDn.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotUpDn.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->RobotUpDn.eActuatorType) // Electric
	{
		pRMachineFix->RobotUpDn.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotUpDn.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================Robot OpnCls=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pRMachineFix->RobotOpnCls.eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_PNEU == pRMachineFix->RobotOpnCls.eActuatorType) // Pneumatic
	{
		pRMachineFix->RobotOpnCls.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotOpnCls.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pRMachineFix->RobotOpnCls.eActuatorType) // Electric
	{
		pRMachineFix->RobotOpnCls.ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pRMachineFix->RobotOpnCls.eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================Plate=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pPlateFix->eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if (ACTUATOR_UNDEFINED == pPlateFix->eActuatorType) // Electric
	{
		pPlateFix->ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pPlateFix->eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	
	/*=================================================ExtrLift=================================================*/
	/*ActuatorType*/
	if(ACTUATOR_HYD == pExtrLiftFix->eActuatorType)  // Hydraulic
	{
		//None
	}	
	else if(ACTUATOR_ELECTRIC == pExtrLiftFix->eActuatorType) // Electric
	{
		pExtrLiftFix->ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pExtrLiftFix->eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	else if (ACTUATOR_UNDEFINED == pExtrLiftFix->eActuatorType) // Electric
	{
		pExtrLiftFix->ePressueOutputType 	= PRESSURE_OUTPUT_UNDEFINED;
		pExtrLiftFix->eFluxOutputType 		= FLUX_OUTPUT_UNDEFINED;
	}
	
}

