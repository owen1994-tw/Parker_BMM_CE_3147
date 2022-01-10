/********************************************************************
 * COPYRIGHT --  B&R
 ********************************************************************
 * Program: cfg_cpy
 * File: cfg_cpy.c
 * Author: lijianjun
 * Created: March 06, 2013
 ********************************************************************
 * Implementation of program cfg_cpy --- copy default value to data 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <string.h>

#include "edge.h"

/********************************************************************
 *                                                         Function		 								                          *
********************************************************************/

unsigned short	var_rd(var_rd_typ	*var_rd_ptr);

/********************************************************************
****	       I N I T U P  O F   T H E   T A S K 		 		 ****
********************************************************************/
void _INIT Rec_cpy_init(void)
{
	wCfgLoad =1;
}  /* end of _INIT */
/********************************************************************
****	  C Y C L I C   P A R T   O F   T H E   T A S K  		 ****
********************************************************************/
void _CYCLIC Rec_cpy_cyclic(void)
{

	/*set config load command bit, for loading*/
	if (gT_Par.Fix.AT_MAX == 0 && wCfgLoad ==0) wCfgLoad =1;

	if(1 == wCfgLoad)
	{
		memset(&cfg_cpy, 0, sizeof(cfg_cpy));
		strcpy(cfg_cpy.modname, "cfg_dat");
		cfg_cpy.error = var_rd(&cfg_cpy);

		if (cfg_cpy.error!=0)
		{
  			DMSTATUS = cfg_cpy.error;
  		}
		else
		{
			wCfgLoad = 0;
  		}
	}
}  /* end of _CYCLIC */

