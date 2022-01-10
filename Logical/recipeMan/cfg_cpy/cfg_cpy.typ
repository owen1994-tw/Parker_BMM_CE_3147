(********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: cfg_cpy
 * File: cfg_cpy.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program cfg_cpy  --- configure parameter copy 
 ********************************************************************)

TYPE
	var_rd_typ : 	STRUCT 
		modname : STRING[20];
		taskname : STRING[20];
		pre_string : STRING[10];
		error : UINT;
		line_no : UINT;
		PV_name : ARRAY[0..49]OF SINT;
	END_STRUCT;
END_TYPE
