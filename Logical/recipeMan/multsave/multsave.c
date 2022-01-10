/*************************************************************************************
 * COPYRIGHT --  B&R China
 *************************************************************************************
 * Program: multsave
 * File: multsave.c
 * Author: lijianjun
 * Created: March 06, 2013
 **************************************************************************************
 * Implementation of program multsave --- mold data save/load 
 *************************************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "recipe.h"

/**************************************************************************************
** 					 	           macro define								                   **
**************************************************************************************/
#define	S$PVNAME_LENGTH		(C_PVNAME_LENGTH +1)
#define	S$LINE_NUMBER		   (C_LINE_NUMBER   +1)

#define S$WAIT_COMMAND			   10

#define S$ROOT_DISK_SAVE		   100
#define S$ROOT_DISK_LOAD		   200
#define S$ROOT_DISK_CLEAR		   500

#define S$USB_SINGLE_OPEN_SAVE	1000
#define S$USB_SINGLE_CREATE		1100
#define S$USB_SINGLE_WRITE_MEM	1200
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
/**************************************************************************************
**                            Function Declarations								     **
**************************************************************************************/
void fun_pv_name ();
void fun_pv_info ();
void fun_pv_save (unsigned short index);
void fun_pv_read (unsigned short index);
void fun_pv_clear(unsigned short index);

/**************************************************************************************
****	        		I N I T U P  O F   T H E   T A S K 		 				   ****
**************************************************************************************/
void _INIT _init(void)
{
	memset(pvList,0,sizeof(pvList));
	strcpy(pv_get.modname,"var_mold");
   
   strcpy(fileSingleName, "mold.usb");		/* file name */
   strcpy(fileAllName, "moldAll.usb");		/* file name */

   strcpy(DataObjOperate.ModName,"recMold");
   strcpy(DataObjOperate.Status,"");
   DataObjOperate.InitStep = 100;
    
   gUserPara.VarNew = 0;
}  /* end of _INIT */


