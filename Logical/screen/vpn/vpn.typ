(********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: vpn
 * File: vpn.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program vpn
 ********************************************************************)

TYPE
	Address_typ : 	STRUCT 
		Address3 : USINT;
		Address2 : USINT;
		Address1 : USINT;
		Address0 : USINT;
END_STRUCT;
	VPN_typ : 	STRUCT 
		FlagManualSetIP : BOOL;
		CmdSetMode : BOOL;
		CmdSetIP : BOOL;
		CmdSetSubnetMark : BOOL;
		CmdSetGateway : BOOL;
		Reserve2 : BOOL;
		Reserve1 : BOOL;
		Reserve0 : BOOL;
		CurrentConfigMode : UDINT;
		CurrentIPString : STRING[15];
		CurrentSubnetMarkString : STRING[15];
		CurrentGatewayString : STRING[15];
		ParaSetConfigMode : UDINT;
		ParaSetIP : Address_typ;
		ParaSetSubnetMark : Address_typ;
		ParaSetGateway : Address_typ;
END_STRUCT;	
END_TYPE
