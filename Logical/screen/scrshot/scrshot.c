/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: length
 * File: length.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program length --- parison length control
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

/* Description: 	screen shot	(C language)	Library - VCScrsht										           */
/*------------------------------------------------------------------------------------------------------*/
/*				这个任务必须在循环7任务里，而且循环7的循环周期必须是1000ms，冗余时间为1000ms			       */
/*				It must also be inserted into Task Class #7 [1000 ms] and have a tolerance of 1000 ms.      */
/*  			程序员需要在初始化任务里定义device名称及相关信息就好了									           */
/*------------------------------------------------------------------------------------------------------*/
/*	version		author			date			decription
*********************************************************************************************************/

/********************************************************************************************************/
/*	<|> Variable to control this task:																                    */
/*		Name							Typ					Description									                    */
/*		step							| UINT				| screen shot step							                 //
//		DateTimeString_0			| DateTimeString	| output current date and time as string	              //
//		more see variable declaration																	                    */
/********************************************************************************************************/

/********************************************************************************************************/
/*	State commentary:																					      //
//			INIT:					   init step															   //
//			WAIT_USB_INSERT:		wait usb device insert												//
//			WAIT_COMMAND:    		wait for command													   //
//			INIT_SCREEN_SHOT:		Init screen shot													   //
//			SCREEN_SHOT:			Screen shot															   //
//			DEINIT_SCREEN_SHOT:		DeInit																//
//			ERROR_STEP:				Error step															   */

/********************************************************************************************************/
/*  Others:
//		指定首选存储介质:																				         //
//          在CPU属性里My Device定义介质名称															    //

//		截屏产生的文件名格式，以当前日期时间为文件名的bmp文件，DateTimeString_0.Date_Mode:		//
//				1:		美国时间格式USA date format  -- DateMM-DD-YYYY_TimeHH-MM-SS					  //
//				2:		亚洲时间格式Asia date format -- DateYYYY-MM-DD_TimeHH-MM-SS					  //
//				others:	默认模式Default format       -- DateDD-MM-YYYY_TimeHH-MM-SS				 //

//		建议：用fileio库来判断所在介质中是否有相同名称的文件，防止被覆盖								//
//		suggest: use filio to judge whether have a same name files in current diretory		  //

//		截屏状态gScreenShot.Status																		       //
//			0		等待存储介质插入插槽
			1		等待截屏开始指令
			2		截屏中
			3		截屏出错																			             //

*********************************************************************************************************/

/**********************************************************************************
**                          函数声明											 **
**********************************************************************************/

/**********************************************************************************
**                          宏定义												 **
**********************************************************************************/
#define INIT				         		0
#define WAIT_USB_INSERT		      		10
#define WAIT_DIR_SCREEN_CREATE		   20
#define WAIT_DIR_SINGLE_CREATE			40
#define WAIT_DIR_ALL_CREATE	   		50

#define WAIT_COMMAND		         100
#define INIT_SCREEN_SHOT	      110
#define SCREEN_SHOT			      120
#define DEINIT_SCREEN_SHOT	      140
#define ERROR_STEP			      200


/**********************************************************************************
**     	        		任务初始化								 				 **
**********************************************************************************/