/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				   ****
**************************************************************************************/
void _CYCLIC _cyclic(void)
{
   pMoldUSBData = (UDINT )&tempMoldMem[0];

   /* --------------------	Step machine -- INIT -------------------- */
	switch(DataObjOperate.InitStep)
	{
		case 0:
			break;
      
		case 100:
			fun_pv_name ();
			if (pv_get.errorNum != 0)
			{
				DataObjOperate.InitStep = 40000;
			}
			else
			{
				DataObjOperate.InitStep = 200;
			}
			break;
      
		case 200:
			fun_pv_info ();
			if (pv_get.errorNum != 0)
			{
				DataObjOperate.InitStep = 40001;
			}
			else
			{
				DataObjOperate.InitStep = 300;
			}
			break;
      
		case 300:
         DOInfo.enable	= 1;
         DOInfo.pName	= (UDINT)DataObjOperate.ModName;
         /* Call  FUB  */
         DatObjInfo(&DOInfo);
         if (doERR_MODULNOTFOUND == DOInfo.status)	/* Data module not found -> create a new one */
         {
            DataObjOperate.InitStep  = 400;
         }
         else if (0 == DOInfo.status)
         {
            DataObjOperate.Ident = DOInfo.ident;
            DataObjOperate.InitStep = 800;
         }
         else if(DOInfo.status != ERR_FUB_BUSY)
         { 
            DataObjOperate.InitStep = 40002;
         }
			break;
      
		case 400:
			 /* Initialize create structure */
			 DOCreate.enable 	= 1;
			 DOCreate.grp	 	= 0;
			 DOCreate.pName 	= (UDINT)DataObjOperate.ModName;
	 		 DOCreate.len 		= sizeVarSum * (S$NUM_RECIPE + 1);
			 DOCreate.MemType 	= doUSRROM;
			 DOCreate.Option 	= 0;
			 DOCreate.pCpyData 	= 0;
			 /* Call FUB */
			 DatObjCreate(&DOCreate);
			 /* Get FUB output information */
	 		 wCreate  = DOCreate.status;
	 		 if( 0 == DOCreate.status)
	 		 {
			 	DataObjOperate.Ident = DOCreate.ident;
		 	 	DataObjOperate.InitStep = 500;
		 	 }
			break;
      
		case 500:
			status_tmpalloc = TMP_alloc(sizeVarSum * (S$NUM_RECIPE + 1), (void*)&pData);
			if(0 == status_tmpalloc)
			{
				memset((void*)pData,0,sizeVarSum * (S$NUM_RECIPE + 1));
				DataObjOperate.InitStep = 600;
			}
			else
			{
				DataObjOperate.InitStep = 40003;
			}
			break;
      
		case 600:
			DOWrite.enable  = 1;
			DOWrite.ident 	 = DataObjOperate.Ident;
			DOWrite.Offset  = 0;
			DOWrite.pSource = (UDINT)pData;
			DOWrite.len 	= sizeVarSum * (S$NUM_RECIPE + 1);

			/* Call FUB */
			DatObjWrite(&DOWrite);

			if(0 == DOWrite.status)
			{
				DataObjOperate.InitStep = 700;
			}
			break;
      
		case 700:
			status_tmpfree = TMP_free(sizeVarSum * (S$NUM_RECIPE + 1), (void*)pData);
			if(0 == status_tmpfree)
			{
				DataObjOperate.InitStep = 800;
			}
			else
			{
				DataObjOperate.InitStep = 40004;
			}
			break;
      
		case 800:
			if(1 == gRecipeOperate.NameOk)
			{
				DataObjOperate.InitStep = 900;
			}
			break;
      
      case 900:
         fun_pv_read(S$NUM_RECIPE);
			RMold.Carriage.ValveLinear.update = 1;
			RMold.Carriage.ValveLinear.update = 1;
         gRecipeOperate.DataOk   = 1;
         DataObjOperate.InitStep = 1000;
         break;
			
      case 1000:
         DataObjOperate.Step = S$WAIT_COMMAND;
         DataObjOperate.InitStep = 3000;
         break;
	
      case 3000:
         break;
	}/* switch(DataObjOperate.InitStep) */
   
   if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000)
   {
		gRecipeOperate.DataLoad = 0;
		gRecipeOperate.DataSave = 0;
      gRecipeOperate.DataRecipeLoad = 0;
      gRecipeOperate.DataRecipeSave = 0;
      gRecipeOperate.DataRecipeClear = 0;
      gRecipeOperate.DataUSBLoad = 0;
      gRecipeOperate.DataUSBSave = 0;
      return;
   }/* if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000) */
   
   if(sizeVarSum > C_MEM_MOLD_LENGTH + 1)
   {
      gAlarm.RecMoldLengthErr = 1;
      return;
   }
   else
   {
      gAlarm.RecMoldLengthErr = 0;
      sizeVarSumUSB = C_MEM_MOLD_LENGTH + 1;
   }

   /* recipe save or load command  */
   switch (DataObjOperate.Step)
   {
      case S$WAIT_COMMAND:
         gRecipeOperate.usbMoldOk = 1;
         if(1 == gRecipeOperate.DataSave)
         {
            gRecipeOperate.DataSave = 0;
            indexRecipe = S$NUM_RECIPE;    	/*  current recipe in data module-index = S$NUM_RECIPE */
            DataObjOperate.Step = S$ROOT_DISK_SAVE;
         }
			else if(1 == gRecipeOperate.DataLoad)
			{
				gRecipeOperate.DataLoad = 0;
				indexRecipe = S$NUM_RECIPE;    	/*  current recipe in data module-index = S$NUM_RECIPE */
				DataObjOperate.Step = S$ROOT_DISK_LOAD;
			}
         else if(1 == gRecipeOperate.DataRecipeLoad)
         {
            gRecipeOperate.DataRecipeLoad = 0;
            indexRecipe = gRecipeOperate.Index % S$NUM_RECIPE;
            DataObjOperate.Step = S$ROOT_DISK_LOAD;
            gRecipeOperate.DataSave = 1;	/*  read from recipe then write to current recipe(S$NUM_RECIPE) in data module */
         }/* else if(1 == gRecipeOperate.DataRecipeLoad) */
         else if(1 == gRecipeOperate.DataRecipeSave)
         {
            gRecipeOperate.DataRecipeSave = 0;
            indexRecipe = gRecipeOperate.Index % S$NUM_RECIPE;
            DataObjOperate.Step = S$ROOT_DISK_SAVE;
         }/* else if(1 == gRecipeOperate.DataRecipeSave) */
         else if(1 == gRecipeOperate.DataRecipeClear)
         {
            gRecipeOperate.DataRecipeClear = 0;
            indexRecipe = gRecipeOperate.Index % S$NUM_RECIPE;
            DataObjOperate.Step = S$ROOT_DISK_CLEAR;
         }/* else if(1 == gRecipeOperate.DataLoad) */
         else if(1 == gRecipeOperate.DataUSBLoad)
         {
            gRecipeOperate.usbMoldOk = 0;
            gRecipeOperate.DataUSBLoad = 0;
            DataObjOperate.Step = S$USB_SINGLE_OPEN_LOAD;
            gRecipeOperate.DataSave = 1;	/*  read from USB then write to current recipe(S$NUM_RECIPE) in data module */
         }/* else if(1 == gRecipeOperate.DataUSBLoad) */
         else if(1 == gRecipeOperate.DataUSBSave)
         {
            gRecipeOperate.usbMoldOk = 0;
            gRecipeOperate.DataUSBSave = 0;
            DataObjOperate.Step = S$USB_SINGLE_OPEN_SAVE;
         }/* if(1 == gRecipeOperate.DataUSBSave) */
         else if(1 == gRecipeOperate.DataUSBLoadAll)
         {
            gRecipeOperate.usbMoldOk = 0;
            gRecipeOperate.DataUSBLoadAll = 0;
            DataObjOperate.Step = S$USB_ALL_OPEN_LOAD;
         }
         else if(1 == gRecipeOperate.DataUSBSaveAll)
         {
            gRecipeOperate.usbMoldOk = 0; 
            gRecipeOperate.DataUSBSaveAll = 0;
            DataObjOperate.Step = S$USB_ALL_OPEN_SAVE;
         }
         break;
		
      /* ---------------- Save to data object ---------------- */	
      case S$ROOT_DISK_SAVE:
         fun_pv_save(indexRecipe);
         DataObjOperate.Step = S$WAIT_COMMAND; 
         break;
		
      /* ---------------- Load from data object ---------------- */	
      case S$ROOT_DISK_LOAD:
         fun_pv_read (indexRecipe);
         DataObjOperate.Step = S$WAIT_COMMAND; 
         break;
			
      /* ---------------- clear data object part ---------------- */
      case S$ROOT_DISK_CLEAR:
         fun_pv_clear(indexRecipe);
         DataObjOperate.Step = S$WAIT_COMMAND; 
         break;
		/* --------------------------------------------------------- */	
      /* ---------------- Single Save to usb disk ---------------- */
      /* --------------------------------------------------------- */
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
               if(FileOpen_0.filelen == sizeVarSumUSB)
               {
                  DataObjOperate.IdentUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$USB_SINGLE_WRITE_MEM;
               }
               else
               {
                  gImply.ParaUSB_Length = 1;
                  DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable  = 0;
               gImply.ParaOP_False = 1;
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
            DataObjOperate.Step = S$USB_SINGLE_WRITE_MEM;
         }
         else if (FileCreate_0.status != ERR_FUB_BUSY) 
         {
            FileCreate_0.enable = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
			
      case S$USB_SINGLE_WRITE_MEM:	/* file request single memory */
         if(pMoldUSBData != 0)
         {
            memset((void *)pMoldUSBData,0,sizeVarSumUSB);
            temp_adr_offset = 0;
            for(iLine = 0;iLine < S$LINE_NUMBER;iLine ++)
               {
               if (pv_len[iLine] != 0 && pv_adr[iLine] != 0)
               {
                  memcpy((void *)(pMoldUSBData + temp_adr_offset), (void *)pv_adr[iLine], pv_len[iLine]);
                  temp_adr_offset = temp_adr_offset + pv_len[iLine];
               }
               else break;
            }
            DataObjOperate.Step = S$USB_SINGLE_WRITE;
         }
         else
         {
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }  
         break;
			
      case S$USB_SINGLE_WRITE:		/* file single write */
         FileWrite_0.enable = 1;
         FileWrite_0.ident  = DataObjOperate.IdentUSB;
         FileWrite_0.offset = 0;
         FileWrite_0.pSrc = (UDINT)pMoldUSBData;
         FileWrite_0.len  = sizeVarSumUSB;
			
         /* Call FUB */
         FileWrite(&FileWrite_0);

         if(ERR_OK == FileWrite_0.status)				/* ok */
         {
            FileWrite_0.enable = 0;
            gImply.ParaOP_OK = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         else if (FileWrite_0.status != ERR_FUB_BUSY) 
         {
            FileWrite_0.enable = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
		
      /* ----------------------------------------------------------- */
      /* ---------------- Single Load from USB disk ---------------- */
      /* ----------------------------------------------------------- */
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
            gImply.ParaUSB_NoFile = 1;
            DataObjOperate.Step = S$WAIT_COMMAND;
         }
         else 
         {
            if(ERR_OK == FileOpen_0.status)				/* file exist */
            {
               FileOpen_0.enable = 0;
               if(FileOpen_0.filelen == sizeVarSumUSB)
               {
                  DataObjOperate.IdentUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$USB_SINGLE_READ_MEM;
               }
               else
               {
                  gImply.ParaUSB_Length = 1;
                  DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable = 0;
               gImply.ParaOP_False = 1;
               DataObjOperate.Step = S$ERROR_STEP;
            }
         }
         break;
		
      case S$USB_SINGLE_READ_MEM:			/* file  read request memory */	
         if(pMoldUSBData != 0)
         {
            DataObjOperate.Step = S$USB_SINGLE_READ;
         }
         else
         {
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
			
      case S$USB_SINGLE_READ:			/* file  read  */
         FileRead_0.enable = 1;
         FileRead_0.ident  = DataObjOperate.IdentUSB;
         FileRead_0.offset = 0;
         FileRead_0.pDest  = (UDINT)pMoldUSBData;
         FileRead_0.len    = sizeVarSumUSB;
			
         /* Call FUB */
         FileRead(&FileRead_0);

         if(ERR_OK == FileRead_0.status)	/* ok */
         {
            FileRead_0.enable = 0;
            temp_adr_offset = 0;
            for(iLine = 0;iLine < S$LINE_NUMBER;iLine ++)
               {
               if (pv_len[iLine] != 0 && pv_adr[iLine] != 0)
               {
                  memcpy((void *)pv_adr[iLine],(void *)(pMoldUSBData + temp_adr_offset), pv_len[iLine]);
                  temp_adr_offset = temp_adr_offset + pv_len[iLine];
               }
               else break;
            }
            gImply.ParaOP_OK = 1;
			
				gRecipeOperate.DataSave = 1;
				gRecipeOperate.NameLoad = 1;
				
            /*gRecipeOperate.DataSave = 1;*/
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         else if (FileRead_0.status != ERR_FUB_BUSY) 
         {
            FileRead_0.enable = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
		/* -------------------------------------------------------------- */
      /* ---------------- Single Close USB file device ---------------- */
      /* -------------------------------------------------------------- */
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
            FileOpen_0.enable  = 0;
            DataObjOperate.Step = S$USB_ALL_CREATE;
         }
         else 
         {
            if(ERR_OK == FileOpen_0.status)			/* file exist */
            {
               FileOpen_0.enable  = 0;
               if(FileOpen_0.filelen == sizeVarSum * (S$NUM_RECIPE + 1))
               {
                  DataObjOperate.IdentAllUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$USB_ALL_MEM_ALLOC;
               }
               else
               {
                  gImply.ParaUSB_Length = 1;
                  DataObjOperate.Step = S$USB_ALL_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable  = 0;
               gImply.ParaOP_False = 1;
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
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_CLOSE;
         }
         break;
			
      case S$USB_ALL_MEM_ALLOC:	/* file request All memory */
         status_tmpalloc = TMP_alloc(sizeVarSum * (S$NUM_RECIPE + 1), (void*)&pData);
         if(0 == status_tmpalloc)
         {
            memset((void *)pData,0,sizeVarSum * (S$NUM_RECIPE + 1));
            DataObjOperate.Step = S$USB_ALL_MEM_GET;
         }
         else
         {
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_CLOSE;
         }  
         break;
      
      case S$USB_ALL_MEM_GET:
         DORead.enable = 1;
         DORead.ident  = DataObjOperate.Ident;
         DORead.Offset = 0;
         DORead.pDestination = (UDINT)pData;
         DORead.len = sizeVarSum * (S$NUM_RECIPE + 1);
 
         /* Call FUB */
         DatObjRead(&DORead); 
         if(0 == DORead.status)
         {
            DORead.enable = 0;
            DataObjOperate.Step = S$USB_ALL_WRITE;
         }
         else if (DORead.status != ERR_FUB_BUSY)
         {
            DORead.enable = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_MEM_FREE;
         }
         break;
			
      case S$USB_ALL_WRITE:		/* file single write */
         FileWrite_0.enable = 1;
         FileWrite_0.ident  = DataObjOperate.IdentAllUSB;
         FileWrite_0.offset = 0;
         FileWrite_0.pSrc = (UDINT)pData;
         FileWrite_0.len  = sizeVarSum * (S$NUM_RECIPE + 1);
			
         /* Call FUB */
         FileWrite(&FileWrite_0);

         if(ERR_OK == FileWrite_0.status)				/* ok */
         {
            FileWrite_0.enable = 0;
            gImply.ParaOP_OK = 1;
            DataObjOperate.Step = S$USB_ALL_MEM_FREE;
         }
         else if (FileWrite_0.status != ERR_FUB_BUSY) 
         {
            FileWrite_0.enable = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_MEM_FREE;
         }
         break;
      
      case S$USB_ALL_MEM_FREE:
         status_tmpfree = TMP_free(sizeVarSum * (S$NUM_RECIPE + 1), (void*)pData);
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
            gImply.ParaUSB_NoFile = 1;
            DataObjOperate.Step = S$WAIT_COMMAND;
         }
         else 
         {
            if(ERR_OK == FileOpen_0.status)				/* file exist */
            {
               FileOpen_0.enable = 0;
               if(FileOpen_0.filelen == sizeVarSum * (S$NUM_RECIPE + 1))
               {
                  DataObjOperate.IdentAllUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$USB_ALL_ALLOC_MEM;
               }
               else
               {
                  gImply.ParaUSB_Length = 1;
                  DataObjOperate.Step = S$USB_ALL_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable = 0;
               gImply.ParaOP_False = 1;
               DataObjOperate.Step = S$ERROR_STEP;
            }
         }
         break;
		
      case S$USB_ALL_ALLOC_MEM:			/* file  read request memory */
         status_tmpalloc = TMP_alloc(sizeVarSum * (S$NUM_RECIPE + 1), (void*)&pData);
         if(0 == status_tmpalloc)
         {
            DataObjOperate.Step = S$USB_ALL_READ;
         }
         else
         {
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_CLOSE;
         }
         break;
			
      case S$USB_ALL_READ:			/* file  read  */
         FileRead_0.enable = 1;
         FileRead_0.ident  = DataObjOperate.IdentAllUSB;
         FileRead_0.offset = 0;
         FileRead_0.pDest  = (UDINT)pData;
         FileRead_0.len    = sizeVarSum * (S$NUM_RECIPE + 1);
			
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
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_FREE_MEM;
         }
         break;
      
      case S$USB_ALL_READ_WRITE:
         DOWrite.enable  = 1;
         DOWrite.ident 	 = DataObjOperate.Ident;
         DOWrite.Offset  = 0;
         DOWrite.pSource = (UDINT)pData;
         DOWrite.len 	= sizeVarSum * (S$NUM_RECIPE + 1);
         
         DatObjWrite(&DOWrite);
         
         if(0 == DOWrite.status)
         {
            DOWrite.enable  = 0;
            gImply.ParaOP_OK = 1;
				gRecipeOperate.DataLoad = 1;
            DataObjOperate.Step = S$USB_ALL_FREE_MEM;
         }
         else if (DOWrite.status != ERR_FUB_BUSY)
         {
            DOWrite.enable  = 0;
            gImply.ParaOP_False = 1;
            DataObjOperate.Step = S$USB_ALL_FREE_MEM;
         }
         break;
      
      case S$USB_ALL_FREE_MEM:
         status_tmpfree = TMP_free(sizeVarSum * (S$NUM_RECIPE + 1), (void*)pData);
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
   
}  /* end of _CYCLIC */

void fun_pv_name ()
{
   DOInfo.enable	= 1;
   DOInfo.pName 	= (UDINT)pv_get.modname;
   DatObjInfo(&DOInfo);
 	 
   if (DOInfo.status!= 0)
   {
      pv_get.errorNum = 1000; /*can not find the config data*/
      return;
   }	
	 
   while (iLine < DOInfo.len/S$PVNAME_LENGTH)
   {
      strcpy(pvList[iLine],(char*)(DOInfo.pDatObjMem+iLine*S$PVNAME_LENGTH));
         iLine++;
   }
}/* end of function */

void fun_pv_info ()
{
   sizeVarSum = 0;
   memset(pv_adr,0,sizeof(pv_adr));
   memset(pv_len,0,sizeof(pv_len));
	
   for (iLine = 0;iLine < DOInfo.len/S$PVNAME_LENGTH;iLine++)
   {
      status = PV_xgetadr(pvList[iLine], &pv_adr[iLine], &pv_len[iLine]);
      if (status == 0)
      {
         sizeVarSum = sizeVarSum + pv_len[iLine];
      }
      else if (status == 14700) /*Invalid PV name*/
      {
         if(sizeVarSum < C_MEM_MOLD_LENGTH - 191)
         {
            sizeVarSum = C_MEM_MOLD_LENGTH - 191;
         }
         else
         {
            pv_get.errorNum = 2001; 
         }
         break;
      }
      else if (status != 0)
      {
         pv_get.errorNum = 2000;
         pv_get.line_no  = iLine + 1;
         return;
      }
   }	
}/* end of _function */

void fun_pv_save (unsigned short index)
{
   status = TMP_alloc(sizeVarSum, (void*)&(temp_adr));
	
   if (status != 0) return;
	 
   temp_adr_offset = 0;
	  
   for(iLine = 0;iLine < S$LINE_NUMBER;iLine ++)
      {
      if (pv_len[iLine] != 0 && pv_adr[iLine] != 0)
      {
         memcpy((void *)(temp_adr + temp_adr_offset), (void *)pv_adr[iLine], pv_len[iLine]);
         temp_adr_offset = temp_adr_offset + pv_len[iLine];
      }
      else break;
   }
	 
   /* Initialize write structure */
   DOWrite.enable 	= 1;
   DOWrite.ident 	    = DataObjOperate.Ident;
   DOWrite.Offset 	= sizeVarSum*index;
   DOWrite.pSource    = (UDINT)temp_adr;
   DOWrite.len 	    = sizeVarSum;
   DatObjWrite(&DOWrite);
   wWrite = DOWrite.status;
	
   status = TMP_free(sizeVarSum, (void*)temp_adr);
	
}/* end of _function */

void fun_pv_read (unsigned short index)
{
   status = TMP_alloc(sizeVarSum, (void*)&(temp_adr));
	
   if (status != 0) return;
	 
   temp_adr_offset = 0;

   /* Initialize read structure */
   DORead.enable 		= 1;
   DORead.ident 		= DataObjOperate.Ident;
   DORead.Offset 		= sizeVarSum*index;
   DORead.pDestination	= (UDINT)temp_adr;
   DORead.len 			= sizeVarSum;
 
   /* Call FUB */
   DatObjRead(&DORead);
 
   /* Get status */
   wRead = DORead.status;	
	  
   for(iLine = 0;iLine < S$LINE_NUMBER;iLine ++)
      {
      if (pv_len[iLine] != 0 && pv_adr[iLine] != 0)
      {
         memcpy((void *)pv_adr[iLine],(void *)(temp_adr + temp_adr_offset), pv_len[iLine]);
         temp_adr_offset = temp_adr_offset + pv_len[iLine];
      }
      else break;
   }
	 
   status = TMP_free(sizeVarSum, (void*)temp_adr);
	 
}/* end of _function */

void fun_pv_clear (unsigned short index)
{
   status = TMP_alloc(sizeVarSum, (void*)&(temp_adr));
	
   if (status != 0) return;
	 
   memset((void *)temp_adr,0,sizeVarSum);
	 	 
   /* Initialize write structure */
   DOWrite.enable 	= 1;
   DOWrite.ident 	    = DataObjOperate.Ident;
   DOWrite.Offset 	= sizeVarSum*index;
   DOWrite.pSource    = (UDINT)temp_adr;
   DOWrite.len 	    = sizeVarSum;
   DatObjWrite(&DOWrite);
   wWrite = DOWrite.status;
	
   status = TMP_free(sizeVarSum, (void*)temp_adr);
	
}/* void fun_pv_clear (unsigned short index) */



