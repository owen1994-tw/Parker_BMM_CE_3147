/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: extruder
 * File: extruder.c
 * Author: lij
 * Created: January 05, 2015
 ********************************************************************
 * Implementation of program extruder
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/**************************************************************************************
** 					               include user files								 **
**************************************************************************************/
#include "edge.h"               /*edge function                         */
#include "keyap2100.h"
#include "pic_id.h"

/**************************************************************************************
** 					 	                 define								                   **
**************************************************************************************/
#define     S$EXTRUDER_WAIT		           	 0
#define     S$EXTRUDER_CONTACTOR_ON		   	10
#define     S$EXTRUDER_POWER_ON_DELAY			20
#define     S$EXTRUDER_POWER_ON		        	30
#define     S$EXTRUDER_READY		       		100
#define     S$EXTRUDER_BASIC_ZONE		  		110
#define     S$EXTRUDER_PRODUCTION_ZONE	   	130
#define     S$EXTRUDER_DEC_ZONE		       	150
#define     S$EXTRUDER_POWER_OFF_DECEL			200
#define     S$EXTRUDER_POWER_OFF_PROTECT		210
#define     S$EXTRUDER_POWER_OFF_DELAY			220
/**************************************************************************************
                               Function Declarations
**************************************************************************************/


/**************************************************************************************
     	        	       I N I T U P  O F   T H E   T A S K
**************************************************************************************/
void _INIT extruderINIT(void)
{
	/* TODO: Add code here */
	RTInfo_Task.enable = 1;
	RTInfo(&RTInfo_Task);

	tTask = RTInfo_Task.cycle_time/1000.0/1000.0;		/*  from us ---> s  */
    
	MotorSpeedA.minVal  	  = 0;
	MotorSpeedA.frequence	  = 30;
	MotorSpeedA.waitTimeSlow = 200;
	MotorSpeedA.waitTimeFast = 500;
	MotorSpeedA.unitsSlow    = 1;
	MotorSpeedA.unitsFast	= 5;
	MotorSpeedA.unitsVFast	= 20;

	MotorSpeedA.maxVal = 100;
	MotorSpeedA.minVal = 0;

	MotorSpeedB.minVal  	 = 0;
	MotorSpeedB.frequence	 = 30;
	MotorSpeedB.waitTimeSlow = 200;
	MotorSpeedB.waitTimeFast = 500;
	MotorSpeedB.unitsSlow    = 1;
	MotorSpeedB.unitsFast	 = 5;
	MotorSpeedB.unitsVFast	 = 20;

	MotorSpeedB.maxVal = 100;
	MotorSpeedB.minVal = 0;

	MotorSpeedVisu.frequence	= 30;
	MotorSpeedVisu.waitTimeSlow = 200;
	MotorSpeedVisu.waitTimeFast = 500;
	MotorSpeedVisu.unitsSlow    = 1;
	MotorSpeedVisu.unitsFast	= 5;
	MotorSpeedVisu.unitsVFast	= 20;

	MotorSpeedVisu.maxVal = 100;
	MotorSpeedVisu.minVal = 0;
    
	cntExtruderStopDelay = 0;


    
	gExtruderA.tTask  = tTask;
	gExtruderB.tTask  = tTask;
	gExtruderVisu.tTask = tTask; 
	
	bHideCurrency = 1;
	bHideMeltTemp = 1;
	
	gExtruderA.Status.OnProtected = 1;
	gExtruderB.Status.OnProtected = 1;
	gExtruderC.Status.OnProtected = 1;
	gExtruderD.Status.OnProtected = 1;
	gExtruderVisu.Status.OnProtected = 1;
	
}
/**************************************************************************************
        				   C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _CYCLIC extruderCYCLIC(void)
{     
	if(0 == gMachineFix.ExtruderA.RPM10v )gMachineFix.ExtruderA.RPM10v = 1500;
	if(0 == gMachineFix.ExtruderB.RPM10v )gMachineFix.ExtruderB.RPM10v  = 1500;
	if(0 == gMachineFix.ExtruderC.RPM10v )gMachineFix.ExtruderC.RPM10v = 1500;
	if(0 == gMachineFix.ExtruderD.RPM10v )gMachineFix.ExtruderD.RPM10v = 1500;
	if(0 == gMachineFix.ExtruderVisu.RPM10v )gMachineFix.ExtruderVisu.RPM10v = 1500;
   
	if(0 == gMachineFix.ExtruderA.Currency10V )gMachineFix.ExtruderA.Currency10V = 50;
	if(0 == gMachineFix.ExtruderB.Currency10V )gMachineFix.ExtruderB.Currency10V= 50;
	if(0 == gMachineFix.ExtruderC.Currency10V )gMachineFix.ExtruderC.Currency10V= 50;
	if(0 == gMachineFix.ExtruderD.Currency10V )gMachineFix.ExtruderD.Currency10V= 50;
	if(0 == gMachineFix.ExtruderVisu.Currency10V )gMachineFix.ExtruderVisu.Currency10V = 50;
	
	if((1 == gMachineFix.ExtruderD.Use) && 1 == gMachineFix.ExtruderVisu.Use)
	{
	
	}
	
	if(1 == gMachineFix.ExtruderD.Use && 1 == gMachineFix.ExtruderVisu.Use)
	{
		gMachineFix.ExtruderD.LinkWithMode = 1;
	}
	else
	{
		gMachineFix.ExtruderD.LinkWithMode = 0;
	}
	gMachineFix.ExtruderVisu.LinkWithMode = gMachineFix.ExtruderD.LinkWithMode;
	
	/*------------- CtrlType--------------- */
	gMachineFix.ExtruderB.eCtrlType = gMachineFix.ExtruderA.eCtrlType;
	gMachineFix.ExtruderC.eCtrlType = gMachineFix.ExtruderA.eCtrlType;
	gMachineFix.ExtruderD.eCtrlType = gMachineFix.ExtruderA.eCtrlType;
	gMachineFix.ExtruderVisu.eCtrlType = gMachineFix.ExtruderA.eCtrlType;
	
	
	if(1 == gEnableAll_HMI)
	{	
		gEnableAll_HMI = 0;
		if(1 == gMachineFix.ExtruderA.Use) 		gExtruderHMI_Enable[0]=1;
		if(1 == gMachineFix.ExtruderB.Use) 		gExtruderHMI_Enable[1]=1;
		if(1 == gMachineFix.ExtruderC.Use) 		gExtruderHMI_Enable[2]=1;
		if(1 == gMachineFix.ExtruderD.Use) 		gExtruderHMI_Enable[3]=1;
		if(1 == gMachineFix.ExtruderVisu.Use) 	gExtruderHMI_Enable[4]=1;
	}
	else if( 1 == gDisableAll_HMI || 1 == gMachineIn.ExtruderNormal ||
		1 == gMachineIn.ExtrA_MotorErr || 1 == gMachineIn.ExtrB_MotorErr || 1 == gMachineIn.ExtrC_MotorErr || 
		1 == gMachineIn.ExtrD_MotorErr || 1 == gMachineIn.ExtrV_MotorErr)
	{
		gDisableAll_HMI = 0;
		if ( 1 == gMachineIn.ExtruderNormal)	gAlarm.ExtruderNormal = 1;  
		gExtruderHMI_Enable[0] = 0;
		gExtruderHMI_Enable[1] = 0;	
		gExtruderHMI_Enable[2] = 0;
		gExtruderHMI_Enable[3] = 0;	
		gExtruderHMI_Enable[4] = 0;
	}

   
	/*  -------------------- Extruder motor speed and current -------------------------- */
	if(CTRL_VF == gMachineFix.ExtruderA.eCtrlType )// Analog VF Control
	{
		/*-------- Extruder A Use-------*/
		if(1 == gMachineFix.ExtruderA.Use)
		{
			// SetSpd condition
			if (1 == gExtruderHMI_Enable[0] && 1 == gMachineOut.ExtrA_On)
			{
				gExtruderA.aoSetRPM = (gMachinePara.ExtruderA.SetSpeed / gMachineFix.ExtruderA.RPM10v)*32767.0;
			}
			else
			{
				gExtruderA.aoSetRPM = 0;
			}
			
			// motor speed
			gExtruderA.ActRPM  = gExtruderA.aiActRPM/32767.0  * gMachineFix.ExtruderA.RPM10v;
			gExtruderA.Current = gExtruderA.aiCurrent/32767.0 * gMachineFix.ExtruderA.Currency10V;
		
			//Screw speed
			if (0 == gMachineFix.ExtruderA.Ratio)
			{
				gMachineFix.ExtruderA.Ratio =1;
			}
			else
			{
				gExtruderA.ActSpeed = gExtruderA.ActRPM /gMachineFix.ExtruderA.Ratio;
			}
		}
		else
		{
			gExtruderA.ActRPM = 0.0;
			gExtruderA.Current = 0.0;
		}
		
		/*-------- Extruder B Use-------*/
		if(1 == gMachineFix.ExtruderB.Use)
		{
			// SetSpd condition
			if (1 == gExtruderHMI_Enable[1] && 1 == gMachineOut.ExtrB_On)
			{
				gExtruderB.aoSetRPM = (gMachinePara.ExtruderB.SetSpeed / gMachineFix.ExtruderB.RPM10v)*32767.0;
			}
			else
			{
				gExtruderB.aoSetRPM = 0;
			}
			
			// motor speed
			gExtruderB.ActRPM = gExtruderB.aiActRPM/32767.0  * gMachineFix.ExtruderB.RPM10v;
			gExtruderB.Current = gExtruderB.aiCurrent/32767.0 * gMachineFix.ExtruderB.Currency10V;
		
			//Screw speed
			if (0 == gMachineFix.ExtruderB.Ratio)
			{
				gMachineFix.ExtruderB.Ratio =1;
			}
			else
			{
				gExtruderB.ActSpeed = gExtruderB.ActRPM /gMachineFix.ExtruderB.Ratio;
			}
		}
		else
		{
			gExtruderB.ActRPM = 0.0;
			gExtruderB.Current = 0.0;
		}
		
		/*-------- Extruder C Use-------*/
		if(1 == gMachineFix.ExtruderC.Use)
		{
			// SetSpd condition
			if (1 == gExtruderHMI_Enable[2] && 1 == gMachineOut.ExtrC_On)
			{
				gExtruderC.aoSetRPM = (gMachinePara.ExtruderC.SetSpeed / gMachineFix.ExtruderC.RPM10v)*32767.0;
			}
			else
			{
				gExtruderC.aoSetRPM = 0;
			}
			
			// motor speed
			gExtruderC.ActRPM = gExtruderC.aiActRPM/32767.0  * gMachineFix.ExtruderC.RPM10v;
			gExtruderC.Current = gExtruderC.aiCurrent/32767.0 * gMachineFix.ExtruderC.Currency10V;
		
			//Screw speed
			if (0 == gMachineFix.ExtruderC.Ratio)
			{
				gMachineFix.ExtruderC.Ratio =1;
			}
			else
			{
				gExtruderC.ActSpeed = gExtruderC.ActRPM /gMachineFix.ExtruderC.Ratio;
			}
		}
		else
		{
			gExtruderC.ActRPM = 0.0;
			gExtruderC.Current = 0.0;
		}
	
		/*-------- Extruder D Use-------*/
		if(1 == gMachineFix.ExtruderD.Use)
		{
			// SetSpd condition
			if (1 == gExtruderHMI_Enable[3] && 1 == gMachineOut.ExtrD_On)
			{
				gExtruderD.aoSetRPM = (gMachinePara.ExtruderD.SetSpeed / gMachineFix.ExtruderD.RPM10v)*32767.0;
			}
			else
			{
				gExtruderD.aoSetRPM = 0;
			}
			
			// motor speed
			gExtruderD.ActRPM = gExtruderD.aiActRPM/32767.0  * gMachineFix.ExtruderD.RPM10v;
			gExtruderD.Current = gExtruderD.aiCurrent/32767.0 * gMachineFix.ExtruderD.Currency10V;
		
			//Screw speed
			if (0 == gMachineFix.ExtruderD.Ratio)
			{
				gMachineFix.ExtruderD.Ratio =1;
			}
			else
			{
				gExtruderD.ActSpeed = gExtruderD.ActRPM /gMachineFix.ExtruderD.Ratio;
			}
		}
		else
		{
			gExtruderD.ActRPM = 0.0;
			gExtruderD.Current = 0.0;
		}
		
		/*-------- Extruder Visu Use-------*/
		if(1 == gMachineFix.ExtruderVisu.Use)
		{
			// SetSpd condition
			if (1 == gExtruderHMI_Enable[4] && 1 == gMachineOut.ExtrVisu_On)
			{
				gExtruderVisu.aoSetRPM = (gMachinePara.ExtruderVisu.SetSpeed / gMachineFix.ExtruderVisu.RPM10v)*32767.0;
			}
			else
			{
				gExtruderVisu.aoSetRPM = 0;
			}
			
			// motor speed
			gExtruderVisu.ActRPM = gExtruderVisu.aiActRPM/32767.0  * gMachineFix.ExtruderVisu.RPM10v;
			gExtruderVisu.Current = gExtruderVisu.aiCurrent/32767.0 * gMachineFix.ExtruderVisu.Currency10V;
		
			//Screw speed
			if (0 == gMachineFix.ExtruderVisu.Ratio)
			{
				gMachineFix.ExtruderVisu.Ratio =1;
			}
			else
			{
				gExtruderVisu.ActSpeed = gExtruderVisu.ActRPM /gMachineFix.ExtruderVisu.Ratio;
			}
		}
		else
		{
			gExtruderVisu.ActRPM = 0.0;
			gExtruderVisu.Current = 0.0;
		}
	
	}
	
	/* ------------------------ Extruder Protect Delay Switch On  ---------------------------- */
	/* ----- A extruder ----- */
	if(0 == gExtruderA.Status.OnProtected  && 0 == gMachineOut.ExtrA_On )
	{
		gMacTimer.ExtrA_OnDelay.IN = 1;
		gMacTimer.ExtrA_OnDelay.PT = gMachineFix.ExtruderA.tProtect*100;
		
		if(1 == gMacTimer.ExtrA_OnDelay.Q)
		{
			gMacTimer.ExtrA_OnDelay.IN = 0;
			gExtruderA.Status.OnProtected = 1;
		}
	}
	else if( 1 == gMachineOut.ExtrA_On )
	{
		gExtruderA.Status.OnProtected = 0;
	}

	
	/* ----- B extruder ----- */
	if(0 == gExtruderB.Status.OnProtected  && 0 == gMachineOut.ExtrB_On )
	{
		gMacTimer.ExtrB_OnDelay.IN = 1;
		gMacTimer.ExtrB_OnDelay.PT = gMachineFix.ExtruderB.tProtect*100;
		
		if(1 == gMacTimer.ExtrB_OnDelay.Q)
		{
			gMacTimer.ExtrB_OnDelay.IN = 0;
			gExtruderB.Status.OnProtected = 1;
		}
	}
	else if( 1 == gMachineOut.ExtrB_On )
	{
		gExtruderB.Status.OnProtected = 0;
	}
	
	/* ----- C extruder ----- */
	if(0 == gExtruderC.Status.OnProtected && 0 == gMachineOut.ExtrC_On )
	{
		gMacTimer.ExtrC_OnDelay.IN = 1;
		gMacTimer.ExtrC_OnDelay.PT = gMachineFix.ExtruderC.tProtect*100;
		
		if(1 == gMacTimer.ExtrC_OnDelay.Q)
		{
			gMacTimer.ExtrC_OnDelay.IN = 0;
			gExtruderC.Status.OnProtected = 1;
		}
	}
	else if( 1 == gMachineOut.ExtrC_On )
	{
		gExtruderC.Status.OnProtected = 0;
	}
	
	/* ----- D extruder ----- */
	if(0 == gExtruderD.Status.OnProtected && 0 == gMachineOut.ExtrD_On )
	{
		gMacTimer.ExtrD_OnDelay.IN = 1;
		gMacTimer.ExtrD_OnDelay.PT = gMachineFix.ExtruderD.tProtect*100;
		
		if(1 == gMacTimer.ExtrD_OnDelay.Q)
		{
			gMacTimer.ExtrD_OnDelay.IN = 0;
			gExtruderD.Status.OnProtected = 1;
		}
	}
	else if( 1 == gMachineOut.ExtrD_On )
	{
		gExtruderD.Status.OnProtected = 0;
	}
	
	/* ----- V extruder ----- */
	if(0 == gExtruderVisu.Status.OnProtected && 0 == gMachineOut.ExtrVisu_On )
	{
		gMacTimer.ExtrVisu_OnDelay.IN = 1;
		gMacTimer.ExtrVisu_OnDelay.PT = gMachineFix.ExtruderVisu.tProtect*100;
		
		if(1 == gMacTimer.ExtrVisu_OnDelay.Q)
		{
			gMacTimer.ExtrVisu_OnDelay.IN = 0;
			gExtruderVisu.Status.OnProtected = 1;
		}
	}
	else if( 1 == gMachineOut.ExtrVisu_On )
	{
		gExtruderVisu.Status.OnProtected = 0;
	}

   
	/* ----------------------- Switch On extruder ----------------------- */	
	if(gPanelObj.GetPicture != PIC_INIT && gPanelObj.GetPicture != PIC_SCREEN_SAVE)
	{
		/*	-----------Extruder A ----------*/
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_A_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_A_ABB]))
			{	
			if (1==gMachineFix.ExtruderA.Use)
			{
				if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
				{
					if (0==gMachineInfo.bLoweringFlag)  //ipis0614
					{	
						if(1 == gExtruderA.Status.OnProtected || 1 == gMachineOut.ExtrA_On) 
						{
							gMachineOut.ExtrA_On = !gMachineOut.ExtrA_On; //DO	
						}
						else
						{	
							gImply.ExtrA_OnDelay = 1;
						}	
					}
					else
					{
						gImply.TempLoweringEnable =1;	
					}
				}	
				else
				{
					gImply.TempLow = gExtruderTempLow_HMIFlag;	
					gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;
				}
			}
			else
			{
				gMachineOut.ExtrA_On =0;
				gImply.ExtruderNoChoose =1;	
			}
		}
				
		/*	-----------Extruder B ----------*/	
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_B_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_B_ABB]))
			{
			if (1==gMachineFix.ExtruderB.Use)
			{
				if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
				{	
					if (0==gMachineInfo.bLoweringFlag) //ipis0614
					{
						if(1 == gExtruderB.Status.OnProtected || 1 == gMachineOut.ExtrB_On) //owen 20200921
						{
							gMachineOut.ExtrB_On = !gMachineOut.ExtrB_On; //DO	
						}
						else
						{	
							gImply.ExtrB_OnDelay = 1;
						}
					}
					else
					{
						gImply.TempLoweringEnable =1;	
					}
				}
				else
				{
					gImply.TempLow = gExtruderTempLow_HMIFlag;	
					gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;	
				}
			}
			else
			{
				gMachineOut.ExtrB_On=0;
				gImply.ExtruderNoChoose =1;	
			}	
		}
	
		/*	-----------Extruder C ----------*/
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_C_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_C_ABB]))
			{
			
			if (1==gMachineFix.ExtruderC.Use)
			{
				if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
				{		
					if (0==gMachineInfo.bLoweringFlag) //ipis0614
					{
						if(1 == gExtruderC.Status.OnProtected || 1 == gMachineOut.ExtrC_On) //owen 20200921
						{
							gMachineOut.ExtrC_On =!gMachineOut.ExtrC_On; //DO	
						}
						else
						{	
							gImply.ExtrC_OnDelay = 1;
						}
					}
					else
					{
						gImply.TempLoweringEnable =1;	
					}
				}
				else
				{
					gImply.TempLow = gExtruderTempLow_HMIFlag;	
					gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;	
				}
			}
			else
			{
				gMachineOut.ExtrC_On =0;
				gImply.ExtruderNoChoose =1;	
			}		
		}
		
		/*	-----------Extruder D ----------*/
		if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_D_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_D_ABB]))
			{
			if (1==gMachineFix.ExtruderD.Use)
			{
				if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
				{
					if (0==gMachineInfo.bLoweringFlag) //ipis0614
					{
		
						if(1 == gExtruderD.Status.OnProtected || 1 == gMachineOut.ExtrD_On) //owen 20200921
						{
							gMachineOut.ExtrD_On =!gMachineOut.ExtrD_On; //DO	
						}
						else
						{	
							gImply.ExtrD_OnDelay = 1;
						}
					}
					else
					{
						gImply.TempLoweringEnable =1;	
					}
				
				}
				else
				{
					gImply.TempLow = gExtruderTempLow_HMIFlag;	
					gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;	
				}
			}
			else
			{
				gMachineOut.ExtrD_On =0;
				gImply.ExtruderNoChoose =1;	
			}
			

		}	
		
		/*	-----------Extruder Visu ----------*/
		if (1 == gMachineFix.ExtruderD.LinkWithMode) 
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_D_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_V_ABB]))
				{	
				if (1==gMachineFix.ExtruderVisu.Use)
				{
					if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
					{
						if (0==gMachineInfo.bLoweringFlag) //ipis0614
						{
							if(1 == gExtruderD.Status.OnProtected )  //from  extruder D switch on delay
							{
								if(1 == gExtruderVisu.Status.OnProtected || 1 == gMachineOut.ExtrVisu_On) //owen 20200921
								{
									gMachineOut.ExtrVisu_On =!gMachineOut.ExtrVisu_On; //DO	
								}
								else
								{	
									gImply.ExtrVisu_OnDelay = 1;
								}
							}
							else
							{
								gImply.ExtrD_OnDelay = 1;
							}			
						}
						else
						{
							gImply.TempLoweringEnable =1;	
						}				
					}
					else
					{
						gImply.TempLow = gExtruderTempLow_HMIFlag;	
						gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;	
					}	
				}
				else
				{
					gMachineOut.ExtrVisu_On=0;
					gImply.ExtruderNoChoose =1;	
				}		
			}
		
		}
		else
		{
			if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_EXTRUDER_V_ABB],PKeyMatric[S$KEYMATRIX_EXTRUDER_V_ABB]))
				{	
				if (1==gMachineFix.ExtruderVisu.Use)
				{
					if (0==gExtruderTempLow_HMIFlag && 0 == gMachineInfo.DieLipTempLow)  //ipis0718
					{
						if (0==gMachineInfo.bLoweringFlag) //ipis0614
						{
							if(1 == gExtruderVisu.Status.OnProtected || 1 == gMachineOut.ExtrVisu_On) //owen 20200921
							{
								gMachineOut.ExtrVisu_On =!gMachineOut.ExtrVisu_On; //DO	
							}
							else
							{	
								gImply.ExtrVisu_OnDelay = 1;
							}			
						}
						else
						{
							gImply.TempLoweringEnable =1;	
						}				
					}
					else
					{
						gImply.TempLow = gExtruderTempLow_HMIFlag;	
						gImply.DieLipTempLow = gMachineInfo.DieLipTempLow;	
					}	
				}
				else
				{
					gMachineOut.ExtrVisu_On=0;
					gImply.ExtruderNoChoose =1;	
				}		
			}
		}
		
	}
	
	/*	----------------------------------------------------------------------------------------------------------------------*/	
	
	
	
	gMachineInfo.Extruder       = gMachineOut.ExtrA_On;
	gMachineOut.ExtruderCooling = gMachineInfo.Extruder;
	
	/*	-------------------------------------------------------------- */
	/*  ---------------------- Stop extruder ------------------------- */
	/*	-------------------------------------------------------------- */
	/* ----- A extruder ----- */
	gExtruderA.bKeyOff = 0;
	if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat        	 || 1 == gMachineInfo.DieLipTempLow   /*|| 0 == gMachineIn.ParisonPumpRunning*/    
		|| 1 == gMachineIn.ExtrA_MotorErr || 0 == gMachineIn.ExtrA_Wire_MotorErr || 1 == gMachineIn.ExtrA_CoolMotorErr || 1 == gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop)
		/*|| 1 == gAlarm.A_ColiTempNormal   ||1 == gAlarm.A_ExtrMotorTempStop     || 1 == gAlarm.ExtrAMeltP */ // 2018.06.05 Jack: Add ExtrAMeltP alarm stop condition   
	{
		if(1 == gMachineOut.ExtrA_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			gAlarm.DieLipTempLow = gMachineInfo.DieLipTempLow;
			//		if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			if ( 1 == gMachineIn.ExtrA_MotorErr ||0 == gMachineIn.ExtrA_Wire_MotorErr)	gAlarm.A_ExtruderMotorOverload =1;  //ipis0323
				
			//		gAlarm.A_ExtruderMotorOverload = gMachineIn.ExtrA_MotorErr;
			gExtruderA.bKeyOff = 1;
			gMachineOut.ExtrA_On =0;
			gMachineInfo.ExtrA_ErrStop_LinkWith = 1;  //Extruder Link With Selection
		}
	}
   
	/* ----- B extruder ----- */
	gExtruderB.bKeyOff = 0;
	if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat        	 || 1 == gMachineInfo.DieLipTempLow   /*|| 0 == gMachineIn.ParisonPumpRunning*/
	 || 1 == gMachineIn.ExtrB_MotorErr || 0 == gMachineIn.ExtrB_Wire_MotorErr || 1 == gMachineIn.ExtrB_CoolMotorErr ||1==gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop) 
	{
		if(1 == gMachineOut.ExtrB_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			gAlarm.DieLipTempLow = gMachineInfo.DieLipTempLow;
			//	if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			if (  1 == gMachineIn.ExtrB_MotorErr ||0 == gMachineIn.ExtrB_Wire_MotorErr)	gAlarm.B_ExtruderMotorOverload =1;  //ipis0323
	
			//		gAlarm.B_ExtruderMotorOverload = gMachineIn.ExtrB_MotorErr;
			gExtruderB.bKeyOff = 1;
			gMachineOut.ExtrB_On =0;
		}
	}
   
	/* ----- C extruder ----- */
	gExtruderC.bKeyOff = 0;
	if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat        	 || 1 == gMachineInfo.DieLipTempLow   /*|| 0 == gMachineIn.ParisonPumpRunning*/   || 
		1 == gMachineIn.ExtrC_MotorErr || 0 == gMachineIn.ExtrC_Wire_MotorErr || 1 == gMachineIn.ExtrC_CoolMotorErr ||1==gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop )
	{
		if(1 == gMachineOut.ExtrC_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			gAlarm.DieLipTempLow = gMachineInfo.DieLipTempLow;
			//	if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			if (  1 == gMachineIn.ExtrC_MotorErr ||0 == gMachineIn.ExtrC_Wire_MotorErr)	gAlarm.C_ExtruderMotorOverload =1;  //ipis0323
	
			//			gAlarm.C_ExtruderMotorOverload = gMachineIn.ExtrC_MotorErr;
			
			gExtruderC.bKeyOff = 1;
			gMachineOut.ExtrC_On =0;
		}
	}
	
	/* ----- D extruder ----- */
	gExtruderD.bKeyOff = 0;
	if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat        	 || 1 == gMachineInfo.DieLipTempLow   /*|| 0 == gMachineIn.ParisonPumpRunning*/   ||  
		1 == gMachineIn.ExtrD_MotorErr || 0 == gMachineIn.ExtrD_Wire_MotorErr|| 1 == gMachineIn.ExtrD_CoolMotorErr||1==gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop) 
	{
		if(1 == gMachineOut.ExtrD_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			gAlarm.DieLipTempLow = gMachineInfo.DieLipTempLow;
			//			if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			if (  1 == gMachineIn.ExtrD_MotorErr ||0 == gMachineIn.ExtrD_Wire_MotorErr)	gAlarm.D_ExtruderMotorOverload = 1;  //ipis0323
	
			//		gAlarm.B_ExtruderMotorOverload = gMachineIn.ExtrD_MotorErr;
			gExtruderD.bKeyOff = 1;
			gMachineOut.ExtrD_On =0;
			gMachineInfo.ExtrD_ErrStop_LinkWith = 1;   //Extruder Link With Selection
		}
	}
			
	/* ----- Visual extruder ----- */
	gExtruderVisu.bKeyOff = 0;
	if (1 == gMachineInfo.ExtrTLow     || 0 == gMachineInfo.IsHeat        	 || 1 == gMachineInfo.DieLipTempLow   /*|| 0 == gMachineIn.ParisonPumpRunning*/   || 
		1 == gMachineIn.ExtrV_MotorErr || 0 == gMachineIn.ExtrV_Wire_MotorErr|| 1 == gMachineIn.ExtrV_CoolMotorErr /*|| 0 == gMachineOut.ExtrA_On*/ ||1==gMachineIn.InstancyStop || 1 == gMachineInfo.SafeInStop) 
	{
		if(1 == gMachineOut.ExtrVisu_On)
		{
			if(1 == gMachineInfo.ExtrTLow)gAlarm.ScrewTempLow = 1;
			if(0 == gMachineInfo.IsHeat)gAlarm.HeatingNotOn = 1;
			gAlarm.DieLipTempLow = gMachineInfo.DieLipTempLow;
			//	if(0 == gMachineIn.ParisonPumpRunning)gAlarm.ParisonPumpNotOn = 1;
			if (  1 == gMachineIn.ExtrV_MotorErr ||0 == gMachineIn.ExtrV_Wire_MotorErr)	gAlarm.VisuExtrMotorOverload =1;  //ipis0323

			//	gAlarm.VisuExtrMotorOverload = gMachineIn.ExtrV_MotorErr;				
			gExtruderVisu.bKeyOff = 1;
			gMachineOut.ExtrVisu_On =0;
			gMachineInfo.ExtrVisu_ErrStop_LinkWith = 1;  //Extruder Link With Selection
		}
	}
	
	/* ----- All extruder ----- */
	
	
	/*	-------------------------------------------------------------- */
	/* ---------------- Extruder Link With Selection  ---------------- */
	/*	-------------------------------------------------------------- */
	/* ----- A extruder ----- */
	if(1 == gMachineFix.ExtruderA.LinkWithMode)
	{
		if( EDGENEG(gMachineOut.ExtrA_On,oldExtrA_On))
		{
			gMachineOut.ParisonPump = 0;
		}
		else if (1 == gMachineInfo.ExtrA_ErrStop_LinkWith)
		{
			gMachineInfo.ExtrA_ErrStop_LinkWith =0;
			gMachineOut.ParisonPump = 0;
		}
	}
	
	/* ----- D extruder ----- */
	if(1 == gMachineFix.ExtruderD.LinkWithMode)
	{
		if( EDGENEG(gMachineOut.ExtrD_On,oldExtrD_On))
		{
			gMachineOut.ExtrVisu_On = 0;
		}
		else if (1 == gMachineInfo.ExtrD_ErrStop_LinkWith)
		{
			gMachineInfo.ExtrD_ErrStop_LinkWith = 0;
			gMachineOut.ExtrVisu_On = 0;
		}
	}

	/* ----- Visu extruder ----- */
	if(1 == gMachineFix.ExtruderVisu.LinkWithMode)
	{
		if( EDGENEG(gMachineOut.ExtrVisu_On,oldExtrVisu_On))
		{
			gMachineOut.ExtrD_On = 0;
		}
		else if (1 == gMachineInfo.ExtrVisu_ErrStop_LinkWith)
		{
			gMachineInfo.ExtrVisu_ErrStop_LinkWith =0;
			gMachineOut.ExtrD_On = 0;
		}
	}
	
	oldExtrA_On = gMachineOut.ExtrA_On;
	oldExtrB_On = gMachineOut.ExtrB_On;
	oldExtrC_On = gMachineOut.ExtrC_On;
	oldExtrD_On = gMachineOut.ExtrD_On;
	oldExtrVisu_On = gMachineOut.ExtrVisu_On;
	


	
	/* ------------------------ Auto error stop extruder ---------------------------- */
	if(1 == gPROCESS_info.extruderStopEnable)
	{
		cntExtruderStopDelay ++;
	}
	else
	{
		cntExtruderStopDelay = 0;
	}
   
	if(gMachineFix.ExtruderA.StopTime <= 0.0)gMachineFix.ExtruderA.StopTime = 3.0;
	if(gMachineFix.ExtruderA.StopTime < cntExtruderStopDelay * tTask)
	{
		if(1 == gTKmanual.Status || 1 == gTKauto.Status)
		{
			gDisableAll_HMI 				=1;	  //ipis0719
			//			gMachineOut.ExtrA_On =0;
			//			gMachineOut.ExtrB_On =0;
			//			gMachineOut.ExtrD_On =0;
			//			gMachineOut.ExtrVisu_On =0;
			gExtruderA.bKeyOff = 1;
			gExtruderB.bKeyOff = 1;
			gExtruderC.bKeyOff = 1;
			gExtruderD.bKeyOff =1;
			gExtruderVisu.bKeyOff = 1;
			gPROCESS_info.extruderStopEnable = 0;
			cntExtruderStopDelay = 0;
		}
	}
}
/**************************************************************************************
        				   E X I T   P A R T   O F   T H E   T A S K
**************************************************************************************/
void _EXIT extruderEXIT(void)
{
	/* TODO: Add code here */
}


