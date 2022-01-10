/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: password
 * File: password.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program password --- password control 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "pic_id.h"

/**************************************************************************************
** 					 	 dedine								              			 **
**************************************************************************************/
#define S$STEP_INIT_OK     10000

#define S$STEP_IDLE        100
#define S$STEP_READ        1000
#define S$STEP_WRITE       2000

#define S$STEP_ERROR	      30000

/**************************************************************************************
** 							         function declare  				  			             **
**************************************************************************************/
void Pwd_Set(unsigned short picGet,unsigned short  picObj);
/**************************************************************************************
*     	        		      I N I T U P  O F   T H E   T A S K 		 				       **
**************************************************************************************/

_INIT  void Pwd_init( void )
{
	InputCompletion = 0;

	strcpy(gPwd.PwdInput,   "           ");    	/*  11 space char  */
	strcpy(gPwd.HisPwdInput,"           ");		/*  11 space char  */

	DataObjOperate.InitStep = 100;
}
/**************************************************************************************
*        				    C Y C L I C   P A R T   O F   T H E   T A S K  				  *
**************************************************************************************/

_CYCLIC  void Pwd_cyclic ( void )
{
	
	if (gPasswordLogout)  //ipis0218
	{
		InputCompletion =1;
		strcpy(gPwd.PwdInput,   "           ");    	/*  11 space char  */
		gPasswordLogout =0;
	}
	
	gPanelObj.PasswordLevel = gPwd.PwdLevel;

	switch(DataObjOperate.InitStep)
	{
		case 100:
			DataObjOperate.Ident = 0;

			DatObj_Info.enable = 1;
			DatObj_Info.pName  =(unsigned long)("pwddata");
			DatObjInfo(&DatObj_Info);

			if(doERR_MODULNOTFOUND == DatObj_Info.status)
			{
				DataObjOperate.InitStep = 200;
			}
			else if(0 == DatObj_Info.status)
			{
				DataObjOperate.Ident    = DatObj_Info.ident;
				DataObjOperate.InitStep = 800;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 200:                              	/* create data module */
			DatObj_Create.enable   = 1;
			DatObj_Create.pName    = DatObj_Info.pName;
			DatObj_Create.len      = sizeof(Pwd_typ);
			DatObj_Create.MemType  = doUSRROM;
			DatObj_Create.Option   = 0;
			DatObj_Create.pCpyData = 0;

			DataObjOperate.InitStep = 300;
			break;
		case 300:
			DatObjCreate(&DatObj_Create);
			if(0 == DatObj_Create.status)
			{
				DataObjOperate.Ident = DatObj_Create.ident;
				DataObjOperate.InitStep = 400;
			}
			break;
		case 400:
			memset(&gPwd,0,sizeof(Pwd_typ));

			strcpy(gPwd.Level[2],"8888");
			strcpy(gPwd.Level[1],"7777");
			strcpy(gPwd.Level[0],"6666");

			gPwd.length_1 = 120;
			gPwd.length_2 = 120;
			gPwd.length_3 = 120;

			strcpy(gPwd.Pwd_1,"0098768876");
			strcpy(gPwd.Pwd_2,"0098768872");
			strcpy(gPwd.Pwd_3,"0098768870");

			strcpy(gPwd.PwdInput,   "           ");    	 	 /*  11 space char  */
			strcpy(gPwd.HisPwdInput,"           ");			/*  11 space char  */

			DataObjOperate.InitStep = 500;
			break;
		case 500:
			DatObj_Write.enable  = 1;
			DatObj_Write.ident	 = DataObjOperate.Ident;
			DatObj_Write.pSource = (unsigned long )&gPwd;
			DatObj_Write.len     = sizeof(Pwd_typ);

			DataObjOperate.InitStep = 600;
			break;
		case 600:
			DatObjWrite(&DatObj_Write);
			if(0 == DatObj_Write.status)
			{
				DataObjOperate.InitStep = 700;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 700:
			DataObjOperate.InitStep = 1000;
			break;
		case 800:
			DatObj_Read.enable 		 = 1;
			DatObj_Read.ident 		 = DataObjOperate.Ident ;
			DatObj_Read.Offset 		 = 0;
			DatObj_Read.pDestination = (unsigned long )&gPwd;
			DatObj_Read.len 		 = sizeof(Pwd_typ);

			DataObjOperate.InitStep = 900;
			break;
		case 900:
			DatObjRead(&DatObj_Read);
			if(0 == DatObj_Read.status)
			{
				DataObjOperate.InitStep = 1000;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 1000:
			gPwd.PwdLevel = 0;

			strcpy(gPwd.PwdInput,   "           ");    	/*  11 space char  */
			strcpy(gPwd.HisPwdInput,"           ");	    /*  11 space char  */

			DataObjOperate.Step = S$STEP_IDLE;
			DataObjOperate.InitStep = S$STEP_INIT_OK;
			break;
		case S$STEP_ERROR:
			break;
		default:
			break;
	}/* switch(DataObjOperate.InitStep) */

	if(DataObjOperate.Ident != 0 && DataObjOperate.InitStep == S$STEP_INIT_OK )
	{
		switch(DataObjOperate.Step )
		{
			case S$STEP_IDLE:
				if(1 == gPwd.wSavePassword)
				{
					gPwd.wSavePassword = 0;
					DataObjOperate.Step = S$STEP_WRITE;
				}
				break;
			case S$STEP_WRITE:
				DatObj_Write.enable  = 1;
				DatObj_Write.ident	 = DataObjOperate.Ident;
				DatObj_Write.pSource = (unsigned long )&gPwd;
				DatObj_Write.len     = sizeof(Pwd_typ);

				DataObjOperate.Step = S$STEP_WRITE + 100;
				break;
			case (S$STEP_WRITE + 100):
				DatObjWrite(&DatObj_Write);
				if(0 == DatObj_Write.status)
				{
					DataObjOperate.Step = S$STEP_WRITE + 200;
				}
				break;
			case (S$STEP_WRITE + 200):
				DataObjOperate.Step = S$STEP_IDLE;
				break;
			default:
				break;
		}/* switch(DataObjOperate.Step) */

		WorkingDays = 0;

		if(1 == gPwd.Option[0] && 0 == gPwd.OK_1)
		{
			WorkingDays = WorkingDays + gPwd.length_1;
		}
		else if(1 == gPwd.Option[1] && 0 == gPwd.OK_2)
		{
			WorkingDays = WorkingDays + gPwd.length_2;
		}
		else if(1 == gPwd.Option[2] && 0 == gPwd.OK_3)
		{
			WorkingDays = WorkingDays + gPwd.length_3;
		}

		if(0 == WorkingDays)
		{
			gMachineInfo.TimeStopMachine = 0;
		}
		else
		{
			if(gMachineStopDays > WorkingDays)
			{
				gMachineInfo.TimeStopMachine = 1;
			}
			else
			{
				gMachineInfo.TimeStopMachine = 0;
			}
		}

		if(gAlarm.batteryLow)
		{
			gMachineInfo.TimeStopMachine = 0;
		}

		if(PIC_PASSWORD_LOGIN == gPanelObj.GetPicture || PIC_INIT == gPanelObj.GetPicture)
		{
			if(1 == InputCompletion)
			{
				InputCompletion = 0;
				strcpy(gPwd.HisPwdInput,gPwd.PwdInput);
				strcpy(gPwd.PwdInput,"           ");		/*  11 space char  */

				if( !strcmp(gPwd.HisPwdInput,gPanelObj.PasswordHigh))
				{
			   		gPwd.PwdLevel = 5;
			   	}
				else if( !strcmp(gPwd.HisPwdInput,gPanelObj.Password4))
				{
			   		gPwd.PwdLevel = 4;
			   	}
				else if(!strcmp(gPwd.HisPwdInput,gPwd.Level[2]))
				{
					gPwd.PwdLevel = 3;
				//	gPwd.PwdLevel = 4;  //ipis0226
				}
				else if(!strcmp(gPwd.HisPwdInput,gPwd.Level[1]))
				{
					gPwd.PwdLevel = 2;
				}
				else if(!strcmp(gPwd.HisPwdInput,gPwd.Level[0]))
				{
					gPwd.PwdLevel = 1;
				}
				else
				{
					if(PIC_PASSWORD_LOGIN == gPanelObj.GetPicture)
					{
						if(!strcmp(gPwd.HisPwdInput,gPwd.Pwd_3))
						{
							if(1 == gPwd.Option[2])
							{
								gMachineStopDays = 0;
								gPwd.OK_3 = 1;
								gPwd.OK_2 = 1;
								gPwd.OK_1 = 1;
								gPwd.wSavePassword = 1;
							}
						}
						else if (!strcmp(gPwd.HisPwdInput,gPwd.Pwd_2))
						{
							if(1 == gPwd.Option[1])
							{
								gMachineStopDays = 0;
								gPwd.OK_2 = 1;
								gPwd.OK_1 = 1;
								gPwd.wSavePassword = 1;
							}
						}
						else if (!strcmp(gPwd.HisPwdInput,gPwd.Pwd_1))
						{
							if(1 == gPwd.Option[0])
							{
								gMachineStopDays = 0;
								gPwd.OK_1 = 1;
								gPwd.wSavePassword = 1;
							}
						}
					}/* if(PIC_PASSWORD_LOGIN == gPanelObj.GetPicture) */
					gPwd.PwdLevel = 0;
				}
			}/* if(1 == InputCompletion */
		}/* if(gPanelObj.GetPicture == PIC_PASSWORD_LOGIN || PIC_INIT_PIC == gPanelObj.GetPicture ) */

		Pwd_Set(gPanelObj.GetPicture,PIC_PASSWORD_SET);

	}/* if(DataObjOperate.Ident != 0 && DataObjOperate.InitStep == S$STEP_INIT_OK ) */
}
/**************************************************************************************
** 							Function Realization  				  			         **
**************************************************************************************/

void Pwd_Set(UINT picGet,UINT picObj)
{
	short	i;

 	if (picGet != picObj)
 	{
		gPwd.StatusOfPasswordSet = 0;
		for (i=0;i<=2;i++)
		{
			strcpy(gPwd.PasswordSet[i],   "           ");
			strcpy(gPwd.HisPasswordSet[i],"           ");
		}
 		return;
 	}

 	for (i=0;i<=2;i++)
 	{

 		if(1 == gPwd.CompletionHisPasswordSet[i])
 		{
 			gPwd.StatusOfPasswordSet         = 1;
 			gPwd.CompletionHisPasswordSet[i] = 0;
 		}

 		if(1 == gPwd.CompletionPasswordSet[i])
	 	{
	 		if (strcmp(gPwd.PasswordSet[i] ,"           ")!= 0)
		    {
			    if (0 == strcmp(gPwd.PasswordSet[i],gPwd.HisPasswordSet[i]))
				{
					gPwd.StatusOfPasswordSet  = 2; /* setting ok */
		    		gPwd.wSavePassword	  	 = 1;
		    		strcpy(gPwd.Level[i],gPwd.PasswordSet[i]);

					strcpy(gPwd.PasswordSet[i],   "           ");
					strcpy(gPwd.HisPasswordSet[i],"           ");

		    	}
	    		else
	    		{
	    			gPwd.StatusOfPasswordSet = 3; /* setting failed */

	 				strcpy(gPwd.PasswordSet[i],   "           ");
					strcpy(gPwd.HisPasswordSet[i],"           ");
	    		}
	    	}
			gPwd.CompletionPasswordSet[i] = 0;
	 	}/* if(1 == gPwd.CompletionPasswordSet[i]) */
	}/* for (i=0;i<=2;i++) */
 }/* end of function */


