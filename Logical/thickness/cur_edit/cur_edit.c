/********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: cur_edit
 * File: cur_edit.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cur_edit --- thick profile curve data compiling 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/***********************************************************************************
** 					 	        Include user files								             **
***********************************************************************************/
#include "keyap2100.h"
#include "pic_id.h"
#include "draw.h"
#include "thick.h"
#include "edge.h"               /*edge function   
/**********************************************************************************
**                            Function Declarations								      **
**********************************************************************************/
void nodeAdd(USINT *enable,UINT nodeNum); /*insert a horizontal line*/
void nodeDel(USINT *enable,UINT nodeNum); /*delete a horizontal line*/
void curveScan(SINT *upDown);  			   /*scan the horizontal to get the focus*/
void curveQuickScan(SINT *upDown);  		/*scan the horizontal to get the focus*/
void curveColor(USINT *action);			   /* */
void vxEdit();
void vxEditUnit(INT unitLength);
void moveCross(SINT *upDown);
void weightSet();
void curveToLine(USINT *action,UINT nodeNum);
void curveMove();
void curveClear();
/**********************************************************************************
**     	        	 I N I T U P  O F   T H E   T A S K 		 				 **
**********************************************************************************/
_INIT  void init(void)
{
	RTInfo_v.enable = 1;
	RTInfo(&RTInfo_v);
	TimeLenth = RTInfo_v.cycle_time/4000;
	tTask = RTInfo_v.cycle_time/1000.0/1000.0;

	TimeLenth  = TimeLenth /6 * 6;
	StepLength = TimeLenth;

	focusColor 		= VSCREEN_COLOR_FOCUS_LINE;
	unfocusColor 	= VSCREEN_COLOR_UNFOCUS_LINE;

	addNode 		= 0;
	delNode 		= 0;
	scanNode  	= 0;
	scanLine    = 0;
	setColor		= 0;
	isDownLoad  = 0;

	memset(oldSetPos,0,sizeof(oldSetPos));
	DrawMan.step  = S$STEP_EDIT;
	DrawMan.input = 0;
   
	Counter = 0;
	CurevInitDownload = 1;
   
	moveValue = 0;
	bBackEnable = 0;
	bRedoEnable = 0;
 
	memset(&gThickProf.IsMarker[0],0,sizeof(gThickProf.IsMarker));
	memset(&IsMarkerOld[0],0,sizeof(IsMarkerOld));
	
	MarkPercentOld = MarkPercent = 20;
	
	bStartUpLineFocus = 0;
	bStartDnLineFocus = 0;
	cntUpButtonPress = 0;
	cntDownButtonPress = 0;
}/*  end of init   */
/**********************************************************************************
**            C Y C L I C   P A R T   O F   T H E   T A S K  				     **
**********************************************************************************/
_CYCLIC void cyc(void)
{
	if(1 == gRecipeOperate.OnceOk && 1 == gRecipeOperate.CurveOk)
	{
		if(1 == CurevInitDownload)
		{
			CurevInitDownload = 0;
			DrawMan.downLoad  = 1;
		}
      
		if(1 == GapMaxLimitChange)
		{
			GapMaxLimitChange = 0;
			DrawMan.downLoad  = 1;
		}
	}
	
	/*IsZeroDn Change */
	if(IsZeroDnOld != gThickProf.FunOption.IsZeroDn)
	{
		DrawMan.downLoad = 1;
	}
	
	IsZeroDnOld = gThickProf.FunOption.IsZeroDn;

	/*  mark point --- display  */
	if((PIC_MARKER_SET_A == GetPictureOld) && (gPanelObj.GetPicture != PIC_MARKER_SET_A))
	{
		memset(&gThickProf.IsMarker[0],0,sizeof(gThickProf.IsMarker));
		memset(&IsMarkerOld[0],0,sizeof(IsMarkerOld));
		DrawMan.downLoad = 1;
	}
	GetPictureOld = gPanelObj.GetPicture;
	
	if(MarkPercent < -50 || MarkPercent > 50)MarkPercent = 20;
	if(MarkPercentOld != MarkPercent)
	{
		DrawMan.downLoad = 1;
	}
	MarkPercentOld = MarkPercent;
	
	if (memcmp(gThickProf.IsMarker,IsMarkerOld,sizeof(IsMarkerOld)) != 0)
	{
		memcpy(IsMarkerOld,gThickProf.IsMarker,sizeof(gThickProf.IsMarker));
		DrawMan.downLoad = 1;
	}
   
	if(ServoMinOld != gThickData.ServoMin)
	{
		isDownLoad = 2;
	}
	
	ServoMinOld = gThickData.ServoMin;

	if(ServoMaxOld != gThickData.ServoMax)
	{
		isDownLoad = 2;
	}
	
	ServoMaxOld = gThickData.ServoMax;
   
	if (1 == DrawMan.downLoad)
	{
		for (i = 0; i< 100; i++)
		{
			if(gMachineFix.Parison.eActuatorType == ACTUATOR_ELECTRIC  || 0 == gThickData.IsZeroDn)
			{
				gThickData.SetData[i] = gThickData.ServoMin + gThickData.ServoLength * (SetPos[i*4].x +
					SetPos[i*4+1].x + SetPos[i*4+2].x + SetPos[i*4+3].x)/12000;
				
				if(1 == gThickProf.IsMarker[i])
				{
					if(0 == i)
					{
						gThickData.SetData[i] = gThickData.SetData[i] + gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;

						gThickData.SetData[i+1] = gThickData.SetData[i+1] + gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i+1] < gThickData.ServoMin)gThickData.SetData[i+1] = gThickData.ServoMin;
						if(gThickData.SetData[i+1] > gThickData.ServoMax)gThickData.SetData[i+1] = gThickData.ServoMax;

					}
					else if(99 == i)
					{
						gThickData.SetData[i-1] = gThickData.SetData[i-1] + gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i-1] < gThickData.ServoMin)gThickData.SetData[i-1] = gThickData.ServoMin;
						if(gThickData.SetData[i-1] > gThickData.ServoMax)gThickData.SetData[i-1] = gThickData.ServoMax;

						gThickData.SetData[i] = gThickData.SetData[i] + gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;
					}
					else
					{
						gThickData.SetData[i-1] = gThickData.SetData[i-1] + gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i-1] < gThickData.ServoMin)gThickData.SetData[i-1] = gThickData.ServoMin;
						if(gThickData.SetData[i-1] > gThickData.ServoMax)gThickData.SetData[i-1] = gThickData.ServoMax;

						gThickData.SetData[i] = gThickData.SetData[i] + gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;
						
						gThickData.SetData[i+1] = gThickData.SetData[i+1] + gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i+1] < gThickData.ServoMin)gThickData.SetData[i+1] = gThickData.ServoMin;
						if(gThickData.SetData[i+1] > gThickData.ServoMax)gThickData.SetData[i+1] = gThickData.ServoMax;
					}				
				}
			}
			else if( gMachineFix.Parison.eActuatorType == ACTUATOR_HYD && 1 == gThickData.IsZeroDn)
			{
				gThickData.SetData[i] = gThickData.ServoMax - gThickData.ServoLength * (SetPos[i*4].x +
					SetPos[i*4+1].x + SetPos[i*4+2].x + + SetPos[i*4+3].x)/12000;
				
				if(1 == gThickProf.IsMarker[i])
				{
					if(0 == i)
					{
						gThickData.SetData[i] = gThickData.SetData[i] - gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;

						gThickData.SetData[i+1] = gThickData.SetData[i+1] - gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i+1] < gThickData.ServoMin)gThickData.SetData[i+1] = gThickData.ServoMin;
						if(gThickData.SetData[i+1] > gThickData.ServoMax)gThickData.SetData[i+1] = gThickData.ServoMax;
					}
					else if(99 == i)
					{
						gThickData.SetData[i-1] = gThickData.SetData[i-1] - gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i-1] < gThickData.ServoMin)gThickData.SetData[i-1] = gThickData.ServoMin;
						if(gThickData.SetData[i-1] > gThickData.ServoMax)gThickData.SetData[i-1] = gThickData.ServoMax;

						gThickData.SetData[i] = gThickData.SetData[i] - gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;
					}
					else
					{
						gThickData.SetData[i-1] = gThickData.SetData[i-1] - gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i-1] < gThickData.ServoMin)gThickData.SetData[i-1] = gThickData.ServoMin;
						if(gThickData.SetData[i-1] > gThickData.ServoMax)gThickData.SetData[i-1] = gThickData.ServoMax;

						gThickData.SetData[i] = gThickData.SetData[i] - gThickData.ServoLength * MarkPercent / 100;
						if(gThickData.SetData[i] < gThickData.ServoMin)gThickData.SetData[i] = gThickData.ServoMin;
						if(gThickData.SetData[i] > gThickData.ServoMax)gThickData.SetData[i] = gThickData.ServoMax;
						
						gThickData.SetData[i+1] = gThickData.SetData[i+1] - gThickData.ServoLength * MarkPercent / 200;
						if(gThickData.SetData[i+1] < gThickData.ServoMin)gThickData.SetData[i+1] = gThickData.ServoMin;
						if(gThickData.SetData[i+1] > gThickData.ServoMax)gThickData.SetData[i+1] = gThickData.ServoMax;
					}
				} 
			}
            
			x_Profile[i] = (SetPos[i*4].x + SetPos[i*4+1].x + SetPos[i*4+2].x + SetPos[i*4+3].x)/120.0;
		}
		
		memcpy(oldSetPos,SetPos,sizeof(SetPos));
		
		DrawMan.downLoad  		 = 0;
		gRecipeOperate.CurveSave = 1;
		isDownLoad        		 = 0;

	}/* if (1 == DrawMan.downLoad) */

	if(gPanelObj.GetPicture == PIC_CURVE_EDIT)
	{
		gPanelObj.LedMatric[S$KEYMATRIX_F15] =  0;
		gPanelObj.LedMatric[S$KEYMATRIX_F15] = isDownLoad;
	}
	else if(gPanelObj.GetPicture == PIC_THICK_START)
	{
		gPanelObj.LedMatric[S$KEYMATRIX_F15] = isDownLoad;
		gPanelObj.LedMatric[S$KEYMATRIX_F15] =  0;
	}
	else
	{
		gPanelObj.LedMatric[S$KEYMATRIX_F15] =  0;
		gPanelObj.LedMatric[S$KEYMATRIX_F15] =  0;
	}


	if(gPanelObj.GetPicture != PIC_CURVE_EDIT  && gPanelObj.GetPicture != PIC_THICK_START)
	{
		Counter = 0;
		return;
	}

	Counter ++;
	if(Counter > 1000)Counter = 120;
	if(Counter < 15)return;

	if(0 == gThickProf.FunOption.IsAccu)
	{
		AccuColor  = 0x0060;
		ContiColor = 0x003A;

	}
	else
	{
		AccuColor  = 0x003A;
		ContiColor = 0x0060;
	}

	if((gPanelObj.PasswordLevel >= 1)&&(1 == DrawMan.flagLineFocus)&& (DrawMan.step == S$STEP_EDIT))
	{
		inputenable = 1;
	}
	else
	{
		inputenable = 0;
	}

	bHideBack = ! bBackEnable;

	if(gPanelObj.GetPicture == PIC_CURVE_EDIT)
	{
		if(gPanelObj.PasswordLevel >= 1)
		{
			if(gPanelObj.PasswordLevel >= 3)
			{
			//	changeButton = gPanelObj.KeyMatric[S$KEYMATRIX_F7];
			}
			else
			{
				changeButton = 0;	
			}
//			delButton 		= gPanelObj.KeyMatric[S$KEYMATRIX_F8] | gPanelObj.KeyMatric[S$KEYMATRIX_DEL];
//			curtolButton 	= gPanelObj.KeyMatric[S$KEYMATRIX_F9] | bCurveChangeLine;	
			delButton 		=  gPanelObj.KeyMatric[S$KEYMATRIX_DEL];
			curtolButton 	=  bCurveChangeLine;
			escButton 		= gPanelObj.KeyMatric[S$KEYMATRIX_ESC];
			downloadButton	= gPanelObj.KeyMatric[S$KEYMATRIX_F15];

//			v1Add 			= gPanelObj.KeyMatric[S$KEYMATRIX_F13] | bCurveDnRight;
//			v1Sub 			= gPanelObj.KeyMatric[S$KEYMATRIX_F12] | bCurveDnLeft;
//			v2Add 			= gPanelObj.KeyMatric[S$KEYMATRIX_F11] | bCurveUpRight;
//			v2Sub 			= gPanelObj.KeyMatric[S$KEYMATRIX_F10] | bCurveUpLeft;
				v1Add 			=  bCurveDnRight;
				v1Sub 			=  bCurveDnLeft;
				v2Add 			= bCurveUpRight;
				v2Sub 			=  bCurveUpLeft;
			v03Sub 		   = gPanelObj.KeyMatric[S$KEYMATRIX_LEFT];
			v03Add 		   = gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT];
				
			weightUpButton   = bWeightUp;
			weightDownButton = bWeightDown;
	  	
			if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_UP])
			{
			/*-//ipis0619--------------------------------------------------------------------------------*/
//				cntUpButtonPress = cntUpButtonPress + 1; 
//				if(cntUpButtonPress >= 10000)cntUpButtonPress = 100;
//				
//				if((cntUpButtonPress % 12) < 8)
//				{
//					upButton = 1;	
//				}
//				else
//				{
//					upButton = 0;	
//				}
			/*-//ipis0619--------------------------------------------------------------------------------*/		
			upButton = 1;	 //ipis0619
			}
			else
			{
				cntUpButtonPress = 0;
				upButton = 0;
			}
			
			if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_DOWN])
			{
			/*-//ipis0619--------------------------------------------------------------------------------*/
//				cntDownButtonPress = cntDownButtonPress + 1;	
//				if(cntDownButtonPress >= 10000)cntDownButtonPress = 100;
//				
//				if((cntDownButtonPress % 12) < 8)
//				{
//					downButton = 1;	
//				}
//				else
//				{
//					downButton = 0;	
//				}
			/*-//ipis0619--------------------------------------------------------------------------------*/
			downButton = 1;	//ipis0619	
			}
			else
			{
				cntDownButtonPress = 0;
				downButton = 0;
			}
				
			if(1 == bQuickUp)
			{
			/*-//ipis0619--------------------------------------------------------------------------------*/				
//				cntQuickUpPress = cntQuickUpPress + 1;
//				if(cntQuickUpPress >= 12000)cntQuickUpPress = 120;
//					
//				if((cntQuickUpPress % 12) < 8)
//				{
//					quickUpButton = 1;	
//				}
//				else
//				{
//					quickUpButton = 0;	
//				}
			/*-//ipis0619--------------------------------------------------------------------------------*/	
			quickUpButton = 1;	//ipis0619			
			}
			else
			{
				cntQuickUpPress = 0;
				quickUpButton = 0;
			}
			
			if(1 == bQuickDown)
			{
			/*-//ipis0619--------------------------------------------------------------------------------*/	
//				cntQuickDownPress = cntQuickDownPress + 1;
//				if(cntQuickDownPress >= 12000)cntQuickDownPress = 120;
//						
//				if((cntQuickDownPress % 12) < 8)
//				{
//					quickDownButton = 1;	
//				}
//				else
//				{
//					quickDownButton = 0;	
//				}
			/*-//ipis0619--------------------------------------------------------------------------------*/		
				quickDownButton = 1;
			}
			else
			{
				cntQuickDownPress = 0;
				quickDownButton = 0;
			}
			
			if(1 == bFocusUp)
			{
		/*-//ipis0619--------------------------------------------------------------------------------*/			
//				cntFocusUpPress = cntFocusUpPress + 1;
//				if(cntFocusUpPress >= 7000)cntFocusUpPress = 700;
//						
//				if((cntFocusUpPress % 7) < 4)
//				{
//					focusUpButton = 1;	
//				}
//				else
//				{
//					focusUpButton = 0;	
//				}
		/*-//ipis0619--------------------------------------------------------------------------------*/	
			focusUpButton = 1;		
			}
			else
			{
				cntFocusUpPress = 0;
				focusUpButton = 0;
			}
				
			if(1 == bFocusDown)
			{
		/*-//ipis0619--------------------------------------------------------------------------------*/					
//				cntFocusDownPress = cntFocusDownPress + 1;
//				if(cntFocusDownPress >= 7000)cntFocusDownPress = 700;
//							
//				if((cntFocusDownPress % 7) < 4)
//				{
//					focusDownButton = 1;	
//				}
//				else
//				{
//					focusDownButton = 0;	
//				}
		/*-//ipis0619--------------------------------------------------------------------------------*/	
				focusDownButton = 1;				
			}
			else
			{
				cntFocusDownPress = 0;
				focusDownButton = 0;
			}
			
			tabButton = gPanelObj.KeyMatric[S$KEYMATRIX_TAB];
				
			if(1 == bCurveInit)
			{
				cntPressbuttonTime = cntPressbuttonTime + 1;
			}
			else
			{
				cntPressbuttonTime = 0;
			}
				
			if( cntPressbuttonTime * TimeLenth * 4 > 3000)
			{
				clearButton = 1;
			}
			else
			{
				clearButton = 0;
			}
		}
		else
		{
//			if(1 == gPanelObj.KeyMatric[S$KEYMATRIX_DOWN] ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_UP]   ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_LEFT] ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_RIGHT]||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F7]   ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F8]   ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F9]   ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F10]  ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F11]  ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F12]  ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_F13]  ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_ESC]  ||
//				1 == gPanelObj.KeyMatric[S$KEYMATRIX_DEL])
//				{
//				gImply.PsdLevel1 = 1;
//			}	
//
//			downloadButton	= gPanelObj.KeyMatric[S$KEYMATRIX_F15];
//			cntUpButtonPress   = 0; 
//			cntDownButtonPress = 0;
//			cntQuickUpPress = 0;
//			cntQuickDownPress = 0;
//			cntFocusUpPress = 0;
//			cntFocusDownPress = 0;
		}
	}
	else if(gPanelObj.GetPicture == PIC_THICK_START)
	{
		downloadButton	= gPanelObj.KeyMatric[S$KEYMATRIX_F15];
	}

	R_TRIG_DOWNLOAD.CLK = downloadButton;
	R_TRIG(&R_TRIG_DOWNLOAD);
	if (1 == R_TRIG_DOWNLOAD.Q)
	{
		DrawMan.downLoad = 1;
	}

	if (memcmp(SetPos,oldSetPos,sizeof(SetPos)) != 0)
	{
		memcpy(oldSetPos,SetPos,sizeof(SetPos));
		DrawMan.draw = 1;
		isDownLoad   = 2;
	}

	if (inputok == 0)
	{
		currentGap = SetPos[tempIndex % S$THICK_POINT].x;
		oldcurrentGap = currentGap;
	}

	if(0 == gThickProf.FunOption.Is100_300)
	{
		currentIndex = tempIndex;
	}
	else
	{
		currentIndex = tempIndex/4;
	}

	if(1 == gThickProf.FunOption.Is100_300)
	{
		IndexAddLineMax = 99;
		if(IndexAddLine > 99)IndexAddLine = 99;
	}
	else
	{
		IndexAddLineMax = 399;
		if(IndexAddLine > 399)IndexAddLine = 399;
	}
	
	switch (DrawMan.step)
	{
		case S$STEP_EDIT:
			if(0 == gThickProf.FunOption.bLengthUnit)
			{
				vxEdit();
			}
			else
			{
				if(LengthUnit < 3)LengthUnit = 3;
				if(LengthUnit > 150)LengthUnit = 150;
				vxEditUnit(LengthUnit);
			}

			tempIndex = DrawMan.currentNode;

			if(gPanelObj.PasswordLevel >= 2)  //ipis0619
			{
			moveInputEnable = 1;
			}
			else
			{
			moveInputEnable = 0;
			}

			if(1 == moveInputOk)
			{
				moveInputOk = 0;
				if(moveValue != 0)
				{
					bBackEnable = 1;
					memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
					memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
					memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
					DrawMan.step = S$STEP_CURVEMOVE;
				}
			}

			if(1 == bBackEnable && 1 == bKeyBack)
			{
				bBackEnable = 0;
				DrawMan.step = S$STEP_UNDO;
			}
			
			if(1 == bRedoEnable && 1 == bKeyRedo)
			{
				bRedoEnable = 0;
				DrawMan.step = S$STEP_REDO;
			}

			R_TRIG_UP.CLK = upButton;
			R_TRIG(&R_TRIG_UP);
			R_TRIG_DOWN.CLK = downButton;
			R_TRIG(&R_TRIG_DOWN);
			
			if ( R_TRIG_UP.Q == 1)   scanNode = 1;
			if ( R_TRIG_DOWN.Q == 1) scanNode = -1;
			
			R_TRIG_QUICK_UP.CLK = 	quickUpButton;
			R_TRIG(&R_TRIG_QUICK_UP);
			R_TRIG_QUICK_DOWN.CLK = quickDownButton;
			R_TRIG(&R_TRIG_QUICK_DOWN);
			
			if ( R_TRIG_QUICK_UP.Q == 1)   scanLine = 1;
			if ( R_TRIG_QUICK_DOWN.Q == 1) scanLine = -1;

			if(1 == bAddLineMode)
			{
				bAddLineMode = 0;
				if(1 == gThickProf.FunOption.Is100_300)
				{
					if(0 == IndexAddLine)IndexAddLine = DrawMan.currentNode;
					DrawMan.currentPoint = IndexAddLine * 4;
				}
				else
				{
					if(0 == IndexAddLine)IndexAddLine = DrawMan.currentNode;
					DrawMan.currentPoint = IndexAddLine;
				}
				DrawMan.step  = S$STEP_ADDLINE;
				setColor = 2;
				IndexAddLine = 0;
			}
			
			if(1 == focusUpButton || 1 == focusDownButton)
			{
				DrawMan.currentPoint = DrawMan.currentNode;;
				DrawMan.step = S$STEP_ADDLINE;
				setColor = 2;
			}
			
			if(0 == gPanelObj.vcHandle)
			{
				gPanelObj.vcHandle = VA_Setup(1, gPanelObj.VisualName);
			}

			if (gPanelObj.vcHandle != 0)
			{
				if (!VA_Saccess(1, gPanelObj.vcHandle))
				{
					VA_GetTouchAction (1,gPanelObj.vcHandle,0,(UDINT)(&TouchPoint));
					VA_Srelease(1, gPanelObj.vcHandle);
				}
			}
			
			if(1 == TouchPoint.status) 
			{
				cntPressed = cntPressed + 1;	
		
				if(10 == cntPressed)  
				{
					if(TouchPoint.x > ORIGINAL_X0 + 10 && TouchPoint.x < (ORIGINAL_X0 + 302 - 10) && 
						TouchPoint.y > ORIGINAL_Y0 + 10 && TouchPoint.y < (ORIGINAL_Y0 + 402 - 10))  /* In range */
					{
						if(1 == gThickProf.FunOption.Is100_300)
						{
							DrawMan.currentPoint = 551 - TouchPoint.y;
							if(DrawMan.currentPoint > 399)DrawMan.currentPoint = 399;
						}
						else
						{
							DrawMan.currentPoint = (551 - TouchPoint.y + 1)/4 * 4;
							if(DrawMan.currentPoint > 396)DrawMan.currentPoint = 396;
						}
						DrawMan.step = S$STEP_ADDLINE;
						setColor = 2;
					}
				}
			}
			else
			{
				cntPressed = 0;	
			}

			R_TRIG_DEL.CLK = delButton;
			R_TRIG(&R_TRIG_DEL);
			if(R_TRIG_DEL.Q == 1)
			{
				DrawMan.step = S$STEP_DELLINE;
			}

			R_TRIG_CURTOL.CLK = curtolButton;
			R_TRIG(&R_TRIG_CURTOL);
			if ( R_TRIG_CURTOL.Q ==1) 
			{
				DrawMan.step = S$STEP_CURVETOL;
			}
			
			R_TRIG_Change.CLK = changeButton;
			R_TRIG(&R_TRIG_Change);
			if ( R_TRIG_Change.Q ==1)
			{
				bCurveIndexChgBtn = 1;
			}

			if(bCurveIndexChange == 1)
			{
				bCurveIndexChange = 0;
				bBackEnable = 0;
				bRedoEnable = 0;
				ChangeEnable = 1;
			}
                  
			R_TRIG_CLEAR.CLK = clearButton;
			R_TRIG(&R_TRIG_CLEAR);
			if ( R_TRIG_CLEAR.Q ==1) 
			{
				bBackEnable = 1;
				memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
				memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
				memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
 		
				DrawMan.step = S$STEP_CLAER;
			}
			break;

		case S$STEP_ADDLINE:
				moveInputEnable = 0;

			if (1 == upButton  || 1 == focusUpButton)
			{
			movePoint = 1;
			}
			else if (1 == downButton || 1 == focusDownButton)
			{
			movePoint = -1;
			}
			else
			{
			movePoint = 0;
			}

			/*move the cross cursor*/
			moveCross(&movePoint);
			
			if(1 == bAddLineMode)
			{
				if(1 == gThickProf.FunOption.Is100_300)
				{
					DrawMan.currentPoint = IndexAddLine * 4;
				}
				else
				{
					DrawMan.currentPoint = IndexAddLine;
				}
				setColor = 2;
				bAddLineMode = 0;
				IndexAddLine = 0;
			}
			
			if(0 == gPanelObj.vcHandle)
			{
				gPanelObj.vcHandle = VA_Setup(1, gPanelObj.VisualName);
			}

			if (gPanelObj.vcHandle != 0)
			{
				if (!VA_Saccess(1, gPanelObj.vcHandle))
				{
					VA_GetTouchAction (1,gPanelObj.vcHandle,0,(UDINT)(&TouchPoint));
					VA_Srelease(1, gPanelObj.vcHandle);
				}
			}
			
			if(1 == TouchPoint.status) 
			{
				cntPressed = cntPressed + 1;	
		
				if(10 == cntPressed) 
				{
					if(TouchPoint.x > ORIGINAL_X0 + 10 && TouchPoint.x < (ORIGINAL_X0 + 302 - 10) && 
						TouchPoint.y > ORIGINAL_Y0 + 10 && TouchPoint.y < (ORIGINAL_Y0 + 402 - 10))  /* In range */
					{
						if(1 == gThickProf.FunOption.Is100_300)
						{
							DrawMan.currentPoint = 551 - TouchPoint.y;
							if(DrawMan.currentPoint > 399)DrawMan.currentPoint = 399;
						}
						else
						{
							DrawMan.currentPoint = (551 - TouchPoint.y + 1)/4 * 4;
							if(DrawMan.currentPoint > 396)DrawMan.currentPoint = 396;
						}
					}
					setColor = 2;
				}
			}
			else
			{
				cntPressed = 0;	
			}

			/*add a vertical line*/
			R_TRIG_ENTER.CLK = tabButton; 
			R_TRIG(&R_TRIG_ENTER);

			if ( R_TRIG_ENTER.Q ==1)
			{
				bBackEnable = 1;
				memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
				memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
				memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
			
				addNode  = 1;
				nodeAdd(&addNode,DrawMan.currentPoint);
				DrawMan.currentNode 	= DrawMan.currentPoint;
				DrawMan.step = S$STEP_EDIT;
				DrawMan.flagLineFocus 	= 0;
				scanNode = 1;
			}

			/*esc the cross cursor*/
			F_TRIG_TAB.CLK = escButton;
			F_TRIG(&F_TRIG_TAB);
			if ( F_TRIG_TAB.Q ==1)
			{
				addNode 	  = 0;
				scanNode 	  = 1;
				DrawMan.step = S$STEP_EDIT;
			}

			tempIndex = DrawMan.currentPoint;
			break;

		case S$STEP_DELLINE:
			moveInputEnable = 0;

			if (DrawMan.flagLineFocus == 1 )
			{
				bBackEnable = 1;
				memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
				memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
				memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
				
				delNode = 1;
				nodeDel(&delNode,DrawMan.currentNode);
			}
			DrawMan.step = S$STEP_EDIT;
			break;
		
		case S$STEP_CURVETOL:
			moveInputEnable = 0;

			if(DrawMan.flagLineFocus == 0)
			{
				bBackEnable = 1;
				memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
				memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
				memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
				
				curveToL = 1;
				curveToLine((USINT *)(&curveToL),DrawMan.currentNode);
			}
			DrawMan.step = S$STEP_EDIT;
			break;
		
		case S$STEP_CURVEMOVE:
			moveInputEnable = 0;

			curveMove();
			moveValue = 0;
			DrawMan.step = S$STEP_EDIT;
			break;
		
		case S$STEP_UNDO:
			bRedoEnable = 1;	
			memcpy(&redoDrawMan,&DrawMan,sizeof(DrawMan));
			memcpy(&redoCurveNode,&CurveNode,sizeof(CurveNode));
			memcpy(&redoSetPos,&SetPos,sizeof(SetPos));
			
			bBackEnable = 0;
			memcpy(&DrawMan,&mDrawMan,sizeof(mDrawMan));
			memcpy(&CurveNode,&mCurveNode,sizeof(mCurveNode));
			memcpy(&SetPos,&mSetPos,sizeof(mSetPos));

			DrawMan.draw = 1;
			DrawMan.step = S$STEP_EDIT;
			break;
		
		case S$STEP_REDO:
			bBackEnable = 1;
			memcpy(&mDrawMan,&DrawMan,sizeof(DrawMan));
			memcpy(&mCurveNode,&CurveNode,sizeof(CurveNode));
			memcpy(&mSetPos,&SetPos,sizeof(SetPos));
			
			bRedoEnable = 0;
			memcpy(&DrawMan,&redoDrawMan,sizeof(redoDrawMan));
			memcpy(&CurveNode,&redoCurveNode,sizeof(redoCurveNode));
			memcpy(&SetPos,&redoSetPos,sizeof(redoSetPos));
			
			DrawMan.draw = 1;
			DrawMan.step = S$STEP_EDIT;
			break;
		
		case S$STEP_CLAER:
			curveClear();
			DrawMan.draw = 1;
			DrawMan.step = S$STEP_EDIT;
			break;
	}/* switch (DrawMan.step) */

	if(DrawMan.step != S$STEP_ADDLINE)
	{
		bHideCrossImply = 1;
	}
	else
	{
		bHideCrossImply = 0;
	}

	if(DrawMan.step != S$STEP_EDIT || gPanelObj.PasswordLevel < 1)
	{
		bHideInsert       = 1;
		bHideCurveOperate = 1;
		bHideDel          = 1;
	}
	else
	{
		if(DrawMan.flagLineFocus == 1)
		{
			bHideInsert       = 0;
			bHideCurveOperate = 1;
			if(tempIndex != 0 && tempIndex != S$THICK_MAX)
			{
				bHideDel = 0;
			}
			else
			{
				bHideDel = 1;
			}
		}
		else
		{
			bHideInsert       = 0;
			bHideCurveOperate = 0;
			bHideDel          = 1;
		}
	}

	curveScan(&scanNode);
	curveQuickScan(&scanLine);
	curveColor(&setColor);
	weightSet();

	for(i=0;i<10;i++)
	{
		inputLock[i] = 0;
	}
	if(DrawMan.input > 9)DrawMan.input= 9;

	inputLock[DrawMan.input] = 1;

	cntFlash ++;
	if(cntFlash >= 10000)cntFlash = 0;

	if(0 != isDownLoad)
	{
		if((cntFlash % ((UINT)(1.0/tTask))) < (0.51/tTask))
		{
			loadBmpGrpFlash = 0;
		}
		else
		{
			loadBmpGrpFlash = 1;
		}
	}
	else
	{
		loadBmpGrpFlash = 0;
	}

}/*end of CYCLIC*/
/**************************************************************************************
**        				FUNCTION  REALIZE				   							 **
**************************************************************************************/
void nodeAdd(USINT *enable, UINT nodeNum)
{
	curveNode_typ    *p0,*p1;
	unsigned short   i;
	float			  t;

	if (*enable == 1) *enable = 0;
	else return;

	if (CurveNode[nodeNum].flagNode == 1 || nodeNum > S$THICK_MAX - 3 || nodeNum < 3)
	{
		strcpy(DrawMan.status,"can not add new node");
		return;
	}
	else
	{
		strcpy(DrawMan.status,"the new node is added");
		CurveNode[nodeNum].flagNode = 1;
	}


	for (i = 0 ; i<= S$THICK_MAX; i ++)
	{
		if ( CurveNode[i].next != 0 && CurveNode[i].next > nodeNum )
		{

			p0 = &CurveNode[nodeNum];
			p1 = &CurveNode[CurveNode[i].next];

			t	=(float)(SetPos[nodeNum].y - p1->v0.y)/(float)(p1->v3.y - p1->v0.y);

			p0->former   = i;
			p0->next     = CurveNode[i].next;

			CurveNode[i].next = nodeNum;

			p0->v0.x     = CurveNode[i].v3.x;
			p0->v0.y     = CurveNode[i].v3.y;

			p0->v1.x     = (1-t)*p1->v0.x + t*p1->v1.x;
			p0->v1.y     = (1-t)*p1->v0.y + t*p1->v1.y;
			p0->v2.x     = (1-t)*(1-t)*p1->v0.x + t*t*p1->v2.x + 2*t*(1-t)*p1->v1.x + 1;
			p0->v2.y     = (1-t)*(1-t)*p1->v0.y + t*t*p1->v2.y + 2*t*(1-t)*p1->v1.y;
			p0->v3.x     = SetPos[nodeNum].x;
			p0->v3.y     = SetPos[nodeNum].y;

			p1->v0.x     = p0->v3.x;
			p1->v0.y     = p0->v3.y;
			p1->v1.x     = (1-t)*(1-t)*p1->v1.x + 2*t*(1-t)*p1->v2.x + t*t*p1->v3.x + 1;
			p1->v1.y     = (1-t)*(1-t)*p1->v1.y + 2*t*(1-t)*p1->v2.y + t*t*p1->v3.y;
			p1->v2.x     = (1-t)*p1->v2.x + t*p1->v3.x + 1;
			p1->v2.y     = (1-t)*p1->v2.y + t*p1->v3.y;
			p1->v3.x     = p1->v3.x;
			p1->v3.y     = p1->v3.y;

			p1->former   = nodeNum;
			p0 = 0;
			p1 = 0;
			break;
		}
	}
} /*end of function*/

