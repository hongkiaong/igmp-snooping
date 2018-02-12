/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "snmp_trap_fhHardwareTraps.h"
#include "snmp_trap_comm.h"
#include "snmp_global.h"
#include "snmp_core_ex.h"


extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

int
send_fhBoardNotExist_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhBoardNotExist_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,1,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1, };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5,  };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6,  };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,  };


    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhBoardNotExist_oid, sizeof(fhBoardNotExist_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhBoardFault_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhBoardFault_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,2,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5,  };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6,  };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };


    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhBoardFault_oid, sizeof(fhBoardFault_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhIntfPktLossExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhIntfPktLossExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,3,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1, };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4, };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,};

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhIntfPktLossExceed_oid, sizeof(fhIntfPktLossExceed_oid));
    
    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));


    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhBoardTempExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhBoardTempExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,4,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5, };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };


    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhBoardTempExceed_oid, sizeof(fhBoardTempExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhMemoryUsageExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhMemoryUsageExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,5,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5,  };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };


    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhMemoryUsageExceed_oid, sizeof(fhMemoryUsageExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhCPUUsageExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhCPUUsageExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,7,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5, };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6,  };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhCPUUsageExceed_oid, sizeof(fhCPUUsageExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhPowerSupplyShortage_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhPowerSupplyShortage_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,10,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhBoardType_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,5, };
    const oid fhBoardStatus_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,6,  };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,};

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhPowerSupplyShortage_oid, sizeof(fhPowerSupplyShortage_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardType_oid, OID_LENGTH(fhBoardType_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_type, sizeof(alarm_info.board_info.board_base_info.board_type));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardStatus_oid, OID_LENGTH(fhBoardStatus_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.board_status, sizeof(alarm_info.board_info.board_base_info.board_status));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhLaserSendFault_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhLaserSendFault_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,14,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1, };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,  };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4,  };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,  };

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhLaserSendFault_oid, sizeof(fhLaserSendFault_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
       (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhLaserTempExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhLaserTempExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,15,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,  };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4,  };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,
		};

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhLaserTempExceed_oid, sizeof(fhLaserTempExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhPowerOutShortage_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhPowerOutShortage_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,16,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2,};
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4, };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,  };

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhPowerOutShortage_oid, sizeof(fhPowerOutShortage_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhPowerOutExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhPowerOutExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,17,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1, };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3, };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4, };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,};

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhPowerOutExceed_oid, sizeof(fhPowerOutExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
       (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhPowerInShortage_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhPowerInShortage_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,18,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1,  };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2,  };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,  };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4, };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8,  };

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhPowerInShortage_oid, sizeof(fhPowerInShortage_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));


    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_fhPowerInExceed_trap( snmp_alarm_info_t alarm_info )
{
    netsnmp_variable_list  *var_list = NULL;
    const oid fhPowerInExceed_oid[] = { 1,3,6,1,4,1,11408,11,1,1,5,19,1 };
    const oid fhSlotNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,1, };
    const oid fhSubSlotNO_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,2, };
    const oid fhBoardName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,3,  };
    const oid fhIfName_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,4, };
    const oid fhPortNo_oid[] = { 1,3,6,1,4,1,11408,11,1,1,1,5,1,8, };

    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        fhPowerInExceed_oid, sizeof(fhPowerInExceed_oid));

    snmp_varlist_add_variable(&var_list,
        fhSlotNo_oid, OID_LENGTH(fhSlotNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.slot_id, sizeof(alarm_info.board_info.board_base_info.slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhSubSlotNO_oid, OID_LENGTH(fhSubSlotNO_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.board_base_info.s_slot_id, sizeof(alarm_info.board_info.board_base_info.s_slot_id));
	
    snmp_varlist_add_variable(&var_list,
        fhBoardName_oid, OID_LENGTH(fhBoardName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.board_base_info.name, strlen(alarm_info.board_info.board_base_info.name));
	
    snmp_varlist_add_variable(&var_list,
        fhIfName_oid, OID_LENGTH(fhIfName_oid),
        ASN_OCTET_STR,
        (u_char *)alarm_info.board_info.ifname, strlen(alarm_info.board_info.ifname));
	
    snmp_varlist_add_variable(&var_list,
        fhPortNo_oid, OID_LENGTH(fhPortNo_oid),
        ASN_INTEGER,
        (u_char *)&alarm_info.board_info.port_id, sizeof(alarm_info.board_info.port_id));

    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}

#if SNMP_PRJ_NAME_610A
#include "mBase.h"
#include "event/event_eth_port.h"
#include "mpapi.h"
#include "comm/lr_errors.h"

#endif