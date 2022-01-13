(********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * File: Global.typ
 * Author: lij
 * Created: December 19, 2013
 ********************************************************************
 * Global data types of project BMM
 ********************************************************************)
(*ABB*)

TYPE
	Acs380Cmd_type : 	STRUCT 
		SetTor : ARRAY[0..NUM_ACS380_MINUS_ONE]OF INT;
		LimitSpdMin : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		LimitSpdMax : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		SetSpd : ARRAY[0..NUM_ACS380_MINUS_ONE]OF INT;
		PowerOff : ARRAY[0..NUM_ACS380_MINUS_ONE]OF BOOL;
		AlmReset : ARRAY[0..NUM_ACS380_MINUS_ONE]OF BOOL;
		Move : ARRAY[0..NUM_ACS380_MINUS_ONE]OF BOOL;
		CMDD : ARRAY[0..NUM_ACS380_MINUS_ONE]OF UINT;
		TorMode : ARRAY[0..NUM_ACS380_MINUS_ONE]OF BOOL;
		PowerOn : ARRAY[0..NUM_ACS380_MINUS_ONE]OF USINT;
	END_STRUCT;
	Acs380Ctl_type : 	STRUCT 
		Command : Acs380Cmd_type;
		Status : Acs380Status_type;
	END_STRUCT;
	Acs380Status_type : 	STRUCT 
		ModuleOk : ARRAY[0..NUM_ACS380_MINUS_ONE]OF BOOL;
		ActState : ARRAY[0..NUM_ACS380_MINUS_ONE]OF UINT;
		ActPosition : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		ETAD : ARRAY[0..NUM_ACS380_MINUS_ONE]OF UINT;
		FaultNum : ARRAY[0..NUM_ACS380_MINUS_ONE]OF UINT;
		StateIndex : ARRAY[0..NUM_ACS380_MINUS_ONE]OF USINT;
		ActSpd : ARRAY[0..NUM_ACS380_MINUS_ONE]OF INT;
		ActTorque : ARRAY[0..NUM_ACS380_MINUS_ONE]OF INT;
		PowerOnFlag : ARRAY[0..NUM_ACS380_MINUS_ONE]OF USINT;
		ActFrequency : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		ActCurrent : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		ActDCVoltage : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
		ActPower : ARRAY[0..NUM_ACS380_MINUS_ONE]OF DINT;
	END_STRUCT;
	Action_Limit_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		bHomeOk : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		Out : UINT;
	END_STRUCT;
	Action_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		bHomeOk : USINT;
		Timer : timer_typ;
		Out : UINT;
	END_STRUCT;
END_TYPE

(*action information type*)

TYPE
	Act_Info_typ : 	STRUCT 
		ClampCls : USINT;
		ClampOpn : USINT;
		BlowPinUp : USINT;
		BlowPinDn : USINT;
		BottomPinUp : USINT;
		BottomPinDn : USINT;
		CarriageFw : USINT;
		CarriageBw : USINT;
		LowBlow : USINT;
		HighBlow : USINT;
		SupportAir : USINT;
		SuckAir : USINT;
		TopDeflashFw : USINT;
		TopDeflashBw : USINT;
		TopDeflashPuncher : USINT;
		BottomDeflashFw : USINT;
		BottomDeflashBw : USINT;
		BottomDeflash : USINT;
		Reserve : USINT;
		ExtrLiftUp : USINT;
		ExtrLiftDn : USINT;
		CoolPinDn : USINT;
		CoolPinBlow : USINT;
		CoolPin2Dn : USINT;
		CoolPin2Blow : USINT;
		CoolPin3Dn : USINT;
		CoolPin3Blow : USINT;
		RobotFw : USINT;
		RobotBw : USINT;
		RobotUp : USINT;
		RobotDn : USINT;
		RobotOpn : USINT;
		RobotCls : USINT;
		Cutter : USINT;
		BlowDeflash : USINT;
		CoolDeflash : USINT;
		PullBottom : USINT;
		SubMoldOpn : USINT;
		SubMoldCls : USINT;
		ScrewFw : USINT;
		ScrewBw : USINT;
		PinPartFw : USINT;
		PinPartBw : USINT;
		InnerPin : USINT;
		CutNeckCls : USINT;
		CutNeck : USINT;
		ChuteDeflashBlow : USINT;
		Sealer : USINT;
		BottomSubMoldOpn : USINT;
		BottomSubMoldCls : USINT;
		HookPinFw : USINT;
	END_STRUCT;
	Alarm_typ : 	STRUCT 
		batteryLow : USINT; (*0*)
		ModuleErr : USINT; (*1*)
		SaftyDoor : USINT;
		EmergencyStop : USINT;
		ScrewTempLow : USINT;
		ScrewTempHigh : USINT;
		ThermocoupleBroken : USINT;
		TempException : USINT;
		OilPumpErr : USINT;
		ParisonPumpErr : USINT;
		VisuExtrMotorOverload : USINT; (*10*)
		A_ExtruderMotorOverload : USINT;
		B_ExtruderMotorOverload : USINT;
		HeatingException : USINT;
		ParisonLinearBreak : USINT;
		ParisonLinearCalErr : USINT;
		ParisonValuebeyond : USINT;
		OilPumpFilter : USINT;
		ParisonPumpFilter : USINT;
		CoolingPumpFilter : USINT;
		TonnageFilter : USINT; (*20*)
		LAlarm : Mold_Alarm_typ; (*21~110*)
		RAlarm : Mold_Alarm_typ; (*111~200*)
		LCycleTime : USINT; (*201*)
		RCycleTime : USINT;
		OilPosition : USINT;
		reserve02 : USINT;
		LubrTimeZero : USINT;
		reserve03 : USINT;
		OilTempHigh : USINT;
		OilPumpOff : USINT;
		LCarriageLinearBreak : USINT;
		RCarriageLinearBreak : USINT; (*210*)
		LClampLinearBreak : USINT;
		RClampLinearBreak : USINT;
		LowSafeValvePumpOff : USINT;
		HighSafeValvePumpOff : USINT;
		LubriFilter : USINT;
		LubriOverload : USINT;
		VisuExtrCoolOverload : USINT;
		CoolingPumpErr : USINT;
		AccuSensorError : USINT;
		AccuChargeError : USINT; (*220*)
		PneuForceNormal : USINT;
		CutterHeatNormal : USINT;
		AccuRelieveError : USINT;
		ExtrAMeltP_Warn : USINT;
		ExtrAMeltP_Alarm : USINT;
		ExtrBMeltP_Warn : USINT;
		ExtrBMeltP_Alarm : USINT;
		ExtrACurrentWarn : USINT;
		ExtrACurrentAlarm : USINT;
		ExtrBCurrentWarn : USINT; (*230*)
		ExtrBCurrentAlarm : USINT;
		ExtrA_NotOn : USINT;
		ExtrB_NotOn : USINT;
		ExtrV_NotOn : USINT;
		LengthTimeout : USINT;
		HeatingPowerNormal : USINT;
		A_ExtrLubricCheck : USINT;
		B_ExtrLubricCheck : USINT;
		ExtruderNormal : USINT;
		Voltage24vNormal : USINT; (*240*)
		L_LubricNormal : USINT;
		R_LubricNormal : USINT;
		PresEnaOffHydHigh : USINT;
		HeatingNormal : USINT;
		ServoTransferErrL : USINT;
		ServoUpDnErrL : USINT;
		ServoTransferErrR : USINT;
		ServoUpDnErrR : USINT;
		CarriageLockL : USINT;
		CarriageLockR : USINT; (*250*)
		ServoTransProtectL : USINT;
		ServoUpDnProtectL : USINT;
		ServoTransProtectR : USINT;
		ServoUpDnProtectR : USINT;
		RecMoldLengthErr : USINT;
		RecFixLengthErr : USINT;
		RecCurveLengthErr : USINT;
		ParisonPumpNotOn : USINT;
		HeatingNotOn : USINT;
		SwingInvertErr : USINT; (*260*)
		LowerLSPC : SPC_Mold_Ctrl_typ; (*261~*)
		UpperLSPC : SPC_Mold_Ctrl_typ; (*~276*)
		LowerRSPC : SPC_Mold_Ctrl_typ; (*277~*)
		UpperRSPC : SPC_Mold_Ctrl_typ; (*~292*)
		A_ExtrMotorTempAlarm : USINT; (*293*)
		A_ExtrMotorTempStop : USINT;
		A_ColiTempNormal : USINT;
		B_ExtrMotorTempAlarm : USINT;
		B_ExtrMotorTempStop : USINT;
		B_ColiTempNormal : USINT;
		V_ExtrMotorTempAlarm : USINT;
		V_ExtrMotorTempStop : USINT; (*300*)
		V_ColiTempNormal : USINT;
		ParisonNetworkErr : USINT;
		ParisonAxisErr : USINT;
		AccuCharge_ExtrliftError : USINT; (*304*)
		LHwAlarm : Mold_Hw_typ; (*305~354*)
		RHwAlarm : Mold_Hw_typ; (*355~404*)
		A_ExtrCoolFault : USINT; (*405*)
		B_ExtrCoolFault : USINT;
		C_ExtrCoolFault : USINT;
		D_ExtrCoolFault : USINT;
		E_ExtrCoolFault : USINT;
		V_ExtrCoolFault : USINT; (*410*)
		A_BarrelCoolFault : USINT;
		B_BarrelCoolFault : USINT;
		C_BarrelCoolFault : USINT;
		D_BarrelCoolFault : USINT;
		E_BarrelCoolFault : USINT;
		V_BarrelCoolFault : USINT;
		WarnExtrMeltP : USINT;
		AlarmExtrMeltP : USINT;
		A_ExtrMotorOverload : USINT;
		B_ExtrMotorOverload : USINT; (*420*)
		C_ExtrMotorOverload : USINT;
		D_ExtrMotorOverload : USINT;
		E_ExtrMotorOverload : USINT;
		V_ExtrMotorOverload : USINT;
		OilPump2Overload : USINT; (*425*)
		OilPump1Overload : USINT; (*426*)
		ParisonMotorOverload : USINT;
		CycleTimeCounReach : USINT;
		HeatError : USINT;
		WaterPressureLow : USINT; (*430*)
		AirPressureLow : USINT;
		C_ExtruderMotorOverload : USINT;
		AccuChargeRPressure : USINT;
		reserve01 : USINT;
		D_ExtruderMotorOverload : USINT;
		TempLow : USINT;
		AirPressureLowDly : USINT;
		TempFanErr : USINT;
		DieLipTempLow : USINT;
		ParisonrUpHWSwitch : USINT; (*440*)
		ParisonrDnHWSwitch : USINT;
		PlateMotorOverload : USINT;
		BatchDoneCounReach : USINT;
		SafeModule : USINT;
		SafeLogicValve : USINT;
		SafeCalibPressure : USINT;
	END_STRUCT;
	AP3131_typ : 	STRUCT 
		PmeanT : INT;
		QmeanT : INT;
		SmeanT : INT;
		PFmeanT : INT;
		AenergyT : DINT;
		RenergyT : DINT;
		SAenergyT : UDINT;
		IrmsA : UINT;
		IrmsB : UINT;
		IrmsC : UINT;
		EnableEnergy : BOOL;
		ClearEnergy : BOOL;
		TrigForceEnergy : BOOL;
		TrigDFT : BOOL;
	END_STRUCT;
	Axis_Comd_typ : 	STRUCT 
		power : BOOL;
		homing : BOOL;
		jogPos : BOOL;
		jogNeg : BOOL;
		absolute : BOOL;
		relative : BOOL;
		acknowledge : BOOL;
		stop : BOOL;
	END_STRUCT;
	Axis_Ctrl_typ : 	STRUCT 
		As : REAL;
		bRefer : BOOL;
		bReady : BOOL;
		bHomeOk : BOOL;
		bRes : BOOL;
		step : UINT;
		cmd : Axis_Comd_typ;
		param : Motorky_Param_typ;
		errorText : STRING[99];
		errorID : UINT;
	END_STRUCT;
	Base_Pos_typ : 	STRUCT 
		StartSize : UINT;
		StartDly : UINT;
		StopBuffer : UINT;
		Reserve : UINT;
	END_STRUCT;
	Base_Time_typ : 	STRUCT 
		CycleTime : UINT;
		StartDly : UINT;
		AutoCycleTime : UINT;
		PurgeGap : UINT;
		New_Member : REAL;
		New_Member1 : REAL;
	END_STRUCT;
	Batch_Info_typ : 	STRUCT 
		Batch : UDINT;
		BatchDoneCount : UDINT;
	END_STRUCT;
	Blowpin_Fix_typ : 	STRUCT 
		Zero : DINT;
		Max : DINT;
		CalibOK : UINT;
		Length : REAL;
		pCalibZero : REAL;
		vCalibZero : REAL;
		pCalibMax : REAL;
		vCalibMax : REAL;
		Up : Hyd_Fix_typ;
		Down : Hyd_Fix_typ;
		accel_Emergency : REAL;
		Tolerance : REAL;
		pRelease : REAL;
		vRelease : REAL;
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		eSensorType : sensor_type_enum;
		eMechanismType : mechanism_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pMax : REAL;
		pPress2Hold : REAL;
		Reserve0_REAL2 : REAL;
	END_STRUCT;
	Blowpin_Para_typ : 	STRUCT 
		S_Up : REAL;
		S_UpMicro : REAL;
		S_Dn : REAL;
		S_DnSlow : REAL;
		S_Dn2nd : REAL;
		V_Up : REAL;
		V_UpMicro : REAL;
		V_Dn : REAL;
		V_DnSlow : REAL;
		V_Dn2nd : REAL;
		P_Up : REAL;
		P_UpMicro : REAL;
		P_Dn : REAL;
		P_DnSlow : REAL;
		P_Dn2nd : REAL;
		P_DnHold : REAL;
		V_UpReset : REAL;
		P_UpReset : REAL;
		reserve1 : REAL;
		reserve2 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Blowpin_typ : 	STRUCT 
		Enable : BOOL;
		Stop : BOOL;
		NetworkInit : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		UpHWSwitch : BOOL;
		DnHWSwitch : BOOL;
		InitPower : BOOL;
		AxisReady : BOOL;
		Error : BOOL;
		Step : UINT;
		As : DINT;
		Ss : DINT;
		mmAs : REAL;
		DnEnable : USINT;
		UpEnable : USINT;
		UpPos : USINT;
		DnPos : USINT;
		actTjUp : REAL;
		actTjDn : REAL;
		Tolerance : REAL;
		ZeroPos : DINT;
		MaxPos : DINT;
		CalOK : USINT;
		PropValveOk : USINT;
		S_Up : REAL;
		S_UpMicro : REAL;
		S_Dn : REAL;
		S_DnSlow : REAL;
		S_Dn2nd : REAL;
		V_Up : REAL;
		V_UpMicro : REAL;
		V_Dn : REAL;
		V_DnSlow : REAL;
		V_Dn2nd : REAL;
		P_Up : REAL;
		P_UpMicro : REAL;
		P_Dn : REAL;
		P_DnSlow : REAL;
		P_Dn2nd : REAL;
		P_UpReset : REAL;
		V_UpReset : REAL;
		vProfGen : HydvProfGen;
		vRamp : HydSCurveRamp;
		vTimeProf : HydvTimeProf;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_old : REAL;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
		BtnOffDlyFlag : USINT;
		bDnPosFlag_Switch : USINT;
	END_STRUCT;
	Blow_Fix_typ : 	STRUCT 
		eBlowMode : blow_mode_enum;
		bBlowing2Low : REAL;
		New_Member2 : REAL;
		New_Member : REAL;
	END_STRUCT;
	Blow_typ : 	STRUCT 
		Enable : USINT;
		bOnce : BOOL;
		Step : UINT;
		tCounter : UDINT;
		tTemp : UDINT;
		Timer : timer_typ;
		TimerTotal : timer_typ;
		Out : UINT;
	END_STRUCT;
	Carriage_Fix_typ : 	STRUCT 
		Zero : DINT;
		Max : DINT;
		CalibOK : UINT;
		Length : REAL;
		pCalibZero : REAL;
		vCalibZero : REAL;
		pCalibMax : REAL;
		vCalibMax : REAL;
		pKeep : REAL;
		vKeep : REAL;
		Fw : Hyd_Fix_typ;
		Bw : Hyd_Fix_typ;
		accel_Emergency : REAL;
		Tolerance : REAL;
		ValveLin : ARRAY[0..20]OF Hyd_Node_typ;
		ToggleLin : ARRAY[0..MAX_TOGGLE_LIN_NUM]OF Hyd_Node_typ;
		ToggleLinNodeNum : USINT;
		ValueFix : REAL;
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		eSensorType : sensor_type_enum;
		eMechanismType : mechanism_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pMax : REAL;
		New_Member : REAL;
		New_Member3 : REAL;
		New_Member2 : REAL;
	END_STRUCT;
	Carriage_Para_typ : 	STRUCT 
		S_Blowpin : DINT;
		S_Head : DINT;
		V_Fw : REAL;
		V_FwSlow : REAL;
		V_FwHold : REAL;
		P_Fw : REAL;
		P_FwSlow : REAL;
		P_FwHold : REAL;
		S_Fw : REAL;
		S_FwCreep : REAL;
		S_FwSlow : REAL;
		V_Bw : REAL;
		V_BwSlow : REAL;
		V_BwReset : REAL;
		V_BwHold : REAL;
		P_Bw : REAL;
		P_BwSlow : REAL;
		P_BwHold : REAL;
		P_BwReset : REAL;
		S_Bw : REAL;
		S_BwCreep : REAL;
		S_BwSlow : REAL;
		New_Member5 : REAL;
		New_Member6 : REAL;
		New_Member1 : REAL;
		New_Member2 : REAL;
		New_Member4 : REAL;
		New_Member3 : REAL;
	END_STRUCT;
	Carriage_typ : 	STRUCT 
		Enable : USINT;
		bForwardLockUp : USINT;
		NetworkInit : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		FwHWSwitch : BOOL;
		BwHWSwitch : BOOL;
		InitPower : BOOL;
		AxisReady : BOOL;
		Error : BOOL;
		Step : UINT;
		As : DINT;
		Ss : DINT;
		mmAs : REAL;
		mmAsDrive : REAL;
		mmAsMax : REAL;
		mmAsMax_HMI : REAL;
		Tolerance : REAL;
		V_Fw : REAL;
		V_FwSlow : REAL;
		reserve : REAL;
		V_FwHold : REAL;
		P_Fw : REAL;
		P_FwSlow : REAL;
		reserve2 : REAL;
		P_FwHold : REAL;
		S_Fw : REAL;
		S_FwCreep : REAL;
		S_FwCreepDrive : REAL;
		S_FwSlow : REAL;
		S_FwSlowDrive : REAL;
		reserve3 : REAL;
		reserve8 : REAL;
		V_Bw : REAL;
		V_BwSlow : REAL;
		reserve4 : REAL;
		V_BwHold : REAL;
		V_BwReset : REAL;
		P_Bw : REAL;
		P_BwSlow : REAL;
		reserve5 : REAL;
		P_BwHold : REAL;
		P_BwReset : REAL;
		S_Bw : REAL;
		S_BwCreep : REAL;
		S_BwCreepDrive : REAL;
		S_BwSlow : REAL;
		S_BwSlowDrive : REAL;
		reserve6 : REAL;
		reserve7 : REAL;
		FwPos : USINT;
		BwPos : USINT;
		LimitBwPos : USINT;
		LimitOtherMoldBwPos : USINT;
		actTjFw : REAL;
		actTjBw : REAL;
		ZeroPos : DINT;
		MaxPos : DINT;
		CalOK : USINT;
		PropValveOk : USINT;
		vProfGen : HydvProfGen;
		vRamp : HydSCurveRamp;
		ValveLinear : HydLinear;
		ExtRobotTimer : timer_typ;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_old : REAL;
		p_set : REAL;
		v_set : REAL;
		v_add : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
		BtnOffDlyFlag : USINT;
		ResetSlowFlag : USINT;
	END_STRUCT;
	ClampLock_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
	END_STRUCT;
	Clamp_Fix_typ : 	STRUCT 
		Zero : DINT;
		Max : DINT;
		CalibOK : UINT;
		Length : REAL;
		pMax : REAL;
		pCalibZero : REAL;
		pCalibMax : REAL;
		vCalibZero : REAL;
		vCalibMax : REAL;
		Open : Hyd_Fix_typ;
		Close : Hyd_Fix_typ;
		accel_Emergency : REAL;
		v_OpnEnd : REAL;
		v_ClsEnd : REAL;
		Tolerance : REAL;
		ecZeroForce : DINT;
		ecMaxForce : DINT;
		SensorForce : REAL;
		ToggleLin : ARRAY[0..MAX_TOGGLE_LIN_NUM]OF Hyd_Node_typ;
		ToggleLinNodeNum : USINT;
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		eSensorType : sensor_type_enum;
		eMechanismType : mechanism_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pPress2Hold : REAL;
		ExtInMoldAlarmTime : REAL;
		New_Member1 : REAL;
		New_Member3 : REAL;
		New_Member4 : REAL;
		New_Member2 : REAL;
	END_STRUCT;
	Clamp_Para_typ : 	STRUCT 
		S_CutterCls : REAL;
		S_BlowpinOut : REAL;
		S_SubMoldIn : REAL;
		S_Cls : REAL;
		S_ClsEnd : REAL;
		V_Cls : REAL;
		V_ClsSlow : REAL;
		V_ClsManual : REAL;
		P_Cls : REAL;
		P_ClsSlow : REAL;
		P_ClsManual : REAL;
		S_SubMoldOut : REAL;
		S_OpnSlow : REAL;
		S_OpnEnd : REAL;
		V_Opn : REAL;
		V_OpnSlow : REAL;
		V_OpnSlow2 : REAL;
		V_OpnMicro : REAL;
		S_OpnSlow2 : REAL;
		P_Opn : REAL;
		P_OpnSlow : REAL;
		P_OpnMicro : REAL;
		P_OpnSlow2 : REAL;
		S_ScrewBw : REAL;
		P_OpnReset : REAL;
		V_OpnReset : REAL;
		P_Tonnage : REAL;
		V_Tonnage : REAL;
		S_BlowPinUp : REAL;
		P_Lock : REAL; (*Albert add lock para for pressure*)
		V_Lock : REAL; (*Albert add lock para for flow*)
		reserve1 : REAL;
		reserve2 : REAL;
		reserve3 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Clamp_typ : 	STRUCT 
		Enable : BOOL;
		Stop : BOOL;
		NetworkInit : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		OpnHWSwitch : BOOL;
		ClsHWSwitch : BOOL;
		InitPower : BOOL;
		AxisReady : BOOL;
		Error : BOOL;
		Step : UINT;
		Af_Ton : REAL;
		AfL_Ton : REAL;
		AfR_Ton : REAL;
		As : DINT;
		mmAsDrive : REAL;
		Ss : DINT;
		mmAs : REAL;
		mmAsMax : REAL;
		ClsPos : USINT;
		TonnagePos : USINT;
		OpnPos : USINT;
		S_Cls : REAL;
		S_ClsDrive : REAL;
		S_ClsEnd : REAL;
		S_ClsEndDrive : REAL;
		S_CutterCls : REAL;
		S_BlowPinUp : REAL;
		actTjOpn : REAL;
		actTjCls : REAL;
		V_Cls : REAL;
		V_ClsSlow : REAL;
		V_Tonnage : REAL;
		V_ClsLock : REAL; (*Albert,  add flow setting for hold clamp *)
		S_OpnSlow : REAL;
		S_OpnSlowDrive : REAL;
		S_OpnEnd : REAL;
		S_OpnEndDrive : REAL;
		S_ScrewBw : REAL;
		S_SubMoldOut : REAL;
		S_SubMoldIn : REAL;
		V_Opn : REAL;
		V_OpnSlow : REAL;
		V_OpnSlow2 : REAL;
		V_OpnReset : REAL;
		P_Cls : REAL;
		P_ClsSlow : REAL;
		P_Opn : REAL;
		P_OpnSlow : REAL;
		P_OpnMicro : REAL;
		P_OpnSlow2 : REAL;
		P_Tonnage : REAL;
		P_Lock : REAL; (*Albert add lock para for pressure*)
		Tolerance : REAL;
		mmDriveTonnage : REAL;
		ZeroPos : DINT;
		MaxPos : DINT;
		CalOK : USINT;
		PropValveOk : USINT;
		vProfGen : HydvProfGen;
		vRamp : HydSCurveRamp;
		vTimeProf : HydvTimeProf;
		ExtRobotTimer : timer_typ;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_old : REAL;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		BtnOffDlyFlag : USINT;
		aoPropValve : INT;
		OpnPosAtCarriageDn : USINT;
		OpnOverHalf : REAL;
		S_OpnSlow2 : REAL;
		S_OpnSlow2Drive : REAL;
		rAfterPress2_Tolerance : REAL;
		ExtInMoldInDone_Flag : USINT;
		ExtInMoldInDone_cnt : UINT;
	END_STRUCT;
	Clamp_User_typ : 	STRUCT 
		Timer : timer_typ;
	END_STRUCT;
	CleanValve_typ : 	STRUCT 
		CleanValveTotalTime : UDINT;
		CleanValveFwTime : UDINT;
		CleanValveInterval : UDINT;
		CleanValveBwTime : UDINT;
	END_STRUCT;
	Control_Info_typ : 	STRUCT 
		CurrentPoint : UINT;
		CurrentAccu : UINT;
		CurrentGapPercent : REAL;
		CurrentAccuPercent : REAL;
		CurrentGapSet : INT;
		CurrentGapSetPercent : REAL;
		CurrentTime : UINT;
		ControlStatus : UINT;
		ManualRun : UINT;
		AutoRun : UINT;
		Alarm : UINT;
		Reserve : UINT;
	END_STRUCT;
	Curve_typ : 	STRUCT 
		Cursor1Value : INT;
		Cursor2Value : INT;
		Value_Min : INT;
		Value_Max : INT;
		Status : UINT;
	END_STRUCT;
	DatObj_Operate_typ : 	STRUCT 
		InitStep : UINT;
		Step : UINT;
		ModName : STRING[19];
		Ident : UDINT;
		IdentUSB : UDINT;
		IdentAllUSB : UDINT;
		Status : STRING[39];
	END_STRUCT;
	DCDT_typ : 	STRUCT 
		Current : INT;
		IsReady : INT;
		ZeroPos : DINT;
		MaxPos : DINT;
		Unit : UDINT;
		Length : UDINT;
		ManualSpeed : INT;
		AutoSpeed : INT;
		MoveLength : DINT;
		MoveLengthV : DINT;
		GapMaxLimit : REAL;
		Status : UINT;
		res2_DINT : DINT;
		res1_DINT : DINT;
		res0_DINT : DINT;
	END_STRUCT;
	DI_Filter_typ : 	STRUCT 
		In : USINT;
		TimeCounter : UDINT;
		Counter : UDINT;
		tTime : UDINT;
		TC_Time : UINT;
		Percent : UINT;
		Out : USINT;
	END_STRUCT;
	DO_Fix_typ : 	STRUCT 
		Free0 : USINT;
		Free1 : USINT;
		Free2 : USINT;
		Free3 : USINT;
		Free4 : USINT;
		Free5 : USINT;
		Free6 : USINT;
		Free7 : USINT;
		Free8 : USINT;
		Free9 : USINT;
		Free10 : USINT;
		Free11 : USINT;
		Free12 : USINT;
		Free13 : USINT;
		Free14 : USINT;
		Free15 : USINT;
	END_STRUCT;
