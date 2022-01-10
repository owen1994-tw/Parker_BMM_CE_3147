/********************************************************************
 * COPYRIGHT --  B&R CHINA
 ********************************************************************
 * Program: cur_draw
 * File: cur_draw.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cur_draw --- thick profile curve draw in HMI 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

/***********************************************************************************
** 			                    Include user files								  **
***********************************************************************************/
#include "draw.h"
#include "pic_id.h"
#include "thick.h"

/***********************************************************************************
                             Function Declarations
***********************************************************************************/
UINT vsLine( UDINT VCHandle, UINT x1, UINT y1,UINT x2, UINT y2, USINT color);
UINT vsLineH( UDINT VCHandle, UINT x, UINT y, UINT len, USINT color);
UINT vsLineV( UDINT VCHandle, UINT x, UINT y, UINT len, USINT color);
UINT vsRect(  UDINT VCHandle, UINT x1, UINT y1,UINT x2, UINT y2, USINT color);

/**********************************************************************************
**     	        		 I N I T U P  O F   T H E   T A S K 		 		     **
**********************************************************************************/
_INIT  void init(void)
{
	width 		= 	VSCREEN_WIDTH;
	height 		= 	VSCREEN_HEIGHT;
	backColor  	= 	VSCREEN_COLOR_BACKGROUND;
	frameColor 	= 	VSCREEN_COLOR_FRAME;
	crossColor	= 	VSCREEN_COLOR_CROSS_CURSOR;
	fillColor   =  VSCREEN_COLOR_BACKGROUND;

	ORIGINAL_X0  = 100;
	ORIGINAL_Y0  = 150;
	
	rSetScale = 0.1;
	rActScale = 1.0;
	xScaleBmp = 0;

	drawStep = 0;
}/*  end of INIT */

