/********************************************************************
 * COPYRIGHT --  B&R China
 ********************************************************************
 * Program: vpn
 * File: vpn.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program vpn --- ip address control 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <string.h>

//#include "keyap880.h"
#include "keyap2100.h"
#include "pic_id.h"

char substring(char *dst,char *src,int start,int len);

/********************************************************************************************************/
/*  Others:
	vpn.FlagManualSetIP：0=自动获取IP地址类型，1=手动设置IP地址
	vpn存放当前IP地址，子网掩码，网关地址等，以及手动设置IP地址，子网掩码，网关地址等参数，

		typedef struct
		{
			BOOL			FlagManualSetIP;		* 标志位：1 = 手动设定IP地址 *
			BOOL			CmdSetIP;				* 设置IP地址指令 *
			BOOL			CmdSetSubnetMark;		* 设置子网掩码指令 *
			BOOL			CmdSetGateway;			* 设置网关地址指令 *

			STRING			CurrentIPString[16];	* 当前IP地址 *
			STRING			CurrentSubnetMarkString[16];* 当前子网掩码 *
			STRING			CurrentGatewayString[16];* 当前网关地址 *

			Address_typ		ParaSetIP;				* 手动设置IP地址模式下，IP地址参数 *
			Address_typ		ParaSetSubnetMark;		* 手动设置IP地址模式下，子网掩码参数 *
			Address_typ		ParaSetGateway;			* 手动设置IP地址模式下，网关地址参数 *

		}VPN_typ;

*********************************************************************************************************/


/**********************************************************************************
**                          宏定义												 **
**********************************************************************************/
#define INIT				       0
#define GET_ETH_CFG_MODE	   10
#define GET_ETH_CFG_ADR	      20
#define GET_ETH_CFG_SUBNET	   30
#define GET_ETH_CFG_GATEWAY 	40

#define WAIT_COMMAND          100
#define SET_ETH_CFG_MODE		110
#define SET_ETH_CFG_ADR	      120
#define SET_ETH_CFG_SUBNET 	130
#define SET_ETH_CFG_GATEWAY   140
#define ERROR_STEP            9999

/**********************************************************************************
**     	        		I N I T U P  O F   T H E   T A S K 		 				 **
**********************************************************************************/
_INIT void init()
{
	/*------------- 编程人员根据实际需要修改的地方 开始 -----------------*/

	strcpy(vpnDevice,"IF3");	/*指定VPN使用的端口*/

	/*------------- 编程人员根据实际需要修改的地方 结束 -----------------*/
	step = INIT;
	
	bGetCurrentIP_USINT = 1;

}/* _INIT void init() */