END_TYPE

(*Energy Monitor*)

TYPE
	energy_info_typ : 	STRUCT 
		Power : REAL;
		AenergyT : REAL;
		SumAenergyT : REAL;
		AvgEnergy : REAL;
		AvgEnergyHeating : REAL;
		Current_A : REAL;
		Current_B : REAL;
		Current_C : REAL;
	END_STRUCT;
	ExSPC_Mold_typ : 	STRUCT 
		tCycle : REAL;
		tTotalCycle : REAL;
		ClampClsDelay : REAL;
		tCalmpCls : REAL;
		ClampHoldTime : REAL;
		ClampLockTime : REAL;
		BlowPinUpDelay : REAL;
		CarriageBwDelay : REAL;
		tCarriageBw : REAL;
		CarriageBwHoldTime : REAL;
		BlowPinDnDelay : REAL;
		BlowPinDnTime : REAL;
		BlowingDelay : REAL;
		BlowingTime : REAL;
		AirVentingTime : REAL;
		BlowingTime2 : REAL;
		AirVentingTime2 : REAL;
		CoolPinBlowDelay : REAL;
		CoolPinBlowTime : REAL;
		CoolPinDnDelay : REAL;
		CoolPinDnTime : REAL;
		tBlowPinUp : REAL;
		TopDeflashFwDelay : REAL;
		tTopDeflash1 : REAL;
		TopDeflashFwTime : REAL;
		TopDeflashSecDelay : REAL;
		tTopDeflash2 : REAL;
		TopDeflashSecTime : REAL;
		TopDeflashBwDelay : REAL;
		tTopDeflashBw : REAL;
		BlowpintinyreturnDelay : REAL;
		BlowpintinyreturnTime : REAL;
		tSubMoldMicroOpnTime : REAL;
		ClampOpnDelay : REAL;
		tClampOpn : REAL;
		CarriageFwDelay : REAL;
		tCarriageFw : REAL;
		CarriageFwHoldTime : REAL;
		MoldClsWait : REAL;
	END_STRUCT;
	ExSPC_typ : 	STRUCT 
		RMold : ExSPC_Mold_typ;
		LMold : ExSPC_Mold_typ;
	END_STRUCT;
	ExtrLift_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		eMechanismType : mechanism_typ_enum;
		Zero : DINT;
		Max : DINT;
		CalibOK : UINT;
		Length : REAL;
		pMax : REAL;
		pCalibMax : REAL;
		pCalibZero : REAL;
		vCalibZero : REAL;
		vCalibMax : REAL;
		Up : Hyd_Fix_typ;
		Dn : Hyd_Fix_typ;
		accel_Emergency : REAL;
		Tolerance : REAL;
		ValveLin : ARRAY[0..20]OF Hyd_Node_typ;
		ToggleLin : ARRAY[0..MAX_TOGGLE_LIN_NUM]OF Hyd_Node_typ;
		ToggleLinNodeNum : USINT;
		ValueFix : REAL;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL2 : REAL;
		Reserve0_REAL3 : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	ExtrLift_Para_typ : 	STRUCT 
		V_Up : REAL;
		P_Up : REAL;
		S_Up : REAL;
		V_Dn : REAL;
		P_Dn : REAL;
		S_Dn : REAL;
		V_UpReset : REAL;
		V_DnReset : REAL;
		res1_REAL1 : REAL;
		res1_REAL : REAL;
		res0_REAL1 : REAL;
		res0_REAL : REAL;
	END_STRUCT;
	ExtrLift_typ : 	STRUCT 
		NetworkInit : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		UpHWSwitch : BOOL;
		DnHWSwitch : BOOL;
		Step : UINT;
		vProfGen : HydvProfGen;
		vRamp : HydSCurveRamp;
		InitPower : BOOL;
		AxisReady : BOOL;
		ZeroPos : DINT;
		MaxPos : DINT;
		CalOK : USINT;
		Error : BOOL;
		As : DINT;
		Ss : DINT;
		mmAs : REAL;
		mmAsDrive : REAL;
		mmAsMax : REAL;
		V_Up : REAL;
		P_Up : REAL;
		S_Up : REAL;
		S_UpDrive : REAL;
		V_Dn : REAL;
		P_Dn : REAL;
		S_Dn : REAL;
		S_DnDrive : REAL;
		V_UpReset : REAL;
		V_DnReset : REAL;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		Tolerance : REAL;
		UpPos : USINT;
		DnPos : USINT;
		actTjUp : REAL;
		actTjDn : REAL;
		v_set : REAL;
		p_old : REAL;
		p_set : REAL;
		v_set1 : REAL;
		v_add : REAL;
		p_act : REAL;
		v_act : REAL;
		Out : UINT;
	END_STRUCT;
END_TYPE

(*Extuer Basic Structure  for powerlink*)

TYPE
	Extruder_Basic_typ : 	STRUCT  (*wzq*)
		SwitchOn : BOOL; (*wzq*)
		OperateEnable : BOOL;
		Inverse : BOOL;
		InverseHmi : BOOL;
		SetMotorSpeedINT : {REDUND_UNREPLICABLE} INT;
		SetMotorSpeedREAL : REAL;
		ActMotorSpeed : INT;
		ActMotorCurrent : UINT;
		Acc : ARRAY[0..2]OF REAL;
		AccTime : ARRAY[0..2]OF REAL;
		SetExtrSpeed : INT;
		ActExtrSpeed : INT;
		Pro : INT;
		RateTorque : USINT;
		AlarmTorque : USINT;
		FastStop : BOOL;
		ActTorque : INT;
	END_STRUCT;
	Extruder_Ctrl_typ : 	STRUCT 
		Status : Extruder_Status_typ;
		tTask : REAL;
		FoucsBasic : UINT;
		FocusProduct : UINT;
		bKeyOnOff : BOOL;
		bKeyOff : BOOL;
		bKeyStop : BOOL;
		bKeyBasic : BOOL;
		bKeyProduction : BOOL;
		bKeyFeedingCylFw : BOOL;
		bKeyInc : BOOL;
		bKeyDec : BOOL;
		tOnDelay : REAL;
		tOffDelay : REAL;
		tProtect : REAL;
		Step : UINT;
		BasicSpeed : REAL; (*RPM*)
		ProductionSpeed : REAL; (*RPM*)
		tET_s : REAL;
		cntProtect : UDINT;
		ActSpeed : REAL;
		aiCurrent : INT;
		Current : REAL;
		diActRPM : BOOL;
		aiActRPM : INT;
		aoSetRPM : INT;
		ActRPM : REAL;
		MotorRPM : REAL;
		MeltTemp : REAL;
		aiMeltPressure : INT;
		MeltPressure : REAL;
		bKeyOnOffOld : BOOL;
		bKeyOffOld : BOOL;
		bKeyBasicOld : BOOL;
		bKeyProductionOld : BOOL;
		bKeyFeedingCylFwOld : BOOL;
		doOnOld : BOOL;
		doContactor : BOOL;
		doOn : BOOL;
		doFeedingCylFw : USINT;
		LedOn : USINT;
		LedBaisic : BOOL;
		LedProduction : BOOL;
		OnFeedbackErr : BOOL;
		Out : REAL; (*RPM*)
		Frequency_ABB : REAL;
		Current_ABB : REAL;
	END_STRUCT;
	Extruder_Fix_typ : 	STRUCT 
		Use : USINT;
		RPM10v : REAL;
		Accel : REAL;
		StopTime : REAL;
		MeltP10V : REAL;
		MeltP0V : REAL;
		Currency10V : REAL;
		WarningMeltP : UINT;
		AlarmMeltP : UINT;
		tProtect : REAL;
		tOnDelay : REAL;
		tOffDelay : REAL;
		tLubricateCheck : REAL;
		WarningCurrent : UINT;
		AlarmCurrent : UINT;
		Ratio : REAL;
		LinkWithMode : USINT;
		New_Member2 : REAL;
		New_Member3 : USINT;
		New_Member4 : USINT;
		eCtrlType : extruder_ctrl_typ_enum;
	END_STRUCT;
	Extruder_Para_typ : 	STRUCT 
		BasicSpeed : REAL; (*RPM*)
		ProductionSpeed : REAL; (*RPM*)
		TempCoolStart : REAL;
		TempCoolStop : REAL;
		SetSpeed : INT;
	END_STRUCT;
	Extruder_Status_typ : 	STRUCT 
		OnProtected : USINT;
	END_STRUCT;
	Extr_Lift_Lower_typ : 	STRUCT 
		vLowerQuick : REAL;
		vLowerSlow : REAL;
		vCalib : REAL;
		accel : REAL;
		res1 : REAL;
		res0 : REAL;
		warningMotorTemp : REAL;
		AlarmMotorTemp : REAL;
	END_STRUCT;
	Flux_Sensor_typ : 	STRUCT 
		Zero : REAL;
		Max : REAL;
		Length : REAL;
	END_STRUCT;
	Grp_Ctrl_typ : 	STRUCT 
		OnOff : ARRAY[0..MAX_IDX_TEMPGROUP]OF BOOL; (*heating group on off*)
		TuneOnOff : ARRAY[0..MAX_IDX_TEMPGROUP]OF BOOL; (*tune group on off*)
		PreOnOff : ARRAY[0..MAX_IDX_TEMPGROUP]OF BOOL; (*prepare heating on off*)
	END_STRUCT;
END_TYPE

(*Hardware typ info*)

