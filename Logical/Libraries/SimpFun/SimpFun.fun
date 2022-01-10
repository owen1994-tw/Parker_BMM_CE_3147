FUNCTION_BLOCK DateTimeString
VAR
		DTGetTime_0	:DTGetTime;	
		currentTime	:DTStructure;	
		strYYYY	:STRING[4];	
		strDD	:STRING[2];	
		strHH	:STRING[2];	
		strSS	:STRING[2];	
		strMonthM	:STRING[2];	
		strMinuteM	:STRING[2];	
	END_VAR
	VAR_INPUT
		enable	:BOOL;	
		Date_Mode	:USINT;	
	END_VAR
	VAR_OUTPUT
		DisplayString	:STRING[80];	
		StringLength	:UINT;	
	END_VAR
END_FUNCTION_BLOCK
FUNCTION_BLOCK IOForcingOperate
VAR
		step	:USINT;	
		AsIOSetForceValue_0	:AsIOSetForceValue;	
		AsIOEnableForcing_0	:AsIOEnableForcing;	
		AsIODisableForcing_0	:AsIODisableForcing;	
	END_VAR
	VAR_INPUT
		enable	:BOOL;	
		DataPoint	:STRING[80];	
		EnableForcing	:BOOL;	
		Value	:UDINT;	
		DisableForcing	:BOOL;	
	END_VAR
	VAR_OUTPUT
		status	:UINT;	
		InForcing	:USINT;	
	END_VAR
END_FUNCTION_BLOCK
FUNCTION_BLOCK Scaler_ActZone
VAR_INPUT
		Enable	:BOOL;	
		MaxInV	:INT;	
		MinInV	:INT;	
		ActZoneMaxV	:INT;	
		ActZoneMinV	:INT;	
		MaxOutV	:INT;	
		MinOutV	:INT;	
		SV	:INT;	
		Mode_Current	:BOOL;	
	END_VAR
	VAR_OUTPUT
		Out	:INT;	
		status	:UINT;	
	END_VAR
END_FUNCTION_BLOCK
