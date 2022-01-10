
FUNCTION_BLOCK HydPumpOnOff
	VAR_INPUT
		bButtonOnOff : BOOL;
		bButtonOff : BOOL;
		bFeedback : BOOL;
		diMotorOn : BOOL;
		tTask : REAL;
		tOnDelay : REAL;
		tOffDelay : REAL;
		tProtection : REAL;
		tAccuChargeOffDelay : REAL;
	END_VAR
	VAR_OUTPUT
		ledMotor : USINT;
		doContactor : BOOL;
		doOn : BOOL;
		doAccuCharge : BOOL;
	END_VAR
	VAR
		bButtonOnOffOld : BOOL;
		bdoOnOld : BOOL;
		cntOffDelay : UDINT;
		cntOnDelay : UDINT;
		cntProtection : UDINT;
		cntAccuChargeOffDelay : UDINT;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydvProfGen (*End stop : Tj and Tj*)
	VAR_INPUT
		init : BOOL; (*enable function*)
		end : BOOL; (*command end*)
		Ts : REAL; (*sample time -- s*)
		Tj : REAL; (*jerk time -- s*)
		Td : REAL; (*delay time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		decel_end : REAL; (*decelaration speed end -- mm/s2*)
		v_act : REAL; (*act velocity -- mm/s*)
		v_start : REAL; (*start velocity -- mm/s*)
		v_set : REAL; (*setting velocity -- mm/s*)
		v_end : REAL; (*target velocity -- mm/s*)
		s_act : REAL; (*act position    mm*)
		s_end : REAL; (*target position mm*)
		decel_gain : REAL; (*decel end gain*)
		v_zero : REAL; (*velocity zero value*)
	END_VAR
	VAR_OUTPUT
		status : UINT; (*function status*)
		v_out : REAL; (*out velocity*)
	END_VAR
	VAR
		Internal : HydvProfJerk_Internal_typ;
		s_start : REAL;
		v_set_old : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydPT1
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		Tf : REAL; (*filter time -- s*)
		u : REAL; (*input*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*output*)
		f0 : REAL; (*cut off frequency  Hz*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydHT1
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		Tf : REAL; (*filter time -- s*)
		u : REAL; (*input*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*output*)
		f0 : REAL; (*cut off frequency  Hz*)
	END_VAR
	VAR
		u_old : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydBandPass
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		f0 : REAL; (*setting frequency*)
		bandWidth : REAL;
		u : REAL;
	END_VAR
	VAR_OUTPUT
		y : REAL;
		status : UINT;
	END_VAR
	VAR
		t1 : REAL;
		PT1 : HydPT1;
		t2 : REAL;
		HT1 : HydHT1;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydBandStop
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		f0 : REAL;
		bandWidth : REAL;
		u : REAL;
	END_VAR
	VAR_OUTPUT
		y : REAL;
		status : UINT;
	END_VAR
	VAR
		t1 : REAL;
		PT1 : HydPT1;
		t2 : REAL;
		HT1 : HydHT1;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydvProfJerk
	VAR_INPUT
		init : BOOL;
		end : BOOL;
		Ts : REAL; (*sample time -- s*)
		Tj : REAL; (*jerk time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		decel_end : REAL; (*decelaration speed -- mm/s2*)
		v_set : REAL;
		v_creep : REAL;
		s_end : REAL;
		s_act : REAL;
	END_VAR
	VAR_OUTPUT
		v_out : REAL;
		status : UINT;
	END_VAR
	VAR
		Internal : HydvProfJerk_Internal_typ;
		s_start : REAL;
		sAcc : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydSPF_PJerk
	VAR_INPUT
		enable : BOOL;
		init : BOOL;
		brake : BOOL;
		stop : BOOL;
		Ts : REAL; (*sample time -- s*)
		movePara : HydMovePara_typ;
		s_act : REAL;
		s_end : REAL;
		accelMode : BOOL;
		v_accelMin : REAL;
		v_creep : REAL;
	END_VAR
	VAR_OUTPUT
		j_out : REAL;
		a_out : REAL;
		v_out : REAL;
		s_out : REAL;
		moveDir : SINT;
		reached : SINT;
		status : UINT;
	END_VAR
	VAR
		internal : HydSPFJerk_Internal_typ;
		s_start : REAL;
		v_old : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydSPF_PCos
	VAR_INPUT
		enable : BOOL;
		init : BOOL;
		brake : BOOL;
		stop : BOOL;
		Ts : REAL; (*sample time -- s*)
		Td : REAL; (*dealy time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		s_act : REAL;
		s_end : REAL;
		accelMode : BOOL;
		v_accelMin : REAL;
		v_set : REAL;
		v_creep : REAL;
		v_act : REAL; (*act velocity*)
	END_VAR
	VAR_OUTPUT
		a_out : REAL;
		v_out : REAL;
		s_out : REAL;
		status : UINT;
	END_VAR
	VAR
		internal : HydSPFCos_Internal_typ;
		s_start : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydSPF_TCos
	VAR_INPUT
		enable : BOOL;
		init : BOOL;
		brake : BOOL;
		stop : BOOL;
		Ts : REAL; (*sample time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		s_act : REAL;
		s_end : REAL;
		v_set : REAL;
	END_VAR
	VAR_OUTPUT
		a_out : REAL;
		v_out : REAL;
		s_out : REAL;
		status : UINT;
	END_VAR
	VAR
		internal : HydSPFCos_Internal_typ;
		s_start : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydProfSqrt
	VAR_INPUT
		init : BOOL;
		Ts : REAL; (*sample time -- s*)
		moveStatus : SINT;
		vSetPercent : REAL;
		vCreepPercent : REAL;
		vMax : REAL;
		accel : REAL;
		decel : REAL;
		decel_end : REAL;
		s_target : REAL;
		s_act : REAL;
		s_creep : REAL;
	END_VAR
	VAR_OUTPUT
		status : UINT;
		vOutPercent : REAL;
	END_VAR
	VAR
		vSetInternal : REAL;
		vCreepInternal : REAL;
		vOutInternal : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydProfLin
	VAR_INPUT
		init : BOOL;
		Ts : REAL; (*sample time -- s*)
		vSet : REAL;
		vCreep : REAL;
		accel : REAL;
		s_act : REAL;
		s_target : REAL;
		s_creep : REAL;
		kpEnd : REAL;
	END_VAR
	VAR_OUTPUT
		moveStatus : SINT;
		vOut : REAL;
	END_VAR
	VAR
		Internal : ProfGen_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydFIFO
	VAR_INPUT
		enable : BOOL;
		Times : UINT;
		u : REAL;
	END_VAR
	VAR_OUTPUT
		y : REAL;
		status : UINT;
	END_VAR
	VAR
		Count : UINT;
		Buffer : ARRAY[0..199] OF REAL;
		y_old : REAL;
		OldTimes : UINT;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydProf_Thick
	VAR_INPUT
		Start : BOOL;
		End : BOOL;
		Update : BOOL;
		reserve : BOOL;
		Ts : REAL; (*sample time -- s*)
		aiGapValue : INT;
		aiAccuValue : INT;
		Parameter : Thick_Profile_Para_typ;
	END_VAR
	VAR_OUTPUT
		ActValue : ARRAY[0..100] OF REAL;
		Timer : REAL;
		CurrentPoint : DINT;
		Out : REAL;
		Status : UINT;
	END_VAR
	VAR
		Internal : Thick_Profile_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydPID_Thick
	VAR_INPUT
		enable : BOOL;
		Parameter : Thick_Para_typ;
		W : REAL;
		X : REAL;
	END_VAR
	VAR_OUTPUT
		e : REAL;
		Yp : REAL;
		Yi : REAL;
		Yd : REAL;
		Y_FF : REAL;
		Y : REAL;
		status : UINT;
	END_VAR
	VAR
		Internal : Thick_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydLinear
	VAR_INPUT
		enable : BOOL;
		Y : REAL;
		nNodes : UINT;
		pNodes : REFERENCE TO Hyd_Node_typ;
		update : BOOL;
	END_VAR
	VAR_OUTPUT
		X : REAL;
		ratio : REAL;
		errIndex : UINT;
		status : UINT;
	END_VAR
	VAR
		enable_old : BOOL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydActFlt
	VAR_INPUT
		enable : BOOL;
		u : REAL;
		deltaMax : REAL;
	END_VAR
	VAR_OUTPUT
		y : REAL;
		y_avg : REAL;
		status : UINT;
	END_VAR
	VAR
		count : UINT;
		diff : REAL;
		lost : UINT;
		buffer : ARRAY[0..9] OF REAL;
		total : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydMidFlt
	VAR_INPUT
		enable : BOOL;
		u : REAL;
		N : USINT;
	END_VAR
	VAR_OUTPUT
		y : REAL;
		status : UINT;
	END_VAR
	VAR
		oldN : USINT;
		count : UDINT;
		buffer : ARRAY[0..99] OF REAL;
		sort : ARRAY[0..99] OF REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydSPF_TJerk
	VAR_INPUT
		enable : BOOL;
		init : BOOL;
		brake : BOOL;
		stop : BOOL;
		Ts : REAL; (*sample time -- s*)
		movePara : HydMovePara_typ;
		setHome : BOOL;
		s_home : REAL;
		s_end : REAL;
	END_VAR
	VAR_OUTPUT
		j_out : REAL;
		a_out : REAL;
		v_out : REAL;
		s_out : REAL;
		moveDir : SINT;
		reached : SINT;
		status : UINT;
	END_VAR
	VAR
		internal : HydSPFJerk_Internal_typ;
		v_old : REAL;
		s_start : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydMovAvgFlt
	VAR_INPUT
		enable : BOOL;
		u : INT;
		Times : UINT;
	END_VAR
	VAR_OUTPUT
		y : INT;
		status : UINT;
	END_VAR
	VAR
		Total : DINT;
		Count : UINT;
		OldTimes : UINT;
		Buffer : ARRAY[0..99] OF INT;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydP2P
	VAR_INPUT
		enable : BOOL;
		Ts : REAL;
		Parameter : P2P_Para_typ;
		Update : BOOL;
		s_set : REAL;
		s_act : REAL;
		Yadd : REAL;
	END_VAR
	VAR_OUTPUT
		e : REAL;
		Yp : REAL;
		Yi : REAL;
		Yd : REAL;
		Y : REAL;
		status : UINT;
	END_VAR
	VAR
		Internal : P2P_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydPID
	VAR_INPUT
		enable : BOOL;
		Ts : REAL;
		Parameter : PID_Para_typ;
		Update : BOOL;
		W : REAL;
		X : REAL;
		Yadd : REAL;
	END_VAR
	VAR_OUTPUT
		e : REAL;
		Yp : REAL;
		Yi : REAL;
		Yd : REAL;
		Y : REAL;
		status : UINT;
	END_VAR
	VAR
		Internal : PID_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydPT2
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		T1 : REAL;
		T2 : REAL;
		u : REAL;
	END_VAR
	VAR_OUTPUT
		f1 : REAL;
		f2 : REAL;
		y : REAL;
	END_VAR
	VAR
		PT1 : HydPT1;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydDT2
	VAR_INPUT
		u : REAL;
		Ts : REAL; (*sample time -- s*)
		Tv : REAL;
		T1 : REAL;
		T2 : REAL;
	END_VAR
	VAR_OUTPUT
		y : REAL;
	END_VAR
	VAR
		u_old : REAL;
		PT2 : HydPT2;
		D : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydProfile
	VAR_INPUT
		Init : BOOL;
		Ts : REAL; (*sample time -- s*)
		T_dead : REAL; (*valve dead time -- s*)
		Accel : REAL; (*accelaration speed -- mm/s2*)
		Decel : REAL; (*decelaration speed -- mm/s2*)
		Decel_end : REAL; (*decelaration end speed -- mm/s2*)
		V_target : REAL; (*target velocity*)
		V_set : REAL; (*setting velocity*)
		S_act : REAL; (*act position*)
		S_target : REAL; (*target position*)
		V_act : REAL; (*act velocity*)
	END_VAR
	VAR_OUTPUT
		MoveDir : SINT;
		Accel_out : REAL;
		V_out : REAL;
		Reached : SINT;
	END_VAR
	VAR
		Internal : Profile_Internal_typ;
		S_start : REAL;
		V_set_old : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydSCurveRamp
	VAR_INPUT
		Set : REAL;
		Accel : REAL; (*accelaration speed*)
		Ts : REAL; (*sample time -- s*)
		Once : BOOL;
	END_VAR
	VAR_OUTPUT
		Accel_out : REAL;
		Out : REAL;
	END_VAR
	VAR
		OldSet : REAL;
		Internal : Ramp_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydRampCos
	VAR_INPUT
		Ts : REAL; (*sample time -- s*)
		Set : REAL; (*set value*)
		Accel : REAL; (*accelaration speed*)
		Once : BOOL;
	END_VAR
	VAR_OUTPUT
		Out : REAL;
	END_VAR
	VAR
		Internal : Ramp_Internal_typ;
		OldSet : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydLinRamp
	VAR_INPUT
		setVal : INT; (*set value*)
		accel : DINT; (*accelaration speed*)
		Ts : INT; (*sample time -- s*)
	END_VAR
	VAR_OUTPUT
		out : INT;
	END_VAR
	VAR
		outOld : DINT;
		Internal : LRamp_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydvTimeProf
	VAR_INPUT
		init : BOOL; (*start*)
		Ts : REAL; (*sample time -- s*)
		t_move : REAL; (*move time*)
		v_set : REAL; (*set velocity*)
		v_start : REAL; (*start velocity*)
		v_end : REAL; (*stop velocity*)
		accel : REAL; (*accelaration speed*)
		decel : REAL; (*decelaration speed*)
	END_VAR
	VAR_OUTPUT
		end : BOOL; (*end *)
		status : UINT;
		v_out : REAL; (*velocity out*)
	END_VAR
	VAR
		internal : HydvTimeProf_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydvProfGenH (*End stop : Tj  and Tj + Td*)
	VAR_INPUT
		init : BOOL; (*enable function*)
		end : BOOL; (*command end*)
		Ts : REAL; (*sample time -- s*)
		Tj : REAL; (*jerk time -- s*)
		Td : REAL; (*delay time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		decel_end : REAL; (*decelaration speed end -- mm/s2*)
		v_act : REAL; (*act velocity -- mm/s*)
		v_start : REAL; (*start velocity -- mm/s*)
		v_set : REAL; (*setting velocity -- mm/s*)
		v_end : REAL; (*target velocity -- mm/s*)
		s_act : REAL; (*act position    mm*)
		s_end : REAL; (*target position mm*)
		decel_gain : REAL; (*decel end gain*)
		v_zero : REAL; (*velocity zero value*)
	END_VAR
	VAR_OUTPUT
		status : UINT; (*function status*)
		v_out : REAL; (*out velocity*)
	END_VAR
	VAR
		Internal : HydvProfJerk_Internal_typ;
		s_start : REAL;
		v_set_old : REAL;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HydvProfGenL (*End stop : Linear + Td*)
	VAR_INPUT
		init : BOOL; (*enable function*)
		end : BOOL; (*command end*)
		Ts : REAL; (*sample time -- s*)
		Tj : REAL; (*jerk time -- s*)
		Td : REAL; (*delay time -- s*)
		accel : REAL; (*accelaration speed -- mm/s2*)
		decel : REAL; (*decelaration speed -- mm/s2*)
		decel_end : REAL; (*decelaration speed end -- mm/s2*)
		v_act : REAL; (*act velocity -- mm/s*)
		v_start : REAL; (*start velocity -- mm/s*)
		v_set : REAL; (*setting velocity -- mm/s*)
		v_end : REAL; (*target velocity -- mm/s*)
		s_act : REAL; (*act position    mm*)
		s_end : REAL; (*target position mm*)
		decel_gain : REAL; (*decel end gain*)
		v_zero : REAL; (*velocity zero value*)
	END_VAR
	VAR_OUTPUT
		status : UINT; (*function status*)
		v_out : REAL; (*out velocity*)
	END_VAR
	VAR
		Internal : HydvProfJerk_Internal_typ;
		s_start : REAL;
		v_limit : REAL;
		a_decel : REAL;
		v_set_old : REAL;
	END_VAR
END_FUNCTION_BLOCK
