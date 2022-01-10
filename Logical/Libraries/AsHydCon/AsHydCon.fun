(********************************************************************
 * COPYRIGHT -- B&R
 ********************************************************************
 * Library: AsHydCon
 * File: AsHydCon.fun
 * Author: DI Thomas J. Frauscher
 * Created: July 2011
 ********************************************************************
 * Function blocks of library AsHydCon
 ********************************************************************)

FUNCTION_BLOCK HCRPT1 (*1st order delay element.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		V : REAL; (*Gain. Units: [1].*)
		T1 : REAL; (*Time constant. Units: [s]. Valid value range: T1 >= 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRPT1_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRPT2 (*2nd order delay element.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		V : REAL; (*Gain. Units: [1].*)
		T1 : REAL; (*Time constant 1. Units: [s]. Valid value range: T1 >= 0.*)
		T2 : REAL; (*Time constant 2. Units: [s]. Valid value range: T2 >= 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRPT2_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRPT2s (*Oscillating 2nd order delay element.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		V : REAL; (*Gain. Units: [1].*)
		D : REAL; (*Damping constant. Units: [1]. Valid value range: D >= 0.*)
		Tf : REAL; (*Filter time constant. Units: [s]. Valid value range: Tf > 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRPT2s_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRDT1 (*1st order differentiator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		Tv : REAL; (*Differentiation time constant. Units: [s]. Valid value range: Tv >= 0.*)
		Tf : REAL; (*Filter time constant. Units: [s]. Valid value range: Tf >= 0.*)
		u : REAL; (*Input value.*)
		n_Avg : USINT; (*Moving average filter for the input value u. Valid value range: {0, 1, 2, ..., 100}.*)
		Reset_y : USINT; (*Reset controller input y to 0. Valid value range: {0,1,2}.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRDT1_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRDT2 (*2nd order differentiator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		Tv : REAL; (*Differentiation time constant. Units: [s]. Valid value range: Tv >= 0.*)
		T1 : REAL; (*Filter time constant 1. Units: [s]. Valid value range: T1 >= 0.*)
		T2 : REAL; (*Filter time constant 2. Units: [s]. Valid value range: T2 >= 0.*)
		u : REAL; (*Input value.*)
		n_Avg : USINT; (*Moving average filter for the input value u. Valid value range: {0, 1, 2, ..., 100}.*)
		Reset_y : USINT; (*Reset controller input y to 0. Valid value range: {0,1,2}.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRDT2_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRDT2s (*Oscillating 2nd order differentiator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		D : REAL; (*Damping constant. Units: [1]. Valid value range: D >= 0.*)
		Tv : REAL; (*Differentiation time constant. Units: [s]. Valid value range: Tv >= 0.*)
		Tf : REAL; (*Filter time constant. Units: [s]. Valid value range: Tf >= 0.*)
		u : REAL; (*Input value.*)
		Reset_y : USINT; (*Reset controller input y to 0. Valid value range: {0,1,2}.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRDT2s_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRIT1 (*1st order integrator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		V : REAL; (*Gain. Units: [1].*)
		Tn : REAL; (*Integration time constant (integral action time). Units: [s]. Valid value range: Tn > 0.*)
		T1 : REAL; (*Time constant. Units: [s]. Valid value range: T1 >= 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRIT1_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRNotch (*2nd order notch filter.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		V : REAL; (*Gain. Units: [1].*)
		f0 : REAL; (*Filter frequency. Units: [Hz]. Valid value range: f0 > 0.*)
		B : REAL; (*Filter band width. Units: [Hz]. Valid value range: B > 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRNotch_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRPT1cP (*Converging 1st order delay element.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		T1 : REAL; (*Time constant. Units: [s]. Valid value range: T1 >= 0.*)
		u : REAL; (*Input value.*)
		Set_y : USINT; (*"Apply preset value for y" controller input. Valid value range: {0,1,2}.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRPT1cP_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRJoltFree (*Jolt-free switching between 2 inputs.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		tSwitch : REAL; (*Switching time. Units: [s]. Valid value range: tSwitch >= 0.*)
		uSwitch : BOOL; (*Switch between signal inputs u1 and u2.*)
		u1 : REAL; (*Input value 1.*)
		u2 : REAL; (*Input value 2.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRJoltFree_Internal_typ; (*Data for internal use.*)
		PT1cP : HCRPT1cP; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRFIFO (*FIFO (dead time) element.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		t_FIFO : DINT; (*Delay time. Units: [µs].*)
		x : REAL; (*Input value.*)
		Set_y : BOOL; (*"Apply preset value for y" controller input.*)
		y_set : REAL; (*Preset value for y.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Output value.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRFIFO_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRRampGen (*Ramp generator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		t_rise : REAL; (*Rise time. Units [s]. Valid value range: t_rise >= 0.*)
		t_hold : REAL; (*Constant holding time. Units [s]. Valid value range: t_hold >= 0.*)
		t_fall : REAL; (*Fall time. Units [s]. Valid value range: t_fall >= 0.*)
		y_start : REAL; (*Start value for the output value y.*)
		y_hold : REAL; (*Stop value for the output value y.*)
		y_end : REAL; (*End value for the output value y.*)
		Init : BOOL; (*Based on the start value y_start, Init = 1 initiates a new ramp that ends with an end value y_end, after crossing a stop phase t_hold with stop value y_hold.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Ramp generator output value.*)
		dy : REAL; (*Time derivative (dy/dt) of y.*)
		dy_ramped : REAL; (*Ramped time derivative (dy/dt) of y.*)
		Finished : BOOL; (*Finished = 1 indicates the end of ramp generation.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRRampGen_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRSPF (*Smart path finder.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pMove_Para : REFERENCE TO HCRMovePara_typ; (*Movement parameters (t_Jolt, vC_pos, vC_neg, aAcc_pos, aDec_pos, aAcc_neg, aDec_neg).*)
		Stop : BOOL; (*Stop = 1 stops trajectory planning.*)
		s_home : REAL; (*Homing position. Units: [mm].*)
		Set_home : BOOL; (*If no set trajectory is being processed at the moment, setting Set_home = 1 sets s_set = s_home and Reached = 1.*)
		s_end : REAL; (*End position of the trajectory. Units: [mm].*)
		Init : BOOL; (*Based on the current set position s_set and the current set speed v_set, Init = 1 initiates a new trajectory that ends at the point s_end.*)
		Brake : BOOL; (*Based on the current values of s_set and v_set, Brake = 1 stops the movement with the delay aDec_pos (positive direction) or aDec_neg (negative direction).*)
	END_VAR
	VAR_OUTPUT
		s_set : REAL; (*Set position. Units: [mm].*)
		v_set : REAL; (*Set speed. Units: [mm/s]. Value range: -vC_neg <= v_set <= vC_pos.*)
		a_set : REAL; (*Set acceleration. Units: [mm/s²]. Value range: -max(aDec_pos, aDec_neg) <= a_set <= max(aAcc_pos, aAcc_neg).*)
		MoveDir : SINT; (*Direction of movement. Value range: {-3,-2,-1: negative direction of movement (-3: Deceleration phase, -2: Constant speed phase, -1: Acceleration phase), 0: standstill, 1,2,3: positive direction of movement (1: Acceleration phase, 2: Constant speed phase, 3: Deceleration phase)}.*)
		ApproachDir : SINT; (*Approach direction indicates in which direction the end position s_end is reached. Value range: {-1: negative approach direction, 0: undefined, 1: positive approach direction}.*)
		Reached : BOOL; (*Reached = 1 indicates that the end position has been reached and that the movement is complete.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRSPF_Internal_typ; (*Data for internal use.*)
		SystemInfo : SysInfo; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRSFG (*Smart force generator.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		t_Jolt : REAL; (*Jolt time. Units [s]. Valid value range: 0 <= t_Jolt <= 0.2.*)
		Stop : BOOL; (*Stop = 1 stops trajectory planning.*)
		dFp_home : REAL; (*Homing value for the set force rate. Units: [N/s].*)
		Fp_home : REAL; (*Homing value for the set force. Units: [N].*)
		Set_home : BOOL; (*If no set trajectory is being processed at the moment, setting Set_home = 1 sets Fp_set = Fp_home, dFp_set = dFp_home and Reached = 1.*)
		t_delta : REAL; (*Time difference specified for bringing Fp_set from its present value to the end value Fp_end. Units: [s]. Valid value range: t_delta > t_Jolt.*)
		Fp_end : REAL; (*End value of the set force trajectory. Units: [N].*)
		Init : BOOL; (*Based on the current set force Fp_set and the current set force rate dFp_set, Init = 1 initiates a new trajectory that ends at the point Fp_end (dFp_set = 0).*)
	END_VAR
	VAR_OUTPUT
		Fp_set : REAL; (*Set force. Units: [N].*)
		dFp_set : REAL; (*Set force rate. Units: [N/s].*)
		t_delta_act : REAL; (*Actual time difference needed for bringing Fp_set to the specified end value Fp_end. Units: [s]. Value range: t_delta_act > t_Jolt.*)
		Reached : BOOL; (*Reached = 1 indicates that end value of the set force has been reached and that the profile generation is complete.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRSFG_Internal_typ; (*Data for internal use.*)
		SPF : HCRSPF; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRAutoTune (*Autotuning-algorithm for Kp.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSupply_max : REAL; (*Maximum system pressure (supply pressure). Units: [bar]. Valid value range: pSupply_max >= 0.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		Qnom_pos : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_pos results in a movement in the positive direction (e.g. extending piston). Units: [l/min]. Valid value range: Qnom_pos > 0.*)
		Qnom_neg : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_neg results in a movement in the negative direction (e.g. retracting piston). Units: [l/min]. Valid value range: Qnom_neg > 0.*)
		pDelta_nom : REAL; (*Nominal pressure drop at valve edge. Units: [bar]. Valid value range: pDelta_nom > 0.*)
		V0_pos : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'positive' direction (e.g. extending piston). Units: [l]. Valid value range: V0_pos >= 0.*)
		V0_neg : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'negative' direction (e.g. retracting piston). Units: [l]. Valid value range: V0_neg >= 0.*)
		h_Cyl : REAL; (*Cylinder height. Units: [mm]. Valid value range: h_Cyl > 0.*)
		m_Cyl : REAL; (*Resulting mass of the moving hydraulic system. Units: [kg]. Valid value range: m_Cyl > 0.*)
		s_min : REAL; (*Minimum value of set position and actual position. Units: [mm]. Valid value range: s_min < s_max.*)
		s_max : REAL; (*Maximum value of set position and actual position. Units: [mm]. Valid value range: s_max > s_min.*)
		Update : USINT; (*Any changes to the above variables (pSupply_max, A_Cyl_pos, ..., s_max) must be applied with Update = 1.*)
	END_VAR
	VAR_OUTPUT
		Kp : REAL; (*Calculated proportional gain of the position controller. Units: [1].*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRAutoTune_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCROmega0 (*Omega0-correction for Kp.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		V0_pos : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'positive' direction (e.g. extending piston). Units: [l]. Valid value range: V0_pos >= 0.*)
		V0_neg : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'negative' direction (e.g. retracting piston). Units: [l]. Valid value range: V0_neg >= 0.*)
		h_Cyl : REAL; (*Cylinder height. Units: [mm]. Valid value range: h_Cyl > 0.*)
		x_Cyl_min : REAL; (*Minimum value of x_Cyl until a correction of the proportional gain of the position controller has been calculated. Units: [mm]. Valid value range: 0 <= x_Cyl_min < x_Cyl_max.*)
		x_Cyl_max : REAL; (*Maximum value of x_Cyl until a correction of the proportional gain of the position controller has been calculated. Units: [mm]. Valid value range: x_Cyl_min < x_Cyl_max <= h_Cyl.*)
		Update : USINT; (*Any changes to the above variables (A_Cyl_pos, A_Cyl_neg, ..., x_Cyl_max) must be applied with Update = 1.*)
		x_Cyl : REAL; (*Current position of the cylinder piston (always based on the cylinder). Units: [mm]. Valid value range: 0 <= x_Cyl <= h_Cyl.*)
	END_VAR
	VAR_OUTPUT
		x0_Cyl : REAL; (*Position of cylinder piston at which the critical natural frequency of the hydraulic system occurs. Units: [mm]. Value range: 0 <= x0_Cyl <= h_Cyl.*)
		Kp_xCorr : REAL; (*Calculated multiplication factor for correcting the proportional gain of the position controller with Kp_xCorr = Kp_xCorr(x_Cyl). Units: [1]. Value range: 1 <= Kp_xCorr <= 2.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCROmega0_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRvyServo (*v-y servo correction (pressure).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSupply_max : REAL; (*Maximum system pressure (supply pressure). Units: [bar]. Valid value range: pSupply_max >= 0.*)
		pSupply : REAL; (*System pressure (supply pressure). Units: [bar]. Valid value range: pSupply > 0.*)
		pTank : REAL; (*Tank pressure. Units: [bar]. Valid value range: pTank >= 0.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		Qnom_pos : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_pos results in a movement in the positive direction (e.g. extending piston). Units: [l/min]. Valid value range: Qnom_pos > 0.*)
		Qnom_neg : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_neg results in a movement in the negative direction (e.g. retracting piston). Units: [l/min]. Valid value range: Qnom_neg > 0.*)
		pDelta_nom : REAL; (*Nominal pressure drop at valve edge. Units: [bar]. Valid value range: pDelta_nom > 0.*)
		yValve_0 : REAL; (*Valve overlap. Units: [1]. Valid value range:  0 <= yValve_0 < 1.*)
		yValve_min : REAL; (*Minimum valve opening (|yValve| = yValve_min). Units: [1]. Valid value range: 0 <= yValve_min <= 1.*)
		Neg_y : BOOL; (*Negative values for the valve opening yValve result in a positive speed, and vice versa (inverted valve behavior).*)
		Update : USINT; (*Any changes to the above variables must be applied with Update = (Mode + 1).*)
		pCyl_pos : REAL; (*Chamber pressure for movement in the 'positive' direction. Units: [bar]. Valid value range: 0 <= pCyl_pos < pSupply.*)
		pCyl_neg : REAL; (*Chamber pressure for movement in the 'negative' direction. Units: [bar]. Valid value range: 0 <= pCyl_neg < pSupply.*)
		vCyl_abs : REAL; (*Absolute speed of cylinder movement. Units: [mm/s]. Valid value range: -vCyl_sup < vCyl_abs < vCyl_sup.*)
		vCyl_rel : REAL; (*Relative speed of cylinder movement. Units: [%]. Valid value range: -100 <= vCyl_rel <= 100.*)
		Zero_yValve : BOOL; (*Setting this input results in the immediate output of yValve = 0 and Qy = 0.*)
	END_VAR
	VAR_OUTPUT
		Qy_abs : REAL; (*Volume flow produced by vCyl_abs (Qy = Qy_abs + Qy_rel). Units: [l/min]. Value range: -Qnom_neg <= Qy_abs <= Qnom_pos.*)
		Qy_rel : REAL; (*Volume flow produced by vCyl_rel (Qy_rel = Qy - Qy_abs). Units: [l/min]. Value range: -Qnom_neg <= Qy_rel <= Qnom_pos.*)
		Qy : REAL; (*Volume flow produced by valve opening yValve. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRvyServo_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRvyServo_Fp (*v-y servo correction (force).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSupply_max : REAL; (*Maximum system pressure (supply pressure). Units: [bar]. Valid value range: pSupply_max >= 0.*)
		pSupply : REAL; (*System pressure (supply pressure). Units: [bar]. Valid value range: pSupply > 0.*)
		pTank : REAL; (*Tank pressure. Units: [bar]. Valid value range: pTank >= 0.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		Qnom_pos : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_pos results in a movement in the positive direction (e.g. extending piston). Units: [l/min]. Valid value range: Qnom_pos > 0.*)
		Qnom_neg : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_neg results in a movement in the negative direction (e.g. retracting piston). Units: [l/min]. Valid value range: Qnom_neg > 0.*)
		pDelta_nom : REAL; (*Nominal pressure drop at valve edge. Units: [bar]. Valid value range: pDelta_nom > 0.*)
		yValve_0 : REAL; (*Valve overlap. Units: [1]. Valid value range:  0 <= yValve_0 < 1.*)
		yValve_min : REAL; (*Minimum valve opening (|yValve| = yValve_min). Units: [1]. Valid value range: 0 <= yValve_min <= 1.*)
		Neg_y : BOOL; (*Negative values for the valve opening yValve result in a positive speed, and vice versa (inverted valve behavior).*)
		Update : USINT; (*Any changes to the above variables must be applied with Update = (Mode + 1).*)
		Fp_Cyl : REAL; (*Process force. Units: [N].*)
		Ff_Cyl_pos : REAL; (*Friction force in positive direction. Units: [N]. Valid value range:  Ff_Cyl_pos >= 0.*)
		Ff_Cyl_neg : REAL; (*Friction force in negative direction. Units: [N]. Valid value range:  Ff_Cyl_neg >= 0.*)
		vCyl_abs : REAL; (*Absolute speed of cylinder movement. Units: [mm/s]. Valid value range: -vCyl_sup < vCyl_abs < vCyl_sup.*)
		vCyl_rel : REAL; (*Relative speed of cylinder movement. Units: [%]. Valid value range: -100 <= vCyl_rel <= 100.*)
		Zero_yValve : BOOL; (*Setting this input results in the immediate output of yValve = 0 and Qy = 0.*)
	END_VAR
	VAR_OUTPUT
		Qy_abs : REAL; (*Volume flow produced by vCyl_abs (Qy = Qy_abs + Qy_rel). Units: [l/min]. Value range: -Qnom_neg <= Qy_abs <= Qnom_pos.*)
		Qy_rel : REAL; (*Volume flow produced by vCyl_rel (Qy_rel = Qy - Qy_abs). Units: [l/min]. Value range: -Qnom_neg <= Qy_rel <= Qnom_pos.*)
		Qy : REAL; (*Volume flow produced by valve opening yValve. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRvyServo_Fp_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRFyServo (*F-y servo correction (pressure).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSupply_max : REAL; (*Maximum system pressure (supply pressure). Units: [bar]. Valid value range: pSupply_max >= 0.*)
		pSupply : REAL; (*System pressure (supply pressure). Units: [bar]. Valid value range: pSupply > 0.*)
		pTank : REAL; (*Tank pressure. Units: [bar]. Valid value range: pTank >= 0.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		Qnom_pos : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_pos results in a movement in the positive direction (e.g. extending piston). Units: [l/min]. Valid value range: Qnom_pos > 0.*)
		Qnom_neg : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_neg results in a movement in the negative direction (e.g. retracting piston). Units: [l/min]. Valid value range: Qnom_neg > 0.*)
		pDelta_nom : REAL; (*Nominal pressure drop at valve edge. Units: [bar]. Valid value range: pDelta_nom > 0.*)
		yValve_0 : REAL; (*Valve overlap. Units: [1]. Valid value range:  0 <= yValve_0 < 1.*)
		V0_pos : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'positive' direction (e.g. extending piston). Units: [l]. Valid value range: V0_pos >= 0.*)
		V0_neg : REAL; (*Static volume in reference to the cylinder chamber responsible for movement in the 'negative' direction (e.g. retracting piston). Units: [l]. Valid value range: V0_neg >= 0.*)
		h_Cyl : REAL; (*Cylinder height. Units: [mm]. Valid value range: h_Cyl > 0.*)
		Neg_y : BOOL; (*Negative values for the valve opening yValve result in a positive speed, and vice versa (inverted valve behavior).*)
		Update : USINT; (*Any changes to the above variables must be applied with Update = (Mode + 1).*)
		x_Cyl : REAL; (*Current position of the cylinder piston (always based on the cylinder). Units: [mm]. Valid value range: 0 <= x_Cyl <= h_Cyl.*)
		v_Cyl : REAL; (*Current speed of the cylinder piston (always based on the cylinder). Units: [mm].*)
		pCyl_pos : REAL; (*Chamber pressure for movement in the 'positive' direction. Units: [bar]. Valid value range: 0 <= pCyl_pos < pSupply.*)
		pCyl_neg : REAL; (*Chamber pressure for movement in the 'negative' direction. Units: [bar]. Valid value range: 0 <= pCyl_neg < pSupply.*)
		dF_Cyl_abs : REAL; (*Absolute rate of force increase of the cylinder. Units: [N/s]. Valid value range:  -dF_Cyl_sup < dF_Cyl_abs < dF_Cyl_sup.*)
		dF_Cyl_rel : REAL; (*Relative rate of force increase of the cylinder. Units: [%]. Valid value range: -100 <= dF_Cyl_rel <= 100.*)
	END_VAR
	VAR_OUTPUT
		Qy : REAL; (*Volume flow produced by valve opening yValve. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRFyServo_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRFyServo_Fp (*F-y servo correction (force).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSupply_max : REAL; (*Maximum system pressure (supply pressure). Units: [bar]. Valid value range: pSupply_max >= 0.*)
		pSupply : REAL; (*System pressure (supply pressure). Units: [bar]. Valid value range: pSupply > 0.*)
		pTank : REAL; (*Tank pressure. Units: [bar]. Valid value range: pTank >= 0.*)
		A_Cyl_pos : REAL; (*Cylinder piston area whose pressurization induces a movement in the positive direction (e.g. extending piston). Units: [cm]. Valid value range: A_Cyl_pos > 0.*)
		A_Cyl_neg : REAL; (*Cylinder piston area whose pressurization induces a movement in the negative direction (e.g. retracting piston). Units: [cm]. Valid value range: A_Cyl_neg > 0.*)
		Qnom_pos : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_pos results in a movement in the positive direction (e.g. extending piston). Units: [l/min]. Valid value range: Qnom_pos > 0.*)
		Qnom_neg : REAL; (*Nominal valve-volume flow, which, together with A_Cyl_neg results in a movement in the negative direction (e.g. retracting piston). Units: [l/min]. Valid value range: Qnom_neg > 0.*)
		yValve_0 : REAL; (*Valve overlap. Units: [1]. Valid value range:  0 <= yValve_0 < 1.*)
		Neg_y : BOOL; (*Negative values for the valve opening yValve result in a positive speed, and vice versa (inverted valve behavior).*)
		Update : USINT; (*Any changes to the above variables must be applied with Update = (Mode + 1).*)
		Fp : REAL; (*Process force. Units: [N].*)
		u : REAL; (*Input value. Units: [1]. Valid value range: -1 <= u <= 1.*)
	END_VAR
	VAR_OUTPUT
		Qy : REAL; (*Volume flow produced by valve opening yValve. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRFyServo_Fp_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRLinSpline (*Linear spline function.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		NbNodes : UINT; (*Number of nodes from the node vector that are used to define the point-to-point linear function 'f(x)'. Valid value range: NbNodes >= 0.*)
		pNodeVec : REFERENCE TO HCRNode_typ; (*Pointer to the first node vector element to be used, which represents the basis of 'f(x)' ([(X1,Y1), (X2,Y2), ..., (Xn,Yn)]) and 'f-1(x)' ([(Y1,X1), (Y2,X2), ..., (Yn,Xn)]), and which is formed as an array of HCRNode_typ elements.*)
		Invert : BOOL; (*Invert = 1 inverts the point-to-point linear function 'f(x)'. Also, y = 'f-1(x)', which is based on the inverted node vector [(Y1,X1), (Y2,X2), ..., (Yn,Xn)].*)
		Smooth : USINT; (*Smoothing in the area of around the node (Xi,Yi) of 'f(x)' and (Yi,Xi) of 'f-1(x)'. Valid value range: {0: off, 1: on}.*)
		Rness : REAL; (*Smoothing factor. Valid value range: 0 <= Rness <= 1000.*)
		Update : BOOL; (*Any changes to the above variables (NbNodes, ..., Rness) must be applied with Update = 1.*)
		DisMode : SINT; (*Processing discontinuities in 'f(x)' and 'f-1(x)'. Valid value range: {-1, 0, 1}.*)
		x : REAL; (*Abscissa value of 'f(x)' or 'f-1(x)'. Valid value range: X1 <= x <= Xn or Y1 <= x <= Yn.*)
	END_VAR
	VAR_OUTPUT
		y : REAL; (*Ordinate value of 'f(x)' or 'f-1(x)'.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRLinSpline_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRLinValve (*Valve linearization.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		NbNodes : UINT; (*Number of nodes from the node vector that are used to define the point-to-point linear valve characteristic curve linearization 'f-1(Q)'. Valid value range: NbNodes >= 0.*)
		pNodeVec : REFERENCE TO HCRNode_typ; (*Pointer to the node vector element that is to be used first, which is the basis for the valve characteristic curve 'f(U)' ([(U1,Q1), (U2,Q2), ..., (Un,Qn)]), and which consists of an array of HCRNode_typ elements.*)
		Neg_Qy : BOOL; (*Neg_Qy = 1 negates the valve volume flow Qy (abscissa value of 'f-1(Q)').*)
		Neg_Uy : BOOL; (*Neg_Uy = 1 negates the control voltage Uy of the valve, i.e. +10V is reduced to -10V.*)
		Update : BOOL; (*Any changes to the above variables (NbNodes, pNodeVec and Neg_Uy) must be applied with Update = 1.*)
		DisMode : SINT; (*Processing of discontinuities in 'f-1(Q)', as they occur in positive overlapped valves. Valid value range: {-1, 0, 1}.*)
		Qy : REAL; (*Nominal valve volume flow as abscissa value of 'f-1(Q)'. Units: [l/min]. Valid value range: Q1 <= Qy <= Qn.*)
	END_VAR
	VAR_OUTPUT
		Uy : REAL; (*Control voltage of the valve (Neg_Uy = 0: U1 <= Uy <= Un, Neg_Uy = 1: -Un <= Uy <= -U1). Units: [V].*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRLinValve_Internal_typ; (*Data for internal use.*)
		LinSpline : HCRLinSpline; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRPID (*PID controller.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		Parameter : HCRPIDPara_typ; (*PID parameters.*)
		Update : USINT; (*Every change to the above PID parameter vector must be made active using Update = 1.*)
		W : REAL; (*Reference variable (set value). Valid value range: WX_min <= W  <= WX_max.*)
		X : REAL; (*Controlled variable (actual value). Valid value range: WX_min <= X  <= WX_max.*)
		Invert : BOOL; (*Inversion of the effective direction of the controller using Invert = 1.*)
		I_Switch : SINT; (*Switch for the I part. Valid value range: I_{-4, -3, -2, -1, 0, 1, 2, 3, 4}.*)
		CTRLmode : USINT; (*Controller operating mode. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}.*)
		Yman : REAL; (*Manipulated variable for manual operation. Units: [%]. Valid value range: Y_min <= Yman <= Y_max.*)
		Yadd : REAL; (*Feed forward value. Units: [%]. Valid value range: -100 <= Yadd <= 100.*)
	END_VAR
	VAR_OUTPUT
		e : REAL; (*Control deviation (e = W - X). Value range: (WX_min - WX_max) <= e  <= (WX_max - WX_min).*)
		Yp : REAL; (*Proportional part. Units: [%]. Value range: -100 <= Yp <= 100.*)
		Yi : REAL; (*Integration part. Units: [%]. Value range: Y_min <= Yi <= Y_max.*)
		Yd : REAL; (*Differential part. Units: [%]. Value range: -100 <= Yd <= 100.*)
		Y : REAL; (*Manipulated variable. Unit: [%]. Value range: Y_min <= Y <= Y_max.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRPID_Internal_typ; (*Data for internal use.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRP2P_Cont (*Point-to-point position controller.*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		s_set : REAL; (*Set position. Units: [mm].*)
		s_act : REAL; (*Actual position. Units: [mm].*)
		Kp : REAL; (*Proportional gain. Units: [%/mm]. Valid value range: Kp > 0.*)
		P2Pmode : SINT; (*Operating mode - position controller. Valid value range: {-1, 0, 1}.*)
		pAddPar : REFERENCE TO HCRP2P_Cont_AddPar_typ; (*Additional control parameters and options (Y_P2P_min, Y_P2P_max, dY_max, Tn_pos, Tn_neg, I_Switch, Tv, Tf).*)
	END_VAR
	VAR_OUTPUT
		Y_P2P : REAL; (*Manipulated variable. Unit: [%]. Value range: -100 <= Y_P2P <= 100.*)
		Y_P2P_int : INT; (*Valve opening in integer format. Value range: -32760 <= Y_P2P_int <= 32760.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRP2P_Cont_Internal_typ; (*Data for internal use.*)
		PID : HCRPID; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRs_Cont (*Position controller (force).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_s : REFERENCE TO HCRPIDPara_s_typ; (*PID parameter - position controller.*)
		vyServoMode : USINT; (*v-y servo correction mode. Valid value range: {0, 2, 4, 6, 8}.*)
		Update : USINT; (*Every change to vyServoMode or to the parameters of the above vectors must be made active using Update = 1.*)
		s_set : REAL; (*Set position. Units: [mm].*)
		s_act : REAL; (*Actual position. Units: [mm].*)
		Invert : BOOL; (*Inversion of the effective direction of the controller using Invert = 1.*)
		I_Switch : USINT; (*Switch for the I part of the PID. Valid value range: {0, 1, 2, 3, 4}.*)
		CTRLmode : USINT; (*Operating mode - position controller. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}.*)
		v_man : REAL; (*Speed of movement in manual operation. Units: [mm/s]. Valid value range: v_min <= v_man <= v_max.*)
		v_add : REAL; (*Additional speed input. Units: [mm/s]. Valid value range: -vCyl_sup < v_add < vCyl_sup.*)
		v_set : REAL; (*Set speed for speed input control of the cylinder movement. Units: [mm/s]. Valid value range: -vCyl_sup < v_set < vCyl_sup.*)
		Fp_Cyl : REAL; (*Process force. Units: [N].*)
		Ff_Cyl_pos : REAL; (*Friction force in positive direction. Units: [N]. Valid value range:  Ff_Cyl_pos >= 0.*)
		Ff_Cyl_neg : REAL; (*Friction force in negative direction. Units: [N]. Valid value range:  Ff_Cyl_neg >= 0.*)
		Qy_Oset_ext : REAL; (*External volume flow offset. Units: [l/min].*)
		Oset_Mode : USINT; (*Selection switch for offset compensation. Valid value range: {00000000, ..., 01111111}.*)
	END_VAR
	VAR_OUTPUT
		Qy_s : REAL; (*Calculated volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy_s <= Qnom_pos.*)
		Qy_Oset_s : REAL; (*Additive volume flow for offset compensation. Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRs_Cont_Internal_typ; (*Data for internal use.*)
		PID : HCRPID; (*Internally used function block.*)
		vyServo_Fp : HCRvyServo_Fp; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRs_Cont_p (*Position controller (pressure).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_s : REFERENCE TO HCRPIDPara_s_typ; (*PID parameter - position controller.*)
		vyServoMode : USINT; (*v-y servo correction mode. Valid value range: {0, 2, 4, 6, 8}.*)
		Update : USINT; (*Every change to vyServoMode or to the parameters of the above vectors must be made active using Update = 1.*)
		s_set : REAL; (*Set position. Units: [mm].*)
		s_act : REAL; (*Actual position. Units: [mm].*)
		Invert : BOOL; (*Inversion of the effective direction of the controller using Invert = 1.*)
		I_Switch : USINT; (*Switch for the I part of the PID. Valid value range: {0, 1, 2, 3, 4}.*)
		CTRLmode : USINT; (*Operating mode - position controller. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}.*)
		v_man : REAL; (*Speed of movement in manual operation. Units: [mm/s]. Valid value range: v_min <= v_man <= v_max.*)
		v_add : REAL; (*Additional speed input. Units: [mm/s]. Valid value range: -vCyl_sup < v_add < vCyl_sup.*)
		v_set : REAL; (*Set speed for speed input control of the cylinder movement. Units: [mm/s]. Valid value range: -vCyl_sup < v_set < vCyl_sup.*)
		pCyl_pos : REAL; (*Chamber pressure for movement in the 'positive' direction. Units: [bar]. Valid value range: 0 <= pCyl_pos < pSupply.*)
		pCyl_neg : REAL; (*Chamber pressure for movement in the 'negative' direction. Units: [bar]. Valid value range: 0 <= pCyl_neg < pSupply.*)
		Qy_Oset_ext : REAL; (*External volume flow offset. Units: [l/min].*)
		Oset_Mode : USINT; (*Selection switch for offset compensation. Valid value range: {00000000, ..., 01111111}.*)
	END_VAR
	VAR_OUTPUT
		Qy_s : REAL; (*Calculated volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy_s <= Qnom_pos.*)
		Qy_Oset_s : REAL; (*Additive volume flow for offset compensation. Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRs_Cont_p_Internal_typ; (*Data for internal use.*)
		PID : HCRPID; (*Internally used function block.*)
		vyServo : HCRvyServo; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRF_Cont (*Force controller (force).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_F : REFERENCE TO HCRPIDPara_F_typ; (*PID parameter - force controller.*)
		FyServoMode : USINT; (*F-y servo correction mode. Valid value range: {0, 2, 8}.*)
		Update : USINT; (*Every change to FyServoMode or to the parameters of the above vectors must be made active using Update = 1.*)
		Fp_set : REAL; (*Set force. Units: [N].*)
		Fp_act : REAL; (*Actual force. Units: [N].*)
		Invert : BOOL; (*Inversion of the effective direction of the controller using Invert = 1.*)
		I_Switch : USINT; (*Switch for the I part of the PID. Valid value range: {0, 1, 2, 3, 4}.*)
		CTRLmode : USINT; (*Operating mode - force controller. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x10, 0x20}.*)
		Q_man : REAL; (*Valve volume flow in manual operation. Units: [l/min]. Valid value range: -Q_min <= Q_man <= Q_max.*)
		Q_add : REAL; (*Additional volume flow input. Units: [l/min]. Valid value range: -Qnom_neg <= Q_add <= Qnom_pos.*)
		v_act : REAL; (*Actual speed of the cylinder piston. Units: [mm/s].*)
		Qy_Oset_ext : REAL; (*External volume flow offset. Units: [l/min].*)
		Oset_Mode : USINT; (*Selection switch for offset compensation. Valid value range: {00000000, ..., 01111111}.*)
	END_VAR
	VAR_OUTPUT
		Qy_F : REAL; (*Calculated volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy_F <= Qnom_pos.*)
		Qy_Oset_F : REAL; (*Additive volume flow for offset compensation. Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRF_Cont_Internal_typ; (*Data for internal use.*)
		PID : HCRPID; (*Internally used function block.*)
		FyServo_Fp : HCRFyServo_Fp; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRF_Cont_p (*Force controller (pressure).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pSystem_ParaAdd : REFERENCE TO HCRSystemParaAdd_typ; (*Additional system parameter (V0_pos, V0_neg, h_Cyl, m_Cyl).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_F : REFERENCE TO HCRPIDPara_F_typ; (*PID parameter - force controller.*)
		FyServoMode : USINT; (*F-y servo correction mode. Valid value range: {0, 2, 8}.*)
		Update : USINT; (*Every change to FyServoMode or to the parameters of the above vectors must be made active using Update = 1.*)
		Fp_set : REAL; (*Set force. Units: [N].*)
		Fp_act : REAL; (*Actual force. Units: [N].*)
		Invert : BOOL; (*Inversion of the effective direction of the controller using Invert = 1.*)
		I_Switch : USINT; (*Switch for the I part of the PID. Valid value range: {0, 1, 2, 3, 4}.*)
		CTRLmode : USINT; (*Operating mode - force controller. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x30}.*)
		Q_man : REAL; (*Valve volume flow in manual operation. Units: [l/min]. Valid value range: -Q_min <= Q_man <= Q_max.*)
		Q_add : REAL; (*Additional volume flow input. Units: [l/min]. Valid value range: -Qnom_neg <= Q_add <= Qnom_pos.*)
		dFp_set : REAL; (*Set value of the rate of force increase of the cylinder. Units: [N/s].*)
		x_Cyl : REAL; (*Current position of the cylinder piston (always based on the cylinder). Units: [mm]. Valid value range: 0 <= x_Cyl <= h_Cyl.*)
		v_Cyl : REAL; (*Current speed of the cylinder piston (always based on the cylinder). Units: [mm/s].*)
		pCyl_pos : REAL; (*Chamber pressure for movement in the 'positive' direction. Units: [bar]. Valid value range: 0 <= pCyl_pos < pSupply.*)
		pCyl_neg : REAL; (*Chamber pressure for movement in the 'negative' direction. Units: [bar]. Valid value range: 0 <= pCyl_neg < pSupply.*)
		v_act : REAL; (*Actual speed of the cylinder piston. Units: [mm/s].*)
		Qy_Oset_ext : REAL; (*External volume flow offset. Units: [l/min].*)
		Oset_Mode : USINT; (*Selection switch for offset compensation. Valid value range: {00000000, ..., 01111111}.*)
	END_VAR
	VAR_OUTPUT
		Qy_F : REAL; (*Calculated volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy_F <= Qnom_pos.*)
		Qy_Oset_F : REAL; (*Additive volume flow for offset compensation. Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRF_Cont_p_Internal_typ; (*Data for internal use.*)
		PID : HCRPID; (*Internally used function block.*)
		FyServo : HCRFyServo; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRsF_Cont (*Switchable position/force controller (force).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_s : REFERENCE TO HCRPIDPara_s_typ; (*PID parameter - position controller.*)
		pPID_Para_F : REFERENCE TO HCRPIDPara_F_typ; (*PID parameter - force controller.*)
		vyServoMode : USINT; (*v-y servo correction mode. Valid value range: {0, 2, 4, 6, 8}.*)
		FyServoMode : USINT; (*F-y servo correction mode. Valid value range: {0, 2, 8}.*)
		tSwitch : REAL; (*Switching time. Units: [s]. Valid value range: tSwitch >= 0.*)
		Update : USINT; (*Every change to vyServoMode, FyServoMode and tSwitch or to the parameters of the above vectors must be made active using Update = 1.*)
		s_set : REAL; (*Set position. Units: [mm].*)
		s_act : REAL; (*Actual position. Units: [mm].*)
		s_Cont_InVar : HCRs_Cont_InVar_typ; (*Additional input values - position controller.*)
		Fp_set : REAL; (*Set force. Units: [N].*)
		Fp_act : REAL; (*Actual force. Units: [N].*)
		F_Cont_InVar : HCRF_Cont_InVar_typ; (*Additional input values - force controller.*)
		CTRLmode : USINT; (*Operating mode. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x10, 0x20}.*)
		uSwitch : USINT; (*Switches between position controller and force controller.*)
	END_VAR
	VAR_OUTPUT
		Qy_s : REAL; (*Calculated volume flow (position). Units: [l/min]. Value range: -Qnom_neg <= Qy_s <= Qnom_pos.*)
		Qy_Oset_s : REAL; (*Additive volume flow for offset compensation (position). Units: [l/min].*)
		Qy_F : REAL; (*Calculated volume flow (force). Units: [l/min]. Value range: -Qnom_neg <= Qy_F <= Qnom_pos.*)
		Qy_Oset_F : REAL; (*Additive volume flow for offset compensation (force). Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		InForceCtrl : BOOL; (*Force controller active.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRsF_Cont_Internal_typ; (*Data for internal use.*)
		s_Cont : HCRs_Cont; (*Internally used function block.*)
		F_Cont : HCRF_Cont; (*Internally used function block.*)
		JoltFree : HCRJoltFree; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK HCRsF_Cont_p (*Switchable position/force controller (pressure).*)
	VAR_INPUT
		enable : BOOL; (*Enable function block.*)
		pSystem_Para : REFERENCE TO HCRSystemPara_typ; (*System parameter (pSupply_max, pSupply, pTank, A_Cyl_pos, A_Cyl_neg).*)
		pSystem_ParaAdd : REFERENCE TO HCRSystemParaAdd_typ; (*Additional system parameter (V0_pos, V0_neg, h_Cyl, m_Cyl).*)
		pValve_Para : REFERENCE TO HCRValvePara_typ; (*Valve parameter (Qnom_pos, Qnom_neg, pDelta_nom, yValve_0, Neg_y).*)
		pPID_Para_s : REFERENCE TO HCRPIDPara_s_typ; (*PID parameter - position controller.*)
		pPID_Para_F : REFERENCE TO HCRPIDPara_F_typ; (*PID parameter - force controller.*)
		vyServoMode : USINT; (*v-y servo correction mode. Valid value range: {0, 2, 4, 6, 8}.*)
		FyServoMode : USINT; (*F-y servo correction mode. Valid value range: {0, 2, 8}.*)
		tSwitch : REAL; (*Switching time. Units: [s]. Valid value range: tSwitch >= 0.*)
		Update : USINT; (*Every change to vyServoMode, FyServoMode and tSwitch or to the parameters of the above vectors must be made active using Update = 1.*)
		s_set : REAL; (*Set position. Units: [mm].*)
		s_act : REAL; (*Actual position. Units: [mm].*)
		s_Cont_p_InVar : HCRs_Cont_p_InVar_typ; (*Additional input values - position controller.*)
		Fp_set : REAL; (*Set force. Units: [N].*)
		Fp_act : REAL; (*Actual force. Units: [N].*)
		F_Cont_InVar : HCRF_Cont_p_InVar_typ; (*Additional input values - force controller.*)
		CTRLmode : USINT; (*Operating mode. Valid value range: {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x10, 0x20, 0x30}.*)
		uSwitch : USINT; (*Switches between position controller and force controller.*)
	END_VAR
	VAR_OUTPUT
		Qy_s : REAL; (*Calculated volume flow (position). Units: [l/min]. Value range: -Qnom_neg <= Qy_s <= Qnom_pos.*)
		Qy_Oset_s : REAL; (*Additive volume flow for offset compensation (position). Units: [l/min].*)
		Qy_F : REAL; (*Calculated volume flow (force). Units: [l/min]. Value range: -Qnom_neg <= Qy_F <= Qnom_pos.*)
		Qy_Oset_F : REAL; (*Additive volume flow for offset compensation (force). Units: [l/min].*)
		Qy : REAL; (*Volume flow. Units: [l/min]. Value range: -Qnom_neg <= Qy <= Qnom_pos.*)
		yValve : REAL; (*Valve opening. Units: [1]. Value range: -1 <= yValve <= 1.*)
		MoveDir : SINT; (*Direction of movement. Value range: {-1: negative direction of movement, 0: undefined, 1: positive direction of movement}.*)
		InForceCtrl : BOOL; (*Force controller active.*)
		status : UINT; (*Function block status message.*)
	END_VAR
	VAR
		Internal : HCRsF_Cont_p_Internal_typ; (*Data for internal use.*)
		s_Cont_p : HCRs_Cont_p; (*Internally used function block.*)
		F_Cont : HCRF_Cont; (*Internally used function block.*)
		F_Cont_p : HCRF_Cont_p; (*Internally used function block.*)
		vyServo : HCRvyServo; (*Internally used function block.*)
		JoltFree : HCRJoltFree; (*Internally used function block.*)
	END_VAR
END_FUNCTION_BLOCK
