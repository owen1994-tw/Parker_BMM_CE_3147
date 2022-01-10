/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: spc_ctrl
 * File: spc_ctrl.c
 * Author: lij
 * Created: July 16, 2015
 ********************************************************************
 * Implementation of program spc_ctrl
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "edge.h"
#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

#define S$MAX_SPC_NO 					 600	   /* Define Maximum Length of Quality Array is 600 */	
#define S$SCREEN_SPC_NO 	    	    22		/* display Length of Quality Array is 22 */	
/**************************************************************************************
** 							         function declare  				  			             **
**************************************************************************************/
void  SPC_Alarm();
void  SPC_Handle(UINT number);

/**************************************************************************************
**     	        		      I N I T U P  O F   T H E   T A S K 		 				       **
**************************************************************************************/

void _INIT spc_ctrlINIT(void)
{
	/* TODO: Add code here */
   strcpy(DataObjOperate.ModName,"spcdata");

   DataObjOperate.InitStep = 100;
   DataObjOperate.Step = 100;

}
/*************************************************************************************
**        				 C Y C L I C   P A R T   O F   T H E   T A S K  				   **  
*************************************************************************************/
void _CYCLIC spc_ctrlCYCLIC(void)
{
	/* TODO: Add code here */
   int idx,i;
   /* --------------------	Step machine -- INIT -------------------- */
   switch(DataObjOperate.InitStep)
   {
      case 0:
         break;
			
      case 100:
         DatObj_Info.enable	= 1;
         DatObj_Info.pName	= (UDINT)DataObjOperate.ModName;
			 
         /* Call  FUB  */
         DatObjInfo(&DatObj_Info);
			
         if (DatObj_Info.status == doERR_MODULNOTFOUND)	/* Data module not found -> create a new one */
         {
            DatObj_Info.enable = 0;
            DataObjOperate.InitStep  = 200;
         }
         else
         {
            DataObjOperate.Ident = DatObj_Info.ident;
            DataObjOperate.InitStep = 600;
         }
         break;
			
      case 200:
         /* Initialize create structure */
         DatObj_Create.enable = 1;
         DatObj_Create.grp	 	= 0;
         DatObj_Create.pName 	= (UDINT)DataObjOperate.ModName;
         DatObj_Create.len 	= sizeof(SPC_Unit_typ)* (S$MAX_SPC_NO + S$SCREEN_SPC_NO)+ sizeof(idxSPC);
         DatObj_Create.MemType  = doUSRROM;
         DatObj_Create.Option   = 0;
         DatObj_Create.pCpyData = 0;
			
         /* Call FUB */
         DatObjCreate(&DatObj_Create);
			
         /* Get FUB output information */
         if( 0 == DatObj_Create.status)
         {
            DatObj_Create.enable = 0; 
            DataObjOperate.Ident = DatObj_Create.ident;
            DataObjOperate.InitStep = 300;
         }	 	 
         break;
      
      case 300:
         status_tmpalloc = TMP_alloc(sizeof(SPC_Unit_typ)* (S$MAX_SPC_NO + S$SCREEN_SPC_NO) + sizeof(idxSPC), (void*)&pData);
         if(0 == status_tmpalloc)
         {
            memset((void*)pData,0,sizeof(SPC_Unit_typ)* (S$MAX_SPC_NO + S$SCREEN_SPC_NO) + sizeof(idxSPC));
            DataObjOperate.InitStep = 400;
         }
         else
         {
            DataObjOperate.InitStep = 9999;
         }  
         break;
					
      case 400:
         DatObj_Write.enable  = 1;
         DatObj_Write.ident 	= DataObjOperate.Ident;
         DatObj_Write.Offset  = 0;
         DatObj_Write.pSource = (UDINT)pData;
         DatObj_Write.len 	= sizeof(SPC_Unit_typ)* (S$MAX_SPC_NO + S$SCREEN_SPC_NO) + sizeof(idxSPC);
			
         /* Call FUB */
         DatObjWrite(&DatObj_Write);
			
         if(0 == DatObj_Write.status)
         {
            DatObj_Write.enable = 0; 
            DataObjOperate.InitStep = 500;
         }
         break;
			
      case 500:
         status_tmpfree = TMP_free(sizeof(SPC_Unit_typ)* (S$MAX_SPC_NO + S$SCREEN_SPC_NO) + sizeof(idxSPC), (void*)pData); 
         if(0 == status_tmpfree)
         {
            DataObjOperate.InitStep = 600;
         }
         else
         {
            DataObjOperate.InitStep = 9999;
         }
         break;
			
      case 600:
         DatObj_Read.enable = 1;
         DatObj_Read.ident  = DataObjOperate.Ident;
         DatObj_Read.Offset = 0;
         DatObj_Read.pDestination = (UDINT)&idxSPC;
         DatObj_Read.len = sizeof(idxSPC);  
 
         /* Call FUB */
         DatObjRead(&DatObj_Read);
         if(0 == DatObj_Read.status)
			{
				oldCycleNo = gCycleTimes;
				gSPC_Current.CycleNo = gCycleTimes;
				DatObj_Read.enable = 0; 
			
				if(idxSPC >= 0 && idxSPC < 2 * S$MAX_SPC_NO)
				{
					DataObjOperate.InitStep = 3000;
				}
				else
				{
					DataObjOperate.InitStep = 300;  /* init value  */
				}
			}
         else
         {
            DataObjOperate.InitStep = 9999;
         }
         break;
			
      case 3000:
         break;
			
      case 9999:
         DataObjOperate.InitStep = 0;
         break;
   }/* switch(DataObjOperate.InitStep) */

   if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000)
   {
      return;
   }/* if(DataObjOperate.Ident == 0 || DataObjOperate.InitStep != 3000) */
   
   if(PIC_SPC_CTRL == gPanelObj.GetPicture)
   {
      for(i=0;i<sizeof(SPC_Mold_Ctrl_typ);i++)
      {
         if(16384 == FocusCtrlL[i])
         {
            if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB]))
            {
               ((USINT *)(&gSPC_Par.CtrlEnable.MoldL.tCycle))[i] = !((USINT *)(&gSPC_Par.CtrlEnable.MoldL.tCycle))[i];
               gRecipeOperate.OnceSave = 1;
            }
         }
         
         if(16384 == FocusCtrlR[i])
         {
            if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_TAB],KeyMatric[S$KEYMATRIX_TAB]))
            {
               ((USINT *)(&gSPC_Par.CtrlEnable.MoldR.tCycle))[i] = !((USINT *)(&gSPC_Par.CtrlEnable.MoldR.tCycle))[i];
               gRecipeOperate.OnceSave = 1;
            }
         }
      }
   }
   KeyMatric[S$KEYMATRIX_TAB] = gPanelObj.KeyMatric[S$KEYMATRIX_TAB];
      
   /* SPC oil temperature  */
   gSPC_Current.OilTemp = gMachineInfo.OilTemp;
   
   /* cycle change  */
   gSPC_Current.CycleNo = gCycleTimes;
	
   if(oldCycleNo != gSPC_Current.CycleNo)
   {
      bCycleChange = 1;
   }
	
   oldCycleNo = gSPC_Current.CycleNo;
	
   /*  call function  */
   SPC_Alarm();
   
   if(gPanelObj.GetPicture != PIC_SPC_1 && gPanelObj.GetPicture != PIC_SPC_2 &&
      gPanelObj.GetPicture != PIC_SPC_CTRL)
   {
      bCmdRefresh = 1;
   }

   /* ------------- step machine ctrl ------------- */
   switch(DataObjOperate.Step )
   {
      case 100:
         if(1 == bCycleChange)
         {
            bCycleChange = 0;
            DataObjOperate.Step = 1000;
         }/* if(1 == bCycleChange) */
         else
         {
            if(gPanelObj.GetPicture == PIC_SPC_1 || gPanelObj.GetPicture == PIC_SPC_2 ||
               gPanelObj.GetPicture == PIC_SPC_CTRL)
            {
               if(1 == bCmdRefresh)
               { 
                  bCmdRefresh = 0;
                  idxScreen  = idxSPC;
                  DataObjOperate.Step = 2000;
               }
            }
            
            if(gPanelObj.GetPicture == PIC_SPC_1 || gPanelObj.GetPicture == PIC_SPC_2)
            {
               if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F7],KeyMatric[S$KEYMATRIX_F7]))				
					{
						if((idxScreen % S$MAX_SPC_NO) >= S$SCREEN_SPC_NO)idxScreen = idxScreen - S$SCREEN_SPC_NO;
						if(idxScreen > 0 && idxScreen < 2 * S$MAX_SPC_NO)
						{
							DataObjOperate.Step = 2000;
						}
					}
					
               if(1 == EDGEPOS(gPanelObj.KeyMatric[S$KEYMATRIX_F8],KeyMatric[S$KEYMATRIX_F8]))
					{
						if((idxScreen % S$MAX_SPC_NO) < S$MAX_SPC_NO - S$SCREEN_SPC_NO)idxScreen = idxScreen + S$SCREEN_SPC_NO;
						if(idxScreen > 0 && idxScreen < 2 * S$MAX_SPC_NO)
						{
							DataObjOperate.Step = 2000;
						}
					}
            }/* if(gPanelObj.GetPicture == PIC_SPC_1 || gPanelObj.GetPicture == PIC_SPC_2) */
         }/* if(1 == bCycleChange) */
         break;	
		
      case 1000:
         DatObj_Write.enable  = 1;
         DatObj_Write.ident	= DataObjOperate.Ident;
			
         DatObj_Write.Offset  = sizeof(gSPC_Current) * (idxSPC % S$MAX_SPC_NO) + sizeof(idxSPC);
         DatObj_Write.pSource = (UDINT)(&gSPC_Current);
         DatObj_Write.len     = sizeof(gSPC_Current);
				
         DatObjWrite(&DatObj_Write);
         if(0 == DatObj_Write.status)
         {
            DatObj_Write.enable = 0;
			
			if(1 == gSPC_Current.MoldR.bCleanCycle)
			{
				memset(&gSPC_Current.MoldR,0,sizeof(gSPC_Current.MoldR));
			}
			else if(1 == gSPC_Current.MoldL.bCleanCycle)
			{
				memset(&gSPC_Current.MoldL,0,sizeof(gSPC_Current.MoldL));
			}
			
            DataObjOperate.Step = 1100;
         }
         break;
		
		//SPC CleanCycle
