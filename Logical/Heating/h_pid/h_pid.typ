(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: h_pid
 * File: h_pid.typ
 * Author: lij
 * Created: February 27, 2013
 ********************************************************************
 * Local data types of program h_pid
 ********************************************************************)

TYPE
	Heat_Alarm_typ : 	STRUCT 
		tTempSwitchOff : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tSensorBroken : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tTempHigh : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tTempLow : ARRAY[0..MAX_IDX_ZONE]OF REAL;
	END_STRUCT;
	Heat_Test_typ : 	STRUCT 
		LastTemp : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tTempFreeze : ARRAY[0..MAX_IDX_ZONE]OF REAL;
		tPreHeating : ARRAY[0..MAX_IDX_ZONE]OF REAL;
	END_STRUCT;
END_TYPE
