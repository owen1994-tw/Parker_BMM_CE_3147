/***********************************************************************************
 * COPYRIGHT -- B&R China 
 ***********************************************************************************
 * Program: recipe
 * File: recipe.c
 * Author: lijianjun
 * Created: March 06, 2013
 ***********************************************************************************
 * Implementation of program recipe --- recipe management -load/save/clear 
 **********************************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"
#include "recipe.h"

/***********************************************************************************
** 					 	         Macro Define								                **
***********************************************************************************/
#define S$STEP_INIT_OK     10000

#define S$STEP_IDLE        100
#define S$STEP_DM_READ     1000
#define S$STEP_DM_WRITE    2000
#define S$STEP_USB_READ    11000
#define S$STEP_USB_WRITE   12000
#define S$STEP_USB_CLOSE   13000

#define S$STEP_ERROR       30000

#define FILE_RECIPE_ALL         0
#define FILE_TYPE_ALL           1
#define FILE_TYPE_MOLD          2
#define FILE_TYPE_PROFILE       3
#define FILE_TYPE_MACHINE_FIX   4


/**********************************************************************************
**                            Function Declarations								      **
**********************************************************************************/
void InputMatric();

void KeyInputFun(unsigned short key,unsigned short current,Text_Input_typ * pInputCtrl);

/***********************************************************************************
**     	        		       I N I T U P  O F   T H E   T A S K 		 				 **
***********************************************************************************/
_INIT void receipe_init ( void )
{
	DataObjOperate.InitStep = 100;
	gRecipeOperate.NameOk   = 0;
	gRecipeOperate.DataOk   = 0;
	gRecipeOperate.CurveOk  = 0;
   
   strcpy(fileRecName, "nameAll.usb");		/* file name */


   strcpy(DataObjOperate.ModName,"recName");
	strcpy(DataObjOperate.Status,"Recipe name not start !");
   
   StepRecipeUSB = 0;
   cntTime = 0;

}/* end of _INIT */

