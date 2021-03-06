/************************************************************
  FileName:
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-4-22
  Description:                   
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/

#ifndef __BC_SYS_DEFS_H__
#define __BC_SYS_DEFS_H__
#include <sys/timeb.h>
#include "bc_common_defs.h"
#include "bc_modules_com_para.h"
#include "bc_err.h"
#include "bc_msg.h"

#define BC_SYS_BASEINFO_HW_LEN		128
#define BC_SYS_BASEINFO_SW_LEN		128
#define BC_SYS_CONTACT_LEN			256
#define BC_SYS_LOCATION_LEN			256
#define BC_SYS_OAM_OUI_LEN			3
#define BC_SYS_IP_INTERFACE_MAC_MAX	128
typedef bc_uint8	bc_oam_oui_t[BC_SYS_OAM_OUI_LEN];
//设备类型
typedef enum
{
	BC_SYS_DEV_TYPE_PT_D12G = 1,
	BC_SYS_DEV_TYPE_MAXTYPE
}bc_sys_dev_type_e;

typedef enum
{
	BC_SYS_DEV_STYLE_MINTYPE = 0,
	BC_SYS_DEV_STYLE_PT_D12G = 1,
	BC_SYS_DEV_STYLE_MAXTYPE,
}bc_sys_dev_style_e;
typedef struct {
	unsigned long  MemTotal;
	unsigned long  MemFree;
	unsigned long  cpu;
  	unsigned long  voltage;  /* (a*100)/(1000*1000) (b*100)%(1000*1000) a.b */
} bc_sys_basestatus_t;


typedef struct {
	bc_char	sys_name[BC_MODULE_NAME_LENGTH];
	bc_char	hw[BC_SYS_BASEINFO_HW_LEN];
	bc_char	sw[BC_SYS_BASEINFO_SW_LEN];
	bc_lan_e def_lan;
	bc_ulong run_time;/* seconds since boot */
	bc_sys_dev_type_e		bc_type;
	bc_char	mf[BC_MODULE_NAME_LENGTH];/*生产厂家*/
	bc_char	sys_contact[BC_SYS_CONTACT_LEN];
	bc_char	sys_location[BC_SYS_LOCATION_LEN];
	bc_sys_dev_style_e olt_style;
	bc_uint16 num_service_slot;
	bc_uint16 num_power_slot;
	bc_uint16 num_fan_slot;
	bc_oam_oui_t	oam_oui;
	bc_sys_basestatus_t status;
	struct tm up_time;
} bc_sys_baseinfo_t;


#define BC_SYS_START_COMPLETE 					"bc_ready" /* from S99oltstartcomplete */

#define BC_SYS_MODULE_INIT_COMPLETE   			"module_init_complete"

#define BC_SYS_INIT_COMPLETE 					"bc_init_complete"
#define BC_SYS_INIT_COMPLETE_MASTER 			"bc_init_complete_master"
#define BC_SYS_INIT_COMPLETE_SLAVE 			"bc_init_complete_slave"

#define BC_SYS_CLI_CFG_COMPLETE				"cli_cfg_complete"

#endif

