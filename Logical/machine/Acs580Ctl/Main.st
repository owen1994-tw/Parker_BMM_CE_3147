(********************************************************************
 * COPYRIGHT -- Microsoft
 ********************************************************************
 * PROGRAM: Acs380Ctl
 * File: Acs380Cyclic.st
 * Author: HuangPengbin
 * Created: April 16, 2018
 ********************************************************************
 * Implementation OF PROGRAM Acs380Ctl
 ********************************************************************)

PROGRAM _INIT
	RTInfo_0.enable := 1;
	RTInfo_0();
	CycTime := RTInfo_0.cycle_time/1000;
	AcsIndex :=0;
	FOR AcsIndex := 0 TO NUM_ACS380_MINUS_ONE BY 1DO
		Step[AcsIndex] := STATE_ESTOP;
		gAcs380Ctrl.Command.AlmReset[AcsIndex] := 1;
		gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;    // 20190114
	END_FOR
	
END_PROGRAM


PROGRAM _CYCLIC
	
	
	IF (CTRL_ABB_POWERLINK = gMachineFix.ExtruderA.eCtrlType) THEN   // ABB Driver powerlink Commuication 
	
		Extruder_Use[0] :=gMachineFix.ExtruderA.Use;
		Extruder_Use[1] :=gMachineFix.ExtruderB.Use;
		Extruder_Use[2] :=gMachineFix.ExtruderC.Use;
		Extruder_Use[3] :=gMachineFix.ExtruderD.Use;
		Extruder_Use[4] :=gMachineFix.ExtruderVisu.Use;
	
		(* AcsIndex :=0;*)
		FOR AcsIndex := 0 TO NUM_ACS380_MINUS_ONE BY 1 DO
			//ETADÊÇACS380×´Ì¬×Ö
			TempETAD[AcsIndex] := gAcs380Ctrl.Status.ETAD[AcsIndex] AND 16#0008;
			////////////////////////////////////////////////////////////////////////////////////////////////////
			EstopCon[AcsIndex]			:= gAcs380Ctrl.Status.ETAD[AcsIndex] AND 16#028F;//16#0280,10#640
			StopCon[AcsIndex]			:= gAcs380Ctrl.Status.ETAD[AcsIndex] AND 16#023F;//16#0231,10#561
			ReadySwitchOnCon[AcsIndex] 	:= gAcs380Ctrl.Status.ETAD[AcsIndex] AND 16#023F;//16#0233,10#563
			SwitchOnCon[AcsIndex] 		:= gAcs380Ctrl.Status.ETAD[AcsIndex] AND 16#133F;//16#1337,10#4919
			////////////////////////////////////////////////////////////////////////////////////////////////////
			// Acs380 state  switch off;run;fault;
			FaultFlg[AcsIndex] := 0;
			IF  ( BIT_TST(TempETAD[AcsIndex],ALARM_BIT) OR (gAcs380Ctrl.Status.ModuleOk[AcsIndex] = 0)) THEN
				gAcs380Ctrl.Status.ActState[AcsIndex] := ACS_FAULT;
				FaultFlg[AcsIndex] := 1;
			END_IF		
			IF ReadySwitchOnCon[AcsIndex] = 16#0233 OR EstopCon[AcsIndex] = 16#0280 THEN
				gAcs380Ctrl.Status.ActState[AcsIndex] := ACS_POWER_OFF;
			END_IF
			IF SwitchOnCon[AcsIndex] = 16#1337 OR SwitchOnCon[AcsIndex] = 16#1237 OR SwitchOnCon[AcsIndex] = 16#1B37 THEN
				gAcs380Ctrl.Status.ActState[AcsIndex] := ACS_RUN;
			END_IF

			IF FaultFlg[AcsIndex] = 1 THEN
				//			gWndCtrl.Alarm.ServErrNm[AcsIndex] := gAcs380Ctrl.Status.FaultNum[AcsIndex];//20151012 ±äÆµÆ÷´íÎóºÅ
			ELSE
				//			gWndCtrl.Alarm.ServErrNm[AcsIndex] := 0;
			END_IF
			///////////////////////////////////////////////////////////////////////////////////	
			//		gWndCtrl.Status.ActState[AcsIndex] := gAcs380Ctrl.Status.ActState[AcsIndex];
			//		gAcs380Ctrl.Command.PowerOff[AcsIndex] := gWndCtrl.Command.PowerOff[AcsIndex];
			//		gAcs380Ctrl.Command.Move[AcsIndex] := gWndCtrl.Command.Move[AcsIndex];
		
			//---------------------------------ipis0508 for Plate---------------------------------------------------//
			//		gMachineOut.ExtrD_On	:=gMachineOut.MotionPower;			
			//		IF 0=gMachineOut.MotionPower THEN
			//			gExtruderD.bKeyOff :=1;
			//		ELSE
			//			gExtruderD.bKeyOff :=0;
			//		END_IF;
		
		
			//****************************************************************************************************//
			gAcs380Ctrl.Command.PowerOn[0]:=	gMachineOut.ExtrA_On; //ExtruderA 
			gAcs380Ctrl.Command.PowerOn[1]:=    gMachineOut.ExtrC_On; //ExtruderB
			gAcs380Ctrl.Command.PowerOn[2]:=    gMachineOut.ExtrB_On; //ExtruderC		
			gAcs380Ctrl.Command.PowerOn[3]:=    gMachineOut.ExtrD_On; //ExtruderD= Plate follow MotionPower
			gAcs380Ctrl.Command.PowerOn[4]:=    gMachineOut.ExtrVisu_On; //ExtruderVisu	
				
			//****************************************************************************************************//		
		
			gAcs380Ctrl.Command.PowerOff[0] := 	gExtruderA.bKeyOff; //ExtruderA
			gAcs380Ctrl.Command.PowerOff[1] := 	gExtruderC.bKeyOff; //ExtruderC
			gAcs380Ctrl.Command.PowerOff[2] := 	gExtruderB.bKeyOff; //ExtruderB
			gAcs380Ctrl.Command.PowerOff[3] := 	gExtruderD.bKeyOff; //ExtruderD
			gAcs380Ctrl.Command.PowerOff[4] := 	gExtruderVisu.bKeyOff; //ExtruderVisu	
			//****************************************************************************************************//
	
			//*********** ExtruderD= Plate follow MotionPower **************************************************//
			//		gMachineOut.ExtrD_On	:=gMachineOut.MotionPower;			
			//		IF 0=gMachineOut.MotionPower THEN
			//			gExtruderD.bKeyOff :=1;
			//		ELSE
			//			gExtruderD.bKeyOff :=0;
			//		END_IF;
			//****************************************************************************************************//	
			
		
