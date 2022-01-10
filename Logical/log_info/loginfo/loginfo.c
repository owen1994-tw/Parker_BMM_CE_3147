/**************************************************************************************
*   Copyright:  2010  BERNECKER & RAINER Industrie-Elektronik Ges.m.b.H       															*
*-------------------------------------------------------------------------------------*
*        Identification:                                                              *
*       Taskname : loginfo                                                            *
*       Filename : loginfo.c                                                          *
*       Description : plc log information                                             *
*-------------------------------------------------------------------------------------*
*   History                                                                           *
*   Version     Date         Autor                                                    *
*   01.00       2010.10.8    B&R China     created                       	          *
*   CmdPageUp    -----  system log page up                                     		  *
*   CmdPageDown  -----  system log page down                                          *
*   OptionLog    -----  change log type                                               *
**************************************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "pic_id.h"

/***************************************************************************************
**                              define                                                **
***************************************************************************************/
#define 	C_MAX_LOG_BUFFER      27
#define 	C_MAX_LOG_SCREEN      25

/***************************************************************************************
**                            defines                                                 **
***************************************************************************************/
#define		STEP_WAIT			 0
#define		STEP_GET_INFO		10
#define		STEP_READ_INIT		20
#define		STEP_READ		  	30
#define		STEP_ERR_CHECK	    40
#define		STEP_GET_TEXT		50
#define		STEP_READ_ADD		60
#define		STEP_READ_END		70
#define		STEP_READ_ERR		999

/**************************************************************************************/
/*   						  Functions					                              */
/**************************************************************************************/

