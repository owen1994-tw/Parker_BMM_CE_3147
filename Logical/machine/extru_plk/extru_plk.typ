
TYPE
	speedAdjust_typ : 	STRUCT 
		pSpeed : REFERENCE TO REAL;
		maxVal : REAL;
		minVal : REAL;
		frequence : REAL;
		waitTimeSlow : UINT;
		waitTimeFast : UINT;
		unitsSlow : REAL;
		unitsFast : REAL;
		unitsVFast : REAL;
		iSpeed : ARRAY[0..3]OF INT;
	END_STRUCT;
	ControlStep : 
		( (*wzq*)
		READY_TO_SWITCH_ON,
		SWITCH_ON,
		ENABLE_OPERATE,
		SET_SPEED,
		FEEDBACK
		);
	ExtruderStatus_typ : 	STRUCT  (*wzq*)
		PowerOnReady : BOOL;
		SwitchOnReady : BOOL;
		EnableOnReady : BOOL;
	END_STRUCT;
END_TYPE