//			IF gEnableAll_HMI THEN
//	
//				IF 1 = gMachineFix.ExtruderA.Use THEN
//					gExtruderHMI_Enable[0]:=1;
//				END_IF;
//				IF 1 = gMachineFix.ExtruderB.Use THEN
//					gExtruderHMI_Enable[1]:=1;
//				END_IF;
//				IF 1 = gMachineFix.ExtruderC.Use THEN
//					gExtruderHMI_Enable[2]:=1;
//				END_IF;
//				IF 1 = gMachineFix.ExtruderD.Use THEN
//					gExtruderHMI_Enable[3]:=1;
//				END_IF;
//				IF 1 = gMachineFix.ExtruderVisu.Use THEN
//					gExtruderHMI_Enable[4]:=1;
//				END_IF;
//			
//				gEnableAll_HMI :=0;	
//			END_IF;
//						
//			IF gDisableAll_HMI 
//				OR 1 = gMachineIn.ExtrA_MotorErr OR 1 = gMachineIn.ExtrB_MotorErr 
//				OR 1 = gMachineIn.ExtrC_MotorErr OR 1 = gMachineIn.ExtrV_MotorErr  THEN //ipis0323
//				//	gExtruderHMI_Enable[AcsIndex]:=0;
//				gExtruderHMI_Enable[0]:=0;
//				gExtruderHMI_Enable[1]:=0;	
//				gExtruderHMI_Enable[2]:=0;
//				gExtruderHMI_Enable[3]:=0;	
//				gExtruderHMI_Enable[4]:=0;
//			
//				gDisableAll_HMI :=0;
//			END_IF;	
			//****************************************************************************************************//		
				
		
			gAcs380Ctrl.Command.Move[AcsIndex] := gExtruderHMI_Enable[AcsIndex];
	
		
			IF 0= AcsIndex  THEN //ExtruderA 
				IF 1=Extruder_Use[AcsIndex] THEN		
					// error
					IF gAcs380Ctrl.Status.FaultNum[AcsIndex] <>0 THEN  
						gMachineIn.ExtrA_MotorErr :=1;
					ELSE
						gMachineIn.ExtrA_MotorErr :=0;
					END_IF;	
				
					// Imply power not on
					IF 0=gAcs380Ctrl.Command.PowerOn[AcsIndex] THEN  			
						IF 1=gExtruderHMI_Enable[AcsIndex] THEN 
							gImply.ExtruderA_NotOn := 1;
							gExtruderHMI_Enable[AcsIndex] :=0;
						END_IF;	
					END_IF;		
				
					// SetSpd condition
					IF (1=gExtruderHMI_Enable[AcsIndex]) AND (STATE_RUN =Step[AcsIndex]) AND (1= gAcs380Ctrl.Command.PowerOn[AcsIndex])  THEN
						gAcs380Ctrl.Command.SetSpd[AcsIndex] :=REAL_TO_INT( INT_TO_REAL((gMachinePara.ExtruderA.SetSpeed))*(20000/gMachineFix.ExtruderA.RPM10v)); //rpm //abb unit = speed : 20000  0 //ipis0718
					ELSE
						gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					END_IF;		
				
					// motor speed
					gExtruderA.ActRPM := gAcs380Ctrl.Status.ActSpd[AcsIndex]*(gMachineFix.ExtruderA.RPM10v/20000); 
			
					//Screw speed
					IF 0=gMachineFix.ExtruderA.Ratio THEN
						gMachineFix.ExtruderA.Ratio :=1;
					ELSE
						gExtruderA.ActSpeed := gExtruderA.ActRPM /gMachineFix.ExtruderA.Ratio;
					END_IF;
				
					//motor Current Frequency
					gExtruderA.Current_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActCurrent[AcsIndex])/100;
					gExtruderA.Frequency_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActFrequency[AcsIndex])/100;
					
				ELSE		
					gExtruderHMI_Enable[AcsIndex]:=0;
					gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;
					gMachineIn.ExtrA_MotorErr :=0;
				END_IF;
			END_IF;
		
			IF 1= AcsIndex  THEN //ExtruderB 
				IF 1=Extruder_Use[AcsIndex]  THEN
					// error
					IF gAcs380Ctrl.Status.FaultNum[AcsIndex] <>0 THEN 
						gMachineIn.ExtrB_MotorErr :=1;
					ELSE
						gMachineIn.ExtrB_MotorErr :=0;
					END_IF;
				
					// Imply power not on
					IF 0=gAcs380Ctrl.Command.PowerOn[AcsIndex] THEN  			
						IF 1=gExtruderHMI_Enable[AcsIndex] THEN 
							gImply.ExtruderB_NotOn := 1;
							gExtruderHMI_Enable[AcsIndex] :=0;
						END_IF;	
					END_IF;
				
					// SetSpd condition
					IF (1=gExtruderHMI_Enable[AcsIndex]) AND (STATE_RUN =Step[AcsIndex]) AND (1= gAcs380Ctrl.Command.PowerOn[AcsIndex])  THEN
						gAcs380Ctrl.Command.SetSpd[AcsIndex] :=REAL_TO_INT( INT_TO_REAL((gMachinePara.ExtruderB.SetSpeed))*(20000/gMachineFix.ExtruderB.RPM10v)); //rpm //abb unit = speed : 20000  0 //ipis0718		
					ELSE
						gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					END_IF;
				
					// motor speed
					gExtruderB.ActRPM := gAcs380Ctrl.Status.ActSpd[AcsIndex]*(gMachineFix.ExtruderB.RPM10v/20000); //ipis0620
		
					//Screw speed
					IF 0=gMachineFix.ExtruderB.Ratio THEN
						gMachineFix.ExtruderB.Ratio :=1;
					ELSE
						gExtruderB.ActSpeed := gExtruderB.ActRPM /gMachineFix.ExtruderB.Ratio;
					END_IF;
		
					//motor Current Frequency
					gExtruderB.Current_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActCurrent[AcsIndex])/100;
					gExtruderB.Frequency_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActFrequency[AcsIndex])/100;
						
				ELSE
					gExtruderHMI_Enable[AcsIndex]:=0;
					gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;
					gMachineIn.ExtrB_MotorErr :=0;
				END_IF;	
			END_IF;
		
		
			IF 2= AcsIndex THEN //ExtruderC 
				IF 1=Extruder_Use[AcsIndex] THEN
					// error
					IF gAcs380Ctrl.Status.FaultNum[AcsIndex] <>0 THEN 
						gMachineIn.ExtrC_MotorErr :=1;
					ELSE
						gMachineIn.ExtrC_MotorErr :=0;
					END_IF;
				
					// Imply power not on
					IF 0=gAcs380Ctrl.Command.PowerOn[AcsIndex] THEN  			
						IF 1=gExtruderHMI_Enable[AcsIndex] THEN 
							gImply.ExtruderC_NotOn := 1;
							gExtruderHMI_Enable[AcsIndex] :=0;
						END_IF;	
					END_IF;
				
					// SetSpd condition
					IF (1=gExtruderHMI_Enable[AcsIndex]) AND (STATE_RUN =Step[AcsIndex]) AND (1= gAcs380Ctrl.Command.PowerOn[AcsIndex])  THEN
						gAcs380Ctrl.Command.SetSpd[AcsIndex] :=REAL_TO_INT( INT_TO_REAL((gMachinePara.ExtruderC.SetSpeed))*(20000/gMachineFix.ExtruderC.RPM10v)); //rpm //abb unit = speed : 20000  0 //ipis0718
					ELSE
						gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					END_IF;
				
					// motor speed
					gExtruderC.ActRPM := gAcs380Ctrl.Status.ActSpd[AcsIndex]*(gMachineFix.ExtruderC.RPM10v/20000); //ipis0620
		
					//Screw speed
					IF 0=gMachineFix.ExtruderC.Ratio THEN
						gMachineFix.ExtruderC.Ratio :=1;
					ELSE
						gExtruderC.ActSpeed := gExtruderC.ActRPM /gMachineFix.ExtruderC.Ratio;	
					END_IF;

					//motor Current Frequency
					gExtruderC.Current_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActCurrent[AcsIndex])/100;
					gExtruderC.Frequency_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActFrequency[AcsIndex])/100;
				ELSE
					gExtruderHMI_Enable[AcsIndex]:=0;
					gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;
					gMachineIn.ExtrC_MotorErr :=0;
				END_IF;
			END_IF;
			
			IF 3= AcsIndex THEN //ExtruderD 
					
				IF 1=Extruder_Use[AcsIndex]  THEN
													
					// error
					IF gAcs380Ctrl.Status.FaultNum[AcsIndex] <>0 THEN 
						gMachineIn.ExtrD_MotorErr :=1;
					ELSE
						gMachineIn.ExtrD_MotorErr :=0;
					END_IF;
				
					// Imply power not on
					IF 0=gAcs380Ctrl.Command.PowerOn[AcsIndex] THEN  			
						IF 1=gExtruderHMI_Enable[AcsIndex] THEN 
							gImply.ExtruderD_NotOn := 1;
							gExtruderHMI_Enable[AcsIndex] :=0;
						END_IF;	
					END_IF;
				
				
					// SetSpd condition	
					IF (1=gExtruderHMI_Enable[AcsIndex]) AND (STATE_RUN =Step[AcsIndex]) AND (1= gAcs380Ctrl.Command.PowerOn[AcsIndex])  THEN
						gAcs380Ctrl.Command.SetSpd[AcsIndex] :=REAL_TO_INT( INT_TO_REAL((gMachinePara.ExtruderD.SetSpeed))*(20000/gMachineFix.ExtruderD.RPM10v)); //rpm //abb unit = speed : 20000  0 //ipis0718
					ELSE
						gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					END_IF;	

					
					// motor speed
					gExtruderD.ActRPM := gAcs380Ctrl.Status.ActSpd[AcsIndex]*(gMachineFix.ExtruderD.RPM10v/20000); //ipis0724

					//Screw speed
					IF 0=gMachineFix.ExtruderD.Ratio THEN
						gMachineFix.ExtruderD.Ratio :=1;
					ELSE
						gExtruderD.ActSpeed := gExtruderD.ActRPM /gMachineFix.ExtruderD.Ratio;
					END_IF;

					//motor Current Frequency
					gExtruderD.Current_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActCurrent[AcsIndex])/100;
					gExtruderD.Frequency_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActFrequency[AcsIndex])/100;
					
				ELSE
					gExtruderHMI_Enable[AcsIndex]:=0;
					gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;
					gMachineIn.ExtrD_MotorErr :=0;
				END_IF;	
			END_IF;
		
		
			IF 4= AcsIndex THEN //ExtruderVisu 
		
				IF 1=Extruder_Use[AcsIndex]  THEN
					// error
					IF gAcs380Ctrl.Status.FaultNum[AcsIndex] <>0 THEN 
						gMachineIn.ExtrV_MotorErr :=1;
					ELSE
						gMachineIn.ExtrV_MotorErr :=0;
					END_IF;
					// Imply power not on
					IF 0=gAcs380Ctrl.Command.PowerOn[AcsIndex] THEN  			
						IF 1=gExtruderHMI_Enable[AcsIndex] THEN 
							gImply.ExtruderV_NotOn := 1;
							gExtruderHMI_Enable[AcsIndex] :=0;
						END_IF;	
					END_IF;
				
					IF (1=gExtruderHMI_Enable[AcsIndex]) AND (STATE_RUN =Step[AcsIndex]) AND (1= gAcs380Ctrl.Command.PowerOn[AcsIndex])  THEN
						gAcs380Ctrl.Command.SetSpd[AcsIndex] :=REAL_TO_INT( INT_TO_REAL((gMachinePara.ExtruderVisu.SetSpeed))*(20000/gMachineFix.ExtruderVisu.RPM10v)); //rpm //abb unit = speed : 20000  0 //ipis0718
					ELSE
						gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					END_IF;
				
					// motor speed
					gExtruderVisu.ActRPM := gAcs380Ctrl.Status.ActSpd[AcsIndex]*(gMachineFix.ExtruderVisu.RPM10v/20000); //ipis0620
				
					//Screw speed
					IF 0=gMachineFix.ExtruderVisu.Ratio THEN
						gMachineFix.ExtruderVisu.Ratio :=1;
					ELSE
						gExtruderVisu.ActSpeed := gExtruderVisu.ActRPM /gMachineFix.ExtruderVisu.Ratio;
					END_IF;
				
					//motor Current Frequency			
					gExtruderVisu.Current_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActCurrent[AcsIndex])/100;
					gExtruderVisu.Frequency_ABB := DINT_TO_REAL(gAcs380Ctrl.Status.ActFrequency[AcsIndex])/100;
	
				ELSE
					gExtruderHMI_Enable[AcsIndex]:=0;
					gAcs380Ctrl.Command.SetSpd[AcsIndex] := 0;	
					gMachineIn.ExtrV_MotorErr :=0;
				END_IF;	
			END_IF;
		
			IF (0= gAcs380Ctrl.Command.PowerOn[AcsIndex]) THEN
				Step[AcsIndex] :=STATE_ESTOP;
			END_IF;
				
		
			/////////////////////////////////////////////////////////////////////////////////////////
			CASE Step[AcsIndex] OF
				STATE_ESTOP:
				
					IF (1=gAcs380Ctrl.Command.PowerOn[AcsIndex] AND gAcs380Ctrl.Status.ModuleOk[AcsIndex] ) THEN // ipis0513 
						IF EstopCon[AcsIndex] = 16#0280 THEN
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;
						ELSIF StopCon[AcsIndex] = 16#0231 THEN
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0407;
						END_IF;
			
						//	IF ReadySwitchOnCon[AcsIndex] = 16#0233 THEN 
						Step[AcsIndex] := STATE_WAIT_CMD;
						//	END_IF;
			
						IF FaultFlg[AcsIndex] THEN
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406; // 20190114
							Step[AcsIndex] := STATE_FAULT_RST;
						END_IF								
					END_IF;
				
			
				STATE_WAIT_CMD:
					IF gAcs380Ctrl.Command.AlmReset[AcsIndex] THEN      // 20190128
						Step[AcsIndex] := STATE_FAULT_RST;
					ELSIF gAcs380Ctrl.Command.PowerOff[AcsIndex] THEN
						gAcs380Ctrl.Command.PowerOff[AcsIndex] := 0;
						Step[AcsIndex] := STATE_POWER_OFF;
					ELSIF gAcs380Ctrl.Command.Move[AcsIndex] THEN
						gAcs380Ctrl.Command.Move[AcsIndex] := 0;
						Step[AcsIndex] := STATE_RUN;
					END_IF

					IF FaultFlg[AcsIndex] THEN
						gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;	// 20190114
						Step[AcsIndex] := STATE_FAULT_RST;
					END_IF

				STATE_POWER_OFF:
					gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0407;
					IF ReadySwitchOnCon[AcsIndex] = 16#0233 THEN
						Step[AcsIndex] := STATE_WAIT_CMD;
					END_IF
					IF FaultFlg[AcsIndex] THEN
						gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;  // 20190114
						Step[AcsIndex] := STATE_FAULT_RST;
					END_IF

				STATE_FAULT_RST:
					IF  gAcs380Ctrl.Status.ModuleOk[AcsIndex] = 1 THEN
						IF BIT_TST(TempETAD[AcsIndex],ALARM_BIT) THEN
							IF gAcs380Ctrl.Command.AlmReset[AcsIndex] THEN
								gAcs380Ctrl.Command.AlmReset[AcsIndex] := 0;
								gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0080;
								Step[AcsIndex] := STATE_FAULT_RST_WAIT;
								RstTimer[AcsIndex] := 0;
							END_IF 
						ELSE
							gAcs380Ctrl.Command.AlmReset[AcsIndex] := 0;
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406; // 20190114
							Step[AcsIndex] := STATE_ESTOP;
						END_IF
					END_IF

				STATE_FAULT_RST_WAIT:
					RstTimer[AcsIndex] := RstTimer[AcsIndex] + CycTime;
					IF (RstTimer[AcsIndex] >= 1500) THEN
						RstTimer[AcsIndex] := 0;
						gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;    // 20190114
						Step[AcsIndex] := STATE_ESTOP;
					END_IF
	
				STATE_RUN:
					IF ReadySwitchOnCon[AcsIndex] = 16#0233 THEN
						gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#040F;
					ELSIF SwitchOnCon[AcsIndex] = 16#1337 OR SwitchOnCon[AcsIndex] = 16#1237 THEN
						IF gAcs380Ctrl.Command.TorMode[AcsIndex] THEN//×ª¾àÄ£Ê½
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0C7F;
						ELSE
							gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#047F;
						END_IF;	
					END_IF
				
					IF gAcs380Ctrl.Command.PowerOff[AcsIndex] THEN
						gAcs380Ctrl.Command.PowerOff[AcsIndex] := 0;
						Step[AcsIndex] := STATE_POWER_OFF;
					END_IF	

					IF FaultFlg[AcsIndex] THEN
						gAcs380Ctrl.Command.CMDD[AcsIndex] := 16#0406;    // 20190114
						Step[AcsIndex] := STATE_FAULT_RST;
					END_IF
						
			END_CASE
		

			IF FaultFlg[AcsIndex] <> 0 THEN
				gAcs380Ctrl.Status.StateIndex[AcsIndex] := 3;//´íÎó
			END_IF;	
			IF ReadySwitchOnCon[AcsIndex] = 16#0233 OR EstopCon[AcsIndex] = 16#0280 THEN
				gAcs380Ctrl.Status.StateIndex[AcsIndex] := 0;//¶Ïµç
			END_IF;		
			IF SwitchOnCon[AcsIndex] = 16#1337 OR SwitchOnCon[AcsIndex] = 16#1237 OR SwitchOnCon[AcsIndex] = 16#1B37 THEN
				IF gAcs380Ctrl.Status.ActSpd[AcsIndex] <> 0 THEN
					gAcs380Ctrl.Status.StateIndex[AcsIndex] := 2;//ÔË¶¯
				ELSE
					gAcs380Ctrl.Status.StateIndex[AcsIndex] := 1;//Í£Ö¹
				END_IF;
			END_IF;	
		

			ActTorque[AcsIndex] 	:= INT_TO_REAL(gAcs380Ctrl.Status.ActTorque[AcsIndex]) / 100;
								
		END_FOR

	END_IF
	
		

	
END_PROGRAM
