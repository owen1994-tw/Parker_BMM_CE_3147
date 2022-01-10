(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Package: thickness
 * File: thickness.typ
 * Author: lij
 * Created: December 19, 2013
 ********************************************************************
 * Data types of package thickness
 ********************************************************************)

TYPE
	position_typ : 	STRUCT 
		x : INT;
		y : INT;
	END_STRUCT;
	curveNode_typ : 	STRUCT 
		former : INT;
		next : INT;
		flagNode : USINT;
		color : UINT;
		vLineColor : UINT;
		v0 : position_typ;
		v1 : position_typ;
		v2 : position_typ;
		v3 : position_typ;
	END_STRUCT;
	drawMan_typ : 	STRUCT 
		draw : USINT;
		downLoad : USINT;
		weight : INT;
		oldWeight : INT;
		v1 : INT;
		v2 : INT;
		v03 : INT;
		currentPoint : INT;
		currentNode : INT;
		flagLineFocus : USINT;
		status : STRING[29];
		step : INT;
		input : USINT;
		curve : USINT;
	END_STRUCT;
	Thick_PID_typ : 	STRUCT 
		Set : DINT;
		Act : DINT;
		e0 : DINT;
		e1 : DINT;
		e2 : DINT;
		KpGain : INT;
		KiGain : INT;
		KdGain : INT;
		RepairOut : INT;
		Slew : INT;
		OutMax : INT;
		OutMin : INT;
		Out : INT;
		OutLast : INT;
		Wave : INT;
		Change : INT;
		Enable : USINT;
		Reserve : USINT;
		Yi : DINT;
	END_STRUCT;
END_TYPE