//		case 1050:
//			if(1 == gSPC_Current.MoldR.bCleanCycle)
//			{
//				memset(&gSPC_Current.MoldR,0,sizeof(gSPC_Current.MoldR));
//			}
//			else if(1 == gSPC_Current.MoldL.bCleanCycle)
//			{
//				memset(&gSPC_Current.MoldL,0,sizeof(gSPC_Current.MoldL));
//			}
//			DataObjOperate.Step = 1100;
//			break;
		
      case 1100:
         if((idxSPC % S$MAX_SPC_NO) < S$SCREEN_SPC_NO)
         {
            DataObjOperate.Step = 1200;
         }
         else
         {
            DataObjOperate.Step = 1300;
         }
         break;
		
      case 1200:
         DatObj_Write.enable  = 1;
         DatObj_Write.ident	 = DataObjOperate.Ident;
			
         DatObj_Write.Offset  = sizeof(gSPC_Current) * ((idxSPC % S$MAX_SPC_NO) + S$MAX_SPC_NO)+ sizeof(idxSPC);
         DatObj_Write.pSource = (UDINT)(&gSPC_Current);
         DatObj_Write.len     = sizeof(gSPC_Current);
				
         DatObjWrite(&DatObj_Write);
         if(0 == DatObj_Write.status)
         {
            DatObj_Write.enable = 0;
			
			if(1 == gSPC_Current.MoldR.bCleanCycle)
			{
				memset(&gSPC_Current.MoldR,0,sizeof(gSPC_Current.MoldR));
			}
			else if(1 == gSPC_Current.MoldL.bCleanCycle)
			{
				memset(&gSPC_Current.MoldL,0,sizeof(gSPC_Current.MoldL));
			}
			
            DataObjOperate.Step = 1300;
         }
         break;
		
		//SPC clean
