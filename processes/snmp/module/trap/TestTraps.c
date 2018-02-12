/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "TestTraps.h"

#include "bc_print.h"

#if 0

//extern const oid snmptrap_oid[] = {1,3,6,1,6,3,1,1,4,1,0};
//extern const size_t snmptrap_oid_len = OID_LENGTH(snmptrap_oid);

oid snmptrap_oid[] = {1,3,6,1,6,3,1,1,4,1,0};
size_t snmptrap_oid_len = OID_LENGTH(snmptrap_oid);

void judge_send_cputrap(int cpu);

void read_cpudata_repeat(unsigned int clientreg, void *clientarg)  
{  
    int cpu = 90;  
    judge_send_cputrap(cpu);  
}


void init_TestTraps(void)  
{  
    DEBUGMSGTL(("TestTraps","Initializing\n"));  
    snmp_alarm_register(1,SA_REPEAT,read_cpudata_repeat, NULL);  
}

void
send_cpuRatioHigh_trap(unsigned int clientreg, void *clientarg)
{
    netsnmp_variable_list  *var_list = NULL;
    const oid cpuRatioHigh_oid[] = { 1,3,6,1,4,1,1000,1,1 };
 //   const oid SystemTrapDescription_oid[] = { , /* insert index here */ };
	const oid TestTrapDescription_oid[] = { 1,3,6,1,4,1,1000,1,2,1, 0 };

	static char TestTrapDescription[50];
	strcpy(TestTrapDescription, "CPU使用率过高");

	BC_PT_FUN_ENTER(BC_MODULE_SNMP);
	
    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        cpuRatioHigh_oid, sizeof(cpuRatioHigh_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        TestTrapDescription_oid, OID_LENGTH(TestTrapDescription_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for SystemTrapDescription */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "send_v2trap begin....\n");
    send_v2trap( var_list );
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "send_v2trap finished\n\n");
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}

void judge_send_cputrap(int cpu)  
{  
    static unsigned int cputrap_clientreg = 0;  
	unsigned int clientreg;
	void *clientarg;

    if(cpu > 80)  
    {  
        if(cputrap_clientreg == 0){  
            send_cpuRatioHigh_trap(clientreg, clientarg);  
            cputrap_clientreg = snmp_alarm_register(5,SA_REPEAT,send_cpuRatioHigh_trap,NULL);  
  
        }  
    }  
    else  
    {  
        if(cputrap_clientreg != 0)  
        {  
            snmp_alarm_unregister(cputrap_clientreg);  
            cputrap_clientreg = 0;  
        }  
    }  
}  

#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "TestTraps.h"

extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

int
__send_testAlarmNotification_trap(int b)
{
    netsnmp_variable_list  *var_list = NULL;
    const oid testAlarmNotification_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,1,1 };
    const oid testTrapSequenceNumber_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,7, 0 };
    const oid testTrapOccurTime_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,6, 0 };
    const oid testTrapCode_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,4, 0 };
    const oid testTrapInstance_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,1, 0 };
    const oid testTrapSeverity_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,5, 0 };
    const oid testTrapCorrelationId_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,2, 0 };
    const oid testTrapAdditionalText_oid[] = { 1,3,6,1,4,1,17409,2,2,11,1,2,3, 0 };

	struct timeval tv;
	struct timezone tz;

	int seqnum = 1234;
	time_t frst_occre_time;

	int testTrapCorrelationId = 2;
