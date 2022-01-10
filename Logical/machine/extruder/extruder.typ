(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: extruder
 * File: extruder.typ
 * Author: lij
 * Created: January 05, 2015
 ********************************************************************
 * Local data types of program extruder
 ********************************************************************)

TYPE
	Lubricate_Check_typ : 	STRUCT 
		bEnable : BOOL;
		diLubricCheck : BOOL;
		tSetCheckTime : REAL;
		tActCheckTime : REAL;
		tCheckDly : REAL;
		bLubricError : BOOL;
	END_STRUCT;
	speedAdjust_typ : 	STRUCT 
		pSpeed : REFERENCE TO REAL;
		maxVal : UINT;
		minVal : UINT;
		frequence : UINT;
		waitTimeSlow : UINT;
		waitTimeFast : UINT;
		unitsSlow : UINT;
		unitsFast : UINT;
		unitsVFast : UINT;
		iSpeed : ARRAY[0..3]OF UINT;
	END_STRUCT;
END_TYPE
