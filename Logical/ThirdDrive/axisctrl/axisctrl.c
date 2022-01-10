/********************************************************************
 * COPYRIGHT -- B&R China 
 ********************************************************************
 * Program: axisctrl
 * File: axisctrl.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program axisctrl --- Third servo drive axis control 
 ********************************************************************/
#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#define	STATE_WAIT				 0
#define	STATE_POWER_ON			10
#define	STATE_INIT_HOME		20

#define	STATE_READY				100

#define	STATE_HOME				110
#define	STATE_STOP				120
#define	STATE_MOVE_JOG_POS	130
#define	STATE_MOVE_JOG_NEG	140
#define	STATE_MOVE_ABSOLUTE	150
#define	STATE_MOVE_RELATIVE	160

#define	STATE_ERROR				210
#define	STATE_ERROR_RESET		220
/**************************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				     **
**************************************************************************************/
_INIT void axisctrl_init(void)
{

	for(idxAxis=0;idxAxis<MAX_AXIS_NUM;idxAxis++)
	{
		gDS1119_Axis[idxAxis].init.taskTime = 0.002;    /* 2ms task time   */
		gDS1119_Axis[idxAxis].init.units = 1250;		/* 12.5mm * 100/rev  */
		gDS1119_Axis[idxAxis].init.microsteps = 10000;		/* 10000 pulse /rev   */
		gDS1119_Axis[idxAxis].init.reverseDirection = 1;
	

		gAxisCtrl[idxAxis].param.speed = 3000;
		gAxisCtrl[idxAxis].param.acceleration = 187500;
		gAxisCtrl[idxAxis].param.deceleration = 187500;

		gAxisCtrl[idxAxis].step = STATE_WAIT;
		gAxisCtrl[idxAxis].cmd.power = 1;
		gAxisCtrl[idxAxis].bHomeOk = 0;
	}
}/* end of _INIT */