_INIT  	void init()
{
	/*--------- 程序员根据实际，需要修改的代码 开始 ---------*/

	strcpy(gUSB_1.DeviceName, "USB_1");		/* 指定首选存储介质 */
	strcpy(gUSB_2.DeviceName, "USB_2");		/* 指定第二个选择存储介质 */
   
   strcpy(gUSB_1.DirScreenDeviceName, "SCREEN_1");		/* 指定首选存储介质目录 */
   strcpy(gUSB_2.DirScreenDeviceName, "SCREEN_2");		/* 指定第二个选择存储介质目录 */

   strcpy(dirScreenName,"ScreenShot");
   strcpy(dirSingleName,"SingleRecipe");
   strcpy(dirAllName,"AllRecipe");
   
   strcpy(gUSB_1.DirSingleDeviceName, "SINGLE_1");		/* 指定首选存储介质目录 */
   strcpy(gUSB_2.DirSingleDeviceName, "SINGLE_2");		/* 指定第二个选择存储介质目录 */

   strcpy(gUSB_1.DirAllDeviceName, "ALL_1");		/* 指定首选存储介质目录 */
   strcpy(gUSB_2.DirAllDeviceName, "ALL_2");		/* 指定第二个选择存储介质目录 */

	DateTimeString_0.Date_Mode = 2;		 	/* 截屏产生的文件名格式，见上面的说明 */
	
	statusUSB1 = 0;
	statusUSB2 = 0;
	
	gUSB_1.reserve = 0; 
	gUSB_2.reserve = 0;

	/*--------- 程序员根据实际，需要修改的代码 结束 ---------*/
}/* _INIT  	void init() */

