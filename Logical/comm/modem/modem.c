/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: modem
 * File: modem.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program modem --- modem remote communication  control 
 ********************************************************************/

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <bur/plctypes.h>
#include <string.h>
#include "edge.h"               

/**************************************************************************************
** 					 	     define 								                 **
**************************************************************************************/
#define	BRTRSM_COMMAND_CHAR_MAXCOUT			20
#define	BRTRSM_COMMAND_MAXCOUT					20

#define  BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT		64

#define  BRTRSM_ERR_FUB_BUSY		0xFFFF

/**************************************************************************************
** 					 	     function 								                 **
**************************************************************************************/
UINT	BRFindString(char * p,char * str);

/**************************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				     **
**************************************************************************************/
 _INIT void init ( void )
 {
	 BRInitStep		= 1000;
	 BRStep			= 0;
 }

 /**************************************************************************************
**        				C Y C L I C   P A R T   O F   T H E   T A S K  				 **
**************************************************************************************/

 _CYCLIC void cyclic ( void )
 {

	switch	(BRInitStep)
	{
		case 1000:

			cmdModem		=0;

			strcpy(FmDevice,"IF1");
			#ifdef __i386__

			strcpy(FmMode,"/PHY=RS232 /BD=57600 /PA=N /DB=8 /SB=1");

			#else

			strcpy(FmMode,"57600,N,8,1,RS232");

			#endif


			i	=0;

			strcpy(AcT[i++].Command,"AT&F");	  /* DEFAULT SETTING  */
			strcpy(AcT[i++].Command,"ATS0=1");
			strcpy(AcT[i++].Command,"ATS37=9");
			strcpy(AcT[i++].Command,"AT&C0");
			strcpy(AcT[i++].Command,"AT&D0");
			strcpy(AcT[i++].Command,"AT&K0");
			strcpy(AcT[i++].Command,"AT&Q0");
			strcpy(AcT[i++].Command,"AT&W0");
			strcpy(AcT[i++].Command,"AT&Y0");


			ATCommandCount = i;

			BRInitStep	   = 2000;


			break;
		case 2000:
			if (0 == BRStep && 1 == cmdModem)
			{
				gMachineInfo.Modem_Ok	= 0;
				BRStep		= 1000;
				BRInitStep	= 3000;
			}
			break;
		case 3000:
			Status	=BRFindString(BRReadBuf,BRWriteBuf);
			break;
		default:
         break;
	}
	switch	(BRStep)
	{
		case 1000:

			BRFRMxopen.device	= (unsigned long)&FmDevice;
			BRFRMxopen.mode		= (unsigned long)&FmMode;
			BRFRMxopen.enable	= 1;

			FRM_xopen(&BRFRMxopen);

			if(0 == BRFRMxopen.status)
			{
				BRStep	= 2000;
			}

			break;

		case 2000:

			BRFRMctrl.enable	= 1;
			BRFRMctrl.ident		= BRFRMxopen.ident;
			BRFRMctrl.ioctrl	= 0x12;
			BRFRMctrl.inarg		= 1;

			FRM_ctrl(&BRFRMctrl);

			BRFRMctrl.enable	= 1;
			BRFRMctrl.ident		= BRFRMxopen.ident;
			BRFRMctrl.ioctrl	= 0x27;

			FRM_ctrl(&BRFRMctrl);

			BRStep	= 3000;

			break;
		case 3000:

			memset(BRWriteBuf,0,BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT);
			strcpy(BRWriteBuf,"AT&F");
			BRWriteBuf[4]	= 0x0D;
			BRWriteBuf[5]	= 0x00;

			BRFRMwrite.enable	= 1;
			BRFRMwrite.ident	= BRFRMxopen.ident;
			BRFRMwrite.buffer	= (unsigned long)BRWriteBuf;
			BRFRMwrite.buflng	= strlen(BRWriteBuf);

			FRM_write(&BRFRMwrite);

			if(0 == BRFRMwrite.status)
			{
				BRStep = 3200;
			}

			break;
		case 3200:
			memset(BRWriteBuf,0,BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT);
			strcpy(BRWriteBuf,"ATE0");
			BRWriteBuf[4]	= 0x0D;
			BRWriteBuf[5]	= 0x00;

			BRFRMwrite.enable	= 1;
			BRFRMwrite.ident	= BRFRMxopen.ident;
			BRFRMwrite.buffer	= (unsigned long)BRWriteBuf;
			BRFRMwrite.buflng	= strlen(BRWriteBuf);

			FRM_write(&BRFRMwrite);

			if(0 == BRFRMwrite.status)
			{
				BRStep = 3600;
			}

			break;
		case 3600:
			memset(BRWriteBuf,0,BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT);
			strcpy(BRWriteBuf,"AT&Q0");
			BRWriteBuf[4]	= 0x0D;
			BRWriteBuf[5]	= 0x00;

			BRFRMwrite.enable	= 1;
			BRFRMwrite.ident	= BRFRMxopen.ident;
			BRFRMwrite.buffer	= (unsigned long)BRWriteBuf;
			BRFRMwrite.buflng	= strlen(BRWriteBuf);

			FRM_write(&BRFRMwrite);

			if(0 == BRFRMwrite.status)
			{
				BRStep = 4000;
			}
			break;
		case 4000:

			BRFRMread.enable = 1;
			BRFRMread.ident	 = BRFRMxopen.ident;

			FRM_read(&BRFRMread);

			if(0 == BRFRMread.status)
			{
				if(BRFRMread.buflng<=BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT)
				{
					memcpy(BRReadBuf,(void*)BRFRMread.buffer,BRFRMread.buflng);		/* copy received data */

					BRFRMrbuf.enable	= 1;
					BRFRMrbuf.ident		= BRFRMxopen.ident;
					BRFRMrbuf.buffer	= BRFRMread.buffer;
					BRFRMrbuf.buflng	= BRFRMread.buflng;

					FRM_rbuf(&BRFRMrbuf);											/* release buffer again */

					if(BRFindString(BRReadBuf,"OK")==1)
					{
						CurrentAT	= 0;
						BRStep		= 5000;
					}
					else
					{
						BRStep		= 3000;
					}
				}
			}

			break;
		case 5000:

			memset(BRWriteBuf,0,BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT);
			strcpy(BRWriteBuf,AcT[CurrentAT].Command);
			TempLength	=strlen(BRWriteBuf);
			BRWriteBuf[TempLength]		= 0x0D;
			BRWriteBuf[TempLength+1]	= 0x00;

			TempLength++;

			BRFRMwrite.enable		=1;
			BRFRMwrite.ident		=BRFRMxopen.ident;
			BRFRMwrite.buffer		=(unsigned long)BRWriteBuf;
			BRFRMwrite.buflng		=strlen(BRWriteBuf);

			FRM_write(&BRFRMwrite);

			if(BRFRMwrite.status==0)
			{
				BRStep			=6000;
			}

			break;
		case 6000:

			BRFRMread.enable		=1;
			BRFRMread.ident			=BRFRMxopen.ident;

			FRM_read(&BRFRMread);

			if(BRFRMread.status==0)
			{
				if(BRFRMread.buflng<=BRTRSM_EXCHANGEDATA_CHAR_MAXCOUT)
				{
					memcpy(BRReadBuf,(void*)BRFRMread.buffer,BRFRMread.buflng);		/* copy received data */

					BRFRMrbuf.enable	=1;
					BRFRMrbuf.ident		=BRFRMxopen.ident;
					BRFRMrbuf.buffer	=BRFRMread.buffer;
					BRFRMrbuf.buflng	=BRFRMread.buflng;

					FRM_rbuf(&BRFRMrbuf);											/* release buffer again */

					if(BRFindString(BRReadBuf,"OK")==1)
					{
						CurrentAT++;
						if(CurrentAT>=ATCommandCount)
						{
							BRStep		=7000;
							break;
						}
					}
					BRStep			=5000;

				}
			}
			break;
		case 7000:

			BRFRMclose.enable	=1;
			BRFRMclose.ident	=BRFRMxopen.ident;

			FRM_close(&BRFRMclose);

			BRStep	= 8000;

			break;

		case 8000:
			cmdModem		=0;
			gMachineInfo.Modem_Ok =1;
			BRStep		    =0;

			break;
		default:
         break;
	}


 }
/**************************************************************************************
** 					 	     function 								                 **
**************************************************************************************/
UINT	BRFindString(char * p,char * str)
{
   UINT pLength,strLength,i,j;

   pLength	   =strlen(p);
   strLength	=strlen(str);

   if(strLength==0)
      return 0;
   if(pLength==0)
      return 0;

   j		=0;

   for(i=0;i<pLength;i++)
   {
      if(p[i]==str[j])
      {
         j++;
         if(j>=strLength)
         {
            return 1;
         }
      }
      else
      {
         j		=0;
         if(p[i]==str[j])
         {
            j++;
            if(j>=strLength)
            {
               return 1;
            }
         }
      }
   }

   return 0;
}


