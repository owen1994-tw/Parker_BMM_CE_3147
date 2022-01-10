/********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: cur_save
 * File: cur_save.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cur_save --- thick profile curve data save/load 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/***********************************************************************************
**                            Include user files								  **
***********************************************************************************/
#include "draw.h"
#include "recipe.h"
#include "keyap2100.h"
#include "pic_id.h"

/***********************************************************************************
** 					 	       Macro Define								          **
***********************************************************************************/
#define  S$CURVE_NUM	5

#define S$STEP_INIT_OK  10000

#define S$STEP_IDLE     100
#define S$STEP_READ     1000
#define S$STEP_WRITE    2000

#define S$STEP_ERROR	   30000

#define S$WAIT_COMMAND			   0

#define S$ROOT_DISK_SAVE		   100
#define S$ROOT_DISK_LOAD		   200
#define S$ROOT_DISK_CLEAR		   500

#define S$USB_SINGLE_OPEN_SAVE	1000
#define S$USB_SINGLE_CREATE		1100
#define S$USB_SINGLE_WRITE		   1300

#define S$USB_SINGLE_OPEN_LOAD	2000
#define S$USB_SINGLE_READ_MEM	   2100
#define S$USB_SINGLE_READ			2200

#define S$USB_SINGLE_CLOSE		   3000

#define S$USB_ALL_OPEN_SAVE	   11000
#define S$USB_ALL_CREATE		   11100
#define S$USB_ALL_MEM_ALLOC	   11200
#define S$USB_ALL_MEM_GET	      11300
#define S$USB_ALL_WRITE		      11400
#define S$USB_ALL_MEM_FREE		   11500

#define S$USB_ALL_OPEN_LOAD	   12000
#define S$USB_ALL_ALLOC_MEM	   12100
#define S$USB_ALL_READ			   12200
#define S$USB_ALL_READ_WRITE	   12300
#define S$USB_ALL_FREE_MEM  	   12400

#define S$USB_ALL_CLOSE		      13000

#define S$ERROR_STEP			      40000
/**********************************************************************************
**                          Function Declarations								 **
**********************************************************************************/
void changeCurve(USINT *enable);
void newCurve();
void writeData(UINT Index);
void readData(UINT Index);
void initCurve(unsigned short length);
/**********************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				 **
**********************************************************************************/
_INIT  void init(void)
{
	strcpy(fileSingleName, "curve.usb");		/* file single name */
	strcpy(fileAllName, "curveAll.usb");		/* file all name */
	
	strcpy(DataObjOperate.ModName,"recCurve");
	strcpy(DataObjOperate.Status,"");
	DataObjOperate.InitStep = 100;
}/*end of INIT*/

