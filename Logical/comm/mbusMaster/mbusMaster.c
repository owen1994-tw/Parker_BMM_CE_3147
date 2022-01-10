/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: mbusMaster
 * File: mbusMaster.c
 * Author: lij
 * Created: December 12, 2014
 ********************************************************************
 * Implementation of program mbusMaster
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#define	_AR_I386_AUTOMATION_STUDIO_   1

void _INIT mbusMasterINIT(void)
{
	/* TODO: Add code here */
   
   ascii = 0;    /* 0 = RTU / 1 = ASCII */
    
#ifdef _AR_I386_AUTOMATION_STUDIO_
   strcpy(sDeviceName, "SL1.SS1.IF1.ST2.IF1");
   if (!ascii)
   {
      strcpy(sModeName, "PHY=RS485 /BD=19200 /PA=O /DB=8");
   }
   else
   {
      strcpy(sModeName, "PHY=RS485 /BD=19200 /PA=O /DB=7");
   }
#else 
   strcpy(sDeviceName, "SS1.IF1");
   if (!ascii)
   {
      strcpy(sModeName, "RS232,19200,E,8,1");
   }
   else
   {
      strcpy(sModeName, "RS232,19200,E,8,1");
   }
#endif
    
   strcpy(sConfigName, "mb_cmd");
   
   timeout  = 500; /* 500ms */

   /* modbus - Slave open */
   MOpen.enable   = 1;
   MOpen.pDevice  = (UDINT) &sDeviceName[0];
   MOpen.pMode    = (UDINT) &sModeName[0];
   MOpen.pConfig  = (UDINT) &sConfigName[0];
   MOpen.timeout  = timeout;
   MOpen.ascii = ascii;
   
   MBMOpen(&MOpen);

   statusMOpen = MOpen.status;
   if (!statusMOpen)
   {
      ident    = MOpen.ident;
      fMOpen   = 0;
      fMaster  = 1;
      fMClose  = 0;
   }
   else
   {
      fMaster = 0;
      ident   = 0;
      fMOpen  = 1;
   }
   
   fMClose = 0;
}

void _CYCLIC mbusMasterCYCLIC(void)
{
	/* TODO: Add code here */
  
   if(fMOpen)
   {
      /* modbus - Master open */
      MOpen.enable   = 1;
      MOpen.pDevice  = (UDINT) &sDeviceName[0];
      MOpen.pMode    = (UDINT) &sModeName[0];
      MOpen.pConfig  = (UDINT) &sConfigName[0];
      MOpen.timeout  = timeout;
      MOpen.ascii    = ascii;
      
      MBMOpen(&MOpen);
      
      statusMOpen = MOpen.status;
      if (!statusMOpen)
      {
         ident   = MOpen.ident;
         fMOpen  = 0;
         fMaster = 1;
         fMClose = 0;
      }
      else
      {
         fMaster = 0;
         ident   = 0;
         fMOpen  = 1;
      }
   }
   
   if(fMaster)
   {
      /* modbus - Master */
      Master.enable = 1;
      Master.ident  = ident;
      MBMaster(&Master);

      statusMaster= Master.status;
      execute     = Master.execute;
      recordnum   = Master.recordnum;

      if (!statusMaster)
      {
         fMaster = 1;
      }
   }
    
   if(fMClose)
   {
      fMaster = 0;
      fMClose = 0;

      /* modbus - Master close */
      MClose.enable = 1;
      MClose.ident  = ident;
      MBMClose(&MClose);

      statusMClose = MClose.status;
      if (!statusMClose)
      {
         ident  = 0;
         fMOpen = 0;
      }
   }  
}

void _EXIT mbusMasterEXIT(void)
{
	/* TODO: Add code here */
   
   MClose.enable   = 1;
   MClose.ident    = ident;
   MBMClose(&MClose);
}
