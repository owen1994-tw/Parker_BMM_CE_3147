(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Package: spc
 * File: spc.typ
 * Author: lij
 * Created: July 15, 2015
 ********************************************************************
 * Data types of package spc
 ********************************************************************)

TYPE
	SPC_Ctrl_typ : 	STRUCT 
		MoldL : SPC_Mold_Ctrl_typ;
		MoldR : SPC_Mold_Ctrl_typ;
	END_STRUCT;
	SPC_Par_typ : 	STRUCT 
		UpperLimit : SPC_Unit_typ;
		LowerLimit : SPC_Unit_typ;
		CtrlEnable : SPC_Ctrl_typ;
	END_STRUCT;
	SPC_Screen_typ : 	STRUCT 
		Data : ARRAY[0..23]OF SPC_Unit_typ;
		Max : SPC_Unit_typ;
		Avg : SPC_Unit_typ;
		Min : SPC_Unit_typ;
		Upper : SPC_Unit_typ;
		Lower : SPC_Unit_typ;
	END_STRUCT;
END_TYPE