void nodeDel(USINT *enable,UINT nodeNum)
{
	UINT	oldNodeNum;
	*enable 	= 0;

	if (CurveNode[nodeNum].flagNode == 0 || nodeNum == 0 || nodeNum == S$THICK_MAX)
	{
		strcpy(DrawMan.status,"can not delete the node");
		return;
	}
	else
	{
		strcpy(DrawMan.status,"the node is deleted");
		CurveNode[nodeNum].flagNode = 0;
	}
	oldNodeNum  = CurveNode[nodeNum].next;
	CurveNode[CurveNode[nodeNum].former].next 	= CurveNode[nodeNum].next;
	CurveNode[oldNodeNum].former = CurveNode[nodeNum].former;
	CurveNode[oldNodeNum].v0.x   = CurveNode[nodeNum].v0.x;
	CurveNode[oldNodeNum].v0.y   = CurveNode[nodeNum].v0.y;
	CurveNode[oldNodeNum].v1.x   = CurveNode[nodeNum].v1.x;
	CurveNode[oldNodeNum].v1.y   = CurveNode[oldNodeNum].v0.y +
		(CurveNode[oldNodeNum].v3.y - CurveNode[oldNodeNum].v0.y)*1/
		S$THICK_V_SCALE;
	CurveNode[oldNodeNum].v2.y   = CurveNode[oldNodeNum].v0.y +
		(CurveNode[oldNodeNum].v3.y - CurveNode[oldNodeNum].v0.y)*(S$THICK_V_SCALE - 1)/
		S$THICK_V_SCALE;
	CurveNode[nodeNum].v0.x    = 0;
	CurveNode[nodeNum].v0.y    = 0;
	CurveNode[nodeNum].v1.x    = 0;
	CurveNode[nodeNum].v1.y    = 0;
	CurveNode[nodeNum].v2.x    = 0;
	CurveNode[nodeNum].v2.y    = 0;
	CurveNode[nodeNum].v3.x    = 0;
	CurveNode[nodeNum].v3.y    = 0;
	CurveNode[nodeNum].former  = 0;
	CurveNode[nodeNum].next    = 0;
	DrawMan.currentNode		  	 = oldNodeNum;

	DrawMan.flagLineFocus 	= 1;
	scanNode = -1;

}/*end of function*/

