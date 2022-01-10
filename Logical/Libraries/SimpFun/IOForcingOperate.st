FUNCTION_BLOCK IOForcingOperate
(* Implementation of IOForcingOperate *)

(*
//******************************************************************************************************//
//		 B & R - Bernecker and Rainer China 		              										              //
//******************************************************************************************************//
// Library Name:	SimpFun																			                       //
// Function Name:	IOForcingOperate																	                    //
// Autor:			B&R 						      	  										                          //
//------------------------------------------------------------------------------------------------------//
// Description: 	operate IO: Enable/disable 		 									  				              //
//------------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------//
//	History:	 	23.Nov.2008	 V1.00														                             //

//******************************************************************************************************//

//******************************************************************************************************//
//	<|> Variable to control this task:																	                 //
//		Name						Typ						Description									                    //
//		AsIOSetForceValue_0			| AsIOSetForceValue 	| Set force value							              //
//		AsIOEnableForcing_0			| AsIOEnableForcing 	| Enable force value						              //
//		AsIODisableForcing_0		| AsIODisableForcing 	| Disable forceing							           //

//******************************************************************************************************//

//******************************************************************************************************//
//  Others:																								                       //
//		V1.00:																							                       //
			be careful: make sure EnableForcing and DisableForcing, only one is true.
//		Name						Typ						Description									                    //
		IN:
//			enable					| BOOL					| enable this function						              //
//			EnableForcing			| BOOL					| =1:this function use to enable forcing	           //
//			Value					| UDINT					| the forcing value							                 //
//			DisableForcing			| BOOL					| =1:this function use to disable forcing	           //
		OUT:
//			status					| UINT					| =0: no error								                 //
//			InForcing				| USINT					| =0: No forcing							                 //
//															  =1: Forcing to zero						                    //
//															  =2: Forcing to one						                       //

//******************************************************************************************************//
*)

IF (enable = 1) THEN
	CASE (step) OF
		0:			(* init step *)
			status := 65535;
			IF (EnableForcing = 1) THEN								(* Enable Forcing *)
				step := 10;
			ELSIF (DisableForcing = 1) THEN
				step := 30;											(* Disable Forcing *)
			END_IF

		10:			(* set force value *)
			AsIOSetForceValue_0.enable     := 1;
			AsIOSetForceValue_0.pDatapoint := ADR(DataPoint);		(* Pointer to the I/O data point name *)
			AsIOSetForceValue_0.value      := Value;

			AsIOSetForceValue_0();

			IF (AsIOSetForceValue_0.status <> 65535) THEN
				IF (AsIOSetForceValue_0.status = 0) THEN
					step := 20;
				ELSE
					status := AsIOSetForceValue_0.status;
					step := 200;									(* Error *)
				END_IF
			END_IF

		20:			(* Enable forcing *)
			AsIOEnableForcing_0.enable := 1;
			AsIOEnableForcing_0.pDatapoint := ADR(DataPoint);

			AsIOEnableForcing_0();

			IF (AsIOEnableForcing_0.status <> 65535) THEN
				IF (AsIOEnableForcing_0.status = 0) THEN
					IF (Value = 0) THEN
						InForcing := 1;
					ELSE
						InForcing := 2;
					END_IF
					step := 100;									(* Enable forcing ok *)

				ELSE
					status := AsIOEnableForcing_0.status;
					step := 200;									(* Error *)

				END_IF
			END_IF

		30:			(* Disable forcing *)
			AsIODisableForcing_0.enable := 1;
			AsIODisableForcing_0.pDatapoint := ADR(DataPoint);

			AsIODisableForcing_0();

			IF (AsIODisableForcing_0.status <> 65535) THEN
				IF (AsIODisableForcing_0.status = 0) THEN
					InForcing := 0;
					step := 100;									(* Disable forcing ok *)
				ELSE
					status := AsIODisableForcing_0.status;
					step := 200;									(* Error *)
				END_IF
			END_IF

		100:		(* Operator OK *)
			status := 0;
			step := 0;

		200: 		(* Error step *)
			(* the error number has record in status *)
			step := 0;

	END_CASE
END_IF
END_FUNCTION_BLOCK
