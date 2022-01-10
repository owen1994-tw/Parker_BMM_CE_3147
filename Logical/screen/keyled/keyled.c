/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: panel
 * File: panel.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program panel --- display led control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "defines.h"
#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/

_CYCLIC void cyclic ( void )
{
	/* ------------------ VK key and Real key ------------------ */
  
	for(index=0;index<32;index++)  /*  5AP880.1505 KEY L1 - L12 F1 - F16 R1 - R12 */
	{
		//ipis0218
		if ((gPanelObj.PasswordLevel >=1))
		{
			if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone)  && (0 == btnTesetPQ))
			{
				if( index <= 19 /*&& (1== gMachineInfo.Manual)*/)
				{
					if(  1 == gPanelObj.RKeyMatric[index] && 1 == gPanelObj.KeyMatric[index] )
					{
						index_Main = index;
						Lock_Flag = 1; 
					}	
					else if(  0 == gPanelObj.RKeyMatric[index_Main] && 1 == Lock_Flag )
					{
						index_Main = 0;
						Lock_Flag = 0;
					}	
					
					if( 0 == Lock_Flag )
					{
						gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
					}
				}
				else
				{
					gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
				}
			}
		}
		else
		{
			if (gPanelObj.PasswordLevel < 1)
			{
				if (EDGEPOS(gPanelObj.RKeyMatric[index],RKeyMatric[index]) || EDGEPOS(gPanelObj.VKeyMatric[index],RKeyMatric[index]) )
				{
					gImply.PsdLevel1=1;
				}	
			}			
		}

		//		if ((0==gMachineIn.OilPumpOk_DI || 0==gMachineInfo.OilPumpDelayDone))
		//		{
		//			if (EDGEPOS(gPanelObj.RKeyMatric[index],RKeyMatric[index]) || EDGEPOS(gPanelObj.VKeyMatric[index],RKeyMatric[index]) )
		//			{
		//				gImply.OilPumpNotOn=1;
		//			}
		//		}
		
		
		//		if(EDGEPOS(gPanelObj.KeyMatric[index],PKeyMatric[index]) )
		//		{
		//			if(1 == gMachineInfo.Manual || 1 == gMachineInfo.Calib )
		//			{
		//				if ( 1 == gMachineInfo.IsImply ) gMachineOut.ImplyBuzzer  = 1;
		//			}
		//			
		//		}
		//		
		//		if(EDGENEG(gPanelObj.KeyMatric[index],NKeyMatric[index]) )
		//		{
		//			gMachineOut.ImplyBuzzer  = 0;
		//		}

	}	
	
	

	//		if ((1==gMachineIn.OilPumpOk_DI && 1==gMachineInfo.OilPumpDelayDone)  && (0 == btnTesetPQ))
	//		{
	//			for(index=0;index<22;index++)  /*  5AP880.1505 KEY L1 - L12 F1 - F16 R1 - R12 */
	//			{
	//				gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
	//
	//			}	
	//		}


	for(index=32;index<400;index++)  /*  5AP880.1505 KEY L1 - L12 F1 - F16 R1 - R12 */
	{
		gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
	}
	
	//	if ((0==gMachineIn.OilPumpOk_DI || 0==gMachineInfo.OilPumpDelayDone))
	//	{
	//
	//		for(index=0;index<22;index++)  /*  5AP880.1505 KEY L1 - L12 F1 - F16 R1 - R12 */
	//		{
	//			if (EDGEPOS(gPanelObj.RKeyMatric[index],RKeyMatric[index]) )
	//			{
	//				gImply.OilPumpNotOn=1;
	//			}
	//		}		
	//	}
	

	
	//	for(index=0;index<400;index++)  /*  5AP880.1505 KEY L1 - L12 F1 - F16 R1 - R12 */
	//   {
	//      gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
	//   }
	
	//	for(index=100;index<200;index++)
	//	{
	//		gPanelObj.KeyMatric[index] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
	//	}
	//	
	//	for(index=276;index<296;index++)  /*  5AC800.EX2-01  number key and arrow key  */
	//	{
	//		gPanelObj.KeyMatric[index - OFFSET_EX2_01] = gPanelObj.RKeyMatric[index] | gPanelObj.VKeyMatric[index] | gPanelObj.MKeyMatric[index];
	//	}
	
	/* ------------------ ActInfo ------------------ */
	ActInfo.ClampCls    = LMold.ActInfo.ClampCls | RMold.ActInfo.ClampCls;    
	ActInfo.ClampOpn    = LMold.ActInfo.ClampOpn | RMold.ActInfo.ClampOpn;

	ActInfo.CarriageFw  = LMold.ActInfo.CarriageFw | RMold.ActInfo.CarriageFw;
	ActInfo.CarriageBw  = LMold.ActInfo.CarriageBw | RMold.ActInfo.CarriageBw;

	ActInfo.BlowPinDn   = LMold.ActInfo.BlowPinDn  | RMold.ActInfo.BlowPinDn;
	ActInfo.BlowPinUp   = LMold.ActInfo.BlowPinUp  | RMold.ActInfo.BlowPinUp;

	ActInfo.LowBlow 	= gMachineOut.LMold.LowBlow  | gMachineOut.RMold.LowBlow;
	ActInfo.HighBlow 	= gMachineOut.LMold.HighBlow  | gMachineOut.RMold.HighBlow;
   
	ActInfo.ExtrLiftUp 	= LMold.ActInfo.ExtrLiftUp | RMold.ActInfo.ExtrLiftUp;
	ActInfo.ExtrLiftDn 	= LMold.ActInfo.ExtrLiftDn | RMold.ActInfo.ExtrLiftDn;

	ActInfo.Cutter      = LMold.ActInfo.Cutter | RMold.ActInfo.Cutter;
	ActInfo.InnerPin 	= LMold.ActInfo.InnerPin   | RMold.ActInfo.InnerPin;
	ActInfo.PullBottom 	= LMold.ActInfo.PullBottom | RMold.ActInfo.PullBottom;
	ActInfo.CoolPinDn   = LMold.ValveOut.CoolPinDn | RMold.ValveOut.CoolPinDn;
	ActInfo.CoolPinBlow	= LMold.ValveOut.CoolPinBlow | RMold.ValveOut.CoolPinBlow;
	ActInfo.CoolPin2Dn  = LMold.ValveOut.CoolPin2Dn | RMold.ValveOut.CoolPin2Dn;
	ActInfo.CoolPin2Blow= LMold.ValveOut.CoolPin2Blow | RMold.ValveOut.CoolPin2Blow;
	ActInfo.CoolPin3Dn  = LMold.ValveOut.CoolPin3Dn | RMold.ValveOut.CoolPin3Dn;
	ActInfo.CoolPin3Blow= LMold.ValveOut.CoolPin3Blow | RMold.ValveOut.CoolPin3Blow;
	ActInfo.BlowDeflash = LMold.ActInfo.BlowDeflash  | RMold.ActInfo.BlowDeflash;
	ActInfo.CoolDeflash = LMold.ActInfo.CoolDeflash  | RMold.ActInfo.CoolDeflash;
	ActInfo.SubMoldOpn	= LMold.ActInfo.SubMoldOpn | RMold.ActInfo.SubMoldOpn;
	ActInfo.SubMoldCls	= LMold.ActInfo.SubMoldCls | RMold.ActInfo.SubMoldCls;
	ActInfo.PinPartFw	= LMold.ActInfo.PinPartFw | RMold.ActInfo.PinPartFw;
	ActInfo.PinPartBw	= LMold.ActInfo.PinPartBw | RMold.ActInfo.PinPartBw;
	ActInfo.ScrewFw		= LMold.ActInfo.ScrewFw   | RMold.ActInfo.ScrewFw;
	ActInfo.ScrewBw		= LMold.ActInfo.ScrewBw   | RMold.ActInfo.ScrewBw;
	ActInfo.RobotFw		= LMold.ValveOut.RobotFw  | RMold.ValveOut.RobotFw | ( ACTUATOR_PNEU != gMachineFix.MoldR.RobotTransfer.eActuatorType && (LMold.ActInfo.RobotFw| RMold.ActInfo.RobotFw));
	ActInfo.RobotBw		= LMold.ValveOut.RobotBw  | RMold.ValveOut.RobotBw | ( ACTUATOR_PNEU != gMachineFix.MoldR.RobotTransfer.eActuatorType && (LMold.ActInfo.RobotBw| RMold.ActInfo.RobotBw));
	ActInfo.RobotCls	= LMold.ValveOut.RobotCls | RMold.ValveOut.RobotCls;
	ActInfo.RobotUp		= LMold.ValveOut.RobotUp  | RMold.ValveOut.RobotUp;
	ActInfo.RobotDn		= LMold.ValveOut.RobotDn  | RMold.ValveOut.RobotDn;
	ActInfo.CutNeck		= LMold.ValveOut.CutNeck    | RMold.ValveOut.CutNeck;
	ActInfo.CutNeckCls	= LMold.ValveOut.CutNeckCls | RMold.ValveOut.CutNeckCls;
	ActInfo.SupportAir	= gMachineOut.SupportAir_1 || gMachineOut.SupportAir_TimeVersion ;
	ActInfo.TopDeflashFw = LMold.ValveOut.TopDeflashFw  | RMold.ValveOut.TopDeflashFw;
	ActInfo.TopDeflashBw = LMold.ValveOut.TopDeflashBw  | RMold.ValveOut.TopDeflashBw;
	ActInfo.BottomDeflashFw = LMold.ValveOut.BottomDeflashFw  | RMold.ValveOut.BottomDeflashFw;
	ActInfo.BottomDeflashBw = LMold.ValveOut.BottomDeflashBw  | RMold.ValveOut.BottomDeflashBw;
	ActInfo.PullBottom 	= LMold.ValveOut.PullBottom  | RMold.ValveOut.PullBottom;
	ActInfo.HookPinFw  = LMold.ValveOut.HookPinFw | RMold.ValveOut.HookPinFw;
	
	/*  5AP880.1505 KEY L1 - L12 */
	gPanelObj.LedMatric[S$KEYMATRIX_MAIN_PUMP] = gMachineOut.OilPump;
		gPanelObj.LedMatric[S$KEYMATRIX_PARISON_PUMP] = gMachineOut.ParisonPump;
	
		gPanelObj.LedMatric[S$KEYMATRIX_HEAT_POWER] = gMachineOut.HeatPower;
		
		//	gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_A] = gMachineIn.ExtrA_OnOff;
		//	gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_B] = gMachineIn.ExtrB_OnOff;
		
		//	gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_A_PLK] = gExtruderACtrl.InverterStatus.bSwitchOn;/*Albert*/
		//	gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_B_PLK] = gExtruderBCtrl.InverterStatus.bSwitchOn;/*Albert*/	
		//	gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_C_PLK] = gExtruderCCtrl.InverterStatus.bSwitchOn;/*Albert*/
		//	gPanelObj.LedMatric[S$KEYMATRIX_VISU_EXTRUDER_PLK] = gExtruderVCtrl.InverterStatus.bSwitchOn;/*Albert*/
		
		//		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_A_PLK] = gMachineOut.ExtrA_Enable;/*ipis*/
		//		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_B_PLK] = gMachineOut.ExtrB_Enable;/*ipis*/	
		//		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_C_PLK] = gMachineOut.ExtrC_Enable;/*ipis*/
		//		gPanelObj.LedMatric[S$KEYMATRIX_VISU_EXTRUDER_PLK] = gMachineOut.ExtrV_Enable;/*ipis*/
	
		/* for ABB inveter ------------------------------------------------*/
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_A_ABB] = gMachineOut.ExtrA_On;	
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_B_ABB] = gMachineOut.ExtrB_On;	
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_C_ABB] = gMachineOut.ExtrC_On;	
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_D_ABB] = gMachineOut.ExtrD_On;
		if(1 == gMachineFix.ExtruderD.LinkWithMode && 1 == gMachineFix.ExtruderVisu.LinkWithMode)
	{
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_D_ABB] = gMachineOut.ExtrD_On || gMachineOut.ExtrVisu_On;	
			}
	else
	{
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_D_ABB] = gMachineOut.ExtrD_On;	
			}
	/*---------------------------------------------------------------------------*/	
		
	gPanelObj.LedMatric[S$KEYMATRIX_MODE_CALIB] = gMachineInfo.Calib;
		gPanelObj.LedMatric[S$KEYMATRIX_MODE_MANUAL]= gMachineInfo.Manual;
		gPanelObj.LedMatric[S$KEYMATRIX_MODE_AUTO]  = gMachineInfo.Auto;
		gPanelObj.LedMatric[S$KEYMATRIX_AUTO_RESET] = gMachineInfo.MachineReset;
		
		gPanelObj.LedMatric[S$KEYMATRIX_AUTO_STOP] = (2 == gMachineInfo.LedAutoStop) ;/*Albert*/
		
		gPanelObj.LedMatric[S$KEYMATRIX_CUTTER_FW]   = ActInfo.Cutter;	
		if( ACTUATOR_ELECTRIC  == gMachineFix.ExtrLift.eActuatorType )
	{
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_UP] = ActInfo.ExtrLiftUp;
			gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_DN] = ActInfo.ExtrLiftDn;
			}
	else
	{
		gPanelObj.LedMatric[S$KEYMATRIX_EXTRUDER_UP] = ActInfo.ExtrLiftUp||ActInfo.ExtrLiftDn;
			}
	
		
	if (1==gMacOption.Station) //Right ipis0128 
	{
		gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_UP] = ActInfo.CarriageFw;
			gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_DN] = ActInfo.CarriageBw;
			}
	if (0==gMacOption.Station) //Left ipis0128
	{
		gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_UP] = ActInfo.CarriageBw;
			gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_DN] = ActInfo.CarriageFw;
			}
	if (2==gMacOption.Station) //Right ipis0129
	{
		gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_UP] = RMold.ActInfo.CarriageFw ||LMold.ActInfo.CarriageBw;
			gPanelObj.LedMatric[S$KEYMATRIX_CARRIAGE_DN] = RMold.ActInfo.CarriageFw ||LMold.ActInfo.CarriageFw ;	
			}

	gPanelObj.LedMatric[S$KEYMATRIX_CLAMP_OPN ]  = ActInfo.ClampOpn;
		gPanelObj.LedMatric[S$KEYMATRIX_CLAMP_CLS]   = ActInfo.ClampCls;
		gPanelObj.LedMatric[S$KEYMATRIX_BLOWPIN_UP]  = ActInfo.BlowPinUp;
		gPanelObj.LedMatric[S$KEYMATRIX_BLOWPIN_DN]  = ActInfo.BlowPinDn;
		
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_UP] = gMachineOut.PlateUp;
		gPanelObj.LedMatric[S$KEYMATRIX_PLATE_DN] = gMachineOut.PlateDn;
		gPanelObj.LedMatric[S$KEYMATRIX_TOP_DEFLASH_FW] = ActInfo.TopDeflashFw;
		//	gPanelObj.LedMatric[S$KEYMATRIX_BOT_DEFLASH_FW] = ActInfo.BottomDeflash;
		//	gPanelObj.LedMatric[S$KEYMATRIX_BOT_DEFLASH_FW] = ActInfo.BottomDeflashFw;
		gPanelObj.LedMatric[S$KEYMATRIX_TOP_DEFLASH_BW] = ActInfo.TopDeflashBw;
		//	gPanelObj.LedMatric[S$KEYMATRIX_BOT_DEFLASH_BW] = ActInfo.BottomDeflashBw;
		gPanelObj.LedMatric[S$KEYMATRIX_PULL_BOTTOM_FW] = ActInfo.PullBottom;
			
		gPanelObj.LedMatric[S$KEYMATRIX_ROBOT_FW]  = ActInfo.RobotFw;
		gPanelObj.LedMatric[S$KEYMATRIX_ROBOT_BW]  = ActInfo.RobotBw;
		//	gPanelObj.LedMatric[S$KEYMATRIX_ROBOT_FWBW] =ActInfo.RobotFw ||ActInfo.RobotBw ;
		gPanelObj.LedMatric[S$KEYMATRIX_ROBOT_CLS] = ActInfo.RobotCls;
		gPanelObj.LedMatric[S$KEYMATRIX_BLOW]      = ActInfo.HighBlow;
		gPanelObj.LedMatric[S$KEYMATRIX_SUPPORT]   = ActInfo.SupportAir;
		gPanelObj.LedMatric[S$KEYMATRIX_COOL_DEFLASH]   = ActInfo.CoolDeflash;
		gPanelObj.LedMatric[S$KEYMATRIX_CUT_NECK]   = ActInfo.CutNeck;
	
		gPanelObj.LedMatric[S$KEYMATRIX_LUBRICATION] = gMachineOut.Lubricate;	
		gPanelObj.LedMatric[S$KEYMATRIX_DRY_CYCLE] = gMachineInfo.bDryCycle = gMacOption.bDryCycle;
		//	gPanelObj.LedMatric[S$KEYMATRIX_PRODUCTAUX_CLS] = LMold.ValveOut.ProductAuxClamp  | RMold.ValveOut.ProductAuxClamp;
	
		gPanelObj.LedMatric[S$KEYMATRIX_COOL_DN] = ActInfo.CoolPinDn;
		gPanelObj.LedMatric[S$KEYMATRIX_COOL_BLOW] = ActInfo.CoolPinBlow;
		
		gPanelObj.LedMatric[S$KEYMATRIX_COOL2_DN] = ActInfo.CoolPin2Dn;
		gPanelObj.LedMatric[S$KEYMATRIX_COOL2_BLOW] = ActInfo.CoolPin2Blow;

		gPanelObj.LedMatric[S$KEYMATRIX_COOL3_DN] = ActInfo.CoolPin3Dn;
		gPanelObj.LedMatric[S$KEYMATRIX_COOL3_BLOW] = ActInfo.CoolPin3Blow;

		
		
		/*Albert Led status of PIC_FAST_SETTING*/

	
		/*IO DI*/
		//	if(gPanelObj.GetPicture == PIC_IO_DI_1)
		//	{
		//		gPanelObj.LedMatric[S$PAGE_MONITOR] = 1;
		//	}
		//	else
		//	{
		//		gPanelObj.LedMatric[S$PAGE_MONITOR] = 0;
		//	}
		//	
		//	/*Temperature*/
		//	if(gPanelObj.GetPicture == PIC_TEMP_ALL)
		//	{
		//		gPanelObj.LedMatric[S$PAGE_TEMP_ALL] = 1;
		//	}
		//	else
		//	{
		//		gPanelObj.LedMatric[S$PAGE_TEMP_ALL] = 0;
		//	}
		//	
		//	/*Extruder Speed */
		//	if(gPanelObj.GetPicture == PIC_SPEED)
		//	{
		//		gPanelObj.LedMatric[S$PAGE_EXTRUDER] = 1;
		//	}
		//	else
		//	{
		//		gPanelObj.LedMatric[S$PAGE_EXTRUDER] = 0;
		//	}
	
		/*------------------------------------------------------------*/
	
	
		gHardWare.XP0101Led_1[1]  = gMachineOut.LMold.BlowPinDnHold  | gMachineOut.RMold.BlowPinDnHold;;
	if ((1==LMold.Option.SubMold&& 0==LMold.Option.BottomSubMold) || (1==RMold.Option.SubMold&& 0==RMold.Option.BottomSubMold))    //ipis0128 topsubmold
	{
		gHardWare.XP0101Led_1[5]  = ActInfo.SubMoldCls;		
		gHardWare.XP0101Led_1[9]  = ActInfo.SubMoldOpn;
	}
	if ((1==LMold.Option.BottomSubMold&&0==LMold.Option.SubMold) || (1==RMold.Option.BottomSubMold&&0==RMold.Option.SubMold)) //ipis0128 BottomSubMold
	{
		gHardWare.XP0101Led_1[5]  = ActInfo.SubMoldOpn;		
		gHardWare.XP0101Led_1[9]  = ActInfo.SubMoldCls;
	}
	
	gHardWare.XP0101Led_1[13] = gMachineOut.PlateUp;;
	gHardWare.XP0101Led_1[17] = gMachineOut.PlateDn;
	
	if (6600==RMold.Plate.Step || 6600==LMold.Plate.Step) //ipis0802
	{
		gHardWare.XP0101Led_1[21] = 0; 
	}
	else
	{
		gHardWare.XP0101Led_1[21] = gMachineOut.PlateFw; 
		
	}
	
	
	gHardWare.XP0101Led_1[25] = gMachineOut.PlateBw;
	
	if(gPanelObj.GetPicture == PIC_FAST_SETTING)
	{
		gHardWare.XP0101Led_1[29] = 1;
	}
	else
	{
		gHardWare.XP0101Led_1[29] = 0;
	}
	

	
	

} /* end of _CYCLIC */





