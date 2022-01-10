/***************************************************************************************/
/*		 Include for Edge Detection											  		               */
/*-------------------------------------------------------------------------------------*/
/*		 Filename:	   edge.h												  		                  */
/*       Author:       B&R				                                      		         */
/*       Date:         24.07.2001                                             		   */
/***************************************************************************************/
/* Description:                                                            	  		   */
/*	Macros for Edgedetection. This macros uses a special feature of the GNU compiler:   */
/*	"Statements and Declarations in Expressions" see more details in GNU-Documentation  */
/*	So they may not work with other compilers.										            */
/*																					                        */
/*	EDGEPOS  Detection of positive transition of signal  							            */
/*	EDGENEG  Detection of negative transition of signal 							            */
/*	EDGE     Detection of positive and negative transition of signal 				         */
/*                                                                            		   */
/***************************************************************************************/

#ifndef __EDGE_H__
#define __EDGE_H__

/*****************************************************************************************/
/*	EDGEPOS  return 1 at positive transition of signal, in all other cases 0 is returned */
/*****************************************************************************************/
#define EDGEPOS(variable,__buffer)	\
({plcbit result; 					      \
	if (variable && !__buffer)       \
    {								         \
       	result=1;                  \
    }								         \
    else                            \
    {								         \
       	result=0;                  \
    }								         \
    __buffer=variable;              \
    result;                         \
})

/*****************************************************************************************/
/*	EDGENEG  return 1 at negative transition of signal, in all other cases 0 is returned */
/*****************************************************************************************/
#define EDGENEG(variable,__buffer) 	\
({plcbit result; 					      \
	if (!variable && __buffer)       \
    {								         \
       	result=1;                  \
    }								         \
    else                            \
    {								         \
       	result=0;                  \
    }								         \
    __buffer=variable;              \
    result;                         \
})

/**************************************************************************************************/
/*	EDGE return 1 at positive and negative transition of signal, in all other cases 0 is returned */
/**************************************************************************************************/
#define EDGE(variable,__buffer)		\
({plcbit result; 					      \
	if (variable != __buffer)        \
    {								         \
       	result=1;                  \
    }								         \
    else                            \
    {								         \
       	result=0;                  \
    }								         \
    __buffer=variable;              \
    result;                         \
})

#endif /*__EDGE_H__*/
