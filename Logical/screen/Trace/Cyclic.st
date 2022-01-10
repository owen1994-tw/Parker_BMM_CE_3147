(*********************************************************************************
 * Copyright: Bernecker+Rainer
 * Author:    ho2c
 * Created:   September 4, 2019/9:59 AM 
 *********************************************************************************)

PROGRAM _CYCLIC
	(* Insert code here *)
	//----------------------
	//ÐÞ¸Ä´ËÌŽ
	//----------------------
	//Data -> Struct
	
//	rData[0]	:= TraceData.rPosition;
//	rData[1]	:= TraceData.rSpeed;
//	rData[2]	:= TraceData.rTorque;
//	rData[3]	:= TraceData.rCurrent;
	
	IF 0=uMotorIndex THEN //RClamp
		rData[0]	:= gMotorTrace.RMold.Clamp.Position;
		rData[1]	:= gMotorTrace.RMold.Clamp.RPM;
		rData[2]	:= gMotorTrace.RMold.Clamp.Current;
		rData[3]	:= gMotorTrace.RMold.Clamp.Torque;
		Max_Torque	:= gMotorTrace.RMold.Clamp.Max_Torque_Set;
	END_IF;
	
	IF 1=uMotorIndex THEN //LClamp
		rData[0]	:= gMotorTrace.LMold.Clamp.Position;
		rData[1]	:= gMotorTrace.LMold.Clamp.RPM;
		rData[2]	:= gMotorTrace.LMold.Clamp.Current;
		rData[3]	:= gMotorTrace.LMold.Clamp.Torque;
		Max_Torque	:= gMotorTrace.LMold.Clamp.Max_Torque_Set;
	END_IF;
	
	IF 2=uMotorIndex THEN // RCarriage	
		rData[0]	:= gMotorTrace.RMold.Carriage.Position;
		rData[1]	:= gMotorTrace.RMold.Carriage.RPM;
		rData[2]	:= gMotorTrace.RMold.Carriage.Current;
		rData[3]	:= gMotorTrace.RMold.Carriage.Torque;
		Max_Torque	:= gMotorTrace.RMold.Carriage.Max_Torque_Set;
	END_IF;
	
	IF 3=uMotorIndex THEN // LCarriage
		rData[0]	:= gMotorTrace.LMold.Carriage.Position;
		rData[1]	:= gMotorTrace.LMold.Carriage.RPM;
		rData[2]	:= gMotorTrace.LMold.Carriage.Current;
		rData[3]	:= gMotorTrace.LMold.Carriage.Torque;
		Max_Torque	:= gMotorTrace.LMold.Carriage.Max_Torque_Set;
	END_IF;
	
	IF 4=uMotorIndex THEN // RBlowpin1	
		rData[0]	:= gMotorTrace.RMold.BlowPin.Position;
		rData[1]	:= gMotorTrace.RMold.BlowPin.RPM;
		rData[2]	:= gMotorTrace.RMold.BlowPin.Current;
		rData[3]	:= gMotorTrace.RMold.BlowPin.Torque;
		Max_Torque	:= gMotorTrace.RMold.BlowPin.Max_Torque_Set;
	END_IF;
	
	IF 5=uMotorIndex THEN // LBlowpin1	
		rData[0]	:= gMotorTrace.LMold.BlowPin.Position;
		rData[1]	:= gMotorTrace.LMold.BlowPin.RPM;
		rData[2]	:= gMotorTrace.LMold.BlowPin.Current;
		rData[3]	:= gMotorTrace.LMold.BlowPin.Torque;	
		Max_Torque	:= gMotorTrace.LMold.BlowPin.Max_Torque_Set;
	END_IF;
	
	IF 6=uMotorIndex THEN // RRobotTransfer	
		rData[0]	:= gMotorTrace.RMold.RobotTransfer.Position;
		rData[1]	:= gMotorTrace.RMold.RobotTransfer.RPM;
		rData[2]	:= gMotorTrace.RMold.RobotTransfer.Current;
		rData[3]	:= gMotorTrace.RMold.RobotTransfer.Torque;	
		Max_Torque	:= gMotorTrace.RMold.RobotTransfer.Max_Torque_Set;
	END_IF;
	
	IF 7=uMotorIndex THEN // LRobotTransfer	
		rData[0]	:= gMotorTrace.LMold.RobotTransfer.Position;
		rData[1]	:= gMotorTrace.LMold.RobotTransfer.RPM;
		rData[2]	:= gMotorTrace.LMold.RobotTransfer.Current;
		rData[3]	:= gMotorTrace.LMold.RobotTransfer.Torque;	
		Max_Torque	:= gMotorTrace.LMold.RobotTransfer.Max_Torque_Set;
	END_IF;
	
	IF 10=uMotorIndex THEN // Extrlift	
		rData[0]	:= gMotorTrace.Extrlift.Position;
		rData[1]	:= gMotorTrace.Extrlift.RPM;
		rData[2]	:= gMotorTrace.Extrlift.Current;
		rData[3]	:= gMotorTrace.Extrlift.Torque;	
		Max_Torque	:= gMotorTrace.Extrlift.Max_Torque_Set;
	END_IF;
	
	IF 11=uMotorIndex THEN // Parison	
		rData[0]	:= gMotorTrace.Parison.Position;
		rData[1]	:= gMotorTrace.Parison.RPM;
		rData[2]	:= gMotorTrace.Parison.Current;
		rData[3]	:= gMotorTrace.Parison.Torque;		
		Max_Torque	:= gMotorTrace.Parison.Max_Torque_Set;
	END_IF;
	