/**************************************************************************************
****                    I N I T U P  O F   T H E   T A S K                         ****
**************************************************************************************/
_INIT void init ()
{
	DOInfo.enable = 1;
 	DOInfo.pName  = (UDINT) "log_txt";
    DatObjInfo(&DOInfo);
 	if (DOInfo.status == 0) /* Data object not found or error -> Just don't copy the error message */
	{
		doIdent = DOInfo.ident;
		pSrc = (SINT *)DOInfo.pDatObjMem;
	}
	else
	{
	    pSrc = 0;
	}

	bCmdRefresh = 1;
	OptionLog  = arlogLEVEL_FATAL - 1;  /* error log   */
	StepLog = STEP_WAIT;

	actualIndexOld = 0;
}/* End of Init */
/**************************************************************************************
****                C Y C L I C   P A R T   O F   T H E   T A S K                  ****
**************************************************************************************/
_CYCLIC void cyclic ()
{
	STRING strTemp[30];
	/* page change to system log page */
	if(gPanelObj.GetPicture == PIC_LOG_INFO && oldPicture != PIC_LOG_INFO)
	{
		bCmdRefresh = 1;
	}
	oldPicture = gPanelObj.GetPicture;

	/* ----------------- log type change ----------------- */
	if(OptionLog != OptionLogOld)
	{
		bCmdRefresh    = 1;
		bCmdTypeChange = 1;
	}

	OptionLogOld = OptionLog;

	/* ----------------- step mathine control ----------------- */
	switch(StepLog)
	{
		case STEP_WAIT:
			if(1 == bCmdRefresh)
			{
				bCmdRefresh = 0;
				StepLog = STEP_GET_INFO;
			}

			if(1 == bCmdPageDown)
			{
				bCmdPageDown = 0;
				if(indexRecord > 0)
				{
					StepLog = STEP_READ_INIT;
				}
			}
			else if(1 == bCmdPageUp)
			{
				bCmdPageUp = 0;
				if(indexRecord + C_MAX_LOG_SCREEN <= AsArLogGetInfo_0.actualIndex)
				{
					indexRecord = indexRecord + C_MAX_LOG_SCREEN;
					StepLog = STEP_READ_INIT;
				}
			}
			else if(1 == bCmdPageInit)
			{
				bCmdPageInit = 0;
				indexRecord = AsArLogGetInfo_0.actualIndex;	
				
				StepLog = STEP_READ_INIT;
			}
			else if(1 == bCmdPageEnd)
			{
				bCmdPageEnd = 0;
			
				if(AsArLogGetInfo_0.actualIndex >= C_MAX_LOG_SCREEN)
				{
					indexRecord = C_MAX_LOG_SCREEN;
				}
				else
				{
					indexRecord = AsArLogGetInfo_0.actualIndex;
				}	
			}
			break;

		case STEP_GET_INFO:
			AsArLogGetInfo_0.enable = 1;
			AsArLogGetInfo_0.ident = arlogBASE_SYSTEM_LOG_IDENT;

			/* call function  */
			AsArLogGetInfo(&AsArLogGetInfo_0);

			if(AsArLogGetInfo_0.status != ERR_OK )
			{
				if(AsArLogGetInfo_0.status == ERR_FUB_BUSY)
				{
					AsArLogGetInfo_0.enable = 1;
				}
				else
				{
					AsArLogGetInfo_0.enable = 0;
					StepLog = STEP_READ_ERR;
				}
			}
			else
			{
				AsArLogGetInfo_0.enable = 0;

				if(0 == bCmdTypeChange)
				{
					if(actualIndexOld != AsArLogGetInfo_0.actualIndex)
					{
						actualIndexOld = AsArLogGetInfo_0.actualIndex;
						indexRecord = AsArLogGetInfo_0.actualIndex;
						StepLog = STEP_READ_INIT;
					}
					else
					{
						StepLog = STEP_WAIT;
					}
				}
				else
				{
					bCmdTypeChange = 0;
					actualIndexOld = AsArLogGetInfo_0.actualIndex;
					indexRecord = AsArLogGetInfo_0.actualIndex;
					StepLog = STEP_READ_INIT;
				}
			}
			break;

		case STEP_READ_INIT:
			percentProgressBar = 0;
			indexLog = 0;
			memset(&LogBuffer,0,sizeof(LogBuffer));
			memset(&LogDataOld,0,sizeof(LogDataOld));
			strcpy(sLogContext,"");
			StepLog = STEP_READ;
			break;

		case STEP_READ:
			AsArLogRead_0.enable = 1;
			AsArLogRead_0.ident  = AsArLogGetInfo_0.ident;
			AsArLogRead_0.index  = indexRecord;
			AsArLogRead_0.arlogRead = (UDINT)&LogData;
			AsArLogRead_0.memBin  = (UDINT)&LogBin;
			AsArLogRead_0.lenBin  = 100;
			AsArLogRead_0.memAscii  = (UDINT)&sLogContext;
			AsArLogRead_0.lenAscii  = 100;

			/* call function  */
			AsArLogRead(&AsArLogRead_0);

			if(AsArLogRead_0.status != ERR_OK && AsArLogRead_0.status != arlogERR_READ_BINARY_BUFFER)
			{
				if(AsArLogRead_0.status == ERR_FUB_BUSY)
				{
					AsArLogRead_0.enable = 1;
				}
				else
				{
					AsArLogRead_0.enable = 0;
					StepLog = STEP_READ_ERR;
				}
			}
			else /* if(AsArLogRead_0.status != ERR_OK) */
			{
				AsArLogRead_0.enable = 0;

				/* compact logbook from pcc and display  */
				if(LogData.errornr == LogDataOld.errornr  && LogData.errDay == LogDataOld.errDay  &&
					LogData.errHour == LogDataOld.errHour  && LogData.errMinute == LogDataOld.errMinute)
				{
					StepLog = STEP_READ_ADD;	/* skip  */
				}/*  if(A && B && C	&& D && E) */
				else
				{
					if( LogData.logLevel == OptionLog + 1 )
					{
						/* error number  */
						itoa((DINT)LogData.errornr, (UDINT)sTmp);
						strcat(LogBuffer.strErrNr[indexLog], sTmp);

						/* error task name  */
						memcpy(strTemp,LogData.taskName,30);
						strcpy(LogBuffer.strTaskName[indexLog],strTemp);

						/* get error occur time to string  */
						strcpy(LogBuffer.strTime[indexLog], "");

						itoa((DINT)LogData.errYear, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						strcat(LogBuffer.strTime[indexLog], "/");
						if(LogData.errMonth<10)strcat(LogBuffer.strTime[indexLog], "0");
						itoa((DINT)LogData.errMonth, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						strcat(LogBuffer.strTime[indexLog], "/");
						if(LogData.errDay<10)strcat(LogBuffer.strTime[indexLog], "0");
						itoa((DINT)LogData.errDay, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						strcat(LogBuffer.strTime[indexLog], " ");
						if(LogData.errHour<10)strcat(LogBuffer.strTime[indexLog], "0");
						itoa((DINT)LogData.errHour, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						strcat(LogBuffer.strTime[indexLog], ":");
						if(LogData.errMinute<10)strcat(LogBuffer.strTime[indexLog], "0");
						itoa((DINT)LogData.errMinute, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						strcat(LogBuffer.strTime[indexLog], ":");
						if(LogData.errSecond<10)strcat(LogBuffer.strTime[indexLog], "0");
						itoa((DINT)LogData.errSecond, (UDINT)sTmp);
						strcat(LogBuffer.strTime[indexLog], sTmp);

						LogDataOld = LogData;
						if(strlen(sLogContext) == 0)
						{
							StepLog = STEP_ERR_CHECK;
						}
						else
						{
							strcpy(LogBuffer.strContext[indexLog], sLogContext);
							indexLog = indexLog+1;
							StepLog = STEP_READ_ADD;
						}
					}/* if( LogData.logLevel == OptionLog + 1) */
					else
					{
						StepLog = STEP_READ_ADD;
					}
				}/*  if(A && B && C	&& D && E)else */
			}/* if(AsArLogRead_0.status != ERR_OK) */
			break;

		case STEP_ERR_CHECK:
			if((DOInfo.status == 0)&&(pDat != 0))
			{
				pDat = pSrc;
				StepLog = STEP_GET_TEXT;
			}
			else
			{
				strcpy(LogBuffer.strContext[indexLog], "Can not find,please find out in help");
				StepLog = STEP_READ_ADD;
			}
			break;

		case STEP_GET_TEXT:
			do
			{
				/* Copy the error number */
				if(LogData.errornr <= 65535)  /* ERROR NUBER IS UINT */
				{
					memcpy(&error_read,pDat,2);
					pDat++;
					pDat++;   /* " */
				}
				else                         /* ERROR NUBER IS UDINT */
				{
					memcpy(&error_read,pDat,4);
					pDat++;
					pDat++;	
					pDat++;
					pDat++;	/* " */
				}
				if(error_read == LogData.errornr)				/* The index error is founded */
				{
					strcpy(LogBuffer.strContext[indexLog],pDat);/* Copy the error string      */
					break;
				}
				else if(error_read > LogData.errornr)
				{
					strcpy(LogBuffer.strContext[indexLog],"The error string is not in the data base!");
					break;
				}

				while(*pDat != 0) pDat++;		/* Find the end of the string */
				pDat++;							/* Go to the start of the error number */
			}while(pDat <= ((SINT *) DOInfo.pDatObjMem + DOInfo.len));
			indexLog = indexLog+1;
			StepLog = STEP_READ_ADD;
			break;

		case STEP_READ_ADD:
			strcpy(sLogContext,"");
			indexRecord--;

			if((indexRecord == 0) || (indexLog == C_MAX_LOG_BUFFER))
			{
				StepLog = STEP_READ_END;
			}
			else
			{
				StepLog = STEP_READ;
			}
			break;

		case STEP_READ_END:
			StepLog = STEP_WAIT;
			break;

		case STEP_READ_ERR:
			AsArLogGetInfo_0.enable = 0;
			AsArLogRead_0.enable    = 0;
			StepLog = STEP_WAIT;
			break;

		default:
			break;
	}/* switch(StepLog) */

	/* ------------ progress bar display ------------ */
	if(StepLog > STEP_READ_INIT && StepLog < STEP_READ_END)
	{

		if(OptionLog != arlogLEVEL_WARNING - 1) /* warnning  */
		{
			if(AsArLogGetInfo_0.actualIndex > 0)
			{
				percentProgressBar = (AsArLogGetInfo_0.actualIndex - indexRecord) * 100/AsArLogGetInfo_0.actualIndex ;
			}
		}
		else									/*    */
		{
			if(AsArLogGetInfo_0.actualIndex > C_MAX_LOG_SCREEN)
			{
				percentProgressBar = indexLog * 4;
			}
			else if(AsArLogGetInfo_0.actualIndex > 1)
			{
				percentProgressBar = indexLog * 100 / (AsArLogGetInfo_0.actualIndex-1) ;
			}
		}
	}/* if(StepLog > C_STEP_READ_INIT && StepLog < C_STEP_READ_END) */
}/* End of cyclic */


