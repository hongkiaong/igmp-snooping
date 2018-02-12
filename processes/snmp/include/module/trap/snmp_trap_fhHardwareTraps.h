/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */
#ifndef FHHARDWARETRAPS_H
#define FHHARDWARETRAPS_H

#include "snmp_trap_comm.h"

/* function declarations */
int send_fhBoardNotExist_trap(snmp_alarm_info_t alarm_info);
int send_fhBoardFault_trap(snmp_alarm_info_t alarm_info);
int send_fhIntfPktLossExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhBoardTempExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhMemoryUsageExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhCPUUsageExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhPowerSupplyShortage_trap(snmp_alarm_info_t alarm_info);
int send_fhLaserSendFault_trap(snmp_alarm_info_t alarm_info);
int send_fhLaserTempExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhPowerOutShortage_trap(snmp_alarm_info_t alarm_info);
int send_fhPowerOutExceed_trap(snmp_alarm_info_t alarm_info);
int send_fhPowerInShortage_trap(snmp_alarm_info_t alarm_info);
int send_fhPowerInExceed_trap(snmp_alarm_info_t alarm_info);

#if SNMP_PRJ_NAME_610A
#include "mBase.h"
#include "event/event_eth_port.h"
#include "mpapi.h"
#include "comm/lr_errors.h"

#endif

#endif /* FHHARDWARETRAPS_H */
