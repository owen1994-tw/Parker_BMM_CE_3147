(*Axes Configuration*)

TYPE
	actuator_typ_enum : 
		( (*Axes Actuator types *)
		ACTUATOR_UNDEFINED := 0, (* Actuator: undefined *)
		ACTUATOR_HYD := 1, (* Actuator: Hydraulic *)
		ACTUATOR_PNEU := 2, (* Actuator: Pneumatic *)
		ACTUATOR_ELECTRIC := 3 (* Actuator: Electric drive *)
		);
END_TYPE

(*Electric*)

TYPE
	mechanism_typ_enum : 
		(
		MECHANISM_LINEAR := 0,
		MECHANISM_TOGGLE := 1
		);
END_TYPE

(*Hydraulic*)

TYPE
	valve_typ_enum : 
		(
		VALVE_UNDEFINED := 0,
		VALVE_DIR := 1,
		VALVE_PROP := 2
		);
	sensor_type_enum : 
		(
		SENSOR_UNDEFINED := 0,
		SENSOR_LVDT := 1,
		SENSOR_SWITCH := 2
		);
	pressure_output_enum : 
		(
		PRESSURE_OUTPUT_UNDEFINED := 0,
		PRESSURE_OUTPUT_SYSTEM_SINGLE := 1,
		PRESSURE_OUTPUT_ACC := 2,
		PRESSURE_OUTPUT_ACC_EXTRLIFT := 3,
		PRESSURE_OUTPUT_NONE := 4
		);
	flux_output_enum : 
		(
		FLUX_OUTPUT_UNDEFINED := 0,
		FLUX_OUTPUT_SYSTEM_SINGLE := 1,
		FLUX_OUTPUT_PROP_VALVE_SINGLE := 2,
		FLUX_OUTPUT_ACC := 3,
		FLUX_OUTPUT_ACC_EXTRLIFT := 4,
		FLUX_OUTPUT_NONE := 5
		);
END_TYPE

(*Extruder*)

TYPE
	extruder_ctrl_typ_enum : 
		(
		CTRL_UNDEFINED := 0,
		CTRL_VF := 1,
		CTRL_ABB_POWERLINK := 2
		);
END_TYPE

(*Blow*)

TYPE
	blow_mode_enum : 
		(
		BLOW_MODE_STANDARD := 0,
		BLOW_MODE_FREEZINGBLOW := 1,
		New_Member1,
		New_Member
		);
END_TYPE

(*Support Air*)

TYPE
	supportair_mode_enum : 
		(
		SUPPORTAIR_MODE_STANDARD := 0,
		SUPPORTAIR_MODE_AFTER_CLAMP_CLS := 1,
		New_Member3,
		New_Member2
		);
END_TYPE
