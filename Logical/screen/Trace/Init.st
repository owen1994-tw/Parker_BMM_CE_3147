

PROGRAM _INIT
	(* Insert code here *)
	Trace.uDIndex := MAX_DATA;  // VC4 trend bug for cursor
	

	(******Trend Option******)
	memset(ADR(uOptMotionTrend), 2, SIZEOF(uOptMotionTrend));
	uOptMotionTrend[0]  	:= 0;//RClamp
//	uOptMotionTrend[1]  	:= 0;//LClamp
	uOptMotionTrend[2]  	:= 0;//RCarriage
//	uOptMotionTrend[3]  	:= 0;//LCarriage
	uOptMotionTrend[4]  	:= 0;//RBlowpin1
//	uOptMotionTrend[5]  	:= 0;//LBlowpin1
	uOptMotionTrend[6]  	:= 0;//RRobotTransfer
//	uOptMotionTrend[7]  	:= 0;//LRobotTransfer
	uOptMotionTrend[10]  	:= 0;//Extrlift
	uOptMotionTrend[11]  	:= 0;//Parison
//	uOptMotionTrend[14]  	:= 0;//RBlowpin2
	//	uOptMotionTrend[15]  	:= 0;//LBlowpin2
	
	
	
	
	//Max Torque Set
	gMotorTrace.RMold.Clamp.Max_Torque_Set := 104;
	gMotorTrace.RMold.Carriage.Max_Torque_Set := 31;
	gMotorTrace.RMold.BlowPin.Max_Torque_Set := 38.7;
	gMotorTrace.Extrlift.Max_Torque_Set := 3.3;
	gMotorTrace.Parison.Max_Torque_Set := 12.6;
	
END_PROGRAM