/**********************************************************************************
**        			C Y C L I C   P A R T   O F   T H E   T A S K  				 **
**********************************************************************************/
_CYCLIC void cyc(void)
{
	/*  配方名字及数据初始化  */
	switch(DataObjOperate.InitStep)
	{
		case 100:
			/* Initialize info structure */
			DataObjOperate.Ident = 0;
			DatObj_Info.enable	= 1;
			DatObj_Info.pName	   = (UDINT)DataObjOperate.ModName;

			/* Call  FUB  */
			DatObjInfo(&DatObj_Info);

			if(doERR_MODULNOTFOUND == DatObj_Info.status)
			{
				DatObj_Info.enable = 0;
				DataObjOperate.InitStep = 200;
			}
			else if(0 == DatObj_Info.status)
			{
				DatObj_Info.enable = 0;
				DataObjOperate.Ident = DatObj_Info.ident;
				DataObjOperate.InitStep  = 500;
			}
			else if (DatObj_Info.status != ERR_FUB_BUSY)
			{
				DatObj_Info.enable = 0;
				DataObjOperate.InitStep = 40000;
			}
			break;
      
		case 200:
			/* Initialize create structure */
			DatObj_Create.enable = 1;
			DatObj_Create.grp	 	= 0;
			DatObj_Create.pName 	= (UDINT)DataObjOperate.ModName;
			DatObj_Create.len 	= sizeof(curveData)*(S$NUM_RECIPE + 1);
				DatObj_Create.MemType  = doUSRROM;
			DatObj_Create.Option   = 0;
			DatObj_Create.pCpyData = 0;

			/* Call FUB */
			DatObjCreate(&DatObj_Create);

			if(0 == DatObj_Create.status)
			{
				DataObjOperate.Ident = DatObj_Create.ident;
				DataObjOperate.InitStep = 300;
			}
			break;
      
		case 300:
			counter = 0;
			initCurve(S$THICK_INIT_LENGTH);
				DataObjOperate.InitStep = 350;
			break;
      
		case 350:
			status_tmpalloc = TMP_alloc(sizeof(curveData) * (S$NUM_RECIPE + 1), (void*)&pCurveData);
				if(0 == status_tmpalloc)
			{
				for(i=0;i<(S$NUM_RECIPE+1) * S$CURVE_NUM;i++)
					{
					((curveData_typ *)pCurveData)[i] = curveData[0];
				}
				DataObjOperate.InitStep = 380;
			}
			else
			{
				DataObjOperate.InitStep = 40001;
			}
			break;
      
		case 380:
			DatObj_Write.enable  = 1;
			DatObj_Write.ident 	= DataObjOperate.Ident;
			DatObj_Write.Offset  = 0;
			DatObj_Write.pSource = (UDINT)pCurveData;
			DatObj_Write.len 	   = sizeof(curveData) * (S$NUM_RECIPE + 1);

				/* Call FUB */
				DatObjWrite(&DatObj_Write);

			if(0 == DatObj_Write.status )
			{
				DataObjOperate.InitStep = 400;
			}
			break;
      
		case 400:
			status_tmpfree = TMP_free(sizeof(curveData) * (S$NUM_RECIPE + 1), (void*)pCurveData);
				if(0 == status_tmpfree)
			{
				readData(curveIndex);
				DataObjOperate.InitStep = 700;
			}
			else
			{
				DataObjOperate.InitStep = 40002;
			}
			break;
      
		case 500:
			if(1 == gRecipeOperate.NameOk)
			{
				DataObjOperate.InitStep = 600;
			}
			break;
      
		case 600:
			/* Initialize read structure */
			DatObj_Read.enable 		 = 1;
			DatObj_Read.ident 		 = DataObjOperate.Ident;
			DatObj_Read.Offset 		 = sizeof(curveData) * S$NUM_RECIPE;
			DatObj_Read.pDestination = (UDINT) &curveData;
			DatObj_Read.len 		    = sizeof (curveData);

			/* Call FUB */
			DatObjRead(&DatObj_Read);
			if (curveData[0].usedIndex < S$CURVE_NUM )
				{
				curveIndex = curveData[0].usedIndex;
			}
			else
			{
				curveIndex	= S$CURVE_NUM - 1;
			}

			readData(curveIndex);

			if(0 == DatObj_Read.status)
			{
				DataObjOperate.InitStep = 700;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
      
		case 700:       /*   recipe data  ok*/
			gRecipeOperate.CurveOk  = 1;
			strcpy(DataObjOperate.Status,"curvedata init ok !");
			DataObjOperate.Step     = S$STEP_IDLE;
			DataObjOperate.InitStep = S$STEP_INIT_OK;
			break;
      
		case S$STEP_INIT_OK:
				break;
      
		case S$STEP_ERROR:
				break;
      
		default:
			break;
	}/* switch(DataObjOperate.InitStep) */

	if((0 == DataObjOperate.Ident)||(DataObjOperate.InitStep != S$STEP_INIT_OK ))
		{
		gRecipeOperate.CurveUSBSave = 0;
		gRecipeOperate.CurveUSBLoad = 0;
		curveIndexOld = curveIndex;
		return;
	}

	/* ----------------- Step machine control ----------------- */
	switch (DataObjOperate.Step)
	{
		case S$WAIT_COMMAND:
				gRecipeOperate.usbCurveOk = 1;
			if(1 == gRecipeOperate.CurveSave)
			{
				gRecipeOperate.CurveSave = 0;
				indexRecipe = S$NUM_RECIPE;			/*  current recipe in data module-index = S$NUM_RECIPE */
				DataObjOperate.Step = S$ROOT_DISK_SAVE;
			}/* else if(1 == gRecipeOperate.CurveSave) */
			else if(1 == gRecipeOperate.CurveLoad)
			{
				gRecipeOperate.CurveLoad = 0;
				indexRecipe = S$NUM_RECIPE;			/*  current recipe in data module-index = S$NUM_RECIPE */
				DataObjOperate.Step = S$ROOT_DISK_LOAD;
			}/* else if(1 == gRecipeOperate.CurveSave) */
			else if(1 == gRecipeOperate.CurveRecipeSave)
			{
				gRecipeOperate.CurveRecipeSave = 0;
				indexRecipe = gRecipeOperate.Index % S$NUM_RECIPE; 
				DataObjOperate.Step = S$ROOT_DISK_SAVE;
			}/* else if(1 == gRecipeOperate.CurveSave) */
			else if(1 == gRecipeOperate.CurveRecipeLoad)
			{
				gRecipeOperate.CurveRecipeLoad = 0;
				indexRecipe = gRecipeOperate.Index % S$NUM_RECIPE; 
				DataObjOperate.Step = S$ROOT_DISK_LOAD;
				gRecipeOperate.CurveSave = 1;		/*  read from recipe then write to current recipe(S$NUM_RECIPE) in data module */
			}/* else if(1 == gRecipeOperate.CurveSave) */
			else if(1 == gRecipeOperate.CurveUSBSave)
			{
				gRecipeOperate.usbCurveOk = 0;
				gRecipeOperate.CurveUSBSave = 0;
				DataObjOperate.Step = S$USB_SINGLE_OPEN_SAVE;
			}/* else if(1 == gRecipeOperate.CurveUSBSave) */
			else if(1 == gRecipeOperate.CurveUSBLoad)
			{
				gRecipeOperate.usbCurveOk = 0;
				gRecipeOperate.CurveUSBLoad = 0;
				DataObjOperate.Step = S$USB_SINGLE_OPEN_LOAD;
				gRecipeOperate.CurveSave = 1;		/*  read from USB then write to current recipe(S$NUM_RECIPE) in data module */
			}/* else if(1 == gRecipeOperate.CurveUSBLoad) */
			else if(1 == gRecipeOperate.CurveUSBLoadAll)
			{
				gRecipeOperate.usbCurveOk = 0;
				gRecipeOperate.CurveUSBLoadAll = 0;
				DataObjOperate.Step = S$USB_ALL_OPEN_LOAD;
			}
			else if(1 == gRecipeOperate.CurveUSBSaveAll)
			{
				gRecipeOperate.usbCurveOk = 0;
				gRecipeOperate.CurveUSBSaveAll = 0;
				DataObjOperate.Step = S$USB_ALL_OPEN_SAVE;
			}
			break;
	
		/* ---------------- Save to root disk ---------------- */	
		case S$ROOT_DISK_SAVE:
				curveData[0].usedIndex = curveIndex;
			writeData(curveIndex);
			/* Initialize write structure */
			DatObj_Write.enable 	= 1;
			DatObj_Write.ident 	= DataObjOperate.Ident;
			DatObj_Write.Offset 	= sizeof(curveData)*indexRecipe;
			DatObj_Write.pSource = (UDINT)&curveData;
			DatObj_Write.len 	   = sizeof(curveData);
	 
			/* Call FUB */
			DatObjWrite(&DatObj_Write);
			
			if(DatObj_Write.status == 0)
			{
				strcpy(DataObjOperate.Status,"curvedata write ok !");
			}
			else
			{
				strcpy(DataObjOperate.Status,"curvedata write false !");
			}
	
			DataObjOperate.Step = S$WAIT_COMMAND; 
			break;
		
		/* ---------------- Load from root disk ---------------- */	
		case S$ROOT_DISK_LOAD:
				/* Initialize read structure */
				DatObj_Read.enable 		  = 1;
			DatObj_Read.ident 		  = DataObjOperate.Ident;
			DatObj_Read.Offset 		  = sizeof(curveData)*indexRecipe;
			DatObj_Read.pDestination  = (UDINT)&curveData;
			DatObj_Read.len 		     = sizeof(curveData);
	 
			/* Call FUB */
			DatObjRead(&DatObj_Read);
			
			if(DatObj_Read.status == 0)
			{
				strcpy(DataObjOperate.Status,"curvedata read ok !");
			}
			else
			{
				strcpy(DataObjOperate.Status,"curvedata read false !");
			}
			
			if (curveData[0].usedIndex < S$CURVE_NUM)
				{
				curveIndex = curveData[0].usedIndex;
			}
			else
			{
				curveIndex	= S$CURVE_NUM - 1;
			}
			
			readData(curveIndex);

			DataObjOperate.Step = S$WAIT_COMMAND; 
			break;
	
		/* --------------------------------------------------------------- */
		/* ---------------- Save Single curve to usb disk ---------------- */
		/* --------------------------------------------------------------- */
		case S$USB_SINGLE_OPEN_SAVE:			/* file OPEN for save  */
				FileOpen_0.enable  = 1;
			FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileOpen_0.pFile   = (UDINT)&fileSingleName;
			FileOpen_0.mode    = fiREAD_WRITE;
			
			/* Call FUB */
			FileOpen(&FileOpen_0);
 
			if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)/* no file   */
			{
				FileOpen_0.enable  = 0;
				DataObjOperate.Step = S$USB_SINGLE_CREATE;
			}
			else 
			{
				if(ERR_OK == FileOpen_0.status)			/* file exist */
				{
					FileOpen_0.enable  = 0;
					if(FileOpen_0.filelen == sizeof(curveData))
					{
						DataObjOperate.IdentUSB = FileOpen_0.ident;
						DataObjOperate.Step = S$USB_SINGLE_WRITE;
					}
					else
					{
						gImply.CurveUSB_Length = 1;
						DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
					}
				}
				else if (FileOpen_0.status != ERR_FUB_BUSY) 
				{
					FileOpen_0.enable  = 0;
					gImply.CurveOP_False = 1;
					DataObjOperate.Step = S$ERROR_STEP;
				}
			}
			break;
			
		case S$USB_SINGLE_CREATE:			/* file not exist, create*/
				FileCreate_0.enable  = 1;
			FileCreate_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileCreate_0.pFile   = (UDINT)&fileSingleName;
			
			/* Call FUB */
			FileCreate(&FileCreate_0);

			if(ERR_OK == FileCreate_0.status)		/* ok */
			{
				FileCreate_0.enable = 0;
				DataObjOperate.IdentUSB = FileCreate_0.ident;
				DataObjOperate.Step = S$USB_SINGLE_WRITE;
			}
			else if (FileCreate_0.status != ERR_FUB_BUSY) 
			{
				FileCreate_0.enable= 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_SINGLE_CLOSE;
			}
			break;
			
		case S$USB_SINGLE_WRITE:		/* file write */
				FileWrite_0.enable = 1;
			FileWrite_0.ident  = DataObjOperate.IdentUSB;
			FileWrite_0.offset = 0;
			FileWrite_0.pSrc = (UDINT)&curveData;
			FileWrite_0.len  = sizeof(curveData);
			
			/* Call FUB */
			FileWrite(&FileWrite_0);

			if(ERR_OK == FileWrite_0.status)				/* ok */
			{
				FileWrite_0.enable = 0;
				gImply.CurveOP_OK = 1;
				DataObjOperate.Step = S$USB_SINGLE_CLOSE;
			}
			else if (FileWrite_0.status != ERR_FUB_BUSY) 
			{
				FileWrite_0.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_SINGLE_CLOSE;
			}
			break;
		
		/* ----------------------------------------------------------------- */
		/* ---------------- Load single curve from USB disk ---------------- */
		/* ----------------------------------------------------------------- */
		case S$USB_SINGLE_OPEN_LOAD:			/* file OPEN  for load  */
				FileOpen_0.enable  = 1;
			FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileOpen_0.pFile   = (UDINT)&fileSingleName;
			FileOpen_0.mode    = fiREAD_WRITE;
			
			/* Call FUB */
			FileOpen(&FileOpen_0);
 
			if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)	/* no file   */
			{
				FileOpen_0.enable  = 0;
				gImply.FixUSB_NoFile = 1;
				DataObjOperate.Step = S$WAIT_COMMAND;
			}
			else 
			{
				if(ERR_OK == FileOpen_0.status)				/* file exist */
				{
					FileOpen_0.enable = 0;
					if(FileOpen_0.filelen == sizeof(curveData))
					{
						DataObjOperate.IdentUSB = FileOpen_0.ident;
						DataObjOperate.Step = S$USB_SINGLE_READ;
					}
					else
					{
						gImply.CurveUSB_Length = 1;
						DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
					}
				}
				else if (FileOpen_0.status != ERR_FUB_BUSY) 
				{
					FileOpen_0.enable = 0;
					gImply.CurveOP_False = 1;
					DataObjOperate.Step = S$ERROR_STEP;
				}
			}
			break;
			
		case S$USB_SINGLE_READ:			/* file single read  */
				FileRead_0.enable = 1;
			FileRead_0.ident  = DataObjOperate.IdentUSB;
			FileRead_0.offset = 0;
			FileRead_0.pDest  = (UDINT)&curveData;
			FileRead_0.len    = sizeof(curveData);
			
			/* Call FUB */
			FileRead(&FileRead_0);

			if(ERR_OK == FileRead_0.status)	/* ok */
			{
				FileRead_0.enable = 0;
				gImply.CurveOP_OK = 1;
				if (curveData[0].usedIndex < S$CURVE_NUM)
					{
					curveIndex = curveData[0].usedIndex;
				}
				else
				{
					curveIndex	= S$CURVE_NUM - 1;
				}
        
				readData(curveIndex);
        
				gRecipeOperate.CurveSave = 1;		/*  read from USB then write to current recipe(S$NUM_RECIPE) in data module */
				DataObjOperate.Step = S$USB_SINGLE_CLOSE;
			}
			else if (FileRead_0.status != ERR_FUB_BUSY) 
			{
				FileRead_0.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_SINGLE_CLOSE;
			}
			break;
      
		/* ------------------------------------------------------- */
		/* ---------------- Close USB file device ---------------- */
		/* ------------------------------------------------------- */
		case S$USB_SINGLE_CLOSE:			/* file close*/
				FileClose_0.enable = 1;
			FileClose_0.ident  = DataObjOperate.IdentUSB;
			
			/* Call FBK */
			FileClose(&FileClose_0);
            
			if(ERR_OK == FileClose_0.status)	/* ok */
			{
				FileClose_0.enable = 0;
				DataObjOperate.Step = S$WAIT_COMMAND;
			}
			else if (FileClose_0.status != ERR_FUB_BUSY) 
			{
				FileClose_0.enable = 0;
				DataObjOperate.Step = S$ERROR_STEP;
			}
			break;
		
		/* ------------------------------------------------------ */
		/* ---------------- All Save to usb disk ---------------- */
		/* ------------------------------------------------------ */
		case S$USB_ALL_OPEN_SAVE:			/* file OPEN for save  */
				FileOpen_0.enable  = 1;
			FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileOpen_0.pFile   = (UDINT)&fileAllName;
			FileOpen_0.mode    = fiREAD_WRITE;
			
			/* Call FUB */
			FileOpen(&FileOpen_0);
 
			if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)/* no file   */
			{
				FileOpen_0.enable   = 0;
				DataObjOperate.Step = S$USB_ALL_CREATE;
			}
			else 
			{
				if(ERR_OK == FileOpen_0.status)			/* file exist */
				{
					FileOpen_0.enable  = 0;
					if(FileOpen_0.filelen == sizeof(curveData)*(S$NUM_RECIPE + 1))
						{
						DataObjOperate.IdentAllUSB = FileOpen_0.ident;
						DataObjOperate.Step = S$USB_ALL_MEM_ALLOC;
					}
					else
					{
						gImply.CurveUSB_Length = 1;
						DataObjOperate.Step = S$USB_ALL_CLOSE;	
					}
				}
				else if (FileOpen_0.status != ERR_FUB_BUSY) 
				{
					FileOpen_0.enable  = 0;
					gImply.CurveOP_False = 1;
					DataObjOperate.Step = S$ERROR_STEP;
				}
			}
			break;
			
		case S$USB_ALL_CREATE:			/* file not exist, create*/
				FileCreate_0.enable  = 1;
			FileCreate_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileCreate_0.pFile   = (UDINT)&fileAllName;
			
			/* Call FUB */
			FileCreate(&FileCreate_0);

			if(ERR_OK == FileCreate_0.status)		/* ok */
			{
				FileCreate_0.enable = 0;
				DataObjOperate.IdentAllUSB = FileCreate_0.ident;
				DataObjOperate.Step = S$USB_ALL_MEM_ALLOC;
			}
			else if (FileCreate_0.status != ERR_FUB_BUSY) 
			{
				FileCreate_0.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}
			break;
			
		case S$USB_ALL_MEM_ALLOC:	/* file request All memory */
				status_tmpalloc = TMP_alloc(sizeof(curveData)*(S$NUM_RECIPE + 1), (void*)&pCurveData);
				if(0 == status_tmpalloc)
			{
				memset((void *)pCurveData,0,sizeof(curveData)*(S$NUM_RECIPE + 1));
					DataObjOperate.Step = S$USB_ALL_MEM_GET;
			}
			else
			{
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}  
			break;
      
		case S$USB_ALL_MEM_GET:
				DatObj_Read.enable = 1;
			DatObj_Read.ident  = DataObjOperate.Ident;
			DatObj_Read.Offset = 0;
			DatObj_Read.pDestination = (UDINT)pCurveData;
			DatObj_Read.len = sizeof(curveData)*(S$NUM_RECIPE + 1);
 
				/* Call FUB */
				DatObjRead(&DatObj_Read); 
			if(0 == DatObj_Read.status)
			{
				DatObj_Read.enable = 0;
				DataObjOperate.Step = S$USB_ALL_WRITE;
			}
			else if (DatObj_Read.status != ERR_FUB_BUSY)
			{
				DatObj_Read.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_MEM_FREE;
			}
			break;
			
		case S$USB_ALL_WRITE:		/* file single write */
				FileWrite_0.enable = 1;
			FileWrite_0.ident  = DataObjOperate.IdentAllUSB;
			FileWrite_0.offset = 0;
			FileWrite_0.pSrc = (UDINT)pCurveData;
			FileWrite_0.len  = sizeof(curveData)*(S$NUM_RECIPE + 1);
			
				/* Call FUB */
				FileWrite(&FileWrite_0);

			if(ERR_OK == FileWrite_0.status)				/* ok */
			{
				FileWrite_0.enable = 0;
				gImply.CurveOP_OK = 1;
				DataObjOperate.Step = S$USB_ALL_MEM_FREE;
			}
			else if (FileWrite_0.status != ERR_FUB_BUSY) 
			{
				FileWrite_0.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_MEM_FREE;
			}
			break;
      
		case S$USB_ALL_MEM_FREE:
				status_tmpfree = TMP_free(sizeof(curveData)*(S$NUM_RECIPE + 1), (void*)pCurveData);
				if(0 == status_tmpfree)
			{
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}
			else
			{
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}
			break;
		
		/* ----------------------------------------------------------- */
		/* ----------------  All Load from USB disk   ---------------- */
		/* ----------------------------------------------------------- */
		case S$USB_ALL_OPEN_LOAD:			/* file OPEN  for load  */
				FileOpen_0.enable  = 1;
			FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
			FileOpen_0.pFile   = (UDINT)&fileAllName;
			FileOpen_0.mode    = fiREAD_WRITE;
			
			/* Call FUB */
			FileOpen(&FileOpen_0);
 
			if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)	/* no file   */
			{
				FileOpen_0.enable  = 0;
				gImply.CurveUSB_NoFile = 1;
				DataObjOperate.Step = S$WAIT_COMMAND;
			}
			else 
			{
				if(ERR_OK == FileOpen_0.status)				/* file exist */
				{
					FileOpen_0.enable = 0;
					if(FileOpen_0.filelen == sizeof(curveData)*(S$NUM_RECIPE + 1))
						{
						DataObjOperate.IdentAllUSB = FileOpen_0.ident;
						DataObjOperate.Step = S$USB_ALL_ALLOC_MEM;
					}
					else
					{
						gImply.CurveUSB_Length = 1;
						DataObjOperate.Step = S$USB_ALL_CLOSE;	
					}
				}
				else if (FileOpen_0.status != ERR_FUB_BUSY) 
				{
					FileOpen_0.enable = 0;
					gImply.CurveOP_False = 1;
					DataObjOperate.Step = S$ERROR_STEP;
				}
			}
			break;
		
		case S$USB_ALL_ALLOC_MEM:			/* file  read request memory */
				status_tmpalloc = TMP_alloc(sizeof(curveData)*(S$NUM_RECIPE + 1), (void*)&pCurveData);
				if(0 == status_tmpalloc)
			{
				DataObjOperate.Step = S$USB_ALL_READ;
			}
			else
			{
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}
			break;
			
		case S$USB_ALL_READ:			/* file  read  */
				FileRead_0.enable = 1;
			FileRead_0.ident  = DataObjOperate.IdentAllUSB;
			FileRead_0.offset = 0;
			FileRead_0.pDest  = (UDINT)pCurveData;
			FileRead_0.len    = sizeof(curveData)*(S$NUM_RECIPE + 1);
			
				/* Call FUB */
				FileRead(&FileRead_0);

			if(ERR_OK == FileRead_0.status)	/* ok */
			{
				FileRead_0.enable = 0;
				DataObjOperate.Step = S$USB_ALL_READ_WRITE;
			}
			else if (FileRead_0.status != ERR_FUB_BUSY) 
			{
				FileRead_0.enable = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_FREE_MEM;
			}
			break;
      
		case S$USB_ALL_READ_WRITE:
				DatObj_Write.enable  = 1;
			DatObj_Write.ident 	 = DataObjOperate.Ident;
			DatObj_Write.Offset  = 0;
			DatObj_Write.pSource = (UDINT)pCurveData;
			DatObj_Write.len 	 = sizeof(curveData)*(S$NUM_RECIPE + 1);
         
				DatObjWrite(&DatObj_Write);
         
			if(0 == DatObj_Write.status)
			{
				DatObj_Write.enable  = 0;
				gImply.CurveOP_OK = 1;
				gRecipeOperate.CurveLoad = 1;
				DataObjOperate.Step = S$USB_ALL_FREE_MEM;
			}
			else if (DatObj_Write.status != ERR_FUB_BUSY)
			{
				DatObj_Write.enable  = 0;
				gImply.CurveOP_False = 1;
				DataObjOperate.Step = S$USB_ALL_FREE_MEM;
			}
			break;
      
		case S$USB_ALL_FREE_MEM:
				status_tmpfree = TMP_free(sizeof(curveData)*(S$NUM_RECIPE + 1), (void*)pCurveData);
				if(0 == status_tmpfree)
			{
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}
			else
			{
				DataObjOperate.Step = S$USB_ALL_CLOSE;
			}         
			break;
		/* ----------------------------------------------------------- */
		/* ---------------- All Close USB file device ---------------- */
		/* ----------------------------------------------------------- */
		case S$USB_ALL_CLOSE:			/* file close*/
				FileClose_0.enable = 1;
			FileClose_0.ident  = DataObjOperate.IdentAllUSB;
			
			/* Call FBK */
			FileClose(&FileClose_0);
            
			if(ERR_OK == FileClose_0.status)	/* ok */
			{
				FileClose_0.enable = 0;
				DataObjOperate.Step = S$WAIT_COMMAND;
			}
			else if (FileClose_0.status != ERR_FUB_BUSY) 
			{
				FileClose_0.enable = 0;
				DataObjOperate.Step = S$ERROR_STEP;
			}
			break;
      
		case S$ERROR_STEP:
				DataObjOperate.Step = S$WAIT_COMMAND; 
			break;
	}/* switch (DataObjOperate.Step) */
	
	if(1 == bCurveIndexChgBtn)
	{
		bCurveIndexChgBtn = 0;
		curveIndex = curveIndex + 1;
		curveIndex = curveIndex % 5;
	}
	
	if(curveIndex != curveIndexOld)
	{
		ChangeEnable = 1;
		changeCurve(&ChangeEnable);
		bCurveIndexChange = 1;
	}
	curveIndexOld = curveIndex;

}/*end of CYCLIC*/

