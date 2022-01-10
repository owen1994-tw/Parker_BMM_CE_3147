(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: power
 * File: power.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program power
 ********************************************************************)

TYPE
	CleanCtrl_typ : 	STRUCT 
		Step : UINT;
		Enable : USINT;
		oldEnable : USINT;
		p_set : REAL;
		Para : CleanValvePara_typ;
		ValveOut : CleanValveOut_typ;
		TimeDis : CleanValveTimeDis_typ;
	END_STRUCT;
	CleanValveOut_typ : 	STRUCT 
		ValveFw1 : USINT;
		ValveFw2 : USINT;
		ValveBw1 : USINT;
		ValveBw2 : USINT;
	END_STRUCT;
	CleanValveTimeDis_typ : 	STRUCT 
		CleanValveTotalDis : UDINT;
		CleanValveFwDis : UDINT;
		CleanValveBwDis : UDINT;
		CleanValveIntervalDis : UDINT;
	END_STRUCT;
	CleanValvePara_typ : 	STRUCT 
		P_FwClean : REAL;
		P_BwClean : REAL;
	END_STRUCT;
END_TYPE
