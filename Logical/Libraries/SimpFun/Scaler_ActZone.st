FUNCTION_BLOCK Scaler_ActZone
(* Implementation of Scaler_ActZone *)

IF Enable = 1 THEN
	IF (MaxInV <= MinInV) OR (MaxOutV <= MinOutV) OR (ActZoneMaxV <= ActZoneMinV) THEN

		status := 1;							(* Parameter Error: MaxInV/MaxOutV/ActZoneMaxV must larger than MinInV/MinOutV/ActZoneMinV *)
	ELSIF (ActZoneMinV < MinInV) OR (MaxInV < ActZoneMaxV) THEN
		status := 2;							(* Parameter Error: [ActZoneMinV, ActZoneMaxV] must be loacaled in [MinInV, MaxInV] *)
	ELSIF (Mode_Current = 0) AND ((MinOutV < -10) OR (MaxOutV > 10)) THEN
		status := 3;							(* Parameter Error: For Voltage Output, MaxInV and MinInV must be localed in [-10, 10] *)
	ELSIF (Mode_Current = 1) AND ((MinOutV < -20) OR (MaxOutV > 20)) THEN
		status := 4;							(* Parameter Error: For Current Output, MaxInV and MinInV must be localed in [-20, 20] *)
	ELSE										(* Parameter Correct *)
		status := 0;

		IF (SV < ActZoneMinV) THEN
			IF (Mode_Current = 0) THEN
				Out := REAL_TO_INT(INT_TO_REAL(MinOutV)*32767/10);
			ELSE
				Out := REAL_TO_INT(INT_TO_REAL(MinOutV)*32767/20);
			END_IF
		ELSIF (SV > ActZoneMaxV) THEN
			IF (Mode_Current = 0) THEN
				Out := REAL_TO_INT(INT_TO_REAL(MinOutV)*32767/10);
			ELSE
				Out := REAL_TO_INT(INT_TO_REAL(MinOutV)*32767/20);
			END_IF
		ELSE
			IF (Mode_Current = 0) THEN
				Out := REAL_TO_INT(((INT_TO_REAL(MaxOutV) - MinOutV)*(SV - MinInV)/(MaxInV - MinInV) + MinOutV)*32767/10);
			ELSE
				Out := REAL_TO_INT(((INT_TO_REAL(MaxOutV) - MinOutV)*(SV - MinInV)/(MaxInV - MinInV) + MinOutV)*32767/20);
			END_IF
		END_IF
	END_IF
END_IF
END_FUNCTION_BLOCK