/**************************************************************************************
**        				FUNCTION  REALIZE				   							 **
**************************************************************************************/
void changeCurve(USINT *enable)
{

	if (1 == *enable)
	{
		writeData(curveIndexOld);

		/*curveIndex = curveIndex + 1;*/

		curveIndex = curveIndex % S$CURVE_NUM;

		if (curveData[curveIndex].CurveNode[S$THICK_MAX].v3.y == 0)
			{
			newCurve();
		}
		else
		{
			readData(curveIndex);
		}

		*enable = 0;
	}
}

void newCurve()
{
	memset(SetPos,0,sizeof(SetPos));
	memset(CurveNode,0,sizeof(CurveNode));

	for( i = 0; i<= S$THICK_MAX; i++)
		{
		DrawMan.weight = 300;
		SetPos[i].x = DrawMan.weight;
		SetPos[i].y = i;
	}

	DrawMan.oldWeight 			=  DrawMan.weight;
	DrawMan.v1 						=  (DrawMan.weight) + 1;
	DrawMan.v2 						=  (DrawMan.weight) + 1;
	DrawMan.v03 					=  0;
	DrawMan.currentNode  		=  S$THICK_MAX;
	DrawMan.currentPoint 		=  0;
	DrawMan.flagLineFocus 		=  0;
	CurveNode[0].former    	=  S$THICK_MAX;
	CurveNode[0].next    		=  S$THICK_MAX;
	CurveNode[0].v3.x    		=  DrawMan.weight;
	CurveNode[0].v3.y    		=  0;
	CurveNode[0].flagNode  		=  1;

	CurveNode[S$THICK_MAX].former 	= 0;
	CurveNode[S$THICK_MAX].next   	= 0;
	CurveNode[S$THICK_MAX].v0.x   	= DrawMan.weight;
	CurveNode[S$THICK_MAX].v0.y   	= 0;
	CurveNode[S$THICK_MAX].v3.x   	= DrawMan.weight;
	CurveNode[S$THICK_MAX].v3.y   	= S$THICK_MAX;

	CurveNode[S$THICK_MAX].v1.x   	= (DrawMan.weight) + 1;
	CurveNode[S$THICK_MAX].v1.y   	= CurveNode[S$THICK_MAX].v0.y +
		(CurveNode[S$THICK_MAX].v3.y - CurveNode[S$THICK_MAX].v0.y)*1/
		3 ;
		CurveNode[S$THICK_MAX].v2.x   	= (DrawMan.weight) + 1;
	CurveNode[S$THICK_MAX].v2.y   	= CurveNode[S$THICK_MAX].v0.y +
		(CurveNode[S$THICK_MAX].v3.y - CurveNode[S$THICK_MAX].v0.y)*(S$THICK_V_SCALE -1)/
		S$THICK_V_SCALE;

	CurveNode[S$THICK_MAX].flagNode =  1;

	setColor = 1;
}/*end of function */

