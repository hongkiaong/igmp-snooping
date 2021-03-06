/*******************************************************************************
  -------------------------------------------------------------------------
  [Author]     : bain.wang@outlook.com
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

#ifndef __BC_USERMGMT_CORE_H__
#define __BC_USERMGMT_CORE_H__

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								头   文   件								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#include "bc_err.h"
#include "bc_usermgmt_def.h"
#include "bc_modules_com_para.h"
#include "bc_sys_event.h"

////////////////////////////////////////////////////////////////////////////////
//																			  //
//								宏   定   义								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//																			  //
//					结 构 体 、联 合 体、枚 举 定 义						  //
//																			  //
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//																			  //
//						外 部 变 量、 函 数 声 明							  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



bc_err_e bc_usermgmt_core_init(void);

bc_err_e bc_usermgmt_core_file_write(void);

bc_err_e bc_usermgmt_core_reg_user
(
	IN bc_int32 sid, 
	IN bc_usermgmt_info_t *usermgmt_info
);

bc_err_e bc_usermgmt_core_unreg_user
(
	IN bc_int32 sid,
	IN bc_char name[BC_USERMGMT_NAME_LEN]
);

bc_err_e bc_usermgmt_core_set_user
(
	IN bc_int32 sid, 
	IN bc_char name[BC_USERMGMT_NAME_LEN], 
	IN bc_char pwd[BC_USERMGMT_NAME_LEN],
	IN bc_usermgmt_level_e lvl
);

bc_err_e bc_usermgmt_core_login_user
(
	IN bc_int32 sid,
	IN bc_char name[BC_USERMGMT_NAME_LEN],
	IN bc_char pwd[BC_USERMGMT_NAME_LEN]
);


bc_boolean bc_usermgmt_core_logout_user
(
	IN bc_int32 sid
);


bc_boolean bc_usermgmt_core_get_info
(
	IN bc_int32 sid, 
	IN bc_char name[BC_USERMGMT_NAME_LEN], 
	OUT bc_usermgmt_info_t *usermgmt_info
);


bc_boolean bc_usermgmt_core_get_list
(
	IN bc_int32 sid,
	OUT bc_usermgmt_info_t usermgmt_info[BC_USERMGMT_USER_NUM], 
	OUT bc_int32 *num
);

bc_err_e bc_usermgmt_core_get_list_dump
(
	OUT bc_usermgmt_info_t usermgmt_info[BC_USERMGMT_USER_NUM], 
	OUT bc_int32 *num
);

void bc_usermgmt_core_abend_logout(bc_sys_event_msg_t *msg);


bc_err_e bc_usermgmt_core_get_user_count
(
	OUT bc_int32 *num
);

bc_boolean bc_usermgmt_core_get_line_info
(
	IN bc_int32 sid,
	OUT bc_usermgmt_info_t *usermgmt_info
);

bc_err_e bc_usermgmt_core_get_userinfo_by_name(IN bc_char name[BC_USERMGMT_NAME_LEN], OUT bc_usermgmt_info_t *usermgmt_info);

bc_err_e bc_usermgmt_init();

#endif /* __BC_USERMGMT_CORE_H__ */