TYPE
	HardWare_typ : 	STRUCT 
		ATC402_1 : ARRAY[0..5]OF INT;
		ATC402_2 : ARRAY[0..5]OF INT;
		ATC402_3 : ARRAY[0..5]OF INT;
		ATC402_4 : ARRAY[0..5]OF INT;
		ATC402_5 : ARRAY[0..5]OF INT;
		ATC402_6 : ARRAY[0..5]OF INT;
		AI4632_1 : ARRAY[0..3]OF INT;
		AI4632_2 : ARRAY[0..3]OF INT;
		AI4632_3 : ARRAY[0..3]OF INT;
		AI4632_4 : ARRAY[0..3]OF INT;
		AO4632_1 : ARRAY[0..3]OF INT;
		AO4632_2 : ARRAY[0..3]OF INT;
		AO4632_3 : ARRAY[0..3]OF INT;
		AO4632_4 : ARRAY[0..3]OF INT;
		AO4622_1 : ARRAY[0..3]OF INT;
		AO4622_2 : ARRAY[0..3]OF INT;
		AO4622_3 : ARRAY[0..3]OF INT;
		AO4622_4 : ARRAY[0..3]OF INT;
		DI9371_1 : ARRAY[0..11]OF BOOL;
		DI9371_2 : ARRAY[0..11]OF BOOL;
		DI9371_3 : ARRAY[0..11]OF BOOL;
		DI9371_4 : ARRAY[0..11]OF BOOL;
		DI9371_5 : ARRAY[0..11]OF BOOL;
		DI9371_6 : ARRAY[0..11]OF BOOL;
		DI9371_7 : ARRAY[0..11]OF BOOL;
		DI9371_8 : ARRAY[0..11]OF BOOL;
		DI9371_9 : ARRAY[0..11]OF BOOL;
		DI9371_10 : ARRAY[0..11]OF BOOL;
		DO8332_1 : ARRAY[0..7]OF BOOL;
		DO8332_2 : ARRAY[0..7]OF BOOL;
		DO8332_3 : ARRAY[0..7]OF BOOL;
		DO8332_4 : ARRAY[0..7]OF BOOL;
		DO8332_5 : ARRAY[0..7]OF BOOL;
		DO8332_6 : ARRAY[0..7]OF BOOL;
		DO8332_7 : ARRAY[0..7]OF BOOL;
		DO8332_8 : ARRAY[0..7]OF BOOL;
		DO8332_9 : ARRAY[0..7]OF BOOL;
		DO8332_10 : ARRAY[0..7]OF BOOL;
		DO8332_11 : ARRAY[0..7]OF BOOL;
		DO8332_12 : ARRAY[0..7]OF BOOL;
		AP3131_1 : AP3131_typ;
		AP3131_2 : AP3131_typ;
		AP3131_3 : AP3131_typ;
		AP3131_4 : AP3131_typ;
		ModuleOk : Module_Ok_typ;
		ModuleHide : Module_Hide_typ;
		AOForceNum : USINT;
		AOForce : ARRAY[0..39]OF INT;
		DOForce : ARRAY[0..199]OF BOOL;
		DIF371_1 : ARRAY[0..15]OF BOOL;
		DIF371_2 : ARRAY[0..15]OF BOOL;
		DIF371_3 : ARRAY[0..15]OF BOOL;
		DIF371_4 : ARRAY[0..15]OF BOOL;
		DOF322_1 : ARRAY[0..15]OF BOOL;
		DOF322_2 : ARRAY[0..15]OF BOOL;
		DOF322_3 : ARRAY[0..15]OF BOOL;
		DOF322_4 : ARRAY[0..15]OF BOOL;
		DOF322_5 : ARRAY[0..15]OF BOOL;
		DOF322_6 : ARRAY[0..15]OF BOOL;
		DO9322_1 : ARRAY[0..11]OF BOOL;
		DO9322_2 : ARRAY[0..11]OF BOOL;
		DO9322_3 : ARRAY[0..11]OF BOOL;
		DO9322_4 : ARRAY[0..11]OF BOOL;
		DO9322_5 : ARRAY[0..11]OF BOOL;
		DO9322_6 : ARRAY[0..11]OF BOOL;
		DO9322_7 : ARRAY[0..11]OF BOOL;
		DO9322_8 : ARRAY[0..11]OF BOOL;
		DO9322_9 : ARRAY[0..11]OF BOOL;
		DO9322_10 : ARRAY[0..11]OF BOOL;
		XP0101_1 : ARRAY[0..7]OF BOOL;
		XP0101Led_1 : ARRAY[0..31]OF BOOL;
		SO4110_1 : ARRAY[0..3]OF BOOL;
		SI8110_2 : ARRAY[0..7]OF BOOL;
		SI8110_1 : ARRAY[0..7]OF BOOL;
	END_STRUCT;
	Heating_Plan_typ : 	STRUCT 
		HeatingOn : ARRAY[0..6]OF Hour_Minute_typ; (*heating on time*)
		HeatingOff : ARRAY[0..6]OF Hour_Minute_typ; (*heating off time*)
		Enable : ARRAY[0..13]OF BOOL; (*enable *)
	END_STRUCT;
END_TYPE

(*HMI Interface Type*)

TYPE
	hmi_ctrl_typ : 	STRUCT 
		bEsc : BOOL; (*Escape-Key pressed*)
		bEnter : BOOL; (*Enter-Key pressed*)
		bBackspace : BOOL; (*Backspace-Key pressed*)
		bSelect : BOOL; (*Select-Key pressed*)
		bShift : BOOL; (*Shift-Key pressed*)
		bCrsUp : BOOL; (*CursorUp-Key pressed*)
		bCrsDown : BOOL; (*CursorDown-Key pressed*)
		bCrsRight : BOOL; (*CursorRight-Key pressed*)
		bCrsLeft : BOOL; (*CursorLeft-Key pressed*)
		bFuncKey : BOOL; (*function key*)
		uChar : USINT; (*input character*)
		uNum : USINT; (*input number*)
		bCutterFw : BOOL;
		bCutterBw : BOOL;
		bExtruderUp : BOOL;
		bExtruderDn : BOOL;
		bCarriageUp : BOOL;
		bCarriageDn : BOOL;
		bClampOpn : BOOL;
		bClampCls : BOOL;
		bBlowPinUp : BOOL;
		bBlowPinDn : BOOL;
		bTopDeflashFw : BOOL;
		bTopDeflashBw : BOOL;
	END_STRUCT;
	Hour_Minute_typ : 	STRUCT 
		Hour : USINT;
		Minute : USINT;
		Second : UINT;
	END_STRUCT;
	HWEnable_Fix_typ : 	STRUCT 
		RMold : HWEnable_Mold_Fix_typ;
		LMold : HWEnable_Mold_Fix_typ;
		HighPrsMotor : USINT;
		ACCPump : USINT;
		ACCPump_Extrlift : USINT;
		ExtrLift : USINT;
		Cutter : USINT;
		New_Member2 : USINT;
		New_Member3 : USINT;
		New_Member4 : USINT;
		New_Member8 : USINT;
		New_Member9 : USINT;
		New_Member : USINT;
	END_STRUCT;
	HWEnable_Mold_Fix_typ : 	STRUCT 
		TopDeflash : USINT;
		BottomDeflash : USINT;
		PunchHandle : USINT;
		PullBottom : USINT; (*拉瓶底  於關模後下拉多出來的料*)
		CoolPin : USINT;
		CoolPin2 : USINT;
		CoolPin3 : USINT;
		SubMold : USINT;
		BottomSubmold : USINT;
		RobotTransfer : USINT;
		RobotOpnCls : USINT;
		RobotUpDn : USINT;
		ExtRobot : USINT;
		ExtInMoldSticker : USINT;
		Screw : USINT;
		InnerPin : USINT;
		PinPart : USINT;
		IntervalBlow : USINT;
		CutNeck : USINT;
		BlowPin2 : USINT;
		HookPin : USINT;
		New_Member4 : USINT;
		New_Member3 : USINT;
		New_Member2 : USINT;
		New_Member1 : USINT;
		New_Member : USINT;
	END_STRUCT;
	Hyd_Fix_typ : 	STRUCT 
		accel : REAL;
		decel : REAL;
		decel_end : REAL;
		Tj : REAL;
		Td : REAL;
		accel_stop : REAL;
		sTol : REAL;
		aRampPrs : REAL;
		tVOn : UDINT;
		tVOff : UDINT;
		V_Max : REAL;
		VoltageMax : REAL;
		VoltageMin : REAL;
		bCloseLoop : BOOL;
		bRes : BOOL;
		Kp : REAL;
		vDeadZone : REAL;
	END_STRUCT;
END_TYPE

(*Heating zone control type*)

TYPE
	H_Zone_typ : 	STRUCT 
		y_heat : ARRAY[0..MAX_IDX_ZONE]OF INT; (*Controller heat for h_pwm Task*)
		y_cool : ARRAY[0..MAX_IDX_ZONE]OF INT; (*Controller cool for h_pwm Task*)
		ZoneColor : ARRAY[0..MAX_IDX_ZONE]OF UINT; (*zone display color*)
		HC_sta : ARRAY[0..MAX_IDX_ZONE]OF UINT;
		ActT : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*Measured Actual Temperature*)
		do_H : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*heating digital output*)
		do_C : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*cooling digital output*)
		ao_H : ARRAY[0..MAX_IDX_ZONE]OF INT; (*heating analog output*)
		ao_C : ARRAY[0..MAX_IDX_ZONE]OF INT; (*cooling analog output*)
		HeatNormal : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*Feedback normal*)
	END_STRUCT;
	Imply_typ : 	STRUCT 
		OilPumpNotOn : USINT; (*1*)
		CoolingPumpNotOn : USINT;
		FrequencyNotOn : USINT;
		ParisonPumpNotOn : USINT;
		TempLow : USINT;
		HeatNotOn : USINT;
		SafeGate : USINT;
		Reset : USINT;
		Small3V : USINT;
		Big6V : USINT; (*10*)
		LowerSafeFeed : USINT;
		ForceAO : USINT;
		ResetSuccess : USINT;
		ResetFalure : USINT;
		AutoRunStop : USINT;
		CalMode : USINT;
		LCarriageFw : USINT;
		LCarriageBw : USINT;
		LClampOpn : USINT;
		LClampCls : USINT; (*20*)
		LPinUp : USINT;
		LPinDn : USINT;
		LTopDeflashBw : USINT;
		LBottomDeflashBw : USINT;
		LCoolPinUp : USINT;
		LCoolPin2Up : USINT;
		LCoolPin3Up : USINT;
		LSubMoldFw : USINT;
		LSubMoldBw : USINT;
		LPinPartFw : USINT; (*30*)
		LPinPartBw : USINT;
		LHookPinFw : USINT;
		LHookPinBw : USINT;
		LScrewFw : USINT;
		LScrewBw : USINT;
		LRobotBw : USINT;
		LRobotFw : USINT;
		LRobotOpn : USINT;
		LRobotFw_A : USINT;
		LRobotBw_A : USINT; (*40*)
		LSubMoldOpn : USINT;
		LInnerPinBw : USINT;
		LCutNeckBw : USINT;
		LSealerOpn : USINT;
		LClampCalErr : USINT;
		LCarriageCalErr : USINT;
		LClampValveOk : USINT;
		LCarriageValveOk : USINT;
		LBlowPinValveOk : USINT;
		LRobotFwOrBw : USINT; (*50*)
		LRobotUp : USINT;
		RCarriageFw : USINT;
		RCarriageBw : USINT;
		RClampOpn : USINT;
		RClampCls : USINT;
		RPinUp : USINT;
		RPinDn : USINT;
		RTopDeflashBw : USINT;
		RBottomDeflashBw : USINT;
		RCoolPinUp : USINT; (*60*)
		RCoolPin2Up : USINT;
		RCoolPin3Up : USINT;
		RSubMoldFw : USINT;
		RSubMoldBw : USINT;
		RPinPartFw : USINT;
		RPinPartBw : USINT;
		RHookPinFw : USINT;
		RHookPinBw : USINT;
		RScrewFw : USINT;
		RScrewBw : USINT; (*70*)
		RRobotBw : USINT;
		RRobotFw : USINT;
		RRobotOpn : USINT;
		RRobotFw_A : USINT;
		RRobotBw_A : USINT;
		RSubMoldOpn : USINT;
		RInnerPinBw : {REDUND_UNREPLICABLE} USINT;
		RCutNeckBw : USINT;
		RSealerOpn : USINT;
		RClampCalErr : USINT; (*80*)
		RCarriageCalErr : USINT;
		RClampValveOk : USINT;
		RCarriageValveOk : USINT;
		RBlowPinValveOk : USINT;
		RRobotFwOrBw : USINT;
		RRobotUp : USINT;
		PsdLevel1 : USINT;
		PsdLevel2 : USINT;
		PsdLevel3 : USINT;
		PsdLevel4 : USINT; (*90*)
		InfoRecipe : USINT;
		StateManual : USINT;
		ResetOk : USINT;
		ResetFalse : USINT;
		PneumaticValveOff : USINT;
		L_RobotPower : USINT;
		R_RobotPower : USINT;
		TimeStopMachine : USINT;
		NoFunction : USINT;
		NoDeflasherL : USINT; (*100*)
		NoDeflasherR : USINT;
		CarriageEnable : USINT;
		ScreenShotFalse : USINT;
		NoUSB : USINT;
		RobotReset : USINT;
		ParaUSB_NoFile : USINT;
		ParaUSB_Length : USINT;
		ParaOP_OK : USINT;
		ParaOP_False : USINT;
		FixUSB_NoFile : USINT; (*110*)
		FixUSB_Length : USINT;
		FixOP_OK : USINT;
		FixOP_False : USINT;
		CurveUSB_NoFile : USINT;
		CurveUSB_Length : USINT;
		CurveOP_OK : USINT;
		CurveOP_False : USINT;
		L_CarriageLockLimit : USINT;
		R_CarriageLockLimit : USINT;
		PinCalib : USINT; (*120*)
		PinCalibFalse : USINT;
		PinCalibSuccess : USINT;
		ExtruderA_NotOn : USINT;
		ExtruderB_NotOn : USINT;
		ExtruderC_NotOn : USINT;
		ExtruderV_NotOn : USINT;
		ExtruderA_NotOut : USINT;
		ExtruderB_NotOut : USINT;
		ExtruderC_NotOut : USINT;
		ExtruderV_NotOut : USINT; (*130*)
		MainPumpCondition : USINT;
		ParisonPumpCondition : USINT;
		CoolingPumpCondition : USINT;
		LBlowPinCalErr : USINT;
		L_RobotTransferCalErr : USINT;
		L_RobotUpDnCalErr : USINT;
		L_SensorCalErr : USINT;
		L_SensorUp : USINT;
		RBlowPinCalErr : USINT;
		R_RobotTransferCalErr : USINT; (*140*)
		R_RobotUpDnCalErr : USINT;
		R_SensorCalErr : USINT;
		R_SensorUp : USINT;
		DisableCutterCalibMode : USINT;
		ParisonCalibNotManual : USINT;
		HeaterPowerNotOn : USINT;
		ExtruderA_P84_Error : USINT;
		ExtruderB_P84_Error : USINT;
		ExtruderC_P84_Error : USINT;
		ExtruderD_P84_Error : USINT; (*150*)
		ExtruderE_P84_Error : USINT;
		ExtruderV_P84_Error : USINT;
		LBottomSubMoldFw : USINT;
		LBottomSubMoldBw : USINT;
		RBottomSubMoldFw : USINT;
		RBottomSubMoldBw : USINT;
		LTopDeflashOpn : USINT;
		RTopDeflashOpn : USINT;
		L_SubmoldOut_Clamp : USINT;
		R_SubmoldOut_Clamp : USINT; (*160*)
		LRobotOpn_L : USINT;
		LRobotOpn_R : USINT;
		RRobotOpn_L : USINT;
		RRobotOpn_R : USINT;
		LTopDeflashFw : USINT;
		RTopDeflashFw : USINT;
		LProductAuxClampOpn : USINT;
		RProductAuxClampOpn : USINT;
		NoLRobot : USINT;
		NoRRobot : USINT; (*170*)
		LCarriageFwBw : USINT;
		RCarriageFwBw : USINT;
		LClampOpnCls : USINT;
		RClampOpnCls : USINT;
		LevelTooLow : USINT;
		TempLoweringEnable : USINT;
		ExtruderEnable : USINT;
		AutoModeLock : USINT;
		ExtruderD_NotOn : USINT;
		ExtruderNoChoose : USINT; (*180*)
		PlateUpLimit : USINT;
		PlateDnLimit : USINT;
		ExtrA_OnDelay : USINT;
		ExtrB_OnDelay : USINT;
		ExtrC_OnDelay : USINT;
		ExtrD_OnDelay : USINT;
		ExtrVisu_OnDelay : USINT;
		LExtRobotUp : USINT;
		RExtRobotUp : USINT;
		LPin2Up : USINT; (*190*)
		RPin2Up : USINT;
		LPin2Dn : USINT;
		RPin2Dn : USINT;
		LBlowPin2CalErr : USINT;
		RBlowPin2CalErr : USINT;
		DriverNetworkPhaseNotReady : USINT;
		RBlowPinEnable : USINT;
		RBlowPin2Enable : USINT;
		LBlowPinEnable : USINT;
		LBlowPin2Enable : USINT; (*200*)
		DieLipTempLow : USINT;
		ExtrLiftUp : USINT;
		ExtrLiftDn : USINT;
		PlateFwBwLimit : USINT;
		RDeflashAndPullbottomInterlock : USINT;
		LDeflashAndPullbottomInterlock : USINT;
		LExtInMoldStickerBw : USINT;
		RExtInMoldStickerBw : USINT;
		LExtInMoldStickerAlarm : USINT;
		RExtInMoldStickerAlarm : USINT; (*210*)
	END_STRUCT;
END_TYPE

(*P84 Comand control Structure*)

