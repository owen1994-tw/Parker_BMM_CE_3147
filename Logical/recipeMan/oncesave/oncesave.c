/***************************************************************************************
 * COPYRIGHT --  B&R China
 **************************************************************************************
 * Program: oncesave
 * File: oncesave.c
 * Author: lijianjun
 * Created: March 06, 2013
 **************************************************************************************
 * Implementation of program oncesave --- Fix data load/save 
 *************************************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "recipe.h"

/**************************************************************************************
** 					 	          macro define								                   **
**************************************************************************************/
#define	S$PVNAME_LENGTH		(C_PVNAME_LENGTH + 1)
#define	S$LINE_NUMBER		   (C_LINE_NUMBER   + 1)

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

#define S$ERROR_STEP			      40000
/**************************************************************************************
**                            Function Declarations								          **
**************************************************************************************/
void fun_pv_name ();
void fun_pv_info ();
void fun_pv_save ();
void fun_pv_read ();
void fun_pv_clear();
/**************************************************************************************
****	        		    I N I T U P  O F   T H E   T A S K 		 				        ****
**************************************************************************************/
void _INIT _init(void)
{
   memset(pvList,0,sizeof(pvList));
   strcpy(pv_get.modname,"var_fix");
   strcpy(fileSingleName, "fix.usb");		/* file name */
	
   strcpy(DataObjOperate.ModName,"recFix");
   strcpy(DataObjOperate.Status,"");
   DataObjOperate.InitStep = 100;
    
   gUserFix.VarNew = 0;
}  /* end of _INIT */


