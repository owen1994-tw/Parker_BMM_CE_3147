/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: mode
 * File: mode.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program mode --- task stop/start 
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "defines.h"
/*************************************************************************************/
/****	        		I N I T U P  O F   T H E   T A S K 		 				             ****/
/*************************************************************************************/
void _INIT Mode_init(void)
{
	memset(&TKmode, 0,sizeof(TKmode));
	memset(&gTKauto, 0,sizeof(gTKauto));
	memset(&gTKmanual, 0,sizeof(gTKmanual));
	memset(&gTKcalib, 0,sizeof(gTKcalib));

	gPROCESS_info.mode_req = MODE_ADJUST;
	

	status_auto = ST_ident("autorun", 0, &id_auto);		/* Automatic Task */
  	if (status_auto!=0)
	{
		ERRxwarning (status_auto, 0, "Error mode task: auto task not found");
	}
	status_man = ST_ident("manual", 0, &id_manual);		/* Manual Task */
  	if (status_man!=0)
	{
		ERRxwarning (status_man, 0, "Error mode task: manual task not found");
	}
	status_calib = ST_ident("calib", 0, &id_calib);	/* Manual Task */
  	if (status_calib!=0)
	{
		ERRxwarning (status_calib, 0, "Error mode task: calib task not found");
	}

	TKmode.Step=MODE_NOTHING;                           /* ready state   */
} /* end of _INIT */

/**************************************************************************************
****				C Y C L I C   P A R T   O F   T H E   T A S K  				           ****
**************************************************************************************/
void _CYCLIC Mode_cyclic(void)
{
	TKmode.Active++;                       /*  task  mode status number  */

	switch (TKmode.Step)
 	{
	   /* -------------------------------------------------------------------*/
    	case MODE_NOTHING: /* Mode undefined */
	   /* -------------------------------------------------------------------*/
    		gPROCESS_info.mode 	= MODE_NOTHING;	/* No operation mode defined */

			/* Treating the request of operation mode */
			switch (gPROCESS_info.mode_req)
			{
				case MODE_AUTO:
	       			TKmode.Step = MODE_AUTO;
          			gTKauto.Stop = 0;
        			break;

				case MODE_MANUAL:
					TKmode.Step = MODE_MANUAL;
	        		gTKmanual.Stop = 0;
				    break;
			    case MODE_ADJUST:
			    	TKmode.Step = MODE_ADJUST;
	        		gTKmanual.Stop = 0;
					break;
			}
	    	break;
		/* -------------------------------------------------------------------*/
	    case MODE_AUTO:			    /* Automatic mode */
	    case MODE_SEMI_AUTO:		/* Semi-Automatic mode */
		 /* -------------------------------------------------------------------*/
		   /* If there is a request of change of operation mode */
   		if ((gPROCESS_info.mode_req != MODE_NOTHING) &&
   			((gPROCESS_info.mode_req != MODE_AUTO) && (gPROCESS_info.mode_req != MODE_SEMI_AUTO) ))
       	{
       			gTKauto.Stop = 1;                                  /*request manual ,send auto stop message */
        		   TKmode.Step = MODE_NOTHING;
			}
			/* start autotask and wait until manual task has finished */
   		else if ((gTKmanual.Status !=CONTROL_STAT_ACTIVE)  &&
   					(gTKauto.Status   !=CONTROL_STAT_ACTIVE) &&
   					(gTKcalib.Status !=CONTROL_STAT_ACTIVE))			/* Manual task has stopped and auto task is suspend*/
      	{
   	      	ST_tmp_resume(id_auto);			                     /* Resume the  Autotask */
       	}
       		/* if automatic task runs and mode not taken: take over mode and response */
			else if (((gTKauto.Status & CONTROL_STAT_ACTIVE) != 0) &&   /* Automatic task active */
   	    	      (gPROCESS_info.mode_req != MODE_NOTHING))			/* No response yet */
       	{
       		gPROCESS_info.mode = gPROCESS_info.mode_req;	            /* Take over operation mode */
				gPROCESS_info.mode_req = MODE_NOTHING;	                  /* Response */
        	}
        	else
        	{
        	}
   		break;
	    /* -------------------------------------------------------------------*/
    	case MODE_MANUAL:		/* Manual mode */
	    /* -------------------------------------------------------------------*/
			/* If there is a request of change of operation mode */
   		if ((gPROCESS_info.mode_req != MODE_NOTHING) &&
   			 (gPROCESS_info.mode_req != MODE_MANUAL ))
   	   {
       		gTKmanual.Stop = 1;                                              /* Send manual stop message */
       		TKmode.Step = MODE_NOTHING;
			}
			/* start manual task and wait until auto  task has finished */
   		else if(((gTKauto.Status    & CONTROL_STAT_ACTIVE) == 0) &&			/* Automatic task has stopped */
		    		  ((gTKmanual.Status  & CONTROL_STAT_ACTIVE) == 0) &&
   			     ((gTKcalib.Status & CONTROL_STAT_ACTIVE) == 0) )   		    /* Manual task not active yet */
         {
   	      ST_tmp_resume(id_manual);		/* Resume the Manual task */
         }
       		/* if manual task runs and mode not taken: take over mode and response */
			else if (((gTKmanual.Status & CONTROL_STAT_ACTIVE) != 0) &&	        /* Manual task active */
   	    	      (gPROCESS_info.mode_req != MODE_NOTHING))					/* No response yet */
       	{
       		gPROCESS_info.mode = gPROCESS_info.mode_req;	             /* Take over operation mode */
				gPROCESS_info.mode_req = MODE_NOTHING;	                        /* Response */
        	}
   		break;
   	    /* -------------------------------------------------------------------*/
    	case MODE_ADJUST:
    		if ((gPROCESS_info.mode_req != MODE_NOTHING) &&
   			 (gPROCESS_info.mode_req != MODE_ADJUST ))
   	   {
       		gTKcalib.Stop = 1;                        /* Send manual stop message */
       		TKmode.Step = MODE_NOTHING;
			}
			/* start manual task and wait until auto  task has finished */
   		else if(((gTKauto.Status   & CONTROL_STAT_ACTIVE) == 0) &&	/* Automatic task has stopped */
		    		  ((gTKmanual.Status & CONTROL_STAT_ACTIVE) == 0) &&
   			     ((gTKcalib.Status & CONTROL_STAT_ACTIVE) == 0))   	/* Manual task not active yet */
         {
   	      ST_tmp_resume(id_calib);		/* Resume the Manual task */
         }
       		/* if manual task runs and mode not taken: take over mode and response */
			else if (((gTKcalib.Status & CONTROL_STAT_ACTIVE) != 0) &&	/* Manual task active */
   	    	      (gPROCESS_info.mode_req != MODE_NOTHING))			/* No response yet */
       	{
       		gPROCESS_info.mode = gPROCESS_info.mode_req;	            /* Take over operation mode */
				gPROCESS_info.mode_req = MODE_NOTHING;	                  /* Response */
        	}
    	   break;
  	} /* switch (TKmode.Step) */
}  /* end of _CYCLIC */



