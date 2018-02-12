/*******************************************************************************
  [Author]	   : bain.wang@outlook.com.uestc@gmail.com
  [Version]    : 1.0
  [Date]       : 2017-03-14
  [Description]:

  [Others]     :

  [History]:
	 Date          Modification 							    Initials
	----------     ----------                                   -------
	2017-03-14      Created								   		bain.wang@outlook.com
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#ifndef __SNMP_TRAP_X86_H__
#define __SNMP_TRAP_X86_H__

#include "snmp_trap_comm.h"



#if 1	//暂不开放
snmp_alarm_info_t* snmp_get_trap_info();

#define SNMP_TRAP_ALARM_INFO_GET(info)\
	do{\
		info = snmp_get_trap_info();\
	} while(0)
#endif

int snmp_trap_cfg_get(snmp_alarm_cfg_t *alarm_info);
int snmp_trap_cfg_set(snmp_alarm_cfg_t alarm_info);
int snmp_trap_alarm_start(snmp_alarm_cfg_t alarm_cfg);
int snmp_trap_alarm_stop(snmp_alarm_cfg_t alarm_cfg);
int snmp_trap_alarm_reset(snmp_alarm_cfg_t alarm_cfg);

int snmp_trap_init_x86();


#endif 	/* __SNMP_TRAP_X86_H__ */