/********************************************************************
                                                          Function		 								                    
********************************************************************/
unsigned short	var_rd(var_rd_typ	*var_rd_ptr)
{
	
	/* Internal PV */
	float  					 value=0;
	long			    	longvalue=0;
	unsigned  long  		DA_ID;
	plcbit 			  	*bitval;                 /*    !                     */
	unsigned  char 	   *bval;                   /*    !                     */
	float 					 *fval;                   /*    !                     */
	short 					*i16val;                 /*    !                     */
	long 					*i32val;                 /*    !                     */
	char 					*i8val;                  /*    !                     */
	unsigned  long 		*lval;                    /*    !                     */
	unsigned short 		*wval;                   /*    !                     */
	char					*var_start_ptr,*full_line_ptr;
	char					*var_end_ptr;
	char					*val_start_ptr;
	char				 	*val_end_ptr;
	unsigned  long  		pv_adr;
	unsigned  long  		pv_len;
	char					    IPtyp;
	unsigned  long  	   dat_adr;
	unsigned short  	   dimension;
	long  					    PV_adr;
	char		  			  *PV_ptr;

	unsigned  long  		line_adr;
	unsigned short  		status;
	unsigned  long  		type;

	long				        end_adr,line_start_adr;
	char					* line_start_ptr;
	unsigned  long  	   length;                 /*    !                     */

	DatObjInfo_typ		   fDOInfo;

	/****************************************************************************************/
	/* Get PV initialization DM information */
	fDOInfo.enable    = 1;
	fDOInfo.pName     = (UDINT)var_rd_ptr->modname;
	DatObjInfo(&fDOInfo);
	DA_ID   = fDOInfo.ident;
	dat_adr = fDOInfo.pDatObjMem;
	length  = fDOInfo.len;
	IPtyp   = fDOInfo.MemType;
	status  = fDOInfo.status;
	if (status != 0)
	{
		strcpy(DM_ER_STR, "DatObjInfo: DM read error");
		return status;
	}
	/* Allocate memory for PV lines */
	status = TMP_alloc(100, (void*)&(line_adr));
	if (status != 0)
	{
		strcpy(DM_ER_STR, "TMP_alloc: lines memory allocate error");
		return status;
	}
	line_start_ptr 	= (char*)dat_adr;
	full_line_ptr 	= (char*)line_adr;
	/* Allocate memory for PV */
	status = TMP_alloc(100,(void*)&(PV_adr));
	if (status != 0)
	{
		strcpy(DM_ER_STR, "TMP_alloc: PV memory allocate error");
		return status;
	}
	PV_ptr = (char*)PV_adr;
	var_rd_ptr->line_no = 0;
	end_adr = dat_adr+length-3;
	line_start_adr = (long)(line_start_ptr);
	while (line_start_adr < end_adr)  /*****while (*line_start_ptr != 'e')           */
	{
		var_rd_ptr->line_no++;
		strncpy(full_line_ptr,line_start_ptr,100);
		var_start_ptr =full_line_ptr;
		while ((*var_start_ptr ==' ')||(*var_start_ptr =='"'))		/* Suche Variable Start */
		{
			var_start_ptr++;
		}
		var_end_ptr = var_start_ptr;		/* Suche Variable Ende  */
		while ((*var_end_ptr !=' ') && (*var_end_ptr !='='))
		{
			var_end_ptr++;
		}
		val_start_ptr = var_end_ptr;
		while ((*val_start_ptr ==' ') || (*val_start_ptr =='='))
		{
			val_start_ptr++;
		}
		val_end_ptr = val_start_ptr;
		while ((*val_end_ptr !=' ') && (*val_end_ptr !=0))		/* Suche Variable Start */
		{
			val_end_ptr++;
		}
		*var_end_ptr = 0;		/* Nullterminierung */
		*val_end_ptr = 0;		/* Nullterminierung */
		strcpy(PV_ptr,var_rd_ptr->taskname);	/* Zusammensetzung der Variable */
		if (strlen(var_rd_ptr->taskname)!=0)
		{
			strcat(PV_ptr, ":");
		} /*if*/
		if (strlen(var_rd_ptr->pre_string)!=0)
		{
			strcat(PV_ptr, var_rd_ptr->pre_string);
		}
		strcat(PV_ptr, var_start_ptr);
		/* Get PV address */
		status = PV_xgetadr((char*)PV_ptr, &(pv_adr), &(pv_len)) ;
		if (status != 0)
		{
			strcpy(DM_ER_STR, "PV_xgetadr: PV address get error");
			return status;
		}
		/* Get PV information */
		status = PV_ninfo((char*)PV_ptr, &(type), &(pv_len), &(dimension)) ;
		if (status != 0)
		{
			strcpy(DM_ER_STR, "PV_ninfo: PV information get error");
			return status;
		}
		line_start_ptr += strlen(line_start_ptr);
		line_start_ptr++;
		line_start_adr = (long)line_start_ptr;
		if (type == 8) /**** FLOAT )*/
		{
			value =	atof((unsigned long) (val_start_ptr));
		}
		else
		{
			longvalue = atoi((unsigned long) (val_start_ptr)); /* strtol(val_start_ptr, 0, 0); */
		}
		switch ((unsigned short)(type))
		{
			case 1: /**** BIT                                                 */
				bitval  = (plcbit *)pv_adr ;
				*bitval = (unsigned)((long)(longvalue)) ;
				break;
			case 2: /**** INT8                                                */
				i8val  = (char *)pv_adr ;
				*i8val = (char)((long)(longvalue)) ;
				break;
			case 3: /**** INT16                                               */
				i16val  = (short *)pv_adr;
				*i16val = (short)((long)(longvalue));
				break;
			case 4: /**** INT32                                               */
				i32val  = (long *)pv_adr;
				*i32val = (long)(longvalue);
				break;
			case 5: /**** BYTE                                                */
				bval  = (unsigned char *)pv_adr;
				*bval = (unsigned char)((unsigned long)(longvalue));
				break;
			case 6: /**** WORD                                                */
				wval  = (unsigned short *)pv_adr ;
				*wval = (unsigned short)((unsigned long)(longvalue));
				break;
			case 7: /**** LONG                                                */
				lval  = (unsigned long *)pv_adr ;
				*lval = (unsigned long)(longvalue);
				break;
			case 8: /**** FLOAT                                               */
				fval  = (float *)pv_adr;
				*fval = value;
				break;
			default:
				break;
		} /* End of switch */
	} /* End of while (line_start_adr < end_adr) */
	/* Free memory for PV */
	status = TMP_free(100, (void*)PV_ptr);
	if (status != 0)
	{
		strcpy(DM_ER_STR, "TMP_free: PV memory free error");
		return status;
	}
	/* Free memory for PV lines */
	status = TMP_free(100, (void*)line_adr);
	if (status != 0)
	{
		strcpy(DM_ER_STR, "TMP_free: line memory free error");
		return status;
	}

	return status;
} /* End of var_rd function */
