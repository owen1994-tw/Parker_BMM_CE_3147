
TYPE
	epl_typ : 	STRUCT  (*datatype for AsEPL Library*)
		cmd : eplCmd_typ; (*variable with commands*)
		para : eplPara_typ; (*variable with parameters*)
		fub : eplFUB_typ; (*variable with Function Blocks*)
		state : state_enum; (*variable with actual state*)
	END_STRUCT;
	eplCmd_typ : 	STRUCT  (*datatype with commands*)
		readNode : BOOL; (*command to read the Node Number of the Powerlink interface*)
		readThresholdCnt : BOOL; (*command to read an Object Entry*)
		index : UINT;
		subindex : USINT;
		bWriteObj : BOOL;
		bReadObj : BOOL;
	END_STRUCT;
	eplPara_typ : 	STRUCT  (*datatype with parameters*)
		nodeNr : USINT; (*Node number of the Powerling interface*)
		readData : UDINT; (*Read Data*)
		writeData : UDINT; (*Data to be written*)
		MotorVoltage : UINT; (*Motor voltage from parameter read*)
		MotorPower : INT; (*Motor power from parameter read*)
		EnergyCons : UINT;
	END_STRUCT;
	eplFUB_typ : 	STRUCT  (*datatype with Function Blocks*)
		fnEplGetLocalNodeID : EplGetLocalNodeID; (*FUB to get Node Number of local Powerlink interface*)
		fnEplSDOWrite : EplSDOWrite; (*FUB to write a node's object entries*)
		fnEplSDORead : EplSDORead; (*FUB to read a node's object entries*)
	END_STRUCT;
	state_enum : 
		( (*enumerators for the state in the task*)
		EPL_WAIT, (*state to wait for any commands*)
		EPL_READ_NODE, (*state to read powerlink node number*)
		EPL_READ_OE, (*state to read object entry*)
		EPL_WRITE_OE, (*state to write object entry*)
		EPL_ERROR (*state when an error occured*)
		);
END_TYPE