void curveScan(SINT *upDown)
{
	if (*upDown == 1)
	{
		if (DrawMan.flagLineFocus == 1)
		{
			if (DrawMan.currentNode != S$THICK_MAX)
				{
				DrawMan.flagLineFocus = 0;
				DrawMan.currentNode   = CurveNode[DrawMan.currentNode].next;
				DrawMan.v1 = CurveNode[DrawMan.currentNode].v1.x;
				DrawMan.v2 = CurveNode[DrawMan.currentNode].v2.x;
			}
			else
			{
				DrawMan.currentNode = 0;
				DrawMan.v2  = CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03 = CurveNode[DrawMan.currentNode].v3.x;
				DrawMan.v1  = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
			}
		}
		else
		{
			DrawMan.flagLineFocus = 1;
			DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
			DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
			DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
		}

		setColor = 1;
		*upDown = 0;
	}

	if(*upDown == -1)
	{
		if (DrawMan.flagLineFocus == 1)
		{
			if (DrawMan.currentNode != 0)
			{
				DrawMan.flagLineFocus = 0;
				DrawMan.v1 = CurveNode[DrawMan.currentNode].v1.x;
				DrawMan.v2 = CurveNode[DrawMan.currentNode].v2.x;
			}
			else
			{
				DrawMan.currentNode = S$THICK_MAX;
				DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
			}
		}
		else
		{
			DrawMan.flagLineFocus = 1;
			DrawMan.currentNode   = CurveNode[DrawMan.currentNode].former;
			DrawMan.v2	 = CurveNode[DrawMan.currentNode].v2.x;
			DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
			DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
		}
		setColor = 1;
		*upDown = 0;
	}

	strcpy(DrawMan.status,"scan finished");
}/*end of function*/