/**********************************************************************************
**        			C Y C L I C   P A R T   O F   T H E   T A S K  				 **
**********************************************************************************/
_CYCLIC void cyclic()
{
	if(gPanelObj.GetPicture == PIC_IP_CONFIG && step == WAIT_COMMAND)
	{
		F5_bHiden = 0;
	}
	else
	{
		F5_bHiden = 1;
	}

	/*-------------- 判断手自动模式是否切换了 -------------------------*/
	R_TRIG_KeyF5.CLK = gPanelObj.KeyMatric[S$KEYMATRIX_F5];
		R_TRIG(&R_TRIG_KeyF5);

	/*if(1 == R_TRIG_KeyF5.Q)
	{
		if(gPanelObj.GetPicture == PIC_IP_CONFIG)
		{
			if(0 == F5_bHiden)
			{
				if( cfgCONFIGMODE_MANUALLY == vpn.CurrentConfigMode)
				{
					vpn.ParaSetConfigMode = cfgCONFIGMODE_DHCPCLIENT;
				}
				else
				{
					vpn.ParaSetConfigMode = cfgCONFIGMODE_MANUALLY;
				}
				vpn.CmdSetMode = 1;
			}
		}
	}*//* if(1 == R_TRIG_KeyF5.Q) */

	vpn.ParaSetConfigMode = cfgCONFIGMODE_MANUALLY;

	switch(step)
	{
		case INIT:				/*初始化*/
			step = GET_ETH_CFG_MODE;
			break;

		case GET_ETH_CFG_MODE:	/*读当前配置模式*/
			CfgGetEthConfigMode_0.pDevice = (UDINT)&vpnDevice[0];
			CfgGetEthConfigMode_0.enable  = 1;

			CfgGetEthConfigMode(&CfgGetEthConfigMode_0);

			if(ERR_FUB_BUSY != CfgGetEthConfigMode_0.status)
			{
				if(ERR_OK == CfgGetEthConfigMode_0.status)
				{
					if(cfgCONFIGMODE_MANUALLY == CfgGetEthConfigMode_0.ConfigMode)
					{
						vpn.FlagManualSetIP = 1;
						step = GET_ETH_CFG_ADR;
					}
					else if(cfgCONFIGMODE_DHCPCLIENT == CfgGetEthConfigMode_0.ConfigMode)
					{
						vpn.FlagManualSetIP = 0;
						step = GET_ETH_CFG_ADR;
					}
					vpn.CurrentConfigMode = CfgGetEthConfigMode_0.ConfigMode;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case GET_ETH_CFG_ADR:		/*读当前IP地址*/
			CfgGetIPAddr_0.pDevice = (UDINT)&vpnDevice[0];
			CfgGetIPAddr_0.pIPAddr = (UDINT)&vpn.CurrentIPString[0];
			CfgGetIPAddr_0.Len     = 16;
			CfgGetIPAddr_0.enable  = 1;

			CfgGetIPAddr(&CfgGetIPAddr_0);

			if(ERR_FUB_BUSY != CfgGetIPAddr_0.status)
			{
				if(ERR_OK == CfgGetIPAddr_0.status)
				{
					step = GET_ETH_CFG_SUBNET;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case GET_ETH_CFG_SUBNET:		/*读当前子网掩码*/
			CfgGetSubnetMask_0.pDevice = (UDINT)&vpnDevice[0];
			CfgGetSubnetMask_0.pSubnetMask = (UDINT)&vpn.CurrentSubnetMarkString[0];
			CfgGetSubnetMask_0.Len     = 16;
			CfgGetSubnetMask_0.enable  = 1;

			CfgGetSubnetMask(&CfgGetSubnetMask_0);

			if(ERR_FUB_BUSY != CfgGetSubnetMask_0.status)
			{
				if(ERR_OK == CfgGetSubnetMask_0.status)
				{
					step = GET_ETH_CFG_GATEWAY;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case GET_ETH_CFG_GATEWAY:		/*读当前默认网关*/
			CfgGetDefaultGateway_0.pDevice = (UDINT)&vpnDevice[0];
			CfgGetDefaultGateway_0.pGateway= (UDINT)&vpn.CurrentGatewayString[0];
			CfgGetDefaultGateway_0.Len     = 16;
			CfgGetDefaultGateway_0.enable  = 1;

			CfgGetDefaultGateway(&CfgGetDefaultGateway_0);

			if(ERR_FUB_BUSY != CfgGetDefaultGateway_0.status)
			{
				if(ERR_OK == CfgGetDefaultGateway_0.status)
				{
					step = WAIT_COMMAND;
				}
				else
				{
					if(cfgERR_PARAM_NOT_SET == CfgGetDefaultGateway_0.status)
					{
						step = WAIT_COMMAND;
					}
					else
					{
						step = ERROR_STEP;
					}
				}
			}
			break;

		case WAIT_COMMAND:
			if(0 == vpn.FlagManualSetIP)
			{
				vpn.CmdSetIP         = 0;
				vpn.CmdSetSubnetMark = 0;
				vpn.CmdSetGateway    = 0;

				if(1 == vpn.CmdSetMode)
				{
					vpn.CmdSetMode = 0;
					step = SET_ETH_CFG_MODE;
				}
				else
				{
					step = GET_ETH_CFG_MODE;
				}
			}
			else
			{
				if(1 == vpn.CmdSetMode)
				{
					vpn.CmdSetMode = 0;
					step = SET_ETH_CFG_MODE;
				}
				else
				{
					if(1 == vpn.CmdSetIP)
					{
						vpn.CmdSetIP = 0;
						step = SET_ETH_CFG_ADR;
					}
					else if(1 == vpn.CmdSetSubnetMark)
					{
						vpn.CmdSetSubnetMark = 0;
						step = SET_ETH_CFG_SUBNET;
					}
					else if(1 == vpn.CmdSetGateway)
					{
						vpn.CmdSetGateway = 0;
						step = SET_ETH_CFG_GATEWAY;
					}
				}/* if(1 == vpn.CmdSetMode) */
			}/* if(0 == vpn.FlagManualSetIP) */
			break;

		case SET_ETH_CFG_MODE:		/*设定配置模式*/
			CfgSetEthConfigMode_0.pDevice = (UDINT)&vpnDevice[0];

			CfgSetEthConfigMode_0.ConfigMode = vpn.ParaSetConfigMode;
			CfgSetEthConfigMode_0.Option  = cfgOPTION_VOLATILE;

			CfgSetEthConfigMode_0.enable  = 1;

			CfgSetEthConfigMode(&CfgSetEthConfigMode_0);

			if(ERR_FUB_BUSY != CfgSetEthConfigMode_0.status)
			{
				if(ERR_OK == CfgSetEthConfigMode_0.status)
				{
					step = GET_ETH_CFG_MODE;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case SET_ETH_CFG_ADR:		/*设定IP*/
			itoa(vpn.ParaSetIP.Address3, (UDINT)&tempString);
			strcpy(setIPString, tempString);

			strcat(setIPString, ".");

			itoa(vpn.ParaSetIP.Address2, (UDINT)&tempString);
			strcat(setIPString, tempString);

			strcat(setIPString, ".");

			itoa(vpn.ParaSetIP.Address1, (UDINT)&tempString);
			strcat(setIPString, tempString);

			strcat(setIPString, ".");

			itoa(vpn.ParaSetIP.Address0, (UDINT)&tempString);
			strcat(setIPString, tempString);

			CfgSetIPAddr_0.pDevice = (UDINT)&vpnDevice[0];
			CfgSetIPAddr_0.pIPAddr = (UDINT)&setIPString[0];
			CfgSetIPAddr_0.Option  = cfgOPTION_NON_VOLATILE;
			CfgSetIPAddr_0.enable  = 1;

			CfgSetIPAddr(&CfgSetIPAddr_0);

			if(ERR_FUB_BUSY != CfgSetIPAddr_0.status)
			{
				if(ERR_OK == CfgSetIPAddr_0.status)
				{
					step = GET_ETH_CFG_MODE;
					
					/*update fix*/
					gUserFix.ProjectInfo.Address3 = vpn.ParaSetIP.Address3; 
					gUserFix.ProjectInfo.Address2 = vpn.ParaSetIP.Address2; 
					gUserFix.ProjectInfo.Address1 = vpn.ParaSetIP.Address1; 
					gUserFix.ProjectInfo.Address0 = vpn.ParaSetIP.Address0; 
					
					gRecipeOperate.OnceSave = 1;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case SET_ETH_CFG_SUBNET:		/*设定子网掩码*/
			itoa(vpn.ParaSetSubnetMark.Address3, (UDINT)&tempString);
			strcpy(setSubnetMarkString, tempString);

			strcat(setSubnetMarkString, ".");

			itoa(vpn.ParaSetSubnetMark.Address2, (UDINT)&tempString);
			strcat(setSubnetMarkString, tempString);

			strcat(setSubnetMarkString, ".");

			itoa(vpn.ParaSetSubnetMark.Address1, (UDINT)&tempString);
			strcat(setSubnetMarkString, tempString);

			strcat(setSubnetMarkString, ".");

			itoa(vpn.ParaSetSubnetMark.Address0, (UDINT)&tempString);
			strcat(setSubnetMarkString, tempString);


			CfgSetSubnetMask_0.pDevice = (UDINT)&vpnDevice[0];
			CfgSetSubnetMask_0.pSubnetMask = (UDINT)&setSubnetMarkString[0];
			CfgSetSubnetMask_0.Option  = cfgOPTION_NON_VOLATILE;
			CfgSetSubnetMask_0.enable  = 1;

			CfgSetSubnetMask(&CfgSetSubnetMask_0);

			if(ERR_FUB_BUSY != CfgSetSubnetMask_0.status)
			{
				if(ERR_OK == CfgSetSubnetMask_0.status)
				{
					step = GET_ETH_CFG_MODE;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case SET_ETH_CFG_GATEWAY:		/*设定网关*/
			itoa(vpn.ParaSetGateway.Address3, (UDINT)&tempString);
			strcpy(setGatewayString, tempString);

			strcat(setGatewayString, ".");

			itoa(vpn.ParaSetGateway.Address2, (UDINT)&tempString);
			strcat(setGatewayString, tempString);

			strcat(setGatewayString, ".");

			itoa(vpn.ParaSetGateway.Address1, (UDINT)&tempString);
			strcat(setGatewayString, tempString);

			strcat(setGatewayString, ".");

			itoa(vpn.ParaSetGateway.Address0, (UDINT)&tempString);
			strcat(setGatewayString, tempString);

			CfgSetDefaultGateway_0.pDevice = (UDINT)&vpnDevice[0];
			CfgSetDefaultGateway_0.pGateway = (UDINT)&setGatewayString[0];
			CfgSetDefaultGateway_0.Option  = cfgOPTION_NON_VOLATILE;
			CfgSetDefaultGateway_0.enable  = 1;

			CfgSetDefaultGateway(&CfgSetDefaultGateway_0);

			if(ERR_FUB_BUSY != CfgSetDefaultGateway_0.status)
			{
				if(ERR_OK == CfgSetDefaultGateway_0.status)
				{
					step = GET_ETH_CFG_MODE;
				}
				else
				{
					step = ERROR_STEP;
				}
			}
			break;

		case ERROR_STEP:	/*error step*/
			step = 0;
			break;

		default:
			break;
	}/* switch(step) */
	
	/*Get config ip*/
	if(bGetCurrentIP_USINT)
	{
		bGetCurrentIP_USINT = 0;
		Part_num_count = 0;
		Char_num = 0;
		old_Char_num = 0;
		for (ii = 0; ii < sizeof(vpn.CurrentIPString); ii++)
		{
			Charr = (char*)(vpn.CurrentIPString[ii]);
			
			if( Charr == 46)
			{
				Char_num = ii;
				
				switch (Part_num_count)
				{
					case 0:
						substring(sTempAddress3,vpn.CurrentIPString,0,Char_num);
						old_Char_num = Char_num;
						break;
					case 1:
						substring(sTempAddress2,vpn.CurrentIPString,old_Char_num+1,Char_num-old_Char_num-1);
						old_Char_num = Char_num;
						break;
					case 2:
						substring(sTempAddress1,vpn.CurrentIPString,old_Char_num+1,Char_num-old_Char_num-1);
						old_Char_num = Char_num;
						break;
				}
				Part_num_count++;
			}
			if(Part_num_count == 3)
			{
				substring(sTempAddress0,vpn.CurrentIPString,old_Char_num+1,sizeof(vpn.CurrentIPString)-old_Char_num);
			}
	
		}

		uConfigAddress3 = atoi((UDINT) &sTempAddress3); 
		uConfigAddress2 = atoi((UDINT) &sTempAddress2); 
		uConfigAddress1 = atoi((UDINT) &sTempAddress1); 
		uConfigAddress0 = atoi((UDINT) &sTempAddress0); 
			
		/*check current vs fix (if different than excute set ip cmd)*/
		if(gUserFix.ProjectInfo.Address0 != uConfigAddress0 
			|| gUserFix.ProjectInfo.Address1 != uConfigAddress1
			|| gUserFix.ProjectInfo.Address2 != uConfigAddress2
			|| gUserFix.ProjectInfo.Address3 != uConfigAddress3
			)
		{
			vpn.ParaSetIP.Address0 = gUserFix.ProjectInfo.Address0; 
			vpn.ParaSetIP.Address1 = gUserFix.ProjectInfo.Address1;  
			vpn.ParaSetIP.Address2 = gUserFix.ProjectInfo.Address2; 
			vpn.ParaSetIP.Address3 = gUserFix.ProjectInfo.Address3;  
		
			vpn.CmdSetIP = 1;
		}
	}
}/* _CYCLIC void cyclic() */

char substring(char *dst,char *src,int start,int len)
{  
	char *p=dst;  
	char *q=src;  
	int length=strlen(src); 
	if(start>=length||start<0) 
		return NULL;  
	if(len>length) 
		len=length-start; 
	q+=start;
	while(len--)  
	{   
		*(p++)=*(q++); 
	}  
	*(p++)='\0';  
	return dst;
}
