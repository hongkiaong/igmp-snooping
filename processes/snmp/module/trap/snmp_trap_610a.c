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
//								?   ??   ??								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/header_complex.h>


#include "snmp_trap_fhIntfTraps.h"
#include "snmp_global.h"
#include "snmp_core_ex.h"

#include "snmp_trap_fhHardwareTraps.h"
#include "snmp_trap_fhIntfTraps.h"
#include "snmp_trap_fhVCTraps.h"
#include "snmp_trap_610a.h"
#include "snmp_trap_comm.h"
#include <pthread.h>
#include <semaphore.h>

#include "mBase.h"
#include "event/event_eth_port.h"
#include "mpapi.h"
#include "comm/lr_errors.h"

#if 0
typedef enum {
	EVENT_DEV_BOARD = 1,        /**< 板类相关事件   */
	EVENT_DEV_CLOCK,            /**< 时钟类相关事件 */
	EVENT_DEV_PTP,              /**< 时钟同步类事件 */
	EVENT_DEV_INTERFACE,        /**< 接口事件 */  
	EVENT_DEV_PORT,             /**< 端口事件 */  
	EVENT_MPLS,                 /**< MPLS 事件  */
	EVENT_E1,
	EVENT_SDH,
	EVENT_ETH_OAM,

	__MAX_EVENT_TYPE          
}event_type_t;
#endif

#define ALARM_EVENT_PROC_GET(t, hr)	\
	do{	\
		int _x_ = 0;	\
		for(_x_=0; _x_<snmp_alarm_event_info_count; _x_++){	\
			if(snmp_alarm_events[_x_].type==t){	\
				hr = snmp_alarm_events[_x_].hr;	\
				break;	\
			}	\
		}	\
	}while(0)
	
#if 1
/*????μ????*/
#define SNMP_ALARM_DECLARE_EVENT(vars)	\
	struct event_collect _collects_[API_ALARM_TYPE_DEF_MAX]; \
	snmp_alarm_event_info_t vars[] = {	\
		{"BOARD", _collects_[EVENT_DEV_BOARD], EVENT_DEV_BOARD, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		/*BOARD_CARD_ABSENT|BOARD_FAILD|BOARD_CARD_TCT*/},	\
		\
		{"CLOCK", _collects_[EVENT_DEV_CLOCK], EVENT_DEV_CLOCK, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		}, \
		\
		{"PTP", _collects_[EVENT_DEV_PTP], EVENT_DEV_PTP, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF,  \
		}, \
		\
		{"INTERFACE", _collects_[EVENT_DEV_INTERFACE], EVENT_DEV_INTERFACE, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF,  \
		}, \
		{"PORT", _collects_[EVENT_DEV_PORT], EVENT_DEV_PORT, alarm_proc_dev_port, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		ETHPORT_LINK|ETHPORT_RX_ERR/*|ETHPORT_FLOW_OVERRUN*|/ETHPORT_IF_LINK/*|ETHPORT_PK_LOSS*/}, \
		\
		{"MPLS", _collects_[EVENT_MPLS], EVENT_MPLS, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		}, \
		\
		{"E1", _collects_[EVENT_E1], EVENT_E1, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		}, \
		\
		{"SDH", _collects_[EVENT_SDH], EVENT_SDH, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		}, \
		\
		{"ETH_OAM", _collects_[EVENT_ETH_OAM], EVENT_ETH_OAM, NULL, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, SNMP_ALARM_LEVEL_DEF, \
		}, \
	}; \
	snmp_alarm_event_info_count = SNMP_ALARM_TYPE_REG_NUM

#define SNMP_ALARM_EVENTS_REGISTER(clie, ev)	\
	do{	\
		int _x_;	\
		for(_x_=0; _x_< sizeof(ev)/sizeof(snmp_alarm_event_info_t); _x_++){	\
			SNMP_ALARM_EVENT_SET(	\
					ev[_x_].collect, \
					ev[_x_].important_flag, \
					ev[_x_].minor_flag, \
					ev[_x_].prompt_flag, \
					ev[_x_].type, \
					ev[_x_].events); \
			SNMP_ALARM_EVENT_REG(clie, &ev[_x_].collect); \
		} \
	}while(0)
#endif

#define SNMP_TRAP_COUNTER_MAX	65535	/*max num of send trap*/

static time_t update_time;
struct mBase_client alarm_client;				/*define by API*/
snmp_alarm_info_t snmp_priv_alarm_info;		/*body of alarm*/
snmp_alarm_event_info_t *snmp_alarm_events;	/*backup event collects, for debug*/
int snmp_alarm_event_info_count;			/*count of backup event collects*/

