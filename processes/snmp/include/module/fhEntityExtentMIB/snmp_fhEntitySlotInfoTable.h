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
//								ͷ   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "bc_common_defs.h"
#include "snmp_global.h"
#include "snmp_core_ex.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								��   ��   ��								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#define FHENTITYSLOTINFOTABLE_INDEX_NUM			1
#define FHENTITYSLOTINFOTABLE_OID_LEN				11		/*����OID����*/
#define FHENTITYSLOTINFOTABLE_OID_NODE_LEN		(FHENTITYSLOTINFOTABLE_OID_LEN+2)/*Ҷ�ӽ��OID����*/
#define FHENTITYSLOTINFOTABLE_OID_FULL_LEN		(FHENTITYSLOTINFOTABLE_OID_NODE_LEN+FHENTITYSLOTINFOTABLE_INDEX_NUM)/*�������OID����*/

/* column number definitions for table fhEntitySlotInfoTable */
       #define COLUMN_FHSLOTENTITYINDEX		1
       #define COLUMN_FHSLOTSEQ				2
       #define COLUMN_FHSLOTTYPE				3
       #define COLUMN_FHSLOTNAME				4
       #define COLUMN_FHSLOTOCCUPYSTATUS	5




////////////////////////////////////////////////////////////////////////////////
//																			  //
//					�� �� �� ���� �� �塢ö �� �� ��						  //
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
//						�� �� �� ���� �� �� �� ��							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
void snmp_fhEntitySlotInfoTable_init(void);

#endif /* __SNMP_FHENTITYSLOTINFOTABLE_H__ */