/**********************************************************************************
**     	        		任务循环部分							 				 **
**********************************************************************************/
_CYCLIC void cyclic()
{
   STRING NumName[5];

	/* 接收截屏开始指令 在别的任务里定义，原因：这个任务周期太长，有时抓不到信号 */
	gScreenShot.CmdScreenShot = gScreenShot.CmdScreenShot;

	gScreenShot.Status = gScreenShot.Status;/* 对外接口，截屏状态 */

	/*judge have a usb device insert or not*/
	/* 判断存储介质是否插入插槽内 */
	DirInfo_1.enable= 1;
	DirInfo_1.pDevice= (UDINT)(&gUSB_1.DeviceName);	/* 首选存储介质 */
	DirInfo_1.pPath= 0;							/* For Root, pPath is 0 */

	DirInfo(&DirInfo_1);

	if(DirInfo_1.status != ERR_FUB_BUSY)
	{
		if(DirInfo_1.status == ERR_OK)
		{
			statusUSB1 = 1;
		}
		else
		{
			if(DirInfo_1.status == fiERR_SYSTEM && ERR_FUB_BUSY == FileIoGetSysError())
			{
				statusUSB1 = 1;
			}
			else
			{
				statusUSB1 = 0;  /*  no usb  */
				gUSB_1.DeviceOk = 0;
				gUSB_1.reserve = 0;
			}
		}
	}/* if(DirInfo_1.status != ERR_FUB_BUSY) */
	
	if(1 == statusUSB1 && 0 == statusUSB1_old)
	{
		gUSB_1.reserve = 1;
	}

	
	statusUSB1_old = statusUSB1;

	DirInfo_2.enable= 1;
	DirInfo_2.pDevice= (UDINT)(&gUSB_2.DeviceName);
	DirInfo_2.pPath= 0;

	DirInfo(&DirInfo_2);

	if(DirInfo_2.status != ERR_FUB_BUSY)
	{
		if(DirInfo_2.status == ERR_OK)
		{
			statusUSB2 = 1;
		}
		else
		{
			if(DirInfo_2.status == fiERR_SYSTEM && ERR_FUB_BUSY == FileIoGetSysError())
			{
				statusUSB2 = 1;
			}
			else
			{
				statusUSB2 = 0;   /*  no usb  */
				gUSB_2.DeviceOk = 0;
				gUSB_2.reserve = 0;
			}
		}
	}/* if(DirInfo_2.status != ERR_FUB_BUSY) */
	
	if(1 == statusUSB2 && 0 == statusUSB2_old)
	{
		gUSB_2.reserve = 1;
	}
	
	
   if(0 == gUSB_2.DeviceOk && 0 == gUSB_1.DeviceOk)
   {
      if(1 == gScreenShot.CmdScreenShot)
      {
         gImply.NoUSB = 1; 
         gScreenShot.CmdScreenShot = 0;
      }
   }
	/* 不在等待截屏指令状态， 如果谁发了截屏指令，无效 */
	if(gScreenShot.Status != 1)
	{
		gScreenShot.CmdScreenShot = 0;
		HideButton =1; //ipis1226
	}
	else
	{
		HideButton =0;	//ipis1226
	}

	/* 所有设备都没有准备好，就停在等待存储介质插入步 */
	if((gUSB_1.reserve == 0) && (gUSB_2.reserve == 0))
	{
		step = WAIT_USB_INSERT;
	}

	switch(step)
	{
		case INIT:				/* Init */
			gScreenShot.Status = 0;	/* 等待存储介质插入插槽 */
			step = WAIT_USB_INSERT;
			break;

		case WAIT_USB_INSERT:	/* wait for usb device insert */
			gScreenShot.Status = 0;	/* 等待存储介质插入插槽 */

			step = step;

			if((gUSB_1.reserve == 1) || (gUSB_2.reserve == 1))
			{
				step = WAIT_DIR_SCREEN_CREATE;
			}
			break;
      
      case WAIT_DIR_SCREEN_CREATE:
         if(gUSB_1.reserve == 1)
         {
            strcpy(deviceName, gUSB_1.DeviceName);
         }
         else if(gUSB_2.reserve == 1)
         {
            strcpy(deviceName, gUSB_2.DeviceName);
         }
         DirCreate_1.enable = 1;
         DirCreate_1.pDevice= (UDINT)(&deviceName);	
         DirCreate_1.pName  = (UDINT)(&dirScreenName);
         
         /* Call FBK */
         DirCreate(&DirCreate_1);

         if(ERR_OK == DirCreate_1.status || fiERR_DIR_ALREADY_EXIST == DirCreate_1.status)
         {
            DirCreate_1.enable = 0;
            step = WAIT_DIR_SINGLE_CREATE;
         }
         else if(DirCreate_1.status != ERR_FUB_BUSY)
         {
				if(0 == strcmp(deviceName, gUSB_1.DeviceName))gUSB_1.reserve = 0;
				if(0 == strcmp(deviceName, gUSB_2.DeviceName))gUSB_2.reserve = 0;
			
            DirCreate_1.enable = 0;
            step = INIT;
         }
         break;
		
      case WAIT_DIR_SINGLE_CREATE:
         if(gUSB_1.reserve == 1)
         {
            strcpy(deviceName, gUSB_1.DeviceName);
         }
         else if(gUSB_2.reserve == 1)
         {
            strcpy(deviceName, gUSB_2.DeviceName);
         }
         DirCreate_1.enable = 1;
         DirCreate_1.pDevice= (UDINT)(&deviceName);	
         DirCreate_1.pName  = (UDINT)(&dirSingleName);

         /* Call FBK */
         DirCreate(&DirCreate_1);

         if(ERR_OK == DirCreate_1.status || fiERR_DIR_ALREADY_EXIST == DirCreate_1.status)
         {
            DirCreate_1.enable = 0;
            step = WAIT_DIR_ALL_CREATE;
         } 
         else if(DirCreate_1.status != ERR_FUB_BUSY)
         {
				if(0 == strcmp(deviceName, gUSB_1.DeviceName))gUSB_1.reserve = 0;
				if(0 == strcmp(deviceName, gUSB_2.DeviceName))gUSB_2.reserve = 0;
			
            DirCreate_1.enable = 0;
            step = INIT;
         }
         break;
      
      case WAIT_DIR_ALL_CREATE:
         if(gUSB_1.reserve == 1)
         {
            strcpy(deviceName, gUSB_1.DeviceName);
         }
         else if(gUSB_2.reserve == 1)
         {
            strcpy(deviceName, gUSB_2.DeviceName);
         }
         DirCreate_1.enable = 1;
         DirCreate_1.pDevice= (UDINT)(&deviceName);	
         DirCreate_1.pName  = (UDINT)(&dirAllName);
         
         /* Call FBK */
         DirCreate(&DirCreate_1);

         if(ERR_OK == DirCreate_1.status || fiERR_DIR_ALREADY_EXIST == DirCreate_1.status)
         {
				if(0 == strcmp(deviceName, gUSB_1.DeviceName))gUSB_1.DeviceOk = 1;
				if(0 == strcmp(deviceName, gUSB_2.DeviceName))gUSB_2.DeviceOk = 1;

            DirCreate_1.enable = 0;
            step = WAIT_COMMAND;
         }
         else if(DirCreate_1.status != ERR_FUB_BUSY)
         {
				if(0 == strcmp(deviceName, gUSB_1.DeviceName))gUSB_1.reserve = 0;
				if(0 == strcmp(deviceName, gUSB_2.DeviceName))gUSB_2.reserve = 0;

            DirCreate_1.enable = 0;
            step = INIT;
         }
         break;

		case WAIT_COMMAND:		/* Wait for command */

			gScreenShot.Status = 1;	/* 等待截屏开始指令 */

			/* 允许客户按截屏开始按键 */
			gScreenShot.ScreenShotBtnStatus = gScreenShot.ScreenShotBtnStatus & (65535-2);

			if(gScreenShot.CmdScreenShot == 1)
			{
				gScreenShot.CmdScreenShot = 0;

				gScreenShot.ScreenShotBtnStatus = gScreenShot.ScreenShotBtnStatus | 2;

				/* Name format of Screen file: Year-month-day-hour-minute-second.bmp */
				DateTimeString_0.enable = 1;
				/*DateTimeString_0.Date_Mode = 2;		 Asia mode: DateYYYY-MM-DD_TimeHH-MM-SS */

            itoa((long)(gPanelObj.GetPicture),(unsigned long)NumName);
				
            strcpy(scrshotFileName,"Page_");
				
            strcat(scrshotFileName,NumName);
				
            strcat(scrshotFileName, "_");
				
            DateTimeString(&DateTimeString_0);
				
            strcat(scrshotFileName, DateTimeString_0.DisplayString);
				
            strcat(scrshotFileName, ".bmp");
	
				if(gUSB_1.DeviceOk == 1)
				{
					strcpy(deviceName, gUSB_1.DirScreenDeviceName);
				}
				else if(gUSB_2.DeviceOk == 1)
				{
					strcpy(deviceName, gUSB_2.DirScreenDeviceName);
				}
				step = INIT_SCREEN_SHOT;
			}
			break;

		case INIT_SCREEN_SHOT:	/* Init screen shot */

			gScreenShot.Status = 2;	/* 截屏中 */

			status = InitScreenShot(1, (UDINT)(&pHandle), (UDINT)(&scrshotFileName), (UDINT)(&deviceName), (UDINT)(&gPanelObj.VisualName));
			if(status == 0)
			{
				step = SCREEN_SHOT;
			}
			else
			{
				gScreenShot.errorNumber = status;
				step = ERROR_STEP;
			}
			break;

		case SCREEN_SHOT:		/* Screen shot */

			status = ScreenShot(1, pHandle);

			if(status != 1001)
			{
				if(status == 0)
				{
					step = DEINIT_SCREEN_SHOT;
				}
				else
				{
					gScreenShot.errorNumber = status;
					step = ERROR_STEP;
				}
			}
			break;

		case DEINIT_SCREEN_SHOT:/* DeInit screen shot */

			status = DeInitScreenShot(1, pHandle);

			if(status == 0)
			{
				step = WAIT_COMMAND;
			}
			else
			{
				gScreenShot.errorNumber = status;
				step = ERROR_STEP;
			}
			break;

		case ERROR_STEP:		/* Error step */

			gScreenShot.Status = 3;	/* 截屏出错 */
			step = INIT;
			break;
	}/* switch(step) */
}/* _CYCLIC void cyclic() */