#if 1
int snmp_trap_cfg_get(snmp_alarm_cfg_t *alarm_cfg)
{
	if(!alarm_cfg)
	{
		BC_PT_ERROR(BC_MODULE_SNMP,"alarm cfg is NULL!\n");
		return FALSE;
	}
	memcpy(alarm_cfg, &snmp_priv_alarm_info.cfg_info, sizeof(snmp_alarm_cfg_t));
	return TRUE;
}

int snmp_trap_cfg_set(snmp_alarm_cfg_t alarm_cfg)
{
	memcpy(&snmp_priv_alarm_info.cfg_info, &alarm_cfg, sizeof(snmp_alarm_cfg_t));
	return TRUE;
}

static int snmp_trap_cfg_get_counter(int *cnt)
{
	*cnt = snmp_priv_alarm_info.cfg_info.counter;
	return snmp_priv_alarm_info.cfg_info.counter;
}

static void snmp_trap_cfg_set_counter(int cnt)
{
	snmp_priv_alarm_info.cfg_info.counter = cnt;
}
static void snmp_trap_cfg_counter_add()
{
	snmp_priv_alarm_info.cfg_info.counter++;
	
	if(snmp_priv_alarm_info.cfg_info.counter >= SNMP_TRAP_COUNTER_MAX)
		snmp_priv_alarm_info.cfg_info.counter = 0;
}
#endif

#if 1
bc_boolean do_trap_alarm_event_proc(void *info)
{
	int cnt;
	bc_boolean is_trap = FALSE;
	
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);

	snmp_alarm_info_t alarm_info = *(snmp_alarm_info_t *)info;
	
	BC_PT_EVENT(BC_MODULE_SNMP,"code = %d: 0x%x\n", alarm_info.cfg_info.code, alarm_info.cfg_info.code);
	switch(alarm_info.cfg_info.code)
	{
		case SNMP_ALARM_CODE_LINK_DOWN:
			BC_PT_FUN_NOTSUPPORTED(BC_MODULE_SNMP);
			break;
		case SNMP_ALARM_CODE_BOARD_NO_EXIST:
			send_fhBoardNotExist_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_BOARD_FAULT:
			send_fhBoardFault_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_INTF_TRAFFIC_EXCEED:
			send_fhIntfTrafficExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_INTF_RX_BAD_PKT_EXCEED:
			send_fhIntfRxBadPktExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_PHYSICAL_INTERFACE_DOWN:
			send_fhPhysicalInterfaceDown_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_INTF_PKT_LOSS_EXCEED:
			send_fhIntfPktLossExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_BOARD_TEMP_EXCEED:
			send_fhBoardTempExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_MEMORY_USAGE_EXCEED:
			send_fhMemoryUsageExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_CPU_USAGE_EXCEED:
			send_fhCPUUsageExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_POWER_SUPPLY_SHORTAGE:
			send_fhPowerSupplyShortage_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_LASER_SEND_FAULT:
			send_fhLaserSendFault_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_LASER_TEMP_EXCEED:
			send_fhLaserTempExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_POWER_OUT_SHORTAGE:
			send_fhPowerOutShortage_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_POWER_OUT_EXCEED:
			send_fhPowerOutExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_POWER_IN_SHORTAGE:
			send_fhPowerInShortage_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_POWER_IN_EXCEED:
			send_fhPowerInExceed_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_VC_DOWN:
			send_fhVCDown_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_VC_UP:
			send_fhVCUp_trap(alarm_info);
			is_trap = TRUE;
			break;
		case SNMP_ALARM_CODE_PWE3_VC_W2P:
		//	send_fhPwe3VCW2P_trap();
			BC_PT_FUN_NOTSUPPORTED(BC_MODULE_SNMP);
			break;
		case SNMP_ALARM_CODE_PWE3_VC_P2W:
		//	send_fhPwe3VCP2W_trap();
			BC_PT_FUN_NOTSUPPORTED(BC_MODULE_SNMP);
			break;
		default:
			BC_PT_ERROR(BC_MODULE_SNMP,"default!!\n");
			return FALSE;
	}
	if(is_trap == TRUE)
	{
		snmp_trap_cfg_counter_add();
		BC_PT_EVENT(BC_MODULE_SNMP,"snmp alarm trap sucessed, code:%d counter:%d\n", alarm_info.cfg_info.code, snmp_trap_cfg_get_counter(&cnt));
	}
	else
	{
		SNMP_PT_WARNING(BC_MODULE_SNMP, ("snmp alarm code invalid, code:%d\n", alarm_info.cfg_info.code);
	}
	
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);

	return TRUE;
}

