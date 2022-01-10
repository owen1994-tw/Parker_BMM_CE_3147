
TYPE
	Axis_ID_typ : 	STRUCT 
		Address : UDINT; (*Address of the axis object data*)
		Name : STRING[20]; (*Logical name of the axis*)
		UnitFactor : REAL; (*Factor to calculate the displayed units*)
		UnitName : STRING[8]; (*Name of the unit to be displayed (e.g. deg,mm..)*)
		AxisType : UINT; (*ncAXIS or ncV_AXIS*)
	END_STRUCT;
	Axis_Status_typ : 	STRUCT 
		ActPosition : REAL; (*actual position*)
		ActVelocity : REAL; (*actual velocity*)
		ActTorq : REAL; (*act torque*)
		ErrorID : UINT; (*Shows the Error ID (0 = No Error)*)
		ErrorText : ARRAY[0..3]OF STRING[79]; (*Shows the Acopos Errortext as string*)
	END_STRUCT;
	Axis_State_typ : 	STRUCT 
		Disabled : BOOL; (*Status power disabled*)
		Standstill : BOOL; (*Status axis at standstill*)
		Stopping : BOOL; (*Status axis is stopping*)
		Homing : BOOL; (*Status axis is homing*)
		MotionDiscrete : BOOL; (*Status discrete motion is active*)
		MotionContinous : BOOL; (*Status continous motion is active*)
		MotionSynchronized : BOOL; (*Status synchronized motion is active*)
		ErrorStop : BOOL; (*Status axis stopped after error*)
	END_STRUCT;
	Axis_Parameter_typ : 	STRUCT 
		Position : REAL; (*Parameter absolute target position*)
		Distance : REAL; (*Parameter relative move distance*)
		Velocity : REAL; (*Parameter target velocity*)
		Direction : USINT; (*Parameter movement direction*)
		Acceleration : REAL; (*Parameter movement acceleration*)
		Deceleration : REAL; (*Parameter movement deceleration*)
		HomePosition : REAL; (*Parameter home position*)
		HomeMode : USINT; (*Parameter homing mode*)
		JogVelocity : REAL; (*Parameter jog movement target velocity*)
		OverrideVelocity : REAL; (*Parameter velocity override (0.0 - 2.0)*)
		OverrideAcceleration : REAL; (*Parameter acceleration override (0.0001 - 2.0)*)
	END_STRUCT;
	Axis_Cmd_typ : 	STRUCT 
		Init : BOOL; (*Download encoder parameter*)
		Power : BOOL; (*Turns the power on*)
		Home : BOOL; (*homes the axis*)
		MoveAbsolute : BOOL; (*Starts the absolute movement*)
		MoveAdditive : BOOL; (*Starts the additive movement*)
		MoveVelocity : BOOL; (*Starts the velocity movement*)
		Halt : BOOL; (*Stop every active movement*)
		Stop : BOOL; (*Stop every active movement as long as is set*)
		MoveJogPos : BOOL; (*Starts the jog movement in positive direction*)
		MoveJogNeg : BOOL; (*Starts the jog movement in negative direction*)
		ErrorAcknowledge : BOOL; (*Acknowledge an error with a pos. Edge*)
	END_STRUCT;
	Cyclic_Read_typ : 	STRUCT 
		s_encoder : DINT;
		v_act : REAL;
		torq_act : REAL;
	END_STRUCT;
	Cyclic_Write_typ : 	STRUCT 
		v_set : REAL;
		torq_set : REAL;
	END_STRUCT;
	MC_Axis_typ : 	STRUCT 
		Step : UINT;
		AxisID : Axis_ID_typ; (*axis info*)
		AxisCmd : Axis_Cmd_typ; (*axis comdand*)
		Status : Axis_Status_typ;
		AxisState : Axis_State_typ;
		Parameter : Axis_Parameter_typ;
		CycRead : Cyclic_Read_typ;
		CycWrite : Cyclic_Write_typ;
		DriverStatus : MC_DRIVESTATUS_TYP; (*actual status of the axis*)
	END_STRUCT;
	Par_ID_typ : 	STRUCT 
		ParID : UINT;
		DataType : UINT;
		DataAddress : UDINT;
		pValue : ARRAY[0..3]OF SINT;
	END_STRUCT;
	Par_RW_typ : 	STRUCT 
		idxInit : UINT;
		idxWrite : UINT;
		idxRead : UINT;
		init : ARRAY[0..7]OF Par_ID_typ;
		Write : ARRAY[0..15]OF Par_ID_typ;
		Read : ARRAY[0..9]OF Par_ID_typ;
	END_STRUCT;
	Encoder_Par_typ : 	STRUCT 
		count_dir : USINT; (*ENCOD_COUNT_DIR*)
		load_units : UDINT;
		encoder_rev : UDINT;
	END_STRUCT;
	Limit_Par_typ : 	STRUCT 
		a1_pos : REAL;
		a2_pos : REAL;
		a1_neg : REAL;
		a2_neg : REAL;
		v_pos : REAL;
		v_neg : REAL;
	END_STRUCT;
	ParID_Read_typ : 	STRUCT 
		torqueRated : REAL;
		currentRated : REAL;
		offset : REAL;
		actUDC : REAL;
		actCurrent : REAL;
		tempMotor : REAL;
		actLimitTorque : REAL;
		tempJunction : REAL;
		torqueMax : REAL;
	END_STRUCT;
	PID_Par_typ : 	STRUCT 
		kv_p : REAL;
		tn_p : REAL;
		kv_s : REAL;
		tn_s : REAL;
		tf_s : REAL;
		kpJunction : REAL;
		kpMotorTemp : REAL;
		tdFF_s : REAL;
	END_STRUCT;
	Axis_Fix_typ : 	STRUCT 
		encoderPar : Encoder_Par_typ;
		limitPar : Limit_Par_typ;
		pidPar : PID_Par_typ;
	END_STRUCT;
	ParID_Value_typ : 	STRUCT 
		encoderPar : Encoder_Par_typ;
		limitPar : Limit_Par_typ;
		pidPar : PID_Par_typ;
		v_Max : REAL;
		readPar : ParID_Read_typ;
	END_STRUCT;
END_TYPE
