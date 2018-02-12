/************************************************************
  FileName:
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-10-10
  Description:                   
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/

#ifndef __BC_SYS_H__
#define __BC_SYS_H__

#include "bc_common_defs.h"
#include "bc_modules_com_para.h"
#include "bc_err.h"
#include "bc_msg.h"

typedef void (*bc_sys_reset_notify_cb)(void);
/*************************************************
  Function: bc_sys_startup_complete
  Description: system startup complete
  Input: 
  Output:
  		NULL
  Return:
  		FALSE: no complete
  		TRUE: complete
  Note: 
  History: 
*************************************************/
extern bc_boolean bc_sys_startup_complete(void);

extern bc_boolean bc_sys_bc_startup_complete(void);

extern bc_boolean bc_sys_module_init_complete(void);

extern bc_boolean bc_sys_cfg_init_complete(void);

extern bc_boolean bc_sys_snmp_startup_complete(void);

/*************************************************
  Function: bc_sys_reboot
  Description: system reboot
  Input: 
  Output:
  		NULL
  Return:
  		
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_sys_reboot(void);
bc_err_e bc_sys_init(void);
bc_err_e bc_sys_reset_reg(bc_modules_e module_id, bc_sys_reset_notify_cb cb);
bc_err_e bc_sys_reset_unreg(bc_modules_e module_id);


extern bc_err_e bc_sys_reset(void);


#endif