int alarm_event_proc_get(event_type_t t, AlarmEachEventTypeProc *hr)
{
	int i = 0;

	for(i=0; i<snmp_alarm_event_info_count; i++)
	{
		if(snmp_alarm_events[i].type == t)
		{
			*hr = snmp_alarm_events[i].hr;
			if(!hr){
				BC_PT_DBG_FUN(BC_MODULE_SNMP, "find hr of event_type[%d], but it is NULL!!!\n");
				return FALSE;
			}
			return TRUE;
		}
	}
	BC_PT_FUN_TAG(BC_MODULE_SNMP);
	hr = NULL;
	return FALSE;
}

static int trap_alarm_event_proc(IN void *request, IN struct ResultMessage *result)
{
	eventNotifyRes *resData;   
	struct event_bitmap event;
	
	AlarmEachEventTypeProc hr = NULL;
	
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);
/*	
	if(!request || !result){
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "NULL!!!!\n");
		return FALSE;
	}
*/		
	resData = (eventNotifyRes *)result->data;
	
	eventCalcEventMask((struct event_entry *)resData, &event);  
	
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "eventType = %x\n", resData->eventCollect.eventType);

	alarm_event_proc_get(resData->eventCollect.eventType, &hr);
	if(!hr)
	{
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "cannot find event proc func, event id:%d\n", resData->eventCollect.eventType);
		return FALSE;
	}
	
	switch(resData->eventCollect.eventType)
	{
		case EVENT_DEV_PORT:
			BC_PT_FUN_TAG(BC_MODULE_SNMP);
			hr(resData, event, &alarm_client, snmp_priv_alarm_info);
			break;   
		default:   
			BC_PT_DBG_FUN(BC_MODULE_SNMP, "no register event!\n");  
			break;
	}
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
}

static bc_boolean trap_alarm_signal_proc(struct mBase_signal_data *data)
{
	struct mBase_param param;
	int ret = 0;

	SNMP_PT_FUN("\n");
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);

	if(snmp_priv_alarm_info.cfg_info.enable == SNMP_TRAP_DISABLE)
	{
		SNMP_PT_WARNING(BC_MODULE_SNMP, ("eventID:0x%x, data:0x%x, but trap is disable!\n", data->eventId, data->data);
		return TRUE;
	}
	
	BC_PT_DBG_FUN(BC_MODULE_SNMP, "eventID:0x%x, data:0x%x\n", data->eventId, data->data);

	memset(&param, 0, sizeof(struct mBase_param));

	param.result_cb = trap_alarm_event_proc; 	/*event proc func*/
	param.timeout_process = NULL;   
	param.timeout = 1000;
	param.privateParam = &data->eventId;   
	param.pparamLen = sizeof(int);   

	if((ret = eventParse(&alarm_client, FLAG_VERSION, &param)) != LR_OK)
	{  
		return FALSE;
		BC_PT_DBG_FUN(BC_MODULE_SNMP, "eventParse error, ret=%s\n", getJsonMsg(ret));  
	}  

	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
	return TRUE;
	
}

static int signal_handler(struct mBase_signal_data *data)
{
	BC_PT_EVENT(BC_MODULE_SNMP,"signal_handler()\n");
	return TRUE;
}

static bc_boolean __trap_alarm_client_init(OUT struct mBase_client *clie,  alarm_signal_process handler)
{
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);
	
	clie->name = SNMP_TRAP_ALARM_NAME;
	clie->path = NULL;
	clie->signalEnable = TRUE;
#if 1
	clie->signal_process = handler;
#else
	clie->signal_process = signal_handler;
#endif
	
	if(mBase_init_client(clie) != LR_OK)
	{
		BC_PT_FUN_TAG(BC_MODULE_SNMP);
		return FALSE;
	}
	if(mBase_init_signal(clie) != LR_OK)
	{
		BC_PT_FUN_TAG(BC_MODULE_SNMP);
		return FALSE;
	}
	if(eventClientInit(NULL, FALSE, NULL) != LR_OK)
	{
		BC_PT_FUN_TAG(BC_MODULE_SNMP);
		return FALSE;
	}
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
	return TRUE;
}

