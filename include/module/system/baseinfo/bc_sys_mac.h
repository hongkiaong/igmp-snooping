/************************************************************
  FileName:
  Author:  bain.wang@outlook.com      Version :  1.0        Date:2016-4-24
  Description:                   
  Version:                        
  History:        
                  
    1. Date:
       Author:
       Modification:
    2. ...
***********************************************************/

#ifndef __bc_sys_mac_h__
#define __bc_sys_mac_h__

#include "bc_common_defs.h"
#include "bc_modules.h"
#include "bc_err.h"
//#include "xx/l1/port/bc_port_def.h"

typedef enum {
		BC_SYS_MAC_TYPE_INBAND_E 			= 0x00,
		BC_SYS_MAC_TYPE_OUTBAND_E 		= 0x01,
		BC_SYS_MAC_TYPE_INTERNAL_E		= 0x02,
		BC_SYS_MAC_TYPE_SWITCH_CPU_E	= 0x03
} bc_sys_mac_type_e;



bc_err_e bc_sys_mac_get(bc_mac mac);

bc_err_e bc_sys_mac_set(bc_mac mac);
/*************************************************
  Function: bc_sys_mac_segm_set
  Description: set system mac segment
  Input: 
  		mac: mac segment
  Output:
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  		mac last byte will be used(00:0f:1e:xx:xx:00/ff:ff:ff:ff:ff:00)
  History: 
*************************************************/
extern bc_err_e bc_sys_mac_segm_set(bc_mac mac);

/*************************************************
  Function: bc_sys_mac_get_by_sys_type
  Description: get system interface mac
  Input: 
  		type: system interface
  Output:
  		mac: system's mac
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_sys_mac_get_by_sys_type(bc_sys_mac_type_e type, bc_mac mac);


/*************************************************
  Function: bc_sys_mac_get_by_port
  Description: get port's mac
  Input: 
  		port: port
  Output:
  		mac: port's mac
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/

extern bc_err_e bc_sys_mac_get_by_ip_interface(bc_mac mac, bc_uint32 *num);

extern bc_boolean bc_sys_mac_check(bc_mac mac);
/*************************************************
  Function: bc_sys_mac_init
  Description: sys mac init
  Input: 
  Output:
  Return:
  		0: BC_ERR_OK
  		~0: error id
  Note: 
  History: 
*************************************************/
extern bc_err_e bc_sys_mac_init(void);


#endif

