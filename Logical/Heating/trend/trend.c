/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: panel
 * File: panel.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program panel --- display control 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "keyap2100.h"
//#include "keyap880.h"
#include "pic_id.h"

/**************************************************************************************
                            Function Declarations
**************************************************************************************/

void Trend( Trend_typ * pTrend);

/**************************************************************************************
     	        		I N I T U P  O F   T H E   T A S K
**************************************************************************************/
_INIT void init ( void )
{
	memset(&trend_1,0,sizeof(trend_1));

 	trend_1.Time_Zoom    = 1.0;
	trend_1.Value_Zoom   = 1.0;
	trend_1.Time_Scroll  = 0.0;
	trend_1.Value_Scroll = 0.0;

	trend_1.Curve[0].Status    = 1;
	trend_1.Curve[0].Value_Min = 0;
	trend_1.Curve[0].Value_Max = 300;

	trend_1.Curve[1].Status    = 1;
	trend_1.Curve[1].Value_Min = 0;
	trend_1.Curve[1].Value_Max = 300;

	trend_1.Curve[2].Status    = 1;
	trend_1.Curve[2].Value_Min = 0;
	trend_1.Curve[2].Value_Max = 300;

	trend_1.Curve[3].Status    = 1;
	trend_1.Curve[3].Value_Min = 0;
	trend_1.Curve[3].Value_Max = 300;

} /* end of _INIT */

 /*************************************************************************************
        				C Y C L I C   P A R T   O F   T H E   T A S K
**************************************************************************************/
 _CYCLIC void cyclic ( void )
{
	trend_1.Curve[0].Status = 0;
	trend_1.Curve[1].Status = 0;
	trend_1.Curve[2].Status = 0;
	trend_1.Curve[3].Status = 0;
	trend_1.Curve[4].Status = 0;

	if(gPanelObj.GetPicture == PIC_TEMP_TRACE_1 ||
	   gPanelObj.GetPicture == PIC_TEMP_TRACE_2 ||
	   gPanelObj.GetPicture == PIC_TEMP_TRACE_3 ||
	   gPanelObj.GetPicture == PIC_TEMP_TRACE_4 ||
	   gPanelObj.GetPicture == PIC_TEMP_TRACE_5)
	{
		trend_1.Reset         = gPanelObj.KeyMatric[S$KEYMATRIX_F8];
		trend_1.Time_ScrollDn = gPanelObj.KeyMatric[S$KEYMATRIX_F9];
		trend_1.Time_ScrollUp = gPanelObj.KeyMatric[S$KEYMATRIX_F10];
		trend_1.Value_ScrollDn = gPanelObj.KeyMatric[S$KEYMATRIX_F11];
		trend_1.Value_ScrollUp = gPanelObj.KeyMatric[S$KEYMATRIX_F12];
		trend_1.Value_ZoomDec = gPanelObj.KeyMatric[S$KEYMATRIX_F13];
		trend_1.Value_ZoomInc = gPanelObj.KeyMatric[S$KEYMATRIX_F14];
		
	}

	Trend(&trend_1);

}/* end of _CYCLIC */

void Trend( Trend_typ * pTrend)
{
	if(0 == pTrend->Reset && 1 == pTrend->OldReset)
	{
		pTrend->Time_Zoom    = 1.0;
		pTrend->Value_Zoom   = 1.0;
		pTrend->Time_Scroll  = 0.0;
		pTrend->Value_Scroll = 0.0;
	}

	pTrend->OldReset = pTrend->Reset;

	if(1 == pTrend->Value_ScrollUp)
	{
		pTrend->Value_Scroll = pTrend->Value_Scroll + 0.01;
	}

	if(1 == pTrend->Value_ScrollDn)
	{
		pTrend->Value_Scroll = pTrend->Value_Scroll - 0.01;
	}

	if(pTrend->Value_Scroll < -1.0)pTrend->Value_Scroll = -1.0;
	if(pTrend->Value_Scroll > 1.0)pTrend->Value_Scroll  = 1.0;

	if(1 == pTrend->Time_ScrollUp)
	{
		pTrend->Time_Scroll = pTrend->Time_Scroll + 0.01;
	}

	if(1 == pTrend->Time_ScrollDn)
	{
		pTrend->Time_Scroll = pTrend->Time_Scroll - 0.01;
	}

//	if(pTrend->Time_Scroll < -10.0)pTrend->Time_Scroll = -10.0;
//	if(pTrend->Time_Scroll > 10.0)pTrend->Time_Scroll  = 10.0;


	if(1 == pTrend->Value_ZoomInc)
	{
		pTrend->Value_Zoom = pTrend->Value_Zoom * 1.1;
	}

	if(1 == pTrend->Value_ZoomDec)
	{
		pTrend->Value_Zoom = pTrend->Value_Zoom * 0.9;
	}

	if(pTrend->Value_Zoom < 0.1 )pTrend->Value_Zoom = 0.1;
	if(pTrend->Value_Zoom > 10.0)pTrend->Value_Zoom = 10.0;


	if(1 == pTrend->Time_ZoomInc)
	{
		pTrend->Time_Zoom = pTrend->Time_Zoom * 1.1;
	}

	if(1 == pTrend->Time_ZoomDec)
	{
		pTrend->Time_Zoom = pTrend->Time_Zoom * 0.9;
	}

	if(pTrend->Time_Zoom < 0.1 )pTrend->Time_Zoom = 0.1;
	if(pTrend->Time_Zoom > 10.0)pTrend->Time_Zoom = 10.0;

}/*  void Trend( Trend_typ * pTrend) */