void curveQuickScan(SINT *upDown)
{
	if (*upDown == 1)
	{
		if (DrawMan.flagLineFocus == 1)
		{
			if (DrawMan.currentNode != S$THICK_MAX)
				{
				bStartUpLineFocus = 1;
				DrawMan.flagLineFocus = 0;
				DrawMan.currentNode   = CurveNode[DrawMan.currentNode].next;
				DrawMan.v1	= CurveNode[DrawMan.currentNode].v1.x;
				DrawMan.v2  = CurveNode[DrawMan.currentNode].v2.x;
			}
			else
			{
				bStartUpLineFocus   = 0;
				DrawMan.currentNode = 0;
				DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
				DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
			}
		}
		else
		{
			bStartUpLineFocus     = 0;
			DrawMan.flagLineFocus = 1;
			DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
			DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
			DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
		}
		
		if(1 == bStartUpLineFocus)
		{
			if (DrawMan.flagLineFocus == 1)
			{
				if (DrawMan.currentNode != S$THICK_MAX)
					{
					DrawMan.flagLineFocus = 0;
					DrawMan.currentNode   = CurveNode[DrawMan.currentNode].next;
					DrawMan.v1	= CurveNode[DrawMan.currentNode].v1.x;
					DrawMan.v2  = CurveNode[DrawMan.currentNode].v2.x;
				}
				else
				{
					DrawMan.currentNode = 0;
					DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
					DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
					DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
				}
			}
			else
			{
				DrawMan.flagLineFocus = 1;
				DrawMan.v2   = CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03  = CurveNode[DrawMan.currentNode].v3.x;
				DrawMan.v1   = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
			}
		}

		setColor = 1;
		*upDown = 0;
	}

	if(*upDown == -1)
	{
		if (DrawMan.flagLineFocus == 1)
		{
			if (DrawMan.currentNode != 0)
			{
				bStartDnLineFocus     = 1;
				DrawMan.flagLineFocus = 0;
				DrawMan.v1 = CurveNode[DrawMan.currentNode].v1.x;
				DrawMan.v2 = CurveNode[DrawMan.currentNode].v2.x;
			}
			else
			{
				bStartDnLineFocus   = 0;
				DrawMan.currentNode = S$THICK_MAX;
				DrawMan.v2  = CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03 = CurveNode[DrawMan.currentNode].v3.x;
			}
		}
		else
		{
			bStartDnLineFocus     = 0;
			DrawMan.flagLineFocus = 1;
			DrawMan.currentNode   = CurveNode[DrawMan.currentNode].former;
			DrawMan.v2	= CurveNode[DrawMan.currentNode].v2.x;
			DrawMan.v03 = CurveNode[DrawMan.currentNode].v3.x;
			DrawMan.v1  = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
		}
		
		if(1 == bStartDnLineFocus)
		{
			if (DrawMan.flagLineFocus == 1)
			{
				if (DrawMan.currentNode != 0)
				{
					DrawMan.flagLineFocus = 0;
					DrawMan.v1 = CurveNode[DrawMan.currentNode].v1.x;
					DrawMan.v2 = CurveNode[DrawMan.currentNode].v2.x;
				}
				else
				{
					DrawMan.currentNode = S$THICK_MAX;
					DrawMan.v2  = CurveNode[DrawMan.currentNode].v2.x;
					DrawMan.v03 = CurveNode[DrawMan.currentNode].v3.x;
				}
			}
			else
			{
				DrawMan.flagLineFocus = 1;
				DrawMan.currentNode   = CurveNode[DrawMan.currentNode].former;
				DrawMan.v2	= CurveNode[DrawMan.currentNode].v2.x;
				DrawMan.v03 = CurveNode[DrawMan.currentNode].v3.x;
				DrawMan.v1  = CurveNode[CurveNode[DrawMan.currentNode].next].v1.x;
			}
		}
		
		setColor = 1;
		*upDown  = 0;
	}

	strcpy(DrawMan.status,"quick scan finished");
}/*end of function*/

