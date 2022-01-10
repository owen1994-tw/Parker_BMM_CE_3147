
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT eplInit(void)
{
	EPLInverter[0].cmd.readNode = 1;
	EPLInverter[1].cmd.readNode = 1;
	EPLInverter[2].cmd.readNode = 1;
	EPLInverter[3].cmd.readNode = 1;
	
	strcpy(&sDevice[0],"SL1.IF1");
	strcpy(&sDevice[1],"SL1.IF1");
	strcpy(&sDevice[2],"SL1.IF1");
	strcpy(&sDevice[3],"SL1.IF1");
	
	
		
	strcpy(&sDeviceAExtr,"SL1.IF1");
	
	uIndexOffset = 7;
	
	
	/*Albert*/
	
	uCountTimerA = 100;/*1 second*/
	uCountTimerB = 100;/*1 second*/
	uCountTimerC = 100;/*1 second*/
	uCountTimerV = 100;/*1 second*/
	
	uIndexA = 0;
	uIndexB = 0;
	uIndexC = 0;
	uIndexV = 0;
}

void _CYCLIC eplCyclic(void)
{
//	int i;
//	for(i=0;i<4;i++)/*For extruder A , B , C and V*/
//	{
//		if(EPLInverter[i].para.nodeNr != 0)
//		{
//			if(EPLInverter[i].state = EPL_WAIT)
//			{
//				EPLInverter[i].cmd.bReadObj = 1;/*read command*/
//				if (0 == uIndexRead[i])
//				{
//					EPLInverter[i].para.MotorPower = (INT) EPLInverter[i].para.readData;
//					uIndexRead[i] = uIndexRead[i]+10;
//					EPLInverter[i].cmd.index = 0x2002;
//					EPLInverter[i].cmd.subindex = 0x9;
//				}
//				else if (10 == EPLInverter[i].cmd.index)
//				{
//					EPLInverter[i].para.MotorVoltage = (INT) EPLInverter[i].para.readData;
//					uIndexRead[i] = 0;
//					EPLInverter[i].cmd.index = 0x2002;
//					EPLInverter[i].cmd.subindex = 0xC;
//				}
//			}
//		}
//		switch (EPLInverter[i].state)
//		{
//			case EPL_WAIT: /*state to wait for user Commands*/
//				
//				if(1 == EPLInverter[i].cmd.readNode)
//				{
//					EPLInverter[i].cmd.readNode = 0;
//					EPLInverter[i].state = EPL_READ_NODE;
//				}
//				else if(1 == EPLInverter[i].cmd.bReadObj)
//				{
//					EPLInverter[i].cmd.readThresholdCnt = 0;
//					EPLInverter[i].state = EPL_READ_OE;
//				}
//				else if(1 == EPLInverter[i].cmd.bWriteObj)
//				{
//					EPLInverter[i].cmd.bWriteObj = 0;
//					EPLInverter[i].state = EPL_WRITE_OE;
//				}
//				
//				break;
//			case EPL_READ_NODE:/*state to read the local Powerlink node number*/
//				EPLInverter[i].fub.fnEplGetLocalNodeID.enable = 1;
//				EPLInverter[i].fub.fnEplGetLocalNodeID.pDevice = (UDINT) &sDevice[i];
//				
//				if(EPLInverter[i].fub.fnEplGetLocalNodeID.status == 0)
//				{
//					EPLInverter[i].para.nodeNr = EPLInverter[i].fub.fnEplGetLocalNodeID.nodeid;
//					EPLInverter[i].fub.fnEplGetLocalNodeID.enable = 0;
//					EPLInverter[i].state = EPL_WAIT;
//				}
//				else if(EPLInverter[i].fub.fnEplGetLocalNodeID.status == ERR_FUB_BUSY)
//				{
//					EPLInverter[i].state = EPL_READ_NODE;/*creating not finished --> do again*/
//				}
//				else if (EPLInverter[i].fub.fnEplGetLocalNodeID.status == ERR_FUB_ENABLE_FALSE)
//				{
//					/*FUB is disabled*/
//				}
//				else
//				{
//					EPLInverter[i].state = EPL_ERROR;
//				}
//				
//				
//				break;
//			case EPL_READ_OE:/*state to read an object entry*/
//				
//				EPLInverter[i].fub.fnEplSDORead.enable = 1;
//				EPLInverter[i].fub.fnEplSDORead.pDevice = (UDINT) &sDevice[i];
//				EPLInverter[i].fub.fnEplSDORead.node = uIndexOffset + i;
//				EPLInverter[i].fub.fnEplSDORead.index = EPLInverter[i].cmd.index;
//				EPLInverter[i].fub.fnEplSDORead.subindex = EPLInverter[i].cmd.subindex;
//				EPLInverter[i].fub.fnEplSDORead.pData = (UDINT) &EPLInverter[i].para.readData;
//				EPLInverter[i].fub.fnEplSDORead.datalen = sizeof(EPLInverter[i].para.readData);
//				
//				if(EPLInverter[i].fub.fnEplSDORead.status == 0)
//				{
//					EPLInverter[i].fub.fnEplSDORead.enable = 0;
//					EPLInverter[i].state = EPL_WAIT;
//				}
//				else if(EPLInverter[i].fub.fnEplSDORead.status == ERR_FUB_BUSY)
//				{
//					EPLInverter[i].state = EPL_READ_OE;
//				}
//				else if(EPLInverter[i].fub.fnEplSDORead.status == ERR_FUB_ENABLE_FALSE)
//				{
//				}
//				else
//				{
//					EPLInverter[i].state = EPL_ERROR;
//				}
//				
//				break;
//			case EPL_WRITE_OE:/*state to write an object entry*/
//				break;
//			case EPL_ERROR:/*State when an error occured*/
//				break;
//		}
//	}

	
//	if(bTestRead)
//	{
		for(i=0;i<4;i++)
		{
			if(EPLInverter[i].para.nodeNr != 0)
			{
				if(EPLInverter[i].state == EPL_WAIT)
				{
					EPLInverter[i].cmd.bReadObj = 1;/*read command*/
					if (0 == uIndexRead[i])
					{
						EPLInverter[i].para.EnergyCons = (INT) EPLInverter[i].para.readData;
//						EPLInverter[i].para.MotorPower = (INT) EPLInverter[i].para.readData;
						uIndexRead[i] = uIndexRead[i]+10;
						EPLInverter[i].cmd.index = 0x2002;
						EPLInverter[i].cmd.subindex = 0x9;
					}
					else if (10 == uIndexRead[i])
					{
						EPLInverter[i].para.MotorVoltage = (INT) EPLInverter[i].para.readData;
//						EPLInverter[i].para.MotorVoltage = (INT) EPLInverter[i].para.readData;
						uIndexRead[i] = uIndexRead[i]+10;
						EPLInverter[i].cmd.index = 0x2002;
						EPLInverter[i].cmd.subindex = 0xC;
					}
					else if(20 == uIndexRead[i])
					{
						EPLInverter[i].para.MotorPower = (INT) EPLInverter[i].para.readData;
//						EPLInverter[i].para.EnergyCons = (INT) EPLInverter[i].para.readData;
						uIndexRead[i] = 0;
						EPLInverter[i].cmd.index = 0x2002;
						EPLInverter[i].cmd.subindex = 0x1F;
					}
				}
			}
			switch (EPLInverter[i].state)
			{
				case EPL_WAIT: /*state to wait for user Commands*/
				
					if(1 == EPLInverter[i].cmd.readNode)
					{
						EPLInverter[i].cmd.readNode = 0;
						EPLInverter[i].state = EPL_READ_NODE;
					}
					else if(1 == EPLInverter[i].cmd.bReadObj)
					{
						EPLInverter[i].cmd.bReadObj = 0;
						EPLInverter[i].state = EPL_READ_OE;
					}
					else if(1 == EPLInverter[i].cmd.bWriteObj)
					{
						EPLInverter[i].cmd.bWriteObj = 0;
						EPLInverter[i].state = EPL_WRITE_OE;
					}
				
					break;
				case EPL_READ_NODE:/*state to read the local Powerlink node number*/
					EPLInverter[i].fub.fnEplGetLocalNodeID.enable = 1;
					EPLInverter[i].fub.fnEplGetLocalNodeID.pDevice = (UDINT) &sDevice[i];
				
					if(EPLInverter[i].fub.fnEplGetLocalNodeID.status == 0)
					{
						EPLInverter[i].para.nodeNr = EPLInverter[i].fub.fnEplGetLocalNodeID.nodeid;
						EPLInverter[i].fub.fnEplGetLocalNodeID.enable = 0;
						EPLInverter[i].state = EPL_WAIT;

					}
					else if(EPLInverter[i].fub.fnEplGetLocalNodeID.status == ERR_FUB_BUSY)
					{
						EPLInverter[i].state = EPL_READ_NODE;/*creating not finished --> do again*/
					}
					else if (EPLInverter[i].fub.fnEplGetLocalNodeID.status == ERR_FUB_ENABLE_FALSE)
					{
						/*FUB is disabled*/
					}
					else
					{
						EPLInverter[i].state = EPL_ERROR;
					}
				
				
					break;
				case EPL_READ_OE:/*state to read an object entry*/
				
					EPLInverter[i].fub.fnEplSDORead.enable = 1;
					EPLInverter[i].fub.fnEplSDORead.pDevice = (UDINT) &sDeviceAExtr;
					EPLInverter[i].fub.fnEplSDORead.node = uIndexOffset;
					EPLInverter[i].fub.fnEplSDORead.index = EPLInverter[i].cmd.index;
					EPLInverter[i].fub.fnEplSDORead.subindex = EPLInverter[i].cmd.subindex;
					EPLInverter[i].fub.fnEplSDORead.pData = (UDINT) &EPLInverter[i].para.readData;
					EPLInverter[i].fub.fnEplSDORead.datalen = sizeof(EPLInverter[i].para.readData);
				
					if(EPLInverter[i].fub.fnEplSDORead.status == 0)
					{
						EPLInverter[i].fub.fnEplSDORead.enable = 0;
						EPLInverter[i].state = EPL_WAIT;
					}
					else if(EPLInverter[i].fub.fnEplSDORead.status == ERR_FUB_BUSY)
					{
						EPLInverter[i].state = EPL_READ_OE;
					}
					else if(EPLInverter[i].fub.fnEplSDORead.status == ERR_FUB_ENABLE_FALSE)
					{
					}
					else
					{
						EPLInverter[i].state = EPL_ERROR;
					}
				
					break;
				case EPL_WRITE_OE:/*state to write an object entry*/
					break;
				case EPL_ERROR:/*State when an error occured*/
					break;
			}
		}
//	}
//	else
//	{
//		EPLInverter[i].fub.fnEplSDORead.enable = 0;
//		EPLInverter[i].fub.fnEplGetLocalNodeID.enable = 0;
//		EPLInverter[i].fub.fnEplSDOWrite.enable = 0;
//	}
	
	
	
	
	
	/*P84 Inverter power consumption*/
	
	
	/*Albert Add power consumption function*/
	
	/*----A Extruder-----*/
	switch (uStepPowerA)
	{
		case 0:/*Weit step*/
		if(gMachineInfo.Auto)
//			if(bTestAuto)
			{
				fnTON_10msA.IN = 1;
				fnTON_10msA.PT = uCountTimerA;
				uStepPowerA = 10;
			}
			break;
		case 10:/*Start Timer*/
			if(fnTON_10msA.Q)
			{
				fnTON_10msA.IN = 0;
				uIndexA++;
				uStepPowerA = 20;
			}
			if(0 == gMachineInfo.Auto)
//			if(0 == bTestAuto)
			{
				uIndexA	=	0;
				fnTON_10msA.IN = 0;
				uStepPowerA = 0;
			}
			break;
		case 20:/*Get value*/
			if(uIndexA > 59) 
			{
				uIndexA = 0;
				rPowerTotalA += (rPowerTotalPerSecA/3600);
			}
			rPowerTotalPerSecA = 0;	
			
			
			rActCurrentA[uIndexA] = gExtruderACtrl.InverterStatus.Current/10;/*Change unit from 0.1A to 1A*/
			rActVoltageA[uIndexA] = EPLInverter[0].para.MotorVoltage;
	//		rActCurrentA[uIndexA] = mcParID_Value.readPar.actCurrent;
	//		rActVoltageA[uIndexA] = mcParID_Value.readPar.actUDC;
	//		rActCurrentA[uIndexA] = rTestCurrent;
	//		rActVoltageA[uIndexA] = rTestVoltage;
			
			uStepPowerA = 30;
			
			break;
		case 30:/*Calculate*/
			rPowerA[uIndexA] = (rActCurrentA[uIndexA] * rActVoltageA[uIndexA])/1000;
			for(i_A=0;i_A<60;i_A++)
			{
				rPowerTotalPerSecA += rPowerA[i_A];
			}
			rPowerPerMinA= rPowerTotalPerSecA/60;
			uStepPowerA = 0;
			break;
	}
	
	
	/*Function call*/
	TON_10ms(&fnTON_10msA);
	
		
	/*Reset Function*/
	if(bCleanPowerA)
	{
		iClickTimeA--;
		if(iClickTimeA <= 0)
		{
			bCleanPowerA = 0;
			rPowerTotalA = 0;
			rPowerPerMinA = 0;
		}
	}
	else
	{
		iClickTimeA = 10;
	}
	
	
	
	
	
	/*----B Extruder-----*/
	switch (uStepPowerB)
	{
		case 0:/*Weit step*/
			if(gMachineInfo.Auto)
			{
				fnTON_10msB.IN = 1;
				fnTON_10msB.PT = uCountTimerB;
				uStepPowerB = 10;
			}
			break;
		case 10:/*Start Timer*/
			if(fnTON_10msB.Q)
			{
				fnTON_10msB.IN = 0;
				uIndexB++;
				uStepPowerB = 20;
			}
			if(0 == gMachineInfo.Auto)
			{
				uIndexB	=	0;
				fnTON_10msB.IN = 0;
				uStepPowerB = 0;
			}
			break;
		case 20:/*Get value*/
			if(uIndexB > 59) 
			{
				uIndexB = 0;
				rPowerTotalB += (rPowerTotalPerSecB/3600);
			}
			rPowerTotalPerSecB = 0;	
			
			
			rActCurrentB[uIndexB] = gExtruderBCtrl.InverterStatus.Current/10;/*Change unit from 0.1A to 1A*/
			rActVoltageB[uIndexB] = EPLInverter[1].para.MotorVoltage;
			//		rActCurrentA[uIndexA] = mcParID_Value.readPar.actCurrent;
			//		rActVoltageA[uIndexA] = mcParID_Value.readPar.actUDC;
			
			uStepPowerB = 30;
			
			break;
		case 30:/*Calculate*/
			rPowerB[uIndexB] = (rActCurrentB[uIndexB] * rActVoltageB[uIndexB])/1000;
			for(i_B=0;i_B<60;i_B++)
			{
				rPowerTotalPerSecB += rPowerB[i_B];
			}
			rPowerPerMinB = rPowerTotalPerSecB/60;
			uStepPowerB = 0;
			break;
	}
	/*Calculate total power*/
//	switch (uStepPowerHourB)
//	{
//		case 0:
//			if(gMachineInfo.Auto)
//			{
//				fnTON_10msB_HR.IN = 1;
//				fnTON_10msB_HR.PT = 60000;
//				uStepPowerHourB = 10;
//			}		
//			break;
//		case 10:
//			if(fnTON_10msB_HR.Q)
//			{
//				fnTON_10msB_HR.IN = 0;
//				uStepPowerHourB = 20;
//			}
//			if(0 == gMachineInfo.Auto)
//			{
//				fnTON_10msB_HR.IN = 1;
//				fnTON_10msB_HR.PT = 60000;
//				uStepPowerHourB = 10;
//			}
//			break;
//		case 20:
//			rPowerTotalB = rPowerTotalB + (rPowerPerSecB/6);
//			uStepPowerHourB = 30;
//			break;
//		case 30:
//			uStepPowerHourB = 0;
//			break;
//	}
	
	/*Function call*/
	TON_10ms(&fnTON_10msB);
	TON_10ms(&fnTON_10msB_HR);
		
	/*Reset Function*/
	if(bCleanPowerB)
	{
		iClickTimeB--;
		if(iClickTimeB <= 0)
		{
			bCleanPowerB = 0;
			rPowerTotalB = 0;
			rPowerPerMinB = 0;
		}
	}
	else
	{
		iClickTimeB = 10;
	}
	
	
	
	
	/*----C Extruder-----*/
	switch (uStepPowerC)
	{
		case 0:/*Weit step*/
			if(gMachineInfo.Auto)
			{
				fnTON_10msC.IN = 1;
				fnTON_10msC.PT = uCountTimerC;
				uStepPowerC = 10;
			}
			break;
		case 10:/*Start Timer*/
			if(fnTON_10msC.Q)
			{
				fnTON_10msC.IN = 0;
				uIndexC++;
				uStepPowerC = 20;
			}
			if(0 == gMachineInfo.Auto)
			{
				uIndexC	=	0;
				fnTON_10msC.IN = 0;
				uStepPowerC = 0;
			}
			break;
		case 20:/*Get value*/
			if(uIndexC > 59) 
			{
				uIndexC = 0;
				rPowerTotalC += rPowerTotalPerSecC;
//				rPowerPerHrC = rPowerPerSecC * 60 * 60;
			}
			rPowerTotalPerSecC = 0;	
			
			
			rActCurrentC[uIndexC] = gExtruderCCtrl.InverterStatus.Current/10;/*Change unit from 0.1A to 1A*/
			rActVoltageC[uIndexC] = EPLInverter[2].para.MotorVoltage;
			//		rActCurrentA[uIndexA] = mcParID_Value.readPar.actCurrent;
			//		rActVoltageA[uIndexA] = mcParID_Value.readPar.actUDC;
			
			uStepPowerC = 30;
			
			break;
		case 30:/*Calculate*/
			rPowerC[uIndexC] = (rActCurrentC[uIndexC] * rActVoltageC[uIndexC])/1000;
			for(i_C=0;i_C<60;i_C++)
			{
				rPowerTotalPerSecC += rPowerC[i_C];
			}
			rPowerPerMinC = rPowerTotalPerSecC/60;
			uStepPowerC = 0;
			break;
	}
	
	
	/*Function call*/
	TON_10ms(&fnTON_10msC);
	
		
	/*Reset Function*/
	if(bCleanPowerC)
	{
		iClickTimeC--;
		if(iClickTimeC <= 0)
		{
			bCleanPowerC = 0;
			rPowerTotalC = 0;
			rPowerPerMinC = 0;
		}
	}
	else
	{
		iClickTimeC = 10;
	}
	
	
	/*----V Extruder-----*/
	switch (uStepPowerV)
	{
		case 0:/*Weit step*/
			if(gMachineInfo.Auto)
			{
				fnTON_10msV.IN = 1;
				fnTON_10msV.PT = uCountTimerV;
				uStepPowerV = 10;
			}
			break;
		case 10:/*Start Timer*/
			if(fnTON_10msV.Q)
			{
				fnTON_10msV.IN = 0;
				uIndexV++;
				uStepPowerV = 20;
			}
			if(0 == gMachineInfo.Auto)
			{
				uIndexV	=	0;
				fnTON_10msV.IN = 0;
				uStepPowerV = 0;
			}
			break;
		case 20:/*Get value*/
			if(uIndexV > 59) 
			{
				uIndexV = 0;
				rPowerTotalV += (rPowerTotalPerSecV/3600);
			}
			rPowerTotalPerSecV = 0;	
			
			
			rActCurrentV[uIndexV] = gExtruderVCtrl.InverterStatus.Current/10;/*Change unit from 0.1A to 1A*/
			rActVoltageV[uIndexV] = EPLInverter[3].para.MotorVoltage;
			//		rActCurrentA[uIndexA] = mcParID_Value.readPar.actCurrent;
			//		rActVoltageA[uIndexA] = mcParID_Value.readPar.actUDC;
			
			uStepPowerV = 30;
			
			break;
		case 30:/*Calculate*/
			rPowerV[uIndexV] = (rActCurrentV[uIndexV] * rActVoltageV[uIndexV])/1000;
			for(i_V=0;i_V<60;i_V++)
			{
				rPowerTotalPerSecV += rPowerV[i_V];
			}
			rPowerPerMinV = rPowerTotalPerSecV/60;
			uStepPowerV = 0;
			break;
	}
	
	
	/*Function call*/
	TON_10ms(&fnTON_10msV);
	
		
	/*Reset Function*/
	if(bCleanPowerV)
	{
		iClickTimeV--;
		if(iClickTimeV <= 0)
		{
			bCleanPowerV = 0;
			rPowerTotalV = 0;
			rPowerPerMinV = 0;
		}
	}
	else
	{
		iClickTimeV = 10;
	}
	
	/*Function Call*/
	
	EplGetLocalNodeID(&EPLInverter[0].fub.fnEplGetLocalNodeID);
	EplGetLocalNodeID(&EPLInverter[1].fub.fnEplGetLocalNodeID);
	EplGetLocalNodeID(&EPLInverter[2].fub.fnEplGetLocalNodeID);
	EplGetLocalNodeID(&EPLInverter[3].fub.fnEplGetLocalNodeID);
	
	EplSDORead(&EPLInverter[0].fub.fnEplSDORead);
	EplSDORead(&EPLInverter[1].fub.fnEplSDORead);
	EplSDORead(&EPLInverter[2].fub.fnEplSDORead);
	EplSDORead(&EPLInverter[3].fub.fnEplSDORead);
	
	EplSDOWrite(&EPLInverter[0].fub.fnEplSDOWrite);
	EplSDOWrite(&EPLInverter[1].fub.fnEplSDOWrite);
	EplSDOWrite(&EPLInverter[2].fub.fnEplSDOWrite);
	EplSDOWrite(&EPLInverter[3].fub.fnEplSDOWrite);
	
}

void _EXIT eplExit(void)
{
	// Insert code here 

}