/*************************************************************************************
**        				C Y C L I C   P A R T   O F   T H E   T A S K  				**
*************************************************************************************/
_CYCLIC void axisctrl_cyc(void)
{
	/*DS1119_1_OK = gDS1119_Axis[0].IOMapping.ModuleOk;
	DS1119_2_OK = gDS1119_Axis[1].IOMapping.ModuleOk;*/

	for(idxAxis=0;idxAxis<MAX_AXIS_NUM;idxAxis++)
	{
		/* default speed  */
		if(0 == gAxisCtrl[idxAxis].param.speed)gAxisCtrl[idxAxis].param.speed = 3000;


		gAxisCtrl[idxAxis].As = gDS1119_Axis[idxAxis].info.position/100.0;
		/* step machine control  */
		switch(gAxisCtrl[idxAxis].step)
		{
			case STATE_WAIT:
				gAxisCtrl[idxAxis].cmd.homing = 0;
				gAxisCtrl[idxAxis].cmd.stop   = 0;
				gAxisCtrl[idxAxis].cmd.jogPos   = 0;
				gAxisCtrl[idxAxis].cmd.jogNeg   = 0;
				gAxisCtrl[idxAxis].cmd.absolute = 0;
				gAxisCtrl[idxAxis].cmd.relative = 0;
				gAxisCtrl[idxAxis].cmd.acknowledge = 0;

				gDS1119_Axis[idxAxis].cmd.homing = 0;
				gDS1119_Axis[idxAxis].cmd.stop   = 0;
				gDS1119_Axis[idxAxis].cmd.absolute = 0;
				gDS1119_Axis[idxAxis].cmd.additive = 0;
				gDS1119_Axis[idxAxis].cmd.velocity= 0;
				gDS1119_Axis[idxAxis].cmd.acknowledge = 0;

				if(1 == gDS1119_Axis[idxAxis].info.initialized)
				{
					if(1 == gAxisCtrl[idxAxis].cmd.power)
					{
						gDS1119_Axis[idxAxis].cmd.power = 1;
						gAxisCtrl[idxAxis].step = STATE_POWER_ON;
					}

					gDS1119_Axis[idxAxis].cmd.homing = 0;
				}
				break;
			case STATE_POWER_ON:
				if(1 == gDS1119_Axis[idxAxis].info.powerOn)
				{
					gDS1119_Axis[idxAxis].cmd.homing = 1;
					gAxisCtrl[idxAxis].step = STATE_INIT_HOME;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_INIT_HOME:
				if(1 == gDS1119_Axis[idxAxis].info.homingOk)
				{
					if(1 == gAxisCtrl[idxAxis].bRefer)
					{
						gAxisCtrl[idxAxis].bHomeOk = 1;
					}

					gAxisCtrl[idxAxis].step = STATE_READY;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_READY:
				if(1 == gAxisCtrl[idxAxis].cmd.homing)
				{
					gAxisCtrl[idxAxis].cmd.homing  = 0;
					gDS1119_Axis[idxAxis].cmd.homing = 1;
					gAxisCtrl[idxAxis].step = STATE_HOME;
				}
				else if(1 == gAxisCtrl[idxAxis].cmd.stop)
				{
					gAxisCtrl[idxAxis].cmd.stop = 0;
					gDS1119_Axis[idxAxis].cmd.stop = 1;
					gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

					gAxisCtrl[idxAxis].step = STATE_STOP;
				}
				else if(1 == gAxisCtrl[idxAxis].cmd.jogPos)
				{
					gDS1119_Axis[idxAxis].cmd.velocity = gAxisCtrl[idxAxis].cmd.jogPos;
					gDS1119_Axis[idxAxis].param.speed  = gAxisCtrl[idxAxis].param.speed;
					gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
					gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

					gAxisCtrl[idxAxis].step = STATE_MOVE_JOG_POS;
				}
				else if(1 == gAxisCtrl[idxAxis].cmd.jogNeg)
				{
					gDS1119_Axis[idxAxis].cmd.velocity = gAxisCtrl[idxAxis].cmd.jogNeg;
					gDS1119_Axis[idxAxis].param.speed  = -gAxisCtrl[idxAxis].param.speed;
					gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
					gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

					gAxisCtrl[idxAxis].step = STATE_MOVE_JOG_NEG;
				}
				else if(1 == gAxisCtrl[idxAxis].cmd.absolute)
				{
					gAxisCtrl[idxAxis].cmd.absolute  = 0;
					gDS1119_Axis[idxAxis].cmd.absolute = 1;
					gDS1119_Axis[idxAxis].param.position = gAxisCtrl[idxAxis].param.position;
					gDS1119_Axis[idxAxis].param.speed    = gAxisCtrl[idxAxis].param.speed;
					gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
					gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

					gAxisCtrl[idxAxis].step = STATE_MOVE_ABSOLUTE;
				}
				else if(1 == gAxisCtrl[idxAxis].cmd.relative)
				{
					gAxisCtrl[idxAxis].cmd.relative = 0;
					gDS1119_Axis[idxAxis].cmd.additive = 1;
					gDS1119_Axis[idxAxis].param.position = gAxisCtrl[idxAxis].param.position;
					gDS1119_Axis[idxAxis].param.speed    = gAxisCtrl[idxAxis].param.speed;
					gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
					gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

					gAxisCtrl[idxAxis].step = STATE_MOVE_RELATIVE;
				}
				break;

			case STATE_HOME:
				if(1 == gDS1119_Axis[idxAxis].info.homingOk)
				{
					if(1 == gAxisCtrl[idxAxis].bRefer)
					{
						gAxisCtrl[idxAxis].bHomeOk = 1;
					}

					gAxisCtrl[idxAxis].step = STATE_READY;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_STOP:
				gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

				if(0 == gDS1119_Axis[idxAxis].info.moveActive)
				{
					gAxisCtrl[idxAxis].step = STATE_READY;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_MOVE_JOG_POS:
				gDS1119_Axis[idxAxis].cmd.velocity = gAxisCtrl[idxAxis].cmd.jogPos;

				gDS1119_Axis[idxAxis].param.speed  = gAxisCtrl[idxAxis].param.speed;
				gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
				gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

				if(1 == gAxisCtrl[idxAxis].cmd.stop)
				{
					gAxisCtrl[idxAxis].cmd.stop   = 0;
					gAxisCtrl[idxAxis].cmd.jogPos = 0;
					gDS1119_Axis[idxAxis].cmd.velocity = 0;
					gDS1119_Axis[idxAxis].cmd.stop     = 1;
					gAxisCtrl[idxAxis].step = STATE_STOP;
				}
				else if(0 == gDS1119_Axis[idxAxis].cmd.velocity)
				{
					if(0 == gDS1119_Axis[idxAxis].info.moveActive)
					{
						gAxisCtrl[idxAxis].step = STATE_READY;
					}
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_MOVE_JOG_NEG:
				gDS1119_Axis[idxAxis].cmd.velocity = gAxisCtrl[idxAxis].cmd.jogNeg;

				gDS1119_Axis[idxAxis].param.speed  = -gAxisCtrl[idxAxis].param.speed;
				gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
				gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

				if(1 == gAxisCtrl[idxAxis].cmd.stop)
				{
					gAxisCtrl[idxAxis].cmd.stop   = 0;
					gAxisCtrl[idxAxis].cmd.jogNeg = 0;
					gDS1119_Axis[idxAxis].cmd.velocity = 0;
					gDS1119_Axis[idxAxis].cmd.stop     = 1;
					gAxisCtrl[idxAxis].step = STATE_STOP;
				}
				else if(0 == gDS1119_Axis[idxAxis].cmd.velocity)
				{
					if(0 == gDS1119_Axis[idxAxis].info.moveActive)
					{
						gAxisCtrl[idxAxis].step = STATE_READY;
					}
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_MOVE_ABSOLUTE:
				gDS1119_Axis[idxAxis].param.position = gAxisCtrl[idxAxis].param.position;
				gDS1119_Axis[idxAxis].param.speed    = gAxisCtrl[idxAxis].param.speed;
				gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
				gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

				if(1 == gAxisCtrl[idxAxis].cmd.stop)
				{
					gAxisCtrl[idxAxis].cmd.stop = 0;
					gDS1119_Axis[idxAxis].cmd.stop = 1;
					gAxisCtrl[idxAxis].step = STATE_STOP;
				}
				else if(0 == gDS1119_Axis[idxAxis].info.moveActive)
				{
					gAxisCtrl[idxAxis].step = STATE_READY;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;

			case STATE_MOVE_RELATIVE:
				gDS1119_Axis[idxAxis].param.position = gAxisCtrl[idxAxis].param.position;
				gDS1119_Axis[idxAxis].param.speed    = gAxisCtrl[idxAxis].param.speed;
				gDS1119_Axis[idxAxis].param.acceleration = gAxisCtrl[idxAxis].param.acceleration;
				gDS1119_Axis[idxAxis].param.deceleration = gAxisCtrl[idxAxis].param.deceleration;

				if(1 == gAxisCtrl[idxAxis].cmd.stop)
				{
					gAxisCtrl[idxAxis].cmd.stop = 0;
					gDS1119_Axis[idxAxis].cmd.stop = 1;
					gAxisCtrl[idxAxis].step = STATE_STOP;
				}
				else if(0 == gDS1119_Axis[idxAxis].info.moveActive)
				{
					gAxisCtrl[idxAxis].step = STATE_READY;
				}

				if(gDS1119_Axis[idxAxis].info.status != 0)
				{
					gAxisCtrl[idxAxis].errorID = gDS1119_Axis[idxAxis].info.status;
					strcpy(gAxisCtrl[idxAxis].errorText,gDS1119_Axis[idxAxis].info.errorText);
					gAxisCtrl[idxAxis].step = STATE_ERROR;
				}
				break;


			case STATE_ERROR:
				if(1 == gAxisCtrl[idxAxis].cmd.acknowledge)
				{
					gAxisCtrl[idxAxis].errorID = 0;
					strcpy(gAxisCtrl[idxAxis].errorText," ");
					gDS1119_Axis[idxAxis].cmd.acknowledge = 1;
					errAckCounter = 0;
					gAxisCtrl[idxAxis].step = STATE_ERROR_RESET;
				}
				break;

			case STATE_ERROR_RESET:
				errAckCounter++;
				if(0 == gDS1119_Axis[idxAxis].info.status)
				{
					gAxisCtrl[idxAxis].step = STATE_WAIT;
				}
				else
				{
					if(errAckCounter > 200)		/* 800 ms */
					{
						gAxisCtrl[idxAxis].step = STATE_WAIT;
															/* error can not acknowledge  */
					}
				}
				break;

		}/* gAxisCtrl[idxAxis].step */

		/* call function DS  */
		DS( &gDS1119_Axis[idxAxis]);

		gAxisCtrl[idxAxis].bReady = ((0 == gAxisCtrl[idxAxis].errorID)        &&
		                             (1 == gDS1119_Axis[idxAxis].info.powerOn)&&
		                             (1 == gDS1119_Axis[idxAxis].info.homingOk));

	}/* for(idxAxis=0;idxAxis<AXIS_NUM_MAX;idxAxis++) */
}/* end of _CYCLIC */