//	  case 1250:
//			if(1 == gSPC_Current.MoldR.bCleanCycle)
//			{
//				memset(&gSPC_Current.MoldR,0,sizeof(gSPC_Current.MoldR));
//			}
//			else if(1 == gSPC_Current.MoldL.bCleanCycle)
//			{
//				memset(&gSPC_Current.MoldL,0,sizeof(gSPC_Current.MoldL));
//			}
//			DataObjOperate.Step = 1300;
//		 break;
		
      case 1300:
         idxSPC = idxSPC + 1;
         if(idxSPC >= 2 * S$MAX_SPC_NO)idxSPC = S$MAX_SPC_NO;
         DataObjOperate.Step = 1400;
         break;
			
      case 1400:
         DatObj_Write.enable  = 1;
         DatObj_Write.ident	 = DataObjOperate.Ident;
			
         DatObj_Write.Offset  = 0;
         DatObj_Write.pSource = (UDINT)(&idxSPC);
         DatObj_Write.len     = sizeof(idxSPC);
				
         DatObjWrite(&DatObj_Write);
         if(0 == DatObj_Write.status)
         {
            DatObj_Write.enable = 0;
            DataObjOperate.Step = 1500;
         }
         break;
			
      case 1500:
         idxScreen  = idxSPC;
         DataObjOperate.Step = 2000;					
         break;
			
      case 2000:
         memset(&DataList[0],0,sizeof(DataList));
         if(0 == idxScreen)
         {
            DataObjOperate.Step = 5000;
         }
         else if(idxScreen <= S$SCREEN_SPC_NO)
         {
            DataObjOperate.Step = 2100;
         }
         else if(idxScreen > S$SCREEN_SPC_NO)
         {
            DataObjOperate.Step = 2600;
         }
         break;
		
      case 2100:
         DatObj_Read.enable = 1;
         DatObj_Read.ident  = DataObjOperate.Ident;
			
         DatObj_Read.Offset = sizeof(idxSPC);
         DatObj_Read.pDestination = (UDINT)&DataList;
         DatObj_Read.len = sizeof(gSPC_Current) * idxScreen;
 
         /* Call FUB */
         DatObjRead(&DatObj_Read);
         if(0 == DatObj_Read.status)
         {
            DatObj_Read.enable = 0; 
            DataObjOperate.Step = 2200;
         }
         break;
			
      case 2200:
         for(idx=0;idx<idxScreen;idx++)
         {
            SPC_Dis.Data[idxScreen -1 - idx]= DataList[idx];
         } 
         DataObjOperate.Step = 2300;
         break;
			
      case 2300:
         SPC_Dis.Max = SPC_Dis.Data[0];
         SPC_Dis.Min = SPC_Dis.Data[0];
         memset(&SPC_Dis.Avg,0,sizeof(SPC_Dis.Avg));
         SPC_Handle(idxScreen);
			
         DataObjOperate.Step = 2400;
         break;
			
      case 2400:
         DataObjOperate.Step = 100;
         break;
		
      case 2600:
         DatObj_Read.enable = 1;
         DatObj_Read.ident  = DataObjOperate.Ident;
			
         if(idxScreen < (S$MAX_SPC_NO + S$SCREEN_SPC_NO))
         {
            DatObj_Read.Offset = (idxScreen - S$SCREEN_SPC_NO) * sizeof(gSPC_Current) + sizeof(idxSPC);
         }
         else
         {
            DatObj_Read.Offset = ((idxScreen % S$MAX_SPC_NO) - S$SCREEN_SPC_NO) * sizeof(gSPC_Current)+ sizeof(idxSPC);
         }
         DatObj_Read.pDestination = (UDINT)&DataList;
         DatObj_Read.len = sizeof(gSPC_Current) * S$SCREEN_SPC_NO;
 
         /* Call FUB */
         DatObjRead(&DatObj_Read);
         if(0 == DatObj_Read.status)
         {
            DatObj_Read.enable = 0; 
            DataObjOperate.Step = 2700;
         }
         break;
			
      case 2700:
         for(idx=0;idx<S$SCREEN_SPC_NO;idx++)
         {
            SPC_Dis.Data[S$SCREEN_SPC_NO - 1 - idx]= DataList[idx];
         } 
         DataObjOperate.Step = 2800;
         break;
			
      case 2800:
         SPC_Dis.Max = SPC_Dis.Data[0];
         SPC_Dis.Min = SPC_Dis.Data[0];
         memset(&SPC_Dis.Avg,0,sizeof(SPC_Dis.Avg));
         
         SPC_Handle(S$SCREEN_SPC_NO);
			
         DataObjOperate.Step = 2900;			
         break;
			
      case 2900:
         DataObjOperate.Step = 100;
         break;
	
      case 5000:
         DataObjOperate.Step = 100; 
         break;
      default:
         break;
   }/* switch(DataObjOperate.Step) */

   oldPicture = gPanelObj.GetPicture;
}

