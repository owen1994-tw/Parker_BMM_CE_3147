(********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: pvgchg
 * File: pvgchg.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program pvgchg -- point variable change log
 ********************************************************************)

TYPE
	Var_Info_typ : 	STRUCT 
		pvName : STRING[C_PV_NAME_LENGTH];
		idxGroup : UINT;
		idxText : UINT;
		address : UDINT;
		length : UDINT;
		typ : UDINT;
		pValue : ARRAY[0..3]OF SINT;
	END_STRUCT;
	Var_Log_typ : 	STRUCT 
		idxGroup : UINT;
		idxText : UINT;
		oldValue : STRING[11];
		curValue : STRING[11];
		chgTime : STRING[19];
	END_STRUCT;
	Var_Name_typ : 	STRUCT 
		sText : ARRAY[0..29]OF UINT;
	END_STRUCT;
END_TYPE
