(*Insert your comment here.*)

TYPE
	TraceData_typ : 	STRUCT 
		rData : ARRAY[0..MAX_DATA]OF REAL;
		rYDMin : REAL; (*實際最小值*)
		rYDMax : REAL; (*實際最大值*)
		rYMin : REAL; (*人機顯示最小值，目的為了讓曲線更集中*)
		rYMax : REAL; (*人機顯示最大值，目的為了讓曲線更集中*)
		rCursorX : REAL; (*第幾筆資料*)
		rCursorY : REAL; (*光標資料*)
	END_STRUCT;
	TraceConfig_typ : 	STRUCT 
		rZoomX : REAL;
		rScrollX : REAL;
		rCursorMove : REAL;
		rHMICursorMove : REAL; (*重疊在Trend上元件，目的是為了直接點圖找位置*)
	END_STRUCT;
	TraceCmd_typ : 	STRUCT 
		bEnable : BOOL; (*啟動後，開始擷取資料，擷取完畢，自動清為0*)
		bHideY : USINT;
		bHideX : USINT;
		bHideData : ARRAY[0..MAX_NUM]OF USINT;
	END_STRUCT;
	Trace_typ : 	STRUCT 
		uStep : UINT;
		uTraceIndex : UINT; (*Channel Index*)
		uDIndex : UINT; (*資料Index*)
		Data : ARRAY[0..MAX_NUM]OF TraceData_typ;
		Config : TraceConfig_typ;
		Cmd : TraceCmd_typ;
	END_STRUCT;
END_TYPE