void vxEdit()
{
	if (DrawMan.flagLineFocus == 0)
	{
		if ( v1Add == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v1 = DrawMan.v1 + 3;
			else if (counter1 < 4);
			else if (counter1 < 8) DrawMan.v1 = DrawMan.v1 + StepLength/2;
			else DrawMan.v1 = DrawMan.v1 + StepLength;
		}
		else if ( v1Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v1 = DrawMan.v1 - 3;
			else if (counter1 < 4);
			else if (counter1 < 8) DrawMan.v1 =DrawMan.v1 - StepLength/2;
			else DrawMan.v1 =DrawMan.v1  - StepLength;
		}
		else if ( v2Add == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v2 = DrawMan.v2 + 3;
			else if (counter1 < 4);
			else if (counter1 < 8)DrawMan.v2 = DrawMan.v2 + StepLength/2;
			else DrawMan.v2 = DrawMan.v2 + StepLength;
		}
		else if ( v2Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v2 = DrawMan.v2 - 3;
			else if (counter1 < 4);
			else if (counter1 < 8)DrawMan.v2 = DrawMan.v2 - StepLength/2;
			else DrawMan.v2 = DrawMan.v2 - StepLength;
		}
		else
		{
			counter0 = 0;
			counter1 = 0;
		}
	}
	else 
	{
		tempGap = currentGap - oldcurrentGap;

		if (inputok == 1)
		{
			bBackEnable = 1;
			memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
			memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
			memcpy(&mSetPos,&SetPos,sizeof(mSetPos));
			
			DrawMan.v1 = DrawMan.v1 + tempGap;
			DrawMan.v2 = DrawMan.v2 + tempGap;
			DrawMan.v03 = currentGap;
			inputok = 0;
		}

		if ( v03Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1)
			{
				if (DrawMan.v03 > (DrawMan.weight))
				{
					DrawMan.v1  = DrawMan.v1  - 3;
					DrawMan.v2  = DrawMan.v2  - 3;
					DrawMan.v03 = DrawMan.v03 - 3;
				}
			}
			else if (counter1 < 4);
			else if (counter1 < 8)
			{
				if (DrawMan.v03 > (DrawMan.weight))
				{
					DrawMan.v1  = DrawMan.v1  - StepLength/2;
					DrawMan.v2  = DrawMan.v2  - StepLength/2;
					DrawMan.v03 = DrawMan.v03 - StepLength/2;
				}
			}
			else
			{
				if (DrawMan.v03 > (DrawMan.weight))
				{
					DrawMan.v1 = DrawMan.v1  - StepLength;
					DrawMan.v2 = DrawMan.v2  - StepLength;
					DrawMan.v03= DrawMan.v03 - StepLength;
				}
			}
		}
		else if ( v03Add == 1)
		{
			counter0++;
			counter1++;

			if (counter1 == 1)
			{
				if (DrawMan.v03 < 2970)
				{
					DrawMan.v1  = DrawMan.v1  + 3;
					DrawMan.v2  = DrawMan.v2  + 3;
					DrawMan.v03 = DrawMan.v03 + 3;
				}
			}
			else if (counter1 < 4);
			else if (counter1 < 8)
			{
				if (DrawMan.v03 < 2970)
				{
					DrawMan.v1  = DrawMan.v1  + StepLength/2;
					DrawMan.v2  = DrawMan.v2  + StepLength/2;
					DrawMan.v03 = DrawMan.v03 + StepLength/2;
				}
			}
			else
			{
				if (DrawMan.v03 < 2970)
				{
					DrawMan.v1 = DrawMan.v1  + StepLength;
					DrawMan.v2 = DrawMan.v2  + StepLength;
					DrawMan.v03= DrawMan.v03 + StepLength;
				}
			}
		}
		else
		{
			counter0 = 0;
			counter1 = 0;
		}
	}
}/*end of function*/

