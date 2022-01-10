/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: pvchg
 * File: pvchg.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program pvchg --- point variable change logger 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "keyap2100.h"
#include "pic_id.h"
#include "edge.h"

/*********************************************************************
**                            defines                               **
*********************************************************************/
#define		C_STEP_WAIT			     100
#define		C_STEP_SAVE		         1000
#define		C_STEP_LOAD			     2000
#define		C_STEP_ERR_CHECK	     3000


#define 	S$UNIT_LENGTH		      8
#define		S$LINE_LENGTH		      (C_PV_NAME_LENGTH + 1 + 4)		

#define		S$LOG_MAX_NO	         200
#define     S$LOG_MAX_SCREEN        (C_MAX_LOG__SCREEN + 1)

/**************************************************************************************
**                            Function Declarations								     **
**************************************************************************************/
void fun_pv_name();

/**************************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				     **
**************************************************************************************/
_INIT void pvchg_init(void)
{
	memset(VarInfoList,0,sizeof(VarInfoList));
	strcpy(pv_get.modname,"pv_chg");

	strcpy(DataObjOperate.ModName,"pvchglog");

	DataObjOperate.InitStep = 100;
	DataObjOperate.Step = C_STEP_WAIT;

	pvChange = 0;

}/* end of _INIT */

/*************************************************************************************
**        				C Y C L I C   P A R T   O F   T H E   T A S K  				**
*************************************************************************************/
_CYCLIC void pvchg_cyc(void)
{
	gMachinePara.MoldL.Carriage.V_Bw = gMachinePara.MoldL.Carriage.V_Bw;
	
	int idx, buffer_idx;

	/* ---------------- step machine init ---------------- */
	switch(DataObjOperate.InitStep)
	{
		case 100:
			fun_pv_name ();
			if (pv_get.errorNum != 0)
			{
				DataObjOperate.InitStep = 9999;
			}
			else
			{
				pv_get.length = DOInfo.len;
				DataObjOperate.Ident  = DOInfo.ident;
				DataObjOperate.InitStep = 200;
			}
			break;

		case 200:
			DOInfo.enable	= 1;
			DOInfo.pName	= (UDINT)DataObjOperate.ModName;
			/* Call  FUB  */
			DatObjInfo(&DOInfo);
			if (DOInfo.status == doERR_MODULNOTFOUND)	/* Data module not found -> create a new one */
			{
				DataObjOperate.InitStep  = 300;
			}
			else
			{
				DataObjOperate.Ident  = DOInfo.ident;
				DataObjOperate.InitStep = 900;
			}
			break;

		case 300:
			/* Initialize create structure */
			DOCreate.enable 	= 1;
			DOCreate.grp	 	= 0;
			DOCreate.pName 	= (UDINT)DataObjOperate.ModName;
			DOCreate.len 		= sizeof(VarLog) * (S$LOG_MAX_NO + 1) + 4;
				DOCreate.MemType 	= doUSRROM;
			DOCreate.Option 	= 0;
			DOCreate.pCpyData 	= 0;
			/* Call FUB */
			DatObjCreate(&DOCreate);
			/* Get FUB output information */
			if( 0 == DOCreate.status)
			{
				idxLog = 0;
				DOCreate.enable = 0;
				DataObjOperate.Ident    = DOCreate.ident;
				DataObjOperate.InitStep = 1000;
			}
			break;

		case 400:
			status_tmpalloc = TMP_alloc(sizeof(VarLog) * (S$LOG_MAX_NO + 1) + 4,(void**)&pData);
				if(0 == status_tmpalloc)
			{
				memset(pData,0,sizeof(VarLog) * (S$LOG_MAX_NO + 1) + 4);
					DataObjOperate.InitStep = 500;
			}
			else
			{
				DataObjOperate.InitStep = 9999;
			}
			break;

		case 500:
			DOWrite.enable  = 1;
			DOWrite.ident   = DataObjOperate.Ident;
			DOWrite.Offset  = 0;
			DOWrite.pSource = (UDINT)pData;
			DOWrite.len 	= sizeof(VarLog) * (S$LOG_MAX_NO + 1) + 4;

				/* Call FUB */
				DatObjWrite(&DOWrite);

			if(0 == DOWrite.status)
			{
				DOWrite.enable = 0;
				DataObjOperate.InitStep = 600;
			}
			break;

		case 600:
			status_tmpfree = TMP_free(sizeof(VarLog) * (S$LOG_MAX_NO + 1) + 4,(void**)pData);
				if(0 == status_tmpfree)
			{
				idxLog = 0;
				idxScreen = 0;
				memset(&VarLogList[0],0,sizeof(VarLogList));
				memset(&VarNameList[0],0,sizeof(VarNameList));
			
				memset(&VarLogList_Buffer[0],0,sizeof(VarLogList_Buffer));
				memset(&VarNameList_Buffer[0],0,sizeof(VarNameList_Buffer));

				DataObjOperate.InitStep = 1000;
			}
			else
			{
				DataObjOperate.InitStep = 9999;
			}
			break;

		case 900:
			DORead.enable 		= 1;
			DORead.ident 		= DataObjOperate.Ident;
			DORead.Offset 		= 0;
			DORead.pDestination	= (UDINT)(&idxLog);
			DORead.len 			= sizeof(idxLog);

			/* Call FUB */
			DatObjRead(&DORead);

			/* Get status */
			if(0 == DORead.status)
			{
				DORead.enable = 0;
				DataObjOperate.InitStep = 1000;
			}
			break;

		case 1000:
			DataObjOperate.InitStep = 3000;
			break;

		case 9999:

			break;
	}/* switch(DataObjOperate.InitStep) */


	if(DOInfo.ident == 0 || DataObjOperate.InitStep != 3000)
	{
		return;
	}
	else
	{
		/* GET pv change edge */
		for(idx=0; idx<pv_get.length/S$LINE_LENGTH;idx++)
			{
			switch ((unsigned short)(VarInfoList[idx].typ))
			{
				/******* BIT *******/
				case 1:
					if( (*((plcbit *)VarInfoList[idx].address)) != (*((plcbit *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((plcbit *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((plcbit *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((plcbit *)VarInfoList[idx].pValue)) = (*((plcbit *)VarInfoList[idx].address));
					break;

				/******* INT8 *******/
				case 2:
					if( (*((SINT *)VarInfoList[idx].address)) != (*((SINT *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((SINT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((SINT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((SINT *)VarInfoList[idx].pValue)) = (*((SINT *)VarInfoList[idx].address));

					break;

				/******* INT16 *******/
				case 3:
					if( (*((INT *)VarInfoList[idx].address)) != (*((INT *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((INT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((INT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((INT *)VarInfoList[idx].pValue)) = (*((INT *)VarInfoList[idx].address));

					break;

				/******* INT32 *******/
				case 4:
					if( (*((DINT *)VarInfoList[idx].address)) != (*((DINT  *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((INT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((INT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((DINT *)VarInfoList[idx].pValue)) = (*((DINT *)VarInfoList[idx].address));

					break;

				/******* BYTE *******/
				case 5:
					if( (*((USINT *)VarInfoList[idx].address)) != (*((USINT *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((USINT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((USINT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((USINT *)VarInfoList[idx].pValue)) = (*((USINT *)VarInfoList[idx].address));

					break;

				/******* WORD *******/
				case 6:
					if( (*((UINT *)VarInfoList[idx].address)) != (*((UINT *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((UINT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((UINT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((UINT *)VarInfoList[idx].pValue)) = (*((UINT *)VarInfoList[idx].address));

					break;

				/******* LONG *******/
				case 7:
					if( (*((UDINT *)VarInfoList[idx].address)) != (*((UDINT *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						itoa((DINT)(*((UDINT *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						itoa((DINT)(*((UDINT *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);					
					
					}
					(*((UDINT *)VarInfoList[idx].pValue)) = (*((UDINT *)VarInfoList[idx].address));
					break;

				/******* FLOAT *******/
				case 8:
					if( (*((REAL *)VarInfoList[idx].address)) != (*((REAL *)VarInfoList[idx].pValue)) )
					{
						pvChange = 1;
						VarLog.idxGroup = VarInfoList[idx].idxGroup;
						VarLog.idxText = VarInfoList[idx].idxText;
						strcpy(VarLog.chgTime,gPanelObj.CurrentTime);
						ftoa((*((REAL *)VarInfoList[idx].pValue)),(UDINT)VarLog.oldValue);
						ftoa((*((REAL *)VarInfoList[idx].address)),(UDINT)VarLog.curValue);
					}
					(*((REAL *)VarInfoList[idx].pValue)) = (*((REAL *)VarInfoList[idx].address));
					break;

				default:
					break;
			}/* End of switch */
		}/* for(idx=0; idx<DOInfo.len/S$LINE_LENGTH,idx++) */

		/* step machine save and load  */
		switch (DataObjOperate.Step)
		{
			case C_STEP_WAIT:
				if(1 == pvChange)
				{
					pvChange = 0;
					DataObjOperate.Step = C_STEP_SAVE;
				}

				if(oldIdxLog  != idxLog)
				{
					memset(&VarLogList[0],0,sizeof(VarLogList));
					memset(&VarNameList[0],0,sizeof(VarNameList));
				
					memset(&VarLogList_Buffer[0],0,sizeof(VarLogList_Buffer));
					memset(&VarNameList_Buffer[0],0,sizeof(VarNameList_Buffer));
				
					//					idxScreen = ((idxLog/S$LOG_MAX_SCREEN  * S$LOG_MAX_SCREEN )%S$LOG_MAX_NO);
					
					//					idxScreen = 0;
					DataObjOperate.Step = C_STEP_LOAD;
					oldIdxLog  = idxLog;
				}

				if(gPanelObj.GetPicture == PIC_PV_LOG)
				{
					CmdPageUp 	= EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F6],OldF6);
					CmdPageDown = EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],OldF7);
				
					if(1 == CmdDelete)
					{
						CmdDelete = 0;
						DataObjOperate.InitStep = 400;
					}
					else if(1 == CmdPageUp)
					{
						CmdPageUp = 0;
						memset(&VarLogList[0],0,sizeof(VarLogList));
						memset(&VarNameList[0],0,sizeof(VarNameList));
						
						memset(&VarLogList_Buffer[0],0,sizeof(VarLogList_Buffer));
						memset(&VarNameList_Buffer[0],0,sizeof(VarNameList_Buffer));
						
						if(idxScreen >= S$LOG_MAX_SCREEN)idxScreen = idxScreen - S$LOG_MAX_SCREEN;
						DataObjOperate.Step = C_STEP_LOAD;
					}
					else if(1 == CmdPageDown)
					{
						if(idxLog >= S$LOG_MAX_SCREEN)
						{
							CmdPageDown = 0;
							memset(&VarLogList[0],0,sizeof(VarLogList));
							memset(&VarNameList[0],0,sizeof(VarNameList));
						
							memset(&VarLogList_Buffer[0],0,sizeof(VarLogList_Buffer));
							memset(&VarNameList_Buffer[0],0,sizeof(VarNameList_Buffer));
						
							if(idxScreen < S$LOG_MAX_NO - S$LOG_MAX_SCREEN)idxScreen = idxScreen + S$LOG_MAX_SCREEN;
						
							DataObjOperate.Step = C_STEP_LOAD;
						}
					}
					else if(1 == bRefresh)
					{
						bRefresh = 0;
						memset(&VarLogList[0],0,sizeof(VarLogList));
						memset(&VarNameList[0],0,sizeof(VarNameList));
						
						memset(&VarLogList_Buffer[0],0,sizeof(VarLogList_Buffer));
						memset(&VarNameList_Buffer[0],0,sizeof(VarNameList_Buffer));
						DataObjOperate.Step = C_STEP_LOAD;
					}
				}
				else
				{
					bRefresh = 1;
				}
				
				break;

			/* ---------------- Save to data module ---------------- */
			case C_STEP_SAVE:
				DOWrite.enable 	= 1;
				DOWrite.ident 	= DataObjOperate.Ident;
				DOWrite.Offset 	= sizeof(VarLog) * (idxLog %S$LOG_MAX_NO) + sizeof(idxLog);
					DOWrite.pSource = (UDINT)(&VarLog);
				DOWrite.len     = sizeof(VarLog);

				DatObjWrite(&DOWrite);

				if(0 == DOWrite.status)
				{
					DOWrite.enable = 0;
					DataObjOperate.Step = C_STEP_SAVE + 100;
				}
				break;

			case C_STEP_SAVE + 100:
				idxLog = idxLog + 1;
				if(idxLog == S$LOG_MAX_NO-1)
					{
					idxLog = 0;
					CmdDelete = 1;
					DataObjOperate.Step = C_STEP_WAIT;
				}
				else
				{
					DataObjOperate.Step = C_STEP_SAVE + 200;
				}
				break;

			case C_STEP_SAVE + 200:
				DOWrite.enable 	= 1;
				DOWrite.ident 	= DataObjOperate.Ident;
				DOWrite.Offset 	= 0;
				DOWrite.pSource = (UDINT)(&idxLog);
				DOWrite.len     = sizeof(idxLog);

				DatObjWrite(&DOWrite);

				if(0 == DOWrite.status)
				{
					DOWrite.enable = 0;
					DataObjOperate.Step = C_STEP_SAVE + 300;
				}
				break;

			case C_STEP_SAVE + 300:
				DataObjOperate.Step = C_STEP_WAIT;
				break;

			/* ---------------- read from data module ---------------- */
			case C_STEP_LOAD:
				DORead.enable 		= 1;
				DORead.ident 		= DataObjOperate.Ident;
				if(0 == idxScreen)
				{
					if(idxLog < S$LOG_MAX_SCREEN)
					{
						DORead.Offset = 0* sizeof(VarLog) + sizeof(idxLog);
					}
					else
					{
						DORead.Offset = (idxLog-S$LOG_MAX_SCREEN)* sizeof(VarLog) + sizeof(idxLog);
					}
				}
				else  //page change 
				{
					if((idxLog - idxScreen) < idxScreen)
					{
						DORead.Offset = 0* sizeof(VarLog) + sizeof(idxLog);
					}
					else
					{
						DORead.Offset = (idxLog-idxScreen*2)* sizeof(VarLog) + sizeof(idxLog);
					}
					
				}	
				DORead.pDestination	= (UDINT)(&VarLogList_Buffer[0]);
				DORead.len 			= sizeof(VarLogList_Buffer);

				/* Call FUB */
				DatObjRead(&DORead);

				/* Get status */
				if(0 == DORead.status)
				{
					DORead.enable = 0;
					DataObjOperate.Step = C_STEP_LOAD + 50;
				}
				break;

			case C_STEP_LOAD + 50:  // buffer -> main(reverse data sequence)
				
				for(idx=0;idx<S$LOG_MAX_SCREEN;idx++)
					{
					if(0 == idxScreen)
					{
						if(idxLog < 20)
						{
							buffer_idx = idxLog - idx - 1 ;
						}
						else
						{
							buffer_idx = S$LOG_MAX_SCREEN - idx - 1 ;
						}
					}
					else
					{
						if((idxLog-idxScreen) < 20)
						{
							buffer_idx = (idxLog-idxScreen) - idx - 1 ;
						}
						else
						{
							buffer_idx = S$LOG_MAX_SCREEN - idx - 1 ;
						}

					}
					if(buffer_idx < 0) break;
					VarLogList[buffer_idx] = VarLogList_Buffer[idx];
				}
			
				DataObjOperate.Step = C_STEP_LOAD + 100;
				
				break;

			
			case C_STEP_LOAD + 100:
				if(gPanelObj.vcHandle != 0)
				{
					if (!VA_Saccess(1,gPanelObj.vcHandle))
					{
						for(idx=0;idx<S$LOG_MAX_SCREEN;idx++)
							{
							pcTextlength = 30;
							if(VarLogList[idx].idxGroup == 0 && VarLogList[idx].idxText == 0)break;
							status=VA_wcGetTextByTextGroup(1,gPanelObj.vcHandle,VarLogList[idx].idxGroup,
								VarLogList[idx].idxText,(UDINT)VarNameList[idx].sText,(UDINT)&pcTextlength);
						}
						VA_Srelease(1,gPanelObj.vcHandle);
					}
				}
				DataObjOperate.Step = C_STEP_WAIT;
				break;
		}/* switch (DataObjOperate.Step) */

	}/* if(DOInfo.ident == 0 || DataObjOperate.InitStep != 3000) */

}/* end of _CYCLIC */

void fun_pv_name ()
{
	DOInfo.enable	= 1;
	DOInfo.pName 	= (UDINT)pv_get.modname;
	DatObjInfo(&DOInfo);

	if (DOInfo.status!= 0)
	{
		pv_get.errorNum = 60001; /*can not find the config data*/
		return;
	}
   
	if(DOInfo.len/S$LINE_LENGTH > C_PV_MAX_NO)
		{
		pv_get.errorNum = 60011; /*can not find the config data*/
		return;   
	}

	while (iLine < DOInfo.len/S$LINE_LENGTH)
		{
		memcpy(&VarInfoList[iLine].pvName,(UDINT*)(DOInfo.pDatObjMem+iLine*S$LINE_LENGTH),S$LINE_LENGTH);

			status = PV_xgetadr(VarInfoList[iLine].pvName, &VarInfoList[iLine].address, &VarInfoList[iLine].length);
		if (status == 0)
		{
		}
		else if (status == 14700) /*Invalid PV name*/
		{
			break;
		}
		else if (status != 0)
		{
			pv_get.errorNum = 60111;
			pv_get.line_no  = iLine + 1;
			return;
		}

		status = PV_ninfo(VarInfoList[iLine].pvName, &VarInfoList[iLine].typ, &Length,&Dimension);
		if (status == 0)
		{
		}
		else if (status == 14700) /*Invalid PV name*/
		{
			break;
		}
		else if (status != 0)
		{
			pv_get.errorNum = 61111;
			pv_get.line_no  = iLine + 1;
			return;
		}
		iLine++;
	}/* while (iLine < DOInfo.len/S$LINE_LENGTH) */

}/* end of function */