/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				           ****
**************************************************************************************/
void _CYCLIC _cyclic(void)
{
   pFixUSBData = (UDINT )&tempOnceMem[0];
   
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
			 if (DOInfo.status == doERR_MODULNOTFOUND)	/* Data module not found -> create a new one */
			 {
			 	 DataObjOperate.InitStep  = 400;
		 	 }
			 else
			 {
				 DataObjOperate.Ident  = DOInfo.ident;
				 DataObjOperate.InitStep = 800;
			 }
			break;
      
		case 400:
			 /* Initialize create structure */
			 DOCreate.enable 	= 1;
			 DOCreate.grp	 	= 0;
			 DOCreate.pName 	= (UDINT)DataObjOperate.ModName;
	 		 DOCreate.len 		= sizeVarSumFix;
			 DOCreate.MemType = doUSRROM;
			 DOCreate.Option 	= 0;
			 DOCreate.pCpyData= 0;
			 /* Call FUB */
			 DatObjCreate(&DOCreate);
			 /* Get FUB output information */
			 wCreate  = DOCreate.status;
			 if( 0 == DOCreate.status)
			 {
			 	DataObjOperate.Ident  = DOCreate.ident;
		 	 	DataObjOperate.InitStep = 500;
		 	 }
			break;
      
		case 500:
			status_tmpalloc = TMP_alloc(sizeVarSumFix, (void*)&pData);
			if(0 == status_tmpalloc)
			{
				memset((void*)pData,0,sizeVarSumFix);
				DataObjOperate.InitStep = 600;
			}
			else
			{
				DataObjOperate.InitStep = 40002;
			}
			break;
      
		case 600:
			DOWrite.enable  = 1;
			DOWrite.ident 	 = DataObjOperate.Ident;
			DOWrite.Offset  = 0;
			DOWrite.pSource = (UDINT)pData;
			DOWrite.len 	= sizeVarSumFix;

			/* Call FUB */
			DatObjWrite(&DOWrite);

			if(0 == DOWrite.status)
			{
				DataObjOperate.InitStep = 700;
			}
			break;
      
		case 700:
			status_tmpfree = TMP_free(sizeVarSumFix, (void*)pData);
			if(0 == status_tmpfree)
			{
				DataObjOperate.InitStep = 800;
			}
			else
			{
				DataObjOperate.InitStep = 40003;
			}
			break;
      
		case 800:
			DataObjOperate.InitStep = 900;
			break;
      
		case 900:
			 fun_pv_read ();
			 gRecipeOperate.OnceOk = 1;
			 DataObjOperate.InitStep = 1000;
			break;
      
		case 1000:
         DataObjOperate.Step = S$WAIT_COMMAND;
         DataObjOperate.InitStep  = 3000;
			break;
      
      case 3000:
         break;
         
		case 9999:
			break;
	}/* switch(DataObjOperate.InitStep) */
   
   if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000)
   {
      gRecipeOperate.OnceUSBSave = 0;
      gRecipeOperate.OnceUSBLoad = 0; 
      return;
   }/* if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000) */
	
   if(sizeVarSumFix > C_MEM_FIX_LENGTH + 1)
   {
      gAlarm.RecFixLengthErr = 1;
      return;
   }
   else
   {
      gAlarm.RecFixLengthErr = 0;
      sizeVarSumUSB = C_MEM_FIX_LENGTH + 1;
   }
	
   /* ----------------- Step machine control ----------------- */
   switch (DataObjOperate.Step)
   {
      case S$WAIT_COMMAND:
         gRecipeOperate.usbFixOk = 1;
         if(1 == gRecipeOperate.OnceSave)
         {
            gRecipeOperate.OnceSave = 0;
            DataObjOperate.Step = S$ROOT_DISK_SAVE;
         }/* else if(1 == gRecipeOperate.OnceSave) */
         else if(1 == gRecipeOperate.OnceLoad)
         {
            gRecipeOperate.OnceLoad = 0;
            DataObjOperate.Step = S$ROOT_DISK_LOAD;
         }/* else if(1 == gImply.OnceLoad) */
         else if(1 == gRecipeOperate.OnceUSBSave)
         {
            gRecipeOperate.usbFixOk = 0;
            gRecipeOperate.OnceUSBSave = 0;
            DataObjOperate.Step = S$USB_SINGLE_OPEN_SAVE;
         }/* else if(1 == gImply.DataUSBSave) */
         else if(1 == gRecipeOperate.OnceUSBLoad)
         {
            gRecipeOperate.usbFixOk = 0;
            gRecipeOperate.OnceUSBLoad = 0;
            DataObjOperate.Step = S$USB_SINGLE_OPEN_LOAD;
				gRecipeOperate.OnceSave = 1;
         }/* else if(1 == gImply.DataUSBLoad) */
         break;
	
      /* ---------------- Save to root disk ---------------- */	
      case S$ROOT_DISK_SAVE:
         fun_pv_save();
         DataObjOperate.Step = S$WAIT_COMMAND; 
         break;
		
      /* ---------------- Load from root disk ---------------- */	
      case S$ROOT_DISK_LOAD:
         fun_pv_read();
         DataObjOperate.Step = S$WAIT_COMMAND; 
         break;
	
      /* -------------------------------------------------- */
      /* ---------------- Save to usb disk ---------------- */
      /* -------------------------------------------------- */
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
                  gImply.FixUSB_Length = 1;
                  DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable  = 0;
               gImply.FixOP_False = 1;
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
            FileCreate_0.enable= 0;
            gImply.FixOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
			
      case S$USB_SINGLE_WRITE_MEM:	/* file write request memory */
         if(pFixUSBData != 0)
         {
            temp_adr_offset = 0;
            for(iLine = 0;iLine < S$LINE_NUMBER;iLine ++)
               {
               if (pv_len[iLine] != 0 && pv_adr[iLine] != 0)
               {
                  memcpy((void *)(pFixUSBData + temp_adr_offset), (void *)pv_adr[iLine], pv_len[iLine]);
                  temp_adr_offset = temp_adr_offset + pv_len[iLine];
               }
               else break;
            }
            DataObjOperate.Step = S$USB_SINGLE_WRITE;
         }
         else
         {
            gImply.FixOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }  
         break;
			
      case S$USB_SINGLE_WRITE:		/* file write */
         FileWrite_0.enable = 1;
         FileWrite_0.ident  = DataObjOperate.IdentUSB;
         FileWrite_0.offset = 0;
         FileWrite_0.pSrc = (UDINT)pFixUSBData;
         FileWrite_0.len  = sizeVarSumUSB;
			
         /* Call FUB */
         FileWrite(&FileWrite_0);

         if(ERR_OK == FileWrite_0.status)				/* ok */
         {
            FileWrite_0.enable = 0;
            gImply.FixOP_OK = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         else if (FileWrite_0.status != ERR_FUB_BUSY) 
         {
            FileWrite_0.enable = 0;
            gImply.FixOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
		
      /* ---------------------------------------------------- */
      /* ---------------- Load from USB disk ---------------- */
      /* ---------------------------------------------------- */
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
               if(FileOpen_0.filelen == sizeVarSumUSB)
               {
                  DataObjOperate.IdentUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$USB_SINGLE_READ_MEM;
               }
               else
               {
                  gImply.FixUSB_Length = 1;
                  DataObjOperate.Step = S$USB_SINGLE_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable = 0;
               gImply.FixOP_False = 1;
               DataObjOperate.Step = S$ERROR_STEP;
            }
         }
         break;
			
      case S$USB_SINGLE_READ_MEM:			/* file  read request memory */	
         if(0 == status_tmpalloc)
         {
            DataObjOperate.Step = S$USB_SINGLE_READ;
         }
         else
         {
            gImply.FixOP_False = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         break;
	
      case S$USB_SINGLE_READ:			/* file single read  */
         FileRead_0.enable = 1;
         FileRead_0.ident  = DataObjOperate.IdentUSB;
         FileRead_0.offset = 0;
         FileRead_0.pDest  = (UDINT)pFixUSBData;
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
                  memcpy((void *)pv_adr[iLine],(void *)(pFixUSBData + temp_adr_offset), pv_len[iLine]);
                  temp_adr_offset = temp_adr_offset + pv_len[iLine];
               }
               else break;
            }
            gImply.FixOP_OK = 1;
            DataObjOperate.Step = S$USB_SINGLE_CLOSE;
         }
         else if (FileRead_0.status != ERR_FUB_BUSY) 
         {
            FileRead_0.enable = 0;
            gImply.FixOP_False = 1;
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
   sizeVarSumFix = 0;
   memset(pv_adr,0,sizeof(pv_adr));
   memset(pv_len,0,sizeof(pv_len));

   for (iLine = 0;iLine < DOInfo.len/S$PVNAME_LENGTH;iLine++)
   {
      status = PV_xgetadr(pvList[iLine], &pv_adr[iLine], &pv_len[iLine]);
      if (status == 0)
      {
         sizeVarSumFix = sizeVarSumFix + pv_len[iLine];
      }
      else if (status == 14700) /*Invalid PV name*/
      {
         if(sizeVarSumFix < C_MEM_FIX_LENGTH - 191)
         {
            sizeVarSumFix = C_MEM_FIX_LENGTH - 191;
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
}/* end of _CYCLIC */

void fun_pv_read ()
{
	 status = TMP_alloc(sizeVarSumFix, (void*)&(temp_adr));

	 if (status != 0) return;

	 temp_adr_offset = 0;

	 /* Initialize read structure */
		DORead.enable 		= 1;
		DORead.ident 		= DataObjOperate.Ident;
		DORead.Offset 		= 0;
		DORead.pDestination	= (UDINT)temp_adr;
		DORead.len 			= sizeVarSumFix;

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

	 status = TMP_free(sizeVarSumFix, (void*)temp_adr);
}

void fun_pv_save ()
{
	 status = TMP_alloc(sizeVarSumFix, (void*)&(temp_adr));

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
	 DOWrite.ident 	= DataObjOperate.Ident;
	 DOWrite.Offset 	= 0;
	 DOWrite.pSource  = (UDINT)temp_adr;
	 DOWrite.len 	   = sizeVarSumFix;
	 DatObjWrite(&DOWrite);
	 wWrite = DOWrite.status;

     status = TMP_free(sizeVarSumFix, (void*)temp_adr);
}/* end of _CYCLIC */

void fun_pv_clear ()
{
   status = TMP_alloc(sizeVarSumFix, (void*)&(temp_adr));
	
   if (status != 0) return;
	 
   memset((void *)temp_adr,0,sizeVarSumFix);
	 	 
   /* Initialize write structure */
   DOWrite.enable  = 1;
   DOWrite.ident 	 = DataObjOperate.Ident;
   DOWrite.Offset  = 0;
   DOWrite.pSource = (UDINT)temp_adr;
   DOWrite.len 	 = sizeVarSumFix;
   DatObjWrite(&DOWrite);
   wWrite = DOWrite.status;
	
   status = TMP_free(sizeVarSumFix, (void*)temp_adr);
}/* void fun_pv_clear (unsigned short index) */





