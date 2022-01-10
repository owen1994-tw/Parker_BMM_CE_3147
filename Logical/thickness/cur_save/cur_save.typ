(********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: cur_save
 * File: cur_save.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program cur_save --- thickness profile load/save
 ********************************************************************)

TYPE
	curveData_typ : 	STRUCT 
		CurveNode : ARRAY[0..399]OF curveNode_typ;
		SetPos : ARRAY[0..399]OF position_typ;
		weight : INT;
		v1 : INT;
		v2 : INT;
		v03 : INT;
		currentPoint : INT;
		currentNode : INT;
		usedIndex : UINT;
		flagLineFocus : USINT;
	END_STRUCT;
END_TYPE