void vxEditUnit(INT unitLength)
{
	if(unitLength < 0)unitLength = -unitLength;
	if(unitLength < 3)unitLength = 3;
	
	if (DrawMan.flagLineFocus == 0)
	{
		if ( v1Add == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v1 = DrawMan.v1 + 3;
			else if (counter1 < 4);
			else if (counter1 < 8) DrawMan.v1 = DrawMan.v1 + StepLength/2;
			else DrawMan.v1 = DrawMan.v1 + StepLength;
		}
		else if ( v1Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v1 = DrawMan.v1 - 3;
			else if (counter1 < 4);
			else if (counter1 < 8) DrawMan.v1 =DrawMan.v1 - StepLength/2;
			else DrawMan.v1 =DrawMan.v1  - StepLength/2;
		}
		else if ( v2Add == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v2 = DrawMan.v2 + 3;
			else if (counter1 < 4);
			else if (counter1 < 8)DrawMan.v2 = DrawMan.v2 + StepLength/2;
			else DrawMan.v2 = DrawMan.v2 + StepLength;
		}
		else if ( v2Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1) DrawMan.v2 = DrawMan.v2 - 3;
			else if (counter1 < 4);
			else if (counter1 < 8)DrawMan.v2 = DrawMan.v2 - StepLength/2;
			else DrawMan.v2 = DrawMan.v2 - StepLength;
		}
		else
		{
			counter0 = 0;
			counter1 = 0;
		}
	}
	else 
	{
		if(currentGap < DrawMan.weight + 30)currentGap = DrawMan.weight + 30;
		tempGap = ((INT)currentGap) - oldcurrentGap;

		if (inputok == 1)
		{
			bBackEnable = 1;
			memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
			memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
			memcpy(&mSetPos,&SetPos,sizeof(mSetPos));

			DrawMan.v1 = DrawMan.v1 + tempGap;
			DrawMan.v2 = DrawMan.v2 + tempGap;
			DrawMan.v03 = currentGap;
			inputok = 0;
		}

		if ( v03Sub == 1)
		{
			counter0++;
			counter1++;
			if (counter1 == 1)
			{
				if (DrawMan.v03 > (DrawMan.weight + unitLength))
				{
					DrawMan.v1  = DrawMan.v1  - unitLength;
					DrawMan.v2  = DrawMan.v2  - unitLength;
					DrawMan.v03 = DrawMan.v03 - unitLength;
				}
			}
			else if (counter1 < 4);
			else if ((counter1 % 2) == 0)
			{
				if (DrawMan.v03 > (DrawMan.weight + unitLength))
				{
					DrawMan.v1  = DrawMan.v1  - unitLength;
					DrawMan.v2  = DrawMan.v2  - unitLength;
					DrawMan.v03 = DrawMan.v03 - unitLength;
				}
			}
			else;
		}
		else if ( v03Add == 1)
		{
			counter0++;
			counter1++;

			if (counter1 == 1)
			{
				if (DrawMan.v03 < 2970 - unitLength)
				{
					DrawMan.v1  = DrawMan.v1  + unitLength;
					DrawMan.v2  = DrawMan.v2  + unitLength;
					DrawMan.v03 = DrawMan.v03 + unitLength;
				}
			}
			else if (counter1 < 4);
			else if ((counter1 % 2) == 0)
			{
				if (DrawMan.v03 < 2970 - unitLength)
				{
					DrawMan.v1  = DrawMan.v1  + unitLength;
					DrawMan.v2  = DrawMan.v2  + unitLength;
					DrawMan.v03 = DrawMan.v03 + unitLength;
				}
			}
			else;
		}
		else
		{
			counter0 = 0;
			counter1 = 0;
		}
	}
}/*end of function*/

void moveCross(SINT *upDown)
{
	if(0 == gThickProf.FunOption.Is100_300)
	{
		nodeIdxInc = 1;
	}
	else
	{
		nodeIdxInc = 4;
	}

	if (*upDown == 1)
	{
		counter0++;
		counter1++;

		if (counter1 == 1)DrawMan.currentPoint = DrawMan.currentPoint + nodeIdxInc;
		else if (counter1 < 10);
		else
		{
			DrawMan.currentPoint = DrawMan.currentPoint + nodeIdxInc;
		}

		if (DrawMan.currentPoint > S$THICK_MAX) DrawMan.currentPoint = 0;

		if(1 == gThickProf.FunOption.Is100_300)
		{
			DrawMan.currentPoint = DrawMan.currentPoint/4 * 4;
		}

	}
	else if (*upDown == -1)
	{
		counter0++;
		counter1++;

		if (counter1 == 1) DrawMan.currentPoint = DrawMan.currentPoint - nodeIdxInc;
		else if (counter1 < 10);
		else
		{
			DrawMan.currentPoint = DrawMan.currentPoint - nodeIdxInc;
		}

		if (DrawMan.currentPoint < 0) DrawMan.currentPoint = S$THICK_MAX ;

		if(1 == gThickProf.FunOption.Is100_300)
		{
			DrawMan.currentPoint = DrawMan.currentPoint/4 * 4;
		}
	}
	else
	{
		counter0 = 0;
		counter1 = 0;
	}

	if (*upDown != 0) DrawMan.draw = 1;

}/*end of function*/

void curveColor(USINT *action)
{
	if (*action == 0) return;
	if (*action != 0)
	{
		for (i = 0 ; i <= S$THICK_MAX; i ++)
			{
			CurveNode[i].color 		= unfocusColor;
			CurveNode[i].vLineColor = unfocusColor;
		}
	}

	if (*action == 1)
	{
		if (DrawMan.flagLineFocus == 1)
		{
			CurveNode[DrawMan.currentNode].vLineColor = focusColor;
		}
		else
		{
			for(i=CurveNode[DrawMan.currentNode].former;i<=DrawMan.currentNode;i++)
			{
				CurveNode[i].color = focusColor;
			}
		}
	}

	*action 	= 0;
	DrawMan.draw = 1;
}/*end of function*/

void weightSet()
{
	switch (weightStep)
	{
		case 0:
			if(LengthUnit < 3)LengthUnit =3;
			if(LengthUnit > 150)LengthUnit = 150;
			if(1 == weightUpButton)
			{
				cntTime0++;
		
				if(cntTime0 == 1)
				{
					if(0 == gThickProf.FunOption.bLengthUnit)
					{
						DrawMan.weight = DrawMan.weight + 3;
					}
					else
					{
						DrawMan.weight = DrawMan.weight + LengthUnit;
					}
				}
			}
			else if (1 == weightDownButton)
			{
				cntTime0++;
		
				if(cntTime0 == 1)
				{
					if(0 == gThickProf.FunOption.bLengthUnit)
					{
						DrawMan.weight = DrawMan.weight - 3;
					}
					else
					{
						DrawMan.weight = DrawMan.weight - LengthUnit;
					}
				}
			}
			else
			{
				cntTime0 = 0;
			}
			
			if (DrawMan.oldWeight != DrawMan.weight)
			{
				if( DrawMan.oldWeight >=0 && DrawMan.weight >= 0)
				{
					bBackEnable = 1;
					memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
					mDrawMan.weight = mDrawMan.oldWeight;
					memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
					memcpy(&mSetPos,&SetPos,sizeof(mSetPos));

					weightStep = 10;			/* positive vlaue */
				}
				else if( DrawMan.oldWeight >= 0 && DrawMan.weight < 0)
				{
					bBackEnable = 1;
					memcpy(&mDrawMan,&DrawMan,sizeof(mDrawMan));
					mDrawMan.weight = mDrawMan.oldWeight;
					memcpy(&mCurveNode,&CurveNode,sizeof(mCurveNode));
					memcpy(&mSetPos,&SetPos,sizeof(mSetPos));

					weightStep = 110; 		/* negative negative vlaue */
				}
				else
				{
					DrawMan.weight = DrawMan.oldWeight;
					weightStep = 0;
				}
			}
			break;

		/* positive weight  */
		case 10:
			for (i = 0 ; i<= S$THICK_MAX; i ++)
				{
				if ((SetPos[i].x + (DrawMan.weight - DrawMan.oldWeight)) > S$THICK_X_LENGTH ||
						(SetPos[i].x + (DrawMan.weight - DrawMan.oldWeight)) < 3)    //6//ipis0619
					{
					DrawMan.weight = DrawMan.oldWeight;
					weightStep = 0;
					break;
				}
				else if (i == S$THICK_MAX) weightStep = 20;
			}
			break;
		case 20:
			for (i = 0 ; i<= S$THICK_MAX; i ++)
				{
				SetPos[i].x += ((DrawMan.weight - DrawMan.oldWeight));
				if (CurveNode[i].flagNode == 1)
				{
					CurveNode[i].v0.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v1.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v2.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v3.x += ((DrawMan.weight - DrawMan.oldWeight));
				}
			}
			DrawMan.v1 	+= ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.v2 	+= ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.v03 += ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.oldWeight = DrawMan.weight =0; //ipis0619
			weightStep = 0;
			break;

		/* negative weight  */
		case 110:
			for (i = 0 ; i<= S$THICK_MAX; i ++)
				{
				if ((SetPos[i].x + (DrawMan.weight - DrawMan.oldWeight)) > S$THICK_X_LENGTH ||
						(SetPos[i].x + (DrawMan.weight - DrawMan.oldWeight)) < 6)
					{
					DrawMan.weight = DrawMan.oldWeight;
					weightStep = 0;
					break;
				}
				else if (i == S$THICK_MAX) weightStep = 120;
			}
			break;
		case 120:
			for (i = 0 ; i<= S$THICK_MAX; i ++)
				{
				SetPos[i].x += ((DrawMan.weight - DrawMan.oldWeight));
				if (CurveNode[i].flagNode == 1)
				{
					CurveNode[i].v0.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v1.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v2.x += ((DrawMan.weight - DrawMan.oldWeight));
					CurveNode[i].v3.x += ((DrawMan.weight - DrawMan.oldWeight));
				}
			}
			DrawMan.v1 	+= ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.v2 	+= ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.v03 += ((DrawMan.weight - DrawMan.oldWeight));
			DrawMan.oldWeight = DrawMan.weight = 0;
			weightStep = 0;
			break;
	}
}/*end of function*/