TYPE
	Inverter_Cmd_typ : 	STRUCT 
		bSwitchOn : BOOL;
		bEnableVotage : BOOL;
		bNotQuickStop : BOOL;
		bEnableCtrl : BOOL;
		bResetFault : BOOL;
		bAskStop : BOOL;
		bSpeed32767 : BOOL;
		bInverse : BOOL;
	END_STRUCT;
	Inverter_Ctrl_typ : 	STRUCT  (*Albert*)
		aoSetSpeed : INT;
		SetTorque : INT;
		InverterCmd : Inverter_Cmd_typ;
		InverterStatus : Inverter_Status_typ;
		SetSpeed : INT;
		rActRPM : REAL;
		InverterFub : Inverter_fub_typ;
	END_STRUCT;
	Inverter_fub_typ : 	STRUCT 
		fnLCRRamp : LCRRamp;
	END_STRUCT;
	Inverter_Status_typ : 	STRUCT 
		bModuleOk : BOOL;
		bSwitchOn : BOOL;
		bRunning : BOOL;
		bFault : BOOL;
		bWarningOrAlarm : BOOL;
		ActSpeed : INT;
		ActTorque : INT;
		Current : UINT;
		DriverTemp : UINT;
		MotorTemp : UINT;
		FaultID : INT;
		nEncoder : UINT;
		PowerOnReady : BOOL;
	END_STRUCT;
	LengthCtrl_typ : 	STRUCT 
		Enable : USINT;
		Start : BOOL;
		Step : UINT;
		MagicEye : BOOL;
		ClampOpn : BOOL;
		TolLengthPosTime : DINT;
		TolLengthNegTime : DINT;
		TimePosDis : DINT;
		TimeNegDis : DINT;
		TimeCycleDis : DINT;
		Timer : timer_typ;
		TimerCycle : timer_typ;
		Out : REAL;
	END_STRUCT;
	Length_Para_typ : 	STRUCT 
		SetLength : REAL;
		Kp : REAL;
		TolLengthPosTime : UDINT;
		TolLengthNegTime : UDINT;
		LengthEnable : USINT;
		IncRPM : REAL;
		DecRPM : REAL;
		TimeOut : REAL;
		New_Member1 : REAL;
		New_Member2 : REAL;
		New_Member3 : REAL;
	END_STRUCT;
	Lubrication_Fix_typ : 	STRUCT 
		AutoLubrEnable : UINT;
		LubrPumpWorkTime : UDINT;
		LubrPumpStopTime : UDINT;
		SetCycleTimes : UDINT;
		ExtrGearLubrTime : UDINT;
		ResUINT : UINT;
		resREAL : REAL;
	END_STRUCT;
	Machine_AlarmLight_typ : 	STRUCT 
		Red : USINT;
		Yellow : USINT;
		Green : USINT;
	END_STRUCT;
	Machine_Fix_typ : 	STRUCT 
		MoldL : Mold_Fix_typ;
		MoldR : Mold_Fix_typ;
		Plate : Plate_Fix_typ;
		ExtrLift : ExtrLift_Fix_typ;
		Parison : Parison_Fix_typ;
		ExtruderA : Extruder_Fix_typ;
		ExtruderB : Extruder_Fix_typ;
		ExtruderC : Extruder_Fix_typ;
		ExtruderD : Extruder_Fix_typ;
		ExtruderE : Extruder_Fix_typ;
		ExtruderVisu : Extruder_Fix_typ;
		Pump : Pump_Fix_typ;
		OilAccu : Oil_Accu_typ;
		OilAccu_Extrlift : Oil_Accu_typ;
		OilCooling : Oil_Cooling_typ;
		Lubrication : Lubrication_Fix_typ;
		CleanValve : Valve_Clean_typ;
		Option : Option_Fix_typ;
		Do : DO_Fix_typ;
	END_STRUCT;
	Machine_Info_typ : 	STRUCT 
		OilTemp : REAL;
		OilAccuPressure : REAL;
		TempHopper : REAL;
		TempExtrCoolWater : REAL;
		RunStatus : UINT;
		LClampPos : REAL;
		RClampPos : REAL;
		LCarriagePos : REAL;
		RCarriagePos : REAL;
		ServoPos : UINT;
		ReceipeNum : UINT;
		LTimeCyc : UDINT;
		RTimeCyc : UDINT;
		Yield_H : UDINT;
		Yield : UDINT;
		CycleTimes : UDINT;
		CycleTimesL : UDINT;
		CycleTimesR : UDINT;
		Station : UINT;
		IsZeroDn : UINT;
		IsHeat : UINT;
		IsAlarm : UINT;
		IsOpt : UINT;
		Mark : UINT;
		Extruder : UINT;
		PermitAuto : UINT;
		PasswordLevel : UINT;
		Language : UINT;
		AlarmIndex : UINT;
		ImplyIdex : UINT;
		IsImply : UINT;
		ImplyTime : UINT;
		TimeLimit : USINT;
		ExtrTLow : USINT;
		Auto : USINT;
		Calib : USINT;
		Manual : USINT;
		LedReset : USINT;
		PinCalibration : USINT;
		LedPinCalibMode : USINT;
		bDryCycle : USINT;
		Left : USINT;
		Right : USINT;
		LeftRight : USINT;
		MachineReset : USINT;
		IsAccuOK : USINT;
		Modem_Ok : USINT;
		LedAutoRun : USINT;
		LedAutoStop : USINT;
		SafeInStop : USINT;
		TimeStopMachine : USINT;
		GoodBottles : UDINT;
		BadBottles : UDINT;
		GoogPercent : REAL;
		Power_1 : REAL;
		Power_2 : REAL;
		AenergyT_1 : REAL;
		AenergyT_2 : REAL;
		SumAenergyT_1 : REAL;
		SumAenergyT_2 : REAL;
		AvgEnergy : REAL;
		AvgEnergyHeating : REAL;
		Current_A2 : REAL;
		Current_B2 : REAL;
		Current_C2 : REAL;
		Current_A1 : REAL;
		Current_B1 : REAL;
		Current_C1 : REAL;
		aiOilFlux : INT;
		RBlowPinPos : REAL;
		LBlowPinPos : REAL;
		EnergyInfo : ARRAY[0..MAX_IDX_ENERGY]OF energy_info_typ; (*Albert *)
		OilPumpDelayDone : USINT;
		bAccuPumpCalib : BOOL;
		LClampPos1 : REAL;
		RClampPos1 : REAL;
		bAccuPumpMinCalib : BOOL;
		LCarriagePos1 : REAL;
		CycleTimeCounReach : BOOL;
		bLoweringFlag : BOOL;
		ExtrHeatTimeFlag : USINT;
		PlateUpFlag : USINT;
		AirPressureLowDlyFlag : USINT;
		AlarmLight : Machine_AlarmLight_typ;
		ExtParisonCtrlStart : USINT;
		OilAccuChargeAuto : USINT;
		OilAccuChargeAuto_Extrlift : USINT;
		OilAccuPressure_Extrlift : REAL;
		IsAccuOK_Extrlift : USINT;
		bAccuPump_Extrlift_MinCalib : USINT;
		bAccuPump_Extrlift_Calib : BOOL;
		bRClampOpnClsValveDlyDone : USINT;
		bLClampOpnClsValveDlyDone : USINT;
		ExtrA_ErrStop_LinkWith : USINT;
		ExtrD_ErrStop_LinkWith : USINT;
		ExtrVisu_ErrStop_LinkWith : USINT;
		ParisonPump_ErrStop_LinkWith : USINT;
		DriverNetworkPhaseReady : USINT;
		RBlowPin2Pos : REAL;
		LBlowPin2Pos : REAL;
		ManualModeReguest : USINT;
		ExtrliftAlarmReqSemiAutoFlag : USINT;
		DieLipTempLow : USINT;
		i40_Parison_ErrID : UINT;
		oldYield : UDINT;
		SafeLogicValveDelayDone : USINT;
		SafeLogicValveOffDelayTimeOut : USINT;
	END_STRUCT;
	Machine_In_typ : 	STRUCT 
		OilPumpEnable : USINT;
		OilPumpOnoff : USINT;
		OilPumpOverload : USINT;
		OilPumpMotorErr : USINT;
		OilPumpFilter : USINT;
		OilPumpRunning : USINT;
		ParisonPumpEnable : USINT;
		ParisonPumpMotorErr : USINT;
		ParisonPumpFilter : USINT;
		ParisonPumpRunning : USINT;
		CoolingPumpFilter : USINT;
		TonnagePumpFilter : USINT;
		OilPositionNormal : USINT;
		CoolingPumpEnable : USINT;
		CoolingPumpOnoff : USINT;
		CoolingPumpRunning : USINT;
		VoltageLower : USINT;
		LowerPressureHigh : USINT;
		LowerPressureLow : USINT;
		LubriFilter : USINT;
		LubriOverload : USINT;
		HeatingPowerNormal : USINT;
		Voltage24vNormal : USINT;
		HotCutterNormal : USINT;
		PneuForceNormal : USINT;
		HydPressureSW : USINT;
		PnePressureSW : USINT;
		ExtrA_OnOff : USINT;
		ExtrA_Wire_MotorErr : USINT;
		ExtrA_MotorErr : USINT;
		ExtrA_CoolMotorErr : USINT;
		ExtrA_FrequencyErr : USINT;
		ExtrA_Speed : USINT;
		ExtrA_HopperKey : USINT;
		ExtrA_HopperOpn : USINT;
		ExtrA_LubrNormal : USINT;
		ExtrA_ColiTempNormal : USINT;
		ExtrA_NoFeed : USINT;
		ExtrB_OnOff : USINT;
		ExtrB_Wire_MotorErr : USINT;
		ExtrB_MotorErr : USINT;
		ExtrB_CoolMotorErr : USINT;
		ExtrB_FrequencyErr : USINT;
		ExtrB_Speed : USINT;
		ExtrB_HopperKey : USINT;
		ExtrB_HopperOpn : USINT;
		ExtrB_LubrNormal : USINT;
		ExtrB_ColiTempNormal : USINT;
		ExtrB_NoFeed : USINT;
		ExtrC_OnOff : USINT;
		ExtrC_Wire_MotorErr : USINT;
		ExtrC_MotorErr : USINT;
		ExtrC_CoolMotorErr : USINT;
		ExtrC_FrequencyErr : USINT;
		ExtrC_Speed : USINT;
		ExtrC_HopperKey : USINT;
		ExtrC_HopperOpn : USINT;
		ExtrC_LubrNormal : USINT;
		ExtrC_NoFeed : USINT;
		ExtrD_OnOff : USINT;
		ExtrD_Wire_MotorErr : USINT;
		ExtrD_MotorErr : USINT;
		ExtrD_CoolMotorErr : USINT;
		ExtrD_FrequencyErr : USINT;
		ExtrD_Speed : USINT;
		ExtrD_HopperKey : USINT;
		ExtrD_HopperOpn : USINT;
		ExtrD_LubrNormal : USINT;
		ExtrD_NoFeed : USINT;
		ExtrV_OnOff : USINT;
		ExtrV_Wire_MotorErr : USINT;
		ExtrV_MotorErr : USINT;
		ExtrV_CoolMotorErr : USINT;
		ExtrV_FrequencyErr : USINT;
		ExtrV_Speed : USINT;
		ExtrV_HopperKey : USINT;
		ExtrV_HopperOpn : USINT;
		ExtrV_LubrNormal : USINT;
		ExtrV_ColiTempNormal : USINT;
		ExtrV_NoFeed : USINT;
		ExtruderNormal : USINT;
		EBoxCoolMotorErr : USINT;
		TransferMotorErr : USINT;
		ExtrLiftInverterFault : USINT;
		ExtrLowerSlowLimit : USINT;
		ExtrLowerLimit : USINT;
		InstancyStop : BOOL;
		MainControl : USINT;
		AutoEnable : USINT;
		AutoStop : USINT;
		SafeModule : USINT;
		SafeLogicValve : USINT;
		SafeCalibPressure : USINT;
		SafeGate : BOOL;
		SafeGate_R1 : USINT;
		SafeGate_R2 : USINT;
		SafeGate_R3 : USINT;
		SafeGate_R4 : USINT;
		SafeGate_R5 : USINT;
		SafeGate_R6 : USINT;
		SafeGate_R7 : USINT;
		SafeGate_R8 : USINT;
		SafeGate_L1 : USINT;
		SafeGate_L2 : USINT;
		SafeGate_L3 : USINT;
		SafeGate_L4 : USINT;
		SafeGate_L5 : USINT;
		SafeGate_L6 : USINT;
		SafeGate_L7 : USINT;
		SafeGate_L8 : USINT;
		ProductInConvey : USINT;
		SafetyGateBypass : USINT;
		SafeCarriageMove : USINT;
		ParisonLength : USINT;
		ParisonLengthA : USINT;
		ParisonLengthB : USINT;
		ParisonLengthC : USINT;
		ParisonLengthD : USINT;
		Left : USINT;
		Right : USINT;
		CalibMode : USINT;
		CalibModeForceDI : USINT;
		Reset : USINT;
		AlarmReset : USINT;
		MacPlateUpLimit : USINT;
		MacPlateDnLimit : USINT;
		MacPlateProtect : USINT;
		CutterBreak : USINT;
		ProducutCnt : USINT;
		LMold : MoldIn_typ;
		Key : Machine_Key_typ;
		RMold : MoldIn_typ;
		ExtruderLiftDnLimit : USINT; (*wzq, extruder lift down limit *)
		ExtruderLiftUpLimit : USINT; (*wzq, extruder lift up limit *)
		WaterPressureLow : USINT;
		PlateUpLimit : USINT;
		PlateDnLimit : USINT;
		PlateFwBwLimit : USINT;
		AccuChargeON : USINT;
		NoFeed : USINT;
		ProductAuxClampLimit : USINT;
		PreHeatDI : USINT;
		OilPump2Overload : USINT;
		OilPump1Overload : USINT;
		OilPumpOk_DI : USINT;
		AirPressureLow : USINT;
		OilPumpErr : USINT;
		TempFanErr : USINT;
		ExtParisonCtrlEnd : USINT;
		PlateMotorOverload : USINT;
	END_STRUCT;
	Machine_Key_typ : 	STRUCT 
		LeftCtrl : USINT;
		RightCtrl : USINT;
		RobotUp : USINT;
		RobotDn : USINT;
		RobotCls : USINT;
		RobotReset : USINT;
		RobotReject : USINT;
		TopDefalshFw : USINT;
		TopDefalshBw : USINT;
		RobotTransferFw : USINT;
		RobotTransferBw : USINT;
		HotCutter : USINT;
		New_Member1 : USINT;
	END_STRUCT;
	Machine_Option_typ : 	STRUCT 
		Cut1stCycle : USINT;
		bDryCycle : USINT;
		bAutoStopPinDown : USINT;
		bTimeLubricate : USINT;
		bForwardLockUp : USINT;
		ACCPumpCharge : USINT; (*Option ACC Charge*)
		Station : USINT;
		CutterMode : USINT;
		LRobotTransfer : USINT;
		LTopDeflash : USINT;
		LBottomDeflash : USINT;
		LCoolPin : USINT;
		LCoolPin2 : USINT;
		LCoolPin3 : USINT;
		LRobotUpDn : USINT;
		RRobotTransfer : USINT;
		RTopDeflash : USINT;
		RBottomDeflash : USINT;
		RCoolPin : USINT;
		RCoolPin2 : USINT;
		RCoolPin3 : USINT;
		RRobotUpDn : USINT;
		LSubMold : USINT;
		LBottomSubmold : USINT;
		LPunchHandle : USINT;
		LScrew : USINT;
		LInnerPin : USINT;
		LCutNeck : USINT;
		LIntervalBlow : USINT;
		RSubMold : USINT;
		RBottomSubmold : USINT;
		RPunchHandle : USINT;
		RScrew : USINT;
		RInnerPin : USINT;
		RCutNeck : USINT;
		RIntervalBlow : USINT;
		HighPrsMotorEnable : USINT;
		ExhaustOnClampClose : USINT;
		LPinPart : USINT;
		RPinPart : USINT;
		OilPumpStartMode : USINT;
		ExtrLift : USINT;
		Cutter : USINT;
		RExtRobot : USINT;
		LExtRobot : USINT;
		RBlowPinClampClosingUp : USINT;
		LBlowPinClampClosingUp : USINT;
		ACCPumpCharge_Extrlift : USINT;
		RPullBottom : USINT;
		LPullBottom : USINT;
		RBlowPin : USINT;
		RBlowPin2 : USINT;
		LBlowPin : USINT;
		LBlowPin2 : USINT;
		RRobotOpnCls : USINT;
		LRobotOpnCls : USINT;
		RExtInMoldSticker : USINT;
		LExtInMoldSticker : USINT;
		LHookPin : {REDUND_UNREPLICABLE} USINT;
		RHookPin : USINT;
		New_Member5 : USINT;
		New_Member4 : USINT;
		New_Member3 : USINT;
		New_Member2 : USINT;
		New_Member1 : USINT;
		New_Member8 : USINT;
		New_Member : USINT;
	END_STRUCT;
	Machine_Out_typ : 	STRUCT 
		CarriagePower : USINT;
		ClampPower : USINT;
		BlowPinPower : USINT;
		HeatPower : USINT;
		AirPower : USINT;
		OilPumpContactor : USINT;
		OilPumpEnable : USINT;
		OilPump : USINT;
		OilCoolingPump : USINT;
		OilPump2Contactor : USINT;
		OilPump2 : USINT;
		ParisonContactor : USINT;
		ParisonPumpEnable : USINT;
		ParisonPump : USINT;
		TempProtection : USINT;
		Lubricate : USINT;
		LowerRelieveValve : USINT;
		HigherRelieveValve : USINT;
		AccuRelieveValve : USINT;
		SystemRelieveValve : USINT;
		BigPumpUnload : USINT;
		AccuChargeValve : USINT;
		TonnageEnable : USINT;
		ExtruderCooling : USINT;
		ExtrOnEnable : USINT;
		ExtrA_Enable : USINT;
		ExtrA_Power : USINT;
		ExtrA_On : USINT;
		ExtrA_CoolWaterOn : USINT;
		ExtrB_Enable : USINT;
		ExtrB_Power : USINT;
		ExtrB_On : USINT;
		ExtrB_CoolWaterOn : USINT;
		ExtrC_Enable : USINT;
		ExtrC_Power : USINT;
		ExtrC_On : USINT;
		ExtrC_CoolWaterOn : USINT;
		ExtrD_Enable : USINT;
		ExtrD_Power : USINT;
		ExtrD_On : USINT;
		ExtrD_CoolWaterOn : USINT;
		ExtrV_Enable : USINT;
		VisuExtrPower : USINT;
		ExtrVisu_On : USINT;
		ExtrV_CoolWaterOn : USINT;
		LowPressureEnable : USINT;
		HighPressureEnable : USINT;
		PneumaticValve : USINT;
		HopperGateOpenA : USINT;
		HopperCoolingA : USINT;
		HopperGateOpenB : USINT;
		HopperCoolingB : USINT;
		CutterFw : USINT;
		CutterBw : USINT;
		CutterHeating : INT;
		SupportAir : USINT;
		SupportAir_1 : USINT;
		SupportAir_2 : USINT;
		SuckAir_1 : USINT;
		SuckAir_2 : USINT;
		SuckAir_TimeVersion : USINT;
		SuckAirRelieve_TimeVersion : USINT;
		SupportAir_TimeVersion : USINT;
		SafeCalibPressure : USINT;
		AlarmRed : USINT;
		AlarmBuzzer : USINT;
		AlarmGreen : USINT;
		AlarmYellow : USINT;
		InstancyStop : USINT;
		InerloclResetLed : USINT;
		InerloclResetEnable : USINT;
		MeltPrShunt : USINT;
		SafetyGateLock : USINT;
		SafetyGateUnlock : USINT;
		ExtrLiftDn : USINT; (*albert , extrlift down*)
		ExtrLiftUp : USINT;
		ExtrLiftUpCheck : USINT;
		ExtrLowerSlow : USINT;
		TransferBeltSuck : USINT;
		WasteBelt : USINT;
		ValveFw : USINT;
		ValveBw : USINT;
		SafetyModuleReset : USINT;
		SafetyGateBypass : USINT;
		BackGateBypass : USINT;
		Running : USINT;
		AutoRun : USINT;
		LedAutoRun : USINT;
		Manual : USINT;
		Calib : USINT;
		ExtrA_Run : USINT;
		ExtrB_Run : USINT;
		VisuExtrRun : USINT;
		WaterCooling : USINT;
		WaterHeating : USINT;
		PlateUp : USINT;
		PlateDn : USINT;
		PlateFw : USINT;
		PlateBw : USINT;
		StartThick : USINT;
		L_RobotOn : USINT;
		R_RobotOn : USINT;
		LeftStation : USINT;
		LMold : MoldOut_typ;
		RMold : MoldOut_typ;
		MainMotorOut : INT;
		MinorMotorOut : INT;
		VisuMotorOut : INT;
		P_Set : REAL;
		P_Set2 : REAL; (*Albert, second pressure output set*)
		Q_Set : REAL;
		Q_Set2 : REAL;
		P_Test : REAL;
		P_Test2 : REAL; (*Albert, second pressure output test*)
		Q_Test : REAL;
		Q_Test2 : REAL;
		Q_Charge : REAL; (*Output flow only for charge acculumator *)
		P_Charge : REAL;
		ChargeHold : USINT; (*Output for charge ACC pump hold valve*)
		ChargeHold_Extrlift : USINT;
		HighPrsMotorOn : USINT; (*Output for high pressure motor *)
		MotionPower : USINT;
		LockingValve : USINT; (*Albert Locking vavle*)
		aoLiftDieLipTemp : INT; (*Albert , SCR Output*)
		aoRightDieLipTemp : INT; (*Albert. SCR Output*)
		aoDieLipTemp3 : INT; (*Chaoi , SCR Output*)
		aoDieLipTemp4 : INT; (*Chaoi , SCR Output*)
		Reserve01 : USINT;
		P_Act : REAL;
		P_Act2 : REAL;
		ExtrLiftDo : USINT;
		PneuValveOff : BOOL;
		ExtrLiftUpFast : USINT;
		OilCoolingValve : USINT;
		ImplyBuzzer : USINT;
		FrontWorkingLight : USINT;
		ExtParisonCtrl : USINT;
		AccuChargeValve_Extrlift : USINT;
		Q_Charge_Extrlift : USINT;
		P_Charge_Extrlift : USINT;
		SafePLC_DO : USINT;
	END_STRUCT;
	Machine_Para_typ : 	STRUCT 
		MoldL : Mold_Para_typ;
		MoldR : Mold_Para_typ;
		ExtrLiftLower : Extr_Lift_Lower_typ;
		Extrlift : ExtrLift_Para_typ;
		Plate : Plate_Para_typ;
		ExtruderA : Extruder_Para_typ;
		ExtruderB : Extruder_Para_typ;
		ExtruderC : Extruder_Para_typ;
		ExtruderD : Extruder_Para_typ;
		ExtruderE : Extruder_Para_typ;
		ExtruderVisu : Extruder_Para_typ;
		LengthPara : Length_Para_typ;
		SupportAirTimeVersion : SupportAirTimeVer_Para_typ;
		SupportAirTimeVersionEnable : UINT;
		SupportAirEnable : ARRAY[0..1]OF UINT;
		SupportAirVentingStrart : UINT;
		SupportAirVentingDelay : UDINT;
		SupportAirVentingTime : UDINT;
		SupportAirVentEnable : UINT;
		DieheadNum : UINT;
		StopYield : UDINT;
		bReachYieldStop : USINT;
		HopperCoolStart : REAL;
		HopperCoolStop : REAL;
		WateBeltOnTime : REAL;
		WateBeltOffTime : REAL;
		BatchSize : REAL;
		New_Member4 : USINT;
		New_Member3 : USINT;
		New_Member2 : USINT;
		New_Member1 : REAL;
		New_Member : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	Machine_Timer_typ : 	STRUCT 
		WasteBelt : timer_typ;
		LubrWork : timer_typ;
		LubrStop : timer_typ;
		ValveCleanTotal : timer_typ;
		ValveClean : timer_typ;
		LedAutoRunFlashOn : timer_typ;
		LedAutoRunFlashOff : timer_typ;
		ExtrA_OnDelay : timer_typ;
		ExtrB_OnDelay : timer_typ;
		ExtrC_OnDelay : timer_typ;
		ExtrD_OnDelay : timer_typ;
		ExtrVisu_OnDelay : timer_typ;
		AirPressureLowAlarmDly : timer_typ;
		ExtParisonCtrlOffDelay : timer_typ;
		RClampOpnValveDly : timer_typ;
		RClampClsValveDly : timer_typ;
		LClampOpnValveDly : timer_typ;
		LClampClsValveDly : timer_typ;
		RClampBtnOffDly : timer_typ;
		LClampBtnOffDly : timer_typ;
		RCarriageBtnOffDly : timer_typ;
		LCarriageBtnOffDly : timer_typ;
		RBlowPinBtnOffDly : timer_typ;
		LBlowPinBtnOffDly : timer_typ;
		RClampValveCleanTotal : timer_typ;
		RClampValveClean : timer_typ;
		LClampValveCleanTotal : timer_typ;
		LClampValveClean : timer_typ;
		RCarriageValveCleanTotal : timer_typ;
		RCarriageValveClean : timer_typ;
		LCarriageValveCleanTotal : timer_typ;
		LCarriageValveClean : timer_typ;
		RBlowPinValveCleanTotal : timer_typ;
		RBlowPinValveClean : timer_typ;
		LBlowPinValveCleanTotal : timer_typ;
		LBlowPinValveClean : timer_typ;
		ExtParison : timer_typ;
		SafeLogicValveDelay : timer_typ;
		SafeLogicValveOffDelay : timer_typ;
	END_STRUCT;
	Mbus_Comm_typ : 	STRUCT 
		ReadStatus : INT; (* read status 0*)
		ReadStatusH : INT; (* read status 1*)
		ReadCurrent : INT; (*uint:0.1A*)
		ReadFrequency : INT; (*unit:0.01Hz*)
		ReadVoltage : INT; (*unit:0.1v*)
		ReadAlarmNum : INT;
		WriteCommand : INT;
		WriteFrequency : INT; (*unit:0.01Hz*)
		WriteAckAlarm : INT;
	END_STRUCT;
	Module_Hide_typ : 	STRUCT 
		DI9371_1 : USINT;
		DI9371_2 : USINT;
		DI9371_3 : USINT;
		DI9371_4 : USINT;
		DI9371_5 : USINT;
		DI9371_6 : USINT;
		DI9371_7 : USINT;
		DI9371_8 : USINT;
		DO9322_1 : USINT;
		DO9322_2 : USINT;
		DO9322_3 : USINT;
		DO9322_4 : USINT;
		DO9322_5 : USINT;
		DO9322_6 : USINT;
		DO9322_7 : USINT;
		DO9322_8 : USINT;
		DO9322_9 : USINT;
		DO9322_10 : USINT;
	END_STRUCT;
	Module_Ok_typ : 	STRUCT 
		BR9300_1 : BOOL;
		PS9400_1 : BOOL;
		PS9400_2 : BOOL;
		BC0083_1 : BOOL;
		BC0083_2 : BOOL;
		PS2100_1 : BOOL;
		PS2100_2 : BOOL;
		AI4632_1 : BOOL;
		AI4632_2 : BOOL;
		AI4632_3 : BOOL;
		AI4632_4 : BOOL;
		AO4632_1 : BOOL;
		AO4632_2 : BOOL;
		AO4632_3 : BOOL;
		AO4632_4 : BOOL;
		AO4622_1 : BOOL;
		AO4622_2 : BOOL;
		AO4622_3 : BOOL;
		AO4622_4 : BOOL;
		ATC402_1 : BOOL;
		ATC402_2 : BOOL;
		ATC402_3 : BOOL;
		ATC402_4 : BOOL;
		ATC402_5 : BOOL;
		ATC402_6 : BOOL;
		DI9371_1 : BOOL;
		DI9371_2 : BOOL;
		DI9371_3 : BOOL;
		DI9371_4 : BOOL;
		DI9371_5 : BOOL;
		DI9371_6 : BOOL;
		DI9371_7 : BOOL;
		DI9371_8 : BOOL;
		DI9371_9 : BOOL;
		DI9371_10 : BOOL;
		DI9371_11 : BOOL;
		DI9371_12 : BOOL;
		DO8332_1 : BOOL;
		DO8332_2 : BOOL;
		DO8332_3 : BOOL;
		DO8332_4 : BOOL;
		DO8332_5 : BOOL;
		DO8332_6 : BOOL;
		DO8332_7 : BOOL;
		DO8332_8 : BOOL;
		DO8332_9 : BOOL;
		DO8332_10 : BOOL;
		DO8332_11 : BOOL;
		DO8332_12 : BOOL;
		AP3131_1 : BOOL;
		AP3131_2 : BOOL;
		AP3131_3 : BOOL;
		AP3131_4 : BOOL;
		DOF322_1 : BOOL;
		DOF322_2 : BOOL;
		DOF322_3 : BOOL;
		DOF322_4 : BOOL;
		DOF322_5 : BOOL;
		DOF322_6 : BOOL;
		DIF371_1 : BOOL;
		DIF371_2 : BOOL;
		DIF371_4 : BOOL;
		DIF371_3 : BOOL;
		PS3300_2 : BOOL;
		PS3300_1 : BOOL;
		DO9322_1 : BOOL;
		DO9322_2 : BOOL;
		DO9322_3 : BOOL;
		DO9322_4 : BOOL;
		DO9322_5 : BOOL;
		DO9322_6 : BOOL;
		DO9322_7 : BOOL;
		DO9322_8 : BOOL;
		DO9322_9 : BOOL;
		DO9322_10 : BOOL;
		XP0101_1 : BOOL;
		SO4110_1 : BOOL;
		SI8110_2 : BOOL;
		SI8110_1 : BOOL;
		SL8100_1 : BOOL;
	END_STRUCT;
	MoldIn_typ : 	STRUCT 
		CarriageLockLimit : USINT;
		CarriageSlowFwLimit : USINT;
		CarriageFwLimit : USINT;
		CarriageSlowBwLimit : USINT;
		CarriageBwLimit : USINT;
		ClampClsSlowLimit : USINT;
		ClampClsLimit : USINT;
		ClampOpnMicroLimit : USINT;
		ClampOpnSlowLimit : USINT;
		ClampOpnLimit : USINT;
		PinPlateUplimit : USINT;
		BlowPinUpLimit : USINT;
		BlowPinDnLimit : USINT;
		BlowPin2UpLimit : USINT;
		BlowPin2DnLimit : USINT;
		BottomPinUpLimit : USINT;
		BottomPinDnLimit : USINT;
		TopDeflashFwLimit : USINT;
		TopDeflashPreFwLimit : USINT;
		TopDeflashBwLimit : USINT;
		TopDeflashOpnLimit : USINT;
		TopDeflashClsLimit : USINT;
		BottomDeflashBwLimit : USINT;
		BottomDeflashFwLimit : USINT;
		RobotFwLimit : USINT;
		RobotFwSlowLimit : USINT;
		RobotBwLimit : USINT;
		RobotBwSlowLimit : USINT;
		RobotUpLimit : USINT;
		RobotDnLimit : USINT;
		RobotOpnLimit : USINT;
		RobotClsLimit : USINT;
		PinPartFwLimit : USINT;
		PinPartBwLimit : USINT;
		SubMoldOpnLimit : USINT;
		SubMoldClsLimit : USINT;
		ScrewFwLimit : USINT;
		ScrewBwLimit : USINT;
		CutNeckOpnLimit : USINT;
		CutNeckClsLimit : USINT;
		InnerPinBwLimit : USINT;
		CoolPinFwLimit : USINT;
		CoolPinBwLimit : USINT;
		CoolPinUpLimit : USINT;
		CoolPinDnLimit : USINT;
		CoolPin2UpLimit : USINT;
		CoolPin3UpLimit : USINT;
		FlashScanHigh : USINT;
		BlockBottleBwLimit : USINT;
		SealerOpnLimit : USINT;
		SafeGate : USINT;
		ParisonLength : USINT;
		RobotRef : USINT;
		LubricNormal : USINT;
		SensorUp : USINT;
		SensorDn : USINT;
		SensorBottle : USINT;
		SensorWaste : USINT;
		bCarriageUp : USINT;
		bCarriageDown : USINT;
		RobotOpnLimitL : USINT;
		RobotOpnLimitR : USINT;
		BottomSubMoldOpnLimit : USINT;
		BottomSubMoldClsLimit : USINT;
		ExtRobotUpLimit : USINT;
		AirPressureLow : USINT;
		ExtInMoldStickerBwLimit : USINT;
		BlowPinUpLimit_2 : USINT;
		ExtInMoldStickerAlarm : USINT;
		HookPinFwLimit : USINT;
		HookPinBwLimit : USINT;
	END_STRUCT;
	MoldOut_typ : 	STRUCT 
		CarriageEnable : USINT;
		ClampCls : USINT;
		ClampOpn : USINT;
		ClampClsDiff : USINT;
		ClampOpnDiff : USINT;
		ClampUnLock : USINT; (*Albert Clamp Unlock valve for release pressure*)
		ClampLocking : USINT; (*Albert Clamp Locking for certain time*)
		CarriageLock : USINT;
		BlowPinUp : USINT;
		BlowPinDn : USINT;
		BlowPinDnFast : USINT;
		BottomPinUp : USINT;
		BottomPinDn : USINT;
		TopDeflashFw : USINT;
		TopDeflashBw : USINT;
		TopDeflashOpn : USINT;
		TopDeflashCls : USINT;
		BottomDeflashFw : USINT;
		BottomDeflashBw : USINT;
		CarriageFw : USINT;
		CarriageBw : USINT;
		LowBlow : USINT;
		HighBlow : USINT;
		FreezingBlow : USINT;
		Blow : USINT;
		CoolDeflash : USINT;
		BlowDeflash : USINT;
		CoolPinFw : USINT;
		CoolPinBw : USINT;
		CoolPinDn : USINT;
		CoolPinBlow : USINT;
		InnerPinFw : USINT;
		InnerPinBlow : USINT;
		CoolPin2Dn : USINT;
		CoolPin2Blow : USINT;
		CoolPin3Dn : USINT;
		CoolPin3Blow : USINT;
		CutNeckCls : USINT;
		CutNeck : USINT;
		SubMoldOpn : USINT;
		SubMoldCls : USINT;
		ScrewFw : USINT;
		ScrewBw : USINT;
		PinPartFw : USINT;
		PinPartBw : USINT;
		ClampEjectBlow : USINT;
		SealerFw : USINT;
		RobotFw : USINT;
		RobotBw : USINT;
		RobotUp : USINT;
		RobotDn : USINT;
		RobotOpn : USINT;
		RobotCls : USINT;
		RobotOn : USINT;
		Exhaust : USINT;
		ChuteDeflashBlow : USINT;
		Lubricate : USINT;
		BlockBottle : USINT;
		MovementValve : USINT; (*Albert. movement valve *)
		ClampOpnRelease : USINT; (*Albert. release pressure of clamp valve*)
		ClampClsAux : USINT; (*Albert  Clamp Close aux. valve*)
		ClampOpnAux : USINT; (*Albert  Clamp Close aux. valve*)
		ClampMoveValve : USINT;
		ClampClsSlow : USINT; (*Albert release oil of clamp valve*)
		ReleaseOilValve : USINT; (*Albert release oil of clamp valve*)
		RobotCarriageUp : USINT; (*Albert  output singal for robot when carriage at Upper position*)
		RobotCarriageDn : USINT; (*Albert  output singal for robot when carriage at down position*)
		RobotClampCls : USINT; (*Albert output singal for robot when clamp at close position*)
		RobotClampOpn : USINT; (*Albert output singal for robot when clamp at open position*)
		TopDeflashCool : USINT; (*Albert output singal for topdeflash cooling*)
		CarriageFwHold : USINT;
		CarriageBwHold : USINT;
		CarriageAux : USINT;
		CarriageFwCalib : USINT;
		CarriageBwCalib : USINT;
		BlowPinUpMicro : USINT;
		BlowPinDnHold : USINT;
		ProductAuxClamp : USINT;
		BottomSubMoldOpn : USINT;
		BottomSubMoldCls : USINT;
		ExhaustForBlow2 : USINT;
		PneumaticOn : USINT;
		PullBottom : USINT;
		ExtInMoldStickerAllow : USINT;
		CalibValve_1 : USINT;
		CalibValve_2 : USINT;
		HookPinFw : USINT;
	END_STRUCT;
	Mold_Alarm_typ : 	STRUCT 
		ClampNotAtClsDI : USINT; (*21/111*)
		ClampNotAtOpnDI : USINT;
		CarriageNotAtFwDI : USINT;
		CarriageNotAtBwDI : USINT;
		BlowPinNotAtUpPos : USINT;
		BlowPinNotAtDnPos : USINT;
		TopDeflashNotAtBwPos : USINT;
		RobotFwClsTimeOut : USINT;
		SubMoldNotAtOpnPos : USINT;
		TopDeflashNotAtOpnPos : USINT; (*30/120*)
		CoolPinNotAtUpPos : USINT;
		CoolPin2NotAtUpPos : USINT;
		CoolPin3NotAtUpPos : USINT;
		ScrewNotAtFwPos : USINT;
		ScrewNotAtBwPos : USINT;
		PinPartNotAtFwPos : USINT;
		PinPartNotAtBwPos : USINT;
		CutNeckFwNotAtOpnPos : USINT;
		SubMoldNotAtClsPos : USINT;
		RobotNotAtFwPos : USINT; (*40/130*)
		RobotNotAtBwPos : USINT;
		RobotNotAtOpnPos : USINT;
		InnerPinNotAtBwPos : USINT;
		ParisonTimeSmall : USINT;
		TopDeflashNotAtFwPos : USINT;
		BottomDefalshTimeOut : USINT;
		ClampCalErr : USINT;
		CarriageCalErr : USINT;
		BlowPinCalErr : USINT;
		RobotNotAtUpPos : USINT; (*50/140*)
		RobotNotAtDnPos : USINT;
		ClampValveErr : USINT;
		CarriageValveErr : USINT;
		BlowPinValveErr : USINT;
		HookPinNotAtBwPos : USINT;
		HookPinNotAtFwPos : USINT;
		RobotFwDnUpBwTimeout : USINT;
		BottleNotPutDown : USINT;
		RobotTransferCalErr : USINT;
		RobotUpDnCalErr : USINT; (*60/150*)
		TopDefalshTimeOut : USINT;
		ClampLinNotOK : USINT;
		BottomDeflashNotAtBwPos : USINT;
		BottomSubMoldNotAtClsPos : USINT;
		BottomSubMoldNotAtOpnPos : USINT;
		RobotNotAtClsPos : USINT;
		BottomDeflashNotAtFwPos : USINT;
		ExtrLiftNotAtUpPos : USINT;
		CarriageLinNotOK : USINT;
		New_Member1 : USINT; (*70/160*)
		CarriageNotAtFwBwPos : USINT;
		ClampNotAtOpnClsPos : USINT;
		BlowPin2NotAtUpPos : USINT;
		BlowPin2NotAtDnPos : USINT;
		BlowPin2CalErr : USINT;
		ExtrLiftNotAtDnPos : USINT;
		ExtrliftCalErr : USINT;
		ExtrliftLinNotOK : USINT;
		ExtRobotNotAtUp : USINT;
		RobotFwTimeout : USINT; (*80/170*)
		RobotClsTimeout : USINT;
		RobotBwTimeout : USINT;
		RobotOpnTimeout : USINT;
		PullBottomTimeOut : USINT;
		ExtInMoldStickerNotAtBw : USINT;
		ExtInMoldStickerAlarm : USINT;
		ExtInMoldStickerTimeOut : USINT;
		New_Member3s : USINT;
		New_Member3x : USINT;
		New_Member5 : USINT; (*90/180*)
		New_Member21 : USINT;
		New_Member20 : USINT;
		New_Member17 : USINT;
		New_Member16 : USINT;
		New_Member15 : USINT;
		New_Member14 : USINT;
		New_Member13 : USINT;
		New_Member12 : USINT;
		New_Member11 : USINT;
		New_Member10 : USINT; (*100/190*)
		New_Member9 : USINT;
		New_Member8 : USINT;
		New_Member7 : USINT;
		New_Member6 : USINT;
		New_Member4 : USINT;
		New_Member3 : USINT;
		New_Member2 : USINT;
		New_Member23 : USINT;
		New_Member22 : USINT;
		New_Member : USINT; (*110/200*)
	END_STRUCT;
	Mold_CleanValve_typ : 	STRUCT 
		Clamp : CleanValve_typ;
		Carriage : CleanValve_typ;
		BlowPin : CleanValve_typ;
	END_STRUCT;
	Mold_Fix_typ : 	STRUCT 
		Clamp : Clamp_Fix_typ;
		Carriage : Carriage_Fix_typ;
		BlowPin : Blowpin_Fix_typ;
		BlowPin2 : Blowpin_Fix_typ;
		SubMold : SubMold_Fix_typ;
		BottomSubMold : SubMold_Fix_typ;
		TopDeflash : TopDeflash_Fix_typ;
		BottomDeflash : TopDeflash_Fix_typ;
		Blow : Blow_Fix_typ;
		SupportAir : Support_Fix_typ;
		RobotTransfer : Robot_Transfer_Fix_typ;
		RobotUpDn : Robot_UpDn_Fix_typ;
		RobotOpnCls : Robot_OpnCls_Fix_typ;
	END_STRUCT;
	Mold_Hw_typ : 	STRUCT 
		CarrFwHWSwitch : USINT; (*305/355*)
		CarrBwHWSwitch : USINT;
		CarrNetworkErr : USINT;
		CarrPowerOn : USINT;
		CarrAxisErr : USINT;
		ClampOpnHWSwitch : USINT; (*310/360*)
		ClampClsHWSwitch : USINT;
		ClampNetworkErr : USINT;
		ClampPowerOn : USINT;
		ClampAxisErr : USINT;
		BlowPinUpHWSwitch : USINT;
		BlowPinDnHWSwitch : USINT;
		BlowPinNetworkErr : USINT;
		BlowPinPowerOn : USINT;
		BlowPinAxisErr : USINT;
		SensorUpHWSwitch : USINT; (*320/370*)
		SensorDnHWSwitch : USINT;
		SensorNetworkErr : USINT;
		SensorPowerOn : USINT;
		SensorAxisErr : USINT;
		WasteNotDn : USINT;
		BottleOnConveyer : USINT;
		GripNotAtWaste : USINT;
		SensorWaste : USINT;
		SensorBottle : USINT;
		ConveyNoBottle : USINT; (*330/380*)
		BlowPin2UpHWSwitch : USINT;
		BlowPin2DnHWSwitch : USINT;
		BlowPin2NetworkErr : USINT;
		BlowPin2PowerOn : USINT;
		BlowPin2AxisErr : USINT;
		ExtrLiftUpHWSwitch : USINT;
		ExtrLiftDnHWSwitch : USINT;
		ExtrLiftNetworkErr : USINT;
		ExtrLiftPowerOn : USINT;
		ExtrLiftAxisErr : USINT; (*340/390*)
		RobotTransferFwHWSwitch : USINT;
		RobotTransferBwHWSwitch : USINT;
		RobotTransferNetworkErr : USINT;
		RobotTransferPowerOn : USINT;
		RobotTransferAxisErr : USINT;
		New_Member15 : USINT;
		New_Member14 : USINT;
		New_Member13 : USINT;
		New_Member12 : USINT;
		New_Member : USINT; (*350/400*)
		New_Member11 : USINT;
		New_Member1 : USINT;
		New_Member2 : USINT;
		New_Member3 : USINT; (*354/404*)
	END_STRUCT;
	Mold_Para_typ : 	STRUCT 
		Clamp : Clamp_Para_typ;
		Carriage : Carriage_Para_typ;
		BlowPin : Blowpin_Para_typ;
		BlowPin2 : Blowpin_Para_typ;
		SubMold : SubMold_Para_typ;
		BottomSubMold : SubMold_Para_typ;
		TopDeflash : {REDUND_UNREPLICABLE} TopDeflash_Para_typ;
		BottomDeflash : TopDeflash_Para_typ;
		Robot : Robot_Para_typ;
		RobotOpnCls : Robot_OpnCls_Para_typ;
		Screw : Screw_Para_typ;
		SupportAir : Support_Para_typ;
	END_STRUCT;
