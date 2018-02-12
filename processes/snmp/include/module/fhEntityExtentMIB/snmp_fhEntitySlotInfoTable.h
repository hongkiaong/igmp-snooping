/*******************************************************************************
  [Author]	   : bain.wang@outlook.com.uestc@gmail.com
  [Version]    : 1.0
  [Date]       : 2017-06-11
  [Description]:

  [Others]     :

  [Function List]:  
    1. ....
    2. ....
  [History]:
	 D a t e    Modification 										  Initials
	---------- --------------                                        ----------
	2016-06-03  Created												  bain.wang@outlook.com
*******************************************************************************/
#ifndef __SNMP_FHENTITYSLOTINFOTABLE_H__
#define __SNMP_FHENTITYSLOTINFOTABLE_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "bc_common_defs.h"
#include "snmp_global.h"
#include "snmp_core_ex.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								宏   定   义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#define FHENTITYSLOTINFOTABLE_INDEX_NUM			1
#define FHENTITYSLOTINFOTABLE_OID_LEN				11		/*表的OID个数*/
#define FHENTITYSLOTINFOTABLE_OID_NODE_LEN		(FHENTITYSLOTINFOTABLE_OID_LEN+2)/*叶子结点OID个数*/
#define FHENTITYSLOTINFOTABLE_OID_FULL_LEN		(FHENTITYSLOTINFOTABLE_OID_NODE_LEN+FHENTITYSLOTINFOTABLE_INDEX_NUM)/*完整结点OID个数*/

/* column number definitions for table fhEntitySlotInfoTable */
       #define COLUMN_FHSLOTENTITYINDEX		1
       #define COLUMN_FHSLOTSEQ				2
       #define COLUMN_FHSLOTTYPE				3
       #define COLUMN_FHSLOTNAME				4
       #define COLUMN_FHSLOTOCCUPYSTATUS	5




////////////////////////////////////////////////////////////////////////////////
//																			  //
//					结 构 体 、联 合 体、枚 举 定 义						  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
/*
 * Typical data structure for a row entry 
 */
struct fhEntitySlotInfoTable_entry
{
    /*
     * Index values 
     */
    long            fhSlotEntityIndex;

    /*
     * Column values 
     */
    long fhSlotSeq;
    char fhSlotType[SNMP_DEV_NAME_LEN];
    char fhSlotName[SNMP_DEV_NAME_LEN];
    char fhSlotOccupyStatus[SNMP_DEV_NAME_LEN];

	time_t			update_time;/*2017.02.20*/
    /*
     * Illustrate using a simple linked list 
     */
    int             storageType;
};


////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 声 明							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
void snmp_fhEntitySlotInfoTable_init(void);

#endif /* __SNMP_FHENTITYSLOTINFOTABLE_H__ */