void curveToLine(USINT *action,UINT nodeNum)
{
	if (*action == 0) return;
	*action = 0;
	if (DrawMan.flagLineFocus == 0)
	{
		CurveNode[nodeNum].v1.x   	= 1 + CurveNode[nodeNum].v0.x +
			(CurveNode[nodeNum].v3.x - CurveNode[nodeNum].v0.x)*1/
			S$THICK_V_SCALE;
		CurveNode[nodeNum].v2.x   	= 1 + CurveNode[nodeNum].v0.x +
			(CurveNode[nodeNum].v3.x - CurveNode[nodeNum].v0.x)*(S$THICK_V_SCALE - 1)/
			S$THICK_V_SCALE;
	}
	DrawMan.flagLineFocus 	= 1;
	scanNode 				= -1;
}/*end of function*/

void curveMove()
{
	INT idx,idxMove;
	float t;

	memset(tempCurveNode,0,sizeof(tempCurveNode));

	if(moveValue > 20 || moveValue < -20)return;

	idxMove = 3 * moveValue;

	if(idxMove < 0)idxMove=-idxMove;

	if(moveValue > 0)
	{
		/* ------------ first node ------------ */
		tempCurveNode[0].former   = S$THICK_MAX;
		tempCurveNode[0].next     = idxMove;
		tempCurveNode[0].flagNode = 1;
		tempCurveNode[0].v3.x     = SetPos[0].x;

		/* ------------ second node ------------ */
		tempCurveNode[idxMove].former = 0;
		tempCurveNode[idxMove].next = CurveNode[0].next + idxMove;
		if(tempCurveNode[idxMove].next > S$THICK_MAX)tempCurveNode[idxMove].next = S$THICK_MAX;

		tempCurveNode[idxMove].flagNode = 1;

		tempCurveNode[idxMove].v0.x = SetPos[0].x;
		tempCurveNode[idxMove].v0.y = 0;

		tempCurveNode[idxMove].v1.x = SetPos[0].x+1;
		tempCurveNode[idxMove].v1.y = idxMove/3;

		tempCurveNode[idxMove].v2.x = SetPos[0].x+1;
		tempCurveNode[idxMove].v2.y = idxMove - idxMove/3;

		tempCurveNode[idxMove].v3.x = SetPos[0].x;
		tempCurveNode[idxMove].v3.y = idxMove;

		endIdx = 0;
		for (idx = 1; idx<= S$THICK_MAX -1; idx ++)
		{
			if(1 == CurveNode[idx].flagNode)
			{
				if(CurveNode[idx].next >= S$THICK_MAX - idxMove && idx < S$THICK_MAX - idxMove)
					{
					endIdx = idx;
					break;
				}
			}
		}

		for (idx = 1; idx< endIdx; idx ++)
		{
			if(1 == CurveNode[idx].flagNode)
			{
				tempCurveNode[idx + idxMove] = CurveNode[idx];

				tempCurveNode[idx + idxMove].former = CurveNode[idx].former + idxMove;
				tempCurveNode[idx + idxMove].next   = CurveNode[idx].next   + idxMove;
				if(tempCurveNode[idx + idxMove].next > S$THICK_MAX)tempCurveNode[idx + idxMove].next = S$THICK_MAX;

				tempCurveNode[idx + idxMove].v0.y = CurveNode[idx].v0.y + idxMove;
				tempCurveNode[idx + idxMove].v1.y = CurveNode[idx].v1.y + idxMove;
				tempCurveNode[idx + idxMove].v2.y = CurveNode[idx].v2.y + idxMove;
				tempCurveNode[idx + idxMove].v3.y = CurveNode[idx].v3.y + idxMove;
			}
		}

		if(CurveNode[S$THICK_MAX - idxMove].flagNode != 1)
		{
			if(endIdx != 0)
			{
				tempCurveNode[endIdx + idxMove] = CurveNode[endIdx];

				tempCurveNode[endIdx + idxMove].former = CurveNode[endIdx].former + idxMove;
				tempCurveNode[endIdx + idxMove].next   = S$THICK_MAX;

				tempCurveNode[endIdx + idxMove].v0.y = CurveNode[endIdx].v0.y + idxMove;
				tempCurveNode[endIdx + idxMove].v1.y = CurveNode[endIdx].v1.y + idxMove;
				tempCurveNode[endIdx + idxMove].v2.y = CurveNode[endIdx].v2.y + idxMove;
				tempCurveNode[endIdx + idxMove].v3.y = CurveNode[endIdx].v3.y + idxMove;
			}

			t =(float)(S$THICK_MAX - idxMove - CurveNode[CurveNode[endIdx].next].v0.y)/(float)(CurveNode[CurveNode[endIdx].next].v3.y -
				CurveNode[CurveNode[endIdx].next].v0.y);

				tempCurveNode[S$THICK_MAX].former = endIdx + idxMove;
			tempCurveNode[S$THICK_MAX].next   = 0;

			tempCurveNode[S$THICK_MAX].flagNode = 1;

			tempCurveNode[S$THICK_MAX].v0.x = CurveNode[CurveNode[endIdx].next].v0.x;
			tempCurveNode[S$THICK_MAX].v0.y = CurveNode[CurveNode[endIdx].next].v0.y + idxMove;

			tempCurveNode[S$THICK_MAX].v1.x = (1-t)*CurveNode[CurveNode[endIdx].next].v0.x + t*CurveNode[CurveNode[endIdx].next].v1.x;
			tempCurveNode[S$THICK_MAX].v1.y = (1-t)*CurveNode[CurveNode[endIdx].next].v0.y + t*CurveNode[CurveNode[endIdx].next].v1.y;
			tempCurveNode[S$THICK_MAX].v1.y = tempCurveNode[S$THICK_MAX].v1.y + idxMove;

			tempCurveNode[S$THICK_MAX].v2.x = (1-t)*(1-t)*CurveNode[CurveNode[endIdx].next].v0.x +
				t*t*CurveNode[CurveNode[endIdx].next].v2.x + 2*t*(1-t)*CurveNode[CurveNode[endIdx].next].v1.x;
			tempCurveNode[S$THICK_MAX].v2.y = (1-t)*(1-t)*CurveNode[CurveNode[endIdx].next].v0.y +
				t*t*CurveNode[CurveNode[endIdx].next].v2.y + 2*t*(1-t)*CurveNode[CurveNode[endIdx].next].v1.y;
			tempCurveNode[S$THICK_MAX].v2.y = tempCurveNode[S$THICK_MAX].v2.y + idxMove;

			tempCurveNode[S$THICK_MAX].v3.x = SetPos[S$THICK_MAX - idxMove].x;
			tempCurveNode[S$THICK_MAX].v3.y = S$THICK_MAX;
		}
		else
		{
			if(endIdx != 0)
			{
				tempCurveNode[endIdx + idxMove] = CurveNode[endIdx];

				tempCurveNode[endIdx + idxMove].former = CurveNode[endIdx].former + idxMove;
				tempCurveNode[endIdx + idxMove].next   = S$THICK_MAX;

				tempCurveNode[endIdx + idxMove].v0.y = CurveNode[endIdx].v0.y + idxMove;
				tempCurveNode[endIdx + idxMove].v1.y = CurveNode[endIdx].v1.y + idxMove;
				tempCurveNode[endIdx + idxMove].v2.y = CurveNode[endIdx].v2.y + idxMove;
				tempCurveNode[endIdx + idxMove].v3.y = CurveNode[endIdx].v3.y + idxMove;
			}

			tempCurveNode[S$THICK_MAX] = CurveNode[S$THICK_MAX - idxMove];

				tempCurveNode[S$THICK_MAX].former = endIdx + idxMove;
			tempCurveNode[S$THICK_MAX].next   = 0;

			tempCurveNode[S$THICK_MAX].v0.y = CurveNode[S$THICK_MAX - idxMove].v0.y + idxMove;
			tempCurveNode[S$THICK_MAX].v1.y = CurveNode[S$THICK_MAX - idxMove].v1.y + idxMove;
			tempCurveNode[S$THICK_MAX].v2.y = CurveNode[S$THICK_MAX - idxMove].v2.y + idxMove;
			tempCurveNode[S$THICK_MAX].v3.y = S$THICK_MAX;
		}

		memcpy(CurveNode,tempCurveNode,sizeof(CurveNode));

		for (idx = idxMove; idx<= S$THICK_MAX; idx ++)
		{
			tempSetPos[idx].x= SetPos[idx-idxMove].x;
			tempSetPos[idx].y = idx;
		}

		for (idx = 0; idx<idxMove; idx ++)
		{
			tempSetPos[idx].x = SetPos[0].x;
			tempSetPos[idx].y = idx;
		}

		memcpy(SetPos,tempSetPos,sizeof(SetPos));
	}
	else 
	{
		startIdx = S$THICK_MAX;
		for (idx = S$THICK_MAX; idx > 1 ; idx --)
		{
			if(1 == CurveNode[idx].flagNode)
			{
				if(CurveNode[idx].v0.y < idxMove && idx <= CurveNode[idx].v3.y)
				{
					startIdx = idx;
					break;
				}
			}
		}

		if(CurveNode[idxMove].flagNode != 1)
		{
			tempCurveNode[0].former   = S$THICK_MAX;
			if(startIdx != 0)
			{
				tempCurveNode[0].next = startIdx - idxMove;
			}
			else
			{
				tempCurveNode[0].next = S$THICK_MAX - idxMove;
			}
			tempCurveNode[0].flagNode = 1;

			tempCurveNode[0].v3.x     = SetPos[idxMove].x;
			tempCurveNode[0].v3.y     = 0;

			tempCurveNode[tempCurveNode[0].next].former = 0;
			if(startIdx == S$THICK_MAX)
				{
				tempCurveNode[tempCurveNode[0].next].next = S$THICK_MAX;
			}
			else
			{
				tempCurveNode[tempCurveNode[0].next].next = CurveNode[startIdx].next - idxMove;
			}
			tempCurveNode[tempCurveNode[0].next].flagNode = 1;

			t =(float)(idxMove - CurveNode[startIdx].v0.y)/(float)(CurveNode[startIdx].v3.y - CurveNode[startIdx].v0.y);

			tempCurveNode[tempCurveNode[0].next].v0.x = SetPos[idxMove].x;
			tempCurveNode[tempCurveNode[0].next].v0.y = 0;

			tempCurveNode[tempCurveNode[0].next].v1.x = (1-t)*(1-t)*CurveNode[startIdx].v1.x + 2*t*(1-t)*CurveNode[startIdx].v2.x + t*t*CurveNode[startIdx].v3.x + 1;
			tempCurveNode[tempCurveNode[0].next].v1.y = (1-t)*(1-t)*CurveNode[startIdx].v1.y + 2*t*(1-t)*CurveNode[startIdx].v2.y + t*t*CurveNode[startIdx].v3.y;
			tempCurveNode[tempCurveNode[0].next].v1.y = tempCurveNode[tempCurveNode[0].next].v1.y - idxMove;

			tempCurveNode[tempCurveNode[0].next].v2.x = (1-t)*CurveNode[startIdx].v2.x + t*CurveNode[startIdx].v3.x + 1;
			tempCurveNode[tempCurveNode[0].next].v2.y = (1-t)*CurveNode[startIdx].v2.y + t*CurveNode[startIdx].v3.y;
			tempCurveNode[tempCurveNode[0].next].v2.y = tempCurveNode[tempCurveNode[0].next].v2.y - idxMove;

			tempCurveNode[tempCurveNode[0].next].v3.x = CurveNode[startIdx].v3.x;
			tempCurveNode[tempCurveNode[0].next].v3.y = CurveNode[startIdx].v3.y;
			tempCurveNode[tempCurveNode[0].next].v3.y = tempCurveNode[tempCurveNode[0].next].v3.y - idxMove;
		}
		else
		{
			tempCurveNode[0].former = S$THICK_MAX;
			tempCurveNode[0].next   = CurveNode[startIdx].next - idxMove;
			tempCurveNode[0].flagNode = 1;

			tempCurveNode[0].v3.x     = SetPos[idxMove].x;
			tempCurveNode[0].v3.y     = 0;
		}

		for (idx = startIdx + 1; idx< S$THICK_MAX; idx ++)
		{
			if(1 == CurveNode[idx].flagNode)
			{
				tempCurveNode[idx - idxMove] = CurveNode[idx];

				if(CurveNode[idx].former != 0)
				{
					tempCurveNode[idx - idxMove].former = CurveNode[idx].former - idxMove;
				}
				else
				{
					tempCurveNode[idx - idxMove].former = 0;
				}

				tempCurveNode[idx - idxMove].next = CurveNode[idx].next - idxMove;

				tempCurveNode[idx - idxMove].v0.y = CurveNode[idx].v0.y - idxMove;
				tempCurveNode[idx - idxMove].v1.y = CurveNode[idx].v1.y - idxMove;
				tempCurveNode[idx - idxMove].v2.y = CurveNode[idx].v2.y - idxMove;
				tempCurveNode[idx - idxMove].v3.y = CurveNode[idx].v3.y - idxMove;
			}
		}

		if(startIdx != S$THICK_MAX)
		{
			tempCurveNode[S$THICK_MAX - idxMove] = CurveNode[S$THICK_MAX];

				tempCurveNode[S$THICK_MAX - idxMove].former = CurveNode[S$THICK_MAX].former - idxMove;
			tempCurveNode[S$THICK_MAX - idxMove].next   = S$THICK_MAX;
			tempCurveNode[S$THICK_MAX - idxMove].flagNode = 1;

			tempCurveNode[S$THICK_MAX - idxMove].v0.y = CurveNode[idx].v0.y - idxMove;
			tempCurveNode[S$THICK_MAX - idxMove].v1.y = CurveNode[idx].v1.y - idxMove;
			tempCurveNode[S$THICK_MAX - idxMove].v2.y = CurveNode[idx].v2.y - idxMove;
			tempCurveNode[S$THICK_MAX - idxMove].v3.y = CurveNode[idx].v3.y - idxMove;
		}

		tempCurveNode[S$THICK_MAX].former = S$THICK_MAX - idxMove;
		tempCurveNode[S$THICK_MAX].next   = 0;
		tempCurveNode[S$THICK_MAX].flagNode = 1;

		tempCurveNode[S$THICK_MAX].v0.x = SetPos[S$THICK_MAX].x;
		tempCurveNode[S$THICK_MAX].v0.y = S$THICK_MAX - idxMove;

		tempCurveNode[S$THICK_MAX].v1.x = SetPos[S$THICK_MAX].x + 1;
		tempCurveNode[S$THICK_MAX].v1.y = S$THICK_MAX - idxMove * 2/3;

		tempCurveNode[S$THICK_MAX].v2.x = SetPos[S$THICK_MAX].x + 1;
		tempCurveNode[S$THICK_MAX].v2.y = S$THICK_MAX - idxMove/3;

		tempCurveNode[S$THICK_MAX].v3.x = SetPos[S$THICK_MAX].x;
		tempCurveNode[S$THICK_MAX].v3.y = S$THICK_MAX ;

		memcpy(CurveNode,tempCurveNode,sizeof(CurveNode));

		for (idx = S$THICK_MAX; idx> S$THICK_MAX -idxMove; idx --)
		{
			tempSetPos[idx].x = SetPos[S$THICK_MAX].x;
			tempSetPos[idx].y = idx;
		}
		for (idx = 0; idx<= S$THICK_MAX - idxMove; idx ++)
		{
			tempSetPos[idx].x = SetPos[idx+idxMove].x;
			tempSetPos[idx].y = idx;
		}

		memcpy(SetPos,tempSetPos,sizeof(SetPos));

	}/* if(moveValue > 0) */

	DrawMan.v1 				=  tempCurveNode[S$THICK_MAX].v1.x;
	DrawMan.v2 				=  tempCurveNode[S$THICK_MAX].v2.x;
	DrawMan.v03 			=  tempCurveNode[S$THICK_MAX].v3.x;;
	DrawMan.currentNode  	=  S$THICK_MAX;
	DrawMan.currentPoint 	=  0;
	DrawMan.flagLineFocus 	=  0;

	DrawMan.draw = 1;

}/*end of function*/