END_TYPE

(*User Para structure*)

TYPE
	Mold_Para_User_typ : 	STRUCT 
		Timer : timer_typ;
		Clamp : Clamp_User_typ;
		TimeSet : Mold_Time_User_typ;
		TimeDis : Mold_Time_User_typ;
	END_STRUCT;
	Mold_Time_typ : 	STRUCT 
		CycleAlarmTime : UDINT;
		CarriageFwAlarmTime : UDINT;
		CarriageBwAlarmTime : UDINT;
		ClampOpnAlarmTime : UDINT;
		ClampClsAlarmTime : UDINT;
		ClampLockDly : UDINT;
		BlowPinUpAlarmTime : UDINT;
		BlowPinDnAlarmTime : UDINT;
		BlowPin2UpAlarmTime : UDINT;
		BlowPin2DnAlarmTime : UDINT;
		PinPartFwAlarmTime : UDINT;
		PinPartBwAlarmTime : UDINT;
		TopDeflashBwAlarmTime : UDINT;
		TopDeflashOpnAlarmTime : UDINT;
		TopDeflashFwAlarmTime : UDINT;
		BottomDeflashBwAlarmTime : UDINT;
		RobotOpnAlarmTime : UDINT;
		RobotFwAlarmTime : UDINT;
		RobotBwAlarmTime : UDINT;
		RobotUpAlarmTime : UDINT;
		RobotDnAlarmTime : UDINT;
		CutNeckOpnAlarmTime : UDINT;
		SubMoldOpnAlarmTime : UDINT;
		SubMoldClsAlarmTime : UDINT;
		ScrewFwAlarmTime : UDINT;
		ScrewBwAlarmTime : UDINT;
		InnerPinBwAlarmTime : UDINT;
		CoolPinUpAlarmTime : UDINT;
		CoolPin2UpAlarmTime : UDINT;
		CoolPin3UpAlarmTime : UDINT;
		HandleBwAlarmTime : UDINT;
		SensorUpAlarmTime : UDINT;
		SensorDnAlarmTime : UDINT;
		ResetAlarmTime : UDINT;
		MagicEyeTime : UDINT;
		CarriageFwDelay : UDINT;
		CarriageBwDelay : UDINT;
		ClampClsDelay : UDINT;
		ClampOpnDelay : UDINT;
		ClampLockTime : UDINT;
		BlowPinDnDelay : UDINT;
		BlowPinDnTime : UDINT;
		BlowPinDnPressTime : UDINT;
		BlowPinUpDelay : UDINT;
		BlowpintinyreturnDelay : UDINT;
		BlowpintinyreturnTime : UDINT;
		BlowPin2DnDelay : UDINT;
		BlowPin2DnTime : UDINT;
		BlowPin2DnPressTime : UDINT;
		BlowPin2UpDelay : UDINT;
		Blowpin2tinyreturnDelay : UDINT;
		Blowpin2tinyreturnTime : UDINT;
		PreBlowDelay : UDINT;
		PreBlowTime : UDINT;
		TotalBlowTime : UDINT;
		BlowingDelay : UDINT;
		BlowingTime : UDINT;
		AirVentingTime : UDINT;
		CutterDelay : UDINT;
		CutterTime : UDINT;
		CutterBwTime : UDINT;
		ExtrLiftUpDelay : UDINT;
		ExtrLiftUpHoldTime : UDINT;
		ExtrLiftSlowTime : UDINT;
		ExtrLiftDnHoldTime : UDINT;
		ExtrLiftDnDelay : UDINT;
		ExtrLiftUpAlarmTime : UDINT;
		ExtrLiftDnAlarmTime : UDINT;
		DieheadBlowDelay : UDINT;
		DieheadBlowTime : UDINT;
		SupportAirStart : ARRAY[0..1]OF UDINT;
		SupportAirStop : ARRAY[0..1]OF UDINT;
		VacuumDelay : UDINT;
		VacuumTime : UDINT;
		TopDeflashFwDelay : UDINT;
		TopDeflashFwTime : UDINT;
		TopDeflashBwDelay : UDINT;
		TopDeflashBwTime : UDINT;
		TopDeflashOpnDelay : UDINT;
		TopDeflashOpnTime : UDINT;
		TopDeflashClsDelay : UDINT;
		TopDeflashClsTime : UDINT;
		BottomDeflashFwDelay : UDINT;
		BottomDeflashFwTime : UDINT;
		BlowDeflashDelay : UDINT;
		BlowDeflashTime : UDINT;
		CoolDeflashDelay : UDINT;
		CoolDeflashTime : UDINT;
		InterValBlowOnTime : UDINT;
		InterValBlowOffTime : UDINT;
		CoolPinDnDelay : UDINT;
		CoolPinDnTime : UDINT;
		CoolPinBlowDelay : UDINT;
		CoolPinBlowTime : UDINT;
		CoolPin2DnDelay : UDINT;
		CoolPin2DnTime : UDINT;
		CoolPin2BlowDelay : UDINT;
		CoolPin2BlowTime : UDINT;
		CoolPin3DnDelay : UDINT;
		CoolPin3DnTime : UDINT;
		CoolPin3BlowDelay : UDINT;
		CoolPin3BlowTime : UDINT;
		PullBottomDelay : UDINT;
		PullBottomTime : UDINT;
		CutNeckClsDelay : UDINT;
		CutNeckClsTime : UDINT;
		CutNeckDelay : UDINT;
		CutNeckTime : UDINT;
		CutNeckOpnDelay : UDINT;
		InnerPinFwDelay : UDINT;
		InnerPinFwTime : UDINT;
		InnerPinBlowDelay : UDINT;
		InnerPinBlowTime : UDINT;
		InnerPinBwDelay : UDINT;
		SubMoldOpnDelay : UDINT;
		SubMoldClsDelay : UDINT;
		SubMoldMicroOpnTime : UDINT;
		SubMoldOpnTime : UDINT;
		SubMoldClsHoldTime : UDINT;
		BottomSubMoldOpnDelay : UDINT;
		BottomSubMoldClsDelay : UDINT;
		BottomSubMoldOpnAlarmTime : UDINT;
		BottomSubMoldClsAlarmTime : UDINT;
		BottomSubMoldOpnTime : UDINT;
		BottomSubMoldClsHoldTime : UDINT;
		ScrewFwDelay : UDINT;
		ScrewBwDelay : UDINT;
		PinPartFwDelay : UDINT;
		PinPartBwDelay : UDINT;
		ChuteDeflashBlowDelay : UDINT;
		ChuteDeflashBlowTime : UDINT;
		RobotClsDly : UDINT;
		RobotClsTime : UDINT;
		RobotOpnDly : UDINT;
		RobotOpnTime : UDINT;
		RobotFwDly : UDINT;
		RobotFwTime : UDINT;
		RobotBwDly : UDINT;
		RobotBwTime : UDINT;
		RobotUpDly : UDINT;
		RobotUpTime : UDINT;
		RobotDnDly : UDINT;
		RobotDnTime : UDINT;
		RobotDnTimeA : UDINT;
		BottomPinUpDelay : UDINT;
		BottomPinDnDelay : UDINT;
		BlowPinDnFastTime : {REDUND_UNREPLICABLE} UDINT;
		TopDeflashPreFwTime : UDINT;
		LowBlowDelay : UDINT;
		LowBlowTime : UDINT;
		ScrewMicroOpnTime : UDINT;
		MoldClsWait : UDINT;
		SealerDelay : UDINT;
		SealerTime : UDINT;
		ClampMircoOpnDelay : UDINT;
		RobotClsAlarmTime : UDINT;
		ProductAuxClampClsDelay : UDINT;
		BottomDeflashFwAlarmTime : UDINT;
		BlowingTime2 : UDINT;
		AirVentingTime2 : UDINT;
		BottomDeflashBwDelay : UDINT;
		BottomDeflashBwTime : UDINT;
		SupportAirDelay : UDINT;
		SupportAirTime : UDINT;
		SupportAirSecDelay : UDINT;
		SupportAirSecTime : UDINT;
		BlowPinDnPress2HoldTime : UDINT;
		ClampClsPress2HoldTime : UDINT;
		HookPinFwDelay : UDINT;
		HookPinBwDelay : UDINT;
		HookPinFwAlarmTime : UDINT;
		HookPinBwAlarmTime : UDINT;
		New_Member1 : UDINT;
		New_Member2 : UDINT;
		New_Member3 : UDINT;
		New_Member5 : UDINT;
		New_Member7 : UDINT;
		New_Member8 : UDINT;
		New_Member9 : UDINT;
		New_Member6 : UDINT;
		New_Member4 : UDINT;
	END_STRUCT;
	Mold_Time_User_typ : 	STRUCT 
		ExtrLiftDnDelay : UDINT; (*Albert Extruderlift down delay*)
		ClampHoldingTime : UDINT;
		ClampUnLockTime : UDINT; (*Albert UnLock Time for clamp*)
		BottomDeflashSecDelay : UDINT; (*Albert Second Topdeflash delay time*)
		BottomDeflashSecTime : UDINT; (*Albert Second Topdeflash forward time*)
		TopDeflashSecDelay : UDINT; (*Albert Second Topdeflash delay time*)
		TopDeflashSecTime : UDINT; (*Albert Second Topdeflash forward time*)
		SupportAirSecDelay : UDINT; (*Albert Second SupportAir Delay time*)
		SupportAirSecTime : UDINT; (*Albert Second SupportAir Blow time*)
		CarriageBwHoldTime : UDINT;
		CarriageFwHoldTime : UDINT;
		TopDeflashExtraDelay : UDINT;
		BottomDeflashExtraDelay : UDINT;
		SafeLogicValveDelay : UDINT;
		SafeLogicValveOffDelay : UDINT;
		New_Member2 : UDINT;
		New_Member9 : UDINT;
	END_STRUCT;
	Mold_typ : 	STRUCT 
		AutoStep : UINT;
		StopAutoStep : UINT;
		ResetStep : UINT;
		PinCalibStep : UINT;
		OtherMoldBw : USINT;
		InBasicPos : USINT;
		SemiAuto : USINT;
		AutoReqStop : USINT;
		CurCycEnd : USINT;
		NextCycStart : USINT;
		OnceCut : BOOL;
		OnceSupport : BOOL;
		OncePin2ndDn : BOOL;
		FirstCut : BOOL;
		OnceSubMoldIn : USINT;
		OnceBlowPinUp : USINT;
		OnceReserve : USINT;
		AutorunFirstCycle : BOOL;
		Carriage : Carriage_typ;
		Clamp : Clamp_typ;
		BlowPin : Blowpin_typ;
		BlowPin2 : Blowpin_typ;
		BottomPin : Action_typ;
		SubMold : SubMold_typ;
		PinPart : Action_Limit_typ;
		Sealer : Action_typ;
		Screw : Screw_typ;
		Lock : ClampLock_typ;
		Cutter : Action_typ;
		ExtrLift : ExtrLift_typ;
		ExtrLiftDn : Action_typ;
		LowBlow : Blow_typ;
		HighBlow : Blow_typ;
		TopDeflash : TopDeflash_typ;
		BottomDeflash : TopDeflash_typ;
		SupportAir : Support_typ;
		SupportSuckAir : Action_typ;
		CoolPin : Action_Limit_typ;
		CoolPinBlow : Action_typ;
		CoolPin2 : Action_Limit_typ;
		CoolPin2Blow : Action_typ;
		CoolPin3 : Action_Limit_typ;
		CoolPin3Blow : Action_typ;
		HookPin : Action_Limit_typ;
		BlowDeflash : Action_typ;
		CoolDeflash : Action_typ;
		PullBottom : Action_typ;
		InnerPin : Action_typ;
		CutNeck : Action_typ;
		Lubricate : Action_typ;
		Robot : Robot_typ;
		RobotOpnCls : Robot_OpnCls_typ;
		Sensor : Sensor_typ;
		ChuteDeflashBlow : Action_typ;
		LengthCtrl : LengthCtrl_typ;
		Option : Option_typ;
		ValveDelay : Valve_Delay_typ;
		TimeSet : Mold_Time_typ;
		TimeDis : Mold_Time_typ;
		ActInfo : Act_Info_typ;
		Alarm : Mold_Alarm_typ;
		HwAlarm : Mold_Hw_typ;
		Timer : timer_typ;
		TimeCycle : timer_typ;
		TransDIn : TansDucer_In_typ;
		ValveOut : Vlave_Out_typ;
		Plate : Plate_typ;
		BottomSubMold : SubMold_typ;
	END_STRUCT;
	MotorTraceData_typ : 	STRUCT 
		Position : REAL;
		Current : REAL;
		RPM : REAL;
		Torque : REAL;
		Max_Torque_Set : REAL;
	END_STRUCT;
	MotorTrace_Mold_typ : 	STRUCT 
		Clamp : MotorTraceData_typ;
		BlowPin : MotorTraceData_typ;
		BlowPin2 : MotorTraceData_typ;
		Carriage : MotorTraceData_typ;
		RobotTransfer : MotorTraceData_typ;
	END_STRUCT;
	MotorTrace_typ : 	STRUCT 
		Extrlift : MotorTraceData_typ;
		Parison : MotorTraceData_typ;
		RMold : MotorTrace_Mold_typ;
		LMold : MotorTrace_Mold_typ;
	END_STRUCT;
	Oil_Accu_typ : 	STRUCT 
		rawOffset : DINT;
		AccuPressure10V : REAL;
		AccuPressureStart : REAL;
		AccuPressureStop : REAL;
		AccuDelayTim : REAL;
		AccuTimeout : REAL;
		SafeGain : REAL;
		AccuFlow : REAL;
		AccuChargePressure : REAL;
	END_STRUCT;
	Oil_Cooling_typ : 	STRUCT 
		OilTempAlarm : REAL;
		OilTempCoolingStart : REAL;
		OilTempCoolingStop : REAL;
		OilTempHeatingStart : REAL;
		OilTempHeatingStop : REAL;
		Reserve1_REAL : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	Option_Fix_typ : 	STRUCT 
		HWEnable : HWEnable_Fix_typ;
		AlarmLedMode : BOOL;
		AlarmTextMode : BOOL;
		bBlowModeCBM : BOOL;
		bLowBlow : BOOL;
		bIntervalBlow : BOOL;
		bHideLeftStation : BOOL;
		bExtParisonCtrl : BOOL;
		bExtruderMotorCooling : BOOL;
		bHopperCooling : BOOL;
		bHandledeflashRelpy : BOOL;
		bTopDeflashMode : BOOL;
		bProductAuxClamp : BOOL;
		bBlowUpDuringClampCls : BOOL;
		bCoolPinUpNextCycClampCls : BOOL;
		bCutterHeating : BOOL;
		bExtrLiftUpFastValve : BOOL;
		bSupportAirVentTimeVersion : BOOL;
		bPlateFwAfterClampHold : BOOL;
		bDieLip : ARRAY[0..7]OF BOOL;
		bPlateFwBw : BOOL;
		bClampPropValveOutputInverse : BOOL;
		bCarriagePropValveOutputInverse : BOOL;
		bBlowpinPropValveOutputInverse : BOOL;
		bRobotFwAfterTopdeflash : BOOL;
		bBlowingTime2LowBlow : BOOL;
		bOilAccu_Extrlift_DisChar_Calib : BOOL;
		res2_BOOL5 : BOOL;
		res2_BOOL6 : BOOL;
		res1_BOOL4 : BOOL;
		res2_BOOL7 : BOOL;
		res0_BOOL : BOOL;
	END_STRUCT;
	Option_typ : 	STRUCT 
		Enable : USINT;
		CutterMode : USINT;
		Cutter : USINT;
		Parison : USINT;
		PinOpenUp : USINT;
		TopDeflash : USINT;
		BottomDeflash : USINT;
		PunchHandle : USINT;
		PullBottom : USINT;
		InnerPin : USINT;
		SubMold : USINT;
		Screw : USINT;
		CoolPin : USINT;
		CoolPin2 : USINT;
		CoolPin3 : USINT;
		CutNeck : USINT;
		PinPart : USINT;
		IntervalBlow : USINT;
		ExtRobot : USINT;
		RobotTransfer : USINT;
		RobotOpnCls : USINT;
		RobotUpDn : USINT;
		Sealer : USINT;
		BottomPin : USINT;
		BottomSubMold : USINT;
		ExtrLift : USINT;
		BlowPinClampClosingUp : USINT;
		BlowPin : USINT;
		BlowPin2 : USINT;
		ExtInMoldSticker : USINT;
		HookPin : USINT;
		New_Member2 : USINT;
		New_Member1 : USINT;
		New_Member5 : USINT;
		New_Member : USINT;
	END_STRUCT;
	Panel_Basic_typ : 	STRUCT 
		GetPicture : UINT;
		SetPicture : UINT;
		OldPicture : UINT;
		PasswordLevel : USINT;
		LevelBand_1 : USINT;
		LevelBand_2 : USINT;
		LevelBand_3 : USINT;
		LevelBand_4 : USINT;
		GlobalColor : UINT;
		FocusColor : UINT;
		SetKeyLevel : USINT;
		GetKeyLevel : USINT;
		SetLanguage : USINT;
		GetLanguage : USINT;
		idxKeyPicture : ARRAY[0..MAX_KEY_COUNT]OF UINT;
		VKeyMatric : ARRAY[0..MAX_KEY_COUNT]OF USINT;
		RKeyMatric : ARRAY[0..MAX_KEY_COUNT]OF USINT;
		MKeyMatric : ARRAY[0..MAX_KEY_COUNT]OF USINT;
		KeyMatric : ARRAY[0..MAX_KEY_COUNT]OF USINT;
		LedMatric : ARRAY[0..MAX_KEY_COUNT]OF USINT;
		TempCPU : UINT;
		TempCoolPlate : UINT;
		VisualName : STRING[40];
		vcHandle : UDINT;
		CurrentTime : STRING[20];
		PasswordHigh : STRING[12];
		Password4 : STRING[12];
		Ctrl : hmi_ctrl_typ;
		LevelHide2 : USINT;
		LevelHide3 : USINT;
		LevelHide4 : USINT;
		LevelHide1 : USINT;
		XP0101_1 : ARRAY[0..7]OF BOOL;
	END_STRUCT;
	Parison_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL2 : REAL;
		Reserve0_REAL3 : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	Plate_Fix_typ : 	STRUCT  (*Albert TopDeflash action type*)
		eActuatorType : actuator_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pCalibUp : REAL;
		pCalibDn : REAL;
		pCalibFw : REAL;
		pCalibBw : REAL;
		vCalibUp : REAL;
		vCalibDn : REAL;
		vCalibFw : REAL;
		vCalibBw : REAL;
		bFwBwPressureOutputLeft : BOOL;
		bUpDnPressureOutputLeft : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL3 : REAL;
		Reserve0_REAL4 : REAL;
		Reserve0_REAL2 : REAL;
	END_STRUCT;
	Plate_Para_typ : 	STRUCT  (*Albert Plate type*)
		V_Fw : REAL;
		V_Bw : REAL;
		P_Fw : REAL;
		P_Bw : REAL;
		New_Member1 : REAL;
		Reserve0_REAL2 : REAL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL3 : REAL;
		New_Member : REAL;
	END_STRUCT;
	Plate_typ : 	STRUCT  (*Albert Plate action type*)
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
		Out : USINT;
	END_STRUCT;
	Process_Status_typ : 	STRUCT 
		mode_req : USINT;
		mode : USINT;
		manual_req : USINT;
		semi_auto : USINT;
		moduleErr : USINT;
		extruderStopEnable : USINT;
		oilpumpStop : USINT;
	END_STRUCT;
	Profile_typ : 	STRUCT 
		FunOption : Thick_Function_typ;
		ShareSet : Share_Set_typ;
		AccuSet : Base_Pos_typ;
		TimSet : Base_Time_typ;
		IsMarker : ARRAY[0..99]OF USINT;
	END_STRUCT;
	Project_Info_typ : 	STRUCT 
		MachineSeriesNumber : STRING[20];
		SoftwareVersion : STRING[20];
		ProgramVersion : STRING[20];
		Address3 : USINT;
		Address2 : USINT;
		Address1 : USINT;
		Address0 : USINT;
	END_STRUCT;
	Pump_Fix_typ : 	STRUCT 
		AccuOffDealy : UDINT;
		PumpOnDealy : UDINT;
		PumpOffDealy : UDINT;
		Reserve5 : UDINT;
		LinP1 : ARRAY[0..20]OF Hyd_Node_typ; (*for PQ valve*)
		LinQ1 : ARRAY[0..20]OF Hyd_Node_typ; (*for PQ valve*)
		MaxP1 : REAL; (*for PQ valve*)
		MaxQ1 : REAL; (*for PQ valve*)
		P_Calib : REAL; (*for PQ valve / servo pump*)
		P_Idle : REAL; (*for servo pump*)
		Q_Idle : REAL; (*for servo pump*)
		accel_P : REAL; (*for servo pump*)
		percentMaxQ : REAL; (*for servo pump*)
		percentMaxP : REAL; (*for servo pump*)
		percentCarriageQ : REAL; (*for servo pump*)
		P_CleanValve : REAL;
		Q_CleanValve : REAL;
		MaxP2 : REAL; (*Albert, for test pump of pressure*)
		Reserve2 : REAL;
		Reserve1 : REAL;
		Reserve0 : REAL;
	END_STRUCT;
	pv_get_typ : 	STRUCT 
		modname : STRING[19];
		errorNum : UINT;
		errorInfo : UINT;
		line_no : UINT;
		length : UDINT;
	END_STRUCT;
	Pwd_typ : 	STRUCT 
		PwdInput : STRING[12];
		HisPwdInput : STRING[12];
		Level : ARRAY[0..3]OF STRING[12];
		PasswordSet : ARRAY[0..3]OF STRING[12];
		HisPasswordSet : ARRAY[0..3]OF STRING[12];
		Pwd_1 : STRING[12];
		Pwd_2 : STRING[12];
		Pwd_3 : STRING[12];
		length_1 : UDINT;
		length_2 : UDINT;
		length_3 : UDINT;
		Option : ARRAY[0..2]OF USINT;
		OK_1 : USINT;
		OK_2 : USINT;
		OK_3 : USINT;
		CompletionPasswordSet : ARRAY[0..3]OF USINT;
		CompletionHisPasswordSet : ARRAY[0..3]OF USINT;
		FocusPasswordSet : ARRAY[0..3]OF UINT;
		FocusHisPasswordSet : ARRAY[0..3]OF UINT;
		StatusOfPasswordSet : UINT;
		FlagSet : UINT;
		PwdLevel : UINT;
		wSavePassword : USINT;
		rReadPassword : USINT;
		Language : UINT;
	END_STRUCT;
	Recipe_Operate_typ : 	STRUCT 
		deviceNameUSB : STRING[80];
		bHideUSB : USINT;
		usbDiskOk : USINT;
		Index : UINT;
		CurveIni : UINT;
		CurveLoad : UINT;
		CurveSave : UINT;
		DataIni : UINT;
		DataLoad : UINT;
		DataSave : UINT;
		OnceIni : UINT;
		OnceLoad : UINT;
		OnceSave : UINT;
		NameLoad : UINT;
		NameSave : UINT;
		CfgLoad : UINT;
		NameOk : UINT;
		DataOk : UINT;
		CurveOk : UINT;
		OnceOk : UINT;
		CfgOk : UINT;
		DataRecipeLoad : USINT;
		DataRecipeSave : USINT;
		DataRecipeClear : USINT;
		CurveRecipeLoad : USINT;
		CurveRecipeSave : USINT;
		DataUSBLoad : USINT;
		DataUSBSave : USINT;
		OnceUSBLoad : USINT;
		OnceUSBSave : USINT;
		CurveUSBLoad : USINT;
		CurveUSBSave : USINT;
		DataUSBLoadAll : USINT;
		DataUSBSaveAll : USINT;
		CurveUSBLoadAll : USINT;
		CurveUSBSaveAll : USINT;
		usbFixOk : USINT;
		usbMoldOk : USINT;
		usbCurveOk : USINT;
	END_STRUCT;
	Robot_OpnCls_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pCalibOpn : REAL;
		pCalibCls : REAL;
		vCalibOpn : REAL;
		vCalibCls : REAL;
		bEnableRobotClsLimit : BOOL;
		bEnableRobotTwoOpnLimit : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		reserve3 : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_OpnCls_Para_typ : 	STRUCT 
		S_Opn : REAL;
		P_Opn : REAL;
		V_Opn : REAL;
		S_Cls : REAL;
		P_Cls : REAL;
		V_Cls : REAL;
		reserve3 : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_OpnCls_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
		RobotFwTimeOutFlag : USINT;
		RobotBwTimeOutFlag : USINT;
	END_STRUCT;
	Robot_Para_typ : 	STRUCT 
		ParaTransfer : Robot_Transfer_Para_typ;
		ParaUpDn : Robot_UpDn_Para_typ;
		reserve3 : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_Transfer_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		sHome : REAL;
		accel : REAL;
		decel : REAL;
		pMax : REAL;
		pCalibMax : REAL;
		pCalibZero : REAL;
		vCalibMax : REAL;
		vCalibZero : REAL;
		sMax : REAL;
		Tolerance : REAL;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		reserve4 : REAL;
		reserve3 : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_Transfer_Para_typ : 	STRUCT 
		V_Fw : REAL;
		P_Fw : REAL;
		V_FwSlow : REAL;
		P_FwSlow : REAL;
		S_Fw : REAL;
		V_WasteFw : REAL;
		S_WasteFw : REAL;
		V_Bw : REAL;
		P_Bw : REAL;
		V_BwSlow : REAL;
		P_BwSlow : REAL;
		S_Bw : REAL;
		V_BwReset : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_Transfer_typ : 	STRUCT 
		CalOK : BOOL;
		NetworkInit : BOOL;
		AxisReady : BOOL;
		HomingOk : BOOL;
		Error : BOOL;
		FwHWSwitch : BOOL;
		BwHWSwitch : BOOL;
		InitPower : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		Home : BOOL;
		MoveAbsolute : BOOL;
		MoveJogPos : BOOL;
		MoveJogNeg : BOOL;
		Stop : BOOL;
		FwPos : BOOL;
		BwPos : BOOL;
		WastePos : BOOL;
		ecAs : DINT;
		mmAs : REAL;
		v_act : REAL;
		p_act : REAL;
		p_set : REAL;
		v_set : REAL;
		s_set : REAL;
	END_STRUCT;
	Robot_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		bHomeOk : USINT;
		Transfer : Robot_Transfer_typ;
		UpDn : Robot_UpDn_typ;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		Out : UINT;
		reserve01 : USINT;
	END_STRUCT;
	Robot_UpDn_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		eValveType : valve_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		vMoldDn : REAL;
		sMoldDn : REAL;
		vNormalDn : REAL;
		sNormalDn : REAL;
		vWasteDn : REAL;
		sWasteDn : REAL;
		vHome : REAL;
		sHome : REAL;
		vMoldUp : REAL;
		vNormalUp : REAL;
		sUp : REAL;
		accel : REAL;
		decel : REAL;
		vMax : REAL;
		sMaxMold : REAL;
		sMax : REAL;
		Tolerance : REAL;
		sSensorMax : REAL;
		vSensorHome : REAL;
		sSensorBottle : REAL;
		sSensorWaste : REAL;
		vSensor : REAL;
		sTransferBeltStart : REAL;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		reserve3 : REAL;
		reserve2 : REAL;
		reserve1 : REAL;
		reserve0 : REAL;
	END_STRUCT;
	Robot_UpDn_Para_typ : 	STRUCT 
		vMoldDn : REAL;
		sMoldDn : REAL;
		vNormalDn : REAL;
		sNormalDn : REAL;
		vWasteDn : REAL;
		sWasteDn : REAL;
		vHome : REAL;
		sHome : REAL;
		vMoldUp : REAL;
		vNormalUp : REAL;
		sUp : REAL;
		sMax : REAL;
		Tolerance : REAL;
		sSensorMax : REAL;
		vSensorHome : REAL;
		sSensorBottle : REAL;
		sSensorWaste : REAL;
		vSensor : REAL;
		sTransferBeltStart : REAL;
		Reserve0_REAL : REAL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL2 : REAL;
	END_STRUCT;
	Robot_UpDn_typ : 	STRUCT 
		CalOK : BOOL;
		network : BOOL;
		AxisReady : BOOL;
		HomingOk : BOOL;
		Error : BOOL;
		FwHWSwitch : BOOL;
		BwHWSwitch : BOOL;
		InitPower : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		Home : BOOL;
		MoveAbsolute : BOOL;
		MoveJogPos : BOOL;
		MoveJogNeg : BOOL;
		Stop : BOOL;
		UpPos : BOOL;
		DnPos : BOOL;
		ecAs : DINT;
		mmAs : REAL;
		v_act : REAL;
		p_act : REAL;
		v_set : REAL;
		s_set : REAL;
	END_STRUCT;
	ScreenShotObj_typ : 	STRUCT 
		CmdScreenShot : USINT;
		CmdScreenShotAll : USINT;
		ScreenShotBtnStatus : UINT;
		Status : UINT;
		errorNumber : UINT;
		ScreenShotRun : USINT;
	END_STRUCT;
	Screw_Para_typ : 	STRUCT 
		SsFw : REAL;
		SpFw : REAL;
		SvFw : REAL;
		SsBw : REAL;
		SpBw : REAL;
		SvBw : REAL;
		Reserve3_REAL : REAL;
		Reserve2_REAL : REAL;
		Reserve1_REAL : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	Screw_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
	END_STRUCT;
	Sensor_typ : 	STRUCT 
		Step : UINT;
		ZeroPos : USINT;
		BottlePos : USINT;
		WastePos : USINT;
		UpDn : Robot_UpDn_typ;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		vRamp : HydSCurveRamp;
	END_STRUCT;
	Share_Set_typ : 	STRUCT 
		Gain : UINT;
		SealGap : UINT;
		SealFlow : UINT;
		StartFlow : UINT;
		ThickFlow : UINT;
		OpenGapTime : UINT;
		SealGapTime : UINT;
		MarkerTime : UINT;
		ServoValveComp : INT;
		Change : INT;
		Wave : INT;
		New_Member : REAL;
		New_Member1 : REAL;
	END_STRUCT;
	SPC_Mold_Ctrl_typ : 	STRUCT 
		tCycle : USINT;
		tCarriageFw : USINT;
		tCarriageBw : USINT;
		tCalmpOpn : USINT;
		tCalmpCls : USINT;
		tBlowPinUp : USINT;
		sCarriageFw : USINT;
		res : USINT;
	END_STRUCT;