//	oid 		enterpriese_oid[]={1,3,6,1,4,1,17409,2,3,63818};
	int severity = 1;
	int code = 0x0f;
	bc_uint8	activeAlarmInstance_val[8];

	char testTrapAdditionalText[128];
	strcpy(testTrapAdditionalText, "testTrapAdditionalText");

	printf("-----------------------------------b = %d----------------------------------------\n", b);
	printf("-----------%d\n", __LINE__);
	
	activeAlarmInstance_val[0] = 1;
	activeAlarmInstance_val[1] = 2;
	activeAlarmInstance_val[2] = 3;
	activeAlarmInstance_val[3] = 5;	
	gettimeofday(&tv, &tz);
	frst_occre_time = tv.tv_sec;
	printf("-----------%d\n", __LINE__);
    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, snmptrap_oid_len,
        ASN_OBJECT_ID,
        (u_char *)testAlarmNotification_oid, sizeof(testAlarmNotification_oid));
    printf("-----------%d\n", __LINE__);
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        testTrapSequenceNumber_oid, OID_LENGTH(testTrapSequenceNumber_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for testTrapSequenceNumber */
        (u_char *)&seqnum, sizeof(seqnum));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapOccurTime_oid, OID_LENGTH(testTrapOccurTime_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for testTrapOccurTime */
       (u_char *)&frst_occre_time, sizeof(frst_occre_time));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapCode_oid, OID_LENGTH(testTrapCode_oid),
        ASN_INTEGER,
        /* Set an appropriate value for testTrapCode */
       (u_char *)& code, sizeof(code));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapInstance_oid, OID_LENGTH(testTrapInstance_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for testTrapInstance */
       (u_char *)activeAlarmInstance_val, sizeof(activeAlarmInstance_val));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapSeverity_oid, OID_LENGTH(testTrapSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for testTrapSeverity */
        (u_char *)&severity, sizeof(severity));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapCorrelationId_oid, OID_LENGTH(testTrapCorrelationId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for testTrapCorrelationId */
        (u_char *)&testTrapCorrelationId, sizeof(testTrapCorrelationId));
	printf("-----------%d\n", __LINE__);
    snmp_varlist_add_variable(&var_list,
        testTrapAdditionalText_oid, OID_LENGTH(testTrapAdditionalText_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for testTrapAdditionalText */
        (u_char *)testTrapAdditionalText, strlen(testTrapAdditionalText));
	printf("-----------%d\n", __LINE__);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
	printf("-----------%d\n", __LINE__);
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}

int get_a(int *a)
{
	*a = 11;
	
	return 1;
}
void
send_testAlarmNotification_trap(unsigned int clientreg, void *clientarg)
{
	int a = 0;

	get_a(&a);
	__send_testAlarmNotification_trap(a);
}
#if 1

//extern const oid snmptrap_oid[] = {1,3,6,1,6,3,1,1,4,1,0};
//extern const size_t snmptrap_oid_len = OID_LENGTH(snmptrap_oid);

void judge_send_cputrap(int cpu);

void read_cpudata_repeat(unsigned int clientreg, void *clientarg)  
{  
    int cpu = 90;  
	printf("-----------%d\n", __LINE__);
    judge_send_cputrap(cpu);  
	printf("-----------%d\n", __LINE__);
}


void init_TestTraps(void)  
{  
//    DEBUGMSGTL(("TestTraps","Initializing\n"));  
//    snmp_alarm_register(1,SA_REPEAT,read_cpudata_repeat, NULL);  
	snmp_alarm_register(5,SA_REPEAT,send_testAlarmNotification_trap,NULL);
}

#endif


void judge_send_cputrap(int cpu)  
{  
    static unsigned int cputrap_clientreg = 0;  
	unsigned int clientreg;
	void *clientarg;

printf("-----------%d\n", __LINE__);
    if(cpu > 80)  
    {  
        if(cputrap_clientreg == 0){  
            send_testAlarmNotification_trap(clientreg, clientarg);  
            cputrap_clientreg = snmp_alarm_register(5,SA_REPEAT,send_testAlarmNotification_trap,NULL);  
			printf("-----------%d\n", __LINE__);
  
        }  
    }  
    else  
    {  
        if(cputrap_clientreg != 0)  
        {  
            snmp_alarm_unregister(cputrap_clientreg);  
            cputrap_clientreg = 0;  
        }  
    }  
}  