void snmp_alarm_event_item_pt(void)
{
	int i = 0;

	SNMP_PT_FUN("\r\n event collect items:[%d]", snmp_alarm_event_info_count);
	SNMP_PT_FUN("\r\n -------------------------------------------------------------------------------------------------------------");
	SNMP_PT_FUN("\r\n %-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s\t%-8s", "Name", "Type", "subType", "I-Level", "M-Level", "P-Level", "ProcFunction");
	SNMP_PT_FUN("\r\n -------------------------------------------------------------------------------------------------------------");
	for(i=0; i<snmp_alarm_event_info_count; i++)
	{
		SNMP_PT_FUN("\r\n %-8s\t", snmp_alarm_events[i].name);
		SNMP_PT_FUN("%-8d\t", snmp_alarm_events[i].type);
		SNMP_PT_FUN("0x%-8x\t", snmp_alarm_events[i].events);
		SNMP_PT_FUN("0x%-8x\t", snmp_alarm_events[i].important_flag);
		SNMP_PT_FUN("0x%-8x\t", snmp_alarm_events[i].minor_flag);
		SNMP_PT_FUN("0x%-8x\t", snmp_alarm_events[i].prompt_flag);
		SNMP_PT_FUN("%-8s", !snmp_alarm_events[i].hr?"NULL":"alarm_proc_xxx");
	}
	SNMP_PT_FUN("\r\n -------------------------------------------------------------------------------------------------------------\n");
}

bc_boolean snmp_trap_alarm_event_init()
{
	struct mBase_param *param;
	bc_int32 num = 0;

	BC_PT_FUN_ENTER(BC_MODULE_SNMP);

	/*init alarm info*/
	SNMP_MEM_GET(snmp_alarm_events, snmp_alarm_event_info_t, API_ALARM_TYPE_DEF_MAX);
	if(!snmp_alarm_events){
		BC_PT_ERROR(BC_MODULE_SNMP,"SNMP_MEM_GET(snmp_alarm_events) error!\n");
		return FALSE;
	}
	SNMP_MEM_ZERO(&snmp_priv_alarm_info, sizeof(snmp_alarm_info_t));
	snmp_priv_alarm_info.cfg_info.enable = SNMP_TRAP_ENABLE;
	snmp_priv_alarm_info.cfg_info.parse = do_trap_alarm_event_proc;	/*alarm proc func*/

	/*define events*/
	SNMP_ALARM_DECLARE_EVENT(ALARM_EVENTS_INFO_TABLE);
	
	if(SNMP_ALARM_TYPE_REG_NUM >= API_ALARM_TYPE_DEF_MAX){
		BC_PT_ERROR(BC_MODULE_SNMP,"SNMP_ALARM_TYPE_REG_NUM >= API_ALARM_TYPE_DEF_MAX\n");
		SNMP_MEM_FREE(snmp_alarm_events);
		return FALSE;
	}
	memcpy(snmp_alarm_events, ALARM_EVENTS_INFO_TABLE, SNMP_ALARM_TYPE_REG_SIZE);
	snmp_alarm_event_item_pt();

	/*init mBase_client*/
	if(__trap_alarm_client_init(&alarm_client, trap_alarm_signal_proc) != TRUE)	/*signal proc func*/
	{
		BC_PT_ERROR(BC_MODULE_SNMP,"trap_alarm_client_init() failure\n");
		SNMP_MEM_FREE(snmp_alarm_events);
		return FALSE;
	}
	BC_PT_DBG_CPL(BC_MODULE_SNMP, "trap_alarm_client_init() successed\n");

	/*init and register events*/
	SNMP_ALARM_EVENTS_REGISTER(&alarm_client, ALARM_EVENTS_INFO_TABLE);

	BC_PT_DBG_CPL(BC_MODULE_SNMP, "snmp trap alarm events init successed\n");
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
	
	return TRUE;

API_ERR:
	BC_PT_DBG_CPL(BC_MODULE_SNMP, "snmp trap alarm events init failure\n");
	SNMP_MEM_FREE(snmp_alarm_events);
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
	
	return FALSE;
}
#endif

int snmp_trap_init_610a()
{
	BC_PT_FUN_ENTER(BC_MODULE_SNMP);
	if(snmp_trap_alarm_event_init() != TRUE)
	{
		BC_PT_ERROR(BC_MODULE_SNMP,"snmp_trap_alarm_event_init() failure\n");
		BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
		return FALSE;
	}
	BC_PT_FUN_LEAVE(BC_MODULE_SNMP);
	return TRUE;
}