/**********************************************************************************
**        	     C Y C L I C   P A R T   O F   T H E   T A S K  				 **
**********************************************************************************/
_CYCLIC void cyc(void)
{
	INT X,Y;
	
	if((gPanelObj.GetPicture == PIC_CURVE_TRACE) ||
		(1 == gThickProf.FunOption.IsTraceInEdit && gPanelObj.GetPicture == PIC_CURVE_EDIT))
	{
		if (memcmp(getAT,gThickData.RealData,sizeof(getAT)) != 0 && DrawMan.draw == 0)
		{
			DrawMan.draw = 1;
			memcpy(getAT,gThickData.RealData,sizeof(getAT));
		}
	}

	if (gPanelObj.GetPicture == PIC_CURVE_EDIT)
	{
		if (DRAW_PIC != PIC_CURVE_EDIT && DRAW_PIC != 255)
		{
			DrawMan.draw = 1;
		}
		DRAW_PIC = PIC_CURVE_EDIT;
	}
	else if (gPanelObj.GetPicture == PIC_CURVE_TRACE)
	{
		if (DRAW_PIC != PIC_CURVE_TRACE && DRAW_PIC != 255)
		{
			DrawMan.draw = 1;
		}
		DRAW_PIC = PIC_CURVE_TRACE;
	}
	else
	{
		DRAW_PIC = 255;
	}

	if (gPanelObj.GetPicture == PIC_CURVE_EDIT || gPanelObj.GetPicture == PIC_CURVE_TRACE)
	{
		switch (drawStep)
		{
			case 0:
				vcHandle = VA_Setup(1,gPanelObj.VisualName);
				if (vcHandle)  drawStep = 100;
				break;
            
			case 100:
				status = VA_Saccess(1, vcHandle);
				if(status == 0)
				{
					drawStep = 200;
				}
				break;

			case 200:
				Status_Attach = 1;
				if (gPanelObj.GetPicture == PIC_CURVE_EDIT)
				{
					Status_Attach = VA_Attach(1,vcHandle,0,(UDINT)"Page_82_Profile/Default/DrawBox_1");
				}
				else
				{
					Status_Attach = VA_Attach(1,vcHandle,0,(UDINT)"Page_83_Trace/Default/DrawBox_1");
				}
				if(Status_Attach == 0)
				{
					/* Get width of DrawBox Control*/
					VA_GetDisplayInfo(1,vcHandle,1,(UDINT)&Drawbox_width);
					/* Get height of DrawBox Control */ 
					VA_GetDisplayInfo(1,vcHandle,2,(UDINT)&Drawbox_height);
				
					drawStep = 300;
				}
				else
				{
					VA_Srelease(1, vcHandle);
					drawStep = 100;
				}
				break;
			case 300:
				xMax = 0.0;
				for(i=0;i<= S$THICK_MAX;i++)
					{
					if(xMax < SetPos[i].x)xMax = SetPos[i].x;
				}
				
				
				if (usScaleIndex == 1) //ipis0619 
				{
					rSetScale = 0.4;
					rActScale = 4.0;
					xScaleBmp = 2;
				}else if (usScaleIndex == 2)
				{
					rSetScale = 0.2;
					rActScale = 2.0;
					xScaleBmp = 1;
				}else
				{
					rSetScale = 0.1;
					rActScale = 1.0;
					xScaleBmp = 0;
				}
		/*---//ipis0619 -----------------------------------------*/		
//				if(xMax >= 1500)
//				{
//					rSetScale = 0.1;
//					rActScale = 1.0;
//					xScaleBmp = 0;
//				}
//				else if(xMax >= 750)
//				{
//					rSetScale = 0.2;
//					rActScale = 2.0;
//					xScaleBmp = 1;
//				}
//				else
//				{
//					rSetScale = 0.4;
//					rActScale = 4.0;
//					xScaleBmp = 2;
//				}
		/*---//ipis0619 -----------------------------------------*/				
				status = VA_Redraw (1,vcHandle);
				if(0 == status)
				{
					if (gPanelObj.GetPicture == PIC_CURVE_EDIT)  drawStep  = 400;
					if (gPanelObj.GetPicture == PIC_CURVE_TRACE) drawStep  = 600;
				}
				break;
			/**************draw the setting curve*************/
			case 400:
				for(i=0;i<= S$THICK_MAX;i++)
					{
					X = (INT)(SetPos[i].x*rSetScale + 0.1);
					if(X > Drawbox_width - 2)X = Drawbox_width - 2;
					Y = S$THICK_POINT - SetPos[i].y;    /* 0 <SetPos[i].y <= 399  ->  1 < Y < 400*/
              
					vsLineH(vcHandle,0,Y,Drawbox_width-1,VSCREEN_COLOR_BACKGROUND);
  
					if (1 == CurveNode[i].flagNode)
					{
						if(i == DrawMan.currentNode && 1 == DrawMan.flagLineFocus)
						{
							vsLineH(vcHandle,0,Y,X,crossColor);
							vsLineH(vcHandle,0,Y,DrawMan.weight*rSetScale,VSCREEN_COLOR_WEIGHT);
						}
						else
						{
							vsLineH(vcHandle,0,Y,X,VSCREEN_COLOR_CURVE);
							vsLineH(vcHandle,0,Y,DrawMan.weight*rSetScale,VSCREEN_COLOR_WEIGHT);
						}
					}
					else
					{
						vsLineH(vcHandle,0,Y,X,VSCREEN_COLOR_UNFOCUS_LINE);
						vsLineH(vcHandle,0,Y,DrawMan.weight*rSetScale,VSCREEN_COLOR_WEIGHT);
					}
    
					if(0 == DrawMan.flagLineFocus)
					{
						if(i > CurveNode[DrawMan.currentNode].former + 1 && i <= DrawMan.currentNode)
						{
							vsLine(vcHandle,(INT)(SetPos[i-1].x*rSetScale + 0.1),S$THICK_POINT - SetPos[i-1].y,X,Y,crossColor);
								}
						else
						{
							if(i > 0)
							{
								vsLine(vcHandle,(INT)(SetPos[i-1].x*rSetScale + 0.1),S$THICK_POINT - SetPos[i-1].y,X,Y,VSCREEN_COLOR_CURVE);
									}
						}
					}
					else
					{
						if(i > 0)
						{
							vsLine(vcHandle,(INT)(SetPos[i-1].x*rSetScale + 0.1),S$THICK_POINT - SetPos[i-1].y,X,Y,VSCREEN_COLOR_CURVE);
								}
					}
				}
    
				if(DrawMan.step == S$STEP_ADDLINE)
					{
					drawStep = 500;
				}
				else
				{
					drawStep = 550;
				}
				break;
			/*---draw the cross line ---*/
			case 500:
				xTemp = (INT)(SetPos[DrawMan.currentPoint].x*rSetScale + 0.1) - 20;
				if(xTemp < 0)xTemp = 0;
				
				if(xTemp + 40 < width)
				{
					xLength = 40;
				}
				else
				{
					xLength = width - 40;
				}
				vsLineH(vcHandle,xTemp,S$THICK_POINT - DrawMan.currentPoint,xLength,crossColor);
				
					yTemp	= S$THICK_POINT - DrawMan.currentPoint - 20;
				if(yTemp < 0)yTemp = 0;
				
				if(yTemp + 40 < height)
				{
					yLength = 40;
				}
				else
				{
					yLength = height - 40;
				}
				vsLineV(vcHandle,(INT)(SetPos[DrawMan.currentPoint].x*rSetScale + 0.1),yTemp,yLength,crossColor);
				drawStep = 550;
				break;
			/*------ is or not draw the trace curve in edit profile page ------*/
			case 550:
				if(1 == gThickProf.FunOption.IsTraceInEdit)
				{
					drawStep = 700;
				}
				else
				{
					drawStep = 900;
				}
				break;
			/**************draw the setting curve*************/
			case 600:
				for(i=0;i<= S$THICK_MAX;i++)
					{
					vsLineH(vcHandle,0,S$THICK_POINT - SetPos[i].y,Drawbox_width-1,VSCREEN_COLOR_BACKGROUND);
						vsLineH(vcHandle,0,S$THICK_POINT - SetPos[i].y,(INT)(SetPos[i].x*rSetScale + 0.1),VSCREEN_COLOR_UNFOCUS_LINE);
						}
				drawStep = 700;
				break;
			/**************draw the trace curve*************/
			case 700:
				for(i=0;i<S$THICK_SAMPLE-1;i++)
					{
					if (getAT[i+1] != 0)
					{
						firstPos.x  = getAT[i] * rActScale;
						firstPos.y  = i*4;
						secondPos.x = getAT[i+1] * rActScale;
						secondPos.y = (i+1)*4;
						status = vsLine(vcHandle, firstPos.x, S$THICK_POINT-firstPos.y, secondPos.x, S$THICK_POINT-secondPos.y, crossColor);
							}
				}
    
				if(getAT[S$THICK_SAMPLE-1]!= 0)
					{
					status = vsLine(vcHandle, getAT[S$THICK_SAMPLE-1] * rActScale, S$THICK_POINT - 396, getAT[S$THICK_SAMPLE-1] * rActScale, S$THICK_POINT - 399, crossColor);
						}
				drawStep = 900;
				break;
			/* copy the screen to interface  */
			case 900:
				vsRect(vcHandle,0,0,VSCREEN_WIDTH-1,VSCREEN_HEIGHT-1,VSCREEN_COLOR_FRAME);
				VA_Redraw (1,vcHandle);
				VA_Detach(1,vcHandle);
				VA_Srelease(1, vcHandle);
				if((gPanelObj.GetPicture == PIC_CURVE_EDIT) || (gPanelObj.GetPicture == PIC_CURVE_TRACE))
				{
					DrawMan.draw = 0;
					drawStep = 1000;
				}
				else
				{
					drawStep = 2000;
				}
				break;
			/*	waiting for draw  */
			case 1000:
				if (1 == DrawMan.draw )
				{
					drawStep  = 100;
				}
				break;
			case 2000:
				drawStep  = 100;
				break;
		}
	}
	else
	{
		if (drawStep != 0 && vcHandle != 0) drawStep = 100;
	}

}/*end of CYCLIC*/


UINT vsLine( UDINT VCHandle, UINT x1, UINT y1,UINT x2, UINT y2, USINT color)
{
	if(x1 > Drawbox_width-2)x1 = Drawbox_width-2;
	if(x2 > Drawbox_width-2)x2 = Drawbox_width-2;
	return  VA_Line(1, VCHandle,x1,y1,x2,y2,color);
}

UINT vsLineH( UDINT VCHandle, UINT x, UINT y, UINT len, USINT color)
{
	if(x+len <= Drawbox_width-2)
	{
		return  VA_Line(1, VCHandle,x,y,x+len,y,color);
	}
	else
	{
		return  VA_Line(1, VCHandle,x,y,Drawbox_width-2,y,color);
	}
}

UINT vsLineV( UDINT VCHandle, UINT x, UINT y, UINT len, USINT color)
{
	if(x > Drawbox_width-2)x = Drawbox_width-2;
	return  VA_Line(1, VCHandle,x,y,x,y+len,color);
}

UINT vsRect(  UDINT VCHandle, UINT x1, UINT y1,UINT x2, UINT y2, USINT color)
{
	return VA_Rect(1,VCHandle,x1,y1,x2-x1,y2-y1,255,color);
}



