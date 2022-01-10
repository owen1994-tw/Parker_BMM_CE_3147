(********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: trend
 * File: trend.typ
 * Author: lij
 * Created: March 06, 2013
 ********************************************************************
 * Local data types of program trend
 ********************************************************************)

TYPE
	Trend_typ : 	STRUCT 
		Time_ScrollUp : BOOL;
		Time_ScrollDn : BOOL;
		Value_ScrollUp : BOOL;
		Value_ScrollDn : BOOL;
		Time_ZoomInc : BOOL;
		Time_ZoomDec : BOOL;
		Value_ZoomInc : BOOL;
		Value_ZoomDec : BOOL;
		Reset : BOOL;
		OldReset : BOOL;
		TimeMode : UINT;
		Time_Zoom : REAL;
		Value_Zoom : REAL;
		Time_Scroll : REAL;
		Value_Scroll : REAL;
		Cursor1Time : REAL;
		Cursor2Time : REAL;
		Curve : ARRAY[0..TREND_CURVE_NUM]OF Curve_typ;
END_STRUCT;
END_TYPE
