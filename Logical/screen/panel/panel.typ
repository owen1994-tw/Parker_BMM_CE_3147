(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: h_plan
 * File: h_plan.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program h_plan
 ********************************************************************)

TYPE
	Object_Focus_typ : 	STRUCT 
		Obj : ARRAY[0..MAX_OBJECT]OF UINT;
		Touch : ARRAY[0..MAX_OBJECT]OF USINT;
		pSource : REFERENCE TO USINT;
		pHideSource : REFERENCE TO USINT;
		Current : INT;
		Count : INT;
		Section : INT;
		Mode : INT;
	END_STRUCT;
END_TYPE