void _EXIT spc_ctrlEXIT(void)
{
	/* TODO: Add code here */
}

void  SPC_Alarm()
{
   int idx;
   
   for(idx=0;idx<sizeof(SPC_Mold_Ctrl_typ);idx++)
   {
      if(1 == ((USINT *)(&gSPC_Par.CtrlEnable.MoldL.tCycle))[idx]) /*   */
      {
         if(((REAL *)(&gSPC_Current.MoldL.tCycle))[idx] > ((REAL *)(&gSPC_Par.UpperLimit.MoldL.tCycle))[idx])
         {
            ((USINT *)(&gAlarm.UpperLSPC.tCycle))[idx] = 1;
         }
        
         if(((REAL *)(&gSPC_Current.MoldL.tCycle))[idx] > ((REAL *)(&gSPC_Par.LowerLimit.MoldL.tCycle))[idx])
         {
            ((USINT *)(&gAlarm.LowerLSPC.tCycle))[idx] = 1;
         }
      }
      else
      {
         ((USINT *)(&gAlarm.UpperLSPC.tCycle))[idx] = 0;
         ((USINT *)(&gAlarm.LowerLSPC.tCycle))[idx] = 0;
      }
      
      if(1 == ((USINT *)(&gSPC_Par.CtrlEnable.MoldR.tCycle))[idx])
      {
         if(((REAL *)(&gSPC_Current.MoldR.tCycle))[idx] > ((REAL *)(&gSPC_Par.UpperLimit.MoldR.tCycle))[idx])
         {
            ((USINT *)(&gAlarm.UpperRSPC.tCycle))[idx] = 1;
         }
        
         if(((REAL *)(&gSPC_Current.MoldR.tCycle))[idx] > ((REAL *)(&gSPC_Par.LowerLimit.MoldR.tCycle))[idx])
         {
            ((USINT *)(&gAlarm.LowerRSPC.tCycle))[idx] = 1;
         }
      }
      else
      {
         ((USINT *)(&gAlarm.UpperRSPC.tCycle))[idx] = 0;
         ((USINT *)(&gAlarm.LowerRSPC.tCycle))[idx] = 0;
      }
   }/* for(idx=0;idx<sizeof(gSPC_Par.CtrlEnable.MoldL);idx++) */ 
}/* void  SPC_Alarm() */

