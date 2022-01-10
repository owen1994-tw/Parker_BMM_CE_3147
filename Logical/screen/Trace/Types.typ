(*Insert your comment here.*)

TYPE
	TraceData_typ : 	STRUCT 
		rData : ARRAY[0..MAX_DATA]OF REAL;
		rYDMin : REAL; (*���H��Сֵ*)
		rYDMax : REAL; (*���H���ֵ*)
		rYMin : REAL; (*�˙C�@ʾ��Сֵ��Ŀ�Ğ���׌����������*)
		rYMax : REAL; (*�˙C�@ʾ���ֵ��Ŀ�Ğ���׌����������*)
		rCursorX : REAL; (*�ڎ׹P�Y��*)
		rCursorY : REAL; (*����Y��*)
	END_STRUCT;
	TraceConfig_typ : 	STRUCT 
		rZoomX : REAL;
		rScrollX : REAL;
		rCursorMove : REAL;
		rHMICursorMove : REAL; (*�دB��Trend��Ԫ����Ŀ���Ǟ���ֱ���c�D��λ��*)
	END_STRUCT;
	TraceCmd_typ : 	STRUCT 
		bEnable : BOOL; (*�����ᣬ�_ʼ�Xȡ�Y�ϣ��Xȡ�ꮅ���Ԅ����0*)
		bHideY : USINT;
		bHideX : USINT;
		bHideData : ARRAY[0..MAX_NUM]OF USINT;
	END_STRUCT;
	Trace_typ : 	STRUCT 
		uStep : UINT;
		uTraceIndex : UINT; (*Channel Index*)
		uDIndex : UINT; (*�Y��Index*)
		Data : ARRAY[0..MAX_NUM]OF TraceData_typ;
		Config : TraceConfig_typ;
		Cmd : TraceCmd_typ;
	END_STRUCT;
END_TYPE
