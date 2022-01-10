/**************************************************************************************
 * COPYRIGHT --  B&R CHINA
 **************************************************************************************
 * Program: cur_cal
 * File: cur_cal.c
 * Author: lijianjun
 * Created: March 06, 2013
 *************************************************************************************
 * Implementation of program cur_cal --- thick profile curve data calculation 
 ************************************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


/***********************************************************************************
** 					 	           Include user files								          **
***********************************************************************************/
#include "draw.h"

/**********************************************************************************
**                             Function Declarations								      **
**********************************************************************************/
void BesselCal(short nodeNum);

/**********************************************************************************
**     	        		      I N I T U P  O F   T H E   T A S K 		 				   **
**********************************************************************************/
_INIT void init ( void )
{
	calStep = 0;
}

/**********************************************************************************
**        			 C Y C L I C   P A R T   O F   T H E   T A S K  				   **
**********************************************************************************/
_CYCLIC void cyclic ( void )
{
	if (oldV2 != DrawMan.v2   && enable == 0 && calStep == 0)enable = 1;
	if (oldV1 != DrawMan.v1   && enable == 0 && calStep == 0)enable = 1;
	if (oldV03 != DrawMan.v03 && enable == 0 && calStep == 0)enable = 1;

	SetPos[0].x  = CurveNode[0].v3.x;
	SetPos[0].y  = 0;
	SetPos[S$THICK_MAX].x = CurveNode[S$THICK_MAX].v3.x;
	SetPos[S$THICK_MAX].y = S$THICK_MAX;
	
	BesselCal(DrawMan.currentNode);
	
}/*end of CYCLIC*/


/**************************************************************************************
**        				          FUNCTION  REALIZE				   							 **
**************************************************************************************/
void BesselCal(short nodeNum)
{
	switch (calStep)
	{
		case 0:
			if (1 == enable)
			{
				p0 = &CurveNode[nodeNum];
				p1 = &CurveNode[CurveNode[nodeNum].next];

				v1  = DrawMan.v1;
				v2  = DrawMan.v2;
				v03 = DrawMan.v03;
				if ((p0->v3.y - p0->v0.y) != 0) interval0	= 1/(float)(p0->v3.y - p0->v0.y);
				if ((p1->v3.y - p1->v0.y) != 0) interval1	= 1/(float)(p1->v3.y - p1->v0.y);
				calStep = 10;
			}
			break;
		case 10:
			if (1 == enable)
			{
				if (0 == DrawMan.flagLineFocus)
				{
					p0->v1.x = v1;
					p0->v2.x = v2;
				}
				else
				{
					if (v03 >= (DrawMan.weight) && v03 <= S$THICK_X_LENGTH)
						{
						p0->v3.x = v03;
						p1->v0.x = v03;
						p0->v2.x = v2;
						p1->v1.x = v1;
					}
				}
				calStep = 20;
			}
			break;
		case 20:
			t0  = 0;
			t1  = 0;
			memcpy(oldSetPos,SetPos,sizeof(SetPos));
			calStep = 30;
			break;
		case 30:
			for(i = p0->v0.y + 1 ;i < p0->v3.y ;i++)
			{
				oldSetPos[i].x=(short)((1-t0)*(1-t0)*(1-t0)*(float)p0->v0.x
					+3*t0*(1-t0)*(1-t0)*(float)p0->v1.x
					+3*t0*t0*(1-t0)*(float)p0->v2.x
					+t0*t0*t0*(float)p0->v3.x);

				t0	+= interval0;
				if (oldSetPos[i].x > S$THICK_X_LENGTH || oldSetPos[i].x < DrawMan.weight)
					{
					v1  = oldV1;
					v2  = oldV2;
					v03 = oldV03;
	
					calStep = 10;
					return;
				}
			}
			
			if (1 == DrawMan.flagLineFocus)
			{
				calStep = 40;
			}
			else
			{
				calStep = 50;
			}
			break;
		case 40:
			for(i = p1->v0.y + 1 ;i < p1->v3.y ;i++)
			{

				oldSetPos[i].x=(short)((1-t1)*(1-t1)*(1-t1)*(float)p1->v0.x
					+3*t1*(1-t1)*(1-t1)*(float)p1->v1.x
					+3*t1*t1*(1-t1)*(float)p1->v2.x
					+t1*t1*t1*(float)p1->v3.x);

				t1	+= interval1;
				if (oldSetPos[i].x > S$THICK_X_LENGTH || oldSetPos[i].x < DrawMan.weight)
					{

					v1  = oldV1;
					v2  = oldV2;
					v03 = oldV03;

					calStep = 10;
					return;
				}
			}
			oldSetPos[p0->v3.y].x = v03;
			calStep = 50;
			break;
		case 50:
			calStep = 0;
			if (enable != 0)
			{
				enable = 0;
				memcpy(SetPos,oldSetPos,sizeof(SetPos));
				DrawMan.v1 	= oldV1   = v1;
				DrawMan.v2 	= oldV2   = v2;
				DrawMan.v03 = oldV03  = v03;
			}
			break;
	}

}/*end of function*/