void curveClear()
{
	memset(SetPos,0,sizeof(SetPos));
	memset(CurveNode,0,sizeof(CurveNode));
	
	for( i = 0; i<= S$THICK_MAX; i++)
	{
	//	DrawMan.weight = 300;  //ipis0619 dont need 10% after clear
		SetPos[i].x = DrawMan.weight + 300;
		SetPos[i].y = i;
	}

	DrawMan.oldWeight 			=  DrawMan.weight;
	DrawMan.v1 						=  (DrawMan.weight + 300) + 1;
	DrawMan.v2 						=  (DrawMan.weight + 300) + 1;
	DrawMan.v03 					=  0;
	DrawMan.currentNode  	   =  S$THICK_MAX;
	DrawMan.currentPoint 		=  0;
	DrawMan.flagLineFocus 		=  0;
	CurveNode[0].former    		=  S$THICK_MAX;
	CurveNode[0].next    		=  S$THICK_MAX;  
	CurveNode[0].v3.x    		=  DrawMan.weight + 300;
	CurveNode[0].v3.y    		=  0;
	CurveNode[0].flagNode  		=  1;
       
	CurveNode[S$THICK_MAX].former 	= 0;
	CurveNode[S$THICK_MAX].next   	= 0;
	CurveNode[S$THICK_MAX].v0.x   	= DrawMan.weight + 300;
	CurveNode[S$THICK_MAX].v0.y   	= 0;
	CurveNode[S$THICK_MAX].v3.x   	= DrawMan.weight + 300;
	CurveNode[S$THICK_MAX].v3.y   	= S$THICK_MAX;
    
	CurveNode[S$THICK_MAX].v1.x   	= (DrawMan.weight+ 300) + 1;
	CurveNode[S$THICK_MAX].v1.y   	= CurveNode[S$THICK_MAX].v0.y + 
		(CurveNode[S$THICK_MAX].v3.y - CurveNode[S$THICK_MAX].v0.y)*1/
		3 ;
		CurveNode[S$THICK_MAX].v2.x      = (DrawMan.weight + 300) + 1;
	CurveNode[S$THICK_MAX].v2.y   	= CurveNode[S$THICK_MAX].v0.y + 
		(CurveNode[S$THICK_MAX].v3.y - CurveNode[S$THICK_MAX].v0.y)*(S$THICK_V_SCALE -1)/
		S$THICK_V_SCALE;
    
	CurveNode[S$THICK_MAX].flagNode =  1;
	
	setColor = 1;
}/*end of function */
/************************************  END *******************************************/