//	IF 14=uMotorIndex THEN // RBlowpin2	
//		rData[0]	:= gMotorTrace.RMold.BlowPin2.Position;
//		rData[1]	:= gMotorTrace.RMold.BlowPin2.RPM;
//		rData[2]	:= gMotorTrace.RMold.BlowPin2.Current;
//		rData[3]	:= gMotorTrace.RMold.BlowPin2.Torque;
//	END_IF;
//	
//	IF 15=uMotorIndex THEN //LBlowpin2
//		rData[0]	:= gMotorTrace.LMold.BlowPin2.Position;
//		rData[1]	:= gMotorTrace.LMold.BlowPin2.RPM;
//		rData[2]	:= gMotorTrace.LMold.BlowPin2.Current;
//		rData[3]	:= gMotorTrace.LMold.BlowPin2.Torque;			
//	END_IF;
	
	//	
//	IF 6=uMotorIndex THEN //ExtruderVisu
//		rData[0]	:= gExtruderB.ActSpeed;
//		rData[1]	:= gAcs380Ctrl.Status.ActCurrent[1]/100;
//		rData[2]	:= gAcs380Ctrl.Status.ActTorque[1]/100;
//		//	rData[3]	:= ;		
//	END_IF;
	
	
	
	//----------------------------------
	//----------------------------------
	//Cursor
	Trace.Config.rCursorMove	:= Trace.Config.rHMICursorMove / 1000;
	//----------------------
	//Main
	CASE Trace.uStep OF
		0://INIT
			IF Trace.Cmd.bEnable THEN
				Trace.uDIndex	:= 0;
				Trace.uStep		:= 20;
				memset(ADR(Trace.Data),0,SIZEOF(Trace.Data)); //ipis0226
				//				FOR Trace.uTraceIndex := 0 TO MAX_NUM DO
				//					
//				END_FOR;
			END_IF;
		20://Data -> Trend
			IF Trace.uDIndex < MAX_DATA THEN
				FOR Trace.uTraceIndex := 0 TO MAX_NUM DO
					Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex]	:= rData[Trace.uTraceIndex];
					//YMax
					IF Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex] >= Trace.Data[Trace.uTraceIndex].rYDMax THEN
						Trace.Data[Trace.uTraceIndex].rYMax := Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex] * 1.1;
						Trace.Data[Trace.uTraceIndex].rYDMax := Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex];
					END_IF;
					//YMin
					IF Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex] <= Trace.Data[Trace.uTraceIndex].rYDMin THEN
						Trace.Data[Trace.uTraceIndex].rYMin := Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex] * 1.1;
						Trace.Data[Trace.uTraceIndex].rYDMin := Trace.Data[Trace.uTraceIndex].rData[Trace.uDIndex];
					END_IF;
				END_FOR;
				Trace.uDIndex := Trace.uDIndex + 1;
			ELSE
				Trace.Cmd.bEnable	:= FALSE;
				Trace.uStep			:= 0;
			END_IF;
	END_CASE;
	//----------------------
END_PROGRAM