END_TYPE

(*heating Plan type*)

TYPE
	SPC_Mold_typ : 	STRUCT 
		tCycle : REAL;
		tCarriageFw : REAL;
		tCarriageBw : REAL;
		tCalmpOpn : REAL;
		tCalmpCls : REAL;
		tBlowPinUp : REAL;
		sCarriageFw : REAL;
		bCleanCycle : REAL;
	END_STRUCT;
	SPC_Unit_typ : 	STRUCT 
		CycleNo : UDINT;
		OilTemp : REAL; (*oil temperature*)
		MoldL : SPC_Mold_typ; (*left station data*)
		MoldR : SPC_Mold_typ; (*right station data*)
	END_STRUCT;
	SubMold_Fix_typ : 	STRUCT 
		Zero : DINT;
		Max : DINT;
		Length : REAL;
		Tolerance : REAL;
		Open : Hyd_Fix_typ;
		Close : Hyd_Fix_typ;
		eActuatorType : actuator_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pMax : REAL;
		pCalibMax : REAL;
		pCalibZero : REAL;
		vCalibMax : REAL;
		vCalibZero : REAL;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		Reserve1_REAL : REAL;
		Reserve0_REAL2 : REAL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	SubMold_Para_typ : 	STRUCT 
		S_Opn : REAL;
		P_Opn : REAL;
		V_Opn : REAL;
		S_Cls : REAL;
		P_Cls : REAL;
		V_Cls : REAL;
		P_ClsKeep : REAL;
		V_ClsKeep : REAL;
		P_Reset : REAL;
		V_Reset : REAL;
		Reserve1_REAL1 : REAL;
		Reserve0_REAL1 : REAL;
		Reserve1_REAL : REAL;
		Reserve0_REAL : REAL;
	END_STRUCT;
	SubMold_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
	END_STRUCT;
	SupportAirTimeVer_Para_typ : 	STRUCT 
		Enable : USINT;
	END_STRUCT;
	Support_Fix_typ : 	STRUCT 
		eSupportAirMode : supportair_mode_enum;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		New_Member1 : REAL;
		New_Member3 : REAL;
		New_Member2 : REAL;
		New_Member : REAL;
	END_STRUCT;
	Support_Para_typ : 	STRUCT 
		SetPre_1 : REAL;
		SetFlowA1 : REAL;
		SetFlowB1 : REAL;
		SetFlowC1 : REAL;
		SetFlowD1 : REAL;
		SetPre_2 : REAL;
		SetFlowA2 : REAL;
		SetFlowB2 : REAL;
		SetFlowC2 : REAL;
		SetFlowD2 : REAL;
		res1 : REAL;
		Reserve0_REAL : REAL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL2 : REAL;
		res0 : REAL;
	END_STRUCT;
	Support_typ : 	STRUCT 
		Step : UINT;
		aiFlowDieA : INT;
		aiFlowDieB : INT;
		aiFlowDieC : INT;
		aiFlowDieD : INT;
		FlowDieA : REAL;
		FlowDieB : REAL;
		FlowDieC : REAL;
		FlowDieD : REAL;
		Alarm : USINT;
		bHomeOk : USINT;
		cntTime : UDINT;
		Timer : timer_typ;
		PressureSet : REAL;
		FlowSetDieA : REAL;
		FlowSetDieB : REAL;
		FlowSetDieC : REAL;
		FlowSetDieD : REAL;
		Out_1 : UINT;
		Out_2 : UINT;
		Out_TimeVersion : USINT;
	END_STRUCT;
	TansDucer_In_typ : 	STRUCT 
		CarriageFwLimit : USINT;
		CarriageBwLimit : USINT;
		CarriageSlowFwLimit : USINT;
		CarriageSlowBwLimit : USINT;
		ClampClsSlowLimit : USINT;
		ClampClsLimit : USINT;
		ClampOpnMicroLimit : USINT;
		ClampOpnSlowLimit : USINT;
		ClampOpnLimit : USINT;
		BlowPinUpLimit : USINT;
		BlowPinDnLimit : USINT;
		BlowPin2UpLimit : USINT;
		BlowPin2DnLimit : USINT;
		BottomPinUpLimit : USINT;
		BottomPinDnLimit : USINT;
		TopDeflashFwLimit : USINT;
		TopDeflashBwLimit : USINT;
		TopDeflashOpnLimit : USINT;
		TopDeflashClsLimit : USINT;
		BottomDeflashBwLimit : USINT;
		BottomDeflashMwLimit : USINT;
		BottomDeflashFwLimit : USINT;
		RobotFwLimit : USINT;
		RobotFwSlowLimit : USINT;
		RobotBwLimit : USINT;
		RobotBwSlowLimit : USINT;
		RobotUpLimit : USINT;
		RobotDnLimit : USINT;
		RobotOpnLimit : USINT;
		PinPartFwLimit : USINT;
		PinPartBwLimit : USINT;
		SubMoldOpnLimit : USINT;
		SubMoldClsLimit : USINT;
		ScrewFwLimit : USINT;
		ScrewBwLimit : USINT;
		CutNeckOpnLimit : USINT;
		CutNeckClsLimit : USINT;
		CoolPinUpLimit : USINT;
		CoolPin2UpLimit : USINT;
		CoolPin3UpLimit : USINT;
		InnerPinBwLimit : USINT;
		SealOpnLimit : USINT;
		SensorUp : USINT;
		SensorDn : USINT;
		RobotOpnLimitL : USINT;
		RobotOpnLimitR : USINT;
		BottomSubMoldOpnLimit : USINT;
		BottomSubMoldClsLimit : USINT;
		ExtRobotUpLmit : USINT;
		ExtruderLiftDnLimit : USINT;
		ExtruderLiftUpLimit : USINT;
		RobotClsLimit : USINT;
		ExtInMoldStickerBwLimit : USINT;
		ExtInMoldStickerAlarm : USINT;
		HookPinFwLimit : USINT;
		HookPinBwLimit : USINT;
	END_STRUCT;
	task_status_type : 	STRUCT 
		Active : UDINT;
		Status : USINT;
		Step : USINT;
		Stop : USINT;
		Start : USINT;
	END_STRUCT;
	Temp_Ctrl_typ : 	STRUCT 
		Fix : Zones_Fix_typ; (*Fix parameter*)
		Par : Zones_Par_typ; (*Mold parameter*)
		Ctl : Zones_Ctrl_typ; (*Ctrl variable*)
		Status : Zones_Status_typ; (*Status*)
		GrpSwh : Grp_Ctrl_typ; (*group control*)
	END_STRUCT;
	Thick_Data_typ : 	STRUCT 
		ServoAct : DINT;
		CurrentPos : DINT;
		Counter : DINT;
		SetData : ARRAY[0..99]OF DINT;
		RealData : ARRAY[0..99]OF INT;
		PosData : ARRAY[0..100]OF DINT;
		PurgePos : DINT;
		InitLock : ARRAY[0..99]OF USINT;
		FocusNum : USINT;
		IsZeroDn : USINT;
		CycleTimes : DINT;
		CurrentTime : UDINT;
		TimeOpenDly : DINT;
		AccuOpenDly : DINT;
		SealGapDly : DINT;
		TimePerDot : DINT;
		PointLength : DINT;
		ServoMin : INT;
		ServoMax : INT;
		ServoLength : INT;
		SealPos : INT;
		StartServoPos : INT;
		ControlOk : USINT;
		ecLength : DINT;
		SetPID : INT;
		Offset : DINT;
	END_STRUCT;
	Thick_Fix_typ : 	STRUCT 
		ecMin : DINT;
		ecMax : DINT;
		CylinderLength : REAL;
		pCalib : REAL;
		vCalib : REAL;
		vCalib_bypass : REAL;
		res4_REAL : REAL;
		res3_REAL : REAL;
		res2_REAL : REAL;
		res1_REAL : REAL;
		res0_REAL : REAL;
	END_STRUCT;
	Thick_Function_typ : 	STRUCT 
		IsAccu : USINT;
		IsZeroDn : USINT;
		AutoTrace : USINT;
		IsAutoCalibrate : USINT;
		Is100_300 : USINT;
		IsTraceInEdit : USINT;
		Servo : USINT;
		SealMode : USINT;
		bLengthUnit : USINT;
		bMoveFrame : USINT;
		Reserve5_USINT : USINT;
		Reserve4_USINT : USINT;
		Reserve3_USINT : USINT;
		Reserve2_USINT : USINT;
		Reserve1_USINT : USINT;
		Reserve0_USINT : USINT;
	END_STRUCT;
	Thick_In_typ : 	STRUCT 
		StartThick : USINT;
		SealGap : USINT;
		Purge : USINT;
		Stop : USINT;
	END_STRUCT;
	Thick_IO_typ : 	STRUCT 
		enable : BOOL;
		NetworkInit : BOOL;
		BtnPowerOn : BOOL;
		PowerOn : BOOL;
		UpHWSwitch : BOOL;
		DnHWSwitch : BOOL;
		InitPower : BOOL;
		HomeOK : BOOL;
		AxisReady : BOOL;
		Error : BOOL;
		cmdHome : BOOL;
		cmdStop : BOOL;
		HomingOk : BOOL;
		ExtruerPermitOn : BOOL;
		AutoRun : USINT;
		Step : UINT;
		ecAs : DINT;
		mmAsDrive : REAL;
		mmAs : REAL;
		SetPercent : REAL;
		ActPercent : REAL;
		vMax : REAL;
		StartPosition : DINT;
		ecMin : DINT;
		ecMax : DINT;
		CalOK : USINT;
		Out : Thick_Out_typ;
		In : Thick_In_typ;
		tSealGapDly : REAL;
		ServoOut : INT;
		p_set : REAL;
		v_limit : REAL;
		v_set : REAL;
		sPosition : DINT;
		p_act : REAL;
		v_act : REAL;
	END_STRUCT;
	Thick_Out_typ : 	STRUCT 
		EmptyPos : USINT;
		FullPos : USINT;
		MarkPulse : USINT;
		ControlEnd : USINT;
		Start : USINT;
		End : USINT;
	END_STRUCT;
	timer_typ : 	STRUCT 
		IN : USINT;
		Q : USINT;
		PT : UDINT;
		ET : UDINT;
		PT_ms : UDINT;
		ET_ms : UDINT;
	END_STRUCT;
	TopDeflash_Fix_typ : 	STRUCT 
		eActuatorType : actuator_typ_enum;
		ePressueOutputType : pressure_output_enum;
		eFluxOutputType : flux_output_enum;
		pCalibMax : REAL;
		pCalibZero : REAL;
		vCalibMax : REAL;
		vCalibZero : REAL;
		Reserve0_BOOL1 : BOOL;
		Reserve0_BOOL2 : BOOL;
		Reserve0_BOOL : BOOL;
		Reserve0_BOOL3 : BOOL;
		New_Member0 : REAL;
		New_Member1 : REAL;
		New_Member2 : REAL;
	END_STRUCT;
	TopDeflash_Para_typ : 	STRUCT 
		V_Fw : REAL;
		V_Fw2nd : REAL;
		V_Bw : REAL;
		P_Fw : REAL;
		P_Fw2nd : REAL;
		P_Bw : REAL;
		V_BwReset : REAL;
		P_BwReset : REAL;
		Reserve0_REAL1 : REAL;
		Reserve0_REAL3 : REAL;
		Reserve0_REAL4 : REAL;
		Reserve0_REAL2 : REAL;
	END_STRUCT;
	TopDeflash_typ : 	STRUCT 
		Step : UINT;
		Alarm : USINT;
		Timer : timer_typ;
		LimitTimer : timer_typ;
		p_set : REAL;
		v_set : REAL;
		p_act : REAL;
		v_act : REAL;
		aoPropValve : INT;
		Out : UINT;
	END_STRUCT;
	USB_Device_typ : 	STRUCT 
		DeviceName : STRING[19];
		DirScreenDeviceName : STRING[19];
		DirSingleDeviceName : STRING[19];
		DirAllDeviceName : STRING[19];
		DeviceOk : BOOL;
		reserve : BOOL;
	END_STRUCT;
	User_Fix_new_typ : 	STRUCT 
		AccuPressure_Calib : REAL;
		AccuPumpMinCalib : REAL;
		AccuPressure_Extrlift_Calib : REAL;
		AccuPump_Extrlift_MinCalib : REAL;
		AccuPressure0V : REAL;
		AccChargeValveDelayTime : REAL;
		AccuPressure_Extrlift_0V : REAL;
		AccChargeValve_Extrlfit_DlyTime : REAL;
		TempUpRange : REAL;
		TempDwnRange : REAL;
		AlaUpTol_Same : REAL;
		AlaDwnTol_Same : REAL;
		HeatLowTimeOut : REAL;
		CarriageKeepMode : USINT;
		ProtectionTemp : REAL;
		bTimeLubricate : USINT;
		FrontWorkingLight : USINT;
		AirPressureLowAlarmDlyTime : UDINT;
		PlateFwTime : UDINT;
	END_STRUCT;
	User_Fix_typ : 	STRUCT 
		VarNew : REAL;
		iMaxFluxV : INT;
		iMinFluxV : INT;
		rMaxServoFlow : REAL;
		rMinServoFlow : REAL;
		ProjectInfo : Project_Info_typ;
	END_STRUCT;
	User_Para_typ : 	STRUCT 
		VarNew : REAL;
		FluxSensor : ARRAY[0..5]OF Flux_Sensor_typ;
		SetTimePower : REAL;
		Current32767_A : REAL;
		Current32767_B : REAL;
		Current32767_C : REAL; (*Albert for power consumption*)
		Power32767_A : REAL;
		Power32767_B : REAL;
		Power32767_C : REAL; (*Albert for power consumption*)
		RMold : Mold_Para_User_typ;
		LMold : Mold_Para_User_typ;
		LiftDieLipTempSCR : REAL; (*Albet for SCR Temp output*)
		RightDieLipTempSCR : REAL; (*Albet for SCR Temp output*)
		DieLipTempSCR3 : REAL; (*Chaoi for SCR Temp output*)
		DieLipTempSCR4 : REAL; (*Chaoi for SCR Temp output*)
		DieLipTempSCR_Lower : REAL;
		OilPumpDetectTime : REAL;
		OilPumpDelayTime : REAL;
		AccuPressure_Calib : REAL;
		AccuPressure0V : REAL;
		AccuPumpMinCalib : REAL;
		HandledeflashCount : REAL;
		ThickTime_TwoMold : UINT;
		ThickTime_OneMold : UINT;
		bProductAuxClamp : BOOL;
		SetExtrSpeed_A : INT;
		SetExtrSpeed_B : INT;
		SetExtrSpeed_C : INT;
		SetExtrSpeed_V : INT;
		SupportAirVentTimeVersionEnable : USINT;
		CutterHeatingSCR : REAL;
	END_STRUCT;
	Valve_Clean_typ : 	STRUCT 
		CleanValveTotalTime : UDINT;
		CleanValveFwTime : UDINT;
		CleanValveInterval : UDINT;
		CleanValveBwTime : UDINT;
		MoldR : Mold_CleanValve_typ;
		MoldL : Mold_CleanValve_typ;
	END_STRUCT;
	Valve_Delay_typ : 	STRUCT 
		ClampOpn : UINT;
		ClampCls : UINT;
		CarriageFw : UINT;
		CarriageBw : UINT;
		BlowPinUp : UINT;
		BlowPinDn : UINT;
		ExtrLift : UINT;
		Lock : UINT;
		New_Member3 : UINT;
		New_Member2 : UINT;
		New_Member1 : UINT;
		New_Member : UINT;
	END_STRUCT;
	Vlave_Out_typ : 	STRUCT 
		BlowPinDnHold : USINT;
		ProductAuxClamp : USINT;
		CoolPinDn : USINT;
		CoolPin2Dn : USINT;
		CoolPin3Dn : USINT;
		RobotFw : USINT;
		RobotBw : USINT;
		RobotUp : USINT;
		RobotDn : USINT;
		RobotOpn : USINT;
		RobotCls : USINT;
		TopDeflashCls : USINT;
		ClampCls : USINT;
		ClampOpn : USINT;
		Lock : USINT;
		ClampClsDiff : USINT;
		ClampOpnDiff : USINT;
		BlowPinUp : USINT;
		BlowPinDn : USINT;
		BottomPinUp : USINT;
		BottomPinDn : USINT;
		DeflashFw : USINT;
		DeflashBw : USINT;
		CarriageFw : USINT;
		CarriageBw : USINT;
		LowBlow : USINT;
		HighBlow : USINT;
		FreezingBlow : USINT;
		Exhaust : USINT;
		CoolDeflash : USINT;
		SuckAir : USINT;
		BlowDeflash : USINT;
		EraseDeflash : USINT;
		Cutter : USINT;
		CutNeckCls : USINT;
		CutNeck : USINT;
		SubMoldOpn : USINT;
		SubMoldCls : USINT;
		ScrewFw : USINT;
		ScrewBw : USINT;
		PinPartFw : USINT;
		PinPartBw : USINT;
		SealerFw : USINT;
		SealerBw : USINT;
		CoolPinBlow : USINT;
		CoolPin2Blow : USINT;
		CoolPin3Blow : USINT;
		TopDeflashFw : USINT;
		TopDeflashBw : USINT;
		BottomDeflashFw : USINT;
		BottomDeflashBw : USINT;
		BlowPinDnQuick : USINT;
		MovementValve : USINT; (*Albert, add movement*)
		ClampOpnRelease : USINT; (*Albert Clamp Release Valve*)
		ClampClsAux : USINT; (*Albert. Clamp close aux. Valve*)
		ClampOpnAux : USINT; (*Albert. Clamp open aux. Valve*)
		ClampMoveValve : USINT;
		ReleaseOilValve : USINT; (*Albert RealseOilValve*)
		PlateUp : USINT; (*Albert Plate Up vavle*)
		PlateDn : USINT; (*Albert Plate Down valve*)
		PlateFw : USINT; (*Albert Plate Up vavle*)
		PlateBw : USINT; (*Albert Plate Down valve*)
		ClampClsSlow : USINT; (*Albert RealseOilValve*)
		ClampLocking : USINT; (*Albert Locking Valve*)
		RobotCarriageUp : USINT; (*Albert output singal for robot when carriage at down position*)
		RobotCarriageDn : USINT; (*Albert output singal for robot when carriage at down position*)
		RobotClampCls : USINT; (*Albert output singal for robot when clamp at close position*)
		RobotClampOpn : USINT; (*Albert output singal for robot when clamp at open position*)
		TopDeflashCool : USINT; (*Albert output singal for topdeflash cooling *)
		CarriageFwHold : USINT;
		CarriageBwHold : USINT;
		CarriageAux : USINT;
		CarriageFwCalib : USINT;
		CarriageBwCalib : USINT;
		BlowPinUpMicro : USINT;
		BottomSubMoldOpn : USINT;
		BottomSubMoldCls : USINT;
		ExhaustForBlow2 : USINT;
		ExtrLiftUp : USINT;
		ExtrLiftUpCheck : USINT;
		ExtrLiftUpFast : USINT;
		ExtrLiftDn : USINT;
		PullBottom : USINT;
		SupportAir_TimeVersion : USINT;
		SuckAir_TimeVersion : USINT;
		SuckAirRelieve_TimeVersion : USINT;
		ExtInMoldStickerAllow : USINT;
		HookPinFw : USINT;
	END_STRUCT;
	Zones_Ctrl_typ : 	STRUCT 
		OnOff : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*controler on / off*)
		TuneOnOff : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*tuning on / off*)
		PreOnOff : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*prepare heating*)
		IsPreHeatingPlan : BOOL;
		IsHeatingOnPlan : BOOL; (*Timer heating on*)
		IsHeatingOffPlan : BOOL; (*Timer heating off*)
		IsAlarmConfirm : BOOL; (*alarm confirm*)
		IsQuickSetValue : UINT; (*quick setting value*)
	END_STRUCT;
	Zones_Fix_typ : 	STRUCT  (*FIX parameter  ---> recipe*)
		Enable : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone enable*)
		Cooling : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*Zong cooling 0 -- no cooling 1 -- cooling*)
		ProtectEna : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone heating protect*)
		bContactor : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone heating protect*)
		SynchGroup : ARRAY[0..MAX_IDX_ZONE]OF USINT; (*group sync heating*)
		tMinHeat : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*min time heating*)
		tPeriodHeat : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*period heating time*)
		tMinCool : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*min time cooling*)
		tPeriodCool : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*period cooling time*)
		tTempFreeze : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*Time for temperature freeze*)
		ParTune : ARRAY[0..MAX_IDX_ZONE]OF lcrtemp_pid_opt_typ; (*tune parameter*)
		AT_MAX : REAL; (*act temperature stop heating*)
		tCheck : REAL; (*check sensor time*)
		tProtectCooling : REAL; (*protect extruder cooling time*)
		tHeatNormal : REAL; (*heating normal check time*)
		resA1 : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		resA0 : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tProtectCoolingFix : REAL;
		OEMZoneOptionNum : REAL;
		New_Member1 : USINT;
		New_Member3 : USINT;
		New_Member2 : REAL;
		New_Member : REAL;
	END_STRUCT;
	Zones_Par_typ : 	STRUCT  (*Mold parameter ---> recipe*)
		ST_Tune : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*set temperature for tune*)
		ST_PID : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*set temperature for pid*)
		SenComT : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*sensor compemsation*)
		AlaUpTol : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*temp high*)
		AlaDwnTol : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*temperature low*)
		ST_PID_Same : REAL; (*same pid set temperature*)
		AlaUpTol_Same : REAL; (*same set alarm up tol*)
		AlaDwnTol_Same : REAL; (*same set alarm down tol*)
		tPreHeating : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*prepare heating time*)
		ST_PreHeating : ARRAY[0..MAX_IDX_ZONE]OF REAL; (*pre-heating temperature*)
		ST_GrpPreHeating : ARRAY[0..MAX_IDX_TEMPGROUP]OF REAL;
		tGrpPreHeating : ARRAY[0..MAX_IDX_TEMPGROUP]OF REAL; (*prepare heating time*)
		ST_Tune_Same : REAL; (*same tune set temperature*)
		resA1 : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		resA0 : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		Lower : REAL; (*Albert for lowering function, this variable is relative, so if it set 30, then means set value - 30.*)
		res0 : REAL;
	END_STRUCT;