void readData(UINT Index)
{
	Index = Index % 5;
	memcpy(CurveNode,curveData[Index].CurveNode,sizeof(CurveNode));
	memcpy(SetPos,curveData[Index].SetPos,sizeof(SetPos));
	DrawMan.weight 	   = 	curveData[Index].weight;
	DrawMan.oldWeight 	=  DrawMan.weight;
	DrawMan.v1 				= 	curveData[Index].v1;
	DrawMan.v2				= 	curveData[Index].v2;
	DrawMan.v03				= 	curveData[Index].v03;
	DrawMan.currentPoint	=	curveData[Index].currentPoint;
	DrawMan.currentNode	=	curveData[Index].currentNode;
	DrawMan.flagLineFocus=	curveData[Index].flagLineFocus;

}/*end of function */

void writeData(UINT Index)
{
	Index = Index % 5;
	memcpy(curveData[Index].CurveNode,CurveNode,sizeof(CurveNode));
	memcpy(curveData[Index].SetPos,SetPos,sizeof(SetPos));
	curveData[Index].weight 			= 	DrawMan.weight;
	curveData[Index].v1 				   = 	DrawMan.v1;
	curveData[Index].v2			      = 	DrawMan.v2;
	curveData[Index].v03				   = 	DrawMan.v03;
	curveData[Index].currentPoint	   =	DrawMan.currentPoint;
	curveData[Index].currentNode	   =	DrawMan.currentNode;
	curveData[Index].flagLineFocus	=	DrawMan.flagLineFocus;

}/*end of function */
void initCurve(unsigned short length)
{
	memset(curveData[0].SetPos,0,sizeof(curveData[0].SetPos));
	memset(curveData[0].CurveNode,0,sizeof(curveData[0].CurveNode));

	for( i = 0; i<= S$THICK_MAX; i++)
		{
		curveData[0].SetPos[i].x = length;
		curveData[0].SetPos[i].y = i;
	}

	curveData[0].weight        =  0;
	curveData[0].v1 	         =  length+1;
	curveData[0].v2		      =  length+1;
	curveData[0].v03	         =  length;
	curveData[0].currentNode   =  S$THICK_MAX;
	curveData[0].currentPoint  =  0;
	curveData[0].flagLineFocus =  0;
	curveData[0].usedIndex     =  0;

	curveData[0].CurveNode[0].former   =  S$THICK_MAX;
	curveData[0].CurveNode[0].next     =  S$THICK_MAX;
	curveData[0].CurveNode[0].v3.x     =  length;
	curveData[0].CurveNode[0].v3.y     =  0;
	curveData[0].CurveNode[0].flagNode  =  1;

	curveData[0].CurveNode[S$THICK_MAX].former 	= 0;
	curveData[0].CurveNode[S$THICK_MAX].next   	= 0;
	curveData[0].CurveNode[S$THICK_MAX].v0.x   	= length;
	curveData[0].CurveNode[S$THICK_MAX].v0.y   	= 0;
	curveData[0].CurveNode[S$THICK_MAX].v3.x   	= length;
	curveData[0].CurveNode[S$THICK_MAX].v3.y   	= S$THICK_MAX;

	curveData[0].CurveNode[S$THICK_MAX].v1.x   	= length+1;
	curveData[0].CurveNode[S$THICK_MAX].v1.y   	= CurveNode[S$THICK_MAX].v0.y +
		(curveData[0].CurveNode[S$THICK_MAX].v3.y - curveData[0].CurveNode[S$THICK_MAX].v0.y)*1/3 ;
		curveData[0].CurveNode[S$THICK_MAX].v2.x   	= length+1;
	curveData[0].CurveNode[S$THICK_MAX].v2.y   	= CurveNode[S$THICK_MAX].v0.y +
		(curveData[0].CurveNode[S$THICK_MAX].v3.y - curveData[0].CurveNode[S$THICK_MAX].v0.y)*2/3;
		curveData[0].CurveNode[S$THICK_MAX].flagNode =  1;


	memcpy(&curveData[1],&curveData[0],sizeof(curveData_typ));
	memcpy(&curveData[2],&curveData[0],sizeof(curveData_typ));
	memcpy(&curveData[3],&curveData[0],sizeof(curveData_typ));
	memcpy(&curveData[4],&curveData[0],sizeof(curveData_typ));
}

/*************************************************************************************/


