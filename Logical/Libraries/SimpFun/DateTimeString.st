FUNCTION_BLOCK DateTimeString
(* Implementation of ST_DateTimeString *)

(*
//******************************************************************************************************//
//		 B & R - Bernecker and Rainer China 		              										              //
//******************************************************************************************************//
// Library Name:	SimpFun																			                       //
// Function Name:	DateTimeString																		                    //
// Autor:			B&R 						      	  										                          //
//------------------------------------------------------------------------------------------------------//
// Description: 	Print Different type Date and Time									  				              //
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
//	History:	6.Nov.2008	 V1.00														                                 //
//				V1.00: Just 24 hour time mode															                     //
//*******************************************************************************************************//

//*******************************************************************************************************//
//	<|> Variable to control this task:																	                  //
//		Name							Typ					Description									                     //
//		strYYYY							STRING[4]			Save Year as STRING	type			                     //
//		strMonthM						STRING[2]			Save Month as STRING type			                     //
//		strDD							STRING[2]			Save Day as STRING type						                  //
//		strHH							STRING[2]			Save Hour as STRING type					                  //
//		strMinuteM						STRING[2]			Save Minute as STRING type				                  //
//		strSS							STRING[2]			Save Second as STRING type					                  //
//		flagAMPM						BOOL				current time is AM or PM:0=AM,1=PM			                  //
//*******************************************************************************************************//

//*******************************************************************************************************//
//  Others:																								                        //
//		V1.00:																							                        //
//			Date_Mode																					                        //
//				1:		USA date format -- DateMM-DD-YYYY_TimeHH-MM-SS								                  //
//				2:		Asia DATE format -- DateYYYY-MM-DD_TimeHH-MM-SS								                  //
//				others:	Default format -- DateDD-MM-YYYY_TimeHH-MM-SS							                  //
//*******************************************************************************************************//
*)

IF (enable = 1) THEN

	DTGetTime_0.enable := 1;
	DTGetTime_0();

	DT_TO_DTStructure(DTGetTime_0.DT1, ADR(currentTime));

	strcpy(ADR(DisplayString),ADR('Date'));								(* DisplayString = "Date" *)

	itoa(currentTime.year, ADR(strYYYY));								(* string -- "YYYY" *)
	itoa(currentTime.month, ADR(strMonthM));							(* string -- "MM: month" *)
	itoa(currentTime.day, ADR(strDD));									(* string -- "DD" *)

	CASE Date_Mode OF
		1:		(* USA date format -- MM-DD-YYYY *)

			(***** ADD MM   Month *****)
			IF currentTime.month <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "Date0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strMonthM));					(* DisplayString = "DateMM" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateMM-" *)

			(***** ADD DD *****)
			IF currentTime.day <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "DateMM-0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strDD));						(* DisplayString = "DateMM-DD" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateMM-DD-" *)

			(***** ADD YYYY *****)
			strcat(ADR(DisplayString), ADR(strYYYY));					(* DisplayString = "DateMM-DD-YYYY" *)

		2:		(* Asia date format -- YYYY-MM-DD *)

			(***** ADD YYYY *****)
			strcat(ADR(DisplayString), ADR(strYYYY));					(* DisplayString = "DateYYYY" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateYYYY-" *)

			(***** ADD MM   Month *****)
			IF currentTime.month <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "DateYYYY-0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strMonthM));					(* DisplayString = "DateYYYY-MM" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateYYYY-MM-" *)

			(***** ADD DD *****)
			IF currentTime.day <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "DateYYYY-MM-0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strDD));						(* DisplayString = "DateYYYY-MM-DD" *)

		ELSE	(* Default format -- DD-MM-YYYY *)

			(***** ADD DD *****)
			IF currentTime.day <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "Date0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strDD));						(* DisplayString = "DateDD" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateDD-" *)

			(***** ADD MM   Month *****)
			IF currentTime.month <= 9 THEN
				strcat(ADR(DisplayString), ADR('0'));					(* DisplayString = "DateDD-0" *)
			END_IF
			strcat(ADR(DisplayString), ADR(strMonthM));					(* DisplayString = "DateDD-MM" *)
			strcat(ADR(DisplayString), ADR('-'));						(* DisplayString = "DateDD-MM-" *)

			(***** ADD YYYY *****)
			strcat(ADR(DisplayString), ADR(strYYYY));					(* DisplayString = "DateDD-MM-YYYY" *)

	END_CASE

	strcat(ADR(DisplayString),ADR('_Time'));							(* DisplayString = "DateDD-MM-YYYY_Time" *)

	itoa(currentTime.minute, ADR(strMinuteM));							(* string -- "MM: minute" *)
	itoa(currentTime.second, ADR(strSS));								(* string -- "SS" *)

	(***** ADD HH : 24 Hour mode*****)
		IF currentTime.hour <= 9 THEN
			strcat(ADR(DisplayString), ADR('0'));						(* DisplayString = "DateDD-MM-YYYY_Time0" *)
		END_IF
		itoa(currentTime.hour, ADR(strHH));								(* string -- "HH" *)
		strcat(ADR(DisplayString), ADR(strHH));							(* DisplayString = "DateDD-MM-YYYY_TimeHH" *)
	END_IF
	strcat(ADR(DisplayString), ADR('-'));								(* DisplayString = "DateDD-MM-YYYY_TimeHH-" *)

	(***** ADD MM   minute *****)
	IF currentTime.minute <= 9 THEN
		strcat(ADR(DisplayString), ADR('0'));							(* DisplayString = "DateDD-MM-YYYY_TimeHH-0" *)
	END_IF
	strcat(ADR(DisplayString), ADR(strMinuteM));						(* DisplayString = "DateDD-MM-YYYY_TimeHH-MM" *)
	strcat(ADR(DisplayString), ADR('-'));								(* DisplayString = "DateDD-MM-YYYY_TimeHH-MM-" *)

	(***** ADD SS *****)
	IF currentTime.second <= 9 THEN
		strcat(ADR(DisplayString), ADR('0'));							(* DisplayString = "DateDD-MM-YYYY_TimeHH-MM-0" *)
	END_IF
	strcat(ADR(DisplayString), ADR(strSS));								(* DisplayString = "DateDD-MM-YYYY_TimeHH-MM-SS" *)

	StringLength := strlen(ADR(DisplayString));							(* Output StringLength -- length of the DisplayString *)

END_FUNCTION_BLOCK