/***********************************************************************************
**        				C Y C L I C   P A R T   O F   T H E   T A S K  			       **
***********************************************************************************/
 _CYCLIC void receipe_cyclic ( void )
{
   /* USB disk is insert or not  */
   gRecipeOperate.usbDiskOk = gUSB_1.DeviceOk | gUSB_2.DeviceOk;
   gRecipeOperate.bHideUSB  = !gRecipeOperate.usbDiskOk;
   
	/*  Åä·½Ãû×Ö¼°Êý¾Ý³õÊ¼»¯  */
	switch(DataObjOperate.InitStep)
	{
		case 100:
			/* Initialize info structure */
			DataObjOperate.Ident = 0;
			DatObj_Info.enable	= 1;
			DatObj_Info.pName	= (UDINT)DataObjOperate.ModName;

			/* Call  FUB  */
			DatObjInfo(&DatObj_Info);

			if(doERR_MODULNOTFOUND == DatObj_Info.status)
			{
				DataObjOperate.InitStep = 200;
			}
			else if(0 == DatObj_Info.status)
			{
				DataObjOperate.Ident = DatObj_Info.ident;
				DataObjOperate.InitStep  = 500;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 200:
			/* Initialize create structure */
			DatObj_Create.enable 	= 1;
			DatObj_Create.grp	 	   = 0;
			DatObj_Create.pName 	   = (UDINT)DataObjOperate.ModName;
			DatObj_Create.len 		= sizeof(RecipeName);
			DatObj_Create.MemType 	= doUSRROM;
			DatObj_Create.Option 	= 0;
			DatObj_Create.pCpyData 	= 0;

			/* Call FUB */
			DatObjCreate(&DatObj_Create);

			if(0 == DatObj_Create.status)
			{
				DataObjOperate.Ident = DatObj_Create.ident;
				DataObjOperate.InitStep = 300;
			}
			break;
		case 300:
			/*   Initialize Name structure info  */
			for (i=0;i<S$NUM_RECIPE;i++)
			{
            itoa((long)(i),(unsigned long)NumName);
				strcpy(RecipeName.recipeName[i],"recipe");
            strcat(RecipeName.recipeName[i],NumName);
				strcpy(RecipeName.recipeTime[i],"----.--.-- --:--");
			}
			DataObjOperate.InitStep = 400;
			break;
		case 400:
			/* Initialize write structure */
			DatObj_Write.enable  = 1;
			DatObj_Write.ident 	= DataObjOperate.Ident;
			DatObj_Write.Offset  = 0;
			DatObj_Write.pSource = (UDINT) &RecipeName;
			DatObj_Write.len 	 = sizeof (RecipeName);

			/* Call FUB */
			DatObjWrite(&DatObj_Write);

			if(0 == DatObj_Write.status)
			{
				DataObjOperate.InitStep = 500;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 500:
			/* Initialize read structure */
			DatObj_Read.enable = 1;
			DatObj_Read.ident  = DataObjOperate.Ident;
			DatObj_Read.Offset = 0;
			DatObj_Read.pDestination = (UDINT) &RecipeName;
			DatObj_Read.len = sizeof (RecipeName);

			/* Call FUB */
			DatObjRead(&DatObj_Read);

			if(0 == DatObj_Read.status)
			{
				DataObjOperate.InitStep = 600;
			}
			else
			{
				DataObjOperate.InitStep = S$STEP_ERROR;
			}
			break;
		case 600:		/*  load recipe name */
			if(strcmp(RecordName,"USB_FILE"))
			{
				gRecipeOperate.Index = RecipeName.recipeIndex;
				strcpy(RecordName,RecipeName.recipeName[gRecipeOperate.Index ]);
				strcpy(RecordTime,RecipeName.recipeTime[gRecipeOperate.Index ]);
			}
			else
			{
				gRecipeOperate.Index = S$NUM_RECIPE;
			}	
			
			ListIndex = gRecipeOperate.Index ;
			strcpy(ListName,RecipeName.recipeName[ListIndex]);
			strcpy(ListTime,RecipeName.recipeTime[ListIndex]);
			DataObjOperate.InitStep = 700;
			break;
		case 700:       /*  load recipe data */
			gRecipeOperate.NameOk = 1;
			strcpy(DataObjOperate.Status,"Recipe name init ok !");
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

	if((0 == DataObjOperate.Ident)||( gPanelObj.GetPicture != PIC_RECIPE)||
	   (DataObjOperate.InitStep != S$STEP_INIT_OK ))
	{
		cntDealy = 10;
		
		RecipeButton.lockFlag     = 0;
		RecipeButton.lockListName = 0;
		RecipeButton.rename   	  = 0;
		RecipeButton.save         = 0;
		RecipeButton.open         = 0;
		RecipeButton.clear        = 0;
		RecipeButton.former       = 0;
		RecipeButton.next         = 0;
		RecipeButtonNeg = RecipeButton;
      
      gPanelObj.SetKeyLevel = 0;
		return;
	}
	else
	{
		if(cntDealy > 0)
		{
			cntDealy--;
			return;
		}
		
		if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F11],OldKeyMatric[S$KEYMATRIX_F11]) && gPanelObj.PasswordLevel >0)
		{
			gImply.InfoRecipe = 1;
			RecipeButton.rename = 1;
		}
		if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F12],OldKeyMatric[S$KEYMATRIX_F12])&& gPanelObj.PasswordLevel >0)
		{
			gImply.InfoRecipe = 1;
			RecipeButton.save = 1;
		}
		if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F13],OldKeyMatric[S$KEYMATRIX_F13]))
		{
			gImply.InfoRecipe = 1;
			RecipeButton.open = 1;
		}
		if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F14],OldKeyMatric[S$KEYMATRIX_F14])&& gPanelObj.PasswordLevel >0)
		{
			gImply.InfoRecipe = 1;
			RecipeButton.clear = 1;
		}

		RecipeButton.cancle = gPanelObj.KeyMatric[S$KEYMATRIX_ESC];
		RecipeButton.enter  = gPanelObj.KeyMatric[S$KEYMATRIX_ENTER];
         
      /* recipe in USB disk file button  */
      if(1 == gRecipeOperate.usbDiskOk)
      {
			if(1 == gRecipeOperate.NameLoad)
			{
				gRecipeOperate.NameLoad = 0;
				strcpy(RecordName,"USB_FILE");
				strcpy(RecordTime,gPanelObj.CurrentTime);
				gRecipeOperate.Index = S$NUM_RECIPE;
			}
			if (gPanelObj.PasswordLevel<4) //ipis0226
			{
				FiletypeUSB=FILE_TYPE_ALL;
			}	
         bHideUSB_Key = 0;
         switch(FiletypeUSB)
         {
            case FILE_TYPE_ALL:
               if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1))
               {
                  if(gUSB_1.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_1.DirSingleDeviceName);
                  }
                  else if(gUSB_2.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_2.DirSingleDeviceName);	
                  }
               }/* if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1)) */
               
               if(StepRecipeUSB != 0)bHideUSB_Key = 1;
               
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],OldKeyMatric[S$KEYMATRIX_F4]))
               {
                 // if(0 == StepRecipeUSB)StepRecipeUSB = 100; 
					if(0 == StepRecipeUSB)StepRecipeUSB = 120;  //ipis0226
               }
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],OldKeyMatric[S$KEYMATRIX_F5]))
               {
                //  if(0 == StepRecipeUSB)StepRecipeUSB = 200;
				if(0 == StepRecipeUSB)StepRecipeUSB = 220;	  //ipis0226
               }
               break;
			
            case FILE_TYPE_MOLD:
               if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1))
               {
                  if(gUSB_1.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_1.DirSingleDeviceName);
                  }
                  else if(gUSB_2.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_2.DirSingleDeviceName);	
                  }
               }/* if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1)) */
               
               bHideUSB_Key = !gRecipeOperate.usbMoldOk;
               
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],OldKeyMatric[S$KEYMATRIX_F4]))
               {
                  if(1 == gRecipeOperate.usbMoldOk)gRecipeOperate.DataUSBSave = 1;
               }
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],OldKeyMatric[S$KEYMATRIX_F5]))
               {
                  if(1 == gRecipeOperate.usbMoldOk)gRecipeOperate.DataUSBLoad = 1;
               }
               break;
					          
            case FILE_TYPE_PROFILE:
               if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1))
               {
                  if(gUSB_1.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_1.DirSingleDeviceName);
                  }
                  else if(gUSB_2.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_2.DirSingleDeviceName);	
                  }
               }/* if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1)) */
               
               bHideUSB_Key = !gRecipeOperate.usbCurveOk;
               
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],OldKeyMatric[S$KEYMATRIX_F4]))
               {
                  if(1 == gRecipeOperate.usbCurveOk)gRecipeOperate.CurveUSBSave = 1;
               }
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],OldKeyMatric[S$KEYMATRIX_F5]))
               {
                  if(1 == gRecipeOperate.usbCurveOk)gRecipeOperate.CurveUSBLoad = 1;
               }
               break; 
					
            case FILE_TYPE_MACHINE_FIX:
               if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1))
               {
                  if(gUSB_1.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_1.DirSingleDeviceName);
                  }
                  else if(gUSB_2.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_2.DirSingleDeviceName);	
                  }
               }/* if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1)) */
               
               bHideUSB_Key = !gRecipeOperate.usbFixOk;
               
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],OldKeyMatric[S$KEYMATRIX_F4]))
               {
                  if(1 == gRecipeOperate.usbFixOk)gRecipeOperate.OnceUSBSave = 1;
               }
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],OldKeyMatric[S$KEYMATRIX_F5]))
               {
                  if(1 == gRecipeOperate.usbFixOk)gRecipeOperate.OnceUSBLoad = 1;
               }
               break; 
            
            case FILE_RECIPE_ALL:
               if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1))
               {
                  if(gUSB_1.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_1.DirAllDeviceName);
                  }
                  else if(gUSB_2.DeviceOk == 1)
                  {
                     strcpy(gRecipeOperate.deviceNameUSB, gUSB_2.DirAllDeviceName);	
                  }
               }/* if((gUSB_1.DeviceOk == 1) || (gUSB_2.DeviceOk == 1)) */
               
               if(StepRecipeUSB != 0)bHideUSB_Key = 1;
               
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F4],OldKeyMatric[S$KEYMATRIX_F4]))
               {
                  if(0 == StepRecipeUSB)StepRecipeUSB = 1100;
               }
               if(EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F5],OldKeyMatric[S$KEYMATRIX_F5]))
                  {
                  if(0 == StepRecipeUSB)StepRecipeUSB = 1200;
               }
               break;
         }/* switch(FiletypeUSB) */
			
         /* step machine to save / load USB pecipe  */
         switch(StepRecipeUSB)
         {
            case 0:
               cntTime = 0; 
               break;
				
            /* save current recipe to USB disk */	
            case 100:
               gRecipeOperate.OnceUSBSave = 1;
               cntTime = 0; 
               StepRecipeUSB = 110;
               break;
					
            case 110:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbFixOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 120;
               }
               break;
					
            case 120:
               gRecipeOperate.DataUSBSave = 1;
               cntTime = 0; 
               StepRecipeUSB = 130;
               break;
					
            case 130:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbMoldOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 140;
               }
               break;
					
            case 140:
               gRecipeOperate.CurveUSBSave = 1;
               cntTime = 0; 
               StepRecipeUSB = 150;
               break;
					
            case 150:
               cntTime = cntTime + 1;
               if(cntTime > 30 && 1 == gRecipeOperate.usbCurveOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 0;
               }
               break;
				
            /* load current recipe from USB disk */	
            case 200:
               gRecipeOperate.OnceUSBLoad = 1;
               cntTime = 0; 
               StepRecipeUSB = 210;
               break;
					
            case 210:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbFixOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 220;
               }
               break;
					
            case 220:
               gRecipeOperate.DataUSBLoad = 1;
               cntTime = 0; 
               StepRecipeUSB = 230;
               break;
					
            case 230:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbMoldOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 240;
               }
               break;
					
            case 240:
               gRecipeOperate.CurveUSBLoad = 1;
               cntTime = 0; 
               StepRecipeUSB = 250;
               break;
					
            case 250:
               cntTime = cntTime + 1;
               if(cntTime > 30 && 1 == gRecipeOperate.usbCurveOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 0;
               }
               break;
            
            /* ------------------------------- */
            /*   save all recipe to USB disk   */
            /* ------------------------------- */
            case 1100:
               gRecipeOperate.OnceUSBSave = 1;
               cntTime = 0; 
               StepRecipeUSB = 1110;
               break;
					
            case 1110:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbFixOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1120;
               }
               break;
					
            case 1120:
               gRecipeOperate.DataUSBSaveAll = 1;
               cntTime = 0; 
               StepRecipeUSB = 1130;
               break;
					
            case 1130:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbMoldOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1140;
               }
               break;
					
            case 1140:
               gRecipeOperate.CurveUSBSaveAll = 1;
               cntTime = 0; 
               StepRecipeUSB = 1150;
               break;
					
            case 1150:
               cntTime = cntTime + 1;
               if(cntTime > 30 && 1 == gRecipeOperate.usbCurveOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1160;
               }
               break;
           
            case 1160:
               DataObjOperate.Step = S$STEP_USB_WRITE;
               cntTime = 0; 
               StepRecipeUSB = 1170;
               break;
            
            case 1170:
               cntTime = cntTime + 1;
               if(cntTime > 20 && S$STEP_IDLE == DataObjOperate.Step)
               {
                  cntTime = 0;
                  StepRecipeUSB = 0;
               }               
               break;
				
            /* --------------------------------- */
            /*    load all recipe from USB disk  */
            /* --------------------------------- */
            case 1200:
               gRecipeOperate.OnceUSBLoad = 1;
               cntTime = 0; 
               StepRecipeUSB = 1210;
               break;
					
            case 1210:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbFixOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1220;
               }
               break;
					
            case 1220:
               gRecipeOperate.DataUSBLoadAll = 1;
               cntTime = 0; 
               StepRecipeUSB = 1230;
               break;
					
            case 1230:
               cntTime = cntTime + 1;
               if(cntTime > 10 && 1 == gRecipeOperate.usbMoldOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1240;
               }
               break;
					
            case 1240:
               gRecipeOperate.CurveUSBLoadAll = 1;
               cntTime = 0; 
               StepRecipeUSB = 1250;
               break;
					
            case 1250:
               cntTime = cntTime + 1;
               if(cntTime > 30 && 1 == gRecipeOperate.usbCurveOk)
               {
                  cntTime = 0;
                  StepRecipeUSB = 1260;
               }
               break;
            
            case 1260:
               DataObjOperate.Step = S$STEP_USB_READ;
               cntTime = 0; 
               StepRecipeUSB = 1270;
               break;
            
            case 1270:
               cntTime = cntTime + 1;
               if(cntTime > 20 && S$STEP_IDLE == DataObjOperate.Step)
               {
                  cntTime = 0;
                  StepRecipeUSB = 0;
               }               
               break;         
         }/* switch(StepRecipeUSB) */
			
      }/* if(1 == gImply.usbDiskOk) */
      else
      {
         bHideUSB_Key = 1;
         cntTime = 0;
         StepRecipeUSB = 0;
      }
	} /* if((0 == DataObjOperate.Ident)||( gPanelObj.GetPicture != PIC_RECIPE)||
	   (DataObjOperate.InitStep != S$STEP_INIT_OK )) */

	if(1 == RecipeButton.lockFlag)
	{
      gPanelObj.SetKeyLevel = 0;
		RenameColor = 0x0016;  /* »Æµ×ºÚ×Ö */
	}
	else
	{
      gPanelObj.SetKeyLevel = 0;
		RenameColor = 0x0007;  	/* »Òµ×ºÚ×Ö */
      
      for(i=0;i<10;i++)
      {
         ABCColor[i]    = 0x0707;
         ABCColor[10+i] = 0x0707;
         ABCColor[20+i] = 0x0707;
         ABCColor[30+i] = 0x0707;
         ABCColor[40+i] = 0x0707;
         ABCColor[50+i] = 0x0707;
         ABCColor[60+i] = 0x0707;
      }

		strcpy(ListName,RecipeName.recipeName[ListIndex]);

		if(0 == RecipeButton.lockFlag)
		{
			RecipeButton.former = gPanelObj.KeyMatric[S$KEYMATRIX_UP];
			RecipeButton.next   = gPanelObj.KeyMatric[S$KEYMATRIX_DOWN];
		}
		else
		{
			RecipeButton.former = 0;
			RecipeButton.next   = 0;
		}
	}

	OldLockFlag = RecipeButton.lockFlag;

	switch(DataObjOperate.Step)
	{
		case S$STEP_IDLE:
			if(0 == RecipeButton.lockFlag)
			{
				if (EDGENEG(RecipeButton.former,RecipeButtonNeg.former))
				{
					if (ListIndex > 0)
					{
						ListIndex--;
					}
					else
					{
						ListIndex = S$NUM_RECIPE - 1;
					}

					strcpy(ListName,RecipeName.recipeName[ListIndex]);
					strcpy(ListTime,RecipeName.recipeTime[ListIndex]);
				}
				if (EDGENEG(RecipeButton.next,RecipeButtonNeg.next))
				{
					if (ListIndex < (S$NUM_RECIPE - 1))
					{
						ListIndex++;
					}
					else
					{
						ListIndex = 0;
					}

					strcpy(ListName,RecipeName.recipeName[ListIndex]);
					strcpy(ListTime,RecipeName.recipeTime[ListIndex]);
				}

				if (1 == RecipeButton.rename)
				{
					RecipeButton.lockFlag = 1;
				}

				if (1 == RecipeButton.save)
				{
					RecipeButton.lockFlag = 2;
				}

				if (1 == RecipeButton.open)
				{
					RecipeButton.lockFlag = 3;
				}

				if (1 == RecipeButton.clear)
				{
					RecipeButton.lockFlag = 4;
				}

				if(1 == RecipeButton.rename)
				{
					RecipeButton.lockListName = 1;
				}
				else
				{
					RecipeButton.lockListName = 0;
				}
			}/* if(0 == RecipeButton.lockFlag) */
			else if(1 == RecipeButton.lockFlag)
			{
				RecipeButton.save   = 0;
				RecipeButton.open   = 0;
				RecipeButton.clear  = 0;
			}
			else if(2 == RecipeButton.lockFlag)
			{
				RecipeButton.rename = 0;
				RecipeButton.open   = 0;
				RecipeButton.clear  = 0;
			}
			else if(3 == RecipeButton.lockFlag)
			{
				RecipeButton.rename = 0;
				RecipeButton.save   = 0;
				RecipeButton.clear  = 0;
			}
			else
			{
				RecipeButton.rename = 0;
				RecipeButton.save   = 0;
				RecipeButton.open  = 0;
			}

			/*  Assert operation   */
			if (EDGENEG(RecipeButton.enter,RecipeButtonNeg.enter))
			{
				switch(RecipeButton.lockFlag)
				{
					case 1:              /*  rename  */
						strcpy(RecipeName.recipeName[ListIndex],ListName/*TextInput.CurrentName*/);
						if(ListIndex == gRecipeOperate.Index)strcpy(RecordName,ListName/*TextInput.CurrentName*/);
						DataObjOperate.Step = S$STEP_DM_WRITE;
						break;
					case 2:              /*  save  */
						/*  set recipe data from current*/
						gRecipeOperate.Index = ListIndex;
				RecipeName.recipeIndex = gRecipeOperate.Index;
						strcpy(RecordTime,gPanelObj.CurrentTime);
                  strcpy(RecordName,RecipeName.recipeName[gRecipeOperate.Index]);
						strcpy(RecipeName.recipeTime[gRecipeOperate.Index],gPanelObj.CurrentTime);
                  gRecipeOperate.CurveRecipeSave = 1;
                  gRecipeOperate.DataRecipeSave  = 1;
                  gRecipeOperate.OnceSave = 1;
                  DataObjOperate.Step = S$STEP_DM_WRITE;
						break;
					case 3:              /*  open  */
						gRecipeOperate.Index = ListIndex;
						RecipeName.recipeIndex = gRecipeOperate.Index;

						strcpy(RecordName,RecipeName.recipeName[gRecipeOperate.Index]);
						strcpy(RecordTime,RecipeName.recipeTime[gRecipeOperate.Index]);

						strcpy(ListName,RecipeName.recipeName[ListIndex]);
						strcpy(ListTime,RecipeName.recipeTime[ListIndex]);
						/*  load recipe data */
                  gRecipeOperate.CurveRecipeLoad = 1;
                  gRecipeOperate.DataRecipeLoad  = 1;
                  gRecipeOperate.OnceLoad = 1;
                  DataObjOperate.Step = S$STEP_DM_READ;
						break;
					case 4:              /*  clear  */
						if(gRecipeOperate.Index != ListIndex)
						{
							strcpy(RecipeName.recipeName[ListIndex],"Recipe");
							strcpy(RecipeName.recipeTime[ListIndex],"----.--.-- --:--");
							DataObjOperate.Step = S$STEP_DM_WRITE;
							/*  set recipe data  0*/
						}
						break;
				}/* switch(RecipeButton.lockFlag) */

				RecipeButton.lockFlag = 0;
				RecipeButton.rename   = 0;
				RecipeButton.save     = 0;
				RecipeButton.open     = 0;
				RecipeButton.clear    = 0;
			}/* (EDGENEG(RecipeButton.enter,RecipeButtonNeg.enter)) */

			/*cancle any operation */
			if(EDGENEG(RecipeButton.cancle,RecipeButtonNeg.cancle))
			{
				if(1 == RecipeButton.lockFlag)
				{
					strcpy(ListName,RecipeName.recipeName[ListIndex]);
					strcpy(ListTime,RecipeName.recipeTime[ListIndex]);
				}
				RecipeButton.lockFlag = 0;
				RecipeButton.rename   = 0;
				RecipeButton.save     = 0;
				RecipeButton.open     = 0;
				RecipeButton.clear    = 0;
			}
			
			if(1 == gRecipeOperate.NameSave)
			{
				gRecipeOperate.NameSave = 0;
				DataObjOperate.Step = S$STEP_DM_WRITE;
			}
			break; /* S$STEP_IDLE:  */
      
		case S$STEP_DM_READ:
			DataObjOperate.Step = S$STEP_DM_READ + 100;
			strcpy(DataObjOperate.Status,"Recipe name read ok !");
			break;
      
		case (S$STEP_DM_READ + 100):
			DataObjOperate.Step = S$STEP_IDLE;
			break;
      
		case S$STEP_DM_WRITE:
			strcpy(DataObjOperate.Status,"");
			DatObj_Write.enable = 1;
			DatObj_Write.ident 	= DataObjOperate.Ident;
			DatObj_Write.Offset = 0;
			DatObj_Write.pSource = (UDINT) &RecipeName;
			DatObj_Write.len 	 = sizeof (RecipeName);

			/* Call FUB */
			DatObjWrite(&DatObj_Write);

			if(0 == DatObj_Write.status)
			{
				DataObjOperate.Step = S$STEP_DM_WRITE + 100;
			}
			else
			{
				DataObjOperate.Step = S$STEP_IDLE;
			}
			break;
      
		case (S$STEP_DM_WRITE + 100):
			DataObjOperate.Step = S$STEP_IDLE;
			if(0 == DatObj_Write.status)
			{
				strcpy(DataObjOperate.Status,"Recipe name write ok !");
			}
			else
			{
				strcpy(DataObjOperate.Status,"Recipe name write false !");
			}
			break;
      
      /* ------------------------------------------------- */
      /* ----------------- save to USB disk -------------- */
      /* ------------------------------------------------- */
      case S$STEP_USB_WRITE:
         FileOpen_0.enable  = 1;
         FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
         FileOpen_0.pFile   = (UDINT)&fileRecName;
         FileOpen_0.mode    = fiREAD_WRITE;
			
         /* Call FUB */
         FileOpen(&FileOpen_0);
 
         if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)/* no file   */
         {
            FileOpen_0.enable  = 0;
            DataObjOperate.Step = S$STEP_USB_WRITE + 100;
         }
         else 
         {
            if(ERR_OK == FileOpen_0.status)			/* file exist */
            {
               FileOpen_0.enable  = 0;
               if(FileOpen_0.filelen == sizeof(RecipeName))
               {
                  DataObjOperate.IdentUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$STEP_USB_WRITE + 200;
               }
               else
               {
                  DataObjOperate.Step = S$STEP_USB_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable  = 0;
               DataObjOperate.Step = S$STEP_ERROR;
            }
         }  
         break;
      
      case (S$STEP_USB_WRITE + 100):
         FileCreate_0.enable  = 1;
         FileCreate_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
         FileCreate_0.pFile   = (UDINT)&fileRecName;
			
         /* Call FUB */
         FileCreate(&FileCreate_0);

         if(ERR_OK == FileCreate_0.status)		/* ok */
         {
            FileCreate_0.enable = 0;
            DataObjOperate.IdentUSB = FileCreate_0.ident;
            DataObjOperate.Step = S$STEP_USB_WRITE + 200;
         }
         else if (FileCreate_0.status != ERR_FUB_BUSY) 
         {
            FileCreate_0.enable= 0;
            DataObjOperate.Step = S$STEP_USB_CLOSE;
         }
        break;
  
      case (S$STEP_USB_WRITE + 200):
         FileWrite_0.enable = 1;
         FileWrite_0.ident  = DataObjOperate.IdentUSB;
         FileWrite_0.offset = 0;
         FileWrite_0.pSrc   = (UDINT)&RecipeName;
         FileWrite_0.len    = sizeof(RecipeName);
			
         /* Call FUB */
         FileWrite(&FileWrite_0);

         if(ERR_OK == FileWrite_0.status)				/* ok */
         {
            FileWrite_0.enable = 0;
            DataObjOperate.Step = S$STEP_USB_CLOSE;
         }
         else if (FileWrite_0.status != ERR_FUB_BUSY) 
         {
            FileWrite_0.enable = 0;
            DataObjOperate.Step = S$STEP_USB_CLOSE;
         }
         break;
      
      /* ------------------------------------------------- */
      /* ----------------- Load from USB disk ------------ */
      /* ------------------------------------------------- */
      case S$STEP_USB_READ:
         FileOpen_0.enable  = 1;
         FileOpen_0.pDevice = (UDINT)&gRecipeOperate.deviceNameUSB;
         FileOpen_0.pFile   = (UDINT)&fileRecName;
         FileOpen_0.mode    = fiREAD_WRITE;
			
         /* Call FUB */
         FileOpen(&FileOpen_0);
 
         if(fiERR_FILE_NOT_FOUND == FileOpen_0.status)	/* no file   */
         {
            FileOpen_0.enable  = 0;
            DataObjOperate.Step = S$STEP_IDLE;
         }
         else 
         {
            if(ERR_OK == FileOpen_0.status)				/* file exist */
            {
               FileOpen_0.enable = 0;
               if(FileOpen_0.filelen == sizeof(RecipeName))
               {
                  DataObjOperate.IdentUSB = FileOpen_0.ident;
                  DataObjOperate.Step = S$STEP_USB_READ + 100;
               }
               else
               {
                  DataObjOperate.Step = S$STEP_USB_CLOSE;	
               }
            }
            else if (FileOpen_0.status != ERR_FUB_BUSY) 
            {
               FileOpen_0.enable = 0;
               DataObjOperate.Step = S$STEP_ERROR;
            }
         }
         break;
      
      case (S$STEP_USB_READ + 100):
         FileRead_0.enable = 1;
         FileRead_0.ident  = DataObjOperate.IdentUSB;
         FileRead_0.offset = 0;
         FileRead_0.pDest  = (UDINT)&RecipeName;
         FileRead_0.len    = sizeof(RecipeName);
			
         /* Call FUB */
         FileRead(&FileRead_0);
         
         if(ERR_OK == FileRead_0.status)	/* ok */
         {
            FileRead_0.enable = 0;
				gRecipeOperate.NameSave = 1;
            DataObjOperate.Step = S$STEP_USB_CLOSE;
         }
         else
         {
            FileRead_0.enable = 0;
            DataObjOperate.Step = S$STEP_USB_CLOSE;
         }
         break;
      /* ------------------------------------------------------- */
      /* ---------------- Close USB file device ---------------- */
      /* ------------------------------------------------------- */
      case S$STEP_USB_CLOSE:
         FileClose_0.enable = 1;
         FileClose_0.ident  = DataObjOperate.IdentUSB;
			
         /* Call FBK */
         FileClose(&FileClose_0);
            
         if(ERR_OK == FileClose_0.status)	/* ok */
         {
            FileClose_0.enable = 0;
            DataObjOperate.Step =  S$STEP_IDLE;
         }
         else if (FileClose_0.status != ERR_FUB_BUSY) 
         {
            FileClose_0.enable = 0;
            DataObjOperate.Step = S$STEP_ERROR;
         }
         break;

		case S$STEP_ERROR:
			DataObjOperate.Step = S$STEP_IDLE;
			break;
	}/* switch(DataObjOperate.Step) */

	if(0 == RecipeButton.rename && 0 == RecipeButton.save &&
	   0 == RecipeButton.open   && 0 == RecipeButton.clear)
	{
		RecipeButton.lockFlag = 0;
	}
	RecipeButtonNeg = RecipeButton;
}
/**************************************************************************************/
void InputMatric()
{
	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM0])
	{
		CurrentInput ++ ;
		if(CurrentInput < 63)
		{
			CurrentInput = 63;
		}

		if(CurrentInput > 69)
		{
			CurrentInput = 63;
		}

	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM1])
	{
		CurrentInput ++ ;
		if(CurrentInput < 0)
		{
			CurrentInput = 0;
		}

		if(CurrentInput > 6)
		{
			CurrentInput = 0;
		}
	}

	if(1 ==gPanelObj.KeyMatric[S$KEYMATRIX_NUM2])
	{
		CurrentInput ++ ;
		if(CurrentInput < 7)
		{
			CurrentInput = 7;
		}

		if(CurrentInput > 13)
		{
			CurrentInput = 7;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM3])
	{
		CurrentInput ++ ;
		if(CurrentInput < 14)
		{
			CurrentInput = 14;
		}

		if(CurrentInput > 20)
		{
			CurrentInput = 14;
		}
	}
	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM4])
	{
		CurrentInput ++ ;
		if(CurrentInput < 21)
		{
			CurrentInput = 21;
		}

		if(CurrentInput > 27)
		{
			CurrentInput = 21;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM5])
	{
		CurrentInput ++ ;
		if(CurrentInput < 28)
		{
			CurrentInput = 28;
		}

		if(CurrentInput > 34)
		{
			CurrentInput = 28;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM6])
	{
		CurrentInput ++ ;
		if(CurrentInput < 35)
		{
			CurrentInput = 35;
		}

		if(CurrentInput > 41)
		{
			CurrentInput = 35;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM7])
	{
		CurrentInput ++ ;
		if(CurrentInput < 42)
		{
			CurrentInput = 42;
		}

		if(CurrentInput > 48)
		{
			CurrentInput = 42;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM8])
	{
		CurrentInput ++ ;
		if(CurrentInput < 49)
		{
			CurrentInput = 49;
		}

		if(CurrentInput > 55)
		{
			CurrentInput = 49;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_NUM9])
	{
		CurrentInput ++ ;
		if(CurrentInput < 56)
		{
			CurrentInput = 56;
		}

		if(CurrentInput > 62)
		{
			CurrentInput = 56;
		}
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_DOWN])
	{
		CurrentInput = CurrentInput + 7;
		if(CurrentInput > 70)CurrentInput = CurrentInput - 70;
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_UP])
	{
		CurrentInput = CurrentInput - 7;
		if(CurrentInput < 0)CurrentInput = CurrentInput + 70;
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_LEFT])
	{
		CurrentInput = CurrentInput - 1;
		if((CurrentInput % 7) == 0)CurrentInput = CurrentInput + 7;
	}

	if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT])
	{
		CurrentInput = CurrentInput + 1;
		if((CurrentInput % 7) == 0)CurrentInput = CurrentInput - 7;
	}

	if(CurrentInput < 0)CurrentInput= 0;
	if(CurrentInput > 69)CurrentInput =69;
}
void KeyInputFun(unsigned short key,unsigned short current, Text_Input_typ * pInputCtrl)
{

	strcpy(pInputCtrl->SourceChar,"1ABCabc2DEFdef3GHIghi4JKLjhl5MNOmno6PQRpqr7STUstu8VWXvwz9YZyz-.0*/()[]        ");

	if(1 == EDGEPOS(gPanelObj.KeyMatric[key],KeyMatric[key]))
	{
		switch (key)
		{
			case S$KEYMATRIX_TAB:
				if(strlen(pInputCtrl->CurrentName) < 12)
				{
					strcpy(pInputCtrl->Text,"");
					memcpy(pInputCtrl->Text,pInputCtrl->SourceChar+current,1);
					strcat(pInputCtrl->CurrentName,pInputCtrl->Text);
				}
				break;
			case S$KEYMATRIX_DEL:
				if(strlen(pInputCtrl->CurrentName) > 0)
				{
					memset(pInputCtrl->CurrentName + strlen(pInputCtrl->CurrentName)- 1,0,1);
				}
				break;
			case S$KEYMATRIX_ENTER:
				pInputCtrl->Flash = 0;
				break;
			default:
				break;
		}

	}

	if (1 == pInputCtrl->Flash)
    {
 		iFlash++;
 		if(iFlash > 6)iFlash=0;

	 	if (iFlash  < 3)
	 	{
	    	strcpy(pInputCtrl->TempName,pInputCtrl->CurrentName);
	 		strcat(pInputCtrl->TempName, "_");
	 	}
	 	else
	 	{
	    	strcpy(pInputCtrl->TempName,pInputCtrl->CurrentName);
	 		strcat(pInputCtrl->TempName, " ");
	 	}
 	}
 	else
 	{
 		strcpy(pInputCtrl->TempName,pInputCtrl->CurrentName);
 	}

	for(i = strlen(pInputCtrl->TempName);i<14;i++)
	{
		pInputCtrl->TempName[i] = ' ';
	}
}