END_TYPE

(*Temperature fix and parameter type *)

TYPE
	Zones_Status_typ : 	STRUCT 
		idxNotNormal : USINT;
		bNotNormal : BOOL;
		bAlarmHeatNormal : ARRAY[0..MAX_IDX_ZONE]OF BOOL;
		idxBroken : USINT;
		bBroken : BOOL; (*machine sensor broken*)
		SensorBroken : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone sensor broken*)
		bLowInternal : BOOL;
		idxLow : USINT;
		bLow : BOOL; (*machine alarm low*)
		AlmLow : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone alarm low*)
		idxHigh : USINT;
		bHigh : BOOL; (*machine alarm high*)
		AlmHigh : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone alarm high*)
		idxSwitchoff : USINT;
		bSwitchOff : BOOL; (*machine temperature high -switch off*)
		TempSwitchOff : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*temperature too high -- switch off*)
		idxFreeze : USINT;
		bFreeze : BOOL; (*machine heating exception*)
		TempFreeze : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*heating exception -- temperature freeze*)
		Tuning : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone is tuning*)
		TuneOk : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone tuning ok*)
		TuneStatus : ARRAY[0..MAX_IDX_ZONE]OF UINT; (*zone tune step and error feedback*)
		PreHeatingOk : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*zone pre-heating ok or not*)
		okToHeat : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*function need*)
		okToFree : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*function need*)
		okToFreeEnd : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*function need*)
		okToCool : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*function need*)
		okToCoolEnd : ARRAY[0..MAX_IDX_ZONE]OF BOOL; (*function need*)
		Out : ARRAY[0..MAX_IDX_ZONE]OF INT;
	END_STRUCT;
END_TYPE
