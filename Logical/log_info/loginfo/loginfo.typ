(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Package: log_info
 * File: log_info.typ
 * Author: lij
 * Created: July 02, 2012
 ********************************************************************
 * Data types of package log_info
 ********************************************************************)

TYPE
	Log_Buffer_typ : 	STRUCT 
		strTime : ARRAY[0..MAX_LOG_BUFFER]OF STRING[79];
		strTaskName : ARRAY[0..MAX_LOG_BUFFER]OF STRING[31];
		strErrNr : ARRAY[0..MAX_LOG_BUFFER]OF STRING[5];
		strContext : ARRAY[0..MAX_LOG_BUFFER]OF STRING[119];
	END_STRUCT;
END_TYPE