void  SPC_Handle(UINT number)
{
   int idx,idxValue;
   
   for(idx=0;idx<number;idx++)
   {
      if(SPC_Dis.Max.OilTemp < SPC_Dis.Data[idx].OilTemp)SPC_Dis.Max.OilTemp = SPC_Dis.Data[idx].OilTemp;
      if(SPC_Dis.Min.OilTemp > SPC_Dis.Data[idx].OilTemp)SPC_Dis.Min.OilTemp = SPC_Dis.Data[idx].OilTemp;
      SPC_Dis.Avg.OilTemp = SPC_Dis.Avg.OilTemp + SPC_Dis.Data[idx].OilTemp/number;
      
      for(idxValue=0;idxValue<sizeof(SPC_Mold_Ctrl_typ);idxValue++)
      {
         /*  left mold value   */
         if(((REAL *)(&SPC_Dis.Max.MoldL.tCycle))[idxValue] < ((REAL *)(&SPC_Dis.Data[idx].MoldL.tCycle))[idxValue])
         {
            ((REAL *)(&SPC_Dis.Max.MoldL.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Data[idx].MoldL.tCycle))[idxValue];
         }
   
         if(((REAL *)(&SPC_Dis.Min.MoldL.tCycle))[idxValue] > ((REAL *)(&SPC_Dis.Data[idx].MoldL.tCycle))[idxValue])
         {
            ((REAL *)(&SPC_Dis.Min.MoldL.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Data[idx].MoldL.tCycle))[idxValue];
         }
      
         ((REAL *)(&SPC_Dis.Avg.MoldL.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Avg.MoldL.tCycle))[idxValue] +
                                                           ((REAL *)(&SPC_Dis.Data[idx].MoldL.tCycle))[idxValue]/number;
         
         /*  right mold value   */
         if(((REAL *)(&SPC_Dis.Max.MoldR.tCycle))[idxValue] < ((REAL *)(&SPC_Dis.Data[idx].MoldR.tCycle))[idxValue])
         {
            ((REAL *)(&SPC_Dis.Max.MoldR.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Data[idx].MoldR.tCycle))[idxValue];
         }
   
         if(((REAL *)(&SPC_Dis.Min.MoldR.tCycle))[idxValue] > ((REAL *)(&SPC_Dis.Data[idx].MoldR.tCycle))[idxValue])
         {
            ((REAL *)(&SPC_Dis.Min.MoldR.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Data[idx].MoldR.tCycle))[idxValue];
         }
      
         ((REAL *)(&SPC_Dis.Avg.MoldR.tCycle))[idxValue] = ((REAL *)(&SPC_Dis.Avg.MoldR.tCycle))[idxValue] +
                                                           ((REAL *)(&SPC_Dis.Data[idx].MoldR.tCycle))[idxValue]/number;
      }/* for(idxValue=0;idxValue<sizeof(SPC_Mold_Ctrl_typ);idxValue++) */
   }/* for(idx=0;idx<number;idx++) */
}/* void  SPC_Handle(UINT number) */
